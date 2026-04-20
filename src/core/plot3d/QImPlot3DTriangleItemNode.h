#ifndef QIMPLOT3DTRIANGLEITEMNODE_H
#define QIMPLOT3DTRIANGLEITEMNODE_H

#include "QImPlot3DItemNode.h"
#include "QImPlot3DDataSeries.h"
#include <QColor>

namespace QIM
{
/**
 * \if ENGLISH
 * @brief 3D triangle plot item node
 *
 * @class QImPlot3DTriangleItemNode
 * @ingroup plot3d
 *
 * @details Renders 3D triangles defined by sequences of X, Y, Z data points.
 * Supports configurable visibility for lines, fill, and markers,
 * as well as separate color and style properties for each element.
 *
 * @see QImPlot3DItemNode
 * @see QImPlot3DSurfaceItemNode
 * \endif
 *
 * \if CHINESE
 * @brief 三维三角形绘图项节点
 *
 * @class QImPlot3DTriangleItemNode
 * @ingroup plot3d
 *
 * @details 通过X、Y、Z数据点序列渲染三维三角形。
 * 支持线条、填充和标记点的可见性配置，
 * 以及每种元素的独立颜色和样式属性。
 *
 * @see QImPlot3DItemNode
 * @see QImPlot3DSurfaceItemNode
 * \endif
 */
class QIM_CORE_API QImPlot3DTriangleItemNode : public QImPlot3DItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlot3DTriangleItemNode)

    // Triangle visibility flags
    Q_PROPERTY(bool linesVisible READ isLinesVisible WRITE setLinesVisible NOTIFY triangleFlagChanged)
    Q_PROPERTY(bool fillVisible READ isFillVisible WRITE setFillVisible NOTIFY triangleFlagChanged)
    Q_PROPERTY(bool markersVisible READ isMarkersVisible WRITE setMarkersVisible NOTIFY triangleFlagChanged)
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

public:
    // Triangle item type = InnerType3D + 5
    enum
    {
        Type = InnerType3D + 5
    };

    explicit QImPlot3DTriangleItemNode(QObject* parent = nullptr);
    ~QImPlot3DTriangleItemNode() override;

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

    int triangleFlags() const;
    void setTriangleFlags(int flags);

Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when triangle data changes
     * @details Triggered when the triangle XYZ data vectors are updated via setData().
     * \endif
     *
     * \if CHINESE
     * @brief 三角形数据更改时触发
     * @details 通过setData()更新三角形的XYZ数据向量时触发。
     * \endif
     */
    void dataChanged();
    /**
     * \if ENGLISH
     * @brief Emitted when any triangle visibility flag changes
     * @details Triggered when linesVisible, fillVisible, or markersVisible properties change.
     * \endif
     *
     * \if CHINESE
     * @brief 任意三角形可见性标志更改时触发
     * @details 当linesVisible、fillVisible或markersVisible属性变更时触发。
     * \endif
     */
    void triangleFlagChanged();
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

protected:
    bool beginDraw() override;
};
}  // namespace QIM

#endif  // QIMPLOT3DTRIANGLEITEMNODE_H
