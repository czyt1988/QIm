#include "QImLTTBDownsampler.h"
#include <algorithm>
#include <cmath>
#include <cassert>

namespace QIM
{

// ===== 构造函数 =====
QImLTTBDownsampler::QImLTTBDownsampler(QImAbstractXYDataSeries* source, int target_points) : m_source(source)
{
    assert(source && "Source must not be null");
    setTargetPoints(target_points);
}


int QImLTTBDownsampler::size() const
{
    // 优先返回下采样数据大小，若未下采样则返回原始数据大小
    return m_cached_valid ? static_cast< int >(m_cached_x.size()) : (m_source ? m_source->size() : 0);
}

bool QImLTTBDownsampler::isContiguous() const
{
    return m_cached_valid || (m_source && m_source->isContiguous());
}

int QImLTTBDownsampler::stride() const
{
    // 连续内存步幅固定为double大小
    return sizeof(double);
}

const double* QImLTTBDownsampler::xRawData() const
{
    // 缓存有效则返回下采样数据，否则透传原始数据
    if (m_cached_valid) {
        return m_cached_x.data();
    }
    return m_source ? m_source->xRawData() : nullptr;
}

const double* QImLTTBDownsampler::yRawData() const
{
    if (m_cached_valid) {
        return m_cached_y.data();
    }
    return m_source ? m_source->yRawData() : nullptr;
}

double QImLTTBDownsampler::xScale() const
{
    return m_source->xScale();
}

double QImLTTBDownsampler::xStart() const
{
    return m_source->xStart();
}

int QImLTTBDownsampler::offset() const
{
    return m_source->offset();
}

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

int QImLTTBDownsampler::targetPoints() const
{
    return m_target_points;
}

double QImLTTBDownsampler::xValue(int index) const
{
    if (!m_cached_valid) {
        return m_source ? m_source->xValue(index) : std::numeric_limits< double >::quiet_NaN();
    }
    return m_cached_x[ index ];
}

double QImLTTBDownsampler::yValue(int index) const
{
    if (!m_cached_valid) {
        return m_source ? m_source->yValue(index) : std::numeric_limits< double >::quiet_NaN();
    }
    return m_cached_y[ index ];
}

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
void QImLTTBDownsampler::lttb(const double* x_data, const double* y_data, int start_idx, int end_idx, int target_points)
{
    // 前置校验：输入参数合法性
    if (!m_source)
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
