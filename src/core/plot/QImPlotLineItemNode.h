#ifndef QIMPLOTLINEITEMNODE_H
#define QIMPLOTLINEITEMNODE_H
#include "QImPlotItemNode.h"
#include "QImPlotDataSeries.h"

namespace QIM
{
class QImAbstractXYDataSeries;

/**
 * \if ENGLISH
 * @brief Qt-style wrapper for ImPlot line plot visualization
 * @details Provides Qt-style retained mode encapsulation for ImPlot line plots.
 *          Supports customizable line weight, data series binding (XY or Y-only mode),
 *          and full Qt property system integration with signal-slot event handling.
 *          Inherits from QImPlotItemNode and follows the same PIMPL design pattern
 *          for consistency with other plot item nodes.
 *
 * @note Line plots visualize continuous data series; use QImPlotBarsItemNode for
 *       categorical/discrete data and QImPlotScatterItemNode for point distributions.
 *
 * @see QImPlotItemNode, QImAbstractXYDataSeries, QImPlotNode
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot折线图可视化的Qt风格封装
 * @details 为ImPlot折线图提供Qt风格的保留模式封装。
 *          支持可自定义线宽、数据系列绑定（XY或Y-only模式），
 *          以及完整的Qt属性系统集成和信号槽事件处理。
 *          继承自QImPlotItemNode，并遵循与其他绘图项目节点相同的PIMPL设计模式以保持一致性。
 *
 * @note 折线图用于可视化连续数据系列；分类/离散数据请使用QImPlotBarsItemNode，
 *       点分布请使用QImPlotScatterItemNode。
 *
 * @see QImPlotItemNode, QImAbstractXYDataSeries, QImPlotNode
 * \endif
 */
class QIM_CORE_API QImPlotLineItemNode : public QImPlotItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotLineItemNode)

    // Line flags (affirmative semantics)
    Q_PROPERTY(bool segments READ isSegments WRITE setSegments NOTIFY lineFlagChanged)
    Q_PROPERTY(bool loop READ isLoop WRITE setLoop NOTIFY lineFlagChanged)
    Q_PROPERTY(bool skipNaN READ isSkipNaN WRITE setSkipNaN NOTIFY lineFlagChanged)
    Q_PROPERTY(bool clippingEnabled READ isClippingEnabled WRITE setClippingEnabled NOTIFY lineFlagChanged)
    Q_PROPERTY(bool shaded READ isShaded WRITE setShaded NOTIFY lineFlagChanged)
public:
    QImPlotLineItemNode(QObject* par = nullptr);
    ~QImPlotLineItemNode();
    enum
    {
        Type = InnerType + 1
    };
    virtual int type() const override
    {
        return Type;
    }
    //----------------------------------------------------
    // Data setting
    //----------------------------------------------------
    void setData(QImAbstractXYDataSeries* series);
    template< typename ContainerX, typename ContainerY >
    QImAbstractXYDataSeries* setData(const ContainerX& x, const ContainerY& y);
    template< typename ContainerX, typename ContainerY >
    QImAbstractXYDataSeries* setData(ContainerX&& x, ContainerY&& y);
    // Get data
    QImAbstractXYDataSeries* data() const;
    //----------------------------------------------------
    // ImPlotLineFlags
    //----------------------------------------------------
    // Flag accessors (affirmative semantics)
    bool isSegments() const;
    void setSegments(bool on);

    bool isLoop() const;
    void setLoop(bool on);

    bool isSkipNaN() const;
    void setSkipNaN(bool on);

    bool isClippingEnabled() const;  // Corresponds to !NoClip
    void setClippingEnabled(bool enabled);

    bool isShaded() const;
    void setShaded(bool on);

    // Raw flag access
    int lineFlags() const;
    void setLineFlags(int flags);
    //

    //===============================================================
    // style
    //===============================================================
    void setColor(const QColor& c);
    QColor color() const;
    //===============================================================
    // name
    //===============================================================
    void setAdaptivesSampling(bool on);
    bool isAdaptiveSampling() const;
Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when any line flag changes
     * @details Triggered by any flag property setter (setSegments, setLoop, setSkipNaN,
     *          setClippingEnabled, setShaded) when value actually changes.
     * \endif
     *
     * \if CHINESE
     * @brief 线条标志更改时触发
     * @details 任何标志属性setter（setSegments、setLoop、setSkipNaN、
     *          setClippingEnabled、setShaded）当值实际更改时触发。
     * \endif
     */
    void lineFlagChanged();

protected:
    virtual bool beginDraw() override;
};

template< typename ContainerX, typename ContainerY >
inline QImAbstractXYDataSeries* QImPlotLineItemNode::setData(const ContainerX& x, const ContainerY& y)
{
    QImAbstractXYDataSeries* d = new QImVectorXYDataSeries(x, y);
    setData(d);
    return d;
}
template< typename ContainerX, typename ContainerY >
QImAbstractXYDataSeries* QImPlotLineItemNode::setData(ContainerX&& x, ContainerY&& y)
{
    QImAbstractXYDataSeries* d = new QImVectorXYDataSeries(x, y);
    setData(d);
    return d;
}

}  // end namespace QIM
#endif  // QIMPLOTLINEITEMNODE_H
