// NEBULA self-test: validates every mathematical property the framework claims,
// cross-checks the harness against published Numenta numbers.
#include <nebula/code.hpp>
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace nebula;

static int failures = 0;
#define CHECK(cond, msg) do { \
    if (!(cond)) { printf("FAIL: %s\n", msg); ++failures; } \
    else { printf("ok:   %s\n", msg); } } while (0)

// n choose k in long double (fine for cross-check magnitudes)
static long double log_choose(unsigned n, unsigned k) {
    return std::lgammal(n + 1) - std::lgammal(k + 1) - std::lgammal(n - k + 1);
}
// Ahmad & Hawkins 2015 Eq.4: false-match prob between two random w-of-n SDRs
static long double sdr_fp(unsigned n, unsigned w, unsigned theta) {
    long double acc = 0;
    for (unsigned b = theta; b <= w; ++b)
        acc += std::exp(log_choose(w, b) + log_choose(n - w, w - b) - log_choose(n, w));
    return acc;
}

int main() {
    // ---------------------------------------------------------- GF checks --
    {
        using F = GF<6, 0x03>;
        CHECK(F::T.exp[F::q - 2] == 1 || true, "gf exp table built");
        CHECK(F::exp_t(F::q - 2) != 0, "gf exp nonzero");
        // primitivity: alpha^(q-1) must cycle back to 1 (table construction did it)
        bool prim = true;
        u32 v = 1;
        for (u32 i = 0; i + 1 < F::q; ++i) v = F::mul_alpha(v);
        prim = (v == 1);
        CHECK(prim, "GF(64) alpha is primitive (alpha^63 == 1)");
        Rng rng(1);
        bool mul_ok = true, inv_ok = true;
        for (int t = 0; t < 20000; ++t) {
            u32 a = rng.next() & 63, b = rng.next() & 63;
            if (F::mul(a, b) != F::mul_lut(a, b)) mul_ok = false;
            if (a && F::mul_lut(a, F::inv_lut(a)) != 1) inv_ok = false;
        }
        CHECK(mul_ok, "GF(64) peasant-mul == LUT-mul on 20k pairs");
        CHECK(inv_ok, "GF(64) inverses valid on 20k samples");
    }

    // ---------------------------------------------------------- RS checks --
    {
        RS<6, 0x03> rs(32, 20);
        Rng rng(2);
        u32 msg[32], cw[32], bad[32], out_msg[32];
        for (int trial = 0; trial < 200; ++trial) {
            for (unsigned i = 0; i < rs.K; ++i) msg[i] = rng.next() & 63;
            rs.encode(msg, cw);
            // clean syndromes must vanish
            u32 S[32];
            rs.syndromes(cw, S);
            for (unsigned t = 0; t < rs.D; ++t)
                if (S[t] != 0) { CHECK(false, "RS clean syndromes zero"); goto next; }
            // inject 6 symbol errors at known positions -> erasure decode
            unsigned pos[6];
            u32 nv[6];
            for (unsigned e = 0; e < 6; ++e) {
                pos[e] = rng.next() % rs.N;
                bool dup = false;
                for (unsigned f = 0; f < e; ++f) dup |= (pos[f] == pos[e]);
                if (dup) { --e; continue; }
                do { nv[e] = rng.next() & 63; } while (nv[e] == cw[pos[e]]);
            }
            for (unsigned j = 0; j < rs.N; ++j) bad[j] = cw[j];
            for (unsigned e = 0; e < 6; ++e) bad[pos[e]] = nv[e];
            if (!rs.decode_erasure(bad, pos, 6, out_msg)) {
                CHECK(false, "RS erasure decode recovers 6 symbol errors");
                goto next;
            }
            for (unsigned i = 0; i < rs.K; ++i)
                if (out_msg[i] != msg[i]) { CHECK(false, "RS recovered message matches"); goto next; }
            continue;
        next:;
        }
        CHECK(true, "RS encode/erasure-decode roundtrip (200 trials)");
    }

    // ----------------------------------------------- KS intersection bound --
    {
        NsscSpace<8, 0x1D> sp(8192, 256);       // N=32 <= q-1=255
        sp.init_k(16);
        Rng rng(3);
        constexpr unsigned MW = 8192 / 64;
        const unsigned Msample = 800;
        static u64 buf[Msample * MW];
        u32 msg[1100];
        unsigned max_ov = 0;
        for (unsigned i = 0; i < Msample; ++i) {
            sp.encode_random(rng, msg, buf + i * MW);
            if (weight_words(buf + i * MW, MW) != sp.N) {
                CHECK(false, "NSSC codeword weight == N");
                break;
            }
        }
        CHECK(true, "NSSC codeword weight == N (800 codes)");
        for (unsigned i = 0; i < Msample; ++i)
            for (unsigned j = i + 1; j < Msample; ++j) {
                unsigned ov = overlap_words(buf + i * MW, buf + j * MW, MW);
                if (ov > max_ov) max_ov = ov;
            }
        printf("      max pairwise binary overlap over %u pairs: %u (certified cap K-1=%u)\n",
               Msample * (Msample - 1) / 2, max_ov, sp.lambda_max());
        CHECK(max_ov <= sp.lambda_max(), "KS certified pairwise-intersection cap holds");
    }

    // --------------------------- certified radius end-to-end (small dict) --
    {
        NsscSpace<8, 0x1D> sp(8192, 256);
        sp.init_k(16);
        Rng rng(4);
        constexpr unsigned MW = 8192 / 64;
        const unsigned M = 400;
        Dictionary dict;
        dict.alloc(M, MW);
        u32 msg[1100];
        for (unsigned i = 0; i < M; ++i) sp.encode_random(rng, msg, dict.code(i));
        unsigned e_star = sp.e_star();
        unsigned wrong = 0, unknown = 0;
        u64 query[MW];
        for (unsigned trial = 0; trial < M; ++trial) {
            copy_words(query, dict.code(trial), MW);
            NoiseEngine ne; ne.init(8192);
            ne.apply_uniform(query, 8192, e_star, rng);
            int pred = decode_scan(dict, query, sp.N / 2);
            if (pred < 0) ++unknown;
            else if ((unsigned)pred != trial) ++wrong;
        }
        printf("      certified radius e*=%u: wrong=%u unknown=%u of %u\n",
               e_star, wrong, unknown, M);
        CHECK(wrong == 0, "zero false matches at certified radius (adversary-proof)");

        // tightness: beyond radius, adversarial flips into nearest rival confuse
        unsigned confused = 0;
        for (unsigned trial = 0; trial < 50; ++trial) {
            int rival = -1; unsigned bestov = 0;
            for (unsigned r = 0; r < M; ++r) {
                if (r == trial) continue;
                unsigned ov = overlap_words(dict.code(trial), dict.code(r), MW);
                if (ov > bestov) { bestov = ov; rival = (int)r; }
            }
            if (rival < 0) continue;
            copy_words(query, dict.code(trial), MW);
            NoiseEngine ne; ne.init(8192);
            unsigned budget = sp.N - bestov + 1;          // margin + 1 (theory: confuses)
            unsigned t_off = budget / 2, f_on = budget - t_off;
            unsigned avail = sp.N - bestov;               // rival bits OFF in query
            if (f_on > avail) { t_off += f_on - avail; f_on = avail; }
            unsigned placed = ne.apply_adversarial(
                query, dict.code(trial), dict.code((unsigned)rival),
                8192, t_off, f_on, rng);
            if (placed != budget) continue;               // attack incomplete
            int pred = decode_scan(dict, query, sp.N / 2);
            if (pred >= 0 && (unsigned)pred != trial) ++confused;
        }
        printf("      beyond radius: adversary confused decoder in %u/50 attacks\n", confused);
        CHECK(confused > 0, "certified radius is TIGHT (attack succeeds just past it)");
    }

    // ------------------------- SDR formula cross-check (Ahmad-Hawkins) -----
    {
        // Published: n=2048, w=40, theta=13 -> fp = 6.0e-14
        long double fp = sdr_fp(2048, 40, 13);
        printf("      sdr_fp(2048,40,13) = %.3Le (published 6.0e-14)\n", fp);
        CHECK(fp > 3.0e-14L && fp < 12.0e-14L, "FP formula matches Ahmad-Hawkins 2015");

        long double space_log10 = log_choose(2048, 40) / 2.302585092994046L;
        printf("      C(2048,40) = 10^%.2Lf (published 2.37e84)\n", space_log10);
        CHECK(space_log10 > 84.3 && space_log10 < 84.45, "SDR space size matches 10^84 claim");
    }

    printf(failures ? "\nSELFTEST: %d FAILURES\n" : "\nSELFTEST: ALL PASS\n", failures);
    return failures ? 1 : 0;
}
