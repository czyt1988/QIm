#ifndef QIMLTTBDOWNSAMPLER_H
#define QIMLTTBDOWNSAMPLER_H
#include "QImPlotDataSeries.h"
#include <vector>
#include <memory>
#include <optional>
#include <utility>

namespace QIM
{

/**
 * @brief LTTB 下采样代理类 - 作为 QImAbstractXYDataSeries 的透明装饰器
 *
 * 设计特点：
 * 1. 完全符合 QImAbstractXYDataSeries 接口，可无缝替换原始数据
 * 2. 内部维护下采样缓存，xRawData()/yRawData() 返回下采样后数据指针
 * 3. 全局一次性下采样：初始化时对全量数据下采样，视图变化不重新采样
 * 4. 自动处理 Y-only 模式 → 下采样后转为显式 XY 模式
 * 5. 零拷贝优先：小数据集时直接透传原始数据
 */
class QIM_CORE_API QImLTTBDownsampler : public QImAbstractXYDataSeries
{
public:
    /**
     * @brief 构造代理（不拥有原始数据所有权）
     * @param source 原始数据系列（必须保证生命周期长于代理）
     * @param target_points 目标点数（默认2000，≈1.5倍典型屏幕宽度）
     */
    explicit QImLTTBDownsampler(QImAbstractXYDataSeries* source, int target_points = 2000);
    ~QImLTTBDownsampler() override = default;

    // ===== QImAbstractXYDataSeries 接口重写 =====
    int type() const override
    {
        return XYData;
    }  // 代理后总是XY模式

    int size() const override;

    bool isContiguous() const override;  // 缓存数据总是连续

    int stride() const override;

    const double* xRawData() const override;

    const double* yRawData() const override;

    // 代理后不再支持Y-only模式（下采样破坏等间隔假设），返回默认值
    double xScale() const override;
    double xStart() const override;
    int offset() const override;

    // ===== 配置接口 =====
    void setTargetPoints(int points);

    int targetPoints() const;

    virtual double xValue(int index) const override;
    virtual double yValue(int index) const override;
    // 根据目标点数更新数据，这个函数在目标点数变化,或原数据发生变化是调用，用于更新
    void downSampler();

private:
    // ===== 内部状态 =====
    QImAbstractXYDataSeries* m_source;  // 原始数据指针
    int m_target_points;
    // 缓存状态
    mutable std::vector< double > m_cached_x;
    mutable std::vector< double > m_cached_y;
    mutable bool m_cached_valid = false;


    // 查找视图范围内的数据索引 [start_idx, end_idx)
    std::pair< int, int > findVisibleRange(double x_min, double x_max) const;

    // LTTB核心算法（裁剪后数据段）
    void lttb(const double* x_data, const double* y_data, int start_idx, int end_idx, int target_points);
};

}  // namespace QIM

#endif  // QIMLTTBDOWNSAMPLER_H
