#ifndef QIMPLOTITEMNODE_H
#define QIMPLOTITEMNODE_H
#include "QImAbstractNode.h"
#include "QImPlot.h"

struct ImPlotItem;


namespace QIM
{

// ============================================================
// Type alias definitions - simplifies Plot Item color property declarations
// ============================================================



class QImPlotNode;
/**
 * \if ENGLISH
 * @brief Base class for all ImPlot plot item nodes
 * @details Provides the foundational QObject-based node for all plot visualization
 *          items (lines, bars, scatter, etc.) within the QIm object tree. Manages
 *          common properties such as label, axis binding, visibility, and color.
 *          Each plot item is associated with a QImPlotNode parent that provides
 *          the rendering context (BeginPlot/EndPlot block).
 *          Uses the PIMPL pattern via QIM_DECLARE_PRIVATE for encapsulation.
 * @note Subclasses must implement type() for fast runtime type identification
 *       without dynamic_cast overhead.
 * @see QImPlotNode, QImPlotLineItemNode, QImPlotBarsItemNode
 * \endif
 *
 * \if CHINESE
 * @brief PlotItem对应的基类
 * @details 为QIm对象树中所有绘图可视化项目（折线、柱状、散点等）提供基于QObject的基础节点。
 *          管理通用属性如标签、坐标轴绑定、可见性和颜色。
 *          每个绘图项目关联一个QImPlotNode父节点，后者提供渲染上下文（BeginPlot/EndPlot块）。
 *          通过QIM_DECLARE_PRIVATE采用PIMPL模式实现封装。
 * @note 子类必须实现type()以实现快速运行时类型识别，避免dynamic_cast开销。
 * @see QImPlotNode, QImPlotLineItemNode, QImPlotBarsItemNode
 * \endif
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
    // Quick type identification to avoid qobject_cast/dynamic_cast overhead
    virtual int type() const = 0;
    //
    void setLabel(const QString& name);
    QString label() const;
    const char* labelConstData() const;
    // Bind axes
    void bindAxis(QImPlotAxisId x, QImPlotAxisId y);
    // Screen to plot coordinate conversion
    QPointF pixelsToPlot(const float& screenX, const float& screenY);
    QPointF plotToPixels(const double& doubleX, const double& doubleY);
    // Bound X axis ID
    QImPlotAxisId xAxisId() const;
    // Bound Y axis ID
    QImPlotAxisId yAxisId() const;
    // Get plot node
    QImPlotNode* plotNode() const;
    // Item color
    QColor itemColor() const;
    // Whether hovered in legend
    bool isLegendHovered() const;
    //
    virtual bool isVisible() const override;
    virtual void setVisible(bool visible) override;

    /**
     * \if ENGLISH
     * @brief Calculate optimal target points from plot pixel width
     * @param pixelWidth Plot width in pixels (from ImPlot::GetPlotSize().x)
     * @return Clamped target point count: max(100, min(pixelWidth * kPixelToPointRatio, 10000))
     * \endif
     *
     * \if CHINESE
     * @brief 根据绘图像素宽度计算最佳降采样目标点数
     * @param pixelWidth 以像素为单位的绘图宽度（来自 ImPlot::GetPlotSize().x）
     * @return 钳位后的目标点数：max(100, min(pixelWidth * kPixelToPointRatio, 10000))
     * \endif
     */
    static constexpr double kPixelToPointRatio = 1.5;
    static int pixelAwareTargetPoints(int pixelWidth);
Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when label changes
     * @param[in] name The new label value
     * @details Triggered by setLabel() when value actually changes.
     * \endif
     *
     * \if CHINESE
     * @brief 标签更改时触发
     * @param[in] name 新标签值
     * @details 当值实际更改时由setLabel()触发。
     * \endif
     */
    void labelChanged(const QString& name);

protected:
    virtual void endDraw() override;
    // ImPlotItem operations
    ImPlotItem* imPlotItem() const;
    void setImPlotItem(ImPlotItem* item);
};
}  // end namespace QIM

#endif  // QIMPLOTITEMNODE_H
