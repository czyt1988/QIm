#ifndef QIMPLOT3DMOUSEPICKERNODE_H
#define QIMPLOT3DMOUSEPICKERNODE_H

#include "QImAPI.h"
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
 *       Signals are kept minimal (picked + doubleClicked) to avoid per-frame overhead.
 * @see QImPlot3DNode, QImPlot3DItemNode, QImAbstractXYZDataSeries
 * \endif
 *
 * \if CHINESE
 * @brief 3D 鼠标拾取节点，在屏幕空间中查找离鼠标最近的数据点
 * @details 使用 ImPlot3D::PlotToPixels 将数据系列点投影到像素空间，然后找到最近点并
 *          渲染辅助线、标记和坐标文本。参考 implot3d_demo.cpp 中的 DemoCustomOverlay。
 * @note 需要通过 setData() 提供数据系列以供拾取。
 *       信号保持最小（picked + doubleClicked）以避免每帧开销。
 * @see QImPlot3DNode, QImPlot3DItemNode, QImAbstractXYZDataSeries
 * \endif
 */
class QIM_CORE_API QImPlot3DMousePickerNode : public QImPlot3DItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlot3DMousePickerNode)
    Q_DISABLE_COPY(QImPlot3DMousePickerNode)

    Q_PROPERTY(bool showMarker READ isShowMarker WRITE setShowMarker)
    Q_PROPERTY(bool showCoordinatesText READ isShowCoordinatesText WRITE setShowCoordinatesText)
    Q_PROPERTY(QColor markerColor READ markerColor WRITE setMarkerColor)
    Q_PROPERTY(float markerSize READ markerSize WRITE setMarkerSize)
    Q_PROPERTY(QColor helperLineColor READ helperLineColor WRITE setHelperLineColor)
    Q_PROPERTY(float helperLineWidth READ helperLineWidth WRITE setHelperLineWidth)

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

    // Read-only state (valid during beginDraw scope)
    QImPlot3DPoint hoveredPoint() const;
    int hoveredIndex() const;
    bool isPlotHovered() const;
    bool hasValidHoveredPoint() const;

Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when a data point is clicked (single click)
     * @param button Mouse button that triggered the click
     * @param point 3D coordinates of the clicked data point
     * \endif
     *
     * \if CHINESE
     * @brief 数据点被单击时发射
     * @param button 触发点击的鼠标按钮
     * @param point 被点击数据点的三维坐标
     * \endif
     */
    void picked(Qt::MouseButton button, const QImPlot3DPoint& point);

    /**
     * \if ENGLISH
     * @brief Emitted when a data point is double-clicked
     * @param button Mouse button that triggered the double-click
     * @param point 3D coordinates of the double-clicked data point
     * \endif
     *
     * \if CHINESE
     * @brief 数据点被双击时发射
     * @param button 触发双击的鼠标按钮
     * @param point 被双击数据点的三维坐标
     * \endif
     */
    void doubleClicked(Qt::MouseButton button, const QImPlot3DPoint& point);

protected:
    virtual void renderTooltip(int index, double x, double y, double z,
                               float distancePx, float mouseScreenPosX,float mouseScreenPosY);

    bool beginDraw() override;
};

}  // namespace QIM

#endif  // QIMPLOT3DMOUSEPICKERNODE_H
