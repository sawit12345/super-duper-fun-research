// Axis B: role-binding composition benchmark.
// K roles hold value codes in DISJOINT bit slices of one buffer (the hypothesis
// of binding_overlap_split). Composition = OR of slices; query-by-role extracts
// the slice and certified-decodes against that role's value dictionary.
// The lemma predicts combined overlap = SUM of per-role overlaps, so crosstalk
// across roles is structurally zero and margins stay per-role clean.
#include <nebula/code.hpp>
#include <cstdio>
#include <vector>

using namespace nebula;

int main() {
    const unsigned q = 64;
    const unsigned n_total = 3840;
    const unsigned MW = n_total / 64;

    printf("BIND,K,wrong,trials,min_true_ov,max_crosstalk,predicted_floor\n");
    for (unsigned K : {1u, 2u, 4u}) {
        const unsigned n_role = n_total / K;          // disjoint slice size
        const unsigned Nr = n_role / q;               // symbols per role code
        const unsigned Kr = Nr / 2;                   // message symbols
        if (Nr < 4 || Kr < 1) continue;

        NsscSpace<6, 0x03> sp(n_role, q);
        sp.init_k(Kr);

        const unsigned NV = 12;
        // one value dictionary per role (independent spaces)
        std::vector<Dictionary> dicts(K);
        u32 msg[1100];
        Rng rng(0x501 + K);
        for (unsigned r = 0; r < K; ++r) {
            dicts[r].alloc(NV, n_role / 64);
            for (unsigned i = 0; i < NV; ++i)
                sp.encode_random(rng, msg, dicts[r].code(i));
        }

        unsigned wrong = 0, trials = 200;
        long min_true = 1L << 30, max_cross = 0;
        for (unsigned t = 0; t < trials; ++t) {
            std::vector<u64> comp(MW, 0);
            unsigned planted[8];
            for (unsigned r = 0; r < K; ++r) {
                planted[r] = rng.next() % NV;
                copy_words(comp.data() + r * (n_role / 64),
                           dicts[r].code(planted[r]), n_role / 64);
            }
            // query every role
            for (unsigned r = 0; r < K; ++r) {
                int pred = decode_scan(dicts[r],
                                       comp.data() + r * (n_role / 64), Nr / 2);
                if (pred != (int)planted[r]) ++wrong;
                unsigned ovt = overlap_words(comp.data() + r * (n_role / 64),
                                             dicts[r].code(planted[r]),
                                             n_role / 64);
                if ((long)ovt < min_true) min_true = ovt;
                for (unsigned v = 0; v < NV; ++v) {
                    if (v == planted[r]) continue;
                    unsigned ovx = overlap_words(comp.data() + r * (n_role / 64),
                                                 dicts[r].code(v), n_role / 64);
                    if ((long)ovx > max_cross) max_cross = ovx;
                }
            }
        }
        printf("BIND,K=%u,wrong=%u,trials=%u,min_true_ov=%ld,"
               "max_crosstalk=%ld,predicted_floor=%ld\n",
               K, wrong, trials, min_true, max_cross,
               (long)(Kr - 1));   // per-role certified crosstalk cap
    }
    return 0;
}
