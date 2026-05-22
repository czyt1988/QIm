#pragma once

#include "collector/ProcessInfo.h"
#include "core/ColorPalette.h"
#include "aggregator/SustainedMetricsTracker.h"
#include <QByteArray>
#include <QHash>
#include <QList>
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
    void setRankingTopN(int n);           ///< Set number of processes shown in ranking chart
    int rankingTopN() const;              ///< Get current ranking display count
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
    QList<double> m_tickPositions;          ///< Y-axis tick positions for ranking chart
    QList<QByteArray> m_tickLabels;         ///< Y-axis tick labels (process names, UTF-8)
    QImSystemMonitor::ColorManager colorManager_;
    SustainedMetricsTracker* tracker_ = nullptr;
    SustainedMetricSelector* metricSelector_ = nullptr;
    SustainedMetric currentMetric_ = SustainedMetric::CpuTime;

    static constexpr int kWindowDurationSec = 600;
    static constexpr int kTimelineTopN = 8;
    int rankingTopN_ = 10;          ///< Number of processes shown in ranking, user-adjustable
};