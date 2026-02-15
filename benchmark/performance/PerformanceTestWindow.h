// PerformanceTestWindow.h
#ifndef PERFORMANCETESTWINDOW_H
#define PERFORMANCETESTWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QChartView>
#include <QProgressBar>
#include <QLabel>
#include "PerformanceTestController.h"

// Qt Chart
#include <QtCharts>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QCategoryAxis>

namespace QIM
{
class QImFigureWidget;
}

class PerformanceTestWindow : public QMainWindow
{
    Q_OBJECT

public:
    PerformanceTestWindow(QWidget* parent = nullptr);
    ~PerformanceTestWindow();

private slots:
    void onStartTest();
    void onTestStarted(const QString& libraryName, int pointCount);
    void onTestResultReady(const TestResult& result);
    void onAllTestsCompleted(const QVector< TestResult >& allResults);

private:
    void setupUI();
    void setupCharts();
    void updateResultTable(const TestResult& result);
    void updateSummaryCharts(const QVector< TestResult >& allResults);


private:
    // UI组件
    QTabWidget* m_tabWidget;
    QTableWidget* m_resultTable;
    QPushButton* m_startButton;
    QComboBox* m_librarySelector;
    QCheckBox* m_enableDownsampling;
    QSpinBox* m_pointCountSelector;
    QProgressBar* m_progressBar;
    QLabel* m_statusLabel;

    // 测试控制器
    PerformanceTestController* m_testController;

    // 图表组件
    QChartView* m_timeChartView;
    QChartView* m_fpsChartView;
    QChartView* m_memoryChartView;
};

#endif  // PERFORMANCETESTWINDOW_H
