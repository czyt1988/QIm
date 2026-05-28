#include <QObject>
#include <QtTest>
#include <QSignalSpy>
#include <memory>

#include "QImPlotScatterItemNode.h"
#include "../core/TestHelpers.h"

using namespace QIM;

class TestQImPlotScatterItemNode : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    // Construction
    void testDefaultConstruction();

    // markerSize
    void testMarkerSizeGetterSetter();
    void testMarkerSizeChangedSignal();

    // markerShape
    void testMarkerShapeGetterSetter();
    void testMarkerShapeChangedSignal();

    // markerFill
    void testMarkerFillGetterSetter();
    void testMarkerFillChangedSignal();

    // color
    void testColorGetterSetter();
    void testColorChangedSignal();

    // clippingEnabled
    void testClippingEnabledGetterSetter();
    void testClippingEnabledChangedSignal();

    // setData() with random walk
    void testSetDataRandomWalk();
};

void TestQImPlotScatterItemNode::initTestCase()
{
}

void TestQImPlotScatterItemNode::cleanupTestCase()
{
}

// ============================================================================
// Construction
// ============================================================================
void TestQImPlotScatterItemNode::testDefaultConstruction()
{
    auto node = std::make_unique<QImPlotScatterItemNode>();
    QVERIFY(node != nullptr);
    QVERIFY(node->isVisible());
    QVERIFY(node->data() == nullptr);
}

// ============================================================================
// markerSize — getter/setter
// ============================================================================
void TestQImPlotScatterItemNode::testMarkerSizeGetterSetter()
{
    auto node = std::make_unique<QImPlotScatterItemNode>();

    // Default value check
    float defaultSize = node->markerSize();

    // Set new value
    node->setMarkerSize(12.0f);
    QCOMPARE(node->markerSize(), 12.0f);

    // Redundant set (same value) — getter still returns same
    node->setMarkerSize(12.0f);
    QCOMPARE(node->markerSize(), 12.0f);

    // Set another value
    node->setMarkerSize(3.0f);
    QCOMPARE(node->markerSize(), 3.0f);
}

// ============================================================================
// markerSize — signal deduplication
// ============================================================================
void TestQImPlotScatterItemNode::testMarkerSizeChangedSignal()
{
    auto node = std::make_unique<QImPlotScatterItemNode>();
    QSignalSpy spy(node.get(), &QImPlotScatterItemNode::markerSizeChanged);

    node->setMarkerSize(8.0f);
    QCOMPARE(spy.count(), 1);

    // Same value — no duplicate signal
    node->setMarkerSize(8.0f);
    QCOMPARE(spy.count(), 1);

    // Different value — emitted again
    node->setMarkerSize(10.0f);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// markerShape — getter/setter
// ============================================================================
void TestQImPlotScatterItemNode::testMarkerShapeGetterSetter()
{
    auto node = std::make_unique<QImPlotScatterItemNode>();

    int defaultShape = node->markerShape();

    // Set new value
    node->setMarkerShape(2);
    QCOMPARE(node->markerShape(), 2);

    // Redundant set
    node->setMarkerShape(2);
    QCOMPARE(node->markerShape(), 2);

    // Another value
    node->setMarkerShape(5);
    QCOMPARE(node->markerShape(), 5);
}

// ============================================================================
// markerShape — signal deduplication
// ============================================================================
void TestQImPlotScatterItemNode::testMarkerShapeChangedSignal()
{
    auto node = std::make_unique<QImPlotScatterItemNode>();
    QSignalSpy spy(node.get(), &QImPlotScatterItemNode::markerShapeChanged);

    node->setMarkerShape(3);
    QCOMPARE(spy.count(), 1);

    // Same value — no duplicate signal
    node->setMarkerShape(3);
    QCOMPARE(spy.count(), 1);

    // Different value
    node->setMarkerShape(7);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// markerFill — getter/setter
// ============================================================================
void TestQImPlotScatterItemNode::testMarkerFillGetterSetter()
{
    auto node = std::make_unique<QImPlotScatterItemNode>();

    // Default is true (filled)
    QCOMPARE(node->isMarkerFill(), true);

    // Set to false
    node->setMarkerFill(false);
    QCOMPARE(node->isMarkerFill(), false);

    // Redundant set
    node->setMarkerFill(false);
    QCOMPARE(node->isMarkerFill(), false);

    // Set back to true
    node->setMarkerFill(true);
    QCOMPARE(node->isMarkerFill(), true);
}

// ============================================================================
// markerFill — signal deduplication
// ============================================================================
void TestQImPlotScatterItemNode::testMarkerFillChangedSignal()
{
    auto node = std::make_unique<QImPlotScatterItemNode>();
    QSignalSpy spy(node.get(), &QImPlotScatterItemNode::markerFillChanged);

    node->setMarkerFill(false);
    QCOMPARE(spy.count(), 1);

    // Same value — no duplicate
    node->setMarkerFill(false);
    QCOMPARE(spy.count(), 1);

    // Different value
    node->setMarkerFill(true);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// color — getter/setter
// ============================================================================
void TestQImPlotScatterItemNode::testColorGetterSetter()
{
    auto node = std::make_unique<QImPlotScatterItemNode>();

    // Default color is invalid (QColor())
    QColor defaultColor = node->color();

    // Set a specific color
    node->setColor(QColor(255, 0, 0));
    QCOMPARE(node->color(), QColor(255, 0, 0));

    // Redundant set
    node->setColor(QColor(255, 0, 0));
    QCOMPARE(node->color(), QColor(255, 0, 0));

    // Different color
    node->setColor(QColor(0, 114, 189));
    QCOMPARE(node->color(), QColor(0, 114, 189));
}

// ============================================================================
// color — signal deduplication
// ============================================================================
void TestQImPlotScatterItemNode::testColorChangedSignal()
{
    auto node = std::make_unique<QImPlotScatterItemNode>();
    QSignalSpy spy(node.get(), &QImPlotScatterItemNode::colorChanged);

    node->setColor(QColor(100, 200, 50));
    QCOMPARE(spy.count(), 1);

    // Same value — no duplicate
    node->setColor(QColor(100, 200, 50));
    QCOMPARE(spy.count(), 1);

    // Different value
    node->setColor(QColor(0, 0, 0));
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// clippingEnabled — getter/setter
// ============================================================================
void TestQImPlotScatterItemNode::testClippingEnabledGetterSetter()
{
    auto node = std::make_unique<QImPlotScatterItemNode>();

    // Default should be true (clipping enabled = !NoClip)
    QCOMPARE(node->isClippingEnabled(), true);

    // Disable clipping
    node->setClippingEnabled(false);
    QCOMPARE(node->isClippingEnabled(), false);

    // Redundant set
    node->setClippingEnabled(false);
    QCOMPARE(node->isClippingEnabled(), false);

    // Re-enable
    node->setClippingEnabled(true);
    QCOMPARE(node->isClippingEnabled(), true);
}

// ============================================================================
// clippingEnabled — signal deduplication
// ============================================================================
void TestQImPlotScatterItemNode::testClippingEnabledChangedSignal()
{
    auto node = std::make_unique<QImPlotScatterItemNode>();
    QSignalSpy spy(node.get(), &QImPlotScatterItemNode::scatterFlagChanged);

    node->setClippingEnabled(false);
    QCOMPARE(spy.count(), 1);

    // Same value — no duplicate
    node->setClippingEnabled(false);
    QCOMPARE(spy.count(), 1);

    // Different value
    node->setClippingEnabled(true);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// setData() — random walk
// ============================================================================
void TestQImPlotScatterItemNode::testSetDataRandomWalk()
{
    auto node = std::make_unique<QImPlotScatterItemNode>();
    QVERIFY(node->data() == nullptr);

    // Generate random walk data
    const int n = 100;
    std::vector<double> xs = TestHelpers::makeLinearXs(n);
    std::vector<double> ys = TestHelpers::makeRandomWalk(n, 0.0);

    // Set data
    node->setData(xs, ys);

    // Verify data is set
    QVERIFY(node->data() != nullptr);
}

QTEST_GUILESS_MAIN(TestQImPlotScatterItemNode)
#include "tst_qimplotscatteritemnode.moc"
