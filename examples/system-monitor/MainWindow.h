#pragma once

#include <QMainWindow>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QAction>

#include "collector/ProcessInfo.h"

namespace QIM {
class QImFigureWidget;
}

class ProcessTreeWidget;
class ViewModeSelector;
class ProcessMonitor;
class ViewManager;
class SustainedMetricSelector;

// Main application window for the QIm System Monitor
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

private:
    void setupUI();
    void connectSignals();

    // Top-level container
    QWidget* centralWidget_;
    QVBoxLayout* mainLayout_;
    QTabWidget* tabWidget_;

    // Widgets
    ProcessTreeWidget* processTree_;
    QIM::QImFigureWidget* figureWidget_;
    ViewModeSelector* viewSelector_;

    // Controllers
    ProcessMonitor* monitor_;
    ViewManager* viewManager_;

    SustainedMetricSelector* metricSelector_;
    QMenu* sustainedMenu_;
    QAction* resetAction_;

    // Cached data for mode switches
    QList<AggregatedProcessInfo> lastAggregatedData_;
};