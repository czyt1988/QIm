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
 * @brief 值追踪器组 - 管理多个QImPlotValueTrackerNode的联动
 *
 * 使用示例：
 * @code
 * auto group = new QImPlotValueTrackerNodeGroup();
 * group->setSyncMode(QImPlotValueTrackerNodeGroup::SyncMode::Value); // 值同步
 *
 * tracker1->setGroup(group);
 * tracker2->setGroup(group);
 * @endcode
 */
class QIM_CORE_API QImPlotValueTrackerNodeGroup : public QObject
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotValueTrackerNodeGroup)
public:
    enum class SyncMode
    {
        Pixel  ///< 像素同步：所有追踪器在各自plot中显示相同像素比例位置
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
    // 供 Tracker 在绘制时查询同步状态
    bool getSyncState(float& outPixelRatio, SyncMode& outMode) const;
};

}  // namespace QIM

#endif  // QIMPLOTVALUETRACKERGROUP_H
