#ifndef PERFORMANCETESTWIDGET_H
#define PERFORMANCETESTWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QVector>
#include "PerformanceTestController.h"
class PerformanceTestReportDialog;
namespace Ui
{
class PerformanceTestWidget;
}

class QCustomPlot;

class PerformanceTestWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PerformanceTestWidget(QWidget* parent = nullptr);
    ~PerformanceTestWidget();

protected:
    void changeEvent(QEvent* e) override;

private slots:
    void onStartButtonClicked();
    void onFullBenchmarkClicked();  // 完整基准测试
    void onShowReportClicked();
    void onTestStarted(const QString& libraryName, int pointCount);
    void onProgressChanged(int current, int total);
    void onAllTestsCompleted(const QVector< TestResult >& allResults);

private:
    void setupModel();
    void populateTable(const QVector< TestResult >& results);
    QVector< int > parsePointCounts(const QString& text) const;
    Ui::PerformanceTestWidget* ui;
    QStandardItemModel* model;
    PerformanceTestController* controller;
    QVector< TestResult > allTestResults;
    PerformanceTestController::TestConfig m_currentTestConfig;
    PerformanceTestReportDialog* reportDialog;
};

#endif  // PERFORMANCETESTWIDGET_H
