#include "QImPlotHistogramItemNode.h"
#include "QtImGuiUtils.h"
#include "implot.h"

namespace QIM
{
QImPlotHistogramItemNode::QImPlotHistogramItemNode(QObject* parent) : QImPlotItemNode(parent)
{
}

QImPlotHistogramItemNode::~QImPlotHistogramItemNode()
{
}

const std::vector< double >& QImPlotHistogramItemNode::data() const
{
    return m_values;
}

int QImPlotHistogramItemNode::binCount() const
{
    return m_binCount;
}

void QImPlotHistogramItemNode::setBinCount(int bins)
{
    if (m_binCount != bins) {
        m_binCount = bins;
        Q_EMIT histogramConfigChanged();
    }
}

double QImPlotHistogramItemNode::barScale() const
{
    return m_barScale;
}

void QImPlotHistogramItemNode::setBarScale(double scale)
{
    if (!qFuzzyCompare(m_barScale + 1.0, scale + 1.0)) {
        m_barScale = scale;
        Q_EMIT histogramConfigChanged();
    }
}

bool QImPlotHistogramItemNode::isHorizontal() const
{
    return (m_histogramFlags & ImPlotHistogramFlags_Horizontal) != 0;
}

void QImPlotHistogramItemNode::setHorizontal(bool on)
{
    const int oldFlags = m_histogramFlags;
    if (on) {
        m_histogramFlags |= ImPlotHistogramFlags_Horizontal;
    } else {
        m_histogramFlags &= ~ImPlotHistogramFlags_Horizontal;
    }
    if (m_histogramFlags != oldFlags) {
        Q_EMIT histogramConfigChanged();
    }
}

bool QImPlotHistogramItemNode::isCumulative() const
{
    return (m_histogramFlags & ImPlotHistogramFlags_Cumulative) != 0;
}

void QImPlotHistogramItemNode::setCumulative(bool on)
{
    const int oldFlags = m_histogramFlags;
    if (on) {
        m_histogramFlags |= ImPlotHistogramFlags_Cumulative;
    } else {
        m_histogramFlags &= ~ImPlotHistogramFlags_Cumulative;
    }
    if (m_histogramFlags != oldFlags) {
        Q_EMIT histogramConfigChanged();
    }
}

bool QImPlotHistogramItemNode::isDensity() const
{
    return (m_histogramFlags & ImPlotHistogramFlags_Density) != 0;
}

void QImPlotHistogramItemNode::setDensity(bool on)
{
    const int oldFlags = m_histogramFlags;
    if (on) {
        m_histogramFlags |= ImPlotHistogramFlags_Density;
    } else {
        m_histogramFlags &= ~ImPlotHistogramFlags_Density;
    }
    if (m_histogramFlags != oldFlags) {
        Q_EMIT histogramConfigChanged();
    }
}

bool QImPlotHistogramItemNode::isNoOutliers() const
{
    return (m_histogramFlags & ImPlotHistogramFlags_NoOutliers) != 0;
}

void QImPlotHistogramItemNode::setNoOutliers(bool on)
{
    const int oldFlags = m_histogramFlags;
    if (on) {
        m_histogramFlags |= ImPlotHistogramFlags_NoOutliers;
    } else {
        m_histogramFlags &= ~ImPlotHistogramFlags_NoOutliers;
    }
    if (m_histogramFlags != oldFlags) {
        Q_EMIT histogramConfigChanged();
    }
}

bool QImPlotHistogramItemNode::isRangeEnabled() const
{
    return m_rangeEnabled;
}

void QImPlotHistogramItemNode::setRangeEnabled(bool on)
{
    if (m_rangeEnabled != on) {
        m_rangeEnabled = on;
        Q_EMIT histogramConfigChanged();
    }
}

double QImPlotHistogramItemNode::rangeMin() const
{
    return m_rangeMin;
}

void QImPlotHistogramItemNode::setRangeMin(double value)
{
    if (!qFuzzyCompare(m_rangeMin + 1.0, value + 1.0)) {
        m_rangeMin = value;
        Q_EMIT histogramConfigChanged();
    }
}

double QImPlotHistogramItemNode::rangeMax() const
{
    return m_rangeMax;
}

void QImPlotHistogramItemNode::setRangeMax(double value)
{
    if (!qFuzzyCompare(m_rangeMax + 1.0, value + 1.0)) {
        m_rangeMax = value;
        Q_EMIT histogramConfigChanged();
    }
}

QColor QImPlotHistogramItemNode::fillColor() const
{
    return m_fillColor;
}

void QImPlotHistogramItemNode::setFillColor(const QColor& color)
{
    if (m_fillColor != color) {
        m_fillColor = color;
        Q_EMIT fillColorChanged(color);
    }
}

int QImPlotHistogramItemNode::histogramFlags() const
{
    return m_histogramFlags;
}

void QImPlotHistogramItemNode::setHistogramFlags(int flags)
{
    if (m_histogramFlags != flags) {
        m_histogramFlags = flags;
        Q_EMIT histogramConfigChanged();
    }
}

bool QImPlotHistogramItemNode::beginDraw()
{
    if (m_values.empty()) {
        return false;
    }

    if (m_fillColor.isValid()) {
        ImPlot::SetNextFillStyle(toImVec4(m_fillColor));
        ImPlot::SetNextLineStyle(toImVec4(m_fillColor));
    }

    const ImPlotRange range = m_rangeEnabled ? ImPlotRange(m_rangeMin, m_rangeMax) : ImPlotRange();
    ImPlot::PlotHistogram(
        labelConstData(),
        m_values.data(),
        static_cast< int >(m_values.size()),
        m_binCount,
        m_barScale,
        range,
        static_cast< ImPlotHistogramFlags >(m_histogramFlags)
    );
    return false;
}
}  // namespace QIM
