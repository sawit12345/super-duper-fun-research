// Axis A: empirical scale demonstration - 8KB-class codes (n=63488), 10^5
// memories, certified-radius dropout, algebraic decode. Makes the ">10^900
// certified memories" claim empirical rather than formula arithmetic.
#include <nebula/code.hpp>
#include <chrono>
#include <cstdio>
#include <vector>
#include <cmath>

using namespace nebula;
using Clock = std::chrono::steady_clock;

int main() {
    // GF(256): N <= 255. N=248 symbols -> n = 248*256 = 63488 bits (~7.8 KB).
    const unsigned q = 256, N = 248, K = 124;
    const unsigned n = N * q;
    const unsigned MW = n / 64;               // 992 words
    const unsigned E_STAR = N - K;            // 124 certified flip radius

    printf("# scale: n=%u (%.1f KB/code) w=%u K=%u e_star=%u lambda_max=%u\n",
           n, n / 8192.0, N, K, E_STAR, K - 1);
    printf("SCALE,capacity_log10M=%.1f\n",
           (N - (double)E_STAR) * log10((double)q));

    NsscSpace<8, 0x1D> sp(n, q);
    sp.init_k(K);

    const unsigned M = 100000;
    Dictionary dict;
    dict.alloc(M, MW);                        // ~790 MB
    u32 msg[1100];
    Rng rng(0xBEEF);
    auto t0 = Clock::now();
    for (unsigned i = 0; i < M; ++i)
        sp.encode_random(rng, msg, dict.code(i));
    printf("# built %u codes in %.1fs\n", M,
           std::chrono::duration<double>(Clock::now() - t0).count());

    // Certified-radius dropout queries, decoded ALGEBRAICALLY (no scan).
    const unsigned TRIALS = 300;
    unsigned correct = 0, unknown = 0, wrong = 0;
    std::vector<u64> query(MW), out(MW);
    NoiseEngine ne; ne.init(n);
    t0 = Clock::now();
    for (unsigned t = 0; t < TRIALS; ++t) {
        unsigned target = rng.next() % M;
        copy_words(query.data(), dict.code(target), MW);
        ne.apply_uniform(query.data(), n, E_STAR, rng);
        if (sp.decode_algebraic(query.data(), out.data(), msg)) {
            if (overlap_words(out.data(), dict.code(target), MW) == N) ++correct;
            else ++wrong;
        } else ++unknown;
    }
    double alg_s = std::chrono::duration<double>(Clock::now() - t0).count();

    // Subsampled scan baseline: scan a 2000-code subset per query.
    const unsigned SUB = 2000;
    Dictionary sub; sub.alloc(SUB, MW);
    for (unsigned i = 0; i < SUB; ++i)
        copy_words(sub.code(i), dict.code(rng.next() % M), MW);
    t0 = Clock::now();
    volatile int sink = 0;
    for (unsigned t = 0; t < TRIALS; ++t)
        sink += decode_scan(sub, query.data(), N / 2);
    double scan_s = std::chrono::duration<double>(Clock::now() - t0).count();
    (void)sink;

    printf("SCALE,codes=%u,recall=%.4f,wrong=%u,unknown=%u,"
           "alg_us_per_q=%.1f,scan2000_us_per_q=%.1f\n",
           M, (double)correct / TRIALS, wrong, unknown,
           alg_s / TRIALS * 1e6, scan_s / TRIALS * 1e6);
    return 0;
}
