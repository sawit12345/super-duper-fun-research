# x2 — whole-brain loop demo integrated

## Hypothesis
The certified decoder is sufficient to build a whole-brain loop (beyond Monty's
neocortex-only scope) in which NO region can hallucinate: recall either returns
the exact stored item or reports unknown.

## Added
- include/nebula/brain.hpp — Thalamus (gated dual-route relay + efference-copy
  priming), Hippocampus (rotation-bound episodic index), BasalGanglia (WTA with
  ties->no-action), Cerebellum (supervised transition store), ReticularFormation
  (density regulation w/ guaranteed-progress decimation), Astrocyte (saturating
  plasticity window).
- bench/brain_demo.cpp — scripted scenario: 6 codes, 2 contexts, sequences
  X:A->B->C, Y:A->D->B.

## Results (all gates green; SCORE stays 8.73 — demo is a gate not a metric yet)
- Episodic recall 100/100 at cue noise 0,8,16,24,30 flips; ZERO hallucinations.
- Cerebellum predicts X:A -> B via certified transition lookup.
- Closed thalamic gate blocks payload without false assertion.
- BG selects RECALL under cortical affordance; reticular regulate terminates;
  astrocyte opens plasticity after novelty.

## Bugs fixed this round
- ReticularFormation::decimate could make no progress on even-lane bits ->
  infinite loop; replaced with guaranteed-progress highest-bit clearing.
- Demo originally algebraically decoded an OR of two codes (weight ~120 > D=30
  erasures) -> always failed; replaced with per-role certified identification.
- AND-tap cerebellar association cannot link disjoint sparse codes; replaced by
  rotation-bound transition store reusing the certified machinery.

## Verdict
COMMITTED. Adds whole-brain scope (user requirement) with all prior metrics held.
