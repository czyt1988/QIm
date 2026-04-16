#include "QImPlot3DMeshItemNode.h"
#include "QtImGuiUtils.h"

namespace QIM
{
QImPlot3DMeshItemNode::QImPlot3DMeshItemNode(QObject* parent) : QImPlot3DItemNode(parent)
{
}

QImPlot3DMeshItemNode::~QImPlot3DMeshItemNode()
{
}

void QImPlot3DMeshItemNode::setMeshData(const std::vector< ImPlot3DPoint >& vertices, const std::vector< unsigned int >& indices)
{
    m_vertices = vertices;
    m_indices = indices;
    Q_EMIT dataChanged();
}

const std::vector< ImPlot3DPoint >& QImPlot3DMeshItemNode::vertices() const
{
    return m_vertices;
}

const std::vector< unsigned int >& QImPlot3DMeshItemNode::indices() const
{
    return m_indices;
}

bool QImPlot3DMeshItemNode::isLinesVisible() const
{
    return (m_meshFlags & ImPlot3DMeshFlags_NoLines) == 0;
}

void QImPlot3DMeshItemNode::setLinesVisible(bool visible)
{
    const int oldFlags = m_meshFlags;
    if (visible) {
        m_meshFlags &= ~ImPlot3DMeshFlags_NoLines;
    } else {
        m_meshFlags |= ImPlot3DMeshFlags_NoLines;
    }
    if (m_meshFlags != oldFlags) {
        Q_EMIT meshFlagChanged();
    }
}

bool QImPlot3DMeshItemNode::isFillVisible() const
{
    return (m_meshFlags & ImPlot3DMeshFlags_NoFill) == 0;
}

void QImPlot3DMeshItemNode::setFillVisible(bool visible)
{
    const int oldFlags = m_meshFlags;
    if (visible) {
        m_meshFlags &= ~ImPlot3DMeshFlags_NoFill;
    } else {
        m_meshFlags |= ImPlot3DMeshFlags_NoFill;
    }
    if (m_meshFlags != oldFlags) {
        Q_EMIT meshFlagChanged();
    }
}

bool QImPlot3DMeshItemNode::isMarkersVisible() const
{
    return (m_meshFlags & ImPlot3DMeshFlags_NoMarkers) == 0;
}

void QImPlot3DMeshItemNode::setMarkersVisible(bool visible)
{
    const int oldFlags = m_meshFlags;
    if (visible) {
        m_meshFlags &= ~ImPlot3DMeshFlags_NoMarkers;
    } else {
        m_meshFlags |= ImPlot3DMeshFlags_NoMarkers;
    }
    if (m_meshFlags != oldFlags) {
        Q_EMIT meshFlagChanged();
    }
}

int QImPlot3DMeshItemNode::markerShape() const
{
    return m_markerShape;
}

void QImPlot3DMeshItemNode::setMarkerShape(int shape)
{
    if (m_markerShape != shape) {
        m_markerShape = shape;
        Q_EMIT markerShapeChanged(shape);
    }
}

float QImPlot3DMeshItemNode::markerSize() const
{
    return m_markerSize;
}

void QImPlot3DMeshItemNode::setMarkerSize(float size)
{
    if (!qFuzzyCompare(m_markerSize, size)) {
        m_markerSize = size;
        Q_EMIT markerStyleChanged();
    }
}

float QImPlot3DMeshItemNode::markerWeight() const
{
    return m_markerWeight;
}

void QImPlot3DMeshItemNode::setMarkerWeight(float weight)
{
    if (!qFuzzyCompare(m_markerWeight, weight)) {
        m_markerWeight = weight;
        Q_EMIT markerStyleChanged();
    }
}

QColor QImPlot3DMeshItemNode::fillColor() const
{
    return m_fillColor;
}

void QImPlot3DMeshItemNode::setFillColor(const QColor& color)
{
    if (m_fillColor != color) {
        m_fillColor = color;
        Q_EMIT fillColorChanged(color);
    }
}

QColor QImPlot3DMeshItemNode::lineColor() const
{
    return m_lineColor;
}

void QImPlot3DMeshItemNode::setLineColor(const QColor& color)
{
    if (m_lineColor != color) {
        m_lineColor = color;
        Q_EMIT lineColorChanged(color);
    }
}

QColor QImPlot3DMeshItemNode::markerFillColor() const
{
    return m_markerFillColor;
}

void QImPlot3DMeshItemNode::setMarkerFillColor(const QColor& color)
{
    if (m_markerFillColor != color) {
        m_markerFillColor = color;
        Q_EMIT markerFillColorChanged(color);
    }
}

QColor QImPlot3DMeshItemNode::markerOutlineColor() const
{
    return m_markerOutlineColor;
}

void QImPlot3DMeshItemNode::setMarkerOutlineColor(const QColor& color)
{
    if (m_markerOutlineColor != color) {
        m_markerOutlineColor = color;
        Q_EMIT markerOutlineColorChanged(color);
    }
}

float QImPlot3DMeshItemNode::lineWidth() const
{
    return m_lineWidth;
}

void QImPlot3DMeshItemNode::setLineWidth(float width)
{
    if (!qFuzzyCompare(m_lineWidth, width)) {
        m_lineWidth = width;
        Q_EMIT lineWidthChanged(width);
    }
}

int QImPlot3DMeshItemNode::meshFlags() const
{
    return m_meshFlags;
}

void QImPlot3DMeshItemNode::setMeshFlags(int flags)
{
    if (m_meshFlags != flags) {
        m_meshFlags = flags;
        Q_EMIT meshFlagChanged();
    }
}

bool QImPlot3DMeshItemNode::beginDraw()
{
    if (m_vertices.empty() || m_indices.size() < 3 || (m_indices.size() % 3) != 0) {
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

    ImPlot3D::PlotMesh(
        labelConstData(),
        m_vertices.data(),
        m_indices.data(),
        static_cast< int >(m_vertices.size()),
        static_cast< int >(m_indices.size()),
        static_cast< ImPlot3DMeshFlags >(m_meshFlags)
    );
    return false;
}
}  // namespace QIM
