#include "QImPlot3DSurfaceItemNode.h"
#include "QtImGuiUtils.h"
#include "implot3d.h"
#include <algorithm>

namespace QIM
{
QImPlot3DSurfaceItemNode::QImPlot3DSurfaceItemNode(QObject* parent) : QImPlot3DItemNode(parent)
{
}

QImPlot3DSurfaceItemNode::~QImPlot3DSurfaceItemNode()
{
}

const std::vector< double >& QImPlot3DSurfaceItemNode::xData() const
{
    return m_xData;
}

const std::vector< double >& QImPlot3DSurfaceItemNode::yData() const
{
    return m_yData;
}

const std::vector< double >& QImPlot3DSurfaceItemNode::zData() const
{
    return m_zData;
}

int QImPlot3DSurfaceItemNode::xCount() const
{
    return m_xCount;
}

void QImPlot3DSurfaceItemNode::setXCount(int count)
{
    if (count > 0 && m_xCount != count) {
        m_xCount = count;
        trimDataToGrid();
        Q_EMIT gridShapeChanged();
    }
}

int QImPlot3DSurfaceItemNode::yCount() const
{
    return m_yCount;
}

void QImPlot3DSurfaceItemNode::setYCount(int count)
{
    if (count > 0 && m_yCount != count) {
        m_yCount = count;
        trimDataToGrid();
        Q_EMIT gridShapeChanged();
    }
}

bool QImPlot3DSurfaceItemNode::isLinesVisible() const
{
    return (m_surfaceFlags & ImPlot3DSurfaceFlags_NoLines) == 0;
}

void QImPlot3DSurfaceItemNode::setLinesVisible(bool visible)
{
    const int oldFlags = m_surfaceFlags;
    if (visible) {
        m_surfaceFlags &= ~ImPlot3DSurfaceFlags_NoLines;
    } else {
        m_surfaceFlags |= ImPlot3DSurfaceFlags_NoLines;
    }
    if (m_surfaceFlags != oldFlags) {
        Q_EMIT surfaceFlagChanged();
    }
}

bool QImPlot3DSurfaceItemNode::isFillVisible() const
{
    return (m_surfaceFlags & ImPlot3DSurfaceFlags_NoFill) == 0;
}

void QImPlot3DSurfaceItemNode::setFillVisible(bool visible)
{
    const int oldFlags = m_surfaceFlags;
    if (visible) {
        m_surfaceFlags &= ~ImPlot3DSurfaceFlags_NoFill;
    } else {
        m_surfaceFlags |= ImPlot3DSurfaceFlags_NoFill;
    }
    if (m_surfaceFlags != oldFlags) {
        Q_EMIT surfaceFlagChanged();
    }
}

bool QImPlot3DSurfaceItemNode::isMarkersVisible() const
{
    return (m_surfaceFlags & ImPlot3DSurfaceFlags_NoMarkers) == 0;
}

void QImPlot3DSurfaceItemNode::setMarkersVisible(bool visible)
{
    const int oldFlags = m_surfaceFlags;
    if (visible) {
        m_surfaceFlags &= ~ImPlot3DSurfaceFlags_NoMarkers;
    } else {
        m_surfaceFlags |= ImPlot3DSurfaceFlags_NoMarkers;
    }
    if (m_surfaceFlags != oldFlags) {
        Q_EMIT surfaceFlagChanged();
    }
}

int QImPlot3DSurfaceItemNode::markerShape() const
{
    return m_markerShape;
}

void QImPlot3DSurfaceItemNode::setMarkerShape(int shape)
{
    if (m_markerShape != shape) {
        m_markerShape = shape;
        Q_EMIT markerShapeChanged(shape);
    }
}

float QImPlot3DSurfaceItemNode::markerSize() const
{
    return m_markerSize;
}

void QImPlot3DSurfaceItemNode::setMarkerSize(float size)
{
    if (!qFuzzyCompare(m_markerSize, size)) {
        m_markerSize = size;
        Q_EMIT markerStyleChanged();
    }
}

float QImPlot3DSurfaceItemNode::markerWeight() const
{
    return m_markerWeight;
}

void QImPlot3DSurfaceItemNode::setMarkerWeight(float weight)
{
    if (!qFuzzyCompare(m_markerWeight, weight)) {
        m_markerWeight = weight;
        Q_EMIT markerStyleChanged();
    }
}

QColor QImPlot3DSurfaceItemNode::fillColor() const
{
    return m_fillColor;
}

void QImPlot3DSurfaceItemNode::setFillColor(const QColor& color)
{
    if (m_fillColor != color) {
        m_fillColor = color;
        Q_EMIT fillColorChanged(color);
    }
}

QColor QImPlot3DSurfaceItemNode::lineColor() const
{
    return m_lineColor;
}

void QImPlot3DSurfaceItemNode::setLineColor(const QColor& color)
{
    if (m_lineColor != color) {
        m_lineColor = color;
        Q_EMIT lineColorChanged(color);
    }
}

QColor QImPlot3DSurfaceItemNode::markerFillColor() const
{
    return m_markerFillColor;
}

void QImPlot3DSurfaceItemNode::setMarkerFillColor(const QColor& color)
{
    if (m_markerFillColor != color) {
        m_markerFillColor = color;
        Q_EMIT markerFillColorChanged(color);
    }
}

QColor QImPlot3DSurfaceItemNode::markerOutlineColor() const
{
    return m_markerOutlineColor;
}

void QImPlot3DSurfaceItemNode::setMarkerOutlineColor(const QColor& color)
{
    if (m_markerOutlineColor != color) {
        m_markerOutlineColor = color;
        Q_EMIT markerOutlineColorChanged(color);
    }
}

float QImPlot3DSurfaceItemNode::lineWidth() const
{
    return m_lineWidth;
}

void QImPlot3DSurfaceItemNode::setLineWidth(float width)
{
    if (!qFuzzyCompare(m_lineWidth, width)) {
        m_lineWidth = width;
        Q_EMIT lineWidthChanged(width);
    }
}

bool QImPlot3DSurfaceItemNode::isColormapEnabled() const
{
    return m_colormapEnabled;
}

void QImPlot3DSurfaceItemNode::setColormapEnabled(bool enabled)
{
    if (m_colormapEnabled != enabled) {
        m_colormapEnabled = enabled;
        Q_EMIT colormapChanged();
    }
}

int QImPlot3DSurfaceItemNode::colormap() const
{
    return m_colormap;
}

void QImPlot3DSurfaceItemNode::setColormap(int colormap)
{
    if (m_colormap != colormap) {
        m_colormap = colormap;
        Q_EMIT colormapChanged();
    }
}

int QImPlot3DSurfaceItemNode::surfaceFlags() const
{
    return m_surfaceFlags;
}

void QImPlot3DSurfaceItemNode::setSurfaceFlags(int flags)
{
    if (m_surfaceFlags != flags) {
        m_surfaceFlags = flags;
        Q_EMIT surfaceFlagChanged();
    }
}

bool QImPlot3DSurfaceItemNode::beginDraw()
{
    const int expectedCount = m_xCount * m_yCount;
    if (m_xCount < 2 || m_yCount < 2 || expectedCount <= 0) {
        return false;
    }
    if (static_cast< int >(std::min({ m_xData.size(), m_yData.size(), m_zData.size() })) < expectedCount) {
        return false;
    }

    const bool useColormap = m_colormapEnabled && !m_fillColor.isValid();
    if (useColormap) {
        ImPlot3D::PushColormap(static_cast< ImPlot3DColormap >(m_colormap));
    }
    if (m_fillColor.isValid()) {
        ImPlot3D::SetNextFillStyle(toImVec4(m_fillColor));
    }
    if (m_lineColor.isValid()) {
        ImPlot3D::SetNextLineStyle(toImVec4(m_lineColor), m_lineWidth);
    } else {
        ImPlot3D::SetNextLineStyle(IMPLOT3D_AUTO_COL, m_lineWidth);
    }
    if (m_markerShape != ImPlot3DMarker_None) {
        const ImVec4 fill = m_markerFillColor.isValid() ? toImVec4(m_markerFillColor) : IMPLOT3D_AUTO_COL;
        const ImVec4 outline = m_markerOutlineColor.isValid() ? toImVec4(m_markerOutlineColor) : IMPLOT3D_AUTO_COL;
        ImPlot3D::SetNextMarkerStyle(static_cast< ImPlot3DMarker >(m_markerShape), m_markerSize, fill, m_markerWeight, outline);
    }

    ImPlot3D::PlotSurface(
        labelConstData(),
        m_xData.data(),
        m_yData.data(),
        m_zData.data(),
        m_xCount,
        m_yCount,
        0.0,
        0.0,
        static_cast< ImPlot3DSurfaceFlags >(m_surfaceFlags)
    );
    if (useColormap) {
        ImPlot3D::PopColormap();
    }
    return false;
}

void QImPlot3DSurfaceItemNode::trimDataToGrid()
{
    const std::size_t commonSize = std::min({ m_xData.size(), m_yData.size(), m_zData.size() });
    const std::size_t gridSize = (m_xCount > 0 && m_yCount > 0) ? static_cast< std::size_t >(m_xCount) * static_cast< std::size_t >(m_yCount)
                                                                : commonSize;
    const std::size_t finalSize = std::min(commonSize, gridSize);
    m_xData.resize(finalSize);
    m_yData.resize(finalSize);
    m_zData.resize(finalSize);
}
}  // namespace QIM
