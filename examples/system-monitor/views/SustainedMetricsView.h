#pragma once

#include "collector/ProcessInfo.h"
#include "core/ColorPalette.h"
#include "aggregator/SustainedMetricsTracker.h"
#include <QHash>
#include <QStringList>

namespace QIM {
class QImFigureWidget;
class QImPlotNode;
class QImPlotLineItemNode;
class QImPlotBarGroupsItemNode;
}

class SustainedMetricSelector;

class SustainedMetricsView {
public:
    void setTracker(SustainedMetricsTracker* tracker);
    void setMetricSelector(SustainedMetricSelector* selector);
    void buildView(QIM::QImFigureWidget* figure, const QList<AggregatedProcessInfo>& data);
    void updateData(const QList<AggregatedProcessInfo>& data);
    void resetAccumulation();

private:
    static QString metricTitle(SustainedMetric metric);
    static QString metricUnit(SustainedMetric metric);

    QIM::QImPlotNode* rankingPlot_ = nullptr;
    QIM::QImPlotNode* timelinePlot_ = nullptr;
    QIM::QImPlotBarGroupsItemNode* rankingBars_ = nullptr;
    QHash<QString, QIM::QImPlotLineItemNode*> timelineLines_;
    QStringList orderedNames_;
    QImSystemMonitor::ColorManager colorManager_;
    SustainedMetricsTracker* tracker_ = nullptr;
    SustainedMetricSelector* metricSelector_ = nullptr;
    SustainedMetric currentMetric_ = SustainedMetric::CpuTime;

    static constexpr int kWindowDurationSec = 600;
    static constexpr int kTopN = 8;
};