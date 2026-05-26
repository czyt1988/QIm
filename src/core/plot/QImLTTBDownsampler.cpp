#include "QImLTTBDownsampler.h"
#include <algorithm>
#include <cmath>
#include <cassert>

namespace QIM
{

/**
 * \if ENGLISH
 * @brief Constructs an LTTB downsampling proxy for the given data series
 * @details Creates a transparent decorator that wraps the source data series.
 *          The proxy does not own the source data — the caller must ensure
 *          the source outlives this object.
 * @param source Original data series (must outlive the proxy, non-null)
 * @param target_points Target point count for downsampling (default 2000, ~1.5× typical screen width)
 * \endif
 *
 * \if CHINESE
 * @brief 为给定数据系列构造 LTTB 下采样代理
 * @details 创建一个包装源数据系列的透明装饰器。代理不拥有源数据——调用者必须确保
 *          源数据的生命周期超过此对象。
 * @param source 原始数据系列（必须超过代理生命周期，不可为空）
 * @param target_points 下采样的目标点数（默认2000，约为典型屏幕宽度的1.5倍）
 * \endif
 */
QImLTTBDownsampler::QImLTTBDownsampler(QImAbstractXYDataSeries* source, int target_points) : m_source(source)
{
    assert(source && "Source must not be null");
    setTargetPoints(target_points);
}


/**
 * \if ENGLISH
 * @brief Returns the effective size of the data series
 * @details If downsampled cache is valid, returns the cached point count;
 *          otherwise returns the source series size.
 * @return Number of data points available for rendering
 * \endif
 *
 * \if CHINESE
 * @brief 返回数据系列的有效大小
 * @details 如果下采样缓存有效，返回缓存的点数；否则返回源系列大小。
 * @return 可用于渲染的数据点数量
 * \endif
 */
int QImLTTBDownsampler::size() const
{
    // 优先返回下采样数据大小，若未下采样则返回原始数据大小
    return m_cached_valid ? static_cast< int >(m_cached_x.size()) : (m_source ? m_source->size() : 0);
}

/**
 * \if ENGLISH
 * @brief Checks whether data is stored in contiguous memory
 * @details Returns true if cached downsampled data is valid (always contiguous)
 *          or if the source series reports contiguous storage.
 * @return True if data can be accessed via raw pointers with stride
 * \endif
 *
 * \if CHINESE
 * @brief 检查数据是否以连续内存存储
 * @details 如果缓存的下采样数据有效（始终连续）或源系列报告连续存储，返回true。
 * @return 如果可以通过原始指针+步幅访问数据则返回true
 * \endif
 */
bool QImLTTBDownsampler::isContiguous() const
{
    return m_cached_valid || (m_source && m_source->isContiguous());
}

/**
 * \if ENGLISH
 * @brief Returns the byte stride between consecutive elements in contiguous memory
 * @details For contiguous double arrays, stride is always sizeof(double).
 * @return Size of a double in bytes
 * \endif
 *
 * \if CHINESE
 * @brief 返回连续内存中相邻元素间的字节步幅
 * @details 对于连续的double数组，步幅始终为sizeof(double)。
 * @return double类型的大小（字节）
 * \endif
 */
int QImLTTBDownsampler::stride() const
{
    // 连续内存步幅固定为double大小
    return sizeof(double);
}

/**
 * \if ENGLISH
 * @brief Returns a pointer to raw X data for zero-copy rendering
 * @details When cached downsampled data is valid, returns the cached X array pointer.
 *          Otherwise, passes through to the source series. Nullptr if no source.
 * @return Pointer to raw X data array, or nullptr
 * \endif
 *
 * \if CHINESE
 * @brief 返回指向原始X数据的指针，用于零拷贝渲染
 * @details 当缓存的下采样数据有效时，返回缓存的X数组指针。
 *          否则透传到源系列。无源数据时返回nullptr。
 * @return 指向原始X数据数组的指针，或nullptr
 * \endif
 */
const double* QImLTTBDownsampler::xRawData() const
{
    // 缓存有效则返回下采样数据，否则透传原始数据
    if (m_cached_valid) {
        return m_cached_x.data();
    }
    return m_source ? m_source->xRawData() : nullptr;
}

/**
 * \if ENGLISH
 * @brief Returns a pointer to raw Y data for zero-copy rendering
 * @details When cached downsampled data is valid, returns the cached Y array pointer.
 *          Otherwise, passes through to the source series. Nullptr if no source.
 * @return Pointer to raw Y data array, or nullptr
 * \endif
 *
 * \if CHINESE
 * @brief 返回指向原始Y数据的指针，用于零拷贝渲染
 * @details 当缓存的下采样数据有效时，返回缓存的Y数组指针。
 *          否则透传到源系列。无源数据时返回nullptr。
 * @return 指向原始Y数据数组的指针，或nullptr
 * \endif
 */
const double* QImLTTBDownsampler::yRawData() const
{
    if (m_cached_valid) {
        return m_cached_y.data();
    }
    return m_source ? m_source->yRawData() : nullptr;
}

/**
 * \if ENGLISH
 * @brief Returns the X-axis scale factor (Y-only mode support)
 * @details After downsampling, Y-only mode is no longer supported since
 *          downsampling breaks the equidistant assumption. Delegates to source.
 * @return X-axis scale factor from the source series
 * \endif
 *
 * \if CHINESE
 * @brief 返回X轴缩放因子（支持仅Y模式）
 * @details 下采样后不再支持仅Y模式，因为下采样破坏了等间隔假设。委托给源系列。
 * @return 源系列的X轴缩放因子
 * \endif
 */
double QImLTTBDownsampler::xScale() const
{
    return m_source->xScale();
}

/**
 * \if ENGLISH
 * @brief Returns the starting X value (Y-only mode support)
 * @details Used only when X data is not explicitly provided. Delegates to source.
 * @return X-axis start value from the source series
 * \endif
 *
 * \if CHINESE
 * @brief 返回起始X值（支持仅Y模式）
 * @details 仅在未显式提供X数据时使用。委托给源系列。
 * @return 源系列的X轴起始值
 * \endif
 */
double QImLTTBDownsampler::xStart() const
{
    return m_source->xStart();
}

/**
 * \if ENGLISH
 * @brief Returns the byte offset for raw data pointers
 * @details Delegates to the source series.
 * @return Byte offset from the source series
 * \endif
 *
 * \if CHINESE
 * @brief 返回原始数据指针的字节偏移量
 * @details 委托给源系列。
 * @return 源系列的字节偏移量
 * \endif
 */
int QImLTTBDownsampler::offset() const
{
    return m_source->offset();
}

/**
 * \if ENGLISH
 * @brief Sets the target point count for downsampling
 * @details Clamped to a minimum of 100 points to avoid invalid sampling.
 *          Triggers re-downsampling if the value changes.
 * @param points Desired target point count
 * \endif
 *
 * \if CHINESE
 * @brief 设置下采样的目标点数
 * @details 限制最少为100点，避免无效采样。值变化时触发重新下采样。
 * @param points 期望的目标点数
 * \endif
 */
void QImLTTBDownsampler::setTargetPoints(int points)
{
    // 限制最小目标点数，避免无效采样
    int new_points = std::max(points, 100);  // 最小100点
    if (new_points != m_target_points) {
        m_target_points = new_points;
        // 目标点数变化时重新下采样
        downSampler();
    }
}

/**
 * \if ENGLISH
 * @brief Returns the current target point count for downsampling
 * @return Configured target point count
 * \endif
 *
 * \if CHINESE
 * @brief 返回当前下采样的目标点数
 * @return 配置的目标点数
 * \endif
 */
int QImLTTBDownsampler::targetPoints() const
{
    return m_target_points;
}

/**
 * \if ENGLISH
 * @brief Returns the X value at the given index
 * @details If downsampled cache is valid, returns from the cached array.
 *          Otherwise delegates to the source series.
 * @param index Data point index
 * @return X value at the specified index, or NaN if out of range
 * \endif
 *
 * \if CHINESE
 * @brief 返回指定索引处的X值
 * @details 如果下采样缓存有效，从缓存数组返回。否则委托给源系列。
 * @param index 数据点索引
 * @return 指定索引处的X值，超出范围则返回NaN
 * \endif
 */
double QImLTTBDownsampler::xValue(int index) const
{
    if (!m_cached_valid) {
        return m_source ? m_source->xValue(index) : std::numeric_limits< double >::quiet_NaN();
    }
    return m_cached_x[ index ];
}

/**
 * \if ENGLISH
 * @brief Returns the Y value at the given index
 * @details If downsampled cache is valid, returns from the cached array.
 *          Otherwise delegates to the source series.
 * @param index Data point index
 * @return Y value at the specified index, or NaN if out of range
 * \endif
 *
 * \if CHINESE
 * @brief 返回指定索引处的Y值
 * @details 如果下采样缓存有效，从缓存数组返回。否则委托给源系列。
 * @param index 数据点索引
 * @return 指定索引处的Y值，超出范围则返回NaN
 * \endif
 */
double QImLTTBDownsampler::yValue(int index) const
{
    if (!m_cached_valid) {
        return m_source ? m_source->yValue(index) : std::numeric_limits< double >::quiet_NaN();
    }
    return m_cached_y[ index ];
}

/**
 * \if ENGLISH
 * @brief Performs LTTB downsampling on the full source data
 * @details Clears old cache, then runs the LTTB algorithm over the entire
 *          source range. If source data is small enough to fit within the target
 *          point budget, no downsampling occurs (direct pass-through).
 *          Call this after source data or target point count changes.
 * \endif
 *
 * \if CHINESE
 * @brief 对完整源数据执行 LTTB 下采样
 * @details 清除旧缓存，然后对整个源数据范围运行LTTB算法。
 *          如果源数据足够小，在目标点数预算内则不下采样（直接透传）。
 *          在源数据或目标点数变化后调用此函数。
 * \endif
 */
void QImLTTBDownsampler::downSampler()
{
    // 清空旧缓存
    m_cached_x.clear();
    m_cached_y.clear();
    m_cached_valid = false;

    if (!m_source || m_source->size() <= 0) {
        return;
    }

    const int source_size = m_source->size();
    // 原始数据量 ≤ 目标点数 → 直接透传，不下采样
    if (source_size <= m_target_points || source_size < 3) {
        m_cached_valid = false;
        return;
    }

    // 对全量数据执行LTTB下采样
    lttb(m_source->xRawData(), m_source->yRawData(), 0, source_size, m_target_points);
    m_cached_valid = true;
}


// ===== 辅助：查找可见范围（二分查找优化）=====
/**
 * \if ENGLISH
 * @brief Finds the index range of data points visible within the given X range
 * @details Uses binary search for XY mode (sorted X data) and direct index
 *          calculation for Y-only mode. Returns [start, end) half-open interval.
 * @param x_min Minimum X value of the visible range
 * @param x_max Maximum X value of the visible range
 * @return Pair of {start_index, end_index} in the source data
 * \endif
 *
 * \if CHINESE
 * @brief 查找在给定X范围内可见的数据点索引范围
 * @details XY模式使用二分查找（X数据已排序），仅Y模式使用直接索引计算。
 *          返回[start, end)半开区间。
 * @param x_min 可见范围的最小X值
 * @param x_max 可见范围的最大X值
 * @return 源数据中的{起始索引, 结束索引}对
 * \endif
 */
std::pair< int, int > QImLTTBDownsampler::findVisibleRange(double x_min, double x_max) const
{
    const int total_size = m_source->size();
    if (total_size == 0)
        return { 0, 0 };

    // 处理Y-only模式：X坐标可计算
    if (const double* x_ptr = m_source->xRawData()) {
        // XY模式：二分查找
        auto x_begin = x_ptr;
        auto x_end   = x_ptr + total_size;

        int start_idx = std::lower_bound(x_begin, x_end, x_min) - x_begin;
        int end_idx   = std::upper_bound(x_begin, x_end, x_max) - x_begin;

        return { std::max(0, start_idx), std::min(total_size, end_idx) };
    } else {
        // Y-only模式：直接计算索引范围
        const double x_start = m_source->xStart();
        const double x_scale = m_source->xScale();

        if (x_scale == 0)
            return { 0, total_size };  // 退化情况

        int start_idx = static_cast< int >(std::floor((x_min - x_start) / x_scale));
        int end_idx   = static_cast< int >(std::ceil((x_max - x_start) / x_scale)) + 1;

        return { std::max(0, start_idx), std::min(total_size, end_idx) };
    }
}


// ===== LTTB核心算法（O(n)）=====
/**
 * \if ENGLISH
 * @brief Core LTTB algorithm implementation (O(n) complexity)
 * @details Largest-Triangle-Three-Buckets: selects visually representative points
 *          by maximizing triangle area within each bucket. Preserves first and last
 *          points. Supports both XY mode (explicit X data) and Y-only mode
 *          (computed X from xScale/xStart). Results stored in m_cached_x/y.
 * @param x_data Raw X data pointer (nullptr for Y-only mode)
 * @param y_data Raw Y data pointer (required)
 * @param start_idx Start index in source data (inclusive)
 * @param end_idx End index in source data (exclusive)
 * @param target_points Desired output point count (min 3)
 * \endif
 *
 * \if CHINESE
 * @brief 核心 LTTB 算法实现（O(n)复杂度）
 * @details 最大三角形三桶法：通过在每个桶内最大化三角形面积来选择视觉代表性点。
 *          保留首末两点。同时支持XY模式（显式X数据）和仅Y模式（通过xScale/xStart计算X）。
 *          结果存储在m_cached_x/y中。
 * @param x_data 原始X数据指针（仅Y模式时为nullptr）
 * @param y_data 原始Y数据指针（必需）
 * @param start_idx 源数据起始索引（包含）
 * @param end_idx 源数据结束索引（不包含）
 * @param target_points 期望的输出点数（最少3个）
 * \endif
 */
void QImLTTBDownsampler::lttb(const double* x_data, const double* y_data, int start_idx, int end_idx, int target_points)
{
    // 前置校验：输入参数合法性
    if (!m_source || !y_data)
        return;
    const int n = end_idx - start_idx;
    if (n <= 0 || target_points < 3 || start_idx < 0 || end_idx > m_source->size()) {
        m_cached_x.clear();
        m_cached_y.clear();
        return;
    }

    // 预分配缓存
    m_cached_x.reserve(target_points);
    m_cached_y.reserve(target_points);

    // 辅助lambda：安全获取X坐标（兼容Y-only模式）
    auto getX = [ this, x_data, start_idx ](int local_idx) -> double {
        if (local_idx < 0 || local_idx >= (m_source->size() - start_idx)) {
            return std::numeric_limits< double >::quiet_NaN();
        }
        // XY模式：直接取X数组
        if (x_data) {
            return x_data[ start_idx + local_idx ];
        }
        // Y-only模式：计算X坐标（防护除0）
        const double x_scale = m_source->xScale();
        if (std::fabs(x_scale) < 1e-12) {
            return m_source->xStart();
        }
        return m_source->xStart() + (start_idx + local_idx) * x_scale;
    };

    // 辅助lambda：安全获取Y坐标
    auto getY = [ y_data, start_idx ](int local_idx) -> double { return y_data[ start_idx + local_idx ]; };

    // 1. 保留第一个点
    m_cached_x.push_back(getX(0));
    m_cached_y.push_back(getY(0));

    // 2. 中间点：LTTB核心（最大三角形面积采样）
    const double avg_bucket_size = static_cast< double >(n - 2) / (target_points - 2);
    double bucket_left           = 1.0;

    for (int i = 1; i < target_points - 1; ++i) {
        // 计算当前桶的范围（最后一个桶强制包含末尾）
        double bucket_right =
            (i == target_points - 2) ? (n - 1) : std::min(bucket_left + avg_bucket_size, static_cast< double >(n - 1));

        const int bucket_start = static_cast< int >(std::floor(bucket_left));
        const int bucket_end   = static_cast< int >(std::ceil(bucket_right)) + 1;
        const int bucket_size  = bucket_end - bucket_start;

        if (bucket_size <= 0 || bucket_start >= n) {
            continue;  // 空桶跳过，避免崩溃
        }

        // 计算桶内平均点（三角形第三点）
        double avg_x = 0.0, avg_y = 0.0;
        int valid_count = 0;
        for (int j = bucket_start; j < bucket_end && j < n; ++j) {
            double x = getX(j);
            double y = getY(j);
            if (std::isnan(x) || std::isnan(y))
                continue;
            avg_x += x;
            avg_y += y;
            valid_count++;
        }
        if (valid_count == 0) {
            avg_x = getX(bucket_start);
            avg_y = getY(bucket_start);
        } else {
            avg_x /= valid_count;
            avg_y /= valid_count;
        }

        // 寻找桶内最大三角形面积的点
        double max_area     = -1.0;
        int max_idx         = bucket_start;
        const double last_x = m_cached_x.back();
        const double last_y = m_cached_y.back();

        for (int j = bucket_start; j < bucket_end && j < n; ++j) {
            const double curr_x = getX(j);
            const double curr_y = getY(j);
            if (std::isnan(curr_x) || std::isnan(curr_y))
                continue;

            // 三角形面积计算（省略/2和abs，仅比较大小）
            const double area = std::fabs((curr_x - last_x) * (avg_y - last_y) - (avg_x - last_x) * (curr_y - last_y));

            if (area > max_area) {
                max_area = area;
                max_idx  = j;
            }
        }

        // 添加最大面积点到缓存
        m_cached_x.push_back(getX(max_idx));
        m_cached_y.push_back(getY(max_idx));
        bucket_left = bucket_right + 1;
    }

    // 3. 保留最后一个点
    m_cached_x.push_back(getX(n - 1));
    m_cached_y.push_back(getY(n - 1));
}

}  // namespace QIM
