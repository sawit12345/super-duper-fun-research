# VSA / Coding Theory Research Notes — Capacity & Noise Robustness for a New Sparse Distributed Representation

Date: 2026-08-24. Research-only (no src/ changes). Sources: arXiv PDFs in `research/papers/` (list at bottom) plus verified web sources (Wikipedia Bloom filter page, Bonomi et al. dlCBF, Inan et al. group testing, Liu–Shangguan Steiner/packing paper snippets).

Legend for implementability flags:
- **[BITWISE]** = implementable with ONLY bitwise ops (+add/sub/shift, no multiply/divide/float).
- **[INT]** = integer arithmetic OK (adds/multiplies), no float.
- **[FLOAT/HW]** = needs multiply/divide/trig/float (or expensive LUT emulation).

---

## 1. Collision math for k-of-n codes (hypergeometric)

Two **independent uniform k-subsets** A, B ⊆ [n]:

```
P(|A ∩ B| = j) = C(k, j) · C(n-k, k-j) / C(n, k),      j = 0..k      (hypergeometric)
E|A ∩ B|       = k² / n
Var|A ∩ B|     = k · (n-k)/n · (n-k)/(n-1)?  -> Var = k·(k/n)·((n-k)/n)·((n-k)/(n-1))
P(|A ∩ B| ≥ 1) = 1 − C(n-k, k) / C(n, k)                              ("collision")
```

Tail form used for false matches (union bound over M−1 non-target codewords, query lost e of its k ones):

```
P_false ≤ (M−1) · P(|A∩B| ≥ k−e)
        = (M−1) · Σ_{j=k−e}^{k} C(k,j)·C(n−k, k−j) / C(n, k)
```

Deterministic guarantee: if the family has **max pairwise intersection λ** and the query loses e < k − λ ones (and gains none), **zero** false matches are possible (nearest codeword still uniquely closest). This is exactly the d-disjunct condition of superimposed-code theory (see Kautz–Singleton below).

Dense (non-sparse) analogue: random codebook {c_i} ⊂ {0,1}^n, query y = c_1 with e bit flips, accept within radius τ:

```
P_false ≤ (M−1) · Σ_{i=0}^{τ} C(n,i) / 2^n        (union bound; each other codeword uniform on {0,1}^n)
```

**Design note:** evaluate these offline in float; at RUNTIME the decision rule "match ⟺ overlap ≥ T" is an integer threshold — fully bitwise-friendly.

---

## 2. VSA / hyperdimensional computing capacity

### 2.1 Kanerva 2009 (`kanerva2009_hyperdimensional_computing.pdf`)
- Space: 10,000-bit binary HVs; Hamming distance `d(A,B) = |A ⊕ B|` (verbatim notation from the paper: "the number of 1s in it is the Hamming distance between the two vectors").
- Distances concentrate: mean D/2, sd √D/2 (binomial). "Private neighborhood": distances up to ~D/3 (~3,333 bits for D=10,000) are effectively unique neighborhoods; two meaningful vectors are similar iff their distance is "considerably smaller than 0.5" (normalized).
- Multiplication (XOR) preserves distance ⇒ preserves noise; permutations likewise distance-preserving.
- Superposition (sum + threshold/majority) is similar to every added vector; robustness comes from holographic distribution, not replication ("much better ways than triple-modular redundancy").

### 2.2 Superposition capacity (signal-detection form; Frady–Kleyko–Sommer 2018 theory, restated in Kent et al. 2020 / Kleyko surveys)
Bundle m bipolar HVs x_i ∈ {±1}^D: s = Σ x_i. Inner product with target:
```
⟨s, x_j⟩ ~ Gauss(mean = D, var = D(m−1))     ⇒     SNR = sqrt(D/(m−1))
per-bit error after majority/sign cleanup:    p_e = Q( SNR ) = Q( sqrt(D/m) )
Capacity at fidelity ε:  m* ≈ D / z(ε)² ,  z(ε)=Φ⁻¹(1−ε)      ⇒ LINEAR in D
```
Classical associative-memory companion numbers: Hopfield Hebbian capacity α_c ≈ 0.138·D patterns; simple superposition memory ~ D/(2 ln D) retrievable items with vanishing error (Amit–Gutfreund–Sompolinsky line, cited in the resonator papers).

### 2.3 Rigorous finite-D bounds — Clarkson, Ubaru, Yang 2023 (`clarkson2023_vsa_capacity.pdf`, arXiv:2301.10352)
Analyzes **MAP-I, MAP-B and two sparse-binary VSAs** as sketching/JL transforms. Verbatim-grade statements extracted:
- Set-intersection estimation and membership testing get dimension bounds m = O(poly(log d, 1/ε)) via sign-matrix JL structure.
- Theorem 10: for scaled sign matrix S̄, rotation R, sequences v(0..L−1) ∈ {0,1}^d with K = ‖Σ v(j)‖∞: `m = O(K² ε⁻² log(K/(εδ)))` gives ‖S̄R v‖ ≈ ‖v‖.
- Theorem 12 (bundles of 2-bindings, v ∈ {0,1}^(d²)): `m = O(ε⁻² log³(‖v‖₁/εδ))`; generalized Corollary 13 for all k with log^k terms.
- Rotation-based sequence storage: error bound grows **quadratically in length L** (they conjecture better bounds exist).
- Also analyzes a "Hopfield±" novel variant vs classical Hopfield nets.

### 2.4 Resonator networks — Frady/Kent/Olshausen/Sommer 2020 (`frady2020_resonator_networks_1.pdf`, `kent2020_resonator_networks_2.pdf`)
Factorize composite s = x*∘y*∘z* from codebooks X,Y,Z. Dynamics (paper eq. 3.2, verbatim):
```
x̂(t+1) = g( X Xᵀ ( s ∘ ŷ(t) ∘ ẑ(t)) ),  similarly for ŷ, ẑ;  g thresholds elements to ±1
```
- Solutions emerge as stable fixed points; no Lyapunov function guaranteed, but empirically reliable inside capacity.
- **Operational capacity M_max scales QUADRATICALLY in dimension N** (measured for F = 2..7 factors, p=0.99 success, iteration budget 0.001M); beats ALS/PGD/gradient methods by ~2 orders of magnitude.
- Failure mode of competitors: spurious fixed points ("percolated noise" makes resonators less locally stable than Hopfield but better globally).
- Implication for our project: any replacement representation must keep a **superposition-search decoder** viable; quadratic-in-N decode capacity is the bar.

### 2.5 Surveys
- Kleyko, Rachkovskij, Osipov, Rahimi, ACM Computing Surveys 2023, Parts I & II (arXiv:2111.06077, arXiv:2112.15424). Taxonomy: TPR, HRR, MBAT, FHRR, BSC, MAP, SBDR, sparse block-codes, MCR, GAHRR. Key facts: quasi-orthogonality/concentration-of-measure; capacity increases roughly linearly with D and is model-insensitive (Part II §cleanup discussion); clean-up memory = Hopfield-style projection.
- Kleyko et al. 2022 "VSA as a computing framework for emerging hardware" (Proc IEEE; arXiv:2106.05268): Turing-completeness sketches, permutation-based sequence encoding, superposition computing.
- Thomas et al. 2021 (`thomas2021_theoretical_perspective_hd.pdf`): HD classification as kernel machines; noise robustness emerges from architecture.
- Frady, Kleyko, Kymn, Olshausen, Sommer 2021 (`frady2021_computing_functions_fpe.pdf`, arXiv:2109.03429): Vector Function Architectures; **fractional power encoding (FPE)** z^(i): self-binding exponentiation; uniformly-sampled base ⇒ **sinc kernel**, RKHS of band-limited functions; base-vector sampling distribution shapes the kernel; decoding/denoising of continuous values.
- Follow-up: "Improved Cleanup and Decoding of Fractional Power Encodings" (arXiv:2412.00488): composite-likelihood + ML optimization beats denoising autoencoders/resonator/grid search for FHRR continuous-value cleanup.

---

## 3. Constant-weight binary codes / superimposed codes

### 3.1 Packing ↔ code equivalence (Liu & Shangguan 2024, `liu2024_partial_steiner_constant_weight_codes.pdf`, arXiv:2401.00733) — verbatim
> "P ⊆ [n]^w is called an (n, w, t)-packing if for every distinct A, B ∈ P, |A ∩ B| < t. … By a double counting argument … P(n,w,t) ≤ C(n,t)/C(w,t)."
> "P ⊆ [n]^w is an (n,w,t)-packing if and only if σ(P) … is an (n, 2w−2t+2, w)_2-CWC", i.e. `A₂(n, d, w) = P(n, w, (2w−d+2)/2)`.

For **pairwise intersection ≤ 1** (t = 2):
```
max family size  P(n, k, 2)  ≤  C(n,2)/C(k,2) = n(n−1) / (k(k−1))        (packing/counting bound)
Rödl 1985 (nibble):  P(n,k,t) ≥ (1−o(1)) · C(n,t)/C(w,t)                 (asymptotically tight; Erdős–Hanani conjecture resolved)
Keevash 2014:        exact Steiner systems S(t,k,n) exist for admissible n (divisibility)
Liu–Shangguan 2024:  near-Johnson-optimal constant-weight/composition codes for fixed odd d,
                     incl. q-ary generalization A_q(n,d,w) ≥ (1−o(1))(q−1)^t · n^t/w^t,  t=(2w−d+1)/2
```
So families of k-subsets of [n] with **all pairwise overlaps ≤ 1 can have ≈ n²/k² members** — this is the theoretical ceiling for "collision-free-at-depth-1" sparse SDR codebooks.

### 3.2 Johnson bounds (classical, dense regime)
```
Recursive:   A₂(n, d, w) ≤ ⌊ (n/w) · A₂(n−1, d, w−1) ⌋
Closed form: A₂(n, 2δ, w) ≤ ⌊ δ·n / (w² − wn + δ·n) ⌋        (when denominator > 0)
```
Regime caveat: Johnson bounds are tight-ish for **dense** constant-weight codes (w proportional to n); for fixed w and n→∞ the packing/Rödl asymptotics n²/k² govern. Use the right one for your parameter regime. (See also Sidon-set-in-F₂^t tables: Aims AMC 2023 paper "Sidon sets, sum-free sets and linear codes" links max Sidon sets in F₂^t ↔ linear codes with d ≥ 5, sharpened Johnson bound.)

### 3.3 Kautz–Singleton (1964) construction
RS [q, k]_q code (length q, dimension k, distance q−k+1), each symbol mapped to unit vector of length q ("identity code"):
```
length n = q²,  weight w = q,  #codewords M = q^k,
pairwise intersection λ_max = k − 1  (= MDS ⇒ meets it with equality),
min distance d_min = 2(q−k+1),
d-disjunct ⇔ λ_max·d + 1 ≤ w_min  (lemma verbatim from Inan–Kairouz–Özgür arXiv:1711.05403 / 1808.01457)
```
- Choosing **k = 2**: M = q² codewords of weight q on n = q² coordinates with pairwise intersection ≤ 1 ⇒ a **partial Steiner system S(2, q, q²)**; compare packing bound q²(q²−1)/(q(q−1)) = q(q+1) ≈ M. KS@k=2 essentially MEETS the packing bound.
- Group-testing optimality: Bonomi-era result — Inan et al. (arXiv:1808.01457): KS is Θ(d log N)-optimal in *probabilistic* group testing (noiseless AND noisy), first strongly explicit such construction; suboptimal (O(d² log²d · logN)) for combinatorial GT at moderate d (Porat–Rothschild better there).
- **[BITWISE-ish]** RS over GF(2^m): encoder = shift-register XOR feedback (LFSR); GF(2^m) multiply = shift+xor loops (no hardware clmul required); syndrome/BM decoding also xor/shift. Generation-time discrete-log/exp tables for Sidon variants are offline.

### 3.4 Sidon / Bose–Chowla
```
Bose–Chowla theorem (h ≥ 2, q prime power): exists A ⊆ Z_{q^h − 1}, |A| = q,
with all h-term sums (repetition allowed) distinct mod q^h − 1   (B_h set).
h = 2:  Sidon set — all pairwise sums a_i + a_j distinct ⇔ all differences distinct up to sign
        ⇒ cyclic perfect-difference-type structure.
Counting ceiling: F_h(N)·h! ≤ ... ⇒ limsup F_h(N)/N^{1/h} < ∞;  BC gives liminf > 0 (order-optimal density q ≈ √v for h=2).
```
Use: a Sidon set of size q yields a **weight-q cyclic difference family** whose translates give pairwise-overlap-controlled codewords; runtime membership test = table lookup/add mod n ⇒ **[BITWISE]** after one-time generation (generation needs GF(q^h) exponentials — offline).

---

## 4. Multi-resolution / graceful-degradation codes

No canonical "multi-resolution SDR" paper exists yet (opportunity!). Nearest literature anchors:

1. **Successive refinement (source coding)** — Equitz & Cover, IEEE Trans-IT 1991: rate pairs (R₁,R₁+R₂) achievable with embedded codebooks iff distortion conditions; the formal theory of "coarse layer decodes first, refinements degrade gracefully." Embedded codes = progressive transmission backbone (SPIHT; EBCOT/JPEG2000 bit-plane embedding — practical instantiations).
2. **UEP / rate-compatible / rateless**: unequal-error-protection codes assign more protection to coarser layers; LT/Raptor fountain codes give incremental recovery as more symbols arrive — integer-friendly (XOR-only!) — **[BITWISE]**.
3. **VSA-side granularity**:
   - FPE (Frady et al. 2021): sinc-shaped similarity kernel ⇒ similarity peaks are naturally **multi-scale** (main lobe fine resolution, side lobes coarse phase info); hierarchical decoding demonstrated (also Renner et al. 2022 hierarchical resonators for translation×rotation factorization).
   - Permutation/rotation binding gives shift-equivariance: coarse alignment = whole-sequence rotation probe before fine symbol probing (Kleyko survey Part II spell-checking/alignment examples).
   - Clarkson et al. 2023: rotation-bound sequence error grows quadratically in L — hierarchy depth costs accuracy; sparse-binary VSAs analyzed separately.
4. Design hint for project: layered constant-weight codes where level-ℓ codewords are unions of level-(ℓ+1) atoms chosen from packings with intersection ≤ 1 give provable "graceful degradation ladder": losing e atoms drops you at most one granularity level if e < k − λ (see §1 deterministic guarantee).

---

## 5. Bloom-filter family (comparison baseline)

Verbatim standard results (Wikipedia/Guava/Bonomi et al.):

```
Standard BF (m bits, n items, k hashes):
  ε = (1 − [1 − 1/m]^{kn})^k  ≈  (1 − e^{−kn/m})^k
  optimal k = (m/n)·ln2
  ε_opt = (1/2)^k = 0.6185^{m/n}
  m = −n·ln ε / (ln 2)²          (≈ 1.44 n log₂(1/ε) bits; ≈ 9.6 bits/item @ 1% FPP)
  rigorous (Goel–Gupta):  ε ≤ (1 − e^{−k(n+0.5)/(m−1)})^k
Counting BF (Fan et al. 2000): 4-bit counters, ~4× space; overflow handling = saturate.
d-left CBF (Bonomi–Mitzenmacher–Panigrahy–Singh–Varghese, ESA 2006):
  FPP = 1 − (1 − 1/(BR))^{|S|} ≈ m/(BR);  bits/entry = (4·log₂(1/f) + 20 + 4·log₂3)/3;
  ~half the space of standard CBF at equal FPP.
Cuckoo filter (Fan et al. 2014): f-bit fingerprints, 2 candidate buckets, alternate index
  i₂ = i₁ ⊕ h(fingerprint)   (pure XOR);
  space ≈ (log₂(1/ε) + log₂ 2b)/α bits/entry; best for FPP ≤ ~3%; supports deletion.
Comparison cheat-sheet: BF 1.44·log₂(1/ε) b/elem; cuckoo ≈ 1.05·(log₂(1/ε)+log₂2b); CBF ×c counters.
```

**All four are integer-only:** BF **[BITWISE]** (set/check bits; k hash functions derivable by double hashing h₁+i·h₂), CBF **[INT]** (saturating add/sub), dlCBF **[INT]**, cuckoo **[BITWISE]** (truncate hash for fingerprint, XOR for alternate bucket). Note the structural analogy: a Bloom filter IS a dense superimposed code with random weight-k rows; our constant-weight-with-packing-design replaces random collisions with bounded ones.

---

## 6. Integer-only / bitwise-friendly algorithm inventory

| Construction / operation | Ops needed | Flag |
|---|---|---|
| XOR binding (BSC/MAP-B), unbinding = re-XOR | XOR | **[BITWISE]** |
| Permutation binding (rotation of coordinates, "permutation coding") | barrel shift / rotate | **[BITWISE]** |
| Bundling of bipolar HVs (majority) | POPCNT + threshold | **[BITWISE]** |
| Similarity of binary HVs | POPCNT(a XOR b) | **[BITWISE]** |
| Sparse-binary bundling (OR) + context-dependent thinning (AND/OR masks, Rachkovskij SBDR) | OR/AND | **[BITWISE]** |
| MBAT with {0,±1} matrices | shift + add/sub accumulate | **[BITWISE]** (weights stored unary/one-hot) |
| Bloom filter / cuckoo filter insert-query | hash (xorshift/multiply-free mixers ok), set/test, XOR | **[BITWISE]** |
| Counting BF / dlCBF | saturating counters (4-bit) add/sub | **[INT]** |
| Kautz–Singleton encode/decode (RS over GF(2^m)) | LFSR shift-XOR; GF mul = shift+xor loop | **[BITWISE]** (slower than table GF mul but legal) |
| Sidon/Bose–Chowla runtime membership | precomputed log/exp table + add mod n | **[BITWISE]** (tables built offline) |
| Resonator network, binary variant (XOR bind + popcount cleanup, sign update g) | XOR + POPCNT | **[BITWISE]** (avoid OLS-weighted variant — that needs multiplies; use outer-product/binary-weight variant) |
| Hypergeometric/tail probabilities for threshold selection | float/gamma | offline only — freeze integer thresholds T |
| FHRR / fractional power encoding (phasors, z^x) | complex multiply, exp/log, CORDIC/LUT | **[FLOAT/HW]** |
| Circular convolution (HRR binding) | FFT or O(D²) int convolution | **[FLOAT/HW]** (heavy even in ints) |
| Fractional powers / Gaussian-like kernels | exp/log | **[FLOAT/HW]** |

Rotation-as-binding detail (Kleyko survey Part I §3.x + Proc-IEEE 2022): position i encoded as ρ^i(c) (i-fold cyclic shift); sequence = bundle_i ρ^i(c_i); probing position i = apply ρ^{−i}; whole-sequence shift = single rotation of the bundle. All shifts ⇒ **[BITWISE]**. Fractional-power alternative without multiplication does not exist in phasor form; closest bitwise analogues are (a) multi-resolution rotation ladders (ρ^{2^j}) and (b) sparse-block-code phase quantization.

---

## 7. Downloaded papers (`research/papers/`)

New downloads for this task (all verified `%PDF`):
1. `kanerva2009_hyperdimensional_computing.pdf` — Kanerva, Cognitive Computation 1:139–159 (Redwood mirror).
2. `kleyko2023_vsa_survey_part1.pdf` — arXiv:2111.06077 (models, data transforms, capacity §2.4).
3. `kleyko2023_vsa_survey_part2.pdf` — arXiv:2112.15424 (applications, cleanup, challenges).
4. `frady2020_resonator_networks_1.pdf` — arXiv:2007.03748 / Neural Computation 32(12):2311–2331.
5. `kent2020_resonator_networks_2.pdf` — arXiv:1906.11684 / Neural Computation 32(12):2332–2388 (quadratic operational capacity).
6. `clarkson2023_vsa_capacity.pdf` — arXiv:2301.10352 (JL-style capacity of MAP-I/MAP-B/sparse-binary VSAs).
7. `frady2021_computing_functions_fpe.pdf` — arXiv:2109.03429 (Vector Function Architectures, FPE, sinc kernel).
8. `liu2024_partial_steiner_constant_weight_codes.pdf` — arXiv:2401.00733 (near-optimal CWCs, packing↔CWC equivalence, nibble methods).
9. `kautz_singleton_group_testing_1808.01457.pdf` — arXiv:1808.01457 (KS optimality in probabilistic group testing; disjunct-matrix lemmas).
10. `thomas2021_theoretical_perspective_hd.pdf` — Thomas et al., "A Theoretical Perspective on Hyperdimensional Computing" (Redwood mirror).

(Pre-existing unrelated neuroscience PDFs in the same directory were left untouched.)

## 8. Open threads for the new representation

- Ceiling comparison: flat random k-of-n SDR ⇒ collision ~ k²/n per pair; packing-designed families (partial Steiner / KS@k=2) ⇒ ≤1 shared atom pairwise while keeping ≈ n²/k² codewords ⇒ capacity win without giving up union-bound noise analysis (§1).
- Noise story: deterministic zero-false-match window e < k−λ, plus probabilistic tail (§1) — strictly stronger than Bloom-style random superposition at equal density.
- Decode: resonator-style superposition search is bitwise-feasible and its N² capacity scaling is the benchmark to beat/meet (§2.4).
- Gap: no principled multi-resolution constant-weight construction exists in the literature surveyed (§4) — combine embedded-codebook theory (Equitz–Cover) with Steiner packings per layer.
