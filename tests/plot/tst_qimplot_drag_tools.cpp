#include <QObject>
#include <QtTest>
#include <QSignalSpy>
#include <memory>

#include "QImPlotDragPointNode.h"
#include "QImPlotDragLineXNode.h"
#include "QImPlotDragLineYNode.h"
#include "../core/TestHelpers.h"

using namespace QIM;

class TestDragTools : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    // --- QImPlotDragPointNode ---
    void pointDefaultConstruction();
    void pointPositionGetterSetter();
    void pointPositionSignalDedup();
    void pointColorGetterSetter();
    void pointFlagsGetterSetter();
    void pointFlagAccessors();
    void pointSizeAndId();

    // --- QImPlotDragLineXNode ---
    void lineXDefaultConstruction();
    void lineXValueGetterSetter();
    void lineXValueSignalDedup();
    void lineXColorGetterSetter();

    // --- QImPlotDragLineYNode ---
    void lineYDefaultConstruction();
    void lineYValueGetterSetter();
    void lineYValueSignalDedup();
    void lineYColorGetterSetter();
};

// ============================================================================
// QImPlotDragPointNode
// ============================================================================

void TestDragTools::pointDefaultConstruction()
{
    auto node = std::make_unique<QImPlotDragPointNode>();
    QVERIFY(node->type() == QImPlotDragPointNode::Type);
    QCOMPARE(node->position(), QPointF(0.0, 0.0));
    QVERIFY(!node->color().isValid());
    QCOMPARE(node->size(), 4.0f);
    QCOMPARE(node->id(), -1);
    QCOMPARE(node->flags(), 0);
}

void TestDragTools::pointPositionGetterSetter()
{
    auto node = std::make_unique<QImPlotDragPointNode>();
    node->setPosition(QPointF(3.5, 7.2));
    QCOMPARE(node->position(), QPointF(3.5, 7.2));

    node->setPosition(10.0, 20.0);
    QCOMPARE(node->position(), QPointF(10.0, 20.0));
}

void TestDragTools::pointPositionSignalDedup()
{
    auto node = std::make_unique<QImPlotDragPointNode>();
    QSignalSpy spy(node.get(), &QImPlotDragPointNode::positionChanged);

    node->setPosition(1.0, 2.0);
    QCOMPARE(spy.count(), 1);

    // Same value → no signal
    node->setPosition(1.0, 2.0);
    QCOMPARE(spy.count(), 1);

    // Different value → signal
    node->setPosition(1.0, 3.0);
    QCOMPARE(spy.count(), 2);
}

void TestDragTools::pointColorGetterSetter()
{
    auto node = std::make_unique<QImPlotDragPointNode>();
    QSignalSpy spy(node.get(), &QImPlotDragPointNode::colorChanged);

    node->setColor(Qt::red);
    QCOMPARE(node->color(), QColor(Qt::red));
    QCOMPARE(spy.count(), 1);

    // Same color → no signal
    node->setColor(Qt::red);
    QCOMPARE(spy.count(), 1);

    // Different color → signal
    node->setColor(Qt::blue);
    QCOMPARE(node->color(), QColor(Qt::blue));
    QCOMPARE(spy.count(), 2);
}

void TestDragTools::pointFlagsGetterSetter()
{
    auto node = std::make_unique<QImPlotDragPointNode>();
    QSignalSpy spy(node.get(), &QImPlotDragPointNode::flagsChanged);

    node->setFlags(7);
    QCOMPARE(node->flags(), 7);
    QCOMPARE(spy.count(), 1);

    // Same value → no signal
    node->setFlags(7);
    QCOMPARE(spy.count(), 1);
}

void TestDragTools::pointFlagAccessors()
{
    auto node = std::make_unique<QImPlotDragPointNode>();
    QSignalSpy spy(node.get(), &QImPlotDragPointNode::dragToolFlagChanged);

    // cursorsEnabled default true
    QVERIFY(node->isCursorsEnabled());
    node->setCursorsEnabled(false);
    QVERIFY(!node->isCursorsEnabled());
    QCOMPARE(spy.count(), 1);

    node->setCursorsEnabled(false); // same → no signal
    QCOMPARE(spy.count(), 1);

    node->setCursorsEnabled(true);
    QCOMPARE(spy.count(), 2);

    // fitEnabled default true
    QVERIFY(node->isFitEnabled());
    node->setFitEnabled(false);
    QCOMPARE(spy.count(), 3);

    // inputsEnabled default true
    QVERIFY(node->isInputsEnabled());
    node->setInputsEnabled(false);
    QCOMPARE(spy.count(), 4);

    // delayed default false
    QVERIFY(!node->isDelayed());
    node->setDelayed(true);
    QVERIFY(node->isDelayed());
    QCOMPARE(spy.count(), 5);
}

void TestDragTools::pointSizeAndId()
{
    auto node = std::make_unique<QImPlotDragPointNode>();
    QSignalSpy sizeSpy(node.get(), &QImPlotDragPointNode::sizeChanged);
    QSignalSpy idSpy(node.get(), &QImPlotDragPointNode::idChanged);

    node->setSize(8.5f);
    QCOMPARE(node->size(), 8.5f);
    QCOMPARE(sizeSpy.count(), 1);

    node->setSize(8.5f); // same → no signal
    QCOMPARE(sizeSpy.count(), 1);

    node->setId(42);
    QCOMPARE(node->id(), 42);
    QCOMPARE(idSpy.count(), 1);

    node->setId(42); // same → no signal
    QCOMPARE(idSpy.count(), 1);
}

// ============================================================================
// QImPlotDragLineXNode
// ============================================================================

void TestDragTools::lineXDefaultConstruction()
{
    auto node = std::make_unique<QImPlotDragLineXNode>();
    QVERIFY(node->type() == QImPlotDragLineXNode::Type);
    QCOMPARE(node->value(), 0.0);
    QVERIFY(!node->color().isValid());
    QCOMPARE(node->thickness(), 1.0f);
    QCOMPARE(node->id(), -1);
}

void TestDragTools::lineXValueGetterSetter()
{
    auto node = std::make_unique<QImPlotDragLineXNode>();
    QSignalSpy spy(node.get(), &QImPlotDragLineXNode::valueChanged);

    node->setValue(5.5);
    QCOMPARE(node->value(), 5.5);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toDouble(), 5.5);
}

void TestDragTools::lineXValueSignalDedup()
{
    auto node = std::make_unique<QImPlotDragLineXNode>();
    QSignalSpy spy(node.get(), &QImPlotDragLineXNode::valueChanged);

    node->setValue(3.0);
    QCOMPARE(spy.count(), 1);

    node->setValue(3.0); // same → no signal
    QCOMPARE(spy.count(), 1);

    node->setValue(3.001);
    QCOMPARE(spy.count(), 2);
}

void TestDragTools::lineXColorGetterSetter()
{
    auto node = std::make_unique<QImPlotDragLineXNode>();
    QSignalSpy spy(node.get(), &QImPlotDragLineXNode::colorChanged);

    node->setColor(QColor(100, 150, 200));
    QCOMPARE(node->color(), QColor(100, 150, 200));
    QCOMPARE(spy.count(), 1);

    node->setColor(QColor(100, 150, 200)); // same → no signal
    QCOMPARE(spy.count(), 1);
}

// ============================================================================
// QImPlotDragLineYNode
// ============================================================================

void TestDragTools::lineYDefaultConstruction()
{
    auto node = std::make_unique<QImPlotDragLineYNode>();
    QVERIFY(node->type() == QImPlotDragLineYNode::Type);
    QCOMPARE(node->value(), 0.0);
    QVERIFY(!node->color().isValid());
    QCOMPARE(node->thickness(), 1.0f);
    QCOMPARE(node->id(), -1);
}

void TestDragTools::lineYValueGetterSetter()
{
    auto node = std::make_unique<QImPlotDragLineYNode>();
    QSignalSpy spy(node.get(), &QImPlotDragLineYNode::valueChanged);

    node->setValue(9.1);
    QCOMPARE(node->value(), 9.1);
    QCOMPARE(spy.count(), 1);
}

void TestDragTools::lineYValueSignalDedup()
{
    auto node = std::make_unique<QImPlotDragLineYNode>();
    QSignalSpy spy(node.get(), &QImPlotDragLineYNode::valueChanged);

    node->setValue(7.0);
    QCOMPARE(spy.count(), 1);

    node->setValue(7.0); // same → no signal
    QCOMPARE(spy.count(), 1);

    node->setValue(7.5);
    QCOMPARE(spy.count(), 2);
}

void TestDragTools::lineYColorGetterSetter()
{
    auto node = std::make_unique<QImPlotDragLineYNode>();
    QSignalSpy spy(node.get(), &QImPlotDragLineYNode::colorChanged);

    node->setColor(Qt::green);
    QCOMPARE(node->color(), QColor(Qt::green));
    QCOMPARE(spy.count(), 1);

    node->setColor(Qt::green); // same → no signal
    QCOMPARE(spy.count(), 1);
}

QTEST_GUILESS_MAIN(TestDragTools)
#include "tst_qimplot_drag_tools.moc"
