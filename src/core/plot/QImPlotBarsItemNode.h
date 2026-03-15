#ifndef QIMPLOTBARSITEMNODE_H
#define QIMPLOTBARSITEMNODE_H
#include <QColor>
#include "QImPlotItemNode.h"
#include "QImPlotDataSeries.h"

namespace QIM
{
class QImAbstractXYDataSeries;

/**
 * \if ENGLISH
 * @brief Qt-style wrapper for ImPlot bar chart visualization
 *
 * @class QImPlotBarsItemNode
 * @ingroup plot_items
 *
 * @details Provides Qt-style retained mode encapsulation for ImPlot bar charts.
 *          Supports horizontal/vertical orientation, customizable bar width,
 *          and full Qt property system integration with signal-slot event handling.
 *          Inherits from QImPlotItemNode and follows the same PIMPL design pattern
 *          as QImPlotLineItemNode for consistency.
 *
 * @note Bar charts visualize categorical or discrete data using rectangular bars.
 *       Use QImPlotLineItemNode for continuous line plots.
 * @warning Bar width changes require redraw to take effect.
 *
 * @param[in] parent Parent QObject (optional)
 *
 * @see QImPlotItemNode
 * @see QImPlotLineItemNode
 * @see QImPlotNode
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot柱状图可视化的Qt风格封装
 *
 * @class QImPlotBarsItemNode
 * @ingroup plot_items
 *
 * @details 为ImPlot柱状图提供Qt风格的保留模式封装。
 *          支持水平/垂直方向、可自定义柱宽，
 *          以及完整的Qt属性系统集成和信号槽事件处理。
 *          继承自QImPlotItemNode，并遵循与QImPlotLineItemNode相同的PIMPL设计模式以保持一致性。
 *
 * @note 柱状图使用矩形柱可视化分类或离散数据。
 *       连续线图请使用QImPlotLineItemNode。
 * @warning 柱宽更改需要重绘才能生效。
 *
 * @param[in] parent 父QObject对象（可选）
 *
 * @see QImPlotItemNode
 * @see QImPlotLineItemNode
 * @see QImPlotNode
 * \endif
 */
class QIM_CORE_API QImPlotBarsItemNode : public QImPlotItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotBarsItemNode)

    /**
     * \if ENGLISH
     * @property QImPlotBarsItemNode::barWidth
     * @brief Width of bars in plot units
     *
     * @details Controls the visual width of bars in the bar chart.
     *          Value is in plot units (not pixels).
     *          Default value is 0.67.
     * @accessors READ barWidth WRITE setBarWidth NOTIFY barWidthChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotBarsItemNode::barWidth
     * @brief 柱子的宽度（绘图单位）
     *
     * @details 控制柱状图中柱子的视觉宽度。
     *          值为绘图单位（非像素）。
     *          默认值为0.67。
     * @accessors READ barWidth WRITE setBarWidth NOTIFY barWidthChanged
     * \endif
     */
    Q_PROPERTY(double barWidth READ barWidth WRITE setBarWidth NOTIFY barWidthChanged)

    /**
     * \if ENGLISH
     * @property QImPlotBarsItemNode::horizontal
     * @brief Horizontal orientation flag
     *
     * @details When true, bars are rendered horizontally on the current y-axis.
     *          When false, bars are rendered vertically (default).
     * @accessors READ isHorizontal WRITE setHorizontal NOTIFY orientationChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotBarsItemNode::horizontal
     * @brief 水平方向标志
     *
     * @details 为true时，柱子沿当前y轴水平渲染。
     *          为false时，柱子垂直渲染（默认）。
     * @accessors READ isHorizontal WRITE setHorizontal NOTIFY orientationChanged
     * \endif
     */
    Q_PROPERTY(bool horizontal READ isHorizontal WRITE setHorizontal NOTIFY orientationChanged)

    /**
     * \if ENGLISH
     * @property QImPlotBarsItemNode::color
     * @brief Color of bars
     *
     * @details Defines the primary color for bar chart bars.
     *          When not set, uses ImPlot's default color sequence.
     * @accessors READ color WRITE setColor NOTIFY colorChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotBarsItemNode::color
     * @brief 柱子的颜色
     *
     * @details 定义柱状图柱子的主要颜色。
     *          未设置时，使用ImPlot的默认颜色序列。
     * @accessors READ color WRITE setColor NOTIFY colorChanged
     * \endif
     */
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    // Unique type identifier for QImPlotBarsItemNode
    enum
    {
        Type = InnerType + 4
    };

    // Returns the type identifier of this plot item
    virtual int type() const override
    {
        return Type;
    }

    // Constructs a QImPlotBarsItemNode with optional parent
    QImPlotBarsItemNode(QObject* parent = nullptr);

    // Destroys the QImPlotBarsItemNode
    ~QImPlotBarsItemNode();

    //----------------------------------------------------
    // Data setting interface
    //----------------------------------------------------

    // Sets the data series for the bar chart
    void setData(QImAbstractXYDataSeries* series);

    // Sets bar chart data from X and Y containers
    template< typename ContainerX, typename ContainerY >
    QImAbstractXYDataSeries* setData(const ContainerX& x, const ContainerY& y);

    // Sets bar chart data from X and Y containers (move semantics)
    template< typename ContainerX, typename ContainerY >
    QImAbstractXYDataSeries* setData(ContainerX&& x, ContainerY&& y);

    // Gets the current data series
    QImAbstractXYDataSeries* data() const;

    //----------------------------------------------------
    // Style property accessors
    //----------------------------------------------------

    // Gets the bar width in plot units
    double barWidth() const;

    // Sets the bar width in plot units
    void setBarWidth(double width);

    // Checks if horizontal orientation is enabled
    bool isHorizontal() const;

    // Sets horizontal orientation
    void setHorizontal(bool horizontal);

    // Gets the bar color
    QColor color() const;

    // Sets the bar color
    void setColor(const QColor& color);

    // Gets the raw ImPlotBarsFlags
    int barsFlags() const;

    // Sets the raw ImPlotBarsFlags
    void setBarsFlags(int flags);

Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when bar width changes
     * @param[in] width New bar width in plot units
     * @details Triggered by setBarWidth() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 柱宽更改时触发
     * @param[in] width 新柱宽（绘图单位）
     * @details 当值实际更改时由setBarWidth()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void barWidthChanged(double width);

    /**
     * \if ENGLISH
     * @brief Emitted when orientation changes
     * @param[in] horizontal New horizontal orientation state
     * @details Triggered by setHorizontal() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 方向更改时触发
     * @param[in] horizontal 新水平方向状态
     * @details 当值实际更改时由setHorizontal()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void orientationChanged(bool horizontal);

    /**
     * \if ENGLISH
     * @brief Emitted when bar color changes
     * @param[in] color New bar color
     * @details Triggered by setColor() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 柱子颜色更改时触发
     * @param[in] color 新柱子颜色
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
     * @brief Emitted when bar chart flags change
     * @details Triggered by any flag property change (horizontal, etc.).
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 柱状图标志更改时触发
     * @details 任何标志属性更改（水平方向等）时触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void barsFlagChanged();

protected:
    // Begins drawing the bar chart
    virtual bool beginDraw() override;
};

// Template function implementation
template< typename ContainerX, typename ContainerY >
inline QImAbstractXYDataSeries* QImPlotBarsItemNode::setData(const ContainerX& x, const ContainerY& y)
{
    QImAbstractXYDataSeries* d = new QImVectorXYDataSeries(x, y);
    setData(d);
    return d;
}

template< typename ContainerX, typename ContainerY >
inline QImAbstractXYDataSeries* QImPlotBarsItemNode::setData(ContainerX&& x, ContainerY&& y)
{
    QImAbstractXYDataSeries* d = new QImVectorXYDataSeries(std::move(x), std::move(y));
    setData(d);
    return d;
}

}  // end namespace QIM

#endif  // QIMPLOTBARSITEMNODE_H
