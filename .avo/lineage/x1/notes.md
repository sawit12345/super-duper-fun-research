# x1 — NEBULA v1: certified sparse codes + whole-kernel verification

## Hypothesis
Kautz–Singleton structured sparse codes (RS over GF(2^m) + one-hot) with a
Lean-proved certification theorem beat flat SDRs on usable capacity and noise
robustness at matched (n, w), while running on pure bitwise kernels.

## What was built
- include/nebula/gf.hpp  — GF(2^m): shift/XOR peasant mul + exp/log LUT ops
- include/nebula/rs.hpp  — systematic RS encode (synthetic division),
                          syndromes, erasure decode via GF Gaussian elimination
                          + re-encode verification. Row stride padded to pow2.
- include/nebula/code.hpp— SDR baseline, NSSC binary space, xorshift128+ RNG,
                          exact fixed-count flip noise engine, mixed adversarial
                          attack, scan decoder (ties->unknown), algebraic decoder
- bench/selftest.cpp     — property gates incl. Ahmad-Hawkins FP cross-check
- bench/bench_all.cpp    — capacity/noise/throughput vs matched flat SDR
- scripts/check_kernel_asm.sh — disassembly purity audit (no mul/div/float)
- proofs/Nebula.lean     — 7 theorems, Mathlib, zero sorryAx

## Commands run + results
./evaluate.sh → SCORE: 8.73 / 10
  capacity=1.000 noise=0.683 throughput=1.000 lean=1.0
Key numbers (n=3840, w=60, q=64, K=30, e*=30):
- eta=20% uniform: NSSC recall=1.000 wrong=0 at M=10^5;
  SDR recall collapses 0.71→0.32, wrong up to 35% as M grows 10^3→10^5
- Mechanism: lambda_max = K-1 = 29 < theta = w/2 = 30 makes false matches
  structurally impossible for NSSC (matches Lean theorem exactly)
- Algebraic decode ~14us CONSTANT vs scan growing to 1104us at M=10^5 (77x)
- Certified analytic capacity: log10 M = (w-e*)·log10(q) = 54.2 at e*=30
- Purity audit: 5042 kernel instructions, 0 mul/div/float
- Lean: 7 theorems verified [propext, Classical.choice, Quot.sound] only

## Bugs found & fixed by selftest (record for posterity)
1. exp_t wraparound used mask instead of subtract (log-sum overflow)
2. mul_lut indexed raw table bypassing cyclic wrap
3. inv_lut out-of-bounds at a=1
4. RS encode LFSR tap-order ambiguity -> replaced with explicit synthetic division
5. Syndrome evaluation ascended powers instead of descending (high-first layout)
6. Generator roots alpha^0..D-1 vs syndrome points alpha^1..D mismatch
7. Adversarial attack needed MIXED t-off/f-on flips to strictly confuse (ties
   otherwise) — matches the Lean noise model exactly

## Verdict
COMMITTED. Strict improvement over baseline state (repo was empty).
