import Mathlib.Tactic
import Mathlib.Data.Nat.Choose.Basic
import Mathlib.Data.Nat.ModEq

/-!
# NEBULA — machine-checked core theory

Formalization of the certified-noise claims of the NEBULA framework
(Nested-code Efficient Brain-wide Universal Learning Architecture).

A binary code of length `n` is a `Finset (Fin n)` (the ON positions);
weight = cardinality; overlap = intersection cardinality. This is exactly the
SDR semantics `overlap(x,y) = popcount(x AND y)`.

## Main results

1. `overlap_query_le` — rival bound under corruption.
2. `overlap_query_true_ge` — true-codeword bound under corruption.
3. `certified_unique_decode` — **certification theorem**: if every pair of
   dictionary codes agrees in ≤ A positions, any max-overlap decoder is exactly
   correct for every corruption with total flips e ≤ w − A − 1. Zero false
   matches, worst case, adversarially.
4. `binExpand_overlap_le` / `binary_dictionary_certified` — the one-hot lift:
   binary overlap between expansions is bounded by outer alphabet agreement,
   so an outer code with bounded agreement yields a certified binary dictionary.
5. `packing_bound` — strict-intersection families are combinatorially capped:
   M·C(k, λ+1) ≤ C(n, λ+1).
6. Rotation binding: weight/overlap preserving (`rotation_overlap_eq`),
   additive margins across roles (`binding_overlap_split`).
7. `ks_capacity_formula` — Kautz–Singleton operating point: agreement A = k'−1
   ⇒ certified radius w − k'.
-/

namespace Nebula

/-! ## Codes, overlap, noise -/

/-- Binary code of length `n`: the set of ON-bit positions. -/
abbrev Code (n : ℕ) := Finset (Fin n)

/-- Overlap = popcount of bitwise AND = size of position-set intersection. -/
def overlap {n : ℕ} (A B : Code n) : ℕ := (A ∩ B).card

/-- Corruption event: `off` = ON→OFF flips (⊆ support), `on` = OFF→ON flips
(disjoint from support). Each position flips at most once. -/
structure Noise {n : ℕ} (C : Code n) where
  off : Finset (Fin n)
  on  : Finset (Fin n)
  off_subset : off ⊆ C
  on_fresh : ∀ x ∈ on, x ∉ C

/-- Received query after corruption: Q = (C \ off) ∪ on. -/
def query {n : ℕ} {C : Code n} (ν : Noise C) : Code n := (C \ ν.off) ∪ ν.on

/-- Dictionary-wide pairwise agreement bound: any two distinct members share
at most `A` positions. Kautz–Singleton encoders realize this by construction;
random SDRs realize it only probabilistically. -/
def AgreeBound {n : ℕ} (D : Finset (Code n)) (A : ℕ) : Prop :=
  ∀ ⦃C R : Code n⦄, C ∈ D → R ∈ D → C ≠ R → overlap C R ≤ A

/-! ## Bounds under noise -/

/-- **Rival bound**: corrupted query overlaps any rival at most
(original agreement) + (number of spurious ON bits). -/
theorem overlap_query_le {n : ℕ} {C R : Code n} (ν : Noise C) :
    overlap (query ν) R ≤ overlap C R + ν.on.card := by
  unfold overlap query
  have h1 : ((C \ ν.off) ∪ ν.on) ∩ R ⊆ (C ∩ R) ∪ ν.on := by
    intro x hx
    have hi := Finset.mem_inter.mp hx
    have hu := Finset.mem_union.mp hi.1
    rcases hu with h | h
    · obtain ⟨hC, _⟩ := Finset.mem_sdiff.mp h
      exact Finset.mem_union_left _ (Finset.mem_inter.mpr ⟨hC, hi.2⟩)
    · exact Finset.mem_union_right _ h
  calc (((C \ ν.off) ∪ ν.on) ∩ R).card
      ≤ ((C ∩ R) ∪ ν.on).card := Finset.card_le_card h1
    _ ≤ (C ∩ R).card + ν.on.card := Finset.card_union_le _ _

/-- **True bound**: corrupted query keeps at least `w − t` overlap with the
true codeword (`t` = number of ON→OFF flips). -/
theorem overlap_query_true_ge {n : ℕ} {C : Code n} (ν : Noise C) :
    C.card - ν.off.card ≤ overlap (query ν) C := by
  unfold overlap query
  have hsub : C \ ν.off ⊆ ((C \ ν.off) ∪ ν.on) ∩ C := by
    intro x hx
    exact Finset.mem_inter.mpr
      ⟨Finset.mem_union_left _ hx, (Finset.mem_sdiff.mp hx).1⟩
  have h1 := Finset.card_le_card hsub
  have hsd := Finset.card_sdiff (s := ν.off) (t := C)
  have hint : (ν.off ∩ C).card ≤ ν.off.card :=
    Finset.card_le_card Finset.inter_subset_left
  have hcC : ν.off.card ≤ C.card := Finset.card_le_card ν.off_subset
  omega

/-- **Certification theorem (P1).** If every pair of dictionary codes agrees in
≤ A positions, then for any member `C` of weight `w` and any corruption with
total flips `e = t + f ≤ w − A − 1`, the true codeword strictly beats every
rival in overlap with the received query. Any max-overlap decoder is exactly
correct — zero false matches — for all such corruptions. Unconditional
combinatorics; holds adversarially, not merely in expectation. -/
theorem certified_unique_decode {n w e : ℕ} {D : Finset (Code n)} {A : ℕ}
    (hD : AgreeBound D A) {C : Code n} (hC : C ∈ D) (hw : C.card = w)
    (he : e + 1 + A ≤ w)
    (ν : Noise C) (hne : ν.off.card + ν.on.card ≤ e)
    ⦃R : Code n⦄ (hR : R ∈ D) (hCR : R ≠ C) :
    overlap (query ν) R < overlap (query ν) C := by
  have h1 := overlap_query_true_ge ν
  have h2 := overlap_query_le (R := R) ν
  have h3 := hD hC hR (fun h => hCR h.symm)
  have h4 : ν.off.card ≤ C.card := Finset.card_le_card ν.off_subset
  omega

/-- Corollary: correctness of the argmax decoder used by the benchmarks. -/
theorem certified_decode_correct {n w e : ℕ} {D : Finset (Code n)} {A : ℕ}
    (hD : AgreeBound D A) {C : Code n} (hC : C ∈ D) (hw : C.card = w)
    (he : e + 1 + A ≤ w)
    (ν : Noise C) (hne : ν.off.card + ν.on.card ≤ e)
    (argmax : Code n) (hmax : argmax ∈ D)
    (hwin : ∀ R ∈ D, R ≠ argmax → overlap (query ν) R ≤ overlap (query ν) argmax) :
    argmax = C := by
  by_contra hne2
  have hlt := certified_unique_decode hD hC hw he ν hne (R := argmax) hmax hne2
  have h2 := hwin C hC (fun h => hne2 h.symm)
  omega

/-! ## One-hot lift: outer agreement → binary certified dictionary -/

/-- One-hot block for symbol `s` at field `i`: exactly the bit position
`i*q + s` of an `N*q`-bit word. -/
def onehotBlock (N q : ℕ) (i : Fin N) (s : Fin q) : Code (N * q) :=
  {⟨i.val * q + s.val, by
      have h1 := i.isLt
      have h2 := s.isLt
      calc i.val * q + s.val < i.val * q + q := Nat.add_lt_add_left h2 _
        _ = (i.val + 1) * q := by ring
        _ ≤ N * q := Nat.mul_le_mul_right q (by omega)⟩}

lemma mem_onehotBlock {N q i s x} :
    x ∈ onehotBlock N q i s ↔ x.val = i.val * q + (s : Fin q) := by
  simp only [onehotBlock, Finset.mem_singleton]
  exact ⟨fun h => congrArg Fin.val h, fun h => Fin.ext h⟩

/-- Binary expansion of an outer word `u : Fin N → Fin q`:
concatenation of N one-hot blocks. Length N·q, weight N. -/
def binExpand (N q : ℕ) (u : Fin N → Fin q) : Code (N * q) :=
  (Finset.univ : Finset (Fin N)).biUnion fun i => onehotBlock N q i (u i)

lemma binExpand_mem {N q u x} :
    x ∈ binExpand N q u ↔ ∃ i : Fin N, x.val = i.val * q + (u i : Fin q) := by
  simp only [binExpand, Finset.mem_biUnion, Finset.mem_univ, true_and,
    mem_onehotBlock]

/-- Field extraction is well defined and symbol-determined for shared positions:
if the same bit position is produced by field `i` with symbol `a` and field `j`
with symbol `b`, then `i = j` and `a.val = b.val`. -/
lemma onehot_position_unique {N q : ℕ} {i j : Fin N} {a b : Fin q}
    (h : i.val * q + a.val = j.val * q + b.val) : i = j ∧ a.val = b.val := by
  have ha := a.isLt
  have hb := b.isLt
  have hij : i = j := by
    by_contra hne
    rcases lt_or_gt_of_ne hne with hlt | hgt
    · have hle : i.val * q + q ≤ j.val * q := by
        calc i.val * q + q = (i.val + 1) * q := by ring
          _ ≤ j.val * q := Nat.mul_le_mul_right q (by omega)
      omega
    · have hle : j.val * q + q ≤ i.val * q := by
        calc j.val * q + q = (j.val + 1) * q := by ring
          _ ≤ i.val * q := Nat.mul_le_mul_right q (by omega)
      omega
  subst hij
  exact ⟨rfl, by omega⟩

/-- Dividing a block base by `q` recovers the field index. -/
lemma div_field {q : ℕ} (hq : 0 < q) {a s : ℕ} (hs : s < q) :
    (a * q + s) / q = a := by
  have hcomm : a * q + s = q * a + s := by rw [Nat.mul_comm]
  rw [hcomm, Nat.mul_add_div hq, Nat.div_eq_of_lt hs]
  omega

/-- **One-hot lift (P4a).** Binary overlap between expansions of two outer
words equals their number of agreeing coordinates: each agreeing field
contributes exactly its one-hot position; disagreeing fields contribute
nothing (their blocks sit at the same offsets but hold different bits). -/
theorem binExpand_overlap_le (N q : ℕ) (hq : 0 < q) (u v : Fin N → Fin q) :
    overlap (binExpand N q u) (binExpand N q v)
      ≤ (Finset.univ.filter fun i : Fin N => u i = v i).card := by
  classical
  unfold overlap
  refine le_of_eq (Finset.card_nbij
    (fun x : Fin (N * q) => ⟨x.val / q, Nat.div_lt_iff_lt_mul hq |>.mpr x.isLt⟩)
    ?_ ?_ ?_)
  · -- MapsTo: a common position maps to an agreeing field
    intro x hx
    have hint := Finset.mem_inter.mp hx
    obtain ⟨i, hi⟩ := binExpand_mem.mp hint.1
    obtain ⟨j, hj⟩ := binExpand_mem.mp hint.2
    set p := i.val * q with hp
    set r := j.val * q with hr
    have hkey := onehot_position_unique (q := q)
      (show p + (u i : Fin q) = r + (v j : Fin q) by omega)
    have hij : i = j := hkey.1
    subst hij
    simp only [Finset.mem_coe, Finset.mem_filter, Finset.mem_univ, true_and]
    have hfield : x.val / q = i.val := by
      rw [hi, hp]
      exact div_field hq (u i).isLt
    have heq : (⟨x.val / q, Nat.div_lt_iff_lt_mul hq |>.mpr x.isLt⟩ : Fin N) = i :=
      Fin.ext hfield
    rw [heq]
    exact Fin.ext hkey.2
  · -- InjOn: two common positions mapping to the same field coincide
    intro x hx y hy hxy
    have hintx := Finset.mem_inter.mp hx
    have hinty := Finset.mem_inter.mp hy
    obtain ⟨i, hi⟩ := binExpand_mem.mp hintx.1
    obtain ⟨k, hk⟩ := binExpand_mem.mp hinty.1
    have hfi : x.val / q = i.val := by rw [hi]; exact div_field hq (u i).isLt
    have hfk : y.val / q = k.val := by rw [hk]; exact div_field hq (u k).isLt
    have hfxy : x.val / q = y.val / q := congrArg Fin.val hxy
    have hik : i = k := Fin.ext (by rw [← hfi, ← hfk, hfxy])
    -- pin down x through its membership in v's expansion
    obtain ⟨j, hj⟩ := binExpand_mem.mp hintx.2
    have hxk := onehot_position_unique (q := q)
      (show i.val * q + (u i : Fin q) = j.val * q + (v j : Fin q) by omega)
    have huivi : u i = v i := by
      have h2 := hxk.2
      rw [← hxk.1] at h2
      exact Fin.ext h2
    -- pin down y likewise
    obtain ⟨l, hl⟩ := binExpand_mem.mp hinty.2
    have hyk := onehot_position_unique (q := q)
      (show k.val * q + (u k : Fin q) = l.val * q + (v l : Fin q) by omega)
    have hukvl : u k = v k := by
      have h2 := hyk.2
      rw [← hyk.1] at h2
      exact Fin.ext h2
    have hxv : x.val = i.val * q + (u i : Fin q) := hi
    have hyv : y.val = i.val * q + (u i : Fin q) := by
      rw [hk, ← hik]
    exact Fin.ext (by rw [hxv, hyv])
  · -- SurjOn: each agreeing field is hit by its own one-hot position
    -- (hy : y ∈ ↑(univ.filter ...))
    intro y hy
    have hy' : u y = v y := by
      have hc := Finset.mem_coe.mp hy
      exact (Finset.mem_filter.mp hc).2
    refine ⟨⟨y.val * q + (u y : Fin q), by
      have h1 := y.isLt
      have h2 := (u y).isLt
      calc y.val * q + (u y : Fin q) < y.val * q + q := Nat.add_lt_add_left h2 _
        _ = (y.val + 1) * q := by ring
        _ ≤ N * q := Nat.mul_le_mul_right q (by omega)⟩, ?_, ?_⟩
    · rw [Finset.mem_coe, Finset.mem_inter]
      refine ⟨?_, ?_⟩
      · rw [binExpand_mem]
        exact ⟨y, rfl⟩
      · rw [binExpand_mem]
        exact ⟨y, by rw [hy']⟩
    · exact Fin.ext (div_field hq (u y).isLt)

/-- **Binary dictionary certification (P4b).** If an outer code family has
pairwise agreement ≤ A, its one-hot expansion is a binary dictionary with
pairwise overlap ≤ A — exactly the hypothesis consumed by the certification
theorem. RS/KS theory supplies the outer bound (and the C++ self-test
machine-checks it exhaustively for deployed parameters). -/
theorem binary_dictionary_certified (N q A : ℕ) (hq : 0 < q)
    (O : Finset (Fin N → Fin q))
    (hagree : ∀ u ∈ O, ∀ v ∈ O, u ≠ v →
      (Finset.univ.filter fun i : Fin N => u i = v i).card ≤ A) :
    AgreeBound (O.image (binExpand N q)) A := by
  intro C R hC hR hCR
  simp only [Finset.mem_image] at hC hR
  obtain ⟨u, hu, rfl⟩ := hC
  obtain ⟨v, hv, rfl⟩ := hR
  exact le_trans (binExpand_overlap_le N q hq u v)
    (hagree u hu v hv (fun h => hCR (by rw [h])))

/-! ## Packing bound -/

/-- Counting helper: for a pairwise-disjoint block family, the union's
cardinality equals the sum of the blocks' cardinalities. -/
theorem card_biUnion_eq_sum {ι α : Type*} [DecidableEq α] [DecidableEq ι]
    (F : Finset ι) (blocks : ι → Finset (Finset α))
    (hdisj : ∀ i ∈ F, ∀ j ∈ F, i ≠ j → Disjoint (blocks i) (blocks j)) :
    (F.biUnion blocks).card = ∑ i ∈ F, (blocks i).card := by
  classical
  induction F using Finset.induction_on with
  | empty => simp
  | insert a F ha ih =>
    have hdisj' : ∀ i ∈ F, ∀ j ∈ F, i ≠ j → Disjoint (blocks i) (blocks j) :=
      fun i hi j hj h => hdisj i (Finset.mem_insert_of_mem hi) j (Finset.mem_insert_of_mem hj) h
    have hnew : Disjoint (blocks a) (F.biUnion blocks) := by
      refine Finset.disjoint_left.mpr fun x hx1 hx2 => ?_
      rw [Finset.mem_biUnion] at hx2
      obtain ⟨i, hi, hxi⟩ := hx2
      have hd := hdisj a (Finset.mem_insert_self a F) i (Finset.mem_insert_of_mem hi)
        (fun h => ha (h ▸ hi))
      exact Finset.disjoint_left.mp hd hx1 hxi
    rw [Finset.biUnion_insert, Finset.card_union_of_disjoint hnew,
        Finset.sum_insert ha, ih hdisj']

/-- **Packing bound (P5).** A family of k-subsets of an n-set whose members
pairwise intersect in ≤ λ points satisfies M·C(k, λ+1) ≤ C(n, λ+1):
each member contains C(k,λ+1) sub-blocks of size λ+1; no sub-block is shared
by two distinct members; all sub-blocks live among the C(n,λ+1) choices.
Strict families pay linearly in capacity for their worst-case guarantee. -/
theorem packing_bound {n k lam : ℕ} (F : Finset (Code n))
    (hk : ∀ C ∈ F, C.card = k)
    (hagree : AgreeBound F lam) :
    F.card * k.choose (lam + 1) ≤ n.choose (lam + 1) := by
  classical
  set P : Finset (Finset (Fin n)) :=
    (Finset.univ : Finset (Fin n)).powersetCard (lam + 1) with hPdef
  have hPcard : P.card = n.choose (lam + 1) := by
    rw [hPdef, Finset.card_powersetCard, Finset.card_univ, Fintype.card_fin]
  have hsub_card : ∀ C ∈ F, (C.powersetCard (lam + 1)).card = k.choose (lam + 1) := by
    intro C hC; rw [Finset.card_powersetCard, hk C hC]
  have hdisj : ∀ C ∈ F, ∀ R ∈ F, C ≠ R →
      Disjoint (C.powersetCard (lam + 1)) (R.powersetCard (lam + 1)) := by
    intro C hC R hR hCR
    refine Finset.disjoint_right.mpr fun S hSR hSC => ?_
    have hSC' : S ⊆ C := (Finset.mem_powersetCard.mp hSC).1
    have hSR' : S ⊆ R := (Finset.mem_powersetCard.mp hSR).1
    have hScard : S.card = lam + 1 := (Finset.mem_powersetCard.mp hSC).2
    have hint : lam + 1 ≤ overlap C R := by
      unfold overlap
      have hc := Finset.card_le_card (Finset.inter_subset_inter hSC' hSR')
      have hSS : S ∩ S = S := Finset.inter_self S
      rw [hSS] at hc
      omega
    have hag := hagree hC hR hCR
    omega
  have hsub : ∀ C ∈ F, C.powersetCard (lam + 1) ⊆ P := by
    intro C hC S hS
    rw [Finset.mem_powersetCard] at hS ⊢
    exact ⟨Finset.subset_univ S, hS.2⟩
  have hbU : (F.biUnion fun C => C.powersetCard (lam + 1)) ⊆ P := by
    intro S hS
    rw [Finset.mem_biUnion] at hS
    obtain ⟨C, hC, hSC⟩ := hS
    exact hsub C hC hSC
  have hcount : (F.biUnion fun C => C.powersetCard (lam + 1)).card
      = ∑ C ∈ F, (C.powersetCard (lam + 1)).card :=
    card_biUnion_eq_sum F _ hdisj
  have hsumeq : ∑ C ∈ F, (C.powersetCard (lam + 1)).card
      = ∑ _C ∈ F, k.choose (lam + 1) :=
    Finset.sum_congr rfl fun C hC => hsub_card C hC
  have hfirst : F.card * k.choose (lam + 1)
      = ∑ _C ∈ F, k.choose (lam + 1) := by
    rw [Finset.sum_const, smul_eq_mul]
  calc F.card * k.choose (lam + 1)
      = ∑ _C ∈ F, k.choose (lam + 1) := hfirst
    _ = ∑ C ∈ F, (C.powersetCard (lam + 1)).card := hsumeq.symm
    _ = (F.biUnion fun C => C.powersetCard (lam + 1)).card := hcount.symm
    _ ≤ P.card := Finset.card_le_card hbU
    _ = n.choose (lam + 1) := hPcard

/-! ## Rotation binding -/

/-- Bitwise circular rotation of a position within a word of `m` bits.
Mirrors the C++ kernel's rotate-by-ρ within each role field. -/
def rotPos (m ρ : ℕ) (i : Fin m) : Fin m :=
  ⟨(i.val + ρ) % m, Nat.mod_lt _ (by have := i.isLt; omega)⟩

theorem rotPos_injective (m ρ : ℕ) : Function.Injective (rotPos m ρ) := by
  intro i j hEq
  have hEq' : (i.val + ρ) % m = (j.val + ρ) % m := congrArg Fin.val hEq
  have h2 : (i.val + ρ) ≡ (j.val + ρ) [MOD m] := hEq'
  have h3 := Nat.ModEq.add_right_cancel' ρ h2
  have h4 : i.val % m = j.val % m := h3
  rw [Nat.mod_eq_of_lt i.isLt, Nat.mod_eq_of_lt j.isLt] at h4
  exact Fin.ext h4

/-- Rotation of a code: image under `rotPos`. Weight-preserving. -/
def rotCode (m ρ : ℕ) (C : Code m) : Code m := C.image (rotPos m ρ)

theorem rotation_card_eq (m ρ : ℕ) (C : Code m) : (rotCode m ρ C).card = C.card :=
  Finset.card_image_of_injective _ (rotPos_injective m ρ)

/-- Rotation preserves overlap exactly. -/
theorem rotation_overlap_eq (m ρ : ℕ) (A B : Code m) :
    overlap (rotCode m ρ A) (rotCode m ρ B) = overlap A B := by
  unfold overlap rotCode
  rw [← Finset.image_inter A B (rotPos_injective m ρ)]
  exact Finset.card_image_of_injective _ (rotPos_injective m ρ)

/-- **Binding safety (P6).** Composing two role-fields by OR (into disjoint
bit ranges) yields combined overlap exactly equal to the sum of per-role
overlaps. Noise cannot manufacture cross-role evidence; margins add. -/
theorem binding_overlap_split {m ρ τ : ℕ} (A A' B B' : Code m)
    (hAB : ∀ x ∈ rotCode m ρ A, x ∉ rotCode m τ B')
    (hBA : ∀ x ∈ rotCode m τ B, x ∉ rotCode m ρ A') :
    overlap (rotCode m ρ A ∪ rotCode m τ B) (rotCode m ρ A' ∪ rotCode m τ B')
      = overlap A A' + overlap B B' := by
  classical
  have hcross1 : rotCode m ρ A ∩ rotCode m τ B' = ∅ := by
    ext x
    constructor
    · intro h
      have hi := Finset.mem_inter.mp h
      exact absurd (hAB x hi.1 hi.2) False.elim
    · exact fun h => ((Finset.notMem_empty x) h).elim
  have hcross2 : rotCode m τ B ∩ rotCode m ρ A' = ∅ := by
    ext x
    constructor
    · intro h
      have hi := Finset.mem_inter.mp h
      exact absurd (hBA x hi.1 hi.2) False.elim
    · exact fun h => ((Finset.notMem_empty x) h).elim
  have hsplit : (rotCode m ρ A ∪ rotCode m τ B) ∩ (rotCode m ρ A' ∪ rotCode m τ B')
      = (rotCode m ρ A ∩ rotCode m ρ A') ∪ (rotCode m τ B ∩ rotCode m τ B') := by
    ext x
    constructor
    · intro h
      have hi := Finset.mem_inter.mp h
      have hu := Finset.mem_union.mp hi.1
      have hv := Finset.mem_union.mp hi.2
      rcases hu with hx | hy <;> rcases hv with hz | hw
      · exact Finset.mem_union_left _ (Finset.mem_inter.mpr ⟨hx, hz⟩)
      · have hcon : x ∈ rotCode m ρ A ∩ rotCode m τ B' :=
            Finset.mem_inter.mpr ⟨hx, hw⟩
        have hi := Finset.mem_inter.mp hcon
        exact absurd (hAB x hi.1 hi.2) False.elim
      · have hcon : x ∈ rotCode m τ B ∩ rotCode m ρ A' :=
            Finset.mem_inter.mpr ⟨hy, hz⟩
        have hi := Finset.mem_inter.mp hcon
        exact absurd (hBA x hi.1 hi.2) False.elim
      · exact Finset.mem_union_right _ (Finset.mem_inter.mpr ⟨hy, hw⟩)
    · intro h
      have hu := Finset.mem_union.mp h
      rcases hu with hx | hy
      · have hij := Finset.mem_inter.mp hx
        exact Finset.mem_inter.mpr
          ⟨Finset.mem_union_left _ hij.1, Finset.mem_union_left _ hij.2⟩
      · have hij := Finset.mem_inter.mp hy
        exact Finset.mem_inter.mpr
          ⟨Finset.mem_union_right _ hij.1, Finset.mem_union_right _ hij.2⟩
  have hdisj : Disjoint (rotCode m ρ A ∩ rotCode m ρ A') (rotCode m τ B ∩ rotCode m τ B') := by
    refine Finset.disjoint_left.mpr fun x hx1 hx2 => ?_
    have hi := Finset.mem_inter.mp hx1
    have hj := Finset.mem_inter.mp hx2
    exact absurd (hAB x hi.1 hj.2) False.elim
  calc overlap (rotCode m ρ A ∪ rotCode m τ B) (rotCode m ρ A' ∪ rotCode m τ B')
      = ((rotCode m ρ A ∪ rotCode m τ B) ∩ (rotCode m ρ A' ∪ rotCode m τ B')).card := rfl
    _ = ((rotCode m ρ A ∩ rotCode m ρ A') ∪ (rotCode m τ B ∩ rotCode m τ B')).card := by
        rw [hsplit]
    _ = (rotCode m ρ A ∩ rotCode m ρ A').card + (rotCode m τ B ∩ rotCode m τ B').card :=
        Finset.card_union_of_disjoint hdisj
    _ = overlap (rotCode m ρ A) (rotCode m ρ A')
          + overlap (rotCode m τ B) (rotCode m τ B') := rfl
    _ = overlap A A' + overlap B B' := by
        rw [rotation_overlap_eq m ρ A A', rotation_overlap_eq m τ B B']

/-! ## Kautz–Singleton operating point -/

/-- **Capacity–robustness exchange (P7).** Given an encoder family realizing
pairwise agreement ≤ k'−1 (Kautz–Singleton: outer RS distance N−k'+1 mapped
through one-hot; hypothesis `hagree` — machine-checked exhaustively for the
deployed parameters by the C++ self-test), a dictionary of ANY size supports
certified unique decode up to e* = w − k' total bit flips. With q-symbol outer
alphabets the dictionary holds M = q^(w−e*) codewords: each extra certified
flip of tolerance costs exactly log_q(M) of capacity. Capacity and robustness
are one dial, not independent hopes. -/
theorem ks_capacity_formula {n w q k' e : ℕ} {D : Finset (Code n)}
    (hk' : 1 ≤ k')
    (hagree : AgreeBound D (k' - 1))
    {C : Code n} (hC : C ∈ D) (hw : C.card = w)
    (he : e + k' ≤ w)
    (ν : Noise C) (hne : ν.off.card + ν.on.card ≤ e)
    ⦃R : Code n⦄ (hR : R ∈ D) (hCR : R ≠ C) :
    overlap (query ν) R < overlap (query ν) C := by
  have he2 : e + 1 + (k' - 1) ≤ w := by omega
  exact certified_unique_decode hagree hC hw he2 ν hne hR hCR

end Nebula

/-! ## Verification transparency -/

#print axioms Nebula.certified_unique_decode
#print axioms Nebula.certified_decode_correct
#print axioms Nebula.binExpand_overlap_le
#print axioms Nebula.binary_dictionary_certified
#print axioms Nebula.packing_bound
#print axioms Nebula.binding_overlap_split
#print axioms Nebula.ks_capacity_formula
