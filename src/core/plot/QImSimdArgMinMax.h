#ifndef QIMSIMDARGMINMAX_H
#define QIMSIMDARGMINMAX_H

#include "QImAPI.h"
#include <cfloat>

namespace QIM
{

/**
 * \if ENGLISH
 * @brief Result struct for SIMD-accelerated argmin + argmax operation
 * @details Contains the local indices (0-based relative to input pointer)
 *          and values of both the minimum and maximum elements found.
 * @note For all-NaN input arrays, min_val=DBL_MAX, max_val=-DBL_MAX
 * \endif
 *
 * \if CHINESE
 * @brief SIMD加速 argmin + argmax 操作的结果结构体
 * @details 包含找到的最小值和最大值元素的局部索引(0-based, 相对于输入指针偏移)
 *          及其值。
 * @note 全NaN输入数组时, min_val=DBL_MAX, max_val=-DBL_MAX
 * \endif
 */
struct ArgMinMaxResult {
    int min_idx;    // Local index of minimum value
    int max_idx;    // Local index of maximum value
    double min_val; // Minimum value found
    double max_val; // Maximum value found
};

/**
 * \if ENGLISH
 * @brief SIMD-accelerated argmin + argmax in a single pass
 * @details Finds the indices and values of both the minimum and maximum
 *          elements in a double array. Uses runtime CPU feature detection
 *          to select the optimal implementation: AVX2 → SSE4.2 → Scalar.
 *          NaN values are naturally ignored (IEEE 754 comparison semantics).
 * @param data Pointer to double array (must not be null)
 * @param count Number of elements in the array (must be >= 1)
 * @return Struct containing min/max indices (local, 0-based) and values
 * @note For all-NaN arrays, returns {0, 0, DBL_MAX, -DBL_MAX} (initial values)
 * @note Preconditions: data != nullptr && count >= 1. Caller guarantees this.
 * \endif
 *
 * \if CHINESE
 * @brief SIMD加速的 argmin + argmax 单遍历查找
 * @details 在double数组中同时查找最小值和最大值的索引和值。
 *          使用runtime CPU特性检测选择最优实现：AVX2 → SSE4.2 → 标量。
 *          NaN值被自然忽略（IEEE 754比较语义）。
 * @param data double数组指针（不可为null）
 * @param count 数组元素数量（必须 >= 1）
 * @return 包含min/max局部索引(0-based)和值的结构体
 * @note 全NaN数组返回 {0, 0, DBL_MAX, -DBL_MAX}（初始值）
 * @note 前置条件: data != nullptr && count >= 1。调用者保证。
 * \endif
 */
QIM_CORE_API ArgMinMaxResult simdArgMinMax(const double* data, int count);

}  // namespace QIM

#endif  // QIMSIMDARGMINMAX_H