#include "PerformanceTestWidget.h"
#include "ui_PerformanceTestWidget.h"
#include <QStandardItem>
#include <QMessageBox>
#include <QDebug>
#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include "PerformanceTestReportDialog.h"
#include "qcustomplot.h"
PerformanceTestWidget::PerformanceTestWidget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::PerformanceTestWidget)
    , model(new QStandardItemModel(this))
    , controller(new PerformanceTestController(this))
    , reportDialog(new PerformanceTestReportDialog(this))
{
    ui->setupUi(this);

    // 设置表格模型
    setupModel();
    ui->tableView->setModel(model);

    // 连接信号槽
    connect(ui->pushButtonRunOne, &QPushButton::clicked, this, &PerformanceTestWidget::onStartButtonClicked);
    connect(ui->pushButtonRunAll, &QPushButton::clicked, this, &PerformanceTestWidget::onFullBenchmarkClicked);
    connect(ui->pushButtonShowReport, &QPushButton::clicked, this, &PerformanceTestWidget::onShowReportClicked);
    connect(controller, &PerformanceTestController::testStarted, this, &PerformanceTestWidget::onTestStarted);
    connect(controller, &PerformanceTestController::progressChanged, this, &PerformanceTestWidget::onProgressChanged);
    connect(controller, &PerformanceTestController::allTestsCompleted, this, &PerformanceTestWidget::onAllTestsCompleted);

    // 初始化UI状态
    ui->progressBar->setVisible(false);
    ui->labelInfo->setText("Ready to start performance test");

    // 设置表格自适应列宽
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setAlternatingRowColors(true);
}

PerformanceTestWidget::~PerformanceTestWidget()
{
    delete ui;
}

void PerformanceTestWidget::changeEvent(QEvent* e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void PerformanceTestWidget::setupModel()
{
    // 设置表头
    model->setHorizontalHeaderItem(0, new QStandardItem("Library"));
    model->setHorizontalHeaderItem(1, new QStandardItem("Point Count"));
    model->setHorizontalHeaderItem(2, new QStandardItem("Setup Time (ms)"));
    model->setHorizontalHeaderItem(3, new QStandardItem("Render Time (ms)"));
    model->setHorizontalHeaderItem(4, new QStandardItem("FPS"));
    model->setHorizontalHeaderItem(5, new QStandardItem("OpenGL"));
    model->setHorizontalHeaderItem(6, new QStandardItem("Use Downsampling"));
    model->setHorizontalHeaderItem(7, new QStandardItem("Memory (MB)"));

    // 设置列宽比例
    ui->tableView->setColumnWidth(0, 120);  // Library
    ui->tableView->setColumnWidth(1, 120);  // Point Count
    ui->tableView->setColumnWidth(2, 120);  // Setup Time
    ui->tableView->setColumnWidth(3, 120);  // Render Time
    ui->tableView->setColumnWidth(4, 100);  // FPS
    ui->tableView->setColumnWidth(5, 100);  // OpenGL
    ui->tableView->setColumnWidth(6, 100);  // Downsampling
    ui->tableView->setColumnWidth(7, 100);  // Memory
}

void PerformanceTestWidget::onStartButtonClicked()
{
    // 重置表格
    model->removeRows(0, model->rowCount());
    allTestResults.clear();

    // 解析配置
    PerformanceTestController::TestConfig config;
    config.pointCounts     = parsePointCounts(ui->lineEdit->text());
    config.testFrames      = ui->spinBoxTestFrames->value();
    config.warmupFrames    = 10;  // 固定预热帧数
    config.useDownsampling = ui->checkBoxDownsampling->isChecked();
    config.useOpenGL       = ui->checkBoxUseOpenGL->isChecked();
    // 验证输入
    if (config.pointCounts.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter valid point counts (comma separated numbers)");
        return;
    }

    // 显示进度条
    ui->progressBar->setVisible(true);
    ui->progressBar->setValue(0);
    ui->pushButtonRunOne->setEnabled(false);
    ui->pushButtonRunAll->setEnabled(false);
    ui->labelInfo->setText("Starting tests...");
    m_currentTestConfig = config;
    // 启动测试
    QTimer::singleShot(100, this, [ this, config ]() { controller->runTests(config); });
}

void PerformanceTestWidget::onFullBenchmarkClicked()
{
    QMessageBox::information(
        this,
        "Full Benchmark",
        "Full benchmark will run 4 configurations:\n"
        "1. Baseline (no optimizations)\n"
        "2. OpenGL only\n"
        "3. Downsampling only\n"
        "4. OpenGL + Downsampling\n\n"
        "This may take several minutes. Results will be shown in a comprehensive report."
    );
}

void PerformanceTestWidget::onShowReportClicked()
{
    reportDialog->show();
}

QVector< int > PerformanceTestWidget::parsePointCounts(const QString& text) const
{
    QVector< int > result;
    QStringList parts = text.split(',', Qt::SkipEmptyParts);

    for (const QString& part : std::as_const(parts)) {
        bool ok   = false;
        int value = part.trimmed().toInt(&ok);
        if (ok && value > 0) {
            result.append(value);
        }
    }

    return result;
}

void PerformanceTestWidget::onTestStarted(const QString& libraryName, int pointCount)
{
    ui->labelInfo->setText(QString("Testing %1 with %2 points...").arg(libraryName).arg(pointCount));
}

void PerformanceTestWidget::onProgressChanged(int current, int total)
{
    ui->progressBar->setMaximum(total);
    ui->progressBar->setValue(current);
}

void PerformanceTestWidget::onAllTestsCompleted(const QVector< TestResult >& allResults)
{
    // 保存结果
    allTestResults = allResults;

    // 填充表格
    populateTable(allResults);

    // 更新UI状态
    ui->pushButtonRunOne->setEnabled(true);
    ui->pushButtonRunAll->setEnabled(true);
    ui->labelInfo->setText(QString("All tests completed! Total: %1 results").arg(allResults.size()));


    // 显示报告对话框
    reportDialog->setTestResults(allResults, m_currentTestConfig, false);
    reportDialog->show();
}

void PerformanceTestWidget::populateTable(const QVector< TestResult >& results)
{
    model->removeRows(0, model->rowCount());
    auto useOpenGlStr = [ & ](const TestResult& result) -> QString {
        if (0 == result.libraryName.compare("QIm", Qt::CaseInsensitive)) {
            return tr("yes");
        }
        return this->m_currentTestConfig.useOpenGL ? tr("yes") : tr("no");
    };

    for (const TestResult& result : results) {
        QList< QStandardItem* > row;
        row.append(new QStandardItem(result.libraryName));
        row.append(new QStandardItem(QString::number(result.pointCount)));
        row.append(new QStandardItem(QString::number(result.setupTime, 'f', 2)));
        row.append(new QStandardItem(QString::number(result.renderTime, 'f', 2)));
        row.append(new QStandardItem(QString::number(result.fps, 'f', 1)));
        row.append(new QStandardItem(useOpenGlStr(result)));
        row.append(new QStandardItem(m_currentTestConfig.useDownsampling ? tr("yes") : tr("no")));
        row.append(new QStandardItem(QString::number(result.memoryUsage, 'f', 2)));

        // 根据FPS设置颜色（绿色表示高性能，红色表示低性能）
        double fps = result.fps;
        if (fps > 50) {
            for (auto* item : row)
                item->setBackground(QColor(200, 255, 200));
        } else if (fps < 10) {
            for (auto* item : row)
                item->setBackground(QColor(255, 200, 200));
        }

        model->appendRow(row);
    }
}
