#include <QObject>
#include <QtTest>
#include <memory>

#include "implot3d.h"
#include "QImPlot3DNode.h"
#include "QImPlot3DAxisInfo.h"
#include "QImPlot3DStyleNode.h"

using namespace QIM;

class TestPlot3DNode : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    // Construction
    void testDefaultConstruction();
    void testTitleConstructor();

    // Title property
    void testTitleGetterSetter();
    void testTitleChangedSignal();

    // Size property
    void testSizeGetterSetter();
    void testSizeChangedSignal();

    // AutoSize property
    void testAutoSizeGetterSetter();
    void testAutoSizeChangedSignal();

    // ImPlot3DFlags - negative to positive semantics
    void testTitleEnabledFlag();
    void testLegendEnabledFlag();
    void testMouseTextEnabledFlag();
    void testClipEnabledFlag();
    void testMenusEnabledFlag();
    void testRotateEnabledFlag();
    void testPanEnabledFlag();
    void testZoomEnabledFlag();
    void testInputsEnabledFlag();

    // ImPlot3DFlags - direct mapping
    void testEqualFlag();

    // ImPlot3DFlags - combined (CanvasOnly)
    void testCanvasEnabledFlag();

    // Raw flag access
    void testImPlot3DFlagsRaw();
    void testSetImPlot3DFlagsRaw();

    // plot3DFlagChanged signal
    void testPlot3DFlagChangedSignal();

    // Axis info accessors
    void testAxisInfoAccessors();

    // Legend configuration
    void testLegendLocation();
    void testLegendFlags();
    void testLegendConfigChangedSignal();

    // Style node
    void testStyleNode();

    // Box rotation
    void testSetBoxRotation();
    void testSetBoxRotationQuaternion();
    void testSetBoxInitialRotation();
    void testSetBoxInitialRotationQuaternion();

    // Box scale
    void testSetBoxScale();

    // Setup axes convenience
    void testSetupAxesConvenience();
    void testSetupAxesLimitsConvenience();

    // Colormap push/pop
    void testPushPopColormap();

};

void TestPlot3DNode::initTestCase()
{
}

void TestPlot3DNode::cleanupTestCase()
{
}

// ============================================================================
// Construction
// ============================================================================
void TestPlot3DNode::testDefaultConstruction()
{
    QImPlot3DNode plot3d;
    QCOMPARE(plot3d.title(), QString("##Plot3D"));
    QVERIFY(plot3d.isAutoSize());
    QCOMPARE(plot3d.childNodeCount(), 0);
}

void TestPlot3DNode::testTitleConstructor()
{
    QImPlot3DNode plot3d("My 3D Plot");
    QCOMPARE(plot3d.title(), QString("My 3D Plot"));
}

// ============================================================================
// Title
// ============================================================================
void TestPlot3DNode::testTitleGetterSetter()
{
    QImPlot3DNode plot3d;
    plot3d.setTitle("3D Test");
    QCOMPARE(plot3d.title(), QString("3D Test"));

    // Same value
    plot3d.setTitle("3D Test");
    QCOMPARE(plot3d.title(), QString("3D Test"));
}

void TestPlot3DNode::testTitleChangedSignal()
{
    QImPlot3DNode plot3d;
    QSignalSpy spy(&plot3d, &QImPlot3DNode::titleChanged);

    plot3d.setTitle("New 3D Title");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toString(), QString("New 3D Title"));

    // Same value — no signal
    plot3d.setTitle("New 3D Title");
    QCOMPARE(spy.count(), 1);
}

// ============================================================================
// Size
// ============================================================================
void TestPlot3DNode::testSizeGetterSetter()
{
    QImPlot3DNode plot3d;

    // Default: auto-size (-1, -1)
    QVERIFY(plot3d.size().width() < 0);

    plot3d.setSize(QSizeF(800, 600));
    QCOMPARE(plot3d.size(), QSizeF(800, 600));
}

void TestPlot3DNode::testSizeChangedSignal()
{
    QImPlot3DNode plot3d;
    QSignalSpy spy(&plot3d, &QImPlot3DNode::sizeChanged);

    plot3d.setSize(QSizeF(400, 300));
    QCOMPARE(spy.count(), 1);

    plot3d.setSize(QSizeF(400, 300));
    QCOMPARE(spy.count(), 1);
}

// ============================================================================
// AutoSize
// ============================================================================
void TestPlot3DNode::testAutoSizeGetterSetter()
{
    QImPlot3DNode plot3d;
    QVERIFY(plot3d.isAutoSize());

    plot3d.setAutoSize(false);
    QVERIFY(!plot3d.isAutoSize());

    plot3d.setAutoSize(true);
    QVERIFY(plot3d.isAutoSize());
}

void TestPlot3DNode::testAutoSizeChangedSignal()
{
    QImPlot3DNode plot3d;
    QSignalSpy spy(&plot3d, &QImPlot3DNode::autoSizeChanged);

    plot3d.setAutoSize(false);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toBool(), false);

    plot3d.setAutoSize(false);
    QCOMPARE(spy.count(), 1);

    plot3d.setAutoSize(true);
    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.at(1).at(0).toBool(), true);
}

// ============================================================================
// ImPlot3DFlags - negative to positive
// ============================================================================
void TestPlot3DNode::testTitleEnabledFlag()
{
    QImPlot3DNode plot3d;
    QVERIFY(plot3d.isTitleEnabled());

    plot3d.setTitleEnabled(false);
    QVERIFY(!plot3d.isTitleEnabled());

    plot3d.setTitleEnabled(true);
    QVERIFY(plot3d.isTitleEnabled());
}

void TestPlot3DNode::testLegendEnabledFlag()
{
    QImPlot3DNode plot3d;
    QVERIFY(plot3d.isLegendEnabled());

    plot3d.setLegendEnabled(false);
    QVERIFY(!plot3d.isLegendEnabled());

    plot3d.setLegendEnabled(true);
    QVERIFY(plot3d.isLegendEnabled());
}

void TestPlot3DNode::testMouseTextEnabledFlag()
{
    QImPlot3DNode plot3d;
    QVERIFY(plot3d.isMouseTextEnabled());

    plot3d.setMouseTextEnabled(false);
    QVERIFY(!plot3d.isMouseTextEnabled());

    plot3d.setMouseTextEnabled(true);
    QVERIFY(plot3d.isMouseTextEnabled());
}

void TestPlot3DNode::testClipEnabledFlag()
{
    QImPlot3DNode plot3d;
    QVERIFY(plot3d.isClipEnabled());

    plot3d.setClipEnabled(false);
    QVERIFY(!plot3d.isClipEnabled());

    plot3d.setClipEnabled(true);
    QVERIFY(plot3d.isClipEnabled());
}

void TestPlot3DNode::testMenusEnabledFlag()
{
    QImPlot3DNode plot3d;
    QVERIFY(plot3d.isMenusEnabled());

    plot3d.setMenusEnabled(false);
    QVERIFY(!plot3d.isMenusEnabled());

    plot3d.setMenusEnabled(true);
    QVERIFY(plot3d.isMenusEnabled());
}

void TestPlot3DNode::testRotateEnabledFlag()
{
    QImPlot3DNode plot3d;
    QVERIFY(plot3d.isRotateEnabled());

    plot3d.setRotateEnabled(false);
    QVERIFY(!plot3d.isRotateEnabled());

    plot3d.setRotateEnabled(true);
    QVERIFY(plot3d.isRotateEnabled());
}

void TestPlot3DNode::testPanEnabledFlag()
{
    QImPlot3DNode plot3d;
    QVERIFY(plot3d.isPanEnabled());

    plot3d.setPanEnabled(false);
    QVERIFY(!plot3d.isPanEnabled());

    plot3d.setPanEnabled(true);
    QVERIFY(plot3d.isPanEnabled());
}

void TestPlot3DNode::testZoomEnabledFlag()
{
    QImPlot3DNode plot3d;
    QVERIFY(plot3d.isZoomEnabled());

    plot3d.setZoomEnabled(false);
    QVERIFY(!plot3d.isZoomEnabled());

    plot3d.setZoomEnabled(true);
    QVERIFY(plot3d.isZoomEnabled());
}

void TestPlot3DNode::testInputsEnabledFlag()
{
    QImPlot3DNode plot3d;
    QVERIFY(plot3d.isInputsEnabled());

    plot3d.setInputsEnabled(false);
    QVERIFY(!plot3d.isInputsEnabled());

    plot3d.setInputsEnabled(true);
    QVERIFY(plot3d.isInputsEnabled());
}

// ============================================================================
// ImPlot3DFlags - direct mapping
// ============================================================================
void TestPlot3DNode::testEqualFlag()
{
    QImPlot3DNode plot3d;
    QVERIFY(!plot3d.isEqual());

    plot3d.setEqual(true);
    QVERIFY(plot3d.isEqual());

    plot3d.setEqual(false);
    QVERIFY(!plot3d.isEqual());
}

// ============================================================================
// ImPlot3DFlags - combined
// ============================================================================
void TestPlot3DNode::testCanvasEnabledFlag()
{
    QImPlot3DNode plot3d;
    QVERIFY(plot3d.isCanvasEnabled());

    plot3d.setCanvasEnabled(false);
    QVERIFY(!plot3d.isCanvasEnabled());

    plot3d.setCanvasEnabled(true);
    QVERIFY(plot3d.isCanvasEnabled());
}

// ============================================================================
// Raw flag access
// ============================================================================
void TestPlot3DNode::testImPlot3DFlagsRaw()
{
    QImPlot3DNode plot3d;
    QCOMPARE(plot3d.imPlot3DFlags(), 0);  // ImPlot3DFlags_None
}

void TestPlot3DNode::testSetImPlot3DFlagsRaw()
{
    QImPlot3DNode plot3d;
    QSignalSpy spy(&plot3d, &QImPlot3DNode::plot3DFlagChanged);

    // Set some flags
    plot3d.setImPlot3DFlags(ImPlot3DFlags_NoTitle);
    QVERIFY(!plot3d.isTitleEnabled());
    QCOMPARE(spy.count(), 1);

    // Same value — no signal
    plot3d.setImPlot3DFlags(ImPlot3DFlags_NoTitle);
    QCOMPARE(spy.count(), 1);
}

// ============================================================================
// plot3DFlagChanged signal
// ============================================================================
void TestPlot3DNode::testPlot3DFlagChangedSignal()
{
    QImPlot3DNode plot3d;
    QSignalSpy spy(&plot3d, &QImPlot3DNode::plot3DFlagChanged);

    plot3d.setEqual(true);
    QCOMPARE(spy.count(), 1);

    plot3d.setTitleEnabled(false);
    QCOMPARE(spy.count(), 2);

    // CanvasEnabled sets multiple flags → one signal
    plot3d.setCanvasEnabled(false);
    QCOMPARE(spy.count(), 3);
}

// ============================================================================
// Axis info accessors
// ============================================================================
void TestPlot3DNode::testAxisInfoAccessors()
{
    QImPlot3DNode plot3d;

    QVERIFY(plot3d.xAxis() != nullptr);
    QVERIFY(plot3d.yAxis() != nullptr);
    QVERIFY(plot3d.zAxis() != nullptr);

    QCOMPARE(plot3d.axisInfo(QImPlot3DAxisId::X1), plot3d.xAxis());
    QCOMPARE(plot3d.axisInfo(QImPlot3DAxisId::Y1), plot3d.yAxis());
    QCOMPARE(plot3d.axisInfo(QImPlot3DAxisId::Z1), plot3d.zAxis());
}

// ============================================================================
// Legend configuration
// ============================================================================
void TestPlot3DNode::testLegendLocation()
{
    QImPlot3DNode plot3d;

    // Default: NorthWest
    QCOMPARE(plot3d.legendLocation(), QImPlot3DLocation::NorthWest);

    plot3d.setLegendLocation(QImPlot3DLocation::SouthEast);
    QCOMPARE(plot3d.legendLocation(), QImPlot3DLocation::SouthEast);
}

void TestPlot3DNode::testLegendFlags()
{
    QImPlot3DNode plot3d;
    QCOMPARE(plot3d.legendFlags(), 0);

    plot3d.setLegendFlags(1);
    QCOMPARE(plot3d.legendFlags(), 1);
}

void TestPlot3DNode::testLegendConfigChangedSignal()
{
    QImPlot3DNode plot3d;
    QSignalSpy spy(&plot3d, &QImPlot3DNode::legendConfigChanged);

    plot3d.setLegendLocation(QImPlot3DLocation::South);
    QCOMPARE(spy.count(), 1);

    plot3d.setLegendFlags(2);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// Style node
// ============================================================================
void TestPlot3DNode::testStyleNode()
{
    QImPlot3DNode plot3d;
    QVERIFY(plot3d.styleNode() != nullptr);
}

// ============================================================================
// Box rotation
// ============================================================================
void TestPlot3DNode::testSetBoxRotation()
{
    QImPlot3DNode plot3d;
    // Verify no crash
    plot3d.setBoxRotation(30.0, 60.0);
    QVERIFY(true);

    plot3d.setBoxRotation(45.0, 90.0, true, QImPlot3DCondition::Always);
    QVERIFY(true);
}

void TestPlot3DNode::testSetBoxRotationQuaternion()
{
    QImPlot3DNode plot3d;
    plot3d.setBoxRotation(QQuaternion(1, 0, 0, 0));
    QVERIFY(true);
}

void TestPlot3DNode::testSetBoxInitialRotation()
{
    QImPlot3DNode plot3d;
    plot3d.setBoxInitialRotation(20.0, 40.0);
    QVERIFY(true);
}

void TestPlot3DNode::testSetBoxInitialRotationQuaternion()
{
    QImPlot3DNode plot3d;
    plot3d.setBoxInitialRotation(QQuaternion(1, 0, 0, 0));
    QVERIFY(true);
}

// ============================================================================
// Box scale
// ============================================================================
void TestPlot3DNode::testSetBoxScale()
{
    QImPlot3DNode plot3d;
    plot3d.setBoxScale(1.0, 1.5, 2.0);
    QVERIFY(true);
}

// ============================================================================
// Setup axes convenience
// ============================================================================
void TestPlot3DNode::testSetupAxesConvenience()
{
    QImPlot3DNode plot3d;
    plot3d.setupAxes("X Label", "Y Label", "Z Label");
    QVERIFY(true);
}

void TestPlot3DNode::testSetupAxesLimitsConvenience()
{
    QImPlot3DNode plot3d;
    plot3d.setupAxesLimits(0.0, 10.0, 0.0, 20.0, 0.0, 30.0);
    QVERIFY(true);
}

// ============================================================================
// Colormap push/pop
// ============================================================================
void TestPlot3DNode::testPushPopColormap()
{
    QImPlot3DNode plot3d;
    // Verify pushColormap doesn't crash
    plot3d.pushColormap(QImPlot3DColormap::Viridis);
    plot3d.pushColormap("Hot");
    plot3d.popColormap(2);  // Pop both
    // popColormap with count > pushes should be safe
    plot3d.popColormap();
    QVERIFY(true);
}

QTEST_GUILESS_MAIN(TestPlot3DNode)
#include "tst_plot3d_node.moc"
