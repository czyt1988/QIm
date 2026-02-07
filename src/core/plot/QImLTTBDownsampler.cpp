#include "QImLTTBDownsampler.h"
#include <algorithm>
#include <cmath>
#include <cassert>

namespace QIM
{

// ===== 构造函数 =====
QImLTTBDownsampler::QImLTTBDownsampler(QImAbstractXYDataSeries* source, int target_points)
    : m_source(source), m_target_points(target_points)
{
    assert(source && "Source must not be null");
}

// ===== 视图更新主入口 =====
bool QImLTTBDownsampler::updateViewRange(double x_min, double x_max, double threshold)
{
    // ===== 智能决策：是否执行下采样 =====
    bool should_downsample = shouldDownsampleNow(x_min, x_max);

    if (!should_downsample) {
        // 禁用下采样：透传原始数据
        m_cached_valid = false;  // 触发xRawData()/yRawData()返回原始指针

        // 更新诊断信息
        m_last_stats = Stats {
            m_source->size(),
            m_source->size(),
            x_max - x_min,
            true  // cached = true 表示透传
        };

        // 首次绘制保护计数器递减
        if (m_initialization_frames > 0) {
            --m_initialization_frames;
        }

        return false;
    }

    const int current_size   = m_source->size();
    const auto current_range = std::make_pair(x_min, x_max);

    // 情况1：数据为空，清空缓存
    if (current_size == 0) {
        m_cached_valid = false;
        m_last_stats.reset();
        return false;
    }

    // 情况2：无需下采样（数据量小）
    if (current_size <= m_target_points * 2) {
        m_cached_valid = false;  // 透传原始数据
        m_last_stats   = Stats { current_size, current_size, x_max - x_min, true };
        return false;
    }

    // 情况3：检测是否需要重新下采样
    bool need_resample = false;

    if (!m_cached_valid) {
        need_resample = true;
    } else if (current_size != m_last_source_size) {
        need_resample = true;  // 数据变更
    } else {
        // 检查视图范围变化
        const double last_width    = m_last_range.second - m_last_range.first;
        const double current_width = x_max - x_min;
        const double width_change  = std::abs(current_width - last_width) / std::max(last_width, 1e-6);

        const double last_center    = (m_last_range.first + m_last_range.second) * 0.5;
        const double current_center = (x_min + x_max) * 0.5;
        const double center_change  = std::abs(current_center - last_center) / std::max(last_width, 1e-6);

        if (width_change > threshold || center_change > threshold) {
            need_resample = true;
        }
    }

    if (need_resample) {
        performDownsampling(x_min, x_max);
        m_last_range       = current_range;
        m_last_source_size = current_size;
        return true;
    }

    // 复用缓存
    m_last_stats->cached = true;
    return false;
}

int QImLTTBDownsampler::size() const
{
    // 优先返回下采样数据大小，若未下采样则返回原始数据大小
    return m_cached_valid ? static_cast< int >(m_cached_x.size()) : m_source->size();
}

bool QImLTTBDownsampler::isContiguous() const
{
    return m_source->isContiguous();
    ;
}

int QImLTTBDownsampler::stride() const
{
    return m_source->stride();
}

const double* QImLTTBDownsampler::xRawData() const
{
    // 关键：返回下采样后X数据指针（若有效），否则透传原始数据
    return m_cached_valid ? m_cached_x.data() : m_source->xRawData();
}

const double* QImLTTBDownsampler::yRawData() const
{
    return m_cached_valid ? m_cached_y.data() : m_source->yRawData();
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
    if (points >= 100)
        m_target_points = points;
}

int QImLTTBDownsampler::targetPoints() const
{
    return m_target_points;
}

std::optional< QImLTTBDownsampler::Stats > QImLTTBDownsampler::lastStats() const
{
    return m_last_stats;
}

// ===== 核心：裁剪+下采样 =====
void QImLTTBDownsampler::performDownsampling(double x_min, double x_max)
{
    // 1. 查找可见数据范围
    auto [ start_idx, end_idx ] = findVisibleRange(x_min, x_max);
    const int visible_count     = end_idx - start_idx;

    // 2. 边界情况处理
    if (visible_count == 0) {
        m_cached_x.clear();
        m_cached_y.clear();
        m_cached_valid = true;
        m_last_stats   = Stats { m_source->size(), 0, x_max - x_min, false };
        return;
    }

    if (visible_count <= m_target_points) {
        // 可见数据已足够稀疏，直接复制
        m_cached_x.resize(visible_count);
        m_cached_y.resize(visible_count);

        if (const double* x_ptr = m_source->xRawData()) {
            std::copy(x_ptr + start_idx, x_ptr + end_idx, m_cached_x.begin());
        } else {
            // Y-only 模式：生成显式X坐标
            const double x_start = m_source->xStart();
            const double x_scale = m_source->xScale();
            for (int i = 0; i < visible_count; ++i) {
                m_cached_x[ i ] = x_start + (start_idx + i) * x_scale;
            }
        }
        std::copy(m_source->yRawData() + start_idx, m_source->yRawData() + end_idx, m_cached_y.begin());

        m_cached_valid = true;
        m_last_stats   = Stats { m_source->size(), visible_count, x_max - x_min, false };
        return;
    }

    // 3. 执行LTTB下采样
    lttbAlgorithm(m_source->xRawData(), m_source->yRawData(), start_idx, end_idx, std::min(m_target_points, visible_count));

    m_cached_valid = true;
    m_last_stats   = Stats { m_source->size(), static_cast< int >(m_cached_x.size()), x_max - x_min, false };
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

// QImLTTBDownsampler.cpp
void QImLTTBDownsampler::cacheDataRange() const
{
    if (m_data_range_cached || m_source->size() == 0)
        return;

    double x_min   = std::numeric_limits< double >::max();
    double x_max   = std::numeric_limits< double >::lowest();
    const int size = m_source->size();

    if (const double* x_ptr = m_source->xRawData()) {
        // XY模式：扫描X数据
        for (int i = 0; i < size; ++i) {
            double x = x_ptr[ i ];
            if (std::isfinite(x)) {
                x_min = std::min(x_min, x);
                x_max = std::max(x_max, x);
            }
        }
    } else {
        // Y-only模式：计算隐式X范围
        double x_start = m_source->xStart();
        double x_scale = m_source->xScale();
        x_min          = x_start;
        x_max          = x_start + (size - 1) * x_scale;
    }

    // 处理全NaN/无效数据
    if (!std::isfinite(x_min) || !std::isfinite(x_max) || x_min > x_max) {
        x_min = 0.0;
        x_max = 1.0;
    }

    m_data_x_range      = { x_min, x_max };
    m_data_range_cached = true;
}

bool QImLTTBDownsampler::shouldDownsampleNow(double x_min, double x_max) const
{
    const int total_size = m_source->size();

    // 规则1：数据量小，不需要下采样
    if (total_size <= m_target_points) {
        return false;
    }

    // 规则2：首次绘制保护（前2帧透传原始数据）
    if (m_initialization_frames > 0) {
        return false;
    }

    // 规则3：检测ImPlot默认范围 (0,1) - 未初始化状态
    // 使用容差避免浮点误差（ImPlot可能有微小padding）
    const double epsilon = 1e-4;
    if (std::abs(x_min) < epsilon && std::abs(x_max - 1.0) < epsilon) {
        return false;
    }

    // 规则4：缓存数据范围（懒加载）
    if (!m_data_range_cached) {
        cacheDataRange();
    }

    // 规则5：视图范围异常小（可能未完成自适应）
    double data_width = m_data_x_range.second - m_data_x_range.first;
    double view_width = x_max - x_min;

    if (data_width > 1e-6 && view_width < data_width * 0.001) {
        return false;  // 视图宽度 < 数据宽度的0.1%
    }

    // 规则6：视图完全不包含数据（可能未初始化）
    if (x_max < m_data_x_range.first - 1e-6 || x_min > m_data_x_range.second + 1e-6) {
        return false;
    }

    return true;
}

// ===== LTTB核心算法（O(n)）=====
void QImLTTBDownsampler::lttbAlgorithm(const double* x_data, const double* y_data, int start_idx, int end_idx, int target_points)
{
    const int n = end_idx - start_idx;
    if (n <= target_points || target_points < 3) {
        // 直接复制
        m_cached_x.resize(n);
        m_cached_y.resize(n);

        if (x_data) {
            std::copy(x_data + start_idx, x_data + end_idx, m_cached_x.begin());
        } else {
            const double x_start = m_source->xStart();
            const double x_scale = m_source->xScale();
            for (int i = 0; i < n; ++i) {
                m_cached_x[ i ] = x_start + (start_idx + i) * x_scale;
            }
        }
        std::copy(y_data + start_idx, y_data + end_idx, m_cached_y.begin());
        return;
    }

    // 预分配缓存
    m_cached_x.clear();
    m_cached_y.clear();
    m_cached_x.reserve(target_points);
    m_cached_y.reserve(target_points);

    // 辅助lambda：获取X坐标（处理Y-only）
    auto getX = [ this, x_data, start_idx ](int idx) -> double {
        if (x_data)
            return x_data[ start_idx + idx ];
        return m_source->xStart() + (start_idx + idx) * m_source->xScale();
    };

    // 1. 保留首点
    m_cached_x.push_back(getX(0));
    m_cached_y.push_back(y_data[ start_idx ]);

    // 2. 中间点：LTTB核心
    const double avg_bucket_size = static_cast< double >(n - 2) / (target_points - 2);
    double bucket_left           = 1.0;

    for (int i = 1; i < target_points - 1; ++i) {
        double bucket_right = std::min(bucket_left + avg_bucket_size, static_cast< double >(n - 1));
        if (i == target_points - 2)
            bucket_right = n - 1;  // 最后一个桶包含末尾

        // 计算桶内平均点（用于三角形第三点）
        double avg_x = 0.0, avg_y = 0.0;
        int bucket_start = static_cast< int >(bucket_left);
        int bucket_end   = static_cast< int >(bucket_right) + 1;
        int bucket_size  = bucket_end - bucket_start;

        for (int j = bucket_start; j < bucket_end; ++j) {
            avg_x += getX(j);
            avg_y += y_data[ start_idx + j ];
        }
        avg_x /= bucket_size;
        avg_y /= bucket_size;

        // 在桶内寻找最大三角形面积的点
        double max_area = -1.0;
        int max_idx     = bucket_start;

        for (int j = bucket_start; j < bucket_end; ++j) {
            // 三角形面积 = |(x1-x0)*(y2-y0) - (x2-x0)*(y1-y0)| / 2
            // 优化：省略/2和abs（比较时符号不影响）
            double area = std::abs((getX(j) - m_cached_x.back()) * (avg_y - m_cached_y.back())
                                   - (avg_x - m_cached_x.back()) * (y_data[ start_idx + j ] - m_cached_y.back()));

            if (area > max_area) {
                max_area = area;
                max_idx  = j;
            }
        }

        m_cached_x.push_back(getX(max_idx));
        m_cached_y.push_back(y_data[ start_idx + max_idx ]);
        bucket_left = bucket_right + 1;
    }

    // 3. 保留末点
    m_cached_x.push_back(getX(n - 1));
    m_cached_y.push_back(y_data[ end_idx - 1 ]);
}

}  // namespace QIM
