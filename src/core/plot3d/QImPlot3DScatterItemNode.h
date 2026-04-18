#ifndef QIMPLOT3DSCATTERITEMNODE_H
#define QIMPLOT3DSCATTERITEMNODE_H

#include "QImPlot3DItemNode.h"
#include "QImPlot3DDataSeries.h"

namespace QIM
{

class QImAbstractXYZDataSeries;

/**
 * \if ENGLISH
 * @brief Qt-style wrapper for ImPlot3D scatter plot visualization
 *
 * @class QImPlot3DScatterItemNode
 * @ingroup plot3d_items
 *
 * @details Provides Qt-style retained mode encapsulation for ImPlot3D scatter plots.
 *          Supports marker customization (shape, size, color) and full Qt property
 *          system integration with signal-slot event handling.
 *          Inherits from QImPlot3DItemNode and follows the same PIMPL design pattern.
 *
 * @note Scatter plots visualize discrete data points in 3D space as markers.
 *       Use QImPlot3DLineItemNode for continuous line plots.
 * @warning Marker size and shape changes require redraw to take effect.
 *
 * @param[in] parent Parent QObject (optional, typically a QImPlot3DNode)
 *
 * @see QImPlot3DItemNode
 * @see QImAbstractXYZDataSeries
 * @see QImPlot3DNode
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot3D散点图可视化的Qt风格封装
 *
 * @class QImPlot3DScatterItemNode
 * @ingroup plot3d_items
 *
 * @details 为ImPlot3D散点图提供Qt风格的保留模式封装。
 *          支持标记自定义（形状、大小、颜色）以及完整的Qt属性系统集成和信号槽事件处理。
 *          继承自QImPlot3DItemNode，并遵循相同的PIMPL设计模式。
 *
 * @note 散点图以标记形式可视化3D空间中的离散数据点。
 *       连续线图请使用QImPlot3DLineItemNode。
 * @warning 标记大小和形状更改需要重绘才能生效。
 *
 * @param[in] parent 父QObject对象（可选，通常是QImPlot3DNode）
 *
 * @see QImPlot3DItemNode
 * @see QImAbstractXYZDataSeries
 * @see QImPlot3DNode
 * \endif
 */
class QIM_CORE_API QImPlot3DScatterItemNode : public QImPlot3DItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlot3DScatterItemNode)

    // Color of scatter plot markers
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

    // Size of scatter plot markers in pixels
    Q_PROPERTY(float markerSize READ markerSize WRITE setMarkerSize NOTIFY markerSizeChanged)

    // Shape of scatter plot markers
    Q_PROPERTY(int markerShape READ markerShape WRITE setMarkerShape NOTIFY markerShapeChanged)

public:
    /**
     * \if ENGLISH
     * @brief Unique type identifier for QImPlot3DScatterItemNode
     * @details InnerType3D + 1 distinguishes this from other 3D item types.
     * \endif
     *
     * \if CHINESE
     * @brief QImPlot3DScatterItemNode的唯一类型标识符
     * @details InnerType3D + 1 用于与其他3D元素类型区分。
     * \endif
     */
    enum
    {
        Type = InnerType3D + 1
    };

    virtual int type() const override
    {
        return Type;
    }

    // Constructs a QImPlot3DScatterItemNode with optional parent
    explicit QImPlot3DScatterItemNode(QObject* parent = nullptr);

    // Destructor
    ~QImPlot3DScatterItemNode() override;

    //----------------------------------------------------
    // Data setting interface
    //----------------------------------------------------

    // Sets the XYZ data series for the scatter plot
    void setData(QImAbstractXYZDataSeries* series);

    // Sets scatter plot data from X, Y, and Z containers
    template<typename CX, typename CY, typename CZ>
    QImAbstractXYZDataSeries* setData(const CX& x, const CY& y, const CZ& z);

    // Gets the current XYZ data series
    QImAbstractXYZDataSeries* data() const;

    //----------------------------------------------------
    // Style property accessors
    //----------------------------------------------------

    // Gets the marker color
    QColor color() const;

    // Sets the marker color
    void setColor(const QColor& c);

    // Gets the marker size in pixels
    float markerSize() const;

    // Sets the marker size in pixels
    void setMarkerSize(float size);

    // Gets the marker shape
    int markerShape() const;

    // Sets the marker shape
    void setMarkerShape(int shape);

    //----------------------------------------------------
    // ImPlot3DScatterFlags
    //----------------------------------------------------

    // Gets the raw ImPlot3DScatterFlags
    int scatterFlags() const;

    // Sets the raw ImPlot3DScatterFlags
    void setScatterFlags(int flags);

Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when marker color changes
     * @param color New marker color
     * \endif
     *
     * \if CHINESE
     * @brief 标记颜色更改时触发
     * @param color 新标记颜色
     * \endif
     */
    void colorChanged(const QColor& color);

    /**
     * \if ENGLISH
     * @brief Emitted when marker size changes
     * @param size New marker size in pixels
     * \endif
     *
     * \if CHINESE
     * @brief 标记大小更改时触发
     * @param size 新标记大小（像素）
     * \endif
     */
    void markerSizeChanged(float size);

    /**
     * \if ENGLISH
     * @brief Emitted when marker shape changes
     * @param shape New marker shape as QImPlot3DMarkerShape enum value
     * \endif
     *
     * \if CHINESE
     * @brief 标记形状更改时触发
     * @param shape 新标记形状（QImPlot3DMarkerShape枚举值）
     * \endif
     */
    void markerShapeChanged(int shape);

    /**
     * \if ENGLISH
     * @brief Emitted when scatter plot flags change
     * \endif
     *
     * \if CHINESE
     * @brief 散点图标志更改时触发
     * \endif
     */
    void scatterFlagChanged();

    /**
     * \if ENGLISH
     * @brief Emitted when data series changes
     * \endif
     *
     * \if CHINESE
     * @brief 数据系列更改时触发
     * \endif
     */
    void dataChanged();

protected:
    // Begins drawing the scatter plot
    virtual bool beginDraw() override;
};

// Template function implementation
template<typename CX, typename CY, typename CZ>
inline QImAbstractXYZDataSeries* QImPlot3DScatterItemNode::setData(const CX& x, const CY& y, const CZ& z)
{
    QImAbstractXYZDataSeries* s = new QImVectorXYZDataSeries<CX, CY, CZ>(x, y, z);
    setData(s);
    return s;
}

}  // end namespace QIM

#endif  // QIMPLOT3DSCATTERITEMNODE_H