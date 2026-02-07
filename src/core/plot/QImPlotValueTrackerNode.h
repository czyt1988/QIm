#ifndef QIMPLOTVALUETRACKERNODE_H
#define QIMPLOTVALUETRACKERNODE_H
#include "QImAbstractNode.h"
#include <QColor>
namespace QIM
{
class QImPlotNode;
class QImPlotValueTrackerNodeGroup;

/**
 * @brief 智能值追踪器节点 - 自动绑定父Plot的所有曲线
 *
 * 显示的Tracker模式：
 *
 * - 构造时传入QImPlotNode*，自动监听所有子曲线（QImPlotLineItemNode）
 * - 仅追踪可见/启用的曲线
 * - 原生支持多Y轴（自动按Y轴分组显示）
 *
 * 使用示例：
 * @code
 * auto plot = ui->figureWidget->createPlotNode();
 * // 自动追踪plot下所有曲线
 * auto tracker = new QImPlotValueTrackerNode(plot);
 * tracker->setGroup(stockGroup); // 可选：加入联动组
 * @endcode
 */
class QIM_CORE_API QImPlotValueTrackerNode : public QImAbstractNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotValueTrackerNode)
public:
    // 跟踪值在指定X位置
    struct TrackedValue
    {
        const char* label;
        QColor color;
        double xValue;
        double yValue;
        std::string xValueLabel;
        std::string yValueLabel;
    };

public:
    explicit QImPlotValueTrackerNode(QImPlotNode* plotNode, QObject* parent = nullptr);
    ~QImPlotValueTrackerNode() override;
    // 分组
    void setGroup(QImPlotValueTrackerNodeGroup* group);
    bool hasGroup() const;
    QImPlotValueTrackerNodeGroup* group() const;
    // 样式设置
    // 宽度
    void setFixedWidth(float width);
    float fixedWidth() const;
    // 设置自动宽度计算
    void setAutoWidthEnabled(bool on);
    bool isAutoWidthEnabled() const;
    // 文字颜色
    void setTextColor(const QColor& color);
    QColor textColor() const;
    // 背景颜色
    void setBackgroundColor(const QColor& color);
    QColor backgroundColor() const;
    // 边框颜色
    void setBorderColor(const QColor& color);
    QColor borderColor() const;
    // 追踪线颜色
    void setTrackerLineColor(const QColor& color);
    QColor trackerLineColor() const;
    // 设置是否跳过nan或者finite
    void setSkipNanFiniteValues(bool on);
    bool isSkipNanFiniteValues() const;
    //

Q_SIGNALS:
    void activeChanged(bool on);

protected:
    bool beginDraw() override;
    virtual void renderTooltip(const std::vector< TrackedValue >& values, const QPointF& mouseScreenPos);
private Q_SLOTS:
    void onChildNodeAdded(QIM::QImAbstractNode* n);
    void onChildNodeRemoved(QIM::QImAbstractNode* n);

private:
};
}

#endif  // QIMPLOTVALUETRACKERNODE_H
