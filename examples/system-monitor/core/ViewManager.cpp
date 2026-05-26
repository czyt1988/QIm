#include "ViewManager.h"

#include "QImFigureWidget.h"
#include "views/CpuUsageView.h"
#include "views/ResourcePieView.h"
#include "views/ProcessResource3DView.h"
#include "views/SystemMetricsView.h"
#include "views/SustainedMetricsView.h"
#include "widgets/SustainedMetricSelector.h"
#include "aggregator/SustainedMetricsTracker.h"
#include "aggregator/HistoryBuffer.h"
#include "plot3d/QImPlot3DNode.h"
#include "collector/ProcessInfo.h"
#include "ColorPalette.h"
#include "plot/QImPlotColormapManager.h"

ViewManager::ViewManager(QIM::QImFigureWidget* figure, QObject* parent)
    : QObject(parent), figure_(figure) {
    cpuUsageView_ = new CpuUsageView;
    resourcePieView_ = new ResourcePieView;
    processResource3DView_ = new ProcessResource3DView;
    systemMetricsView_ = new SystemMetricsView;
    sustainedMetricsView_ = new SustainedMetricsView;
}

ViewManager::~ViewManager() {
    delete cpuUsageView_;
    delete resourcePieView_;
    delete processResource3DView_;
    delete systemMetricsView_;
    delete sustainedMetricsView_;
}

void ViewManager::setHistoryBuffer(HistoryBuffer* buffer)
{
    cpuUsageView_->setHistoryBuffer(buffer);
    systemMetricsView_->setHistoryBuffer(buffer);
}

void ViewManager::setSustainedMetricsTracker(SustainedMetricsTracker* tracker)
{
    sustainedTracker_ = tracker;
    sustainedMetricsView_->setTracker(tracker);
    resourcePieView_->setTracker(tracker);
    processResource3DView_->setTracker(tracker);
}

void ViewManager::setSustainedMetricSelector(SustainedMetricSelector* selector)
{
    sustainedMetricsView_->setMetricSelector(selector);
}

void ViewManager::switchTo(ViewMode mode, const QList<AggregatedProcessInfo>& data) {
    if (!figure_) return;

    // Register the custom colormap once on first switchTo call.
    // This must be done after ImPlot context is ready (after initializeGL),
    // so we do it here instead of in the constructor.
    // Using static bool ensures it only happens once across all calls.
    static bool colormapRegistered = false;
    if (!colormapRegistered) {
        QIM::QImPlotColormapManager::addColormap(
            "__bar_groups_custom__",
            QList<QColor>(QImSystemMonitor::kColorPalette.begin(), QImSystemMonitor::kColorPalette.end()),
            true  // qualitative
        );
        colormapRegistered = true;
    }

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
    case ViewMode::SystemMetrics:
        systemMetricsView_->buildView(figure_, data);
        break;
    case ViewMode::SustainedMetrics:
        sustainedMetricsView_->setTracker(sustainedTracker_);
        sustainedMetricsView_->buildView(figure_, data);
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
    case ViewMode::SystemMetrics:
        systemMetricsView_->updateData(data);
        break;
    case ViewMode::SustainedMetrics:
        sustainedMetricsView_->updateData(data);
        break;
    }
}