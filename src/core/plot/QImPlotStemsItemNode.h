#ifndef QIMPLOTSTEMSITEMNODE_H
#define QIMPLOTSTEMSITEMNODE_H
#include <QColor>
#include "QImPlotItemNode.h"
#include "QImPlotDataSeries.h"

namespace QIM
{
class QImAbstractXYDataSeries;

/**
 * \if ENGLISH
 * @brief Qt-style wrapper for ImPlot stems visualization
 *
 * @class QImPlotStemsItemNode
 * @ingroup plot_items
 *
 * @details Provides Qt-style retained mode encapsulation for ImPlot stems.
 *          Stems plot draws a line from a reference value (baseline) to each data point,
 *          useful for visualizing discrete data points and their deviation from a baseline.
 *          Supports vertical (default) and horizontal orientations.
 *          Inherits from QImPlotItemNode and follows the same PIMPL design pattern
 *          as QImPlotLineItemNode for consistency.
 *
 * @note Stems are typically used to visualize discrete data points and their
 *       relationship to a baseline (usually zero).
 * @warning The reference value (baseline) should be set appropriately for your data.
 *
 * @param[in] parent Parent QObject (optional)
 *
 * @see QImPlotItemNode
 * @see QImPlotLineItemNode
 * @see QImPlotNode
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot茎叶图的Qt风格封装
 *
 * @class QImPlotStemsItemNode
 * @ingroup plot_items
 *
 * @details 为ImPlot茎叶图提供Qt风格的保留模式封装。
 *          茎叶图从参考值（基线）绘制线条到每个数据点，
 *          适用于可视化离散数据点及其与基线的偏差。
 *          支持垂直（默认）和水平方向。
 *          继承自QImPlotItemNode，并遵循与QImPlotLineItemNode相同的PIMPL设计模式以保持一致性。
 *
 * @note 茎叶图通常用于可视化离散数据点及其与基线（通常是零）的关系。
 * @warning 应根据数据适当设置参考值（基线）。
 *
 * @param[in] parent 父QObject对象（可选）
 *
 * @see QImPlotItemNode
 * @see QImPlotLineItemNode
 * @see QImPlotNode
 * \endif
 */
class QIM_CORE_API QImPlotStemsItemNode : public QImPlotItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotStemsItemNode)

    /**
     * \if ENGLISH
     * @property QImPlotStemsItemNode::referenceValue
     * @brief Reference value (baseline) for stems
     *
     * @details Controls the baseline from which stems are drawn.
     *          Default value is 0.0.
     * @accessors READ referenceValue WRITE setReferenceValue NOTIFY referenceValueChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotStemsItemNode::referenceValue
     * @brief 茎叶图的参考值（基线）
     *
     * @details 控制茎叶图绘制的基线位置。
     *          默认值为0.0。
     * @accessors READ referenceValue WRITE setReferenceValue NOTIFY referenceValueChanged
     * \endif
     */
    Q_PROPERTY(double referenceValue READ referenceValue WRITE setReferenceValue NOTIFY referenceValueChanged)

    /**
     * \if ENGLISH
     * @property QImPlotStemsItemNode::horizontal
     * @brief Horizontal orientation flag
     *
     * @details When true, stems are rendered horizontally on the current y-axis.
     *          When false, stems are rendered vertically (default).
     * @accessors READ isHorizontal WRITE setHorizontal NOTIFY orientationChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotStemsItemNode::horizontal
     * @brief 水平方向标志
     *
     * @details 为true时，茎叶图沿当前y轴水平渲染。
     *          为false时，茎叶图垂直渲染（默认）。
     * @accessors READ isHorizontal WRITE setHorizontal NOTIFY orientationChanged
     * \endif
     */
    Q_PROPERTY(bool horizontal READ isHorizontal WRITE setHorizontal NOTIFY orientationChanged)

    /**
     * \if ENGLISH
     * @property QImPlotStemsItemNode::color
     * @brief Color of stems
     *
     * @details Defines the color for stems.
     *          When not set, uses ImPlot's default color sequence.
     * @accessors READ color WRITE setColor NOTIFY colorChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotStemsItemNode::color
     * @brief 茎叶图的颜色
     *
     * @details 定义茎叶图的颜色。
     *          未设置时，使用ImPlot的默认颜色序列。
     * @accessors READ color WRITE setColor NOTIFY colorChanged
     * \endif
     */
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    // Unique type identifier for QImPlotStemsItemNode
    enum
    {
        Type = InnerType + 7
    };

    // Returns the type identifier of this plot item
    virtual int type() const override
    {
        return Type;
    }

    // Constructs a QImPlotStemsItemNode with optional parent
    QImPlotStemsItemNode(QObject* parent = nullptr);

    // Destroys the QImPlotStemsItemNode
    ~QImPlotStemsItemNode();

    //----------------------------------------------------
    // Data setting interface
    //----------------------------------------------------

    // Sets the data series for stems
    void setData(QImAbstractXYDataSeries* series);

    // Sets stems data from X and Y containers
    template< typename ContainerX, typename ContainerY >
    QImAbstractXYDataSeries* setData(const ContainerX& x, const ContainerY& y);

    // Sets stems data from X and Y containers (move semantics)
    template< typename ContainerX, typename ContainerY >
    QImAbstractXYDataSeries* setData(ContainerX&& x, ContainerY&& y);

    // Gets the data series
    QImAbstractXYDataSeries* data() const;

    //----------------------------------------------------
    // Style property accessors
    //----------------------------------------------------

    // Gets the reference value (baseline)
    double referenceValue() const;

    // Sets the reference value (baseline)
    void setReferenceValue(double value);

    // Checks if horizontal orientation is enabled
    bool isHorizontal() const;

    // Sets horizontal orientation
    void setHorizontal(bool horizontal);

    // Gets the stems color
    QColor color() const;

    // Sets the stems color
    void setColor(const QColor& color);

    // Gets the raw ImPlotStemsFlags
    int stemsFlags() const;

    // Sets the raw ImPlotStemsFlags
    void setStemsFlags(int flags);

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
     * @brief Emitted when stems color changes
     * @param[in] color New stems color
     * @details Triggered by setColor() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 茎叶图颜色更改时触发
     * @param[in] color 新茎叶图颜色
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
     * @brief Emitted when stems flags change
     * @details Triggered by any flag property change (horizontal, etc.).
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 茎叶图标志更改时触发
     * @details 任何标志属性更改（水平方向等）时触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void stemsFlagChanged();

protected:
    // Begins drawing the stems
    virtual bool beginDraw() override;
};

// Template function implementation
template< typename ContainerX, typename ContainerY >
inline QImAbstractXYDataSeries* QImPlotStemsItemNode::setData(const ContainerX& x, const ContainerY& y)
{
    QImAbstractXYDataSeries* d = new QImVectorXYDataSeries(x, y);
    setData(d);
    return d;
}

template< typename ContainerX, typename ContainerY >
inline QImAbstractXYDataSeries* QImPlotStemsItemNode::setData(ContainerX&& x, ContainerY&& y)
{
    QImAbstractXYDataSeries* d = new QImVectorXYDataSeries(x, y);
    setData(d);
    return d;
}

}  // end namespace QIM

#endif  // QIMPLOTSTEMSITEMNODE_H
