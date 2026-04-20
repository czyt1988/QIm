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
    QIM_D(d);
    // 从所有追踪器中移除组引用
    for (auto tracker : std::as_const(d->trackers)) {
        if (tracker) {
            tracker->setGroup(nullptr);
        }
    }
}

void QImPlotValueTrackerNodeGroup::addTracker(QImPlotValueTrackerNode* tracker)
{
    QIM_D(d);
    if (!tracker || d->trackers.contains(tracker)) {
        return;
    }

    d->trackers.append(tracker);
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
    QIM_DC(d);
    return d->syncMode;
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
    QIM_DC(d);
    return d->activeTracker != nullptr;
}

float QImPlotValueTrackerNodeGroup::pixelRatio() const
{
    QIM_DC(d);
    return d->pixelRatio;
}

void QImPlotValueTrackerNodeGroup::updateActiveTracker(QImPlotValueTrackerNode* activeTracker, float pixelRatio)
{
    QIM_D(d);
    d->activeTracker = activeTracker;
    d->pixelRatio    = pixelRatio;
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
