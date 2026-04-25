#include "QImMinMaxLTTBDownsampler.h"
#include <algorithm>
#include <cmath>
#include <cassert>

namespace QIM
{

// ===== 构造函数 =====
QImMinMaxLTTBDownsampler::QImMinMaxLTTBDownsampler(QImAbstractXYDataSeries* source, int target_points, double preselection_ratio)
    : m_source(source)
{
    assert(source && "Source must not be null");
    m_preselection_ratio = std::max(2.0, preselection_ratio);
    assert(m_preselection_ratio >= 2.0 && "Preselection ratio must be at least 2.0");
    setTargetPoints(target_points);
}


int QImMinMaxLTTBDownsampler::size() const
{
    // 优先返回下采样数据大小，若未下采样则返回原始数据大小
    return m_cached_valid ? static_cast< int >(m_cached_x.size()) : (m_source ? m_source->size() : 0);
}

bool QImMinMaxLTTBDownsampler::isContiguous() const
{
    return m_cached_valid || (m_source && m_source->isContiguous());
}

int QImMinMaxLTTBDownsampler::stride() const
{
    // 连续内存步幅固定为 double 大小
    return sizeof(double);
}

const double* QImMinMaxLTTBDownsampler::xRawData() const
{
    // 缓存有效则返回下采样数据，否则透传原始数据
    if (m_cached_valid) {
        return m_cached_x.data();
    }
    return m_source ? m_source->xRawData() : nullptr;
}

const double* QImMinMaxLTTBDownsampler::yRawData() const
{
    if (m_cached_valid) {
        return m_cached_y.data();
    }
    return m_source ? m_source->yRawData() : nullptr;
}

double QImMinMaxLTTBDownsampler::xScale() const
{
    return m_source->xScale();
}

double QImMinMaxLTTBDownsampler::xStart() const
{
    return m_source->xStart();
}

int QImMinMaxLTTBDownsampler::offset() const
{
    return m_source->offset();
}

void QImMinMaxLTTBDownsampler::setTargetPoints(int points)
{
    // 限制最小目标点数（算法至少需要 3 个点）
    int new_points = std::max(points, 3);
    if (new_points != m_target_points) {
        m_target_points = new_points;
        // 目标点数变化时重新下采样
        downSampler();
    }
}

int QImMinMaxLTTBDownsampler::targetPoints() const
{
    return m_target_points;
}

void QImMinMaxLTTBDownsampler::setPreselectionRatio(double ratio)
{
    double new_ratio = std::max(ratio, 2.0);  // 最小比例为 2.0
    if (std::fabs(new_ratio - m_preselection_ratio) > 1e-6) {
        m_preselection_ratio = new_ratio;
        // 预筛选比例变化时重新下采样
        downSampler();
    }
}

double QImMinMaxLTTBDownsampler::preselectionRatio() const
{
    return m_preselection_ratio;
}

double QImMinMaxLTTBDownsampler::xValue(int index) const
{
    if (!m_cached_valid) {
        return m_source ? m_source->xValue(index) : std::numeric_limits< double >::quiet_NaN();
    }
    return m_cached_x[ index ];
}

double QImMinMaxLTTBDownsampler::yValue(int index) const
{
    if (!m_cached_valid) {
        return m_source ? m_source->yValue(index) : std::numeric_limits< double >::quiet_NaN();
    }
    return m_cached_y[ index ];
}

void QImMinMaxLTTBDownsampler::downSampler()
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

    // 对全量数据执行 MinMaxLTTB 下采样
    minMaxLTTB(m_source->xRawData(), m_source->yRawData(), 0, source_size, m_target_points);
    m_cached_valid = !m_cached_x.empty();
}

void QImMinMaxLTTBDownsampler::downSampler(double x_min, double x_max)
{
    m_cached_x.clear();
    m_cached_y.clear();
    m_cached_valid = false;

    if (!m_source || m_source->size() <= 0) {
        return;
    }

    const int source_size = m_source->size();

    auto [start_idx, end_idx] = findVisibleRange(x_min, x_max);
    if (start_idx >= end_idx) {
        return;
    }

    const int visible_count = end_idx - start_idx;
    if (visible_count <= m_target_points || visible_count < 3) {
        m_cached_valid = false;
        return;
    }

    minMaxLTTB(m_source->xRawData(), m_source->yRawData(), start_idx, end_idx, m_target_points);
    m_cached_valid = !m_cached_x.empty();
}


// ===== 辅助：查找可见范围（二分查找优化）=====
std::pair< int, int > QImMinMaxLTTBDownsampler::findVisibleRange(double x_min, double x_max) const
{
    const int total_size = m_source->size();
    if (total_size == 0)
        return { 0, 0 };

    // 处理 Y-only 模式：X 坐标可计算
    if (const double* x_ptr = m_source->xRawData()) {
        // XY 模式：二分查找
        auto x_begin = x_ptr;
        auto x_end   = x_ptr + total_size;

        int start_idx = std::lower_bound(x_begin, x_end, x_min) - x_begin;
        int end_idx   = std::upper_bound(x_begin, x_end, x_max) - x_begin;

        return { std::max(0, start_idx), std::min(total_size, end_idx) };
    } else {
        // Y-only 模式：直接计算索引范围
        const double x_start = m_source->xStart();
        const double x_scale = m_source->xScale();

        if (x_scale == 0)
            return { 0, total_size };  // 退化情况

        int start_idx = static_cast< int >(std::floor((x_min - x_start) / x_scale));
        int end_idx   = static_cast< int >(std::ceil((x_max - x_start) / x_scale)) + 1;

        return { std::max(0, start_idx), std::min(total_size, end_idx) };
    }
}


// ===== MinMaxLTTB 核心算法（O(n)，带 MinMax 预筛选）=====
void QImMinMaxLTTBDownsampler::minMaxLTTB(const double* x_data, const double* y_data, int start_idx, int end_idx, int target_points)
{
    // 前置校验
    if (!m_source || target_points < 3) {
        m_cached_x.clear();
        m_cached_y.clear();
        return;
    }

    const int n = end_idx - start_idx;
    if (n <= 0 || start_idx < 0 || end_idx > m_source->size()) {
        m_cached_x.clear();
        m_cached_y.clear();
        return;
    }


    // 预分配缓存
    m_cached_x.clear();
    m_cached_y.clear();
    m_cached_x.reserve(target_points);
    m_cached_y.reserve(target_points);

    // Lambda getters: inline direct access, no temp vector allocation
    auto getX = [&](int local_idx) -> double {
        if (x_data)
            return x_data[ start_idx + local_idx ];
        const double scale = m_source->xScale();
        if (std::fabs(scale) < 1e-12)
            return m_source->xStart();
        return m_source->xStart() + (start_idx + local_idx) * scale;
    };
    auto getY = [&](int local_idx) -> double {
        return y_data[ start_idx + local_idx ];
    };

    // 1. 保留第一个点
    m_cached_x.push_back(getX(0));
    m_cached_y.push_back(getY(0));

    // 2. 预计算每个桶的边界 - 整数计算替代浮点
    const int num_buckets = target_points - 2;
    const int bucket_size = n / num_buckets;
    const int remainder   = n % num_buckets;

    // 3. 中间点：优化MinMaxLTTB
    int current_idx                     = 1;
    const double preselection_factor    = std::max(2.0, m_preselection_ratio);
    const int max_candidates_per_bucket = static_cast< int >(std::ceil(preselection_factor * 2));

    // 预分配候选点数组
    std::vector< int > candidate_indices;
    candidate_indices.reserve(max_candidates_per_bucket);

    for (int bucket = 0; bucket < num_buckets; ++bucket) {
        // 计算桶边界 - 整数算法
        const int bucket_start = current_idx;
        const int extra        = (bucket < remainder) ? 1 : 0;
        int bucket_end         = bucket_start + bucket_size + extra;

        if (bucket_start >= n || bucket_start >= bucket_end) {
            continue;
        }
        if (bucket_end > n) {
            bucket_end = n;
        }

        // 计算桶大小
        const int bucket_count = bucket_end - bucket_start;

        // 预筛选候选点数量
        const int num_sub_intervals = std::max(1, static_cast< int >(std::ceil(bucket_count / preselection_factor)));
        const int points_per_subinterval = std::max(1, bucket_count / num_sub_intervals);

        // 重置候选点
        candidate_indices.clear();

        // 高效寻找极值点 - 无需去重检查
        for (int sub = 0; sub < num_sub_intervals; ++sub) {
            const int sub_start = bucket_start + sub * points_per_subinterval;
            const int sub_end =
                (sub == num_sub_intervals - 1) ? bucket_end : std::min(bucket_end, sub_start + points_per_subinterval);

            if (sub_start >= sub_end)
                continue;

            int max_idx    = sub_start;
            int min_idx    = sub_start;
            double max_val = getY(sub_start);
            double min_val = max_val;

            // 寻找子区间内的极值
            for (int j = sub_start + 1; j < sub_end; ++j) {
                const double y = getY(j);
                if (std::isnan(y))
                    continue;

                if (y > max_val) {
                    max_val = y;
                    max_idx = j;
                }
                if (y < min_val) {
                    min_val = y;
                    min_idx = j;
                }
            }

            candidate_indices.push_back(max_idx);
            if (max_idx != min_idx) {
                candidate_indices.push_back(min_idx);
            }
        }

        // 确保至少有一个候选点
        if (candidate_indices.empty()) {
            candidate_indices.push_back(bucket_start);
        }

        // 优化：只使用候选点计算平均点
        double avg_x    = 0.0;
        double avg_y    = 0.0;
        int valid_count = 0;

        for (int idx : candidate_indices) {
            const double x = getX(idx);
            const double y = getY(idx);
            if (!std::isnan(x) && !std::isnan(y)) {
                avg_x += x;
                avg_y += y;
                valid_count++;
            }
        }

        if (valid_count > 0) {
            avg_x /= valid_count;
            avg_y /= valid_count;
        } else {
            // 回退到桶内第一个有效点
            avg_x = getX(bucket_start);
            avg_y = getY(bucket_start);
        }

        // 寻找最大三角形面积的候选点
        const double last_x = m_cached_x.back();
        const double last_y = m_cached_y.back();
        double max_area     = -1.0;
        int best_idx        = candidate_indices[ 0 ];

        for (int idx : candidate_indices) {
            const double curr_x = getX(idx);
            const double curr_y = getY(idx);

            if (std::isnan(curr_x) || std::isnan(curr_y))
                continue;

            // 三角形面积计算（简化版，避免绝对值和除法）
            const double dx1  = curr_x - last_x;
            const double dy1  = curr_y - last_y;
            const double dx2  = avg_x - last_x;
            const double dy2  = avg_y - last_y;
            const double area = std::fabs(dx1 * dy2 - dx2 * dy1);

            if (area > max_area) {
                max_area = area;
                best_idx = idx;
            }
        }

        // 添加最佳点
        m_cached_x.push_back(getX(best_idx));
        m_cached_y.push_back(getY(best_idx));

        current_idx = bucket_end;
    }

    // 3. 保留最后一个点
    if (m_cached_x.size() < static_cast< size_t >(target_points)) {
        m_cached_x.push_back(getX(n - 1));
        m_cached_y.push_back(getY(n - 1));
    }

    // NaN pass-through: if all cached Y values are NaN, treat as degenerate → clear cache
    bool all_y_nan = true;
    for (size_t i = 0; i < m_cached_y.size(); ++i) {
        if (!std::isnan(m_cached_y[ i ])) {
            all_y_nan = false;
            break;
        }
    }
    if (all_y_nan) {
        m_cached_x.clear();
        m_cached_y.clear();
        m_cached_valid = false;
    }
}

}  // namespace QIM
