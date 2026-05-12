#include "Scatter3DView.h"

#include <algorithm>
#include <vector>

#include "QImFigureWidget.h"
#include "plot3d/QImPlot3DNode.h"
#include "plot3d/QImPlot3DScatterItemNode.h"
#include "plot3d/QImPlot3D.h"
#include "collector/ProcessInfo.h"
#include "aggregator/ProcessAggregator.h"

static constexpr int kMaxVisibleProcesses = 16;  // Top 15 + "Other"

void Scatter3DView::buildView(QIM::QImFigureWidget* figure, const QList<AggregatedProcessInfo>& data)
{
    if (!figure)
        return;

    scatterItems_.clear();

    // Clear any existing 2D subplot grid
    figure->setSubplotGrid(1, 1);

    // Set up 3D subplot grid and create plot node
    figure->setSubplot3DGrid(1, 1);
    plotNode_ = figure->createPlot3DNode();
    if (!plotNode_)
        return;

    plotNode_->setTitle(QStringLiteral("Resource Usage"));
    plotNode_->setupAxes(QByteArray("CPU%"), QByteArray("Memory(MB)"), QByteArray("Disk(KB/s)"));

    updateData(data);
}

void Scatter3DView::updateData(const QList<AggregatedProcessInfo>& data)
{
    if (!plotNode_)
        return;

    // Sort by CPU descending and take top N
    QList<AggregatedProcessInfo> sorted = data;
    std::sort(sorted.begin(), sorted.end(),
              [](const AggregatedProcessInfo& a, const AggregatedProcessInfo& b) {
                  return a.totalCpuPercent > b.totalCpuPercent;
              });

    // Trim to max visible + collect "Other"
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

    // Create/update scatter items for active processes
    for (const auto& proc : displayList) {
        activeNames.insert(proc.processName);

        auto* scatter = scatterItems_.value(proc.processName, nullptr);
        if (!scatter) {
            // New process: create a scatter item with stable label
            scatter = new QIM::QImPlot3DScatterItemNode(plotNode_);
            scatter->setLabel(proc.processName);       // label set ONCE, never changed
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

    // Hide items for processes that are no longer active
    for (auto it = scatterItems_.begin(); it != scatterItems_.end(); ++it) {
        it.value()->setVisible(activeNames.contains(it.key()));
    }
}
