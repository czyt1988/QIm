#ifndef QIMPLOTNODE_H
#define QIMPLOTNODE_H
#include "QImAbstractNode.h"
#include <QSizeF>
#include <QRectF>
#include <QColor>
#include "QImPlot.h"
#include "QImPlotLineItemNode.h"
namespace QIM
{
class QImPlotAxisInfo;
class QImPlotItemNode;
class QImPlotLegendNode;
class QImPlotLineItemNode;
/**
 * \if ENGLISH
 * @brief ImPlot plot area node
 * @details Manages the lifecycle, axis configuration, and rendering context for a
 *          single ImPlot plot area within the QIm object tree. Provides Qt-style
 *          properties for plot flags (title, legend, mouse text, inputs, menus,
 *          box select, frame, equal aspect, crosshairs, canvas), and convenience
 *          methods for adding plot items and axis manipulation.
 *
 *          Rendering flow (strictly follows ImPlot constraints):
 *          1. BeginPlot() - Creates the plot context
 *          2. SetupAxes() - Must set axes before the first plot item call
 *          3. Child node rendering (Series elements)
 *          4. EndPlot()
 *
 * @note Uses the PIMPL pattern via QIM_DECLARE_PRIVATE for encapsulation.
 * @see QImPlotItemNode, QImPlotAxisInfo, QImPlotLegendNode
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot 绘图区域节点
 * @details 在QIm对象树中管理单个ImPlot绘图区域的生命周期、坐标轴配置和渲染上下文。
 *          为ImPlot标志提供Qt风格属性（标题、图例、鼠标文本、输入、菜单、
 *          框选、边框、等比例、十字线、画布），以及添加绘图项目和坐标轴操作的便捷方法。
 *
 *          渲染流程（严格遵循 ImPlot 约束）：
 *          1. BeginPlot() - 创建绘图上下文
 *          2. SetupAxes() - 必须在首个绘图调用前设置坐标轴
 *          3. 子节点渲染（Series 元素）
 *          4. EndPlot()
 *
 * @note 通过QIM_DECLARE_PRIVATE采用PIMPL模式实现封装。
 * @see QImPlotItemNode, QImPlotAxisInfo, QImPlotLegendNode
 * \endif
 */
class QIM_CORE_API QImPlotNode : public QImAbstractNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotNode)

    // == Title properties ==
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    // === Size properties ===
    Q_PROPERTY(QSizeF size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(bool autoSize READ isAutoSize WRITE setAutoSize NOTIFY autoSizeChanged)
    // ImPlotFlags
    Q_PROPERTY(bool titleEnabled READ isTitleEnabled WRITE setTitleEnabled NOTIFY plotFlagChanged)
    Q_PROPERTY(bool legendEnabled READ isLegendEnabled WRITE setLegendEnabled NOTIFY plotFlagChanged)
    Q_PROPERTY(bool mouseTextEnabled READ isMouseTextEnabled WRITE setMouseTextEnabled NOTIFY plotFlagChanged)
    Q_PROPERTY(bool inputsEnabled READ isInputsEnabled WRITE setInputsEnabled NOTIFY plotFlagChanged)
    Q_PROPERTY(bool menusEnabled READ isMenusEnabled WRITE setMenusEnabled NOTIFY plotFlagChanged)
    Q_PROPERTY(bool boxSelectEnabled READ isBoxSelectEnabled WRITE setBoxSelectEnabled NOTIFY plotFlagChanged)
    Q_PROPERTY(bool frameEnabled READ isFrameEnabled WRITE setFrameEnabled NOTIFY plotFlagChanged)
    Q_PROPERTY(bool equal READ isEqual WRITE setEqual NOTIFY plotFlagChanged)
    Q_PROPERTY(bool crosshairs READ isCrosshairs WRITE setCrosshairs NOTIFY plotFlagChanged)
    Q_PROPERTY(bool canvasEnabled READ isCanvasEnabled WRITE setCanvasEnabled NOTIFY plotFlagChanged)
    // Runtime mouse state
    Q_PROPERTY(bool isHovered READ isHovered NOTIFY isHoveredChanged)
    // impl
    Q_DISABLE_COPY(QImPlotNode)

public:
    explicit QImPlotNode(QObject* parent = nullptr);
    explicit QImPlotNode(const QString& title, QObject* parent = nullptr);
    ~QImPlotNode() override;

    //----------------------------------------------------
    // Title
    //----------------------------------------------------
    QString title() const;
    void setTitle(const QString& title);

    //----------------------------------------------------
    // Size
    //----------------------------------------------------
    QSizeF size() const;
    void setSize(const QSizeF& size);

    bool isAutoSize() const;
    void setAutoSize(bool autoSize);
    //----------------------------------------------------
    // Axes
    //----------------------------------------------------
    QImPlotAxisInfo* axisInfo(QImPlotAxisId aid) const;
    QImPlotAxisInfo* x1Axis() const;
    QImPlotAxisInfo* y1Axis() const;
    QImPlotAxisInfo* x2Axis() const;
    QImPlotAxisInfo* y2Axis() const;
    QImPlotAxisInfo* x3Axis() const;
    QImPlotAxisInfo* y3Axis() const;
    bool isAxisEnabled(QImPlotAxisId aid) const;
    void setAxisEnabled(QImPlotAxisId aid, bool on);
    //----------------------------------------------------
    // ImPlotFlags
    //----------------------------------------------------
    // Flag accessors (affirmative semantics)
    bool isTitleEnabled() const;
    void setTitleEnabled(bool enabled);

    bool isLegendEnabled() const;
    void setLegendEnabled(bool enabled);

    bool isMouseTextEnabled() const;
    void setMouseTextEnabled(bool enabled);

    bool isInputsEnabled() const;
    void setInputsEnabled(bool enabled);

    bool isMenusEnabled() const;
    void setMenusEnabled(bool enabled);

    bool isBoxSelectEnabled() const;
    void setBoxSelectEnabled(bool enabled);

    bool isFrameEnabled() const;
    void setFrameEnabled(bool enabled);

    bool isEqual() const;
    void setEqual(bool on);

    bool isCrosshairs() const;
    void setCrosshairs(bool on);

    bool isCanvasEnabled() const;
    void setCanvasEnabled(bool enabled);

    // Raw flag access
    int imPlotFlags() const;
    void setImPlotFlags(int flags);
    //----------------------------------------------------
    // Plot items
    //----------------------------------------------------
    void addPlotItem(QImPlotItemNode* item);
    // Get all plot items
    QList< QImPlotItemNode* > plotItemNodes() const;
    //----------------------------------------------------
    // Convenience functions
    //----------------------------------------------------
    void addLine(QImPlotLineItemNode* lineItem);
    template< typename ContainerX, typename ContainerY >
    QImPlotLineItemNode* addLine(const ContainerX& x, const ContainerY& y, const QString& label);
    //===============================================================
    // Legend
    //===============================================================
    QImPlotLegendNode* legendNode() const;
    //----------------------------------------------------
    // Interaction (must be called after beginDraw)
    //----------------------------------------------------
    // Runtime mouse state (updated per-frame in beginDraw)
    bool isHovered() const;
    /// @deprecated Use isHovered() instead
    bool isPlotHovered() const;
    // Screen to plot coordinate conversion
    QPointF pixelsToPlot(const float& screenX, const float& screenY);
    QPointF plotToPixels(const double& doubleX, const double& doubleY);
    // Axis label text (must only be used inside beginDraw of current plot)
    std::string axisValueText(double val, QImPlotAxisId axisId) const;
    // Auto-fit axes to show all curves
    void rescaleAxes();
    void setAxesToFit();
    //----------------------------------------------------
    // Colormap push/pop
    //----------------------------------------------------

    // Pushes a colormap onto the colormap stack by enum (applied in beginDraw)
    void pushColormap(QImPlotColormap colormap);

    // Pushes a colormap onto the colormap stack by name (applied in beginDraw)
    void pushColormap(const QByteArray& name);

    // Pops colormaps from the stack (applied in endDraw)
    void popColormap(int count = 1);
Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when title changes
     * @param[in] title The new title value
     * @details Triggered by setTitle() when value actually changes.
     * \endif
     *
     * \if CHINESE
     * @brief 标题更改时触发
     * @param[in] title 新标题值
     * @details 当值实际更改时由setTitle()触发。
     * \endif
     */
    void titleChanged(const QString& title);

    /**
     * \if ENGLISH
     * @brief Emitted when size changes
     * @param[in] size The new size value
     * @details Triggered by setSize() when value actually changes.
     * \endif
     *
     * \if CHINESE
     * @brief 尺寸更改时触发
     * @param[in] size 新尺寸值
     * @details 当值实际更改时由setSize()触发。
     * \endif
     */
    void sizeChanged(const QSizeF& size);

    /**
     * \if ENGLISH
     * @brief Emitted when autoSize changes
     * @param[in] autoSize The new autoSize state
     * @details Triggered by setAutoSize() when value actually changes.
     * \endif
     *
     * \if CHINESE
     * @brief 自适应尺寸更改时触发
     * @param[in] autoSize 新自适应尺寸状态
     * @details 当值实际更改时由setAutoSize()触发。
     * \endif
     */
    void autoSizeChanged(bool autoSize);

    /**
     * \if ENGLISH
     * @brief Emitted when any ImPlot flag changes
     * @details Triggered by any flag property setter (setTitleEnabled, setLegendEnabled, etc.)
     *          when value actually changes.
     * \endif
     *
     * \if CHINESE
     * @brief ImPlot标志更改时触发
     * @details 任何标志属性setter（setTitleEnabled、setLegendEnabled等）
     *          当值实际更改时触发。
     * \endif
     */
    void plotFlagChanged();

    /**
     * \if ENGLISH
     * @brief Emitted when plot viewport limits change
     * @details Triggered when the user pans or zooms the plot, causing the visible axis ranges
     *          to change. Uses epsilon-based comparison to avoid false triggers from
     *          floating-point jitter.
     * \endif
     *
     * \if CHINESE
     * @brief 绘图视口范围更改时触发
     * @details 当用户平移或缩放绘图导致可见坐标轴范围更改时触发。
     *          使用基于epsilon的比较避免浮点抖动引发误触发。
     * \endif
     */
    void plotLimitsChanged();

    /**
     * \if ENGLISH
     * @brief Emitted when the mouse hover state over the plot area changes
     * @param[in] isHovered true if mouse cursor is currently over the plot area
     * @details Updated per-frame during beginDraw() via ImPlot::IsPlotHovered().
     *          Only emitted when the state actually toggles between hovered and unhovered.
     * \endif
     *
     * \if CHINESE
     * @brief 鼠标悬停绘图区域状态更改时触发
     * @param[in] isHovered true 表示鼠标光标当前位于绘图区域上方
     * @details 每帧在 beginDraw() 中通过 ImPlot::IsPlotHovered() 更新。
     *          仅在悬停/非悬停状态实际切换时触发。
     * \endif
     */
    void isHoveredChanged(bool isHovered);
protected:
    bool beginDraw() override;
    void endDraw() override;
};

template< typename ContainerX, typename ContainerY >
inline QImPlotLineItemNode* QImPlotNode::addLine(const ContainerX& x, const ContainerY& y, const QString& label)
{
    QImPlotLineItemNode* line = new QImPlotLineItemNode();
    line->setData(x, y);
    line->setLabel(label);
    addPlotItem(line);
    return line;
}

}
#endif
