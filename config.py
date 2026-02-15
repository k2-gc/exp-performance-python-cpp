import time
import numpy as np

Ns = [100_000, 1_000_000, 10_000_000]
trials = 200
REAL = 0.45969769413186023

def measure(xs, func):
    start = time.perf_counter()
    result = func(xs)
    end = time.perf_counter()
    return (end - start) * 1000  # ms

def bootstrap_ci(data, n_boot=1000, ci=95):
    medians = []
    n = len(data)

    for _ in range(n_boot):
        sample = np.random.choice(data, n, replace=True)
        medians.append(np.median(sample))

    low = np.percentile(medians, (100 - ci) / 2)
    high = np.percentile(medians, 100 - (100 - ci) / 2)

    return low, high
