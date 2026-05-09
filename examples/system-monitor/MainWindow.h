#pragma once

#include <QMainWindow>
#include <QSplitter>
#include <QVBoxLayout>

#include "collector/ProcessInfo.h"

namespace QIM {
class QImFigureWidget;
}

class ProcessTreeWidget;
class ViewModeSelector;
class ProcessMonitor;
class ViewManager;

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
    QSplitter* splitter_;

    // Widgets
    ProcessTreeWidget* processTree_;
    QIM::QImFigureWidget* figureWidget_;
    ViewModeSelector* viewSelector_;

    // Controllers
    ProcessMonitor* monitor_;
    ViewManager* viewManager_;

    // Cached data for mode switches
    QList<AggregatedProcessInfo> lastAggregatedData_;
};