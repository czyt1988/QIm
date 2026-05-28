#include <QObject>
#include <QtTest>
#include <memory>

#include "QImPlotLegendNode.h"
#include "../core/TestHelpers.h"

using namespace QIM;

class TestQImPlotLegendNode : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    // Construction
    void testDefaultConstruction();

    // location property
    void testLocationGetterSetter();
    void testLocationChangedSignal();
    void testLocationSignalDedu();

    // orientation property
    void testOrientationGetterSetter();
    void testOrientationChangedSignal();
    void testOrientationSignalDedu();

    // outside property
    void testOutsideGetterSetter();
    void testOutsideChangedSignal();
    void testOutsideSignalDedu();

    // buttonEnabled property
    void testButtonEnabledGetterSetter();
    void testButtonEnabledChangedSignal();
    void testButtonEnabledSignalDedu();

    // highlightItemEnabled property
    void testHighlightItemEnabledGetterSetter();
    void testHighlightItemEnabledChangedSignal();
    void testHighlightItemEnabledSignalDedu();

    // highlightAxisEnabled property
    void testHighlightAxisEnabledGetterSetter();
    void testHighlightAxisEnabledChangedSignal();
    void testHighlightAxisEnabledSignalDedu();

    // sort property
    void testSortGetterSetter();
    void testSortChangedSignal();
    void testSortSignalDedu();

    // reverse property
    void testReverseGetterSetter();
    void testReverseChangedSignal();
    void testReverseSignalDedu();

private:
    // Helper: create node
    std::unique_ptr<QImPlotLegendNode> makeNode()
    {
        return std::make_unique<QImPlotLegendNode>();
    }
};

void TestQImPlotLegendNode::initTestCase()
{
}

void TestQImPlotLegendNode::cleanupTestCase()
{
}

// ============================================================================
// Construction
// ============================================================================
void TestQImPlotLegendNode::testDefaultConstruction()
{
    auto node = makeNode();
    QVERIFY(node != nullptr);
    QVERIFY(node->isVisible());
}

// ============================================================================
// location — getter/setter
// ============================================================================
void TestQImPlotLegendNode::testLocationGetterSetter()
{
    auto node = makeNode();

    // Set new value
    node->setLocation(QImPlotLegendLocation::North);
    QCOMPARE(node->location(), QImPlotLegendLocation::North);

    // Change to another value
    node->setLocation(QImPlotLegendLocation::SouthEast);
    QCOMPARE(node->location(), QImPlotLegendLocation::SouthEast);

    // Redundant set (same value) — getter still returns same
    node->setLocation(QImPlotLegendLocation::SouthEast);
    QCOMPARE(node->location(), QImPlotLegendLocation::SouthEast);
}

// ============================================================================
// location — signal emission
// ============================================================================
void TestQImPlotLegendNode::testLocationChangedSignal()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotLegendNode::locationChanged);

    node->setLocation(QImPlotLegendLocation::West);
    QCOMPARE(spy.count(), 1);

    node->setLocation(QImPlotLegendLocation::East);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// location — signal deduplication
// ============================================================================
void TestQImPlotLegendNode::testLocationSignalDedu()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotLegendNode::locationChanged);

    node->setLocation(QImPlotLegendLocation::Center);
    QCOMPARE(spy.count(), 1);

    // Same value — no duplicate signal
    node->setLocation(QImPlotLegendLocation::Center);
    QCOMPARE(spy.count(), 1);
}

// ============================================================================
// orientation — getter/setter
// ============================================================================
void TestQImPlotLegendNode::testOrientationGetterSetter()
{
    auto node = makeNode();

    // Set horizontal
    node->setOrientation(Qt::Horizontal);
    QCOMPARE(node->orientation(), Qt::Horizontal);

    // Set vertical
    node->setOrientation(Qt::Vertical);
    QCOMPARE(node->orientation(), Qt::Vertical);
}

// ============================================================================
// orientation — signal emission
// ============================================================================
void TestQImPlotLegendNode::testOrientationChangedSignal()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotLegendNode::legendFlagChanged);

    node->setOrientation(Qt::Horizontal);
    QCOMPARE(spy.count(), 1);

    node->setOrientation(Qt::Vertical);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// orientation — signal deduplication
// ============================================================================
void TestQImPlotLegendNode::testOrientationSignalDedu()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotLegendNode::legendFlagChanged);

    node->setOrientation(Qt::Horizontal);
    QCOMPARE(spy.count(), 1);

    // Same value — no duplicate signal
    node->setOrientation(Qt::Horizontal);
    QCOMPARE(spy.count(), 1);
}

// ============================================================================
// outside — getter/setter
// ============================================================================
void TestQImPlotLegendNode::testOutsideGetterSetter()
{
    auto node = makeNode();

    // Set outside
    node->setOutside(true);
    QVERIFY(node->isOutside());

    // Set back
    node->setOutside(false);
    QVERIFY(!node->isOutside());
}

// ============================================================================
// outside — signal emission
// ============================================================================
void TestQImPlotLegendNode::testOutsideChangedSignal()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotLegendNode::legendFlagChanged);

    node->setOutside(true);
    QCOMPARE(spy.count(), 1);

    node->setOutside(false);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// outside — signal deduplication
// ============================================================================
void TestQImPlotLegendNode::testOutsideSignalDedu()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotLegendNode::legendFlagChanged);

    node->setOutside(true);
    QCOMPARE(spy.count(), 1);

    // Same value — no duplicate signal
    node->setOutside(true);
    QCOMPARE(spy.count(), 1);
}

// ============================================================================
// buttonEnabled — getter/setter
// ============================================================================
void TestQImPlotLegendNode::testButtonEnabledGetterSetter()
{
    auto node = makeNode();

    // Set enabled
    node->setButtonEnabled(true);
    QVERIFY(node->isButtonEnabled());

    // Set disabled
    node->setButtonEnabled(false);
    QVERIFY(!node->isButtonEnabled());
}

// ============================================================================
// buttonEnabled — signal emission
// ============================================================================
void TestQImPlotLegendNode::testButtonEnabledChangedSignal()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotLegendNode::legendFlagChanged);

    node->setButtonEnabled(false);
    QCOMPARE(spy.count(), 1);

    node->setButtonEnabled(true);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// buttonEnabled — signal deduplication
// ============================================================================
void TestQImPlotLegendNode::testButtonEnabledSignalDedu()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotLegendNode::legendFlagChanged);

    node->setButtonEnabled(false);
    QCOMPARE(spy.count(), 1);

    // Same value — no duplicate signal
    node->setButtonEnabled(false);
    QCOMPARE(spy.count(), 1);
}

// ============================================================================
// highlightItemEnabled — getter/setter
// ============================================================================
void TestQImPlotLegendNode::testHighlightItemEnabledGetterSetter()
{
    auto node = makeNode();

    // Set enabled
    node->setHighlightItemEnabled(true);
    QVERIFY(node->isHighlightItemEnabled());

    // Set disabled
    node->setHighlightItemEnabled(false);
    QVERIFY(!node->isHighlightItemEnabled());
}

// ============================================================================
// highlightItemEnabled — signal emission
// ============================================================================
void TestQImPlotLegendNode::testHighlightItemEnabledChangedSignal()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotLegendNode::legendFlagChanged);

    node->setHighlightItemEnabled(false);
    QCOMPARE(spy.count(), 1);

    node->setHighlightItemEnabled(true);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// highlightItemEnabled — signal deduplication
// ============================================================================
void TestQImPlotLegendNode::testHighlightItemEnabledSignalDedu()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotLegendNode::legendFlagChanged);

    node->setHighlightItemEnabled(false);
    QCOMPARE(spy.count(), 1);

    // Same value — no duplicate signal
    node->setHighlightItemEnabled(false);
    QCOMPARE(spy.count(), 1);
}

// ============================================================================
// highlightAxisEnabled — getter/setter
// ============================================================================
void TestQImPlotLegendNode::testHighlightAxisEnabledGetterSetter()
{
    auto node = makeNode();

    // Set enabled
    node->setHighlightAxisEnabled(true);
    QVERIFY(node->isHighlightAxisEnabled());

    // Set disabled
    node->setHighlightAxisEnabled(false);
    QVERIFY(!node->isHighlightAxisEnabled());
}

// ============================================================================
// highlightAxisEnabled — signal emission
// ============================================================================
void TestQImPlotLegendNode::testHighlightAxisEnabledChangedSignal()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotLegendNode::legendFlagChanged);

    node->setHighlightAxisEnabled(false);
    QCOMPARE(spy.count(), 1);

    node->setHighlightAxisEnabled(true);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// highlightAxisEnabled — signal deduplication
// ============================================================================
void TestQImPlotLegendNode::testHighlightAxisEnabledSignalDedu()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotLegendNode::legendFlagChanged);

    node->setHighlightAxisEnabled(false);
    QCOMPARE(spy.count(), 1);

    // Same value — no duplicate signal
    node->setHighlightAxisEnabled(false);
    QCOMPARE(spy.count(), 1);
}

// ============================================================================
// sort — getter/setter
// ============================================================================
void TestQImPlotLegendNode::testSortGetterSetter()
{
    auto node = makeNode();

    // Set enabled
    node->setSort(true);
    QVERIFY(node->isSort());

    // Set disabled
    node->setSort(false);
    QVERIFY(!node->isSort());
}

// ============================================================================
// sort — signal emission
// ============================================================================
void TestQImPlotLegendNode::testSortChangedSignal()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotLegendNode::legendFlagChanged);

    node->setSort(true);
    QCOMPARE(spy.count(), 1);

    node->setSort(false);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// sort — signal deduplication
// ============================================================================
void TestQImPlotLegendNode::testSortSignalDedu()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotLegendNode::legendFlagChanged);

    node->setSort(true);
    QCOMPARE(spy.count(), 1);

    // Same value — no duplicate signal
    node->setSort(true);
    QCOMPARE(spy.count(), 1);
}

// ============================================================================
// reverse — getter/setter
// ============================================================================
void TestQImPlotLegendNode::testReverseGetterSetter()
{
    auto node = makeNode();

    // Set enabled
    node->setReverse(true);
    QVERIFY(node->isReverse());

    // Set disabled
    node->setReverse(false);
    QVERIFY(!node->isReverse());
}

// ============================================================================
// reverse — signal emission
// ============================================================================
void TestQImPlotLegendNode::testReverseChangedSignal()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotLegendNode::legendFlagChanged);

    node->setReverse(true);
    QCOMPARE(spy.count(), 1);

    node->setReverse(false);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// reverse — signal deduplication
// ============================================================================
void TestQImPlotLegendNode::testReverseSignalDedu()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotLegendNode::legendFlagChanged);

    node->setReverse(true);
    QCOMPARE(spy.count(), 1);

    // Same value — no duplicate signal
    node->setReverse(true);
    QCOMPARE(spy.count(), 1);
}

QTEST_GUILESS_MAIN(TestQImPlotLegendNode)
#include "tst_qimplotlegendnode.moc"
