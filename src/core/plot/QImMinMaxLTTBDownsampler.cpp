#include "QImMinMaxLTTBDownsampler.h"
#include "QImSimdArgMinMax.h"
#include <algorithm>
#include <cmath>
#include <cassert>

namespace QIM
{

// ===== 构造函数 =====
QImMinMaxLTTBDownsampler::QImMinMaxLTTBDownsampler(QImAbstractXYDataSeries* source,
                                                   int target_points,
                                                   double preselection_ratio,
                                                   bool autoDownsample)
    : m_source(source)
{
    assert(source && "Source must not be null");
    m_preselection_ratio = std::max(2.0, preselection_ratio);
    if (autoDownsample) {
        setTargetPoints(target_points);  // triggers downSampler()
    } else {
        m_target_points = std::max(target_points, 3);  // only store, don't downsample
    }
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

    auto [ start_idx, end_idx ] = findVisibleRange(x_min, x_max);
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

// ===== MinMaxLTTB 核心算法（O(n)，带 MinMax 预筛选 + SIMD加速）=====
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

    // 总点数不足以降采样
    if (n <= target_points || n < 3) {
        m_cached_valid = false;
        return;
    }

    m_cached_x.clear();
    m_cached_y.clear();
    m_cached_x.reserve(target_points);
    m_cached_y.reserve(target_points);

    // ========= 微优化1: 消除lambda间接访问，直接指针偏移 =========
    const bool isXY = (x_data != nullptr);
    const double* x_ptr = isXY ? (x_data + start_idx) : nullptr;
    const double* y_ptr = y_data + start_idx;
    const double x_start = m_source->xStart();
    const double x_scale = m_source->xScale();

    // Helper: get X value at local index
    auto getXValue = [&](int local_idx) -> double {
        if (isXY) return x_ptr[local_idx];
        if (std::fabs(x_scale) < 1e-12) return x_start;
        return x_start + (start_idx + local_idx) * x_scale;
    };

    // ========= 微优化3: NaN快速路径 — 预扫描 =========
    bool hasNaN = false;
    for (int i = 0; i < n && !hasNaN; ++i) {
        if (std::isnan(y_ptr[i])) hasNaN = true;
    }

    // ========= 微优化4: 全NaN检测合并到主循环 — anyValidY flag =========
    bool anyValidY = false;

    // ========= 1. 始终保留第一个点 =========
    m_cached_x.push_back(getXValue(0));
    m_cached_y.push_back(y_ptr[0]);
    if (!std::isnan(y_ptr[0])) anyValidY = true;

    // ========= 2. 桶划分 =========
    const int num_buckets  = target_points - 2;
    const int middle_count = n - 2;

    // 退化情况
    if (middle_count <= num_buckets) {
        for (int i = 1; i < n - 1; ++i) {
            m_cached_x.push_back(getXValue(i));
            m_cached_y.push_back(y_ptr[i]);
            if (!std::isnan(y_ptr[i])) anyValidY = true;
        }
        m_cached_x.push_back(getXValue(n - 1));
        m_cached_y.push_back(y_ptr[n - 1]);
        if (!std::isnan(y_ptr[n - 1])) anyValidY = true;

        if (!anyValidY) {
            m_cached_x.clear(); m_cached_y.clear(); m_cached_valid = false;
        }
        return;
    }

    const int bucket_base      = middle_count / num_buckets;
    const int bucket_remainder = middle_count % num_buckets;

    // ========= 微优化2: 栈数组替代 candidate_indices vector =========
    constexpr int MAX_CANDIDATES = 64;
    int candidate_stack[MAX_CANDIDATES];
    int candidate_count = 0;

    int current_idx = 1;

    for (int bucket = 0; bucket < num_buckets; ++bucket) {
        // 桶边界
        const int bucket_start = current_idx;
        const int extra        = (bucket < bucket_remainder) ? 1 : 0;
        int bucket_end         = bucket_start + bucket_base + extra;
        if (bucket_end > n - 1) bucket_end = n - 1;
        if (bucket_end <= bucket_start) {
            bucket_end = bucket_start + 1;
            if (bucket_end > n - 1) bucket_end = n - 1;
        }

        const int bucket_count = bucket_end - bucket_start;

        // --------- 3. 子区间极值查找 + 平均值计算 ---------
        double sum_x = 0.0, sum_y = 0.0;
        int valid_count = 0;
        candidate_count = 0;

        const int num_sub = std::max(1, static_cast<int>(std::ceil(static_cast<double>(bucket_count) / m_preselection_ratio)));
        const int sub_base      = bucket_count / num_sub;
        const int sub_remainder = bucket_count % num_sub;

        int sub_pos = bucket_start;
        for (int sub = 0; sub < num_sub; ++sub) {
            const int sub_extra = (sub < sub_remainder) ? 1 : 0;
            int sub_end         = sub_pos + sub_base + sub_extra;
            if (sub_end > bucket_end) sub_end = bucket_end;

            if (sub_pos >= sub_end) {
                sub_pos = sub_end;
                continue;
            }

            const int sub_len = sub_end - sub_pos;

            if (!hasNaN) {
                // ===== SIMD加速路径: 无NaN数据 =====
                auto result = simdArgMinMax(y_ptr + sub_pos, sub_len);
                int local_max_idx = result.max_idx + sub_pos;
                int local_min_idx = result.min_idx + sub_pos;

                candidate_stack[candidate_count++] = local_max_idx;
                if (local_max_idx != local_min_idx)
                    candidate_stack[candidate_count++] = local_min_idx;

                // 平均值简化（无NaN，直接累加）
                for (int j = sub_pos; j < sub_end; ++j) {
                    sum_x += getXValue(j);
                    sum_y += y_ptr[j];
                }
                valid_count += sub_len;
            } else {
                // ===== 标量路径: 有NaN数据 =====
                int max_idx    = sub_pos;
                int min_idx    = sub_pos;
                double max_val = y_ptr[sub_pos];
                double min_val = max_val;

                for (int j = sub_pos; j < sub_end; ++j) {
                    const double y = y_ptr[j];

                    if (!std::isnan(y)) {
                        sum_x += getXValue(j);
                        sum_y += y;
                        ++valid_count;
                    }

                    if (std::isnan(y)) continue;
                    if (y > max_val) { max_val = y; max_idx = j; }
                    if (y < min_val) { min_val = y; min_idx = j; }
                }

                candidate_stack[candidate_count++] = max_idx;
                if (max_idx != min_idx)
                    candidate_stack[candidate_count++] = min_idx;
            }

            sub_pos = sub_end;
        }

        if (candidate_count == 0) {
            candidate_stack[candidate_count++] = bucket_start;
        }

        // --------- 4. 虚拟平均点 ---------
        double avg_x, avg_y;
        if (valid_count > 0) {
            avg_x = sum_x / valid_count;
            avg_y = sum_y / valid_count;
        } else {
            avg_x = getXValue(bucket_start);
            avg_y = y_ptr[bucket_start];
        }

        // --------- 5. 从候选点中选择三角形面积最大者 ---------
        const double last_x = m_cached_x.back();
        const double last_y = m_cached_y.back();
        double max_area     = -1.0;
        int best_idx        = candidate_stack[0];

        for (int ci = 0; ci < candidate_count; ++ci) {
            const int idx = candidate_stack[ci];
            const double cx = getXValue(idx);
            const double cy = y_ptr[idx];
            if (std::isnan(cx) || std::isnan(cy)) continue;

            const double area = std::fabs((cx - last_x) * (avg_y - last_y) - (avg_x - last_x) * (cy - last_y));
            if (area > max_area) {
                max_area = area;
                best_idx = idx;
            }
        }

        m_cached_x.push_back(getXValue(best_idx));
        m_cached_y.push_back(y_ptr[best_idx]);
        if (!std::isnan(y_ptr[best_idx])) anyValidY = true;

        current_idx = bucket_end;
    }

    // ========= 6. 始终保留最后一个点 =========
    m_cached_x.push_back(getXValue(n - 1));
    m_cached_y.push_back(y_ptr[n - 1]);
    if (!std::isnan(y_ptr[n - 1])) anyValidY = true;

    // ========= 7. 全NaN退化检测（已合并到主循环） =========
    if (!anyValidY) {
        m_cached_x.clear();
        m_cached_y.clear();
        m_cached_valid = false;
    }
}

}  // namespace QIM
