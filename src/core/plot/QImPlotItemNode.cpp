#include "QImPlotItemNode.h"
#include "implot.h"
#include "implot_internal.h"
#include "QtImGuiUtils.h"
#include "QImPlotNode.h"

namespace QIM
{

class QImPlotItemNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlotItemNode)
public:
    PrivateData(QImPlotItemNode* p);

public:
    QByteArray utf8Label;
    ImAxis xAxisId { ImAxis_X1 };
    ImAxis yAxisId { ImAxis_Y1 };
    ImPlotItem* plotItem { nullptr };
    bool userVisible { true };  ///< 用户设置的可见性状态（首次渲染前有效）
};

QImPlotItemNode::PrivateData::PrivateData(QImPlotItemNode* p) : q_ptr(p)
{
}
//----------------------------------------------------
// QImPlotItemNode
//----------------------------------------------------
QImPlotItemNode::QImPlotItemNode(QObject* par) : QImAbstractNode(par), QIM_PIMPL_CONSTRUCT
{
    setAutoIdEnabled(false);
    setRenderOption(RenderIgnoreVisible, true);  // line的visible由ImPlotItem::Show决定，这样才可以和legend联动
}

QImPlotItemNode::~QImPlotItemNode()
{
}

void QImPlotItemNode::setLabel(const QString& name)
{
    QIM_D(d);
    QByteArray nameUtf = name.toUtf8();
    if (nameUtf != d->utf8Label) {
        d->utf8Label = nameUtf;
        Q_EMIT labelChanged(name);
    }
}

QString QImPlotItemNode::label() const
{
    return QString::fromUtf8(d_ptr->utf8Label);
}

const char* QImPlotItemNode::labelConstData() const
{
    return (d_ptr->utf8Label.isEmpty() ? "##plotItem" : d_ptr->utf8Label.constData());
}

void QImPlotItemNode::bindAxis(QImPlotAxisId x, QImPlotAxisId y)
{
    if (isXAxisId(x) && isYAxisId(y)) {
        d_ptr->xAxisId = toImAxis(x);
        d_ptr->yAxisId = toImAxis(y);
    }
}

QPointF QImPlotItemNode::pixelsToPlot(const float& screenX, const float& screenY)
{
    return toQPointF(ImPlot::PixelsToPlot(screenX, screenY, d_ptr->xAxisId, d_ptr->yAxisId));
}

QPointF QImPlotItemNode::plotToPixels(const double& doubleX, const double& doubleY)
{
    return toQPointF(ImPlot::PlotToPixels(doubleX, doubleY, d_ptr->xAxisId, d_ptr->yAxisId));
}

QImPlotAxisId QImPlotItemNode::xAxisId() const
{
    return toQImPlotAxisId(d_ptr->xAxisId);
}

QImPlotAxisId QImPlotItemNode::yAxisId() const
{
    return toQImPlotAxisId(d_ptr->yAxisId);
}

/**
 * @brief 获取绘图节点
 *
 * 此函数会一直寻找第一个为QImPlotNode的父节点
 * @return
 */
QImPlotNode* QImPlotItemNode::plotNode() const
{
    QImAbstractNode* parent = parentNode();

    while (parent != nullptr) {
        if (auto* plotNode = qobject_cast< QImPlotNode* >(parent)) {
            return plotNode;
        }
        parent = parent->parentNode();
    }

    return nullptr;
}

QColor QImPlotItemNode::itemColor() const
{
    QIM_DC(d);
    if (d->plotItem) {
        return toQColor(d->plotItem->Color);
    }
    return QColor();
}

bool QImPlotItemNode::isLegendHovered() const
{
    QIM_DC(d);
    if (d->plotItem) {
        return d->plotItem->LegendHovered;
    }
    return false;
}

bool QImPlotItemNode::isVisible() const
{
    ImPlotItem* plotItem = d_ptr->plotItem;
    if (plotItem) {
        // 渲染后返回 ImPlot 的实际状态
        return plotItem->Show;
    }
    return false;
}

void QImPlotItemNode::setVisible(bool visible)
{
    ImPlotItem* plotItem = d_ptr->plotItem;
    if (plotItem) {
        // 渲染后同步到 ImPlotItem
        plotItem->Show = visible;
        // 此函数同步根节点的可见性状态，同时会触发信号
        QImAbstractNode::setVisible(visible);
    }
}

void QImPlotItemNode::endDraw()
{
}

ImPlotItem* QImPlotItemNode::imPlotItem() const
{
    return d_ptr->plotItem;
}

/**
 * @brief 记录绘图节点
 *
 * 记录的过程会把当前节点的可见性状态同步到绘图节点
 * @param item
 */
void QImPlotItemNode::setImPlotItem(ImPlotItem* item)
{
    if (d_ptr->plotItem == item) {
        return;
    }
    d_ptr->plotItem = item;
}

}  // end namespace QIM
