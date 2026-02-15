import numpy as np

from config import Ns, trials, REAL, measure, bootstrap_ci

def compute_numpy(xs):
    return np.sin(xs).mean()

for N in Ns:
    print(f"N = {N}")

    np.random.seed(42)
    xs = np.random.rand(N)

    compute_numpy(xs)
    warm = compute_numpy(xs)
    print(f"Warmup Result: {warm}, Abs Error: {abs(warm - REAL)}")

    times = []
    for _ in range(trials):
        times.append(measure(xs, compute_numpy))

    times = np.array(times)

    median = np.median(times)
    min_v  = np.min(times)
    p90    = np.percentile(times, 90)
    p95    = np.percentile(times, 95)
    low, high = bootstrap_ci(times)

    print(f"Median (ms): {median:.3f}")
    print(f"Min (ms):    {min_v:.3f}")
    print(f"P90 (ms):    {p90:.3f}")
    print(f"P95 (ms):    {p95:.3f}\n")
    print(f"Bootstrap CI (95%) (ms): [{low:.3f}, {high:.3f}]\n")
