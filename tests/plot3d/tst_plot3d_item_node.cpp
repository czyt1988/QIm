#include <QObject>
#include <QtTest>
#include <memory>

#include "QImPlot3DItemNode.h"
#include "QImPlot3DScatterItemNode.h"
#include "QImPlot3DNode.h"

using namespace QIM;

class TestPlot3DItemNode : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    // Construction
    void testDefaultConstruction();

    // Label property
    void testLabelGetterSetter();
    void testLabelChangedSignal();
    void testLabelConstData();

    // Plot node access
    void testPlot3DNodeWhenOrphan();
    void testPlot3DNodeWhenChild();

    // Item flags (positive semantics from negative flags)
    void testLegendEntryEnabled();
    void testFitEnabled();

    // Flag changed signals
    void testLegendEntryEnabledChangedSignal();
    void testFitEnabledChangedSignal();

    // Visibility
    void testDefaultVisible();
    void testSetVisible();

    // Type identification
    void testConcreteType();

    // Inherited properties
    void testInheritedAutoIdDisabled();
};

void TestPlot3DItemNode::initTestCase()
{
}

void TestPlot3DItemNode::cleanupTestCase()
{
}

// ============================================================================
// Construction
// ============================================================================
void TestPlot3DItemNode::testDefaultConstruction()
{
    // QImPlot3DItemNode is abstract; test via concrete QImPlot3DScatterItemNode
    auto node = std::make_unique<QImPlot3DScatterItemNode>();

    QVERIFY(node->label().isEmpty());
    QVERIFY(node->isVisible());
}

// ============================================================================
// Label
// ============================================================================
void TestPlot3DItemNode::testLabelGetterSetter()
{
    auto node = std::make_unique<QImPlot3DScatterItemNode>();

    node->setLabel("Scatter Points");
    QCOMPARE(node->label(), QString("Scatter Points"));

    node->setLabel("Updated Scatter");
    QCOMPARE(node->label(), QString("Updated Scatter"));

    node->setLabel("");
    QVERIFY(node->label().isEmpty());
}

void TestPlot3DItemNode::testLabelChangedSignal()
{
    auto node = std::make_unique<QImPlot3DScatterItemNode>();
    QSignalSpy spy(node.get(), &QImPlot3DItemNode::labelChanged);

    node->setLabel("First");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toString(), QString("First"));

    // Same value — no signal
    node->setLabel("First");
    QCOMPARE(spy.count(), 1);

    node->setLabel("Second");
    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.at(1).at(0).toString(), QString("Second"));
}

void TestPlot3DItemNode::testLabelConstData()
{
    auto node = std::make_unique<QImPlot3DScatterItemNode>();

    // Default empty label → returns "##plot3dItem"
    QCOMPARE(QString(node->labelConstData()), QString("##plot3dItem"));

    node->setLabel("Test3D");
    QCOMPARE(QString(node->labelConstData()), QString("Test3D"));
}

// ============================================================================
// Plot node access
// ============================================================================
void TestPlot3DItemNode::testPlot3DNodeWhenOrphan()
{
    auto node = std::make_unique<QImPlot3DScatterItemNode>();
    QVERIFY(node->plot3DNode() == nullptr);
}

void TestPlot3DItemNode::testPlot3DNodeWhenChild()
{
    QImPlot3DNode plot3d;
    auto* scatter = new QImPlot3DScatterItemNode(&plot3d);
    QCOMPARE(scatter->plot3DNode(), &plot3d);
}

// ============================================================================
// Item flags
// ============================================================================
void TestPlot3DItemNode::testLegendEntryEnabled()
{
    auto node = std::make_unique<QImPlot3DScatterItemNode>();

    // Default: legend entry enabled (NoLegend NOT set)
    QVERIFY(node->isLegendEntryEnabled());

    node->setLegendEntryEnabled(false);
    QVERIFY(!node->isLegendEntryEnabled());

    node->setLegendEntryEnabled(true);
    QVERIFY(node->isLegendEntryEnabled());
}

void TestPlot3DItemNode::testFitEnabled()
{
    auto node = std::make_unique<QImPlot3DScatterItemNode>();

    // Default: fit enabled (NoFit NOT set)
    QVERIFY(node->isFitEnabled());

    node->setFitEnabled(false);
    QVERIFY(!node->isFitEnabled());

    node->setFitEnabled(true);
    QVERIFY(node->isFitEnabled());
}

// ============================================================================
// Flag changed signals
// ============================================================================
void TestPlot3DItemNode::testLegendEntryEnabledChangedSignal()
{
    auto node = std::make_unique<QImPlot3DScatterItemNode>();
    QSignalSpy spy(node.get(), &QImPlot3DItemNode::legendEntryEnabledChanged);

    node->setLegendEntryEnabled(false);
    QCOMPARE(spy.count(), 1);

    // Same value — no signal
    node->setLegendEntryEnabled(false);
    QCOMPARE(spy.count(), 1);

    node->setLegendEntryEnabled(true);
    QCOMPARE(spy.count(), 2);
}

void TestPlot3DItemNode::testFitEnabledChangedSignal()
{
    auto node = std::make_unique<QImPlot3DScatterItemNode>();
    QSignalSpy spy(node.get(), &QImPlot3DItemNode::fitEnabledChanged);

    node->setFitEnabled(false);
    QCOMPARE(spy.count(), 1);

    node->setFitEnabled(false);
    QCOMPARE(spy.count(), 1);

    node->setFitEnabled(true);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// Visibility
// ============================================================================
void TestPlot3DItemNode::testDefaultVisible()
{
    auto node = std::make_unique<QImPlot3DScatterItemNode>();
    QVERIFY(node->isVisible());
}

void TestPlot3DItemNode::testSetVisible()
{
    auto node = std::make_unique<QImPlot3DScatterItemNode>();

    node->setVisible(false);
    QVERIFY(!node->isVisible());

    node->setVisible(true);
    QVERIFY(node->isVisible());
}

// ============================================================================
// Type identification
// ============================================================================
void TestPlot3DItemNode::testConcreteType()
{
    auto node = std::make_unique<QImPlot3DScatterItemNode>();
    int t = node->type();
    QVERIFY(t >= QImPlot3DItemNode::InnerType3D);
    QVERIFY(t != 0);
}

// ============================================================================
// Inherited properties
// ============================================================================
void TestPlot3DItemNode::testInheritedAutoIdDisabled()
{
    auto node = std::make_unique<QImPlot3DScatterItemNode>();
    // QImPlot3DItemNode constructor sets auto-id disabled
    QVERIFY(!node->isAutoIdEnabled());
}

QTEST_GUILESS_MAIN(TestPlot3DItemNode)
#include "tst_plot3d_item_node.moc"
