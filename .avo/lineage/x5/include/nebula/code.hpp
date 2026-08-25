// NEBULA kernel — code representations, noise engine, decoders.
// HARD CONSTRAINTS (kernel paths): uint64 words; bitwise ops, add/sub,
// compares, shifts, rotates, popcount. NO mult/div/mod, NO floats.
//
// Layout: a binary code of length n lives in n/64 uint64 words, little-endian
// lanes. NSSC codes are one-hot expansions of RS codewords over GF(2^m):
//   bit position of symbol j (value s) is  j*q + s   (q = 2^m).
#pragma once
#include "gf.hpp"
#include "rs.hpp"
#include <vector>
#include <cstring>
#include <cassert>

namespace nebula {

// ------------------------------------------------------------------ RNG ---
// xorshift128+ (Marsaglia/Vigna family): shifts, XOR, ADD only. No multiply.
struct Rng {
    u64 s[4];
    explicit Rng(u64 seed) {
        // splitmix-free seeding: simple additive cascade of the seed
        u64 z = seed;
        for (int i = 0; i < 4; ++i) {
            z += 0x9E3779B97F4A7C15ull;          // golden ratio constant
            u64 r = z;
            r ^= r >> 30; r += 0x6A09E667F3BCC909ull;
            r ^= r >> 27; r += 0xBB67AE8584CAA73Bull;
            r ^= r >> 31;
            s[i] = r;
        }
        for (int i = 0; i < 8; ++i) next();       // warm up
    }
    inline u64 next() {
        u64 ret = rotl_(s[0] + s[3], 23) + s[0];
        u64 t = s[1] << 17;
        s[2] ^= s[0];
        s[3] ^= s[1];
        s[1] ^= s[2];
        s[0] ^= s[3];
        s[2] ^= t;
        s[3] = rotl_(s[3], 45);
        return ret;
    }
    static inline u64 rotl_(u64 x, unsigned k) { return (x << k) | (x >> (64 - k)); }
};

// Uniform position in [0, n) without division: mask to next power of two,
// reject over-range draws. Robust even though xorshift low bits are weak.
inline u64 rand_below(Rng& rng, unsigned n) {
    u64 p = n - 1;
    p |= p >> 1; p |= p >> 2; p |= p >> 4; p |= p >> 8; p |= p >> 16; p |= p >> 32;
    for (;;) {
        u64 v = rng.next() & p;
        if (v < (u64)n) return v;
    }
}

// -------------------------------------------------------------- words -----
inline unsigned overlap_words(const u64* a, const u64* b, size_t words) {
    unsigned acc = 0;
    for (size_t i = 0; i < words; ++i)
        acc += (unsigned)__builtin_popcountll(a[i] & b[i]);
    return acc;
}
inline void xor_words(u64* dst, const u64* src, size_t words) {
    for (size_t i = 0; i < words; ++i) dst[i] ^= src[i];
}
inline void copy_words(u64* dst, const u64* src, size_t words) {
    for (size_t i = 0; i < words; ++i) dst[i] = src[i];
}
inline void zero_words(u64* dst, size_t words) {
    for (size_t i = 0; i < words; ++i) dst[i] = 0;
}
inline unsigned weight_words(const u64* a, size_t words) {
    unsigned acc = 0;
    for (size_t i = 0; i < words; ++i)
        acc += (unsigned)__builtin_popcountll(a[i]);
    return acc;
}

// ------------------------------------------------------- flat SDR (base) --
struct SdrSpace {
    unsigned n, w;
    size_t words;
    explicit SdrSpace(unsigned n_, unsigned w_) : n(n_), w(w_), words(n_ / 64) {
        assert(n % 64 == 0 && w >= 4);
    }
    // Random w-of-n code by rejection sampling (positions are powers of two).
    void encode_random(Rng& rng, u64* out) const {
        zero_words(out, words);
        unsigned placed = 0;
        while (placed < w) {
            u64 pos = rand_below(rng, n);
            u64* wp = out + (pos >> 6);
            u64 bit = 1ull << (pos & 63);
            if (*wp & bit) continue;
            *wp |= bit;
            ++placed;
        }
    }
};

// -------------------------------------------------- NSSC binary space -----
template <unsigned MBITS, unsigned POLY>
struct NsscSpace {
    using F = GF<MBITS, POLY>;
    using RsT = RS<MBITS, POLY>;

    unsigned q;        // field size = 2^m
    unsigned l2q;      // log2(q): j*q becomes j << l2q (no integer multiply)
    unsigned N;        // symbols per code  (= binary weight w)
    unsigned K;        // message symbols   (dictionary size q^K)
    unsigned n;        // binary length = N*q
    unsigned NC;       // category repetition blocks appended after N blocks
    size_t words;
    RsT rs;

    NsscSpace(unsigned n_, unsigned q_, unsigned nc = 0)
        : q(q_), l2q(0), N(0), K(0), NC(nc), n(n_), words(n_ >> 6),
          rs(2, 1) {
        assert(n_ % 64 == 0 && (q_ & (q_ - 1)) == 0 && (n_ & (q_ - 1)) == 0);
        unsigned v = q_;
        while (v > 1) { v >>= 1; ++l2q; }
        N = (n_ >> l2q) - nc;                // identity core blocks
        this->rs = RsT(N, 1);
    }

    void init_k(unsigned k_msg) {
        assert(k_msg <= N && k_msg >= 1);
        const_cast<unsigned&>(K) = k_msg;
        this->rs = RsT(N, k_msg);
    }

    unsigned lambda_max() const { return K - 1; }      // certified agreement cap
    unsigned e_star() const { return N - K; }          // certified flip radius

    // Deterministic message -> code. msg: K symbols in [0,q).
    void encode(const u32* msg, u64* out) const {
        zero_words(out, words);
        u32 cw[N_MAX_C()];
        rs.encode(msg, cw);
        for (unsigned j = 0; j < N; ++j) {
            u64 pos = ((u64)j << l2q) + cw[j];         // pure shift+add
            out[pos >> 6] |= 1ull << (pos & 63);
        }
        // nested category field: NC repetition blocks of msg[0]
        for (unsigned j = 0; j < NC; ++j) {
            u64 pos = ((u64)(N + j) << l2q) + msg[0];
            out[pos >> 6] |= 1ull << (pos & 63);
        }
    }

    // Category readout: majority vote over the NC repetition blocks.
    // Returns winning symbol; ties/empty -> q (invalid sentinel).
    u32 category_vote(const u64* query) const {
        u32 votes[512];
        for (unsigned s2 = 0; s2 < q && s2 < 512; ++s2) votes[s2] = 0;
        for (unsigned j = 0; j < NC; ++j) {
            u32 sv;
            unsigned pc = block_info(query, N + j, &sv);
            if (pc == 1 && sv < 512) ++votes[sv];
        }
        u32 best = q; unsigned bestv = 0;
        bool tie = false;
        for (unsigned s = 0; s < q; ++s) {
            if (votes[s] > bestv) { bestv = votes[s]; best = s; tie = false; }
            else if (votes[s] && votes[s] == bestv) tie = true;
        }
        return tie ? q : best;
    }
    static constexpr unsigned N_MAX_C() { return 1100; }

    // Random message with fixed weight of nonzero? Not needed: any msg ok.
    void encode_random(Rng& rng, u32* msg, u64* out) const {
        for (unsigned i = 0; i < K; ++i)
            msg[i] = (u32)(rng.next() & (q - 1));
        encode(msg, out);
    }

    // Binary query -> per-symbol votes -> erasure list (popcount != 1 blocks).
    // Returns number of erasures; fills sym[] with recovered symbols where
    // unambiguous (popcount==1), else garbage.
    unsigned to_symbols(const u64* query, u32* sym, unsigned* erase) const {
        unsigned E = 0;
        for (unsigned j = 0; j < N; ++j) {
            u32 sv;
            unsigned pc = block_info(query, j, &sv);
            if (pc == 1) sym[j] = sv;
            else { sym[j] = 0; erase[E++] = j; }
        }
        return E;
    }
    // General block scanner: works for ANY q (including q > 64).
    // Returns block popcount; sets *sym to the set-bit index when popcount==1,
    // else 0. Early-exits once ambiguity is proven. Pure bitwise.
    inline unsigned block_info(const u64* v, unsigned j, u32* sym_out) const {
        u64 start = (u64)j << l2q;
        u64 end = start + q;
        unsigned pc = 0;
        u32 sym = 0;
        u64 pos = start;
        while (pos < end) {
            unsigned wo = (unsigned)(pos >> 6), bo = (unsigned)(pos & 63);
            u64 word = v[wo] >> bo;
            unsigned avail = 64 - bo;
            u64 remain = end - pos;
            if (remain < (u64)avail) avail = (unsigned)remain;
            u64 mask = (avail == 64) ? ~0ull : ((1ull << avail) - 1);
            word &= mask;
            unsigned c = (unsigned)__builtin_popcountll(word);
            if (c) {
                if (pc == 0 && c == 1)
                    sym = (u32)(pos - start) + (u32)__builtin_ctzll(word);
                else
                    sym = 0;
                pc += c;
                if (pc > 1) break;
            }
            pos += avail;
        }
        *sym_out = sym;
        return pc;
    }

    // Algebraic decode path: erasure-correct damaged blocks, re-expand.
    // Returns 1 decoded, 0 failed (caller reports UNKNOWN).
    bool decode_algebraic(const u64* query, u64* out, u32* msg_scratch) const {
        u32 sym[N_MAX_C()];
        unsigned erase[N_MAX_C()];
        unsigned E = to_symbols(query, sym, erase);
        u32 y[N_MAX_C()];
        for (unsigned j = 0; j < N; ++j) y[j] = sym[j];
        u32 msg[N_MAX_C()];
        if (!rs.decode_erasure(y, erase, E, msg)) return false;
        encode(msg, out);
        (void)msg_scratch;
        return true;
    }
};

// --------------------------------------------------------- noise engine ---
// Exact fixed-count distinct-position flips (toggles). Positions drawn by
// rejection against a touched-bitmap cleared afterwards.
struct NoiseEngine {
    std::vector<u64> touch;         // bitmap over positions
    std::vector<u64> touched_list;  // positions touched (for O(touched) clear)
    void init(unsigned n) {
        touch.assign((n + 63) / 64, 0);
        touched_list.clear();
        touched_list.reserve(4096);
    }
    inline void clear() {
        for (u64 p : touched_list) touch[p >> 6] &= ~(1ull << (p & 63));
        touched_list.clear();
    }
    // Uniform: exactly `count` distinct random positions toggled.
    void apply_uniform(u64* query, unsigned n, unsigned count, Rng& rng) {
        unsigned placed = 0;
        while (placed < count) {
            u64 pos = rand_below(rng, n);
            u64 w = pos >> 6, bit = 1ull << (pos & 63);
            if (touch[w] & bit) continue;
            touch[w] |= bit;
            touched_list.push_back(pos);
            query[pos >> 6] ^= bit;
            ++placed;
        }
        clear();
    }
    // Adversarial mixed attack (matches the Lean theorem's noise model):
    // toggle t_off ON bits of the query OFF, then toggle f_on OFF bits of the
    // query ON inside the rival's support. Confusion iff t_off+f_on > margin
    // (= weight(self) - agreement), exactly as certified_unique_decode predicts.
    unsigned apply_adversarial(u64* query, const u64* selfcode, const u64* rival,
                               unsigned n, unsigned t_off, unsigned f_on, Rng& rng) {
        unsigned done = 0, guard = 0;
        while (done < t_off && guard < t_off * 64u + 4096u) {
            ++guard;
            u64 pos = rand_below(rng, n);
            u64 bit = 1ull << (pos & 63);
            if (!(selfcode[pos >> 6] & bit)) continue;   // must be an ON bit of self
            if (!(query[pos >> 6] & bit)) continue;      // still on?
            query[pos >> 6] ^= bit;
            ++done;
        }
        unsigned done2 = 0; guard = 0;
        while (done2 < f_on && guard < f_on * 8192u + 262144u) {
            ++guard;
            u64 pos = rand_below(rng, n);
            u64 bit = 1ull << (pos & 63);
            if (!(rival[pos >> 6] & bit)) continue;
            if (query[pos >> 6] & bit) continue;
            query[pos >> 6] ^= bit;
            ++done2;
        }
        return done + done2;
    }
    // Adversarial: flip `count` OFF bits of the query INTO rival's support.
    // Precondition: count <= weight(rival &~ query). Returns flips applied.
    unsigned apply_adversarial_into(u64* query, const u64* rival,
                                    unsigned n, unsigned count, Rng& rng) {
        // Rejection-sample positions inside the rival's support that are OFF
        // in the query, toggling them ON. Exact distinct flips.
        unsigned placed = 0, guard = 0;
        while (placed < count && guard < count * 8192u + 262144u) {
            ++guard;
            u64 pos = rng.next() & (u64)(n - 1);
            u64 bit = 1ull << (pos & 63);
            if (!(rival[pos >> 6] & bit)) continue;
            if (query[pos >> 6] & bit) continue;
            query[pos >> 6] ^= bit;
            ++placed;
        }
        return placed;
    }
};

// ----------------------------------------------------------- dictionary ---
struct Dictionary {
    std::vector<u64> bits;     // M * words
    unsigned M, words;
    void alloc(unsigned m, size_t w) {
        M = m; words = (unsigned)w;
        bits.assign((size_t)m * w, 0);
    }
    inline const u64* code(unsigned i) const {
        return bits.data() + (size_t)i * words;
    }
    inline u64* code(unsigned i) { return bits.data() + (size_t)i * words; }
};

// Scan decode: unique argmax with unknown-threshold. Ties report UNKNOWN (-1).
inline int decode_scan(const Dictionary& dict, const u64* query,
                       unsigned theta, unsigned* out_overlap = nullptr) {
    int best = -1;
    unsigned bestv = 0;
    bool tie = false;
    for (unsigned i = 0; i < dict.M; ++i) {
        unsigned ov = overlap_words(query, dict.code(i), dict.words);
        if (ov < theta) continue;
        if (ov > bestv) {
            bestv = ov;
            best = (int)i;
            tie = false;
        } else if (ov == bestv) {
            tie = true;
        }
    }
    if (out_overlap) *out_overlap = bestv;
    return tie ? -1 : best;
}

} // namespace nebula
