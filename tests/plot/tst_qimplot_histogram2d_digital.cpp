#include <QObject>
#include <QtTest>
#include <memory>

#include "QImPlotHistogram2DItemNode.h"
#include "QImPlotDigitalItemNode.h"
#include "../core/TestHelpers.h"

using namespace QIM;

class TestQImPlotHistogram2DDigital : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    // ========================================================================
    // QImPlotHistogram2DItemNode tests
    // ========================================================================

    // Construction
    void testHistogram2DDefaultConstruction();

    // xBins property
    void testXBinsGetterSetter();
    void testXBinsChangedSignal();
    void testXBinsSignalDedup();

    // yBins property
    void testYBinsGetterSetter();
    void testYBinsChangedSignal();
    void testYBinsSignalDedup();

    // density property
    void testDensityGetterSetter();
    void testDensityChangedSignal();
    void testDensitySignalDedup();

    // colMajor property
    void testColMajorGetterSetter();
    void testColMajorChangedSignal();
    void testColMajorSignalDedup();

    // setData
    void testHistogram2DSetData();

    // ========================================================================
    // QImPlotDigitalItemNode tests
    // ========================================================================

    // Construction
    void testDigitalDefaultConstruction();

    // color property
    void testColorGetterSetter();
    void testColorChangedSignal();
    void testColorSignalDedup();

    // setData
    void testDigitalSetData();

private:
    std::unique_ptr<QImPlotHistogram2DItemNode> makeHistogram2DNode()
    {
        return std::make_unique<QImPlotHistogram2DItemNode>();
    }

    std::unique_ptr<QImPlotDigitalItemNode> makeDigitalNode()
    {
        return std::make_unique<QImPlotDigitalItemNode>();
    }
};

void TestQImPlotHistogram2DDigital::initTestCase()
{
}

void TestQImPlotHistogram2DDigital::cleanupTestCase()
{
}

// ============================================================================
// QImPlotHistogram2DItemNode — Construction
// ============================================================================
void TestQImPlotHistogram2DDigital::testHistogram2DDefaultConstruction()
{
    auto node = makeHistogram2DNode();
    QVERIFY(node != nullptr);
    QVERIFY(node->isVisible());
    QCOMPARE(node->type(), QImPlotHistogram2DItemNode::Type);
}

// ============================================================================
// QImPlotHistogram2DItemNode — xBins getter/setter
// ============================================================================
void TestQImPlotHistogram2DDigital::testXBinsGetterSetter()
{
    auto node = makeHistogram2DNode();

    // Set new value
    node->setXBins(20);
    QCOMPARE(node->xBins(), 20);

    // Redundant set (same value) — getter still returns same
    node->setXBins(20);
    QCOMPARE(node->xBins(), 20);
}

// ============================================================================
// QImPlotHistogram2DItemNode — xBins signal emission
// ============================================================================
void TestQImPlotHistogram2DDigital::testXBinsChangedSignal()
{
    auto node = makeHistogram2DNode();
    QSignalSpy spy(node.get(), &QImPlotHistogram2DItemNode::xBinsChanged);

    node->setXBins(10);
    QCOMPARE(spy.count(), 1);

    node->setXBins(30);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// QImPlotHistogram2DItemNode — xBins signal deduplication
// ============================================================================
void TestQImPlotHistogram2DDigital::testXBinsSignalDedup()
{
    auto node = makeHistogram2DNode();
    QSignalSpy spy(node.get(), &QImPlotHistogram2DItemNode::xBinsChanged);

    node->setXBins(15);
    QCOMPARE(spy.count(), 1);

    // Same value — no duplicate signal
    node->setXBins(15);
    QCOMPARE(spy.count(), 1);
}

// ============================================================================
// QImPlotHistogram2DItemNode — yBins getter/setter
// ============================================================================
void TestQImPlotHistogram2DDigital::testYBinsGetterSetter()
{
    auto node = makeHistogram2DNode();

    // Set new value
    node->setYBins(25);
    QCOMPARE(node->yBins(), 25);

    // Redundant set (same value) — getter still returns same
    node->setYBins(25);
    QCOMPARE(node->yBins(), 25);
}

// ============================================================================
// QImPlotHistogram2DItemNode — yBins signal emission
// ============================================================================
void TestQImPlotHistogram2DDigital::testYBinsChangedSignal()
{
    auto node = makeHistogram2DNode();
    QSignalSpy spy(node.get(), &QImPlotHistogram2DItemNode::yBinsChanged);

    node->setYBins(10);
    QCOMPARE(spy.count(), 1);

    node->setYBins(50);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// QImPlotHistogram2DItemNode — yBins signal deduplication
// ============================================================================
void TestQImPlotHistogram2DDigital::testYBinsSignalDedup()
{
    auto node = makeHistogram2DNode();
    QSignalSpy spy(node.get(), &QImPlotHistogram2DItemNode::yBinsChanged);

    node->setYBins(20);
    QCOMPARE(spy.count(), 1);

    // Same value — no duplicate signal
    node->setYBins(20);
    QCOMPARE(spy.count(), 1);
}

// ============================================================================
// QImPlotHistogram2DItemNode — density getter/setter
// ============================================================================
void TestQImPlotHistogram2DDigital::testDensityGetterSetter()
{
    auto node = makeHistogram2DNode();

    // Default: false
    QVERIFY(!node->isDensity());

    // Set true
    node->setDensity(true);
    QVERIFY(node->isDensity());

    // Set false
    node->setDensity(false);
    QVERIFY(!node->isDensity());
}

// ============================================================================
// QImPlotHistogram2DItemNode — density signal emission
// ============================================================================
void TestQImPlotHistogram2DDigital::testDensityChangedSignal()
{
    auto node = makeHistogram2DNode();
    QSignalSpy spy(node.get(), &QImPlotHistogram2DItemNode::densityChanged);

    node->setDensity(true);
    QCOMPARE(spy.count(), 1);

    node->setDensity(false);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// QImPlotHistogram2DItemNode — density signal deduplication
// ============================================================================
void TestQImPlotHistogram2DDigital::testDensitySignalDedup()
{
    auto node = makeHistogram2DNode();
    QSignalSpy spy(node.get(), &QImPlotHistogram2DItemNode::densityChanged);

    node->setDensity(true);
    QCOMPARE(spy.count(), 1);

    // Same value — no duplicate signal
    node->setDensity(true);
    QCOMPARE(spy.count(), 1);
}

// ============================================================================
// QImPlotHistogram2DItemNode — colMajor getter/setter
// ============================================================================
void TestQImPlotHistogram2DDigital::testColMajorGetterSetter()
{
    auto node = makeHistogram2DNode();

    // Default: false
    QVERIFY(!node->isColMajor());

    // Set true
    node->setColMajor(true);
    QVERIFY(node->isColMajor());

    // Set false
    node->setColMajor(false);
    QVERIFY(!node->isColMajor());
}

// ============================================================================
// QImPlotHistogram2DItemNode — colMajor signal emission
// ============================================================================
void TestQImPlotHistogram2DDigital::testColMajorChangedSignal()
{
    auto node = makeHistogram2DNode();
    QSignalSpy spy(node.get(), &QImPlotHistogram2DItemNode::colMajorChanged);

    node->setColMajor(true);
    QCOMPARE(spy.count(), 1);

    node->setColMajor(false);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// QImPlotHistogram2DItemNode — colMajor signal deduplication
// ============================================================================
void TestQImPlotHistogram2DDigital::testColMajorSignalDedup()
{
    auto node = makeHistogram2DNode();
    QSignalSpy spy(node.get(), &QImPlotHistogram2DItemNode::colMajorChanged);

    node->setColMajor(true);
    QCOMPARE(spy.count(), 1);

    // Same value — no duplicate signal
    node->setColMajor(true);
    QCOMPARE(spy.count(), 1);
}

// ============================================================================
// QImPlotHistogram2DItemNode — setData
// ============================================================================
void TestQImPlotHistogram2DDigital::testHistogram2DSetData()
{
    auto node = makeHistogram2DNode();

    auto xs = TestHelpers::makeLinearXs(10);
    auto ys = TestHelpers::makeLinearYs(10);

    node->setData(xs, ys);
    QVERIFY(node->data() != nullptr);
}

// ============================================================================
// QImPlotDigitalItemNode — Construction
// ============================================================================
void TestQImPlotHistogram2DDigital::testDigitalDefaultConstruction()
{
    auto node = makeDigitalNode();
    QVERIFY(node != nullptr);
    QVERIFY(node->isVisible());
    QCOMPARE(node->type(), QImPlotDigitalItemNode::Type);
}

// ============================================================================
// QImPlotDigitalItemNode — color getter/setter
// ============================================================================
void TestQImPlotHistogram2DDigital::testColorGetterSetter()
{
    auto node = makeDigitalNode();

    // Set color
    QColor testColor(100, 150, 200, 255);
    node->setColor(testColor);
    QCOMPARE(node->color(), testColor);

    // Set another color
    QColor anotherColor(255, 0, 0, 128);
    node->setColor(anotherColor);
    QCOMPARE(node->color(), anotherColor);
}

// ============================================================================
// QImPlotDigitalItemNode — color signal emission
// ============================================================================
void TestQImPlotHistogram2DDigital::testColorChangedSignal()
{
    auto node = makeDigitalNode();
    QSignalSpy spy(node.get(), &QImPlotDigitalItemNode::colorChanged);

    node->setColor(QColor(255, 0, 0));
    QCOMPARE(spy.count(), 1);

    node->setColor(QColor(0, 255, 0));
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// QImPlotDigitalItemNode — color signal deduplication
// ============================================================================
void TestQImPlotHistogram2DDigital::testColorSignalDedup()
{
    auto node = makeDigitalNode();
    QSignalSpy spy(node.get(), &QImPlotDigitalItemNode::colorChanged);

    QColor testColor(100, 100, 100);
    node->setColor(testColor);
    QCOMPARE(spy.count(), 1);

    // Same value — no duplicate signal
    node->setColor(testColor);
    QCOMPARE(spy.count(), 1);
}

// ============================================================================
// QImPlotDigitalItemNode — setData
// ============================================================================
void TestQImPlotHistogram2DDigital::testDigitalSetData()
{
    auto node = makeDigitalNode();

    auto xs = TestHelpers::makeLinearXs(10);
    auto ys = TestHelpers::makeLinearYs(10);

    node->setData(xs, ys);
    QVERIFY(node->data() != nullptr);
}

QTEST_GUILESS_MAIN(TestQImPlotHistogram2DDigital)
#include "tst_qimplot_histogram2d_digital.moc"
