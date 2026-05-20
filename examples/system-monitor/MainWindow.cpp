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
    , tabWidget_(nullptr)
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
    viewManager_->switchTo(ViewMode::CpuUsage, {});
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUI()
{
    setWindowTitle("QIm System Monitor");
    resize(1400, 900);

    // Central widget with vertical layout (tab widget)
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

    // Create view mode selector (4 radio buttons)
    viewSelector_ = new ViewModeSelector(this);

    // Create tab widget with two tabs
    tabWidget_ = new QTabWidget(this);

    // Tab 0: Table view (process tree)
    tabWidget_->addTab(processTree_, tr("Table"));

    // Tab 1: Plot view (view selector + figure widget)
    QWidget* plotTab = new QWidget(this);
    QVBoxLayout* plotLayout = new QVBoxLayout(plotTab);
    plotLayout->setContentsMargins(0, 0, 0, 0);
    plotLayout->setSpacing(2);
    plotLayout->addWidget(viewSelector_, 0);
    plotLayout->addWidget(figureWidget_, 1);
    tabWidget_->addTab(plotTab, tr("Plot"));

    // Add tab widget to main layout
    mainLayout_->addWidget(tabWidget_, 1);

    // Start on Plot tab
    tabWidget_->setCurrentIndex(1);

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