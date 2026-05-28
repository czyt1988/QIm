#include <QObject>
#include <QtTest>
#include <memory>

#include "QImPlotHeatmapItemNode.h"
#include "../core/TestHelpers.h"

using namespace QIM;

class TestQImPlotHeatmapItemNode : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    // Construction
    void testDefaultConstruction();

    // scaleMin property
    void testScaleMinGetterSetter();
    void testScaleMinChangedSignal();
    void testScaleMinSignalDedup();

    // scaleMax property
    void testScaleMaxGetterSetter();
    void testScaleMaxChangedSignal();

    // labelFormat property
    void testLabelFormatGetterSetter();
    void testLabelFormatChangedSignal();

    // boundsMin property
    void testBoundsMinGetterSetter();
    void testBoundsMinChangedSignal();

    // boundsMax property
    void testBoundsMaxGetterSetter();
    void testBoundsMaxChangedSignal();

    // colMajor property
    void testColMajorGetterSetter();
    void testColMajorChangedSignal();
    void testColMajorSignalDedup();

private:
    std::unique_ptr<QImPlotHeatmapItemNode> makeNode()
    {
        return std::make_unique<QImPlotHeatmapItemNode>();
    }
};

void TestQImPlotHeatmapItemNode::initTestCase()
{
}

void TestQImPlotHeatmapItemNode::cleanupTestCase()
{
}

// ============================================================================
// Construction
// ============================================================================
void TestQImPlotHeatmapItemNode::testDefaultConstruction()
{
    auto node = makeNode();
    QVERIFY(node != nullptr);
    QVERIFY(node->isVisible());
    QCOMPARE(node->type(), QImPlotHeatmapItemNode::Type);

    // Verify default property values
    QCOMPARE(node->scaleMin(), 0.0);
    QCOMPARE(node->scaleMax(), 0.0);
    QCOMPARE(node->labelFormat(), QString("%.1f"));
    QCOMPARE(node->boundsMin(), QPointF(0.0, 0.0));
    QCOMPARE(node->boundsMax(), QPointF(1.0, 1.0));
    QVERIFY(!node->isColMajor());
}

// ============================================================================
// scaleMin — getter/setter
// ============================================================================
void TestQImPlotHeatmapItemNode::testScaleMinGetterSetter()
{
    auto node = makeNode();

    // Default value
    QCOMPARE(node->scaleMin(), 0.0);

    // Set new value
    node->setScaleMin(-5.0);
    QCOMPARE(node->scaleMin(), -5.0);

    // Redundant set (same value) — getter still returns same
    node->setScaleMin(-5.0);
    QCOMPARE(node->scaleMin(), -5.0);
}

// ============================================================================
// scaleMin — signal emission
// ============================================================================
void TestQImPlotHeatmapItemNode::testScaleMinChangedSignal()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotHeatmapItemNode::scaleMinChanged);

    node->setScaleMin(1.0);
    QCOMPARE(spy.count(), 1);

    node->setScaleMin(2.0);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// scaleMin — signal deduplication
// ============================================================================
void TestQImPlotHeatmapItemNode::testScaleMinSignalDedup()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotHeatmapItemNode::scaleMinChanged);

    node->setScaleMin(3.5);
    QCOMPARE(spy.count(), 1);

    // Same value — no duplicate signal
    node->setScaleMin(3.5);
    QCOMPARE(spy.count(), 1);
}

// ============================================================================
// scaleMax — getter/setter
// ============================================================================
void TestQImPlotHeatmapItemNode::testScaleMaxGetterSetter()
{
    auto node = makeNode();

    // Default value
    QCOMPARE(node->scaleMax(), 0.0);

    // Set new value
    node->setScaleMax(10.0);
    QCOMPARE(node->scaleMax(), 10.0);

    // Redundant set (same value)
    node->setScaleMax(10.0);
    QCOMPARE(node->scaleMax(), 10.0);
}

// ============================================================================
// scaleMax — signal emission
// ============================================================================
void TestQImPlotHeatmapItemNode::testScaleMaxChangedSignal()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotHeatmapItemNode::scaleMaxChanged);

    node->setScaleMax(5.0);
    QCOMPARE(spy.count(), 1);

    node->setScaleMax(8.0);
    QCOMPARE(spy.count(), 2);

    // Same value — no duplicate
    node->setScaleMax(8.0);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// labelFormat — getter/setter
// ============================================================================
void TestQImPlotHeatmapItemNode::testLabelFormatGetterSetter()
{
    auto node = makeNode();

    // Default value
    QCOMPARE(node->labelFormat(), QString("%.1f"));

    // Set new value
    node->setLabelFormat("%.3f");
    QCOMPARE(node->labelFormat(), QString("%.3f"));

    // Redundant set (same value)
    node->setLabelFormat("%.3f");
    QCOMPARE(node->labelFormat(), QString("%.3f"));
}

// ============================================================================
// labelFormat — signal emission
// ============================================================================
void TestQImPlotHeatmapItemNode::testLabelFormatChangedSignal()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotHeatmapItemNode::labelFormatChanged);

    node->setLabelFormat("%.2f");
    QCOMPARE(spy.count(), 1);

    node->setLabelFormat("%.4f");
    QCOMPARE(spy.count(), 2);

    // Same value — no duplicate
    node->setLabelFormat("%.4f");
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// boundsMin — getter/setter
// ============================================================================
void TestQImPlotHeatmapItemNode::testBoundsMinGetterSetter()
{
    auto node = makeNode();

    // Default value
    QCOMPARE(node->boundsMin(), QPointF(0.0, 0.0));

    // Set new value
    QPointF newMin(-2.5, -3.0);
    node->setBoundsMin(newMin);
    QCOMPARE(node->boundsMin(), newMin);

    // Redundant set (same value)
    node->setBoundsMin(newMin);
    QCOMPARE(node->boundsMin(), newMin);
}

// ============================================================================
// boundsMin — signal emission
// ============================================================================
void TestQImPlotHeatmapItemNode::testBoundsMinChangedSignal()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotHeatmapItemNode::boundsMinChanged);

    node->setBoundsMin(QPointF(1.0, 2.0));
    QCOMPARE(spy.count(), 1);

    node->setBoundsMin(QPointF(3.0, 4.0));
    QCOMPARE(spy.count(), 2);

    // Same value — no duplicate
    node->setBoundsMin(QPointF(3.0, 4.0));
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// boundsMax — getter/setter
// ============================================================================
void TestQImPlotHeatmapItemNode::testBoundsMaxGetterSetter()
{
    auto node = makeNode();

    // Default value
    QCOMPARE(node->boundsMax(), QPointF(1.0, 1.0));

    // Set new value
    QPointF newMax(5.0, 6.0);
    node->setBoundsMax(newMax);
    QCOMPARE(node->boundsMax(), newMax);

    // Redundant set (same value)
    node->setBoundsMax(newMax);
    QCOMPARE(node->boundsMax(), newMax);
}

// ============================================================================
// boundsMax — signal emission
// ============================================================================
void TestQImPlotHeatmapItemNode::testBoundsMaxChangedSignal()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotHeatmapItemNode::boundsMaxChanged);

    node->setBoundsMax(QPointF(2.0, 3.0));
    QCOMPARE(spy.count(), 1);

    node->setBoundsMax(QPointF(4.0, 5.0));
    QCOMPARE(spy.count(), 2);

    // Same value — no duplicate
    node->setBoundsMax(QPointF(4.0, 5.0));
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// colMajor — getter/setter
// ============================================================================
void TestQImPlotHeatmapItemNode::testColMajorGetterSetter()
{
    auto node = makeNode();

    // Default: row-major
    QVERIFY(!node->isColMajor());

    // Set column-major
    node->setColMajor(true);
    QVERIFY(node->isColMajor());

    // Reset to row-major
    node->setColMajor(false);
    QVERIFY(!node->isColMajor());
}

// ============================================================================
// colMajor — signal emission
// ============================================================================
void TestQImPlotHeatmapItemNode::testColMajorChangedSignal()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotHeatmapItemNode::colMajorChanged);

    node->setColMajor(true);
    QCOMPARE(spy.count(), 1);

    node->setColMajor(false);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// colMajor — signal deduplication
// ============================================================================
void TestQImPlotHeatmapItemNode::testColMajorSignalDedup()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotHeatmapItemNode::colMajorChanged);

    node->setColMajor(true);
    QCOMPARE(spy.count(), 1);

    // Same value — no duplicate signal
    node->setColMajor(true);
    QCOMPARE(spy.count(), 1);
}

QTEST_GUILESS_MAIN(TestQImPlotHeatmapItemNode)
#include "tst_qimplotheatmapitemnode.moc"
