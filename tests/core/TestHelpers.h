#pragma once

#include <vector>
#include <cmath>
#include <cstdlib>

namespace QIM {
namespace TestHelpers {

// Generate linear x values: 0, 1, 2, ..., n-1
inline std::vector<double> makeLinearXs(int n)
{
    std::vector<double> xs(static_cast<size_t>(n));
    for (int i = 0; i < n; ++i)
        xs[static_cast<size_t>(i)] = static_cast<double>(i);
    return xs;
}

// Generate linear y values: 0, 2, 4, ..., 2*(n-1)  (y = 2*x)
inline std::vector<double> makeLinearYs(int n)
{
    std::vector<double> ys(static_cast<size_t>(n));
    for (int i = 0; i < n; ++i)
        ys[static_cast<size_t>(i)] = 2.0 * static_cast<double>(i);
    return ys;
}

// Generate sine wave data: y = sin(freq * x)
// freq defaults to 0.1, producing ~6.3 complete cycles for n=100
inline std::vector<double> makeSinWave(int n, double freq = 0.1)
{
    std::vector<double> ys(static_cast<size_t>(n));
    for (int i = 0; i < n; ++i) {
        double x = static_cast<double>(i);
        ys[static_cast<size_t>(i)] = std::sin(freq * x);
    }
    return ys;
}

// Generate random walk data starting from seed, with step size ~0.1-1.0
// Uses deterministic std::rand (seeded externally in test init if needed)
inline std::vector<double> makeRandomWalk(int n, double seed = 0.0)
{
    std::vector<double> ys(static_cast<size_t>(n));
    ys[0] = seed;
    for (int i = 1; i < n; ++i) {
        double step = static_cast<double>(std::rand()) / RAND_MAX * 2.0 - 1.0;
        ys[static_cast<size_t>(i)] = ys[static_cast<size_t>(i - 1)] + step;
    }
    return ys;
}

} // namespace TestHelpers
} // namespace QIM
