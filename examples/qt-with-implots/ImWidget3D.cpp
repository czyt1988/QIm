#include "ImWidget3D.h"
#include "implot3d.h"
ImWidget3D::ImWidget3D(QWidget* parent, Qt::WindowFlags f) : QIM::QImWidget(parent, f)
{
    setRenderMode(RenderContinuous);  // 有动画，使用RenderContinuous模式
}

void ImWidget3D::afterRenderImNodes()
{
    ImPlot3D::ShowDemoWindow();
}

void ImWidget3D::initializeGL()
{
    QIM::QImWidget::initializeGL();
    m_context = ImPlot3D::CreateContext();
}
