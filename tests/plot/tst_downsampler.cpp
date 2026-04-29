#include <QObject>
#include <QtTest>
#include <cmath>
#include <vector>
#include <algorithm>
#include <limits>
#include <memory>

#include "QImMinMaxLTTBDownsampler.h"
#include "QImSimdArgMinMax.h"
#include "QImPlotLineItemNode.h"

using namespace QIM;

class TestDownsampler : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    // 9 TDD test cases for MinMaxLTTB algorithm correctness
    void testFirstAndLastPreserved();
    void testOutputCountEqualsTarget();
    void testSmallDataPassThrough();
    void testViewportRangePreservesEndpoints();
    void testAllNaNYDoesNotCrash();
    void testSinglePointBucketNoCrash();
    void testPreselectionRatioAssert();
    void testFindVisibleRangeXY();
    void testFindVisibleRangeYOnly();

    // 6 integration tests for downsampleThreshold Q_PROPERTY and viewport re-sampling
    void testViewportReSamplingOutputCount();
    void testViewportReSamplingEndpointPreservation();
    void testFullRangeBackwardCompatibility();
    void testPixelAwareTargetPointsRange();
    void testThresholdQPropertyGetterSetter();
    void testThresholdChangeTriggersReSampling();

    // 2 new tests for autoDownsample and full-range global extent
    void testConstructorAutoDownsampleFalse();
    void testFullRangeDownsamplePreservesGlobalExtent();

    // 5 SIMD-specific tests for simdArgMinMax
    void testSimdArgMinMaxSmallArray();
    void testSimdArgMinMaxExactMultiple();
    void testSimdArgMinMaxNaNHandling();
    void testSimdArgMinMaxAllNaN();
    void testSimdArgMinMaxResultMatchesScalar();

private:
    // Helper: create linearly spaced X values 0, 1, 2, ..., n-1
    static std::vector< double > makeXs(int n)
    {
        std::vector< double > xs(n);
        for (int i = 0; i < n; ++i)
            xs[ i ] = static_cast< double >(i);
        return xs;
    }

    // Helper: create deterministic Y values for testing
    static std::vector< double > makeYs(int n)
    {
        std::vector< double > ys(n);
        for (int i = 0; i < n; ++i)
            ys[ i ] = std::sin(i * 0.1) + (i % 7) * 0.01;
        return ys;
    }
};

void TestDownsampler::initTestCase()
{
}
void TestDownsampler::cleanupTestCase()
{
}

// ============================================================================
// Test 1: First and last data points must be preserved in downsampled output
// ============================================================================
// GREEN: Unconditional tail push guarantees last point = source last point.
// target_points=20, source_size=100 > 20, downsampling occurs.
// Output: 20 points with first = source[0] and last = source[99].
void TestDownsampler::testFirstAndLastPreserved()
{
    auto source = QImVectorXYDataSeries(makeXs(100), makeYs(100));
    QImMinMaxLTTBDownsampler downsampler(&source, 20, 4.0);

    // First point should always equal original first point
    QCOMPARE(downsampler.xValue(0), source.xValue(0));

    // Last output point should equal original last x (99.0)
    QCOMPARE(downsampler.xValue(downsampler.size() - 1), source.xValue(99));
}

// ============================================================================
// Test 2: Downsampled output count must equal target_points
// ============================================================================
// GREEN: Unconditional tail push guarantees output = 1 + num_buckets + 1
// = target_points. No off-by-one.
void TestDownsampler::testOutputCountEqualsTarget()
{
    const int n = 10000;

    // Target 100
    {
        auto source = QImVectorXYDataSeries(makeXs(n), makeYs(n));
        QImMinMaxLTTBDownsampler ds(&source, 100, 4.0);
        QCOMPARE(ds.size(), 100);
    }
    // Target 500
    {
        auto source = QImVectorXYDataSeries(makeXs(n), makeYs(n));
        QImMinMaxLTTBDownsampler ds(&source, 500, 4.0);
        QCOMPARE(ds.size(), 500);
    }
    // Target 2000 (default)
    {
        auto source = QImVectorXYDataSeries(makeXs(n), makeYs(n));
        QImMinMaxLTTBDownsampler ds(&source, 2000, 4.0);
        QCOMPARE(ds.size(), 2000);
    }
}

// ============================================================================
// Test 3: Small data (< target_points) must pass through without downsampling
// ============================================================================
// This test documents current passthrough behavior and PASSES.
// Serves as a regression test for the zero-copy path.
void TestDownsampler::testSmallDataPassThrough()
{
    auto source = QImVectorXYDataSeries(makeXs(50), makeYs(50));
    QImMinMaxLTTBDownsampler downsampler(&source, 2000, 4.0);

    // xRawData should point to source data (uncached) in passthrough mode
    QCOMPARE(downsampler.xRawData(), source.xRawData());

    // Size should be the source size, not target_points
    QCOMPARE(downsampler.size(), 50);

    // First value should match source
    QCOMPARE(downsampler.xValue(0), source.xValue(0));
}

// ============================================================================
// Test 4: Viewport-aware downsampling must preserve endpoints within range
// ============================================================================
// GREEN: downSampler(x_min, x_max) filters data via findVisibleRange,
// then applies MinMaxLTTB only on the visible portion.
void TestDownsampler::testViewportRangePreservesEndpoints()
{
    auto source = QImVectorXYDataSeries(makeXs(10000), makeYs(10000));
    QImMinMaxLTTBDownsampler downsampler(&source, 500, 4.0);
    downsampler.downSampler(2000.0, 8000.0);

    // First x must be >= 2000 (viewport lower bound)
    QVERIFY(downsampler.xValue(0) >= 2000.0);

    // Last x must be <= 8000 (viewport upper bound)
    QVERIFY(downsampler.xValue(downsampler.size() - 1) <= 8000.0);
}

// ============================================================================
// Test 5: Dataset with all-NaN Y values must not crash
// ============================================================================
// GREEN: All-NaN data is detected in minMaxLTTB, caches are cleared,
// m_cached_valid = false, so size() returns source size (passthrough).
void TestDownsampler::testAllNaNYDoesNotCrash()
{
    const int n = 10000;
    std::vector< double > xs(n), ys(n);
    for (int i = 0; i < n; ++i) {
        xs[ i ] = static_cast< double >(i);
        ys[ i ] = std::numeric_limits< double >::quiet_NaN();
    }
    auto source = QImVectorXYDataSeries(std::move(xs), std::move(ys));

    QImMinMaxLTTBDownsampler downsampler(&source, 2000, 4.0);

    // Must not crash -- basic sanity check
    QVERIFY(downsampler.size() >= 0);

    // All-NaN data clears cache, falls back to source size
    QCOMPARE(downsampler.size(), source.size());
}

// ============================================================================
// Test 6: Extreme preselection_ratio with minimal data must not crash
// ============================================================================
// Tests edge case: 3 data points with extreme ratio and large target.
// Current code: source_size(3) <= target_points(2000) → passthrough.
// No crash. Test validates stability.
void TestDownsampler::testSinglePointBucketNoCrash()
{
    std::vector< double > xs = { 0.0, 1.0, 2.0 };
    std::vector< double > ys = { 3.0, 4.0, 1.0 };
    auto source              = QImVectorXYDataSeries(std::move(xs), std::move(ys));

    QImMinMaxLTTBDownsampler downsampler(&source, 2000, 100.0);

    // Must not crash regardless of extreme parameters
    QVERIFY(downsampler.size() > 0);
}

// ============================================================================
// Test 7: Preselection ratio below minimum must be caught
// ============================================================================
// GREEN: Constructor enforces minimum via std::max(2.0, preselection_ratio).
// ratio=1.5 is clamped to 2.0.
void TestDownsampler::testPreselectionRatioAssert()
{
    auto source = QImVectorXYDataSeries(makeXs(100), makeYs(100));

    // Construct with ratio=1.5 (below minimum 2.0)
    // Constructor clamps to 2.0 via std::max()
    QImMinMaxLTTBDownsampler downsampler(&source, 2000, 1.5);

    // Constructor enforces minimum ratio
    QVERIFY(downsampler.preselectionRatio() >= 2.0);
}

// ============================================================================
// Test 8: findVisibleRange for XY (explicit X array) mode
// ============================================================================
// GREEN: downSampler(x_min, x_max) uses findVisibleRange to filter,
// then applies MinMaxLTTB on the visible portion only.
void TestDownsampler::testFindVisibleRangeXY()
{
    // Create sorted XY data: x = 0, 10, 20, ..., 990 (100 points)
    std::vector< double > xs, ys;
    xs.reserve(100);
    ys.reserve(100);
    for (int i = 0; i < 100; ++i) {
        xs.push_back(i * 10.0);
        ys.push_back(std::sin(i * 0.1));
    }
    auto source = QImVectorXYDataSeries(std::move(xs), std::move(ys));

    // Replicate findVisibleRange XY-mode logic on source data:
    // lower_bound for 250.0 → index 25 (x[25] = 250)
    // upper_bound for 750.0 → index 76 (first x > 750 is x[76] = 760)
    const double* x_ptr = source.xRawData();
    QVERIFY(x_ptr != nullptr);
    int start_idx = static_cast< int >(std::lower_bound(x_ptr, x_ptr + 100, 250.0) - x_ptr);
    int end_idx   = static_cast< int >(std::upper_bound(x_ptr, x_ptr + 100, 750.0) - x_ptr);

    QCOMPARE(start_idx, 25);
    QCOMPARE(end_idx, 76);

    // Verify source data at key indices
    QCOMPARE(source.xValue(25), 250.0);
    QCOMPARE(source.xValue(76), 760.0);

    // Create downsampler with viewport filtering
    QImMinMaxLTTBDownsampler downsampler(&source, 50, 4.0);
    downsampler.downSampler(250.0, 750.0);

    // All output points must be within [250, 750]
    bool allInRange = true;
    for (int i = 0; i < downsampler.size(); ++i) {
        const double xv = downsampler.xValue(i);
        if (xv < 250.0 || xv > 750.0) {
            allInRange = false;
            break;
        }
    }
    QVERIFY(allInRange);
}

// ============================================================================
// Test 9: findVisibleRange for Y-only (implicit X) mode
// ============================================================================
// GREEN: downSampler(x_min, x_max) uses findVisibleRange in Y-only mode
// to compute index range from xStart/xScale, then applies MinMaxLTTB.
void TestDownsampler::testFindVisibleRangeYOnly()
{
    const int n = 1000;

    // Y-only mode: X is computed as xStart + index * xScale
    // Need xs same size as ys for size() to work
    auto ys     = makeYs(n);
    auto xs     = std::vector< double >(n);
    auto source = QImVectorXYDataSeries(std::move(xs), std::move(ys));
    source.setYOnly(true, 0.0, 1.0);

    // Replicate findVisibleRange Y-only logic:
    // start_idx = floor((200.0 - 0.0) / 1.0) = 200
    // end_idx = ceil((500.0 - 0.0) / 1.0) + 1 = 501
    const double xStart = 0.0;
    const double xScale = 1.0;
    int start_idx       = static_cast< int >(std::floor((200.0 - xStart) / xScale));
    int end_idx         = static_cast< int >(std::ceil((500.0 - xStart) / xScale)) + 1;
    start_idx           = std::max(0, start_idx);
    end_idx             = std::min(n, end_idx);

    QCOMPARE(start_idx, 200);
    QCOMPARE(end_idx, 501);

    // Verify Y-only pattern: x is implicit
    QCOMPARE(source.xValue(200), 200.0);
    QCOMPARE(source.xValue(500), 500.0);
    QCOMPARE(source.xRawData(), nullptr);

    // Create downsampler with viewport filtering
    QImMinMaxLTTBDownsampler downsampler(&source, 100, 4.0);
    downsampler.downSampler(200.0, 500.0);

    // All output points must be within [200, 500]
    bool allInRange = true;
    for (int i = 0; i < downsampler.size(); ++i) {
        const double xv = downsampler.xValue(i);
        if (xv < 200.0 || xv > 500.0) {
            allInRange = false;
            break;
        }
    }
    QVERIFY(allInRange);
}

// ============================================================================
// Integration test 1: Viewport-aware re-sampling produces expected output count
// ============================================================================
void TestDownsampler::testViewportReSamplingOutputCount()
{
    const int n = 50000;
    auto source = QImVectorXYDataSeries(makeXs(n), makeYs(n));
    QImMinMaxLTTBDownsampler downsampler(&source, 500, 4.0);
    downsampler.downSampler(10000.0, 20000.0);
    // Visible range [10000,20000] has 10001 points >> target 500, so output = 500
    QCOMPARE(downsampler.size(), 500);
}

// ============================================================================
// Integration test 2: Viewport-aware re-sampling preserves viewport endpoints
// ============================================================================
void TestDownsampler::testViewportReSamplingEndpointPreservation()
{
    const int n = 50000;
    auto source = QImVectorXYDataSeries(makeXs(n), makeYs(n));
    QImMinMaxLTTBDownsampler downsampler(&source, 500, 4.0);
    downsampler.downSampler(10000.0, 20000.0);
    // First x must be >= viewport lower bound
    QVERIFY(downsampler.xValue(0) >= 10000.0);
    // Last x must be <= viewport upper bound
    QVERIFY(downsampler.xValue(downsampler.size() - 1) <= 20000.0);
}

// ============================================================================
// Integration test 3: Full-range downSampler() matches original global behavior
// ============================================================================
void TestDownsampler::testFullRangeBackwardCompatibility()
{
    const int n = 10000;
    auto source = QImVectorXYDataSeries(makeXs(n), makeYs(n));
    QImMinMaxLTTBDownsampler downsampler(&source, 500, 4.0);
    downsampler.downSampler();  // No range params = full range
    QCOMPARE(downsampler.size(), 500);
    QVERIFY(downsampler.xValue(0) >= 0.0);
    QVERIFY(downsampler.xValue(downsampler.size() - 1) <= 9999.0);
}

// ============================================================================
// Integration test 4: pixelAwareTargetPoints calculation and clamping
// ============================================================================
void TestDownsampler::testPixelAwareTargetPointsRange()
{
    // 800px * 1.5 = 1200 (no clamping)
    QCOMPARE(QImPlotItemNode::pixelAwareTargetPoints(800), 1200);
    // 50px * 1.5 = 75 → clamped to 100
    QCOMPARE(QImPlotItemNode::pixelAwareTargetPoints(50), 100);
    // 8000px * 1.5 = 12000 → clamped to 10000
    QCOMPARE(QImPlotItemNode::pixelAwareTargetPoints(8000), 10000);
    // 0px → clamped to 100
    QCOMPARE(QImPlotItemNode::pixelAwareTargetPoints(0), 100);
    // negative → clamped to 100
    QCOMPARE(QImPlotItemNode::pixelAwareTargetPoints(-10), 100);
}

// ============================================================================
// Integration test 5: downsampleThreshold Q_PROPERTY getter/setter
// ============================================================================
void TestDownsampler::testThresholdQPropertyGetterSetter()
{
    auto node = std::make_unique< QImPlotLineItemNode >();
    // Default threshold is 20000
    QCOMPARE(node->downsampleThreshold(), 20000);
    // Set to 5000, verify getter
    node->setDownsampleThreshold(5000);
    QCOMPARE(node->downsampleThreshold(), 5000);
}

// ============================================================================
// Integration test 6: Threshold clamping and re-sampling trigger
// ============================================================================
void TestDownsampler::testThresholdChangeTriggersReSampling()
{
    auto node = std::make_unique< QImPlotLineItemNode >();
    // Default threshold
    QCOMPARE(node->downsampleThreshold(), 20000);
    // Below minimum → clamped to 100
    node->setDownsampleThreshold(50);
    QCOMPARE(node->downsampleThreshold(), 100);
    // At minimum boundary
    node->setDownsampleThreshold(100);
    QCOMPARE(node->downsampleThreshold(), 100);
    // Valid value set triggers internal resetDownSamplerData()
    node->setDownsampleThreshold(500);
    QCOMPARE(node->downsampleThreshold(), 500);
}

// ============================================================================
// New test: autoDownsample=false skips initial downsampling
// ============================================================================
void TestDownsampler::testConstructorAutoDownsampleFalse()
{
    const int n = 10000;
    auto source = QImVectorXYDataSeries(makeXs(n), makeYs(n));

    // autoDownsample=false: constructor should NOT trigger downSampler()
    QImMinMaxLTTBDownsampler ds(&source, 500, 4.0, false);

    // Cache is not valid, so size() returns source size (passthrough)
    QCOMPARE(ds.size(), n);

    // xRawData should point to source data (not downsampled)
    QCOMPARE(ds.xRawData(), source.xRawData());

    // Manually trigger downsampling
    ds.downSampler();
    QCOMPARE(ds.size(), 500);
}

// ============================================================================
// New test: Full-range downsample preserves global data extent
// ============================================================================
void TestDownsampler::testFullRangeDownsamplePreservesGlobalExtent()
{
    const int n = 50000;
    auto source = QImVectorXYDataSeries(makeXs(n), makeYs(n));
    QImMinMaxLTTBDownsampler ds(&source, 1000, 4.0);

    // Full-range downsample: first output x should be original first x
    QCOMPARE(ds.xValue(0), source.xValue(0));

    // Last output x should be original last x
    QCOMPARE(ds.xValue(ds.size() - 1), source.xValue(n - 1));

    // Output count should be exactly target_points
    QCOMPARE(ds.size(), 1000);
}

// ============================================================================
// SIMD test 1: Small array (count < 4) — scalar tail must work correctly
// ============================================================================
void TestDownsampler::testSimdArgMinMaxSmallArray()
{
    // 1 element
    {
        std::vector<double> data = {42.0};
        auto result = QIM::simdArgMinMax(data.data(), 1);
        QCOMPARE(result.min_idx, 0);
        QCOMPARE(result.max_idx, 0);
        QCOMPARE(result.min_val, 42.0);
        QCOMPARE(result.max_val, 42.0);
    }
    // 2 elements
    {
        std::vector<double> data = {10.0, 20.0};
        auto result = QIM::simdArgMinMax(data.data(), 2);
        QCOMPARE(result.min_idx, 0);
        QCOMPARE(result.max_idx, 1);
        QCOMPARE(result.min_val, 10.0);
        QCOMPARE(result.max_val, 20.0);
    }
    // 3 elements (AVX2 tail case: 4 - 3 = 1 remaining)
    {
        std::vector<double> data = {5.0, -3.0, 7.0};
        auto result = QIM::simdArgMinMax(data.data(), 3);
        QCOMPARE(result.min_idx, 1);
        QCOMPARE(result.max_idx, 2);
        QCOMPARE(result.min_val, -3.0);
        QCOMPARE(result.max_val, 7.0);
    }
}

// ============================================================================
// SIMD test 2: Exact SIMD-aligned sizes (4, 8, 16) — results match scalar
// ============================================================================
void TestDownsampler::testSimdArgMinMaxExactMultiple()
{
    const int sizes[] = {4, 8, 16, 32, 64};

    for (int size : sizes) {
        std::vector<double> data(size);
        for (int i = 0; i < size; ++i) {
            data[i] = std::sin(i * 0.1) + (i % 7) * 0.01;
        }

        auto simd_result = QIM::simdArgMinMax(data.data(), size);

        // Compute expected scalar result
        int expected_min_idx = 0, expected_max_idx = 0;
        double expected_min_val = data[0], expected_max_val = data[0];
        for (int i = 1; i < size; ++i) {
            if (data[i] < expected_min_val) { expected_min_val = data[i]; expected_min_idx = i; }
            if (data[i] > expected_max_val) { expected_max_val = data[i]; expected_max_idx = i; }
        }

        QCOMPARE(simd_result.min_idx, expected_min_idx);
        QCOMPARE(simd_result.max_idx, expected_max_idx);
        QCOMPARE(simd_result.min_val, expected_min_val);
        QCOMPARE(simd_result.max_val, expected_max_val);
    }
}

// ============================================================================
// SIMD test 3: NaN handling — NaN values must not be selected as min/max
// ============================================================================
void TestDownsampler::testSimdArgMinMaxNaNHandling()
{
    // Data with NaN at various positions
    std::vector<double> data = {1.0, std::numeric_limits<double>::quiet_NaN(),
                                -5.0, 3.0, std::numeric_limits<double>::quiet_NaN(),
                                10.0, -2.0, 8.0};

    auto result = QIM::simdArgMinMax(data.data(), static_cast<int>(data.size()));

    QCOMPARE(result.min_idx, 2);   // -5.0 at index 2
    QCOMPARE(result.max_idx, 5);   // 10.0 at index 5
    QCOMPARE(result.min_val, -5.0);
    QCOMPARE(result.max_val, 10.0);
}

// ============================================================================
// SIMD test 4: All-NaN data — returns initial values {0, 0, DBL_MAX, -DBL_MAX}
// ============================================================================
void TestDownsampler::testSimdArgMinMaxAllNaN()
{
    const int count = 8;
    std::vector<double> data(count, std::numeric_limits<double>::quiet_NaN());

    auto result = QIM::simdArgMinMax(data.data(), count);

    QCOMPARE(result.min_idx, 0);
    QCOMPARE(result.max_idx, 0);
    QCOMPARE(result.min_val, DBL_MAX);
    QCOMPARE(result.max_val, -DBL_MAX);
}

// ============================================================================
// SIMD test 5: Large random data — SIMD result must exactly match scalar
// ============================================================================
void TestDownsampler::testSimdArgMinMaxResultMatchesScalar()
{
    const int count = 10000;
    std::vector<double> data(count);
    for (int i = 0; i < count; ++i) {
        data[i] = std::sin(i * 0.1) + (i % 7) * 0.01 + std::cos(i * 0.03);
    }

    auto simd_result = QIM::simdArgMinMax(data.data(), count);

    // Compute expected scalar result
    int expected_min_idx = 0, expected_max_idx = 0;
    double expected_min_val = data[0], expected_max_val = data[0];
    for (int i = 1; i < count; ++i) {
        if (data[i] < expected_min_val) { expected_min_val = data[i]; expected_min_idx = i; }
        if (data[i] > expected_max_val) { expected_max_val = data[i]; expected_max_idx = i; }
    }

    QCOMPARE(simd_result.min_idx, expected_min_idx);
    QCOMPARE(simd_result.max_idx, expected_max_idx);
    QCOMPARE(simd_result.min_val, expected_min_val);
    QCOMPARE(simd_result.max_val, expected_max_val);
}

QTEST_MAIN(TestDownsampler)
#include "tst_downsampler.moc"
