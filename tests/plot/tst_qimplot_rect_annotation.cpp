#include <QObject>
#include <QtTest>
#include <QSignalSpy>
#include <QRectF>
#include <QPointF>
#include <QColor>

#include "QImPlotDragRectNode.h"
#include "QImPlotAnnotationNode.h"
#include "../core/TestHelpers.h"

using namespace QIM;

class TestRectAnnotation : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    // ---- QImPlotDragRectNode tests ----
    void testDragRectConstruction();
    void testDragRectRectGetterSetter();
    void testDragRectRectConvenienceOverload();
    void testDragRectColor();
    void testDragRectSignals();

    // ---- QImPlotAnnotationNode tests ----
    void testAnnotationConstruction();
    void testAnnotationText();
    void testAnnotationPosition();
    void testAnnotationPixelOffset();
    void testAnnotationColor();
    void testAnnotationClamp();
    void testAnnotationSignals();

private:
};

void TestRectAnnotation::initTestCase()
{
}

void TestRectAnnotation::cleanupTestCase()
{
}

// ============================================================================
// DragRect: Construction — default values and type
// ============================================================================
void TestRectAnnotation::testDragRectConstruction()
{
    QImPlotDragRectNode node;

    // Type identifier must be InnerType + 23
    QCOMPARE(node.type(), QImPlotItemNode::InnerType + 23);

    // Default rect should be null/empty
    QCOMPARE(node.rect(), QRectF());

    // Default color should be invalid (empty QColor)
    QCOMPARE(node.color(), QColor());
}

// ============================================================================
// DragRect: rect getter/setter via QRectF
// ============================================================================
void TestRectAnnotation::testDragRectRectGetterSetter()
{
    QImPlotDragRectNode node;

    QRectF r(1.0, 2.0, 3.0, 4.0);
    node.setRect(r);
    QCOMPARE(node.rect(), r);

    // Setting the same value again should not change anything
    node.setRect(r);
    QCOMPARE(node.rect(), r);

    // New value
    QRectF r2(0.0, 0.0, 10.0, 10.0);
    node.setRect(r2);
    QCOMPARE(node.rect(), r2);
}

// ============================================================================
// DragRect: convenience overload setRect(double, double, double, double)
// ============================================================================
void TestRectAnnotation::testDragRectRectConvenienceOverload()
{
    QImPlotDragRectNode node;

    node.setRect(1.5, 2.5, 5.0, 7.0);
    QRectF expected(1.5, 2.5, 5.0, 7.0);
    QCOMPARE(node.rect(), expected);
}

// ============================================================================
// DragRect: color getter/setter
// ============================================================================
void TestRectAnnotation::testDragRectColor()
{
    QImPlotDragRectNode node;

    QColor c(128, 64, 32, 200);
    node.setColor(c);
    QCOMPARE(node.color(), c);

    // Reset to invalid
    node.setColor(QColor());
    QCOMPARE(node.color(), QColor());
}

// ============================================================================
// DragRect: signal dedup — rectChanged emitted only on actual change
// ============================================================================
void TestRectAnnotation::testDragRectSignals()
{
    QImPlotDragRectNode node;

    QSignalSpy spy(&node, &QImPlotDragRectNode::rectChanged);

    node.setRect(1.0, 2.0, 3.0, 4.0);
    QCOMPARE(spy.count(), 1);

    // Setting same value again should NOT emit signal
    node.setRect(1.0, 2.0, 3.0, 4.0);
    QCOMPARE(spy.count(), 1);

    // Different value emits
    node.setRect(5.0, 6.0, 7.0, 8.0);
    QCOMPARE(spy.count(), 2);

    // colorChanged dedup
    QSignalSpy colorSpy(&node, &QImPlotDragRectNode::colorChanged);
    node.setColor(QColor(255, 0, 0));
    QCOMPARE(colorSpy.count(), 1);
    node.setColor(QColor(255, 0, 0));
    QCOMPARE(colorSpy.count(), 1);
}

// ============================================================================
// Annotation: Construction — default values and type
// ============================================================================
void TestRectAnnotation::testAnnotationConstruction()
{
    QImPlotAnnotationNode node;

    // Type identifier must be InnerType + 24
    QCOMPARE(node.type(), QImPlotItemNode::InnerType + 24);

    // Default position should be zero
    QCOMPARE(node.position(), QPointF());

    // Default text empty
    QCOMPARE(node.text(), QString());

    // Default color invalid
    QCOMPARE(node.color(), QColor());

    // Default pixel offset zero
    QCOMPARE(node.pixelOffset(), QPointF());

    // Default clamp false
    QCOMPARE(node.clamp(), false);
}

// ============================================================================
// Annotation: text getter/setter
// ============================================================================
void TestRectAnnotation::testAnnotationText()
{
    QImPlotAnnotationNode node;

    node.setText(QStringLiteral("Peak"));
    QCOMPARE(node.text(), QStringLiteral("Peak"));

    // Empty string
    node.setText(QString());
    QCOMPARE(node.text(), QString());
}

// ============================================================================
// Annotation: position getter/setter + convenience overload
// ============================================================================
void TestRectAnnotation::testAnnotationPosition()
{
    QImPlotAnnotationNode node;

    QPointF pos(3.5, 7.2);
    node.setPosition(pos);
    QCOMPARE(node.position(), pos);

    // Convenience overload
    node.setPosition(1.0, 2.0);
    QCOMPARE(node.position(), QPointF(1.0, 2.0));
}

// ============================================================================
// Annotation: pixelOffset getter/setter + convenience overload
// ============================================================================
void TestRectAnnotation::testAnnotationPixelOffset()
{
    QImPlotAnnotationNode node;

    QPointF offset(10.0, -5.0);
    node.setPixelOffset(offset);
    QCOMPARE(node.pixelOffset(), offset);

    // Convenience overload
    node.setPixelOffset(-3.0, 8.0);
    QCOMPARE(node.pixelOffset(), QPointF(-3.0, 8.0));
}

// ============================================================================
// Annotation: color getter/setter
// ============================================================================
void TestRectAnnotation::testAnnotationColor()
{
    QImPlotAnnotationNode node;

    QColor c(0, 128, 255, 180);
    node.setColor(c);
    QCOMPARE(node.color(), c);

    node.setColor(QColor());
    QCOMPARE(node.color(), QColor());
}

// ============================================================================
// Annotation: clamp getter/setter
// ============================================================================
void TestRectAnnotation::testAnnotationClamp()
{
    QImPlotAnnotationNode node;

    node.setClamp(true);
    QCOMPARE(node.clamp(), true);

    node.setClamp(false);
    QCOMPARE(node.clamp(), false);
}

// ============================================================================
// Annotation: signal dedup — textChanged emitted only on actual change
// ============================================================================
void TestRectAnnotation::testAnnotationSignals()
{
    QImPlotAnnotationNode node;

    QSignalSpy textSpy(&node, &QImPlotAnnotationNode::textChanged);
    node.setText(QStringLiteral("A"));
    QCOMPARE(textSpy.count(), 1);
    node.setText(QStringLiteral("A"));  // same value - no emit
    QCOMPARE(textSpy.count(), 1);
    node.setText(QStringLiteral("B"));
    QCOMPARE(textSpy.count(), 2);

    // positionChanged dedup
    QSignalSpy posSpy(&node, &QImPlotAnnotationNode::positionChanged);
    node.setPosition(1.0, 2.0);
    QCOMPARE(posSpy.count(), 1);
    node.setPosition(1.0, 2.0);
    QCOMPARE(posSpy.count(), 1);

    // clampChanged dedup
    QSignalSpy clampSpy(&node, &QImPlotAnnotationNode::clampChanged);
    node.setClamp(true);
    QCOMPARE(clampSpy.count(), 1);
    node.setClamp(true);
    QCOMPARE(clampSpy.count(), 1);
    node.setClamp(false);
    QCOMPARE(clampSpy.count(), 2);
}

QTEST_GUILESS_MAIN(TestRectAnnotation)
#include "tst_qimplot_rect_annotation.moc"
