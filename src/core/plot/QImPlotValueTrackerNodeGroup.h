#ifndef QIMPLOTVALUETRACKERGROUP_H
#define QIMPLOTVALUETRACKERGROUP_H
#include "QImAPI.h"
#include <QObject>
#include <QList>
#include <QPointer>

namespace QIM
{

class QImPlotValueTrackerNode;

/**
 * \if ENGLISH
 * @brief Value tracker node group for synchronized multi-plot tracking
 * @details Manages a group of QImPlotValueTrackerNode instances to enable synchronized
 *          cursor tracking across multiple subplots. When the mouse moves in one plot,
 *          all trackers in the group update their crosshair positions at the same X value,
 *          providing a unified data inspection experience across the figure.
 *          Supports pixel-ratio and value-ratio synchronization modes.
 * @note Trackers must belong to different QImPlotNode instances; grouping trackers
 *       in the same plot has no additional effect.
 * @see QImPlotValueTrackerNode, QImPlotNode
 * \endif
 *
 * \if CHINESE
 * @brief 值追踪器组
 * @details 管理一组QImPlotValueTrackerNode实例，实现多个子图之间的联动光标追踪。
 *          当鼠标在某个子图移动时，组内所有追踪器在相同的X值处更新十字线位置，
 *          提供跨图窗的统一数据检查体验。支持像素比例和数值比例同步模式。
 * @note 追踪器必须属于不同的QImPlotNode实例；同一绘图中的追踪器分组无额外效果。
 * @see QImPlotValueTrackerNode, QImPlotNode
 * \endif
 */
class QIM_CORE_API QImPlotValueTrackerNodeGroup : public QObject
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotValueTrackerNodeGroup)
public:
    enum class SyncMode
    {
        Pixel  ///< Pixel sync: trackers show same pixel ratio position in respective plots
    };

    explicit QImPlotValueTrackerNodeGroup(QObject* parent = nullptr);
    ~QImPlotValueTrackerNodeGroup();

    void addTracker(QImPlotValueTrackerNode* tracker);
    void removeTracker(QImPlotValueTrackerNode* tracker);

    SyncMode syncMode() const;
    void setSyncMode(SyncMode mode);

    bool isActive() const;

    float pixelRatio() const;

    void updateActiveTracker(QImPlotValueTrackerNode* activeTracker, float pixelRatio);
    // Query sync state for tracker during rendering
    bool getSyncState(float& outPixelRatio, SyncMode& outMode) const;
};

}  // namespace QIM

#endif  // QIMPLOTVALUETRACKERGROUP_H
