#ifndef QIMPLOTDIGITALITEMNODE_H
#define QIMPLOTDIGITALITEMNODE_H
#include "QImAPI.h"
#include <QColor>
#include "QImPlotItemNode.h"
#include "QImPlotDataSeries.h"

namespace QIM
{
class QImAbstractXYDataSeries;

/**
 * \if ENGLISH
 * @brief Qt-style wrapper for ImPlot digital signal visualization
 *
 * @class QImPlotDigitalItemNode
 * @ingroup plot_items
 *
 * @details Provides Qt-style retained mode encapsulation for ImPlot digital signals.
 *          Digital plots visualize binary/digital signals (on/off states).
 *          They don't respond to y-axis drag/zoom and are always referenced to plot bottom.
 *          Inherits from QImPlotItemNode and follows the same PIMPL design pattern
 *          as QImPlotLineItemNode for consistency.
 *
 * @note Digital signals are typically used for visualizing binary data, logic levels,
 *       or any signal that switches between discrete values (e.g., 0 and 1).
 * @warning Digital plots are always referenced to the plot bottom and ignore y-axis scaling.
 *
 * @param[in] parent Parent QObject (optional)
 *
 * @see QImPlotItemNode
 * @see QImPlotLineItemNode
 * @see QImPlotNode
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot数字信号可视化的Qt风格封装
 *
 * @class QImPlotDigitalItemNode
 * @ingroup plot_items
 *
 * @details 为ImPlot数字信号提供Qt风格的保留模式封装。
 *          数字图可视化二进制/数字信号（开/关状态）。
 *          它们不响应y轴拖动/缩放，并且始终参考绘图底部。
 *          继承自QImPlotItemNode，并遵循与QImPlotLineItemNode相同的PIMPL设计模式以保持一致性。
 *
 * @note 数字信号通常用于可视化二进制数据、逻辑电平或任何在离散值之间切换的信号（例如0和1）。
 * @warning 数字图始终参考绘图底部并忽略y轴缩放。
 *
 * @param[in] parent 父QObject对象（可选）
 *
 * @see QImPlotItemNode
 * @see QImPlotLineItemNode
 * @see QImPlotNode
 * \endif
 */
class QIM_CORE_API QImPlotDigitalItemNode : public QImPlotItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotDigitalItemNode)

    /**
     * \if ENGLISH
     * @property QImPlotDigitalItemNode::color
     * @brief Color of digital signal lines
     *
     * @details Defines the color for digital signal lines.
     *          When not set, uses ImPlot's default color sequence.
     * @accessors READ color WRITE setColor NOTIFY colorChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotDigitalItemNode::color
     * @brief 数字信号线的颜色
     *
     * @details 定义数字信号线的颜色。
     *          未设置时，使用ImPlot的默认颜色序列。
     * @accessors READ color WRITE setColor NOTIFY colorChanged
     * \endif
     */
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    // Unique type identifier for QImPlotDigitalItemNode
    enum
    {
        Type = InnerType + 8
    };

    // Returns the type identifier of this plot item
    virtual int type() const override
    {
        return Type;
    }

    // Constructs a QImPlotDigitalItemNode with optional parent
    QImPlotDigitalItemNode(QObject* parent = nullptr);

    // Destroys the QImPlotDigitalItemNode
    ~QImPlotDigitalItemNode();

    //----------------------------------------------------
    // Data setting interface
    //----------------------------------------------------

    // Sets the data series for digital signal
    void setData(QImAbstractXYDataSeries* series);

    // Sets digital data from X and Y containers
    template< typename ContainerX, typename ContainerY >
    QImAbstractXYDataSeries* setData(const ContainerX& x, const ContainerY& y);

    // Sets digital data from X and Y containers (move semantics)
    template< typename ContainerX, typename ContainerY >
    QImAbstractXYDataSeries* setData(ContainerX&& x, ContainerY&& y);

    // Gets the data series
    QImAbstractXYDataSeries* data() const;

    //----------------------------------------------------
    // Style property accessors
    //----------------------------------------------------

    // Gets the digital signal color
    QColor color() const;

    // Sets the digital signal color
    void setColor(const QColor& color);

    // Gets the raw ImPlotDigitalFlags
    int digitalFlags() const;

    // Sets the raw ImPlotDigitalFlags
    void setDigitalFlags(int flags);

Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when digital signal color changes
     * @param[in] color New digital signal color
     * @details Triggered by setColor() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 数字信号颜色更改时触发
     * @param[in] color 新数字信号颜色
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
     * @brief Emitted when digital flags change
     * @details Triggered by any flag property change.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 数字标志更改时触发
     * @details 任何标志属性更改时触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void digitalFlagChanged();

protected:
    // Begins drawing the digital signal
    virtual bool beginDraw() override;
};

// Template function implementation
template< typename ContainerX, typename ContainerY >
inline QImAbstractXYDataSeries* QImPlotDigitalItemNode::setData(const ContainerX& x, const ContainerY& y)
{
    QImAbstractXYDataSeries* d = new QImVectorXYDataSeries(x, y);
    setData(d);
    return d;
}

template< typename ContainerX, typename ContainerY >
inline QImAbstractXYDataSeries* QImPlotDigitalItemNode::setData(ContainerX&& x, ContainerY&& y)
{
    QImAbstractXYDataSeries* d = new QImVectorXYDataSeries(x, y);
    setData(d);
    return d;
}

}  // end namespace QIM

#endif  // QIMPLOTDIGITALITEMNODE_H