#include "ImTestWidget.h"
#include "imgui.h"

ImTestWidget::ImTestWidget(QWidget* parent, Qt::WindowFlags f) : QIM::QImWidget(parent, f)
{
    setRenderMode(RenderOnDemand);
}

void ImTestWidget::afterRenderImNodes()
{
    ImGui::Begin(u8"控制面板");
    ImGui::Text("这是一段中文文本！");
    ImGui::Text("Hello, 世界！");
    ImGui::Text("中文输入测试: 你好，世界！");
    ImGui::End();
}
