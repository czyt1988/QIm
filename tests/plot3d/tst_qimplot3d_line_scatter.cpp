// ============================================
// Combined test: QImPlot3DLineItemNode + QImPlot3DScatterItemNode
// ============================================

#include <QObject>
#include <QtTest>
#include <memory>

#include "QImPlot3DLineItemNode.h"
#include "QImPlot3DScatterItemNode.h"
#include "../core/TestHelpers.h"

using namespace QIM;

class TestPlot3DLineScatter : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    // === QImPlot3DLineItemNode ===

    // Construction
    void testLineDefaultConstruction();

    // lineWeight
    void testLineWeightGetterSetter();
    void testLineWeightChangedSignal();

    // color
    void testLineColorGetterSetter();

    // segmentsEnabled
    void testSegmentsEnabled();

    // loopEnabled
    void testLoopEnabled();

    // === QImPlot3DScatterItemNode ===

    // Construction
    void testScatterDefaultConstruction();

    // markerSize
    void testMarkerSizeGetterSetter();
    void testMarkerSizeChangedSignal();

    // markerFillColor
    void testMarkerFillColorGetterSetter();

    // markerOutlineColor
    void testMarkerOutlineColorGetterSetter();

    // markerWeight
    void testMarkerWeightGetterSetter();

    // markerShape
    void testMarkerShapeGetterSetter();
};

void TestPlot3DLineScatter::initTestCase()
{
}

void TestPlot3DLineScatter::cleanupTestCase()
{
}

// ============================================================================
// QImPlot3DLineItemNode — Construction
// ============================================================================
void TestPlot3DLineScatter::testLineDefaultConstruction()
{
    auto node = std::make_unique<QImPlot3DLineItemNode>();
    QVERIFY(node != nullptr);
    QVERIFY(node->isVisible());

    // Default color is invalid (deferred initialization)
    QVERIFY(!node->color().isValid());

    // Default lineWeight
    QCOMPARE(node->lineWeight(), 1.0f);

    // Default flags: all off (positive semantics)
    QVERIFY(!node->isSegmentsEnabled());
    QVERIFY(!node->isLoopEnabled());
}

// ============================================================================
// QImPlot3DLineItemNode — lineWeight
// ============================================================================
void TestPlot3DLineScatter::testLineWeightGetterSetter()
{
    auto node = std::make_unique<QImPlot3DLineItemNode>();

    node->setLineWeight(2.5f);
    QCOMPARE(node->lineWeight(), 2.5f);

    node->setLineWeight(0.5f);
    QCOMPARE(node->lineWeight(), 0.5f);

    // Redundant set — getter still correct
    node->setLineWeight(0.5f);
    QCOMPARE(node->lineWeight(), 0.5f);
}

void TestPlot3DLineScatter::testLineWeightChangedSignal()
{
    auto node = std::make_unique<QImPlot3DLineItemNode>();
    QSignalSpy spy(node.get(), &QImPlot3DLineItemNode::lineWeightChanged);

    node->setLineWeight(3.0f);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toFloat(), 3.0f);

    // Same value — no duplicate signal
    node->setLineWeight(3.0f);
    QCOMPARE(spy.count(), 1);

    node->setLineWeight(5.0f);
    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.at(1).at(0).toFloat(), 5.0f);
}

// ============================================================================
// QImPlot3DLineItemNode — color
// ============================================================================
void TestPlot3DLineScatter::testLineColorGetterSetter()
{
    auto node = std::make_unique<QImPlot3DLineItemNode>();

    QColor c(0, 114, 189);
    node->setColor(c);
    QCOMPARE(node->color(), c);

    QColor red(Qt::red);
    node->setColor(red);
    QCOMPARE(node->color(), red);
}

// ============================================================================
// QImPlot3DLineItemNode — segmentsEnabled
// ============================================================================
void TestPlot3DLineScatter::testSegmentsEnabled()
{
    auto node = std::make_unique<QImPlot3DLineItemNode>();

    // Default: false (positive semantics)
    QVERIFY(!node->isSegmentsEnabled());

    node->setSegmentsEnabled(true);
    QVERIFY(node->isSegmentsEnabled());

    node->setSegmentsEnabled(false);
    QVERIFY(!node->isSegmentsEnabled());

    // Signal dedup: same value should not emit again
    QSignalSpy spy(node.get(), &QImPlot3DLineItemNode::lineFlagChanged);
    node->setSegmentsEnabled(false);
    QCOMPARE(spy.count(), 0);

    node->setSegmentsEnabled(true);
    QCOMPARE(spy.count(), 1);
}

// ============================================================================
// QImPlot3DLineItemNode — loopEnabled
// ============================================================================
void TestPlot3DLineScatter::testLoopEnabled()
{
    auto node = std::make_unique<QImPlot3DLineItemNode>();

    QVERIFY(!node->isLoopEnabled());

    node->setLoopEnabled(true);
    QVERIFY(node->isLoopEnabled());

    node->setLoopEnabled(false);
    QVERIFY(!node->isLoopEnabled());
}

// ============================================================================
// QImPlot3DScatterItemNode — Construction
// ============================================================================
void TestPlot3DLineScatter::testScatterDefaultConstruction()
{
    auto node = std::make_unique<QImPlot3DScatterItemNode>();
    QVERIFY(node != nullptr);
    QVERIFY(node->isVisible());

    // Default colors: invalid (deferred initialization)
    QVERIFY(!node->markerFillColor().isValid());
    QVERIFY(!node->markerOutlineColor().isValid());

    // Default markerShape: Circle (0)
    QCOMPARE(node->markerShape(), static_cast<int>(QImPlot3DMarkerShape::Circle));
}

// ============================================================================
// QImPlot3DScatterItemNode — markerSize
// ============================================================================
void TestPlot3DLineScatter::testMarkerSizeGetterSetter()
{
    auto node = std::make_unique<QImPlot3DScatterItemNode>();

    float defaultSize = node->markerSize();
    QCOMPARE(defaultSize, 4.0f);

    node->setMarkerSize(8.0f);
    QCOMPARE(node->markerSize(), 8.0f);

    node->setMarkerSize(2.0f);
    QCOMPARE(node->markerSize(), 2.0f);

    // Redundant set
    node->setMarkerSize(2.0f);
    QCOMPARE(node->markerSize(), 2.0f);
}

void TestPlot3DLineScatter::testMarkerSizeChangedSignal()
{
    auto node = std::make_unique<QImPlot3DScatterItemNode>();
    QSignalSpy spy(node.get(), &QImPlot3DScatterItemNode::markerSizeChanged);

    node->setMarkerSize(6.0f);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toFloat(), 6.0f);

    // Same value — no duplicate signal
    node->setMarkerSize(6.0f);
    QCOMPARE(spy.count(), 1);

    node->setMarkerSize(10.0f);
    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.at(1).at(0).toFloat(), 10.0f);
}

// ============================================================================
// QImPlot3DScatterItemNode — markerFillColor
// ============================================================================
void TestPlot3DLineScatter::testMarkerFillColorGetterSetter()
{
    auto node = std::make_unique<QImPlot3DScatterItemNode>();

    QColor fill(217, 83, 25);
    node->setMarkerFillColor(fill);
    QCOMPARE(node->markerFillColor(), fill);

    QColor blue(Qt::blue);
    node->setMarkerFillColor(blue);
    QCOMPARE(node->markerFillColor(), blue);
}

// ============================================================================
// QImPlot3DScatterItemNode — markerOutlineColor
// ============================================================================
void TestPlot3DLineScatter::testMarkerOutlineColorGetterSetter()
{
    auto node = std::make_unique<QImPlot3DScatterItemNode>();

    QColor outline(120, 45, 10);
    node->setMarkerOutlineColor(outline);
    QCOMPARE(node->markerOutlineColor(), outline);

    QColor green(Qt::green);
    node->setMarkerOutlineColor(green);
    QCOMPARE(node->markerOutlineColor(), green);
}

// ============================================================================
// QImPlot3DScatterItemNode — markerWeight
// ============================================================================
void TestPlot3DLineScatter::testMarkerWeightGetterSetter()
{
    auto node = std::make_unique<QImPlot3DScatterItemNode>();

    float defaultWeight = node->markerWeight();
    QCOMPARE(defaultWeight, 1.0f);

    node->setMarkerWeight(2.5f);
    QCOMPARE(node->markerWeight(), 2.5f);

    node->setMarkerWeight(0.5f);
    QCOMPARE(node->markerWeight(), 0.5f);
}

// ============================================================================
// QImPlot3DScatterItemNode — markerShape
// ============================================================================
void TestPlot3DLineScatter::testMarkerShapeGetterSetter()
{
    auto node = std::make_unique<QImPlot3DScatterItemNode>();

    // Default: Circle
    QCOMPARE(node->markerShape(), static_cast<int>(QImPlot3DMarkerShape::Circle));

    node->setMarkerShape(static_cast<int>(QImPlot3DMarkerShape::Square));
    QCOMPARE(node->markerShape(), static_cast<int>(QImPlot3DMarkerShape::Square));

    node->setMarkerShape(static_cast<int>(QImPlot3DMarkerShape::Diamond));
    QCOMPARE(node->markerShape(), static_cast<int>(QImPlot3DMarkerShape::Diamond));

    node->setMarkerShape(static_cast<int>(QImPlot3DMarkerShape::Cross));
    QCOMPARE(node->markerShape(), static_cast<int>(QImPlot3DMarkerShape::Cross));
}

QTEST_GUILESS_MAIN(TestPlot3DLineScatter)
#include "tst_qimplot3d_line_scatter.moc"
