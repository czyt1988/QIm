#include <QObject>
#include <QtTest>
#include <memory>

#include "QImSubplots3DNode.h"
#include "QImPlot3DNode.h"
#include "QImGridNode.h"

using namespace QIM;

class TestSubplots3DNode : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    // Construction
    void testDefaultConstruction();
    void testTitleConstructor();

    // Title (inherited from QImGridNode)
    void testTitleGetterSetter();
    void testTitleChangedSignal();

    // Grid dimensions (inherited)
    void testRowsGetterSetter();
    void testColumnsGetterSetter();
    void testGridInfoChangedSignal();

    // Grid count (inherited)
    void testGridCount();

    // Size (inherited)
    void testSizeGetterSetter();

    // 3D plot node management
    void testCreatePlotNode();
    void testPlotCount();
    void testPlotNodes();

    // Inherited from QImAbstractNode
    void testDefaultVisibleEnabled();
    void testChildManagement();

};

void TestSubplots3DNode::initTestCase()
{
}

void TestSubplots3DNode::cleanupTestCase()
{
}

// ============================================================================
// Construction
// ============================================================================
void TestSubplots3DNode::testDefaultConstruction()
{
    QImSubplots3DNode subplots3d;
    QCOMPARE(subplots3d.title(), QString("##Subplots3D"));
    QCOMPARE(subplots3d.rows(), 1);
    QCOMPARE(subplots3d.columns(), 1);
    QCOMPARE(subplots3d.gridCount(), 1);
    QCOMPARE(subplots3d.plotCount(), 0);
}

void TestSubplots3DNode::testTitleConstructor()
{
    QImSubplots3DNode subplots3d("3D Grid");
    QCOMPARE(subplots3d.title(), QString("3D Grid"));
}

// ============================================================================
// Title (inherited)
// ============================================================================
void TestSubplots3DNode::testTitleGetterSetter()
{
    QImSubplots3DNode subplots3d;
    subplots3d.setTitle("Custom 3D Grid");
    QCOMPARE(subplots3d.title(), QString("Custom 3D Grid"));

    // Same value
    subplots3d.setTitle("Custom 3D Grid");
    QCOMPARE(subplots3d.title(), QString("Custom 3D Grid"));
}

void TestSubplots3DNode::testTitleChangedSignal()
{
    QImSubplots3DNode subplots3d;
    QSignalSpy spy(&subplots3d, &QImSubplots3DNode::titleChanged);

    subplots3d.setTitle("New Grid");
    QCOMPARE(spy.count(), 1);

    // Same value — no signal
    subplots3d.setTitle("New Grid");
    QCOMPARE(spy.count(), 1);
}

// ============================================================================
// Grid dimensions (inherited)
// ============================================================================
void TestSubplots3DNode::testRowsGetterSetter()
{
    QImSubplots3DNode subplots3d;
    QCOMPARE(subplots3d.rows(), 1);

    subplots3d.setRows(3);
    QCOMPARE(subplots3d.rows(), 3);

    // Invalid — rejected
    subplots3d.setRows(0);
    QCOMPARE(subplots3d.rows(), 3);
}

void TestSubplots3DNode::testColumnsGetterSetter()
{
    QImSubplots3DNode subplots3d;
    QCOMPARE(subplots3d.columns(), 1);

    subplots3d.setColumns(2);
    QCOMPARE(subplots3d.columns(), 2);

    // Invalid — rejected
    subplots3d.setColumns(-1);
    QCOMPARE(subplots3d.columns(), 2);
}

void TestSubplots3DNode::testGridInfoChangedSignal()
{
    QImSubplots3DNode subplots3d;
    QSignalSpy spy(&subplots3d, &QImSubplots3DNode::gridInfoChanged);

    subplots3d.setRows(2);
    QCOMPARE(spy.count(), 1);

    // Same value — no signal
    subplots3d.setRows(2);
    QCOMPARE(spy.count(), 1);

    subplots3d.setColumns(3);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// Grid count
// ============================================================================
void TestSubplots3DNode::testGridCount()
{
    QImSubplots3DNode subplots3d;
    QCOMPARE(subplots3d.gridCount(), 1);

    subplots3d.setRows(3);
    subplots3d.setColumns(4);
    QCOMPARE(subplots3d.gridCount(), 12);
}

// ============================================================================
// Size (inherited)
// ============================================================================
void TestSubplots3DNode::testSizeGetterSetter()
{
    QImSubplots3DNode subplots3d;
    // Default: auto-size (-1, -1)
    QVERIFY(subplots3d.size().width() < 0);

    subplots3d.setSize(QSizeF(800, 600));
    QCOMPARE(subplots3d.size(), QSizeF(800, 600));
}

// ============================================================================
// 3D plot node management
// ============================================================================
void TestSubplots3DNode::testCreatePlotNode()
{
    QImSubplots3DNode subplots3d;
    subplots3d.setRows(1);
    subplots3d.setColumns(2);

    QImPlot3DNode* plot1 = subplots3d.createPlotNode();
    QVERIFY(plot1 != nullptr);
    QCOMPARE(subplots3d.plotCount(), 1);

    QImPlot3DNode* plot2 = subplots3d.createPlotNode();
    QVERIFY(plot2 != nullptr);
    QCOMPARE(subplots3d.plotCount(), 2);

    // Grid full — returns nullptr
    QImPlot3DNode* nullPlot = subplots3d.createPlotNode();
    QVERIFY(nullPlot == nullptr);
    QCOMPARE(subplots3d.plotCount(), 2);
}

void TestSubplots3DNode::testPlotCount()
{
    QImSubplots3DNode subplots3d;
    QCOMPARE(subplots3d.plotCount(), 0);

    subplots3d.setRows(2);
    subplots3d.setColumns(1);
    subplots3d.createPlotNode();
    QCOMPARE(subplots3d.plotCount(), 1);

    subplots3d.createPlotNode();
    QCOMPARE(subplots3d.plotCount(), 2);
}

void TestSubplots3DNode::testPlotNodes()
{
    QImSubplots3DNode subplots3d;
    subplots3d.setRows(1);
    subplots3d.setColumns(3);

    auto* p1 = subplots3d.createPlotNode();
    auto* p2 = subplots3d.createPlotNode();
    auto* p3 = subplots3d.createPlotNode();

    QList<QImPlot3DNode*> nodes = subplots3d.plotNodes();
    QCOMPARE(nodes.size(), 3);
    QVERIFY(nodes.contains(p1));
    QVERIFY(nodes.contains(p2));
    QVERIFY(nodes.contains(p3));
}

// ============================================================================
// Inherited from QImAbstractNode
// ============================================================================
void TestSubplots3DNode::testDefaultVisibleEnabled()
{
    QImSubplots3DNode subplots3d;
    QVERIFY(subplots3d.isVisible());
    QVERIFY(subplots3d.isEnabled());
}

void TestSubplots3DNode::testChildManagement()
{
    QImSubplots3DNode subplots3d;
    auto* plot = subplots3d.createPlotNode();
    QVERIFY(plot != nullptr);

    // The plot is a grandchild (CellNode → QImPlot3DNode)
    // Child of subplots3d is CellNode
    QCOMPARE(subplots3d.childNodeCount(), 1);
    QVERIFY(subplots3d.childNodeAt(0) != nullptr);
}

QTEST_GUILESS_MAIN(TestSubplots3DNode)
#include "tst_subplots3d_node.moc"
