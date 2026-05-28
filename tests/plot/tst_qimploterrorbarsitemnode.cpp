#include <QObject>
#include <QtTest>
#include <memory>

#include "QImPlotErrorBarsItemNode.h"
#include "QImPlotErrorDataSeries.h"
#include "../core/TestHelpers.h"

using namespace QIM;

class TestQImPlotErrorBarsItemNode : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    // Construction
    void testDefaultConstruction();

    // horizontal property
    void testHorizontalGetterSetter();
    void testOrientationChangedSignal();
    void testOrientationSignalDedu();

    // color property
    void testColorGetterSetter();
    void testColorChangedSignal();
    void testColorSignalDedu();

    // errorBarsFlags property
    void testErrorBarsFlagsGetterSetter();
    void testErrorBarsFlagChangedSignal();

    // setData — symmetric and asymmetric error modes
    void testSetDataSymmetric();
    void testSetDataAsymmetric();
    void testSetDataFromSeries();
    void testIsAsymmetricMode();
    void testDataChangedSignal();

private:
    std::unique_ptr<QImPlotErrorBarsItemNode> makeNode()
    {
        return std::make_unique<QImPlotErrorBarsItemNode>();
    }
};

void TestQImPlotErrorBarsItemNode::initTestCase()
{
}

void TestQImPlotErrorBarsItemNode::cleanupTestCase()
{
}

// ============================================================================
// Construction
// ============================================================================
void TestQImPlotErrorBarsItemNode::testDefaultConstruction()
{
    auto node = makeNode();
    QVERIFY(node != nullptr);
    QVERIFY(node->isVisible());
    QCOMPARE(node->type(), QImPlotErrorBarsItemNode::Type);

    // Default: vertical orientation
    QVERIFY(!node->isHorizontal());

    // Default: no data set yet
    QVERIFY(node->data() == nullptr);

    // Default: not in asymmetric mode (no data)
    QVERIFY(!node->isAsymmetricMode());
}

// ============================================================================
// horizontal — getter/setter
// ============================================================================
void TestQImPlotErrorBarsItemNode::testHorizontalGetterSetter()
{
    auto node = makeNode();

    // Default: vertical
    QVERIFY(!node->isHorizontal());

    // Set horizontal
    node->setHorizontal(true);
    QVERIFY(node->isHorizontal());

    // Set back to vertical
    node->setHorizontal(false);
    QVERIFY(!node->isHorizontal());
}

// ============================================================================
// horizontal — signal emission
// ============================================================================
void TestQImPlotErrorBarsItemNode::testOrientationChangedSignal()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotErrorBarsItemNode::orientationChanged);

    node->setHorizontal(true);
    QCOMPARE(spy.count(), 1);

    node->setHorizontal(false);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// horizontal — signal deduplication
// ============================================================================
void TestQImPlotErrorBarsItemNode::testOrientationSignalDedu()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotErrorBarsItemNode::orientationChanged);

    node->setHorizontal(true);
    QCOMPARE(spy.count(), 1);

    // Same value — no duplicate signal
    node->setHorizontal(true);
    QCOMPARE(spy.count(), 1);

    // Different value — signal emitted again
    node->setHorizontal(false);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// color — getter/setter
// ============================================================================
void TestQImPlotErrorBarsItemNode::testColorGetterSetter()
{
    auto node = makeNode();

    // Default: invalid QColor (no color set, ImPlot auto-assigns at render time)
    QVERIFY(!node->color().isValid());

    // Set a specific color
    QColor red(255, 0, 0);
    node->setColor(red);
    QCOMPARE(node->color(), red);

    // Redundant set (same value) — getter still returns same
    node->setColor(red);
    QCOMPARE(node->color(), red);
}

// ============================================================================
// color — signal emission
// ============================================================================
void TestQImPlotErrorBarsItemNode::testColorChangedSignal()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotErrorBarsItemNode::colorChanged);

    node->setColor(QColor(0, 128, 255));
    QCOMPARE(spy.count(), 1);

    node->setColor(QColor(255, 255, 0));
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// color — signal deduplication
// ============================================================================
void TestQImPlotErrorBarsItemNode::testColorSignalDedu()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotErrorBarsItemNode::colorChanged);

    QColor blue(0, 0, 255);
    node->setColor(blue);
    QCOMPARE(spy.count(), 1);

    // Same color — no duplicate signal
    node->setColor(blue);
    QCOMPARE(spy.count(), 1);
}

// ============================================================================
// errorBarsFlags — getter/setter
// ============================================================================
void TestQImPlotErrorBarsItemNode::testErrorBarsFlagsGetterSetter()
{
    auto node = makeNode();

    int defaultFlags = node->errorBarsFlags();
    Q_UNUSED(defaultFlags);

    // Set arbitrary flags
    node->setErrorBarsFlags(0x01);
    QCOMPARE(node->errorBarsFlags(), 0x01);

    // Reset
    node->setErrorBarsFlags(0);
    QCOMPARE(node->errorBarsFlags(), 0);
}

// ============================================================================
// errorBarsFlags — signal emission
// ============================================================================
void TestQImPlotErrorBarsItemNode::testErrorBarsFlagChangedSignal()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotErrorBarsItemNode::errorBarsFlagChanged);

    node->setErrorBarsFlags(0x01);
    QCOMPARE(spy.count(), 1);

    node->setErrorBarsFlags(0x02);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// setData — symmetric error mode
// ============================================================================
void TestQImPlotErrorBarsItemNode::testSetDataSymmetric()
{
    auto node = makeNode();

    auto xs = TestHelpers::makeLinearXs(5);
    auto ys = TestHelpers::makeLinearYs(5);
    std::vector<double> errors = {0.1, 0.2, 0.15, 0.25, 0.3};

    auto* series = node->setData(xs, ys, errors);
    QVERIFY(series != nullptr);
    QVERIFY(node->data() != nullptr);
    QVERIFY(node->data()->isSymmetric());
    QCOMPARE(node->data()->size(), 5);
}

// ============================================================================
// setData — asymmetric error mode
// ============================================================================
void TestQImPlotErrorBarsItemNode::testSetDataAsymmetric()
{
    auto node = makeNode();

    auto xs = TestHelpers::makeLinearXs(5);
    auto ys = TestHelpers::makeLinearYs(5);
    std::vector<double> negErrors = {0.1, 0.2, 0.15, 0.25, 0.3};
    std::vector<double> posErrors = {0.15, 0.25, 0.20, 0.30, 0.35};

    auto* series = node->setData(xs, ys, negErrors, posErrors);
    QVERIFY(series != nullptr);
    QVERIFY(node->data() != nullptr);
    QVERIFY(node->data()->isAsymmetric());
    QCOMPARE(node->data()->size(), 5);
}

// ============================================================================
// setData — from pre-built QImAbstractErrorDataSeries
// ============================================================================
void TestQImPlotErrorBarsItemNode::testSetDataFromSeries()
{
    auto node = makeNode();

    auto xs = TestHelpers::makeLinearXs(3);
    auto ys = TestHelpers::makeLinearYs(3);
    std::vector<double> errors = {0.5, 0.5, 0.5};

    auto* series = new QImVectorErrorDataSeries<std::vector<double>, std::vector<double>, std::vector<double>>(xs, ys, errors);
    node->setData(series);
    QVERIFY(node->data() != nullptr);
    QCOMPARE(node->data()->size(), 3);
    QVERIFY(node->data()->isSymmetric());
}

// ============================================================================
// isAsymmetricMode — reflects current data series type
// ============================================================================
void TestQImPlotErrorBarsItemNode::testIsAsymmetricMode()
{
    auto node = makeNode();

    // Initially no data — should be false
    QVERIFY(!node->isAsymmetricMode());

    // Set symmetric data
    auto xs = TestHelpers::makeLinearXs(3);
    auto ys = TestHelpers::makeLinearYs(3);
    std::vector<double> errors = {0.1, 0.1, 0.1};
    node->setData(xs, ys, errors);
    QVERIFY(!node->isAsymmetricMode());

    // Set asymmetric data
    std::vector<double> negErrors = {0.1, 0.1, 0.1};
    std::vector<double> posErrors = {0.2, 0.2, 0.2};
    node->setData(xs, ys, negErrors, posErrors);
    QVERIFY(node->isAsymmetricMode());
}

// ============================================================================
// dataChanged — signal on setData
// ============================================================================
void TestQImPlotErrorBarsItemNode::testDataChangedSignal()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotErrorBarsItemNode::dataChanged);

    auto xs = TestHelpers::makeLinearXs(3);
    auto ys = TestHelpers::makeLinearYs(3);
    std::vector<double> errors = {0.1, 0.1, 0.1};

    node->setData(xs, ys, errors);
    QCOMPARE(spy.count(), 1);

    // Set new data — signal emitted again
    std::vector<double> negErrors = {0.1, 0.1, 0.1};
    std::vector<double> posErrors = {0.2, 0.2, 0.2};
    node->setData(xs, ys, negErrors, posErrors);
    QCOMPARE(spy.count(), 2);
}

QTEST_GUILESS_MAIN(TestQImPlotErrorBarsItemNode)
#include "tst_qimploterrorbarsitemnode.moc"
