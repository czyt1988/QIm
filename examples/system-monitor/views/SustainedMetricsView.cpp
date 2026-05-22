#include "SustainedMetricsView.h"

#include <algorithm>
#include <vector>

#include "QImFigureWidget.h"
#include "plot/QImPlotNode.h"
#include "plot/QImPlotLineItemNode.h"
#include "plot/QImPlotBarGroupsItemNode.h"
#include "plot/QImPlotLegendNode.h"
#include "plot/QImPlotAxisInfo.h"
#include "plot/QImPlot.h"
#include "aggregator/SustainedMetricsTracker.h"
#include "widgets/SustainedMetricSelector.h"

QString SustainedMetricsView::metricTitle(SustainedMetric metric)
{
    switch (metric) {
    case SustainedMetric::CpuTime:       return QStringLiteral("Sustained CPU Time");
    case SustainedMetric::GpuTime:       return QStringLiteral("Sustained GPU Time");
    case SustainedMetric::AvgMemory:     return QStringLiteral("Sustained Avg Memory");
    case SustainedMetric::DiskReadTotal: return QStringLiteral("Sustained Disk Read");
    case SustainedMetric::DiskWriteTotal:return QStringLiteral("Sustained Disk Write");
    }
    return QStringLiteral("Sustained Metrics");
}

QString SustainedMetricsView::metricUnit(SustainedMetric metric)
{
    switch (metric) {
    case SustainedMetric::CpuTime:       return QStringLiteral("%\u00B7s");
    case SustainedMetric::GpuTime:       return QStringLiteral("%\u00B7s");
    case SustainedMetric::AvgMemory:     return QStringLiteral("MB");
    case SustainedMetric::DiskReadTotal: return QStringLiteral("KB");
    case SustainedMetric::DiskWriteTotal:return QStringLiteral("KB");
    }
    return QString();
}

void SustainedMetricsView::setTracker(SustainedMetricsTracker* tracker)
{
    tracker_ = tracker;
}

void SustainedMetricsView::setMetricSelector(SustainedMetricSelector* selector)
{
    metricSelector_ = selector;
}

void SustainedMetricsView::buildView(QIM::QImFigureWidget* figure, const QList<AggregatedProcessInfo>& /*data*/)
{
    if (!figure)
        return;

    orderedNames_.clear();
    colorManager_.clear();
    timelineLines_.clear();  // Items destroyed as QObject children of old plot nodes during view switch

    figure->setSubplotGrid(2, 1, {1.0f, 2.0f});

    rankingPlot_ = figure->createPlotNode();
    if (!rankingPlot_)
        return;

    rankingPlot_->setTitle(QStringLiteral("Sustained Metrics Ranking"));
    rankingPlot_->setLegendEnabled(true);
    rankingPlot_->x1Axis()->setLabel(metricUnit(currentMetric_));
    rankingPlot_->y1Axis()->setLabel(QString());  // Process names are shown as tick labels

    rankingBars_ = new QIM::QImPlotBarGroupsItemNode(rankingPlot_);
    rankingBars_->setHorizontal(true);
    rankingBars_->setStacked(false);
    rankingBars_->setGroupWidth(0.4);  // Reduce bar density

    rankingPlot_->pushColormap("__bar_groups_custom__");

    QIM::QImPlotLegendNode* rankingLegend = rankingPlot_->legendNode();
    if (rankingLegend) {
        rankingLegend->setOutside(true);
        rankingLegend->setLocation(QIM::QImPlotLegendLocation::South);
    }

    timelinePlot_ = figure->createPlotNode();
    if (!timelinePlot_)
        return;

    timelinePlot_->setTitle(QStringLiteral("Sustained Metrics Timeline"));
    timelinePlot_->setLegendEnabled(true);
    timelinePlot_->x1Axis()->setLabel(QStringLiteral("Time (s)"));
    timelinePlot_->y1Axis()->setAutoFit(true);

    QIM::QImPlotLegendNode* timelineLegend = timelinePlot_->legendNode();
    if (timelineLegend) {
        timelineLegend->setOutside(true);
        timelineLegend->setLocation(QIM::QImPlotLegendLocation::South);
    }

    updateData({});
}

void SustainedMetricsView::updateData(const QList<AggregatedProcessInfo>& /*data*/)
{
    if (!tracker_ || !rankingPlot_ || !timelinePlot_)
        return;

    if (metricSelector_)
        currentMetric_ = metricSelector_->currentMetric();

    QString title = metricTitle(currentMetric_);
    QString unit = metricUnit(currentMetric_);

    // ---- RANKING ----
    QList<QPair<QString, double>> ranking = tracker_->getRanking(currentMetric_);  // ALL processes for ranking chart;

    if (!ranking.isEmpty()) {
        QStringList labels;
        QVector<double> values;
        labels.reserve(ranking.size());
        values.reserve(ranking.size());

        for (const auto& pair : ranking) {
            labels.append(pair.first);
            values.append(pair.second);
        }

        rankingBars_->setData(labels, values, 1, ranking.size());
        rankingPlot_->setTitle(title + QStringLiteral(" Ranking"));
        rankingPlot_->x1Axis()->setLabel(unit);

        // Build Y-axis tick labels: process name at bar index position
        m_tickPositions.clear();
        m_tickLabels.clear();
        m_tickPositions.reserve(ranking.size());
        m_tickLabels.reserve(ranking.size());
        for (int i = 0; i < ranking.size(); ++i) {
            m_tickPositions.append(static_cast<double>(i+1));
            m_tickLabels.append(labels[i].toUtf8());
        }
        rankingPlot_->y1Axis()->setAxisTicks(m_tickPositions, m_tickLabels);
        rankingPlot_->y1Axis()->setLimits(0, ranking.size() + 1);
        qDebug() << "Tick positions:" << m_tickPositions;
        qDebug() << "Y axis limits:" << rankingPlot_->y1Axis()->minLimits()
                 << rankingPlot_->y1Axis()->maxLimits();
    }

    // ---- TIMELINE ----
    SustainedTimelineData timeline = tracker_->getTimeline(currentMetric_, kTopN);

    if (timeline.pointCount < 1 || timeline.timestamps.isEmpty())
        return;

    int numPoints = timeline.pointCount;

    // Maintain orderedNames_: append-only, sort on first call
    if (orderedNames_.isEmpty()) {
        orderedNames_ = timeline.topNames;
    } else {
        QSet<QString> existing = QSet<QString>(orderedNames_.begin(), orderedNames_.end());
        for (const QString& name : timeline.topNames) {
            if (!existing.contains(name)) {
                orderedNames_.append(name);
            }
        }
    }

    // Create/update line items for top-N processes
    QSet<QString> activeNames;
    for (const QString& name : timeline.topNames) {
        activeNames.insert(name);

        if (!timelineLines_.contains(name)) {
            QIM::QImPlotLineItemNode* line = new QIM::QImPlotLineItemNode(timelinePlot_);
            line->setLabel(name);
            line->setColor(colorManager_.colorFor(name));
            timelineLines_.insert(name, line);
        }

        QIM::QImPlotLineItemNode* line = timelineLines_[name];
        QVector<double> yData = timeline.values.value(name, QVector<double>());
        if (yData.size() >= numPoints) {
            std::vector<double> xVec(numPoints);
            std::vector<double> yVec(numPoints);
            for (int i = 0; i < numPoints; ++i) {
                xVec[i] = timeline.timestamps[i];
                yVec[i] = yData[i];
            }
            line->setData(xVec, yVec);
        }
    }

    // Hide stale lines
    for (auto it = timelineLines_.begin(); it != timelineLines_.end(); ++it) {
        if (!activeNames.contains(it.key())) {
            it.value()->setVisible(false);
        } else {
            it.value()->setVisible(true);
        }
    }

    // Sliding window: last 600 seconds
    double xMax = timeline.timestamps.last();
    double xMin = std::max(0.0, xMax - static_cast<double>(kWindowDurationSec));
    if (xMax <= xMin)
        xMax = xMin + 1.0;
    timelinePlot_->x1Axis()->setLimits(xMin, xMax, QIM::QImPlotCondition::Always);
    timelinePlot_->setTitle(title + QStringLiteral(" Timeline"));
    timelinePlot_->y1Axis()->setLabel(unit);
}

void SustainedMetricsView::resetAccumulation()
{
    orderedNames_.clear();
    colorManager_.clear();
    timelineLines_.clear();  // Items destroyed as QObject children of plot nodes during view rebuild
}
