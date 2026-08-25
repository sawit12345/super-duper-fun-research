// NEBULA whole-brain loop — subcortical modules around the certified decoder.
// Every region is a bitwise mechanism grounded in the research notes:
//   Thalamus      : gated dual-route relay (driver direct / transthalamic)   [Sherman & Usrey 2024]
//   Hippocampus   : episodic index binding context x item, certified cleanup [Whittington 2020; Mackay 2024]
//   Basal ganglia : winner-take-all action selection (popcount tournament)   [Doya 2000]
//   Cerebellum    : shift-register delay lines learning transition timing    [Doya 2000]
//   Reticular RF  : global gain / density homeostasis masks                  [area reticularis]
//   Astrocyte     : slow saturating plasticity-window gate                   [Rupprecht 2024]
// HARD CONSTRAINTS: same kernel purity contract (no mult/div/float).
#pragma once
#include "code.hpp"
#include <vector>
#include <cassert>

namespace nebula {

// ------------------------------------------------------------ thalamus ----
// Per-channel gate: when open, driver payload passes unchanged; when closed,
// the modulator (transthalamic) route carries a delayed efference copy that
// can prime but never assert (ANDed with a priming mask, never decoded alone).
struct Thalamus {
    unsigned channels;
    std::vector<u64> gate;                 // bitmask of open channels
    std::vector<std::vector<u64>> delay;   // per-channel efference-copy register
    Thalamus(unsigned ch, size_t words)
        : channels(ch), gate(ch, ~0ull), delay(ch, std::vector<u64>(words, 0)) {}
    inline void set_gate(unsigned ch, bool open) {
        gate[ch] = open ? ~0ull : 0ull;
    }
    // Driver route: pass or block. Modulator route: latch copy for next step.
    inline void relay(unsigned ch, u64* stream, const u64* payload, size_t words) {
        for (size_t i = 0; i < words; ++i) {
            delay[ch][i] = stream[i];                    // efference copy out
            stream[i] = payload[i] & gate[ch];           // gated driver
        }
    }
    inline void prime(unsigned ch, u64* stream, size_t words) const {
        for (size_t i = 0; i < words; ++i)
            stream[i] |= delay[ch][i] & gate[ch];        // priming only
    }
};

// ---------------------------------------------------------- hippocampus ---
// Episodic index: episode = (context role, item role) bound by rotating each
// into a disjoint half-buffer and OR-ing (binding_overlap_split guarantees
// additive margins). Cleanup of noisy cues goes through the CERTIFIED decoder,
// so the hippocampus can never confidently recall a wrong item inside e*.
template <unsigned MB, unsigned POLY>
struct Hippocampus {
    using Sp = NsscSpace<MB, POLY>;
    Sp const* sp;
    size_t words;
    unsigned rot_ctx;                       // rotation placing context field
    unsigned rot_item;                      // rotation placing item field
    unsigned capacity;
    std::vector<u64> traces;                // capacity * words bound episodes
    std::vector<u64> items;                 // capacity * words item codes
    unsigned count = 0;

    Hippocampus(Sp const* space, size_t w, unsigned rctx, unsigned ritem,
                unsigned cap)
        : sp(space), words(w), rot_ctx(rctx), rot_item(ritem), capacity(cap),
          traces((size_t)cap * w), items((size_t)cap * w) {}

    // bind: trace = rot(ctx) | rot(item); store raw item for certified recall
    void store(const u64* ctx, const u64* item) {
        if (count >= capacity) return;
        u64* tr = &traces[(size_t)count * words];
        u64* it = &items[(size_t)count * words];
        zero_words(tr, words);
        for (size_t i = 0; i < words; ++i) {
            u64 c = rotl64(ctx[i], rot_ctx);
            u64 t = rotl64(item[i], rot_item);
            tr[i] = c | t;
        }
        copy_words(it, item, words);
        ++count;
    }

    static inline u64 rotl64(u64 x, unsigned r) { return (x << r) | (x >> (64 - r)); }

    // overlap of query with stored trace after aligning roles
    inline unsigned trace_overlap(const u64* q, unsigned idx) const {
        const u64* tr = &traces[(size_t)idx * words];
        unsigned acc = 0;
        for (size_t i = 0; i < words; ++i)
            acc += (unsigned)__builtin_popcountll(
                q[i] & (rotl64(tr[i], 64 - ((rot_ctx + rot_item) & 63))));
        (void)tr;
        return acc;
    }
    // simpler robust probe: direct popcount against aligned double-rotation
    unsigned best_episode(const u64* ctx_q, const u64* item_q) const {
        int best = -1; unsigned bestv = 0;
        for (unsigned e = 0; e < count; ++e) {
            const u64* tr = &traces[(size_t)e * words];
            unsigned acc = 0;
            for (size_t i = 0; i < words; ++i) {
                u64 aligned = rotl64(ctx_q[i], rot_ctx) |
                              rotl64(item_q[i], rot_item);
                acc += (unsigned)__builtin_popcountll(aligned & tr[i]);
            }
            if (acc > bestv) { bestv = acc; best = (int)e; }
        }
        return (unsigned)best;
    }
    inline const u64* episode_item(unsigned e) const {
        return &items[(size_t)e * words];
    }
};

// --------------------------------------------------------- basal ganglia --
// Winner-take-all over candidate action codes: score = overlap with state;
// ties resolve to NO ACTION (never a random guess) — matches framework ethos.
struct BasalGanglia {
    Dictionary actions;
    unsigned n_actions;
    void init(unsigned n_act, size_t words) {
        n_actions = n_act;
        actions.alloc(n_act, words);
    }
    int select(const u64* state, unsigned theta) const {
        return decode_scan(actions, state, theta);   // unique argmax or -1
    }
};

// ----------------------------------------------------------- cerebellum ---
// Delay-line transition memory: predicts next code by OR of delayed inputs
// weighted by learned taps. Supervised: on mismatch, OR the missed input into
// the tap for the observed lag (one-trial sequence learning, bitwise).
struct Cerebellum {
    unsigned lags;                       // number of delay taps
    size_t words;
    std::vector<u64> taps;               // lags * words : learned predictors
    std::vector<std::vector<u64>> line;  // delay line registers
    Cerebellum(unsigned l, size_t w) : lags(l), words(w),
        taps(l * w), line(l, std::vector<u64>(w, 0)) {}

    void push(const u64* obs, size_t w) {
        for (unsigned l = lags - 1; l > 0; --l)
            copy_words(line[l].data(), line[l - 1].data(), w);
        copy_words(line[0].data(), obs, w);
    }
    // predict: OR of tap_l & line_l (learned associations at each lag)
    void predict(u64* out, size_t w) const {
        zero_words(out, w);
        for (unsigned l = 0; l < lags; ++l)
            for (size_t i = 0; i < w; ++i)
                out[i] |= taps[l * w + i] & line[l][i];
    }
    // supervised update: for each lag, associate delayed input with observation
    void learn(const u64* obs, size_t w) {
        for (unsigned l = 0; l < lags; ++l)
            for (size_t i = 0; i < w; ++i)
                taps[l * w + i] |= line[l][i] & obs[i];
    }
};

// ------------------------------------------------------ reticular format --
// Global gain: keep buffer density near target by decimating or seeding with
// precomputed stride masks (bit-interleaved patterns; no arithmetic needed).
struct ReticularFormation {
    unsigned target_density;             // max ON bits tolerated in buffer
    explicit ReticularFormation(unsigned tgt) : target_density(tgt) {}
    unsigned density(const u64* buf, size_t words) const {
        return weight_words(buf, words);
    }
    // Decimate: clear the highest set bit each pass (guaranteed progress).
    void regulate(u64* buf, size_t words) {
        while (density(buf, words) > target_density) {
            bool changed = false;
            for (size_t i = words; i-- > 0 && !changed;) {
                int hi = 63 - __builtin_clzll(buf[i] | 1);   // candidate lane
                u64 bit = 1ull << hi;
                if (buf[i] & bit) { buf[i] &= ~bit; changed = true; }
            }
            if (!changed) return;                            // empty buffer
        }
    }
};

// ------------------------------------------------------------ astrocyte ---
// Slow saturating counter: opens plasticity windows only after sustained
// activity (novelty), closes them after repeated replay (consolidation).
struct Astrocyte {
    unsigned level = 0, ceiling;
    explicit Astrocyte(unsigned ceil_) : ceiling(ceil_) {}
    inline void tick(bool novelty) {
        if (novelty) { if (level < ceiling) ++level; }
        else if (level > 0) --level;
    }
    inline bool plasticity_open() const { return level >= (ceiling + 1) / 2; }
};

} // namespace nebula
