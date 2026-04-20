#ifndef QIMPLOTVALUETRACKERNODE_H
#define QIMPLOTVALUETRACKERNODE_H
#include "QImAbstractNode.h"
#include <QColor>
namespace QIM
{
class QImPlotNode;
class QImPlotValueTrackerNodeGroup;

/**
 * \if ENGLISH
 * @brief Smart value tracker node for ImPlot
 * @details Provides an intelligent value tracking overlay that displays crosshair-style
 *          annotations at the nearest data point to the mouse cursor. Automatically
 *          tracks all visible plot items within the parent QImPlotNode, extracting
 *          label, color, and Y value information for real-time tooltip rendering.
 *          Supports customizable tooltip style (width, text/background/border colors)
 *          and can join a QImPlotValueTrackerNodeGroup for synchronized multi-plot tracking.
 * @note Tracker rendering occurs within the BeginPlot/EndPlot block of its parent plot node.
 * @see QImPlotValueTrackerNodeGroup, QImPlotNode
 * \endif
 *
 * \if CHINESE
 * @brief 智能值追踪器节点
 * @details 提供智能值追踪覆盖层，在鼠标光标最近的数据点处显示十字线样式的标注。
 *          自动追踪父QImPlotNode中所有可见的绘图项目，提取标签、颜色和Y值信息
 *          用于实时提示框渲染。支持可自定义提示框样式（宽度、文字/背景/边框颜色），
 *          并可加入QImPlotValueTrackerNodeGroup实现多子图联动追踪。
 * @note 追踪器渲染在其父绘图节点的BeginPlot/EndPlot块内进行。
 * @see QImPlotValueTrackerNodeGroup, QImPlotNode
 * \endif
 */
class QIM_CORE_API QImPlotValueTrackerNode : public QImAbstractNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotValueTrackerNode)
public:
    // Tracked value at a specific X position
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
    // Group
    void setGroup(QImPlotValueTrackerNodeGroup* group);
    bool hasGroup() const;
    QImPlotValueTrackerNodeGroup* group() const;
    // Style settings
    // Width
    void setFixedWidth(float width);
    float fixedWidth() const;
    // Enable auto-width calculation
    void setAutoWidthEnabled(bool on);
    bool isAutoWidthEnabled() const;
    // Text color
    void setTextColor(const QColor& color);
    QColor textColor() const;
    // Background color
    void setBackgroundColor(const QColor& color);
    QColor backgroundColor() const;
    // Border color
    void setBorderColor(const QColor& color);
    QColor borderColor() const;
    // Tracker line color
    void setTrackerLineColor(const QColor& color);
    QColor trackerLineColor() const;
    // Skip NaN/finite values
    void setSkipNanFiniteValues(bool on);
    bool isSkipNanFiniteValues() const;
    //

Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when tracker active state changes
     * @param[in] on The new active state
     * @details Triggered when the tracker becomes active or inactive.
     * \endif
     *
     * \if CHINESE
     * @brief 追踪器激活状态更改时触发
     * @param[in] on 新激活状态
     * @details 当追踪器变为激活或非激活状态时触发。
     * \endif
     */
    void activeChanged(bool on);

protected:
    bool beginDraw() override;
    virtual void renderTooltip(const std::vector< TrackedValue >& values, const QPointF& mouseScreenPos);
private Q_SLOTS:
    void onChildNodeAdded(QIM::QImAbstractNode* n);
    void onChildNodeRemoved(QIM::QImAbstractNode* n);

};
}

#endif  // QIMPLOTVALUETRACKERNODE_H
