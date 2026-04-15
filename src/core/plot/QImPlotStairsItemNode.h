#ifndef QIMPLOTSTAIRSITEMNODE_H
#define QIMPLOTSTAIRSITEMNODE_H
#include "QImPlotItemNode.h"
#include "QImPlotDataSeries.h"

namespace QIM
{
class QImAbstractXYDataSeries;

/**
 * \if ENGLISH
 * @brief QImPlotStairsItemNode class for creating staircase plots
 * @details This class encapsulates ImPlot's PlotStairs functionality to create staircase plots
 *          with Qt-style properties and signals
 * @see QImPlotItemNode
 * \endif
 *
 * \if CHINESE
 * @brief 用于创建阶梯图的QImPlotStairsItemNode类
 * @details 此类封装了ImPlot的PlotStairs功能，用于创建阶梯图
 *          并提供Qt风格的属性和信号
 * @see QImPlotItemNode
 * \endif
 */
class QIM_CORE_API QImPlotStairsItemNode : public QImPlotItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotStairsItemNode)

    // 阶梯图标志属性（全部采用肯定语义）
    Q_PROPERTY(bool shaded READ isShaded WRITE setShaded NOTIFY stairsFlagChanged)

    /**
     * \if ENGLISH
     * @property QImPlotStairsItemNode::preStep
     * @brief Enable pre-step mode for staircase rendering
     *
     * @details When true, the stair step is drawn before the next data point (pre-step).
     *          When false (default), the stair step is drawn after the data point (post-step).
     *          Corresponds to ImPlotStairsFlags_PreStep.
     * @accessors READ isPreStep WRITE setPreStep NOTIFY stairsFlagChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotStairsItemNode::preStep
     * @brief 启用前阶梯渲染模式
     *
     * @details 为true时，阶梯在下一个数据点之前绘制（前阶梯）。
     *          为false时（默认），阶梯在数据点之后绘制（后阶梯）。
     *          对应于 ImPlotStairsFlags_PreStep。
     * @accessors READ isPreStep WRITE setPreStep NOTIFY stairsFlagChanged
     * \endif
     */
    Q_PROPERTY(bool preStep READ isPreStep WRITE setPreStep NOTIFY stairsFlagChanged)
public:
    // Constructor for QImPlotStairsItemNode
    QImPlotStairsItemNode(QObject* par = nullptr);
    // Destructor for QImPlotStairsItemNode
    ~QImPlotStairsItemNode();
    enum
    {
        Type = InnerType + 3
    };
    virtual int type() const override
    {
        return Type;
    }
    //----------------------------------------------------
    // 数据设置
    //----------------------------------------------------
    // Set data series for the stairs plot
    void setData(QImAbstractXYDataSeries* series);
    // Set data from containers
    template< typename ContainerX, typename ContainerY >
    QImAbstractXYDataSeries* setData(const ContainerX& x, const ContainerY& y);
    // Set data from rvalue containers
    template< typename ContainerX, typename ContainerY >
    QImAbstractXYDataSeries* setData(ContainerX&& x, ContainerY&& y);
    // Get current data series
    QImAbstractXYDataSeries* data() const;
    //----------------------------------------------------
    // ImPlotStairsFlags
    //----------------------------------------------------
    // 标志访问器（肯定语义）
    // Check if shaded mode is enabled
    bool isShaded() const;
    // Set shaded mode
    void setShaded(bool on);
    // Check if pre-step mode is enabled
    bool isPreStep() const;
    // Set pre-step mode
    void setPreStep(bool on);

    // 原始标志访问
    // Get raw ImPlotStairsFlags
    int stairsFlags() const;
    // Set raw ImPlotStairsFlags
    void setStairsFlags(int flags);

    //===============================================================
    // style
    //===============================================================
    // Set line color
    void setColor(const QColor& c);
    // Get line color
    QColor color() const;
Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when stairs plot flags change
     * \endif
     *
     * \if CHINESE
     * @brief 当阶梯图标志改变时发出
     * \endif
     */
    void stairsFlagChanged();

protected:
    // Begin drawing implementation
    virtual bool beginDraw() override;
};

template< typename ContainerX, typename ContainerY >
inline QImAbstractXYDataSeries* QImPlotStairsItemNode::setData(const ContainerX& x, const ContainerY& y)
{
    QImAbstractXYDataSeries* d = new QImVectorXYDataSeries(x, y);
    setData(d);
    return d;
}
template< typename ContainerX, typename ContainerY >
QImAbstractXYDataSeries* QImPlotStairsItemNode::setData(ContainerX&& x, ContainerY&& y)
{
    QImAbstractXYDataSeries* d = new QImVectorXYDataSeries(x, y);
    setData(d);
    return d;
}

}  // end namespace QIM
#endif  // QIMPLOTSTAIRSITEMNODE_H