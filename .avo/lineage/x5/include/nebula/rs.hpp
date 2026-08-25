// NEBULA kernel — Reed–Solomon over GF(2^m): systematic encode + erasure decode.
// HARD CONSTRAINTS: hot paths use only shifts, XOR, AND, add/sub, compares,
// and table lookups. Multiplication/division are replaced by exp/log LUTs.
// NO floats anywhere.
#pragma once
#include "gf.hpp"
#include <vector>
#include <cassert>
#include <cstring>

namespace nebula {

template <unsigned M, unsigned POLY>
struct RS {
    using F = GF<M, POLY>;
    static constexpr unsigned N_MAX = F::q - 1;
    static constexpr unsigned D_MAX = 1100;

    unsigned N;                 // code symbols
    unsigned K;                 // message symbols
    unsigned D;                 // parity symbols = N - K
    std::vector<u32> gen;       // gen[0..D-1]: low-order coeffs of monic g(x)

    RS(unsigned n, unsigned k) : N(n), K(k), D(n - k) {
        assert(n >= 1 && n <= N_MAX && k >= 1 && n > k && D <= D_MAX);
        build_generator();
    }

    // g(x) = prod_{i=1..D} (x + alpha^i), monic of degree D.
    // Roots alpha^1..alpha^D match the syndrome evaluation points.
    // Coefficient updates: ng[j+1] ^= g[j]; ng[j] ^= g[j]*root  (LUT mul).
    void build_generator() {
        std::vector<u32> g(1, 1);
        for (unsigned i = 0; i < D; ++i) {
            u32 r = F::exp_t(i + 1);
            std::vector<u32> ng(g.size() + 1, 0);
            for (unsigned j = 0; j < g.size(); ++j) {
                ng[j + 1] ^= g[j];
                ng[j] ^= F::mul_lut(g[j], r);
            }
            g.swap(ng);
        }
        assert(g[D] == 1); // monic sanity
        gen.assign(g.begin(), g.begin() + D);
    }

    // Systematic encode: cw[0..K-1] = msg, cw[K..N-1] = parity.
    // Explicit synthetic division of msg(x)*x^D by g(x). LUT multiplies only.
    // Convention: symbol j carries degree N-1-j (high-first).
    void encode(const u32* msg, u32* cw) const {
        u32 w[N_MAX];
        for (unsigned i = 0; i < K; ++i) w[i] = msg[i];
        for (unsigned j = 0; j < D; ++j) w[K + j] = 0;
        for (unsigned i = 0; i < K; ++i) {
            u32 c = w[i];
            if (c) {
                w[i] = 0;                          // monic leading term cancels
                u32 ls = F::log_t(c);
                for (unsigned t = 0; t < D; ++t)
                    if (gen[t]) w[i + D - t] ^= F::exp_t(ls + F::log_t(gen[t]));
            }
        }
        for (unsigned i = 0; i < K; ++i) cw[i] = msg[i];
        for (unsigned j = 0; j < D; ++j) cw[K + j] = w[K + j];
    }

    // Syndromes S_t = C(alpha^{t+1}) with C(x) = sum_j y[j] x^{N-1-j}.
    // Descending powers via inverse-step multiplication (LUT only).
    void syndromes(const u32* y, u32* S) const {
        u32 stepN = F::exp_t(N - 1);                 // alpha^(N-1)
        u32 v0 = stepN;                              // alpha^{(t+1)(N-1)}, t=0
        for (unsigned t = 0; t < D; ++t) {
            u32 step = F::exp_t(t + 1);
            u32 istep = F::inv_lut(step);
            u32 v = v0;
            u32 acc = 0;
            for (unsigned j = 0; j < N; ++j) {
                acc ^= F::mul_lut(y[j], v);
                v = F::mul_lut(v, istep);            // exponent -(t+1) per j
            }
            S[t] = acc;
            v0 = F::mul_lut(v0, stepN);              // power up for next t
        }
    }

    // Erasure decode: corrects up to D erased positions (values unknown).
    // Solves H_E * e = S by Gaussian elimination over GF(q); then verifies
    // by re-encoding. Returns true and writes corrected message on success.
    bool decode_erasure(const u32* y, const unsigned* erase, unsigned E,
                        u32* msg_out) const {
        if (E > D) return false;
        u32 S[D_MAX];
        syndromes(y, S);
        bool zero = true;
        for (unsigned t = 0; t < D; ++t) zero &= (S[t] == 0);
        if (zero) {                                   // already a valid codeword
            for (unsigned i = 0; i < K; ++i) msg_out[i] = y[i];
            return true;
        }

        // Build augmented matrix rows: D rows x (E+1) cols.
        // Row t: [alpha^{(t+1)*e_0}, ..., alpha^{(t+1)*e_{E-1}} | S_t]
        // Column c corresponds to symbol position erase[c], whose syndrome
        // exponent weight is alpha^{(t+1)*(N-1-p)}. Built incrementally per
        // column: v_{t+1} = v_t * alpha^{N-1-p}.  (LUT multiplies only.)
        // Row stride padded to a power of two so that even compiled address
        // arithmetic stays shift-based (no imul by odd constants).
        static constexpr unsigned MAT_STRIDE = 2048;
        static thread_local u32 mat[D_MAX][MAT_STRIDE];
        for (unsigned c = 0; c < E; ++c) {
            u32 base = F::exp_t(N - 1 - erase[c]);
            u32 v = base;
            for (unsigned t = 0; t < D; ++t) {
                mat[t][c] = v;
                v = F::mul_lut(v, base);
            }
        }
        for (unsigned t = 0; t < D; ++t) mat[t][E] = S[t];

        // Forward elimination with partial pivot (any nonzero works in GF).
        unsigned rank = 0;
        for (unsigned c = 0; c < E && rank < D; ++c) {
            unsigned piv = rank;
            while (piv < D && mat[piv][c] == 0) ++piv;
            if (piv == D) continue;                   // free variable: inconsistent unless...
            if (piv != rank) {                        // swap rows
                for (unsigned cc = c; cc <= E; ++cc) {
                    u32 tmpv = mat[rank][cc];
                    mat[rank][cc] = mat[piv][cc];
                    mat[piv][cc] = tmpv;
                }
            }
            u32 inv = F::inv_lut(mat[rank][c]);
            for (unsigned cc = c; cc <= E; ++cc)
                mat[rank][cc] = F::mul_lut(mat[rank][cc], inv);
            for (unsigned rr = 0; rr < D; ++rr) {
                if (rr == rank || mat[rr][c] == 0) continue;
                u32 fac = mat[rr][c];
                for (unsigned cc = c; cc <= E; ++cc)
                    mat[rr][cc] ^= F::mul_lut(fac, mat[rank][cc]);
            }
            ++rank;
        }

        // Consistency: every row beyond rank must have zero RHS.
        for (unsigned rr = rank; rr < D; ++rr)
            if (mat[rr][E] != 0) return false;

        // Back-substitute (reduced rows make this direct).
        u32 err[N_MAX];
        for (unsigned c = 0; c < E; ++c) err[c] = 0;
        for (unsigned rr = 0; rr < rank; ++rr) {
            // find pivot col of row rr
            unsigned c = 0;
            while (c < E && mat[rr][c] == 0) ++c;
            if (c == E) continue;
            err[c] = mat[rr][E];
        }

        // Apply corrections, copy message, verify by re-encode.
        u32 fixed[N_MAX];
        for (unsigned j = 0; j < N; ++j) fixed[j] = y[j];
        for (unsigned c = 0; c < E; ++c) fixed[erase[c]] ^= err[c];
        u32 chk[N_MAX];
        encode(fixed, chk);
        for (unsigned j = K; j < N; ++j)
            if (chk[j] != fixed[j]) return false;     // verification failed
        for (unsigned i = 0; i < K; ++i) msg_out[i] = fixed[i];
        return true;
    }
};

using RS64  = RS<6, 0x43>;   // GF(64),  x^6+x+1
using RS256 = RS<8, 0x1D>;  // GF(256), x^8+x^4+x^3+x^2+1

} // namespace nebula
