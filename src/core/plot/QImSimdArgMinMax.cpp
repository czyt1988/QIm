#include "QImSimdArgMinMax.h"
#include <cfloat>
#include <cmath>
#include <cstddef>

// Platform-specific intrinsics headers
#if defined(_MSC_VER)
    #include <intrin.h>
#elif defined(__GNUC__) || defined(__clang__)
    #include <x86intrin.h>
#endif

namespace QIM
{

// ===== Internal: Runtime CPU feature detection =====
namespace {

enum SimdLevel { Scalar, SSE42, AVX2 };

SimdLevel detectSimdLevel()
{
#if defined(Q_OS_WIN)
    // MSVC: __cpuid / __cpuidex intrinsics
    int cpuinfo[4];
    __cpuid(cpuinfo, 0);
    if (cpuinfo[0] >= 7) {
        __cpuidex(cpuinfo, 7, 0);
        // AVX2: leaf 7, sub-leaf 0, EBX bit 5
        if (cpuinfo[1] & (1 << 5)) return AVX2;
    }
    __cpuid(cpuinfo, 1);
    // SSE4.2: leaf 1, ECX bit 20
    if (cpuinfo[2] & (1 << 20)) return SSE42;
    return Scalar;
#elif defined(Q_OS_LINUX) || defined(Q_OS_MAC)
#if defined(__GNUC__)
    // GCC/Clang: __builtin_cpu_supports
    __builtin_cpu_init();
    if (__builtin_cpu_supports("avx2")) return AVX2;
    if (__builtin_cpu_supports("sse4.2")) return SSE42;
#endif
    return Scalar;
#else
    return Scalar;
#endif
}

// Cache detection result — evaluated once at program startup
static const SimdLevel kSimdLevel = detectSimdLevel();

}  // namespace

// ===== Scalar fallback implementation =====
static ArgMinMaxResult argMinMaxScalar(const double* data, int count)
{
    int min_idx = 0;
    int max_idx = 0;
    double min_val = data[0];
    double max_val = data[0];

    for (int i = 1; i < count; ++i) {
        if (data[i] < min_val) {
            min_val = data[i];
            min_idx = i;
        }
        if (data[i] > max_val) {
            max_val = data[i];
            max_idx = i;
        }
    }

    return {min_idx, max_idx, min_val, max_val};
}

// ===== SSE4.2 implementation =====
#if defined(__SSE4_2__) || (defined(_MSC_VER) && _MSC_VER >= 1700)

static ArgMinMaxResult argMinMaxSSE42(const double* data, int count)
{
    __m128d min_vec = _mm_set1_pd(DBL_MAX);
    __m128d max_vec = _mm_set1_pd(-DBL_MAX);
    __m128d min_idx_vec = _mm_set_pd(1.0, 0.0);
    __m128d max_idx_vec = _mm_set_pd(1.0, 0.0);

    int i = 0;
    for (; i + 2 <= count; i += 2) {
        __m128d vals = _mm_loadu_pd(data + i);
        // _mm_set_pd is reverse order: high lane first → lane[1]=i+1, lane[0]=i
        __m128d current_idx = _mm_set_pd(static_cast<double>(i + 1),
                                          static_cast<double>(i));

        // min comparison and index update
        __m128d cmp_min = _mm_cmp_pd(vals, min_vec, _CMP_LT_OQ);
        min_idx_vec = _mm_blendv_pd(min_idx_vec, current_idx, cmp_min);
        min_vec = _mm_min_pd(min_vec, vals);

        // max comparison and index update
        __m128d cmp_max = _mm_cmp_pd(vals, max_vec, _CMP_GT_OQ);
        max_idx_vec = _mm_blendv_pd(max_idx_vec, current_idx, cmp_max);
        max_vec = _mm_max_pd(max_vec, vals);
    }

    // Horizontal reduce: extract from 2 lanes
    double min_arr[2], max_arr[2], min_idx_arr[2], max_idx_arr[2];
    _mm_storeu_pd(min_arr, min_vec);
    _mm_storeu_pd(max_arr, max_vec);
    _mm_storeu_pd(min_idx_arr, min_idx_vec);
    _mm_storeu_pd(max_idx_arr, max_idx_vec);

    int final_min_idx = static_cast<int>(min_idx_arr[0]);
    double final_min_val = min_arr[0];
    int final_max_idx = static_cast<int>(max_idx_arr[0]);
    double final_max_val = max_arr[0];

    if (min_arr[1] < final_min_val) {
        final_min_val = min_arr[1];
        final_min_idx = static_cast<int>(min_idx_arr[1]);
    }
    if (max_arr[1] > final_max_val) {
        final_max_val = max_arr[1];
        final_max_idx = static_cast<int>(max_idx_arr[1]);
    }

    // Scalar tail: remaining < 2 elements
    for (; i < count; ++i) {
        if (data[i] < final_min_val) {
            final_min_val = data[i];
            final_min_idx = i;
        }
        if (data[i] > final_max_val) {
            final_max_val = data[i];
            final_max_idx = i;
        }
    }

    return {final_min_idx, final_max_idx, final_min_val, final_max_val};
}

#else
// Compiler doesn't support SSE4.2 intrinsics — alias to scalar
#define argMinMaxSSE42 argMinMaxScalar
#endif

// ===== AVX2 implementation =====
#if defined(__AVX2__) || (defined(_MSC_VER) && _MSC_VER >= 1800)

static ArgMinMaxResult argMinMaxAVX2(const double* data, int count)
{
    __m256d min_vec = _mm256_set1_pd(DBL_MAX);
    __m256d max_vec = _mm256_set1_pd(-DBL_MAX);
    // _mm256_set_pd is reverse order (high→low: lane3,2,1,0)
    __m256d min_idx_vec = _mm256_set_pd(3.0, 2.0, 1.0, 0.0);
    __m256d max_idx_vec = _mm256_set_pd(3.0, 2.0, 1.0, 0.0);

    int i = 0;
    for (; i + 4 <= count; i += 4) {
        __m256d vals = _mm256_loadu_pd(data + i);
        // _mm256_set_pd: lane[3]=i+3, lane[2]=i+2, lane[1]=i+1, lane[0]=i
        __m256d current_idx = _mm256_set_pd(static_cast<double>(i + 3),
                                              static_cast<double>(i + 2),
                                              static_cast<double>(i + 1),
                                              static_cast<double>(i));

        // min comparison and index update
        __m256d cmp_min = _mm256_cmp_pd(vals, min_vec, _CMP_LT_OQ);
        min_idx_vec = _mm256_blendv_pd(min_idx_vec, current_idx, cmp_min);
        min_vec = _mm256_min_pd(min_vec, vals);

        // max comparison and index update
        __m256d cmp_max = _mm256_cmp_pd(vals, max_vec, _CMP_GT_OQ);
        max_idx_vec = _mm256_blendv_pd(max_idx_vec, current_idx, cmp_max);
        max_vec = _mm256_max_pd(max_vec, vals);
    }

    // Horizontal reduce: extract from 4 lanes
    double min_arr[4], max_arr[4], min_idx_arr[4], max_idx_arr[4];
    _mm256_storeu_pd(min_arr, min_vec);
    _mm256_storeu_pd(max_arr, max_vec);
    _mm256_storeu_pd(min_idx_arr, min_idx_vec);
    _mm256_storeu_pd(max_idx_arr, max_idx_vec);

    int final_min_idx = static_cast<int>(min_idx_arr[0]);
    double final_min_val = min_arr[0];
    int final_max_idx = static_cast<int>(max_idx_arr[0]);
    double final_max_val = max_arr[0];

    for (int j = 1; j < 4; ++j) {
        if (min_arr[j] < final_min_val) {
            final_min_val = min_arr[j];
            final_min_idx = static_cast<int>(min_idx_arr[j]);
        }
        if (max_arr[j] > final_max_val) {
            final_max_val = max_arr[j];
            final_max_idx = static_cast<int>(max_idx_arr[j]);
        }
    }

    // Scalar tail: remaining < 4 elements
    for (; i < count; ++i) {
        if (data[i] < final_min_val) {
            final_min_val = data[i];
            final_min_idx = i;
        }
        if (data[i] > final_max_val) {
            final_max_val = data[i];
            final_max_idx = i;
        }
    }

    return {final_min_idx, final_max_idx, final_min_val, final_max_val};
}

#else
// Compiler doesn't support AVX2 intrinsics — alias to SSE4.2 (which may alias to scalar)
#define argMinMaxAVX2 argMinMaxSSE42
#endif

// ===== Public API: Function pointer dispatch =====
ArgMinMaxResult simdArgMinMax(const double* data, int count)
{
    // Select implementation at startup — no per-call branch overhead
    using Fn = ArgMinMaxResult(*)(const double*, int);
    static const Fn kFn = []() -> Fn {
        switch (kSimdLevel) {
        case AVX2:  return argMinMaxAVX2;
        case SSE42: return argMinMaxSSE42;
        default:    return argMinMaxScalar;
        }
    }();
    auto result = kFn(data, count);

    // IEEE 754: min(NaN,x) / max(NaN,x) propagate NaN on x86.
    // When all input values are NaN, SIMD paths produce NaN results.
    // Return canonical sentinel: {0, 0, DBL_MAX, -DBL_MAX}
    if (std::isnan(result.min_val) || std::isnan(result.max_val))
        return {0, 0, DBL_MAX, -DBL_MAX};

    return result;
}

}  // namespace QIM