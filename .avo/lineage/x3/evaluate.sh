#!/usr/bin/env bash
# NEBULA evaluation driver — the scoring contract for this research repo.
# Gates: kernel purity audit, C++ self-test, Lean build.
# Metrics: noise robustness, certified capacity, decode throughput vs flat SDR.
set -u
cd "$(dirname "$0")"
mkdir -p /tmp/opencode
export PATH="$HOME/.elan/bin:$PATH"

echo "=== [1/4] kernel purity audit ==="
if ! ./scripts/check_kernel_asm.sh; then
    echo "SCORE: 0.0 (purity FAILED)"; exit 1
fi

echo "=== [2/4] C++ self-test ==="
g++ -O2 -std=c++17 -I include bench/selftest.cpp -o /tmp/opencode/selftest || { echo "SCORE: 0.0"; exit 1; }
if ! /tmp/opencode/selftest > /tmp/opencode/selftest.log; then
    tail -5 /tmp/opencode/selftest.log
    echo "SCORE: 0.0 (selftest FAILED)"; exit 1
fi
grep -E "SELFTEST|max pairwise|certified radius" /tmp/opencode/selftest.log

echo "=== [2b/4] whole-brain loop demo ==="
g++ -O2 -std=c++17 -I include bench/brain_demo.cpp -o /tmp/opencode/brain_demo || { echo "SCORE: 0.0"; exit 1; }
if ! timeout 120 /tmp/opencode/brain_demo > /tmp/opencode/brain.log; then
    tail -5 /tmp/opencode/brain.log
    echo "SCORE: 0.0 (brain demo FAILED)"; exit 1
fi
grep -E "recall|PASS|FAIL" /tmp/opencode/brain.log | head -10

echo "=== [3/4] benchmark: NSSC vs flat SDR (matched n,w) ==="
g++ -O3 -march=native -std=c++17 -I include bench/bench_all.cpp -o /tmp/opencode/bench_all || { echo "SCORE: 0.0"; exit 1; }
/tmp/opencode/bench_all | tee /tmp/opencode/bench.log

echo "=== [4/4] Lean proofs ==="
LEAN_OK=0
if command -v lake >/dev/null 2>&1 || [ -x "$HOME/.elan/bin/lake" ]; then
    if (cd proofs && lake build) >/tmp/opencode/lean.log 2>&1; then
        if grep -q "sorryAx" /tmp/opencode/lean.log; then
            echo "lean: contains sorryAx — not fully verified"
        else
            LEAN_OK=1
            echo "lean: all proofs compile, no sorryAx — fully verified"
        fi
    else
        echo "lean: BUILD FAILED"; tail -5 /tmp/opencode/lean.log
    fi
else
    echo "lean: toolchain not found"
fi

# ------------------------------------------------------------- scoring --
LEAN_OK=$LEAN_OK python3 - <<'PY'
import re, os, math
log = open('/tmp/opencode/bench.log').read()

# --- noise score: recall gap at eta=20%, largest M; NSSC must have zero wrongs ---
noise = 0.0
rows = re.findall(r'UNI,M=(\d+),eta=20,NSSC recall=([\d.]+) wrong=([\d.e+-]+).*SDR recall=([\d.]+) wrong=([\d.e+-]+)', log)
biggest = None
for M, rn, wn, rs_, ws in rows:
    if int(M) == 1000000:
        biggest = (float(rn), float(wn), float(rs_), float(ws))
if biggest:
    rn, wn, rs_, ws = biggest
    if wn == 0.0:
        noise = max(0.0, min(1.0, rn - rs_))

# --- capacity score: certified usable-M ratio under eta=20% (zero-wrong) ---
nssc_ok = sdr_ok = 0
for M, blk in re.findall(r'UNI,M=(\d+),eta=20,(.*)', log):
    nssc_part, sdr_part = blk.split('|')
    if 'wrong=0.00e+00' in nssc_part: nssc_ok = max(nssc_ok, int(M))
    if 'wrong=0.00e+00' in sdr_part:  sdr_ok  = max(sdr_ok, int(M))
cap = 1.0 if sdr_ok == 0 else min(1.0, math.log10(max(nssc_ok, 1) / sdr_ok) / 3.0)

# --- throughput score: algebraic vs scan speedup at M=100000 ---
m = re.search(r'THR,M=1000000,.*speedup=([\d.]+)x', log)
thr = min(1.0, float(m.group(1)) / 500.0) if m else 0.0

lean = float(os.environ.get('LEAN_OK', '0'))
score = 4 * cap + 4 * noise + 1 * thr + 1 * lean
print(f"COMPONENTS: capacity={cap:.3f} noise={noise:.3f} throughput={thr:.3f} lean={lean:.1f}")
print(f"SCORE: {score:.2f}   (max 10 = 4cap + 4noise + 1thr + 1lean)")
PY
