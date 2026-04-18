#ifndef QIMPLOTITEMNODE_H
#define QIMPLOTITEMNODE_H
#include "QImAbstractNode.h"
#include "QImPlot.h"
#include <optional>
#include "QImTrackedValue.hpp"
#include "QtImGuiUtils.h"

struct ImPlotItem;
struct ImVec4;

namespace QIM
{

// ============================================================
// 类型别名定义 - 用于简化 Plot Item 颜色属性的声明
// ============================================================

/**
 * @brief 带脏标记跟踪的 ImVec4 颜色值
 * @details 封装 ImVec4 并提供变化检测功能，用于需要跟踪颜色变化并触发信号的场景
 */
using QImTrackedColor = QImTrackedValue<ImVec4, ImVecComparator<ImVec4>>;

/**
 * @brief 可选的跟踪颜色值
 * @details 用于 Plot Item 的颜色属性：
 *          - std::nullopt: 用户未设置颜色，使用 ImPlot 默认颜色
 *          - 有值: 用户已设置颜色，或已从 ImPlot 捕获默认颜色
 *          配合 beginDraw() 中的延迟初始化模式使用
 */
using QImOptionalColor = std::optional<QImTrackedColor>;


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
