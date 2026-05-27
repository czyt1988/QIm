#include "QImPlot3DMousePickerNode.h"
#include "QImPlot3DNode.h"
#include <QColor>
#include <cmath>
#include <limits>
#include "implot3d.h"
#include "imgui.h"
#include "QtImGuiUtils.h"

namespace QIM {

class QImPlot3DMousePickerNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlot3DMousePickerNode)

public:
    PrivateData(QImPlot3DMousePickerNode* q);

    // Data
    QImAbstractXYZDataSeries* dataSeries {nullptr};

    // Visual
    bool showMarker {true};
    bool showCoordinatesText {true};
    ImVec4 markerColorImV4 {1.0f, 1.0f, 0.0f, 1.0f};   // yellow
    float markerSize {5.0f};

    // Helper line
    ImVec4 helperLineColorImV4 {1.0f, 1.0f, 0.0f, 1.0f};  // yellow
    float helperLineWidth {2.0f};

    // Frame state (NaN = "no value")
    static constexpr double sNaN = std::numeric_limits<double>::quiet_NaN();

    QImPlot3DPoint hoveredPoint {sNaN, sNaN, sNaN};
    int hoveredIndex {-1};
    bool plotHovered {false};
};

QImPlot3DMousePickerNode::PrivateData::PrivateData(QImPlot3DMousePickerNode* q)
    : q_ptr(q)
{
}

//----------------------------------------------------
// QImPlot3DMousePickerNode
//----------------------------------------------------

/**
 * \if ENGLISH
 * @brief Constructor for QImPlot3DMousePickerNode
 * @param parent Parent QObject (typically a QImPlot3DNode)
 * @details Initializes the picker with defaults:
 *          - Yellow marker and helper line, coordinate text display
 *          - Always renders (RenderIgnoreVisible), high z-order (99999)
 * \endif
 *
 * \if CHINESE
 * @brief QImPlot3DMousePickerNode 构造函数
 * @param parent 父 QObject（通常是 QImPlot3DNode）
 * @details 使用默认配置初始化拾取器：
 *          - 黄色标记和辅助线，显示坐标文本
 *          - 始终渲染（RenderIgnoreVisible），高 z-order（99999）
 * \endif
 */
QImPlot3DMousePickerNode::QImPlot3DMousePickerNode(QObject* parent)
    : QImPlot3DItemNode(parent)
    , QIM_PIMPL_CONSTRUCT
{
    setAutoIdEnabled(false);
    setRenderOption(RenderIgnoreVisible, true);
    setZOrder(99999);
    setObjectName("3d_tools_mousepicker");
}

/**
 * \if ENGLISH
 * @brief Destructor for QImPlot3DMousePickerNode
 * \endif
 *
 * \if CHINESE
 * @brief QImPlot3DMousePickerNode 析构函数
 * \endif
 */
QImPlot3DMousePickerNode::~QImPlot3DMousePickerNode()
{
}

//----------------------------------------------------
// Data
//----------------------------------------------------

/**
 * \if ENGLISH
 * @brief Sets the data series to pick from
 * @param series Data series (not owned by this node; caller manages lifetime)
 * \endif
 *
 * \if CHINESE
 * @brief 设置用于拾取的数据系列
 * @param series 数据系列（非此节点拥有；由调用者管理生命周期）
 * \endif
 */
void QImPlot3DMousePickerNode::setData(QImAbstractXYZDataSeries* series)
{
    QIM_D(d);
    d->dataSeries = series;
}

/**
 * \if ENGLISH
 * @brief Gets the current data series
 * @return Current data series pointer (nullptr if not set)
 * \endif
 *
 * \if CHINESE
 * @brief 获取当前数据系列
 * @return 当前数据系列指针（未设置时为 nullptr）
 * \endif
 */
QImAbstractXYZDataSeries* QImPlot3DMousePickerNode::data() const
{
    QIM_DC(d);
    return d->dataSeries;
}

//----------------------------------------------------
// Visual
//----------------------------------------------------

bool QImPlot3DMousePickerNode::isShowMarker() const
{
    QIM_DC(d);
    return d->showMarker;
}

void QImPlot3DMousePickerNode::setShowMarker(bool show)
{
    QIM_D(d);
    d->showMarker = show;
}

bool QImPlot3DMousePickerNode::isShowCoordinatesText() const
{
    QIM_DC(d);
    return d->showCoordinatesText;
}

void QImPlot3DMousePickerNode::setShowCoordinatesText(bool show)
{
    QIM_D(d);
    d->showCoordinatesText = show;
}

QColor QImPlot3DMousePickerNode::markerColor() const
{
    QIM_DC(d);
    return toQColor(d->markerColorImV4);
}

void QImPlot3DMousePickerNode::setMarkerColor(const QColor& color)
{
    QIM_D(d);
    d->markerColorImV4 = toImVec4(color);
}

float QImPlot3DMousePickerNode::markerSize() const
{
    QIM_DC(d);
    return d->markerSize;
}

void QImPlot3DMousePickerNode::setMarkerSize(float size)
{
    QIM_D(d);
    d->markerSize = size;
}

//----------------------------------------------------
// Helper line
//----------------------------------------------------

QColor QImPlot3DMousePickerNode::helperLineColor() const
{
    QIM_DC(d);
    return toQColor(d->helperLineColorImV4);
}

void QImPlot3DMousePickerNode::setHelperLineColor(const QColor& color)
{
    QIM_D(d);
    d->helperLineColorImV4 = toImVec4(color);
}

float QImPlot3DMousePickerNode::helperLineWidth() const
{
    QIM_DC(d);
    return d->helperLineWidth;
}

void QImPlot3DMousePickerNode::setHelperLineWidth(float width)
{
    QIM_D(d);
    d->helperLineWidth = width;
}

//----------------------------------------------------
// Read-only state
//----------------------------------------------------

QImPlot3DPoint QImPlot3DMousePickerNode::hoveredPoint() const
{
    QIM_DC(d);
    return d->hoveredPoint;
}

int QImPlot3DMousePickerNode::hoveredIndex() const
{
    QIM_DC(d);
    return d->hoveredIndex;
}

bool QImPlot3DMousePickerNode::isPlotHovered() const
{
    QIM_DC(d);
    return d->plotHovered;
}

bool QImPlot3DMousePickerNode::hasValidHoveredPoint() const
{
    QIM_DC(d);
    return !d->hoveredPoint.IsNaN();
}

//----------------------------------------------------
// Helper: ImGuiMouseButton → Qt::MouseButton
//----------------------------------------------------

namespace {
    Qt::MouseButton toQtMouseButton(int imguiButton)
    {
        switch (imguiButton) {
        case ImGuiMouseButton_Left:   return Qt::LeftButton;
        case ImGuiMouseButton_Right:  return Qt::RightButton;
        case ImGuiMouseButton_Middle: return Qt::MiddleButton;
        default:                      return Qt::NoButton;
        }
    }
}

//----------------------------------------------------
// Rendering
//----------------------------------------------------

/**
 * \if ENGLISH
 * @brief Per-frame mouse picking and visual feedback rendering
 * @return false (no endDraw needed, no child nodes to render)
 * @details Execution each frame:
 *          1. Get mouse screen position from ImGui
 *          2. Check if mouse is over the plot via GetPlotRectPos/GetPlotRectSize
 *          3. If hovered and data exists, iterate all data points
 *          4. For each point, ImPlot3D::PlotToPixels → compute screen-space distance
 *          5. Find closest point, draw helper line via ImPlot3D::GetPlotDrawList
 *          6. Draw marker at closest point (draw list circle)
 *          7. Draw coordinate text via ImPlot3D::PlotText
 *          8. Show tooltip via virtual renderTooltip()
 *          9. Detect clicks + emit picked/doubleClicked with button enum
 *
 *          Pattern follows DemoCustomOverlay from implot3d_demo.cpp.
 *          No per-frame state-change signals — only event signals on clicks.
 * \endif
 *
 * \if CHINESE
 * @brief 每帧鼠标拾取和视觉反馈渲染
 * @return false（无需 endDraw，无子节点需渲染）
 * @details 每帧执行：
 *          1. 从 ImGui 获取鼠标屏幕位置
 *          2. 通过 GetPlotRectPos/GetPlotRectSize 检查鼠标是否在绘图区域
 *          3. 如果悬停且有数据，遍历所有数据点
 *          4. 对每个点 ImPlot3D::PlotToPixels → 计算屏幕空间距离
 *          5. 找到最近点，通过 ImPlot3D::GetPlotDrawList 画辅助线
 *          6. 在最近点画标记（draw list 圆圈）
 *          7. 通过 ImPlot3D::PlotText 画坐标文本
 *          8. 通过虚函数 renderTooltip() 显示 tooltip
 *          9. 检测点击 + 用按钮枚举发射 picked/doubleClicked
 *
 *          模式参考 implot3d_demo.cpp 中的 DemoCustomOverlay。
 *          无每帧状态变化信号——仅在点击时发射事件信号。
 * \endif
 */
bool QImPlot3DMousePickerNode::beginDraw()
{
    QIM_D(d);
    QImPlot3DNode* plot = plot3DNode();
    if (!plot) return false;

    // 1. Get mouse position in screen space
    ImVec2 mouseScreenPos = ImGui::GetMousePos();

    // 2. Check if mouse is over the plot area
    ImVec2 plotPos = ImPlot3D::GetPlotRectPos();
    ImVec2 plotSize = ImPlot3D::GetPlotRectSize();
    bool isHovered = (mouseScreenPos.x >= plotPos.x && mouseScreenPos.x <= plotPos.x + plotSize.x &&
                      mouseScreenPos.y >= plotPos.y && mouseScreenPos.y <= plotPos.y + plotSize.y);

    d->plotHovered = isHovered;

    // 3. Find closest data point to mouse in screen space
    int closestIndex = -1;
    float minDistSq = 1e10f;
    ImVec2 closestPx;
    double closestX = d->sNaN, closestY = d->sNaN, closestZ = d->sNaN;
    float closestDist = 0.0f;

    if (isHovered && d->dataSeries && d->dataSeries->isValid()) {
        ImDrawList* drawList = ImPlot3D::GetPlotDrawList();
        int n = d->dataSeries->size();

        for (int i = 0; i < n; i++) {
            double px = d->dataSeries->xValue(i);
            double py = d->dataSeries->yValue(i);
            double pz = d->dataSeries->zValue(i);
            ImVec2 pointPx = ImPlot3D::PlotToPixels(px, py, pz);
            float dx = pointPx.x - mouseScreenPos.x;
            float dy = pointPx.y - mouseScreenPos.y;
            float distSq = dx * dx + dy * dy;
            if (distSq < minDistSq) {
                minDistSq = distSq;
                closestIndex = i;
                closestPx = pointPx;
                closestX = px;
                closestY = py;
                closestZ = pz;
            }
        }

        if (closestIndex >= 0) {
            closestDist = ImSqrt(minDistSq);

            // 4. Draw helper line from mouse to closest point
            if (d->helperLineWidth > 0.0f) {
                ImU32 lineColor = ImGui::ColorConvertFloat4ToU32(d->helperLineColorImV4);
                drawList->AddLine(mouseScreenPos, closestPx, lineColor, d->helperLineWidth);
            }

            // 5. Draw marker at closest point
            if (d->showMarker) {
                ImU32 markerColor = ImGui::ColorConvertFloat4ToU32(d->markerColorImV4);
                drawList->AddCircleFilled(closestPx, d->markerSize, markerColor);
                drawList->AddCircle(closestPx, d->markerSize + 1.0f, IM_COL32(255, 255, 255, 200), 1.0f);
            }

            // 6. Draw coordinate text at the closest data point
            if (d->showCoordinatesText) {
                char buf[64];
                snprintf(buf, sizeof(buf), "(%.2f, %.2f, %.2f)", closestX, closestY, closestZ);
                ImPlot3D::PlotText(buf, closestX, closestY, closestZ, 0.0, ImVec2(10, -10));
            }

            // 7. Show tooltip
            ImGui::BeginTooltip();
            renderTooltip(closestIndex, closestX, closestY, closestZ, closestDist, mouseScreenPos.x,mouseScreenPos.y);
            ImGui::EndTooltip();
        }
    }

    // 8. Cache hovered state for getter access
    d->hoveredPoint = (closestIndex >= 0)
        ? QImPlot3DPoint(closestX, closestY, closestZ)
        : QImPlot3DPoint(d->sNaN, d->sNaN, d->sNaN);
    d->hoveredIndex = closestIndex;

    // 9. Detect mouse clicks — emit event signals only on click frames
    bool pointValid = (closestIndex >= 0);
    if (isHovered && pointValid) {
        const QImPlot3DPoint pt(closestX, closestY, closestZ);

        // Single-click: check all mouse buttons
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            Q_EMIT picked(Qt::LeftButton, pt);
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
            Q_EMIT picked(Qt::RightButton, pt);
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Middle))
            Q_EMIT picked(Qt::MiddleButton, pt);

        // Double-click: check all mouse buttons
        if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            Q_EMIT doubleClicked(Qt::LeftButton, pt);
        if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Right))
            Q_EMIT doubleClicked(Qt::RightButton, pt);
        if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Middle))
            Q_EMIT doubleClicked(Qt::MiddleButton, pt);
    }

    return false;
}

/**
 * \if ENGLISH
 * @brief Default tooltip rendering — shows mouse position, point index, coordinates, and distance
 * @details Override this in a subclass to customize. Called inside BeginTooltip/EndTooltip scope.
 * \endif
 *
 * \if CHINESE
 * @brief 默认 tooltip 渲染——显示鼠标位置、点索引、坐标和距离
 * @details 在子类中重写此方法以自定义。在 BeginTooltip/EndTooltip 作用域内调用。
 * \endif
 */
void QImPlot3DMousePickerNode::renderTooltip(int index, double x, double y, double z,
                                              float distancePx, float mouseScreenPosX, float mouseScreenPosY)
{
    ImGui::Text("Mouse: (%.1f, %.1f)", mouseScreenPosX, mouseScreenPosY);

    if (index >= 0) {
        ImGui::Text("Point #%d: (%.3f, %.3f, %.3f)", index, x, y, z);
        ImGui::Text("Distance: %.1f px", distancePx);
    }
}

}  // namespace QIM
