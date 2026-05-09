#include <QObject>
#include <QtTest>

#include "QImPlot3DAxisFormatter.h"

using namespace QIM;

// A concrete formatter for testing the abstract base
class TestFormatter : public QImPlot3DAxisFormatter
{
public:
    ~TestFormatter() override = default;

    QByteArray format(double value, const QByteArray& prefix) override
    {
        QByteArray result = QByteArray::number(value, 'f', 2);
        if (!prefix.isEmpty())
            result = prefix + result;
        return result;
    }
};

class TestPlot3DAxisFormatter : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    // Default formatter tests
    void testDefaultFormatterBasic();
    void testDefaultFormatterPositive();
    void testDefaultFormatterNegative();
    void testDefaultFormatterZero();
    void testDefaultFormatterWithPrefix();
    void testDefaultFormatterWithEmptyPrefix();
    // Custom formatter tests
    void testCustomFormatterBasic();
    void testCustomFormatterWithPrefix();
    void testCustomFormatterNegative();
    // Polymorphism
    void testFormatterPolymorphism();
};

void TestPlot3DAxisFormatter::initTestCase()
{
}

void TestPlot3DAxisFormatter::cleanupTestCase()
{
}

// ============================================================================
// Test 1: Default formatter formats numbers in %g style (6 significant digits)
// ============================================================================
void TestPlot3DAxisFormatter::testDefaultFormatterBasic()
{
    QImPlot3DDefaultAxisFormatter formatter;

    // 3.14159 has 6 significant digits
    QByteArray result = formatter.format(3.14159, QByteArray());
    QVERIFY(result.contains("3.14159"));

    // Integer values
    QByteArray r2 = formatter.format(42.0, QByteArray());
    QVERIFY(!r2.isEmpty());
}

// ============================================================================
// Test 2: Default formatter with positive values
// ============================================================================
void TestPlot3DAxisFormatter::testDefaultFormatterPositive()
{
    QImPlot3DDefaultAxisFormatter formatter;

    QByteArray result = formatter.format(100.5, QByteArray());
    QVERIFY(!result.isEmpty());
    // Should contain a number representation
    QVERIFY(result.contains("100"));
}

// ============================================================================
// Test 3: Default formatter with negative values
// ============================================================================
void TestPlot3DAxisFormatter::testDefaultFormatterNegative()
{
    QImPlot3DDefaultAxisFormatter formatter;

    QByteArray result = formatter.format(-50.25, QByteArray());
    QVERIFY(result.contains("-"));
    QVERIFY(result.contains("50"));
}

// ============================================================================
// Test 4: Default formatter with zero
// ============================================================================
void TestPlot3DAxisFormatter::testDefaultFormatterZero()
{
    QImPlot3DDefaultAxisFormatter formatter;

    QByteArray result = formatter.format(0.0, QByteArray());
    QVERIFY(!result.isEmpty());
    // Should contain "0"
    QVERIFY(result.contains("0"));
}

// ============================================================================
// Test 5: Default formatter prepends prefix when non-empty
// ============================================================================
void TestPlot3DAxisFormatter::testDefaultFormatterWithPrefix()
{
    QImPlot3DDefaultAxisFormatter formatter;

    QByteArray result = formatter.format(42.0, QByteArray("$"));
    QVERIFY(result.startsWith("$"));
    // After the prefix should be a number
    QVERIFY(result.size() > 1);
}

// ============================================================================
// Test 6: Default formatter with empty prefix
// ============================================================================
void TestPlot3DAxisFormatter::testDefaultFormatterWithEmptyPrefix()
{
    QImPlot3DDefaultAxisFormatter formatter;

    QByteArray withPrefix = formatter.format(10.0, QByteArray(""));
    QByteArray noPrefix   = formatter.format(10.0, QByteArray());
    // Both should produce the same output
    QCOMPARE(withPrefix, noPrefix);
}

// ============================================================================
// Test 7: Custom formatter formats with fixed precision
// ============================================================================
void TestPlot3DAxisFormatter::testCustomFormatterBasic()
{
    TestFormatter formatter;

    QByteArray result = formatter.format(3.14159, QByteArray());
    // Fixed precision with 2 decimal places
    QCOMPARE(result, QByteArray("3.14"));
}

// ============================================================================
// Test 8: Custom formatter with prefix
// ============================================================================
void TestPlot3DAxisFormatter::testCustomFormatterWithPrefix()
{
    TestFormatter formatter;

    QByteArray result = formatter.format(5.0, QByteArray("V:"));
    QCOMPARE(result, QByteArray("V:5.00"));
}

// ============================================================================
// Test 9: Custom formatter with negative values
// ============================================================================
void TestPlot3DAxisFormatter::testCustomFormatterNegative()
{
    TestFormatter formatter;

    QByteArray result = formatter.format(-2.5, QByteArray());
    QCOMPARE(result, QByteArray("-2.50"));
}

// ============================================================================
// Test 10: Polymorphism — base pointer calls derived format
// ============================================================================
void TestPlot3DAxisFormatter::testFormatterPolymorphism()
{
    QImPlot3DDefaultAxisFormatter defaultFmt;
    TestFormatter customFmt;

    QImPlot3DAxisFormatter* base = &defaultFmt;
    QByteArray r1 = base->format(42.0, QByteArray());
    QVERIFY(!r1.isEmpty());

    base = &customFmt;
    QByteArray r2 = base->format(42.0, QByteArray());
    QCOMPARE(r2, QByteArray("42.00"));
}

QTEST_MAIN(TestPlot3DAxisFormatter)
#include "tst_axis_formatter.moc"
