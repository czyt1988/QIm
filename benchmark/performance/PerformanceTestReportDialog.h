#ifndef PERFORMANCETESTREPORTDIALOG_H
#define PERFORMANCETESTREPORTDIALOG_H
#include <QDialog>
#include <QVector>
#include "PerformanceTestController.h"

namespace Ui
{
class PerformanceTestReportDialog;
}

class QCustomPlot;

class PerformanceTestReportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PerformanceTestReportDialog(QWidget* parent = nullptr);
    ~PerformanceTestReportDialog();

    // 设置测试结果并生成报告
    void setTestResults(
        const QVector< TestResult >& results, const PerformanceTestController::TestConfig& config, bool isFullBenchmark = false
    );

    // 生成完整HTML报告（含内嵌图表）
    QString generateReportHtml() const;

protected:
    void changeEvent(QEvent* e) override;

private slots:
    void onPushButtonCopy_clicked();
    void onPushButtonExportToPdf_clicked();

private:
    // 图表生成辅助方法
    QPixmap generateFpsChart(const QVector< TestResult >& results) const;
    QPixmap generateCostChart(const QVector< TestResult >& results) const;
    QPixmap generateMemoryChart(const QVector< TestResult >& results) const;
    QString pixmapToBase64(const QPixmap& pixmap) const;

    // 报告内容生成
    QString generateExecutiveSummary() const;
    QString generateDetailedResultsTable() const;
    QString generateRecommendations() const;
    QString getPerformanceClass(double fps) const;

    Ui::PerformanceTestReportDialog* ui;
    QVector< TestResult > m_results;
    PerformanceTestController::TestConfig m_config;
    bool m_isFullBenchmark;
    QString m_reportTitle;
};

#endif  // PERFORMANCETESTREPORTDIALOG_H
