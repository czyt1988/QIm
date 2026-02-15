// PerformanceTestController.h
#ifndef PERFORMANCETESTCONTROLLER_H
#define PERFORMANCETESTCONTROLLER_H

#include <QObject>
#include <QVector>
#include <QElapsedTimer>
#include <QString>

struct TestResult
{
    QString libraryName;
    int pointCount        = 0;
    double setupTime      = 0.0;  // ms
    double renderTime     = 0.0;  // ms (每帧平均)
    double fps            = 0.0;
    double memoryUsage    = 0.0;    // MB
    bool usedOpenGL       = false;  // 是否使用OpenGL
    bool usedDownsampling = false;  // 是否使用降采样
};
Q_DECLARE_METATYPE(TestResult)

class PerformanceTestController : public QObject
{
    Q_OBJECT
public:
    struct TestConfig
    {
        QVector< int > pointCounts = { 100000, 500000, 1000000 };
        int warmupFrames = 10;  ///< 消除测试环境中的“冷启动”效应，获得更准确、更可重复的性能测量结果。
        int testFrames       = 100;
        bool useDownsampling = false;
        bool useOpenGL       = false;  // 是否启用了OpenGL
    };

    explicit PerformanceTestController(QObject* parent = nullptr);
    void cleanupMemory();
public slots:
    void runTests(const PerformanceTestController::TestConfig& config);

signals:
    void testStarted(const QString& libraryName, int pointCount);
    void progressChanged(int current, int total);
    void testResultReady(const TestResult& result);
    void allTestsCompleted(const QVector< TestResult >& allResults);

private:
    TestResult testQImPlot(int pointCount);
#if defined(HAVE_QWT)
    TestResult testQwt(int pointCount);
#endif

#if defined(HAVE_QCUSTOMPLOT)
    TestResult testQCustomPlot(int pointCount);
#endif

#if defined(HAVE_QTCHARTS)
    TestResult testQtCharts(int pointCount);
#endif

    void generateTestData(int totalPoints);

private:
    TestConfig m_config;
    QVector< double > m_testDataX;
    QVector< double > m_testDataY;
    int m_totalDataSize = 0;  // 实际生成的数据总点数（totalPoints * 1.3）
};
Q_DECLARE_METATYPE(PerformanceTestController::TestConfig)
#endif  // PERFORMANCETESTCONTROLLER_H
