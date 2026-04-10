#include "QImPlot3DLineItemNode.h"
#include "QtImGuiUtils.h"
#include "implot3d.h"
#include <algorithm>

namespace QIM
{
QImPlot3DLineItemNode::QImPlot3DLineItemNode(QObject* parent) : QImPlot3DItemNode(parent)
{
}

QImPlot3DLineItemNode::~QImPlot3DLineItemNode()
{
}

const std::vector< double >& QImPlot3DLineItemNode::xData() const
{
    return m_xData;
}

const std::vector< double >& QImPlot3DLineItemNode::yData() const
{
    return m_yData;
}

const std::vector< double >& QImPlot3DLineItemNode::zData() const
{
    return m_zData;
}

bool QImPlot3DLineItemNode::isSegments() const
{
    return (m_lineFlags & ImPlot3DLineFlags_Segments) != 0;
}

void QImPlot3DLineItemNode::setSegments(bool enabled)
{
    const int oldFlags = m_lineFlags;
    if (enabled) {
        m_lineFlags |= ImPlot3DLineFlags_Segments;
    } else {
        m_lineFlags &= ~ImPlot3DLineFlags_Segments;
    }
    if (m_lineFlags != oldFlags) {
        Q_EMIT lineFlagChanged();
    }
}

bool QImPlot3DLineItemNode::isLoop() const
{
    return (m_lineFlags & ImPlot3DLineFlags_Loop) != 0;
}

void QImPlot3DLineItemNode::setLoop(bool enabled)
{
    const int oldFlags = m_lineFlags;
    if (enabled) {
        m_lineFlags |= ImPlot3DLineFlags_Loop;
    } else {
        m_lineFlags &= ~ImPlot3DLineFlags_Loop;
    }
    if (m_lineFlags != oldFlags) {
        Q_EMIT lineFlagChanged();
    }
}

bool QImPlot3DLineItemNode::isSkipNaN() const
{
    return (m_lineFlags & ImPlot3DLineFlags_SkipNaN) != 0;
}

void QImPlot3DLineItemNode::setSkipNaN(bool enabled)
{
    const int oldFlags = m_lineFlags;
    if (enabled) {
        m_lineFlags |= ImPlot3DLineFlags_SkipNaN;
    } else {
        m_lineFlags &= ~ImPlot3DLineFlags_SkipNaN;
    }
    if (m_lineFlags != oldFlags) {
        Q_EMIT lineFlagChanged();
    }
}

int QImPlot3DLineItemNode::lineFlags() const
{
    return m_lineFlags;
}

void QImPlot3DLineItemNode::setLineFlags(int flags)
{
    if (m_lineFlags != flags) {
        m_lineFlags = flags;
        Q_EMIT lineFlagChanged();
    }
}

void QImPlot3DLineItemNode::setColor(const QColor& color)
{
    if (m_color != color) {
        m_color = color;
        Q_EMIT colorChanged(color);
    }
}

QColor QImPlot3DLineItemNode::color() const
{
    return m_color;
}

void QImPlot3DLineItemNode::setLineWidth(float width)
{
    if (!qFuzzyCompare(m_lineWidth, width)) {
        m_lineWidth = width;
        Q_EMIT lineWidthChanged(width);
    }
}

float QImPlot3DLineItemNode::lineWidth() const
{
    return m_lineWidth;
}

bool QImPlot3DLineItemNode::beginDraw()
{
    const int count = static_cast< int >(std::min({ m_xData.size(), m_yData.size(), m_zData.size() }));
    if (count <= 0) {
        return false;
    }

    if (m_color.isValid()) {
        ImPlot3D::SetNextLineStyle(toImVec4(m_color), m_lineWidth);
    } else {
        ImPlot3D::SetNextLineStyle(IMPLOT3D_AUTO_COL, m_lineWidth);
    }

    ImPlot3D::PlotLine(
        labelConstData(),
        m_xData.data(),
        m_yData.data(),
        m_zData.data(),
        count,
        static_cast< ImPlot3DLineFlags >(m_lineFlags)
    );
    return false;
}

void QImPlot3DLineItemNode::trimDataToCommonSize()
{
    const std::size_t commonSize = std::min({ m_xData.size(), m_yData.size(), m_zData.size() });
    m_xData.resize(commonSize);
    m_yData.resize(commonSize);
    m_zData.resize(commonSize);
}
}  // namespace QIM
