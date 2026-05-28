#include <QObject>
#include <QtTest>
#include <memory>

#include "QImPlotLineItemNode.h"
#include "../core/TestHelpers.h"

using namespace QIM;

class TestQImPlotLineItemNode : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    // Construction
    void testDefaultConstruction();

    // segments property
    void testSegmentsGetterSetter();
    void testSegmentsChangedSignal();
    void testSegmentsSignalDedu();

    // loop property
    void testLoopGetterSetter();
    void testLoopChangedSignal();

    // skipNaN property
    void testSkipNaNGetterSetter();
    void testSkipNaNChangedSignal();

    // clippingEnabled property
    void testClippingEnabledGetterSetter();
    void testClippingEnabledChangedSignal();

    // shaded property
    void testShadedGetterSetter();
    void testShadedChangedSignal();

    // downsampleAlgorithm property
    void testDownsampleAlgorithmGetterSetter();
    void testDownsampleAlgorithmChangedSignal();
    void testDownsampleAlgorithmSignalDedu();

    // downsampleThreshold property
    void testDownsampleThresholdGetterSetter();
    void testDownsampleThresholdChangedSignal();
    void testDownsampleThresholdSignalDedu();

    // lineFlags raw access
    void testLineFlagsGetterSetter();
    void testLineFlagChangedSignal();

private:
    std::unique_ptr<QImPlotLineItemNode> makeNode()
    {
        return std::make_unique<QImPlotLineItemNode>();
    }
};

void TestQImPlotLineItemNode::initTestCase()
{
}

void TestQImPlotLineItemNode::cleanupTestCase()
{
}

// ============================================================================
// Construction
// ============================================================================
void TestQImPlotLineItemNode::testDefaultConstruction()
{
    auto node = makeNode();
    QVERIFY(node != nullptr);
    QVERIFY(node->isVisible());
    QCOMPARE(node->type(), QImPlotLineItemNode::Type);
}

// ============================================================================
// segments — getter/setter
// ============================================================================
void TestQImPlotLineItemNode::testSegmentsGetterSetter()
{
    auto node = makeNode();

    // Default: false
    QVERIFY(!node->isSegments());

    node->setSegments(true);
    QVERIFY(node->isSegments());

    node->setSegments(false);
    QVERIFY(!node->isSegments());
}

// ============================================================================
// segments — signal emission
// ============================================================================
void TestQImPlotLineItemNode::testSegmentsChangedSignal()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotLineItemNode::lineFlagChanged);

    node->setSegments(true);
    QCOMPARE(spy.count(), 1);

    node->setSegments(false);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// segments — signal deduplication
// ============================================================================
void TestQImPlotLineItemNode::testSegmentsSignalDedu()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotLineItemNode::lineFlagChanged);

    node->setSegments(true);
    QCOMPARE(spy.count(), 1);

    // Same value — no duplicate signal
    node->setSegments(true);
    QCOMPARE(spy.count(), 1);
}

// ============================================================================
// loop — getter/setter
// ============================================================================
void TestQImPlotLineItemNode::testLoopGetterSetter()
{
    auto node = makeNode();

    QVERIFY(!node->isLoop());

    node->setLoop(true);
    QVERIFY(node->isLoop());

    node->setLoop(false);
    QVERIFY(!node->isLoop());
}

// ============================================================================
// loop — signal emission
// ============================================================================
void TestQImPlotLineItemNode::testLoopChangedSignal()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotLineItemNode::lineFlagChanged);

    node->setLoop(true);
    QCOMPARE(spy.count(), 1);
}

// ============================================================================
// skipNaN — getter/setter
// ============================================================================
void TestQImPlotLineItemNode::testSkipNaNGetterSetter()
{
    auto node = makeNode();

    QVERIFY(!node->isSkipNaN());

    node->setSkipNaN(true);
    QVERIFY(node->isSkipNaN());

    node->setSkipNaN(false);
    QVERIFY(!node->isSkipNaN());
}

// ============================================================================
// skipNaN — signal emission
// ============================================================================
void TestQImPlotLineItemNode::testSkipNaNChangedSignal()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotLineItemNode::lineFlagChanged);

    node->setSkipNaN(true);
    QCOMPARE(spy.count(), 1);
}

// ============================================================================
// clippingEnabled — getter/setter
// ============================================================================
void TestQImPlotLineItemNode::testClippingEnabledGetterSetter()
{
    auto node = makeNode();

    // Default: clipping enabled (NoClip bit is 0)
    QVERIFY(node->isClippingEnabled());

    node->setClippingEnabled(false);
    QVERIFY(!node->isClippingEnabled());

    node->setClippingEnabled(true);
    QVERIFY(node->isClippingEnabled());
}

// ============================================================================
// clippingEnabled — signal emission (shares lineFlagChanged)
// ============================================================================
void TestQImPlotLineItemNode::testClippingEnabledChangedSignal()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotLineItemNode::lineFlagChanged);

    node->setClippingEnabled(false);
    QCOMPARE(spy.count(), 1);

    node->setClippingEnabled(true);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// shaded — getter/setter
// ============================================================================
void TestQImPlotLineItemNode::testShadedGetterSetter()
{
    auto node = makeNode();

    QVERIFY(!node->isShaded());

    node->setShaded(true);
    QVERIFY(node->isShaded());

    node->setShaded(false);
    QVERIFY(!node->isShaded());
}

// ============================================================================
// shaded — signal emission
// ============================================================================
void TestQImPlotLineItemNode::testShadedChangedSignal()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotLineItemNode::lineFlagChanged);

    node->setShaded(true);
    QCOMPARE(spy.count(), 1);
}

// ============================================================================
// downsampleAlgorithm — getter/setter
// ============================================================================
void TestQImPlotLineItemNode::testDownsampleAlgorithmGetterSetter()
{
    auto node = makeNode();

    // Default: Auto
    QCOMPARE(node->downsampleAlgorithm(), QImDownsampleAlgorithm::Auto);

    node->setDownsampleAlgorithm(QImDownsampleAlgorithm::LTTB);
    QCOMPARE(node->downsampleAlgorithm(), QImDownsampleAlgorithm::LTTB);

    node->setDownsampleAlgorithm(QImDownsampleAlgorithm::None);
    QCOMPARE(node->downsampleAlgorithm(), QImDownsampleAlgorithm::None);

    node->setDownsampleAlgorithm(QImDownsampleAlgorithm::MinMaxLTTB);
    QCOMPARE(node->downsampleAlgorithm(), QImDownsampleAlgorithm::MinMaxLTTB);
}

// ============================================================================
// downsampleAlgorithm — signal emission
// ============================================================================
void TestQImPlotLineItemNode::testDownsampleAlgorithmChangedSignal()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotLineItemNode::downsampleAlgorithmChanged);

    node->setDownsampleAlgorithm(QImDownsampleAlgorithm::LTTB);
    QCOMPARE(spy.count(), 1);

    node->setDownsampleAlgorithm(QImDownsampleAlgorithm::None);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// downsampleAlgorithm — signal deduplication
// ============================================================================
void TestQImPlotLineItemNode::testDownsampleAlgorithmSignalDedu()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotLineItemNode::downsampleAlgorithmChanged);

    node->setDownsampleAlgorithm(QImDownsampleAlgorithm::LTTB);
    QCOMPARE(spy.count(), 1);

    // Same value — no duplicate signal
    node->setDownsampleAlgorithm(QImDownsampleAlgorithm::LTTB);
    QCOMPARE(spy.count(), 1);
}

// ============================================================================
// downsampleThreshold — getter/setter
// ============================================================================
void TestQImPlotLineItemNode::testDownsampleThresholdGetterSetter()
{
    auto node = makeNode();

    // Default: 20000
    QCOMPARE(node->downsampleThreshold(), 20000);

    node->setDownsampleThreshold(5000);
    QCOMPARE(node->downsampleThreshold(), 5000);

    node->setDownsampleThreshold(100);
    QCOMPARE(node->downsampleThreshold(), 100);
}

// ============================================================================
// downsampleThreshold — signal emission
// ============================================================================
void TestQImPlotLineItemNode::testDownsampleThresholdChangedSignal()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotLineItemNode::downsampleThresholdChanged);

    node->setDownsampleThreshold(5000);
    QCOMPARE(spy.count(), 1);

    node->setDownsampleThreshold(10000);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// downsampleThreshold — signal deduplication
// ============================================================================
void TestQImPlotLineItemNode::testDownsampleThresholdSignalDedu()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotLineItemNode::downsampleThresholdChanged);

    node->setDownsampleThreshold(5000);
    QCOMPARE(spy.count(), 1);

    // Same value — no duplicate signal
    node->setDownsampleThreshold(5000);
    QCOMPARE(spy.count(), 1);
}

// ============================================================================
// lineFlags — raw getter/setter
// ============================================================================
void TestQImPlotLineItemNode::testLineFlagsGetterSetter()
{
    auto node = makeNode();

    int defaultFlags = node->lineFlags();
    Q_UNUSED(defaultFlags);

    node->setLineFlags(0x01);
    QCOMPARE(node->lineFlags(), 0x01);

    node->setLineFlags(0);
    QCOMPARE(node->lineFlags(), 0);
}

// ============================================================================
// lineFlags — signal emission via lineFlagChanged
// ============================================================================
void TestQImPlotLineItemNode::testLineFlagChangedSignal()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotLineItemNode::lineFlagChanged);

    node->setLineFlags(0x01);
    QCOMPARE(spy.count(), 1);
}

QTEST_GUILESS_MAIN(TestQImPlotLineItemNode)
#include "tst_qimplotlineitemnode.moc"
