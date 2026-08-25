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
See .avo/lineage/LEDGER.md (currently empty — no commits yet).
