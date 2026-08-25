// Adversarial margin scaling: how does the worst-case confusion budget evolve
// with dictionary size M?
//   margin(C) = w - max_{R != C} |C AND R|
// An adversary with flip budget e can confuse a max-overlap decoder iff e > margin.
// Theory: NSSC margins are CERTIFIED >= w - (K-1) at every M (Lean-backed).
//         Flat SDR margins shrink as the birthday tail thickens with M.
// We measure the empirical minimum margin over sampled targets for both
// systems at matched (n, w), plus validate attack mechanics at margin+1.
#include <nebula/code.hpp>
#include <cstdio>
#include <vector>
#include <algorithm>

using namespace nebula;

int main() {
    const unsigned q = 64, N = 60, K = 30;
    const unsigned n = N * q, w = N;
    const unsigned MW = n / 64;

    printf("# advscale: n=%u w=%u K=%u (NSSC certified margin >= %u)\n",
           n, w, K, w - (K - 1));

    NsscSpace<6, 0x03> sp(n, q);
    sp.init_k(K);
    SdrSpace sdr(n, w);
    Rng rng(0xAD5);

    for (unsigned M : {1000u, 10000u, 100000u, 1000000u}) {
        Dictionary dn, ds;
        dn.alloc(M, MW); ds.alloc(M, MW);
        u32 msg[1100];
        for (unsigned i = 0; i < M; ++i) {
            sp.encode_random(rng, msg, dn.code(i));
            sdr.encode_random(rng, ds.code(i));
        }
        const unsigned TARGETS = 2000;
        unsigned min_margin_n = w, min_margin_s = w;
        unsigned arg_maxov_n = 0, arg_maxov_s = 0;
        for (unsigned t = 0; t < TARGETS; ++t) {
            unsigned c = rng.next() % M;
            const u64* Cn = dn.code(c);
            const u64* Cs = ds.code(c);
            for (unsigned r = 0; r < M; ++r) {
                if (r == c) continue;
                unsigned on = overlap_words(Cn, dn.code(r), MW);
                if (on > arg_maxov_n) arg_maxov_n = on;
                unsigned os = overlap_words(Cs, ds.code(r), MW);
                if (os > arg_maxov_s) arg_maxov_s = os;
            }
        }
        min_margin_n = w - arg_maxov_n;
        min_margin_s = w - arg_maxov_s;
        printf("MARGIN,M=%u,NSSC_min_margin=%u,cert_floor=%u | "
               "SDR_min_margin=%u\n",
               M, min_margin_n, w - (K - 1), min_margin_s);

        // Attack-mechanics validation: budget = observed min margin + 1 must
        // actually confuse (sampled), for each system where margin < w.
        auto attack_check = [&](Dictionary& d, unsigned margin) -> double {
            if (margin >= w) return 0.0;
            unsigned e = margin + 1;
            unsigned confused = 0, tries = 50;
            std::vector<u64> query(MW);
            NoiseEngine ne; ne.init(n);
            for (unsigned t = 0; t < tries; ++t) {
                unsigned target = rng.next() % M;
                int rival = -1; unsigned bestov = 0;
                for (unsigned r = 0; r < M; ++r) {
                    if (r == target) continue;
                    unsigned ov = overlap_words(d.code(target), d.code(r), MW);
                    if (ov > bestov) { bestov = ov; rival = (int)r; }
                }
                if (rival < 0) continue;
                copy_words(query.data(), d.code(target), MW);
                unsigned t_off = e / 2, f_on = e - t_off;
                unsigned avail = w - bestov;
                if (f_on > avail) { t_off += f_on - avail; f_on = avail; }
                ne.apply_adversarial(query.data(), d.code(target),
                                     d.code((unsigned)rival), n, t_off, f_on, rng);
                int pred = decode_scan(d, query.data(), w / 2);
                if (pred >= 0 && (unsigned)pred != target) ++confused;
            }
            return (double)confused / tries;
        };
        double cn = attack_check(dn, min_margin_n);
        double cs = attack_check(ds, min_margin_s);
        printf("ATTACK,M=%u,NSSC_confusion_at_minmargin_plus1=%.2f | "
               "SDR_confusion_at_minmargin_plus1=%.2f\n", M, cn, cs);
    }
    return 0;
}
