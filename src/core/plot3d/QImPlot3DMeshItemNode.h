#ifndef QIMPLOT3DMESHITEMNODE_H
#define QIMPLOT3DMESHITEMNODE_H

#include "QImPlot3DItemNode.h"
#include "implot3d.h"
#include <QColor>
#include <vector>

namespace QIM
{
/**
 * \if ENGLISH
 * @brief 3D mesh plot item node
 *
 * @class QImPlot3DMeshItemNode
 * @ingroup plot3d
 *
 * @details Renders a 3D mesh defined by vertices and triangle indices.
 * Supports configurable visibility for lines, fill, and markers,
 * as well as separate color and style properties for each element.
 *
 * @see QImPlot3DItemNode
 * @see QImPlot3DSurfaceItemNode
 * \endif
 *
 * \if CHINESE
 * @brief 三维网格绘图项节点
 *
 * @class QImPlot3DMeshItemNode
 * @ingroup plot3d
 *
 * @details 通过顶点和三角面索引渲染三维网格。
 * 支持线条、填充和标记点的可见性配置，
 * 以及每种元素的独立颜色和样式属性。
 *
 * @see QImPlot3DItemNode
 * @see QImPlot3DSurfaceItemNode
 * \endif
 */
class QIM_CORE_API QImPlot3DMeshItemNode : public QImPlot3DItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlot3DMeshItemNode)

    // Mesh visibility flags
    Q_PROPERTY(bool linesVisible READ isLinesVisible WRITE setLinesVisible NOTIFY meshFlagChanged)
    Q_PROPERTY(bool fillVisible READ isFillVisible WRITE setFillVisible NOTIFY meshFlagChanged)
    Q_PROPERTY(bool markersVisible READ isMarkersVisible WRITE setMarkersVisible NOTIFY meshFlagChanged)
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
    // Mesh item type = InnerType3D + 4
    enum
    {
        Type = InnerType3D + 4
    };

    explicit QImPlot3DMeshItemNode(QObject* parent = nullptr);
    ~QImPlot3DMeshItemNode() override;

    int type() const override
    {
        return Type;
    }

    void setMeshData(const std::vector< ImPlot3DPoint >& vertices, const std::vector< unsigned int >& indices);

    const std::vector< ImPlot3DPoint >& vertices() const;
    const std::vector< unsigned int >& indices() const;

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

    int meshFlags() const;
    void setMeshFlags(int flags);

Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when mesh data changes
     * @details Triggered when the mesh vertices or indices are updated via setMeshData().
     * \endif
     *
     * \if CHINESE
     * @brief 网格数据更改时触发
     * @details 通过setMeshData()更新网格顶点或索引时触发。
     * \endif
     */
    void dataChanged();
    /**
     * \if ENGLISH
     * @brief Emitted when any mesh visibility flag changes
     * @details Triggered when linesVisible, fillVisible, or markersVisible properties change.
     * \endif
     *
     * \if CHINESE
     * @brief 任意网格可见性标志更改时触发
     * @details 当linesVisible、fillVisible或markersVisible属性变更时触发。
     * \endif
     */
    void meshFlagChanged();
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

#endif  // QIMPLOT3DMESHITEMNODE_H
