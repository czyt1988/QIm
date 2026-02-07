#include "ImWidget2D.h"
#include "implot.h"

ImWidget2D::ImWidget2D(QWidget* parent, Qt::WindowFlags f) : QIM::QImWidget(parent, f)
{
    setRenderMode(RenderContinuous);  // 有动画，使用RenderContinuous
}

ImWidget2D::~ImWidget2D()
{
}

void ImWidget2D::afterRenderImNodes()
{
    ImPlot::ShowDemoWindow();
}

void ImWidget2D::initializeGL()
{
    QIM::QImWidget::initializeGL();
    m_context = ImPlot::CreateContext();
}
