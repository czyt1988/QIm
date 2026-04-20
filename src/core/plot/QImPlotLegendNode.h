#ifndef QIMPLOTLEGENDNODE_H
#define QIMPLOTLEGENDNODE_H
#include "QImPlot.h"
#include "QImAbstractNode.h"

namespace QIM
{


/**
 * \if ENGLISH
 * @brief ImPlot legend node class
 * @details Wraps ImPlot::SetupLegend() to provide Qt-style property interfaces
 *          for controlling legend position and flags within the QIm object tree.
 *          Encapsulates legend visibility, location (North/South/West/East/corners/Center),
 *          and orientation (horizontal/vertical) as intuitive boolean and enum properties.
 * @note Legend configuration is applied during plot rendering via beginDraw().
 * @see QImPlotNode, QImPlotLegendLocation
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot 图例节点类
 * @details 封装 ImPlot::SetupLegend 函数，在QIm对象树中提供Qt风格的属性接口
 *          来控制图例的位置和标志。将图例可见性、位置（North/South/West/East/角落/Center）
 *          和方向（水平/垂直）封装为直观的布尔和枚举属性。
 * @note 图例配置在绘图渲染期间通过beginDraw()应用。
 * @see QImPlotNode, QImPlotLegendLocation
 * \endif
 */
class QIM_CORE_API QImPlotLegendNode : public QImAbstractNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotLegendNode)

    // === 位置属性 ===
    Q_PROPERTY(QImPlotLegendLocation location READ location WRITE setLocation NOTIFY locationChanged)

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
    /**
     * \if ENGLISH
     * @brief Emitted when legend location changes
     * @param[in] v The new legend location value
     * @details Triggered by setLocation() when value actually changes.
     * \endif
     *
     * \if CHINESE
     * @brief 图例位置更改时触发
     * @param[in] v 新图例位置值
     * @details 当值实际更改时由setLocation()触发。
     * \endif
     */
    void locationChanged(QIM::QImPlotLegendLocation v);

    /**
     * \if ENGLISH
     * @brief Emitted when any legend flag changes
     * @details Triggered by any flag property setter (setOrientation, setOutside,
     *          setButtonEnabled, setHighlightItemEnabled, setHighlightAxisEnabled,
     *          setSort, setReverse) when value actually changes.
     * \endif
     *
     * \if CHINESE
     * @brief 图例标志更改时触发
     * @details 任何标志属性setter（setOrientation、setOutside、
     *          setButtonEnabled、setHighlightItemEnabled、setHighlightAxisEnabled、
     *          setSort、setReverse）当值实际更改时触发。
     * \endif
     */
    void legendFlagChanged();

protected:
    bool beginDraw() override;
};

}  // namespace QIM


#endif  // QIMPLOTLEGENDNODE_H
