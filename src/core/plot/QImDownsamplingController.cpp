#include "QImDownsamplingController.h"
#include "QImLTTBDownsampler.h"
#include "QImMinMaxLTTBDownsampler.h"
#include <algorithm>
#include <cmath>

namespace QIM
{

// Zoom detection thresholds
static constexpr double kZoomInThreshold  = 0.67;
static constexpr double kZoomOutThreshold = 1.5;
static constexpr double kPixelChangeRatio = 0.1;

static constexpr int kMinTargetPoints = 100;
static constexpr int kMaxTargetPoints = 50000;

// ===== public =====

void QImDownsamplingController::setAlgorithm(QImDownsampleAlgorithm algo)
{
    if (m_algorithm != algo) {
        m_algorithm = algo;
        invalidate();
    }
}

QImDownsampleAlgorithm QImDownsamplingController::algorithm() const
{
    return m_algorithm;
}

void QImDownsamplingController::setThreshold(int points)
{
    int clamped = std::max(points, 100);
    if (m_threshold != clamped) {
        m_threshold = clamped;
        invalidate();
    }
}

int QImDownsamplingController::threshold() const
{
    return m_threshold;
}

/**
 * \if ENGLISH
 * @brief Sets the source data series for downsampling
 * @details The controller does not own the source — the caller must ensure
 *          the source outlives the controller. Changing the source invalidates cache.
 * @param source Source data series (non-owning pointer, must outlive controller)
 * \endif
 *
 * \if CHINESE
 * @brief 设置降采样的源数据系列
 * @details 控制器不拥有源数据——调用者必须确保源数据的生命周期超过控制器。
 *          更改源数据会使缓存失效。
 * @param source 源数据系列（非拥有指针，必须超过控制器生命周期）
 * \endif
 */
void QImDownsamplingController::setSource(QImAbstractXYDataSeries* source)
{
    if (m_source != source) {
        m_source = source;
        invalidate();
    }
}

/**
 * \if ENGLISH
 * @brief Resolves the data series to use for rendering with zoom-aware adaptive downsampling
 * @details On first call, builds the initial downsampled proxy. On subsequent calls,
 *          checks whether zoom level or pixel width has changed enough to warrant
 *          re-sampling. Returns the downsampled proxy when active, or the raw source.
 * @param pixelWidth Available pixel width for rendering (used for pixel-aware target count)
 * @param xMin Minimum X value of the visible range
 * @param xMax Maximum X value of the visible range
 * @return Pointer to the data series to use (either downsampled proxy or raw source)
 * \endif
 *
 * \if CHINESE
 * @brief 解析用于渲染的数据系列，支持缩放感知的自适应降采样
 * @details 首次调用时构建初始降采样代理。后续调用时检查缩放级别或像素宽度
 *          的变化是否足够大以触发重新采样。激活时返回降采样代理，否则返回原始源数据。
 * @param pixelWidth 渲染可用的像素宽度（用于像素感知的目标点数）
 * @param xMin 可见范围的最小X值
 * @param xMax 可见范围的最大X值
 * @return 指向要使用的数据系列的指针（降采样代理或原始源数据）
 * \endif
 */
QImAbstractXYDataSeries* QImDownsamplingController::resolve(int pixelWidth, double xMin, double xMax)
{
    if (m_algorithm == QImDownsampleAlgorithm::None)
        return m_source;
    if (!m_source || m_source->size() < m_threshold)
        return m_source;

    double currentXRange = std::fabs(xMax - xMin);

    if (!m_initialDone) {
        rebuild(pixelWidth, xMin, xMax);
        m_initialDone = true;
    } else if (shouldResample(currentXRange, pixelWidth)) {
        rebuild(pixelWidth, xMin, xMax);
    }

    m_lastXRange     = currentXRange;
    m_lastPixelWidth = pixelWidth;

    return m_downsampled ? m_downsampled.get() : m_source;
}

/**
 * \if ENGLISH
 * @brief Invalidates the downsampled cache, forcing re-downsample on next resolve()
 * @details Resets the downsampled proxy and clears all tracking state
 *          (initial done flag, last X range, last pixel width).
 *          Call after source data changes.
 * \endif
 *
 * \if CHINESE
 * @brief 使降采样缓存失效，强制在下一次 resolve() 时重新降采样
 * @details 重置降采样代理并清除所有追踪状态
 *          （初始完成标志、上次X范围、上次像素宽度）。
 *          在源数据变化后调用。
 * \endif
 */
void QImDownsamplingController::invalidate()
{
    m_downsampled.reset();
    m_initialDone     = false;
    m_lastXRange      = 0.0;
    m_lastPixelWidth  = 0;
}

/**
 * \if ENGLISH
 * @brief Converts pixel width to a target point count for downsampling
 * @details Uses kPixelToPointRatio (default 5) to derive target points from
 *          available pixel width. Result clamped to [kMinTargetPoints, kMaxTargetPoints].
 *          This ensures the downsampled data density matches the rendering resolution.
 * @param pixelWidth Available pixel width for rendering
 * @return Target point count, clamped to [100, 50000]
 * \endif
 *
 * \if CHINESE
 * @brief 将像素宽度转换为降采样的目标点数
 * @details 使用 kPixelToPointRatio（默认5）从可用像素宽度推导目标点数。
 *          结果限制在[kMinTargetPoints, kMaxTargetPoints]范围内。
 *          这确保降采样后的数据密度与渲染分辨率匹配。
 * @param pixelWidth 渲染可用的像素宽度
 * @return 目标点数，限制在[100, 50000]范围内
 * \endif
 */
int QImDownsamplingController::pixelAwareTargetPoints(int pixelWidth)
{
    if (pixelWidth <= 0) return kMinTargetPoints;
    int target = static_cast<int>(static_cast<double>(pixelWidth) * kPixelToPointRatio);
    return std::clamp(target, kMinTargetPoints, kMaxTargetPoints);
}

// ===== private =====

void QImDownsamplingController::rebuild(int pixelWidth, double /*xMin*/, double /*xMax*/)
{
    m_downsampled.reset();

    if (m_algorithm == QImDownsampleAlgorithm::None || !m_source)
        return;
    if (m_source->size() < m_threshold)
        return;

    int dataSize                = m_source->size();
    int effectiveTarget         = pixelAwareTargetPoints(pixelWidth);
    QImDownsampleAlgorithm effectiveAlgo = m_algorithm;

    // Auto: select algorithm based on data size
    if (effectiveAlgo == QImDownsampleAlgorithm::Auto) {
        if (dataSize > 100000)
            effectiveAlgo = QImDownsampleAlgorithm::MinMaxLTTB;
        else if (dataSize > 10000)
            effectiveAlgo = QImDownsampleAlgorithm::LTTB;
        else
            return; // <10K, no downsampling needed
    }

    // Create appropriate downsampler
    switch (effectiveAlgo) {
    case QImDownsampleAlgorithm::LTTB: {
        auto* sampler = new QImLTTBDownsampler(m_source, effectiveTarget);
        m_downsampled.reset(sampler);
        sampler->downSampler();
        break;
    }
    case QImDownsampleAlgorithm::MinMaxLTTB: {
        auto* sampler = new QImMinMaxLTTBDownsampler(m_source, effectiveTarget, 4.0, false);
        m_downsampled.reset(sampler);
        sampler->downSampler();
        break;
    }
    default:
        break;
    }
}

bool QImDownsamplingController::shouldResample(double xRange, int pixelWidth) const
{
    // Pixel width change detection
    if (m_lastPixelWidth > 0) {
        double pixelDelta = std::fabs(static_cast<double>(pixelWidth - m_lastPixelWidth));
        if (pixelDelta > m_lastPixelWidth * kPixelChangeRatio)
            return true;
    }

    // Zoom detection
    if (m_lastXRange > 1e-15) {
        double zoomRatio = xRange / m_lastXRange;
        if (zoomRatio > kZoomOutThreshold || zoomRatio < kZoomInThreshold)
            return true;
    } else {
        return true; // first comparison, force resample
    }

    return false;
}

} // namespace QIM
