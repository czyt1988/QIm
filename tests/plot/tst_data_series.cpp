#include <QObject>
#include <QtTest>
#include <vector>
#include <limits>
#include <cmath>

#include "QImPlotDataSeries.h"

using namespace QIM;

class TestDataSeries : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    // QImAbstractXYDataSeries via QImVectorXYDataSeries
    void testDefaultType();
    void testSize();
    void testIsContiguous();
    void testStride();
    void testXyRawDataAccess();
    void testXyRawDataReturnsXPointer();
    void testXValueYValue();
    void testXValueYValueOutOfBounds();
    void testEmpty();
    // Y-only mode
    void testYOnlyModeRawData();
    void testYOnlyModeXValue();
    void testYOnlyModeStartScale();
    // yValueAtX (binary search)
    void testYValueAtXEmpty();
    void testYValueAtXExactMatch();
    void testYValueAtXBoundary();
    void testYValueAtXInterpolation();
    void testYValueAtXYOnlyMode();

private:
    static std::vector<double> makeXs(int n)
    {
        std::vector<double> xs(n);
        for (int i = 0; i < n; ++i)
            xs[i] = static_cast<double>(i);
        return xs;
    }

    static std::vector<double> makeYs(int n)
    {
        std::vector<double> ys(n);
        for (int i = 0; i < n; ++i)
            ys[i] = static_cast<double>(i * 2);  // y = 2*x
        return ys;
    }
};

void TestDataSeries::initTestCase()
{
}

void TestDataSeries::cleanupTestCase()
{
}

// ============================================================================
// Test 1: type() returns XYData
// ============================================================================
void TestDataSeries::testDefaultType()
{
    QImVectorXYDataSeries series(std::vector<double>{}, std::vector<double>{});
    QCOMPARE(series.type(), static_cast<int>(QImAbstractPlotDataSeries::XYData));
}

// ============================================================================
// Test 2: size returns min of vector sizes
// ============================================================================
void TestDataSeries::testSize()
{
    std::vector<double> xs = { 1.0, 2.0, 3.0, 4.0, 5.0 };
    std::vector<double> ys = { 10.0, 20.0, 30.0 };
    QImVectorXYDataSeries series(std::move(xs), std::move(ys));
    QCOMPARE(series.size(), 3);  // min(5, 3) = 3
}

// ============================================================================
// Test 3: isContiguous returns true for vector-based series
// ============================================================================
void TestDataSeries::testIsContiguous()
{
    QImVectorXYDataSeries series(std::vector<double>{1.0}, std::vector<double>{2.0});
    QVERIFY(series.isContiguous());
}

// ============================================================================
// Test 4: stride returns sizeof(double)
// ============================================================================
void TestDataSeries::testStride()
{
    QImVectorXYDataSeries series(std::vector<double>{1.0}, std::vector<double>{2.0});
    QCOMPARE(series.stride(), static_cast<int>(sizeof(double)));
}

// ============================================================================
// Test 5: xRawData returns non-null for XY mode, yRawData returns non-null
// ============================================================================
void TestDataSeries::testXyRawDataAccess()
{
    auto xs = makeXs(100);
    auto ys = makeYs(100);
    QImVectorXYDataSeries series(std::move(xs), std::move(ys));

    QVERIFY(series.xRawData() != nullptr);
    QVERIFY(series.yRawData() != nullptr);
}

// ============================================================================
// Test 6: xRawData returns pointer to X data, yRawData to Y data
// ============================================================================
void TestDataSeries::testXyRawDataReturnsXPointer()
{
    auto xs   = makeXs(10);
    auto ys   = makeYs(10);
    QImVectorXYDataSeries series(std::move(xs), std::move(ys));

    const double* xPtr = series.xRawData();
    QVERIFY(xPtr != nullptr);
    QCOMPARE(xPtr[0], 0.0);
    QCOMPARE(xPtr[9], 9.0);

    const double* yPtr = series.yRawData();
    QVERIFY(yPtr != nullptr);
    QCOMPARE(yPtr[0], series.yValue(0));
    QCOMPARE(yPtr[9], series.yValue(9));
}

// ============================================================================
// Test 7: xValue/yValue access by index
// ============================================================================
void TestDataSeries::testXValueYValue()
{
    auto xs   = makeXs(5);
    auto ys   = makeYs(5);
    QImVectorXYDataSeries series(std::move(xs), std::move(ys));

    QCOMPARE(series.xValue(0), 0.0);
    QCOMPARE(series.xValue(4), 4.0);
    QCOMPARE(series.yValue(0), 0.0);  // y = 2*x → y[0]=0
    QCOMPARE(series.yValue(4), 8.0);  // y[4] = 2*4 = 8
}

// ============================================================================
// Test 8: xValue/yValue out of bounds returns NaN
// ============================================================================
void TestDataSeries::testXValueYValueOutOfBounds()
{
    auto xs   = makeXs(5);
    auto ys   = makeYs(5);
    QImVectorXYDataSeries series(std::move(xs), std::move(ys));

    double nanX = series.xValue(-1);
    QVERIFY(std::isnan(nanX));

    double nanY = series.yValue(100);
    QVERIFY(std::isnan(nanY));
}

// ============================================================================
// Test 9: empty() returns true when no data
// ============================================================================
void TestDataSeries::testEmpty()
{
    QImVectorXYDataSeries empty(std::vector<double>{}, std::vector<double>{});
    QVERIFY(empty.empty());

    QImVectorXYDataSeries hasData(std::vector<double>{1.0}, std::vector<double>{2.0});
    QVERIFY(!hasData.empty());
}

// ============================================================================
// Test 10: Y-only mode — xRawData returns nullptr
// ============================================================================
void TestDataSeries::testYOnlyModeRawData()
{
    auto xs   = std::vector<double>(10);  // placeholder
    auto ys   = makeYs(10);
    QImVectorXYDataSeries series(std::move(xs), std::move(ys));
    series.setYOnly(true, 0.0, 1.0);

    QVERIFY(series.xRawData() == nullptr);
    QVERIFY(series.yRawData() != nullptr);
}

// ============================================================================
// Test 11: Y-only mode — xValue computed from xStart + index * xScale
// ============================================================================
void TestDataSeries::testYOnlyModeXValue()
{
    auto xs   = std::vector<double>(10);
    auto ys   = std::vector<double>(10);
    for (int i = 0; i < 10; ++i) ys[i] = static_cast<double>(i);
    QImVectorXYDataSeries series(std::move(xs), std::move(ys));
    series.setYOnly(true, 10.0, 2.0);  // xStart=10, xScale=2

    QCOMPARE(series.xValue(0), 10.0);   // 10 + 2*0
    QCOMPARE(series.xValue(1), 12.0);    // 10 + 2*1
    QCOMPARE(series.xValue(5), 20.0);    // 10 + 2*5
}

// ============================================================================
// Test 12: Y-only mode start and scale
// ============================================================================
void TestDataSeries::testYOnlyModeStartScale()
{
    auto xs   = std::vector<double>(10);
    auto ys   = std::vector<double>(10);
    QImVectorXYDataSeries series(std::move(xs), std::move(ys));

    QCOMPARE(series.xScale(), 1.0);   // Default
    QCOMPARE(series.xStart(), 0.0);   // Default

    series.setYOnly(true, 5.0, 0.5);
    QCOMPARE(series.xScale(), 0.5);
    QCOMPARE(series.xStart(), 5.0);
}

// ============================================================================
// Test 13: yValueAtX with empty data returns NaN
// ============================================================================
void TestDataSeries::testYValueAtXEmpty()
{
    QImVectorXYDataSeries empty(std::vector<double>{}, std::vector<double>{});
    double result = empty.yValueAtX(5.0);
    QVERIFY(std::isnan(result));
}

// ============================================================================
// Test 14: yValueAtX exact match via binary search
// ============================================================================
void TestDataSeries::testYValueAtXExactMatch()
{
    auto xs   = makeXs(100);
    auto ys   = makeYs(100);
    QImVectorXYDataSeries series(std::move(xs), std::move(ys));

    int index     = -1;
    bool exact    = false;
    double result = series.yValueAtX(50.0, &index, &exact);
    QCOMPARE(result, 100.0);  // y = 2*x → y[50]=100
    QCOMPARE(index, 50);
    QVERIFY(exact);
}

// ============================================================================
// Test 15: yValueAtX at boundaries
// ============================================================================
void TestDataSeries::testYValueAtXBoundary()
{
    auto xs = makeXs(100);
    auto ys = makeYs(100);
    QImVectorXYDataSeries series(std::move(xs), std::move(ys));

    // Below min X — returns first Y value
    int index = -1;
    double result = series.yValueAtX(-100.0, &index);
    QCOMPARE(index, 0);
    QCOMPARE(result, series.yValue(0));

    // Above max X — returns last Y value
    result = series.yValueAtX(99999.0, &index);
    QCOMPARE(index, 99);
    QCOMPARE(result, series.yValue(99));
}

// ============================================================================
// Test 16: yValueAtX interpolation (non-exact match)
// ============================================================================
void TestDataSeries::testYValueAtXInterpolation()
{
    auto xs = makeXs(100);
    auto ys = makeYs(100);
    QImVectorXYDataSeries series(std::move(xs), std::move(ys));

    bool exact = true;
    double result = series.yValueAtX(50.3, nullptr, &exact);
    // Not an exact match — returns closest neighbor
    QVERIFY(!exact);
    // Closest should be index 50 (y=100.0)
    QCOMPARE(result, 100.0);
}

// ============================================================================
// Test 17: yValueAtX in Y-only mode
// ============================================================================
void TestDataSeries::testYValueAtXYOnlyMode()
{
    auto xs   = std::vector<double>(10);
    auto ys   = std::vector<double>(10);
    for (int i = 0; i < 10; ++i) ys[i] = static_cast<double>(i * 2);
    QImVectorXYDataSeries series(std::move(xs), std::move(ys));
    series.setYOnly(true, 0.0, 1.0);

    int index = -1;
    double result = series.yValueAtX(5.0, &index);
    // In Y-only mode with xStart=0, xScale=1, x=5 → index=round(5)=5
    QCOMPARE(index, 5);
    QCOMPARE(result, 10.0);  // y[5] = 10
}

QTEST_MAIN(TestDataSeries)
#include "tst_data_series.moc"
