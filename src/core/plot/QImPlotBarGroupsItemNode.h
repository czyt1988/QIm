#ifndef QIMPLOTBARGROUPSITEMNODE_H
#define QIMPLOTBARGROUPSITEMNODE_H

#include <QColor>
#include <QStringList>
#include "QImPlotItemNode.h"
#include "QImPlotBarGroupsDataSeries.h"

namespace QIM
{
class QImAbstractBarGroupsDataSeries;

/**
 * \if ENGLISH
 * @brief Qt-style wrapper for ImPlot grouped bar chart visualization
 *
 * @class QImPlotBarGroupsItemNode
 * @ingroup plot_items
 *
 * @details Provides Qt-style retained mode encapsulation for ImPlot grouped bar charts.
 *          Supports horizontal/vertical orientation, customizable group width,
 *          stacked bars, shift offset, and full Qt property system integration
 *          with signal-slot event handling.
 *          Inherits from QImPlotItemNode and follows the same PIMPL design pattern
 *          as QImPlotBarsItemNode for consistency.
 *
 * @note Grouped bar charts visualize multiple items across groups using side-by-side bars.
 *       Use QImPlotBarsItemNode for single series bar charts.
 * @warning Group width changes require redraw to take effect.
 *
 * @param[in] parent Parent QObject (optional)
 *
 * @see QImPlotItemNode
 * @see QImPlotBarsItemNode
 * @see QImPlotNode
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot分组柱状图可视化的Qt风格封装
 *
 * @class QImPlotBarGroupsItemNode
 * @ingroup plot_items
 *
 * @details 为ImPlot分组柱状图提供Qt风格的保留模式封装。
 *          支持水平/垂直方向、可自定义组宽、
 *          堆叠柱状图、偏移量，以及完整的Qt属性系统集成和信号槽事件处理。
 *          继承自QImPlotItemNode，并遵循与QImPlotBarsItemNode相同的PIMPL设计模式以保持一致性。
 *
 * @note 分组柱状图使用并排的柱子可视化多个项目在不同组中的分布。
 *       单系列柱状图请使用QImPlotBarsItemNode。
 * @warning 组宽更改需要重绘才能生效。
 *
 * @param[in] parent 父QObject对象（可选）
 *
 * @see QImPlotItemNode
 * @see QImPlotBarsItemNode
 * @see QImPlotNode
 * \endif
 */
class QIM_CORE_API QImPlotBarGroupsItemNode : public QImPlotItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotBarGroupsItemNode)

    Q_PROPERTY(double groupWidth READ groupWidth WRITE setGroupWidth NOTIFY groupWidthChanged)

    Q_PROPERTY(bool horizontal READ isHorizontal WRITE setHorizontal NOTIFY orientationChanged)

    Q_PROPERTY(bool stacked READ isStacked WRITE setStacked NOTIFY stackedChanged)

    Q_PROPERTY(double shift READ shift WRITE setShift NOTIFY shiftChanged)

    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    // Unique type identifier for QImPlotBarGroupsItemNode
    enum
    {
        Type = InnerType + 9
    };

    // Returns the type identifier of this plot item
    virtual int type() const override
    {
        return Type;
    }

    // Constructs a QImPlotBarGroupsItemNode with optional parent
    QImPlotBarGroupsItemNode(QObject* parent = nullptr);

    // Destroys the QImPlotBarGroupsItemNode
    ~QImPlotBarGroupsItemNode();

    //----------------------------------------------------
    // Data setting interface
    //----------------------------------------------------

    // Sets the data series for the grouped bar chart
    void setData(QImAbstractBarGroupsDataSeries* series);

    // Sets grouped bar chart data from labels and values matrix
    template< typename ContainerValues >
    QImAbstractBarGroupsDataSeries* setData(const QStringList& labels, const ContainerValues& values,
                                            int itemCount, int groupCount);

    // Sets grouped bar chart data from labels and values matrix (move semantics)
    template< typename ContainerValues >
    QImAbstractBarGroupsDataSeries* setData(QStringList&& labels, ContainerValues&& values,
                                            int itemCount, int groupCount);

    // Gets the current data series
    QImAbstractBarGroupsDataSeries* data() const;

    //----------------------------------------------------
    // Style property accessors
    //----------------------------------------------------

    // Gets the group width in plot units
    double groupWidth() const;

    // Sets the group width in plot units
    void setGroupWidth(double width);

    // Checks if horizontal orientation is enabled
    bool isHorizontal() const;

    // Sets horizontal orientation
    void setHorizontal(bool horizontal);

    // Checks if stacked bars are enabled
    bool isStacked() const;

    // Sets stacked bars
    void setStacked(bool stacked);

    // Gets the shift offset in plot units
    double shift() const;

    // Sets the shift offset in plot units
    void setShift(double shift);

    // Gets the bar color
    QColor color() const;

    // Sets the bar color
    void setColor(const QColor& color);

    // Gets the raw ImPlotBarGroupsFlags
    int barGroupsFlags() const;

    // Sets the raw ImPlotBarGroupsFlags
    void setBarGroupsFlags(int flags);

Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when group width changes
     * @param[in] width New group width in plot units
     * @details Triggered by setGroupWidth() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 组宽更改时触发
     * @param[in] width 新组宽（绘图单位）
     * @details 当值实际更改时由setGroupWidth()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void groupWidthChanged(double width);

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
     * @brief Emitted when stacked flag changes
     * @param[in] stacked New stacked state
     * @details Triggered by setStacked() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 堆叠标志更改时触发
     * @param[in] stacked 新堆叠状态
     * @details 当值实际更改时由setStacked()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void stackedChanged(bool stacked);

    /**
     * \if ENGLISH
     * @brief Emitted when shift offset changes
     * @param[in] shift New shift offset in plot units
     * @details Triggered by setShift() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 偏移量更改时触发
     * @param[in] shift 新偏移量（绘图单位）
     * @details 当值实际更改时由setShift()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void shiftChanged(double shift);

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
     * @brief Emitted when bar groups flags change
     * @details Triggered by any flag property change (horizontal, stacked, etc.).
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 分组柱状图标志更改时触发
     * @details 任何标志属性更改（水平方向、堆叠等）时触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void barGroupsFlagChanged();

protected:
    // Begins drawing the grouped bar chart
    virtual bool beginDraw() override;
};

// Template function implementation
template< typename ContainerValues >
inline QImAbstractBarGroupsDataSeries* QImPlotBarGroupsItemNode::setData(const QStringList& labels, const ContainerValues& values,
                                                                         int itemCount, int groupCount)
{
    QImAbstractBarGroupsDataSeries* d = new QImVectorBarGroupsDataSeries<ContainerValues>(labels, values, itemCount, groupCount);
    setData(d);
    return d;
}

template< typename ContainerValues >
inline QImAbstractBarGroupsDataSeries* QImPlotBarGroupsItemNode::setData(QStringList&& labels, ContainerValues&& values,
                                                                         int itemCount, int groupCount)
{
    QImAbstractBarGroupsDataSeries* d = new QImVectorBarGroupsDataSeries<ContainerValues>(std::move(labels), std::move(values), itemCount, groupCount);
    setData(d);
    return d;
}

}  // end namespace QIM

#endif  // QIMPLOTBARGROUPSITEMNODE_H