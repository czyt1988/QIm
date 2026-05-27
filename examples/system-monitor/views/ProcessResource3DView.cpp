#include "ProcessResource3DView.h"

#include <algorithm>
#include <vector>

#include "QImFigureWidget.h"
#include "plot3d/QImPlot3DNode.h"
#include "plot3d/QImPlot3DScatterItemNode.h"
#include "plot3d/QImPlot3DMousePickerNode.h"
#include "plot3d/QImPlot3DDataSeries.h"
#include "plot3d/QImPlot3D.h"
#include "collector/ProcessInfo.h"
#include "aggregator/ProcessAggregator.h"
#include "aggregator/SustainedMetricsTracker.h"

static constexpr int kMaxVisibleProcesses = 16;  // Top 15 + "Other"

void ProcessResource3DView::setTracker(SustainedMetricsTracker* tracker)
{
    tracker_ = tracker;
}

void ProcessResource3DView::buildView(QIM::QImFigureWidget* figure, const QList<AggregatedProcessInfo>& data)
{
    if (!figure)
        return;

    scatterItems_.clear();
    delete combinedDataSeries_;
    combinedDataSeries_ = nullptr;
    combinedXs_.clear();
    combinedYs_.clear();
    combinedZs_.clear();

    // Clear any existing 2D subplot grid
    figure->setSubplotGrid(1, 1);

    // Set up 3D subplot grid and create plot node
    figure->setSubplot3DGrid(1, 1);
    plotNode_ = figure->createPlot3DNode();
    if (!plotNode_)
        return;

    plotNode_->setTitle(QStringLiteral("Resource Usage"));
    plotNode_->setupAxes(QByteArray("CPU Time(%\xc2\xb7s)"), QByteArray("Avg Memory(MB)"), QByteArray("Disk(KB)"));

    // Mouse picker: shows hovered 3D coordinate as floating text with marker
    mousePicker_ = new QIM::QImPlot3DMousePickerNode(plotNode_);
    mousePicker_->setShowCoordinatesText(true);
    mousePicker_->setShowMarker(true);
    mousePicker_->setMarkerSize(6.0f);
    mousePicker_->setMarkerColor(QColor(255, 255, 0, 220));

    updateData(data);
}

void ProcessResource3DView::updateData(const QList<AggregatedProcessInfo>& data)
{
    if (!plotNode_)
        return;

    if (!tracker_) {
        updateDataInstantaneous(data);
        return;
    }

    auto cpuRanking = tracker_->getRanking(SustainedMetric::CpuTime);
    auto memRanking = tracker_->getRanking(SustainedMetric::AvgMemory);
    auto diskReadRanking = tracker_->getRanking(SustainedMetric::DiskReadTotal);
    auto diskWriteRanking = tracker_->getRanking(SustainedMetric::DiskWriteTotal);

    QHash<QString, double> cpuMap, memMap, diskMap;
    for (const auto& [name, val] : cpuRanking)
        cpuMap[name] = val;
    for (const auto& [name, val] : memRanking)
        memMap[name] = val;
    for (const auto& [name, val] : diskReadRanking)
        diskMap[name] = val;
    for (const auto& [name, val] : diskWriteRanking)
        diskMap[name] = diskMap.value(name, 0.0) + val;

    QSet<QString> allNames;
    for (const auto& [name, _] : cpuRanking)
        allNames.insert(name);
    for (const auto& [name, _] : memRanking)
        allNames.insert(name);
    for (const auto& [name, _] : diskReadRanking)
        allNames.insert(name);
    for (const auto& [name, _] : diskWriteRanking)
        allNames.insert(name);

    if (allNames.isEmpty()) {
        updateDataInstantaneous(data);
        return;
    }

    QList<QPair<QString, double>> sortedByCpu;
    for (const auto& name : allNames)
        sortedByCpu.append({name, cpuMap.value(name, 0.0)});
    std::sort(sortedByCpu.begin(), sortedByCpu.end(),
              [](const QPair<QString, double>& a, const QPair<QString, double>& b) {
                  return a.second > b.second;
              });

    QList<QPair<QString, double>> displayList;
    if (sortedByCpu.size() > kMaxVisibleProcesses) {
        displayList = sortedByCpu.mid(0, kMaxVisibleProcesses);
        double otherCpu = 0.0, otherMem = 0.0, otherDisk = 0.0;
        for (int i = kMaxVisibleProcesses; i < sortedByCpu.size(); ++i) {
            const auto& n = sortedByCpu[i].first;
            otherCpu += cpuMap.value(n, 0.0);
            otherMem += memMap.value(n, 0.0);
            otherDisk += diskMap.value(n, 0.0);
        }
        displayList.append({QStringLiteral("Other"), otherCpu});
        memMap[QStringLiteral("Other")] = otherMem;
        diskMap[QStringLiteral("Other")] = otherDisk;
    } else {
        displayList = sortedByCpu;
    }

    QSet<QString> activeNames;

    for (const auto& [procName, cpuTime] : displayList) {
        activeNames.insert(procName);

        auto* scatter = scatterItems_.value(procName, nullptr);
        if (!scatter) {
            scatter = new QIM::QImPlot3DScatterItemNode(plotNode_);
            scatter->setLabel(procName);
            scatterItems_.insert(procName, scatter);
        }

        scatter->setVisible(true);

        double xVal = cpuTime;
        double yVal = memMap.value(procName, 0.0);
        double zVal = diskMap.value(procName, 0.0);

        std::vector<double> xVec = {xVal};
        std::vector<double> yVec = {yVal};
        std::vector<double> zVec = {zVal};

        scatter->setData(xVec, yVec, zVec);

        QColor color = colorManager_.colorFor(procName);
        scatter->setMarkerFillColor(color);
        scatter->setMarkerSize(6.0f);
    }

    for (auto it = scatterItems_.begin(); it != scatterItems_.end(); ++it) {
        it.value()->setVisible(activeNames.contains(it.key()));
    }

    rebuildCombinedData();
}

void ProcessResource3DView::updateDataInstantaneous(const QList<AggregatedProcessInfo>& data)
{
    QList<AggregatedProcessInfo> sorted = data;
    std::sort(sorted.begin(), sorted.end(),
              [](const AggregatedProcessInfo& a, const AggregatedProcessInfo& b) {
                  return a.totalCpuPercent > b.totalCpuPercent;
              });

    QList<AggregatedProcessInfo> displayList;
    if (sorted.size() > kMaxVisibleProcesses - 1) {
        displayList = sorted.mid(0, kMaxVisibleProcesses - 1);
        ProcessAggregator aggregator;
        AggregatedProcessInfo others = aggregator.getOthers(displayList, sorted);
        if (others.totalCpuPercent > 0.0 || others.totalWorkingSetBytes > 0)
            displayList.append(others);
    } else {
        displayList = sorted;
    }

    QSet<QString> activeNames;

    for (const auto& proc : displayList) {
        activeNames.insert(proc.processName);

        auto* scatter = scatterItems_.value(proc.processName, nullptr);
        if (!scatter) {
            scatter = new QIM::QImPlot3DScatterItemNode(plotNode_);
            scatter->setLabel(proc.processName);
            scatterItems_.insert(proc.processName, scatter);
        }

        scatter->setVisible(true);

        double cpu = proc.totalCpuPercent;
        double memMB = static_cast<double>(proc.totalWorkingSetBytes) / (1024.0 * 1024.0);
        double diskKB = (proc.totalDiskReadRate + proc.totalDiskWriteRate) / 1024.0;

        std::vector<double> xVec = {cpu};
        std::vector<double> yVec = {memMB};
        std::vector<double> zVec = {diskKB};

        scatter->setData(xVec, yVec, zVec);

        QColor color = colorManager_.colorFor(proc.processName);
        scatter->setMarkerFillColor(color);
        scatter->setMarkerSize(6.0f);
    }

    for (auto it = scatterItems_.begin(); it != scatterItems_.end(); ++it) {
        it.value()->setVisible(activeNames.contains(it.key()));
    }

    rebuildCombinedData();
}

void ProcessResource3DView::rebuildCombinedData()
{
    combinedXs_.clear();
    combinedYs_.clear();
    combinedZs_.clear();

    for (auto it = scatterItems_.begin(); it != scatterItems_.end(); ++it) {
        const auto* scatter = it.value();
        if (!scatter->isVisible())
            continue;

        const auto* series = scatter->data();
        if (!series || !series->isValid())
            continue;

        for (int i = 0; i < series->size(); ++i) {
            combinedXs_.push_back(series->xValue(i));
            combinedYs_.push_back(series->yValue(i));
            combinedZs_.push_back(series->zValue(i));
        }
    }

    delete combinedDataSeries_;
    combinedDataSeries_ = new QIM::QImVectorXYZDataSeries<
        std::vector<double>, std::vector<double>, std::vector<double>>(
        combinedXs_, combinedYs_, combinedZs_);

    if (mousePicker_)
        mousePicker_->setData(combinedDataSeries_);
}
