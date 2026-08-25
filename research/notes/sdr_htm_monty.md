# SDR / HTM / Monty — Distilled Research Notes
Compiled 2026-08-24 (research pass for FSC project; goal: beat flat SDR on capacity + noise robustness).
All formulas verified against primary PDFs in `research/papers/`; combinatorics re-computed exactly with Python (`math.comb`).

---

## 1. SDR capacity: the 10^84 claim, derived

**Primary source:** Ahmad & Hawkins 2015, "Properties of Sparse Distributed Representations and their Application to Hierarchical Temporal Memory", arXiv:1503.07469.

An SDR is a binary vector of length `n` with exactly `w` ON bits (sparsity `s = w/n`). The number of distinct SDRs is the binomial coefficient C(n,w).

**Verbatim (paper, Appendix A Table 1 discussion):**
> "With more typical values n = 2048 and w = 40, the SDR representation space is astronomically large at **2.37×10^84** encodings; the estimated number of atoms in the observable universe is ~10^80."

**Exact verification (computed):**
```
C(2048, 40) = 2371778511645358086693262639700863268089720614584700731712608317643033681970419921664
            ≈ 2.37178×10^84   (85-digit integer; log10 = 84.375)
```
So "10^84 capacity" is literally the count of representable states of one 2048-bit vector at 40-ON sparsity — NOT the number of items storable in memory. Related counts from same paper: C(1024,2)=523,776 encodings; C(40,4)=91,390; log10 C(1024,20)=41.74; log10 C(512,10)=20.50.

Probability two random SDRs are identical (paper Eq. 2): `P(x=y) = 1/C(n,w)`.

## 2. Noise robustness math (Ahmad & Hawkins 2015)

### 2.1 Definitions (verbatim math)
- Overlap: `overlap(x,y) ≡ |x ∧ y|` (popcount of bitwise AND)
- Match rule: `match(x,y) ≡ overlap(x,y) ≥ θ` ("Typically θ is set such that θ ≤ w_x and θ ≤ w_y")
- Overlap set Ω_b(n,w,b) = set of n-bit vectors with w bits ON having exactly b bits overlapping x. Cardinality:
  `Ω_b(n,w,b) = C(w,b) × C(n−w, w−b)`   (Eq. 3)

### 2.2 False-match probability between random SDRs (THE formula)
Paper Eq. (4):
```
fp_xw(θ) = Σ_{b=θ}^{w} |Ω_b(n,w,b)| / C(n,w)
         = Σ_{b=θ}^{w} [C(w,b)·C(n−w,w−b)] / C(n,w)      (hypergeometric tail)
```
Single-term approximation, paper Eq. (5), valid whenever **w > 7 and θ > w²/n**:
```
fp_xw(θ) ≈ |Ω_θ(n,w,θ)| / C(n,w)  =  C(w,θ)·C(n−w,w−θ)/C(n,w)
```
The threshold condition θ > k²/n (= mean random overlap) is what makes false matches decay exponentially with w.

**Exact numeric tail for n=2048, w=40 (mean random overlap w²/n = 1600/2048 = 0.781):**

| θ | P(overlap ≥ θ) | ≈1-in |
|---|---|---|
| 13 | 6.00×10⁻¹⁴ | 1.7×10¹³ |
| 14 | 1.57×10⁻¹⁵ | 6.4×10¹⁴ |
| 15 | 3.56×10⁻¹⁷ | 2.8×10¹⁶ |
| 16 | 6.99×10⁻¹⁹ | 1.4×10¹⁸ |
| 18 | 1.76×10⁻²² | 5.7×10²¹ |
| 21 | 2.38×10⁻²⁸ | 4.2×10²⁷ |
| 24 | 8.38×10⁻³⁵ | 1.2×10³⁴ |

### 2.3 Noise tolerance (false negatives)
Verbatim: "With w = 40 and θ lowered to 20, the noise can change **50% of the ON bits** and still match x to x′." Rule: corrupted copy with t flipped/lost ON bits still matches iff θ ≤ w − t; no false negatives by construction, only false positives.

### 2.4 Subsampling (Eqs. 6–7)
For subsample x′ with w′′ ≤ w′ ON bits: `Ω_b′′(n,w′,b) = C(w′′,b) × C(n−w′′, w′−b)`; false-positive chance `fp(θ) = Σ_{b=θ}^{min} .../C(n,w′)`.
Worked numbers: n=2048, w′=40, w′′=20, θ=10 → false positive "better than 1 in 10^[large]" (threshold only 25% of original ON count).

### 2.5 Classification of M stored vectors (Eqs. 8–11)
Union bound: `fp_X(θ) ≤ M · fp_xw(θ)`. Paper Table 3 examples (n=1024, w=21):
- θ=14, M=10 → 8.83×10⁻²¹
- θ=17, M=10⁹ → 9.58×10⁻²¹
- θ=21(exact), M=10⁹ → 3.82×10⁻³⁵
→ "a billion vectors" classifiable with FP rate < 1e-20 at 2/3 threshold.

### 2.6 The union property (OR superposition; Eqs. 12–13, Bloom-filter derivation)
Store M SDRs as X = OR(x₁..x_M). Probability a given bit is ON after M unions:
`P(bit ON) = 1 − (1 − w/n)^M`
False positive for new random y (all its w bits happen to be ON in X), Eq. 13:
```
p_fp = [1 − (1 − w/n)^M]^w
```
Expected ON bits in union: n·[1 − (1−s)^M], grows sublinearly.
Worked numbers (paper): n=1024: w=2, M=20 → FP ≈ 1-in-680; w=20, M=20 → **1-in-5.5 billion**; M=80 still leaves ~20% bits OFF. No risk of false negatives ever.
Table 4 (union + inexact match), key rows: n=8192 (=1024 cols × 8 cells), w=20, t(threshold)=18, M=60 → 4.61×10⁻¹⁴; n=65536 (=2048×32) rows continue to even lower rates. This is the math behind Temporal Memory's union-of-predictions.

## 3. HTM Spatial Pooler (Cui, Ahmad & Hawkins 2017, Frontiers Comput. Neurosci. 11:111)

- Mini-columns compete via local inhibition → k-winners-take-all; output is an SDR (~2% active).
- Synaptic permanences ∈ [0,1]; binary weight = permanence > connected threshold. Parameters used in paper's experiments: **connected permanence θ_c = 0.5, increment p+ = 0.1, decrement p− = 0.02**, boosting strength β = 100, duty-cycle period T = 1000.
- Homeostatic boosting: `b_i = e^(−β(ā_i(t) − ⟨ā_i(t)⟩))` (Eq. 10) — excites under-active mini-columns; maximizes coding entropy.

## 4. Temporal Memory / sequence memory (Hawkins & Ahmad 2016, arXiv:1511.00083; Frontiers Neural Circuits 2017 companion)

**Neuron model:** pyramidal cell = soma + 3 dendritic zones: proximal (feedforward RF, drives somatic spike), basal (lateral/context → prediction), apical (top-down feedback → Ca²⁺ plateau, modulates learning). Each dendritic segment is a coincidence detector: fires an **NMDA spike** when ≥ θ of its s synapses are simultaneously active; depolarizes soma sub-threshold = **predictive state**.

**Network (paper's simulation):** 2048 mini-columns × 32 cells/column = 65,536 cells; each cell has **128 basal segments, up to 40 synapses/segment**; HTM neuron model uses 128 detectors total. Column activates if proximal match wins inhibition (~40 active columns of 2048, i.e. 2%); if a column was not predicted → **bursting** (all 32 cells fire = surprise/novel context); if predicted → only the predictive cell fires (context preserved).

**Segment false-match probability (paper Eq. 1)** — same hypergeometric family as SDR Eq. 4:
```
P_false(segment) = Σ_{b=θ}^{s} [C(s,b)·C(n−s,a−b)] / C(n,a)
```
where a = #active cells in population n, s = synapses on segment, θ = NMDA threshold. Example verbatim: NMDA threshold 10 with 20 grown synapses (2× over-spec) tolerates 50% noise, error only **1.6×10⁻¹⁸**; co-located synapses within 40µm suffice (as few as eight).

**Learning:** Hebbian on permanence (grow/strengthen active synapses on near-winning segment, punish others); binary connection threshold "such as 0.3" (this paper; SP uses 0.5). Prediction-before-event enables one-shot-ish sequence learning without backprop.

**Capacity (verbatim):** transitions storable = (cells-per-column / column-sparsity) × patterns-per-cell = **(32/0.02) × 200 = 320,000 transitions**; scales linearly. Distinct temporal representations of one input: 32^40 (one active cell choice per each of 40 columns).

**Predictive state = union property:** next-step predictions are OR-ed into one union vector; bounded by §2.6 math (that's why Table 4 uses n=65536).

## 5. Thousand Brains Project / Monty (status 2024–2026)

**Sources:** whitepaper Clay, Leadholm & Hawkins 2024, arXiv:2412.18354 ("The Thousand Brains Project: A New Paradigm for Sensorimotor Intelligence"); Leadholm et al., "Thousand-Brains Systems: Sensorimotor Intelligence...", arXiv:2507.04494, Neural Computation 38(6):845–896 (2026); docs.thousandbrains.org; github.com/thousandbrainsproject/tbp.monty.

**Status/timeline:**
- Launched from Numenta ~2024 as **independent non-profit, partially funded by the Gates Foundation**. Named after Mountcastle.
- `tbp.monty`: open-source (MIT), Python/NumPy-only (contributions explicitly must avoid PyTorch/DL); self-described **"early beta", major version zero, unstable API, research project not platform**.
- 2026: first quantitative evaluation (YCB household objects, 3D object recognition + pose estimation); claims rapid continual learning, sample/compute efficiency vs ViT/deep-learning baselines, symmetry detection, voting speedups.
- Active 2026 roadmap: hierarchy, more modalities, goal generation, open questions published; FAQ/roadmap/application-criteria docs maintained.

**Architecture:**
- **Sensor modules (SMs)** → stream **features-at-pose** (feature ID + location + orientation relative to body).
- **Learning modules (LMs)** = replicated cortical-column-like units; each learns/matches object models in reference frames via sensorimotor movement displacements; contains short-term buffer + long-term graph/model memory; outputs object ID + pose upward (same format = compositional hierarchy). Implementations: DisplacementGraphLM, FeatureGraphLM, **EvidenceGraphLM (default)** — continuous evidence accumulation per hypothesis, robust to noisy observations vs earlier binary elimination.
- **Voting / Cortical Messaging Protocol (CMP):** LMs laterally exchange hypotheses (unions of CMP messages) to reach consensus fast; inspired by Hawkins/Ahmad/Cui 2017 voting.
- **Motor system:** model-based policies from LMs' goal generators + model-free policies (subcortical-analog); movement is essential — system learns only through sensorimotor interaction, never static datasets.
- Learning is associative/Hebbian binding, **no backprop, no weights in the DL sense**, rapid one-shot-ish model updates.

**What it does NOT cover (verified against full text of whitepaper):**
- Zero mentions of: thalamus, basal ganglia, cerebellum, hippocampus, amygdala, brainstem, spinal cord. "Subcortical" appears only twice, both about model-free motor policies being *analogous* to subcortical action selection — no mechanisms modeled.
- Explicitly deferred low-level neuroscience components (FAQ verbatim): "**HTM, sparse distributed representations (SDRs), grid-cells, and active dendrites**" — Monty operates above them with explicit graphs/evidence tables.
- Neocortex-only theory (Thousand Brains Theory of neocortex); whole-brain loops (thalamic gating, BG action selection, cerebellar timing, hippocampal episodic indexing) are out of scope.
- Not applicable to static-dataset learning (no sensorimotor loop → no learning); language/abstract reasoning and large-scale hierarchy remain open roadmap items; no stable API.

**Implication for our framework (FSC):** the flagship successor to HTM abandoned SDRs rather than fixing their limits; nobody has beaten flat-SDR capacity/noise math at matched (n,k). Multi-field structured codes attack exactly the unclaimed gap between SDR combinatorics (§1–2) and Monty's symbolic graphs.

## 6. Downloaded papers (paths)

| File | Paper |
|---|---|
| `research/papers/sdr_properties_ahmad_hawkins_2015.pdf` | Ahmad & Hawkins 2015, Properties of SDRs (arXiv:1503.07469) |
| `research/papers/thousands_synapses_hawkins_ahmad_2016.pdf` | Hawkins & Ahmad 2016, Why Neurons Have Thousands of Synapses (arXiv:1511.00083) |
| `research/papers/htm_spatial_pooler_cui_ahmad_hawkins_2017.pdf` | Cui, Ahmad & Hawkins 2017, The HTM Spatial Pooler (Frontiers CN 11:111) |
| `research/papers/columns_learn_world_structure_hawkins_2017.pdf` | Hawkins et al. 2017, Columns learn structure of the world (Frontiers Neural Circuits 11:81) |
| `research/papers/tbp_whitepaper_clay_leadholm_hawkins_2024.pdf` | TBP whitepaper (arXiv:2412.18354) |
| `research/papers/monty_sensorimotor_intelligence_tbp_2026.pdf` | Leadholm et al. 2026, Thousand-Brains Systems (arXiv:2507.04494) |

(Pre-existing / concurrently-added from the broader research pass, related: kleyko2023_vsa_survey_part{1,2}.pdf, clarkson2023_vsa_capacity.pdf, frady/kent resonator networks, chavlis_poirazi2024 dendrites, usrey_sherman2024 transthalamic, larkum/leugering plateaus, kogler/mackay episodic indexing, kanerva2009 HDC, doya2000 cerebellum/BG.)
