#include <QObject>
#include <QtTest>
#include <memory>

#include "QImPlotTextItemNode.h"
#include "../core/TestHelpers.h"

using namespace QIM;

class TestQImPlotTextItemNode : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    // Construction
    void testDefaultConstruction();

    // text property
    void testTextGetterSetter();
    void testTextChangedSignal();
    void testTextSignalDedu();

    // position property
    void testPositionGetterSetter();
    void testPositionChangedSignal();

    // pixelOffset property
    void testPixelOffsetGetterSetter();
    void testPixelOffsetChangedSignal();

    // vertical property
    void testVerticalGetterSetter();
    void testVerticalChangedSignal();

    // color property
    void testColorGetterSetter();
    void testColorChangedSignal();

private:
    std::unique_ptr<QImPlotTextItemNode> makeNode()
    {
        return std::make_unique<QImPlotTextItemNode>();
    }
};

void TestQImPlotTextItemNode::initTestCase()
{
}

void TestQImPlotTextItemNode::cleanupTestCase()
{
}

// ============================================================================
// Construction
// ============================================================================
void TestQImPlotTextItemNode::testDefaultConstruction()
{
    auto node = makeNode();
    QVERIFY(node != nullptr);
    QVERIFY(node->isVisible());
    QCOMPARE(node->type(), QImPlotTextItemNode::Type);
}

// ============================================================================
// text — getter/setter
// ============================================================================
void TestQImPlotTextItemNode::testTextGetterSetter()
{
    auto node = makeNode();

    // Default value is empty
    QVERIFY(node->text().isEmpty());

    // Set new value
    node->setText("Hello");
    QCOMPARE(node->text(), QString("Hello"));

    // Redundant set (same value) — getter still returns same
    node->setText("Hello");
    QCOMPARE(node->text(), QString("Hello"));

    // Set different value
    node->setText("World");
    QCOMPARE(node->text(), QString("World"));
}

// ============================================================================
// text — signal
// ============================================================================
void TestQImPlotTextItemNode::testTextChangedSignal()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotTextItemNode::textChanged);

    node->setText("Hello");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toString(), QString("Hello"));
}

// ============================================================================
// text — signal deduplication
// ============================================================================
void TestQImPlotTextItemNode::testTextSignalDedu()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotTextItemNode::textChanged);

    node->setText("Hello");
    QCOMPARE(spy.count(), 1);

    // Same value — no duplicate signal
    node->setText("Hello");
    QCOMPARE(spy.count(), 1);

    // Different value — signal emitted
    node->setText("World");
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// position — getter/setter
// ============================================================================
void TestQImPlotTextItemNode::testPositionGetterSetter()
{
    auto node = makeNode();

    // Default value is (0, 0)
    QCOMPARE(node->position(), QPointF(0.0, 0.0));

    // Set via QPointF
    node->setPosition(QPointF(1.5, 2.5));
    QCOMPARE(node->position(), QPointF(1.5, 2.5));

    // Set via convenience overload
    node->setPosition(3.0, 4.0);
    QCOMPARE(node->position(), QPointF(3.0, 4.0));

    // Redundant set — getter still returns same
    node->setPosition(3.0, 4.0);
    QCOMPARE(node->position(), QPointF(3.0, 4.0));
}

// ============================================================================
// position — signal
// ============================================================================
void TestQImPlotTextItemNode::testPositionChangedSignal()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotTextItemNode::positionChanged);

    node->setPosition(1.0, 2.0);
    QCOMPARE(spy.count(), 1);

    // Same value — no duplicate signal
    node->setPosition(1.0, 2.0);
    QCOMPARE(spy.count(), 1);

    // Different value — signal emitted
    node->setPosition(3.0, 4.0);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// pixelOffset — getter/setter
// ============================================================================
void TestQImPlotTextItemNode::testPixelOffsetGetterSetter()
{
    auto node = makeNode();

    // Default value is (0, 0)
    QCOMPARE(node->pixelOffset(), QPointF(0.0, 0.0));

    // Set via QPointF
    node->setPixelOffset(QPointF(10.0, 20.0));
    QCOMPARE(node->pixelOffset(), QPointF(10.0, 20.0));

    // Set via convenience overload
    node->setPixelOffset(30.0f, 40.0f);
    QCOMPARE(node->pixelOffset(), QPointF(30.0, 40.0));

    // Redundant set — getter still returns same
    node->setPixelOffset(30.0f, 40.0f);
    QCOMPARE(node->pixelOffset(), QPointF(30.0, 40.0));
}

// ============================================================================
// pixelOffset — signal
// ============================================================================
void TestQImPlotTextItemNode::testPixelOffsetChangedSignal()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotTextItemNode::pixelOffsetChanged);

    node->setPixelOffset(5.0f, 6.0f);
    QCOMPARE(spy.count(), 1);

    // Same value — no duplicate signal
    node->setPixelOffset(5.0f, 6.0f);
    QCOMPARE(spy.count(), 1);

    // Different value — signal emitted
    node->setPixelOffset(7.0f, 8.0f);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// vertical — getter/setter
// ============================================================================
void TestQImPlotTextItemNode::testVerticalGetterSetter()
{
    auto node = makeNode();

    // Default is false
    QVERIFY(!node->isVertical());

    // Set to true
    node->setVertical(true);
    QVERIFY(node->isVertical());

    // Set to false
    node->setVertical(false);
    QVERIFY(!node->isVertical());
}

// ============================================================================
// vertical — signal
// ============================================================================
void TestQImPlotTextItemNode::testVerticalChangedSignal()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotTextItemNode::verticalChanged);

    node->setVertical(true);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toBool(), true);
}

// ============================================================================
// color — getter/setter
// ============================================================================
void TestQImPlotTextItemNode::testColorGetterSetter()
{
    auto node = makeNode();

    // Default color is invalid (not set)
    QVERIFY(!node->color().isValid());

    // Set new color
    QColor c(255, 0, 0);
    node->setColor(c);
    QCOMPARE(node->color(), c);

    // Set different color
    QColor c2(0, 255, 0);
    node->setColor(c2);
    QCOMPARE(node->color(), c2);
}

// ============================================================================
// color — signal
// ============================================================================
void TestQImPlotTextItemNode::testColorChangedSignal()
{
    auto node = makeNode();
    QSignalSpy spy(node.get(), &QImPlotTextItemNode::colorChanged);

    node->setColor(QColor(255, 0, 0));
    QCOMPARE(spy.count(), 1);
}

QTEST_GUILESS_MAIN(TestQImPlotTextItemNode)
#include "tst_qimplottextitemnode.moc"
