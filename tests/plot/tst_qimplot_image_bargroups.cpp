#include <QObject>
#include <QtTest>
#include <memory>

#include "QImPlotImageItemNode.h"
#include "QImPlotBarGroupsItemNode.h"
#include "../core/TestHelpers.h"

using namespace QIM;

class TestQImPlotImageBarGroups : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    // ===================== QImPlotImageItemNode =====================
    void testImageDefaultConstruction();
    void testImageUv0GetterSetter();
    void testImageUv0ChangedSignal();
    void testImageUv1GetterSetter();
    void testImageUv1ChangedSignal();
    void testImageTintColorGetterSetter();
    void testImageTintColorChangedSignal();
    void testImageUv0SignalDedup();

    // ===================== QImPlotBarGroupsItemNode =====================
    void testBarGroupsDefaultConstruction();
    void testBarGroupsGroupWidthGetterSetter();
    void testBarGroupsGroupWidthChangedSignal();
    void testBarGroupsHorizontalGetterSetter();
    void testBarGroupsOrientationChangedSignal();
    void testBarGroupsStackedGetterSetter();
    void testBarGroupsStackedChangedSignal();
    void testBarGroupsShiftGetterSetter();
    void testBarGroupsShiftChangedSignal();
    void testBarGroupsColorGetterSetter();
    void testBarGroupsColorChangedSignal();
    void testBarGroupsGroupWidthSignalDedup();

private:
    std::unique_ptr<QImPlotImageItemNode> makeImageNode()
    {
        return std::make_unique<QImPlotImageItemNode>();
    }

    std::unique_ptr<QImPlotBarGroupsItemNode> makeBarGroupsNode()
    {
        return std::make_unique<QImPlotBarGroupsItemNode>();
    }
};

void TestQImPlotImageBarGroups::initTestCase()
{
}

void TestQImPlotImageBarGroups::cleanupTestCase()
{
}

// ============================================================================
// QImPlotImageItemNode — Construction
// ============================================================================
void TestQImPlotImageBarGroups::testImageDefaultConstruction()
{
    auto node = makeImageNode();
    QVERIFY(node != nullptr);
    QVERIFY(node->isVisible());
    QCOMPARE(node->type(), QImPlotImageItemNode::Type);
}

// ============================================================================
// QImPlotImageItemNode — uv0
// ============================================================================
void TestQImPlotImageBarGroups::testImageUv0GetterSetter()
{
    auto node = makeImageNode();

    // Set and verify
    node->setUv0(QPointF(0.25, 0.75));
    QCOMPARE(node->uv0(), QPointF(0.25, 0.75));

    // Redundant set (same value)
    node->setUv0(QPointF(0.25, 0.75));
    QCOMPARE(node->uv0(), QPointF(0.25, 0.75));
}

void TestQImPlotImageBarGroups::testImageUv0ChangedSignal()
{
    auto node = makeImageNode();
    QSignalSpy spy(node.get(), &QImPlotImageItemNode::uv0Changed);

    node->setUv0(QPointF(0.1, 0.2));
    QCOMPARE(spy.count(), 1);

    node->setUv0(QPointF(0.3, 0.4));
    QCOMPARE(spy.count(), 2);
}

void TestQImPlotImageBarGroups::testImageUv0SignalDedup()
{
    auto node = makeImageNode();
    QSignalSpy spy(node.get(), &QImPlotImageItemNode::uv0Changed);

    node->setUv0(QPointF(0.5, 0.5));
    QCOMPARE(spy.count(), 1);

    // Same value — no duplicate signal
    node->setUv0(QPointF(0.5, 0.5));
    QCOMPARE(spy.count(), 1);
}

// ============================================================================
// QImPlotImageItemNode — uv1
// ============================================================================
void TestQImPlotImageBarGroups::testImageUv1GetterSetter()
{
    auto node = makeImageNode();

    node->setUv1(QPointF(1.0, 0.0));
    QCOMPARE(node->uv1(), QPointF(1.0, 0.0));

    // Redundant set
    node->setUv1(QPointF(1.0, 0.0));
    QCOMPARE(node->uv1(), QPointF(1.0, 0.0));
}

void TestQImPlotImageBarGroups::testImageUv1ChangedSignal()
{
    auto node = makeImageNode();
    QSignalSpy spy(node.get(), &QImPlotImageItemNode::uv1Changed);

    node->setUv1(QPointF(0.8, 0.9));
    QCOMPARE(spy.count(), 1);
}

// ============================================================================
// QImPlotImageItemNode — tintColor
// ============================================================================
void TestQImPlotImageBarGroups::testImageTintColorGetterSetter()
{
    auto node = makeImageNode();

    node->setTintColor(QColor(255, 128, 0, 200));
    QCOMPARE(node->tintColor(), QColor(255, 128, 0, 200));

    // Redundant set
    node->setTintColor(QColor(255, 128, 0, 200));
    QCOMPARE(node->tintColor(), QColor(255, 128, 0, 200));
}

void TestQImPlotImageBarGroups::testImageTintColorChangedSignal()
{
    auto node = makeImageNode();
    QSignalSpy spy(node.get(), &QImPlotImageItemNode::tintColorChanged);

    node->setTintColor(QColor(100, 200, 50));
    QCOMPARE(spy.count(), 1);

    node->setTintColor(QColor(10, 20, 30));
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// QImPlotBarGroupsItemNode — Construction
// ============================================================================
void TestQImPlotImageBarGroups::testBarGroupsDefaultConstruction()
{
    auto node = makeBarGroupsNode();
    QVERIFY(node != nullptr);
    QVERIFY(node->isVisible());
    QCOMPARE(node->type(), QImPlotBarGroupsItemNode::Type);
}

// ============================================================================
// QImPlotBarGroupsItemNode — groupWidth
// ============================================================================
void TestQImPlotImageBarGroups::testBarGroupsGroupWidthGetterSetter()
{
    auto node = makeBarGroupsNode();

    // Default should be > 0
    QVERIFY(node->groupWidth() > 0.0);

    node->setGroupWidth(2.5);
    QCOMPARE(node->groupWidth(), 2.5);

    // Redundant set
    node->setGroupWidth(2.5);
    QCOMPARE(node->groupWidth(), 2.5);
}

void TestQImPlotImageBarGroups::testBarGroupsGroupWidthChangedSignal()
{
    auto node = makeBarGroupsNode();
    QSignalSpy spy(node.get(), &QImPlotBarGroupsItemNode::groupWidthChanged);

    node->setGroupWidth(1.5);
    QCOMPARE(spy.count(), 1);

    node->setGroupWidth(3.0);
    QCOMPARE(spy.count(), 2);
}

void TestQImPlotImageBarGroups::testBarGroupsGroupWidthSignalDedup()
{
    auto node = makeBarGroupsNode();
    QSignalSpy spy(node.get(), &QImPlotBarGroupsItemNode::groupWidthChanged);

    node->setGroupWidth(1.8);
    QCOMPARE(spy.count(), 1);

    // Same value — no duplicate signal
    node->setGroupWidth(1.8);
    QCOMPARE(spy.count(), 1);
}

// ============================================================================
// QImPlotBarGroupsItemNode — horizontal
// ============================================================================
void TestQImPlotImageBarGroups::testBarGroupsHorizontalGetterSetter()
{
    auto node = makeBarGroupsNode();

    // Default: vertical
    QVERIFY(!node->isHorizontal());

    node->setHorizontal(true);
    QVERIFY(node->isHorizontal());

    node->setHorizontal(false);
    QVERIFY(!node->isHorizontal());
}

void TestQImPlotImageBarGroups::testBarGroupsOrientationChangedSignal()
{
    auto node = makeBarGroupsNode();
    QSignalSpy spy(node.get(), &QImPlotBarGroupsItemNode::orientationChanged);

    node->setHorizontal(true);
    QCOMPARE(spy.count(), 1);

    node->setHorizontal(false);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// QImPlotBarGroupsItemNode — stacked
// ============================================================================
void TestQImPlotImageBarGroups::testBarGroupsStackedGetterSetter()
{
    auto node = makeBarGroupsNode();

    // Default: not stacked
    QVERIFY(!node->isStacked());

    node->setStacked(true);
    QVERIFY(node->isStacked());

    node->setStacked(false);
    QVERIFY(!node->isStacked());
}

void TestQImPlotImageBarGroups::testBarGroupsStackedChangedSignal()
{
    auto node = makeBarGroupsNode();
    QSignalSpy spy(node.get(), &QImPlotBarGroupsItemNode::stackedChanged);

    node->setStacked(true);
    QCOMPARE(spy.count(), 1);

    node->setStacked(true);
    QCOMPARE(spy.count(), 1); // dedup: same value
}

// ============================================================================
// QImPlotBarGroupsItemNode — shift
// ============================================================================
void TestQImPlotImageBarGroups::testBarGroupsShiftGetterSetter()
{
    auto node = makeBarGroupsNode();

    node->setShift(1.5);
    QCOMPARE(node->shift(), 1.5);

    node->setShift(-0.5);
    QCOMPARE(node->shift(), -0.5);
}

void TestQImPlotImageBarGroups::testBarGroupsShiftChangedSignal()
{
    auto node = makeBarGroupsNode();
    QSignalSpy spy(node.get(), &QImPlotBarGroupsItemNode::shiftChanged);

    node->setShift(2.0);
    QCOMPARE(spy.count(), 1);
}

// ============================================================================
// QImPlotBarGroupsItemNode — color
// ============================================================================
void TestQImPlotImageBarGroups::testBarGroupsColorGetterSetter()
{
    auto node = makeBarGroupsNode();

    node->setColor(QColor(60, 179, 113, 220));
    QCOMPARE(node->color(), QColor(60, 179, 113, 220));

    // Redundant set
    node->setColor(QColor(60, 179, 113, 220));
    QCOMPARE(node->color(), QColor(60, 179, 113, 220));
}

void TestQImPlotImageBarGroups::testBarGroupsColorChangedSignal()
{
    auto node = makeBarGroupsNode();
    QSignalSpy spy(node.get(), &QImPlotBarGroupsItemNode::colorChanged);

    node->setColor(QColor(255, 0, 0));
    QCOMPARE(spy.count(), 1);

    node->setColor(QColor(0, 0, 255));
    QCOMPARE(spy.count(), 2);
}

QTEST_GUILESS_MAIN(TestQImPlotImageBarGroups)
#include "tst_qimplot_image_bargroups.moc"
