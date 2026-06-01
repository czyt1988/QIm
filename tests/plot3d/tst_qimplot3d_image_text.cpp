// ============================================
// Combined test: QImPlot3DImageItemNode + QImPlot3DTextItemNode + QImPlot3DDummyItemNode
// ============================================

#include <QObject>
#include <QtTest>
#include <memory>

#include "QImPlot3DImageItemNode.h"
#include "QImPlot3DTextItemNode.h"
#include "QImPlot3DDummyItemNode.h"
#include "../core/TestHelpers.h"

using namespace QIM;

class TestPlot3DImageTextDummy : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    // === QImPlot3DImageItemNode ===

    // Construction
    void testImageDefaultConstruction();

    // tintColor
    void testImageTintColorGetterSetter();
    void testImageTintColorChangedSignal();

    // === QImPlot3DTextItemNode ===

    // Construction
    void testTextDefaultConstruction();

    // text
    void testTextGetterSetter();
    void testTextChangedSignal();

    // x/y/z position
    void testTextXGetterSetter();
    void testTextYGetterSetter();
    void testTextZGetterSetter();
    void testTextPositionChangedSignal();

    // color
    void testTextColorGetterSetter();
    void testTextColorChangedSignal();

    // === QImPlot3DDummyItemNode ===

    // Construction
    void testDummyDefaultConstruction();

    // color
    void testDummyColorGetterSetter();
    void testDummyColorChangedSignal();
};

void TestPlot3DImageTextDummy::initTestCase()
{
}

void TestPlot3DImageTextDummy::cleanupTestCase()
{
}

// ============================================================================
// QImPlot3DImageItemNode - Construction
// ============================================================================
void TestPlot3DImageTextDummy::testImageDefaultConstruction()
{
    auto node = std::make_unique<QImPlot3DImageItemNode>();

    QCOMPARE(node->type(), QImPlot3DImageItemNode::Type);
    QVERIFY(node->label().isEmpty());
    QVERIFY(node->isVisible());
}

// ============================================================================
// QImPlot3DImageItemNode - tintColor
// ============================================================================
void TestPlot3DImageTextDummy::testImageTintColorGetterSetter()
{
    auto node = std::make_unique<QImPlot3DImageItemNode>();

    QColor c(128, 64, 32);
    node->setTintColor(c);
    QCOMPARE(node->tintColor(), c);

    QColor c2(0, 0, 0, 128);
    node->setTintColor(c2);
    QCOMPARE(node->tintColor(), c2);
}

void TestPlot3DImageTextDummy::testImageTintColorChangedSignal()
{
    auto node = std::make_unique<QImPlot3DImageItemNode>();
    QSignalSpy spy(node.get(), &QImPlot3DImageItemNode::tintColorChanged);

    QColor c1(200, 100, 50);
    node->setTintColor(c1);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).value<QColor>(), c1);

    // Same value - no signal
    node->setTintColor(c1);
    QCOMPARE(spy.count(), 1);

    QColor c2(10, 20, 30);
    node->setTintColor(c2);
    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.at(1).at(0).value<QColor>(), c2);
}

// ============================================================================
// QImPlot3DTextItemNode - Construction
// ============================================================================
void TestPlot3DImageTextDummy::testTextDefaultConstruction()
{
    auto node = std::make_unique<QImPlot3DTextItemNode>();

    QCOMPARE(node->type(), QImPlot3DTextItemNode::Type);
    QVERIFY(node->label().isEmpty());
    QVERIFY(node->text().isEmpty());
    QCOMPARE(node->x(), 0.0);
    QCOMPARE(node->y(), 0.0);
    QCOMPARE(node->z(), 0.0);
}

// ============================================================================
// QImPlot3DTextItemNode - text
// ============================================================================
void TestPlot3DImageTextDummy::testTextGetterSetter()
{
    auto node = std::make_unique<QImPlot3DTextItemNode>();

    node->setText("Hello 3D");
    QCOMPARE(node->text(), QString("Hello 3D"));

    node->setText("");
    QVERIFY(node->text().isEmpty());

    node->setText(QString::fromUtf8("\xe4\xb8\xad\xe6\x96\x87"));
    QCOMPARE(node->text(), QString::fromUtf8("\xe4\xb8\xad\xe6\x96\x87"));
}

void TestPlot3DImageTextDummy::testTextChangedSignal()
{
    auto node = std::make_unique<QImPlot3DTextItemNode>();
    QSignalSpy spy(node.get(), &QImPlot3DTextItemNode::textChanged);

    node->setText("First");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toString(), QString("First"));

    // Same value - no signal
    node->setText("First");
    QCOMPARE(spy.count(), 1);

    node->setText("Second");
    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.at(1).at(0).toString(), QString("Second"));
}

// ============================================================================
// QImPlot3DTextItemNode - x/y/z position
// ============================================================================
void TestPlot3DImageTextDummy::testTextXGetterSetter()
{
    auto node = std::make_unique<QImPlot3DTextItemNode>();

    QCOMPARE(node->x(), 0.0);
    node->setX(3.14);
    QCOMPARE(node->x(), 3.14);
    node->setX(-2.5);
    QCOMPARE(node->x(), -2.5);
}

void TestPlot3DImageTextDummy::testTextYGetterSetter()
{
    auto node = std::make_unique<QImPlot3DTextItemNode>();

    QCOMPARE(node->y(), 0.0);
    node->setY(1.5);
    QCOMPARE(node->y(), 1.5);
    node->setY(-0.75);
    QCOMPARE(node->y(), -0.75);
}

void TestPlot3DImageTextDummy::testTextZGetterSetter()
{
    auto node = std::make_unique<QImPlot3DTextItemNode>();

    QCOMPARE(node->z(), 0.0);
    node->setZ(9.99);
    QCOMPARE(node->z(), 9.99);
    node->setZ(-4.0);
    QCOMPARE(node->z(), -4.0);
}

void TestPlot3DImageTextDummy::testTextPositionChangedSignal()
{
    auto node = std::make_unique<QImPlot3DTextItemNode>();
    QSignalSpy spy(node.get(), &QImPlot3DTextItemNode::positionChanged);

    node->setX(1.0);
    QCOMPARE(spy.count(), 1);

    node->setY(2.0);
    QCOMPARE(spy.count(), 2);

    node->setZ(3.0);
    QCOMPARE(spy.count(), 3);

    // Same value - no signal
    node->setX(1.0);
    QCOMPARE(spy.count(), 3);
}

// ============================================================================
// QImPlot3DTextItemNode - color
// ============================================================================
void TestPlot3DImageTextDummy::testTextColorGetterSetter()
{
    auto node = std::make_unique<QImPlot3DTextItemNode>();

    // Default color is invalid (QColor())
    QVERIFY(!node->color().isValid());

    QColor c(255, 128, 0);
    node->setColor(c);
    QCOMPARE(node->color(), c);

    QColor c2(0, 255, 255, 200);
    node->setColor(c2);
    QCOMPARE(node->color(), c2);
}

void TestPlot3DImageTextDummy::testTextColorChangedSignal()
{
    auto node = std::make_unique<QImPlot3DTextItemNode>();
    QSignalSpy spy(node.get(), &QImPlot3DTextItemNode::colorChanged);

    QColor c1(100, 200, 50);
    node->setColor(c1);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).value<QColor>(), c1);

    // Same value - no signal
    node->setColor(c1);
    QCOMPARE(spy.count(), 1);

    QColor c2(0, 0, 0);
    node->setColor(c2);
    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.at(1).at(0).value<QColor>(), c2);
}

// ============================================================================
// QImPlot3DDummyItemNode - Construction
// ============================================================================
void TestPlot3DImageTextDummy::testDummyDefaultConstruction()
{
    auto node = std::make_unique<QImPlot3DDummyItemNode>();

    QCOMPARE(node->type(), QImPlot3DDummyItemNode::Type);
    QVERIFY(node->label().isEmpty());
    QVERIFY(node->isVisible());
    // Default color is invalid (QColor())
    QVERIFY(!node->color().isValid());
}

// ============================================================================
// QImPlot3DDummyItemNode - color
// ============================================================================
void TestPlot3DImageTextDummy::testDummyColorGetterSetter()
{
    auto node = std::make_unique<QImPlot3DDummyItemNode>();

    QColor c(0, 114, 189);
    node->setColor(c);
    QCOMPARE(node->color(), c);

    QColor c2(217, 83, 25, 180);
    node->setColor(c2);
    QCOMPARE(node->color(), c2);
}

void TestPlot3DImageTextDummy::testDummyColorChangedSignal()
{
    auto node = std::make_unique<QImPlot3DDummyItemNode>();
    QSignalSpy spy(node.get(), &QImPlot3DDummyItemNode::colorChanged);

    QColor c1(255, 0, 0);
    node->setColor(c1);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).value<QColor>(), c1);

    // Same value - no signal
    node->setColor(c1);
    QCOMPARE(spy.count(), 1);

    QColor c2(0, 255, 0);
    node->setColor(c2);
    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.at(1).at(0).value<QColor>(), c2);
}

QTEST_GUILESS_MAIN(TestPlot3DImageTextDummy)
#include "tst_qimplot3d_image_text.moc"