# x5 — supervisor directives executed: adversarial negative result, scorer v2,
#       scale demo, binding benchmark

## Supervisor directives and outcomes
1. Adversarial-tail-at-scale: EXECUTED -> NEGATIVE RESULT recorded honestly.
   At SDR-typical sparsity (n=3840,w=60), empirical min-margin is FLAT
   (~49-52) across M=10^3..10^6 for BOTH systems (cert floor 31). Birthday
   tails do not bite at feasible M. H1 adversarial clause DEMOTED to
   theoretical guarantee; differentiation reframed on certificates+complexity.
   bench/advscale.cpp + ADVSCALE/MARGIN/ATTACK lines.
2. Scorer repaired (contract v2 in EVALUATION.md): saturated components became
   hard GATES (purity, selftest, brain demo, lean-no-sorryAx, ladder-zero-wrong);
   points moved to OPEN axes: A scale demo /3, B role binding /2, C ladder
   service /2 (absolute-performance term per anti-incentive guard), D
   throughput /1, E Lean GF-RS formalization /2 (open, 0). Score now < max
   with every deduction mapped to a named thread.
3. n=65536-class headline demo: bench/scale_demo.cpp — 10^5 codes at 7.8KB,
   built 1.2s, certified-radius dropout: recall 0.847, wrong 0, algebraic
   decode 843us vs full-scan-equivalent ~10.9ms (13x). Certified capacity
   log10M = 298.6 >> SDR's uncertified 10^84 narrative.
4. Role-binding benchmark: bench/bind_demo.cpp — K=1,2,4 roles in disjoint
   slices, zero wrong everywhere, crosstalk <=4 vs certified floors 29/14/6.
   NOTE: first attempt used bit-rotations for "disjointness" which violated
   the lemma hypothesis (rotations do not disjointify codes spanning all
   words); fixed by true bit-slices per role.

## Critical bug found & fixed: extract_block UB for q>64
`1ull << q` with q=256 is UB; block extraction silently returned garbage ->
all blocks erased -> scale demo recall 0. Replaced with general multi-word
block_info() scanner (any q, early-exit on ambiguity). Selftest re-run ALL PASS.

## Score: 7.54/10 (gates all pass)
AXES: A_scale=0.85/3 B_binding=1.00/2 C_ladder=1.00/2 D_throughput=1.00/1
E_lean_gfrs=0.00/2
Deductions are NAMED OPEN WORK: scale recall at boundary (A), GF/RS Lean
formalization (E).

## Verdict
COMMITTED. Metric-contract change isolated from capability claims per
supervisor rule; negative results recorded.
