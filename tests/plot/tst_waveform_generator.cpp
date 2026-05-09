#include <QObject>
#include <QtTest>
#include <cmath>
#include <vector>

#ifndef M_PI
constexpr double M_PI = 3.14159265358979323846;
#endif

#include "QImWaveformGenerator.hpp"

using namespace QIM;

class TestWaveformGenerator : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    // Sine wave tests
    void testSineWaveBasic();
    void testSineWaveAmplitude();
    void testSineWavePhase();
    void testSineWavePointCount();
    // Cosine wave tests
    void testCosineWaveBasic();
    void testCosineWavePhase();
    // Damped sine wave tests
    void testDampedSineWaveDecay();
    // Exponential decay tests
    void testExponentialDecay();
    // Linear wave tests
    void testLinearWave();
    // Edge cases
    void testInvalidPointCount();
    void testInvalidXRange();
    void testSingleCycle();
    // Factory function
    void testMakeWaveformFactory();
};

void TestWaveformGenerator::initTestCase()
{
}

void TestWaveformGenerator::cleanupTestCase()
{
}

// ============================================================================
// Sine wave: basic generation with default parameters
// ============================================================================
void TestWaveformGenerator::testSineWaveBasic()
{
    auto wave = make_waveform<SineWave>(1.0);
    auto [x, y] = wave.generate(100, 0.0, 2.0 * M_PI);

    QCOMPARE(static_cast<int>(x.size()), 100);
    QCOMPARE(static_cast<int>(y.size()), 100);

    // Start: sin(0) = 0
    QVERIFY(qFuzzyCompare(y[0], 0.0));

    // At pi/2: sin(pi/2) = 1
    int idx = 24;  // 0 + 24 * (2*pi/99) ≈ pi/2
    QVERIFY(y[idx] > 0.9 && y[idx] < 1.01);

    // End: sin(2*pi) ≈ 0
    QVERIFY(std::abs(y[99]) < 1e-6);
}

// ============================================================================
// Sine wave: amplitude parameter
// ============================================================================
void TestWaveformGenerator::testSineWaveAmplitude()
{
    auto wave = make_waveform<SineWave>(3.0);
    auto [x, y] = wave.generate(100, 0.0, 2.0 * M_PI);

    // At pi/2: 3 * sin(pi/2) = 3
    int idx = 24;
    QVERIFY(y[idx] > 2.9 && y[idx] < 3.01);

    // Min should be around -3
    int idxMin = 74;  // 3*pi/2 ≈ index 74
    QVERIFY(y[idxMin] < -2.9 && y[idxMin] > -3.01);
}

// ============================================================================
// Sine wave: phase parameter
// ============================================================================
void TestWaveformGenerator::testSineWavePhase()
{
    // sin(x + pi/2) = cos(x)
    auto wave = make_waveform<SineWave>(1.0, M_PI / 2.0);
    auto [x, y] = wave.generate(100, 0.0, 2.0 * M_PI);

    // sin(0 + pi/2) = 1
    QVERIFY(qFuzzyCompare(y[0], 1.0));
}

// ============================================================================
// Sine wave: data point count is correct
// ============================================================================
void TestWaveformGenerator::testSineWavePointCount()
{
    auto wave = make_waveform<SineWave>(1.0);

    auto [x1, y1] = wave.generate(10, 0.0, M_PI);
    QCOMPARE(static_cast<int>(x1.size()), 10);
    QCOMPARE(static_cast<int>(y1.size()), 10);

    auto [x2, y2] = wave.generate(1000, 0.0, 4.0 * M_PI);
    QCOMPARE(static_cast<int>(x2.size()), 1000);
    QCOMPARE(static_cast<int>(y2.size()), 1000);
}

// ============================================================================
// Cosine wave: basic generation
// ============================================================================
void TestWaveformGenerator::testCosineWaveBasic()
{
    auto wave = make_waveform<CosineWave>(1.0);
    auto [x, y] = wave.generate(100, 0.0, 2.0 * M_PI);

    // cos(0) = 1
    QVERIFY(qFuzzyCompare(y[0], 1.0));

    // cos(pi/2) ≈ 0
    int idx = 24;
    QVERIFY(std::abs(y[idx]) < 0.1);
}

// ============================================================================
// Cosine wave: phase shift
// ============================================================================
void TestWaveformGenerator::testCosineWavePhase()
{
    // cos(x + pi/2) = -sin(x)
    auto wave = make_waveform<CosineWave>(1.0, M_PI / 2.0);
    auto [x, y] = wave.generate(100, 0.0, 2.0 * M_PI);

    // cos(0 + pi/2) = 0
    QVERIFY(std::abs(y[0]) < 1e-10);
}

// ============================================================================
// Damped sine: amplitude decreases with x
// ============================================================================
void TestWaveformGenerator::testDampedSineWaveDecay()
{
    auto wave = make_waveform<DampedSineWave>(1.0, 0.5);
    auto [x, y] = wave.generate(1000, 0.0, 10.0);

    // At larger x, the envelope should be smaller
    double maxEarly = 0.0;
    double maxLate  = 0.0;
    for (int i = 0; i < 200; ++i)
        maxEarly = std::max(maxEarly, std::abs(y[i]));
    for (int i = 800; i < 1000; ++i)
        maxLate = std::max(maxLate, std::abs(y[i]));

    // Later values should have smaller envelope due to damping
    QVERIFY(maxLate < maxEarly);
}

// ============================================================================
// Exponential decay: values decay over time
// ============================================================================
void TestWaveformGenerator::testExponentialDecay()
{
    auto wave = make_waveform<ExponentialDecay>(1.0, 0.5);
    auto [x, y] = wave.generate(100, 0.0, 10.0);

    // e^(0) = 1
    QVERIFY(qFuzzyCompare(y[0], 1.0));

    // Values should be positive and decreasing
    for (int i = 1; i < 100; ++i) {
        QVERIFY(y[i] > 0.0);
        QVERIFY(y[i] < y[0] + 1e-10);
    }

    // Last value should be much smaller than first
    QVERIFY(y[99] < 0.1);
}

// ============================================================================
// Linear wave: constant slope
// ============================================================================
void TestWaveformGenerator::testLinearWave()
{
    auto wave = make_waveform<LinearWave>(2.0, 10.0);  // slope=2, offset=10
    auto [x, y] = wave.generate(100, 0.0, 10.0);

    // y = 2*x + 10
    QCOMPARE(y[0], 10.0);
    double lastExpected = 2.0 * x[99] + 10.0;
    QVERIFY(qFuzzyCompare(y[99], lastExpected));

    // Midpoint check
    QCOMPARE(y[50], 2.0 * x[50] + 10.0);
}

// ============================================================================
// Invalid numPoints (< 2) returns empty vectors
// ============================================================================
void TestWaveformGenerator::testInvalidPointCount()
{
    auto wave = make_waveform<SineWave>(1.0);

    // 0 points
    auto [x0, y0] = wave.generate(0, 0.0, 10.0);
    QVERIFY(x0.empty());
    QVERIFY(y0.empty());

    // 1 point
    auto [x1, y1] = wave.generate(1, 0.0, 10.0);
    QVERIFY(x1.empty());
    QVERIFY(y1.empty());

    // Negative points
    auto [xNeg, yNeg] = wave.generate(-5, 0.0, 10.0);
    QVERIFY(xNeg.empty());
    QVERIFY(yNeg.empty());
}

// ============================================================================
// Invalid x range (x_end <= x_start) returns empty vectors
// ============================================================================
void TestWaveformGenerator::testInvalidXRange()
{
    auto wave = make_waveform<SineWave>(1.0);

    // x_end == x_start
    auto [x1, y1] = wave.generate(10, 5.0, 5.0);
    QVERIFY(x1.empty());
    QVERIFY(y1.empty());

    // x_end < x_start
    auto [x2, y2] = wave.generate(10, 10.0, 0.0);
    QVERIFY(x2.empty());
    QVERIFY(y2.empty());
}

// ============================================================================
// Single cycle of sine wave: complete exactly one period
// ============================================================================
void TestWaveformGenerator::testSingleCycle()
{
    auto wave = make_waveform<SineWave>(1.0);
    auto [x, y] = wave.generate(5, 0.0, 2.0 * M_PI);

    QCOMPARE(static_cast<int>(x.size()), 5);
    QVERIFY(qFuzzyCompare(y[0], 0.0));   // sin(0) = 0
    QVERIFY(y[1] > 0.0);                  // rising
    QVERIFY(std::abs(y[4]) < 1e-6);       // sin(2*pi) ≈ 0
}

// ============================================================================
// Factory function creates generator with correct type
// ============================================================================
void TestWaveformGenerator::testMakeWaveformFactory()
{
    // Factory should compile and return a correctly typed generator
    auto sineGen    = make_waveform<SineWave>(2.0, M_PI / 4.0);
    auto cosGen     = make_waveform<CosineWave>(1.5);
    auto dampedGen  = make_waveform<DampedSineWave>(3.0, 0.15, 0.0);
    auto expGen     = make_waveform<ExponentialDecay>(2.0, 0.3);
    auto linearGen  = make_waveform<LinearWave>(0.5, -1.0);

    // Verify each generates data without crashing
    auto [xs, ys] = sineGen.generate(50, 0.0, 2.0 * M_PI);
    QCOMPARE(static_cast<int>(xs.size()), 50);

    auto [xc, yc] = cosGen.generate(50, 0.0, 2.0 * M_PI);
    QCOMPARE(static_cast<int>(xc.size()), 50);

    auto [xd, yd] = dampedGen.generate(50, 0.0, 10.0);
    QCOMPARE(static_cast<int>(xd.size()), 50);

    auto [xe, ye] = expGen.generate(50, 0.0, 10.0);
    QCOMPARE(static_cast<int>(xe.size()), 50);

    auto [xl, yl] = linearGen.generate(50, -5.0, 5.0);
    QCOMPARE(static_cast<int>(xl.size()), 50);
}

QTEST_MAIN(TestWaveformGenerator)
#include "tst_waveform_generator.moc"
