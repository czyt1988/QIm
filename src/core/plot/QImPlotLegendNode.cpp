#include "QImPlotLegendNode.h"
#include "implot.h"
#include "QtImGuiUtils.h"
#include <QDebug>

namespace QIM
{

class QImPlotLegendNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlotLegendNode)

public:
    PrivateData(QImPlotLegendNode* p);

    // 位置相关
    ImPlotLocation location { ImPlotLocation_NorthEast };
    // ImPlotLegendFlags（存储原始标志位）
    ImPlotLegendFlags flags { ImPlotLegendFlags_NoMenus };
};

QImPlotLegendNode::PrivateData::PrivateData(QImPlotLegendNode* p) : q_ptr(p)
{
}


// ----------------------------------------------------
// QImPlotLegendNode 实现
// ----------------------------------------------------
QImPlotLegendNode::QImPlotLegendNode(QObject* parent) : QImAbstractNode(parent), QIM_PIMPL_CONSTRUCT
{
    setObjectName(QStringLiteral("PlotLegendNode"));
    setAutoIdEnabled(false);
}

QImPlotLegendNode::~QImPlotLegendNode()
{
}

// ----------------------------------------------------
// 位置相关方法
// ----------------------------------------------------
QImPlotLegendLocation QImPlotLegendNode::location() const
{
    return toQImPlotLegendLocation(d_ptr->location);
}

void QImPlotLegendNode::setLocation(QImPlotLegendLocation v)
{
    QIM_D(d);
    ImPlotLocation iml = toImPlotLocation(v);
    if (d->location != iml) {
        d->location = iml;
        Q_EMIT loactionChanged(v);
    }
}

Qt::Alignment QImPlotLegendNode::alignment() const
{
    return toQAlignment(d_ptr->location);
}

void QImPlotLegendNode::setAlignment(Qt::Alignment a)
{
    QIM_D(d);
    ImPlotLocation iml = toImPlotLocation(a);
    if (d->location != iml) {
        d->location = iml;
        Q_EMIT loactionChanged(toQImPlotLegendLocation(iml));
    }
}

// ----------------------------------------------------
// 方向相关方法
// ----------------------------------------------------

void QImPlotLegendNode::setOrientation(Qt::Orientation vh)
{
    const bool isCurrentHorizontal = (d_ptr->flags & ImPlotLegendFlags_Horizontal) != 0;
    const bool isTargetHorizontal  = (vh == Qt::Horizontal);

    // 方向一致则直接返回
    if (isCurrentHorizontal == isTargetHorizontal) {
        return;
    }

    // 方向不一致时，更新标志位
    if (isTargetHorizontal) {
        d_ptr->flags |= ImPlotLegendFlags_Horizontal;  // 设置水平标志
    } else {
        d_ptr->flags &= (~ImPlotLegendFlags_Horizontal);  // 清除水平标志
    }
    Q_EMIT legendFlagChanged();
}

Qt::Orientation QImPlotLegendNode::orientation() const
{
    return (d_ptr->flags & ImPlotLegendFlags_Horizontal) ? Qt::Horizontal : Qt::Vertical;
}

// clang-format off

// ----------------------------------------------------
// 外观相关方法
// ----------------------------------------------------
QIMPLOT_FLAG_ACCESSOR(QImPlotLegendNode, Outside, ImPlotLegendFlags_Outside, legendFlagChanged)

// ImPlotLegendFlags_NoButtons
QIMPLOT_FLAG_ENABLED_ACCESSOR(QImPlotLegendNode, ButtonEnabled, ImPlotLegendFlags_NoButtons, legendFlagChanged)
// ImPlotLegendFlags_NoHighlightItem
QIMPLOT_FLAG_ENABLED_ACCESSOR(QImPlotLegendNode, HighlightItemEnabled, ImPlotLegendFlags_NoHighlightItem, legendFlagChanged)
// ImPlotLegendFlags_NoButtons
QIMPLOT_FLAG_ENABLED_ACCESSOR(QImPlotLegendNode, HighlightAxisEnabled, ImPlotLegendFlags_NoHighlightAxis, legendFlagChanged)
//ImPlotLegendFlags_Sort
QIMPLOT_FLAG_ACCESSOR(QImPlotLegendNode, Sort, ImPlotLegendFlags_Sort, legendFlagChanged)
//ImPlotLegendFlags_Reverse
QIMPLOT_FLAG_ACCESSOR(QImPlotLegendNode, Reverse, ImPlotLegendFlags_Reverse, legendFlagChanged)

// clang-format on
//===============================================================
// name
//===============================================================
int QImPlotLegendNode::legendFlags() const
{
    return d_ptr->flags;
}

void QImPlotLegendNode::setLegendFlags(int flags)
{
    QIM_D(d);
    if (d->flags != flags) {
        d->flags = static_cast< ImPlotLegendFlags >(flags);
        Q_EMIT legendFlagChanged();
    }
}


// ----------------------------------------------------
// 渲染方法
// ----------------------------------------------------
bool QImPlotLegendNode::beginDraw()
{
    // 在渲染时应用图例设置
    ImPlot::SetupLegend(d_ptr->location, d_ptr->flags);
    return false;  // 不需要特别的渲染上下文
}


}  // namespace QIM
