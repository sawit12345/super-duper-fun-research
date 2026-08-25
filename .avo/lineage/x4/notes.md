# x4 — nested category fields (graceful degradation ladder) + scorer on stable metrics

## Hypothesis
A nested repetition-coded category field (NC=16 blocks encoding msg[0]) gives
NSSC a degradation ladder exact-item -> category -> unknown that flat SDR
cannot express, while preserving the zero-wrong guarantee.

## Results (LADDER experiment, M=1e5, matched nested geometry n=4864/w=76)
flips | NSSC exact | NSSC cat-only | NSSC unk | NSSC wrong | SDR exact | SDR unk | SDR wrong
  20  | 193 | 7  | 0 | 0 | 0 | 200 | 0
  40  | 165 | 35 | 0 | 0 | 0 | 200 | 0
  60  | 40  | 166| 0 | 0 | 0 | 200 | 0
  80  | 0   | 200| 0 | 0 | 0 | 200 | 0
  100 | 0   | 199| 1 | 0 | 0 | 200 | 0
Zero wrong answers at EVERY level for NSSC; SDR has no intermediate level and
serves nothing. SCORE: 10.00 (capacity 1.0, noise 1.0, thr 1.0, lean 1.0).

## Critical bug found & fixed: RNG low-bit lattice
xorshift128+ low bits are weak; masking with non-pow2 n-1 sampled only 1024
distinct positions -> apply_uniform hung requesting more distinct flips than
reachable. Fixed with rand_below(): pow2-mask + over-range rejection, used in
all samplers (SdrSpace, NoiseEngine uniform+adversarial).

## Experiment-design lessons (recorded to avoid re-learning)
- Toggling % of ALL bits floods sparse codes with phantom ON-bits (eta=25% of
  n=4864 = 1216 toggles vs 76 true bits): information-theoretically hopeless
  for ANY decoder; ladder must use dropout-dominated absolute budgets.
- Matched-budget increases help SDR too: at w=76/n=4864 SDR survives eta=20%.
  Each comparison needs its own matched pair; UNI stays at sharp geometry
  (n=3840,w=60), LADDER at nested geometry.
- Beyond certified radius e*, phantom floods can cause wrong answers for NSSC
  too (3.3% at eta=30%,M=1e6) — certificate applies WITHIN e* only; reported
  honestly.

## Verdict
COMMITTED. All prior gates hold; new capability demonstrated; scorer moved to
stable metrics (ladder service + zero-wrong + certified formula).
