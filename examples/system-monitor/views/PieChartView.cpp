#include "PieChartView.h"

#include "QImFigureWidget.h"
#include "plot/QImPlotNode.h"
#include "plot/QImPlotPieChartItemNode.h"
#include "plot/QImPlotAxisInfo.h"
#include "plot/QImPlot.h"
#include "aggregator/ProcessAggregator.h"
#include "collector/ProcessInfo.h"

#include <algorithm>
#include <vector>

static constexpr int kTopN = 8;

void PieChartView::buildView(QIM::QImFigureWidget* figure, const QList<AggregatedProcessInfo>& data)
{
    figure->clearSubplotGrid();
    figure->setSubplotGrid(2, 2);

    auto plots = figure->plotNodes();

    // Ensure 4 plot nodes exist
    if (plots.size() < 1) { cpuPlot_ = figure->createPlotNode(); }
    else { cpuPlot_ = plots[0]; }

    if (plots.size() < 2) { memPlot_ = figure->createPlotNode(); }
    else { memPlot_ = plots[1]; }

    if (plots.size() < 3) { gpuPlot_ = figure->createPlotNode(); }
    else { gpuPlot_ = plots[2]; }

    if (plots.size() < 4) { diskPlot_ = figure->createPlotNode(); }
    else { diskPlot_ = plots[3]; }

    setupPiePlot(cpuPlot_, QStringLiteral("CPU Usage"));
    setupPiePlot(memPlot_, QStringLiteral("Memory Usage"));
    setupPiePlot(gpuPlot_, QStringLiteral("GPU Usage"));
    setupPiePlot(diskPlot_, QStringLiteral("Disk I/O"));

    cpuPie_ = new QIM::QImPlotPieChartItemNode(cpuPlot_);
    cpuPie_->setCenter(QPointF(0.5, 0.5));
    cpuPie_->setRadius(0.40);
    cpuPie_->setLabelFormat(QStringLiteral("%.1f%%"));

    memPie_ = new QIM::QImPlotPieChartItemNode(memPlot_);
    memPie_->setCenter(QPointF(0.5, 0.5));
    memPie_->setRadius(0.40);
    memPie_->setLabelFormat(QStringLiteral("%.1f%%"));

    gpuPie_ = new QIM::QImPlotPieChartItemNode(gpuPlot_);
    gpuPie_->setCenter(QPointF(0.5, 0.5));
    gpuPie_->setRadius(0.40);
    gpuPie_->setLabelFormat(QStringLiteral("%.1f%%"));

    diskPie_ = new QIM::QImPlotPieChartItemNode(diskPlot_);
    diskPie_->setCenter(QPointF(0.5, 0.5));
    diskPie_->setRadius(0.40);
    diskPie_->setLabelFormat(QStringLiteral("%.1f%%"));

    updateData(data);
}

void PieChartView::setupPiePlot(QIM::QImPlotNode* plot, const QString& title)
{
    plot->setTitle(title);
    plot->setEqual(true);
    plot->setMouseTextEnabled(false);
    plot->x1Axis()->setNoDecorations(true);
    plot->y1Axis()->setNoDecorations(true);
    plot->x1Axis()->setLimits(0, 1, QIM::QImPlotCondition::Always);
    plot->y1Axis()->setLimits(0, 1, QIM::QImPlotCondition::Always);
}

void PieChartView::updateData(const QList<AggregatedProcessInfo>& data)
{
    if (!cpuPie_ || !memPie_ || !gpuPie_ || !diskPie_)
        return;

    // CPU pie: top N by CPU + Others
    QList<AggregatedProcessInfo> topCpu = ProcessAggregator().getTopN(data, kTopN, SortBy::ByCpu);
    AggregatedProcessInfo cpuOthers = ProcessAggregator().getOthers(topCpu, data);
    if (cpuOthers.instanceCount > 0)
        topCpu.append(cpuOthers);

    QStringList cpuLabels;
    std::vector<double> cpuValues;
    for (const auto& p : topCpu) {
        cpuLabels << p.processName;
        cpuValues.push_back(p.totalCpuPercent);
    }
    cpuPie_->setData(cpuLabels, cpuValues);

    // Memory pie: top N by memory (converted to MB) + Others
    QList<AggregatedProcessInfo> topMem = ProcessAggregator().getTopN(data, kTopN, SortBy::ByMemory);
    AggregatedProcessInfo memOthers = ProcessAggregator().getOthers(topMem, data);
    if (memOthers.instanceCount > 0)
        topMem.append(memOthers);

    QStringList memLabels;
    std::vector<double> memValues;
    for (const auto& p : topMem) {
        memLabels << p.processName;
        memValues.push_back(static_cast<double>(p.totalWorkingSetBytes) / (1024.0 * 1024.0));
    }
    memPie_->setData(memLabels, memValues);

    // GPU pie: top N by GPU + Others
    QList<AggregatedProcessInfo> topGpu = ProcessAggregator().getTopN(data, kTopN, SortBy::ByGpu);
    AggregatedProcessInfo gpuOthers = ProcessAggregator().getOthers(topGpu, data);
    if (gpuOthers.instanceCount > 0)
        topGpu.append(gpuOthers);

    QStringList gpuLabels;
    std::vector<double> gpuValues;
    for (const auto& p : topGpu) {
        gpuLabels << p.processName;
        gpuValues.push_back(p.avgGpuPercent);
    }
    gpuPie_->setData(gpuLabels, gpuValues);

    // Disk I/O pie: top N by combined disk read+write rate (MB/s) + Others
    QList<AggregatedProcessInfo> topDisk = ProcessAggregator().getTopN(data, kTopN, SortBy::ByDiskRead);
    AggregatedProcessInfo diskOthers = ProcessAggregator().getOthers(topDisk, data);
    if (diskOthers.instanceCount > 0)
        topDisk.append(diskOthers);

    QStringList diskLabels;
    std::vector<double> diskValues;
    for (const auto& p : topDisk) {
        diskLabels << p.processName;
        double combinedMBps = (p.totalDiskReadRate + p.totalDiskWriteRate) / (1024.0 * 1024.0);
        diskValues.push_back(combinedMBps);
    }
    diskPie_->setData(diskLabels, diskValues);
}