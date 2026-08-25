// Lean compiler output
// Module: Nebula
// Imports: public import Init public meta import Init public import Mathlib.Tactic public import Mathlib.Data.Nat.Choose.Basic public import Mathlib.Data.Nat.ModEq
#include <lean/lean.h>
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wunused-label"
#elif defined(__GNUC__) && !defined(__CLANG__)
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-label"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif
#ifdef __cplusplus
extern "C" {
#endif
uint8_t lean_nat_dec_eq(lean_object*, lean_object*);
lean_object* lean_array_push(lean_object*, lean_object*);
lean_object* lean_array_get_size(lean_object*);
uint8_t lean_nat_dec_lt(lean_object*, lean_object*);
size_t lean_usize_of_nat(lean_object*);
uint8_t lean_usize_dec_eq(size_t, size_t);
size_t lean_usize_sub(size_t, size_t);
lean_object* lean_array_uget_borrowed(lean_object*, size_t);
lean_object* lean_nat_mul(lean_object*, lean_object*);
lean_object* lean_nat_add(lean_object*, lean_object*);
lean_object* l_List_finRange(lean_object*);
lean_object* lp_mathlib_Multiset_bind___redArg(lean_object*, lean_object*);
lean_object* lp_batteries_List_pwFilter___redArg(lean_object*, lean_object*);
uint8_t lp_mathlib_Multiset_decidableMem___aux__1___redArg(lean_object*, lean_object*, lean_object*);
lean_object* lean_mk_empty_array_with_capacity(lean_object*);
lean_object* lean_nat_mod(lean_object*, lean_object*);
lean_object* lp_mathlib_Multiset_map___redArg(lean_object*, lean_object*);
lean_object* lean_array_mk(lean_object*);
lean_object* l_instDecidableEqFin___boxed(lean_object*, lean_object*, lean_object*);
lean_object* lp_mathlib_Multiset_filter___redArg(lean_object*, lean_object*);
lean_object* l_List_lengthTR___redArg(lean_object*);
LEAN_EXPORT uint8_t lp_nebula_Multiset_ndinter___at___00Nebula_overlap_spec__0___lam__0(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_nebula_Multiset_ndinter___at___00Nebula_overlap_spec__0___lam__0___boxed(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_nebula_Multiset_ndinter___at___00Nebula_overlap_spec__0(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_nebula_Nebula_overlap(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT uint8_t lp_nebula_List_elem___at___00Multiset_ndunion___at___00Nebula_query_spec__1_spec__2___redArg(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_nebula_List_elem___at___00Multiset_ndunion___at___00Nebula_query_spec__1_spec__2___redArg___boxed(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_nebula___private_Init_Data_Array_Basic_0__Array_foldrMUnsafe_fold___at___00List_foldrTR___at___00Multiset_ndunion___at___00Nebula_query_spec__1_spec__3_spec__5(lean_object*, lean_object*, size_t, size_t, lean_object*);
LEAN_EXPORT lean_object* lp_nebula___private_Init_Data_Array_Basic_0__Array_foldrMUnsafe_fold___at___00List_foldrTR___at___00Multiset_ndunion___at___00Nebula_query_spec__1_spec__3_spec__5___boxed(lean_object*, lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_nebula_List_foldrTR___at___00Multiset_ndunion___at___00Nebula_query_spec__1_spec__3(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_nebula_List_foldrTR___at___00Multiset_ndunion___at___00Nebula_query_spec__1_spec__3___boxed(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_nebula_Multiset_ndunion___at___00Nebula_query_spec__1(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_nebula_Multiset_ndunion___at___00Nebula_query_spec__1___boxed(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_nebula___private_Init_Data_Array_Basic_0__Array_foldrMUnsafe_fold___at___00__private_Init_Data_List_Impl_0__List_eraseTR_go___at___00List_diff___at___00Multiset_sub___at___00Nebula_query_spec__0_spec__0_spec__1_spec__3(lean_object*, size_t, size_t, lean_object*);
LEAN_EXPORT lean_object* lp_nebula___private_Init_Data_Array_Basic_0__Array_foldrMUnsafe_fold___at___00__private_Init_Data_List_Impl_0__List_eraseTR_go___at___00List_diff___at___00Multiset_sub___at___00Nebula_query_spec__0_spec__0_spec__1_spec__3___boxed(lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_nebula___private_Init_Data_List_Impl_0__List_eraseTR_go___at___00List_diff___at___00Multiset_sub___at___00Nebula_query_spec__0_spec__0_spec__1___redArg(lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_nebula___private_Init_Data_List_Impl_0__List_eraseTR_go___at___00List_diff___at___00Multiset_sub___at___00Nebula_query_spec__0_spec__0_spec__1___redArg___boxed(lean_object*, lean_object*, lean_object*, lean_object*);
static const lean_array_object lp_nebula_List_diff___at___00Multiset_sub___at___00Nebula_query_spec__0_spec__0___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_array_object) + sizeof(void*)*0, .m_other = 0, .m_tag = 246}, .m_size = 0, .m_capacity = 0, .m_data = {}};
static const lean_object* lp_nebula_List_diff___at___00Multiset_sub___at___00Nebula_query_spec__0_spec__0___closed__0 = (const lean_object*)&lp_nebula_List_diff___at___00Multiset_sub___at___00Nebula_query_spec__0_spec__0___closed__0_value;
LEAN_EXPORT lean_object* lp_nebula_List_diff___at___00Multiset_sub___at___00Nebula_query_spec__0_spec__0(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_nebula_List_diff___at___00Multiset_sub___at___00Nebula_query_spec__0_spec__0___boxed(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_nebula_Multiset_sub___at___00Nebula_query_spec__0(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_nebula_Multiset_sub___at___00Nebula_query_spec__0___boxed(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_nebula_Nebula_query(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_nebula_Nebula_query___boxed(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT uint8_t lp_nebula_List_elem___at___00Multiset_ndunion___at___00Nebula_query_spec__1_spec__2(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_nebula_List_elem___at___00Multiset_ndunion___at___00Nebula_query_spec__1_spec__2___boxed(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_nebula___private_Init_Data_List_Impl_0__List_eraseTR_go___at___00List_diff___at___00Multiset_sub___at___00Nebula_query_spec__0_spec__0_spec__1(lean_object*, lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_nebula___private_Init_Data_List_Impl_0__List_eraseTR_go___at___00List_diff___at___00Multiset_sub___at___00Nebula_query_spec__0_spec__0_spec__1___boxed(lean_object*, lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_nebula_Nebula_onehotBlock___redArg(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_nebula_Nebula_onehotBlock___redArg___boxed(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_nebula_Nebula_onehotBlock(lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_nebula_Nebula_onehotBlock___boxed(lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_nebula_Nebula_binExpand___lam__0(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_nebula_Nebula_binExpand___lam__0___boxed(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_nebula_Finset_biUnion___at___00Nebula_binExpand_spec__0___redArg___lam__0(lean_object*, lean_object*);
LEAN_EXPORT uint8_t lp_nebula_List_dedup___at___00Multiset_dedup___at___00Multiset_toFinset___at___00Finset_biUnion___at___00Nebula_binExpand_spec__0_spec__0_spec__1_spec__2___redArg___lam__0(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_nebula_List_dedup___at___00Multiset_dedup___at___00Multiset_toFinset___at___00Finset_biUnion___at___00Nebula_binExpand_spec__0_spec__0_spec__1_spec__2___redArg___lam__0___boxed(lean_object*, lean_object*);
static const lean_closure_object lp_nebula_List_dedup___at___00Multiset_dedup___at___00Multiset_toFinset___at___00Finset_biUnion___at___00Nebula_binExpand_spec__0_spec__0_spec__1_spec__2___redArg___closed__0_value = {.m_header = {.m_rc = 0, .m_cs_sz = sizeof(lean_closure_object) + sizeof(void*)*0, .m_other = 0, .m_tag = 245}, .m_fun = (void*)lp_nebula_List_dedup___at___00Multiset_dedup___at___00Multiset_toFinset___at___00Finset_biUnion___at___00Nebula_binExpand_spec__0_spec__0_spec__1_spec__2___redArg___lam__0___boxed, .m_arity = 2, .m_num_fixed = 0, .m_objs = {} };
static const lean_object* lp_nebula_List_dedup___at___00Multiset_dedup___at___00Multiset_toFinset___at___00Finset_biUnion___at___00Nebula_binExpand_spec__0_spec__0_spec__1_spec__2___redArg___closed__0 = (const lean_object*)&lp_nebula_List_dedup___at___00Multiset_dedup___at___00Multiset_toFinset___at___00Finset_biUnion___at___00Nebula_binExpand_spec__0_spec__0_spec__1_spec__2___redArg___closed__0_value;
LEAN_EXPORT lean_object* lp_nebula_List_dedup___at___00Multiset_dedup___at___00Multiset_toFinset___at___00Finset_biUnion___at___00Nebula_binExpand_spec__0_spec__0_spec__1_spec__2___redArg(lean_object*);
LEAN_EXPORT lean_object* lp_nebula_Multiset_dedup___at___00Multiset_toFinset___at___00Finset_biUnion___at___00Nebula_binExpand_spec__0_spec__0_spec__1(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_nebula_Multiset_dedup___at___00Multiset_toFinset___at___00Finset_biUnion___at___00Nebula_binExpand_spec__0_spec__0_spec__1___boxed(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_nebula_Multiset_toFinset___at___00Finset_biUnion___at___00Nebula_binExpand_spec__0_spec__0(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_nebula_Multiset_toFinset___at___00Finset_biUnion___at___00Nebula_binExpand_spec__0_spec__0___boxed(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_nebula_Finset_biUnion___at___00Nebula_binExpand_spec__0___redArg(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_nebula_Finset_biUnion___at___00Nebula_binExpand_spec__0___redArg___boxed(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_nebula_Nebula_binExpand(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_nebula_Finset_biUnion___at___00Nebula_binExpand_spec__0(lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_nebula_Finset_biUnion___at___00Nebula_binExpand_spec__0___boxed(lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_nebula_List_dedup___at___00Multiset_dedup___at___00Multiset_toFinset___at___00Finset_biUnion___at___00Nebula_binExpand_spec__0_spec__0_spec__1_spec__2(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_nebula_List_dedup___at___00Multiset_dedup___at___00Multiset_toFinset___at___00Finset_biUnion___at___00Nebula_binExpand_spec__0_spec__0_spec__1_spec__2___boxed(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_nebula_Nebula_rotPos(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_nebula_Nebula_rotPos___boxed(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_nebula_Finset_image___at___00Nebula_rotCode_spec__0___redArg(lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_nebula_Nebula_rotCode(lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_nebula_Finset_image___at___00Nebula_rotCode_spec__0(lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT lean_object* lp_nebula_Finset_image___at___00Nebula_rotCode_spec__0___boxed(lean_object*, lean_object*, lean_object*, lean_object*);
LEAN_EXPORT uint8_t lp_nebula_Multiset_ndinter___at___00Nebula_overlap_spec__0___lam__0(lean_object* v___x_1_, lean_object* v_t_2_, lean_object* v_a_3_){
_start:
{
uint8_t v___x_4_; 
v___x_4_ = lp_mathlib_Multiset_decidableMem___aux__1___redArg(v___x_1_, v_a_3_, v_t_2_);
return v___x_4_;
}
}
LEAN_EXPORT lean_object* lp_nebula_Multiset_ndinter___at___00Nebula_overlap_spec__0___lam__0___boxed(lean_object* v___x_5_, lean_object* v_t_6_, lean_object* v_a_7_){
_start:
{
uint8_t v_res_8_; lean_object* v_r_9_; 
v_res_8_ = lp_nebula_Multiset_ndinter___at___00Nebula_overlap_spec__0___lam__0(v___x_5_, v_t_6_, v_a_7_);
v_r_9_ = lean_box(v_res_8_);
return v_r_9_;
}
}
LEAN_EXPORT lean_object* lp_nebula_Multiset_ndinter___at___00Nebula_overlap_spec__0(lean_object* v_n_10_, lean_object* v_s_11_, lean_object* v_t_12_){
_start:
{
lean_object* v___x_13_; lean_object* v___f_14_; lean_object* v___x_15_; 
v___x_13_ = lean_alloc_closure((void*)(l_instDecidableEqFin___boxed), 3, 1);
lean_closure_set(v___x_13_, 0, v_n_10_);
v___f_14_ = lean_alloc_closure((void*)(lp_nebula_Multiset_ndinter___at___00Nebula_overlap_spec__0___lam__0___boxed), 3, 2);
lean_closure_set(v___f_14_, 0, v___x_13_);
lean_closure_set(v___f_14_, 1, v_t_12_);
v___x_15_ = lp_mathlib_Multiset_filter___redArg(v___f_14_, v_s_11_);
return v___x_15_;
}
}
LEAN_EXPORT lean_object* lp_nebula_Nebula_overlap(lean_object* v_n_16_, lean_object* v_A_17_, lean_object* v_B_18_){
_start:
{
lean_object* v___x_19_; lean_object* v___x_20_; 
v___x_19_ = lp_nebula_Multiset_ndinter___at___00Nebula_overlap_spec__0(v_n_16_, v_A_17_, v_B_18_);
v___x_20_ = l_List_lengthTR___redArg(v___x_19_);
lean_dec(v___x_19_);
return v___x_20_;
}
}
LEAN_EXPORT uint8_t lp_nebula_List_elem___at___00Multiset_ndunion___at___00Nebula_query_spec__1_spec__2___redArg(lean_object* v_a_21_, lean_object* v_x_22_){
_start:
{
if (lean_obj_tag(v_x_22_) == 0)
{
uint8_t v___x_23_; 
v___x_23_ = 0;
return v___x_23_;
}
else
{
lean_object* v_head_24_; lean_object* v_tail_25_; uint8_t v___x_26_; 
v_head_24_ = lean_ctor_get(v_x_22_, 0);
v_tail_25_ = lean_ctor_get(v_x_22_, 1);
v___x_26_ = lean_nat_dec_eq(v_a_21_, v_head_24_);
if (v___x_26_ == 0)
{
v_x_22_ = v_tail_25_;
goto _start;
}
else
{
return v___x_26_;
}
}
}
}
LEAN_EXPORT lean_object* lp_nebula_List_elem___at___00Multiset_ndunion___at___00Nebula_query_spec__1_spec__2___redArg___boxed(lean_object* v_a_28_, lean_object* v_x_29_){
_start:
{
uint8_t v_res_30_; lean_object* v_r_31_; 
v_res_30_ = lp_nebula_List_elem___at___00Multiset_ndunion___at___00Nebula_query_spec__1_spec__2___redArg(v_a_28_, v_x_29_);
lean_dec(v_x_29_);
lean_dec(v_a_28_);
v_r_31_ = lean_box(v_res_30_);
return v_r_31_;
}
}
LEAN_EXPORT lean_object* lp_nebula___private_Init_Data_Array_Basic_0__Array_foldrMUnsafe_fold___at___00List_foldrTR___at___00Multiset_ndunion___at___00Nebula_query_spec__1_spec__3_spec__5(lean_object* v_n_32_, lean_object* v_as_33_, size_t v_i_34_, size_t v_stop_35_, lean_object* v_b_36_){
_start:
{
uint8_t v___x_37_; 
v___x_37_ = lean_usize_dec_eq(v_i_34_, v_stop_35_);
if (v___x_37_ == 0)
{
size_t v___x_38_; size_t v___x_39_; lean_object* v___x_40_; uint8_t v___x_41_; 
v___x_38_ = ((size_t)1ULL);
v___x_39_ = lean_usize_sub(v_i_34_, v___x_38_);
v___x_40_ = lean_array_uget_borrowed(v_as_33_, v___x_39_);
v___x_41_ = lp_nebula_List_elem___at___00Multiset_ndunion___at___00Nebula_query_spec__1_spec__2___redArg(v___x_40_, v_b_36_);
if (v___x_41_ == 0)
{
lean_object* v___x_42_; 
lean_inc(v___x_40_);
v___x_42_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_42_, 0, v___x_40_);
lean_ctor_set(v___x_42_, 1, v_b_36_);
v_i_34_ = v___x_39_;
v_b_36_ = v___x_42_;
goto _start;
}
else
{
v_i_34_ = v___x_39_;
goto _start;
}
}
else
{
return v_b_36_;
}
}
}
LEAN_EXPORT lean_object* lp_nebula___private_Init_Data_Array_Basic_0__Array_foldrMUnsafe_fold___at___00List_foldrTR___at___00Multiset_ndunion___at___00Nebula_query_spec__1_spec__3_spec__5___boxed(lean_object* v_n_45_, lean_object* v_as_46_, lean_object* v_i_47_, lean_object* v_stop_48_, lean_object* v_b_49_){
_start:
{
size_t v_i_boxed_50_; size_t v_stop_boxed_51_; lean_object* v_res_52_; 
v_i_boxed_50_ = lean_unbox_usize(v_i_47_);
lean_dec(v_i_47_);
v_stop_boxed_51_ = lean_unbox_usize(v_stop_48_);
lean_dec(v_stop_48_);
v_res_52_ = lp_nebula___private_Init_Data_Array_Basic_0__Array_foldrMUnsafe_fold___at___00List_foldrTR___at___00Multiset_ndunion___at___00Nebula_query_spec__1_spec__3_spec__5(v_n_45_, v_as_46_, v_i_boxed_50_, v_stop_boxed_51_, v_b_49_);
lean_dec_ref(v_as_46_);
lean_dec(v_n_45_);
return v_res_52_;
}
}
LEAN_EXPORT lean_object* lp_nebula_List_foldrTR___at___00Multiset_ndunion___at___00Nebula_query_spec__1_spec__3(lean_object* v_n_53_, lean_object* v_init_54_, lean_object* v_l_55_){
_start:
{
lean_object* v___x_56_; lean_object* v___x_57_; lean_object* v___x_58_; uint8_t v___x_59_; 
v___x_56_ = lean_array_mk(v_l_55_);
v___x_57_ = lean_array_get_size(v___x_56_);
v___x_58_ = lean_unsigned_to_nat(0u);
v___x_59_ = lean_nat_dec_lt(v___x_58_, v___x_57_);
if (v___x_59_ == 0)
{
lean_dec_ref(v___x_56_);
return v_init_54_;
}
else
{
size_t v___x_60_; size_t v___x_61_; lean_object* v___x_62_; 
v___x_60_ = lean_usize_of_nat(v___x_57_);
v___x_61_ = ((size_t)0ULL);
v___x_62_ = lp_nebula___private_Init_Data_Array_Basic_0__Array_foldrMUnsafe_fold___at___00List_foldrTR___at___00Multiset_ndunion___at___00Nebula_query_spec__1_spec__3_spec__5(v_n_53_, v___x_56_, v___x_60_, v___x_61_, v_init_54_);
lean_dec_ref(v___x_56_);
return v___x_62_;
}
}
}
LEAN_EXPORT lean_object* lp_nebula_List_foldrTR___at___00Multiset_ndunion___at___00Nebula_query_spec__1_spec__3___boxed(lean_object* v_n_63_, lean_object* v_init_64_, lean_object* v_l_65_){
_start:
{
lean_object* v_res_66_; 
v_res_66_ = lp_nebula_List_foldrTR___at___00Multiset_ndunion___at___00Nebula_query_spec__1_spec__3(v_n_63_, v_init_64_, v_l_65_);
lean_dec(v_n_63_);
return v_res_66_;
}
}
LEAN_EXPORT lean_object* lp_nebula_Multiset_ndunion___at___00Nebula_query_spec__1(lean_object* v_n_67_, lean_object* v_s_68_, lean_object* v_t_69_){
_start:
{
lean_object* v___x_70_; 
v___x_70_ = lp_nebula_List_foldrTR___at___00Multiset_ndunion___at___00Nebula_query_spec__1_spec__3(v_n_67_, v_t_69_, v_s_68_);
return v___x_70_;
}
}
LEAN_EXPORT lean_object* lp_nebula_Multiset_ndunion___at___00Nebula_query_spec__1___boxed(lean_object* v_n_71_, lean_object* v_s_72_, lean_object* v_t_73_){
_start:
{
lean_object* v_res_74_; 
v_res_74_ = lp_nebula_Multiset_ndunion___at___00Nebula_query_spec__1(v_n_71_, v_s_72_, v_t_73_);
lean_dec(v_n_71_);
return v_res_74_;
}
}
LEAN_EXPORT lean_object* lp_nebula___private_Init_Data_Array_Basic_0__Array_foldrMUnsafe_fold___at___00__private_Init_Data_List_Impl_0__List_eraseTR_go___at___00List_diff___at___00Multiset_sub___at___00Nebula_query_spec__0_spec__0_spec__1_spec__3(lean_object* v_as_75_, size_t v_i_76_, size_t v_stop_77_, lean_object* v_b_78_){
_start:
{
uint8_t v___x_79_; 
v___x_79_ = lean_usize_dec_eq(v_i_76_, v_stop_77_);
if (v___x_79_ == 0)
{
size_t v___x_80_; size_t v___x_81_; lean_object* v___x_82_; lean_object* v___x_83_; 
v___x_80_ = ((size_t)1ULL);
v___x_81_ = lean_usize_sub(v_i_76_, v___x_80_);
v___x_82_ = lean_array_uget_borrowed(v_as_75_, v___x_81_);
lean_inc(v___x_82_);
v___x_83_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_83_, 0, v___x_82_);
lean_ctor_set(v___x_83_, 1, v_b_78_);
v_i_76_ = v___x_81_;
v_b_78_ = v___x_83_;
goto _start;
}
else
{
return v_b_78_;
}
}
}
LEAN_EXPORT lean_object* lp_nebula___private_Init_Data_Array_Basic_0__Array_foldrMUnsafe_fold___at___00__private_Init_Data_List_Impl_0__List_eraseTR_go___at___00List_diff___at___00Multiset_sub___at___00Nebula_query_spec__0_spec__0_spec__1_spec__3___boxed(lean_object* v_as_85_, lean_object* v_i_86_, lean_object* v_stop_87_, lean_object* v_b_88_){
_start:
{
size_t v_i_boxed_89_; size_t v_stop_boxed_90_; lean_object* v_res_91_; 
v_i_boxed_89_ = lean_unbox_usize(v_i_86_);
lean_dec(v_i_86_);
v_stop_boxed_90_ = lean_unbox_usize(v_stop_87_);
lean_dec(v_stop_87_);
v_res_91_ = lp_nebula___private_Init_Data_Array_Basic_0__Array_foldrMUnsafe_fold___at___00__private_Init_Data_List_Impl_0__List_eraseTR_go___at___00List_diff___at___00Multiset_sub___at___00Nebula_query_spec__0_spec__0_spec__1_spec__3(v_as_85_, v_i_boxed_89_, v_stop_boxed_90_, v_b_88_);
lean_dec_ref(v_as_85_);
return v_res_91_;
}
}
LEAN_EXPORT lean_object* lp_nebula___private_Init_Data_List_Impl_0__List_eraseTR_go___at___00List_diff___at___00Multiset_sub___at___00Nebula_query_spec__0_spec__0_spec__1___redArg(lean_object* v_l_92_, lean_object* v_a_93_, lean_object* v_a_94_, lean_object* v_a_95_){
_start:
{
if (lean_obj_tag(v_a_94_) == 0)
{
lean_dec_ref(v_a_95_);
lean_inc(v_l_92_);
return v_l_92_;
}
else
{
lean_object* v_head_96_; lean_object* v_tail_97_; uint8_t v___x_98_; 
v_head_96_ = lean_ctor_get(v_a_94_, 0);
lean_inc(v_head_96_);
v_tail_97_ = lean_ctor_get(v_a_94_, 1);
lean_inc(v_tail_97_);
lean_dec_ref_known(v_a_94_, 2);
v___x_98_ = lean_nat_dec_eq(v_head_96_, v_a_93_);
if (v___x_98_ == 0)
{
lean_object* v___x_99_; 
v___x_99_ = lean_array_push(v_a_95_, v_head_96_);
v_a_94_ = v_tail_97_;
v_a_95_ = v___x_99_;
goto _start;
}
else
{
lean_object* v___x_101_; lean_object* v___x_102_; uint8_t v___x_103_; 
lean_dec(v_head_96_);
v___x_101_ = lean_array_get_size(v_a_95_);
v___x_102_ = lean_unsigned_to_nat(0u);
v___x_103_ = lean_nat_dec_lt(v___x_102_, v___x_101_);
if (v___x_103_ == 0)
{
lean_dec_ref(v_a_95_);
return v_tail_97_;
}
else
{
size_t v___x_104_; size_t v___x_105_; lean_object* v___x_106_; 
v___x_104_ = lean_usize_of_nat(v___x_101_);
v___x_105_ = ((size_t)0ULL);
v___x_106_ = lp_nebula___private_Init_Data_Array_Basic_0__Array_foldrMUnsafe_fold___at___00__private_Init_Data_List_Impl_0__List_eraseTR_go___at___00List_diff___at___00Multiset_sub___at___00Nebula_query_spec__0_spec__0_spec__1_spec__3(v_a_95_, v___x_104_, v___x_105_, v_tail_97_);
lean_dec_ref(v_a_95_);
return v___x_106_;
}
}
}
}
}
LEAN_EXPORT lean_object* lp_nebula___private_Init_Data_List_Impl_0__List_eraseTR_go___at___00List_diff___at___00Multiset_sub___at___00Nebula_query_spec__0_spec__0_spec__1___redArg___boxed(lean_object* v_l_107_, lean_object* v_a_108_, lean_object* v_a_109_, lean_object* v_a_110_){
_start:
{
lean_object* v_res_111_; 
v_res_111_ = lp_nebula___private_Init_Data_List_Impl_0__List_eraseTR_go___at___00List_diff___at___00Multiset_sub___at___00Nebula_query_spec__0_spec__0_spec__1___redArg(v_l_107_, v_a_108_, v_a_109_, v_a_110_);
lean_dec(v_a_108_);
lean_dec(v_l_107_);
return v_res_111_;
}
}
LEAN_EXPORT lean_object* lp_nebula_List_diff___at___00Multiset_sub___at___00Nebula_query_spec__0_spec__0(lean_object* v_n_114_, lean_object* v_x_115_, lean_object* v_x_116_){
_start:
{
if (lean_obj_tag(v_x_116_) == 0)
{
return v_x_115_;
}
else
{
lean_object* v_head_117_; lean_object* v_tail_118_; uint8_t v___x_119_; 
v_head_117_ = lean_ctor_get(v_x_116_, 0);
v_tail_118_ = lean_ctor_get(v_x_116_, 1);
v___x_119_ = lp_nebula_List_elem___at___00Multiset_ndunion___at___00Nebula_query_spec__1_spec__2___redArg(v_head_117_, v_x_115_);
if (v___x_119_ == 0)
{
v_x_116_ = v_tail_118_;
goto _start;
}
else
{
lean_object* v___x_121_; lean_object* v___x_122_; 
v___x_121_ = ((lean_object*)(lp_nebula_List_diff___at___00Multiset_sub___at___00Nebula_query_spec__0_spec__0___closed__0));
lean_inc(v_x_115_);
v___x_122_ = lp_nebula___private_Init_Data_List_Impl_0__List_eraseTR_go___at___00List_diff___at___00Multiset_sub___at___00Nebula_query_spec__0_spec__0_spec__1___redArg(v_x_115_, v_head_117_, v_x_115_, v___x_121_);
lean_dec(v_x_115_);
v_x_115_ = v___x_122_;
v_x_116_ = v_tail_118_;
goto _start;
}
}
}
}
LEAN_EXPORT lean_object* lp_nebula_List_diff___at___00Multiset_sub___at___00Nebula_query_spec__0_spec__0___boxed(lean_object* v_n_124_, lean_object* v_x_125_, lean_object* v_x_126_){
_start:
{
lean_object* v_res_127_; 
v_res_127_ = lp_nebula_List_diff___at___00Multiset_sub___at___00Nebula_query_spec__0_spec__0(v_n_124_, v_x_125_, v_x_126_);
lean_dec(v_x_126_);
lean_dec(v_n_124_);
return v_res_127_;
}
}
LEAN_EXPORT lean_object* lp_nebula_Multiset_sub___at___00Nebula_query_spec__0(lean_object* v_n_128_, lean_object* v_s_129_, lean_object* v_t_130_){
_start:
{
lean_object* v___x_131_; 
v___x_131_ = lp_nebula_List_diff___at___00Multiset_sub___at___00Nebula_query_spec__0_spec__0(v_n_128_, v_s_129_, v_t_130_);
return v___x_131_;
}
}
LEAN_EXPORT lean_object* lp_nebula_Multiset_sub___at___00Nebula_query_spec__0___boxed(lean_object* v_n_132_, lean_object* v_s_133_, lean_object* v_t_134_){
_start:
{
lean_object* v_res_135_; 
v_res_135_ = lp_nebula_Multiset_sub___at___00Nebula_query_spec__0(v_n_132_, v_s_133_, v_t_134_);
lean_dec(v_t_134_);
lean_dec(v_n_132_);
return v_res_135_;
}
}
LEAN_EXPORT lean_object* lp_nebula_Nebula_query(lean_object* v_n_136_, lean_object* v_C_137_, lean_object* v_00_u03bd_138_){
_start:
{
lean_object* v_off_139_; lean_object* v_on_140_; lean_object* v___x_141_; lean_object* v___x_142_; 
v_off_139_ = lean_ctor_get(v_00_u03bd_138_, 0);
lean_inc(v_off_139_);
v_on_140_ = lean_ctor_get(v_00_u03bd_138_, 1);
lean_inc(v_on_140_);
lean_dec_ref(v_00_u03bd_138_);
v___x_141_ = lp_nebula_List_diff___at___00Multiset_sub___at___00Nebula_query_spec__0_spec__0(v_n_136_, v_C_137_, v_off_139_);
lean_dec(v_off_139_);
v___x_142_ = lp_nebula_List_foldrTR___at___00Multiset_ndunion___at___00Nebula_query_spec__1_spec__3(v_n_136_, v_on_140_, v___x_141_);
return v___x_142_;
}
}
LEAN_EXPORT lean_object* lp_nebula_Nebula_query___boxed(lean_object* v_n_143_, lean_object* v_C_144_, lean_object* v_00_u03bd_145_){
_start:
{
lean_object* v_res_146_; 
v_res_146_ = lp_nebula_Nebula_query(v_n_143_, v_C_144_, v_00_u03bd_145_);
lean_dec(v_n_143_);
return v_res_146_;
}
}
LEAN_EXPORT uint8_t lp_nebula_List_elem___at___00Multiset_ndunion___at___00Nebula_query_spec__1_spec__2(lean_object* v_n_147_, lean_object* v_a_148_, lean_object* v_x_149_){
_start:
{
uint8_t v___x_150_; 
v___x_150_ = lp_nebula_List_elem___at___00Multiset_ndunion___at___00Nebula_query_spec__1_spec__2___redArg(v_a_148_, v_x_149_);
return v___x_150_;
}
}
LEAN_EXPORT lean_object* lp_nebula_List_elem___at___00Multiset_ndunion___at___00Nebula_query_spec__1_spec__2___boxed(lean_object* v_n_151_, lean_object* v_a_152_, lean_object* v_x_153_){
_start:
{
uint8_t v_res_154_; lean_object* v_r_155_; 
v_res_154_ = lp_nebula_List_elem___at___00Multiset_ndunion___at___00Nebula_query_spec__1_spec__2(v_n_151_, v_a_152_, v_x_153_);
lean_dec(v_x_153_);
lean_dec(v_a_152_);
lean_dec(v_n_151_);
v_r_155_ = lean_box(v_res_154_);
return v_r_155_;
}
}
LEAN_EXPORT lean_object* lp_nebula___private_Init_Data_List_Impl_0__List_eraseTR_go___at___00List_diff___at___00Multiset_sub___at___00Nebula_query_spec__0_spec__0_spec__1(lean_object* v_n_156_, lean_object* v_l_157_, lean_object* v_a_158_, lean_object* v_a_159_, lean_object* v_a_160_){
_start:
{
lean_object* v___x_161_; 
v___x_161_ = lp_nebula___private_Init_Data_List_Impl_0__List_eraseTR_go___at___00List_diff___at___00Multiset_sub___at___00Nebula_query_spec__0_spec__0_spec__1___redArg(v_l_157_, v_a_158_, v_a_159_, v_a_160_);
return v___x_161_;
}
}
LEAN_EXPORT lean_object* lp_nebula___private_Init_Data_List_Impl_0__List_eraseTR_go___at___00List_diff___at___00Multiset_sub___at___00Nebula_query_spec__0_spec__0_spec__1___boxed(lean_object* v_n_162_, lean_object* v_l_163_, lean_object* v_a_164_, lean_object* v_a_165_, lean_object* v_a_166_){
_start:
{
lean_object* v_res_167_; 
v_res_167_ = lp_nebula___private_Init_Data_List_Impl_0__List_eraseTR_go___at___00List_diff___at___00Multiset_sub___at___00Nebula_query_spec__0_spec__0_spec__1(v_n_162_, v_l_163_, v_a_164_, v_a_165_, v_a_166_);
lean_dec(v_a_164_);
lean_dec(v_l_163_);
lean_dec(v_n_162_);
return v_res_167_;
}
}
LEAN_EXPORT lean_object* lp_nebula_Nebula_onehotBlock___redArg(lean_object* v_q_168_, lean_object* v_i_169_, lean_object* v_s_170_){
_start:
{
lean_object* v___x_171_; lean_object* v___x_172_; lean_object* v___x_173_; lean_object* v___x_174_; 
v___x_171_ = lean_nat_mul(v_i_169_, v_q_168_);
v___x_172_ = lean_nat_add(v___x_171_, v_s_170_);
lean_dec(v___x_171_);
v___x_173_ = lean_box(0);
v___x_174_ = lean_alloc_ctor(1, 2, 0);
lean_ctor_set(v___x_174_, 0, v___x_172_);
lean_ctor_set(v___x_174_, 1, v___x_173_);
return v___x_174_;
}
}
LEAN_EXPORT lean_object* lp_nebula_Nebula_onehotBlock___redArg___boxed(lean_object* v_q_175_, lean_object* v_i_176_, lean_object* v_s_177_){
_start:
{
lean_object* v_res_178_; 
v_res_178_ = lp_nebula_Nebula_onehotBlock___redArg(v_q_175_, v_i_176_, v_s_177_);
lean_dec(v_s_177_);
lean_dec(v_i_176_);
lean_dec(v_q_175_);
return v_res_178_;
}
}
LEAN_EXPORT lean_object* lp_nebula_Nebula_onehotBlock(lean_object* v_N_179_, lean_object* v_q_180_, lean_object* v_i_181_, lean_object* v_s_182_){
_start:
{
lean_object* v___x_183_; 
v___x_183_ = lp_nebula_Nebula_onehotBlock___redArg(v_q_180_, v_i_181_, v_s_182_);
return v___x_183_;
}
}
LEAN_EXPORT lean_object* lp_nebula_Nebula_onehotBlock___boxed(lean_object* v_N_184_, lean_object* v_q_185_, lean_object* v_i_186_, lean_object* v_s_187_){
_start:
{
lean_object* v_res_188_; 
v_res_188_ = lp_nebula_Nebula_onehotBlock(v_N_184_, v_q_185_, v_i_186_, v_s_187_);
lean_dec(v_s_187_);
lean_dec(v_i_186_);
lean_dec(v_q_185_);
lean_dec(v_N_184_);
return v_res_188_;
}
}
LEAN_EXPORT lean_object* lp_nebula_Nebula_binExpand___lam__0(lean_object* v_u_189_, lean_object* v_q_190_, lean_object* v_i_191_){
_start:
{
lean_object* v___x_192_; lean_object* v___x_193_; 
lean_inc(v_i_191_);
v___x_192_ = lean_apply_1(v_u_189_, v_i_191_);
v___x_193_ = lp_nebula_Nebula_onehotBlock___redArg(v_q_190_, v_i_191_, v___x_192_);
lean_dec(v___x_192_);
lean_dec(v_i_191_);
return v___x_193_;
}
}
LEAN_EXPORT lean_object* lp_nebula_Nebula_binExpand___lam__0___boxed(lean_object* v_u_194_, lean_object* v_q_195_, lean_object* v_i_196_){
_start:
{
lean_object* v_res_197_; 
v_res_197_ = lp_nebula_Nebula_binExpand___lam__0(v_u_194_, v_q_195_, v_i_196_);
lean_dec(v_q_195_);
return v_res_197_;
}
}
LEAN_EXPORT lean_object* lp_nebula_Finset_biUnion___at___00Nebula_binExpand_spec__0___redArg___lam__0(lean_object* v_t_198_, lean_object* v_a_199_){
_start:
{
lean_object* v___x_200_; 
v___x_200_ = lean_apply_1(v_t_198_, v_a_199_);
return v___x_200_;
}
}
LEAN_EXPORT uint8_t lp_nebula_List_dedup___at___00Multiset_dedup___at___00Multiset_toFinset___at___00Finset_biUnion___at___00Nebula_binExpand_spec__0_spec__0_spec__1_spec__2___redArg___lam__0(lean_object* v_a_201_, lean_object* v_b_202_){
_start:
{
uint8_t v___x_203_; 
v___x_203_ = lean_nat_dec_eq(v_a_201_, v_b_202_);
if (v___x_203_ == 0)
{
uint8_t v___x_204_; 
v___x_204_ = 1;
return v___x_204_;
}
else
{
uint8_t v___x_205_; 
v___x_205_ = 0;
return v___x_205_;
}
}
}
LEAN_EXPORT lean_object* lp_nebula_List_dedup___at___00Multiset_dedup___at___00Multiset_toFinset___at___00Finset_biUnion___at___00Nebula_binExpand_spec__0_spec__0_spec__1_spec__2___redArg___lam__0___boxed(lean_object* v_a_206_, lean_object* v_b_207_){
_start:
{
uint8_t v_res_208_; lean_object* v_r_209_; 
v_res_208_ = lp_nebula_List_dedup___at___00Multiset_dedup___at___00Multiset_toFinset___at___00Finset_biUnion___at___00Nebula_binExpand_spec__0_spec__0_spec__1_spec__2___redArg___lam__0(v_a_206_, v_b_207_);
lean_dec(v_b_207_);
lean_dec(v_a_206_);
v_r_209_ = lean_box(v_res_208_);
return v_r_209_;
}
}
LEAN_EXPORT lean_object* lp_nebula_List_dedup___at___00Multiset_dedup___at___00Multiset_toFinset___at___00Finset_biUnion___at___00Nebula_binExpand_spec__0_spec__0_spec__1_spec__2___redArg(lean_object* v_l_211_){
_start:
{
lean_object* v___f_212_; lean_object* v___x_213_; 
v___f_212_ = ((lean_object*)(lp_nebula_List_dedup___at___00Multiset_dedup___at___00Multiset_toFinset___at___00Finset_biUnion___at___00Nebula_binExpand_spec__0_spec__0_spec__1_spec__2___redArg___closed__0));
v___x_213_ = lp_batteries_List_pwFilter___redArg(v___f_212_, v_l_211_);
return v___x_213_;
}
}
LEAN_EXPORT lean_object* lp_nebula_Multiset_dedup___at___00Multiset_toFinset___at___00Finset_biUnion___at___00Nebula_binExpand_spec__0_spec__0_spec__1(lean_object* v___x_214_, lean_object* v_s_215_){
_start:
{
lean_object* v___x_216_; 
v___x_216_ = lp_nebula_List_dedup___at___00Multiset_dedup___at___00Multiset_toFinset___at___00Finset_biUnion___at___00Nebula_binExpand_spec__0_spec__0_spec__1_spec__2___redArg(v_s_215_);
return v___x_216_;
}
}
LEAN_EXPORT lean_object* lp_nebula_Multiset_dedup___at___00Multiset_toFinset___at___00Finset_biUnion___at___00Nebula_binExpand_spec__0_spec__0_spec__1___boxed(lean_object* v___x_217_, lean_object* v_s_218_){
_start:
{
lean_object* v_res_219_; 
v_res_219_ = lp_nebula_Multiset_dedup___at___00Multiset_toFinset___at___00Finset_biUnion___at___00Nebula_binExpand_spec__0_spec__0_spec__1(v___x_217_, v_s_218_);
lean_dec(v___x_217_);
return v_res_219_;
}
}
LEAN_EXPORT lean_object* lp_nebula_Multiset_toFinset___at___00Finset_biUnion___at___00Nebula_binExpand_spec__0_spec__0(lean_object* v___x_220_, lean_object* v_s_221_){
_start:
{
lean_object* v___x_222_; 
v___x_222_ = lp_nebula_List_dedup___at___00Multiset_dedup___at___00Multiset_toFinset___at___00Finset_biUnion___at___00Nebula_binExpand_spec__0_spec__0_spec__1_spec__2___redArg(v_s_221_);
return v___x_222_;
}
}
LEAN_EXPORT lean_object* lp_nebula_Multiset_toFinset___at___00Finset_biUnion___at___00Nebula_binExpand_spec__0_spec__0___boxed(lean_object* v___x_223_, lean_object* v_s_224_){
_start:
{
lean_object* v_res_225_; 
v_res_225_ = lp_nebula_Multiset_toFinset___at___00Finset_biUnion___at___00Nebula_binExpand_spec__0_spec__0(v___x_223_, v_s_224_);
lean_dec(v___x_223_);
return v_res_225_;
}
}
LEAN_EXPORT lean_object* lp_nebula_Finset_biUnion___at___00Nebula_binExpand_spec__0___redArg(lean_object* v___x_226_, lean_object* v_s_227_, lean_object* v_t_228_){
_start:
{
lean_object* v___f_229_; lean_object* v___x_230_; lean_object* v___x_231_; 
v___f_229_ = lean_alloc_closure((void*)(lp_nebula_Finset_biUnion___at___00Nebula_binExpand_spec__0___redArg___lam__0), 2, 1);
lean_closure_set(v___f_229_, 0, v_t_228_);
v___x_230_ = lp_mathlib_Multiset_bind___redArg(v_s_227_, v___f_229_);
v___x_231_ = lp_nebula_List_dedup___at___00Multiset_dedup___at___00Multiset_toFinset___at___00Finset_biUnion___at___00Nebula_binExpand_spec__0_spec__0_spec__1_spec__2___redArg(v___x_230_);
return v___x_231_;
}
}
LEAN_EXPORT lean_object* lp_nebula_Finset_biUnion___at___00Nebula_binExpand_spec__0___redArg___boxed(lean_object* v___x_232_, lean_object* v_s_233_, lean_object* v_t_234_){
_start:
{
lean_object* v_res_235_; 
v_res_235_ = lp_nebula_Finset_biUnion___at___00Nebula_binExpand_spec__0___redArg(v___x_232_, v_s_233_, v_t_234_);
lean_dec(v___x_232_);
return v_res_235_;
}
}
LEAN_EXPORT lean_object* lp_nebula_Nebula_binExpand(lean_object* v_N_236_, lean_object* v_q_237_, lean_object* v_u_238_){
_start:
{
lean_object* v___f_239_; lean_object* v___x_240_; lean_object* v___x_241_; lean_object* v___x_242_; 
lean_inc(v_q_237_);
v___f_239_ = lean_alloc_closure((void*)(lp_nebula_Nebula_binExpand___lam__0___boxed), 3, 2);
lean_closure_set(v___f_239_, 0, v_u_238_);
lean_closure_set(v___f_239_, 1, v_q_237_);
v___x_240_ = lean_nat_mul(v_N_236_, v_q_237_);
lean_dec(v_q_237_);
v___x_241_ = l_List_finRange(v_N_236_);
v___x_242_ = lp_nebula_Finset_biUnion___at___00Nebula_binExpand_spec__0___redArg(v___x_240_, v___x_241_, v___f_239_);
lean_dec(v___x_240_);
return v___x_242_;
}
}
LEAN_EXPORT lean_object* lp_nebula_Finset_biUnion___at___00Nebula_binExpand_spec__0(lean_object* v___x_243_, lean_object* v_00_u03b1_244_, lean_object* v_s_245_, lean_object* v_t_246_){
_start:
{
lean_object* v___x_247_; 
v___x_247_ = lp_nebula_Finset_biUnion___at___00Nebula_binExpand_spec__0___redArg(v___x_243_, v_s_245_, v_t_246_);
return v___x_247_;
}
}
LEAN_EXPORT lean_object* lp_nebula_Finset_biUnion___at___00Nebula_binExpand_spec__0___boxed(lean_object* v___x_248_, lean_object* v_00_u03b1_249_, lean_object* v_s_250_, lean_object* v_t_251_){
_start:
{
lean_object* v_res_252_; 
v_res_252_ = lp_nebula_Finset_biUnion___at___00Nebula_binExpand_spec__0(v___x_248_, v_00_u03b1_249_, v_s_250_, v_t_251_);
lean_dec(v___x_248_);
return v_res_252_;
}
}
LEAN_EXPORT lean_object* lp_nebula_List_dedup___at___00Multiset_dedup___at___00Multiset_toFinset___at___00Finset_biUnion___at___00Nebula_binExpand_spec__0_spec__0_spec__1_spec__2(lean_object* v___x_253_, lean_object* v_l_254_){
_start:
{
lean_object* v___x_255_; 
v___x_255_ = lp_nebula_List_dedup___at___00Multiset_dedup___at___00Multiset_toFinset___at___00Finset_biUnion___at___00Nebula_binExpand_spec__0_spec__0_spec__1_spec__2___redArg(v_l_254_);
return v___x_255_;
}
}
LEAN_EXPORT lean_object* lp_nebula_List_dedup___at___00Multiset_dedup___at___00Multiset_toFinset___at___00Finset_biUnion___at___00Nebula_binExpand_spec__0_spec__0_spec__1_spec__2___boxed(lean_object* v___x_256_, lean_object* v_l_257_){
_start:
{
lean_object* v_res_258_; 
v_res_258_ = lp_nebula_List_dedup___at___00Multiset_dedup___at___00Multiset_toFinset___at___00Finset_biUnion___at___00Nebula_binExpand_spec__0_spec__0_spec__1_spec__2(v___x_256_, v_l_257_);
lean_dec(v___x_256_);
return v_res_258_;
}
}
LEAN_EXPORT lean_object* lp_nebula_Nebula_rotPos(lean_object* v_m_259_, lean_object* v_00_u03c1_260_, lean_object* v_i_261_){
_start:
{
lean_object* v___x_262_; lean_object* v___x_263_; 
v___x_262_ = lean_nat_add(v_i_261_, v_00_u03c1_260_);
v___x_263_ = lean_nat_mod(v___x_262_, v_m_259_);
lean_dec(v___x_262_);
return v___x_263_;
}
}
LEAN_EXPORT lean_object* lp_nebula_Nebula_rotPos___boxed(lean_object* v_m_264_, lean_object* v_00_u03c1_265_, lean_object* v_i_266_){
_start:
{
lean_object* v_res_267_; 
v_res_267_ = lp_nebula_Nebula_rotPos(v_m_264_, v_00_u03c1_265_, v_i_266_);
lean_dec(v_i_266_);
lean_dec(v_00_u03c1_265_);
lean_dec(v_m_264_);
return v_res_267_;
}
}
LEAN_EXPORT lean_object* lp_nebula_Finset_image___at___00Nebula_rotCode_spec__0___redArg(lean_object* v_f_268_, lean_object* v_s_269_){
_start:
{
lean_object* v___x_270_; lean_object* v___x_271_; 
v___x_270_ = lp_mathlib_Multiset_map___redArg(v_f_268_, v_s_269_);
v___x_271_ = lp_nebula_List_dedup___at___00Multiset_dedup___at___00Multiset_toFinset___at___00Finset_biUnion___at___00Nebula_binExpand_spec__0_spec__0_spec__1_spec__2___redArg(v___x_270_);
return v___x_271_;
}
}
LEAN_EXPORT lean_object* lp_nebula_Nebula_rotCode(lean_object* v_m_272_, lean_object* v_00_u03c1_273_, lean_object* v_C_274_){
_start:
{
lean_object* v___x_275_; lean_object* v___x_276_; 
v___x_275_ = lean_alloc_closure((void*)(lp_nebula_Nebula_rotPos___boxed), 3, 2);
lean_closure_set(v___x_275_, 0, v_m_272_);
lean_closure_set(v___x_275_, 1, v_00_u03c1_273_);
v___x_276_ = lp_nebula_Finset_image___at___00Nebula_rotCode_spec__0___redArg(v___x_275_, v_C_274_);
return v___x_276_;
}
}
LEAN_EXPORT lean_object* lp_nebula_Finset_image___at___00Nebula_rotCode_spec__0(lean_object* v_m_277_, lean_object* v_00_u03b1_278_, lean_object* v_f_279_, lean_object* v_s_280_){
_start:
{
lean_object* v___x_281_; 
v___x_281_ = lp_nebula_Finset_image___at___00Nebula_rotCode_spec__0___redArg(v_f_279_, v_s_280_);
return v___x_281_;
}
}
LEAN_EXPORT lean_object* lp_nebula_Finset_image___at___00Nebula_rotCode_spec__0___boxed(lean_object* v_m_282_, lean_object* v_00_u03b1_283_, lean_object* v_f_284_, lean_object* v_s_285_){
_start:
{
lean_object* v_res_286_; 
v_res_286_ = lp_nebula_Finset_image___at___00Nebula_rotCode_spec__0(v_m_282_, v_00_u03b1_283_, v_f_284_, v_s_285_);
lean_dec(v_m_282_);
return v_res_286_;
}
}
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_Init(uint8_t builtin);
lean_object* initialize_mathlib_Mathlib_Tactic(uint8_t builtin);
lean_object* initialize_mathlib_Mathlib_Data_Nat_Choose_Basic(uint8_t builtin);
lean_object* initialize_mathlib_Mathlib_Data_Nat_ModEq(uint8_t builtin);
void lean_initialize();
static bool _G_initialized = false;
LEAN_EXPORT lean_object* initialize_nebula_Nebula(uint8_t builtin) {
lean_object * res;
if (_G_initialized) return lean_io_result_mk_ok(lean_box(0));
_G_initialized = true;
lean_initialize();
res = initialize_Init(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_Init(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_mathlib_Mathlib_Tactic(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_mathlib_Mathlib_Data_Nat_Choose_Basic(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
res = initialize_mathlib_Mathlib_Data_Nat_ModEq(builtin);
if (lean_io_result_is_error(res)) return res;
lean_dec_ref(res);
return lean_io_result_mk_ok(lean_box(0));
}
#ifdef __cplusplus
}
#endif
