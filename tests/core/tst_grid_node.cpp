#include <QObject>
#include <QtTest>

#include "QImGridNode.h"

using namespace QIM;

class TestGridNode : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    // Default construction
    void testDefaultConstruction();
    // Title property
    void testTitleProperty();
    // titleChanged signal
    void testTitleChangedSignal();
    // rows property getter/setter
    void testRowsProperty();
    // columns property getter/setter
    void testColumnsProperty();
    // gridInfoChanged signal for rows
    void testGridInfoChangedRowsSignal();
    // gridInfoChanged signal for columns
    void testGridInfoChangedColumnsSignal();
    // setGrid sets both rows and columns
    void testSetGrid();
    // Default values
    void testDefaultValues();
    // size property
    void testSizeProperty();
    // sizeChanged signal
    void testSizeChangedSignal();
    // gridCount returns rows * columns
    void testGridCount();
    // cellCount returns number of CellNode children
    void testCellCount();
    // createCellNode creates CellNode with correct subplot index
    void testCreateCellNode();
    // cellNodes returns all CellNode children
    void testCellNodes();
    // Negative/invalid rows/columns are rejected
    void testInvalidRowsColumns();
};

void TestGridNode::initTestCase()
{
}

void TestGridNode::cleanupTestCase()
{
}

// ============================================================================
// Test 1: Default construction
// ============================================================================
void TestGridNode::testDefaultConstruction()
{
    QImGridNode grid;
    // Default title is "##Grid"
    QVERIFY(!grid.title().isEmpty());
    QCOMPARE(grid.rows(), 1);
    QCOMPARE(grid.columns(), 1);
    QCOMPARE(grid.gridCount(), 1);
    QCOMPARE(grid.cellCount(), 0);
}

// ============================================================================
// Test 2: Title property read/write
// ============================================================================
void TestGridNode::testTitleProperty()
{
    QImGridNode grid;

    // Default title contains "Grid"
    QVERIFY(grid.title().contains("Grid"));

    grid.setTitle("My Custom Grid");
    QCOMPARE(grid.title(), QString("My Custom Grid"));

    grid.setTitle("Another Title");
    QCOMPARE(grid.title(), QString("Another Title"));
}

// ============================================================================
// Test 3: titleChanged signal
// ============================================================================
void TestGridNode::testTitleChangedSignal()
{
    QImGridNode grid;
    QSignalSpy spy(&grid, &QImGridNode::titleChanged);

    grid.setTitle("New Title");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toString(), QString("New Title"));

    // Same title — no signal
    grid.setTitle("New Title");
    QCOMPARE(spy.count(), 1);
}

// ============================================================================
// Test 4: rows getter/setter
// ============================================================================
void TestGridNode::testRowsProperty()
{
    QImGridNode grid;

    QCOMPARE(grid.rows(), 1);

    grid.setRows(3);
    QCOMPARE(grid.rows(), 3);

    grid.setRows(5);
    QCOMPARE(grid.rows(), 5);

    // Same value — no change
    grid.setRows(5);
    QCOMPARE(grid.rows(), 5);
}

// ============================================================================
// Test 5: columns getter/setter
// ============================================================================
void TestGridNode::testColumnsProperty()
{
    QImGridNode grid;

    QCOMPARE(grid.columns(), 1);

    grid.setColumns(2);
    QCOMPARE(grid.columns(), 2);

    grid.setColumns(4);
    QCOMPARE(grid.columns(), 4);

    // Same value — no change
    grid.setColumns(4);
    QCOMPARE(grid.columns(), 4);
}

// ============================================================================
// Test 6: gridInfoChanged signal when rows change
// ============================================================================
void TestGridNode::testGridInfoChangedRowsSignal()
{
    QImGridNode grid;
    QSignalSpy spy(&grid, &QImGridNode::gridInfoChanged);

    grid.setRows(2);
    QCOMPARE(spy.count(), 1);

    // Same rows — no signal
    grid.setRows(2);
    QCOMPARE(spy.count(), 1);

    // Invalid row (0 or negative) — no change, no signal
    grid.setRows(0);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(grid.rows(), 2);  // Unchanged
}

// ============================================================================
// Test 7: gridInfoChanged signal when columns change
// ============================================================================
void TestGridNode::testGridInfoChangedColumnsSignal()
{
    QImGridNode grid;
    QSignalSpy spy(&grid, &QImGridNode::gridInfoChanged);

    grid.setColumns(3);
    QCOMPARE(spy.count(), 1);

    // Same columns — no signal
    grid.setColumns(3);
    QCOMPARE(spy.count(), 1);

    // Invalid column (negative) — no change, no signal
    grid.setColumns(-1);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(grid.columns(), 3);  // Unchanged
}

// ============================================================================
// Test 8: setGrid sets both rows and columns
// ============================================================================
void TestGridNode::testSetGrid()
{
    QImGridNode grid;
    QSignalSpy spy(&grid, &QImGridNode::gridInfoChanged);

    grid.setGrid(3, 4);
    QCOMPARE(grid.rows(), 3);
    QCOMPARE(grid.columns(), 4);
    QCOMPARE(grid.gridCount(), 12);
    // setGrid uses QSignalBlocker internally, then emits gridInfoChanged once
    QCOMPARE(spy.count(), 1);
}

// ============================================================================
// Test 9: Default values after construction
// ============================================================================
void TestGridNode::testDefaultValues()
{
    QImGridNode grid;

    QCOMPARE(grid.rows(), 1);
    QCOMPARE(grid.columns(), 1);
    QCOMPARE(grid.gridCount(), 1);
    QCOMPARE(grid.cellCount(), 0);

    // Default size is (-1, -1) meaning auto-fill
    QSizeF sz = grid.size();
    QCOMPARE(sz.width(), -1.0);
    QCOMPARE(sz.height(), -1.0);
}

// ============================================================================
// Test 10: size property
// ============================================================================
void TestGridNode::testSizeProperty()
{
    QImGridNode grid;

    QSizeF defaultSize = grid.size();
    QCOMPARE(defaultSize.width(), -1.0);
    QCOMPARE(defaultSize.height(), -1.0);

    QSizeF newSize(800.0, 600.0);
    grid.setSize(newSize);
    QSizeF retrieved = grid.size();
    QVERIFY(qFuzzyCompare(retrieved.width(), 800.0));
    QVERIFY(qFuzzyCompare(retrieved.height(), 600.0));
}

// ============================================================================
// Test 11: sizeChanged signal
// ============================================================================
void TestGridNode::testSizeChangedSignal()
{
    QImGridNode grid;
    QSignalSpy spy(&grid, &QImGridNode::sizeChanged);

    QSizeF newSize(1024.0, 768.0);
    grid.setSize(newSize);

    QCOMPARE(spy.count(), 1);
    QSizeF emitted = spy.at(0).at(0).value<QSizeF>();
    QVERIFY(qFuzzyCompare(emitted.width(), 1024.0));
    QVERIFY(qFuzzyCompare(emitted.height(), 768.0));

    // Same size — no signal
    grid.setSize(newSize);
    QCOMPARE(spy.count(), 1);
}

// ============================================================================
// Test 12: gridCount returns rows * columns
// ============================================================================
void TestGridNode::testGridCount()
{
    QImGridNode grid;

    grid.setRows(2);
    grid.setColumns(3);
    QCOMPARE(grid.gridCount(), 6);

    grid.setGrid(4, 5);
    QCOMPARE(grid.gridCount(), 20);
}

// ============================================================================
// Test 13: cellCount returns number of CellNode children
// ============================================================================
void TestGridNode::testCellCount()
{
    QImGridNode grid;

    QCOMPARE(grid.cellCount(), 0);

    grid.createCellNode();
    QCOMPARE(grid.cellCount(), 1);

    grid.createCellNode();
    grid.createCellNode();
    QCOMPARE(grid.cellCount(), 3);
}

// ============================================================================
// Test 14: createCellNode creates CellNode with sequential index
// ============================================================================
void TestGridNode::testCreateCellNode()
{
    QImGridNode grid;
    QSignalSpy addedSpy(&grid, &QImGridNode::childNodeAdded);

    auto* cell = grid.createCellNode();
    QVERIFY(cell != nullptr);
    QCOMPARE(addedSpy.count(), 1);

    // CellNode should be a child of the grid
    QCOMPARE(cell->parentNode(), &grid);
    QCOMPARE(grid.childNodeCount(), 1);

    // Second cell
    auto* cell2 = grid.createCellNode();
    QVERIFY(cell2 != nullptr);
    QCOMPARE(grid.childNodeCount(), 2);
    QVERIFY(cell != cell2);
}

// ============================================================================
// Test 15: cellNodes returns all CellNode children in order
// ============================================================================
void TestGridNode::testCellNodes()
{
    QImGridNode grid;

    auto* c1 = grid.createCellNode();
    auto* c2 = grid.createCellNode();
    auto* c3 = grid.createCellNode();

    QList<QImGridNode::CellNode*> cells = grid.cellNodes();
    QCOMPARE(cells.size(), 3);
    QCOMPARE(cells.at(0), c1);
    QCOMPARE(cells.at(1), c2);
    QCOMPARE(cells.at(2), c3);
}

// ============================================================================
// Test 16: Invalid rows/columns values are rejected
// ============================================================================
void TestGridNode::testInvalidRowsColumns()
{
    QImGridNode grid;
    grid.setRows(5);
    grid.setColumns(5);

    // Zero rows — should be rejected
    grid.setRows(0);
    QCOMPARE(grid.rows(), 5);

    // Negative rows — should be rejected
    grid.setRows(-3);
    QCOMPARE(grid.rows(), 5);

    // Zero columns — should be rejected
    grid.setColumns(0);
    QCOMPARE(grid.columns(), 5);

    // Negative columns — should be rejected
    grid.setColumns(-1);
    QCOMPARE(grid.columns(), 5);
}

QTEST_MAIN(TestGridNode)
#include "tst_grid_node.moc"
