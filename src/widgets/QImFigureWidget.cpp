#include "QImFigureWidget.h"
#include <QDebug>
#include "QImTrackedValue.hpp"
#include "implot.h"
#include "implot3d.h"
#include "plot/QImSubplotsNode.h"
#include "plot/QImPlotNode.h"
namespace QIM
{
class QImFigureWidget::PrivateData
{
    QIM_DECLARE_PUBLIC(QImFigureWidget)
public:
    PrivateData(QImFigureWidget* p);

public:
    QImTrackedValue< QImPlotTheme > m_theme;
    QPointer< QImSubplotsNode > m_subplotNode;
    ImPlotContext* m_context { nullptr };
};

QImFigureWidget::PrivateData::PrivateData(QImFigureWidget* p) : q_ptr(p)
{
}

//----------------------------------------------------
// QImFigureWidget
//----------------------------------------------------
QImFigureWidget::QImFigureWidget(QWidget* parent, Qt::WindowFlags f) : QImWidget(parent, f), QIM_PIMPL_CONSTRUCT
{
    // Subplot is no longer created by default. It will be created on demand
    // when setSubplotGrid() is called with rows*cols > 1, or via ensureSubplotNode().
    // Single-plot mode renders QImPlotNode directly at root level.
}

QImFigureWidget::~QImFigureWidget()
{
    ImPlot3D::DestroyContext();
    ImPlot::DestroyContext(d_ptr->m_context);
}

void QImFigureWidget::setPlotTheme(const QImPlotTheme& t)
{
    d_ptr->m_theme = t;
}

const QImPlotTheme& QImFigureWidget::plotTheme() const
{
    return d_ptr->m_theme.value();
}

QImSubplotsNode* QImFigureWidget::ensureSubplotNode()
{
    QIM_D(d);
    if (!d->m_subplotNode) {
        d->m_subplotNode = new QImSubplotsNode();
        addRenderNode(d->m_subplotNode.data());
        d->m_subplotNode->setTitleEnabled(true);
        connect(d->m_subplotNode, &QImSubplotsNode::childNodeAdded, this, &QImFigureWidget::onSubplotChildNodeAdded);
        connect(d->m_subplotNode, &QImSubplotsNode::childNodeRemoved, this, &QImFigureWidget::onSubplotChildNodeRemoved);
    }
    return d->m_subplotNode.data();
}

void QImFigureWidget::setSubplotGrid(int rows, int cols, const std::vector< float >& rowsRatios, const std::vector< float >& colsRatios)
{
    QIM_D(d);
    // If no subplot exists yet and grid is 1x1, don't create one —
    // single-plot mode uses root-level QImPlotNode directly.
    // Only create/use subplot for multi-plot grids or if subplot already exists.
    if (rows * cols > 1 || d->m_subplotNode) {
        QImSubplotsNode* subplot = ensureSubplotNode();
        subplot->setGrid(rows, cols, rowsRatios, colsRatios);
    }
}

int QImFigureWidget::subplotGridRows() const
{
    QIM_DC(d);
    if (d->m_subplotNode) {
        return d->m_subplotNode->rows();
    }
    return -1;
}

int QImFigureWidget::subplotGridColumns() const
{
    QIM_DC(d);
    if (d->m_subplotNode) {
        return d->m_subplotNode->columns();
    }
    return -1;
}

std::vector< float > QImFigureWidget::subplotGridRowRatios() const
{
    QIM_DC(d);
    if (d->m_subplotNode) {
        return d->m_subplotNode->rowRatios();
    }
    return {};
}

std::vector< float > QImFigureWidget::subplotGridColumnRatios() const
{
    QIM_DC(d);
    if (d->m_subplotNode) {
        return d->m_subplotNode->columnRatios();
    }
    return {};
}

void QImFigureWidget::clearSubplotGrid()
{
    QIM_D(d);
    if (d->m_subplotNode) {
        removeRenderNode(d->m_subplotNode.data());
        d->m_subplotNode = nullptr;
    }
}

QImSubplotsNode* QImFigureWidget::subplotNode() const
{
    return d_ptr->m_subplotNode.data();
}

/**
 * @brief Create a plot node, placed intelligently based on current mode
 *
 * If no subplot exists, creates QImPlotNode as a root-level render node (fills window).
 * If subplot exists, delegates to QImSubplotsNode::createPlotNode() (plot occupies a grid cell).
 *
 * @return New QImPlotNode, or nullptr if subplot grid is full
 */
QImPlotNode* QImFigureWidget::createPlotNode()
{
    QIM_D(d);
    if (d->m_subplotNode) {
        // Subplot exists — delegate to it (plot occupies a grid cell)
        return d->m_subplotNode->createPlotNode();
    }
    // No subplot — create plot as root-level render node (fills entire window)
    QImPlotNode* plot = new QImPlotNode();
    addRenderNode(plot);
    return plot;
}

/**
 * @brief 获取所有绘图节点
 * @return
 */
QList< QImPlotNode* > QImFigureWidget::plotNodes() const
{
    QIM_DC(d);
    if (d->m_subplotNode) {
        return d->m_subplotNode->plotNodes();
    }
    // No subplot — search root render node children for QImPlotNode*
    QList< QImPlotNode* > result;
    const auto children = renderNodeList();
    for (QImAbstractNode* node : children) {
        if (QImPlotNode* plot = qobject_cast< QImPlotNode* >(node)) {
            result.append(plot);
        }
    }
    return result;
}

int QImFigureWidget::plotCount() const
{
    return plotNodes().size();
}

void QImFigureWidget::addPlotNode(QImPlotNode* plot)
{
    QIM_D(d);
    if (d->m_subplotNode) {
        d->m_subplotNode->addPlotNode(plot);
    } else {
        addRenderNode(plot);
    }
}

void QImFigureWidget::insertPlotNode(int plotIndex, QImPlotNode* plot)
{
    QIM_D(d);
    if (d->m_subplotNode) {
        d->m_subplotNode->insertPlotNode(plotIndex, plot);
    } else {
        addRenderNode(plot);
    }
}

int QImFigureWidget::plotNodeSubplotIndex(QImPlotNode* plot)
{
    QIM_D(d);
    if (d->m_subplotNode) {
        return d->m_subplotNode->plotNodeSubplotIndex(plot);
    }
    return plotNodes().indexOf(plot);
}

bool QImFigureWidget::takePlotNode(QImPlotNode* plot)
{
    QIM_D(d);
    if (d->m_subplotNode) {
        return d->m_subplotNode->takeChildNode(plot);
    }
    return takeRenderNode(plot);
}

void QImFigureWidget::removePlotNode(QImPlotNode* plot)
{
    QIM_D(d);
    if (d->m_subplotNode) {
        d->m_subplotNode->removeChildNode(plot);
    } else {
        removeRenderNode(plot);
    }
}

void QImFigureWidget::initializeGL()
{
    QIM_D(d);
    QIM::QImWidget::initializeGL();
    d->m_context = ImPlot::CreateContext();
    ImPlot3D::CreateContext();
    // 默认有个subplot
}

void QImFigureWidget::onSubplotChildNodeRemoved(QImAbstractNode* c)
{
    QImPlotNode* plot = qobject_cast< QImPlotNode* >(c);
    if (plot) {
        Q_EMIT plotNodeAttached(plot, false);
    }
}

void QImFigureWidget::onSubplotChildNodeAdded(QImAbstractNode* c)
{
    QImPlotNode* plot = qobject_cast< QImPlotNode* >(c);
    if (plot) {
        Q_EMIT plotNodeAttached(plot, true);
    }
}

}  // end namespace QIM
