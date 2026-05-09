#include <QObject>
#include <QtTest>
#include <memory>

#include "QImPlot3DStyleNode.h"
#include "QImPlot3DNode.h"

using namespace QIM;

class TestPlot3DStyleNode : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    // Construction
    void testDefaultConstruction();

    // Style variable getter/setter + signal
    void testLineWeight();
    void testMarker();
    void testMarkerSize();
    void testMarkerWeight();
    void testFillAlpha();
    void testPlotDefaultSize();
    void testPlotMinSize();
    void testPlotPadding();
    void testLabelPadding();
    void testViewScaleFactor();
    void testLegendPadding();
    void testLegendInnerPadding();
    void testLegendSpacing();

    // Plot color getter/setter + signal
    void testTitleTextColor();
    void testInlayTextColor();
    void testFrameBgColor();
    void testPlotBgColor();
    void testPlotBorderColor();

    // Legend color getter/setter + signal
    void testLegendBgColor();
    void testLegendBorderColor();
    void testLegendTextColor();

    // Axis color getter/setter + signal
    void testAxisTextColor();
    void testAxisGridColor();
    void testAxisTickColor();

    // styleChanged signal
    void testStyleChangedSignal();

    // Theme convenience methods
    void testStyleColorsAuto();
    void testStyleColorsDark();
    void testStyleColorsLight();
    void testStyleColorsClassic();

    // applyStyle
    void testApplyStyleNoCrash();

    // Same value — no signal
    void testSetSameValueNoSignal();
};

void TestPlot3DStyleNode::initTestCase()
{
}

void TestPlot3DStyleNode::cleanupTestCase()
{
}

// ============================================================================
// Construction
// ============================================================================
void TestPlot3DStyleNode::testDefaultConstruction()
{
    QImPlot3DNode plot3d;
    QImPlot3DStyleNode* style = plot3d.styleNode();
    QVERIFY(style != nullptr);

    // Default style variable values (matching ImPlot3DStyle defaults)
    QCOMPARE(style->lineWeight(), 1.0f);
    QCOMPARE(style->marker(), -1);  // ImPlot3DMarker_None = -1
    QCOMPARE(style->markerSize(), 4.0f);
    QCOMPARE(style->markerWeight(), 1.0f);
    QCOMPARE(style->fillAlpha(), 1.0f);
    QCOMPARE(style->plotDefaultSize(), QSizeF(400, 400));
    QCOMPARE(style->plotMinSize(), QSizeF(200, 200));
    QCOMPARE(style->viewScaleFactor(), 1.0f);
}

// ============================================================================
// Style variables - getter/setter + signal per property
// ============================================================================
void TestPlot3DStyleNode::testLineWeight()
{
    QImPlot3DNode plot3d;
    QImPlot3DStyleNode* style = plot3d.styleNode();
    QSignalSpy spy(style, &QImPlot3DStyleNode::styleChanged);

    QCOMPARE(style->lineWeight(), 1.0f);

    style->setLineWeight(2.5f);
    QCOMPARE(style->lineWeight(), 2.5f);
    QCOMPARE(spy.count(), 1);

    // Same value — no signal
    style->setLineWeight(2.5f);
    QCOMPARE(spy.count(), 1);
}

void TestPlot3DStyleNode::testMarker()
{
    QImPlot3DNode plot3d;
    QImPlot3DStyleNode* style = plot3d.styleNode();

    QCOMPARE(style->marker(), -1);

    style->setMarker(5);
    QCOMPARE(style->marker(), 5);
}

void TestPlot3DStyleNode::testMarkerSize()
{
    QImPlot3DNode plot3d;
    QImPlot3DStyleNode* style = plot3d.styleNode();

    QCOMPARE(style->markerSize(), 4.0f);

    style->setMarkerSize(8.0f);
    QCOMPARE(style->markerSize(), 8.0f);
}

void TestPlot3DStyleNode::testMarkerWeight()
{
    QImPlot3DNode plot3d;
    QImPlot3DStyleNode* style = plot3d.styleNode();

    QCOMPARE(style->markerWeight(), 1.0f);

    style->setMarkerWeight(2.0f);
    QCOMPARE(style->markerWeight(), 2.0f);
}

void TestPlot3DStyleNode::testFillAlpha()
{
    QImPlot3DNode plot3d;
    QImPlot3DStyleNode* style = plot3d.styleNode();

    QCOMPARE(style->fillAlpha(), 1.0f);

    style->setFillAlpha(0.5f);
    QCOMPARE(style->fillAlpha(), 0.5f);
}

void TestPlot3DStyleNode::testPlotDefaultSize()
{
    QImPlot3DNode plot3d;
    QImPlot3DStyleNode* style = plot3d.styleNode();
    QSignalSpy spy(style, &QImPlot3DStyleNode::styleChanged);

    QCOMPARE(style->plotDefaultSize(), QSizeF(400, 400));

    style->setPlotDefaultSize(QSizeF(800, 600));
    QCOMPARE(style->plotDefaultSize(), QSizeF(800, 600));
    QCOMPARE(spy.count(), 1);

    // Same value — no signal
    style->setPlotDefaultSize(QSizeF(800, 600));
    QCOMPARE(spy.count(), 1);
}

void TestPlot3DStyleNode::testPlotMinSize()
{
    QImPlot3DNode plot3d;
    QImPlot3DStyleNode* style = plot3d.styleNode();

    QCOMPARE(style->plotMinSize(), QSizeF(200, 200));

    style->setPlotMinSize(QSizeF(100, 100));
    QCOMPARE(style->plotMinSize(), QSizeF(100, 100));
}

void TestPlot3DStyleNode::testPlotPadding()
{
    QImPlot3DNode plot3d;
    QImPlot3DStyleNode* style = plot3d.styleNode();

    // Default plotPadding is (10, 10)
    style->setPlotPadding(QSizeF(20, 15));
    QCOMPARE(style->plotPadding(), QSizeF(20, 15));
}

void TestPlot3DStyleNode::testLabelPadding()
{
    QImPlot3DNode plot3d;
    QImPlot3DStyleNode* style = plot3d.styleNode();

    style->setLabelPadding(QSizeF(10, 10));
    QCOMPARE(style->labelPadding(), QSizeF(10, 10));
}

void TestPlot3DStyleNode::testViewScaleFactor()
{
    QImPlot3DNode plot3d;
    QImPlot3DStyleNode* style = plot3d.styleNode();

    QCOMPARE(style->viewScaleFactor(), 1.0f);

    style->setViewScaleFactor(2.0f);
    QCOMPARE(style->viewScaleFactor(), 2.0f);
}

void TestPlot3DStyleNode::testLegendPadding()
{
    QImPlot3DNode plot3d;
    QImPlot3DStyleNode* style = plot3d.styleNode();

    style->setLegendPadding(QSizeF(15, 15));
    QCOMPARE(style->legendPadding(), QSizeF(15, 15));
}

void TestPlot3DStyleNode::testLegendInnerPadding()
{
    QImPlot3DNode plot3d;
    QImPlot3DStyleNode* style = plot3d.styleNode();

    style->setLegendInnerPadding(QSizeF(8, 8));
    QCOMPARE(style->legendInnerPadding(), QSizeF(8, 8));
}

void TestPlot3DStyleNode::testLegendSpacing()
{
    QImPlot3DNode plot3d;
    QImPlot3DStyleNode* style = plot3d.styleNode();

    style->setLegendSpacing(QSizeF(10, 2));
    QCOMPARE(style->legendSpacing(), QSizeF(10, 2));
}

// ============================================================================
// Plot colors
// ============================================================================
void TestPlot3DStyleNode::testTitleTextColor()
{
    QImPlot3DNode plot3d;
    QImPlot3DStyleNode* style = plot3d.styleNode();
    QSignalSpy spy(style, &QImPlot3DStyleNode::styleChanged);

    QColor color(255, 0, 0);
    style->setTitleTextColor(color);
    QCOMPARE(style->titleTextColor(), color);
    QCOMPARE(spy.count(), 1);
}

void TestPlot3DStyleNode::testInlayTextColor()
{
    QImPlot3DNode plot3d;
    QImPlot3DStyleNode* style = plot3d.styleNode();

    QColor color(0, 255, 0);
    style->setInlayTextColor(color);
    QCOMPARE(style->inlayTextColor(), color);
}

void TestPlot3DStyleNode::testFrameBgColor()
{
    QImPlot3DNode plot3d;
    QImPlot3DStyleNode* style = plot3d.styleNode();

    QColor color(50, 50, 50);
    style->setFrameBgColor(color);
    QCOMPARE(style->frameBgColor(), color);
}

void TestPlot3DStyleNode::testPlotBgColor()
{
    QImPlot3DNode plot3d;
    QImPlot3DStyleNode* style = plot3d.styleNode();

    QColor color(30, 30, 30);
    style->setPlotBgColor(color);
    QCOMPARE(style->plotBgColor(), color);
}

void TestPlot3DStyleNode::testPlotBorderColor()
{
    QImPlot3DNode plot3d;
    QImPlot3DStyleNode* style = plot3d.styleNode();

    QColor color(200, 200, 200);
    style->setPlotBorderColor(color);
    QCOMPARE(style->plotBorderColor(), color);
}

// ============================================================================
// Legend colors
// ============================================================================
void TestPlot3DStyleNode::testLegendBgColor()
{
    QImPlot3DNode plot3d;
    QImPlot3DStyleNode* style = plot3d.styleNode();

    QColor color(40, 40, 40, 200);
    style->setLegendBgColor(color);
    QCOMPARE(style->legendBgColor(), color);
}

void TestPlot3DStyleNode::testLegendBorderColor()
{
    QImPlot3DNode plot3d;
    QImPlot3DStyleNode* style = plot3d.styleNode();

    QColor color(100, 100, 100);
    style->setLegendBorderColor(color);
    QCOMPARE(style->legendBorderColor(), color);
}

void TestPlot3DStyleNode::testLegendTextColor()
{
    QImPlot3DNode plot3d;
    QImPlot3DStyleNode* style = plot3d.styleNode();

    QColor color(230, 230, 230);
    style->setLegendTextColor(color);
    QCOMPARE(style->legendTextColor(), color);
}

// ============================================================================
// Axis colors
// ============================================================================
void TestPlot3DStyleNode::testAxisTextColor()
{
    QImPlot3DNode plot3d;
    QImPlot3DStyleNode* style = plot3d.styleNode();

    QColor color(180, 180, 180);
    style->setAxisTextColor(color);
    QCOMPARE(style->axisTextColor(), color);
}

void TestPlot3DStyleNode::testAxisGridColor()
{
    QImPlot3DNode plot3d;
    QImPlot3DStyleNode* style = plot3d.styleNode();

    QColor color(60, 60, 60);
    style->setAxisGridColor(color);
    QCOMPARE(style->axisGridColor(), color);
}

void TestPlot3DStyleNode::testAxisTickColor()
{
    QImPlot3DNode plot3d;
    QImPlot3DStyleNode* style = plot3d.styleNode();

    QColor color(160, 160, 160);
    style->setAxisTickColor(color);
    QCOMPARE(style->axisTickColor(), color);
}

// ============================================================================
// styleChanged signal
// ============================================================================
void TestPlot3DStyleNode::testStyleChangedSignal()
{
    QImPlot3DNode plot3d;
    QImPlot3DStyleNode* style = plot3d.styleNode();
    QSignalSpy spy(style, &QImPlot3DStyleNode::styleChanged);

    // Each setter emits styleChanged
    style->setLineWeight(3.0f);
    QCOMPARE(spy.count(), 1);

    style->setTitleTextColor(QColor(255, 0, 0));
    QCOMPARE(spy.count(), 2);

    style->setMarker(10);
    QCOMPARE(spy.count(), 3);

    // Multiple properties can change independently
    style->setFillAlpha(0.7f);
    QCOMPARE(spy.count(), 4);
}

// ============================================================================
// Theme convenience methods
// ============================================================================
void TestPlot3DStyleNode::testStyleColorsAuto()
{
    QImPlot3DNode plot3d;
    QImPlot3DStyleNode* style = plot3d.styleNode();
    QSignalSpy spy(style, &QImPlot3DStyleNode::styleChanged);

    spy.clear();
    style->styleColorsAuto();
    QCOMPARE(spy.count(), 1);  // Should emit styleChanged
}

void TestPlot3DStyleNode::testStyleColorsDark()
{
    QImPlot3DNode plot3d;
    QImPlot3DStyleNode* style = plot3d.styleNode();
    QSignalSpy spy(style, &QImPlot3DStyleNode::styleChanged);

    spy.clear();
    style->styleColorsDark();
    QCOMPARE(spy.count(), 1);
}

void TestPlot3DStyleNode::testStyleColorsLight()
{
    QImPlot3DNode plot3d;
    QImPlot3DStyleNode* style = plot3d.styleNode();
    QSignalSpy spy(style, &QImPlot3DStyleNode::styleChanged);

    spy.clear();
    style->styleColorsLight();
    QCOMPARE(spy.count(), 1);
}

void TestPlot3DStyleNode::testStyleColorsClassic()
{
    QImPlot3DNode plot3d;
    QImPlot3DStyleNode* style = plot3d.styleNode();
    QSignalSpy spy(style, &QImPlot3DStyleNode::styleChanged);

    spy.clear();
    style->styleColorsClassic();
    QCOMPARE(spy.count(), 1);
}

// ============================================================================
// applyStyle
// ============================================================================
void TestPlot3DStyleNode::testApplyStyleNoCrash()
{
    QImPlot3DNode plot3d;
    QImPlot3DStyleNode* style = plot3d.styleNode();

    // applyStyle directly manipulates ImPlot3D::GetStyle() which requires
    // a valid ImPlot3D context; verify no crash in a minimal test
    style->applyStyle();
    QVERIFY(true);
}

// ============================================================================
// Same value — no signal
// ============================================================================
void TestPlot3DStyleNode::testSetSameValueNoSignal()
{
    QImPlot3DNode plot3d;
    QImPlot3DStyleNode* style = plot3d.styleNode();
    QSignalSpy spy(style, &QImPlot3DStyleNode::styleChanged);

    // Set a value first
    style->setLineWeight(3.0f);
    QCOMPARE(spy.count(), 1);

    // Set same value — should NOT emit
    style->setLineWeight(3.0f);
    QCOMPARE(spy.count(), 1);

    // Same for color
    QColor red(255, 0, 0);
    style->setTitleTextColor(red);
    int countAfterColor = spy.count();

    style->setTitleTextColor(red);
    QCOMPARE(spy.count(), countAfterColor);  // No new signal
}

QTEST_GUILESS_MAIN(TestPlot3DStyleNode)
#include "tst_plot3d_style_node.moc"
