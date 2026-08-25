# AVO Working Memory — super-duper-fun-research

## Objective (user goal)
Design a NEW brain-like framework beyond SDR/HTM/Thousand-Brains(Monty):
- Whole brain (not just neocortex), inspired by recent neuroscience (download papers).
- Implemented in pure C++, maximally efficient: bitwise + basic arithmetic only,
  NO multiplication, NO division, NO floats in the hot path.
- Efficient dataflow.
- Math proven in Lean.
- Must beat SDR on BOTH capacity and noise robustness (SDR space ~10^84 for n=2048,k=40).

## Environment facts
- g++ 13.3, clang++ available; make/cmake OK. 4 cores, 15GB RAM.
- NO lean/elan installed yet — need `curl elanup` install attempt later.
- Repo was empty (README only). No AGENTS.md existed; evaluate.sh is OUR contract:
  build C++ sim + run benches (capacity/noise/throughput) + lean checks if present.

## Framework direction (v1 — post-research): NEBULA
**NEBULA** = Nested-code Efficient Brain-wide Universal Learning Architecture.
Code family: **NSSC — Nested Superimposed Sparse Codes** (Kautz-Singleton-style
structured constant-weight codes + nested refinement fields + rotation binding).

### Verified research anchors
- SDR space C(2048,40)=2.37178e84 (log10=84.375, exact). It's SPACE SIZE, zero guarantees.
- Numenta FP formula: fp(θ)=Σ_{b≥θ}C(w,b)C(n-w,w-b)/C(n,w); θ=w/2 operating point;
  union bound fp_X ≤ M·fp_xw. Exact tails @2048/40: θ=13→6.0e-14, θ=21→2.4e-28.
- Monty/TBP: ZERO thalamus/BG/cerebellum/hippocampus; abandoned SDRs entirely.
- KEY MATH (Kautz-Singleton): RS[N,k']_q → one-hot concat: n=N·q, w=N, M=q^{k'},
  λ_max(pairwise intersection)=k'-1 GUARANTEED, d_min=2(N-k'+1).
  **Unique decode under e flips guaranteed iff e ≤ w-k'; so M = q^{w-e*}.**
  Capacity and certified noise tolerance are directly exchangeable: log_q M = w - e*.
- GF(2^m) arithmetic = shift+XOR only (LUT for div) → fits no-mult/div/float constraint.
  RS encode = LFSR; decode = syndrome + Chien search + Forney (all table/shift-XOR).
- ALGEBRAIC DECODE kills the O(M) dictionary scan → dataflow efficiency win vs SDR.
- Naive independent-field splitting LOSES to flat SDR on FP (verified reasoning:
  fragmenting 20 coincidences into 4×5 weaker constraints). Structure ≠ fragmentation.
  Nested/refinement fields must ADD consistency constraints, not just partition.
- Open gap (lit): no principled multi-resolution constant-weight construction exists.

### Architecture layers
1. Identity core: KS/RS structured code (guaranteed λ_max, algebraic decode).
2. Nested refinement fields: coarse→fine consistent levels (progressive disclosure
   under noise; coarse survives when fine dies — dendritic compartment analogy:
   proximal=identity core, distal=lateral context, apical=top-down prediction).
3. Role binding: bitwise rotation per channel + OR superposition (cortical areas=roles).
4. Whole-brain loop: thalamic gating (mask AND, driver/modulator dual route),
   hippocampal episodic index (bind place⊗content, cleanup via certified decoder),
   BG winner-take-all (popcount tournament), cerebellar delay-line timing,
   reticular gain (global mask density homeostasis).

### Honest-comparison policy (hard rule)
Matched n and w vs baseline SDR unless variant flag. Baseline = random k-of-n,
θ=w/2, Numenta formulas. Report BOTH random and adversarial noise. Our claims:
(a) certified zero false matches ≤ e* (Lean-proofable conditional theorem),
(b) algebraic decode O(N + LFSR) vs O(M·n/64) scan,
(c) >10^84 addressable certified memories at n=65536 (8KB/code): M=q^{w-e*},
(d) graceful multi-resolution degradation (flat SDR has none).

## Hypothesis queue
1. [ACTIVE] H1: KS-core NSSC at matched (n,w) sustains strictly larger M than random
   SDR at fp≤1e-6, recall≥0.99 under ADVERSARIAL noise; equal-or-better under random.
2. H2: Algebraic decode ≥10x faster than dictionary scan at M≥10^5.
3. H3: Nested fields give monotone coarse-recall advantage under escalating noise
   (fine dies first, identity persists) with no FP regression vs flat.
4. H4 (Lean): conditional unique-decode theorem + intersection/packing lemmas.


## Dead ends
- Naive independent-field splitting LOSES to flat SDR on FP under iid noise
  (fragmenting 20 coincidences into 4×5 is a weaker constraint). Structure must ADD
  consistency constraints (KS algebraic structure), not just partition bits.
- Lean core-only (no Mathlib) too painful for Finset card lemmas → switched to Mathlib,
  cache downloaded fine (~1.5GB), builds fast via cache.

## DONE: Lean proofs (proofs/Nebula.lean) — ALL VERIFIED, no sorryAx
lake build green. 7 theorems, axioms = [propext, Classical.choice, Quot.sound] only:
1. overlap_query_le — rival bound |Q∩R| ≤ |C∩R| + f
2. overlap_query_true_ge — true bound |Q∩C| ≥ w − t
3. certified_unique_decode — THE certification theorem (e ≤ w−A−1 ⇒ exact decode)
4. certified_decode_correct — argmax decoder correctness corollary
5. binExpand_overlap_le / binary_dictionary_certified — one-hot lift:
   binary overlap ≤ outer agreement ⇒ certified binary dictionary from RS/KS theory
6. packing_bound — M·C(k,λ+1) ≤ C(n,λ+1) (strict families pay capacity for guarantees)
7. binding_overlap_split — rotation-bound roles have additive margins
8. ks_capacity_formula — e* = w−k', M = q^(w−e*) operating point
NOTE: hq param name in binExpand_overlap_le must stay `hq` (renaming broke refs).

## Next steps
- C++ kernel: gf.hpp DONE; rs.hpp (RS encode/erasure-decode), sdr.hpp baseline,
  nssc.hpp code type, noise engine (exact fixed-count flips), selftest, benches,
  evaluate.sh + asm purity check (no mul/div/float in kernel.o disassembly).
- Then whole-brain loop demo module.


## Ledger pointer
x1 COMMITTED (score 8.73/10): capacity=1.0 noise=0.683 thr=1.0 lean=1.0.
Headline: eta=20% → NSSC perfect (recall 1.0, wrong=0 @ M=10^5) vs SDR collapse
(recall 0.32, wrong 35%); algebraic decode 77x faster than scan at M=10^5 (O(1)
vs O(M)); purity audit 5042 instrs 0 mul/div/float; Lean 7 theorems no sorryAx.

## Next steps (x2+ hypothesis queue, priority order)
1. [PENDING] Capacity frontier at larger M (10^6) + larger n variant (n=8192,q=256,
   N=32,K=16 vs matched SDR) — confirm SDR usable-M stays tiny while NSSC scales.
2. [PENDING] Adversarial differentiation at larger M: SDR max-intersection grows
   with M (birthday tail), NSSC capped at K-1 forever. Sweep M to 10^6 and show
   adversarial breakpoint divergence (coarse step-4 sweep at small M showed parity).
3. [PENDING] Whole-brain loop demo module (thalamic gate, hippocampal index store/
   cleanup via certified decoder, BG WTA tournament, cerebellar delay lines,
   reticular gain). Demo task: noisy cue → sequence completion with routing.
4. [PENDING] Nested refinement fields (coarse/mid/fine consistent levels) — the
   "progressive disclosure under noise" claim; needs design care (naive splitting
   LOSES — must add consistency constraints, e.g., fine symbols constrained by
   coarse identity via per-bucket sub-codes).
5. [STRETCH] Lean: formalize GF(2^m) + RS distance itself (remove the machine-
   checked-by-C++ hypothesis on hagree).
6. Docs: README with results tables; research notes cross-links.

## Environment quirks / gotchas (do not rediscover)
- exp_t/log conventions: codeword symbol j has degree N-1-j (HIGH-FIRST layout);
  syndrome t evaluates at alpha^{t+1}; generator roots alpha^1..alpha^D.
- exp_t(i) valid only for i < 2(q-1) (single subtract fold).
- NsscSpace ctor: rs member init-list dummy RS(2,1) then reassign in body (RS(1,1)
  trips assert). N computed by shift AFTER l2q loop.
- decode_erasure mat stride is MAT_STRIDE=2048 pow2 (purity: no imul-by-odd).
- evaluate.sh parses bench.log regexes — keep UNI/IXN/ADV/THR/CAP line formats.
- purity audit: strip jump-target symbol names before grepping mnemonics.

