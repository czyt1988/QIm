#include <QObject>
#include <QtTest>
#include <QFont>
#include <QGuiApplication>
#include <QFileInfo>

#include "QImFontFileHelper.h"

using namespace QIM;

class TestFontFileHelper : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    // Constructor/destructor don't crash
    void testConstructDestruct();
    // getRecommendedChineseFontPath returns valid path on Windows
    void testRecommendedChineseFontPath();
    // preloadCommonFonts populates the cache
    void testPreloadCommonFonts();
    // getAvailableFamilies returns a list after preload
    void testGetAvailableFamilies();
    // getFontPixelSize with pixelSize set
    void testFontPixelSizeWithPixelSize();
    // getFontPixelSize with pointSize set
    void testFontPixelSizeWithPointSize();
    // getFontFiles with empty cache returns empty list
    void testGetFontFilesEmptyCache();
    // clearCache actually empties cache
    void testClearCache();
};

void TestFontFileHelper::initTestCase()
{
}

void TestFontFileHelper::cleanupTestCase()
{
}

// ============================================================================
// Test 1: Constructor and destructor must not crash
// ============================================================================
void TestFontFileHelper::testConstructDestruct()
{
    QImFontFileHelper helper;
    // Simply constructing and destructing should not crash
    QVERIFY(true);
}

// ============================================================================
// Test 2: Recommended Chinese font path exists on Win / macOS / Linux
// ============================================================================
void TestFontFileHelper::testRecommendedChineseFontPath()
{
    std::string path = QImFontFileHelper::getRecommendedChineseFontPath();

    // On most platforms, at least one Chinese font should be available
    if (!path.empty()) {
        QVERIFY(QFileInfo::exists(QString::fromStdString(path)));
    }
    // It's acceptable for the path to be empty on minimal systems
}

// ============================================================================
// Test 3: preloadCommonFonts populates the cache and does not crash
// ============================================================================
void TestFontFileHelper::testPreloadCommonFonts()
{
    // Clear cache first to ensure we start fresh
    QImFontFileHelper::clearCache();

    // preloadCommonFonts should not crash; it scans system font directories
    QImFontFileHelper::preloadCommonFonts();

    // After preloading, cache should be populated (on most systems)
    QList<QString> families = QImFontFileHelper::getAvailableFamilies();
    QVERIFY(!families.isEmpty());
}

// ============================================================================
// Test 4: getAvailableFamilies returns cached fonts after preload
// ============================================================================
void TestFontFileHelper::testGetAvailableFamilies()
{
    // Cache should already be loaded from testPreloadCommonFonts
    QList<QString> families = QImFontFileHelper::getAvailableFamilies();
    QVERIFY(families.size() > 0);

    // Check that returned families are non-empty strings
    for (const QString& family : families) {
        QVERIFY(!family.isEmpty());
    }
}

// ============================================================================
// Test 5: getFontPixelSize with explicit pixelSize (no GUI dependencies)
// ============================================================================
void TestFontFileHelper::testFontPixelSizeWithPixelSize()
{
    QFont font("Arial", 16);
    font.setPixelSize(24);  // Explicit pixel size

    float pixSize = QImFontFileHelper::getFontPixelSize(font);
    QCOMPARE(pixSize, 24.0f);  // Should use pixelSize directly
}

// ============================================================================
// Test 6: getFontPixelSize with pointSize
// ============================================================================
void TestFontFileHelper::testFontPixelSizeWithPointSize()
{
    QFont font("Arial", 14);  // pointSize = 14

    float pixSize = QImFontFileHelper::getFontPixelSize(font);

    // With default DPI of 96: 14 * 96 / 72 = 18.67
    // The actual value depends on whether QGuiApplication has a primaryScreen
    // Just verify it's positive and reasonable
    QVERIFY(pixSize > 0.0f);
    QVERIFY(pixSize < 100.0f);  // Should be reasonable
}

// ============================================================================
// Test 7: getFontFiles with empty cache returns empty list
// ============================================================================
void TestFontFileHelper::testGetFontFilesEmptyCache()
{
    // Clear cache to ensure empty state
    QImFontFileHelper::clearCache();

    QFont font("NonexistentFont");
    QList<QString> files = QImFontFileHelper::getFontFiles(font);

    // Should return empty list when cache is empty
    QVERIFY(files.isEmpty());
}

// ============================================================================
// Test 8: clearCache actually empties the cache
// ============================================================================
void TestFontFileHelper::testClearCache()
{
    // First populate the cache
    QImFontFileHelper::preloadCommonFonts();
    QList<QString> before = QImFontFileHelper::getAvailableFamilies();
    QVERIFY(!before.isEmpty());

    // Then clear it
    QImFontFileHelper::clearCache();
    QList<QString> after = QImFontFileHelper::getAvailableFamilies();
    QVERIFY(after.isEmpty());
}

QTEST_GUILESS_MAIN(TestFontFileHelper)
#include "tst_font_file_helper.moc"
