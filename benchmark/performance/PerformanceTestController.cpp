// PerformanceTestController.cpp
#include "PerformanceTestController.h"
#include <QCoreApplication>
#include <QtConcurrent/QtConcurrent>
#include <QThread>
#include <cmath>
#include <random>

// QImPlot 相关
#include "QImFigureWidget.h"
#include "plot/QImPlotNode.h"
#include "plot/QImSubplotsNode.h"
#include "plot/QImPlotLineItemNode.h"
// 可选库的条件编译
#if defined(HAVE_QWT)
#include "QwtPlot.h"
#endif

#if defined(HAVE_QCUSTOMPLOT)
#include "qcustomplot.h"
#endif

#if defined(HAVE_QTCHARTS)
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChart>
#endif

// 跨平台内存测量实现
#if defined(Q_OS_WIN)
#include <windows.h>
#include <psapi.h>
#elif defined(Q_OS_LINUX) || defined(Q_OS_ANDROID)
#include <unistd.h>
#include <sys/resource.h>
#include <fstream>
#include <string>
#elif defined(Q_OS_MACOS) || defined(Q_OS_IOS)
#include <mach/mach.h>
#endif

class MemoryMonitor
{
public:
    MemoryMonitor()
    {
        // 预热一次，确保静态变量初始化
        static double dummy = currentMB();
        Q_UNUSED(dummy);

        restart();
        m_peakMemoryMB = m_startMemoryMB;
    }

    void restart()
    {
        m_startMemoryMB = currentMB();
        m_peakMemoryMB  = m_startMemoryMB;
    }

    /**
     * @brief 更新峰值内存（应在每次重绘后调用）
     */
    void updatePeak()
    {
        double current = currentMB();
        if (current > m_peakMemoryMB) {
            m_peakMemoryMB = current;
        }
    }

    /**
     * @brief 获取从构造/重启到现在的内存增量（MB）
     */
    double deltaMB() const
    {
        return currentMB() - m_startMemoryMB;
    }

    /**
     * @brief 获取测试期间的峰值内存增量（MB）
     */
    double peakDeltaMB() const
    {
        return m_peakMemoryMB - m_startMemoryMB;
    }

    /**
     * @brief 获取当前进程的绝对内存使用量（MB）
     */
    static double currentMB()
    {
#if defined(Q_OS_WIN)
        PROCESS_MEMORY_COUNTERS_EX pmc;
        pmc.cb = sizeof(PROCESS_MEMORY_COUNTERS_EX);
        if (GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast< PROCESS_MEMORY_COUNTERS* >(&pmc), sizeof(pmc))) {
            return pmc.PrivateUsage / (1024.0 * 1024.0);
        }
#elif defined(Q_OS_LINUX)
        // Linux: 使用 statm 更准确
        std::ifstream statm("/proc/self/statm");
        if (statm.is_open()) {
            unsigned long size, resident, share, text, lib, data, dt;
            statm >> size >> resident >> share >> text >> lib >> data >> dt;
            long page_size = sysconf(_SC_PAGESIZE);
            // 私有内存 = 常驻内存 - 共享内存
            return (resident - share) * page_size / (1024.0 * 1024.0);
        }
#elif defined(Q_OS_MACOS)
        struct task_basic_info info;
        mach_msg_type_number_t infoCount = TASK_BASIC_INFO_COUNT;
        if (task_info(mach_task_self(), TASK_BASIC_INFO, (task_info_t)&info, &infoCount) == KERN_SUCCESS) {
            return info.resident_size / (1024.0 * 1024.0);
        }
#endif
        return 0.0;
    }

    double recordedMemory() const
    {
        return m_startMemoryMB;
    }
    double peakMemory() const
    {
        return m_peakMemoryMB;
    }
    double globalDeltaMB() const
    {
        return currentMB() - s_globalBaseline;
    }

    double peakGlobalDeltaMB() const
    {
        return m_peakMemoryMB - s_globalBaseline;
    }
    static void setGlobalBaseline()
    {
        s_globalBaseline = currentMB();
    }

private:
    static double s_globalBaseline;
    double m_startMemoryMB;
    double m_peakMemoryMB;
};
// 静态变量初始化
double MemoryMonitor::s_globalBaseline = 0.0;

PerformanceTestController::PerformanceTestController(QObject* parent) : QObject(parent)
{
}


void PerformanceTestController::runTests(const TestConfig& config)
{
    m_config = config;

    // 计算需要的最大数据点数
    int maxPointCount = *std::max_element(config.pointCounts.begin(), config.pointCounts.end());
    int totalNeeded   = static_cast< int >(maxPointCount * 1.3 + 1);
    generateTestData(totalNeeded);

    QVector< TestResult > allResults;
    int totalTests = 1;

#if defined(HAVE_QWT)
    ++totalTests;
#endif

#if defined(HAVE_QCUSTOMPLOT)
    ++totalTests;
#endif

#if defined(HAVE_QTCHARTS)
    ++totalTests;
#endif

    int testCounter = 0;
    MemoryMonitor::setGlobalBaseline();
    for (int pointCount : config.pointCounts) {

        // 测试 Qwt
#if defined(HAVE_QWT)
        emit testStarted("Qwt", pointCount);
        allResults.append(testQwt(pointCount));
        emit progressChanged(allResults.size(), totalTests);
#endif
        cleanupMemory();
        // 测试 QCustomPlot
#if defined(HAVE_QCUSTOMPLOT)
        emit testStarted("QCustomPlot", pointCount);
        allResults.append(testQCustomPlot(pointCount));
        emit progressChanged(allResults.size(), totalTests);
#endif
        cleanupMemory();
        // 测试 Qt Charts
#if defined(HAVE_QTCHARTS)
        emit testStarted("Qt Charts", pointCount);
        allResults.append(testQtCharts(pointCount));
        emit progressChanged(allResults.size(), totalTests);
#endif
        // 测试 QImPlot
        emit testStarted("QImPlot", pointCount);  // QImPlot使用自己的渲染系统，不直接使用Qt OpenGL
        allResults.append(testQImPlot(pointCount));
        emit progressChanged(allResults.size(), totalTests);
        cleanupMemory();
    }

    emit allTestsCompleted(allResults);
}

TestResult PerformanceTestController::testQImPlot(int pointCount)
{
    TestResult result;
    result.libraryName      = "QIm";
    result.pointCount       = pointCount;
    result.usedDownsampling = m_config.useDownsampling;
    result.usedOpenGL       = true;
    // 记录基准内存（测试开始前）
    MemoryMonitor mem;
    qDebug().noquote() << "\nBegin Test QIm Baseline memory:" << mem.recordedMemory() << "MB";

    auto* figure = new QIM::QImFigureWidget();
    figure->setRenderMode(QIM::QImWidget::RenderOnDemand);
    figure->resize(800, 600);
    figure->show();
    QCoreApplication::processEvents();

    // 计算滑动步长
    int step = (m_totalDataSize - pointCount) / (m_config.testFrames - 1);
    if (step <= 0)
        step = 1;

    QElapsedTimer timer;

    // ---------- 首次设置数据 ----------
    timer.start();
    auto* plotNode = figure->createPlotNode();
    plotNode->setTitle("Performance Test");
    auto* lineNode = plotNode->addLine(
        QVector< double >(m_testDataX.constData(), m_testDataX.constData() + pointCount),
        QVector< double >(m_testDataY.constData(), m_testDataY.constData() + pointCount),
        "Test Curve"
    );
    lineNode->setAdaptivesSampling(result.usedDownsampling);
    result.setupTime = timer.elapsed();

    // ---------- 预热（包含数据更新）----------
    for (int i = 0; i < m_config.warmupFrames; ++i) {
        int startIdx = i * step;
        startIdx     = qMin(startIdx, m_totalDataSize - pointCount);
        lineNode->setData(
            QVector< double >(m_testDataX.constData() + startIdx, m_testDataX.constData() + startIdx + pointCount - 1),
            QVector< double >(m_testDataY.constData() + startIdx, m_testDataY.constData() + startIdx + pointCount - 1)
        );
        plotNode->rescaleAxes();
        figure->requestRender();
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
        mem.updatePeak();  // 更新峰值内存
    }

    // ---------- 正式测试 ----------
    timer.restart();
    for (int i = 0; i < m_config.testFrames; ++i) {
        int startIdx = i * step;
        startIdx     = qMin(startIdx, m_totalDataSize - pointCount);
        lineNode->setData(
            QVector< double >(m_testDataX.constData() + startIdx, m_testDataX.constData() + startIdx + pointCount - 1),
            QVector< double >(m_testDataY.constData() + startIdx, m_testDataY.constData() + startIdx + pointCount - 1)
        );
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
        plotNode->rescaleAxes();
        figure->requestRender();
        mem.updatePeak();  // 更新峰值内存
    }
    double totalTime = timer.elapsed();

    result.renderTime = totalTime / m_config.testFrames;
    result.fps        = 1000.0 / result.renderTime;

    // 内存测量：测试完成后（删除前）
    result.memoryUsage = mem.peakDeltaMB();
    delete figure;
    qDebug() << result.libraryName << ",pointCount=" << result.pointCount << ",renderTime=" << result.renderTime
             << ",fps=" << result.fps << ",use mem=" << result.memoryUsage << " MB";
    return result;
}

#if defined(HAVE_QWT)
TestResult PerformanceTestController::testQwt(int pointCount)
{
    TestResult result;
    result.libraryName      = "Qwt";
    result.pointCount       = pointCount;
    result.usedDownsampling = m_config.useDownsampling;
    result.usedOpenGL       = m_config.useOpenGL;
    MemoryMonitor mem;
    qDebug().noquote() << "\nBegin Test QWT Baseline memory:" << mem.recordedMemory() << "MB";
    auto* plot = new QwtPlot();
    // 启用OpenGL（如果配置要求且Qwt支持）
    if (m_config.useOpenGL) {
        QwtPlotOpenGLCanvas* plotCanvas = new QwtPlotOpenGLCanvas();
        plot->setCanvas(plotCanvas);
    }
    plot->resize(800, 600);
    plot->show();
    auto* curve = new QwtPlotCurve("Test Curve");

    QCoreApplication::processEvents();

    int step = (m_totalDataSize - pointCount) / (m_config.testFrames - 1);
    if (step <= 0)
        step = 1;

    QElapsedTimer timer;

    // ---------- 首次设置数据 ----------
    timer.start();
    curve->setSamples(m_testDataX.constData(), m_testDataY.constData(), pointCount);
    if (m_config.useDownsampling) {
        curve->setPaintAttribute(QwtPlotCurve::FilterPointsAggressive);
    }
    curve->attach(plot);
    result.setupTime = timer.elapsed();

    // ---------- 预热 ----------
    for (int i = 0; i < m_config.warmupFrames; ++i) {
        int startIdx = i * step;
        startIdx     = qMin(startIdx, m_totalDataSize - pointCount);
        curve->setSamples(m_testDataX.constData() + startIdx, m_testDataY.constData() + startIdx, pointCount);
        plot->rescaleAxes();
        plot->replot();
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
        mem.updatePeak();  // 更新峰值内存
    }

    // ---------- 正式测试 ----------
    timer.restart();
    for (int i = 0; i < m_config.testFrames; ++i) {
        int startIdx = i * step;
        startIdx     = qMin(startIdx, m_totalDataSize - pointCount);
        curve->setSamples(m_testDataX.constData() + startIdx, m_testDataY.constData() + startIdx, pointCount);
        plot->rescaleAxes();
        plot->replot();
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
        mem.updatePeak();  // 更新峰值内存
    }
    double totalTime = timer.elapsed();

    result.renderTime  = totalTime / m_config.testFrames;
    result.fps         = 1000.0 / result.renderTime;
    result.memoryUsage = mem.peakDeltaMB();

    delete plot;
    qDebug() << result.libraryName << ",pointCount=" << result.pointCount << ",renderTime=" << result.renderTime
             << ",fps=" << result.fps << ",use mem=" << result.memoryUsage << " MB";
    return result;
}
#endif

#if defined(HAVE_QCUSTOMPLOT)
TestResult PerformanceTestController::testQCustomPlot(int pointCount)
{
    TestResult result;
    result.libraryName      = "QCustomPlot";
    result.pointCount       = pointCount;
    result.usedDownsampling = m_config.useDownsampling;
    result.usedOpenGL       = m_config.useOpenGL;
    MemoryMonitor mem;
    qDebug().noquote() << "\nBegin Test QCustomPlot Baseline memory:" << mem.recordedMemory() << "MB";

    auto* customPlot = new QCustomPlot();
    if (m_config.useOpenGL) {
        customPlot->setOpenGl(true);  // 4ms 刷新间隔
    }
    customPlot->resize(800, 600);
    // 必须 show() 才能测量真实内存
    customPlot->show();
    customPlot->addGraph();
    customPlot->graph(0)->setName("Test Curve");
    customPlot->graph(0)->setAdaptiveSampling(result.usedDownsampling);
    customPlot->setAntialiasedElements(QCP::aeNone);  // 关闭抗锯齿提高性能
    QCoreApplication::processEvents();

    int step = (m_totalDataSize - pointCount) / (m_config.testFrames - 1);
    if (step <= 0)
        step = 1;

    QElapsedTimer timer;

    // ---------- 首次设置数据 ----------
    timer.start();
    customPlot->graph(0)->setData(
        QVector< double >(m_testDataX.constData(), m_testDataX.constData() + pointCount),
        QVector< double >(m_testDataY.constData(), m_testDataY.constData() + pointCount)
    );
    result.setupTime = timer.elapsed();

    // ---------- 预热 ----------
    for (int i = 0; i < m_config.warmupFrames; ++i) {
        int startIdx = i * step;
        startIdx     = qMin(startIdx, m_totalDataSize - pointCount);
        customPlot->graph(0)->setData(
            QVector< double >(m_testDataX.constData() + startIdx, m_testDataX.constData() + startIdx + pointCount),
            QVector< double >(m_testDataY.constData() + startIdx, m_testDataY.constData() + startIdx + pointCount)
        );
        customPlot->rescaleAxes();
        customPlot->replot();
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
        mem.updatePeak();  // 更新峰值内存
    }

    // ---------- 正式测试 ----------
    timer.restart();
    for (int i = 0; i < m_config.testFrames; ++i) {
        int startIdx = i * step;
        startIdx     = qMin(startIdx, m_totalDataSize - pointCount);
        customPlot->graph(0)->setData(
            QVector< double >(m_testDataX.constData() + startIdx, m_testDataX.constData() + startIdx + pointCount),
            QVector< double >(m_testDataY.constData() + startIdx, m_testDataY.constData() + startIdx + pointCount)
        );
        customPlot->rescaleAxes();
        customPlot->replot();
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
        mem.updatePeak();
    }
    double totalTime = timer.elapsed();

    result.renderTime  = totalTime / m_config.testFrames;
    result.fps         = 1000.0 / result.renderTime;
    result.memoryUsage = mem.peakDeltaMB();

    delete customPlot;
    qDebug() << result.libraryName << ",pointCount=" << result.pointCount << ",renderTime=" << result.renderTime
             << ",fps=" << result.fps << ",use mem=" << result.memoryUsage << " MB";
    return result;
}
#endif

#if defined(HAVE_QTCHARTS)
TestResult PerformanceTestController::testQtCharts(int pointCount)
{
    TestResult result;
    result.libraryName      = "Qt Charts";
    result.pointCount       = pointCount;
    result.usedDownsampling = m_config.useDownsampling;
    result.usedOpenGL       = m_config.useOpenGL;

    MemoryMonitor mem;
    qDebug().noquote() << "\nBegin Test QtChart Baseline memory:" << mem.recordedMemory() << "MB";

    // ---------- 1. 批量构建数据点（高效）----------
    QVector< QPointF > points;
    points.reserve(pointCount);
    for (int i = 0; i < pointCount; ++i) {
        points.append(QPointF(m_testDataX[ i ], m_testDataY[ i ]));
    }

    // ---------- 2. 创建图表组件（仅一次）----------
    auto* series = new QLineSeries();
    series->setName("Test Curve");

    auto* chart = new QChart();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->legend()->hide();

    auto* chartView = new QChartView(chart);
    chartView->resize(800, 600);
    chartView->show();                  // 必须显示，否则 grab() 可能返回空像素图
    QCoreApplication::processEvents();  // 确保窗口系统完成初始化

    QElapsedTimer timer;

    // ---------- 3. 数据设置时间（批量替换）----------
    timer.start();
    series->replace(points);  // 一次性替换所有点
    result.setupTime = timer.elapsed();

    // ---------- 4. 渲染性能测试（同步渲染）----------
    // 预热帧：调用 grab() 强制同步渲染到像素图
    for (int i = 0; i < m_config.warmupFrames; ++i) {
        chartView->grab();  // 同步渲染，返回 QPixmap（丢弃）
        mem.updatePeak();   // 更新峰值内存
    }

    timer.restart();
    for (int i = 0; i < m_config.testFrames; ++i) {
        chartView->grab();  // 测量真实渲染时间
        mem.updatePeak();   // 更新峰值内存
    }
    double totalTime = timer.elapsed();

    result.renderTime  = totalTime / m_config.testFrames;
    result.fps         = 1000.0 / result.renderTime;
    result.memoryUsage = mem.peakDeltaMB();

    delete chartView;  // 自动删除 chart 和 series
    qDebug() << result.libraryName << ",pointCount=" << result.pointCount << ",renderTime=" << result.renderTime
             << ",fps=" << result.fps << ",use mem=" << result.memoryUsage << " MB";
    return result;
}
#endif

void PerformanceTestController::generateTestData(int totalPoints)
{
    m_totalDataSize = totalPoints;
    m_testDataX.resize(totalPoints);
    m_testDataY.resize(totalPoints);

    std::mt19937 rng(12345);
    std::uniform_real_distribution< double > noise(-0.1, 0.1);

    for (int i = 0; i < totalPoints; ++i) {
        double x         = i * 10.0 / totalPoints;
        m_testDataX[ i ] = x;
        m_testDataY[ i ] = std::sin(x) + 0.2 * std::sin(10 * x) + 0.05 * std::sin(50 * x) + noise(rng);
    }
}

// 跨平台内存清理：触发GC/内存页回收
void PerformanceTestController::cleanupMemory()
{
    // 1. 强制Qt处理所有待销毁的QObject（延迟删除的对象）
    QCoreApplication::sendPostedEvents(nullptr, QEvent::DeferredDelete);
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

    // 2. 跨平台强制释放空闲内存
#if defined(Q_OS_WIN)
    // Windows：强制工作集修剪
    SetProcessWorkingSetSize(GetCurrentProcess(), (SIZE_T)-1, (SIZE_T)-1);
#elif defined(Q_OS_LINUX)
    // Linux：触发内存页回写并释放
    std::ofstream ofs("/proc/sys/vm/drop_caches");
    if (ofs.is_open()) {
        ofs << "3" << std::endl;  // 释放页缓存、目录项、inodes
        ofs.close();
    }
    // 触发进程堆整理
    mallopt(M_TRIM_THRESHOLD, 0);
#elif defined(Q_OS_MACOS)
    // macOS：触发内存压缩/回收
    vm_deallocate(mach_task_self(), vm_address_t(0), vm_size_t(0));
#endif

    // 3. 短暂等待系统回收（给系统100ms时间处理）
    QThread::msleep(100);
}
