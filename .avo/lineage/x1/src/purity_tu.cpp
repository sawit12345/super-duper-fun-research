// Purity audit translation unit: instantiates every kernel template/function
// so scripts/check_kernel_asm.sh can disassemble and verify the no-mult/div/
// float constraint on real emitted instructions.
#include <nebula/code.hpp>
using namespace nebula;

// Force instantiation of the deployed configurations.
template struct nebula::GF<6, 0x03>;
template struct nebula::GF<8, 0x1D>;
template struct nebula::RS<6, 0x03>;
template struct nebula::RS<8, 0x1D>;
template struct nebula::NsscSpace<6, 0x03>;
template struct nebula::NsscSpace<8, 0x1D>;

unsigned audit_overlap(const u64* a, const u64* b, size_t w) {
    return overlap_words(a, b, w);
}
int audit_scan(const Dictionary* d, const u64* q, unsigned th) {
    return decode_scan(*d, q, th);
}
bool audit_alg(NsscSpace<8, 0x1D>* sp, const u64* q, u64* o, u32* m) {
    return sp->decode_algebraic(q, o, m);
}
void audit_noise(NoiseEngine* ne, u64* q, const u64* self, const u64* riv,
                 unsigned n, unsigned t, unsigned f, Rng* rng) {
    ne->apply_uniform(q, n, t, *rng);
    ne->apply_adversarial(q, self, riv, n, t, f, *rng);
}
