#include <QObject>
#include <QtTest>
#include <memory>

#include "QImPlotBarsItemNode.h"
#include "../core/TestHelpers.h"

using namespace QIM;

class TestQImPlotBarsItemNode : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    // Construction
    void testDefaultConstruction();

    // barWidth property
    void testBarWidthGetterSetter();
    void testBarWidthChangedSignal();
    void testBarWidthSignalDedu();

    // horizontal property
    void testHorizontalGetterSetter();
    void testOrientationChangedSignal();
    void testOrientationSignalDedu();

    // barsFlags property
    void testBarsFlagsGetterSetter();
    void testBarsFlagChangedSignal();

    // setData
    void testSetData();

private:
    // Helper: create node
    std::unique_ptr<QImPlotBarsItemNode> makeNode()
    {
        return std::make_unique<QImPlotBarsItemNode>();
    }
};

void TestQImPlotBarsItemNode::initTestCase()
{
}

void TestQImPlotBarsItemNode::cleanupTestCase()
{
}

// ============================================================================
// Construction
// ============================================================================
void TestQImPlotBarsItemNode::testDefaultConstruction()
{
    auto node = makeNode();
    QVERIFY(node != nullptr);
    QVERIFY(node->isVisible());
    QCOMPARE(node->type(), QImPlotBarsItemNode::Type);
}

// ============================================================================
// barWidth — getter/setter
// ============================================================================
void TestQImPlotBarsItemNode::testBarWidthGetterSetter()
{
    auto node = makeNode();

    // Default value
    QVERIFY(node->barWidth() > 0.0);

    // Set new value
    node->setBarWidth(1.5);
    QCOMPARE(node->barWidth(), 1.5);

    // Redundant set (same value) — getter still returns same
    node->setBarWidth(1.5);
    QCOMPARE(node->barWidth(), 1.5);
}

// ============================================================================
// barWidth — signal emission
// ============================================================================
void TestQImPlotBarsItemNode::testBarWidthChangedSignal()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotBarsItemNode::barWidthChanged);

    node->setBarWidth(2.0);
    QCOMPARE(spy.count(), 1);

    node->setBarWidth(3.0);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// barWidth — signal deduplication
// ============================================================================
void TestQImPlotBarsItemNode::testBarWidthSignalDedu()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotBarsItemNode::barWidthChanged);

    node->setBarWidth(1.5);
    QCOMPARE(spy.count(), 1);

    // Same value — no duplicate signal
    node->setBarWidth(1.5);
    QCOMPARE(spy.count(), 1);
}

// ============================================================================
// horizontal — getter/setter
// ============================================================================
void TestQImPlotBarsItemNode::testHorizontalGetterSetter()
{
    auto node = makeNode();

    // Default: vertical
    QVERIFY(!node->isHorizontal());

    // Set horizontal
    node->setHorizontal(true);
    QVERIFY(node->isHorizontal());

    // Set back
    node->setHorizontal(false);
    QVERIFY(!node->isHorizontal());
}

// ============================================================================
// horizontal — signal emission
// ============================================================================
void TestQImPlotBarsItemNode::testOrientationChangedSignal()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotBarsItemNode::orientationChanged);

    node->setHorizontal(true);
    QCOMPARE(spy.count(), 1);

    node->setHorizontal(false);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// horizontal — signal deduplication
// ============================================================================
void TestQImPlotBarsItemNode::testOrientationSignalDedu()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotBarsItemNode::orientationChanged);

    node->setHorizontal(true);
    QCOMPARE(spy.count(), 1);

    // Same value — no duplicate signal
    node->setHorizontal(true);
    QCOMPARE(spy.count(), 1);
}

// ============================================================================
// barsFlags — getter/setter
// ============================================================================
void TestQImPlotBarsItemNode::testBarsFlagsGetterSetter()
{
    auto node = makeNode();

    int defaultFlags = node->barsFlags();
    Q_UNUSED(defaultFlags);

    // Set arbitrary flags
    node->setBarsFlags(0x01);
    QCOMPARE(node->barsFlags(), 0x01);

    // Reset
    node->setBarsFlags(0);
    QCOMPARE(node->barsFlags(), 0);
}

// ============================================================================
// barsFlags — signal emission
// ============================================================================
void TestQImPlotBarsItemNode::testBarsFlagChangedSignal()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotBarsItemNode::barsFlagChanged);

    node->setBarsFlags(0x01);
    QCOMPARE(spy.count(), 1);
}

// ============================================================================
// setData — verify data assignment
// ============================================================================
void TestQImPlotBarsItemNode::testSetData()
{
    auto node = makeNode();

    auto xs = TestHelpers::makeLinearXs(10);
    auto ys = TestHelpers::makeLinearYs(10);

    node->setData(xs, ys);
    QVERIFY(node->data() != nullptr);
}

QTEST_GUILESS_MAIN(TestQImPlotBarsItemNode)
#include "tst_qimplotbarsitemnode.moc"
