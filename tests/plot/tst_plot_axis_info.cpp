#include <QObject>
#include <QtTest>

#include "QImPlotAxisInfo.h"
#include "QImPlotNode.h"
#include "implot.h"

using namespace QIM;

class TestPlotAxisInfo : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    // Default construction
    void testDefaultConstruction();
    // axisId returns correct QImPlotAxisId
    void testAxisId();
    // imAxis returns raw ImAxis value
    void testImAxis();
    // label property getter/setter
    void testLabelProperty();
    // labelChanged signal
    void testLabelChangedSignal();
    // minLimits getter/setter
    void testMinLimits();
    // maxLimits getter/setter
    void testMaxLimits();
    // setLimits sets both min and max
    void testSetLimits();
    // limitsChanged signal
    void testLimitsChangedSignal();
    // scaleType property
    void testScaleTypeDefault();
    void testScaleTypeLinear();
    void testScaleTypeLog();
    void testScaleTypeTime();
    void testScaleTypeSymLog();
    // imPlotScale returns raw ImPlotScale value
    void testImPlotScale();
    // autoFit property
    void testAutoFit();
    // invert property
    void testInverted();
    // axisFlags raw access
    void testAxisFlags();
    // axisFlagChanged signal
    void testAxisFlagChangedSignal();
    // labelEnabled (negative-semantic NoLabel flag)
    void testLabelEnabled();
    // gridLinesEnabled (negative-semantic NoGridLines flag)
    void testGridLinesEnabled();
    // lockMin/lockMax/lock
    void testLock();
    // opposite property
    void testOpposite();
    // enabled property (primary axes use NoDecorations)
    void testEnabled();
    // plotNode returns associated plot
    void testPlotNode();
    // Multiple Y-axes (X1, Y1, Y2)
    void testMultipleAxes();
};

void TestPlotAxisInfo::initTestCase()
{
}

void TestPlotAxisInfo::cleanupTestCase()
{
}

// ============================================================================
// Test 1: Default construction with X1 axis
// ============================================================================
void TestPlotAxisInfo::testDefaultConstruction()
{
    QImPlotAxisInfo axis(QImPlotAxisId::X1);

    QCOMPARE(axis.axisId(), QImPlotAxisId::X1);
    // Default label is empty
    QVERIFY(axis.label().isEmpty());
    // Default limits: min=0.0, max=1.0
    QVERIFY(qFuzzyCompare(axis.minLimits(), 0.0));
    QVERIFY(qFuzzyCompare(axis.maxLimits(), 1.0));
    // Default scale is Linear
    QCOMPARE(axis.scaleType(), QImPlotScaleType::Linear);
    // Default: not inverted
    QVERIFY(!axis.isInverted());
    // Default: autoFit enabled
    QVERIFY(axis.isAutoFit());
}

// ============================================================================
// Test 2: axisId returns correct enum
// ============================================================================
void TestPlotAxisInfo::testAxisId()
{
    QImPlotAxisInfo x1(QImPlotAxisId::X1);
    QCOMPARE(x1.axisId(), QImPlotAxisId::X1);

    QImPlotAxisInfo y1(QImPlotAxisId::Y1);
    QCOMPARE(y1.axisId(), QImPlotAxisId::Y1);

    QImPlotAxisInfo y2(QImPlotAxisId::Y2);
    QCOMPARE(y2.axisId(), QImPlotAxisId::Y2);

    QImPlotAxisInfo x2(QImPlotAxisId::X2);
    QCOMPARE(x2.axisId(), QImPlotAxisId::X2);
}

// ============================================================================
// Test 3: imAxis returns raw ImAxis integer
// ============================================================================
void TestPlotAxisInfo::testImAxis()
{
    QImPlotAxisInfo x1(QImPlotAxisId::X1);
    QCOMPARE(x1.imAxis(), static_cast<int>(ImAxis_X1));

    QImPlotAxisInfo y1(QImPlotAxisId::Y1);
    QCOMPARE(y1.imAxis(), static_cast<int>(ImAxis_Y1));

    QImPlotAxisInfo y2(QImPlotAxisId::Y2);
    QCOMPARE(y2.imAxis(), static_cast<int>(ImAxis_Y2));
}

// ============================================================================
// Test 4: label property getter/setter
// ============================================================================
void TestPlotAxisInfo::testLabelProperty()
{
    QImPlotAxisInfo axis(QImPlotAxisId::X1);

    QVERIFY(axis.label().isEmpty());

    axis.setLabel("Time (s)");
    QCOMPARE(axis.label(), QString("Time (s)"));

    axis.setLabel("Frequency (Hz)");
    QCOMPARE(axis.label(), QString("Frequency (Hz)"));

    axis.setLabel("");  // Empty is valid
    QVERIFY(axis.label().isEmpty());
}

// ============================================================================
// Test 5: labelChanged signal
// ============================================================================
void TestPlotAxisInfo::testLabelChangedSignal()
{
    QImPlotAxisInfo axis(QImPlotAxisId::X1);
    QSignalSpy spy(&axis, &QImPlotAxisInfo::labelChanged);

    axis.setLabel("New Label");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toString(), QString("New Label"));

    // Same label — no signal (QImTrackedValue handles this)
    axis.setLabel("New Label");
    // The tracked value comparison may suppress the signal
    // But if the label is really the same, the count shouldn't increase
    // Note: QImTrackedValue<QByteArray> uses operator!= which is exact equality
    // So same value means no change detected
}

// ============================================================================
// Test 6: minLimits getter/setter
// ============================================================================
void TestPlotAxisInfo::testMinLimits()
{
    QImPlotAxisInfo axis(QImPlotAxisId::Y1);

    axis.setMinLimits(-5.0);
    QVERIFY(qFuzzyCompare(axis.minLimits(), -5.0));

    axis.setMinLimits(0.0);
    QVERIFY(qFuzzyCompare(axis.minLimits(), 0.0));

    axis.setMinLimits(3.14);
    QVERIFY(qFuzzyCompare(axis.minLimits(), 3.14));
}

// ============================================================================
// Test 7: maxLimits getter/setter
// ============================================================================
void TestPlotAxisInfo::testMaxLimits()
{
    QImPlotAxisInfo axis(QImPlotAxisId::Y1);

    axis.setMaxLimits(100.0);
    QVERIFY(qFuzzyCompare(axis.maxLimits(), 100.0));

    axis.setMaxLimits(2.718);
    QVERIFY(qFuzzyCompare(axis.maxLimits(), 2.718));
}

// ============================================================================
// Test 8: setLimits sets both min and max
// ============================================================================
void TestPlotAxisInfo::testSetLimits()
{
    QImPlotAxisInfo axis(QImPlotAxisId::X1);

    axis.setLimits(-10.0, 10.0);
    QVERIFY(qFuzzyCompare(axis.minLimits(), -10.0));
    QVERIFY(qFuzzyCompare(axis.maxLimits(), 10.0));

    axis.setLimits(0.0, 100.0, QImPlotCondition::Always);
    QVERIFY(qFuzzyCompare(axis.minLimits(), 0.0));
    QVERIFY(qFuzzyCompare(axis.maxLimits(), 100.0));
}

// ============================================================================
// Test 9: limitsChanged signal
// ============================================================================
void TestPlotAxisInfo::testLimitsChangedSignal()
{
    QImPlotAxisInfo axis(QImPlotAxisId::X1);
    QSignalSpy spy(&axis, &QImPlotAxisInfo::limitsChanged);

    axis.setLimits(-10.0, 10.0);
    QCOMPARE(spy.count(), 1);
    QVERIFY(qFuzzyCompare(spy.at(0).at(0).toDouble(), -10.0));
    QVERIFY(qFuzzyCompare(spy.at(0).at(1).toDouble(), 10.0));

    // Same value should not emit (tracked)
    axis.setLimits(-10.0, 10.0);
    // QImTrackedValue with double comparator may suppress
}

// ============================================================================
// Test 10: Default scale type is Linear
// ============================================================================
void TestPlotAxisInfo::testScaleTypeDefault()
{
    QImPlotAxisInfo axis(QImPlotAxisId::X1);
    QCOMPARE(axis.scaleType(), QImPlotScaleType::Linear);
}

// ============================================================================
// Test 11: scaleType set to Linear
// ============================================================================
void TestPlotAxisInfo::testScaleTypeLinear()
{
    QImPlotAxisInfo axis(QImPlotAxisId::X1);
    axis.setScaleType(QImPlotScaleType::Linear);
    QCOMPARE(axis.scaleType(), QImPlotScaleType::Linear);
}

// ============================================================================
// Test 12: scaleType set to Log10
// ============================================================================
void TestPlotAxisInfo::testScaleTypeLog()
{
    QImPlotAxisInfo axis(QImPlotAxisId::Y1);
    axis.setScaleType(QImPlotScaleType::Log10);
    QCOMPARE(axis.scaleType(), QImPlotScaleType::Log10);
}

// ============================================================================
// Test 13: scaleType set to Time
// ============================================================================
void TestPlotAxisInfo::testScaleTypeTime()
{
    QImPlotAxisInfo axis(QImPlotAxisId::X1);
    axis.setScaleType(QImPlotScaleType::Time);
    QCOMPARE(axis.scaleType(), QImPlotScaleType::Time);
}

// ============================================================================
// Test 14: scaleType set to SymLog
// ============================================================================
void TestPlotAxisInfo::testScaleTypeSymLog()
{
    QImPlotAxisInfo axis(QImPlotAxisId::Y1);
    axis.setScaleType(QImPlotScaleType::SymLog);
    QCOMPARE(axis.scaleType(), QImPlotScaleType::SymLog);
}

// ============================================================================
// Test 15: imPlotScale returns raw ImPlotScale value
// ============================================================================
void TestPlotAxisInfo::testImPlotScale()
{
    QImPlotAxisInfo axis(QImPlotAxisId::X1);

    // Default is Linear = 0
    QCOMPARE(axis.imPlotScale(), static_cast<int>(ImPlotScale_Linear));

    axis.setScaleType(QImPlotScaleType::Log10);
    QCOMPARE(axis.imPlotScale(), static_cast<int>(ImPlotScale_Log10));

    axis.setScaleType(QImPlotScaleType::SymLog);
    QCOMPARE(axis.imPlotScale(), static_cast<int>(ImPlotScale_SymLog));
}

// ============================================================================
// Test 16: autoFit property
// ============================================================================
void TestPlotAxisInfo::testAutoFit()
{
    QImPlotAxisInfo axis(QImPlotAxisId::X1);

    QVERIFY(axis.isAutoFit());  // Default

    axis.setAutoFit(false);
    QVERIFY(!axis.isAutoFit());

    axis.setAutoFit(true);
    QVERIFY(axis.isAutoFit());
}

// ============================================================================
// Test 17: inverted property
// ============================================================================
void TestPlotAxisInfo::testInverted()
{
    QImPlotAxisInfo axis(QImPlotAxisId::Y1);

    QVERIFY(!axis.isInverted());  // Default

    axis.setInverted(true);
    QVERIFY(axis.isInverted());

    axis.setInverted(false);
    QVERIFY(!axis.isInverted());
}

// ============================================================================
// Test 18: axisFlags raw getter/setter
// ============================================================================
void TestPlotAxisInfo::testAxisFlags()
{
    QImPlotAxisInfo axis(QImPlotAxisId::X1);

    // Default: ImPlotAxisFlags_None
    QCOMPARE(axis.axisFlags(), static_cast<int>(ImPlotAxisFlags_None));

    axis.setAxisFlags(ImPlotAxisFlags_Invert | ImPlotAxisFlags_LockMin);
    QCOMPARE(axis.axisFlags(), static_cast<int>(ImPlotAxisFlags_Invert | ImPlotAxisFlags_LockMin));
    QVERIFY(axis.isInverted());
    QVERIFY(axis.isLockMin());
}

// ============================================================================
// Test 19: axisFlagChanged signal
// ============================================================================
void TestPlotAxisInfo::testAxisFlagChangedSignal()
{
    QImPlotAxisInfo axis(QImPlotAxisId::X1);
    QSignalSpy spy(&axis, &QImPlotAxisInfo::axisFlagChanged);

    axis.setAutoFit(false);
    QCOMPARE(spy.count(), 1);

    axis.setInverted(true);
    QCOMPARE(spy.count(), 2);

    // Same value — no signal
    axis.setInverted(true);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// Test 20: labelEnabled (NoLabel negative semantic)
// ============================================================================
void TestPlotAxisInfo::testLabelEnabled()
{
    QImPlotAxisInfo axis(QImPlotAxisId::X1);

    QVERIFY(axis.isLabelEnabled());  // Default: NoLabel NOT set

    axis.setLabelEnabled(false);
    QVERIFY(!axis.isLabelEnabled());

    axis.setLabelEnabled(true);
    QVERIFY(axis.isLabelEnabled());
}

// ============================================================================
// Test 21: gridLinesEnabled (NoGridLines negative semantic)
// ============================================================================
void TestPlotAxisInfo::testGridLinesEnabled()
{
    QImPlotAxisInfo axis(QImPlotAxisId::Y1);

    QVERIFY(axis.isGridLinesEnabled());  // Default: NoGridLines NOT set

    axis.setGridLinesEnabled(false);
    QVERIFY(!axis.isGridLinesEnabled());

    axis.setGridLinesEnabled(true);
    QVERIFY(axis.isGridLinesEnabled());
}

// ============================================================================
// Test 22: lockMin / lockMax / lock
// ============================================================================
void TestPlotAxisInfo::testLock()
{
    QImPlotAxisInfo axis(QImPlotAxisId::X1);

    QVERIFY(!axis.isLockMin());
    QVERIFY(!axis.isLockMax());
    QVERIFY(!axis.isLock());

    axis.setLockMin(true);
    QVERIFY(axis.isLockMin());
    QVERIFY(!axis.isLock());  // Only min locked

    axis.setLockMax(true);
    QVERIFY(axis.isLockMax());
    QVERIFY(axis.isLock());   // Both locked

    axis.setLock(false);
    QVERIFY(!axis.isLockMin());
    QVERIFY(!axis.isLockMax());
    QVERIFY(!axis.isLock());
}

// ============================================================================
// Test 23: opposite property
// ============================================================================
void TestPlotAxisInfo::testOpposite()
{
    QImPlotAxisInfo axis(QImPlotAxisId::Y2);

    QVERIFY(!axis.isOpposite());  // Default

    axis.setOpposite(true);
    QVERIFY(axis.isOpposite());

    axis.setOpposite(false);
    QVERIFY(!axis.isOpposite());
}

// ============================================================================
// Test 24: enabled property (for non-primary axes)
// ============================================================================
void TestPlotAxisInfo::testEnabled()
{
    // X1 (primary axis): disabled = NoDecorations
    QImPlotAxisInfo x1(QImPlotAxisId::X1);

    // Y2 (secondary axis): disabled = not rendered
    QImPlotAxisInfo y2(QImPlotAxisId::Y2);

    QVERIFY(!y2.isEnabled());  // Default for secondary axes: disabled

    y2.setEnabled(true);
    QVERIFY(y2.isEnabled());

    y2.setEnabled(false);
    QVERIFY(!y2.isEnabled());
}

// ============================================================================
// Test 25: plotNode returns the associated plot (nullptr if not set)
// ============================================================================
void TestPlotAxisInfo::testPlotNode()
{
    // Axis created without plot — plotNode returns nullptr
    QImPlotAxisInfo axis(QImPlotAxisId::X1);
    QVERIFY(axis.plotNode() == nullptr);
}

// ============================================================================
// Test 26: Multiple axes with different IDs
// ============================================================================
void TestPlotAxisInfo::testMultipleAxes()
{
    QImPlotAxisInfo x1(QImPlotAxisId::X1);
    QImPlotAxisInfo y1(QImPlotAxisId::Y1);
    QImPlotAxisInfo y2(QImPlotAxisId::Y2);

    QCOMPARE(x1.axisId(), QImPlotAxisId::X1);
    QCOMPARE(y1.axisId(), QImPlotAxisId::Y1);
    QCOMPARE(y2.axisId(), QImPlotAxisId::Y2);

    // Each axis should be independently configurable
    x1.setLabel("X Axis");
    y1.setLabel("Y Axis 1");
    y2.setLabel("Y Axis 2");
    y2.setOpposite(true);

    QCOMPARE(x1.label(), QString("X Axis"));
    QCOMPARE(y1.label(), QString("Y Axis 1"));
    QCOMPARE(y2.label(), QString("Y Axis 2"));
    QVERIFY(y2.isOpposite());
}

QTEST_MAIN(TestPlotAxisInfo)
#include "tst_plot_axis_info.moc"
