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
    bool clicked {false};
    bool doubleClicked {false};
    bool rightClicked {false};

    // Previous frame values for change detection
    bool prevHovered {false};
    QImPlot3DPoint prevHoveredPoint {sNaN, sNaN, sNaN};
    int prevHoveredIndex {-1};
    bool prevClicked {false};
    bool prevDoubleClicked {false};
    bool prevRightClicked {false};
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

/**
 * \if ENGLISH
 * @brief Checks if hover marker is visible
 * @return true if marker rendering is enabled
 * \endif
 *
 * \if CHINESE
 * @brief 检查悬停标记是否可见
 * @return 如果标记渲染启用则返回 true
 * \endif
 */
bool QImPlot3DMousePickerNode::isShowMarker() const
{
    QIM_DC(d);
    return d->showMarker;
}

/**
 * \if ENGLISH
 * @brief Sets hover marker visibility
 * @param show New marker visibility state
 * \endif
 *
 * \if CHINESE
 * @brief 设置悬停标记可见性
 * @param show 新的标记可见状态
 * \endif
 */
void QImPlot3DMousePickerNode::setShowMarker(bool show)
{
    QIM_D(d);
    if (d->showMarker != show) {
        d->showMarker = show;
        Q_EMIT showMarkerChanged(show);
    }
}

/**
 * \if ENGLISH
 * @brief Checks if coordinate text is visible
 * @return true if coordinate text rendering is enabled
 * \endif
 *
 * \if CHINESE
 * @brief 检查坐标文本是否可见
 * @return 如果坐标文本渲染启用则返回 true
 * \endif
 */
bool QImPlot3DMousePickerNode::isShowCoordinatesText() const
{
    QIM_DC(d);
    return d->showCoordinatesText;
}

/**
 * \if ENGLISH
 * @brief Sets coordinate text visibility
 * @param show New coordinate text visibility state
 * \endif
 *
 * \if CHINESE
 * @brief 设置坐标文本可见性
 * @param show 新的坐标文本可见状态
 * \endif
 */
void QImPlot3DMousePickerNode::setShowCoordinatesText(bool show)
{
    QIM_D(d);
    if (d->showCoordinatesText != show) {
        d->showCoordinatesText = show;
        Q_EMIT showCoordinatesTextChanged(show);
    }
}

/**
 * \if ENGLISH
 * @brief Gets the marker color
 * @return Current marker color as QColor
 * \endif
 *
 * \if CHINESE
 * @brief 获取标记颜色
 * @return 当前标记颜色（QColor）
 * \endif
 */
QColor QImPlot3DMousePickerNode::markerColor() const
{
    QIM_DC(d);
    return toQColor(d->markerColorImV4);
}

/**
 * \if ENGLISH
 * @brief Sets the marker color
 * @param color New marker color (converted to ImVec4 in setter for beginDraw efficiency)
 * \endif
 *
 * \if CHINESE
 * @brief 设置标记颜色
 * @param color 新的标记颜色（在 setter 中转换为 ImVec4 以提高 beginDraw 效率）
 * \endif
 */
void QImPlot3DMousePickerNode::setMarkerColor(const QColor& color)
{
    QIM_D(d);
    d->markerColorImV4 = toImVec4(color);
    Q_EMIT markerColorChanged(color);
}

/**
 * \if ENGLISH
 * @brief Gets the marker size in pixels
 * @return Current marker size
 * \endif
 *
 * \if CHINESE
 * @brief 获取标记大小（像素）
 * @return 当前标记大小
 * \endif
 */
float QImPlot3DMousePickerNode::markerSize() const
{
    QIM_DC(d);
    return d->markerSize;
}

/**
 * \if ENGLISH
 * @brief Sets the marker size in pixels
 * @param size New marker size
 * \endif
 *
 * \if CHINESE
 * @brief 设置标记大小（像素）
 * @param size 新的标记大小
 * \endif
 */
void QImPlot3DMousePickerNode::setMarkerSize(float size)
{
    QIM_D(d);
    if (d->markerSize != size) {
        d->markerSize = size;
        Q_EMIT markerSizeChanged(size);
    }
}

//----------------------------------------------------
// Helper line
//----------------------------------------------------

/**
 * \if ENGLISH
 * @brief Gets the helper line color
 * @return Current helper line color as QColor
 * \endif
 *
 * \if CHINESE
 * @brief 获取辅助线颜色
 * @return 当前辅助线颜色（QColor）
 * \endif
 */
QColor QImPlot3DMousePickerNode::helperLineColor() const
{
    QIM_DC(d);
    return toQColor(d->helperLineColorImV4);
}

/**
 * \if ENGLISH
 * @brief Sets the helper line color
 * @param color New helper line color (converted to ImVec4 in setter for beginDraw efficiency)
 * \endif
 *
 * \if CHINESE
 * @brief 设置辅助线颜色
 * @param color 新的辅助线颜色（在 setter 中转换为 ImVec4 以提高 beginDraw 效率）
 * \endif
 */
void QImPlot3DMousePickerNode::setHelperLineColor(const QColor& color)
{
    QIM_D(d);
    d->helperLineColorImV4 = toImVec4(color);
    Q_EMIT helperLineColorChanged(color);
}

/**
 * \if ENGLISH
 * @brief Gets the helper line width in pixels
 * @return Current helper line width
 * \endif
 *
 * \if CHINESE
 * @brief 获取辅助线宽度（像素）
 * @return 当前辅助线宽度
 * \endif
 */
float QImPlot3DMousePickerNode::helperLineWidth() const
{
    QIM_DC(d);
    return d->helperLineWidth;
}

/**
 * \if ENGLISH
 * @brief Sets the helper line width in pixels
 * @param width New helper line width
 * \endif
 *
 * \if CHINESE
 * @brief 设置辅助线宽度（像素）
 * @param width 新的辅助线宽度
 * \endif
 */
void QImPlot3DMousePickerNode::setHelperLineWidth(float width)
{
    QIM_D(d);
    if (d->helperLineWidth != width) {
        d->helperLineWidth = width;
        Q_EMIT helperLineWidthChanged(width);
    }
}

//----------------------------------------------------
// Read-only state
//----------------------------------------------------

/**
 * \if ENGLISH
 * @brief Gets the hovered 3D point
 * @return Current hovered point (NaN components if no valid data point nearby)
 * \endif
 *
 * \if CHINESE
 * @brief 获取悬停的三维点
 * @return 当前悬停点（附近无有效数据点时分量为 NaN）
 * \endif
 */
QImPlot3DPoint QImPlot3DMousePickerNode::hoveredPoint() const
{
    QIM_DC(d);
    return d->hoveredPoint;
}

/**
 * \if ENGLISH
 * @brief Gets the index of the closest data point
 * @return Index in the data series (-1 if none)
 * \endif
 *
 * \if CHINESE
 * @brief 获取最近数据点的索引
 * @return 数据系列中的索引（无时为 -1）
 * \endif
 */
int QImPlot3DMousePickerNode::hoveredIndex() const
{
    QIM_DC(d);
    return d->hoveredIndex;
}

/**
 * \if ENGLISH
 * @brief Checks if the mouse is hovering over the plot area
 * @return true if mouse is within the plot bounds
 * \endif
 *
 * \if CHINESE
 * @brief 检查鼠标是否悬停在绘图区域上方
 * @return 如果鼠标在绘图边界内则返回 true
 * \endif
 */
bool QImPlot3DMousePickerNode::isPlotHovered() const
{
    QIM_DC(d);
    return d->plotHovered;
}

/**
 * \if ENGLISH
 * @brief Checks if left mouse button was clicked this frame
 * @return true on the frame a left click occurred on a valid point
 * \endif
 *
 * \if CHINESE
 * @brief 检查当前帧是否发生左键点击
 * @return 在有效点上发生左键点击的帧返回 true
 * \endif
 */
bool QImPlot3DMousePickerNode::isClicked() const
{
    QIM_DC(d);
    return d->clicked;
}

/**
 * \if ENGLISH
 * @brief Checks if left mouse button was double-clicked this frame
 * @return true on the frame a double-click occurred on a valid point
 * \endif
 *
 * \if CHINESE
 * @brief 检查当前帧是否发生左键双击
 * @return 在有效点上发生左键双击的帧返回 true
 * \endif
 */
bool QImPlot3DMousePickerNode::isDoubleClicked() const
{
    QIM_DC(d);
    return d->doubleClicked;
}

/**
 * \if ENGLISH
 * @brief Checks if right mouse button was clicked this frame
 * @return true on the frame a right click occurred on a valid point
 * \endif
 *
 * \if CHINESE
 * @brief 检查当前帧是否发生右键点击
 * @return 在有效点上发生右键点击的帧返回 true
 * \endif
 */
bool QImPlot3DMousePickerNode::isRightClicked() const
{
    QIM_DC(d);
    return d->rightClicked;
}

//----------------------------------------------------
// Convenience
//----------------------------------------------------

/**
 * \if ENGLISH
 * @brief Checks if the hovered point is valid (not NaN)
 * @return true if hoveredPoint has non-NaN coordinates
 * \endif
 *
 * \if CHINESE
 * @brief 检查悬停点是否有效（非 NaN）
 * @return 如果 hoveredPoint 的坐标非 NaN 则返回 true
 * \endif
 */
bool QImPlot3DMousePickerNode::hasValidHoveredPoint() const
{
    QIM_DC(d);
    return !d->hoveredPoint.IsNaN();
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
 *          8. Show tooltip with point info
 *          9. Detect click/double-click/right-click + emit signals
 *
 *          Pattern follows DemoCustomOverlay from implot3d_demo.cpp.
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
 *          8. 显示 tooltip 含点信息
 *          9. 检测点击/双击/右击 + 发射信号
 *
 *          模式参考 implot3d_demo.cpp 中的 DemoCustomOverlay。
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
    if (d->plotHovered != d->prevHovered) {
        Q_EMIT plotHoveredChanged(d->plotHovered);
    }
    d->prevHovered = d->plotHovered;

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
            renderTooltip(closestIndex, closestX, closestY, closestZ, closestDist, mouseScreenPos);
            ImGui::EndTooltip();
        }
    }

    // 8. Update hovered point and emit signals
    QImPlot3DPoint newHoveredPoint = (closestIndex >= 0)
        ? QImPlot3DPoint(closestX, closestY, closestZ)
        : QImPlot3DPoint(d->sNaN, d->sNaN, d->sNaN);

    bool pointChanged = !fuzzyEqual(newHoveredPoint, d->prevHoveredPoint);
    d->hoveredPoint = newHoveredPoint;
    d->hoveredIndex = closestIndex;

    if (pointChanged) {
        Q_EMIT hoveredPointChanged(newHoveredPoint);
    }
    if (closestIndex != d->prevHoveredIndex) {
        Q_EMIT hoveredIndexChanged(closestIndex);
    }
    d->prevHoveredPoint = newHoveredPoint;
    d->prevHoveredIndex = closestIndex;

    // 9. Detect mouse clicks
    bool pointValid = (closestIndex >= 0);
    bool click = isHovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && pointValid;
    bool dblClick = isHovered && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && pointValid;
    bool rClick = isHovered && ImGui::IsMouseClicked(ImGuiMouseButton_Right) && pointValid;

    d->clicked = click;
    d->doubleClicked = dblClick;
    d->rightClicked = rClick;

    if (click) {
        Q_EMIT clicked(newHoveredPoint);
    }
    if (dblClick) {
        Q_EMIT doubleClicked(newHoveredPoint);
    }
    if (rClick) {
        Q_EMIT rightClicked(newHoveredPoint);
    }

    if (click != d->prevClicked) { Q_EMIT clickedChanged(click); d->prevClicked = click; }
    if (dblClick != d->prevDoubleClicked) { Q_EMIT doubleClickedChanged(dblClick); d->prevDoubleClicked = dblClick; }
    if (rClick != d->prevRightClicked) { Q_EMIT rightClickedChanged(rClick); d->prevRightClicked = rClick; }

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
                                              float distancePx, const ImVec2& mouseScreenPos)
{
    ImGui::Text("Mouse: (%.1f, %.1f)", mouseScreenPos.x, mouseScreenPos.y);

    if (index >= 0) {
        ImGui::Text("Point #%d: (%.3f, %.3f, %.3f)", index, x, y, z);
        ImGui::Text("Distance: %.1f px", distancePx);
    }
}

}  // namespace QIM