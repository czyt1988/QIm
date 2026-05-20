#include "ViewManager.h"

#include "QImFigureWidget.h"
#include "views/CpuUsageView.h"
#include "views/ResourcePieView.h"
#include "views/ProcessResource3DView.h"
#include "views/CpuTimeline3DView.h"
#include "views/SystemMetricsView.h"
#include "aggregator/HistoryBuffer.h"
#include "plot3d/QImPlot3DNode.h"
#include "collector/ProcessInfo.h"

ViewManager::ViewManager(QIM::QImFigureWidget* figure, QObject* parent)
    : QObject(parent), figure_(figure) {
    cpuUsageView_ = new CpuUsageView;
    resourcePieView_ = new ResourcePieView;
    processResource3DView_ = new ProcessResource3DView;
    cpuTimeline3DView_ = new CpuTimeline3DView;
    systemMetricsView_ = new SystemMetricsView;
}

ViewManager::~ViewManager() {
    delete cpuUsageView_;
    delete resourcePieView_;
    delete processResource3DView_;
    delete cpuTimeline3DView_;
    delete systemMetricsView_;
}

void ViewManager::setHistoryBuffer(HistoryBuffer* buffer)
{
    cpuUsageView_->setHistoryBuffer(buffer);
    cpuTimeline3DView_->setHistoryBuffer(buffer);
    systemMetricsView_->setHistoryBuffer(buffer);
}

void ViewManager::switchTo(ViewMode mode, const QList<AggregatedProcessInfo>& data) {
    if (!figure_) return;

    // If same mode is already active and built, just update data (no rebuild needed)
    if (mode == currentMode_ && viewBuilt_) {
        updateCurrentView(data);
        return;
    }

    // Clear all subplot grids before rebuilding
    figure_->clearSubplotGrid();
    figure_->clearSubplot3DGrid();

    // clearSubplotGrid()/clearSubplot3DGrid() only remove the subplot containers,
    // not root-level plot nodes created in single-plot mode (1x1 grid).
    // Explicitly remove leftover root-level nodes to prevent stale views stacking.
    for (auto* plot : figure_->plotNodes()) {
        figure_->removePlotNode(plot);
    }
    for (auto* plot3D : figure_->plot3DNodes()) {
        figure_->removeRenderNode(plot3D);  // removePlotNode is PlotNode-specific
    }

    switch (mode) {
    case ViewMode::CpuUsage:
        cpuUsageView_->buildView(figure_, data);
        break;
    case ViewMode::ResourcePie:
        resourcePieView_->buildView(figure_, data);
        break;
    case ViewMode::ProcessResource3D:
        processResource3DView_->buildView(figure_, data);
        break;
    case ViewMode::CpuTimeline3D:
        cpuTimeline3DView_->buildView(figure_, data);
        break;
    case ViewMode::SystemMetrics:
        systemMetricsView_->buildView(figure_, data);
        break;
    }

    currentMode_ = mode;
    viewBuilt_ = true;
    Q_EMIT viewSwitched(mode);
}

void ViewManager::updateCurrentView(const QList<AggregatedProcessInfo>& data) {
    if (!figure_ || !viewBuilt_) return;

    switch (currentMode_) {
    case ViewMode::CpuUsage:
        cpuUsageView_->updateData(data);
        break;
    case ViewMode::ResourcePie:
        resourcePieView_->updateData(data);
        break;
    case ViewMode::ProcessResource3D:
        processResource3DView_->updateData(data);
        break;
    case ViewMode::CpuTimeline3D:
        cpuTimeline3DView_->updateData(data);
        break;
    case ViewMode::SystemMetrics:
        systemMetricsView_->updateData(data);
        break;
    }
}