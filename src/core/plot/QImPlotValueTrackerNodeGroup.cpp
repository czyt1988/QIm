#include "QImPlotValueTrackerNodeGroup.h"
#include "QImPlotValueTrackerNode.h"

namespace QIM
{
class QImPlotValueTrackerNodeGroup::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlotValueTrackerNodeGroup)
public:
    PrivateData(QImPlotValueTrackerNodeGroup* p);

public:
    QList< QImPlotValueTrackerNode* > trackers;
    SyncMode syncMode { SyncMode::Pixel };
    float pixelRatio { 0.0f };  ///< x比例
    QImPlotValueTrackerNode* activeTracker { nullptr };
};

QImPlotValueTrackerNodeGroup::PrivateData::PrivateData(QImPlotValueTrackerNodeGroup* p) : q_ptr(p)
{
}

//===============================================================
// QImPlotValueTrackerNodeGroup
//===============================================================
QImPlotValueTrackerNodeGroup::QImPlotValueTrackerNodeGroup(QObject* parent) : QObject(parent), QIM_PIMPL_CONSTRUCT
{
}

QImPlotValueTrackerNodeGroup::~QImPlotValueTrackerNodeGroup()
{
    // 从所有追踪器中移除组引用
    for (auto tracker : std::as_const(d_ptr->trackers)) {
        if (tracker) {
            tracker->setGroup(nullptr);
        }
    }
}

void QImPlotValueTrackerNodeGroup::addTracker(QImPlotValueTrackerNode* tracker)
{
    if (!tracker || d_ptr->trackers.contains(tracker)) {
        return;
    }

    d_ptr->trackers.append(tracker);
}

void QImPlotValueTrackerNodeGroup::removeTracker(QImPlotValueTrackerNode* tracker)
{
    QIM_D(d);
    d->trackers.removeAll(tracker);

    // 如果移除的是活跃 tracker，重置状态
    if (d->activeTracker == tracker) {
        d->activeTracker = nullptr;
        d->pixelRatio    = 0.0f;
    }
}

QImPlotValueTrackerNodeGroup::SyncMode QImPlotValueTrackerNodeGroup::syncMode() const
{
    return d_ptr->syncMode;
}

void QImPlotValueTrackerNodeGroup::setSyncMode(SyncMode mode)
{
    QIM_D(d);
    if (d->syncMode == mode) {
        return;
    }
    d->syncMode = mode;
    // 无需触发任何操作！Tracker 在下次 beginDraw 时会自动使用新同步模式
}

bool QImPlotValueTrackerNodeGroup::isActive() const
{
    return d_ptr->activeTracker != nullptr;
}

float QImPlotValueTrackerNodeGroup::pixelRatio() const
{
    return d_ptr->pixelRatio;
}

void QImPlotValueTrackerNodeGroup::updateActiveTracker(QImPlotValueTrackerNode* activeTracker, float pixelRatio)
{
    d_ptr->activeTracker = activeTracker;
    d_ptr->pixelRatio    = pixelRatio;
}

bool QImPlotValueTrackerNodeGroup::getSyncState(float& outPixelRatio, SyncMode& outMode) const
{
    QIM_DC(d);
    if (!d->activeTracker) {
        return false;
    }

    outPixelRatio = d->pixelRatio;
    outMode       = d->syncMode;
    return true;
}

}  // namespace QIM
