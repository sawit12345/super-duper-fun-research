// NEBULA benchmark: capacity, noise robustness, throughput — NSSC vs flat SDR.
// Matched parameters: identical binary length n and weight w for both systems.
#include <nebula/code.hpp>
#include <chrono>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace nebula;
using Clock = std::chrono::steady_clock;

static double secs_since(Clock::time_point t0) {
    return std::chrono::duration<double>(Clock::now() - t0).count();
}

struct Outcome { unsigned correct = 0, wrong = 0, unknown = 0; };

// Run uniform-noise trials: pick random target, corrupt, decode by scan.
static Outcome run_uniform(const Dictionary& dict, unsigned n, unsigned w,
                           unsigned flips, unsigned trials, Rng& rng) {
    Outcome o;
    std::vector<u64> query(n / 64);
    NoiseEngine ne; ne.init(n);
    for (unsigned t = 0; t < trials; ++t) {
        unsigned target = rng.next() % dict.M;
        copy_words(query.data(), dict.code(target), n / 64);
        ne.apply_uniform(query.data(), n, flips, rng);
        int pred = decode_scan(dict, query.data(), w / 2);
        if (pred < 0) ++o.unknown;
        else if ((unsigned)pred == target) ++o.correct;
        else ++o.wrong;
    }
    return o;
}

// Max adversarial flips sustained with wrong-rate <= 1e-2 (binary search-ish sweep).
static unsigned adv_breakpoint(const Dictionary& dict, unsigned n, unsigned w,
                               unsigned trials, Rng& rng, double* wrong_at_half) {
    // find nearest rival for each trial target adaptively (full-scan adversary)
    auto attack = [&](unsigned e) {
        unsigned wrong = 0;
        std::vector<u64> query(n / 64);
        NoiseEngine ne; ne.init(n);
        for (unsigned t = 0; t < trials; ++t) {
            unsigned target = rng.next() % dict.M;
            // nearest rival of target under CLEAN codes
            int rival = -1; unsigned bestov = 0;
            for (unsigned r = 0; r < dict.M; ++r) {
                if (r == target) continue;
                unsigned ov = overlap_words(dict.code(target), dict.code(r), n / 64);
                if (ov > bestov) { bestov = ov; rival = (int)r; }
            }
            if (rival < 0) continue;
            copy_words(query.data(), dict.code(target), n / 64);
            unsigned margin = w - bestov;
            if (e <= margin) continue;              // theory: impossible
            unsigned t_off = e / 2, f_on = e - t_off;
            unsigned avail = w - bestov;
            if (f_on > avail) { t_off += f_on - avail; f_on = avail; }
            ne.apply_adversarial(query.data(), dict.code(target),
                                 dict.code((unsigned)rival), n, t_off, f_on, rng);
            int pred = decode_scan(dict, query.data(), w / 2);
            if (pred >= 0 && (unsigned)pred != target) ++wrong;
        }
        return (double)wrong / (double)trials;
    };
    unsigned lo = 1, hi = w;                        // answer in [lo, hi]
    *wrong_at_half = attack(w / 2);
    // coarse sweep to find breakpoint within 1/8 of w
    unsigned step = w / 16 + 1;
    unsigned last_ok = 0;
    for (unsigned e = step; e <= w; e += step) {
        if (attack(e) <= 0.01) last_ok = e;
        else break;
    }
    (void)lo; (void)hi;
    return last_ok;
}

int main(int argc, char**) {
    if (argc > 1) { /* reserved */ }

    // ------------------------------------------------------------ config --
    // Matched systems: binary length n, weight w identical for NSSC and SDR.
    const unsigned q = 64, m_bits = 6, poly = 0x03;
    const unsigned NC = 16;                // nested category repetition blocks
    const unsigned N = 60;                 // NSSC identity-core symbols
    const unsigned K = 30;                 // message symbols
    const unsigned n = N * q;              // 3840 bits (main matched geometry)
    const unsigned w = N;                  // 60
    const unsigned E_STAR = N - K;         // 30 certified flip radius (core)

    printf("# config: n=%u w=%u q=%u N=%u NC=%u K=%u lambda_max=%u e_star=%u\n",
           n, w, q, N, NC, K, K - 1, E_STAR);

    NsscSpace<m_bits, poly> sp(n, q);
    sp.init_k(K);
    SdrSpace sdr(n, w);

    Rng rng(0xC0FFEE);
    const unsigned TRIALS = 300;

    // -------------------------------------------------- build dictionaries --
    std::vector<unsigned> Ms = {1000, 10000, 100000, 1000000};
    for (unsigned M : Ms) {
        Dictionary dn, ds;
        dn.alloc(M, n / 64);
        ds.alloc(M, n / 64);
        u32 msg[1100];
        u64 code[1100];
        for (unsigned i = 0; i < M; ++i) {
            sp.encode_random(rng, msg, code);
            copy_words(dn.code(i), code, n / 64);
            sdr.encode_random(rng, ds.code(i));
        }

        // ---- uniform noise sweep at 10% and 20% of n ----
        for (unsigned eta_pct : {10u, 20u, 30u}) {
            unsigned flips = n * eta_pct / 100;
            Outcome on = run_uniform(dn, n, w, flips, TRIALS, rng);
            Outcome os = run_uniform(ds, n, w, flips, TRIALS, rng);
            printf("UNI,M=%u,eta=%u,NSSC recall=%.4f wrong=%.2e unk=%.3f | "
                   "SDR recall=%.4f wrong=%.2e unk=%.3f\n",
                   M, eta_pct,
                   (on.correct + on.wrong ? (double)on.correct / TRIALS : 0),
                   (double)on.wrong / TRIALS, (double)on.unknown / TRIALS,
                   (os.correct + os.wrong ? (double)os.correct / TRIALS : 0),
                   (double)os.wrong / TRIALS, (double)os.unknown / TRIALS);
        }

        // ---- structural: sampled max pairwise intersection ----
        {
            unsigned mx_n = 0, mx_s = 0;
            for (unsigned t = 0; t < 200000; ++t) {
                unsigned i = rng.next() % M, j = rng.next() % M;
                if (i == j) continue;
                unsigned ov = overlap_words(dn.code(i), dn.code(j), n / 64);
                if (ov > mx_n) mx_n = ov;
                ov = overlap_words(ds.code(i), ds.code(j), n / 64);
                if (ov > mx_s) mx_s = ov;
            }
            printf("IXN,M=%u,NSSC_max_intersection=%u,cert_cap=%u | "
                   "SDR_max_intersection=%u (uncapped, grows with M)\n",
                   M, mx_n, K - 1, mx_s);
        }

        // ---- adversarial breakpoint ----
        double wr_half_n = 0, wr_half_s = 0;
        unsigned adv_n = adv_breakpoint(dn, n, w, 120, rng, &wr_half_n);
        unsigned adv_s = adv_breakpoint(ds, n, w, 120, rng, &wr_half_s);
        printf("ADV,M=%u,NSSC_breakpoint=%u (wrong@w/2=%.2f) | "
               "SDR_breakpoint=%u (wrong@w/2=%.2f)\n",
               M, adv_n, wr_half_n, adv_s, wr_half_s);

        // ---- throughput at this M: scan vs algebraic (NSSC) ----
        {
            const unsigned QB = 200;
            std::vector<u64> query(n / 64), out(n / 64);
            NoiseEngine ne; ne.init(n);
            Clock::time_point t0 = Clock::now();
            volatile int sink = 0;
            for (unsigned t = 0; t < QB; ++t) {
                unsigned target = rng.next() % M;
                copy_words(query.data(), dn.code(target), n / 64);
                ne.apply_uniform(query.data(), n, E_STAR / 2, rng);
                sink += decode_scan(dn, query.data(), w / 2);
            }
            double scan_s = secs_since(t0);
            Clock::time_point t1 = Clock::now();
            unsigned okcnt = 0;
            for (unsigned t = 0; t < QB; ++t) {
                unsigned target = rng.next() % M;
                copy_words(query.data(), dn.code(target), n / 64);
                ne.apply_uniform(query.data(), n, E_STAR / 2, rng);
                okcnt += sp.decode_algebraic(query.data(), out.data(), msg) ? 1 : 0;
            }
            double alg_s = secs_since(t1);
            (void)t0;
            printf("THR,M=%u,scan_us_per_q=%.1f,algebraic_us_per_q=%.1f,speedup=%.1fx,alg_success=%.3f\n",
                   M, scan_s / QB * 1e6, alg_s / QB * 1e6,
                   scan_s / std::max(alg_s, 1e-9), (double)okcnt / QB);
            (void)sink;
        }
    }

    // ------------------------------ graceful degradation ladder (M=1e5) ----
    // Own matched geometry: nested space (+16 category blocks) vs SDR given
    // the same total bit budget (n=4864, w=76).
    // Ladder: exact item (certified) -> category only (nested field vote)
    //         -> unknown. Flat SDR has no intermediate level.
    {
        unsigned M = 100000;
        const unsigned NL = N + NC;                    // ladder weight
        const unsigned nl = (N + NC) * q;              // ladder length
        NsscSpace<m_bits, poly> spl(nl, q, NC);
        spl.init_k(K);
        SdrSpace sdrl(nl, NL);
        Dictionary dn, ds;
        dn.alloc(M, nl / 64); ds.alloc(M, nl / 64);
        u32 msg[1100];
        u64 code[1100];
        for (unsigned i = 0; i < M; ++i) {
            spl.encode_random(rng, msg, code);
            copy_words(dn.code(i), code, nl / 64);
            sdrl.encode_random(rng, ds.code(i));
        }
        for (unsigned flips : {20u, 40u, 60u, 80u, 100u}) {
            unsigned n_exact = 0, n_cat = 0, n_unk = 0, n_wrong = 0;
            unsigned s_ok = 0, s_unk = 0, s_wrong = 0;
            std::vector<u64> query(nl / 64), out(nl / 64);
            NoiseEngine ne; ne.init(nl);
            for (unsigned t = 0; t < 200; ++t) {
                unsigned target = rng.next() % M;
                copy_words(query.data(), dn.code(target), nl / 64);
                ne.apply_uniform(query.data(), nl, flips, rng);

                // NSSC ladder
                if (spl.decode_algebraic(query.data(), out.data(), msg)) {
                    ++n_exact;
                } else {
                    u32 cat = spl.category_vote(query.data());
                    int pred = decode_scan(dn, query.data(), NL / 2);
                    if (pred >= 0 && (unsigned)pred != target) ++n_wrong;
                    else if (cat < q) ++n_cat;
                    else ++n_unk;
                }

                // flat SDR: exact or unknown or wrong — no intermediate level
                int p2 = decode_scan(ds, query.data(), NL / 2);
                if (p2 < 0) ++s_unk;
                else if ((unsigned)p2 == target) ++s_ok;
                else ++s_wrong;
            }
            printf("LADDER,%u,%u,%u,%u,%u,%u,%u,%u\n",
                   flips, n_exact, n_cat, n_unk, n_wrong, s_ok, s_unk, s_wrong);
        }
    }

    // --------------------------------------- headline analytic capacity ----
    // Certified dictionary size at radius e*: M = q^(w - e*).
    {
        unsigned e_star = E_STAR;
        // log10(q^(w-e*)) = (w-e*) * log10(q)
        double lg = (double)(w - e_star) * std::log10((double)q);
        printf("CAP,formula=M=q^(w-e*),log10_M_certified_at_e_star=%u,log10_value=%.1f\n",
               e_star, lg);
        // Flat-SDR comparable certificate does not exist; its space is 10^84-style raw.
        double sdr_space_log10 = 0; // C(3840,60):
        // lgamma-based
        sdr_space_log10 = (std::lgamma(n + 1) - std::lgamma(w + 1) - std::lgamma(n - w + 1)) / std::log(10.0);
        printf("CAP,flat_sdr_space_log10=%.1f (no noise certificate)\n", sdr_space_log10);
    }
    return 0;
}
