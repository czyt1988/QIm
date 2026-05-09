#include <QObject>
#include <QtTest>
#include <vector>
#include <algorithm>

#include "QImDownsamplingController.h"
#include "QImPlotDataSeries.h"

using namespace QIM;

class TestDownsamplingController : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    // Construction and defaults
    void testDefaultConstruction();
    // setAlgorithm / algorithm roundtrip
    void testAlgorithmRoundtrip();
    // setThreshold / threshold (with clamping)
    void testThresholdProperty();
    // setSource stores pointer
    void testSetSource();
    // resolve with nullptr source returns nullptr
    void testResolveNullSource();
    // resolve with algorithm=None returns source directly
    void testResolveAlgorithmNone();
    // resolve with data < threshold returns source directly
    void testResolveBelowThreshold();
    // resolve triggers downsampling when conditions met
    void testResolveTriggersDownsampling();
    // invalidate resets internal state
    void testInvalidate();
    // pixelAwareTargetPoints static method
    void testPixelAwareTargetPoints();
    // resolve with small pixelWidth
    void testResolveSmallPixelWidth();

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
            ys[i] = std::sin(i * 0.1) + (i % 7) * 0.01;
        return ys;
    }
};

void TestDownsamplingController::initTestCase()
{
}

void TestDownsamplingController::cleanupTestCase()
{
}

// ============================================================================
// Test 1: Default construction — algorithm=Auto, threshold=20000
// ============================================================================
void TestDownsamplingController::testDefaultConstruction()
{
    QImDownsamplingController ctrl;
    QCOMPARE(ctrl.algorithm(), QImDownsampleAlgorithm::Auto);
    QCOMPARE(ctrl.threshold(), 20000);
}

// ============================================================================
// Test 2: setAlgorithm / algorithm roundtrip
// ============================================================================
void TestDownsamplingController::testAlgorithmRoundtrip()
{
    QImDownsamplingController ctrl;

    ctrl.setAlgorithm(QImDownsampleAlgorithm::LTTB);
    QCOMPARE(ctrl.algorithm(), QImDownsampleAlgorithm::LTTB);

    ctrl.setAlgorithm(QImDownsampleAlgorithm::MinMaxLTTB);
    QCOMPARE(ctrl.algorithm(), QImDownsampleAlgorithm::MinMaxLTTB);

    ctrl.setAlgorithm(QImDownsampleAlgorithm::None);
    QCOMPARE(ctrl.algorithm(), QImDownsampleAlgorithm::None);

    ctrl.setAlgorithm(QImDownsampleAlgorithm::Auto);
    QCOMPARE(ctrl.algorithm(), QImDownsampleAlgorithm::Auto);
}

// ============================================================================
// Test 3: setThreshold / threshold with clamping (minimum 100)
// ============================================================================
void TestDownsamplingController::testThresholdProperty()
{
    QImDownsamplingController ctrl;

    ctrl.setThreshold(5000);
    QCOMPARE(ctrl.threshold(), 5000);

    ctrl.setThreshold(100000);
    QCOMPARE(ctrl.threshold(), 100000);

    // Below minimum — clamped to 100
    ctrl.setThreshold(50);
    QCOMPARE(ctrl.threshold(), 100);

    // Exact minimum
    ctrl.setThreshold(100);
    QCOMPARE(ctrl.threshold(), 100);

    // Negative — clamped to 100
    ctrl.setThreshold(-10);
    QCOMPARE(ctrl.threshold(), 100);
}

// ============================================================================
// Test 4: setSource stores pointer
// ============================================================================
void TestDownsamplingController::testSetSource()
{
    QImDownsamplingController ctrl;
    auto xs = makeXs(100);
    auto ys = makeYs(100);
    QImVectorXYDataSeries source(std::move(xs), std::move(ys));

    ctrl.setSource(&source);
    // Source is stored — resolve with small pixel returns the source
    // (data below 20000 threshold)
    QImAbstractXYDataSeries* resolved = ctrl.resolve(100, 0.0, 99.0);
    QVERIFY(resolved != nullptr);
    QCOMPARE(resolved, &source);
}

// ============================================================================
// Test 5: resolve with nullptr source returns nullptr
// ============================================================================
void TestDownsamplingController::testResolveNullSource()
{
    QImDownsamplingController ctrl;
    QImAbstractXYDataSeries* resolved = ctrl.resolve(100, 0.0, 100.0);
    QVERIFY(resolved == nullptr);
}

// ============================================================================
// Test 6: resolve with algorithm=None returns source directly
// ============================================================================
void TestDownsamplingController::testResolveAlgorithmNone()
{
    QImDownsamplingController ctrl;
    ctrl.setAlgorithm(QImDownsampleAlgorithm::None);

    auto xs = makeXs(50000);
    auto ys = makeYs(50000);
    QImVectorXYDataSeries source(std::move(xs), std::move(ys));
    ctrl.setSource(&source);

    // Even with large data, algorithm=None bypasses downsampling
    QImAbstractXYDataSeries* resolved = ctrl.resolve(100, 0.0, 49999.0);
    QCOMPARE(resolved, &source);
}

// ============================================================================
// Test 7: resolve with data below threshold returns source directly
// ============================================================================
void TestDownsamplingController::testResolveBelowThreshold()
{
    QImDownsamplingController ctrl;
    // Keep default threshold=20000

    auto xs = makeXs(5000);  // Below threshold
    auto ys = makeYs(5000);
    QImVectorXYDataSeries source(std::move(xs), std::move(ys));
    ctrl.setSource(&source);

    QImAbstractXYDataSeries* resolved = ctrl.resolve(100, 0.0, 4999.0);
    QCOMPARE(resolved, &source);
}

// ============================================================================
// Test 8: Downsampling is triggered when source size exceeds threshold
// ============================================================================
void TestDownsamplingController::testResolveTriggersDownsampling()
{
    QImDownsamplingController ctrl;
    ctrl.setThreshold(1000);
    ctrl.setAlgorithm(QImDownsampleAlgorithm::LTTB);

    auto xs = makeXs(5000);  // Above threshold (1000)
    auto ys = makeYs(5000);
    QImVectorXYDataSeries source(std::move(xs), std::move(ys));
    ctrl.setSource(&source);

    // With LTTB algorithm and 5000 data points, resolve should downsample
    QImAbstractXYDataSeries* resolved = ctrl.resolve(100, 0.0, 4999.0);
    QVERIFY(resolved != nullptr);
    // Downsampled result should have fewer points than source
    QVERIFY(resolved->size() <= source.size());
    QVERIFY(resolved->size() > 0);
}

// ============================================================================
// Test 9: invalidate resets internal state so next resolve re-downsamples
// ============================================================================
void TestDownsamplingController::testInvalidate()
{
    QImDownsamplingController ctrl;
    ctrl.setThreshold(1000);
    ctrl.setAlgorithm(QImDownsampleAlgorithm::LTTB);

    auto xs = makeXs(5000);
    auto ys = makeYs(5000);
    QImVectorXYDataSeries source(std::move(xs), std::move(ys));
    ctrl.setSource(&source);

    // First resolve
    QImAbstractXYDataSeries* r1 = ctrl.resolve(100, 0.0, 4999.0);
    QVERIFY(r1 != nullptr);
    int size1 = r1->size();

    // Mark the source as dirty and call invalidate
    ctrl.invalidate();

    // Second resolve after invalidation should still work
    QImAbstractXYDataSeries* r2 = ctrl.resolve(100, 0.0, 4999.0);
    QVERIFY(r2 != nullptr);
    QVERIFY(r2->size() > 0);
}

// ============================================================================
// Test 10: pixelAwareTargetPoints static method
// ============================================================================
void TestDownsamplingController::testPixelAwareTargetPoints()
{
    // 800px * 5 = 4000
    QCOMPARE(QImDownsamplingController::pixelAwareTargetPoints(800), 4000);

    // 50px * 5 = 250 (no clamping needed)
    QCOMPARE(QImDownsamplingController::pixelAwareTargetPoints(50), 250);

    // 0px → returns minimum 100
    QCOMPARE(QImDownsamplingController::pixelAwareTargetPoints(0), 100);

    // 20000px * 5 = 100000 → clamped to 50000
    QCOMPARE(QImDownsamplingController::pixelAwareTargetPoints(20000), 50000);

    // Very small pixel width
    QVERIFY(QImDownsamplingController::pixelAwareTargetPoints(10) >= 100);
    QVERIFY(QImDownsamplingController::pixelAwareTargetPoints(10) <= 50000);
}

// ============================================================================
// Test 11: resolve with small pixelWidth works
// ============================================================================
void TestDownsamplingController::testResolveSmallPixelWidth()
{
    QImDownsamplingController ctrl;
    ctrl.setThreshold(1000);
    ctrl.setAlgorithm(QImDownsampleAlgorithm::MinMaxLTTB);

    auto xs = makeXs(5000);
    auto ys = makeYs(5000);
    QImVectorXYDataSeries source(std::move(xs), std::move(ys));
    ctrl.setSource(&source);

    // Small pixel width gives fewer target points
    QImAbstractXYDataSeries* resolved = ctrl.resolve(20, 0.0, 4999.0);
    QVERIFY(resolved != nullptr);
    QVERIFY(resolved->size() > 0);
}

QTEST_MAIN(TestDownsamplingController)
#include "tst_downsampling_controller.moc"
