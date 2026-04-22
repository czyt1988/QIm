#ifndef QIMPLOT3DQUADITEMNODE_H
#define QIMPLOT3DQUADITEMNODE_H

#include "QImPlot3DItemNode.h"
#include "QImPlot3DDataSeries.h"
#include <QColor>

namespace QIM
{
/**
 * \if ENGLISH
 * @brief 3D quad plot item node
 *
 * @details Renders 3D quadrilaterals defined by sequences of X, Y, Z data points.
 * Every 4 consecutive points define a quadrilateral.
 * Supports configurable visibility for lines, fill, and markers,
 * as well as separate color and style properties for each element.
 *
 * @see QImPlot3DItemNode
 * @see QImPlot3DTriangleItemNode
 * \endif
 *
 * \if CHINESE
 * @brief 三维四边形绘图项节点
 *
 * @details 通过 X、Y、Z 数据点序列渲染三维四边形。
 * 每 4 个连续点定义一个四边形。
 * 支持线条、填充和标记点的可见性配置，
 * 以及每种元素的独立颜色和样式属性。
 *
 * @see QImPlot3DItemNode
 * @see QImPlot3DTriangleItemNode
 * \endif
 */
class QIM_CORE_API QImPlot3DQuadItemNode : public QImPlot3DItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlot3DQuadItemNode)

    // Quad visibility flags
    Q_PROPERTY(bool linesVisible READ isLinesVisible WRITE setLinesVisible NOTIFY quadFlagChanged)
    Q_PROPERTY(bool fillVisible READ isFillVisible WRITE setFillVisible NOTIFY quadFlagChanged)
    Q_PROPERTY(bool markersVisible READ isMarkersVisible WRITE setMarkersVisible NOTIFY quadFlagChanged)
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
    // Fill alpha
    Q_PROPERTY(float fillAlpha READ fillAlpha WRITE setFillAlpha NOTIFY fillAlphaChanged)

public:
    // Quad item type = InnerType3D + 6
    enum
    {
        Type = InnerType3D + 6
    };

    explicit QImPlot3DQuadItemNode(QObject* parent = nullptr);
    ~QImPlot3DQuadItemNode() override;

    int type() const override
    {
        return Type;
    }

    // setData template - creates QImVectorXYZDataSeries and delegates to non-template setData()
    template<typename ContainerX, typename ContainerY, typename ContainerZ>
    void setData(const ContainerX& x, const ContainerY& y, const ContainerZ& z)
    {
        QImAbstractXYZDataSeries* s = new QImVectorXYZDataSeries<ContainerX, ContainerY, ContainerZ>(x, y, z);
        setData(s);
        Q_EMIT dataChanged();
    }

    // Non-template setData that takes ownership of the series
    void setData(QImAbstractXYZDataSeries* series);

    // Returns the current data series pointer (does not transfer ownership)
    QImAbstractXYZDataSeries* dataSeries() const;

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

    float fillAlpha() const;
    void setFillAlpha(float alpha);

    int quadFlags() const;
    void setQuadFlags(int flags);

Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when quad data changes
     * @details Triggered when the quad XYZ data vectors are updated via setData().
     * \endif
     *
     * \if CHINESE
     * @brief 四边形数据更改时触发
     * @details 通过 setData() 更新四边形的 XYZ 数据向量时触发。
     * \endif
     */
    void dataChanged();
    /**
     * \if ENGLISH
     * @brief Emitted when any quad visibility flag changes
     * @details Triggered when linesVisible, fillVisible, or markersVisible properties change.
     * \endif
     *
     * \if CHINESE
     * @brief 任意四边形可见性标志更改时触发
     * @details 当 linesVisible、fillVisible 或 markersVisible 属性变更时触发。
     * \endif
     */
    void quadFlagChanged();
    /**
     * \if ENGLISH
     * @brief Emitted when the marker shape changes
     * @param[in] shape The new marker shape as ImPlot3DMarker enum value
     * @details Triggered by setMarkerShape() when the marker shape actually changes.
     * \endif
     *
     * \if CHINESE
     * @brief 标记形状更改时触发
     * @param[in] shape 新的标记形状（ImPlot3DMarker 枚举值）
     * @details 当标记形状实际更改时由 setMarkerShape() 触发。
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
     * @details 当 markerSize 或 markerWeight 属性变更时触发。
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
     * @details 当填充颜色值实际更改时由 setFillColor() 触发。
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
     * @details 当线条颜色值实际更改时由 setLineColor() 触发。
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
     * @details 当标记填充颜色值实际更改时由 setMarkerFillColor() 触发。
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
     * @details 当标记轮廓颜色值实际更改时由 setMarkerOutlineColor() 触发。
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
     * @details 当线宽值实际更改时由 setLineWidth() 触发。
     * \endif
     */
    void lineWidthChanged(float width);
    /**
     * \if ENGLISH
     * @brief Emitted when the fill alpha changes
     * @param[in] alpha The new fill alpha value (0.0 to 1.0, or -1.0 for auto)
     * @details Triggered by setFillAlpha() when the fill alpha value actually changes.
     * \endif
     *
     * \if CHINESE
     * @brief 填充透明度更改时触发
     * @param[in] alpha 新的填充透明度值（0.0到1.0，或-1.0表示自动）
     * @details 当填充透明度值实际更改时由 setFillAlpha() 触发。
     * \endif
     */
    void fillAlphaChanged(float alpha);

protected:
    bool beginDraw() override;
};
}  // namespace QIM

#endif  // QIMPLOT3DQUADITEMNODE_H
