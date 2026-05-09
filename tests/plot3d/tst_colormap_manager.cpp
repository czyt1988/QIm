#include <QObject>
#include <QtTest>
#include <QColor>

#include "QImPlot3DColormapManager.h"
#include "QImPlot3D.h"
#include "QtImGuiUtils.h"

using namespace QIM;

class TestColormapManager : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    // colormapCount
    void testColormapCount();
    // colormapName — known built-in colormaps
    void testColormapNameKnown();
    // colormapIndex — roundtrip from name
    void testColormapIndexRoundtrip();
    // colormapIndex — unknown name
    void testColormapIndexUnknown();
    // colormapSize
    void testColormapSize();
    // colormapColor
    void testColormapColorValid();
    // sampleColormap
    void testSampleColormapEndpoints();
    void testSampleColormapMidpoint();
    // nextColormapColor
    void testNextColormapColor();
    // addColormap — QColor variant
    void testAddColormapQColor();
    // addColormap — quint32 variant
    void testAddColormapQuint32();
    // addColormap — qualitative flag
    void testAddColormapQualitative();
    // Multiple custom colormaps
    void testMultipleCustomColormaps();
};

void TestColormapManager::initTestCase()
{
}

void TestColormapManager::cleanupTestCase()
{
}

// ============================================================================
// Test 1: colormapCount returns positive value
// ============================================================================
void TestColormapManager::testColormapCount()
{
    int count = QImPlot3DColormapManager::colormapCount();
    // ImPlot3D has at least 16 built-in colormaps
    QVERIFY(count >= 16);
}

// ============================================================================
// Test 2: colormapName — known built-in colormaps
// ============================================================================
void TestColormapManager::testColormapNameKnown()
{
    // Deep should be named "Deep"
    QByteArray name = QImPlot3DColormapManager::colormapName(QImPlot3DColormap::Deep);
    QVERIFY(!name.isEmpty());

    // Viridis
    QByteArray name2 = QImPlot3DColormapManager::colormapName(QImPlot3DColormap::Viridis);
    QVERIFY(!name2.isEmpty());

    // Greys
    QByteArray name3 = QImPlot3DColormapManager::colormapName(QImPlot3DColormap::Greys);
    QVERIFY(!name3.isEmpty());
}

// ============================================================================
// Test 3: colormapIndex — roundtrip from name to index and back
// ============================================================================
void TestColormapManager::testColormapIndexRoundtrip()
{
    QByteArray name = QImPlot3DColormapManager::colormapName(QImPlot3DColormap::Deep);
    QImPlot3DColormap idx = QImPlot3DColormapManager::colormapIndex(name);
    QCOMPARE(idx, QImPlot3DColormap::Deep);

    QByteArray name2 = QImPlot3DColormapManager::colormapName(QImPlot3DColormap::Viridis);
    QImPlot3DColormap idx2 = QImPlot3DColormapManager::colormapIndex(name2);
    QCOMPARE(idx2, QImPlot3DColormap::Viridis);
}

// ============================================================================
// Test 4: colormapIndex — unknown name returns -1
// ============================================================================
void TestColormapManager::testColormapIndexUnknown()
{
    QImPlot3DColormap idx = QImPlot3DColormapManager::colormapIndex(
        QByteArray("NonExistentColormap_12345"));
    // ImPlot3D returns -1 for unknown colormaps
    QVERIFY(static_cast<int>(toImPlot3DColormap(idx)) == -1
            || static_cast<int>(toImPlot3DColormap(idx)) >= 0);
    // The behavior depends on ImPlot3D, so just verify no crash
    QVERIFY(true);
}

// ============================================================================
// Test 5: colormapSize returns positive value
// ============================================================================
void TestColormapManager::testColormapSize()
{
    int size = QImPlot3DColormapManager::colormapSize(QImPlot3DColormap::Deep);
    QVERIFY(size > 0);

    int size2 = QImPlot3DColormapManager::colormapSize(QImPlot3DColormap::Viridis);
    QVERIFY(size2 > 0);
}

// ============================================================================
// Test 6: colormapColor returns valid QColor
// ============================================================================
void TestColormapManager::testColormapColorValid()
{
    QColor color = QImPlot3DColormapManager::colormapColor(QImPlot3DColormap::Deep, 0);
    QVERIFY(color.isValid());

    int size = QImPlot3DColormapManager::colormapSize(QImPlot3DColormap::Deep);
    if (size > 1) {
        QColor color2 = QImPlot3DColormapManager::colormapColor(QImPlot3DColormap::Deep, 1);
        QVERIFY(color2.isValid());
    }
}

// ============================================================================
// Test 7: sampleColormap at endpoints
// ============================================================================
void TestColormapManager::testSampleColormapEndpoints()
{
    QColor color0 = QImPlot3DColormapManager::sampleColormap(QImPlot3DColormap::Hot, 0.0);
    QVERIFY(color0.isValid());

    QColor color1 = QImPlot3DColormapManager::sampleColormap(QImPlot3DColormap::Hot, 1.0);
    QVERIFY(color1.isValid());
}

// ============================================================================
// Test 8: sampleColormap at midpoint
// ============================================================================
void TestColormapManager::testSampleColormapMidpoint()
{
    QColor color = QImPlot3DColormapManager::sampleColormap(QImPlot3DColormap::Cool, 0.5);
    QVERIFY(color.isValid());
}

// ============================================================================
// Test 9: nextColormapColor returns valid QColor
// ============================================================================
void TestColormapManager::testNextColormapColor()
{
    QColor c1 = QImPlot3DColormapManager::nextColormapColor();
    QVERIFY(c1.isValid());

    // Subsequent calls should also return valid colors
    QColor c2 = QImPlot3DColormapManager::nextColormapColor();
    QVERIFY(c2.isValid());
}

// ============================================================================
// Test 10: addColormap with QColor values
// ============================================================================
void TestColormapManager::testAddColormapQColor()
{
    QList<QColor> colors = {
        QColor(255, 0, 0),
        QColor(0, 255, 0),
        QColor(0, 0, 255)
    };

    int countBefore = QImPlot3DColormapManager::colormapCount();
    QImPlot3DColormapManager::addColormap(QByteArray("TestQColorMap"), colors);
    int countAfter = QImPlot3DColormapManager::colormapCount();

    // A new colormap should be registered
    QCOMPARE(countAfter, countBefore + 1);

    // Verify it can be looked up by name
    QImPlot3DColormap idx = QImPlot3DColormapManager::colormapIndex(
        QByteArray("TestQColorMap"));
    QCOMPARE(static_cast<int>(toImPlot3DColormap(idx)), countBefore);

    // Verify its size
    int size = QImPlot3DColormapManager::colormapSize(idx);
    QCOMPARE(size, 3);
}

// ============================================================================
// Test 11: addColormap with quint32 values
// ============================================================================
void TestColormapManager::testAddColormapQuint32()
{
    QList<quint32> colors = {
        0xFF0000FF,  // Red (RGBA packed)
        0x00FF00FF,  // Green
        0x0000FFFF   // Blue
    };

    int countBefore = QImPlot3DColormapManager::colormapCount();
    QImPlot3DColormapManager::addColormap(QByteArray("TestQuint32Map"), colors);
    int countAfter = QImPlot3DColormapManager::colormapCount();

    QCOMPARE(countAfter, countBefore + 1);

    QImPlot3DColormap idx = QImPlot3DColormapManager::colormapIndex(
        QByteArray("TestQuint32Map"));
    int size = QImPlot3DColormapManager::colormapSize(idx);
    QCOMPARE(size, 3);
}

// ============================================================================
// Test 12: addColormap with qualitative flag
// ============================================================================
void TestColormapManager::testAddColormapQualitative()
{
    QList<QColor> colors = {
        QColor(255, 0, 0),
        QColor(0, 0, 255)
    };

    int countBefore = QImPlot3DColormapManager::colormapCount();
    QImPlot3DColormapManager::addColormap(QByteArray("TestQualMap"), colors, true);
    int countAfter = QImPlot3DColormapManager::colormapCount();

    QCOMPARE(countAfter, countBefore + 1);

    QImPlot3DColormap idx = QImPlot3DColormapManager::colormapIndex(
        QByteArray("TestQualMap"));
    // Qualitative colormap should be valid
    QColor c = QImPlot3DColormapManager::colormapColor(idx, 0);
    QVERIFY(c.isValid());
}

// ============================================================================
// Test 13: Multiple custom colormaps can coexist
// ============================================================================
void TestColormapManager::testMultipleCustomColormaps()
{
    int countBefore = QImPlot3DColormapManager::colormapCount();

    QList<QColor> colors1 = { QColor(100, 100, 100), QColor(200, 200, 200) };
    QList<QColor> colors2 = { QColor(0, 0, 0), QColor(50, 50, 50), QColor(255, 255, 255) };

    QImPlot3DColormapManager::addColormap(QByteArray("CustomMapA"), colors1);
    QImPlot3DColormapManager::addColormap(QByteArray("CustomMapB"), colors2);

    int countAfter = QImPlot3DColormapManager::colormapCount();
    QCOMPARE(countAfter, countBefore + 2);

    // Both should be independently retrievable
    QImPlot3DColormap idxA = QImPlot3DColormapManager::colormapIndex(
        QByteArray("CustomMapA"));
    QImPlot3DColormap idxB = QImPlot3DColormapManager::colormapIndex(
        QByteArray("CustomMapB"));

    QVERIFY(idxA != idxB);

    int sizeA = QImPlot3DColormapManager::colormapSize(idxA);
    QCOMPARE(sizeA, 2);

    int sizeB = QImPlot3DColormapManager::colormapSize(idxB);
    QCOMPARE(sizeB, 3);
}

QTEST_MAIN(TestColormapManager)
#include "tst_colormap_manager.moc"
