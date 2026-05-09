#include <QObject>
#include <QtTest>
#include <QSignalSpy>

#include "QImPlot3DAxisInfo.h"
#include "QImPlot3DNode.h"
#include "QImPlot3DAxisFormatter.h"

using namespace QIM;

class TestPlot3DAxisInfo : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    // Default construction
    void testDefaultConstruction();
    // axisId returns correct enum
    void testAxisId();
    // imAxis3D returns raw ImAxis3D value
    void testImAxis3D();
    // plotNode returns nullptr when no plot attached
    void testPlotNodeNull();
    // label property getter/setter
    void testLabelProperty();
    // labelChanged signal
    void testLabelChangedSignal();
    // min/max limits
    void testMinLimit();
    void testMaxLimit();
    void testSetLimits();
    // limitsChanged signal
    void testLimitsChangedSignal();
    // scale property
    void testScaleDefault();
    void testScaleLinear();
    void testScaleLog10();
    void testScaleSymLog();
    // flag accessors â€?negative to positive semantic
    void testLabelEnabled();
    void testGridLinesEnabled();
    void testTickMarksEnabled();
    void testTickLabelsEnabled();
    // flag accessors â€?positive to positive semantic
    void testLockMin();
    void testLockMax();
    void testAutoFit();
    void testInvert();
    void testPanStretch();
    // combined flags
    void testLock();
    void testDecorationsEnabled();
    // limits constraints
    void testLimitsConstraintMin();
    void testLimitsConstraintMax();
    void testSetLimitsConstraints();
    // zoom constraints
    void testZoomConstraintMin();
    void testZoomConstraintMax();
    void testSetZoomConstraints();
    // tick configuration
    void testTickValues();
    void testTickLabels();
    void testKeepDefaultTicks();
    void testAxisTicksConvenience();
    void testAxisTicksRangeConvenience();
    // axis formatter
    void testAxisFormatterGetterSetter();
    // signal: axisFlagChanged
    void testAxisFlagChangedSignal();
    // signal: scaleChanged
    void testScaleChangedSignal();
    // signal: limitsConstraintsChanged
    void testLimitsConstraintsChangedSignal();
};

void TestPlot3DAxisInfo::initTestCase()
{
}

void TestPlot3DAxisInfo::cleanupTestCase()
{
}

// ============================================================================
// Test 1: Default construction with X axis
// ============================================================================
void TestPlot3DAxisInfo::testDefaultConstruction()
{
    QImPlot3DAxisInfo axis(QImPlot3DAxisId::X1);

    QCOMPARE(axis.axisId(), QImPlot3DAxisId::X1);
    QVERIFY(axis.label().isEmpty());
    QVERIFY(qFuzzyCompare(axis.minLimit(), 0.0));
    QVERIFY(qFuzzyCompare(axis.maxLimit(), 1.0));
    QCOMPARE(axis.scale(), QImPlot3DScaleType::Linear);
    QVERIFY(axis.isAutoFit());
    QVERIFY(!axis.isInvert());
}

// ============================================================================
// Test 2: axisId returns correct enum for each axis
// ============================================================================
void TestPlot3DAxisInfo::testAxisId()
{
    QImPlot3DAxisInfo xAxis(QImPlot3DAxisId::X1);
    QCOMPARE(xAxis.axisId(), QImPlot3DAxisId::X1);

    QImPlot3DAxisInfo yAxis(QImPlot3DAxisId::Y1);
    QCOMPARE(yAxis.axisId(), QImPlot3DAxisId::Y1);

    QImPlot3DAxisInfo zAxis(QImPlot3DAxisId::Z1);
    QCOMPARE(zAxis.axisId(), QImPlot3DAxisId::Z1);
}

// ============================================================================
// Test 3: imAxis3D returns raw ImAxis3D value
// ============================================================================
void TestPlot3DAxisInfo::testImAxis3D()
{
    QImPlot3DAxisInfo xAxis(QImPlot3DAxisId::X1);
    QVERIFY(xAxis.imAxis3D() >= 0);

    QImPlot3DAxisInfo yAxis(QImPlot3DAxisId::Y1);
    QVERIFY(yAxis.imAxis3D() != xAxis.imAxis3D());

    QImPlot3DAxisInfo zAxis(QImPlot3DAxisId::Z1);
    QVERIFY(zAxis.imAxis3D() != xAxis.imAxis3D());
    QVERIFY(zAxis.imAxis3D() != yAxis.imAxis3D());
}

// ============================================================================
// Test 4: plotNode returns nullptr when no plot attached
// ============================================================================
void TestPlot3DAxisInfo::testPlotNodeNull()
{
    QImPlot3DAxisInfo axis(QImPlot3DAxisId::X1);
    QVERIFY(axis.plotNode() == nullptr);
}

// ============================================================================
// Test 5: label property getter/setter
// ============================================================================
void TestPlot3DAxisInfo::testLabelProperty()
{
    QImPlot3DAxisInfo axis(QImPlot3DAxisId::X1);

    QVERIFY(axis.label().isEmpty());

    axis.setLabel("X Axis");
    QCOMPARE(axis.label(), QString("X Axis"));

    axis.setLabel("");
    QVERIFY(axis.label().isEmpty());
}

// ============================================================================
// Test 6: labelChanged signal
// ============================================================================
void TestPlot3DAxisInfo::testLabelChangedSignal()
{
    QImPlot3DAxisInfo axis(QImPlot3DAxisId::X1);
    QSignalSpy spy(&axis, &QImPlot3DAxisInfo::labelChanged);

    axis.setLabel("Test");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toString(), QString("Test"));

    // Same label â€?no signal
    axis.setLabel("Test");
    QCOMPARE(spy.count(), 1);
}

// ============================================================================
// Test 7: setMinLimit / minLimit
// ============================================================================
void TestPlot3DAxisInfo::testMinLimit()
{
    QImPlot3DAxisInfo axis(QImPlot3DAxisId::Y1);

    axis.setMinLimit(-5.0);
    QVERIFY(qFuzzyCompare(axis.minLimit(), -5.0));

    axis.setMinLimit(0.0);
    QVERIFY(qFuzzyCompare(axis.minLimit(), 0.0));
}

// ============================================================================
// Test 8: setMaxLimit / maxLimit
// ============================================================================
void TestPlot3DAxisInfo::testMaxLimit()
{
    QImPlot3DAxisInfo axis(QImPlot3DAxisId::Y1);

    axis.setMaxLimit(100.0);
    QVERIFY(qFuzzyCompare(axis.maxLimit(), 100.0));

    axis.setMaxLimit(2.718);
    QVERIFY(qFuzzyCompare(axis.maxLimit(), 2.718));
}

// ============================================================================
// Test 9: setLimits sets both min and max
// ============================================================================
void TestPlot3DAxisInfo::testSetLimits()
{
    QImPlot3DAxisInfo axis(QImPlot3DAxisId::X1);

    axis.setLimits(-10.0, 10.0);
    QVERIFY(qFuzzyCompare(axis.minLimit(), -10.0));
    QVERIFY(qFuzzyCompare(axis.maxLimit(), 10.0));

    axis.setLimits(0.0, 100.0, QImPlot3DCondition::Always);
    QVERIFY(qFuzzyCompare(axis.minLimit(), 0.0));
    QVERIFY(qFuzzyCompare(axis.maxLimit(), 100.0));
}

// ============================================================================
// Test 10: limitsChanged signal
// ============================================================================
void TestPlot3DAxisInfo::testLimitsChangedSignal()
{
    QImPlot3DAxisInfo axis(QImPlot3DAxisId::X1);
    QSignalSpy spy(&axis, &QImPlot3DAxisInfo::limitsChanged);

    axis.setLimits(-10.0, 10.0);
    QCOMPARE(spy.count(), 1);
    QVERIFY(qFuzzyCompare(spy.at(0).at(0).toDouble(), -10.0));
    QVERIFY(qFuzzyCompare(spy.at(0).at(1).toDouble(), 10.0));

    // Same limits â€?no signal
    axis.setLimits(-10.0, 10.0);
    QCOMPARE(spy.count(), 1);
}

// ============================================================================
// Test 11: Default scale type
// ============================================================================
void TestPlot3DAxisInfo::testScaleDefault()
{
    QImPlot3DAxisInfo axis(QImPlot3DAxisId::X1);
    QCOMPARE(axis.scale(), QImPlot3DScaleType::Linear);
}

// ============================================================================
// Test 12: Set scale to Linear
// ============================================================================
void TestPlot3DAxisInfo::testScaleLinear()
{
    QImPlot3DAxisInfo axis(QImPlot3DAxisId::X1);
    axis.setScale(QImPlot3DScaleType::Linear);
    QCOMPARE(axis.scale(), QImPlot3DScaleType::Linear);
}

// ============================================================================
// Test 13: Set scale to Log10
// ============================================================================
void TestPlot3DAxisInfo::testScaleLog10()
{
    QImPlot3DAxisInfo axis(QImPlot3DAxisId::Y1);
    axis.setScale(QImPlot3DScaleType::Log10);
    QCOMPARE(axis.scale(), QImPlot3DScaleType::Log10);
}

// ============================================================================
// Test 14: Set scale to SymLog
// ============================================================================
void TestPlot3DAxisInfo::testScaleSymLog()
{
    QImPlot3DAxisInfo axis(QImPlot3DAxisId::Y1);
    axis.setScale(QImPlot3DScaleType::SymLog);
    QCOMPARE(axis.scale(), QImPlot3DScaleType::SymLog);
}

// ============================================================================
// Negative to Positive flag accessors
// ============================================================================
void TestPlot3DAxisInfo::testLabelEnabled()
{
    QImPlot3DAxisInfo axis(QImPlot3DAxisId::X1);
    QVERIFY(axis.isLabelEnabled());  // Default: NoLabel NOT set

    axis.setLabelEnabled(false);
    QVERIFY(!axis.isLabelEnabled());

    axis.setLabelEnabled(true);
    QVERIFY(axis.isLabelEnabled());
}

void TestPlot3DAxisInfo::testGridLinesEnabled()
{
    QImPlot3DAxisInfo axis(QImPlot3DAxisId::Y1);
    QVERIFY(axis.isGridLinesEnabled());  // Default: NoGridLines NOT set

    axis.setGridLinesEnabled(false);
    QVERIFY(!axis.isGridLinesEnabled());

    axis.setGridLinesEnabled(true);
    QVERIFY(axis.isGridLinesEnabled());
}

void TestPlot3DAxisInfo::testTickMarksEnabled()
{
    QImPlot3DAxisInfo axis(QImPlot3DAxisId::X1);
    QVERIFY(axis.isTickMarksEnabled());

    axis.setTickMarksEnabled(false);
    QVERIFY(!axis.isTickMarksEnabled());
}

void TestPlot3DAxisInfo::testTickLabelsEnabled()
{
    QImPlot3DAxisInfo axis(QImPlot3DAxisId::X1);
    QVERIFY(axis.isTickLabelsEnabled());

    axis.setTickLabelsEnabled(false);
    QVERIFY(!axis.isTickLabelsEnabled());
}

// ============================================================================
// Positive to Positive flag accessors
// ============================================================================
void TestPlot3DAxisInfo::testLockMin()
{
    QImPlot3DAxisInfo axis(QImPlot3DAxisId::X1);
    QVERIFY(!axis.isLockMin());

    axis.setLockMin(true);
    QVERIFY(axis.isLockMin());

    axis.setLockMin(false);
    QVERIFY(!axis.isLockMin());
}

void TestPlot3DAxisInfo::testLockMax()
{
    QImPlot3DAxisInfo axis(QImPlot3DAxisId::X1);
    QVERIFY(!axis.isLockMax());

    axis.setLockMax(true);
    QVERIFY(axis.isLockMax());
}

void TestPlot3DAxisInfo::testAutoFit()
{
    QImPlot3DAxisInfo axis(QImPlot3DAxisId::X1);
    QVERIFY(axis.isAutoFit());

    axis.setAutoFit(false);
    QVERIFY(!axis.isAutoFit());
}

void TestPlot3DAxisInfo::testInvert()
{
    QImPlot3DAxisInfo axis(QImPlot3DAxisId::Y1);
    QVERIFY(!axis.isInvert());

    axis.setInvert(true);
    QVERIFY(axis.isInvert());
}

void TestPlot3DAxisInfo::testPanStretch()
{
    QImPlot3DAxisInfo axis(QImPlot3DAxisId::X1);
    QVERIFY(!axis.isPanStretch());

    axis.setPanStretch(true);
    QVERIFY(axis.isPanStretch());
}

// ============================================================================
// Combined flags
// ============================================================================
void TestPlot3DAxisInfo::testLock()
{
    QImPlot3DAxisInfo axis(QImPlot3DAxisId::X1);

    QVERIFY(!axis.isLock());

    axis.setLock(true);
    QVERIFY(axis.isLock());
    QVERIFY(axis.isLockMin());
    QVERIFY(axis.isLockMax());

    axis.setLock(false);
    QVERIFY(!axis.isLock());
    QVERIFY(!axis.isLockMin());
    QVERIFY(!axis.isLockMax());
}

void TestPlot3DAxisInfo::testDecorationsEnabled()
{
    QImPlot3DAxisInfo axis(QImPlot3DAxisId::X1);
    QVERIFY(axis.isDecorationsEnabled());

    axis.setDecorationsEnabled(false);
    QVERIFY(!axis.isDecorationsEnabled());
    // When decorations disabled, individual items reflect that
    QVERIFY(!axis.isLabelEnabled());
    QVERIFY(!axis.isGridLinesEnabled());
    QVERIFY(!axis.isTickLabelsEnabled());

    axis.setDecorationsEnabled(true);
    QVERIFY(axis.isDecorationsEnabled());
}

// ============================================================================
// Limits constraints
// ============================================================================
void TestPlot3DAxisInfo::testLimitsConstraintMin()
{
    QImPlot3DAxisInfo axis(QImPlot3DAxisId::X1);

    axis.setLimitsConstraintMin(-50.0);
    QVERIFY(qFuzzyCompare(axis.limitsConstraintMin(), -50.0));
}

void TestPlot3DAxisInfo::testLimitsConstraintMax()
{
    QImPlot3DAxisInfo axis(QImPlot3DAxisId::X1);

    axis.setLimitsConstraintMax(50.0);
    QVERIFY(qFuzzyCompare(axis.limitsConstraintMax(), 50.0));
}

void TestPlot3DAxisInfo::testSetLimitsConstraints()
{
    QImPlot3DAxisInfo axis(QImPlot3DAxisId::X1);

    axis.setLimitsConstraints(-100.0, 100.0);
    QVERIFY(qFuzzyCompare(axis.limitsConstraintMin(), -100.0));
    QVERIFY(qFuzzyCompare(axis.limitsConstraintMax(), 100.0));
}

// ============================================================================
// Zoom constraints
// ============================================================================
void TestPlot3DAxisInfo::testZoomConstraintMin()
{
    QImPlot3DAxisInfo axis(QImPlot3DAxisId::X1);

    axis.setZoomConstraintMin(0.5);
    QVERIFY(qFuzzyCompare(axis.zoomConstraintMin(), 0.5));
}

void TestPlot3DAxisInfo::testZoomConstraintMax()
{
    QImPlot3DAxisInfo axis(QImPlot3DAxisId::X1);

    axis.setZoomConstraintMax(100.0);
    QVERIFY(qFuzzyCompare(axis.zoomConstraintMax(), 100.0));
}

void TestPlot3DAxisInfo::testSetZoomConstraints()
{
    QImPlot3DAxisInfo axis(QImPlot3DAxisId::X1);

    axis.setZoomConstraints(0.1, 50.0);
    QVERIFY(qFuzzyCompare(axis.zoomConstraintMin(), 0.1));
    QVERIFY(qFuzzyCompare(axis.zoomConstraintMax(), 50.0));
}

// ============================================================================
// Tick configuration
// ============================================================================
void TestPlot3DAxisInfo::testTickValues()
{
    QImPlot3DAxisInfo axis(QImPlot3DAxisId::X1);

    QList<double> values = { 0.0, 1.0, 2.0, 3.0 };
    axis.setTickValues(values);
    QCOMPARE(axis.tickValues(), values);
}

void TestPlot3DAxisInfo::testTickLabels()
{
    QImPlot3DAxisInfo axis(QImPlot3DAxisId::X1);

    QList<QByteArray> labels = { QByteArray("A"), QByteArray("B") };
    axis.setTickLabels(labels);
    QCOMPARE(axis.tickLabels(), labels);
}

void TestPlot3DAxisInfo::testKeepDefaultTicks()
{
    QImPlot3DAxisInfo axis(QImPlot3DAxisId::X1);

    QVERIFY(!axis.isKeepDefaultTicks());

    axis.setKeepDefaultTicks(true);
    QVERIFY(axis.isKeepDefaultTicks());
}

void TestPlot3DAxisInfo::testAxisTicksConvenience()
{
    QImPlot3DAxisInfo axis(QImPlot3DAxisId::X1);
    QSignalSpy spy(&axis, &QImPlot3DAxisInfo::tickConfigChanged);

    QList<double> values = { 0.0, 5.0, 10.0 };
    QList<QByteArray> labels = { QByteArray("0"), QByteArray("mid"), QByteArray("10") };
    axis.setAxisTicks(values, labels, true);

    QCOMPARE(spy.count(), 1);
    QCOMPARE(axis.tickValues(), values);
    QCOMPARE(axis.tickLabels(), labels);
    QVERIFY(axis.isKeepDefaultTicks());
}

void TestPlot3DAxisInfo::testAxisTicksRangeConvenience()
{
    QImPlot3DAxisInfo axis(QImPlot3DAxisId::Y1);
    QSignalSpy spy(&axis, &QImPlot3DAxisInfo::tickConfigChanged);

    axis.setAxisTicksRange(0.0, 1.0, 5);
    QCOMPARE(spy.count(), 1);
    // After range ticks, tickValues should be empty, but range is stored internally
}

// ============================================================================
// Axis formatter
// ============================================================================
void TestPlot3DAxisInfo::testAxisFormatterGetterSetter()
{
    QImPlot3DAxisInfo axis(QImPlot3DAxisId::X1);
    QSignalSpy spy(&axis, &QImPlot3DAxisInfo::axisFormatterChanged);

    QVERIFY(axis.axisFormatter() == nullptr);

    QImPlot3DDefaultAxisFormatter formatter;
    axis.setAxisFormatter(&formatter);
    QCOMPARE(axis.axisFormatter(), &formatter);
    QCOMPARE(spy.count(), 1);

    axis.setAxisFormatter(nullptr);
    QVERIFY(axis.axisFormatter() == nullptr);
    QCOMPARE(spy.count(), 2);

    // Same value â€?no signal
    axis.setAxisFormatter(nullptr);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// axisFlagChanged signal
// ============================================================================
void TestPlot3DAxisInfo::testAxisFlagChangedSignal()
{
    QImPlot3DAxisInfo axis(QImPlot3DAxisId::X1);
    QSignalSpy spy(&axis, &QImPlot3DAxisInfo::axisFlagChanged);

    axis.setAutoFit(false);
    QCOMPARE(spy.count(), 1);

    axis.setInvert(true);
    QCOMPARE(spy.count(), 2);

    // Same value â€?no signal
    axis.setInvert(true);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// scaleChanged signal
// ============================================================================
void TestPlot3DAxisInfo::testScaleChangedSignal()
{
    QImPlot3DAxisInfo axis(QImPlot3DAxisId::X1);
    QSignalSpy spy(&axis, &QImPlot3DAxisInfo::scaleChanged);

    axis.setScale(QImPlot3DScaleType::Log10);
    QCOMPARE(spy.count(), 1);

    axis.setScale(QImPlot3DScaleType::Linear);
    QCOMPARE(spy.count(), 2);

    // Same value â€?no signal
    axis.setScale(QImPlot3DScaleType::Linear);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// limitsConstraintsChanged signal
// ============================================================================
void TestPlot3DAxisInfo::testLimitsConstraintsChangedSignal()
{
    QImPlot3DAxisInfo axis(QImPlot3DAxisId::X1);
    QSignalSpy spy(&axis, &QImPlot3DAxisInfo::limitsConstraintsChanged);

    axis.setLimitsConstraints(-10.0, 10.0);
    QCOMPARE(spy.count(), 1);

    // Same values â€?no signal
    axis.setLimitsConstraints(-10.0, 10.0);
    QCOMPARE(spy.count(), 1);
}

QTEST_MAIN(TestPlot3DAxisInfo)
#include "tst_axis_info.moc"
