#include <QObject>
#include <QtTest>
#include <memory>

#include "QImPlotStairsItemNode.h"
#include "QImPlotShadedItemNode.h"
#include "QImPlotStemsItemNode.h"
#include "../core/TestHelpers.h"

using namespace QIM;

class TestStairsShadedStems : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    // QImPlotStairsItemNode tests
    void testStairsDefaultConstruction();
    void testStairsPreStepFlag();
    void testStairsShadedFlag();
    void testStairsSignalDedup();

    // QImPlotShadedItemNode tests
    void testShadedDefaultConstruction();
    void testShadedReferenceValue();
    void testShadedColor();
    void testShadedSignalDedup();

    // QImPlotStemsItemNode tests
    void testStemsDefaultConstruction();
    void testStemsReferenceValue();
    void testStemsHorizontal();
    void testStemsColor();
    void testStemsSignalDedup();

private:
    // Helper methods here
};

void TestStairsShadedStems::initTestCase()
{
}

void TestStairsShadedStems::cleanupTestCase()
{
}

// ============================================================================
// QImPlotStairsItemNode Tests
// ============================================================================

void TestStairsShadedStems::testStairsDefaultConstruction()
{
    auto node = std::make_unique<QImPlotStairsItemNode>();
    QVERIFY(node != nullptr);
    QVERIFY(!node->isPreStep());
    QVERIFY(!node->isShaded());
}

void TestStairsShadedStems::testStairsPreStepFlag()
{
    auto node = std::make_unique<QImPlotStairsItemNode>();
    QCOMPARE(node->isPreStep(), false);

    node->setPreStep(true);
    QCOMPARE(node->isPreStep(), true);

    node->setPreStep(false);
    QCOMPARE(node->isPreStep(), false);

    // Redundant set
    node->setPreStep(false);
    QCOMPARE(node->isPreStep(), false);
}

void TestStairsShadedStems::testStairsShadedFlag()
{
    auto node = std::make_unique<QImPlotStairsItemNode>();
    QCOMPARE(node->isShaded(), false);

    node->setShaded(true);
    QCOMPARE(node->isShaded(), true);

    node->setShaded(false);
    QCOMPARE(node->isShaded(), false);

    // Redundant set
    node->setShaded(false);
    QCOMPARE(node->isShaded(), false);
}

void TestStairsShadedStems::testStairsSignalDedup()
{
    auto node = std::make_unique<QImPlotStairsItemNode>();
    QSignalSpy spy(node.get(), &QImPlotStairsItemNode::stairsFlagChanged);

    node->setPreStep(true);
    QCOMPARE(spy.count(), 1);

    // Same value - no duplicate signal
    node->setPreStep(true);
    QCOMPARE(spy.count(), 1);

    node->setPreStep(false);
    QCOMPARE(spy.count(), 2);

    node->setShaded(true);
    QCOMPARE(spy.count(), 3);

    // Same value - no duplicate signal
    node->setShaded(true);
    QCOMPARE(spy.count(), 3);
}

// ============================================================================
// QImPlotShadedItemNode Tests
// ============================================================================

void TestStairsShadedStems::testShadedDefaultConstruction()
{
    auto node = std::make_unique<QImPlotShadedItemNode>();
    QVERIFY(node != nullptr);
    QCOMPARE(node->referenceValue(), 0.0);
    QVERIFY(node->color().isValid() == false);
}

void TestStairsShadedStems::testShadedReferenceValue()
{
    auto node = std::make_unique<QImPlotShadedItemNode>();
    QCOMPARE(node->referenceValue(), 0.0);

    node->setReferenceValue(5.0);
    QCOMPARE(node->referenceValue(), 5.0);

    node->setReferenceValue(-3.5);
    QCOMPARE(node->referenceValue(), -3.5);

    // Redundant set
    node->setReferenceValue(-3.5);
    QCOMPARE(node->referenceValue(), -3.5);
}

void TestStairsShadedStems::testShadedColor()
{
    auto node = std::make_unique<QImPlotShadedItemNode>();
    QVERIFY(node->color().isValid() == false);

    QColor c(100, 150, 200);
    node->setColor(c);
    QCOMPARE(node->color(), c);

    // Redundant set
    node->setColor(c);
    QCOMPARE(node->color(), c);
}

void TestStairsShadedStems::testShadedSignalDedup()
{
    auto node = std::make_unique<QImPlotShadedItemNode>();
    QSignalSpy spy(node.get(), &QImPlotShadedItemNode::referenceValueChanged);

    node->setReferenceValue(1.0);
    QCOMPARE(spy.count(), 1);

    // Same value - no duplicate signal
    node->setReferenceValue(1.0);
    QCOMPARE(spy.count(), 1);

    node->setReferenceValue(2.0);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// QImPlotStemsItemNode Tests
// ============================================================================

void TestStairsShadedStems::testStemsDefaultConstruction()
{
    auto node = std::make_unique<QImPlotStemsItemNode>();
    QVERIFY(node != nullptr);
    QCOMPARE(node->referenceValue(), 0.0);
    QVERIFY(!node->isHorizontal());
    QVERIFY(node->color().isValid() == false);
}

void TestStairsShadedStems::testStemsReferenceValue()
{
    auto node = std::make_unique<QImPlotStemsItemNode>();
    QCOMPARE(node->referenceValue(), 0.0);

    node->setReferenceValue(10.0);
    QCOMPARE(node->referenceValue(), 10.0);

    node->setReferenceValue(-2.5);
    QCOMPARE(node->referenceValue(), -2.5);

    // Redundant set
    node->setReferenceValue(-2.5);
    QCOMPARE(node->referenceValue(), -2.5);
}

void TestStairsShadedStems::testStemsHorizontal()
{
    auto node = std::make_unique<QImPlotStemsItemNode>();
    QCOMPARE(node->isHorizontal(), false);

    node->setHorizontal(true);
    QCOMPARE(node->isHorizontal(), true);

    node->setHorizontal(false);
    QCOMPARE(node->isHorizontal(), false);

    // Redundant set
    node->setHorizontal(false);
    QCOMPARE(node->isHorizontal(), false);
}

void TestStairsShadedStems::testStemsColor()
{
    auto node = std::make_unique<QImPlotStemsItemNode>();
    QVERIFY(node->color().isValid() == false);

    QColor c(50, 100, 150);
    node->setColor(c);
    QCOMPARE(node->color(), c);

    // Redundant set
    node->setColor(c);
    QCOMPARE(node->color(), c);
}

void TestStairsShadedStems::testStemsSignalDedup()
{
    auto node = std::make_unique<QImPlotStemsItemNode>();

    // Test referenceValueChanged signal dedup
    QSignalSpy refSpy(node.get(), &QImPlotStemsItemNode::referenceValueChanged);

    node->setReferenceValue(1.0);
    QCOMPARE(refSpy.count(), 1);

    // Same value - no duplicate signal
    node->setReferenceValue(1.0);
    QCOMPARE(refSpy.count(), 1);

    node->setReferenceValue(2.0);
    QCOMPARE(refSpy.count(), 2);

    // Test orientationChanged signal dedup
    QSignalSpy orientSpy(node.get(), &QImPlotStemsItemNode::orientationChanged);

    node->setHorizontal(true);
    QCOMPARE(orientSpy.count(), 1);

    // Same value - no duplicate signal
    node->setHorizontal(true);
    QCOMPARE(orientSpy.count(), 1);

    node->setHorizontal(false);
    QCOMPARE(orientSpy.count(), 2);
}

QTEST_GUILESS_MAIN(TestStairsShadedStems)
#include "tst_qimplotstairs_shaded_stems.moc"
