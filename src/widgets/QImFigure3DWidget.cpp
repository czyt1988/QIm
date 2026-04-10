#include "QImFigure3DWidget.h"
#include "plot/QImPlot3DNode.h"
#include "implot3d.h"

namespace QIM
{
QImFigure3DWidget::QImFigure3DWidget(QWidget* parent, Qt::WindowFlags f) : QImWidget(parent, f)
{
}

QImFigure3DWidget::~QImFigure3DWidget()
{
}

QImPlot3DNode* QImFigure3DWidget::createPlotNode()
{
    QImPlot3DNode* plot = new QImPlot3DNode();
    addRenderNode(plot);
    return plot;
}

QList< QImPlot3DNode* > QImFigure3DWidget::plotNodes() const
{
    QList< QImPlot3DNode* > plots;
    const QList< QImAbstractNode* > nodes = renderNodeList();
    for (QImAbstractNode* node : nodes) {
        if (QImPlot3DNode* plot = qobject_cast< QImPlot3DNode* >(node)) {
            plots.push_back(plot);
        }
    }
    return plots;
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
