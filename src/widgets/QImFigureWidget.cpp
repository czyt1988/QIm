#include "QImFigureWidget.h"
#include "QImTrackedValue.hpp"
#include "implot.h"
#include "implot3d.h"
#include "plot/QImSubplotsNode.h"
#include "plot/QImPlot3DNode.h"
#include "plot/QImPlotNode.h"

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
    QImTrackedValue< QImPlotTheme > m_theme;
    QPointer< QImSubplotsNode > m_subplotNode;
    ImPlotContext* m_context2D { nullptr };
    ImPlot3DContext* m_context3D { nullptr };
};

QImFigureWidget::QImFigureWidget(QWidget* parent, Qt::WindowFlags f) : QImWidget(parent, f), QIM_PIMPL_CONSTRUCT
{
    d_ptr->m_subplotNode = new QImSubplotsNode();
    addRenderNode(d_ptr->m_subplotNode.data());
}

QImFigureWidget::~QImFigureWidget()
{
}

void QImFigureWidget::setPlotTheme(const QImPlotTheme& t)
{
    d_ptr->m_theme = t;
}

const QImPlotTheme& QImFigureWidget::plotTheme() const
{
    return d_ptr->m_theme.value();
}

void QImFigureWidget::setSubplotGrid(int rows, int cols, const std::vector< float >& rowsRatios, const std::vector< float >& colsRatios)
{
    Q_UNUSED(rowsRatios);
    Q_UNUSED(colsRatios);
    QIM_D(d);
    if (d->m_subplotNode) {
        d->m_subplotNode->setGrid(rows, cols);
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

std::vector< float > QImFigureWidget::subplotGridRowRatios() const
{
    return {};
}

std::vector< float > QImFigureWidget::subplotGridColumnRatios() const
{
    return {};
}

QImSubplotsNode* QImFigureWidget::subplotNode() const
{
    return d_ptr->m_subplotNode.data();
}

QImPlotNode* QImFigureWidget::createPlotNode()
{
    QImPlotNode* plot = d_ptr->m_subplotNode ? d_ptr->m_subplotNode->createPlotNode() : nullptr;
    if (plot) {
        Q_EMIT plotNodeAttached(plot, true);
    }
    return plot;
}

QList< QImPlotNode* > QImFigureWidget::plotNodes() const
{
    return d_ptr->m_subplotNode ? d_ptr->m_subplotNode->plotNodes() : QList< QImPlotNode* >();
}

int QImFigureWidget::plotCount() const
{
    return d_ptr->m_subplotNode ? d_ptr->m_subplotNode->plotCount() : 0;
}

void QImFigureWidget::addPlotNode(QImPlotNode* plot)
{
    if (!d_ptr->m_subplotNode || !plot) {
        return;
    }
    d_ptr->m_subplotNode->addPlotNode(plot);
    Q_EMIT plotNodeAttached(plot, true);
}

void QImFigureWidget::insertPlotNode(int plotIndex, QImPlotNode* plot)
{
    if (!d_ptr->m_subplotNode || !plot) {
        return;
    }
    d_ptr->m_subplotNode->insertPlotNode(plotIndex, plot);
    Q_EMIT plotNodeAttached(plot, true);
}

int QImFigureWidget::plotNodeSubplotIndex(QImPlotNode* plot)
{
    return d_ptr->m_subplotNode ? d_ptr->m_subplotNode->plotNodeSubplotIndex(plot) : -1;
}

bool QImFigureWidget::takePlotNode(QImPlotNode* plot)
{
    if (!d_ptr->m_subplotNode || !plot) {
        return false;
    }
    const bool ok = d_ptr->m_subplotNode->takePlotNode(plot);
    if (ok) {
        Q_EMIT plotNodeAttached(plot, false);
    }
    return ok;
}

void QImFigureWidget::removePlotNode(QImPlotNode* plot)
{
    if (!d_ptr->m_subplotNode || !plot) {
        return;
    }
    d_ptr->m_subplotNode->removePlotNode(plot);
    Q_EMIT plotNodeAttached(plot, false);
}

QImPlot3DNode* QImFigureWidget::createPlot3DNode()
{
    return d_ptr->m_subplotNode ? d_ptr->m_subplotNode->createPlot3DNode() : nullptr;
}

QList< QImPlot3DNode* > QImFigureWidget::plot3DNodes() const
{
    return d_ptr->m_subplotNode ? d_ptr->m_subplotNode->plot3DNodes() : QList< QImPlot3DNode* >();
}

int QImFigureWidget::plot3DCount() const
{
    return d_ptr->m_subplotNode ? d_ptr->m_subplotNode->plot3DCount() : 0;
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
}  // end namespace QIM
