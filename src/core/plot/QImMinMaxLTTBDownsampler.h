#ifndef QIMMINMAXLTTBDOWNSAMPLER_H
#define QIMMINMAXLTTBDOWNSAMPLER_H

#include "QImPlotDataSeries.h"
#include <vector>
#include <memory>
#include <optional>
#include <utility>

namespace QIM
{

/**
 * @class QImMinMaxLTTBDownsampler
 * @brief MinMaxLTTB 下采样代理类 - 作为 QImAbstractXYDataSeries 的透明装饰器
 *
 * @section algo_principle 算法原理
 *
 * MinMaxLTTB 是比利时根特大学 predict-idlab 团队提出的 LTTB 优化算法。
 * 核心思想：**在计算三角形面积之前，先用 MinMax 策略预筛选出少量候选点，
 * 只在候选点上计算面积**，从而大幅减少计算量。
 *
 * @subsection lttb_vs_minmax 标准 LTTB vs MinMaxLTTB
 *
 * @code
 * 标准 LTTB:
 * ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
 * │  ·  │  ·  │  ·  │  ·  │  ·  │  ·  │  ·  │  ·  │  每个桶遍历所有点
 * │ · · │ · · │ · · │ · · │ · · │ · · │ · · │ · · │  计算三角形面积
 * └─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘
 *        ↓ 面积计算次数 = 桶内点数 × 桶数量
 *
 * MinMaxLTTB:
 * ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
 * │  ★  │  ★  │  ★  │  ★  │  ★  │  ★  │  ★  │  ★  │  先找极值点 (★)
 * │ ★ · │ ★ · │ ★ · │ ★ · │ ★ · │ ★ · │ ★ · │ ★ · │  只在★上算面积
 * └─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘
 *        ↓ 面积计算次数 = 候选点数 × 桶数量 (候选点 << 桶内点数)
 * @endcode
 *
 * @subsection algo_steps 核心步骤
 *
 * 1. **分桶**: 将数据划分为 (target_points - 2) 个桶
 * 2. **MinMax 预筛选**: 每个桶内划分子区间，保留每个子区间的最大值和最小值作为候选点
 * 3. **面积计算**: 仅在候选点上计算三角形面积，选择面积最大的点
 * 4. **保留端点**: 始终保留第一个和最后一个数据点
 *
 * @section algo_advantages 算法优点
 *
 * | 优势 | 说明 | 实际效果 |
 * |------|------|----------|
 * | **性能提升显著** | 面积计算次数减少 10-30 倍 | 百万点下采样从 500ms → 20ms |
 * | **视觉效果几乎无损失** | 极值点保留了波峰波谷特征 | 人眼难以区分与标准 LTTB 的差异 |
 * | **参数可调** | preselection_ratio 控制精度/性能平衡 | 可根据场景灵活调整 |
 * | **内存友好** | 不需要额外的大规模临时缓存 | 与标准 LTTB 内存占用相当 |
 * | **保持关键特征** | MinMax 确保极值不会被遗漏 | 适合监控告警等场景 |
 *
 * @section algo_disadvantages 算法缺点
 *
 * | 劣势 | 说明 | 缓解方案 |
 * |------|------|----------|
 * | **可能遗漏非极值的重要点** | 某些斜率变化点可能不是极值 | 增大 preselection_ratio |
 * | **参数敏感** | 比例设置不当会影响效果 | 默认值 4.0 适合大多数场景 |
 * | **子区间划分边界效应** | 极值点恰好在边界时可能重复 | 代码中已做去重处理 |
 * | **不适合极度不规则数据** | 高频噪声数据效果略降 | 可先做平滑预处理 |
 *
 * @section performance_comparison 性能对比
 *
 * @section design_features 设计特点
 *
 * 1. **完全符合 QImAbstractXYDataSeries 接口** - 可无缝替换原始数据
 * 2. **内部维护下采样缓存** - xRawData()/yRawData() 返回下采样后数据指针
 * 3. **全局一次性下采样** - 初始化时对全量数据下采样，视图变化不重新采样
 * 4. **自动处理 Y-only 模式** - 下采样后转为显式 XY 模式
 * 5. **零拷贝优先** - 小数据集时直接透传原始数据
 * 6. **性能优化** - 采用 MinMax 预筛选策略，比标准 LTTB 快 10-30 倍
 *
 * @section usage_examples 使用示例
 *
 * @subsection basic_usage 基本使用
 *
 * @code
 * // 假设已有原始数据系列
 * QImPlotDataSeries* originalData = new QImPlotDataSeries();
 * // ... 填充 100 万个数据点 ...
 *
 * // 创建 MinMaxLTTB 下采样代理
 * QImMinMaxLTTBDownsampler* downsampledData =
 *     new QImMinMaxLTTBDownsampler(originalData, 2000, 4.0);
 *     // 参数 1: 原始数据指针
 *     // 参数 2: 目标点数 (默认 2000)
 *     // 参数 3: 预筛选比例 (默认 4.0)
 *
 * // 直接使用代理对象进行绘图
 * plotWidget->setSeries(downsampledData);
 *
 * //注意：originalData 的生命周期必须长于 downsampledData
 * @endcode
 *
 * @subsection dynamic_adjust 动态调整参数
 *
 * @code
 * // 根据屏幕分辨率调整目标点数
 * void onWindowResized(int width, int height)
 * {
 *     int targetPoints = static_cast<int>(width * 1.5);  // 1.5 倍屏幕宽度
 *     downsampledData->setTargetPoints(targetPoints);
 *     downsampledData->downSampler();  // 触发重新下采样
 * }
 *
 * // 根据性能需求调整预筛选比例
 * void onPerformanceModeChanged(bool highPerformance)
 * {
 *     if (highPerformance) {
 *         // 高性能模式：减少候选点，更快但精度略降
 *         downsampledData->setPreselectionRatio(2.0);
 *     } else {
 *         // 高质量模式：增加候选点，更接近标准 LTTB
 *         downsampledData->setPreselectionRatio(8.0);
 *     }
 *     downsampledData->downSampler();
 * }
 * @endcode
 *
 * @subsection typical_scenarios 典型应用场景
 *
 * @code
 * // ===== 场景 1: 实时监控系统 (高频数据) =====
 * QImMinMaxLTTBDownsampler* monitorSeries =
 *     new QImMinMaxLTTBDownsampler(rawData, 3000, 2.0);  // 低比例，追求速度
 *
 * // ===== 场景 2: 历史数据分析 (离线处理) =====
 * QImMinMaxLTTBDownsampler* analysisSeries =
 *     new QImMinMaxLTTBDownsampler(rawData, 5000, 8.0);  // 高比例，追求质量
 *
 * // ===== 场景 3: 移动端/嵌入式 (资源受限) =====
 * QImMinMaxLTTBDownsampler* mobileSeries =
 *     new QImMinMaxLTTBDownsampler(rawData, 1000, 2.0);  // 少点数 + 低比例
 * @endcode
 *
 * @section parameter_tuning 参数调优建议
 *
 * @subsection preselection_ratio 预筛选比例推荐值
 *
 * | 应用场景 | 推荐值 | 说明 |
 * |----------|--------|------|
 * | 实时高频监控 | 2.0-3.0 | 优先保证性能 |
 * | 通用数据可视化 | 4.0-5.0 | 性能与质量平衡 (默认) |
 * | 金融/K 线图 | 6.0-8.0 | 需要保留更多极值特征 |
 * | 科学数据/论文图表 | 8.0-10.0 | 追求最高视觉保真度 |
 * | 极度噪声数据 | 10.0+ | 减少噪声干扰 |
 *
 * @subsection target_points 目标点数推荐值
 *
 * @code
 * // 根据屏幕宽度自动计算
 * int screenWidth = QApplication::primaryScreen()->geometry().width();
 * int targetPoints = static_cast<int>(screenWidth * 1.2);  // 1.2-1.5 倍
 *
 * // 根据数据特征调整
 * if (dataHasSharpPeaks) {
 *     targetPoints *= 1.5;  // 尖峰多则增加点数
 * }
 * if (isMobileDevice) {
 *     targetPoints *= 0.6;  // 移动端减少点数
 * }
 * @endcode
 *
 * @section best_practices 最佳实践
 *
 * 1. 数据量 < 1 万点 → 无需下采样
 * 2. 数据量 1 万 -10 万点 → 标准 LTTB
 * 3. 数据量 > 10 万点 → MinMaxLTTB
 * 4. 始终保证 originalData 生命周期长于代理对象
 * 5. 参数调整后记得调用 downSampler() 刷新缓存
 *
 * @note 本算法论文参考：https://github.com/predict-idlab/MinMaxLTTB
 *
 * @see QImLTTBDownsampler, QImAbstractXYDataSeries
 */
class QIM_CORE_API QImMinMaxLTTBDownsampler : public QImAbstractXYDataSeries
{
public:
    /**
     * @brief 构造代理（不拥有原始数据所有权）
     * @param source 原始数据系列（必须保证生命周期长于代理）
     * @param target_points 目标点数（默认 2000，≈1.5 倍典型屏幕宽度）
     * @param preselection_ratio MinMax 预筛选比例（默认 4.0，即每个桶保留 4 个极值候选点）
     */
    explicit QImMinMaxLTTBDownsampler(QImAbstractXYDataSeries* source, int target_points = 2000, double preselection_ratio = 4.0);
    ~QImMinMaxLTTBDownsampler() override = default;

    // ===== QImAbstractXYDataSeries 接口重写 =====
    int type() const override
    {
        return XYData;
    }  // 代理后总是 XY 模式

    int size() const override;

    bool isContiguous() const override;  // 缓存数据总是连续

    int stride() const override;

    const double* xRawData() const override;

    const double* yRawData() const override;

    // 代理后不再支持 Y-only 模式（下采样破坏等间隔假设），返回默认值
    double xScale() const override;
    double xStart() const override;
    int offset() const override;

    // ===== 配置接口 =====
    void setTargetPoints(int points);

    int targetPoints() const;

    void setPreselectionRatio(double ratio);

    double preselectionRatio() const;

    virtual double xValue(int index) const override;
    virtual double yValue(int index) const override;

    // 根据目标点数更新数据，这个函数在目标点数变化，或原数据发生变化时调用，用于更新
    void downSampler();

private:
    // ===== 内部状态 =====
    QImAbstractXYDataSeries* m_source;  // 原始数据指针
    int m_target_points;
    double m_preselection_ratio;  // MinMax 预筛选比例

    // 缓存状态
    mutable std::vector< double > m_cached_x;
    mutable std::vector< double > m_cached_y;
    mutable bool m_cached_valid = false;


    // 查找视图范围内的数据索引 [start_idx, end_idx)
    std::pair< int, int > findVisibleRange(double x_min, double x_max) const;

    // MinMaxLTTB 核心算法（O(n)，带 MinMax 预筛选）
    void minMaxLTTB(const double* x_data, const double* y_data, int start_idx, int end_idx, int target_points);
};

}  // namespace QIM

#endif  // QIMMINMAXLTTBDOWNSAMPLER_H
