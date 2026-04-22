#ifndef QIMPLOT3DMESHITEMNODE_H
#define QIMPLOT3DMESHITEMNODE_H

#include "QImPlot3DItemNode.h"
#include "QImPlot3DTypes.h"
#include <QColor>
#include <QList>
#include <vector>

namespace QIM
{
/**
 * \if ENGLISH
 * @brief 3D mesh plot item node
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
    // Fill alpha
    Q_PROPERTY(float fillAlpha READ fillAlpha WRITE setFillAlpha NOTIFY fillAlphaChanged)
    // Per-triangle coloring
    Q_PROPERTY(bool perTriangleFillColorEnabled READ isPerTriangleFillColorEnabled WRITE setPerTriangleFillColorEnabled NOTIFY perTriangleFillColorEnabledChanged)
    Q_PROPERTY(bool perTriangleLineColorEnabled READ isPerTriangleLineColorEnabled WRITE setPerTriangleLineColorEnabled NOTIFY perTriangleLineColorEnabledChanged)

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

    // Sets mesh vertices and triangle indices
    void setMeshData(const std::vector<QImPlot3DPoint>& vertices, const std::vector<unsigned int>& indices);

    // Returns the mesh vertex positions
    const std::vector<QImPlot3DPoint>& vertices() const;

    // Returns the mesh triangle indices
    const std::vector<unsigned int>& indices() const;

    // Static mesh data accessors
    static QList<QImPlot3DPoint> cubeVertices();
    static QList<unsigned int> cubeIndices();
    static QList<QImPlot3DPoint> sphereVertices();
    static QList<unsigned int> sphereIndices();
    static QList<QImPlot3DPoint> duckVertices();
    static QList<unsigned int> duckIndices();

    // Checks if mesh lines are visible (NoLines flag is not set)
    bool isLinesVisible() const;

    // Sets mesh line visibility (true to show lines, false to hide)
    void setLinesVisible(bool visible);

    // Checks if mesh fill is visible (NoFill flag is not set)
    bool isFillVisible() const;

    // Sets mesh fill visibility (true to show fill, false to hide)
    void setFillVisible(bool visible);

    // Checks if mesh markers are visible (NoMarkers flag is not set)
    bool isMarkersVisible() const;

    // Sets mesh marker visibility (true to show markers, false to hide)
    void setMarkersVisible(bool visible);

    // Returns the marker shape (ImPlot3DMarker enum value)
    int markerShape() const;

    // Sets the marker shape (ImPlot3DMarker enum value)
    void setMarkerShape(int shape);

    // Returns the marker size in pixels
    float markerSize() const;

    // Sets the marker size in pixels
    void setMarkerSize(float size);

    // Returns the marker outline weight in pixels
    float markerWeight() const;

    // Sets the marker outline weight in pixels
    void setMarkerWeight(float weight);

    // Returns the fill color, or invalid QColor if not set (captures ImPlot3D default after first render)
    QColor fillColor() const;

    // Sets the fill color
    void setFillColor(const QColor& color);

    // Returns the line color, or invalid QColor if not set (captures ImPlot3D default after first render)
    QColor lineColor() const;

    // Sets the line color
    void setLineColor(const QColor& color);

    // Returns the marker fill color, or invalid QColor if not set (captures ImPlot3D default after first render)
    QColor markerFillColor() const;

    // Sets the marker fill color
    void setMarkerFillColor(const QColor& color);

    // Returns the marker outline color, or invalid QColor if not set (captures ImPlot3D default after first render)
    QColor markerOutlineColor() const;

    // Sets the marker outline color
    void setMarkerOutlineColor(const QColor& color);

    // Returns the line width in pixels
    float lineWidth() const;
    void setLineWidth(float width);

    float fillAlpha() const;
    void setFillAlpha(float alpha);

    // Checks if per-triangle fill coloring is enabled
    bool isPerTriangleFillColorEnabled() const;
    // Enables or disables per-triangle fill coloring
    void setPerTriangleFillColorEnabled(bool enabled);

    // Checks if per-triangle line coloring is enabled
    bool isPerTriangleLineColorEnabled() const;
    // Enables or disables per-triangle line coloring
    void setPerTriangleLineColorEnabled(bool enabled);

    // Sets per-triangle fill colors from a list of QColor
    void setFillColors(const QList<QColor>& colors);
    // Returns per-triangle fill colors as a list of QColor
    QList<QColor> fillColors() const;

    // Sets per-triangle line colors from a list of QColor
    void setLineColors(const QList<QColor>& colors);
    // Returns per-triangle line colors as a list of QColor
    QList<QColor> lineColors() const;

    // Returns raw ImPlot3DMeshFlags value (integer combining all mesh flag bits)
    int meshFlags() const;

    // Sets raw ImPlot3DMeshFlags value (integer combining all mesh flag bits)
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
     * @details 当填充透明度值实际更改时由setFillAlpha()触发。
     * \endif
     */
    void fillAlphaChanged(float alpha);
    /**
     * \if ENGLISH
     * @brief Emitted when per-triangle fill coloring is enabled or disabled
     * @param[in] enabled True if per-triangle fill coloring is enabled
     * @details Triggered by setPerTriangleFillColorEnabled() when the enabled state actually changes.
     * \endif
     *
     * \if CHINESE
     * @brief 三角面逐面填充颜色启用状态变更时触发
     * @param[in] enabled true表示启用了逐三角面填充颜色
     * @details 当启用状态实际变更时由setPerTriangleFillColorEnabled()触发。
     * \endif
     */
    void perTriangleFillColorEnabledChanged(bool enabled);
    /**
     * \if ENGLISH
     * @brief Emitted when per-triangle line coloring is enabled or disabled
     * @param[in] enabled True if per-triangle line coloring is enabled
     * @details Triggered by setPerTriangleLineColorEnabled() when the enabled state actually changes.
     * \endif
     *
     * \if CHINESE
     * @brief 三角面逐面线条颜色启用状态变更时触发
     * @param[in] enabled true表示启用了逐三角面线条颜色
     * @details 当启用状态实际变更时由setPerTriangleLineColorEnabled()触发。
     * \endif
     */
    void perTriangleLineColorEnabledChanged(bool enabled);

protected:
    bool beginDraw() override;
};
}  // namespace QIM

#endif  // QIMPLOT3DMESHITEMNODE_H
