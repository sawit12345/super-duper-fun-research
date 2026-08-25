#!/usr/bin/env bash
# Disassembly purity audit: NEBULA kernel must contain NO multiply, divide,
# remainder, or floating-point instructions. Bitwise ops, add/sub, shifts,
# popcount, and table lookups only. Run from repo root.
set -u
OBJ=/tmp/opencode/purity_tu.o
g++ -O3 -march=native -std=c++17 -I include -fno-inline-functions \
    -c src/purity_tu.cpp -o "$OBJ" || { echo "PURITY: compile failed"; exit 1; }

# Extract disassembly of functions whose demangled names start with nebula::
objdump -d -C "$OBJ" | awk '
    /^[0-9a-f]+ </ { inb = ($2 ~ /^<(_ZN6nebula|nebula::)/) }
    inb { print }
' > /tmp/opencode/nebula_asm.txt

if [ ! -s /tmp/opencode/nebula_asm.txt ]; then
    echo "PURITY: no nebula symbols found (naming mismatch)"; exit 1
fi

# Inspect only real instruction mnemonics (3rd tab-field of objdump lines),
# so symbol names like GF::mul(...) in jump annotations cannot false-positive.
awk -F'\t' '{ if (NF >= 3 && $1 ~ /:$/) { split($3, w, " "); print w[1] } }' \
    /tmp/opencode/nebula_asm.txt > /tmp/opencode/mnemonics.txt
BAD=$(grep -Ec '^(mul|imul|div|idiv)$' /tmp/opencode/mnemonics.txt || true)
FLOAT=$(grep -Ec '^(movss|movsd|addss|addsd|subss|subsd|mulss|mulsd|divss|divsd|cvtsi2s|cvtsi2sd|cvtts2si|x87|fld|fmul|fdiv|fadd)' /tmp/opencode/mnemonics.txt || true)

echo "PURITY: nebula:: instructions analyzed: $(wc -l < /tmp/opencode/nebula_asm.txt)"
echo "PURITY: integer mul/div instructions: $BAD"
echo "PURITY: floating-point instructions:  $FLOAT"
if [ "$BAD" -eq 0 ] && [ "$FLOAT" -eq 0 ]; then
    echo "PURITY: PASS — kernel is pure bitwise/arithmetic (popcount allowed)"
    exit 0
else
    echo "PURITY: FAIL"
    exit 1
fi
