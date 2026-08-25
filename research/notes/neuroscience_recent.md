# Recent Neuroscience → Computational Primitives for a Whole-Brain Framework
Research notes, compiled Aug 2026. Scope: 2020–2026 findings (pref. 2023–2026) beyond neocortex-only HTM/Thousand Brains.
Each finding: citation (authors, year, venue) + the COMPUTATIONAL PRIMITIVE it suggests.
Local PDFs referenced below live in `research/papers/`.

---

## 1. Dendritic computation: the neuron as multi-layer machine

**F1.1 — Human dendritic calcium action potentials (dCaAPs) implement non-separable logic in one compartment.**
Gidon, Zolnik, Fidzinski, Bolduan, Papoutsi, Poirazi, Holtkamp, Vida & Larkum (2020), *Science* 367:83–87.
In human L2/3 pyramidal neurons, apical dendrites generate *graded* dCaAPs: amplitude is maximal for threshold-level input and *decreases* for stronger input. This lets a single branch classify linearly non-separable (XOR-like) inputs — an operation conventionally requiring a multilayer network.
→ **Primitive:** per-branch "inverted-U" graded nonlinearity; single neuron = ensemble of independently thresholding classifiers (a local deep net).

**F1.2 — Two-layer / multi-compartment neuron as canonical model.**
Poirazi & Papoutsi (2020), *Nature Reviews Neuroscience* 21:303–321 (two-layer model lineage from Poirazi, Brannon & Mel 2003); see also Petousakis/Poirazi lab review of Hodgkin–Huxley impact on dendritic research (*J Physiol*/PMC10600871, 2023): branches act as semi-independent nonlinear subunits; backpropagating APs actively isolate neighboring branches so subunits stay independent; distal plasticity is driven by dendritic spikes rather than somatic feedback because bAPs attenuate distally.
→ **Primitive:** neuron = two-layer network (branch-subunit layer → soma nonlinearity). Learning rules are compartment-specific: proximal synapses use bAP-timed STDP; distal synapses use local dendritic-spike-gated LTP (branch-local credit assignment).

**F1.3 — Plateau potentials = long-lived binary latches that decode spike sequences across timescales.**
Leugering, Nieters & Pipa (2023), *Frontiers in Cognition* 2:1044216 [PDF: `leugering2023_dendritic_plateaus_spike_sequences_frontiers_cognition.pdf`].
All-or-none NMDA/Ca²⁺ plateau potentials persist hundreds of ms and interact across compartments. A network where each neuron detects short feature-sequences on distinct segments solves timing-invariant detection of long, overlapping sequences; stochastic vesicle release supplies graded ensemble responses. Plateau-driven Ca²⁺ — not brief somatic feedback — is the primary driver of plasticity.
→ **Primitive:** segment-level coincidence detector + seconds-scale latch = temporal-feature integrator robust to spike-timing jitter; learning gated by plateau events (eligibility trace with long time constant).

**F1.4 — Apical tuft as independent context/prediction field.**
Larkum line of work (Ca²⁺ spike = coincidence of bottom-up feedforward drive with top-down feedback in L5 tuft), synthesized in the 2024 *Pflügers Archiv* review "A dendrite is a dendrite is a dendrite?" — distal associative inputs integrate separately from proximal thalamic drive; bAP–plateau pairing gates bursts and plasticity windows; relates to attention/consciousness accounts.
→ **Primitive:** apical/distal compartment = separate *context field* validated independently against somatic evidence stream; burst output only when top-down prediction matches bottom-up input (AND-gate between hypothesis and data).

**F1.5 — Dendritic architecture improves ANN learning efficiency.**
Chavlis & Poirazi (2024), arXiv:2404.03708 (published *Nature Communications*) [PDF: `chavlis_poirazi2024_dendrites_endow_anns_arxiv.pdf`].
Bio-realistic dendritic ANNs: partial/local input sampling per dendrite + sparse structured dendrite→soma connectivity match or beat vanilla ANNs on image classification, resist overfitting and noise, and use **orders of magnitude fewer parameters**; benefits extend to sequential learning.
→ **Primitive:** replace dense layers with sampled-input dendritic subunits wired sparsely to their own soma — parameter-efficient, noise-robust feature extraction.

---

## 2. Thalamus: router and gate, not relay

**F2.1 — Transthalamic pathways are ubiquitous second route for corticocortical traffic.**
Sherman & Usrey (2024), *Journal of Neuroscience* 44(35):e0909242024 [PDF: `usrey_sherman2024_transthalamic_pathways_cortical_function.pdf`].
Every studied cortical area sends L5 driver input to a higher-order thalamic nucleus, which drives another cortical area — parallel to direct corticocortical connections. Both legs of the transthalamic loop are *driver* synapses; higher-order relays have richer modulatory/inhibitory control (TRN, zona incerta, basal ganglia → VA/VL) than first-order relays. L5 corticothalamic axons branch to motor centers ⇒ thalamically relayed message is an **efference copy of motor commands**. Feedforward vs feedback configurations give driving vs modulatory effects downstream.
→ **Primitive:** dual-route inter-areal communication: fast direct path + thalamic path that is (a) gatable open/closed per channel, (b) carrying action/efference-copy context, (c) switchable between drive-mode and modulation-mode. Inter-areal bandwidth allocation becomes an explicit control problem.

**F2.2 — Thalamus controls brain-wide information processing.**
Shine, Lewis & Garrett (2023), *Nature Reviews Neuroscience* 24:416–430.
Human neuroimaging shows thalamus shapes evoked activity, inter-regional connectivity, network topology and neural variability at rest and task — a systems-level controller of cortical dynamics, not a passive hub.
→ **Primitive:** global routing/gain variable controlling effective connectivity topology (cf. dynamic reconfiguration of functional networks).

**F2.3 — Core/matrix taxonomy abandoned; classification must be by circuit motif.**
Usrey/Sherman-line commentary (2024), *Journal of Neuroscience* 44(24):e0163242024: no coherent "matrix" class exists; many higher-order relays are topographic ("core"-like).
→ **Primitive:** model thalamocortical links by explicit motif parameters (driver/modulator source, laminar target, topography) instead of a coarse core/matrix flag.

---

## 3. Hippocampus: episodic index + factorized structural memory

**F3.1 — Tolman–Eichenbaum Machine (TEM).**
Whittington, Muller, Mark, Chen, Barry, Burgess & Behrens (2020), *Cell* 183:1249–1263.e23 (open via PMC7707106).
MEC provides a *structural basis* (transition-predictive features → grid/border/band/object-vector cells emerge); hippocampal cells are **conjunctions** of structural features with LEC sensory content. Remapping looks random but preserves structure (grid-phase retention verified in simultaneous place+grid recordings); same machinery explains transitive inference. Hippocampus indexes memory M by current sensory x to retrieve candidate structural states and correct path integration.
→ **Primitive:** factorized representation: structural basis × sensory content, bound conjunctively into sparse episodic pointers. Memory retrieval doubles as state estimation (index lookup refines predictive state).

**F3.2 — Human concept & location neurons validate indexing theory.**
Mackay, Reber, Bausch, Boström, Elger & Mormann (2024), *Nature Communications* 15 [PDF: `mackay2024_concept_location_neurons_episodic_indexing_natcomms.pdf`].
Single units in human MTL split into invariant *concept cells* (hippocampus/amygdala/EC — the "what" index) and parahippocampal *location-selective cells* (the "where" index); firing in both predicts successful item-location binding. Index neurons act as content-addressable pointers to neocortical representations.
→ **Primitive:** two-factor pointer store (semantic pointer ⊗ spatial/context pointer); encoding success = conjunction detection across factor populations.

**F3.3 — Engram evidence reconciles Indexing with Cognitive Map theories.**
Miry, Li & Chen (2021), *Frontiers in Behavioral Neuroscience* 14:632019 [PDF: `miry2021_hippocampal_memory_engram_indexing_frontiers.pdf`].
CA1 engram is largely content-free index (silencing subset impairs recall; engram-to-cortex reactivation confirmed) while a subset simultaneously carries map content (place fidelity across remapping). Allocation: excitability-based recruitment + synaptic-overlap mechanisms link/discriminate engrams.
→ **Primitive:** hybrid episodic store: mostly pointers, partially annotated with context; ensemble overlap controls memory linkage vs separation.

**F3.4 — Pattern separation/completion math & consolidation.**
Rolls & Treves (2024), *Progress in Neurobiology* 238:102636: CA3 recurrent attractor with diluted connectivity, sparse coding a≈0.02–0.05 gives quantitative storage capacity ≈ ln(N/a)/|ln(1−…)|-type scaling (capacity ∝ 1/(a·ln(1/a))); continuous spatial representations reduce capacity — charts of space stored as discrete charts; primate 'spatial view' cells; consolidation via cholinergic gating from orbitofrontal; neocortical storage modeled by Potts networks.
→ **Primitive:** attractor completion for cue-degraded recall; dentate-style expansion recoding for separation; capacity formulas as design constraints on index density; slow consolidation = cortex gradually absorbs indices (complementary learning systems with explicit transfer rule).

---

## 4. Predictive processing / predictive coding

**F4.1 — Canonical microcircuit implementation remains the best-supported neuronal story; empirical status modest but falsifiable.**
Smith, Kirshenbaum... actually: Smith, Friston & Whyte (2022→print 2024), *Neuroscience & Biobehavioral Reviews* 157:105473 "The empirical status of predictive coding and active inference": evidence offers *modest* support; several positive results explainable by feedforward models; active-inference behavior fits good but rarely model-compared. Companion analysis: Friston-line "Is predictive coding falsifiable?" (*Neurosci Biobehav Rev*, 2024): precision-modulated PC can fit contra-predictive responses, but precision changes predict latency/frequency signatures — testable.
→ **Primitive:** adopt PC as *one* message-passing layer, not dogma: hierarchical prediction/error streams mapped to deep/superficial cortical layers; keep alternative feedforward hypotheses as ablations.

**F4.2 — Predictive coding networks are a superset of feedforward nets with local, parallelizable learning.**
Salvatori, Mali, Buckley, Lukasiewicz, Rao, Friston et al. (2023), arXiv:2308.07870 [PDF: `salvatori2023_brain_inspired_predictive_coding_arxiv.pdf`]; tutorial: Alonso et al., arXiv:2407.04117 (2024).
PC nets trained by inference learning: purely local updates (no backprop), asynchronous, better continual-learning/data-efficiency (5–20% gains in some regimes); mathematically strictly more expressive than equal-depth FNNs.
→ **Primitive:** local-error-learning substrate for online adaptation; energy-based relaxation as inference step; natural fit for neuromorphic/async hardware.

**F4.3 — Active inference: expected free energy unifies reward and information gain.**
Pezzulo, Parr & Friston (2024), *Physics of Life Reviews*/*Cogn Neurodyn* reviews: policy value = pragmatic (preferred outcomes) + epistemic (information gain) terms; hierarchical temporally-deep generative models; offline periods minimize complexity — linked to hippocampal/cortical replay; aberrant precision explains psychopathology.
→ **Primitive:** intrinsic curiosity term computed from the same quantity as extrinsic control (epistemic value); sleep/offline = complexity pruning pass over generative models.

---

## 5. Neuromodulation: global gain/uncertainty control

**F5.1 — ACh selectively boosts precision of prediction errors (not repetition suppression).**
Pérez-González, Lao-Rodríguez, Aedo-Sánchez & Malmierca (2024), *eLife* 12:RP91475 [PDF: `perez_gonzalez2024_ach_precision_prediction_error_elife.pdf`].
Iontophoretic ACh in auditory cortex modulates mismatch/prediction-error responses while sparing repetition suppression; effect concentrated in infragranular layers; consistent with representational sharpening = increased PE precision (Kalman gain), gating which errors ascend.
→ **Primitive:** chemically addressable multiplicative gain on error channels — uncertainty/confidence knob controlling belief-update rate (implements Yu & Dayan 2005 "expected uncertainty" computationally).

**F5.2 — Neuromodulator ↔ precision dictionary (theory).**
Parr & Friston (2017–2018); Pezzulo, Parr & Friston (2024): ACh = likelihood/sensory precision; NA = transition/volatility precision; DA = policy precision γ. Bistable-perception modeling shows these precisions jointly set switch rates (UCL Discovery PDF available).
→ **Primitive:** three orthogonal scalar knobs — perceptual confidence, world-stability estimate, commitment-to-plan — broadcast globally but multiply locally.

**F5.3 — DA/ACh opponent dynamics are intrinsic and rhythmically organized.**
Krok, Maltese, Mistry et al. (2023), *Nature* 621:543–549: striatal DA and ACh fluctuate spontaneously ~2 Hz, anticorrelated, same phase relation as reward-evoked; DA inherited from SNc somatic activity, ACh synchronized by glutamatergic cortical/thalamic afferents.
Companion: Wang? — actually **Liu/? no**: (2023) *Nature* "Dopamine and glutamate regulate striatal acetylcholine in decision-making": DA pauses ACh transients via D2R; deleting D2R in CINs impairs history-dependent choice switching; cortical AND thalamic glutamate required for ACh release.
→ **Primitive:** opponent-pair oscillator (DA↑/ACh↓) as global phase clock opening plasticity windows; loss of the opponent brake degrades credit assignment over history — i.e., switching/regime-change signal, not just reward magnitude.

**F5.4 — Multi-neuromodulator lessons for continual learning.**
"Lessons from multi-neuromodulatory dynamics" (2025), arXiv:2501.06762: many-to-one neuromodulator↔task mapping; DA = third-factor plasticity/reward; NA = adaptive gain, network reset, exploration (tonic/phasic modes); ACh = pathway-selective gating reducing interference; metabotropic (slow) effects missing from ANNs.
→ **Primitive:** layered control system: fast weights (ionotropic) + slow meta-parameters (metabotropic) = separate timescales for representation vs regime; NA-phasic event = reset/exploration trigger.

---

## 6. Astrocytes & glia: slow second control loop

**F6.1 — Astrocytes as contextually-guided metaplasticity controllers.**
Gong, Pasqualetti, Papouin & Ching (2024), *PLoS Computational Biology* 20(5):e1012186 [PDF: `gong2024_astrocytes_contextually_guided_network_dynamics_ploscb.pdf`].
One astrocyte contacts ~10⁵ synapses; Ca²⁺ signals run seconds-slower than spikes; nested neuron→astrocyte→synapse loops implement *metaplasticity*: they reshape how synapses adapt, enabling multi-task RL and context-dependent regime shifts; formalizes prior proposals (incl. Kozachkov et al.'s transformer-like attention via astrocytes).
→ **Primitive:** region-scoped slow controller writing learning-rate/plasticity-gate meta-parameters — a second, slower network layered on the fast one (hierarchical controller with 10²–10³× timescale separation).

**F6.2 — Saliency-gated centripetal integration in astrocyte somata.**
Rupprecht, Duss, Becker, Lewis, Bohacek & Helmchen (2024), *Nature Neuroscience* 27:912–922.
Hippocampal astrocyte population activity leaky-integrates past behavioral/neuronal events over seconds; salient events (pupil dilation, LC activation) trigger α1-adrenergic-gated propagation of Ca²⁺ from distal processes to the soma — conditional, nonlinear integration of "was something important?"
→ **Primitive:** saliency-gated leaky integrator computing a slow novelty/arousal trace that can later raise local plasticity thresholds — eligibility signal decoupled from millisecond timing.

**F6.3 — Specialized glutamatergic astrocyte subpopulation with focal release hotspots.**
De Ceglia et al. (2023), *Nature* 623:156–164.
A molecularly distinct hippocampal astrocyte cluster expresses VGLUT1/2 release machinery and fires subsecond glutamate release at spatially precise hotspots in vivo; deletion impairs LTP, hippocampal memory, and nigrostriatal function.
→ **Primitive:** heterogeneous modulator nodes — sparse "booster cells" delivering focal excitatory gain to specific synapse groups (sparse structured side-network, not uniform glia soup).

**F6.4 — Astrocytic ensembles encode complementary information.**
Review: "Shining the Light on Astrocytic Ensembles" (2023), *Cells* 12:1253, citing Curreli et al. 2022 (hippocampal astrocytes encode position complementary to place cells; trajectories reconstructable from astrocyte population activity) and Doron et al. 2022 (reward location signaled only in familiar contexts).
→ **Primitive:** parallel slow information channel with its own ensemble code — candidate substrate for context familiarity gating and homeostatic state variables.

---

## 7. Whole-brain models & NeuroAI position

**F7.1 — Virtual Brain Twins: personalized whole-brain network models work clinically.**
Wang, Triebkorn, Breyton, Dollomaja, … Jirsa (2024), *National Science Review* 11:nwae079 [PDF: `wang2024_virtual_brain_twins_natl_sci_rev.pdf`].
Standard model: nodes = neural-mass models placed on subject connectome (3 personalization levels: anatomy → connectivity → inversion via probabilistic ML); Epileptor nodes localized epileptogenic networks (VEP: false-discovery 0.028 in seizure-free patients; EPINOV trial running); multiscale co-simulation swaps any node's mass model for a spiking microcircuit (TVB×NEST).
→ **Primitive:** connectome-constrained nodal-dynamics architecture with per-region parameter masks; hierarchical nesting (mass ↔ spiking) as first-class design feature; model-inversion/personalization loop as the framework's fitting procedure.

**F7.2 — Whole-brain modeling declared essential methodology.**
Patow, Martin, Sanz Perl, Kringelbach & Deco (2024), *Nature Reviews Methods Primers* comment; EBRAINS/TVB ecosystem (GPU mean-field AdEx parameter sweeps reveal coupling×adaptation state transitions underlying conscious/unconscious-like dynamics, *Applied Sciences* 14:2211, 2024).
→ **Primitive:** bifurcation-parameter view of brain states — small regional excitability/coupling changes flip global dynamical regime (wake/anesthesia/coma) — useful for a framework with explicit global-state variables.

**F7.3 — Roadmap position paper: brain-inspired AI needs whole-brain scope.**
Amunts et al. (2024), *GigaScience* 13:giae033 "The coming decade of digital brain research" (EBRAINS roadmap): eight key areas incl. multi-scale atlases, neuro-derived AI, digital twins; argues cortex-only models insufficient — subcortical systems (thalamus, BG, cerebellum) and glia belong in computational architectures.
→ **Design mandate:** our beyond-neocortex framing is aligned with the field's stated direction.

---

## 8. Cerebellum & basal ganglia: canonical roles

**F8.1 — Three learning algorithms, one state space (classic anchor).**
Doya (2000), *Neural Networks* 13:961–974 [PDF: `doya2000_cerebellum_basalganglia_cortex_learning_algorithms.pdf`]: cerebellum = supervised error learning building internal (forward/inverse) models; basal ganglia = reinforcement learning/action evaluation-selection; cortex = unsupervised representation learning providing shared state space. Architectures: reactive actor-critic, predictive (model-based) selection, state estimation à la Smith predictor/Kalman filter.
→ **Primitive:** modular learning-algorithm specialization wired around a common cortical state code — exactly the decomposition our framework should preserve.

**F8.2 — Timing division of labor.**
Cerebellar timing review (2024), *Current Research in Neurobiology* 7:100111: cerebellar beta oscillations regulate timing; lobule VI/crus I support temporal *prediction*; BG estimate interval timing; CB+thalamus+BG loop underlies proactive action. Patient double dissociation (Diedrichsen & Ivry): cerebellar damage adds noise to the internal clock; Parkinsonism reduces error-correction gain.
→ **Primitive:** separate interval-timing module (BG ramping/striatal beat frequency) feeding a feedforward "when-next" predictor (CB) — proactive, not reactive, control loop.

**F8.3 — Basal ganglia specify actions continuously, not just select them.**
Park, Coddington & Dudman (2020), *Annual Review of Neuroscience* 43:339–356: BG output scales movement vigor/amplitude (graded specification) alongside discrete selection; dopamine teaching signals shape both.
→ **Primitive:** action channel = (selection bit × continuous vigor parameter); softmax-style center-surround competition (direct/indirect pathways) with dopaminergic value updates.

**F8.4 — Granule-layer expansion recoding = sparsified pattern separator for internal models.**
Marr–Albus–Ito line updated by e.g. *The Cerebellum* review (2022, s12311-022-01392-6): GC layer expansion recoding enables pattern separation of mossy-fiber inputs; GC–PC connectivity maps are plastic, individual-specific engrams of adapted internal models; PCs predict position error hundreds of ms ahead.
→ **Primitive:** high-expansion sparse recoder → linear-ish readout = cheap supervised regressor; connectivity maps as per-agent internal-model storage (personalization point).

---

## Synthesis: primitive inventory for the framework

| # | Primitive | Source topic |
|---|-----------|--------------|
| P1 | Neuron = two-layer net: branch subunits (multidimensional sigmoids, XOR-capable graded dCaAPs) → soma | §1 |
| P2 | Distal/apical compartment = independent context field; burst = AND(bottom-up, top-down) | §1 |
| P3 | Plateau latches = long-window sequence detectors + plateau-gated plasticity (long eligibility traces) | §1 |
| P4 | Dual-route inter-area comms: direct fast path + thalamic gated path carrying efference copies; per-channel open/close | §2 |
| P5 | Hippocampal factorized index: structural basis ⊗ sensory content → sparse conjunctive pointers; retrieval = state estimation; separation/completion attractor math | §3 |
| P6 | Hierarchical prediction-error message passing with precision-weighted gains; local inference learning; epistemic-value term | §4 |
| P7 | Neuromodulators = orthogonal global scalars (likelihood/transition/policy precision) multiplying local computations; opponent DA/ACh clock opens plasticity windows | §5 |
| P8 | Glial meta-controller: slow saliency-gated integrator setting regional learning rates; sparse specialized booster nodes | §6 |
| P9 | Whole-brain = connectome-constrained nodal masses + swappable spiking microcircuits + personalization/inversion loop; bifurcation-controlled global states | §7 |
| P10 | Subcortical algorithm triad: CB supervised forward models (+ expansion-recoded pattern separator), BG reinforcement selection+vigor specification, interval-timing module for proactive control | §8 |

## Caveats / open questions
- PC empirical status is modest; keep feedforward baselines (Smith et al. 2024). Precision knobs risk unfalsifiability unless latency/frequency signatures are tested.
- Astrocyte gliotransmission was long contested; De Ceglia 2023 resolves part of it by showing it is *subpopulation*-specific — do not model astrocytes as homogeneous.
- Core/matrix thalamic taxonomy abandoned (2024) — encode motifs, not labels.
- TEM's structural-transfer claims verified for spatial tasks; non-spatial generalization still thinner empirically.
