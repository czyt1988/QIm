#include "QImFigureWidget.h"
#include <QDebug>
#include "QImTrackedValue.hpp"
#include "implot.h"
#include "plot/QImSubplotsNode.h"
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
    d_ptr->m_subplotNode = new QImSubplotsNode();
    addRenderNode(d_ptr->m_subplotNode.data());
    d_ptr->m_subplotNode->setTitleEnabled(true);
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

void QImFigureWidget::setSubplotGrid(int rows,
                                     int cols,
                                     const std::vector< float >& rowsRatios,
                                     const std::vector< float >& colsRatios)
{
    QIM_D(d);
    if (d->m_subplotNode) {
        d->m_subplotNode->setGrid(rows, cols, rowsRatios, colsRatios);
    }
}

QImSubplotsNode* QImFigureWidget::subplotNode() const
{
    return d_ptr->m_subplotNode.data();
}

QImPlotNode* QImFigureWidget::createPlotNode()
{
    return d_ptr->m_subplotNode->createPlotNode();
}

void QImFigureWidget::addPlotNode(QImPlotNode* plot)
{
    return d_ptr->m_subplotNode->addPlotNode(plot);
}

void QImFigureWidget::insertPlotNode(int plotIndex, QImPlotNode* plot)
{
    return d_ptr->m_subplotNode->insertPlotNode(plotIndex, plot);
}

void QImFigureWidget::initializeGL()
{
    QIM_D(d);
    QIM::QImWidget::initializeGL();
    d->m_context = ImPlot::CreateContext();
    // 默认有个subplot
}

}  // end namespace QIM
