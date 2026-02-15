#include <vector>
#include <cmath>
#include <chrono>
#include <iostream>
#include <numeric>
#include <random>
#include <algorithm> // For std::sort

const double real = 0.45969769413186023;

double compute(const std::vector<double> &xs)
{
    double s = 0.0;
    for (size_t i = 0; i < xs.size(); ++i)
    {
        s += std::sin(xs[i]);
    }
    return s / xs.size();
}

double measure(const std::vector<double> &xs)
{
    auto start = std::chrono::high_resolution_clock::now();
    volatile double result = compute(xs); // Avoid optimizing away the compute call
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> diff = end - start;
    return diff.count(); // ms
}

std::pair<double, double> bootstrap_ci(
    const std::vector<double> &times,
    int n_boot = 1000)
{
    std::mt19937 rng(123);
    std::uniform_int_distribution<int> dist(0, times.size() - 1);

    std::vector<double> medians;
    medians.reserve(n_boot);

    std::vector<double> sample(times.size());

    for (int b = 0; b < n_boot; ++b)
    {
        // Resampling with replacement
        for (size_t i = 0; i < times.size(); ++i)
            sample[i] = times[dist(rng)];

        // Compute median of the sample
        std::sort(sample.begin(), sample.end());
        medians.push_back(sample[sample.size() / 2]);
    }

    std::sort(medians.begin(), medians.end());

    double lo = medians[n_boot * 0.025];
    double hi = medians[n_boot * 0.975];

    return {lo, hi};
}

int main()
{
    std::vector<size_t> Ns = {100000, 1000000, 10000000};
    const int trials = 200; // Same number of trials with Python for consistency

    for (size_t N : Ns)
    {
        std::mt19937 gen(42);
        std::uniform_real_distribution<double> dist(0.0, 1.0);

        std::cout << "N = " << N << "\n";

        // ---- Generate random data ----
        std::vector<double> xs(N);
        for (size_t i = 0; i < N; ++i)
            xs[i] = dist(gen);

        // ---- Warmup ----
        compute(xs);
        double warm = compute(xs);
        std::cout << "Warmup Result: " << warm
                  << ", Abs Error: " << std::abs(warm - real) << "\n";

        // ---- Measurement ----
        std::vector<double> times;
        times.reserve(trials);

        for (int t = 0; t < trials; ++t)
        {
            times.push_back(measure(xs));
        }

        // ---- Statistics (median-based) ----
        std::sort(times.begin(), times.end());

        double median;
        if (trials % 2 == 0)
            median = (times[trials / 2 - 1] + times[trials / 2]) / 2.0;
        else
            median = times[trials / 2];

        double min_v = times.front();

        // Percentiles (simple implementation)
        auto idx90 = static_cast<size_t>(trials * 0.90);
        auto idx95 = static_cast<size_t>(trials * 0.95);

        double p90 = times[idx90];
        double p95 = times[idx95];
        auto [ci_lo, ci_hi] = bootstrap_ci(times);

        std::cout << "Median (ms): " << median << "\n";
        std::cout << "Min (ms):    " << min_v << "\n";
        std::cout << "P90 (ms):    " << p90 << "\n";
        std::cout << "P95 (ms):    " << p95 << "\n";
        std::cout << "95% CI Median (ms): ["
                  << ci_lo << ", " << ci_hi << "]\n\n";
    }

    return 0;
}
