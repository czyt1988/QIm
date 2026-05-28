#include <QObject>
#include <QtTest>
#include <memory>

#include "QImPlotTagXNode.h"
#include "QImPlotTagYNode.h"
#include "../core/TestHelpers.h"

using namespace QIM;

// ============================================================================
// TagX tests
// ============================================================================
class TestTagX : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void testDefaultConstruction();
    void testValueGetterSetter();
    void testValueChangedSignal();
    void testValueSignalDedup();
    void testColorGetterSetter();
    void testColorChangedSignal();
    void testTextGetterSetter();
    void testTextChangedSignal();
    void testRoundGetterSetter();
    void testRoundChangedSignal();
    void testType();
};

void TestTagX::initTestCase()
{
}

void TestTagX::cleanupTestCase()
{
}

void TestTagX::testDefaultConstruction()
{
    auto node = std::make_unique<QImPlotTagXNode>();
    QCOMPARE(node->value(), 0.0);
    QVERIFY(node->text().isEmpty());
    QVERIFY(!node->round());
    QVERIFY(node->isVisible());
}

void TestTagX::testValueGetterSetter()
{
    auto node = std::make_unique<QImPlotTagXNode>();

    node->setValue(3.14);
    QCOMPARE(node->value(), 3.14);

    node->setValue(-1.0);
    QCOMPARE(node->value(), -1.0);

    node->setValue(0.0);
    QCOMPARE(node->value(), 0.0);
}

void TestTagX::testValueChangedSignal()
{
    auto node = std::make_unique<QImPlotTagXNode>();
    QSignalSpy spy(node.get(), &QImPlotTagXNode::valueChanged);

    node->setValue(5.0);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toDouble(), 5.0);

    node->setValue(10.0);
    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.at(1).at(0).toDouble(), 10.0);
}

void TestTagX::testValueSignalDedup()
{
    auto node = std::make_unique<QImPlotTagXNode>();
    QSignalSpy spy(node.get(), &QImPlotTagXNode::valueChanged);

    node->setValue(2.5);
    QCOMPARE(spy.count(), 1);

    // Same value — no signal
    node->setValue(2.5);
    QCOMPARE(spy.count(), 1);
}

void TestTagX::testColorGetterSetter()
{
    auto node = std::make_unique<QImPlotTagXNode>();

    node->setColor(QColor(100, 150, 200));
    QCOMPARE(node->color(), QColor(100, 150, 200));

    node->setColor(QColor(Qt::red));
    QCOMPARE(node->color(), QColor(Qt::red));
}

void TestTagX::testColorChangedSignal()
{
    auto node = std::make_unique<QImPlotTagXNode>();
    QSignalSpy spy(node.get(), &QImPlotTagXNode::colorChanged);

    node->setColor(QColor(Qt::red));
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).value<QColor>(), QColor(Qt::red));

    // Same color — no signal
    node->setColor(QColor(Qt::red));
    QCOMPARE(spy.count(), 1);

    node->setColor(QColor(Qt::green));
    QCOMPARE(spy.count(), 2);
}

void TestTagX::testTextGetterSetter()
{
    auto node = std::make_unique<QImPlotTagXNode>();

    node->setText("Hello");
    QCOMPARE(node->text(), QString("Hello"));

    node->setText("");
    QVERIFY(node->text().isEmpty());
}

void TestTagX::testTextChangedSignal()
{
    auto node = std::make_unique<QImPlotTagXNode>();
    QSignalSpy spy(node.get(), &QImPlotTagXNode::textChanged);

    node->setText("First");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toString(), QString("First"));

    // Same text — no signal
    node->setText("First");
    QCOMPARE(spy.count(), 1);

    node->setText("Second");
    QCOMPARE(spy.count(), 2);
}

void TestTagX::testRoundGetterSetter()
{
    auto node = std::make_unique<QImPlotTagXNode>();
    QVERIFY(!node->round());

    node->setRound(true);
    QVERIFY(node->round());

    node->setRound(false);
    QVERIFY(!node->round());
}

void TestTagX::testRoundChangedSignal()
{
    auto node = std::make_unique<QImPlotTagXNode>();
    QSignalSpy spy(node.get(), &QImPlotTagXNode::roundChanged);

    node->setRound(true);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toBool(), true);

    // Same value — no signal
    node->setRound(true);
    QCOMPARE(spy.count(), 1);

    node->setRound(false);
    QCOMPARE(spy.count(), 2);
}

void TestTagX::testType()
{
    auto node = std::make_unique<QImPlotTagXNode>();
    QCOMPARE(node->type(), static_cast<int>(QImPlotItemNode::InnerType + 25));
}

// ============================================================================
// TagY tests
// ============================================================================
class TestTagY : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void testDefaultConstruction();
    void testValueGetterSetter();
    void testValueChangedSignal();
    void testValueSignalDedup();
    void testColorGetterSetter();
    void testColorChangedSignal();
    void testTextGetterSetter();
    void testTextChangedSignal();
    void testRoundGetterSetter();
    void testRoundChangedSignal();
    void testType();
};

void TestTagY::initTestCase()
{
}

void TestTagY::cleanupTestCase()
{
}

void TestTagY::testDefaultConstruction()
{
    auto node = std::make_unique<QImPlotTagYNode>();
    QCOMPARE(node->value(), 0.0);
    QVERIFY(node->text().isEmpty());
    QVERIFY(!node->round());
    QVERIFY(node->isVisible());
}

void TestTagY::testValueGetterSetter()
{
    auto node = std::make_unique<QImPlotTagYNode>();

    node->setValue(3.14);
    QCOMPARE(node->value(), 3.14);

    node->setValue(-1.0);
    QCOMPARE(node->value(), -1.0);

    node->setValue(0.0);
    QCOMPARE(node->value(), 0.0);
}

void TestTagY::testValueChangedSignal()
{
    auto node = std::make_unique<QImPlotTagYNode>();
    QSignalSpy spy(node.get(), &QImPlotTagYNode::valueChanged);

    node->setValue(5.0);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toDouble(), 5.0);

    node->setValue(10.0);
    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.at(1).at(0).toDouble(), 10.0);
}

void TestTagY::testValueSignalDedup()
{
    auto node = std::make_unique<QImPlotTagYNode>();
    QSignalSpy spy(node.get(), &QImPlotTagYNode::valueChanged);

    node->setValue(2.5);
    QCOMPARE(spy.count(), 1);

    // Same value — no signal
    node->setValue(2.5);
    QCOMPARE(spy.count(), 1);
}

void TestTagY::testColorGetterSetter()
{
    auto node = std::make_unique<QImPlotTagYNode>();

    node->setColor(QColor(100, 150, 200));
    QCOMPARE(node->color(), QColor(100, 150, 200));

    node->setColor(QColor(Qt::red));
    QCOMPARE(node->color(), QColor(Qt::red));
}

void TestTagY::testColorChangedSignal()
{
    auto node = std::make_unique<QImPlotTagYNode>();
    QSignalSpy spy(node.get(), &QImPlotTagYNode::colorChanged);

    node->setColor(QColor(Qt::red));
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).value<QColor>(), QColor(Qt::red));

    // Same color — no signal
    node->setColor(QColor(Qt::red));
    QCOMPARE(spy.count(), 1);

    node->setColor(QColor(Qt::green));
    QCOMPARE(spy.count(), 2);
}

void TestTagY::testTextGetterSetter()
{
    auto node = std::make_unique<QImPlotTagYNode>();

    node->setText("Hello");
    QCOMPARE(node->text(), QString("Hello"));

    node->setText("");
    QVERIFY(node->text().isEmpty());
}

void TestTagY::testTextChangedSignal()
{
    auto node = std::make_unique<QImPlotTagYNode>();
    QSignalSpy spy(node.get(), &QImPlotTagYNode::textChanged);

    node->setText("First");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toString(), QString("First"));

    // Same text — no signal
    node->setText("First");
    QCOMPARE(spy.count(), 1);

    node->setText("Second");
    QCOMPARE(spy.count(), 2);
}

void TestTagY::testRoundGetterSetter()
{
    auto node = std::make_unique<QImPlotTagYNode>();
    QVERIFY(!node->round());

    node->setRound(true);
    QVERIFY(node->round());

    node->setRound(false);
    QVERIFY(!node->round());
}

void TestTagY::testRoundChangedSignal()
{
    auto node = std::make_unique<QImPlotTagYNode>();
    QSignalSpy spy(node.get(), &QImPlotTagYNode::roundChanged);

    node->setRound(true);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toBool(), true);

    // Same value — no signal
    node->setRound(true);
    QCOMPARE(spy.count(), 1);

    node->setRound(false);
    QCOMPARE(spy.count(), 2);
}

void TestTagY::testType()
{
    auto node = std::make_unique<QImPlotTagYNode>();
    QCOMPARE(node->type(), static_cast<int>(QImPlotItemNode::InnerType + 26));
}

QTEST_GUILESS_MAIN(TestTagX)
#include "tst_qimplot_tagxy.moc"
