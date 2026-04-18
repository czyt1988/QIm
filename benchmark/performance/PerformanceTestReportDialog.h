#ifndef PERFORMANCETESTREPORTDIALOG_H
#define PERFORMANCETESTREPORTDIALOG_H
#include <QDialog>
#include <QVector>
#include "PerformanceTestController.h"

namespace Ui
{
class PerformanceTestReportDialog;
}

class PerformanceTestReportDialog : public QDialog
{
    Q_OBJECT

public:
    // \if ENGLISH
    /// \brief Report language option
    // \endif
    // \if CHINESE
    /// \brief 报告语言选项
    // \endif
    enum ReportLanguage { English, Chinese };

    explicit PerformanceTestReportDialog(QWidget* parent = nullptr);
    ~PerformanceTestReportDialog();

    // 设置测试结果并生成报告
    void setTestResults(
        const QVector< TestResult >& results, const PerformanceTestController::TestConfig& config, bool isFullBenchmark = false
    );

    // \if ENGLISH
    /// \brief Generate full Markdown report
    // \endif
    // \if CHINESE
    /// \brief 生成完整的Markdown报告
    // \endif
    QString generateReportMarkdown() const;

    // \if ENGLISH
    /// \brief Set whether to include Mermaid chart blocks in the report
    // \endif
    // \if CHINESE
    /// \brief 设置是否在报告中包含Mermaid图表
    // \endif
    void setIncludeMermaid(bool include) { m_includeMermaid = include; }

    // \if ENGLISH
    /// \brief Set the report language
    // \endif
    // \if CHINESE
    /// \brief 设置报告语言
    // \endif
    void setReportLanguage(ReportLanguage lang) { m_reportLanguage = lang; }

protected:
    void changeEvent(QEvent* e) override;

private Q_SLOTS:
    void onPushButtonCopy_clicked();
    void onPushButtonExportToPdf_clicked();

private:
    // 报告内容生成
    QString generateMermaidCharts() const;

    // Markdown辅助方法
    QString formatPointCountList(const QVector< int >& pointCounts, bool chinese = false) const;
    QString formatPointCountLabel(int pointCount) const;
    QString formatNumberList(const QVector< double >& values, int precision = 2) const;

    /// \brief Format a single point-count section as a Markdown table
    QString formatResultsTable(const QVector< TestResult >& results, int pointCount) const;

    /// \brief Generate recommendations section in Markdown
    QString generateRecommendationsMarkdown() const;

    Ui::PerformanceTestReportDialog* ui;
    QVector< TestResult > m_results;
    PerformanceTestController::TestConfig m_config;
    bool m_isFullBenchmark;
    QString m_reportTitle;
    QString m_reportMarkdown;
    bool m_includeMermaid = false;
    ReportLanguage m_reportLanguage = English;
};

#endif  // PERFORMANCETESTREPORTDIALOG_H
