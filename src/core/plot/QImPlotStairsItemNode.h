#ifndef QIMPLOTSTAIRSITEMNODE_H
#define QIMPLOTSTAIRSITEMNODE_H
#include "QImPlotItemNode.h"
#include "QImPlotDataSeries.h"

namespace QIM
{
class QImAbstractXYDataSeries;

class QIM_CORE_API QImPlotStairsItemNode : public QImPlotItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotStairsItemNode)

    // 阶梯图标志属性（全部采用肯定语义）
    Q_PROPERTY(bool shaded READ isShaded WRITE setShaded NOTIFY stairsFlagChanged)
public:
    QImPlotStairsItemNode(QObject* par = nullptr);
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
    void setData(QImAbstractXYDataSeries* series);
    template< typename ContainerX, typename ContainerY >
    QImAbstractXYDataSeries* setData(const ContainerX& x, const ContainerY& y);
    template< typename ContainerX, typename ContainerY >
    QImAbstractXYDataSeries* setData(ContainerX&& x, ContainerY&& y);
    // 获取数据
    QImAbstractXYDataSeries* data() const;
    //----------------------------------------------------
    // ImPlotStairsFlags
    //----------------------------------------------------
    // 标志访问器（肯定语义）
    bool isShaded() const;
    void setShaded(bool on);

    // 原始标志访问
    int stairsFlags() const;
    void setStairsFlags(int flags);

    //===============================================================
    // style
    //===============================================================
    void setColor(const QColor& c);
    QColor color() const;
Q_SIGNALS:
    void stairsFlagChanged();

protected:
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