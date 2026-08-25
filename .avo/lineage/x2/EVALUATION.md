# evaluate.sh contract (defined by AVO run — no prior contract existed)

Score = weighted composite, printed as single float by bench/bench_all:
  S = 4*capacity_score + 4*noise_score + 1*throughput_score + 1*lean_score
- capacity_score: #codewords stored with FP<=1e-6 & recall>=0.99 under noise eta=10%,
  normalized against flat-SDR baseline run in same binary (score=ours/baseline).
- noise_score: max flip-rate eta achieving recall>=0.99 & FP<=1e-6 at fixed M,
  again ours/baseline.
- throughput_score: word-ops/sec of hot kernels vs baseline kernel.
- lean_score: 0 if `lake build` fails or absent; 1 if all proofs compile.
Hard rules: never weaken baseline to inflate score; baseline is honest flat SDR
(n=2048,k=40) and FSC uses matched total n,k unless a variant flag says otherwise.
