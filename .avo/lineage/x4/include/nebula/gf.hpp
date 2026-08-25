// NEBULA kernel — GF(2^m) arithmetic.
// HARD CONSTRAINTS: bitwise ops (+,-,comparisons,shifts,rotates,AND/OR/XOR/NOT,
// popcount) only. NO multiplication, NO division, NO modulo, NO floats.
#pragma once
#include <cstdint>
#include <cstddef>

namespace nebula {

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

// ---------------------------------------------------------------- GF(2^m) --
// Elements are m-bit patterns. Field defined by primitive polynomial poly
// (bit m set implicit). For m=6: x^6+x+1 -> low poly bits 0x43.
template <unsigned M, unsigned POLY>
struct GF {
    static constexpr unsigned m = M;
    static constexpr u32     q = 1u << M;          // field size
    static constexpr u32     mask = q - 1;
    static constexpr u32     high = q;             // bit m set
    static constexpr u32     lowpoly = POLY;       // LOW m bits only (top bit implicit)

    // Multiply: russian-peasant carry-less. Shifts + XOR + AND only.
    static inline u32 mul(u32 a, u32 b) {
        u32 r = 0;
        while (b) {
            if (b & 1u) r ^= a;
            b >>= 1;
            u32 carry = a >> (M - 1);      // bit that will overflow
            a <<= 1;
            a &= mask;                     // keep in field
            if (carry) a ^= lowpoly;       // reduce by primitive poly
        }
        return r;
    }
    // Multiply by generator alpha (=x = element 2): single-step version.
    static constexpr u32 mul_alpha(u32 a) {
        u32 carry = a >> (M - 1);
        a = (a << 1) & mask;
        if (carry) a ^= lowpoly;
        return a;
    }

    // exp/log tables: exp[i] = alpha^i, i in [0, q-2]; log[x] = i for x != 0.
    // Built with mul_alpha only (shift/XOR). Setup-time (not hot path).
    struct Tables {
        u32 exp[q];
        u32 log[q];
        constexpr Tables() : exp{}, log{} {
            u32 v = 1;
            for (u32 i = 0; i < q - 1; ++i) {
                exp[i] = v;
                log[v] = i;
                v = mul_alpha(v);
            }
            // v returned to 1 iff alpha is primitive (self-test asserts).
        }
    };
    static inline constexpr Tables T{};

    // Cyclic access alpha^i; valid for i < 2*(q-1) (single subtract, no div).
    static inline u32 exp_t(u32 i) {
        return T.exp[i >= q - 1 ? i - (q - 1) : i];
    }
    static inline u32 log_t(u32 x)  { return T.log[x]; }           // x!=0

    // Division/mul via log tables: table lookups, no CPU divide.
    static inline u32 mul_lut(u32 a, u32 b) {
        if (a == 0 || b == 0) return 0;
        return exp_t(T.log[a] + T.log[b]);              // index add only
    }
    static inline u32 div_lut(u32 a, u32 b) {           // b != 0
        if (a == 0) return 0;
        return exp_t(T.log[a] + (q - 1) - T.log[b]);
    }
    static inline u32 inv_lut(u32 a) {
        if (a == 0) return 0;
        return exp_t((q - 1) - T.log[a]);
    }
};

using GF64  = GF<6, 0x03>;   // x^6+x+1 -> low bits 0x03 (primitive)
using GF256 = GF<8, 0x1D>;   // x^8+x^4+x^3+x^2+1 -> low bits 0x1D (primitive)

// ------------------------------------------------------------- bit words --
inline unsigned popcnt_u64(u64 x) { return (unsigned)__builtin_popcountll(x); }

} // namespace nebula
