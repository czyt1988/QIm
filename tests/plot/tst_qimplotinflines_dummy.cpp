#include <QObject>
#include <QtTest>
#include <memory>

#include "QImPlotInfLinesItemNode.h"
#include "QImPlotDummyItemNode.h"
#include "../core/TestHelpers.h"

using namespace QIM;

class TestInfLinesDummy : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    // QImPlotInfLinesItemNode tests
    void testInfLinesDefaultConstruction();
    void testInfLinesHorizontal();
    void testInfLinesColor();
    void testInfLinesSignalDedup();

    // QImPlotDummyItemNode tests
    void testDummyDefaultConstruction();
    void testDummyColor();
    void testDummySignalDedup();

private:
};

void TestInfLinesDummy::initTestCase()
{
}

void TestInfLinesDummy::cleanupTestCase()
{
}

// ============================================================================
// QImPlotInfLinesItemNode Tests
// ============================================================================

void TestInfLinesDummy::testInfLinesDefaultConstruction()
{
    auto node = std::make_unique<QImPlotInfLinesItemNode>();
    QVERIFY(node != nullptr);
    QVERIFY(node->isVisible());
    QCOMPARE(node->type(), QImPlotInfLinesItemNode::Type);
    QVERIFY(!node->isHorizontal());
}

void TestInfLinesDummy::testInfLinesHorizontal()
{
    auto node = std::make_unique<QImPlotInfLinesItemNode>();

    QCOMPARE(node->isHorizontal(), false);

    node->setHorizontal(true);
    QCOMPARE(node->isHorizontal(), true);

    node->setHorizontal(false);
    QCOMPARE(node->isHorizontal(), false);

    // Redundant set
    node->setHorizontal(false);
    QCOMPARE(node->isHorizontal(), false);
}

void TestInfLinesDummy::testInfLinesColor()
{
    auto node = std::make_unique<QImPlotInfLinesItemNode>();
    QVERIFY(node->color().isValid() == false);

    QColor c(255, 0, 128);
    node->setColor(c);
    QCOMPARE(node->color(), c);

    // Redundant set
    node->setColor(c);
    QCOMPARE(node->color(), c);
}

void TestInfLinesDummy::testInfLinesSignalDedup()
{
    auto node = std::make_unique<QImPlotInfLinesItemNode>();

    // orientationChanged signal dedup
    QSignalSpy orientSpy(node.get(), &QImPlotInfLinesItemNode::orientationChanged);

    node->setHorizontal(true);
    QCOMPARE(orientSpy.count(), 1);

    // Same value - no duplicate signal
    node->setHorizontal(true);
    QCOMPARE(orientSpy.count(), 1);

    node->setHorizontal(false);
    QCOMPARE(orientSpy.count(), 2);

    // colorChanged signal dedup
    QSignalSpy colorSpy(node.get(), &QImPlotInfLinesItemNode::colorChanged);

    QColor c(0, 128, 255);
    node->setColor(c);
    QCOMPARE(colorSpy.count(), 1);

    // Same value - no duplicate signal
    node->setColor(c);
    QCOMPARE(colorSpy.count(), 1);
}

// ============================================================================
// QImPlotDummyItemNode Tests
// ============================================================================

void TestInfLinesDummy::testDummyDefaultConstruction()
{
    auto node = std::make_unique<QImPlotDummyItemNode>();
    QVERIFY(node != nullptr);
    QVERIFY(node->isVisible());
    QCOMPARE(node->type(), QImPlotDummyItemNode::Type);
}

void TestInfLinesDummy::testDummyColor()
{
    auto node = std::make_unique<QImPlotDummyItemNode>();

    QColor c(100, 200, 50);
    node->setColor(c);
    QCOMPARE(node->color(), c);

    // Redundant set
    node->setColor(c);
    QCOMPARE(node->color(), c);
}

void TestInfLinesDummy::testDummySignalDedup()
{
    auto node = std::make_unique<QImPlotDummyItemNode>();
    QSignalSpy spy(node.get(), &QImPlotDummyItemNode::colorChanged);

    QColor c(10, 20, 30);
    node->setColor(c);
    QCOMPARE(spy.count(), 1);

    // Same value - no duplicate signal
    node->setColor(c);
    QCOMPARE(spy.count(), 1);

    node->setColor(QColor(40, 50, 60));
    QCOMPARE(spy.count(), 2);
}

QTEST_GUILESS_MAIN(TestInfLinesDummy)
#include "tst_qimplotinflines_dummy.moc"
