#include "QImPlot3DMousePickerNode.h"
#include "QImPlot3DNode.h"
#include <QColor>
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

    // Configuration
    QImPlane3D plane { QImPlane3D::XZ };
    bool maskEnabled { true };
    bool showMarker { true };
    bool showCoordinatesText { true };
    ImVec4 markerColorImV4 { 1.0f, 1.0f, 0.0f, 1.0f };
    float markerSize { 5.0f };

    // Frame state (NaN = "no value")
    static constexpr double sNaN = std::numeric_limits<double>::quiet_NaN();

    QImPlot3DRay mouseRay {{ sNaN, sNaN, sNaN }, { sNaN, sNaN, sNaN }};
    QImPlot3DPoint hoveredPoint { sNaN, sNaN, sNaN };
    bool plotHovered { false };
    bool clicked { false };
    bool doubleClicked { false };
    bool rightClicked { false };
    QImPlot3DPoint clickedPoint { sNaN, sNaN, sNaN };

    // Previous frame values for change detection
    bool prevHovered { false };
    QImPlot3DPoint prevHoveredPoint { sNaN, sNaN, sNaN };
    QImPlot3DRay prevMouseRay {{ sNaN, sNaN, sNaN }, { sNaN, sNaN, sNaN }};
    bool prevClicked { false };
    bool prevDoubleClicked { false };
    bool prevRightClicked { false };
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
 * @details Initializes the picker with default configuration:
 *          - XZ ground plane, mask enabled
 *          - Yellow marker with coordinate text display
 *          - Auto-ID disabled, always renders (RenderIgnoreVisible)
 *          - High z-order (99999) to render on top of other items
 * \endif
 *
 * \if CHINESE
 * @brief QImPlot3DMousePickerNode 构造函数
 * @param parent 父 QObject（通常是 QImPlot3DNode）
 * @details 使用默认配置初始化拾取器：
 *          - XZ 地面平面，遮罩启用
 *          - 黄色标记并显示坐标文本
 *          - 自动 ID 禁用，始终渲染（RenderIgnoreVisible）
 *          - 高 z-order（99999）以渲染在其他元素之上
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
// Configuration
//----------------------------------------------------

/**
 * \if ENGLISH
 * @brief Gets the intersection plane
 * @return Current QImPlane3D value (YZ, XZ, or XY)
 * \endif
 *
 * \if CHINESE
 * @brief 获取交点平面
 * @return 当前 QImPlane3D 值（YZ、XZ 或 XY）
 * \endif
 */
QImPlane3D QImPlot3DMousePickerNode::plane() const
{
    QIM_DC(d);
    return d->plane;
}

/**
 * \if ENGLISH
 * @brief Sets the intersection plane for ray-plane intersection
 * @param plane New plane value (QImPlane3D::YZ, XZ, or XY)
 * @details The plane determines which axis-aligned plane the mouse ray
 *          intersects with. XZ is the default "ground plane" in 3D plots.
 * \endif
 *
 * \if CHINESE
 * @brief 设置射线-平面交点的平面
 * @param plane 新的平面值（QImPlane3D::YZ、XZ 或 XY）
 * @details 平面决定鼠标射线与哪个轴对齐平面相交。XZ 是 3D 绘图中的默认"地面平面"。
 * \endif
 */
void QImPlot3DMousePickerNode::setPlane(QImPlane3D plane)
{
    QIM_D(d);
    if (d->plane != plane) {
        d->plane = plane;
        Q_EMIT planeChanged(plane);
    }
}

/**
 * \if ENGLISH
 * @brief Checks if out-of-range masking is enabled
 * @return true if masking is enabled (intersections outside axis limits return NaN)
 * \endif
 *
 * \if CHINESE
 * @brief 检查超范围遮罩是否启用
 * @return 如果遮罩启用则返回 true（超出坐标轴范围的交点返回 NaN）
 * \endif
 */
bool QImPlot3DMousePickerNode::isMaskEnabled() const
{
    QIM_DC(d);
    return d->maskEnabled;
}

/**
 * \if ENGLISH
 * @brief Sets out-of-range masking enabled state
 * @param enabled New mask state
 * \endif
 *
 * \if CHINESE
 * @brief 设置超范围遮罩启用状态
 * @param enabled 新的遮罩状态
 * \endif
 */
void QImPlot3DMousePickerNode::setMaskEnabled(bool enabled)
{
    QIM_D(d);
    if (d->maskEnabled != enabled) {
        d->maskEnabled = enabled;
        Q_EMIT maskEnabledChanged(enabled);
    }
}

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

//----------------------------------------------------
// Visual
//----------------------------------------------------

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
// Read-only state
//----------------------------------------------------

/**
 * \if ENGLISH
 * @brief Gets the hovered 3D point
 * @return Current hovered point (NaN components if no valid intersection)
 * \endif
 *
 * \if CHINESE
 * @brief 获取悬停的三维点
 * @return 当前悬停点（无有效交点时各分量为 NaN）
 * \endif
 */
QImPlot3DPoint QImPlot3DMousePickerNode::hoveredPoint() const
{
    QIM_DC(d);
    return d->hoveredPoint;
}

/**
 * \if ENGLISH
 * @brief Gets the current mouse ray
 * @return Current ray from camera through mouse pixel
 * \endif
 *
 * \if CHINESE
 * @brief 获取当前鼠标射线
 * @return 从相机穿过鼠标像素的当前射线
 * \endif
 */
QImPlot3DRay QImPlot3DMousePickerNode::mouseRay() const
{
    QIM_DC(d);
    return d->mouseRay;
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

/**
 * \if ENGLISH
 * @brief Gets the last clicked 3D point
 * @return 3D point of the most recent left-click (NaN if never clicked)
 * \endif
 *
 * \if CHINESE
 * @brief 获取最后一次点击的三维点
 * @return 最近一次左键点击的三维点（从未点击时为 NaN）
 * \endif
 */
QImPlot3DPoint QImPlot3DMousePickerNode::lastClickedPoint() const
{
    QIM_DC(d);
    return d->clickedPoint;
}

//----------------------------------------------------
// Rendering
//----------------------------------------------------

/**
 * \if ENGLISH
 * @brief Performs per-frame mouse picking and visual feedback rendering
 * @return false (no endDraw needed, no child nodes to render)
 * @details Execution flow each frame:
 *          1. Get mouse screen position from ImGui
 *          2. Convert to ray using parent plot's pixelsToPlotRay wrapper
 *          3. Convert to plane intersection point using pixelsToPlotPlane wrapper
 *          4. Detect plot hover via ImGui::IsItemHovered
 *          5. Detect click/double-click/right-click events
 *          6. Emit Qt signals for state changes and user actions
 *          7. Render marker (PlotScatter) and coordinate text (PlotText) at hovered point
 *          All conversions use QImPlot3DNode wrappers — no direct ImPlot3D internal access.
 * \endif
 *
 * \if CHINESE
 * @brief 执行每帧鼠标拾取和视觉反馈渲染
 * @return false（无需 endDraw，无子节点需渲染）
 * @details 每帧执行流程：
 *          1. 从 ImGui 获取鼠标屏幕位置
 *          2. 使用父绘图节点的 pixelsToPlotRay 封装转换为射线
 *          3. 使用 pixelsToPlotPlane 封装转换为平面交点
 *          4. 通过 ImGui::IsItemHovered 检测绘图区域悬停
 *          5. 检测点击/双击/右击事件
 *          6. 发射 Qt 信号通知状态变化和用户操作
 *          7. 在悬停点渲染标记（PlotScatter）和坐标文本（PlotText）
 *          所有转换使用 QImPlot3DNode 封装方法——无直接 ImPlot3D 内部访问。
 * \endif
 */
bool QImPlot3DMousePickerNode::beginDraw()
{
    QIM_D(d);
    QImPlot3DNode* plot = plot3DNode();
    if (!plot) return false;

    // 1. Get mouse position from ImGui (screen space)
    ImVec2 mouseScreenPos = ImGui::GetMousePos();
    QPointF mousePos(mouseScreenPos.x, mouseScreenPos.y);

    // 2. Convert to ray using parent plot's wrapper method
    QImPlot3DRay ray = plot->pixelsToPlotRay(mousePos);

    bool rayChanged = !fuzzyEqual(ray.Origin, d->prevMouseRay.Origin) ||
                      !fuzzyEqual(ray.Direction, d->prevMouseRay.Direction);
    d->mouseRay = ray;
    if (rayChanged) {
        Q_EMIT mouseRayChanged(ray);
    }
    d->prevMouseRay = ray;

    // 3. Convert to point on selected plane
    QImPlot3DPoint point = plot->pixelsToPlotPlane(mousePos, d->plane, d->maskEnabled);

    bool pointChanged = !fuzzyEqual(point, d->prevHoveredPoint);
    d->hoveredPoint = point;
    if (pointChanged) {
        Q_EMIT hoveredPointChanged(point);
    }
    d->prevHoveredPoint = point;

    bool pointValid = !point.IsNaN();

    // 4. Check if mouse is hovering over the plot area
    d->plotHovered = ImGui::IsItemHovered();
    if (d->plotHovered != d->prevHovered) {
        Q_EMIT plotHoveredChanged(d->plotHovered);
    }
    d->prevHovered = d->plotHovered;

    // 5. Detect mouse clicks (only when hovering + point is valid)
    bool click = d->plotHovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && pointValid;
    bool dblClick = d->plotHovered && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && pointValid;
    bool rClick = d->plotHovered && ImGui::IsMouseClicked(ImGuiMouseButton_Right) && pointValid;

    d->clicked = click;
    d->doubleClicked = dblClick;
    d->rightClicked = rClick;

    if (click) {
        d->clickedPoint = point;
        Q_EMIT clicked(point);
    }
    if (dblClick) {
        Q_EMIT doubleClicked(point);
    }
    if (rClick) {
        Q_EMIT rightClicked(point);
    }

    if (click != d->prevClicked) { Q_EMIT clickedChanged(click); d->prevClicked = click; }
    if (dblClick != d->prevDoubleClicked) { Q_EMIT doubleClickedChanged(dblClick); d->prevDoubleClicked = dblClick; }
    if (rClick != d->prevRightClicked) { Q_EMIT rightClickedChanged(rClick); d->prevRightClicked = rClick; }

    // 6. Render visual feedback: marker at hovered point
    if (d->showMarker && d->plotHovered && pointValid) {
        ImPlot3D::SetNextMarkerStyle(
            ImPlot3DMarker_Circle,
            d->markerSize,
            d->markerColorImV4,
            1.0f,
            d->markerColorImV4);
        ImPlot3D::PlotScatter("##PickerMarker",
            &point.x, &point.y, &point.z, 1,
            ImPlot3DScatterFlags_NoLegend | ImPlot3DScatterFlags_NoFit);
    }

    // 7. Render coordinate text next to marker
    if (d->showCoordinatesText && d->plotHovered && pointValid) {
        char buf[64];
        snprintf(buf, sizeof(buf), "(%.2f, %.2f, %.2f)",
                 point.x, point.y, point.z);
        ImPlot3D::PlotText(buf, point.x, point.y, point.z, 0.0, ImVec2(10, -10));
    }

    return false;
}

}  // namespace QIM