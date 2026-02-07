#ifndef QIMPLOTLEGENDNODE_H
#define QIMPLOTLEGENDNODE_H
#include "QImPlot.h"
#include "QImAbstractNode.h"

namespace QIM
{


/**
 * @brief ImPlot 图例节点类
 *
 * 封装 ImPlot::SetupLegend 函数，提供 Qt 风格的属性接口来控制图例的位置和标志
 * 设计理念：
 * - 将 ImPlotLegendFlags 分解为独立的布尔属性
 * - 提供类型安全的枚举来替代 ImPlotLocation
 * - 支持属性绑定和信号通知
 * - 与 QImPlotNode 无缝集成
 */
class QIM_CORE_API QImPlotLegendNode : public QImAbstractNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotLegendNode)

    // === 位置属性 ===
    Q_PROPERTY(QImPlotLegendLocation location READ location WRITE setLocation NOTIFY loactionChanged)

    // === 方向属性 ===
    Q_PROPERTY(Qt::Orientation orientation READ orientation WRITE setOrientation NOTIFY legendFlagChanged)
    Q_PROPERTY(bool outside READ isOutside WRITE setOutside NOTIFY legendFlagChanged)
    Q_PROPERTY(bool buttonEnabled READ isButtonEnabled WRITE setButtonEnabled NOTIFY legendFlagChanged)
    Q_PROPERTY(bool highlightItemEnabled READ isHighlightItemEnabled WRITE setHighlightItemEnabled NOTIFY legendFlagChanged)
    Q_PROPERTY(bool highlightAxisEnabled READ isHighlightAxisEnabled WRITE setHighlightAxisEnabled NOTIFY legendFlagChanged)
    Q_PROPERTY(bool sort READ isSort WRITE setSort NOTIFY legendFlagChanged)
    Q_PROPERTY(bool reverse READ isReverse WRITE setReverse NOTIFY legendFlagChanged)
public:
    explicit QImPlotLegendNode(QObject* parent = nullptr);
    ~QImPlotLegendNode() override;

    // ----------------------------------------------------
    // 位置相关方法
    // ----------------------------------------------------
    QImPlotLegendLocation location() const;
    void setLocation(QImPlotLegendLocation v);
    // 映射为Qt::Alignment
    Qt::Alignment alignment() const;
    void setAlignment(Qt::Alignment a);

    // ----------------------------------------------------
    // 方向相关方法
    // ----------------------------------------------------
    // ImPlotLegendFlags_Horizontal
    void setOrientation(Qt::Orientation vh);
    Qt::Orientation orientation() const;

    // ----------------------------------------------------
    // ImPlotLegendFlags
    // ----------------------------------------------------
    // ImPlotLegendFlags_Outside
    bool isOutside() const;
    void setOutside(bool on);
    // ImPlotLegendFlags_NoButtons
    bool isButtonEnabled() const;
    void setButtonEnabled(bool on);
    // ImPlotLegendFlags_NoHighlightItem
    bool isHighlightItemEnabled() const;
    void setHighlightItemEnabled(bool on);
    // ImPlotLegendFlags_NoHighlightAxis
    bool isHighlightAxisEnabled() const;
    void setHighlightAxisEnabled(bool on);
    // ImPlotLegendFlags_Sort
    bool isSort() const;
    void setSort(bool on);
    // ImPlotLegendFlags_Reverse
    bool isReverse() const;
    void setReverse(bool on);
    // 原始标志访问（高级用法）
    int legendFlags() const;
    void setLegendFlags(int flags);

    // ----------------------------------------------------
    // 应用图例设置到当前绘图
    // ----------------------------------------------------
    void apply() const;

Q_SIGNALS:
    void loactionChanged(QIM::QImPlotLegendLocation v);
    void legendFlagChanged();

protected:
    bool beginDraw() override;
};

}  // namespace QIM


#endif  // QIMPLOTLEGENDNODE_H
