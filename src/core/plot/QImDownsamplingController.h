#ifndef QIMDOWNSAMPLINGCONTROLLER_H
#define QIMDOWNSAMPLINGCONTROLLER_H

#include "QImPlotDataSeries.h"
#include "QImPlot.h"
#include <memory>

namespace QIM
{

/**
 * \if ENGLISH
 * @brief Strategy class for managing adaptive downsampling in plot item nodes
 * @details Encapsulates all downsampling logic: algorithm selection, threshold
 *          checking, zoom-aware re-sampling, and transparent proxy substitution.
 *          Designed to be held as a member in node PrivateData, eliminating
 *          duplicated downsampling management code per node.
 * @note This is a non-QObject pure logic class. Properties are exposed by the
 *       owning QObject node via Q_PROPERTY delegation.
 * @see QImDownsampleAlgorithm, QImLTTBDownsampler, QImMinMaxLTTBDownsampler
 * \endif
 *
 * \if CHINESE
 * @brief 管理绘图项节点中自适应降采样的策略类
 * @details 封装所有降采样逻辑：算法选择、阈值检测、缩放感知重采样和透明代理替换。
 *          设计为在节点 PrivateData 中作为成员持有，消除每个节点的重复降采样管理代码。
 * @note 这是一个非 QObject 纯逻辑类。属性由持有 QObject 节点通过 Q_PROPERTY 委托暴露。
 * @see QImDownsampleAlgorithm, QImLTTBDownsampler, QImMinMaxLTTBDownsampler
 * \endif
 */
class QIM_CORE_API QImDownsamplingController
{
public:
    QImDownsamplingController() = default;

    // Set the downsampling algorithm
    void setAlgorithm(QImDownsampleAlgorithm algo);
    // Get current algorithm
    QImDownsampleAlgorithm algorithm() const;

    // Set minimum data points to trigger downsampling (default: 20000, min: 100)
    void setThreshold(int points);
    // Get threshold
    int threshold() const;

    // Set source data series (non-owning pointer, must outlive controller)
    void setSource(QImAbstractXYDataSeries* source);

    // Resolve the data series to use for rendering
    QImAbstractXYDataSeries* resolve(int pixelWidth, double xMin, double xMax);

    // Force re-downsample on next resolve() call (call after source data changes)
    void invalidate();

    // Pixel-to-target-points conversion (kPixelToPointRatio = 1.5)
    static constexpr double kPixelToPointRatio = 1.5;
    static int pixelAwareTargetPoints(int pixelWidth);

private:
    QImAbstractXYDataSeries* m_source = nullptr;
    std::unique_ptr<QImAbstractXYDataSeries> m_downsampled;
    QImDownsampleAlgorithm m_algorithm = QImDownsampleAlgorithm::Auto;
    int m_threshold = 20000;
    double m_lastXRange = 0.0;
    int m_lastPixelWidth = 0;
    bool m_initialDone = false;

    void rebuild(int pixelWidth, double xMin, double xMax);
    bool shouldResample(double xRange, int pixelWidth) const;
};

} // namespace QIM

#endif // QIMDOWNSAMPLINGCONTROLLER_H
