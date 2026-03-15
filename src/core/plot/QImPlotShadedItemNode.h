#ifndef QIMPLOTSHADEDITEMNODE_H
#define QIMPLOTSHADEDITEMNODE_H
#include <QColor>
#include "QImPlotItemNode.h"
#include "QImPlotDataSeries.h"

namespace QIM
{
class QImAbstractXYDataSeries;

/**
 * \if ENGLISH
 * @brief Qt-style wrapper for ImPlot shaded (filled) region visualization
 *
 * @class QImPlotShadedItemNode
 * @ingroup plot_items
 *
 * @details Provides Qt-style retained mode encapsulation for ImPlot shaded/filled regions.
 *          Supports filling between a line and a horizontal reference value,
 *          or filling between two lines (upper and lower bounds).
 *          Useful for area charts, confidence intervals, and uncertainty visualization.
 *          Inherits from QImPlotItemNode and follows the same PIMPL design pattern
 *          as QImPlotLineItemNode for consistency.
 *
 * @note Shaded plots visualize filled regions between lines or between a line and a reference.
 *       Use QImPlotLineItemNode for simple line plots.
 * @warning For two-line fill mode, both data series must have the same X coordinates.
 *
 * @param[in] parent Parent QObject (optional)
 *
 * @see QImPlotItemNode
 * @see QImPlotLineItemNode
 * @see QImPlotNode
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot填充区域（阴影）可视化的Qt风格封装
 *
 * @class QImPlotShadedItemNode
 * @ingroup plot_items
 *
 * @details 为ImPlot填充/阴影区域提供Qt风格的保留模式封装。
 *          支持线条与水平参考值之间的填充，或两条线之间（上下边界）的填充。
 *          适用于面积图、置信区间和不确定性可视化。
 *          继承自QImPlotItemNode，并遵循与QImPlotLineItemNode相同的PIMPL设计模式以保持一致性。
 *
 * @note 填充图可视化线条之间或线条与参考值之间的填充区域。
 *       简单线图请使用QImPlotLineItemNode。
 * @warning 对于双线填充模式，两个数据系列必须具有相同的X坐标。
 *
 * @param[in] parent 父QObject对象（可选）
 *
 * @see QImPlotItemNode
 * @see QImPlotLineItemNode
 * @see QImPlotNode
 * \endif
 */
class QIM_CORE_API QImPlotShadedItemNode : public QImPlotItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotShadedItemNode)

    /**
     * \if ENGLISH
     * @property QImPlotShadedItemNode::referenceValue
     * @brief Reference value (Y-axis) for single-line fill mode
     *
     * @details Controls the horizontal reference line for filling.
     *          The area between the data line and this reference value is filled.
     *          Use -INFINITY for fill extending to negative infinity,
     *          +INFINITY for fill extending to positive infinity.
     *          Default value is 0.0.
     * @accessors READ referenceValue WRITE setReferenceValue NOTIFY referenceValueChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotShadedItemNode::referenceValue
     * @brief 单线填充模式的参考值（Y轴）
     *
     * @details 控制填充的水平参考线。
     *          数据线与该参考值之间的区域被填充。
     *          使用-INFINITY表示填充延伸到负无穷，
     *          +INFINITY表示填充延伸到正无穷。
     *          默认值为0.0。
     * @accessors READ referenceValue WRITE setReferenceValue NOTIFY referenceValueChanged
     * \endif
     */
    Q_PROPERTY(double referenceValue READ referenceValue WRITE setReferenceValue NOTIFY referenceValueChanged)

    /**
     * \if ENGLISH
     * @property QImPlotShadedItemNode::color
     * @brief Fill color of the shaded region
     *
     * @details Defines the primary color for the filled region.
     *          When not set, uses ImPlot's default color sequence.
     * @accessors READ color WRITE setColor NOTIFY colorChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotShadedItemNode::color
     * @brief 填充区域的颜色
     *
     * @details 定义填充区域的主要颜色。
     *          未设置时，使用ImPlot的默认颜色序列。
     * @accessors READ color WRITE setColor NOTIFY colorChanged
     * \endif
     */
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    // Unique type identifier for QImPlotShadedItemNode
    enum
    {
        Type = InnerType + 5
    };

    // Returns the type identifier of this plot item
    virtual int type() const override
    {
        return Type;
    }

    // Constructs a QImPlotShadedItemNode with optional parent
    QImPlotShadedItemNode(QObject* parent = nullptr);

    // Destroys the QImPlotShadedItemNode
    ~QImPlotShadedItemNode();

    //----------------------------------------------------
    // Data setting interface - Single line fill mode
    //----------------------------------------------------

    // Sets the data series for single-line fill mode (fill to reference value)
    void setData(QImAbstractXYDataSeries* series);

    // Sets shaded data from X and Y containers (single-line mode)
    template< typename ContainerX, typename ContainerY >
    QImAbstractXYDataSeries* setData(const ContainerX& x, const ContainerY& y);

    // Sets shaded data from X and Y containers (move semantics, single-line mode)
    template< typename ContainerX, typename ContainerY >
    QImAbstractXYDataSeries* setData(ContainerX&& x, ContainerY&& y);

    //----------------------------------------------------
    // Data setting interface - Two-line fill mode
    //----------------------------------------------------

    // Sets two data series for filling between two lines (upper and lower bounds)
    void setData(QImAbstractXYDataSeries* series1, QImAbstractXYDataSeries* series2);

    // Sets two-line fill data from X, Y1, and Y2 containers
    template< typename ContainerX, typename ContainerY1, typename ContainerY2 >
    void setData(const ContainerX& x, const ContainerY1& y1, const ContainerY2& y2);

    // Sets two-line fill data from X, Y1, and Y2 containers (move semantics)
    template< typename ContainerX, typename ContainerY1, typename ContainerY2 >
    void setData(ContainerX&& x, ContainerY1&& y1, ContainerY2&& y2);

    // Gets the primary data series
    QImAbstractXYDataSeries* data() const;

    // Gets the secondary data series (for two-line fill mode)
    QImAbstractXYDataSeries* data2() const;

    //----------------------------------------------------
    // Style property accessors
    //----------------------------------------------------

    // Gets the reference value for single-line fill mode
    double referenceValue() const;

    // Sets the reference value for single-line fill mode
    void setReferenceValue(double value);

    // Gets the fill color
    QColor color() const;

    // Sets the fill color
    void setColor(const QColor& color);

    // Gets the raw ImPlotShadedFlags
    int shadedFlags() const;

    // Sets the raw ImPlotShadedFlags
    void setShadedFlags(int flags);

    // Checks if two-line fill mode is active
    bool isTwoLineMode() const;

Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when reference value changes
     * @param[in] value New reference value
     * @details Triggered by setReferenceValue() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 参考值更改时触发
     * @param[in] value 新参考值
     * @details 当值实际更改时由setReferenceValue()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void referenceValueChanged(double value);

    /**
     * \if ENGLISH
     * @brief Emitted when fill color changes
     * @param[in] color New fill color
     * @details Triggered by setColor() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 填充颜色更改时触发
     * @param[in] color 新填充颜色
     * @details 当值实际更改时由setColor()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void colorChanged(const QColor& color);

    /**
     * \if ENGLISH
     * @brief Emitted when data series changes
     * @details Triggered by setData() when new data is assigned.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 数据系列更改时触发
     * @details 当分配新数据时由setData()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void dataChanged();

    /**
     * \if ENGLISH
     * @brief Emitted when shaded flags change
     * @details Triggered by any flag property change.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 填充标志更改时触发
     * @details 任何标志属性更改时触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void shadedFlagChanged();

protected:
    // Begins drawing the shaded region
    virtual bool beginDraw() override;
};

// Template function implementation - single line mode
template< typename ContainerX, typename ContainerY >
inline QImAbstractXYDataSeries* QImPlotShadedItemNode::setData(const ContainerX& x, const ContainerY& y)
{
    QImAbstractXYDataSeries* d = new QImVectorXYDataSeries(x, y);
    setData(d);
    return d;
}

template< typename ContainerX, typename ContainerY >
inline QImAbstractXYDataSeries* QImPlotShadedItemNode::setData(ContainerX&& x, ContainerY&& y)
{
    QImAbstractXYDataSeries* d = new QImVectorXYDataSeries(x, y);
    setData(d);
    return d;
}

// Template function implementation - two line mode
// Note: For two-line mode, we create two separate data series sharing the same X
// The implementation is in the cpp file to avoid header complexity

}  // end namespace QIM

#endif  // QIMPLOTSHADEDITEMNODE_H
