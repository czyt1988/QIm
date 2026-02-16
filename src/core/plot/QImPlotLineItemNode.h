#ifndef QIMPLOTLINEITEMNODE_H
#define QIMPLOTLINEITEMNODE_H
#include "QImPlotItemNode.h"
#include "QImPlotDataSeries.h"

namespace QIM
{
class QImAbstractXYDataSeries;

class QIM_CORE_API QImPlotLineItemNode : public QImPlotItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotLineItemNode)

    // 线条标志属性（全部采用肯定语义）
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
    // 数据设置
    //----------------------------------------------------
    void setData(QImAbstractXYDataSeries* series);
    template< typename ContainerX, typename ContainerY >
    QImAbstractXYDataSeries* setData(const ContainerX& x, const ContainerY& y);
    template< typename ContainerX, typename ContainerY >
    QImAbstractXYDataSeries* setData(ContainerX&& x, ContainerY&& y);
    // 获取数据
    QImAbstractXYDataSeries* data() const;
    //----------------------------------------------------
    // ImPlotLineFlags
    //----------------------------------------------------
    // 标志访问器（肯定语义）
    bool isSegments() const;
    void setSegments(bool on);

    bool isLoop() const;
    void setLoop(bool on);

    bool isSkipNaN() const;
    void setSkipNaN(bool on);

    bool isClippingEnabled() const;  // 对应 !NoClip
    void setClippingEnabled(bool enabled);

    bool isShaded() const;
    void setShaded(bool on);

    // 原始标志访问
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
