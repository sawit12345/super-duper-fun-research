# NEBULA — Nested-code Efficient Brain-wide Universal Learning Architecture

Design v1 (post-research). Research notes: `research/notes/`. Papers: `research/papers/`.

## 1. What exists and where it stops

| System | Scope | Representation | Certified properties |
|---|---|---|---|
| HTM/Numenta SDR | neocortex | random k-of-n bit vector | none (probabilistic only) |
| Thousand Brains / Monty | neocortex + minimal thalamus | explicit graphs + evidence counts | none; SDRs abandoned |
| Kanerva HD/VSA | abstract | dense bipolar/binary hypervectors | SNR=√(D/m) statistical |
| **NEBULA (this)** | **whole brain loop** | **structured nested sparse codes** | **guaranteed pairwise separation + certified decode radius** |

SDR's headline "capacity 10^84" is the *space size* C(2048,40)=2.37×10^84 — the count
of representable vectors, with **zero guarantees** about distinguishing two of them
under noise. Numenta's own false-match math (Ahmad & Hawkins 2015, Eq. 4) shows the
usable dictionary under noise is union-bound limited: fp_X ≤ M·fp_xw(θ).

## 2. Core object: NSSC — Nested Superimposed Sparse Codes

### 2.1 Identity core (certified layer)
Kautz–Singleton construction over GF(2^m):
- Outer: Reed–Solomon [N, k']_q with q = 2^m, M = q^{k'} codewords, min symbol distance D = N−k'+1.
- Inner: one-hot map GF(q) → q-bit block; concatenate N blocks.
- Result: binary length n = N·q, weight w = N, sparsity 1/q.
- **Pairwise intersection λ_max = k'−1** (agreements in outer coords) — *worst case over all M² pairs*, not average.
- **Certified decode radius**: query corrupted by e bit flips (t on→off, f off→on, e=t+f)
  has overlap ≥ w−t with the true code and ≤ k'−1+f with any rival.
  Unique identification guaranteed iff w−t > k'−1+f ⟺ **e ≤ w−k'**.
- Therefore **M = q^{w−e\*}**: every extra certified flip of tolerance costs log_q(M).
  Capacity and robustness are one dial, not a vague hope.

Concrete headline: n = 65536 bits (8 KB), q = 64, w = 1024, e* = 500 flips tolerated
→ M = 64^501 ≈ 10^903 **certified-separable** memories. The same 8 KB of flat-SDR space
(C(65536,1024) ≈ 10^2286) carries no such certificate — its usable dictionary is
union-bounded by the hypergeometric tail exactly like Ahmad–Hawkins Eq. 4.

GF(2^m) arithmetic needs only shift/XOR (+LUT for inverse/div): RS encode is an LFSR;
decode is syndrome → Chien search → Forney. **No multiply, no divide, no float anywhere.**

### 2.2 Nested refinement fields (the new part — dendritic analogy)
The identity core is the *proximal* field. Add refinement fields bound to it:
- **distal/lateral field**: context SDR XOR-composed via rotation (role binding);
- **apical/top-down field**: prediction from higher level, validated independently.

Consistency rule: a full match requires identity-core certification AND per-field
thresholds. Because fields are *bound* (rotated into disjoint bit ranges), noise in
one field cannot fake evidence in another. Degradation ladder:
`exact item → coarse item → category (core still certified) → gate reports UNKNOWN`.
A flat SDR degrades to *confidently wrong*; NEBULA degrades to *less precise but
never falsely certain*. This mirrors pyramidal compartment integration
(Gidon 2020, Poirazi & Papoutsi 2020) and hippocampal indexing (Mackay 2024).

### 2.3 Whole-brain loop (beyond neocortex — Monty's blind spot)
| Region | Primitive (all bitwise) | Neuroscience anchor |
|---|---|---|
| Thalamus | routing masks AND-ed into streams; driver vs modulator dual route | Sherman & Usrey 2024 transthalamic pathways |
| Hippocampus | episodic index: bind(role=place, filler=code), cleanup via certified decoder | Whittington TEM 2020; Mackay 2024 |
| Basal ganglia | winner-take-all popcount tournament over action codes | Doya 2000 |
| Cerebellum | shift-register delay lines; supervised timing offsets | Doya 2000 |
| Reticular/RF | global gain mask, density homeostasis (target sparsity) | area reticularis gain control |
| Astrocyte meta | slow saturating counters gate plasticity windows | Rupprecht 2024; Gong 2024 |

## 3. Efficiency contract (hard constraints)
- Kernel: `uint64_t` words only; ops = AND/OR/XOR/NOT/shifts/rotates/popcount/add/sub/cmp.
- **No `*`, `/`, `%`, no floats in kernel paths.** Verified post-link by disassembly check
  (`scripts/check_kernel_asm.sh`: no mul/div/ss/sd instructions in `nebula::*` symbols).
- Dataflow: SoA word layout, alignas(64), branchless hot loops, zero heap alloc after init,
  single-pass streaming encode, algebraic decode O(N·m) instead of O(M·n/64) scan.

## 4. Evaluation contract (evaluate.sh)
Baseline: honest random SDR, matched n and w, θ=w/2 (Numenta operating point),
Numenta FP formulas as cross-check of harness.
Metrics: recall, false-match rate, max-M at (fp≤1e−6 ∧ recall≥0.99) under
random η=10% and adversarial noise; decode throughput vs scan; graceful-degradation
curve (coarse recall vs noise). Lean score: proofs compile.

## 5. Proof plan (Lean 4)
- P1 (unconditional): if encoder family has pairwise agreement ≤ A then any decoder
  that picks unique max-overlap is correct for e ≤ w−A−1. (Pure combinatorics.)
- P2: KS/RS family realizes agreement ≤ k'−1 (conditional on RS distance; RS distance
  itself machine-checked exhaustively for deployed params by C++ self-test + cited theory).
- P3: rotation binding injectivity for disjoint offset fields.
- P4: packing bound M ≤ n(n−1)/(λ+1)(...) style counting lemma.
