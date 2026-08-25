# NEBULA evaluation contract v2 (post-supervisor review)

## Gates (all must pass; any failure => SCORE 0)
1. Kernel purity audit: no mul/div/float instructions in nebula:: disassembly.
2. C++ self-test: ALL PASS (incl. certified-radius zero-wrong, KS intersection
   cap, Ahmad-Hawkins FP cross-check).
3. Whole-brain demo: ALL PASS (incl. zero hallucinated recalls).
4. Lean: builds with NO sorryAx.
5. Ladder integrity: NSSC wrong == 0 at every tested degradation level.

## Scored axes (open problems; max 10)
- A. Empirical scale demonstration (3 pts): n=65536 (8 KB/code) dictionary,
   >=10^5 codes stored, recall=1.0 & wrong=0 under certified-radius dropout,
   algebraic decode time reported vs subsampled scan baseline.
- B. Role-binding composition (2 pts): K-role superposition benchmark;
   query-by-role via certified decode; zero-wrong across K sweep; measured
   crosstalk consistent with binding_overlap_split additive margins.
- C. Ladder service quality (2 pts): area served (exact+category) across
   flip budgets 20..100; requires the absolute-performance term
   (NSSC served fraction), NOT merely baseline failure.
- D. Decode throughput (1 pt): algebraic/scan speedup at M=10^6, normalized
   to 500x (log-scale headroom retained).
- E. Lean premise completeness (2 pts): reserved for formalizing GF(2^m) and
   RS distance in Lean itself (removing the C++-checked hypothesis on hagree).
   Currently UNSCORED (0/2) — the premise rests on RS theory + machine checks.

## Demoted / informational
- Adversarial tail differentiation: NEGATIVE RESULT at SDR-typical sparsity —
  birthday tails do not bite within feasible M (empirical min-margin flat
  ~49-52 for both systems across M=10^3..10^6, cert floor 31). The adversarial
  advantage is a worst-case certificate, not an empirical differentiator.
  Reported as ADVSCALE info lines; carries no score weight.

## Anti-incentive guard
No scored component may reward baseline failure alone: axis C requires the
absolute NSSC served fraction; gates require baseline-agnostic properties.

## Honesty records
- capacity claims from CAP formula are analytic, not empirical storage demos
  (axis A exists to make the scale claim empirical).
- Phantom-flood regimes (toggling >=25% of ALL bits) are outside every
  decoder's reach and outside certification; reported when measured.
