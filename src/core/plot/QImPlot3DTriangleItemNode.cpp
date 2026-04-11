#include "QImPlot3DTriangleItemNode.h"
#include "QtImGuiUtils.h"
#include <algorithm>

namespace QIM
{
QImPlot3DTriangleItemNode::QImPlot3DTriangleItemNode(QObject* parent) : QImPlot3DItemNode(parent)
{
}

QImPlot3DTriangleItemNode::~QImPlot3DTriangleItemNode()
{
}

const std::vector< double >& QImPlot3DTriangleItemNode::xData() const
{
    return m_xData;
}

const std::vector< double >& QImPlot3DTriangleItemNode::yData() const
{
    return m_yData;
}

const std::vector< double >& QImPlot3DTriangleItemNode::zData() const
{
    return m_zData;
}

bool QImPlot3DTriangleItemNode::isLinesVisible() const
{
    return (m_triangleFlags & ImPlot3DTriangleFlags_NoLines) == 0;
}

void QImPlot3DTriangleItemNode::setLinesVisible(bool visible)
{
    const int oldFlags = m_triangleFlags;
    if (visible) {
        m_triangleFlags &= ~ImPlot3DTriangleFlags_NoLines;
    } else {
        m_triangleFlags |= ImPlot3DTriangleFlags_NoLines;
    }
    if (m_triangleFlags != oldFlags) {
        Q_EMIT triangleFlagChanged();
    }
}

bool QImPlot3DTriangleItemNode::isFillVisible() const
{
    return (m_triangleFlags & ImPlot3DTriangleFlags_NoFill) == 0;
}

void QImPlot3DTriangleItemNode::setFillVisible(bool visible)
{
    const int oldFlags = m_triangleFlags;
    if (visible) {
        m_triangleFlags &= ~ImPlot3DTriangleFlags_NoFill;
    } else {
        m_triangleFlags |= ImPlot3DTriangleFlags_NoFill;
    }
    if (m_triangleFlags != oldFlags) {
        Q_EMIT triangleFlagChanged();
    }
}

bool QImPlot3DTriangleItemNode::isMarkersVisible() const
{
    return (m_triangleFlags & ImPlot3DTriangleFlags_NoMarkers) == 0;
}

void QImPlot3DTriangleItemNode::setMarkersVisible(bool visible)
{
    const int oldFlags = m_triangleFlags;
    if (visible) {
        m_triangleFlags &= ~ImPlot3DTriangleFlags_NoMarkers;
    } else {
        m_triangleFlags |= ImPlot3DTriangleFlags_NoMarkers;
    }
    if (m_triangleFlags != oldFlags) {
        Q_EMIT triangleFlagChanged();
    }
}

int QImPlot3DTriangleItemNode::markerShape() const
{
    return m_markerShape;
}

void QImPlot3DTriangleItemNode::setMarkerShape(int shape)
{
    if (m_markerShape != shape) {
        m_markerShape = shape;
        Q_EMIT markerShapeChanged(shape);
    }
}

float QImPlot3DTriangleItemNode::markerSize() const
{
    return m_markerSize;
}

void QImPlot3DTriangleItemNode::setMarkerSize(float size)
{
    if (!qFuzzyCompare(m_markerSize, size)) {
        m_markerSize = size;
        Q_EMIT markerStyleChanged();
    }
}

float QImPlot3DTriangleItemNode::markerWeight() const
{
    return m_markerWeight;
}

void QImPlot3DTriangleItemNode::setMarkerWeight(float weight)
{
    if (!qFuzzyCompare(m_markerWeight, weight)) {
        m_markerWeight = weight;
        Q_EMIT markerStyleChanged();
    }
}

QColor QImPlot3DTriangleItemNode::fillColor() const
{
    return m_fillColor;
}

void QImPlot3DTriangleItemNode::setFillColor(const QColor& color)
{
    if (m_fillColor != color) {
        m_fillColor = color;
        Q_EMIT fillColorChanged(color);
    }
}

QColor QImPlot3DTriangleItemNode::lineColor() const
{
    return m_lineColor;
}

void QImPlot3DTriangleItemNode::setLineColor(const QColor& color)
{
    if (m_lineColor != color) {
        m_lineColor = color;
        Q_EMIT lineColorChanged(color);
    }
}

QColor QImPlot3DTriangleItemNode::markerFillColor() const
{
    return m_markerFillColor;
}

void QImPlot3DTriangleItemNode::setMarkerFillColor(const QColor& color)
{
    if (m_markerFillColor != color) {
        m_markerFillColor = color;
        Q_EMIT markerFillColorChanged(color);
    }
}

QColor QImPlot3DTriangleItemNode::markerOutlineColor() const
{
    return m_markerOutlineColor;
}

void QImPlot3DTriangleItemNode::setMarkerOutlineColor(const QColor& color)
{
    if (m_markerOutlineColor != color) {
        m_markerOutlineColor = color;
        Q_EMIT markerOutlineColorChanged(color);
    }
}

float QImPlot3DTriangleItemNode::lineWidth() const
{
    return m_lineWidth;
}

void QImPlot3DTriangleItemNode::setLineWidth(float width)
{
    if (!qFuzzyCompare(m_lineWidth, width)) {
        m_lineWidth = width;
        Q_EMIT lineWidthChanged(width);
    }
}

int QImPlot3DTriangleItemNode::triangleFlags() const
{
    return m_triangleFlags;
}

void QImPlot3DTriangleItemNode::setTriangleFlags(int flags)
{
    if (m_triangleFlags != flags) {
        m_triangleFlags = flags;
        Q_EMIT triangleFlagChanged();
    }
}

bool QImPlot3DTriangleItemNode::beginDraw()
{
    const int count = static_cast< int >(std::min({ m_xData.size(), m_yData.size(), m_zData.size() }));
    if (count < 3) {
        return false;
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

    ImPlot3D::PlotTriangle(
        labelConstData(),
        m_xData.data(),
        m_yData.data(),
        m_zData.data(),
        count,
        static_cast< ImPlot3DTriangleFlags >(m_triangleFlags)
    );
    return false;
}

void QImPlot3DTriangleItemNode::trimDataToCommonSize()
{
    const std::size_t commonSize = std::min({ m_xData.size(), m_yData.size(), m_zData.size() });
    m_xData.resize(commonSize);
    m_yData.resize(commonSize);
    m_zData.resize(commonSize);
}
}  // namespace QIM
