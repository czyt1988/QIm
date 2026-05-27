#ifndef QIMPLOT3DMOUSEPICKERNODE_H
#define QIMPLOT3DMOUSEPICKERNODE_H

#include "../../QImAPI.h"
#include "QImPlot3DItemNode.h"
#include "QImPlot3DTypes.h"
#include "QImPlot3D.h"
#include "QImPlot3DDataSeries.h"

namespace QIM {

class QImPlot3DNode;

/**
 * \if ENGLISH
 * @brief 3D mouse picker node that finds the closest data point to the mouse in screen space
 * @details Uses ImPlot3D::PlotToPixels to project data series points to pixels, then finds the
 *          nearest point and renders a helper line, marker, and coordinate text.
 *          Inspired by DemoCustomOverlay in implot3d_demo.cpp.
 * @note Requires setData() to provide the data series to pick from.
 * @see QImPlot3DNode, QImPlot3DItemNode, QImAbstractXYZDataSeries
 * \endif
 *
 * \if CHINESE
 * @brief 3D 鼠标拾取节点，在屏幕空间中查找离鼠标最近的数据点
 * @details 使用 ImPlot3D::PlotToPixels 将数据系列点投影到像素空间，然后找到最近点并
 *          渲染辅助线、标记和坐标文本。参考 implot3d_demo.cpp 中的 DemoCustomOverlay。
 * @note 需要通过 setData() 提供数据系列以供拾取。
 * @see QImPlot3DNode, QImPlot3DItemNode, QImAbstractXYZDataSeries
 * \endif
 */
class QIM_CORE_API QImPlot3DMousePickerNode : public QImPlot3DItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlot3DMousePickerNode)
    Q_DISABLE_COPY(QImPlot3DMousePickerNode)

    Q_PROPERTY(bool showMarker READ isShowMarker WRITE setShowMarker NOTIFY showMarkerChanged)
    Q_PROPERTY(bool showCoordinatesText READ isShowCoordinatesText WRITE setShowCoordinatesText NOTIFY showCoordinatesTextChanged)
    Q_PROPERTY(QColor markerColor READ markerColor WRITE setMarkerColor NOTIFY markerColorChanged)
    Q_PROPERTY(float markerSize READ markerSize WRITE setMarkerSize NOTIFY markerSizeChanged)
    Q_PROPERTY(QColor helperLineColor READ helperLineColor WRITE setHelperLineColor NOTIFY helperLineColorChanged)
    Q_PROPERTY(float helperLineWidth READ helperLineWidth WRITE setHelperLineWidth NOTIFY helperLineWidthChanged)

    Q_PROPERTY(QImPlot3DPoint hoveredPoint READ hoveredPoint NOTIFY hoveredPointChanged)
    Q_PROPERTY(int hoveredIndex READ hoveredIndex NOTIFY hoveredIndexChanged)
    Q_PROPERTY(bool plotHovered READ isPlotHovered NOTIFY plotHoveredChanged)
    Q_PROPERTY(bool clicked READ isClicked NOTIFY clickedChanged)
    Q_PROPERTY(bool doubleClicked READ isDoubleClicked NOTIFY doubleClickedChanged)
    Q_PROPERTY(bool rightClicked READ isRightClicked NOTIFY rightClickedChanged)

public:
    enum { Type = InnerType3D + 50 };
    int type() const override { return Type; }

    explicit QImPlot3DMousePickerNode(QObject* parent = nullptr);
    ~QImPlot3DMousePickerNode() override;

    // Data
    void setData(QImAbstractXYZDataSeries* series);
    QImAbstractXYZDataSeries* data() const;

    // Visual
    bool isShowMarker() const;
    void setShowMarker(bool show);
    bool isShowCoordinatesText() const;
    void setShowCoordinatesText(bool show);
    QColor markerColor() const;
    void setMarkerColor(const QColor& color);
    float markerSize() const;
    void setMarkerSize(float size);

    // Helper line
    QColor helperLineColor() const;
    void setHelperLineColor(const QColor& color);
    float helperLineWidth() const;
    void setHelperLineWidth(float width);

    // Read-only state (cached per-frame)
    QImPlot3DPoint hoveredPoint() const;
    int hoveredIndex() const;
    bool isPlotHovered() const;
    bool isClicked() const;
    bool isDoubleClicked() const;
    bool isRightClicked() const;

    // Convenience
    bool hasValidHoveredPoint() const;

Q_SIGNALS:
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
     * @brief Emitted when helper line color changes
     * @param color New helper line color
     * \endif
     *
     * \if CHINESE
     * @brief 辅助线颜色改变时发射
     * @param color 新的辅助线颜色
     * \endif
     */
    void helperLineColorChanged(const QColor& color);

    /**
     * \if ENGLISH
     * @brief Emitted when helper line width changes
     * @param width New helper line width in pixels
     * \endif
     *
     * \if CHINESE
     * @brief 辅助线宽度改变时发射
     * @param width 新的辅助线宽度（像素）
     * \endif
     */
    void helperLineWidthChanged(float width);

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
     * @brief Emitted when hovered data index changes
     * @param index Index of the closest data point (-1 if none)
     * \endif
     *
     * \if CHINESE
     * @brief 悬停数据索引改变时发射
     * @param index 最近数据点的索引（无时为 -1）
     * \endif
     */
    void hoveredIndexChanged(int index);

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
    /**
     * \if ENGLISH
     * @brief Override to customize the tooltip content when a data point is hovered
     * @param index Data point index in the data series (-1 if none)
     * @param x X coordinate of the closest point (NaN if no valid point)
     * @param y Y coordinate of the closest point
     * @param z Z coordinate of the closest point
     * @param distancePx Screen-space distance from mouse to closest point (pixels)
     * @param mouseScreenPos Mouse position in screen coordinates
     * @details Called inside ImGui::BeginTooltip()/EndTooltip() scope when the mouse
     *          hovers over the plot area. The default implementation shows mouse position,
     *          point index, coordinates, and distance.
     *          Override this to customize the tooltip — call ImGui::Text() etc. directly.
     *
     * Example custom override:
     * @code
     * void MyPicker::renderTooltip(int idx, double x, double y, double z, float d, const ImVec2& mp) override {
     *     ImGui::Text("Point #%d", idx);
     *     ImGui::Text("Value: (%.1f, %.1f, %.1f)", x, y, z);
     * }
     * @endcode
     * \endif
     *
     * \if CHINESE
     * @brief 重写此方法以自定义数据点悬停时的 tooltip 内容
     * @param index 数据系列中的数据点索引（无有效点时为 -1）
     * @param x 最近点的 X 坐标（无有效点时为 NaN）
     * @param y 最近点的 Y 坐标
     * @param z 最近点的 Z 坐标
     * @param distancePx 鼠标到最近点的屏幕空间距离（像素）
     * @param mouseScreenPos 鼠标屏幕坐标
     * @details 当鼠标悬停在绘图区域时，在 ImGui::BeginTooltip()/EndTooltip() 作用域内调用。
     *          默认实现显示鼠标位置、点索引、坐标和距离。
     *          重写此方法以自定义 tooltip——直接调用 ImGui::Text() 等。
     *
     * 自定义重写示例：
     * @code
     * void MyPicker::renderTooltip(int idx, double x, double y, double z, float d, const ImVec2& mp) override {
     *     ImGui::Text("Point #%d", idx);
     *     ImGui::Text("Value: (%.1f, %.1f, %.1f)", x, y, z);
     * }
     * @endcode
     * \endif
     */
    virtual void renderTooltip(int index, double x, double y, double z,
                               float distancePx, const ImVec2& mouseScreenPos);

    bool beginDraw() override;
};

}  // namespace QIM

#endif  // QIMPLOT3DMOUSEPICKERNODE_H