#ifndef QIMPLOT3DSURFACEITEMNODE_H
#define QIMPLOT3DSURFACEITEMNODE_H

#include "QImPlot3DItemNode.h"
#include "QImPlot3DDataSeries.h"
#include <QColor>

namespace QIM
{
/**
 * \if ENGLISH
 * @brief 3D surface plot item node
 *
 * @class QImPlot3DSurfaceItemNode
 * @ingroup plot3d
 *
 * @details Renders a 3D surface defined by a grid of X, Y, Z data points.
 * Supports configurable visibility for lines, fill, and markers,
 * optional colormap-based coloring, and independent style properties
 * for each visual element.
 *
 * @see QImPlot3DItemNode
 * @see QImPlot3DMeshItemNode
 * \endif
 *
 * \if CHINESE
 * @brief 三维曲面绘图项节点
 *
 * @class QImPlot3DSurfaceItemNode
 * @ingroup plot3d
 *
 * @details 通过X、Y、Z数据点网格渲染三维曲面。
 * 支持线条、填充和标记点的可见性配置，
 * 可选颜色映射着色，以及每种视觉元素的独立样式属性。
 *
 * @see QImPlot3DItemNode
 * @see QImPlot3DMeshItemNode
 * \endif
 */
class QIM_CORE_API QImPlot3DSurfaceItemNode : public QImPlot3DItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlot3DSurfaceItemNode)

    // Surface grid dimensions
    Q_PROPERTY(int xCount READ xCount WRITE setXCount NOTIFY gridShapeChanged)
    Q_PROPERTY(int yCount READ yCount WRITE setYCount NOTIFY gridShapeChanged)
    // Surface visibility flags
    Q_PROPERTY(bool linesVisible READ isLinesVisible WRITE setLinesVisible NOTIFY surfaceFlagChanged)
    Q_PROPERTY(bool fillVisible READ isFillVisible WRITE setFillVisible NOTIFY surfaceFlagChanged)
    Q_PROPERTY(bool markersVisible READ isMarkersVisible WRITE setMarkersVisible NOTIFY surfaceFlagChanged)
    // Marker style
    Q_PROPERTY(int markerShape READ markerShape WRITE setMarkerShape NOTIFY markerShapeChanged)
    Q_PROPERTY(float markerSize READ markerSize WRITE setMarkerSize NOTIFY markerStyleChanged)
    Q_PROPERTY(float markerWeight READ markerWeight WRITE setMarkerWeight NOTIFY markerStyleChanged)
    // Colors
    Q_PROPERTY(QColor fillColor READ fillColor WRITE setFillColor NOTIFY fillColorChanged)
    Q_PROPERTY(QColor lineColor READ lineColor WRITE setLineColor NOTIFY lineColorChanged)
    Q_PROPERTY(QColor markerFillColor READ markerFillColor WRITE setMarkerFillColor NOTIFY markerFillColorChanged)
    Q_PROPERTY(QColor markerOutlineColor READ markerOutlineColor WRITE setMarkerOutlineColor NOTIFY markerOutlineColorChanged)
    // Line width
    Q_PROPERTY(float lineWidth READ lineWidth WRITE setLineWidth NOTIFY lineWidthChanged)
    // Colormap
    Q_PROPERTY(bool colormapEnabled READ isColormapEnabled WRITE setColormapEnabled NOTIFY colormapChanged)
    Q_PROPERTY(int colormap READ colormap WRITE setColormap NOTIFY colormapChanged)

public:
    // Surface item type = InnerType3D + 3
    enum
    {
        Type = InnerType3D + 3
    };

    explicit QImPlot3DSurfaceItemNode(QObject* parent = nullptr);
    ~QImPlot3DSurfaceItemNode() override;

    int type() const override
    {
        return Type;
    }

    // setData template - creates QImVectorXYZDataSeries and delegates to non-template setData()
    template<typename ContainerX, typename ContainerY, typename ContainerZ>
    void setData(const ContainerX& x, const ContainerY& y, const ContainerZ& z, int xCount, int yCount)
    {
        QImAbstractXYZDataSeries* s = new QImVectorXYZDataSeries<ContainerX, ContainerY, ContainerZ>(x, y, z);
        setData(s, xCount, yCount);
        Q_EMIT dataChanged();
        Q_EMIT gridShapeChanged();
    }

    // Non-template setData that takes ownership of the series
    void setData(QImAbstractXYZDataSeries* series, int xCount, int yCount);

    // Returns the current data series pointer (does not transfer ownership)
    QImAbstractXYZDataSeries* dataSeries() const;

    int xCount() const;
    void setXCount(int count);

    int yCount() const;
    void setYCount(int count);

    bool isLinesVisible() const;
    void setLinesVisible(bool visible);

    bool isFillVisible() const;
    void setFillVisible(bool visible);

    bool isMarkersVisible() const;
    void setMarkersVisible(bool visible);

    int markerShape() const;
    void setMarkerShape(int shape);

    float markerSize() const;
    void setMarkerSize(float size);

    float markerWeight() const;
    void setMarkerWeight(float weight);

    QColor fillColor() const;
    void setFillColor(const QColor& color);

    QColor lineColor() const;
    void setLineColor(const QColor& color);

    QColor markerFillColor() const;
    void setMarkerFillColor(const QColor& color);

    QColor markerOutlineColor() const;
    void setMarkerOutlineColor(const QColor& color);

    float lineWidth() const;
    void setLineWidth(float width);

    bool isColormapEnabled() const;
    void setColormapEnabled(bool enabled);

    int colormap() const;
    void setColormap(int colormap);

    int surfaceFlags() const;
    void setSurfaceFlags(int flags);

Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when surface data changes
     * @details Triggered when the surface XYZ data vectors are updated via setData().
     * \endif
     *
     * \if CHINESE
     * @brief 曲面数据更改时触发
     * @details 通过setData()更新曲面的XYZ数据向量时触发。
     * \endif
     */
    void dataChanged();
    /**
     * \if ENGLISH
     * @brief Emitted when the grid shape changes
     * @details Triggered when xCount or yCount properties change, affecting the surface grid dimensions.
     * \endif
     *
     * \if CHINESE
     * @brief 网格形状更改时触发
     * @details 当xCount或yCount属性变更时触发，影响曲面网格维度。
     * \endif
     */
    void gridShapeChanged();
    /**
     * \if ENGLISH
     * @brief Emitted when any surface visibility flag changes
     * @details Triggered when linesVisible, fillVisible, or markersVisible properties change.
     * \endif
     *
     * \if CHINESE
     * @brief 任意曲面可见性标志更改时触发
     * @details 当linesVisible、fillVisible或markersVisible属性变更时触发。
     * \endif
     */
    void surfaceFlagChanged();
    /**
     * \if ENGLISH
     * @brief Emitted when the marker shape changes
     * @param[in] shape The new marker shape as ImPlot3DMarker enum value
     * @details Triggered by setMarkerShape() when the marker shape actually changes.
     * \endif
     *
     * \if CHINESE
     * @brief 标记形状更改时触发
     * @param[in] shape 新的标记形状（ImPlot3DMarker枚举值）
     * @details 当标记形状实际更改时由setMarkerShape()触发。
     * \endif
     */
    void markerShapeChanged(int shape);
    /**
     * \if ENGLISH
     * @brief Emitted when marker style properties change
     * @details Triggered when markerSize or markerWeight properties change.
     * \endif
     *
     * \if CHINESE
     * @brief 标记样式属性更改时触发
     * @details 当markerSize或markerWeight属性变更时触发。
     * \endif
     */
    void markerStyleChanged();
    /**
     * \if ENGLISH
     * @brief Emitted when the fill color changes
     * @param[in] color The new fill color
     * @details Triggered by setFillColor() when the fill color value actually changes.
     * \endif
     *
     * \if CHINESE
     * @brief 填充颜色更改时触发
     * @param[in] color 新的填充颜色
     * @details 当填充颜色值实际更改时由setFillColor()触发。
     * \endif
     */
    void fillColorChanged(const QColor& color);
    /**
     * \if ENGLISH
     * @brief Emitted when the line color changes
     * @param[in] color The new line color
     * @details Triggered by setLineColor() when the line color value actually changes.
     * \endif
     *
     * \if CHINESE
     * @brief 线条颜色更改时触发
     * @param[in] color 新的线条颜色
     * @details 当线条颜色值实际更改时由setLineColor()触发。
     * \endif
     */
    void lineColorChanged(const QColor& color);
    /**
     * \if ENGLISH
     * @brief Emitted when the marker fill color changes
     * @param[in] color The new marker fill color
     * @details Triggered by setMarkerFillColor() when the marker fill color value actually changes.
     * \endif
     *
     * \if CHINESE
     * @brief 标记填充颜色更改时触发
     * @param[in] color 新的标记填充颜色
     * @details 当标记填充颜色值实际更改时由setMarkerFillColor()触发。
     * \endif
     */
    void markerFillColorChanged(const QColor& color);
    /**
     * \if ENGLISH
     * @brief Emitted when the marker outline color changes
     * @param[in] color The new marker outline color
     * @details Triggered by setMarkerOutlineColor() when the marker outline color value actually changes.
     * \endif
     *
     * \if CHINESE
     * @brief 标记轮廓颜色更改时触发
     * @param[in] color 新的标记轮廓颜色
     * @details 当标记轮廓颜色值实际更改时由setMarkerOutlineColor()触发。
     * \endif
     */
    void markerOutlineColorChanged(const QColor& color);
    /**
     * \if ENGLISH
     * @brief Emitted when the line width changes
     * @param[in] width The new line width in pixels
     * @details Triggered by setLineWidth() when the line width value actually changes.
     * \endif
     *
     * \if CHINESE
     * @brief 线宽更改时触发
     * @param[in] width 新的线宽（像素）
     * @details 当线宽值实际更改时由setLineWidth()触发。
     * \endif
     */
    void lineWidthChanged(float width);
    /**
     * \if ENGLISH
     * @brief Emitted when colormap properties change
     * @details Triggered when colormapEnabled or colormap properties change.
     * \endif
     *
     * \if CHINESE
     * @brief 颜色映射属性更改时触发
     * @details 当colormapEnabled或colormap属性变更时触发。
     * \endif
     */
    void colormapChanged();

protected:
    bool beginDraw() override;

private:
    void regenerateEdgeLabels();
};
}  // namespace QIM

#endif  // QIMPLOT3DSURFACEITEMNODE_H
