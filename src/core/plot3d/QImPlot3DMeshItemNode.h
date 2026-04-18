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

    /**
     * \if ENGLISH
     * @brief Sets mesh vertices and triangle indices
     * @param[in] vertices The vertex positions as ImPlot3DPoint vector
     * @param[in] indices The triangle face indices (must be multiple of 3)
     * \endif
     *
     * \if CHINESE
     * @brief 设置网格顶点和三角面索引
     * @param[in] vertices 顶点位置向量（ImPlot3DPoint）
     * @param[in] indices 三角面索引（必须是3的倍数）
     * \endif
     */
    void setMeshData(const std::vector< ImPlot3DPoint >& vertices, const std::vector< unsigned int >& indices);

    /**
     * \if ENGLISH
     * @brief Returns the mesh vertex positions
     * @return Const reference to the vertex vector
     * \endif
     *
     * \if CHINESE
     * @brief 返回网格顶点位置
     * @return 顶点向量的常引用
     * \endif
     */
    const std::vector< ImPlot3DPoint >& vertices() const;

    /**
     * \if ENGLISH
     * @brief Returns the mesh triangle indices
     * @return Const reference to the index vector
     * \endif
     *
     * \if CHINESE
     * @brief 返回网格三角面索引
     * @return 索引向量的常引用
     * \endif
     */
    const std::vector< unsigned int >& indices() const;

    /**
     * \if ENGLISH
     * @brief Checks if mesh lines are visible
     * @return True if lines are visible (NoLines flag is not set)
     * \endif
     *
     * \if CHINESE
     * @brief 检查网格线条是否可见
     * @return 线条可见时返回true（NoLines标志未设置）
     * \endif
     */
    bool isLinesVisible() const;

    /**
     * \if ENGLISH
     * @brief Sets mesh line visibility
     * @param[in] visible True to show lines, false to hide
     * \endif
     *
     * \if CHINESE
     * @brief 设置网格线条可见性
     * @param[in] visible true显示线条，false隐藏
     * \endif
     */
    void setLinesVisible(bool visible);

    /**
     * \if ENGLISH
     * @brief Checks if mesh fill is visible
     * @return True if fill is visible (NoFill flag is not set)
     * \endif
     *
     * \if CHINESE
     * @brief 检查网格填充是否可见
     * @return 填充可见时返回true（NoFill标志未设置）
     * \endif
     */
    bool isFillVisible() const;

    /**
     * \if ENGLISH
     * @brief Sets mesh fill visibility
     * @param[in] visible True to show fill, false to hide
     * \endif
     *
     * \if CHINESE
     * @brief 设置网格填充可见性
     * @param[in] visible true显示填充，false隐藏
     * \endif
     */
    void setFillVisible(bool visible);

    /**
     * \if ENGLISH
     * @brief Checks if mesh markers are visible
     * @return True if markers are visible (NoMarkers flag is not set)
     * \endif
     *
     * \if CHINESE
     * @brief 检查网格标记点是否可见
     * @return 标记点可见时返回true（NoMarkers标志未设置）
     * \endif
     */
    bool isMarkersVisible() const;

    /**
     * \if ENGLISH
     * @brief Sets mesh marker visibility
     * @param[in] visible True to show markers, false to hide
     * \endif
     *
     * \if CHINESE
     * @brief 设置网格标记点可见性
     * @param[in] visible true显示标记点，false隐藏
     * \endif
     */
    void setMarkersVisible(bool visible);

    /**
     * \if ENGLISH
     * @brief Returns the marker shape
     * @return ImPlot3DMarker enum value
     * \endif
     *
     * \if CHINESE
     * @brief 返回标记形状
     * @return ImPlot3DMarker枚举值
     * \endif
     */
    int markerShape() const;

    /**
     * \if ENGLISH
     * @brief Sets the marker shape
     * @param[in] shape ImPlot3DMarker enum value
     * \endif
     *
     * \if CHINESE
     * @brief 设置标记形状
     * @param[in] shape ImPlot3DMarker枚举值
     * \endif
     */
    void setMarkerShape(int shape);

    /**
     * \if ENGLISH
     * @brief Returns the marker size in pixels
     * @return Marker size
     * \endif
     *
     * \if CHINESE
     * @brief 返回标记大小（像素）
     * @return 标记大小
     * \endif
     */
    float markerSize() const;

    /**
     * \if ENGLISH
     * @brief Sets the marker size in pixels
     * @param[in] size Marker size
     * \endif
     *
     * \if CHINESE
     * @brief 设置标记大小（像素）
     * @param[in] size 标记大小
     * \endif
     */
    void setMarkerSize(float size);

    /**
     * \if ENGLISH
     * @brief Returns the marker outline weight in pixels
     * @return Marker weight
     * \endif
     *
     * \if CHINESE
     * @brief 返回标记轮廓粗细（像素）
     * @return 标记轮廓粗细
     * \endif
     */
    float markerWeight() const;

    /**
     * \if ENGLISH
     * @brief Sets the marker outline weight in pixels
     * @param[in] weight Marker weight
     * \endif
     *
     * \if CHINESE
     * @brief 设置标记轮廓粗细（像素）
     * @param[in] weight 标记轮廓粗细
     * \endif
     */
    void setMarkerWeight(float weight);

    /**
     * \if ENGLISH
     * @brief Returns the fill color, or invalid QColor if not set
     * @details After first render, unset colors capture ImPlot3D default.
     * \endif
     *
     * \if CHINESE
     * @brief 返回填充颜色，若未设置则返回无效QColor
     * @details 首次渲染后，未设置的颜色会捕获ImPlot3D默认值。
     * \endif
     */
    QColor fillColor() const;

    /**
     * \if ENGLISH
     * @brief Sets the fill color
     * @param[in] color The new fill color
     * \endif
     *
     * \if CHINESE
     * @brief 设置填充颜色
     * @param[in] color 新的填充颜色
     * \endif
     */
    void setFillColor(const QColor& color);

    /**
     * \if ENGLISH
     * @brief Returns the line color, or invalid QColor if not set
     * @details After first render, unset colors capture ImPlot3D default.
     * \endif
     *
     * \if CHINESE
     * @brief 返回线条颜色，若未设置则返回无效QColor
     * @details 首次渲染后，未设置的颜色会捕获ImPlot3D默认值。
     * \endif
     */
    QColor lineColor() const;

    /**
     * \if ENGLISH
     * @brief Sets the line color
     * @param[in] color The new line color
     * \endif
     *
     * \if CHINESE
     * @brief 设置线条颜色
     * @param[in] color 新的线条颜色
     * \endif
     */
    void setLineColor(const QColor& color);

    /**
     * \if ENGLISH
     * @brief Returns the marker fill color, or invalid QColor if not set
     * @details After first render, unset colors capture ImPlot3D default.
     * \endif
     *
     * \if CHINESE
     * @brief 返回标记填充颜色，若未设置则返回无效QColor
     * @details 首次渲染后，未设置的颜色会捕获ImPlot3D默认值。
     * \endif
     */
    QColor markerFillColor() const;

    /**
     * \if ENGLISH
     * @brief Sets the marker fill color
     * @param[in] color The new marker fill color
     * \endif
     *
     * \if CHINESE
     * @brief 设置标记填充颜色
     * @param[in] color 新的标记填充颜色
     * \endif
     */
    void setMarkerFillColor(const QColor& color);

    /**
     * \if ENGLISH
     * @brief Returns the marker outline color, or invalid QColor if not set
     * @details After first render, unset colors capture ImPlot3D default.
     * \endif
     *
     * \if CHINESE
     * @brief 返回标记轮廓颜色，若未设置则返回无效QColor
     * @details 首次渲染后，未设置的颜色会捕获ImPlot3D默认值。
     * \endif
     */
    QColor markerOutlineColor() const;

    /**
     * \if ENGLISH
     * @brief Sets the marker outline color
     * @param[in] color The new marker outline color
     * \endif
     *
     * \if CHINESE
     * @brief 设置标记轮廓颜色
     * @param[in] color 新的标记轮廓颜色
     * \endif
     */
    void setMarkerOutlineColor(const QColor& color);

    /**
     * \if ENGLISH
     * @brief Returns the line width in pixels
     * @return Line width
     * \endif
     *
     * \if CHINESE
     * @brief 返回线宽（像素）
     * @return 线宽
     * \endif
     */
    float lineWidth() const;

    /**
     * \if ENGLISH
     * @brief Sets the line width in pixels
     * @param[in] width Line width
     * \endif
     *
     * \if CHINESE
     * @brief 设置线宽（像素）
     * @param[in] width 线宽
     * \endif
     */
    void setLineWidth(float width);

    /**
     * \if ENGLISH
     * @brief Returns raw ImPlot3DMeshFlags value
     * @return Integer combining all mesh flag bits
     * \endif
     *
     * \if CHINESE
     * @brief 返回原始ImPlot3DMeshFlags值
     * @return 组合所有网格标志位的整数
     * \endif
     */
    int meshFlags() const;

    /**
     * \if ENGLISH
     * @brief Sets raw ImPlot3DMeshFlags value
     * @param[in] flags Integer combining all mesh flag bits
     * \endif
     *
     * \if CHINESE
     * @brief 设置原始ImPlot3DMeshFlags值
     * @param[in] flags 组合所有网格标志位的整数
     * \endif
     */
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
