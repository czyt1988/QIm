#ifndef QIMPLOTITEMNODE_H
#define QIMPLOTITEMNODE_H
#include "QImAbstractNode.h"
#include "QImPlot.h"

struct ImPlotItem;
namespace QIM
{
class QImPlotNode;
/**
 * @brief PlotItem对应的基类
 */
class QIM_CORE_API QImPlotItemNode : public QImAbstractNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotItemNode)

    Q_PROPERTY(QString label READ label WRITE setLabel NOTIFY labelChanged)
public:
    enum TypeValue
    {
        InnerType = 10,
        UserType  = 10000
    };
    QImPlotItemNode(QObject* par = nullptr);
    ~QImPlotItemNode();
    // 用于快速识别那种绘图类型，避免进行大量的qobject_cast、dynamic_cast
    virtual int type() const = 0;
    //
    void setLabel(const QString& name);
    QString label() const;
    const char* labelConstData() const;
    // 绑定轴
    void bindAxis(QImPlotAxisId x, QImPlotAxisId y);
    // 屏幕到绘图坐标的转换
    QPointF pixelsToPlot(const float& screenX, const float& screenY);
    QPointF plotToPixels(const double& doubleX, const double& doubleY);
    // 绑定的x轴id
    QImPlotAxisId xAxisId() const;
    // 绑定的y轴id
    QImPlotAxisId yAxisId() const;
    // 获取绘图节点
    QImPlotNode* plotNode() const;
    // 颜色
    QColor itemColor() const;
    // 是否在legend上悬停
    bool isLegendHovered() const;
    //
    virtual bool isVisible() const override;
    virtual void setVisible(bool visible) override;
Q_SIGNALS:
    void labelChanged(const QString& name);

protected:
    virtual void endDraw() override;
    // ImPlotItem的操作
    ImPlotItem* imPlotItem() const;
    void setImPlotItem(ImPlotItem* item);
};
}  // end namespace QIM

#endif  // QIMPLOTITEMNODE_H
