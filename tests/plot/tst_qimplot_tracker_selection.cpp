#include <QObject>
#include <QtTest>
#include <memory>

#include "QImPlotValueTrackerNode.h"
#include "QImPlotSelectionNode.h"
#include "QImPlotNode.h"

using namespace QIM;

class TestTrackerSelection : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    // --- Section 1: QImPlotValueTrackerNode (smoke) ---
    void valueTracker_construction();
    void valueTracker_parentChild();
    void valueTracker_fixedWidth();
    void valueTracker_autoWidth();
    void valueTracker_textColor();
    void valueTracker_backgroundColor();
    void valueTracker_borderColor();
    void valueTracker_trackerLineColor();
    void valueTracker_skipNan();
    void valueTracker_groupDefaults();

    // --- Section 2: QImPlotSelectionNode (Q_PROPERTY) ---
    void selection_construction();
    void selection_defaultAxes();
    void selection_xAxis();
    void selection_yAxis();
    void selection_axisChangedSignal();
    void selection_axisSignalDedup();
    void selection_readOnlyDefaults();
    void selection_autoCancel();
    void selection_autoCancelSignal();
    void selection_autoCancelSignalDedup();
};

void TestTrackerSelection::initTestCase()
{
}

void TestTrackerSelection::cleanupTestCase()
{
}

// ============================================================================
// Section 1: QImPlotValueTrackerNode (smoke tests — zero Q_PROPERTYs)
// ============================================================================
void TestTrackerSelection::valueTracker_construction()
{
    QImPlotNode plot;
    auto tracker = std::make_unique<QImPlotValueTrackerNode>(&plot);
    QVERIFY(tracker != nullptr);
}

void TestTrackerSelection::valueTracker_parentChild()
{
    QImPlotNode plot;
    auto tracker = std::make_unique<QImPlotValueTrackerNode>(&plot, &plot);
    QCOMPARE(tracker->parent(), &plot);
}

void TestTrackerSelection::valueTracker_fixedWidth()
{
    QImPlotNode plot;
    QImPlotValueTrackerNode t(&plot);

    t.setFixedWidth(200.0f);
    QCOMPARE(t.fixedWidth(), 200.0f);

    t.setFixedWidth(50.0f);
    QCOMPARE(t.fixedWidth(), 50.0f);
}

void TestTrackerSelection::valueTracker_autoWidth()
{
    QImPlotNode plot;
    QImPlotValueTrackerNode t(&plot);

    t.setAutoWidthEnabled(false);
    QVERIFY(!t.isAutoWidthEnabled());

    t.setAutoWidthEnabled(true);
    QVERIFY(t.isAutoWidthEnabled());
}

void TestTrackerSelection::valueTracker_textColor()
{
    QImPlotNode plot;
    QImPlotValueTrackerNode t(&plot);

    QColor c(255, 0, 0);
    t.setTextColor(c);
    QCOMPARE(t.textColor(), c);
}

void TestTrackerSelection::valueTracker_backgroundColor()
{
    QImPlotNode plot;
    QImPlotValueTrackerNode t(&plot);

    QColor c(0, 255, 0, 128);
    t.setBackgroundColor(c);
    QCOMPARE(t.backgroundColor(), c);
}

void TestTrackerSelection::valueTracker_borderColor()
{
    QImPlotNode plot;
    QImPlotValueTrackerNode t(&plot);

    QColor c(0, 0, 255);
    t.setBorderColor(c);
    QCOMPARE(t.borderColor(), c);
}

void TestTrackerSelection::valueTracker_trackerLineColor()
{
    QImPlotNode plot;
    QImPlotValueTrackerNode t(&plot);

    QColor c(128, 128, 128);
    t.setTrackerLineColor(c);
    QCOMPARE(t.trackerLineColor(), c);
}

void TestTrackerSelection::valueTracker_skipNan()
{
    QImPlotNode plot;
    QImPlotValueTrackerNode t(&plot);

    t.setSkipNanFiniteValues(true);
    QVERIFY(t.isSkipNanFiniteValues());

    t.setSkipNanFiniteValues(false);
    QVERIFY(!t.isSkipNanFiniteValues());
}

void TestTrackerSelection::valueTracker_groupDefaults()
{
    QImPlotNode plot;
    QImPlotValueTrackerNode t(&plot);

    QVERIFY(!t.hasGroup());
    QVERIFY(t.group() == nullptr);

    t.setGroup(nullptr);
    QVERIFY(!t.hasGroup());
    QVERIFY(t.group() == nullptr);
}

// ============================================================================
// Section 2: QImPlotSelectionNode (Q_PROPERTY tests + signal dedup)
// ============================================================================
void TestTrackerSelection::selection_construction()
{
    QImPlotNode plot;
    auto sel = std::make_unique<QImPlotSelectionNode>(&plot);
    QVERIFY(sel != nullptr);
}

void TestTrackerSelection::selection_defaultAxes()
{
    QImPlotNode plot;
    QImPlotSelectionNode sel(&plot);

    // Default axis values should be Auto
    QCOMPARE(sel.xAxis(), QImPlotAxisId::Auto);
    QCOMPARE(sel.yAxis(), QImPlotAxisId::Auto);
}

void TestTrackerSelection::selection_xAxis()
{
    QImPlotNode plot;
    QImPlotSelectionNode sel(&plot);

    sel.setXAxis(QImPlotAxisId::X2);
    QCOMPARE(sel.xAxis(), QImPlotAxisId::X2);

    sel.setXAxis(QImPlotAxisId::X3);
    QCOMPARE(sel.xAxis(), QImPlotAxisId::X3);

    sel.setXAxis(QImPlotAxisId::X1);
    QCOMPARE(sel.xAxis(), QImPlotAxisId::X1);
}

void TestTrackerSelection::selection_yAxis()
{
    QImPlotNode plot;
    QImPlotSelectionNode sel(&plot);

    sel.setYAxis(QImPlotAxisId::Y2);
    QCOMPARE(sel.yAxis(), QImPlotAxisId::Y2);

    sel.setYAxis(QImPlotAxisId::Y3);
    QCOMPARE(sel.yAxis(), QImPlotAxisId::Y3);

    sel.setYAxis(QImPlotAxisId::Y1);
    QCOMPARE(sel.yAxis(), QImPlotAxisId::Y1);
}

void TestTrackerSelection::selection_axisChangedSignal()
{
    QImPlotNode plot;
    QImPlotSelectionNode sel(&plot);
    QSignalSpy spy(&sel, &QImPlotSelectionNode::axisChanged);

    sel.setXAxis(QImPlotAxisId::X2);
    QCOMPARE(spy.count(), 1);

    sel.setYAxis(QImPlotAxisId::Y2);
    QCOMPARE(spy.count(), 2);
}

void TestTrackerSelection::selection_axisSignalDedup()
{
    QImPlotNode plot;
    QImPlotSelectionNode sel(&plot);
    QSignalSpy spy(&sel, &QImPlotSelectionNode::axisChanged);

    sel.setXAxis(QImPlotAxisId::X2);
    QCOMPARE(spy.count(), 1);  // Auto -> X2: signal fires

    // Same value — no additional signal
    sel.setXAxis(QImPlotAxisId::X2);
    QCOMPARE(spy.count(), 1);

    // Different value — signal fires
    sel.setXAxis(QImPlotAxisId::X3);
    QCOMPARE(spy.count(), 2);  // X2 -> X3: signal fires

    // Set Y axis to Auto (same as default) — no signal
    sel.setYAxis(QImPlotAxisId::Auto);
    QCOMPARE(spy.count(), 2);
}

void TestTrackerSelection::selection_readOnlyDefaults()
{
    QImPlotNode plot;
    QImPlotSelectionNode sel(&plot);

    // Before rendering, selection should be null/empty
    QRectF r = sel.selection();
    QVERIFY(r.isNull() || r.isEmpty());

    // No selection active
    QVERIFY(!sel.isSelected());
}

void TestTrackerSelection::selection_autoCancel()
{
    QImPlotNode plot;
    QImPlotSelectionNode sel(&plot);

    // Default is true per header
    QVERIFY(sel.isAutoCancel());

    sel.setAutoCancel(false);
    QVERIFY(!sel.isAutoCancel());

    sel.setAutoCancel(true);
    QVERIFY(sel.isAutoCancel());
}

void TestTrackerSelection::selection_autoCancelSignal()
{
    QImPlotNode plot;
    QImPlotSelectionNode sel(&plot);
    QSignalSpy spy(&sel, &QImPlotSelectionNode::autoCancelChanged);

    sel.setAutoCancel(false);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toBool(), false);

    sel.setAutoCancel(true);
    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.at(1).at(0).toBool(), true);
}

void TestTrackerSelection::selection_autoCancelSignalDedup()
{
    QImPlotNode plot;
    QImPlotSelectionNode sel(&plot);
    QSignalSpy spy(&sel, &QImPlotSelectionNode::autoCancelChanged);

    sel.setAutoCancel(false);
    QCOMPARE(spy.count(), 1);

    // Same value — no additional signal
    sel.setAutoCancel(false);
    QCOMPARE(spy.count(), 1);

    // Change back — signal fires
    sel.setAutoCancel(true);
    QCOMPARE(spy.count(), 2);

    // Same value again — no signal
    sel.setAutoCancel(true);
    QCOMPARE(spy.count(), 2);
}

QTEST_GUILESS_MAIN(TestTrackerSelection)
#include "tst_qimplot_tracker_selection.moc"
