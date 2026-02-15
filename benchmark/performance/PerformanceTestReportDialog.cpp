#include "PerformanceTestReportDialog.h"
#include "ui_PerformanceTestReportDialog.h"
#include "qcustomplot.h"
#include <QPixmap>
#include <QBuffer>
#include <QDateTime>
#include <QFile>
#include <QTextDocument>
#include <QPrinter>
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include <QDebug>

PerformanceTestReportDialog::PerformanceTestReportDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::PerformanceTestReportDialog)
{
    ui->setupUi(this);
    setWindowTitle("Performance Test Report");
    resize(1000, 800);

    // 连接按钮信号
    connect(ui->pushButtonCopy, &QPushButton::clicked, this, &PerformanceTestReportDialog::onPushButtonCopy_clicked);
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

    // 生成并显示报告
    QString reportHtml = generateReportHtml();
    QFile file("./report.html");
    if (file.open(QIODevice::ReadWrite)) {
        file.write(reportHtml.toUtf8());
    }
    ui->textBrowser->setHtml(reportHtml);
}

QString PerformanceTestReportDialog::generateReportHtml() const
{
    QDateTime now = QDateTime::currentDateTime();

    // 生成性能图表（必须在HTML生成前完成）

    QPixmap renderTimeChart  = generateCostChart(m_results);
    QString renderTimeBase64 = pixmapToBase64(renderTimeChart);

    QPixmap fpsChart       = generateFpsChart(m_results);
    QString fpsChartBase64 = pixmapToBase64(fpsChart);

    QPixmap memoryChart       = generateMemoryChart(m_results);
    QString memoryChartBase64 = pixmapToBase64(memoryChart);

    // 格式化配置标签
    QString openglTagClass = m_config.useOpenGL ? "tag-opengl" : "tag-baseline";
    QString openglTagText  = m_config.useOpenGL ? "OpenGL Enabled" : "OpenGL Disabled";

    QString downsampleTagClass = m_config.useDownsampling ? "tag-downsample" : "tag-baseline";
    QString downsampleTagText  = m_config.useDownsampling ? "Downsampling Enabled" : "Downsampling Disabled";

    // 格式化点数列表
    QStringList pointStrs;
    for (int pt : m_config.pointCounts) {
        if (pt >= 1000000)
            pointStrs << QString("%1M").arg(pt / 1000000);
        else if (pt >= 1000)
            pointStrs << QString("%1k").arg(pt / 1000);
        else
            pointStrs << QString::number(pt);
    }
    QString pointsList = pointStrs.join(", ");

    // 修复：拆分 arg() 调用，避免超过9参数限制
    QString htmlTemplatePart1 = R"(
    <!DOCTYPE html>
    <html>
    <head>
        <meta charset="UTF-8">
        <title>%1</title>
        <style>
            * {
                margin: 0;
                padding: 0;
                box-sizing: border-box;
            }

            body {
                font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, "Helvetica Neue", Arial, sans-serif;
                background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
                padding: 20px;
                color: #333;
            }

            .container {
                max-width: 1200px;
                margin: 0 auto;
                background: white;
                border-radius: 15px;
                box-shadow: 0 10px 40px rgba(0, 0, 0, 0.2);
                overflow: hidden;
            }

            .header {
                background: linear-gradient(135deg, #2c3e50 0%, #4a6fa5 100%);
                color: black;
                padding: 30px;
                text-align: center;
            }

            .header h1 {
                font-size: 28px;
                margin-bottom: 10px;
                font-weight: 600;
            }

            .header .subtitle {
                font-size: 14px;
                opacity: 0.9;
                margin-top: 5px;
            }

            .config-tags {
                display: flex;
                justify-content: center;
                gap: 15px;
                margin: 20px 0;
                flex-wrap: wrap;
            }

            .tag {
                padding: 6px 16px;
                border-radius: 20px;
                font-size: 13px;
                font-weight: 500;
                background: rgba(255, 255, 255, 0.2);
                backdrop-filter: blur(10px);
            }

            .tag-opengl { background: rgba(46, 204, 113, 0.3); color: #27ae60; }
            .tag-downsample { background: rgba(52, 152, 219, 0.3); color: #2980b9; }
            .tag-baseline { background: rgba(155, 89, 182, 0.3); color: #8e44ad; }

            .content {
                padding: 30px;
            }

            /* ========== 图表样式 ========== */
            .chart-section {
                margin: 30px 0;
                padding: 20px;
                background: white;
                border-radius: 10px;
                box-shadow: 0 2px 10px rgba(0, 0, 0, 0.08);
            }

            .chart-section h3 {
                margin: 0 0 15px 0;
                color: #2c3e50;
                font-size: 18px;
                font-weight: 600;
                display: flex;
                align-items: center;
                gap: 10px;
            }

            .chart-wrapper {
                display: flex;
                justify-content: center;
                align-items: center;
                padding: 10px;
                background: #f8f9fa;
                border-radius: 8px;
            }

            .report-chart {
                max-width: 100%;
                height: auto;
                display: block;
                border-radius: 6px;
                box-shadow: 0 3px 10px rgba(0, 0, 0, 0.12);
                transition: transform 0.3s ease;
            }

            .report-chart:hover {
                transform: scale(1.02);
                box-shadow: 0 5px 15px rgba(0, 0, 0, 0.18);
            }

            /* ========== 表格样式 ========== */
            .point-group {
                margin-bottom: 35px;
                background: #f8f9fa;
                border-radius: 10px;
                padding: 20px;
                box-shadow: 0 2px 6px rgba(0, 0, 0, 0.05);
            }

            .point-group h3 {
                margin: 0 0 15px 0;
                color: #2c3e50;
                font-size: 17px;
                font-weight: 600;
                display: flex;
                align-items: center;
                gap: 8px;
            }

            .results-table {
                width: 100%;
                border-collapse: collapse;
                margin: 10px 0;
                font-size: 13px;
                background: white;
                border-radius: 8px;
                overflow: hidden;
                box-shadow: 0 1px 4px rgba(0, 0, 0, 0.08);
            }

            .results-table th {
                background: linear-gradient(135deg, #4a6fa5 0%, #2c3e50 100%);
                color: black;
                padding: 12px 10px;
                text-align: left;
                font-weight: 600;
                font-size: 12px;
            }

            .results-table td {
                padding: 10px;
                border-bottom: 1px solid #e9ecef;
            }

            .results-table tr:last-child td {
                border-bottom: none;
            }

            .results-table tr:hover {
                background: #f1f8ff;
            }

            tr.excellent {
                background-color: #d4edda !important;
                border-left: 4px solid #28a745;
            }

            tr.acceptable {
                background-color: #fff3cd !important;
                border-left: 4px solid #ffc107;
            }

            tr.poor {
                background-color: #f8d7da !important;
                border-left: 4px solid #dc3545;
            }

            /* ========== 推荐部分 ========== */
            .recommendations {
                background: linear-gradient(135deg, #f6d365 0%, #fda085 100%);
                padding: 20px;
                border-radius: 10px;
                margin: 30px 0;
            }

            .recommendations h3 {
                color: #2c3e50;
                margin-bottom: 15px;
                font-size: 18px;
                display: flex;
                align-items: center;
                gap: 10px;
            }

            .recommendations ul {
                margin-left: 20px;
                line-height: 1.8;
            }

            .recommendations li {
                margin-bottom: 8px;
                color: #2c3e50;
            }

            /* ========== 页脚 ========== */
            .footer {
                margin-top: 40px;
                padding-top: 20px;
                border-top: 1px solid #e9ecef;
                text-align: center;
                color: #6c757d;
                font-size: 13px;
            }

            .footer small {
                display: block;
                margin-top: 8px;
                color: #999;
                font-size: 11px;
                line-height: 1.5;
            }

            /* 响应式设计 */
            @media (max-width: 768px) {
                .header {
                    padding: 20px;
                }

                .header h1 {
                    font-size: 22px;
                }

                .content {
                    padding: 20px;
                }

                .chart-section {
                    padding: 15px;
                }

                .report-chart {
                    max-width: 95%;
                }

                .results-table {
                    font-size: 12px;
                }

                .results-table th,
                .results-table td {
                    padding: 8px 6px;
                }
            }
        </style>
    </head>
    <body>
        <div class="container">
            <div class="header">
                <h1>%1</h1>
                <div class="subtitle">Generated: %2</div>

                <div class="config-tags">
                    <span class="tag %3">%4</span>
                    <span class="tag %5">%6</span>
                </div>

                <div style="margin-top: 15px; font-size: 13px;">
                    <strong>Point Counts Tested:</strong> %7<br>
                    <strong>Total Test Frames:</strong> %8 (including %9 warmup frames)
                </div>
            </div>

            <div class="content">
    )";

    // 第一步：替换前10个参数
    QString htmlPart1 = htmlTemplatePart1
                            .arg(m_reportTitle)                        // %1
                            .arg(now.toString("yyyy-MM-dd hh:mm:ss"))  // %2
                            .arg(openglTagClass)
                            .arg(openglTagText)  // %3, %4
                            .arg(downsampleTagClass)
                            .arg(downsampleTagText)      // %5, %6
                            .arg(pointsList)             // %7
                            .arg(m_config.testFrames)    // %8
                            .arg(m_config.warmupFrames)  // %9
        ;
    // 第二步：添加图表部分
    QString htmlPart2 = R"(
                <div class="chart-section">
                    <h3>📈 Rendering Performance (Render Time vs Data Points)</h3>
                    <div class="chart-wrapper">
                        <img src="data:image/png;base64,%1" alt="Render Time Chart" class="report-chart">
                    </div>
                </div>

                <div class="chart-section">
                    <h3>📈 Rendering Performance (FPS vs Data Points)</h3>
                    <div class="chart-wrapper">
                        <img src="data:image/png;base64,%2" alt="FPS Chart" class="report-chart">
                    </div>
                </div>

                <div class="chart-section">
                    <h3>💾 Memory Consumption vs Data Points</h3>
                    <div class="chart-wrapper">
                        <img src="data:image/png;base64,%3" alt="Memory Chart" class="report-chart">
                    </div>
                </div>

                %4  <!-- Detailed results tables -->
                %5  <!-- Recommendations -->

                <div class="footer">
                    Report generated by QIm Performance Benchmark Tool • %6
                    <br><small>Note: Performance characteristics vary by hardware, drivers, and Qt build configuration. Always validate on target deployment environment.</small>
                </div>
            </div>
        </body>
        </html>
        )";

    // 第二步替换：图表 + 详细结果 + 建议 + 年份
    QString htmlPart2Filled = htmlPart2
                                  .arg(renderTimeBase64)                // %1
                                  .arg(fpsChartBase64)                  // %2
                                  .arg(memoryChartBase64)               // %3
                                  .arg(generateDetailedResultsTable())  // %4
                                  .arg(generateRecommendations())       // %5
                                  .arg(now.toString("yyyy"));           // %6

    // 合并两部分
    return htmlPart1 + htmlPart2Filled;
}

QPixmap PerformanceTestReportDialog::generateFpsChart(const QVector< TestResult >& results) const
{
    // 创建临时QCustomPlot用于渲染
    QCustomPlot plot;
    plot.resize(800, 400);

    QMap< QString, QVector< QPointF > > dataMap;
    for (const TestResult& result : results) {
        dataMap[ result.libraryName ].append(QPointF(result.pointCount, result.fps));
    }

    struct Styles
    {
        QColor color { Qt::black };
        QCPScatterStyle::ScatterShape shape { QCPScatterStyle::ssCircle };
    };

    QMap< QString, Styles > style;
    style[ "QIm" ]         = { QColor(52, 152, 219), QCPScatterStyle::ssCircle };    // 蓝色
    style[ "Qwt" ]         = { QColor(231, 76, 60), QCPScatterStyle::ssSquare };     // 红色
    style[ "QCustomPlot" ] = { QColor(46, 204, 113), QCPScatterStyle::ssTriangle };  // 绿色
    style[ "Qt Charts" ]   = { QColor(155, 89, 182), QCPScatterStyle::ssDisc };      // 紫色

    int graphIndex = 0;
    for (auto it = dataMap.begin(); it != dataMap.end(); ++it) {
        plot.addGraph();

        QVector< double > keys, values;
        for (const QPointF& p : it.value()) {
            keys.append(p.x());
            values.append(p.y());
        }

        plot.graph(graphIndex)->setData(keys, values, true);
        plot.graph(graphIndex)->setPen(QPen(style.value(it.key()).color, 3));
        plot.graph(graphIndex)->setName(it.key());
        plot.graph(graphIndex)->setScatterStyle(QCPScatterStyle(style.value(it.key()).shape, 9));
        plot.graph(graphIndex)->setLineStyle(QCPGraph::lsLine);

        graphIndex++;
    }

    // 配置坐标轴
    plot.xAxis->setLabel("Number of Points");
    // plot.xAxis->setScaleType(QCPAxis::stLogarithmic);
    plot.xAxis->setNumberFormat("ebc");
    plot.xAxis->setNumberPrecision(0);
    plot.xAxis->setLabelFont(QFont("Segoe UI", 11, QFont::Bold));
    plot.xAxis->setTickLabelFont(QFont("Segoe UI", 10));

    plot.yAxis->setLabel("FPS (Frames Per Second)");
    plot.yAxis->setLabelFont(QFont("Segoe UI", 11, QFont::Bold));
    plot.yAxis->setTickLabelFont(QFont("Segoe UI", 10));


    // 网格
    plot.xAxis->grid()->setVisible(true);
    plot.yAxis->grid()->setVisible(true);
    plot.xAxis->grid()->setPen(QPen(QColor(200, 200, 200), 0.8));
    plot.yAxis->grid()->setPen(QPen(QColor(200, 200, 200), 0.8));

    // 图例
    plot.legend->setVisible(true);
    plot.legend->setFont(QFont("Segoe UI", 10));
    plot.legend->setBrush(QColor(255, 255, 255, 230));
    plot.legend->setBorderPen(QPen(QColor(150, 150, 150)));
    plot.axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignRight);

    // 标题
    plot.plotLayout()->insertRow(0);
    QCPTextElement* title =
        new QCPTextElement(&plot, "Rendering Performance Comparison", QFont("Segoe UI", 14, QFont::Bold));
    plot.plotLayout()->addElement(0, 0, title);

    plot.rescaleAxes(true);
    plot.xAxis->scaleRange(1.1);
    plot.yAxis->scaleRange(1.1);
    plot.xAxis->setRangeLower(0);
    plot.yAxis->setRangeLower(0);

    plot.replot();

    // 渲染为Pixmap
    return plot.toPixmap(plot.width(), plot.height());
}

QPixmap PerformanceTestReportDialog::generateCostChart(const QVector< TestResult >& results) const
{
    // 创建临时QCustomPlot用于渲染
    QCustomPlot plot;
    plot.resize(800, 400);

    // 按库分组数据
    QMap< QString, QVector< QPointF > > dataMap;
    for (const TestResult& result : results) {
        dataMap[ result.libraryName ].append(QPointF(result.pointCount, result.renderTime));
    }

    struct Styles
    {
        QColor color { Qt::black };
        QCPScatterStyle::ScatterShape shape { QCPScatterStyle::ssCircle };
    };
    QMap< QString, Styles > style;
    style[ "QIm" ]         = { QColor(52, 152, 219), QCPScatterStyle::ssCircle };    // 蓝色
    style[ "Qwt" ]         = { QColor(231, 76, 60), QCPScatterStyle::ssSquare };     // 红色
    style[ "QCustomPlot" ] = { QColor(46, 204, 113), QCPScatterStyle::ssTriangle };  // 绿色
    style[ "Qt Charts" ]   = { QColor(155, 89, 182), QCPScatterStyle::ssDisc };      // 紫色

    int graphIndex = 0;
    for (auto it = dataMap.begin(); it != dataMap.end(); ++it) {
        plot.addGraph();

        QVector< double > keys, values;
        for (const QPointF& p : it.value()) {
            keys.append(p.x());
            values.append(p.y());
        }

        plot.graph(graphIndex)->setData(keys, values, true);
        plot.graph(graphIndex)->setPen(QPen(style.value(it.key()).color, 3));
        plot.graph(graphIndex)->setName(it.key());
        plot.graph(graphIndex)->setScatterStyle(QCPScatterStyle(style.value(it.key()).shape, 9));
        plot.graph(graphIndex)->setLineStyle(QCPGraph::lsLine);

        graphIndex++;
    }

    // 配置坐标轴
    plot.xAxis->setLabel("Number of Points");
    // plot.xAxis->setScaleType(QCPAxis::stLogarithmic);
    plot.xAxis->setNumberFormat("ebc");
    plot.xAxis->setNumberPrecision(0);
    plot.xAxis->setLabelFont(QFont("Segoe UI", 11, QFont::Bold));
    plot.xAxis->setTickLabelFont(QFont("Segoe UI", 10));

    plot.yAxis->setLabel("Render Time (ms)");
    plot.yAxis->setLabelFont(QFont("Segoe UI", 11, QFont::Bold));
    plot.yAxis->setTickLabelFont(QFont("Segoe UI", 10));


    // 网格
    plot.xAxis->grid()->setVisible(true);
    plot.yAxis->grid()->setVisible(true);
    plot.xAxis->grid()->setPen(QPen(QColor(200, 200, 200), 0.8));
    plot.yAxis->grid()->setPen(QPen(QColor(200, 200, 200), 0.8));

    // 图例
    plot.legend->setVisible(true);
    plot.legend->setFont(QFont("Segoe UI", 10));
    plot.legend->setBrush(QColor(255, 255, 255, 230));
    plot.legend->setBorderPen(QPen(QColor(150, 150, 150)));
    plot.axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignRight);

    // 标题
    plot.plotLayout()->insertRow(0);
    QCPTextElement* title =
        new QCPTextElement(&plot, "Rendering Performance Comparison", QFont("Segoe UI", 14, QFont::Bold));
    plot.plotLayout()->addElement(0, 0, title);

    plot.rescaleAxes(true);
    plot.xAxis->scaleRange(1.1);
    plot.yAxis->scaleRange(1.1);
    plot.xAxis->setRangeLower(0);
    plot.yAxis->setRangeLower(0);
    plot.replot();

    // 渲染为Pixmap
    return plot.toPixmap(plot.width(), plot.height());
}

QPixmap PerformanceTestReportDialog::generateMemoryChart(const QVector< TestResult >& results) const
{
    QCustomPlot plot;
    plot.resize(800, 400);

    // 按库分组数据
    QMap< QString, QVector< QPointF > > dataMap;
    for (const TestResult& result : results) {
        dataMap[ result.libraryName ].append(QPointF(result.pointCount, result.memoryUsage));
    }

    struct Styles
    {
        QColor color { Qt::black };
        QCPScatterStyle::ScatterShape shape { QCPScatterStyle::ssCircle };
    };

    QMap< QString, Styles > style;
    style[ "QIm" ]         = { QColor(52, 152, 219), QCPScatterStyle::ssCircle };    // 蓝色
    style[ "Qwt" ]         = { QColor(231, 76, 60), QCPScatterStyle::ssSquare };     // 红色
    style[ "QCustomPlot" ] = { QColor(46, 204, 113), QCPScatterStyle::ssTriangle };  // 绿色
    style[ "Qt Charts" ]   = { QColor(155, 89, 182), QCPScatterStyle::ssDisc };      // 紫色

    int graphIndex = 0;
    for (auto it = dataMap.begin(); it != dataMap.end(); ++it) {
        plot.addGraph();

        QVector< double > keys, values;
        for (const QPointF& p : it.value()) {
            keys.append(p.x());
            values.append(p.y());
        }

        plot.graph(graphIndex)->setData(keys, values, true);
        plot.graph(graphIndex)->setPen(QPen(style.value(it.key()).color, 3));
        plot.graph(graphIndex)->setName(it.key());
        plot.graph(graphIndex)->setScatterStyle(QCPScatterStyle(style.value(it.key()).shape, 9));
        plot.graph(graphIndex)->setLineStyle(QCPGraph::lsLine);

        graphIndex++;
    }

    // 配置坐标轴
    plot.xAxis->setLabel("Number of Points");
    // plot.xAxis->setScaleType(QCPAxis::stLogarithmic);
    plot.xAxis->setNumberFormat("ebc");
    plot.xAxis->setNumberPrecision(0);
    plot.xAxis->setLabelFont(QFont("Segoe UI", 11, QFont::Bold));

    plot.yAxis->setLabel("Memory Usage (MB)");
    plot.yAxis->setLabelFont(QFont("Segoe UI", 11, QFont::Bold));

    // 网格
    plot.xAxis->grid()->setVisible(true);
    plot.yAxis->grid()->setVisible(true);

    // 图例
    plot.legend->setVisible(true);
    plot.legend->setFont(QFont("Segoe UI", 10));
    plot.legend->setBrush(QColor(255, 255, 255, 230));

    // 标题
    plot.plotLayout()->insertRow(0);
    QCPTextElement* title = new QCPTextElement(&plot, "Memory Consumption Comparison", QFont("Segoe UI", 14, QFont::Bold));
    plot.plotLayout()->addElement(0, 0, title);

    plot.rescaleAxes(true);
    plot.xAxis->scaleRange(1.1);
    plot.yAxis->scaleRange(1.1);
    plot.xAxis->setRangeLower(0);
    plot.yAxis->setRangeLower(0);
    plot.replot();

    return plot.toPixmap(plot.width(), plot.height());
}

QString PerformanceTestReportDialog::pixmapToBase64(const QPixmap& pixmap) const
{
    if (pixmap.isNull())
        return "";

    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "PNG");
    buffer.close();

    return byteArray.toBase64();
}


QString PerformanceTestReportDialog::generateDetailedResultsTable() const
{
    // 按点数分组结果
    QMap< int, QVector< TestResult > > groupedByPoints;
    for (const TestResult& r : m_results) {
        groupedByPoints[ r.pointCount ].append(r);
    }

    QString tables;

    // 为每个点数生成独立表格
    for (auto it = groupedByPoints.begin(); it != groupedByPoints.end(); ++it) {
        int pointCount                                = it.key();
        const QVector< TestResult >& resultsForPoints = it.value();

        // 格式化点数显示
        QString pointsLabel;
        if (pointCount >= 1000000)
            pointsLabel = QString("%1 Million Points").arg(pointCount / 1000000);
        else if (pointCount >= 1000)
            pointsLabel = QString("%1K Points").arg(pointCount / 1000);
        else
            pointsLabel = QString("%1 Points").arg(pointCount);

        // 表格标题
        tables += QString(R"(
                <div class="point-group">
                    <h3>📊 %1 Test Results</h3>
                    <table class="results-table">
                        <thead>
                            <tr>
                                <th>Library</th>
                                <th>Setup Time (ms)</th>
                                <th>Render Time (ms)</th>
                                <th>FPS</th>
                                <th>Memory (MB)</th>
                                <th>OpenGL</th>
                                <th>Downsampling</th>
                            </tr>
                        </thead>
                        <tbody>
            )")
                      .arg(pointsLabel);

        // 按FPS排序（从高到低）以突出最佳表现
        QVector< TestResult > sortedResults = resultsForPoints;
        std::sort(sortedResults.begin(), sortedResults.end(), [](const TestResult& a, const TestResult& b) {
            return a.fps > b.fps;
        });

        // 生成表格行
        for (const TestResult& r : sortedResults) {
            QString rowClass = getPerformanceClass(r.fps);

            tables += QString(R"(
                    <tr class="%1">
                        <td><strong>%2</strong></td>
                        <td>%3</td>
                        <td>%4</td>
                        <td><strong>%5</strong></td>
                        <td>%6</td>
                        <td>%7</td>
                        <td>%8</td>
                    </tr>
                )")
                          .arg(rowClass)
                          .arg(r.libraryName)
                          .arg(r.setupTime)
                          .arg(r.renderTime)
                          .arg(r.fps)
                          .arg(r.memoryUsage)
                          .arg(r.usedOpenGL ? "✓ Yes" : "× No")
                          .arg(r.usedDownsampling ? "✓ Yes" : "× No");
        }

        tables += R"(
                        </tbody>
                    </table>
                </div>
            )";
    }

    return tables;
}

QString PerformanceTestReportDialog::generateRecommendations() const
{
    QString recommendations = R"(
    <div class="recommendation">
        <h2>💡 Expert Recommendations</h2>
        <ul>
    )";

    if (m_config.useOpenGL && m_config.useDownsampling) {
        recommendations += "<li><strong>Production Deployment:</strong> This configuration (OpenGL + Downsampling) is "
                           "optimal for real-time applications with large datasets (>100k points)</li>";
    } else if (m_config.useOpenGL) {
        recommendations += "<li><strong>Medium Datasets:</strong> OpenGL acceleration provides significant FPS boost; "
                           "consider adding downsampling for datasets >500k points</li>";
    } else if (m_config.useDownsampling) {
        recommendations += "<li><strong>Legacy Hardware:</strong> Downsampling maintains usability on systems without "
                           "GPU acceleration</li>";
    } else {
        recommendations += "<li><strong>Debugging/Small Datasets:</strong> Baseline configuration suitable for "
                           "datasets <50k points where visual fidelity is critical</li>";
    }


    recommendations += R"(
            <li><strong>Memory Consideration:</strong> Monitor memory usage growth; datasets >1M points may require streaming or virtualization techniques</li>
            <li><strong>Validation:</strong> Always test on target hardware - performance varies significantly across GPU/drivers</li>
        </ul>
    </div>
    )";

    return recommendations;
}

QString PerformanceTestReportDialog::getPerformanceClass(double fps) const
{
    if (fps > 50)
        return "excellent";
    if (fps > 15)
        return "acceptable";
    return "poor";
}

void PerformanceTestReportDialog::onPushButtonCopy_clicked()
{
    if (m_results.isEmpty()) {
        QMessageBox::warning(this, "Warning", "No test results to copy");
        return;
    }

    // 1. 生成配置描述
    QString openglStatus     = m_config.useOpenGL ? "开启" : "关闭";
    QString downsampleStatus = m_config.useDownsampling ? "开启" : "关闭";
    QString configDesc       = QString("降采样%1 + OpenGL %2").arg(downsampleStatus).arg(openglStatus);

    // 2. 按点数分组结果
    QMap< int, QVector< TestResult > > groupedByPoints;
    for (const TestResult& result : m_results) {
        groupedByPoints[ result.pointCount ].append(result);
    }

    // 3. 生成Markdown内容
    QString markdown;
    markdown += QString("### 测试：%1\n\n").arg(configDesc);

    // 按点数从小到大排序处理
    QList< int > sortedPoints = groupedByPoints.keys();
    std::sort(sortedPoints.begin(), sortedPoints.end());

    for (int pointCount : sortedPoints) {
        const QVector< TestResult >& results = groupedByPoints[ pointCount ];

        // 格式化点数描述（100K/500K/1M/5M）
        QString pointDesc;
        if (pointCount >= 1000000) {
            int million = pointCount / 1000000;
            int wan     = pointCount / 10000;
            pointDesc   = QString("%1M（%2万）").arg(million).arg(wan);
        } else {
            int k     = pointCount / 1000;
            int wan   = pointCount / 10000;
            pointDesc = QString("%1K（%2万）").arg(k).arg(wan);
        }

        markdown += QString("%1 数据点测试结果\n\n").arg(pointDesc);

        // 表格头
        markdown += "|Library|Render Time (ms)|FPS|Memory (MB)|OpenGL|Downsampling|\n";
        markdown += "|:------:|:------:|:------:|:------:|:------:|:------:|\n";

        // 按库名排序（QImPlot -> Qwt -> QCustomPlot -> Qt Charts）
        QVector< TestResult > sortedResults = results;
        std::sort(sortedResults.begin(), sortedResults.end(), [](const TestResult& a, const TestResult& b) {
            static const QStringList order = { "QIm", "Qwt", "QCustomPlot", "Qt Charts" };
            int idxA                       = order.indexOf(a.libraryName);
            int idxB                       = order.indexOf(b.libraryName);
            if (idxA == -1)
                idxA = 999;
            if (idxB == -1)
                idxB = 999;
            return idxA < idxB;
        });

        // 表格行
        for (const TestResult& r : sortedResults) {
            // 库名缩写映射
            QString libShort = r.libraryName;
            // OpenGL/Downsampling 状态
            QString openglStr     = m_config.useOpenGL ? "✓ Yes" : "× No";
            QString downsampleStr = m_config.useDownsampling ? "✓ Yes" : "× No";

            // 格式化数值（与示例一致的精度）
            QString renderTime = QString::number(r.renderTime, 'f', 2);
            QString fps        = QString::number(r.fps, 'f', 4);
            // 移除FPS末尾多余的0（61.2370 -> 61.237）
            while (fps.contains('.') && fps.endsWith('0')) {
                fps.chop(1);
            }
            if (fps.endsWith('.'))
                fps.chop(1);

            QString memory = QString::number(r.memoryUsage, 'f', 2);
            // 对于小数值保留更多精度（<10时显示5位）
            if (r.memoryUsage < 10.0) {
                memory = QString::number(r.memoryUsage, 'f', 5);
                // 移除末尾0
                while (memory.contains('.') && memory.endsWith('0')) {
                    memory.chop(1);
                }
                if (memory.endsWith('.'))
                    memory.chop(1);
            }

            markdown +=
                QString("|%1|%2|%3|%4|%5|%6|\n").arg(libShort).arg(renderTime).arg(fps).arg(memory).arg(openglStr).arg(downsampleStr);
        }
        markdown += "\n";
    }

    // 4. 复制到剪贴板
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(markdown);

    QMessageBox::information(this, "Copied", QString("✅ 已复制 Markdown 报告到剪贴板"));
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
