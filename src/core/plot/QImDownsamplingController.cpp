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

void QImDownsamplingController::setSource(QImAbstractXYDataSeries* source)
{
    if (m_source != source) {
        m_source = source;
        invalidate();
    }
}

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

void QImDownsamplingController::invalidate()
{
    m_downsampled.reset();
    m_initialDone     = false;
    m_lastXRange      = 0.0;
    m_lastPixelWidth  = 0;
}

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
