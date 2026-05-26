#pragma once

#include "widgets/ViewModeSelector.h"
#include "collector/ProcessInfo.h"

#include <QObject>
#include <QList>

namespace QIM {
class QImFigureWidget;
}

class CpuUsageView;
class ResourcePieView;
class ProcessResource3DView;
class SystemMetricsView;
class SustainedMetricsView;
class HistoryBuffer;
class SustainedMetricsTracker;
class SustainedMetricSelector;

class ViewManager : public QObject {
    Q_OBJECT
public:
    explicit ViewManager(QIM::QImFigureWidget* figure, QObject* parent = nullptr);
    ~ViewManager() override;

    void switchTo(ViewMode mode, const QList<AggregatedProcessInfo>& data);
    void updateCurrentView(const QList<AggregatedProcessInfo>& data);
    void setHistoryBuffer(HistoryBuffer* buffer);
    void setSustainedMetricsTracker(SustainedMetricsTracker* tracker);
    void setSustainedMetricSelector(SustainedMetricSelector* selector);

Q_SIGNALS:
    void viewSwitched(ViewMode mode);

private:
    QIM::QImFigureWidget* figure_;

    CpuUsageView* cpuUsageView_ = nullptr;
    ResourcePieView* resourcePieView_ = nullptr;
    ProcessResource3DView* processResource3DView_ = nullptr;
    SystemMetricsView* systemMetricsView_ = nullptr;
    SustainedMetricsView* sustainedMetricsView_ = nullptr;
    SustainedMetricsTracker* sustainedTracker_ = nullptr;

    ViewMode currentMode_ = static_cast<ViewMode>(-1);
    bool viewBuilt_ = false;
};