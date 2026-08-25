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

# --- noise score: degradation-ladder dominance (dropout regime, M=1e5) ---
# Requirements: NSSC never wrong at ANY level; at flips=60 NSSC still serves
# (exact+category) >= 95% of queries while flat SDR serves <= 10% exactly.
noise = 0.0
ladder = re.findall(r'LADDER,(\d+),(\d+),(\d+),(\d+),(\d+),(\d+),(\d+),(\d+)', log)
if ladder:
    wrongs = [int(r[4]) for r in ladder]
    served60 = None; sdr_served60 = None
    for r in ladder:
        if int(r[0]) == 60:
            served60 = (int(r[1]) + int(r[2])) / 200.0
            sdr_served60 = int(r[5]) / 200.0
    if max(wrongs) == 0 and served60 is not None:
        noise = min(1.0, served60) * (1.0 - min(1.0, sdr_served60 * 10.0))

# --- capacity score: certified capacity formula + served-M dominance ---
mcap = re.search(r'CAP,formula=M=q\^\(w-e\*\),log10_M_certified_at_e_star=(\d+),log10_value=([\d.]+)', log)
cap = 0.0
if mcap and noise > 0.5:
    # certified dictionary dwarfs any practically-scannable SDR dictionary
    # that survives the same noise; log-scale headroom normalized to 10^6
    cap = min(1.0, float(mcap.group(2)) / 6.0)

# --- throughput score: algebraic vs scan speedup at M=100000 ---
m = re.search(r'THR,M=1000000,.*speedup=([\d.]+)x', log)
thr = min(1.0, float(m.group(1)) / 500.0) if m else 0.0

lean = float(os.environ.get('LEAN_OK', '0'))
score = 4 * cap + 4 * noise + 1 * thr + 1 * lean
print(f"COMPONENTS: capacity={cap:.3f} noise={noise:.3f} throughput={thr:.3f} lean={lean:.1f}")
print(f"SCORE: {score:.2f}   (max 10 = 4cap + 4noise + 1thr + 1lean)")
PY
