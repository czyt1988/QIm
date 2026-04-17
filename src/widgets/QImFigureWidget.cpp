#include "QImFigureWidget.h"
#include "QImTrackedValue.hpp"
#include "implot.h"
#include "implot3d.h"
#include "plot/QImSubplotsNode.h"
#include "plot3d/QImPlot3DExtNode.h"
#include "plot/QImPlotNode.h"
#include <QDebug>

namespace QIM
{
class QImFigureWidget::PrivateData
{
    QIM_DECLARE_PUBLIC(QImFigureWidget)
public:
    explicit PrivateData(QImFigureWidget* p) : q_ptr(p)
    {
    }

public:
    QImTrackedValue<QImPlotTheme> m_theme;
    QPointer<QImSubplotsNode> m_subplotNode;  // nullptr until setSubplotGrid() is called
    ImPlotContext* m_context2D { nullptr };
    ImPlot3DContext* m_context3D { nullptr };
};

QImFigureWidget::QImFigureWidget(QWidget* parent, Qt::WindowFlags f) : QImWidget(parent, f), QIM_PIMPL_CONSTRUCT
{
    // Subplot is no longer created by default. It will be created on demand
    // when setSubplotGrid() is called with rows*cols > 1, or via ensureSubplotNode().
    // Single-plot mode renders QImPlotNode directly at root level.
}

QImFigureWidget::~QImFigureWidget()
{
    if (d_ptr->m_context3D) {
        ImPlot3D::DestroyContext(d_ptr->m_context3D);
    }
    if (d_ptr->m_context2D) {
        ImPlot::DestroyContext(d_ptr->m_context2D);
    }
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

void QImFigureWidget::setSubplotGrid(int rows, int cols, const std::vector<float>& rowsRatios, const std::vector<float>& colsRatios)
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
    return d->m_subplotNode ? d->m_subplotNode->rows() : -1;
}

int QImFigureWidget::subplotGridColumns() const
{
    QIM_DC(d);
    return d->m_subplotNode ? d->m_subplotNode->columns() : -1;
}

std::vector<float> QImFigureWidget::subplotGridRowRatios() const
{
    QIM_DC(d);
    return d->m_subplotNode ? d->m_subplotNode->rowRatios() : std::vector<float>();
}

std::vector<float> QImFigureWidget::subplotGridColumnRatios() const
{
    QIM_DC(d);
    return d->m_subplotNode ? d->m_subplotNode->columnRatios() : std::vector<float>();
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
 * @brief Create a 2D plot node, placed intelligently based on current mode
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
        QImPlotNode* plot = d->m_subplotNode->createPlotNode();
        if (plot) {
            Q_EMIT plotNodeAttached(plot, true);
        }
        return plot;
    }
    // No subplot — create plot as root-level render node (fills entire window)
    QImPlotNode* plot = new QImPlotNode();
    addRenderNode(plot);
    Q_EMIT plotNodeAttached(plot, true);
    return plot;
}

QList<QImPlotNode*> QImFigureWidget::plotNodes() const
{
    QIM_DC(d);
    if (d->m_subplotNode) {
        return d->m_subplotNode->plotNodes();
    }
    // No subplot — search root render node children for QImPlotNode*
    QList<QImPlotNode*> result;
    const auto children = renderNodeList();
    for (QImAbstractNode* node : children) {
        if (QImPlotNode* plot = qobject_cast<QImPlotNode*>(node)) {
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
    if (!plot) {
        return;
    }
    QIM_D(d);
    if (d->m_subplotNode) {
        d->m_subplotNode->addPlotNode(plot);
    } else {
        addRenderNode(plot);
    }
    Q_EMIT plotNodeAttached(plot, true);
}

void QImFigureWidget::insertPlotNode(int plotIndex, QImPlotNode* plot)
{
    if (!plot) {
        return;
    }
    QIM_D(d);
    if (d->m_subplotNode) {
        d->m_subplotNode->insertPlotNode(plotIndex, plot);
    } else {
        addRenderNode(plot);
    }
    Q_EMIT plotNodeAttached(plot, true);
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
    if (!plot) {
        return false;
    }
    QIM_D(d);
    if (d->m_subplotNode) {
        const bool ok = d->m_subplotNode->takePlotNode(plot);
        if (ok) {
            Q_EMIT plotNodeAttached(plot, false);
        }
        return ok;
    }
    return takeRenderNode(plot);
}

void QImFigureWidget::removePlotNode(QImPlotNode* plot)
{
    if (!plot) {
        return;
    }
    QIM_D(d);
    if (d->m_subplotNode) {
        d->m_subplotNode->removePlotNode(plot);
    } else {
        removeRenderNode(plot);
    }
    Q_EMIT plotNodeAttached(plot, false);
}

QImPlot3DExtNode* QImFigureWidget::createPlotNode3D()
{
    // 3D nodes are always top-level render nodes — ImPlot3D has no subplot API
    QImPlot3DExtNode* plot3D = new QImPlot3DExtNode();
    addRenderNode(plot3D);
    return plot3D;
}

QList<QImPlot3DExtNode*> QImFigureWidget::plot3DNodes() const
{
    QList<QImPlot3DExtNode*> result;
    const auto children = renderNodeList();
    for (QImAbstractNode* node : children) {
        if (QImPlot3DExtNode* plot3D = qobject_cast<QImPlot3DExtNode*>(node)) {
            result.append(plot3D);
        }
    }
    return result;
}

int QImFigureWidget::plot3DCount() const
{
    return plot3DNodes().size();
}

void QImFigureWidget::addPlotNode3D(QImPlot3DExtNode* plot3D)
{
    if (plot3D) {
        addRenderNode(plot3D);
    }
}

void QImFigureWidget::removePlotNode3D(QImPlot3DExtNode* plot3D)
{
    if (plot3D) {
        removeRenderNode(plot3D);
    }
}

void QImFigureWidget::initializeGL()
{
    QIM_D(d);
    QImWidget::initializeGL();
    d->m_context2D = ImPlot::CreateContext();
    d->m_context3D = ImPlot3D::CreateContext();
}

void QImFigureWidget::beforeRenderImNodes()
{
    QIM_D(d);
    QImWidget::beforeRenderImNodes();
    if (d->m_context2D) {
        ImPlot::SetCurrentContext(d->m_context2D);
        d->m_theme.value().apply(&ImPlot::GetStyle());
    }
    if (d->m_context3D) {
        ImPlot3D::SetCurrentContext(d->m_context3D);
    }
}

void QImFigureWidget::onSubplotChildNodeRemoved(QImAbstractNode* c)
{
    QImPlotNode* plot = qobject_cast<QImPlotNode*>(c);
    if (plot) {
        Q_EMIT plotNodeAttached(plot, false);
    }
}

void QImFigureWidget::onSubplotChildNodeAdded(QImAbstractNode* c)
{
    QImPlotNode* plot = qobject_cast<QImPlotNode*>(c);
    if (plot) {
        Q_EMIT plotNodeAttached(plot, true);
    }
}

}  // end namespace QIM