#include <QObject>
#include <QtTest>
#include <vector>
#include <limits>
#include <cmath>

#include "QImPlot3DDataSeries.h"

using namespace QIM;

class TestDataSeries3D : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    // Construction
    void testDefaultType();
    void testConstructFromMove();
    void testConstructFromCopy();
    // Size
    void testSizeMinOfThree();
    void testSizeEqualContainers();
    // Raw data access
    void testRawDataPointers();
    void testRawDataValues();
    // Indexed access
    void testValueAccess();
    void testValueAccessOutOfBounds();
    // empty
    void testEmptyAllPopulated();
    void testEmptyAnyEmpty();
    // isValid
    void testIsValid();
    void testIsValidWithNull();
    // min/max
    void testMinMaxAccessors();
    // stride
    void testStride();
};

void TestDataSeries3D::initTestCase()
{
}

void TestDataSeries3D::cleanupTestCase()
{
}

// ============================================================================
// Test 1: type() returns XYZData
// ============================================================================
void TestDataSeries3D::testDefaultType()
{
    QImVectorXYZDataSeries series(
        std::vector<double>{}, std::vector<double>{}, std::vector<double>{});
    QCOMPARE(series.type(), static_cast<int>(QImAbstractXYZDataSeries::XYZData));
}

// ============================================================================
// Test 2: Construct from move (rvalue)
// ============================================================================
void TestDataSeries3D::testConstructFromMove()
{
    std::vector<double> xs = { 1.0, 2.0, 3.0 };
    std::vector<double> ys = { 4.0, 5.0, 6.0 };
    std::vector<double> zs = { 7.0, 8.0, 9.0 };

    QImVectorXYZDataSeries series(std::move(xs), std::move(ys), std::move(zs));
    QCOMPARE(series.size(), 3);
    QCOMPARE(series.xValue(0), 1.0);
    QCOMPARE(series.yValue(0), 4.0);
    QCOMPARE(series.zValue(0), 7.0);
}

// ============================================================================
// Test 3: Construct from copy (lvalue)
// ============================================================================
void TestDataSeries3D::testConstructFromCopy()
{
    std::vector<double> xs = { 10.0, 20.0 };
    std::vector<double> ys = { 30.0, 40.0 };
    std::vector<double> zs = { 50.0, 60.0 };

    QImVectorXYZDataSeries series(xs, ys, zs);
    QCOMPARE(series.size(), 2);
    QCOMPARE(series.xValue(1), 20.0);
    QCOMPARE(series.yValue(1), 40.0);
    QCOMPARE(series.zValue(1), 60.0);
}

// ============================================================================
// Test 4: size returns minimum of three container sizes
// ============================================================================
void TestDataSeries3D::testSizeMinOfThree()
{
    std::vector<double> xs = { 1.0, 2.0, 3.0, 4.0, 5.0 };
    std::vector<double> ys = { 1.0, 2.0, 3.0 };
    std::vector<double> zs = { 1.0, 2.0, 3.0, 4.0 };

    QImVectorXYZDataSeries series(std::move(xs), std::move(ys), std::move(zs));
    QCOMPARE(series.size(), 3);  // min(5, 3, 4) = 3
}

// ============================================================================
// Test 5: size with equal-sized containers
// ============================================================================
void TestDataSeries3D::testSizeEqualContainers()
{
    std::vector<double> xs(100, 1.0);
    std::vector<double> ys(100, 2.0);
    std::vector<double> zs(100, 3.0);

    QImVectorXYZDataSeries series(std::move(xs), std::move(ys), std::move(zs));
    QCOMPARE(series.size(), 100);
}

// ============================================================================
// Test 6: raw data pointers are valid
// ============================================================================
void TestDataSeries3D::testRawDataPointers()
{
    std::vector<double> xs = { 1.0, 2.0, 3.0 };
    std::vector<double> ys = { 4.0, 5.0, 6.0 };
    std::vector<double> zs = { 7.0, 8.0, 9.0 };

    QImVectorXYZDataSeries series(std::move(xs), std::move(ys), std::move(zs));

    QVERIFY(series.xRawData() != nullptr);
    QVERIFY(series.yRawData() != nullptr);
    QVERIFY(series.zRawData() != nullptr);
}

// ============================================================================
// Test 7: raw data pointer values match
// ============================================================================
void TestDataSeries3D::testRawDataValues()
{
    std::vector<double> xs = { 10.0, 20.0 };
    std::vector<double> ys = { 30.0, 40.0 };
    std::vector<double> zs = { 50.0, 60.0 };

    QImVectorXYZDataSeries series(std::move(xs), std::move(ys), std::move(zs));

    QCOMPARE(series.xRawData()[0], 10.0);
    QCOMPARE(series.xRawData()[1], 20.0);
    QCOMPARE(series.yRawData()[0], 30.0);
    QCOMPARE(series.zRawData()[0], 50.0);
}

// ============================================================================
// Test 8: indexed value access
// ============================================================================
void TestDataSeries3D::testValueAccess()
{
    std::vector<double> xs = { 1.0, 2.0, 3.0 };
    std::vector<double> ys = { 4.0, 5.0, 6.0 };
    std::vector<double> zs = { 7.0, 8.0, 9.0 };

    QImVectorXYZDataSeries series(std::move(xs), std::move(ys), std::move(zs));

    QCOMPARE(series.xValue(0), 1.0);
    QCOMPARE(series.yValue(0), 4.0);
    QCOMPARE(series.zValue(0), 7.0);

    QCOMPARE(series.xValue(2), 3.0);
    QCOMPARE(series.yValue(2), 6.0);
    QCOMPARE(series.zValue(2), 9.0);
}

// ============================================================================
// Test 9: out-of-bounds indexed access returns NaN
// ============================================================================
void TestDataSeries3D::testValueAccessOutOfBounds()
{
    std::vector<double> xs = { 1.0, 2.0 };
    std::vector<double> ys = { 3.0, 4.0 };
    std::vector<double> zs = { 5.0, 6.0 };

    QImVectorXYZDataSeries series(std::move(xs), std::move(ys), std::move(zs));

    QVERIFY(std::isnan(series.xValue(-1)));
    QVERIFY(std::isnan(series.yValue(100)));
    QVERIFY(std::isnan(series.zValue(2)));  // size=2, index 2 out of bounds
}

// ============================================================================
// Test 10: empty() returns false when all containers have data
// ============================================================================
void TestDataSeries3D::testEmptyAllPopulated()
{
    std::vector<double> xs = { 1.0 };
    std::vector<double> ys = { 2.0 };
    std::vector<double> zs = { 3.0 };

    QImVectorXYZDataSeries series(std::move(xs), std::move(ys), std::move(zs));
    QVERIFY(!series.empty());
}

// ============================================================================
// Test 11: empty() returns true when any container is empty
// ============================================================================
void TestDataSeries3D::testEmptyAnyEmpty()
{
    QImVectorXYZDataSeries emptySeries(
        std::vector<double>{}, std::vector<double>{1.0}, std::vector<double>{1.0});
    QVERIFY(emptySeries.empty());
}

// ============================================================================
// Test 12: isValid() with valid data
// ============================================================================
void TestDataSeries3D::testIsValid()
{
    std::vector<double> xs = { 1.0, 2.0 };
    std::vector<double> ys = { 3.0, 4.0 };
    std::vector<double> zs = { 5.0, 6.0 };

    QImVectorXYZDataSeries series(std::move(xs), std::move(ys), std::move(zs));
    QVERIFY(series.isValid());
}

// ============================================================================
// Test 13: isValid() fails when any data pointer is null
// ============================================================================
void TestDataSeries3D::testIsValidWithNull()
{
    QImVectorXYZDataSeries emptySeries(
        std::vector<double>{}, std::vector<double>{}, std::vector<double>{});
    // Empty vectors have data() that may or may not be null depending on implementation
    // size()=0 means isValid returns false because size() > 0 fails
    QVERIFY(!emptySeries.isValid());
}

// ============================================================================
// Test 14: min/max accessors
// ============================================================================
void TestDataSeries3D::testMinMaxAccessors()
{
    std::vector<double> xs = { 5.0, 3.0, 8.0 };
    std::vector<double> ys = { 1.0, 9.0, 4.0 };
    std::vector<double> zs = { 7.0, 2.0, 6.0 };

    QImVectorXYZDataSeries series(std::move(xs), std::move(ys), std::move(zs));

    // min/max return first and last values respectively
    QCOMPARE(series.xmin(), 5.0);
    QCOMPARE(series.xmax(), 8.0);
    QCOMPARE(series.ymin(), 1.0);
    QCOMPARE(series.ymax(), 4.0);
    QCOMPARE(series.zmin(), 7.0);
    QCOMPARE(series.zmax(), 6.0);
}

// ============================================================================
// Test 15: stride returns sizeof(double)
// ============================================================================
void TestDataSeries3D::testStride()
{
    std::vector<double> xs = { 1.0 };
    std::vector<double> ys = { 2.0 };
    std::vector<double> zs = { 3.0 };

    QImVectorXYZDataSeries series(std::move(xs), std::move(ys), std::move(zs));
    QCOMPARE(series.stride(), static_cast<int>(sizeof(double)));
}

QTEST_MAIN(TestDataSeries3D)
#include "tst_data_series3d.moc"
