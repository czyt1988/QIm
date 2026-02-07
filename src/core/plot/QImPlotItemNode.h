#ifndef QIMPLOTITEMNODE_H
#define QIMPLOTITEMNODE_H

#include "QImAbstractNode.h"
#include "QImPlot.h"
namespace QIM
{

/**
 * @brief PlotItem对应的基类
 */
class QIM_CORE_API QImPlotItemNode : public QImAbstractNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotItemNode)

    Q_PROPERTY(QString label READ label WRITE setLabel NOTIFY labelChanged)
public:
    QImPlotItemNode(QObject* par = nullptr);
    ~QImPlotItemNode();
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
Q_SIGNALS:
    void labelChanged(const QString& name);
};
}  // end namespace QIM

#endif  // QIMPLOTITEMNODE_H
