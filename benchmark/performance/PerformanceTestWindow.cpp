// PerformanceTestWindow.cpp
#include "PerformanceTestWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QMessageBox>
#include <QThread>
#include <QDebug>
// QIm
#include "QImFigureWidget.h"
#include "plot/QImPlotNode.h"
#include "plot/QImPlotLineItemNode.h"
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QCategoryAxis>

// 生成测试数据
static void generateTestData(int pointCount, QVector< double >& x, QVector< double >& y)
{
    x.resize(pointCount);
    y.resize(pointCount);

    for (int i = 0; i < pointCount; ++i) {
        x[ i ] = i * 10.0 / pointCount;  // 0到10的范围
        y[ i ] = sin(x[ i ]) + 0.1 * sin(10 * x[ i ]) + 0.01 * sin(100 * x[ i ]);
    }
}

PerformanceTestWindow::PerformanceTestWindow(QWidget* parent) : QMainWindow(parent)
{
    setupUI();
    setupCharts();

    m_testController = new PerformanceTestController(this);

    connect(m_startButton, &QPushButton::clicked, this, &PerformanceTestWindow::onStartTest);
    connect(m_testController, &PerformanceTestController::testStarted, this, &PerformanceTestWindow::onTestStarted);
    connect(m_testController, &PerformanceTestController::testResultReady, this, &PerformanceTestWindow::onTestResultReady);
    connect(m_testController, &PerformanceTestController::allTestsCompleted, this, &PerformanceTestWindow::onAllTestsCompleted);
}

PerformanceTestWindow::~PerformanceTestWindow()
{
}

void PerformanceTestWindow::setupUI()
{
    QWidget* centralWidget  = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    // 控制面板
    QGroupBox* controlGroup    = new QGroupBox("测试控制", this);
    QHBoxLayout* controlLayout = new QHBoxLayout(controlGroup);

    m_librarySelector = new QComboBox(this);
    m_librarySelector->addItem("所有库");
    m_librarySelector->addItem("QImPlot");
    m_librarySelector->addItem("Qwt");
    m_librarySelector->addItem("QCustomPlot");
    m_librarySelector->addItem("Qt Charts");

    m_pointCountSelector = new QSpinBox(this);
    m_pointCountSelector->setRange(1000, 10000000);
    m_pointCountSelector->setValue(100000);
    m_pointCountSelector->setSuffix(" 点");
    m_pointCountSelector->setSingleStep(10000);

    m_enableDownsampling = new QCheckBox("启用降采样", this);
    m_enableDownsampling->setChecked(true);

    m_startButton = new QPushButton("开始测试", this);

    controlLayout->addWidget(new QLabel("测试库:"));
    controlLayout->addWidget(m_librarySelector);
    controlLayout->addWidget(new QLabel("数据点数:"));
    controlLayout->addWidget(m_pointCountSelector);
    controlLayout->addWidget(m_enableDownsampling);
    controlLayout->addWidget(m_startButton);
    controlLayout->addStretch();

    // 进度条
    m_progressBar = new QProgressBar(this);
    m_progressBar->setRange(0, 100);
    m_progressBar->setVisible(false);

    m_statusLabel = new QLabel("准备就绪", this);

    // 创建标签页
    m_tabWidget = new QTabWidget(this);

    // 结果表格标签页
    QWidget* tableTab        = new QWidget();
    QVBoxLayout* tableLayout = new QVBoxLayout(tableTab);
    m_resultTable            = new QTableWidget(0, 7, this);
    m_resultTable->setHorizontalHeaderLabels(
        { "库名称", "数据点数", "初始化时间(ms)", "渲染时间(ms)", "FPS", "内存使用(MB)", "备注" }
    );
    m_resultTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableLayout->addWidget(m_resultTable);
    m_tabWidget->addTab(tableTab, "详细结果");

    // 图表标签页
    QWidget* chartTab        = new QWidget();
    QGridLayout* chartLayout = new QGridLayout(chartTab);

    m_timeChartView   = new QChartView();
    m_fpsChartView    = new QChartView();
    m_memoryChartView = new QChartView();

    chartLayout->addWidget(new QLabel("渲染时间对比"), 0, 0);
    chartLayout->addWidget(m_timeChartView, 1, 0);
    chartLayout->addWidget(new QLabel("帧率对比"), 0, 1);
    chartLayout->addWidget(m_fpsChartView, 1, 1);
    chartLayout->addWidget(new QLabel("内存使用对比"), 2, 0, 1, 2);
    chartLayout->addWidget(m_memoryChartView, 3, 0, 1, 2);

    m_tabWidget->addTab(chartTab, "性能图表");

    // 主布局
    mainLayout->addWidget(controlGroup);
    mainLayout->addWidget(m_progressBar);
    mainLayout->addWidget(m_statusLabel);
    mainLayout->addWidget(m_tabWidget);

    setCentralWidget(centralWidget);
}

void PerformanceTestWindow::setupCharts()
{
    // ---------- 渲染时间图表 ----------
    QChart* timeChart = new QChart();
    timeChart->setTitle("各库渲染时间对比（越低越好）");
    timeChart->setAnimationOptions(QChart::NoAnimation);
    timeChart->legend()->setVisible(true);
    timeChart->legend()->setAlignment(Qt::AlignBottom);

    QValueAxis* axisX = new QValueAxis();
    axisX->setTitleText("数据点数");
    axisX->setLabelFormat("%d");
    axisX->setTickCount(6);
    axisX->setMinorTickCount(4);

    QValueAxis* axisY = new QValueAxis();
    axisY->setTitleText("渲染时间 (ms)");
    axisY->setLabelFormat("%.2f");

    timeChart->addAxis(axisX, Qt::AlignBottom);
    timeChart->addAxis(axisY, Qt::AlignLeft);
    m_timeChartView->setChart(timeChart);
    m_timeChartView->setRenderHint(QPainter::Antialiasing);

    // ---------- 帧率图表 ----------
    QChart* fpsChart = new QChart();
    fpsChart->setTitle("各库帧率对比（越高越好）");
    fpsChart->setAnimationOptions(QChart::NoAnimation);
    fpsChart->legend()->setVisible(true);
    fpsChart->legend()->setAlignment(Qt::AlignBottom);

    QValueAxis* fpsAxisX = new QValueAxis();
    fpsAxisX->setTitleText("数据点数");
    fpsAxisX->setLabelFormat("%d");
    fpsAxisX->setTickCount(6);
    fpsAxisX->setMinorTickCount(4);

    QValueAxis* fpsAxisY = new QValueAxis();
    fpsAxisY->setTitleText("FPS");
    fpsAxisY->setLabelFormat("%.1f");

    fpsChart->addAxis(fpsAxisX, Qt::AlignBottom);
    fpsChart->addAxis(fpsAxisY, Qt::AlignLeft);
    m_fpsChartView->setChart(fpsChart);
    m_fpsChartView->setRenderHint(QPainter::Antialiasing);

    // ---------- 内存使用图表 ----------
    QChart* memChart = new QChart();
    memChart->setTitle("各库内存占用对比（越低越好）");
    memChart->setAnimationOptions(QChart::NoAnimation);
    memChart->legend()->setVisible(true);
    memChart->legend()->setAlignment(Qt::AlignBottom);

    QValueAxis* memAxisX = new QValueAxis();
    memAxisX->setTitleText("数据点数");
    memAxisX->setLabelFormat("%d");
    memAxisX->setTickCount(6);
    memAxisX->setMinorTickCount(4);

    QValueAxis* memAxisY = new QValueAxis();
    memAxisY->setTitleText("内存 (MB)");
    memAxisY->setLabelFormat("%.1f");

    memChart->addAxis(memAxisX, Qt::AlignBottom);
    memChart->addAxis(memAxisY, Qt::AlignLeft);
    m_memoryChartView->setChart(memChart);
    m_memoryChartView->setRenderHint(QPainter::Antialiasing);
}

void PerformanceTestWindow::onStartTest()
{
    m_startButton->setEnabled(false);
    m_progressBar->setVisible(true);
    m_progressBar->setValue(0);
    m_resultTable->setRowCount(0);

    PerformanceTestController::TestConfig config;
    config.pointCounts     = { 10000, 50000, 100000, 500000, 1000000 };
    config.useDownsampling = m_enableDownsampling->isChecked();


    m_testController->runTests(config);
}

void PerformanceTestWindow::onTestStarted(const QString& libraryName, int pointCount)
{
    m_statusLabel->setText(QString("正在测试: %1, 数据点数: %2").arg(libraryName).arg(pointCount));
}

void PerformanceTestWindow::onTestResultReady(const TestResult& result)
{
    updateResultTable(result);
}

void PerformanceTestWindow::updateResultTable(const TestResult& result)
{
    int row = m_resultTable->rowCount();
    m_resultTable->insertRow(row);

    m_resultTable->setItem(row, 0, new QTableWidgetItem(result.libraryName));
    m_resultTable->setItem(row, 1, new QTableWidgetItem(QString::number(result.pointCount)));
    m_resultTable->setItem(row, 2, new QTableWidgetItem(QString::number(result.setupTime, 'f', 2)));
    m_resultTable->setItem(row, 3, new QTableWidgetItem(QString::number(result.renderTime, 'f', 2)));
    m_resultTable->setItem(row, 4, new QTableWidgetItem(QString::number(result.fps, 'f', 1)));
    m_resultTable->setItem(row, 5, new QTableWidgetItem(QString::number(result.memoryUsage, 'f', 1)));

    // 自动滚动到最后一行
    m_resultTable->scrollToBottom();
}


// 类似实现其他库的测试...

void PerformanceTestWindow::onAllTestsCompleted(const QVector< TestResult >& allResults)
{
    updateSummaryCharts(allResults);
    m_statusLabel->setText("测试完成");
    m_startButton->setEnabled(true);
    // ---------- 使用 qDebug 打印结果 ----------
    qDebug() << "\n========== 性能测试结果汇总 ==========";
    qDebug().noquote() << QString("| %1 | %2 | %3 | %4 | %5 | %6 |")
                              .arg("库名称", -12)
                              .arg("数据点数", -12)
                              .arg("初始化(ms)", -12)
                              .arg("渲染时间(ms)", -14)
                              .arg("FPS", -8)
                              .arg("内存(MB)", -10);
    qDebug() << "|--------------|--------------|--------------|----------------|--------|----------|";

    for (const auto& result : allResults) {
        QString line = QString("| %1 | %2 | %3 | %4 | %5 | %6 |")
                           .arg(result.libraryName, -12)
                           .arg(QLocale().toString(result.pointCount), -12)
                           .arg(QString::number(result.setupTime, 'f', 2), -12)
                           .arg(QString::number(result.renderTime, 'f', 3), -14)
                           .arg(QString::number(result.fps, 'f', 1), -8)
                           .arg(QString::number(result.memoryUsage, 'f', 1), -10);
        qDebug().noquote() << line;
    }
    qDebug() << "========================================";
}

void PerformanceTestWindow::updateSummaryCharts(const QVector< TestResult >& allResults)
{
    // 按库分组
    QMap< QString, QMap< int, TestResult > > resultsByLibrary;
    for (const auto& result : allResults) {
        resultsByLibrary[ result.libraryName ][ result.pointCount ] = result;
    }

    // 创建渲染时间图表
    QChart* timeChart = new QChart();
    timeChart->setTitle("渲染时间对比");
    timeChart->setAnimationOptions(QChart::NoAnimation);

    QValueAxis* xAxisTime = new QValueAxis();
    xAxisTime->setTitleText("数据点数");
    xAxisTime->setLabelFormat("%d");
    xAxisTime->setTickCount(10);

    QValueAxis* yAxisTime = new QValueAxis();
    yAxisTime->setTitleText("渲染时间(ms)");
    yAxisTime->setLabelFormat("%.1f");

    timeChart->addAxis(xAxisTime, Qt::AlignBottom);
    timeChart->addAxis(yAxisTime, Qt::AlignLeft);

    // 创建FPS图表
    QChart* fpsChart = new QChart();
    fpsChart->setTitle("帧率对比");
    fpsChart->setAnimationOptions(QChart::NoAnimation);

    // 为每个库创建曲线
    QMap< QString, QLineSeries* > timeSeries;
    QMap< QString, QLineSeries* > fpsSeries;
    QMap< QString, QLineSeries* > memorySeries;

    QList< QColor > colors = { Qt::red, Qt::blue, Qt::green, Qt::magenta, Qt::cyan };
    int colorIndex         = 0;

    for (auto it = resultsByLibrary.begin(); it != resultsByLibrary.end(); ++it) {
        QString library = it.key();
        auto& results   = it.value();

        // 渲染时间曲线
        auto* timeSerie = new QLineSeries();
        timeSerie->setName(library);
        timeSerie->setColor(colors[ colorIndex % colors.size() ]);

        // FPS曲线
        auto* fpsSerie = new QLineSeries();
        fpsSerie->setName(library);
        fpsSerie->setColor(colors[ colorIndex % colors.size() ]);

        for (auto pointIt = results.begin(); pointIt != results.end(); ++pointIt) {
            int pointCount           = pointIt.key();
            const TestResult& result = pointIt.value();

            timeSerie->append(pointCount, result.renderTime);
            fpsSerie->append(pointCount, result.fps);
        }

        timeChart->addSeries(timeSerie);
        timeSerie->attachAxis(xAxisTime);
        timeSerie->attachAxis(yAxisTime);

        fpsChart->addSeries(fpsSerie);

        colorIndex++;
    }

    m_timeChartView->setChart(timeChart);
    m_fpsChartView->setChart(fpsChart);
}
