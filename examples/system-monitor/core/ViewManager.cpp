#include "ViewManager.h"

#include "QImFigureWidget.h"
#include "views/StackedAreaView.h"
#include "views/PieChartView.h"
#include "views/Scatter3DView.h"
#include "views/TimeSeries3DView.h"
#include "aggregator/HistoryBuffer.h"
#include "plot3d/QImPlot3DNode.h"
#include "collector/ProcessInfo.h"

ViewManager::ViewManager(QIM::QImFigureWidget* figure, QObject* parent)
    : QObject(parent), figure_(figure) {
    stackedAreaView_ = new StackedAreaView;
    pieChartView_ = new PieChartView;
    scatter3DView_ = new Scatter3DView;
    timeSeries3DView_ = new TimeSeries3DView;
}

ViewManager::~ViewManager() {
    delete stackedAreaView_;
    delete pieChartView_;
    delete scatter3DView_;
    delete timeSeries3DView_;
}

void ViewManager::setHistoryBuffer(HistoryBuffer* buffer)
{
    stackedAreaView_->setHistoryBuffer(buffer);
    timeSeries3DView_->setHistoryBuffer(buffer);
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
    case ViewMode::StackedArea:
        stackedAreaView_->buildView(figure_, data);
        break;
    case ViewMode::PieChart:
        pieChartView_->buildView(figure_, data);
        break;
    case ViewMode::Scatter3D:
        scatter3DView_->buildView(figure_, data);
        break;
    case ViewMode::TimeSeries3D:
        timeSeries3DView_->buildView(figure_, data);
        break;
    }

    currentMode_ = mode;
    viewBuilt_ = true;
    Q_EMIT viewSwitched(mode);
}

void ViewManager::updateCurrentView(const QList<AggregatedProcessInfo>& data) {
    if (!figure_ || !viewBuilt_) return;

    switch (currentMode_) {
    case ViewMode::StackedArea:
        stackedAreaView_->updateData(data);
        break;
    case ViewMode::PieChart:
        pieChartView_->updateData(data);
        break;
    case ViewMode::Scatter3D:
        scatter3DView_->updateData(data);
        break;
    case ViewMode::TimeSeries3D:
        timeSeries3DView_->updateData(data);
        break;
    }
}