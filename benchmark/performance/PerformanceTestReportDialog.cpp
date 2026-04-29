#include "PerformanceTestReportDialog.h"
#include "ui_PerformanceTestReportDialog.h"
#include <QDateTime>
#include <QTextDocument>
#include <QPrinter>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QClipboard>
#include <QFile>
#include <QPageSize>
#include <algorithm>

// Helper for formatting point count labels
static QString formatPointCount(int pointCount, bool chinese = false)
{
    if (pointCount >= 1000000) {
        int million = pointCount / 1000000;
        if (chinese) {
            return QString("%1M（%2万）").arg(million).arg(pointCount / 10000);
        }
        return QString("%1M").arg(million);
    }
    if (pointCount >= 1000) {
        int k = pointCount / 1000;
        if (chinese) {
            return QString("%1K（%2万）").arg(k).arg(pointCount / 10000);
        }
        return QString("%1K").arg(k);
    }
    return QString::number(pointCount);
}

PerformanceTestReportDialog::PerformanceTestReportDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::PerformanceTestReportDialog)
{
    ui->setupUi(this);
    setWindowTitle("Performance Test Report");
    resize(1000, 800);

    // 连接按钮信号
    connect(ui->pushButtonCopy, &QPushButton::clicked, this, &PerformanceTestReportDialog::onPushButtonCopy_clicked);
    connect(ui->pushButtonSaveMarkdown, &QPushButton::clicked, this, &PerformanceTestReportDialog::onPushButtonSaveMarkdown_clicked);
    connect(ui->pushButtonExportToPdf, &QPushButton::clicked, this, &PerformanceTestReportDialog::onPushButtonExportToPdf_clicked);

    // 美化 QTextBrowser
    ui->textBrowser->setOpenExternalLinks(true);
    ui->textBrowser->setOpenLinks(true);
    ui->textBrowser->setReadOnly(true);
}

PerformanceTestReportDialog::~PerformanceTestReportDialog()
{
    delete ui;
}

void PerformanceTestReportDialog::changeEvent(QEvent* e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void PerformanceTestReportDialog::setSystemInfo(const SystemInfo& info)
{
    m_systemInfo = info;
}

void PerformanceTestReportDialog::setTestResults(
    const QVector< TestResult >& results, const PerformanceTestController::TestConfig& config, bool isFullBenchmark
)
{
    m_results         = results;
    m_config          = config;
    m_isFullBenchmark = isFullBenchmark;

    // 设置报告标题
    if (isFullBenchmark) {
        m_reportTitle = "Comprehensive Performance Benchmark Report";
    } else {
        m_reportTitle = QString("Performance Test Report - %1%2")
                            .arg(config.useOpenGL ? "OpenGL + " : "")
                            .arg(config.useDownsampling ? "Downsampling" : "Baseline");
    }

    // 生成并缓存Markdown报告
    m_reportMarkdown = generateReportMarkdown();
    ui->textBrowser->setMarkdown(m_reportMarkdown);
}

QString PerformanceTestReportDialog::generateReportMarkdown() const
{
    const bool chinese = (m_reportLanguage == Chinese);

    // --- Title ---
    QString md;
    if (chinese) {
        md += "# 性能基准测试报告\n\n";
    } else {
        md += "# Performance Benchmark Report\n\n";
    }

    // --- System Information ---
    if (chinese) {
        md += "## 系统信息\n\n";
        md += "| 项目 | 值 |\n";
        md += "|---|---|\n";
        md += "| 操作系统 | " + m_systemInfo.osName + " |\n";
        md += "| CPU | " + m_systemInfo.cpuModel + " (" + QString::number(m_systemInfo.cpuCoreCount) + " 核) |\n";
        md += "| 内存 | " + QString::number(m_systemInfo.ramTotalMB, 'f', 0) + " MB |\n";
        md += "| GPU | " + m_systemInfo.gpuName + " |\n";
        md += "| 显存 | " + (m_systemInfo.gpuVramMB > 0 ? QString::number(m_systemInfo.gpuVramMB, 'f', 0) + " MB" : "未知") + " |\n";
        md += "| OpenGL | " + m_systemInfo.openglVersion + " |\n";
        md += "| 屏幕 | " + m_systemInfo.screenResolution + " |\n";
        md += "| 磁盘 | " + m_systemInfo.diskType + " |\n";
        md += "| Qt | " + m_systemInfo.qtVersion + " |\n";
        md += "| 编译器 | " + m_systemInfo.compilerInfo + " |\n";
        if (m_systemInfo.isSoftwareOpenGL) {
            md += "\n> ⚠️ **检测到软件OpenGL** (" + m_systemInfo.openglRenderer + ") — 测试结果可能不代表硬件加速性能。\n";
        }
        md += "\n";
    } else {
        md += "## System Information\n\n";
        md += "| Item | Value |\n";
        md += "|---|---|\n";
        md += "| OS | " + m_systemInfo.osName + " |\n";
        md += "| CPU | " + m_systemInfo.cpuModel + " (" + QString::number(m_systemInfo.cpuCoreCount) + " cores) |\n";
        md += "| RAM | " + QString::number(m_systemInfo.ramTotalMB, 'f', 0) + " MB |\n";
        md += "| GPU | " + m_systemInfo.gpuName + " |\n";
        md += "| VRAM | " + (m_systemInfo.gpuVramMB > 0 ? QString::number(m_systemInfo.gpuVramMB, 'f', 0) + " MB" : "Unknown") + " |\n";
        md += "| OpenGL | " + m_systemInfo.openglVersion + " |\n";
        md += "| Screen | " + m_systemInfo.screenResolution + " |\n";
        md += "| Disk | " + m_systemInfo.diskType + " |\n";
        md += "| Qt | " + m_systemInfo.qtVersion + " |\n";
        md += "| Compiler | " + m_systemInfo.compilerInfo + " |\n";
        if (m_systemInfo.isSoftwareOpenGL) {
            md += "\n> ⚠️ **Software OpenGL detected** (" + m_systemInfo.openglRenderer + ") — results may not reflect hardware-accelerated performance.\n";
        }
        md += "\n";
    }

    // --- Test Configuration ---
    if (chinese) {
        md += "## 测试配置\n\n";
        md += QString("- 降采样: %1\n").arg(m_config.useDownsampling ? "开启" : "关闭");
        md += QString("- OpenGL: %1\n").arg(m_config.useOpenGL ? "开启" : "关闭");
        md += QString("- 测试点数: %1\n").arg(formatPointCountList(m_config.pointCounts, true));
        md += QString("- 测试帧数: %1（含 %2 帧预热）\n").arg(m_config.testFrames).arg(m_config.warmupFrames);
    } else {
        md += "## Test Configuration\n\n";
        md += QString("- Downsampling: %1\n").arg(m_config.useDownsampling ? "On" : "Off");
        md += QString("- OpenGL: %1\n").arg(m_config.useOpenGL ? "On" : "Off");
        md += QString("- Point Counts: %1\n").arg(formatPointCountList(m_config.pointCounts, false));
        md += QString("- Test Frames: %1 (including %2 warmup frames)\n").arg(m_config.testFrames).arg(m_config.warmupFrames);
    }
    md += "\n";

    // --- Results (per point-count tables) ---
    if (chinese) {
        md += "## 测试结果\n\n";
    } else {
        md += "## Results\n\n";
    }

    // Group by point count, sorted ascending
    QMap< int, QVector< TestResult > > groupedByPoints;
    for (const TestResult& r : m_results) {
        groupedByPoints[ r.pointCount ].append(r);
    }

    QList< int > sortedPoints = groupedByPoints.keys();
    std::sort(sortedPoints.begin(), sortedPoints.end());

    for (int pointCount : sortedPoints) {
        md += formatResultsTable(groupedByPoints[ pointCount ], pointCount);
    }

    // --- Recommendations ---
    md += generateRecommendationsMarkdown();

    // --- Mermaid Charts (optional) ---
    if (m_includeMermaid) {
        md += generateMermaidCharts();
    }

    return md;
}

QString PerformanceTestReportDialog::formatPointCountList(const QVector<int>& pointCounts, bool chinese) const
{
    QStringList parts;
    for (int pt : pointCounts) {
        parts << formatPointCount(pt, chinese);
    }
    return parts.join(", ");
}

QString PerformanceTestReportDialog::generateMermaidCharts() const
{
    if (m_results.isEmpty())
        return QString();

    const bool chinese = (m_reportLanguage == Chinese);

    // Group data by library name
    struct LibData {
        QVector<int> pointCounts;
        QVector<double> fps;
        QVector<double> renderTime;
        QVector<double> memory;
    };
    QMap<QString, LibData> libDataMap;

    QVector<int> allPointCounts;
    for (const TestResult& r : m_results) {
        if (!allPointCounts.contains(r.pointCount)) {
            allPointCounts.append(r.pointCount);
        }
    }
    std::sort(allPointCounts.begin(), allPointCounts.end());

    for (const TestResult& r : m_results) {
        LibData& data = libDataMap[r.libraryName];
        if (!data.pointCounts.contains(r.pointCount)) {
            data.pointCounts.append(r.pointCount);
        }
    }

    // Ensure all libs have data for all point counts (fill missing with 0)
    for (auto& entry : libDataMap) {
        for (int pt : allPointCounts) {
            if (!entry.pointCounts.contains(pt)) {
                entry.pointCounts.append(pt);
                entry.fps.append(0);
                entry.renderTime.append(0);
                entry.memory.append(0);
            }
        }
    }

    // Actually populate the values
    QMap<QString, QMap<int, TestResult>> resultMap;
    for (const TestResult& r : m_results) {
        resultMap[r.libraryName][r.pointCount] = r;
    }

    for (auto it = libDataMap.begin(); it != libDataMap.end(); ++it) {
        QString libName = it.key();
        it->pointCounts.clear();
        it->fps.clear();
        it->renderTime.clear();
        it->memory.clear();
        for (int pt : allPointCounts) {
            it->pointCounts.append(pt);
            if (resultMap.contains(libName) && resultMap[libName].contains(pt)) {
                const TestResult& r = resultMap[libName][pt];
                it->fps.append(r.fps);
                it->renderTime.append(r.renderTime);
                it->memory.append(r.memoryUsage);
            } else {
                it->fps.append(0);
                it->renderTime.append(0);
                it->memory.append(0);
            }
        }
    }

    // Sort point counts and format X-axis labels
    QStringList xLabels;
    for (int pt : allPointCounts) {
        xLabels << formatPointCountLabel(pt);
    }

    // Library sort order
    static const QStringList libOrder = { "QIm", "Qwt", "QCustomPlot", "Qt Charts" };
    QStringList sortedLibNames = libDataMap.keys();
    std::sort(sortedLibNames.begin(), sortedLibNames.end(), [](const QString& a, const QString& b) {
        static const QStringList order = { "QIm", "Qwt", "QCustomPlot", "Qt Charts" };
        int idxA = order.indexOf(a);
        int idxB = order.indexOf(b);
        if (idxA == -1) idxA = 999;
        if (idxB == -1) idxB = 999;
        return idxA < idxB;
    });

    QString mermaid;

    // --- FPS Chart ---
    mermaid += "## ";
    mermaid += chinese ? "FPS 对比\n" : "FPS Comparison\n";
    mermaid += "\n```mermaid\n";
    mermaid += "xychart-beta\n";
    mermaid += QString("    title \"%1\"\n").arg(chinese ? "FPS 对比" : "FPS Comparison");
    mermaid += QString("    x-axis [%1]\n").arg(xLabels.join(", "));
    // Calculate FPS range
    double maxFps = 0;
    for (const auto& lib : sortedLibNames) {
        for (double v : libDataMap[lib].fps) {
            if (v > maxFps) maxFps = v;
        }
    }
    mermaid += QString("    y-axis \"FPS\" 0 --> %1\n").arg(maxFps * 1.1, 0, 'f', 0);
    for (const QString& lib : sortedLibNames) {
        mermaid += QString("    line \"%1\" [%2]\n")
                       .arg(lib)
                       .arg(formatNumberList(libDataMap[lib].fps, 2));
    }
    mermaid += "```\n\n";

    // --- Render Time Chart ---
    mermaid += "## ";
    mermaid += chinese ? "渲染时间对比\n" : "Render Time Comparison\n";
    mermaid += "```mermaid\n";
    mermaid += "xychart-beta\n";
    mermaid += QString("    title \"%1\"\n").arg(chinese ? "渲染时间对比 (ms)" : "Render Time Comparison (ms)");
    mermaid += QString("    x-axis [%1]\n").arg(xLabels.join(", "));
    double maxRenderTime = 0;
    for (const auto& lib : sortedLibNames) {
        for (double v : libDataMap[lib].renderTime) {
            if (v > maxRenderTime) maxRenderTime = v;
        }
    }
    mermaid += QString("    y-axis \"Render Time (ms)\" 0 --> %1\n").arg(maxRenderTime * 1.1, 0, 'f', 0);
    for (const QString& lib : sortedLibNames) {
        mermaid += QString("    line \"%1\" [%2]\n")
                       .arg(lib)
                       .arg(formatNumberList(libDataMap[lib].renderTime, 2));
    }
    mermaid += "```\n\n";

    // --- Memory Chart ---
    mermaid += "## ";
    mermaid += chinese ? "内存消耗对比\n" : "Memory Consumption Comparison\n";
    mermaid += "```mermaid\n";
    mermaid += "xychart-beta\n";
    mermaid += QString("    title \"%1\"\n").arg(chinese ? "内存消耗对比 (MB)" : "Memory Consumption Comparison (MB)");
    mermaid += QString("    x-axis [%1]\n").arg(xLabels.join(", "));
    double maxMemory = 0;
    for (const auto& lib : sortedLibNames) {
        for (double v : libDataMap[lib].memory) {
            if (v > maxMemory) maxMemory = v;
        }
    }
    mermaid += QString("    y-axis \"Memory (MB)\" 0 --> %1\n").arg(maxMemory * 1.1, 0, 'f', 0);
    for (const QString& lib : sortedLibNames) {
        mermaid += QString("    line \"%1\" [%2]\n")
                       .arg(lib)
                       .arg(formatNumberList(libDataMap[lib].memory, 2));
    }
    mermaid += "```\n";

    return mermaid;
}

// Helpers for Mermaid generation
QString PerformanceTestReportDialog::formatPointCountLabel(int pointCount) const
{
    if (pointCount >= 1000000) {
        return QString("%1M").arg(pointCount / 1000000);
    }
    return QString("%1K").arg(pointCount / 1000);
}

QString PerformanceTestReportDialog::formatNumberList(const QVector<double>& values, int precision) const
{
    QStringList parts;
    for (double v : values) {
        parts << QString::number(v, 'f', precision);
    }
    return parts.join(", ");
}

QString PerformanceTestReportDialog::formatResultsTable(const QVector<TestResult>& results, int pointCount) const
{
    const bool chinese = (m_reportLanguage == Chinese);

    // Library sort order: QIm, Qwt, QCustomPlot, Qt Charts
    static const QStringList order = { "QIm", "Qwt", "QCustomPlot", "Qt Charts" };
    QVector<TestResult> sortedResults = results;
    std::sort(sortedResults.begin(), sortedResults.end(), [](const TestResult& a, const TestResult& b) {
        static const QStringList libOrder = { "QIm", "Qwt", "QCustomPlot", "Qt Charts" };
        int idxA = libOrder.indexOf(a.libraryName);
        int idxB = libOrder.indexOf(b.libraryName);
        if (idxA == -1) idxA = 999;
        if (idxB == -1) idxB = 999;
        return idxA < idxB;
    });

    QString md;
    if (chinese) {
        md += QString("### %1 数据点\n\n").arg(formatPointCount(pointCount, true));
        md += "| 库 | 设置时间 (ms) | 渲染时间 (ms) | FPS | 内存 (MB) | OpenGL | 降采样 |\n";
        md += "|:---|:---|:---|:---|:---|:---|:---|\n";
    } else {
        md += QString("### %1 Points\n\n").arg(formatPointCount(pointCount, false));
        md += "| Library | Setup Time (ms) | Render Time (ms) | FPS | Memory (MB) | OpenGL | Downsampling |\n";
        md += "|---|---|---|---|---|---|---|\n";
    }

    for (const TestResult& r : sortedResults) {
        QString renderTime = QString::number(r.renderTime, 'f', 2);
        QString fps = QString::number(r.fps, 'f', 4);
        // Trim trailing zeros from FPS
        while (fps.contains('.') && fps.endsWith('0')) { fps.chop(1); }
        if (fps.endsWith('.')) fps.chop(1);

        QString memory = QString::number(r.memoryUsage, 'f', 2);
        if (r.memoryUsage < 10.0) {
            memory = QString::number(r.memoryUsage, 'f', 5);
            while (memory.contains('.') && memory.endsWith('0')) { memory.chop(1); }
            if (memory.endsWith('.')) memory.chop(1);
        }

        QString setupTime = QString::number(r.setupTime, 'f', 2);
        QString openglStr = r.usedOpenGL ? "✓" : "×";
        QString downsampleStr = r.usedDownsampling ? "✓" : "×";
        md += QString("| %1 | %2 | %3 | %4 | %5 | %6 | %7 |\n")
                    .arg(r.libraryName).arg(setupTime).arg(renderTime).arg(fps).arg(memory)
                    .arg(openglStr).arg(downsampleStr);
        
    }
    md += "\n";
    return md;
}

QString PerformanceTestReportDialog::generateRecommendationsMarkdown() const
{
    const bool chinese = (m_reportLanguage == Chinese);

    QString md;
    if (chinese) {
        md += "## 建议\n\n";
        if (m_config.useOpenGL && m_config.useDownsampling) {
            md += "- **生产部署**: 此配置（OpenGL + 降采样）最适合大数据量（>10万点）的实时应用\n";
        } else if (m_config.useOpenGL) {
            md += "- **中等数据集**: OpenGL加速显著提升FPS；对于 >50万点数据建议开启降采样\n";
        } else if (m_config.useDownsampling) {
            md += "- **老旧硬件**: 降采样在无GPU加速的系统上仍可保持可用性\n";
        } else {
            md += "- **调试/小数据集**: 基础配置适合 <5万点数据，视觉保真度最高\n";
        }
        md += "- **内存注意**: 监控内存增长；>100万点数据可能需要流式或虚拟化技术\n";
        md += "- **验证**: 务必在目标硬件上测试——性能因GPU/驱动差异显著\n";
    } else {
        md += "## Recommendations\n\n";
        if (m_config.useOpenGL && m_config.useDownsampling) {
            md += "- **Production Deployment**: This configuration (OpenGL + Downsampling) is optimal for real-time applications with large datasets (>100k points)\n";
        } else if (m_config.useOpenGL) {
            md += "- **Medium Datasets**: OpenGL acceleration provides significant FPS boost; consider adding downsampling for datasets >500k points\n";
        } else if (m_config.useDownsampling) {
            md += "- **Legacy Hardware**: Downsampling maintains usability on systems without GPU acceleration\n";
        } else {
            md += "- **Debugging/Small Datasets**: Baseline configuration suitable for datasets <50k points where visual fidelity is critical\n";
        }
        md += "- **Memory Consideration**: Monitor memory usage growth; datasets >1M points may require streaming or virtualization techniques\n";
        md += "- **Validation**: Always test on target hardware - performance varies significantly across GPU/drivers\n";
    }
    md += "\n";
    return md;
}

void PerformanceTestReportDialog::onPushButtonCopy_clicked()
{
    if (m_results.isEmpty()) {
        QMessageBox::warning(this, "Warning", "No test results to copy");
        return;
    }

    // Use cached Markdown report
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(m_reportMarkdown);

    const bool chinese = (m_reportLanguage == Chinese);
    if (chinese) {
        QMessageBox::information(this, "Copied", QString(" 已复制 Markdown 报告到剪贴板"));
    } else {
        QMessageBox::information(this, "Copied", QString("Markdown report copied to clipboard"));
    }
}

void PerformanceTestReportDialog::onPushButtonSaveMarkdown_clicked()
{
    if (m_reportMarkdown.isEmpty()) {
        QMessageBox::warning(this, "Warning", "No report to save");
        return;
    }

    const bool chinese = (m_reportLanguage == Chinese);
    QString defaultName = chinese ? "benchmark_report.md" : "benchmark_report.md";
    QString filePath = QFileDialog::getSaveFileName(this,
        chinese ? tr("保存 Markdown 报告") : tr("Save Markdown Report"),
        defaultName,
        tr("Markdown Files (*.md);;All Files (*)"));
    if (filePath.isEmpty()) return;

    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.write(m_reportMarkdown.toUtf8());
        file.close();
        QMessageBox::information(this, chinese ? "成功" : "Success",
            chinese ? QString("报告已保存到:\n%1").arg(filePath) : QString("Report saved to:\n%1").arg(filePath));
    } else {
        QMessageBox::warning(this, chinese ? "错误" : "Error",
            chinese ? QString("无法保存文件:\n%1").arg(filePath) : QString("Failed to save file:\n%1").arg(filePath));
    }
}

void PerformanceTestReportDialog::onPushButtonExportToPdf_clicked()
{
    QString fileName =
        QFileDialog::getSaveFileName(this, "Export Report to PDF", "performance_report.pdf", "PDF Files (*.pdf)");

    if (fileName.isEmpty())
        return;

    if (!fileName.endsWith(".pdf", Qt::CaseInsensitive))
        fileName += ".pdf";

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageMargins(QMarginsF(15, 15, 15, 15), QPageLayout::Millimeter);

    ui->textBrowser->document()->print(&printer);

    QMessageBox::information(this, "Success", QString("Report exported successfully to:\n%1").arg(fileName));
}
