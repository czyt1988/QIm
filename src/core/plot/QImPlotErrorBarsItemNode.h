#ifndef QIMPLOTERRORBARSITEMNODE_H
#define QIMPLOTERRORBARSITEMNODE_H
#include <QColor>
#include "QImPlotItemNode.h"
#include "QImPlotErrorDataSeries.h"

namespace QIM
{
class QImAbstractErrorDataSeries;

/**
 * \if ENGLISH
 * @brief Qt-style wrapper for ImPlot error bars visualization
 *
 * @class QImPlotErrorBarsItemNode
 * @ingroup plot_items
 *
 * @details Provides Qt-style retained mode encapsulation for ImPlot error bars.
 *          Supports both symmetric and asymmetric error bars, vertical and horizontal orientations.
 *          Error bars are typically used in combination with line plots or scatter plots
 *          to visualize data uncertainty or measurement errors.
 *          Inherits from QImPlotItemNode and follows the same PIMPL design pattern
 *          as QImPlotLineItemNode for consistency.
 *
 * @note Error bars visualize data uncertainty. The label_id should typically match
 *       the associated line or scatter plot for legend grouping.
 * @warning For asymmetric error mode, positive and negative error arrays must have
 *          the same size as the data series.
 *
 * @param[in] parent Parent QObject (optional)
 *
 * @see QImPlotItemNode
 * @see QImPlotLineItemNode
 * @see QImPlotScatterItemNode
 * @see QImPlotNode
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot误差棒图的Qt风格封装
 *
 * @class QImPlotErrorBarsItemNode
 * @ingroup plot_items
 *
 * @details 为ImPlot误差棒图提供Qt风格的保留模式封装。
 *          支持对称和非对称误差棒，垂直和水平方向。
 *          误差棒通常与线图或散点图组合使用，
 *          用于可视化数据不确定性或测量误差。
 *          继承自QImPlotItemNode，并遵循与QImPlotLineItemNode相同的PIMPL设计模式以保持一致性。
 *
 * @note 误差棒可视化数据不确定性。label_id通常应与关联的线图或散点图匹配，
 *       以便在图例中分组显示。
 * @warning 对于非对称误差模式，正负误差数组必须与数据系列具有相同的大小。
 *
 * @param[in] parent 父QObject对象（可选）
 *
 * @see QImPlotItemNode
 * @see QImPlotLineItemNode
 * @see QImPlotScatterItemNode
 * @see QImPlotNode
 * \endif
 */
class QIM_CORE_API QImPlotErrorBarsItemNode : public QImPlotItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotErrorBarsItemNode)

    /**
     * \if ENGLISH
     * @property QImPlotErrorBarsItemNode::horizontal
     * @brief Horizontal orientation flag
     *
     * @details When true, error bars are rendered horizontally on the current y-axis.
     *          When false, error bars are rendered vertically (default).
     * @accessors READ isHorizontal WRITE setHorizontal NOTIFY orientationChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotErrorBarsItemNode::horizontal
     * @brief 水平方向标志
     *
     * @details 为true时，误差棒沿当前y轴水平渲染。
     *          为false时，误差棒垂直渲染（默认）。
     * @accessors READ isHorizontal WRITE setHorizontal NOTIFY orientationChanged
     * \endif
     */
    Q_PROPERTY(bool horizontal READ isHorizontal WRITE setHorizontal NOTIFY orientationChanged)

    /**
     * \if ENGLISH
     * @property QImPlotErrorBarsItemNode::color
     * @brief Color of error bars
     *
     * @details Defines the color for error bars.
     *          When not set, uses ImPlot's default color sequence.
     * @accessors READ color WRITE setColor NOTIFY colorChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotErrorBarsItemNode::color
     * @brief 误差棒的颜色
     *
     * @details 定义误差棒的颜色。
     *          未设置时，使用ImPlot的默认颜色序列。
     * @accessors READ color WRITE setColor NOTIFY colorChanged
     * \endif
     */
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    // Unique type identifier for QImPlotErrorBarsItemNode
    enum
    {
        Type = InnerType + 6
    };

    // Returns the type identifier of this plot item
    virtual int type() const override
    {
        return Type;
    }

    // Constructs a QImPlotErrorBarsItemNode with optional parent
    QImPlotErrorBarsItemNode(QObject* parent = nullptr);

    // Destroys the QImPlotErrorBarsItemNode
    ~QImPlotErrorBarsItemNode();

    //----------------------------------------------------
    // Data setting interface - Using QImAbstractErrorDataSeries
    //----------------------------------------------------

    // Sets error data series (symmetric or asymmetric)
    void setData(QImAbstractErrorDataSeries* errorDataSeries);

    //----------------------------------------------------
    // Data setting interface - Symmetric error mode with arbitrary containers
    //----------------------------------------------------

    // Sets data from X, Y, and error containers (symmetric errors)
    template< typename ContainerX, typename ContainerY, typename ContainerError >
    QImAbstractErrorDataSeries* setData(const ContainerX& x, const ContainerY& y, const ContainerError& errors);

    // Sets data from X, Y, and error containers with move semantics (symmetric errors)
    template< typename ContainerX, typename ContainerY, typename ContainerError >
    QImAbstractErrorDataSeries* setData(ContainerX&& x, ContainerY&& y, ContainerError&& errors);

    //----------------------------------------------------
    // Data setting interface - Asymmetric error mode with arbitrary containers
    //----------------------------------------------------

    // Sets data from X, Y, and separate neg/pos error containers (asymmetric errors)
    template< typename ContainerX, typename ContainerY, typename ContainerError >
    QImAbstractErrorDataSeries* setData(const ContainerX& x, const ContainerY& y, const ContainerError& negErrors, const ContainerError& posErrors);

    // Sets data from X, Y, and separate neg/pos error containers with move semantics (asymmetric errors)
    template< typename ContainerX, typename ContainerY, typename ContainerError >
    QImAbstractErrorDataSeries* setData(ContainerX&& x, ContainerY&& y, ContainerError&& negErrors, ContainerError&& posErrors);

    // Gets the error data series
    QImAbstractErrorDataSeries* data() const;

    //----------------------------------------------------
    // Style property accessors
    //----------------------------------------------------

    // Checks if horizontal orientation is enabled
    bool isHorizontal() const;

    // Sets horizontal orientation
    void setHorizontal(bool horizontal);

    // Gets the error bar color
    QColor color() const;

    // Sets the error bar color
    void setColor(const QColor& color);

    // Gets the raw ImPlotErrorBarsFlags
    int errorBarsFlags() const;

    // Sets the raw ImPlotErrorBarsFlags
    void setErrorBarsFlags(int flags);

    // Checks if asymmetric error mode is active
    bool isAsymmetricMode() const;

Q_SIGNALS:
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
     * @brief Emitted when error bar color changes
     * @param[in] color New error bar color
     * @details Triggered by setColor() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 误差棒颜色更改时触发
     * @param[in] color 新误差棒颜色
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
     * @brief Emitted when error bars flags change
     * @details Triggered by any flag property change (horizontal, etc.).
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 误差棒标志更改时触发
     * @details 任何标志属性更改（水平方向等）时触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void errorBarsFlagChanged();

protected:
    // Begins drawing the error bars
    virtual bool beginDraw() override;
};

// Template function implementation - symmetric error mode
template< typename ContainerX, typename ContainerY, typename ContainerError >
inline QImAbstractErrorDataSeries* QImPlotErrorBarsItemNode::setData(const ContainerX& x, const ContainerY& y, const ContainerError& errors)
{
    QImAbstractErrorDataSeries* d = new QImVectorErrorDataSeries(x, y, errors);
    setData(d);
    return d;
}

template< typename ContainerX, typename ContainerY, typename ContainerError >
inline QImAbstractErrorDataSeries* QImPlotErrorBarsItemNode::setData(ContainerX&& x, ContainerY&& y, ContainerError&& errors)
{
    QImAbstractErrorDataSeries* d = new QImVectorErrorDataSeries(x, y, errors);
    setData(d);
    return d;
}

// Template function implementation - asymmetric error mode
template< typename ContainerX, typename ContainerY, typename ContainerError >
inline QImAbstractErrorDataSeries* QImPlotErrorBarsItemNode::setData(const ContainerX& x, const ContainerY& y, const ContainerError& negErrors, const ContainerError& posErrors)
{
    QImAbstractErrorDataSeries* d = new QImVectorErrorDataSeries(x, y, negErrors, posErrors);
    setData(d);
    return d;
}

template< typename ContainerX, typename ContainerY, typename ContainerError >
inline QImAbstractErrorDataSeries* QImPlotErrorBarsItemNode::setData(ContainerX&& x, ContainerY&& y, ContainerError&& negErrors, ContainerError&& posErrors)
{
    QImAbstractErrorDataSeries* d = new QImVectorErrorDataSeries(x, y, negErrors, posErrors);
    setData(d);
    return d;
}

}  // end namespace QIM

#endif  // QIMPLOTERRORBARSITEMNODE_H
