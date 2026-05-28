#include <QObject>
#include <QtTest>
#include <memory>

#include "QImPlotHistogramItemNode.h"
#include "../core/TestHelpers.h"

using namespace QIM;

class TestQImPlotHistogramItemNode : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    // Construction
    void testDefaultConstruction();

    // bins property
    void testBinsGetterSetter();
    void testBinsChangedSignal();
    void testBinsSignalDedu();

    // barScale property
    void testBarScaleGetterSetter();

    // rangeMin / rangeMax properties
    void testRangeGetterSetter();

    // cumulative property
    void testCumulativeGetterSetter();

    // density property
    void testDensityGetterSetter();

    // setData
    void testSetData();

private:
    std::unique_ptr<QImPlotHistogramItemNode> makeNode()
    {
        return std::make_unique<QImPlotHistogramItemNode>();
    }
};

void TestQImPlotHistogramItemNode::initTestCase()
{
}

void TestQImPlotHistogramItemNode::cleanupTestCase()
{
}

// ============================================================================
// Construction
// ============================================================================
void TestQImPlotHistogramItemNode::testDefaultConstruction()
{
    auto node = makeNode();
    QVERIFY(node != nullptr);
    QVERIFY(node->isVisible());
    QCOMPARE(node->type(), QImPlotHistogramItemNode::Type);
}

// ============================================================================
// bins — getter/setter
// ============================================================================
void TestQImPlotHistogramItemNode::testBinsGetterSetter()
{
    auto node = makeNode();

    // Default value (Sturges' formula or similar auto method)
    int defaultBins = node->bins();
    Q_UNUSED(defaultBins);

    // Set explicit bin count
    node->setBins(20);
    QCOMPARE(node->bins(), 20);

    // Redundant set (same value)
    node->setBins(20);
    QCOMPARE(node->bins(), 20);
}

// ============================================================================
// bins — signal emission
// ============================================================================
void TestQImPlotHistogramItemNode::testBinsChangedSignal()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotHistogramItemNode::binsChanged);

    node->setBins(15);
    QCOMPARE(spy.count(), 1);

    node->setBins(30);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// bins — signal deduplication
// ============================================================================
void TestQImPlotHistogramItemNode::testBinsSignalDedu()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotHistogramItemNode::binsChanged);

    node->setBins(10);
    QCOMPARE(spy.count(), 1);

    // Same value — no duplicate signal
    node->setBins(10);
    QCOMPARE(spy.count(), 1);
}

// ============================================================================
// barScale — getter/setter
// ============================================================================
void TestQImPlotHistogramItemNode::testBarScaleGetterSetter()
{
    auto node = makeNode();

    // Default bar scale
    double defaultScale = node->barScale();
    QVERIFY(defaultScale > 0.0);

    // Set new scale
    node->setBarScale(0.8);
    QCOMPARE(node->barScale(), 0.8);
}

// ============================================================================
// rangeMin / rangeMax — getter/setter
// ============================================================================
void TestQImPlotHistogramItemNode::testRangeGetterSetter()
{
    auto node = makeNode();

    // Default range (0 = auto)
    QCOMPARE(node->rangeMin(), 0.0);
    QCOMPARE(node->rangeMax(), 0.0);

    // Set explicit range
    node->setRangeMin(-5.0);
    node->setRangeMax(5.0);
    QCOMPARE(node->rangeMin(), -5.0);
    QCOMPARE(node->rangeMax(), 5.0);
}

// ============================================================================
// cumulative — getter/setter
// ============================================================================
void TestQImPlotHistogramItemNode::testCumulativeGetterSetter()
{
    auto node = makeNode();

    // Default: not cumulative
    QVERIFY(!node->isCumulative());

    // Enable
    node->setCumulative(true);
    QVERIFY(node->isCumulative());

    // Disable
    node->setCumulative(false);
    QVERIFY(!node->isCumulative());
}

// ============================================================================
// density — getter/setter
// ============================================================================
void TestQImPlotHistogramItemNode::testDensityGetterSetter()
{
    auto node = makeNode();

    // Default: not density
    QVERIFY(!node->isDensity());

    // Enable
    node->setDensity(true);
    QVERIFY(node->isDensity());

    // Disable
    node->setDensity(false);
    QVERIFY(!node->isDensity());
}

// ============================================================================
// setData — verify Y-only data assignment
// ============================================================================
void TestQImPlotHistogramItemNode::testSetData()
{
    auto node = makeNode();

    auto ys = TestHelpers::makeLinearYs(100);
    node->setData(ys);
    QVERIFY(node->data() != nullptr);
}

QTEST_GUILESS_MAIN(TestQImPlotHistogramItemNode)
#include "tst_qimplothistogramitemnode.moc"
