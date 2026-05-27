#ifndef QIMPLOT3DMOUSEPICKERNODE_H
#define QIMPLOT3DMOUSEPICKERNODE_H

#include "../../QImAPI.h"
#include "QImPlot3DItemNode.h"
#include "QImPlot3DTypes.h"
#include "QImPlot3D.h"

namespace QIM {

class QImPlot3DNode;

/**
 * \if ENGLISH
 * @brief 3D mouse picker node that wraps ImPlot3D raycasting APIs with visual feedback
 * @details Provides real-time mouse interaction with 3D plots by converting mouse
 *          screen coordinates to plot-space rays and plane intersection points.
 *          Emits Qt signals for hover, click, double-click, and right-click events
 *          with 3D coordinates. Renders a visual marker and coordinate text at
 *          the hovered position. Uses QImPlot3DNode wrapper methods (pixelsToPlotRay,
 *          pixelsToPlotPlane) instead of direct ImPlot3D internal access.
 * @note This is an interaction tool node (like 2D DragPoint/Annotation), not a data
 *       visualization node. It always renders regardless of visible state
 *       (RenderIgnoreVisible=true) and does not participate in axis fitting or legend.
 * @see QImPlot3DNode, QImPlot3DItemNode, QImPlane3D
 * \endif
 *
 * \if CHINESE
 * @brief 3D 鼠标拾取节点，封装 ImPlot3D 射线检测 API 并提供视觉反馈
 * @details 通过将鼠标屏幕坐标转换为绘图空间的射线和平面交点，实现与 3D 绘图的实时鼠标交互。
 *          发射 Qt 信号通知悬停、单击、双击和右击事件，附带 3D 坐标。
 *          在悬停位置渲染可视化标记和坐标文本。使用 QImPlot3DNode 封装方法
 *          （pixelsToPlotRay、pixelsToPlotPlane），而非直接访问 ImPlot3D 内部。
 * @note 这是交互工具节点（类似 2D 的 DragPoint/Annotation），不是数据可视化节点。
 *       它始终渲染（RenderIgnoreVisible=true），不参与坐标轴拟合或图例。
 * @see QImPlot3DNode, QImPlot3DItemNode, QImPlane3D
 * \endif
 */
class QIM_CORE_API QImPlot3DMousePickerNode : public QImPlot3DItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlot3DMousePickerNode)
    Q_DISABLE_COPY(QImPlot3DMousePickerNode)

    Q_PROPERTY(QImPlane3D plane READ plane WRITE setPlane NOTIFY planeChanged)
    Q_PROPERTY(bool maskEnabled READ isMaskEnabled WRITE setMaskEnabled NOTIFY maskEnabledChanged)
    Q_PROPERTY(bool showMarker READ isShowMarker WRITE setShowMarker NOTIFY showMarkerChanged)
    Q_PROPERTY(bool showCoordinatesText READ isShowCoordinatesText WRITE setShowCoordinatesText NOTIFY showCoordinatesTextChanged)
    Q_PROPERTY(QColor markerColor READ markerColor WRITE setMarkerColor NOTIFY markerColorChanged)
    Q_PROPERTY(float markerSize READ markerSize WRITE setMarkerSize NOTIFY markerSizeChanged)

    Q_PROPERTY(QImPlot3DPoint hoveredPoint READ hoveredPoint NOTIFY hoveredPointChanged)
    Q_PROPERTY(QImPlot3DRay mouseRay READ mouseRay NOTIFY mouseRayChanged)
    Q_PROPERTY(bool plotHovered READ isPlotHovered NOTIFY plotHoveredChanged)
    Q_PROPERTY(bool clicked READ isClicked NOTIFY clickedChanged)
    Q_PROPERTY(bool doubleClicked READ isDoubleClicked NOTIFY doubleClickedChanged)
    Q_PROPERTY(bool rightClicked READ isRightClicked NOTIFY rightClickedChanged)

public:
    enum { Type = InnerType3D + 50 };
    int type() const override { return Type; }

    explicit QImPlot3DMousePickerNode(QObject* parent = nullptr);
    ~QImPlot3DMousePickerNode() override;

    // Configuration
    QImPlane3D plane() const;
    void setPlane(QImPlane3D plane);
    bool isMaskEnabled() const;
    void setMaskEnabled(bool enabled);
    bool isShowMarker() const;
    void setShowMarker(bool show);
    bool isShowCoordinatesText() const;
    void setShowCoordinatesText(bool show);

    // Visual
    QColor markerColor() const;
    void setMarkerColor(const QColor& color);
    float markerSize() const;
    void setMarkerSize(float size);

    // Read-only state (cached per-frame)
    QImPlot3DPoint hoveredPoint() const;
    QImPlot3DRay mouseRay() const;
    bool isPlotHovered() const;
    bool isClicked() const;
    bool isDoubleClicked() const;
    bool isRightClicked() const;

    // Convenience
    bool hasValidHoveredPoint() const;
    QImPlot3DPoint lastClickedPoint() const;

Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when the intersection plane changes
     * @param plane New plane value (QImPlane3D::YZ, XZ, or XY)
     * \endif
     *
     * \if CHINESE
     * @brief 交点平面改变时发射
     * @param plane 新的平面值（QImPlane3D::YZ、XZ 或 XY）
     * \endif
     */
    void planeChanged(QImPlane3D plane);

    /**
     * \if ENGLISH
     * @brief Emitted when mask enabled state changes
     * @param enabled New mask state (true masks out-of-range values)
     * \endif
     *
     * \if CHINESE
     * @brief 遮罩启用状态改变时发射
     * @param enabled 新的遮罩状态（true 遮蔽超范围值）
     * \endif
     */
    void maskEnabledChanged(bool enabled);

    /**
     * \if ENGLISH
     * @brief Emitted when show marker state changes
     * @param show New marker visibility state
     * \endif
     *
     * \if CHINESE
     * @brief 标记可见状态改变时发射
     * @param show 新的标记可见状态
     * \endif
     */
    void showMarkerChanged(bool show);

    /**
     * \if ENGLISH
     * @brief Emitted when show coordinates text state changes
     * @param show New coordinates text visibility state
     * \endif
     *
     * \if CHINESE
     * @brief 坐标文本可见状态改变时发射
     * @param show 新的坐标文本可见状态
     * \endif
     */
    void showCoordinatesTextChanged(bool show);

    /**
     * \if ENGLISH
     * @brief Emitted when marker color changes
     * @param color New marker color
     * \endif
     *
     * \if CHINESE
     * @brief 标记颜色改变时发射
     * @param color 新的标记颜色
     * \endif
     */
    void markerColorChanged(const QColor& color);

    /**
     * \if ENGLISH
     * @brief Emitted when marker size changes
     * @param size New marker size in pixels
     * \endif
     *
     * \if CHINESE
     * @brief 标记大小改变时发射
     * @param size 新的标记大小（像素）
     * \endif
     */
    void markerSizeChanged(float size);

    /**
     * \if ENGLISH
     * @brief Emitted when hovered 3D point changes (each frame when hover state changes)
     * @param point New hovered 3D point (NaN if no valid intersection)
     * \endif
     *
     * \if CHINESE
     * @brief 悬停三维点改变时发射（每帧悬停状态变化时）
     * @param point 新的悬停三维点（无有效交点时为 NaN）
     * \endif
     */
    void hoveredPointChanged(const QImPlot3DPoint& point);

    /**
     * \if ENGLISH
     * @brief Emitted when mouse ray changes (each frame when mouse moves over plot)
     * @param ray New mouse ray from camera through mouse pixel
     * \endif
     *
     * \if CHINESE
     * @brief 鼠标射线改变时发射（每帧鼠标在绘图区域移动时）
     * @param ray 新的从相机穿过鼠标像素的射线
     * \endif
     */
    void mouseRayChanged(const QImPlot3DRay& ray);

    /**
     * \if ENGLISH
     * @brief Emitted when plot hover state changes
     * @param hovered New hover state (true when mouse is over the plot area)
     * \endif
     *
     * \if CHINESE
     * @brief 绘图区域悬停状态改变时发射
     * @param hovered 新的悬停状态（鼠标在绘图区域上方时为 true）
     * \endif
     */
    void plotHoveredChanged(bool hovered);

    /**
     * \if ENGLISH
     * @brief Emitted when left mouse button is clicked on a valid 3D point
     * @param point 3D point where the click occurred
     * \endif
     *
     * \if CHINESE
     * @brief 在有效三维点上左键点击时发射
     * @param point 点击发生的三维点
     * \endif
     */
    void clicked(const QImPlot3DPoint& point);

    /**
     * \if ENGLISH
     * @brief Emitted when left mouse button is double-clicked on a valid 3D point
     * @param point 3D point where the double-click occurred
     * \endif
     *
     * \if CHINESE
     * @brief 在有效三维点上左键双击时发射
     * @param point 双击发生的三维点
     * \endif
     */
    void doubleClicked(const QImPlot3DPoint& point);

    /**
     * \if ENGLISH
     * @brief Emitted when right mouse button is clicked on a valid 3D point
     * @param point 3D point where the right-click occurred
     * \endif
     *
     * \if CHINESE
     * @brief 在有效三维点上右键点击时发射
     * @param point 右击发生的三维点
     * \endif
     */
    void rightClicked(const QImPlot3DPoint& point);

    /**
     * \if ENGLISH
     * @brief Emitted when clicked state changes
     * @param clicked New clicked state (true on click frame, false otherwise)
     * \endif
     *
     * \if CHINESE
     * @brief 点击状态改变时发射
     * @param clicked 新的点击状态（点击帧为 true，否则为 false）
     * \endif
     */
    void clickedChanged(bool clicked);

    /**
     * \if ENGLISH
     * @brief Emitted when double-clicked state changes
     * @param doubleClicked New double-clicked state
     * \endif
     *
     * \if CHINESE
     * @brief 双击状态改变时发射
     * @param doubleClicked 新的双击状态
     * \endif
     */
    void doubleClickedChanged(bool doubleClicked);

    /**
     * \if ENGLISH
     * @brief Emitted when right-clicked state changes
     * @param rightClicked New right-clicked state
     * \endif
     *
     * \if CHINESE
     * @brief 右击状态改变时发射
     * @param rightClicked 新的右击状态
     * \endif
     */
    void rightClickedChanged(bool rightClicked);

protected:
    bool beginDraw() override;
};

}  // namespace QIM

#endif  // QIMPLOT3DMOUSEPICKERNODE_H