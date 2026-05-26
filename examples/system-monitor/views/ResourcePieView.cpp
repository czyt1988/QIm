#include "ResourcePieView.h"

#include "QImFigureWidget.h"
#include "plot/QImPlotNode.h"
#include "plot/QImPlotPieChartItemNode.h"
#include "plot/QImPlotAxisInfo.h"
#include "plot/QImPlot.h"
#include "aggregator/SustainedMetricsTracker.h"
#include "collector/ProcessInfo.h"

#include <algorithm>
#include <vector>

static constexpr int kTopN = 8;

void ResourcePieView::setTracker(SustainedMetricsTracker* tracker)
{
    tracker_ = tracker;
}

void ResourcePieView::buildView(QIM::QImFigureWidget* figure, const QList<AggregatedProcessInfo>& data)
{
    figure->clearSubplotGrid();
    figure->setSubplotGrid(1, 3);

    auto plots = figure->plotNodes();

    // Ensure 3 plot nodes exist
    if (plots.size() < 1) { cpuPlot_ = figure->createPlotNode(); }
    else { cpuPlot_ = plots[0]; }

    if (plots.size() < 2) { memPlot_ = figure->createPlotNode(); }
    else { memPlot_ = plots[1]; }

    if (plots.size() < 3) { diskPlot_ = figure->createPlotNode(); }
    else { diskPlot_ = plots[2]; }

    setupPiePlot(cpuPlot_, QStringLiteral("Cumulative CPU Time"));
    setupPiePlot(memPlot_, QStringLiteral("Average Memory Usage"));
    setupPiePlot(diskPlot_, QStringLiteral("Total Disk I/O"));

    cpuPie_ = new QIM::QImPlotPieChartItemNode(cpuPlot_);
    cpuPie_->setCenter(QPointF(0.5, 0.5));
    cpuPie_->setRadius(0.40);
    cpuPie_->setLabelFormat(QStringLiteral("%.1f"));

    memPie_ = new QIM::QImPlotPieChartItemNode(memPlot_);
    memPie_->setCenter(QPointF(0.5, 0.5));
    memPie_->setRadius(0.40);
    memPie_->setLabelFormat(QStringLiteral("%.0f MB"));

    diskPie_ = new QIM::QImPlotPieChartItemNode(diskPlot_);
    diskPie_->setCenter(QPointF(0.5, 0.5));
    diskPie_->setRadius(0.40);
    diskPie_->setLabelFormat(QStringLiteral("%.1f MB"));

    updateData(data);
}

void ResourcePieView::setupPiePlot(QIM::QImPlotNode* plot, const QString& title)
{
    plot->setTitle(title);
    plot->setEqual(true);
    plot->setMouseTextEnabled(false);
    plot->x1Axis()->setNoDecorations(true);
    plot->y1Axis()->setNoDecorations(true);
    plot->x1Axis()->setLimits(0, 1, QIM::QImPlotCondition::Always);
    plot->y1Axis()->setLimits(0, 1, QIM::QImPlotCondition::Always);
}

void ResourcePieView::updateData(const QList<AggregatedProcessInfo>& /*data*/)
{
    if (!tracker_ || !cpuPie_ || !memPie_ || !diskPie_)
        return;

    // CPU pie: Top-N by cumulative CPU time (%·s) + Others
    auto cpuRanking = tracker_->getRankingWithOthers(SustainedMetric::CpuTime, kTopN);
    updateRankingPie(cpuPie_, cpuRanking, 1.0, 0.0);

    // Memory pie: Top-N by average memory (MB) + Others
    auto memRanking = tracker_->getRankingWithOthers(SustainedMetric::AvgMemory, kTopN);
    updateRankingPie(memPie_, memRanking, 1.0, 0.0);

    // Disk I/O pie: Top-N by combined total disk read + write (KB -> MB) + Others
    // Merge two rankings by name, then re-sort
    QHash<QString, double> diskMap;
    for (const auto& [name, val] : tracker_->getRanking(SustainedMetric::DiskReadTotal)) {
        diskMap[name] += val;
    }
    for (const auto& [name, val] : tracker_->getRanking(SustainedMetric::DiskWriteTotal)) {
        diskMap[name] += val;
    }
    QList<QPair<QString, double>> diskAll;
    for (auto it = diskMap.begin(); it != diskMap.end(); ++it) {
        diskAll.append({it.key(), it.value()});
    }
    // Sort descending by value, name tie-breaker
    std::sort(diskAll.begin(), diskAll.end(),
        [](const QPair<QString, double>& a, const QPair<QString, double>& b) {
            if (a.second != b.second) return a.second > b.second;
            return a.first < b.first;
        });
    // Slice topN + Others
    QList<QPair<QString, double>> diskPie;
    if (diskAll.size() > kTopN) {
        diskPie = diskAll.mid(0, kTopN);
        double othersSum = 0.0;
        for (int i = kTopN; i < diskAll.size(); ++i) {
            othersSum += diskAll[i].second;
        }
        diskPie.append({QStringLiteral("Other"), othersSum});
    } else {
        diskPie = diskAll;
    }
    // Scale KB -> MB
    updateRankingPie(diskPie_, diskPie, 1.0 / 1024.0, 0.0);
}

void ResourcePieView::updateRankingPie(QIM::QImPlotPieChartItemNode* pie,
                                       const QList<QPair<QString, double>>& ranking,
                                       double scale, double /*totalOverride*/)
{
    if (!pie || ranking.isEmpty())
        return;

    QStringList labels;
    std::vector<double> values;
    labels.reserve(ranking.size());
    values.reserve(ranking.size());
    for (const auto& [name, val] : ranking) {
        labels << name;
        values.push_back(val * scale);
    }
    pie->setData(labels, values);
}