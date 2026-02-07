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
 * 3. 视图感知：需外部调用 updateViewRange() 触发下采样
 * 4. 自动处理 Y-only 模式 → 下采样后转为显式 XY 模式（合理，因下采样破坏等间隔假设）
 * 5. 零拷贝优先：小数据集/视图未变时直接透传原始数据
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

    /**
     * @brief 构造代理（转移所有权）
     * @param source 原始数据系列（代理负责释放）
     */
    explicit QImLTTBDownsampler(std::unique_ptr< QImAbstractXYDataSeries > source, int target_points = 2000);

    ~QImLTTBDownsampler() override = default;

    // ===== 核心接口：视图更新触发下采样 =====
    /**
     * @brief 更新视图范围并触发下采样（需在每次绘制前调用）
     * @param x_min 视图X最小值
     * @param x_max 视图X最大值
     * @param threshold 重采样阈值（相对变化比例，默认5%）
     * @return true 表示实际执行了下采样，false 表示复用缓存
     */
    bool updateViewRange(double x_min, double x_max, double threshold = 0.05);

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

    // ===== 诊断接口 =====
    struct Stats
    {
        int original_size;
        int downsampled_size;
        double view_width;
        bool cached;  // 是否复用缓存
    };
    std::optional< Stats > lastStats() const;

private:
    // ===== 内部状态 =====
    QImAbstractXYDataSeries* m_source;  // 原始数据指针
    int m_target_points;
    // ===== 智能状态管理 =====
    mutable int m_initialization_frames = 2;  // 前2帧禁用下采样
    mutable bool m_data_range_cached    = false;
    mutable std::pair< double, double > m_data_x_range;  // 原始数据X范围
    // 缓存状态
    mutable std::vector< double > m_cached_x;
    mutable std::vector< double > m_cached_y;
    mutable bool m_cached_valid = false;
    mutable std::pair< double, double > m_last_range;
    mutable int m_last_source_size = -1;  // 检测原始数据变更

    // 诊断
    mutable std::optional< Stats > m_last_stats;
    // 懒加载数据范围（仅扫描一次）
    void cacheDataRange() const;

    // 决策：当前视图范围是否适合下采样
    bool shouldDownsampleNow(double x_min, double x_max) const;
    // ===== 核心算法 =====
    void performDownsampling(double x_min, double x_max);

    // 查找视图范围内的数据索引 [start_idx, end_idx)
    std::pair< int, int > findVisibleRange(double x_min, double x_max) const;

    // LTTB核心算法（裁剪后数据段）
    void lttbAlgorithm(const double* x_data, const double* y_data, int start_idx, int end_idx, int target_points);
};

}  // namespace QIM

#endif  // QIMLTTBDOWNSAMPLER_H
