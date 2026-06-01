#include <QObject>
#include <QtTest>
#include <memory>

#include "QImPlotNode.h"
#include "QImPlotLineItemNode.h"
#include "QImPlotAxisInfo.h"
#include "implot.h"

using namespace QIM;

class TestPlotNode : public QObject
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

    // Size property
    void testSizeGetterSetter();
    void testSizeChangedSignal();

    // AutoSize property
    void testAutoSizeGetterSetter();
    void testAutoSizeChangedSignal();

    // ImPlotFlags - negative to positive semantics (NoXxx �?xxxEnabled)
    void testTitleEnabledFlag();
    void testLegendEnabledFlag();
    void testMouseTextEnabledFlag();
    void testInputsEnabledFlag();
    void testMenusEnabledFlag();
    void testBoxSelectEnabledFlag();
    void testFrameEnabledFlag();

    // ImPlotFlags - direct mapping
    void testEqualFlag();
    void testCrosshairsFlag();

    // ImPlotFlags - combined (CanvasOnly)
    void testCanvasEnabledFlag();

    // Raw flag access
    void testImPlotFlagsRaw();
    void testSetImPlotFlagsRaw();

    // plotFlagChanged signal
    void testPlotFlagChangedSignal();

    // Axis info accessors
    void testAxisInfoAccessors();
    void testAxisEnabled();

    // Plot item management
    void testAddPlotItem();
    void testPlotItemNodes();
    void testAddLineTemplate();

    // Legend node
    void testLegendNode();

    // Convenience methods
    void testRescaleAxes();
    void testSetAxesToFit();
    // Child management (inherited)
    void testChildManagement();

private:
    static std::vector<double> makeTestData(int n)
    {
        std::vector<double> data(n);
        for (int i = 0; i < n; ++i)
            data[i] = static_cast<double>(i);
        return data;
    }
};

void TestPlotNode::initTestCase()
{
}

void TestPlotNode::cleanupTestCase()
{
}

// ============================================================================
// Construction
// ============================================================================
void TestPlotNode::testDefaultConstruction()
{
    QImPlotNode plot;
    QCOMPARE(plot.title(), QString("##Plot"));
    QVERIFY(plot.isAutoSize());
    QCOMPARE(plot.childNodeCount(), 0);
}

void TestPlotNode::testTitleConstructor()
{
    QImPlotNode plot("My Plot");
    QCOMPARE(plot.title(), QString("My Plot"));
}

// ============================================================================
// Title
// ============================================================================
void TestPlotNode::testTitleGetterSetter()
{
    QImPlotNode plot;
    plot.setTitle("Test Title");
    QCOMPARE(plot.title(), QString("Test Title"));

    // Same value �?no change
    plot.setTitle("Test Title");
    QCOMPARE(plot.title(), QString("Test Title"));

    // Empty title
    plot.setTitle("");
    QCOMPARE(plot.title(), QString(""));
}

void TestPlotNode::testTitleChangedSignal()
{
    QImPlotNode plot;
    QSignalSpy spy(&plot, &QImPlotNode::titleChanged);

    plot.setTitle("New Title");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toString(), QString("New Title"));

    // Same value �?no signal
    plot.setTitle("New Title");
    QCOMPARE(spy.count(), 1);

    plot.setTitle("Another Title");
    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.at(1).at(0).toString(), QString("Another Title"));
}

// ============================================================================
// Size
// ============================================================================
void TestPlotNode::testSizeGetterSetter()
{
    QImPlotNode plot;

    // Default size is -1,-1 (auto-size)
    QSizeF defaultSize = plot.size();
    QVERIFY(defaultSize.width() < 0);
    QVERIFY(defaultSize.height() < 0);

    plot.setSize(QSizeF(800, 600));
    QCOMPARE(plot.size(), QSizeF(800, 600));

    // Negative �?auto-size
    plot.setSize(QSizeF(-1, -1));
    QVERIFY(plot.size().width() < 0);
}

void TestPlotNode::testSizeChangedSignal()
{
    QImPlotNode plot;
    QSignalSpy spy(&plot, &QImPlotNode::sizeChanged);

    plot.setSize(QSizeF(400, 300));
    QCOMPARE(spy.count(), 1);

    // Same value �?no signal
    plot.setSize(QSizeF(400, 300));
    QCOMPARE(spy.count(), 1);

    plot.setSize(QSizeF(800, 600));
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// AutoSize
// ============================================================================
void TestPlotNode::testAutoSizeGetterSetter()
{
    QImPlotNode plot;

    // Default: auto-size enabled
    QVERIFY(plot.isAutoSize());

    plot.setAutoSize(false);
    QVERIFY(!plot.isAutoSize());

    plot.setAutoSize(true);
    QVERIFY(plot.isAutoSize());

    // Setting same value �?no change
    plot.setAutoSize(true);
    QVERIFY(plot.isAutoSize());
}

void TestPlotNode::testAutoSizeChangedSignal()
{
    QImPlotNode plot;
    QSignalSpy spy(&plot, &QImPlotNode::autoSizeChanged);

    plot.setAutoSize(false);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toBool(), false);

    // Same value �?no signal
    plot.setAutoSize(false);
    QCOMPARE(spy.count(), 1);

    plot.setAutoSize(true);
    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.at(1).at(0).toBool(), true);
}

// ============================================================================
// ImPlotFlags - negative to positive
// ============================================================================
void TestPlotNode::testTitleEnabledFlag()
{
    QImPlotNode plot;
    QVERIFY(plot.isTitleEnabled());  // Default: NoTitle NOT set �?enabled

    plot.setTitleEnabled(false);
    QVERIFY(!plot.isTitleEnabled());

    plot.setTitleEnabled(true);
    QVERIFY(plot.isTitleEnabled());
}

void TestPlotNode::testLegendEnabledFlag()
{
    QImPlotNode plot;
    QVERIFY(plot.isLegendEnabled());

    plot.setLegendEnabled(false);
    QVERIFY(!plot.isLegendEnabled());

    plot.setLegendEnabled(true);
    QVERIFY(plot.isLegendEnabled());
}

void TestPlotNode::testMouseTextEnabledFlag()
{
    QImPlotNode plot;
    QVERIFY(plot.isMouseTextEnabled());

    plot.setMouseTextEnabled(false);
    QVERIFY(!plot.isMouseTextEnabled());

    plot.setMouseTextEnabled(true);
    QVERIFY(plot.isMouseTextEnabled());
}

void TestPlotNode::testInputsEnabledFlag()
{
    QImPlotNode plot;
    QVERIFY(plot.isInputsEnabled());

    plot.setInputsEnabled(false);
    QVERIFY(!plot.isInputsEnabled());

    plot.setInputsEnabled(true);
    QVERIFY(plot.isInputsEnabled());
}

void TestPlotNode::testMenusEnabledFlag()
{
    QImPlotNode plot;
    QVERIFY(plot.isMenusEnabled());

    plot.setMenusEnabled(false);
    QVERIFY(!plot.isMenusEnabled());

    plot.setMenusEnabled(true);
    QVERIFY(plot.isMenusEnabled());
}

void TestPlotNode::testBoxSelectEnabledFlag()
{
    QImPlotNode plot;
    QVERIFY(plot.isBoxSelectEnabled());

    plot.setBoxSelectEnabled(false);
    QVERIFY(!plot.isBoxSelectEnabled());

    plot.setBoxSelectEnabled(true);
    QVERIFY(plot.isBoxSelectEnabled());
}

void TestPlotNode::testFrameEnabledFlag()
{
    QImPlotNode plot;
    QVERIFY(plot.isFrameEnabled());

    plot.setFrameEnabled(false);
    QVERIFY(!plot.isFrameEnabled());

    plot.setFrameEnabled(true);
    QVERIFY(plot.isFrameEnabled());
}

// ============================================================================
// ImPlotFlags - direct mapping
// ============================================================================
void TestPlotNode::testEqualFlag()
{
    QImPlotNode plot;
    QVERIFY(!plot.isEqual());  // Default: NOT set

    plot.setEqual(true);
    QVERIFY(plot.isEqual());

    plot.setEqual(false);
    QVERIFY(!plot.isEqual());
}

void TestPlotNode::testCrosshairsFlag()
{
    QImPlotNode plot;
    QVERIFY(!plot.isCrosshairs());  // Default: NOT set

    plot.setCrosshairs(true);
    QVERIFY(plot.isCrosshairs());

    plot.setCrosshairs(false);
    QVERIFY(!plot.isCrosshairs());
}

// ============================================================================
// ImPlotFlags - combined
// ============================================================================
void TestPlotNode::testCanvasEnabledFlag()
{
    QImPlotNode plot;
    QVERIFY(plot.isCanvasEnabled());  // Default: CanvasOnly NOT set �?all decorations visible

    plot.setCanvasEnabled(false);
    QVERIFY(!plot.isCanvasEnabled());

    plot.setCanvasEnabled(true);
    QVERIFY(plot.isCanvasEnabled());
}

// ============================================================================
// Raw flag access
// ============================================================================
void TestPlotNode::testImPlotFlagsRaw()
{
    QImPlotNode plot;
    QCOMPARE(plot.imPlotFlags(), 0);  // ImPlotFlags_None
}

void TestPlotNode::testSetImPlotFlagsRaw()
{
    QImPlotNode plot;
    QSignalSpy spy(&plot, &QImPlotNode::plotFlagChanged);

    plot.setImPlotFlags(ImPlotFlags_NoTitle);
    QCOMPARE(plot.imPlotFlags(), static_cast<int>(ImPlotFlags_NoTitle));
    QVERIFY(!plot.isTitleEnabled());
    QCOMPARE(spy.count(), 1);

    // Same value �?no signal
    plot.setImPlotFlags(ImPlotFlags_NoTitle);
    QCOMPARE(spy.count(), 1);
}

// ============================================================================
// plotFlagChanged signal
// ============================================================================
void TestPlotNode::testPlotFlagChangedSignal()
{
    QImPlotNode plot;
    QSignalSpy spy(&plot, &QImPlotNode::plotFlagChanged);

    plot.setEqual(true);
    QCOMPARE(spy.count(), 1);

    // Same value �?no signal
    plot.setEqual(true);
    QCOMPARE(spy.count(), 1);

    plot.setCrosshairs(true);
    QCOMPARE(spy.count(), 2);

    // CanvasEnabled is combined: setting false sets 5 flags �?should emit
    plot.setCanvasEnabled(false);
    QCOMPARE(spy.count(), 3);
}

// ============================================================================
// Axis info accessors
// ============================================================================
void TestPlotNode::testAxisInfoAccessors()
{
    QImPlotNode plot;

    // All 6 axes should return non-null
    QVERIFY(plot.x1Axis() != nullptr);
    QVERIFY(plot.y1Axis() != nullptr);
    QVERIFY(plot.x2Axis() != nullptr);
    QVERIFY(plot.y2Axis() != nullptr);
    QVERIFY(plot.x3Axis() != nullptr);
    QVERIFY(plot.y3Axis() != nullptr);

    // axisInfo should return the same objects
    QCOMPARE(plot.axisInfo(QImPlotAxisId::X1), plot.x1Axis());
    QCOMPARE(plot.axisInfo(QImPlotAxisId::Y1), plot.y1Axis());
    QCOMPARE(plot.axisInfo(QImPlotAxisId::X2), plot.x2Axis());
    QCOMPARE(plot.axisInfo(QImPlotAxisId::Y2), plot.y2Axis());
    QCOMPARE(plot.axisInfo(QImPlotAxisId::X3), plot.x3Axis());
    QCOMPARE(plot.axisInfo(QImPlotAxisId::Y3), plot.y3Axis());
}

void TestPlotNode::testAxisEnabled()
{
    QImPlotNode plot;

    // X1 and Y1 are enabled by default
    QVERIFY(plot.isAxisEnabled(QImPlotAxisId::X1));
    QVERIFY(plot.isAxisEnabled(QImPlotAxisId::Y1));

    // Other axes exist but may be disabled
    QVERIFY(plot.isAxisEnabled(QImPlotAxisId::X2));  // Exists as object
}

// ============================================================================
// Plot item management
// ============================================================================
void TestPlotNode::testAddPlotItem()
{
    QImPlotNode plot;
    auto* line = new QImPlotLineItemNode();

    plot.addPlotItem(line);
    QCOMPARE(plot.childNodeCount(), 1);
    QCOMPARE(line->parentNode(), &plot);
}

void TestPlotNode::testPlotItemNodes()
{
    QImPlotNode plot;
    QVERIFY(plot.plotItemNodes().isEmpty());

    auto* line = new QImPlotLineItemNode(&plot);
    QCOMPARE(plot.plotItemNodes().size(), 1);
    QCOMPARE(plot.plotItemNodes().at(0), line);
}

void TestPlotNode::testAddLineTemplate()
{
    QImPlotNode plot;
    auto xData = makeTestData(10);
    auto yData = makeTestData(10);

    auto* line = plot.addLine(xData, yData, "Test Line");
    QVERIFY(line != nullptr);
    QCOMPARE(line->label(), QString("Test Line"));
    QCOMPARE(plot.childNodeCount(), 1);
    QCOMPARE(line->parentNode(), &plot);
}

// ============================================================================
// Legend node
// ============================================================================
void TestPlotNode::testLegendNode()
{
    QImPlotNode plot;
    QVERIFY(plot.legendNode() != nullptr);
}

// ============================================================================
// Convenience methods
// ============================================================================
void TestPlotNode::testRescaleAxes()
{
    QImPlotNode plot;
    // rescaleAxes just sets an internal flag; verify it doesn't crash
    plot.rescaleAxes();
    // No exception = pass
    QVERIFY(true);
}

void TestPlotNode::testSetAxesToFit()
{
    QImPlotNode plot;
    // setAxesToFit is an alias for rescaleAxes
    plot.setAxesToFit();
    QVERIFY(true);
}

// ============================================================================
// Child management (inherited from QImAbstractNode)
// ============================================================================
void TestPlotNode::testChildManagement()
{
    QImPlotNode plot;
    auto* child = new QImPlotLineItemNode();

    plot.addChildNode(child);
    QCOMPARE(plot.childNodeCount(), 1);
    QCOMPARE(plot.indexOfChildNode(child), 0);

    plot.removeChildNode(child);
    QCOMPARE(plot.childNodeCount(), 0);
}

QTEST_GUILESS_MAIN(TestPlotNode)
#include "tst_plot_node.moc"
