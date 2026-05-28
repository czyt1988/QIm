#include <QObject>
#include <QtTest>
#include <memory>

#include "QImPlotPieChartItemNode.h"
#include "../core/TestHelpers.h"

using namespace QIM;

class TestQImPlotPieChartItemNode : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    // Construction
    void testDefaultConstruction();

    // center property
    void testCenterGetterSetter();
    void testCenterChangedSignal();
    void testCenterSignalDedu();

    // radius property
    void testRadiusGetterSetter();
    void testRadiusChangedSignal();
    void testRadiusSignalDedu();

    // startAngle property
    void testStartAngleGetterSetter();
    void testStartAngleChangedSignal();

    // labelFormat property
    void testLabelFormatGetterSetter();
    void testLabelFormatChangedSignal();

    // normalize property
    void testNormalizeGetterSetter();
    void testNormalizeChangedSignal();

    // ignoreHidden property
    void testIgnoreHiddenGetterSetter();
    void testIgnoreHiddenChangedSignal();

    // exploding property
    void testExplodingGetterSetter();
    void testExplodingChangedSignal();

    // setData
    void testSetData();

private:
    std::unique_ptr<QImPlotPieChartItemNode> makeNode()
    {
        return std::make_unique<QImPlotPieChartItemNode>();
    }
};

void TestQImPlotPieChartItemNode::initTestCase()
{
}

void TestQImPlotPieChartItemNode::cleanupTestCase()
{
}

// ============================================================================
// Construction
// ============================================================================
void TestQImPlotPieChartItemNode::testDefaultConstruction()
{
    auto node = makeNode();
    QVERIFY(node != nullptr);
    QVERIFY(node->isVisible());
    QCOMPARE(node->type(), QImPlotPieChartItemNode::Type);
}

// ============================================================================
// center — getter/setter
// ============================================================================
void TestQImPlotPieChartItemNode::testCenterGetterSetter()
{
    auto node = makeNode();

    // Default value exists
    QPointF defaultCenter = node->center();
    Q_UNUSED(defaultCenter);

    // Set new value
    node->setCenter(QPointF(0.5, 0.5));
    QCOMPARE(node->center(), QPointF(0.5, 0.5));

    // Redundant set — getter still returns same
    node->setCenter(QPointF(0.5, 0.5));
    QCOMPARE(node->center(), QPointF(0.5, 0.5));

    // Different value
    node->setCenter(QPointF(0.3, 0.7));
    QCOMPARE(node->center(), QPointF(0.3, 0.7));
}

// ============================================================================
// center — signal emission
// ============================================================================
void TestQImPlotPieChartItemNode::testCenterChangedSignal()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotPieChartItemNode::centerChanged);

    node->setCenter(QPointF(0.5, 0.5));
    QCOMPARE(spy.count(), 1);

    node->setCenter(QPointF(0.3, 0.7));
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// center — signal deduplication
// ============================================================================
void TestQImPlotPieChartItemNode::testCenterSignalDedu()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotPieChartItemNode::centerChanged);

    node->setCenter(QPointF(0.5, 0.5));
    QCOMPARE(spy.count(), 1);

    // Same value — no duplicate signal
    node->setCenter(QPointF(0.5, 0.5));
    QCOMPARE(spy.count(), 1);
}

// ============================================================================
// radius — getter/setter
// ============================================================================
void TestQImPlotPieChartItemNode::testRadiusGetterSetter()
{
    auto node = makeNode();

    // Default value exists
    double defaultRadius = node->radius();
    QVERIFY(defaultRadius > 0.0);

    // Set new value
    node->setRadius(0.4);
    QCOMPARE(node->radius(), 0.4);

    // Redundant set — getter still returns same
    node->setRadius(0.4);
    QCOMPARE(node->radius(), 0.4);

    // Different value
    node->setRadius(0.25);
    QCOMPARE(node->radius(), 0.25);
}

// ============================================================================
// radius — signal emission
// ============================================================================
void TestQImPlotPieChartItemNode::testRadiusChangedSignal()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotPieChartItemNode::radiusChanged);

    node->setRadius(0.4);
    QCOMPARE(spy.count(), 1);

    node->setRadius(0.25);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// radius — signal deduplication
// ============================================================================
void TestQImPlotPieChartItemNode::testRadiusSignalDedu()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotPieChartItemNode::radiusChanged);

    node->setRadius(0.4);
    QCOMPARE(spy.count(), 1);

    // Same value — no duplicate signal
    node->setRadius(0.4);
    QCOMPARE(spy.count(), 1);
}

// ============================================================================
// startAngle — getter/setter
// ============================================================================
void TestQImPlotPieChartItemNode::testStartAngleGetterSetter()
{
    auto node = makeNode();

    // Default value exists
    double defaultAngle = node->startAngle();
    Q_UNUSED(defaultAngle);

    // Set new value
    node->setStartAngle(90.0);
    QCOMPARE(node->startAngle(), 90.0);

    // Different value
    node->setStartAngle(45.0);
    QCOMPARE(node->startAngle(), 45.0);
}

// ============================================================================
// startAngle — signal emission
// ============================================================================
void TestQImPlotPieChartItemNode::testStartAngleChangedSignal()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotPieChartItemNode::startAngleChanged);

    node->setStartAngle(90.0);
    QCOMPARE(spy.count(), 1);

    node->setStartAngle(45.0);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// labelFormat — getter/setter
// ============================================================================
void TestQImPlotPieChartItemNode::testLabelFormatGetterSetter()
{
    auto node = makeNode();

    // Default value exists
    QString defaultFormat = node->labelFormat();
    Q_UNUSED(defaultFormat);

    // Set new value
    node->setLabelFormat("%.0f%%");
    QCOMPARE(node->labelFormat(), QString("%.0f%%"));

    // Different value
    node->setLabelFormat("%.1f");
    QCOMPARE(node->labelFormat(), QString("%.1f"));
}

// ============================================================================
// labelFormat — signal emission
// ============================================================================
void TestQImPlotPieChartItemNode::testLabelFormatChangedSignal()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotPieChartItemNode::labelFormatChanged);

    node->setLabelFormat("%.0f%%");
    QCOMPARE(spy.count(), 1);

    node->setLabelFormat("%.1f");
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// normalize — getter/setter
// ============================================================================
void TestQImPlotPieChartItemNode::testNormalizeGetterSetter()
{
    auto node = makeNode();

    // Default value
    bool defaultNormalize = node->isNormalized();
    Q_UNUSED(defaultNormalize);

    // Set true
    node->setNormalize(true);
    QVERIFY(node->isNormalized());

    // Set false
    node->setNormalize(false);
    QVERIFY(!node->isNormalized());
}

// ============================================================================
// normalize — signal emission
// ============================================================================
void TestQImPlotPieChartItemNode::testNormalizeChangedSignal()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotPieChartItemNode::normalizeChanged);

    node->setNormalize(true);
    QCOMPARE(spy.count(), 1);

    node->setNormalize(false);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// ignoreHidden — getter/setter
// ============================================================================
void TestQImPlotPieChartItemNode::testIgnoreHiddenGetterSetter()
{
    auto node = makeNode();

    // Default value
    bool defaultIgnoreHidden = node->isIgnoreHidden();
    Q_UNUSED(defaultIgnoreHidden);

    // Set true
    node->setIgnoreHidden(true);
    QVERIFY(node->isIgnoreHidden());

    // Set false
    node->setIgnoreHidden(false);
    QVERIFY(!node->isIgnoreHidden());
}

// ============================================================================
// ignoreHidden — signal emission
// ============================================================================
void TestQImPlotPieChartItemNode::testIgnoreHiddenChangedSignal()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotPieChartItemNode::ignoreHiddenChanged);

    node->setIgnoreHidden(true);
    QCOMPARE(spy.count(), 1);

    node->setIgnoreHidden(false);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// exploding — getter/setter
// ============================================================================
void TestQImPlotPieChartItemNode::testExplodingGetterSetter()
{
    auto node = makeNode();

    // Default value
    bool defaultExploding = node->isExploding();
    Q_UNUSED(defaultExploding);

    // Set true
    node->setExploding(true);
    QVERIFY(node->isExploding());

    // Set false
    node->setExploding(false);
    QVERIFY(!node->isExploding());
}

// ============================================================================
// exploding — signal emission
// ============================================================================
void TestQImPlotPieChartItemNode::testExplodingChangedSignal()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotPieChartItemNode::explodingChanged);

    node->setExploding(true);
    QCOMPARE(spy.count(), 1);

    node->setExploding(false);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// setData — verify data assignment
// ============================================================================
void TestQImPlotPieChartItemNode::testSetData()
{
    auto node = makeNode();

    QStringList labels;
    labels << "Desktop" << "Web" << "Embedded" << "Tools";
    std::vector<double> values = {28.0, 34.0, 22.0, 16.0};

    node->setData(labels, values);
    QVERIFY(node->data() != nullptr);
}

QTEST_GUILESS_MAIN(TestQImPlotPieChartItemNode)
#include "tst_qimplotpiechartitemnode.moc"
