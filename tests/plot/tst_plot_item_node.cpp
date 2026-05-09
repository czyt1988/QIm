#include <QObject>
#include <QtTest>
#include <memory>

#include "QImPlotItemNode.h"
#include "QImPlotLineItemNode.h"
#include "QImPlotNode.h"

using namespace QIM;

class TestPlotItemNode : public QObject
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

    // Axis binding
    void testDefaultAxisBinding();
    void testBindAxis();

    // Plot node access
    void testPlotNodeWhenOrphan();
    void testPlotNodeWhenChild();

    // Visibility
    void testDefaultVisible();
    void testSetVisible();

    // Legend hovered
    void testLegendHoveredDefault();

    // pixelAwareTargetPoints (static)
    void testPixelAwareTargetPoints();

    // Type identification (via concrete subclass)
    void testConcreteType();

    // Inherited properties from QImAbstractNode
    void testInheritedAutoIdDisabled();
    void testInheritedRenderOption();

    // itemColor (before rendering)
    void testItemColorBeforeRender();
};

void TestPlotItemNode::initTestCase()
{
}

void TestPlotItemNode::cleanupTestCase()
{
}

// ============================================================================
// Construction
// ============================================================================
void TestPlotItemNode::testDefaultConstruction()
{
    // QImPlotItemNode is abstract; test via concrete QImPlotLineItemNode
    auto node = std::make_unique<QImPlotLineItemNode>();

    QVERIFY(node->label().isEmpty());
    QCOMPARE(node->xAxisId(), QImPlotAxisId::X1);
    QCOMPARE(node->yAxisId(), QImPlotAxisId::Y1);
    QVERIFY(node->isVisible());
}

// ============================================================================
// Label
// ============================================================================
void TestPlotItemNode::testLabelGetterSetter()
{
    auto node = std::make_unique<QImPlotLineItemNode>();

    node->setLabel("My Line");
    QCOMPARE(node->label(), QString("My Line"));

    // Overwrite
    node->setLabel("Updated Line");
    QCOMPARE(node->label(), QString("Updated Line"));

    // Empty label
    node->setLabel("");
    QVERIFY(node->label().isEmpty());
}

void TestPlotItemNode::testLabelChangedSignal()
{
    auto node = std::make_unique<QImPlotLineItemNode>();
    QSignalSpy spy(node.get(), &QImPlotItemNode::labelChanged);

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

void TestPlotItemNode::testLabelConstData()
{
    auto node = std::make_unique<QImPlotLineItemNode>();

    // Default empty label → returns "##plotItem"
    QCOMPARE(QString(node->labelConstData()), QString("##plotItem"));

    node->setLabel("Test");
    QCOMPARE(QString(node->labelConstData()), QString("Test"));
}

// ============================================================================
// Axis binding
// ============================================================================
void TestPlotItemNode::testDefaultAxisBinding()
{
    auto node = std::make_unique<QImPlotLineItemNode>();
    QCOMPARE(node->xAxisId(), QImPlotAxisId::X1);
    QCOMPARE(node->yAxisId(), QImPlotAxisId::Y1);
}

void TestPlotItemNode::testBindAxis()
{
    auto node = std::make_unique<QImPlotLineItemNode>();

    // Valid binding
    node->bindAxis(QImPlotAxisId::X2, QImPlotAxisId::Y2);
    QCOMPARE(node->xAxisId(), QImPlotAxisId::X2);
    QCOMPARE(node->yAxisId(), QImPlotAxisId::Y2);

    // Invalid: X→Y → should reject
    node->bindAxis(QImPlotAxisId::Y1, QImPlotAxisId::X1);
    // Should still be previous valid values since isXAxisId(Y1) = false
    QCOMPARE(node->xAxisId(), QImPlotAxisId::X2);
    QCOMPARE(node->yAxisId(), QImPlotAxisId::Y2);

    // Bind back to X1/Y1
    node->bindAxis(QImPlotAxisId::X3, QImPlotAxisId::Y3);
    QCOMPARE(node->xAxisId(), QImPlotAxisId::X3);
    QCOMPARE(node->yAxisId(), QImPlotAxisId::Y3);
}

// ============================================================================
// Plot node access
// ============================================================================
void TestPlotItemNode::testPlotNodeWhenOrphan()
{
    auto node = std::make_unique<QImPlotLineItemNode>();
    QVERIFY(node->plotNode() == nullptr);
}

void TestPlotItemNode::testPlotNodeWhenChild()
{
    QImPlotNode plot;
    auto* line = new QImPlotLineItemNode(&plot);
    QCOMPARE(line->plotNode(), &plot);
}

// ============================================================================
// Visibility
// ============================================================================
void TestPlotItemNode::testDefaultVisible()
{
    auto node = std::make_unique<QImPlotLineItemNode>();
    QVERIFY(node->isVisible());
}

void TestPlotItemNode::testSetVisible()
{
    auto node = std::make_unique<QImPlotLineItemNode>();

    node->setVisible(false);
    QVERIFY(!node->isVisible());

    node->setVisible(true);
    QVERIFY(node->isVisible());
}

// ============================================================================
// Legend hovered
// ============================================================================
void TestPlotItemNode::testLegendHoveredDefault()
{
    auto node = std::make_unique<QImPlotLineItemNode>();
    // Before rendering, plotItem is null → returns false
    QVERIFY(!node->isLegendHovered());
}

// ============================================================================
// pixelAwareTargetPoints
// ============================================================================
void TestPlotItemNode::testPixelAwareTargetPoints()
{
    // 800px * 1.5 = 1200 (no clamping)
    QCOMPARE(QImPlotItemNode::pixelAwareTargetPoints(800), 1200);
    // 50px * 1.5 = 75 → clamped to 100
    QCOMPARE(QImPlotItemNode::pixelAwareTargetPoints(50), 100);
    // 8000px * 1.5 = 12000 → clamped to 10000
    QCOMPARE(QImPlotItemNode::pixelAwareTargetPoints(8000), 10000);
    // 0px → clamped to 100
    QCOMPARE(QImPlotItemNode::pixelAwareTargetPoints(0), 100);
    // negative → clamped to 100
    QCOMPARE(QImPlotItemNode::pixelAwareTargetPoints(-10), 100);
}

// ============================================================================
// Type identification
// ============================================================================
void TestPlotItemNode::testConcreteType()
{
    auto node = std::make_unique<QImPlotLineItemNode>();
    // QImPlotItemNode::InnerType is 10, QImPlotLineItemNode should have its own type
    int t = node->type();
    QVERIFY(t >= QImPlotItemNode::InnerType);
    QVERIFY(t != 0);
}

// ============================================================================
// Inherited properties
// ============================================================================
void TestPlotItemNode::testInheritedAutoIdDisabled()
{
    auto node = std::make_unique<QImPlotLineItemNode>();
    // QImPlotItemNode constructor sets auto-id disabled
    QVERIFY(!node->isAutoIdEnabled());
}

void TestPlotItemNode::testInheritedRenderOption()
{
    auto node = std::make_unique<QImPlotLineItemNode>();
    // QImPlotItemNode constructor sets RenderIgnoreVisible
    QVERIFY(node->testRenderOption(QImAbstractNode::RenderIgnoreVisible));
}

// ============================================================================
// itemColor before rendering
// ============================================================================
void TestPlotItemNode::testItemColorBeforeRender()
{
    auto node = std::make_unique<QImPlotLineItemNode>();
    // Before rendering, plotItem is null → returns invalid QColor
    QVERIFY(!node->itemColor().isValid());
}

QTEST_GUILESS_MAIN(TestPlotItemNode)
#include "tst_plot_item_node.moc"
