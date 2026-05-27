#ifndef QIMPLOTSELECTIONNODE_H
#define QIMPLOTSELECTIONNODE_H

#include "../../QImAPI.h"
#ifndef QIM_CORE_API
#define QIM_CORE_API
#endif
#include <QRectF>
#include "QImAbstractNode.h"
#include "QImPlot.h"

namespace QIM
{

class QImPlotNode;

/**
 * \if ENGLISH
 * @brief Box selection observer node for ImPlot
 * @details Provides Qt-style retained mode encapsulation for ImPlot's built-in box selection.
 *          The node observes ImPlot's native selection state (IsPlotSelected/GetPlotSelection)
 *          and exposes it as Qt properties and signals. xAxis/yAxis control which axes are used
 *          for pixel-to-data coordinate conversion in GetPlotSelection queries.
 *          This node inherits QImAbstractNode because selection is a
 *          plot-level observation, not a per-item drawing primitive.
 * @note Rendering occurs within the BeginPlot/EndPlot block of the parent QImPlotNode.
 * @see QImPlotNode, QImAbstractNode
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot框选观察器节点
 * @details 为ImPlot内置的框选功能提供Qt风格的保留模式封装。
 *          该节点观察ImPlot的原生选择状态（IsPlotSelected/GetPlotSelection），
 *          并将其暴露为Qt属性和信号。xAxis/yAxis控制GetPlotSelection查询中
 *          用于像素到数据坐标转换的轴。此节点继承QImAbstractNode，
 *          因为框选是绘图级别的观察，而非绘图项。
 * @note 渲染在其父QImPlotNode的BeginPlot/EndPlot块内进行。
 * @see QImPlotNode, QImAbstractNode
 * \endif
 */
class QIM_CORE_API QImPlotSelectionNode : public QImAbstractNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotSelectionNode)
    Q_DISABLE_COPY(QImPlotSelectionNode)

    Q_PROPERTY(QImPlotAxisId xAxis READ xAxis WRITE setXAxis NOTIFY axisChanged)
    Q_PROPERTY(QImPlotAxisId yAxis READ yAxis WRITE setYAxis NOTIFY axisChanged)
    Q_PROPERTY(QRectF selection READ selection NOTIFY selectionChanged)
    Q_PROPERTY(bool selected READ isSelected NOTIFY selectionActiveChanged)
    Q_PROPERTY(bool autoCancel READ isAutoCancel WRITE setAutoCancel NOTIFY autoCancelChanged)

public:
    explicit QImPlotSelectionNode(QImPlotNode* plotNode, QObject* parent = nullptr);
    ~QImPlotSelectionNode() override;

    // X axis used for selection coordinate query
    QImPlotAxisId xAxis() const;
    void setXAxis(QImPlotAxisId axis);

    // Y axis used for selection coordinate query
    QImPlotAxisId yAxis() const;
    void setYAxis(QImPlotAxisId axis);

    // Current selection rectangle in plot data coordinates (read-only)
    QRectF selection() const;

    // Whether a box selection is currently active (read-only)
    bool isSelected() const;

    // Auto-cancel selection on SelectCancel button click (default: true)
    bool isAutoCancel() const;
    void setAutoCancel(bool enabled);

    // Defer CancelPlotSelection to next beginDraw (requires BeginPlot/EndPlot scope)
    void cancelSelection();

Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when a box selection is completed
     * @param[in] selection The selection rectangle in plot coordinates
     * @details Triggered on the frame where IsPlotSelected() returns true.
     *          The rectangle coordinates are in the plot data space defined by xAxis/yAxis.
     * \endif
     *
     * \if CHINESE
     * @brief 框选完成时触发
     * @param[in] selection 绘图坐标系中的框选矩形
     * @details 在IsPlotSelected()返回true的帧上触发。
     *          矩形坐标位于由xAxis/yAxis定义的绘图数据空间中。
     * \endif
     */
    void selectionChanged(const QRectF& selection);

    /**
     * \if ENGLISH
     * @brief Emitted when selection active state changes
     * @param[in] selected true when a selection is active
     * \endif
     *
     * \if CHINESE
     * @brief 框选激活状态更改时触发
     * @param[in] selected 为true时表示框选处于激活状态
     * \endif
     */
    void selectionActiveChanged(bool selected);

    /**
     * \if ENGLISH
     * @brief Emitted when xAxis or yAxis changes
     * \endif
     *
     * \if CHINESE
     * @brief xAxis或yAxis更改时触发
     * \endif
     */
    void axisChanged();

    /**
     * \if ENGLISH
     * @brief Emitted when autoCancel is toggled
     * @param[in] autoCancel New autoCancel state
     * \endif
     *
     * \if CHINESE
     * @brief autoCancel切换时触发
     * @param[in] autoCancel 新的autoCancel状态
     * \endif
     */
    void autoCancelChanged(bool autoCancel);

protected:
    bool beginDraw() override;
};

}  // namespace QIM
#endif  // QIMPLOTSELECTIONNODE_H