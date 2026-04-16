#include "QImPlot3DScatterItemNode.h"
#include "QtImGuiUtils.h"
#include "implot3d.h"
#include <algorithm>

namespace QIM
{
QImPlot3DScatterItemNode::QImPlot3DScatterItemNode(QObject* parent) : QImPlot3DItemNode(parent)
{
}

QImPlot3DScatterItemNode::~QImPlot3DScatterItemNode()
{
}

const std::vector< double >& QImPlot3DScatterItemNode::xData() const
{
    return m_xData;
}

const std::vector< double >& QImPlot3DScatterItemNode::yData() const
{
    return m_yData;
}

const std::vector< double >& QImPlot3DScatterItemNode::zData() const
{
    return m_zData;
}

int QImPlot3DScatterItemNode::markerShape() const
{
    return m_markerShape;
}

void QImPlot3DScatterItemNode::setMarkerShape(int shape)
{
    if (m_markerShape != shape) {
        m_markerShape = shape;
        Q_EMIT markerShapeChanged(shape);
    }
}

float QImPlot3DScatterItemNode::markerSize() const
{
    return m_markerSize;
}

void QImPlot3DScatterItemNode::setMarkerSize(float size)
{
    if (!qFuzzyCompare(m_markerSize, size)) {
        m_markerSize = size;
        Q_EMIT markerSizeChanged(size);
    }
}

float QImPlot3DScatterItemNode::markerWeight() const
{
    return m_markerWeight;
}

void QImPlot3DScatterItemNode::setMarkerWeight(float weight)
{
    if (!qFuzzyCompare(m_markerWeight, weight)) {
        m_markerWeight = weight;
        Q_EMIT markerWeightChanged(weight);
    }
}

QColor QImPlot3DScatterItemNode::fillColor() const
{
    return m_fillColor;
}

void QImPlot3DScatterItemNode::setFillColor(const QColor& color)
{
    if (m_fillColor != color) {
        m_fillColor = color;
        Q_EMIT fillColorChanged(color);
    }
}

QColor QImPlot3DScatterItemNode::outlineColor() const
{
    return m_outlineColor;
}

void QImPlot3DScatterItemNode::setOutlineColor(const QColor& color)
{
    if (m_outlineColor != color) {
        m_outlineColor = color;
        Q_EMIT outlineColorChanged(color);
    }
}

bool QImPlot3DScatterItemNode::beginDraw()
{
    const int count = static_cast< int >(std::min({ m_xData.size(), m_yData.size(), m_zData.size() }));
    if (count <= 0) {
        return false;
    }

    const ImVec4 fill = m_fillColor.isValid() ? toImVec4(m_fillColor) : IMPLOT3D_AUTO_COL;
    const ImVec4 outline = m_outlineColor.isValid() ? toImVec4(m_outlineColor) : IMPLOT3D_AUTO_COL;
    ImPlot3D::SetNextMarkerStyle(
        static_cast< ImPlot3DMarker >(m_markerShape),
        m_markerSize,
        fill,
        m_markerWeight,
        outline
    );

    ImPlot3D::PlotScatter(
        labelConstData(),
        m_xData.data(),
        m_yData.data(),
        m_zData.data(),
        count
    );
    return false;
}

void QImPlot3DScatterItemNode::trimDataToCommonSize()
{
    const std::size_t commonSize = std::min({ m_xData.size(), m_yData.size(), m_zData.size() });
    m_xData.resize(commonSize);
    m_yData.resize(commonSize);
    m_zData.resize(commonSize);
}
}  // namespace QIM
