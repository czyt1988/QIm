#include "MainWindow.h"

#include <QImFigureWidget.h>
#include <QImWidget.h>
#include "widgets/ProcessTreeWidget.h"
#include "widgets/ViewModeSelector.h"
#include "core/ProcessMonitor.h"
#include "core/ViewManager.h"
#include "collector/ProcessInfo.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , centralWidget_(nullptr)
    , mainLayout_(nullptr)
    , splitter_(nullptr)
    , processTree_(nullptr)
    , figureWidget_(nullptr)
    , viewSelector_(nullptr)
    , monitor_(nullptr)
    , viewManager_(nullptr)
    , lastAggregatedData_()
{
    setupUI();
    connectSignals();

    // Start data collection
    monitor_->start();

    // Trigger initial view with empty data (view will populate when first data arrives)
    viewManager_->switchTo(ViewMode::StackedArea, {});
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUI()
{
    setWindowTitle("QIm System Monitor");
    resize(1400, 900);

    // Central widget with vertical layout (splitter + view selector)
    centralWidget_ = new QWidget(this);
    mainLayout_ = new QVBoxLayout(centralWidget_);
    mainLayout_->setContentsMargins(4, 4, 4, 4);
    mainLayout_->setSpacing(4);

    // Create figure widget with continuous rendering for live data
    figureWidget_ = new QIM::QImFigureWidget(this);
    figureWidget_->setRenderMode(QIM::QImWidget::RenderContinuous);
    figureWidget_->setRefreshInterval(55);

    // Create process tree widget
    processTree_ = new ProcessTreeWidget(this);

    // Create horizontal splitter: 33% tree + 67% figure
    splitter_ = new QSplitter(Qt::Horizontal, this);
    splitter_->addWidget(processTree_);
    splitter_->addWidget(figureWidget_);
    splitter_->setStretchFactor(0, 1);
    splitter_->setStretchFactor(1, 2);

    // Create view mode selector (4 radio buttons at the bottom)
    viewSelector_ = new ViewModeSelector(this);

    // Assemble layout: splitter fills most space, selector at bottom
    mainLayout_->addWidget(splitter_, 1);
    mainLayout_->addWidget(viewSelector_, 0);

    setCentralWidget(centralWidget_);

    // Create controllers
    monitor_ = ProcessMonitor::instance();
    viewManager_ = new ViewManager(figureWidget_, this);

    // Wire history buffer to time-series views
    viewManager_->setHistoryBuffer(monitor_->historyBuffer());
}

void MainWindow::connectSignals()
{
    // Aggregated data arrives → update tree and update current view data
    connect(monitor_, &ProcessMonitor::aggregatedReady, this,
            [this](const QList<AggregatedProcessInfo>& data) {
                lastAggregatedData_ = data;
                processTree_->updateData(data);
                viewManager_->updateCurrentView(data);
            });

    // View mode changed → rebuild view with cached data
    connect(viewSelector_, &ViewModeSelector::modeChanged, this,
            [this](ViewMode mode) {
                viewManager_->switchTo(mode, lastAggregatedData_);
            });
}