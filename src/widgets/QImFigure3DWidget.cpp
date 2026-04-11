#include "QImFigure3DWidget.h"
#include "plot/QImPlot3DNode.h"
#include "plot/QImSubplots3DNode.h"
#include "implot3d.h"

namespace QIM
{
QImFigure3DWidget::QImFigure3DWidget(QWidget* parent, Qt::WindowFlags f) : QImWidget(parent, f)
{
    m_subplotNode = new QImSubplots3DNode();
    addRenderNode(m_subplotNode);
}

QImFigure3DWidget::~QImFigure3DWidget()
{
}

void QImFigure3DWidget::setSubplotGrid(int rows, int cols)
{
    if (m_subplotNode) {
        m_subplotNode->setGrid(rows, cols);
    }
}

int QImFigure3DWidget::subplotGridRows() const
{
    return m_subplotNode ? m_subplotNode->rows() : -1;
}

int QImFigure3DWidget::subplotGridColumns() const
{
    return m_subplotNode ? m_subplotNode->columns() : -1;
}

QImSubplots3DNode* QImFigure3DWidget::subplotNode() const
{
    return m_subplotNode;
}

QImPlot3DNode* QImFigure3DWidget::createPlotNode()
{
    return m_subplotNode ? m_subplotNode->createPlotNode() : nullptr;
}

QList< QImPlot3DNode* > QImFigure3DWidget::plotNodes() const
{
    return m_subplotNode ? m_subplotNode->plotNodes() : QList< QImPlot3DNode* >();
}

int QImFigure3DWidget::plotCount() const
{
    return m_subplotNode ? m_subplotNode->plotCount() : 0;
}

void QImFigure3DWidget::initializeGL()
{
    QImWidget::initializeGL();
    m_context = ImPlot3D::CreateContext();
}

void QImFigure3DWidget::beforeRenderImNodes()
{
    QImWidget::beforeRenderImNodes();
    if (m_context) {
        ImPlot3D::SetCurrentContext(m_context);
    }
}
}  // namespace QIM
