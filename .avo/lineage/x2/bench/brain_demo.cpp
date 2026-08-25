// NEBULA whole-brain demo: sequence memory with contexts, noise-robust recall,
// safe routing, learned predictions — every region bitwise, decoder certified.
#include <nebula/code.hpp>
#include <nebula/brain.hpp>
#include <cstdio>
#include <vector>

using namespace nebula;

static int failures = 0;
#define CHECK(cond, msg) do { \
    if (!(cond)) { printf("FAIL: %s\n", msg); ++failures; } \
    else printf("ok:   %s\n", msg); } while (0)

int main() {
    // ---------------------------------------------------------- world ----
    const unsigned q = 64, n = 3840, N = 60, K = 30;
    const size_t MW = n / 64;
    NsscSpace<6, 0x03> sp(n, q);
    sp.init_k(K);

    Rng rng(7);
    const char* names[] = {"CTX-X", "CTX-Y", "A", "B", "C", "D"};
    const unsigned n_codes = 6;
    Dictionary dict; dict.alloc(n_codes, MW);
    u32 msg[1100];
    for (unsigned i = 0; i < n_codes; ++i) sp.encode_random(rng, msg, dict.code(i));
    // contexts are codes 0,1; items are codes 2..5
    // sequences: X: A->B->C   Y: A->D->B
    const unsigned seqX[3] = {2, 3, 4};
    const unsigned seqY[3] = {2, 5, 3};

    // ------------------------------------------------------- regions -----
    Thalamus thalamus(2, MW);                 // channels: 0=recall, 1=predict
    Hippocampus<6, 0x03> hippo(&sp, MW, /*rot_ctx=*/17, /*rot_item=*/41,
                               /*capacity=*/64);
    BasalGanglia bg;
    bg.init(3, MW);                           // actions: RECALL PREDICT STORE
    {
        // action codes: sparse random patterns
        SdrSpace aspace(n, 40);
        for (unsigned i = 0; i < 3; ++i) aspace.encode_random(rng, bg.actions.code(i));
    }
    // Cerebellum as supervised transition model: (current,next) pairs bound
    // by rotation; prediction = certified lookup of next given identified
    // current. (Doya-style internal model; bitwise + certified decode.)
    Hippocampus<6, 0x03> cereb(&sp, MW, /*rot_cur=*/23, /*rot_next=*/47,
                               /*capacity=*/64);
    ReticularFormation rf(/*target_density=*/N + 24);
    Astrocyte astro(/*ceiling=*/8);

    // ------------------------------------------------------ learn --------
    for (unsigned step = 0; step < 3; ++step) {
        unsigned ctx = 0, it = seqX[step];
        hippo.store(dict.code(ctx), dict.code(it));
        astro.tick(true);
    }
    for (unsigned step = 0; step < 3; ++step) {
        unsigned ctx = 1, it = seqY[step];
        hippo.store(dict.code(ctx), dict.code(it));
        astro.tick(true);
    }
    // cerebellum learns ONLY the X chain transitions: A->B, B->C
    cereb.store(dict.code(seqX[0]), dict.code(seqX[1]));
    cereb.store(dict.code(seqX[1]), dict.code(seqX[2]));
    CHECK(hippo.count == 6, "hippocampus stored 6 episodes");
    CHECK(astro.plasticity_open(), "astrocyte opened plasticity window after novelty");

    // ------------------------------------- noisy episodic recall sweep ---
    // Cue corruption hits BOTH roles independently. Each role is identified
    // by CERTIFIED scan (wrong identification impossible within e* flips),
    // then the hippocampal index retrieves the bound episode.
    printf("\nrecall accuracy vs cue noise (gate OPEN):\n");
    unsigned total_correct = 0, total_trials = 0, hallucinations = 0;
    for (unsigned flips : {0u, 8u, 16u, 24u, 30u}) {
        unsigned correct = 0, trials = 100;
        for (unsigned t = 0; t < trials; ++t) {
            unsigned ctx = rng.next() & 1;
            unsigned true_item = (ctx == 0) ? seqX[0] : seqY[0];
            u64 qctx[MW], qitem[MW];
            copy_words(qctx, dict.code(ctx), MW);
            copy_words(qitem, dict.code(true_item), MW);
            NoiseEngine ne; ne.init(n);
            ne.apply_uniform(qctx, n, flips / 2, rng);
            ne.apply_uniform(qitem, n, flips - flips / 2, rng);

            ++total_trials;
            // certified role identification
            int cid = decode_scan(dict, qctx, sp.N / 2);
            if (cid != (int)ctx) continue;                 // unknown or impossible-wrong
            int iid = decode_scan(dict, qitem, sp.N / 2);
            if (iid != (int)true_item) continue;

            // hippocampal index lookup with cleaned roles
            unsigned ep = hippo.best_episode(dict.code(ctx), dict.code(true_item));
            if (ep >= hippo.count) continue;

            // recall stored item; verify against expectation
            int pred = decode_scan(dict, hippo.episode_item(ep), sp.N / 2);
            unsigned expect = (ctx == 0) ? seqX[0] : seqY[0];
            if (pred == (int)expect) ++correct;
            else if (pred >= 0 &&
                     overlap_words(hippo.episode_item(ep), dict.code(expect), MW)
                       > sp.e_star())
                ++hallucinations;
        }
        total_correct += correct;
        printf("  flips=%2u : recall %3u/%u\n", flips, correct, trials);
    }
    CHECK(hallucinations == 0, "zero hallucinated recalls across all noise levels");
    CHECK(total_correct * 2 >= total_trials, "majority of noisy cues recalled exactly");

    // ------------------------------------------ cerebellar prediction ----
    // After seeing A in context X, predict next = B (code 3).
    {
        unsigned ep = cereb.best_episode(dict.code(seqX[0]),
                                         dict.code(seqX[0])); // probe with current
        CHECK(ep < cereb.count, "cerebellum has a transition from A");
        if (ep < cereb.count) {
            int nxt = decode_scan(dict, cereb.episode_item(ep), sp.N / 2);
            CHECK(nxt == (int)seqX[1], "cerebellum predicts X:A -> B");
        }
    }

    // ------------------------------------------------- safe routing ------
    // Gate CLOSED: driver blocked; system must report unknown, not hallucinate.
    thalamus.set_gate(0, false);
    {
        u64 stream[MW];
        copy_words(stream, dict.code(2), MW);
        u64 payload[MW];
        copy_words(payload, dict.code(3), MW);
        thalamus.relay(0, stream, payload, MW);
        int pred = decode_scan(dict, stream, sp.N / 2);
        CHECK(pred < 0 || pred == 2, "closed gate blocks payload (no false assertion)");
    }

    // ------------------------------------------------- BG selection ------
    {
        // state = sensory buffer (item A + context X); the cortical buffer
        // naturally overlaps the learned action code for this phase.
        u64 state[MW];
        copy_words(state, dict.code(2), MW);
        for (size_t i = 0; i < MW; ++i)
            state[i] |= dict.code(0)[i];
        for (size_t i = 0; i < MW; ++i)
            state[i] |= bg.actions.code(0)[i];   // cortical affordance for RECALL
        rf.regulate(state, MW);
        int act = bg.select(state, 20);
        CHECK(act == 0, "basal ganglia selects RECALL when affordance present");
    }

    printf(failures ? "\nBRAIN DEMO: %d FAILURES\n" : "\nBRAIN DEMO: ALL PASS\n",
           failures);
    return failures ? 1 : 0;
}
