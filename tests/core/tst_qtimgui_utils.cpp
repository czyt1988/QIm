#include <QObject>
#include <QtTest>
#include <cmath>

#include "QtImGuiUtils.h"
#include "imgui.h"

using namespace QIM;

class TestQtImGuiUtils : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    // QColor → ImVec4 conversion
    void testQColorToImVec4Opaque();
    void testQColorToImVec4WithAlpha();
    void testQColorToImVec4Black();
    void testQColorToImVec4White();
    // ImVec4 → QColor conversion
    void testImVec4ToQColorOpaque();
    void testImVec4ToQColorWithAlpha();
    void testImVec4ToQColorBlack();
    void testImVec4ToQColorWhite();
    // Round-trip: QColor → ImVec4 → QColor
    void testRoundtripIdentity();
    // QColor → ImU32 → QColor round-trip
    void testImU32Roundtrip();
    // fuzzyEqual float
    void testFuzzyEqualFloat();
    // fuzzyEqual double
    void testFuzzyEqualDouble();
    // fuzzyEqual ImVec2
    void testFuzzyEqualImVec2();
    // fuzzyEqual ImVec4
    void testFuzzyEqualImVec4();
    // Boundary: transparent QColor
    void testTransparentColor();
    // toImPlotPoint toQPointF and back
    void testImPlotPointRoundtrip();
};

void TestQtImGuiUtils::initTestCase()
{
}

void TestQtImGuiUtils::cleanupTestCase()
{
}

// ============================================================================
// QColor → ImVec4: opaque red
// ============================================================================
void TestQtImGuiUtils::testQColorToImVec4Opaque()
{
    QColor red(255, 0, 0);
    ImVec4 v = toImVec4(red);
    QVERIFY(qFuzzyCompare(v.x, 1.0f));
    QVERIFY(v.y < 1e-5f);
    QVERIFY(v.z < 1e-5f);
    QVERIFY(qFuzzyCompare(v.w, 1.0f));
}

// ============================================================================
// QColor → ImVec4: semi-transparent blue
// ============================================================================
void TestQtImGuiUtils::testQColorToImVec4WithAlpha()
{
    QColor blue(0, 0, 255, 128);
    ImVec4 v = toImVec4(blue);
    QVERIFY(v.x < 1e-5f);
    QVERIFY(v.y < 1e-5f);
    QVERIFY(qFuzzyCompare(v.z, 1.0f));
    // alpha 128/255 ≈ 0.502
    float expectedAlpha = 128.0f / 255.0f;
    QVERIFY(std::abs(v.w - expectedAlpha) < 0.01f);
}

// ============================================================================
// QColor → ImVec4: black
// ============================================================================
void TestQtImGuiUtils::testQColorToImVec4Black()
{
    QColor black(0, 0, 0);
    ImVec4 v = toImVec4(black);
    QVERIFY(v.x < 1e-5f);
    QVERIFY(v.y < 1e-5f);
    QVERIFY(v.z < 1e-5f);
    QVERIFY(qFuzzyCompare(v.w, 1.0f));
}

// ============================================================================
// QColor → ImVec4: white
// ============================================================================
void TestQtImGuiUtils::testQColorToImVec4White()
{
    QColor white(255, 255, 255);
    ImVec4 v = toImVec4(white);
    QVERIFY(qFuzzyCompare(v.x, 1.0f));
    QVERIFY(qFuzzyCompare(v.y, 1.0f));
    QVERIFY(qFuzzyCompare(v.z, 1.0f));
    QVERIFY(qFuzzyCompare(v.w, 1.0f));
}

// ============================================================================
// ImVec4 → QColor: opaque green
// ============================================================================
void TestQtImGuiUtils::testImVec4ToQColorOpaque()
{
    ImVec4 green(0.0f, 1.0f, 0.0f, 1.0f);
    QColor c = toQColor(green);
    QCOMPARE(c.red(), 0);
    QCOMPARE(c.green(), 255);
    QCOMPARE(c.blue(), 0);
    QCOMPARE(c.alpha(), 255);
}

// ============================================================================
// ImVec4 → QColor: with alpha
// ============================================================================
void TestQtImGuiUtils::testImVec4ToQColorWithAlpha()
{
    ImVec4 halfRed(0.5f, 0.0f, 0.0f, 0.5f);
    QColor c = toQColor(halfRed);
    QCOMPARE(c.red(), 127);      // 0.5 * 255 ≈ 127
    QCOMPARE(c.green(), 0);
    QCOMPARE(c.blue(), 0);
    QCOMPARE(c.alpha(), 127);
}

// ============================================================================
// ImVec4 → QColor: black (zero components)
// ============================================================================
void TestQtImGuiUtils::testImVec4ToQColorBlack()
{
    ImVec4 black(0.0f, 0.0f, 0.0f, 1.0f);
    QColor c = toQColor(black);
    QCOMPARE(c.red(), 0);
    QCOMPARE(c.green(), 0);
    QCOMPARE(c.blue(), 0);
    QCOMPARE(c.alpha(), 255);
}

// ============================================================================
// ImVec4 → QColor: white
// ============================================================================
void TestQtImGuiUtils::testImVec4ToQColorWhite()
{
    ImVec4 white(1.0f, 1.0f, 1.0f, 1.0f);
    QColor c = toQColor(white);
    QCOMPARE(c.red(), 255);
    QCOMPARE(c.green(), 255);
    QCOMPARE(c.blue(), 255);
    QCOMPARE(c.alpha(), 255);
}

// ============================================================================
// Round-trip QColor → ImVec4 → QColor (identity)
// ============================================================================
void TestQtImGuiUtils::testRoundtripIdentity()
{
    // Test several colors
    QColor colors[] = {
        QColor(255, 0, 0),
        QColor(0, 255, 0),
        QColor(0, 0, 255),
        QColor(255, 255, 255),
        QColor(0, 0, 0),
        QColor(128, 64, 32),
        QColor(200, 100, 50, 200),
    };

    for (const QColor& original : colors) {
        ImVec4 im = toImVec4(original);
        QColor restored = toQColor(im);
        QCOMPARE(restored.red(), original.red());
        QCOMPARE(restored.green(), original.green());
        QCOMPARE(restored.blue(), original.blue());
        QCOMPARE(restored.alpha(), original.alpha());
    }
}

// ============================================================================
// ImU32 round-trip: QColor → ImU32 → QColor
// ============================================================================
void TestQtImGuiUtils::testImU32Roundtrip()
{
    QColor colors[] = {
        QColor(255, 0, 0, 255),
        QColor(0, 128, 255, 200),
        QColor(50, 100, 150, 80),
        QColor(0, 0, 0, 255),
        QColor(255, 255, 255, 255),
    };

    for (const QColor& original : colors) {
        ImU32 packed = toImU32(original);
        QColor restored = toQColor(packed);
        QCOMPARE(restored.red(), original.red());
        QCOMPARE(restored.green(), original.green());
        QCOMPARE(restored.blue(), original.blue());
        QCOMPARE(restored.alpha(), original.alpha());
    }
}

// ============================================================================
// fuzzyEqual: float
// ============================================================================
void TestQtImGuiUtils::testFuzzyEqualFloat()
{
    QVERIFY(fuzzyEqual(1.0f, 1.0f));
    QVERIFY(fuzzyEqual(1.0f, 1.0f + 1e-6f));
    QVERIFY(!fuzzyEqual(1.0f, 1.0f + 1e-4f));

    // Custom epsilon
    QVERIFY(fuzzyEqual(1.0f, 1.1f, 0.2f));
    QVERIFY(!fuzzyEqual(1.0f, 1.1f, 0.05f));
}

// ============================================================================
// fuzzyEqual: double
// ============================================================================
void TestQtImGuiUtils::testFuzzyEqualDouble()
{
    QVERIFY(fuzzyEqual(1.0, 1.0));
    QVERIFY(fuzzyEqual(1.0, 1.0 + 1e-10));
    QVERIFY(!fuzzyEqual(1.0, 1.0 + 1e-8));
}

// ============================================================================
// fuzzyEqual: ImVec2
// ============================================================================
void TestQtImGuiUtils::testFuzzyEqualImVec2()
{
    ImVec2 a(1.0f, 2.0f);
    ImVec2 b(1.0f, 2.0f);
    ImVec2 c(1.0f, 2.1f);

    QVERIFY(fuzzyEqual(a, b));
    QVERIFY(!fuzzyEqual(a, c));
}

// ============================================================================
// fuzzyEqual: ImVec4
// ============================================================================
void TestQtImGuiUtils::testFuzzyEqualImVec4()
{
    ImVec4 a(0.1f, 0.2f, 0.3f, 0.4f);
    ImVec4 b(0.1f, 0.2f, 0.3f, 0.4f);
    ImVec4 c(0.1f, 0.2f, 0.3f, 0.5f);

    QVERIFY(fuzzyEqual(a, b));
    QVERIFY(!fuzzyEqual(a, c));
}

// ============================================================================
// Transparent QColor
// ============================================================================
void TestQtImGuiUtils::testTransparentColor()
{
    QColor transparent(0, 0, 0, 0);
    ImVec4 v = toImVec4(transparent);
    QVERIFY(v.x < 1e-5f);
    QVERIFY(v.y < 1e-5f);
    QVERIFY(v.z < 1e-5f);
    QVERIFY(v.w < 1e-5f);

    QColor back = toQColor(v);
    QCOMPARE(back.alpha(), 0);
}

// ============================================================================
// ImPlotPoint ↔ QPointF round-trip
// ============================================================================
void TestQtImGuiUtils::testImPlotPointRoundtrip()
{
    QPointF orig(3.14, 2.718);
    ImPlotPoint imp = toImPlotPoint(orig);
    QCOMPARE(imp.x, 3.14);
    QCOMPARE(imp.y, 2.718);

    QPointF restored = toQPointF(imp);
    QVERIFY(qFuzzyCompare(restored.x(), orig.x()));
    QVERIFY(qFuzzyCompare(restored.y(), orig.y()));
}

QTEST_MAIN(TestQtImGuiUtils)
#include "tst_qtimgui_utils.moc"
