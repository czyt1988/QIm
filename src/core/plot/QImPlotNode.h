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
 * @brief ImPlot 绘图区域节点
 *
 * 负责管理单个 plot 的生命周期、坐标轴配置和渲染上下文
 *
 * 渲染流程（严格遵循 ImPlot 约束）：
 * 1. BeginPlot() - 创建绘图上下文
 * 2. SetupAxes() - 必须在首个绘图调用前设置坐标轴
 * 3. 子节点渲染（Series 元素）
 * 4. EndPlot()
 *
 */
class QIM_CORE_API QImPlotNode : public QImAbstractNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotNode)

    // == 标题属性 ==
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    // === 尺寸属性 ===
    Q_PROPERTY(QSizeF size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(bool autoSize READ isAutoSize WRITE setAutoSize NOTIFY autoSizeChanged)
    // ImPlotFlags相关
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
    // impl
    Q_DISABLE_COPY(QImPlotNode)

public:
    explicit QImPlotNode(QObject* parent = nullptr);
    explicit QImPlotNode(const QString& title, QObject* parent = nullptr);
    ~QImPlotNode() override;

    //----------------------------------------------------
    // 标题
    //----------------------------------------------------
    QString title() const;
    void setTitle(const QString& title);

    //----------------------------------------------------
    // 尺寸
    //----------------------------------------------------
    QSizeF size() const;
    void setSize(const QSizeF& size);

    bool isAutoSize() const;
    void setAutoSize(bool autoSize);
    //----------------------------------------------------
    // 坐标轴相关
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
    // ImPlotFlags相关
    //----------------------------------------------------
    // 标志访问器（肯定语义）
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

    // 原始标志访问
    int imPlotFlags() const;
    void setImPlotFlags(int flags);
    //----------------------------------------------------
    // 添加绘图
    //----------------------------------------------------
    void addPlotItem(QImPlotItemNode* item);
    // 获取所有绘图item
    QList< QImPlotItemNode* > plotItemNodes() const;
    //----------------------------------------------------
    // 快速函数
    //----------------------------------------------------
    void addLine(QImPlotLineItemNode* lineItem);
    template< typename ContainerX, typename ContainerY >
    QImPlotLineItemNode* addLine(const ContainerX& x, const ContainerY& y, const QString& label);
    //===============================================================
    // Legend
    //===============================================================
    QImPlotLegendNode* legendNode() const;
    //----------------------------------------------------
    // 交互 注意，这些函数需要确保在开始绘制后调用
    //----------------------------------------------------
    bool isPlotHovered() const;
    // 屏幕到绘图坐标的转换
    QPointF pixelsToPlot(const float& screenX, const float& screenY);
    QPointF plotToPixels(const double& doubleX, const double& doubleY);
    // 坐标轴上渲染的文字内容，注意此函数一定只能在当前绘图的beginDraw内部使用
    std::string axisValueText(double val, QImPlotAxisId axisId) const;
Q_SIGNALS:
    void titleChanged(const QString& title);
    void sizeChanged(const QSizeF& size);
    void autoSizeChanged(bool autoSize);
    void plotFlagChanged();

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
