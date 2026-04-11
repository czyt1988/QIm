#include "QImPlot3DNode.h"
#include "QImPlot3DItemNode.h"
#include "implot3d.h"

namespace QIM
{
namespace
{
ImAxis3D toImAxis3D(QImPlot3DNode::Axis axis)
{
    switch (axis) {
    case QImPlot3DNode::AxisX:
        return ImAxis3D_X;
    case QImPlot3DNode::AxisY:
        return ImAxis3D_Y;
    case QImPlot3DNode::AxisZ:
    default:
        return ImAxis3D_Z;
    }
}
}  // namespace

QImPlot3DNode::QImPlot3DNode(QObject* parent) : QImAbstractNode(parent)
{
}

QImPlot3DNode::QImPlot3DNode(const QString& title, QObject* parent) : QImAbstractNode(parent), m_title(title)
{
}

QImPlot3DNode::~QImPlot3DNode()
{
}

QString QImPlot3DNode::title() const
{
    return m_title;
}

void QImPlot3DNode::setTitle(const QString& title)
{
    if (m_title != title) {
        m_title = title;
        Q_EMIT titleChanged(title);
    }
}

QSizeF QImPlot3DNode::size() const
{
    return m_size;
}

void QImPlot3DNode::setSize(const QSizeF& size)
{
    if (m_size != size) {
        m_size = size;
        Q_EMIT sizeChanged(size);
    }
}

bool QImPlot3DNode::isAutoSize() const
{
    return m_autoSize;
}

void QImPlot3DNode::setAutoSize(bool autoSize)
{
    if (m_autoSize != autoSize) {
        m_autoSize = autoSize;
        Q_EMIT autoSizeChanged(autoSize);
    }
}

QString QImPlot3DNode::xAxisLabel() const
{
    return m_axisLabels[ AxisX ];
}

QString QImPlot3DNode::yAxisLabel() const
{
    return m_axisLabels[ AxisY ];
}

QString QImPlot3DNode::zAxisLabel() const
{
    return m_axisLabels[ AxisZ ];
}

void QImPlot3DNode::setXAxisLabel(const QString& label)
{
    setAxisLabel(AxisX, label);
}

void QImPlot3DNode::setYAxisLabel(const QString& label)
{
    setAxisLabel(AxisY, label);
}

void QImPlot3DNode::setZAxisLabel(const QString& label)
{
    setAxisLabel(AxisZ, label);
}

void QImPlot3DNode::setAxisLabel(Axis axis, const QString& label)
{
    if (m_axisLabels[ axis ] != label) {
        m_axisLabels[ axis ] = label;
        Q_EMIT axisLabelChanged();
    }
}

bool QImPlot3DNode::isLegendEnabled() const
{
    return (m_plotFlags & ImPlot3DFlags_NoLegend) == 0;
}

void QImPlot3DNode::setLegendEnabled(bool enabled)
{
    const int oldFlags = m_plotFlags;
    if (enabled) {
        m_plotFlags &= ~ImPlot3DFlags_NoLegend;
    } else {
        m_plotFlags |= ImPlot3DFlags_NoLegend;
    }
    if (m_plotFlags != oldFlags) {
        Q_EMIT plotFlagChanged();
    }
}

bool QImPlot3DNode::isMouseTextEnabled() const
{
    return (m_plotFlags & ImPlot3DFlags_NoMouseText) == 0;
}

void QImPlot3DNode::setMouseTextEnabled(bool enabled)
{
    const int oldFlags = m_plotFlags;
    if (enabled) {
        m_plotFlags &= ~ImPlot3DFlags_NoMouseText;
    } else {
        m_plotFlags |= ImPlot3DFlags_NoMouseText;
    }
    if (m_plotFlags != oldFlags) {
        Q_EMIT plotFlagChanged();
    }
}

bool QImPlot3DNode::isInputsEnabled() const
{
    return (m_plotFlags & ImPlot3DFlags_NoInputs) == 0;
}

void QImPlot3DNode::setInputsEnabled(bool enabled)
{
    const int oldFlags = m_plotFlags;
    if (enabled) {
        m_plotFlags &= ~ImPlot3DFlags_NoInputs;
    } else {
        m_plotFlags |= ImPlot3DFlags_NoInputs;
    }
    if (m_plotFlags != oldFlags) {
        Q_EMIT plotFlagChanged();
    }
}

bool QImPlot3DNode::isMenusEnabled() const
{
    return (m_plotFlags & ImPlot3DFlags_NoMenus) == 0;
}

void QImPlot3DNode::setMenusEnabled(bool enabled)
{
    const int oldFlags = m_plotFlags;
    if (enabled) {
        m_plotFlags &= ~ImPlot3DFlags_NoMenus;
    } else {
        m_plotFlags |= ImPlot3DFlags_NoMenus;
    }
    if (m_plotFlags != oldFlags) {
        Q_EMIT plotFlagChanged();
    }
}

bool QImPlot3DNode::isClippingEnabled() const
{
    return (m_plotFlags & ImPlot3DFlags_NoClip) == 0;
}

void QImPlot3DNode::setClippingEnabled(bool enabled)
{
    const int oldFlags = m_plotFlags;
    if (enabled) {
        m_plotFlags &= ~ImPlot3DFlags_NoClip;
    } else {
        m_plotFlags |= ImPlot3DFlags_NoClip;
    }
    if (m_plotFlags != oldFlags) {
        Q_EMIT plotFlagChanged();
    }
}

bool QImPlot3DNode::isEqual() const
{
    return (m_plotFlags & ImPlot3DFlags_Equal) != 0;
}

void QImPlot3DNode::setEqual(bool enabled)
{
    const int oldFlags = m_plotFlags;
    if (enabled) {
        m_plotFlags |= ImPlot3DFlags_Equal;
    } else {
        m_plotFlags &= ~ImPlot3DFlags_Equal;
    }
    if (m_plotFlags != oldFlags) {
        Q_EMIT plotFlagChanged();
    }
}

int QImPlot3DNode::imPlot3DFlags() const
{
    return m_plotFlags;
}

void QImPlot3DNode::setImPlot3DFlags(int flags)
{
    if (m_plotFlags != flags) {
        m_plotFlags = flags;
        Q_EMIT plotFlagChanged();
    }
}

void QImPlot3DNode::setAxisLimits(Axis axis, double minValue, double maxValue, bool always)
{
    AxisLimits& limits = m_axisLimits[ axis ];
    limits.enabled     = true;
    limits.always      = always;
    limits.minValue    = minValue;
    limits.maxValue    = maxValue;
}

void QImPlot3DNode::clearAxisLimits(Axis axis)
{
    m_axisLimits[ axis ].enabled = false;
}

void QImPlot3DNode::addPlotItem(QImPlot3DItemNode* item)
{
    addChildNode(item);
}

QList< QImPlot3DItemNode* > QImPlot3DNode::plotItemNodes() const
{
    return findChildrenNodes< QImPlot3DItemNode* >();
}

bool QImPlot3DNode::beginDraw()
{
    const QByteArray utf8Title = m_title.toUtf8();
    const char* titleData      = utf8Title.isEmpty() ? "##plot3d" : utf8Title.constData();
    m_beginPlotSuccess         = ImPlot3D::BeginPlot(titleData, imSize(), static_cast< ImPlot3DFlags >(m_plotFlags));
    if (m_beginPlotSuccess) {
        applySetup();
    }
    return m_beginPlotSuccess;
}

void QImPlot3DNode::endDraw()
{
    if (m_beginPlotSuccess) {
        ImPlot3D::EndPlot();
        m_beginPlotSuccess = false;
    }
}

ImVec2 QImPlot3DNode::imSize() const
{
    if (m_autoSize || !m_size.isValid()) {
        return ImVec2(-1.0f, -1.0f);
    }
    return ImVec2(static_cast< float >(m_size.width()), static_cast< float >(m_size.height()));
}

void QImPlot3DNode::applySetup() const
{
    const QByteArray xLabel = m_axisLabels[ AxisX ].toUtf8();
    const QByteArray yLabel = m_axisLabels[ AxisY ].toUtf8();
    const QByteArray zLabel = m_axisLabels[ AxisZ ].toUtf8();

    ImPlot3D::SetupAxes(
        xLabel.isEmpty() ? nullptr : xLabel.constData(),
        yLabel.isEmpty() ? nullptr : yLabel.constData(),
        zLabel.isEmpty() ? nullptr : zLabel.constData()
    );

    for (int i = AxisX; i <= AxisZ; ++i) {
        const AxisLimits& limits = m_axisLimits[ i ];
        if (!limits.enabled) {
            continue;
        }
        ImPlot3D::SetupAxisLimits(
            toImAxis3D(static_cast< Axis >(i)),
            limits.minValue,
            limits.maxValue,
            limits.always ? ImPlot3DCond_Always : ImPlot3DCond_Once
        );
    }
}
}  // namespace QIM
