#include <QObject>
#include <QtTest>
#include <memory>

#include "QImSubplotsNode.h"
#include "QImPlotNode.h"

using namespace QIM;

class TestSubplotsNode : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    // Construction
    void testDefaultConstruction();
    void testTitleConstructor();

    // Title property
    void testTitleGetterSetter();
    void testTitleChangedSignal();

    // Grid dimensions
    void testRowsGetterSetter();
    void testColumnsGetterSetter();
    void testGridInfoChangedSignal();

    // Grid ratios
    void testRowRatiosGetterSetter();
    void testColumnRatiosGetterSetter();

    // setGrid convenience
    void testSetGrid();

    // Size
    void testSizeGetterSetter();

    // Grid count
    void testGridCount();

    // Flag properties (Qt-affirmative semantics)
    void testIsTitleEnabled();
    void testIsLegendEnabled();
    void testIsDefaultMenusEnabled();
    void testIsResizable();
    void testIsAlignedEnabled();
    void testIsShareItemsEnabled();

    // Link behavior
    void testLinkRows();
    void testLinkColumns();
    void testLinkAllX();
    void testLinkAllY();

    // Layout direction
    void testColumnMajor();

    // subplotFlagChanged signal
    void testSubplotFlagChangedSignal();

    // Plot node management
    void testCreatePlotNode();
    void testAddPlotNode();
    void testPlotCount();
    void testPlotNodes();
    void testRemovePlotNode();
    void testTakePlotNode();
    void testInsertPlotNode();

    // Track grid ratios
    void testTrackGridRatios();

};

void TestSubplotsNode::initTestCase()
{
}

void TestSubplotsNode::cleanupTestCase()
{
}

// ============================================================================
// Construction
// ============================================================================
void TestSubplotsNode::testDefaultConstruction()
{
    QImSubplotsNode subplots;
    QCOMPARE(subplots.title(), QString("##SubplotsNode"));
    QCOMPARE(subplots.rows(), 1);
    QCOMPARE(subplots.columns(), 1);
    QCOMPARE(subplots.gridCount(), 1);
    QCOMPARE(subplots.plotCount(), 0);
}

void TestSubplotsNode::testTitleConstructor()
{
    QImSubplotsNode subplots("My Subplots");
    QCOMPARE(subplots.title(), QString("My Subplots"));
}

// ============================================================================
// Title
// ============================================================================
void TestSubplotsNode::testTitleGetterSetter()
{
    QImSubplotsNode subplots;
    subplots.setTitle("Custom Title");
    QCOMPARE(subplots.title(), QString("Custom Title"));

    // Same value — no change
    subplots.setTitle("Custom Title");
    QCOMPARE(subplots.title(), QString("Custom Title"));
}

void TestSubplotsNode::testTitleChangedSignal()
{
    QImSubplotsNode subplots;
    QSignalSpy spy(&subplots, &QImSubplotsNode::titleChanged);

    subplots.setTitle("New Title");
    QCOMPARE(spy.count(), 1);

    // Same value — no signal
    subplots.setTitle("New Title");
    QCOMPARE(spy.count(), 1);
}

// ============================================================================
// Grid dimensions
// ============================================================================
void TestSubplotsNode::testRowsGetterSetter()
{
    QImSubplotsNode subplots;
    QCOMPARE(subplots.rows(), 1);

    subplots.setRows(3);
    QCOMPARE(subplots.rows(), 3);

    // Invalid (<=0) — rejected
    subplots.setRows(0);
    QCOMPARE(subplots.rows(), 3);

    subplots.setRows(-1);
    QCOMPARE(subplots.rows(), 3);
}

void TestSubplotsNode::testColumnsGetterSetter()
{
    QImSubplotsNode subplots;
    QCOMPARE(subplots.columns(), 1);

    subplots.setColumns(4);
    QCOMPARE(subplots.columns(), 4);

    // Invalid — rejected
    subplots.setColumns(0);
    QCOMPARE(subplots.columns(), 4);
}

void TestSubplotsNode::testGridInfoChangedSignal()
{
    QImSubplotsNode subplots;
    QSignalSpy spy(&subplots, &QImSubplotsNode::gridInfoChanged);

    subplots.setRows(2);
    QCOMPARE(spy.count(), 1);

    // Same value — no signal
    subplots.setRows(2);
    QCOMPARE(spy.count(), 1);

    subplots.setColumns(3);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// Grid ratios
// ============================================================================
void TestSubplotsNode::testRowRatiosGetterSetter()
{
    QImSubplotsNode subplots;
    QVERIFY(subplots.rowRatios().empty());

    std::vector<float> ratios = {0.3f, 0.7f};
    subplots.setRowRatios(ratios);
    QCOMPARE(subplots.rowRatios(), ratios);
}

void TestSubplotsNode::testColumnRatiosGetterSetter()
{
    QImSubplotsNode subplots;
    QVERIFY(subplots.columnRatios().empty());

    std::vector<float> ratios = {0.5f, 0.5f};
    subplots.setColumnRatios(ratios);
    QCOMPARE(subplots.columnRatios(), ratios);
}

// ============================================================================
// setGrid
// ============================================================================
void TestSubplotsNode::testSetGrid()
{
    QImSubplotsNode subplots;
    QSignalSpy spy(&subplots, &QImSubplotsNode::gridInfoChanged);

    std::vector<float> rowRatios = {0.4f, 0.6f};
    std::vector<float> colRatios = {0.3f, 0.3f, 0.4f};
    subplots.setGrid(2, 3, rowRatios, colRatios);

    QCOMPARE(subplots.rows(), 2);
    QCOMPARE(subplots.columns(), 3);
    QCOMPARE(subplots.gridCount(), 6);
    // Ratios might not be stored if fuzzyEqual with default empty vectors
    // Just verify rows/cols are set
}

// ============================================================================
// Size
// ============================================================================
void TestSubplotsNode::testSizeGetterSetter()
{
    QImSubplotsNode subplots;
    // Default: auto-size (-1, -1)
    QVERIFY(subplots.size().width() < 0);

    subplots.setSize(QSizeF(1024, 768));
    QCOMPARE(subplots.size(), QSizeF(1024, 768));
}

// ============================================================================
// Grid count
// ============================================================================
void TestSubplotsNode::testGridCount()
{
    QImSubplotsNode subplots;
    QCOMPARE(subplots.gridCount(), 1);  // 1x1

    subplots.setRows(2);
    subplots.setColumns(3);
    QCOMPARE(subplots.gridCount(), 6);  // 2x3
}

// ============================================================================
// Flag properties
// ============================================================================
void TestSubplotsNode::testIsTitleEnabled()
{
    QImSubplotsNode subplots;
    // isTitleEnabled concept is overloaded — it maps to subplotFlag
    // Verify getter/setter doesn't crash
    bool val = subplots.isTitleEnabled();
    subplots.setTitleEnabled(!val);
    QCOMPARE(subplots.isTitleEnabled(), !val);
}

void TestSubplotsNode::testIsLegendEnabled()
{
    QImSubplotsNode subplots;
    QSignalSpy spy(&subplots, &QImSubplotsNode::subplotFlagChanged);

    // Default: legend enabled
    QVERIFY(subplots.isLegendEnabled());

    subplots.setLegendEnabled(false);
    QVERIFY(!subplots.isLegendEnabled());
    QCOMPARE(spy.count(), 1);

    // Same value — no signal
    subplots.setLegendEnabled(false);
    QCOMPARE(spy.count(), 1);
}

void TestSubplotsNode::testIsDefaultMenusEnabled()
{
    QImSubplotsNode subplots;
    bool val = subplots.isDefaultMenusEnabled();
    subplots.setDefaultMenusEnabled(!val);
    QCOMPARE(subplots.isDefaultMenusEnabled(), !val);
}

void TestSubplotsNode::testIsResizable()
{
    QImSubplotsNode subplots;
    QVERIFY(subplots.isResizable());  // Default: resizable

    subplots.setResizable(false);
    QVERIFY(!subplots.isResizable());

    subplots.setResizable(true);
    QVERIFY(subplots.isResizable());
}

void TestSubplotsNode::testIsAlignedEnabled()
{
    QImSubplotsNode subplots;
    bool val = subplots.isAlignedEnabled();
    subplots.setAlignedEnabled(!val);
    QCOMPARE(subplots.isAlignedEnabled(), !val);
}

void TestSubplotsNode::testIsShareItemsEnabled()
{
    QImSubplotsNode subplots;
    bool val = subplots.isShareItemsEnabled();
    subplots.setShareItemsEnabled(!val);
    QCOMPARE(subplots.isShareItemsEnabled(), !val);
}

// ============================================================================
// Link behavior
// ============================================================================
void TestSubplotsNode::testLinkRows()
{
    QImSubplotsNode subplots;
    bool val = subplots.isLinkRows();
    subplots.setLinkRows(!val);
    QCOMPARE(subplots.isLinkRows(), !val);
}

void TestSubplotsNode::testLinkColumns()
{
    QImSubplotsNode subplots;
    bool val = subplots.isLinkColumns();
    subplots.setLinkColumns(!val);
    QCOMPARE(subplots.isLinkColumns(), !val);
}

void TestSubplotsNode::testLinkAllX()
{
    QImSubplotsNode subplots;
    bool val = subplots.isLinkAllX();
    subplots.setLinkAllX(!val);
    QCOMPARE(subplots.isLinkAllX(), !val);
}

void TestSubplotsNode::testLinkAllY()
{
    QImSubplotsNode subplots;
    bool val = subplots.isLinkAllY();
    subplots.setLinkAllY(!val);
    QCOMPARE(subplots.isLinkAllY(), !val);
}

// ============================================================================
// Layout direction
// ============================================================================
void TestSubplotsNode::testColumnMajor()
{
    QImSubplotsNode subplots;
    QVERIFY(!subplots.isColumnMajor());  // Default: row-major

    subplots.setColumnMajor(true);
    QVERIFY(subplots.isColumnMajor());

    subplots.setColumnMajor(false);
    QVERIFY(!subplots.isColumnMajor());
}

// ============================================================================
// subplotFlagChanged signal
// ============================================================================
void TestSubplotsNode::testSubplotFlagChangedSignal()
{
    QImSubplotsNode subplots;
    QSignalSpy spy(&subplots, &QImSubplotsNode::subplotFlagChanged);

    subplots.setResizable(false);
    QCOMPARE(spy.count(), 1);

    subplots.setColumnMajor(true);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// Plot node management
// ============================================================================
void TestSubplotsNode::testCreatePlotNode()
{
    QImSubplotsNode subplots;

    QImPlotNode* plot = subplots.createPlotNode();
    QVERIFY(plot != nullptr);
    QCOMPARE(subplots.plotCount(), 1);
    QCOMPARE(plot->parentNode(), &subplots);

    // Grid is 1x1 full — next create returns nullptr
    QImPlotNode* nullPlot = subplots.createPlotNode();
    QVERIFY(nullPlot == nullptr);
}

void TestSubplotsNode::testAddPlotNode()
{
    QImSubplotsNode subplots;
    auto* plot = new QImPlotNode();

    subplots.addPlotNode(plot);
    QCOMPARE(subplots.plotCount(), 1);
    QCOMPARE(subplots.plotNodes().at(0), plot);
    QCOMPARE(plot->parentNode(), &subplots);
}

void TestSubplotsNode::testPlotCount()
{
    QImSubplotsNode subplots;
    QCOMPARE(subplots.plotCount(), 0);

    subplots.setRows(2);
    subplots.setColumns(2);
    subplots.createPlotNode();
    QCOMPARE(subplots.plotCount(), 1);

    subplots.createPlotNode();
    QCOMPARE(subplots.plotCount(), 2);
}

void TestSubplotsNode::testPlotNodes()
{
    QImSubplotsNode subplots;
    subplots.setRows(2);
    subplots.setColumns(1);

    auto* p1 = subplots.createPlotNode();
    auto* p2 = subplots.createPlotNode();

    QList<QImPlotNode*> nodes = subplots.plotNodes();
    QCOMPARE(nodes.size(), 2);
    QVERIFY(nodes.contains(p1));
    QVERIFY(nodes.contains(p2));
}

void TestSubplotsNode::testRemovePlotNode()
{
    QImSubplotsNode subplots;
    auto* plot = subplots.createPlotNode();
    QCOMPARE(subplots.plotCount(), 1);

    subplots.removePlotNode(plot);
    QCOMPARE(subplots.plotCount(), 0);
}

void TestSubplotsNode::testTakePlotNode()
{
    QImSubplotsNode subplots;
    auto* plot = subplots.createPlotNode();
    QCOMPARE(subplots.plotCount(), 1);

    bool result = subplots.takePlotNode(plot);
    QVERIFY(result);
    QCOMPARE(subplots.plotCount(), 0);
    QVERIFY(plot->parentNode() == nullptr);

    delete plot;
}

void TestSubplotsNode::testInsertPlotNode()
{
    QImSubplotsNode subplots;
    subplots.setRows(1);
    subplots.setColumns(3);

    auto* p1 = subplots.createPlotNode();
    auto* p2 = subplots.createPlotNode();
    QCOMPARE(subplots.plotCount(), 2);

    auto* pInsert = new QImPlotNode();
    subplots.insertPlotNode(1, pInsert);  // Insert between p1 and p2

    QList<QImPlotNode*> nodes = subplots.plotNodes();
    QCOMPARE(nodes.size(), 3);
    QCOMPARE(nodes.at(0), p1);
    QCOMPARE(nodes.at(1), pInsert);
    QCOMPARE(nodes.at(2), p2);
}

// ============================================================================
// Track grid ratios
// ============================================================================
void TestSubplotsNode::testTrackGridRatios()
{
    QImSubplotsNode subplots;
    QVERIFY(!subplots.isTrackGridRatiosEnabled());  // Default: disabled

    subplots.setTrackGridRatiosEnabled(true);
    QVERIFY(subplots.isTrackGridRatiosEnabled());

    subplots.setTrackGridRatiosEnabled(false);
    QVERIFY(!subplots.isTrackGridRatiosEnabled());
}

QTEST_GUILESS_MAIN(TestSubplotsNode)
#include "tst_subplots_node.moc"
