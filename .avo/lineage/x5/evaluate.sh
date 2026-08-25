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
g++ -O3 -march=native -std=c++17 -I include bench/scale_demo.cpp -o /tmp/opencode/scale_demo || { echo "SCORE: 0.0"; exit 1; }
g++ -O3 -march=native -std=c++17 -I include bench/bind_demo.cpp -o /tmp/opencode/bind_demo || { echo "SCORE: 0.0"; exit 1; }
{ /tmp/opencode/bench_all; echo "--- scale demo ---"; timeout 600 /tmp/opencode/scale_demo; \
  echo "--- binding demo ---"; timeout 300 /tmp/opencode/bind_demo; } | tee /tmp/opencode/bench.log

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
# Contract v2 (EVALUATION.md): gates + open-axis points. Gates fail hard;
# scored axes measure OPEN problems so the score retains selection signal.
LEAN_OK=$LEAN_OK python3 - <<'PY'
import re, os, sys
log = open('/tmp/opencode/bench.log').read()
brain = open('/tmp/opencode/brain.log').read()
stlog = open('/tmp/opencode/selftest.log').read()

gates = []
gates.append(('purity', 'PURITY: PASS' in open('/dev/stdin').read() if False else True))  # purity already gated by exit
gates.append(('selftest', 'SELFTEST: ALL PASS' in stlog))
gates.append(('brain_demo', 'BRAIN DEMO: ALL PASS' in brain))
lean = 1 if os.environ.get('LEAN_OK') == '1' else 0
gates.append(('lean_no_sorryax', lean == 1))

ladder = re.findall(r'LADDER,(\d+),(\d+),(\d+),(\d+),(\d+),(\d+),(\d+),(\d+)', log)
zero_wrong = bool(ladder) and all(int(r[4]) == 0 for r in ladder)
gates.append(('ladder_zero_wrong', zero_wrong))

failed = [name for name, ok in gates if not ok]
if failed:
    print('GATES FAILED:', ', '.join(failed))
    print('SCORE: 0.00')
    sys.exit(0)

# --- Axis C: ladder service quality (absolute performance term) ---
# area of served fraction (exact+category)/200 across flip budgets
serv = []
for r in ladder:
    flips = int(r[0])
    served = (int(r[1]) + int(r[2])) / 200.0
    serv.append((flips, served))
serv.sort()
# trapezoid over [20,100], normalized to 1.0
area = 0.0
for (f0, s0), (f1, s1) in zip(serv, serv[1:]):
    area += (s0 + s1) / 2.0 * (f1 - f0)
span = serv[-1][0] - serv[0][0] if len(serv) > 1 else 1
axisC = min(1.0, area / max(span, 1))

# --- Axis D: throughput at M=10^6 ---
m = re.search(r'THR,M=1000000,.*speedup=([\d.]+)x', log)
axisD = min(1.0, float(m.group(1)) / 500.0) if m else 0.0

# --- Axis A: empirical scale demo (n=65536) — parsed from SCALE lines ---
scale_ok = re.search(r'SCALE,codes=(\d+),recall=([\d.]+),wrong=(\d+),', log)
axisA = 0.0
if scale_ok:
    codes = int(scale_ok.group(1)); recall = float(scale_ok.group(2))
    wrong = int(scale_ok.group(3))
    if codes >= 100000 and wrong == 0:
        axisA = min(1.0, recall) * min(1.0, codes / 100000.0)

# --- Axis B: role-binding composition — parsed from BIND lines ---
bind = re.findall(r'BIND,K=(\d+),wrong=(\d+),trials=(\d+),min_true_ov=(\d+),max_crosstalk=(\d+),predicted_floor=(\d+)', log)
axisB = 0.0
if bind:
    ok = all(int(w) == 0 for _, w, _, _, _, _ in bind)
    ratios = [min(1.0, float(mt) / max(1, float(mp)))
              for _, _, _, mt, _, mp in bind]
    axisB = (1.0 if ok else 0.0) * (sum(ratios) / len(ratios))

# --- Axis E: Lean premise completeness (GF/RS formalization) ---
# reserved; currently 0 until proofs/GF_RS.lean exists and compiles
axisE = 0.0

score = 3*axisA + 2*axisB + 2*axisC + 1*axisD + 2*axisE
print(f"GATES: all pass ({len(gates)})")
print(f"AXES: A_scale={axisA:.2f}/3 B_binding={axisB:.2f}/2 C_ladder={axisC:.2f}/2 "
      f"D_throughput={axisD:.2f}/1 E_lean_gfrs={axisE:.2f}/2")
print(f"SCORE: {score:.2f}   (max 10; gates are pass/fail on top)")
PY
