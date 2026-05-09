#include <QObject>
#include <QtTest>
#include <string>

#include "QImTrackedValue.hpp"
#include "imgui.h"

using namespace QIM;

class TestTrackedValue : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    // Default construction
    void testDefaultConstruction();
    // Value construction
    void testValueConstruction();
    // operator= sets dirty flag
    void testAssignmentMakesDirty();
    // mark_clean / clear resets the dirty flag
    void testMarkClean();
    // mark_dirty forces dirty state
    void testMarkDirty();
    // implicit conversion via operator T()
    void testOperatorT();
    // int template instantiation
    void testIntSpecialization();
    // float template instantiation
    void testFloatSpecialization();
    // double template instantiation
    void testDoubleSpecialization();
    // ImVec4 template instantiation
    void testImVec4Specialization();
    // bool operator (dirty state check)
    void testBoolOperator();
    // get_and_clear atomically gets value and clears dirty
    void testGetAndClear();
    // reset() returns to default value
    void testReset();
    // reset(const T&) sets new value
    void testResetWithValue();
    // swap
    void testSwap();
    // equality comparison
    void testEqualityComparison();
};

void TestTrackedValue::initTestCase()
{
}

void TestTrackedValue::cleanupTestCase()
{
}

// ============================================================================
// Test 1: Default construction — value is T{}, not dirty
// ============================================================================
void TestTrackedValue::testDefaultConstruction()
{
    QImTrackedValue<int> v;
    QCOMPARE(v.value(), 0);
    QVERIFY(!v.is_dirty());
    QVERIFY(!v.has_changed());

    QImTrackedValue<double> vd;
    QCOMPARE(vd.value(), 0.0);
    QVERIFY(!vd.is_dirty());
}

// ============================================================================
// Test 2: Value construction — holds the value, not dirty initially
// ============================================================================
void TestTrackedValue::testValueConstruction()
{
    QImTrackedValue<int> v(42);
    QCOMPARE(v.value(), 42);
    QVERIFY(!v.is_dirty());
    QVERIFY(!v.has_changed());

    // Move-construct from rvalue
    QImTrackedValue<std::string> vs(std::string("hello"));
    QCOMPARE(vs.value(), std::string("hello"));
    QVERIFY(!vs.is_dirty());
}

// ============================================================================
// Test 3: Assignment operator sets dirty flag when value changes
// ============================================================================
void TestTrackedValue::testAssignmentMakesDirty()
{
    QImTrackedValue<int> v(10);
    QVERIFY(!v.is_dirty());

    v = 20;  // Different value — should mark dirty
    QVERIFY(v.is_dirty());
    QVERIFY(v.has_changed());
    QCOMPARE(v.value(), 20);

    // Assign same value — should NOT mark dirty again
    v.mark_clean();  // Clear dirty
    v = 20;           // Same value
    QVERIFY(!v.is_dirty());
}

// ============================================================================
// Test 4: mark_clean / clear resets dirty flag
// ============================================================================
void TestTrackedValue::testMarkClean()
{
    QImTrackedValue<int> v(0);
    v = 5;
    QVERIFY(v.is_dirty());

    v.mark_clean();  // Using mark_clean
    QVERIFY(!v.is_dirty());

    v = 10;
    QVERIFY(v.is_dirty());

    v.clear();  // Using clear (alias)
    QVERIFY(!v.is_dirty());

    v.reset_dirty_flag();
    QVERIFY(!v.is_dirty());
}

// ============================================================================
// Test 5: mark_dirty forces dirty flag to true
// ============================================================================
void TestTrackedValue::testMarkDirty()
{
    QImTrackedValue<int> v(0);
    QVERIFY(!v.is_dirty());

    v.mark_dirty();
    QVERIFY(v.is_dirty());
    QVERIFY(v.has_changed());

    // mark_dirty again should stay dirty
    v.mark_dirty();
    QVERIFY(v.is_dirty());
}

// ============================================================================
// Test 6: operator T() implicit conversion for non-bool types
// ============================================================================
void TestTrackedValue::testOperatorT()
{
    QImTrackedValue<int> vi(123);
    // explicit operator int()
    int val = static_cast<int>(vi);
    QCOMPARE(val, 123);

    QImTrackedValue<double> vd(3.14);
    double dval = static_cast<double>(vd);
    QVERIFY(qFuzzyCompare(dval, 3.14));
}

// ============================================================================
// Test 7: int template specialization
// ============================================================================
void TestTrackedValue::testIntSpecialization()
{
    QImTrackedValue<int> v;
    QCOMPARE(v.value(), 0);

    v = -100;
    QCOMPARE(v.value(), -100);
    QVERIFY(v.is_dirty());

    v.clear();
    QVERIFY(!v.is_dirty());

    // operator* dereference
    *v = 50;  // Direct write — does NOT auto-set dirty
    QCOMPARE(v.value(), 50);
    // After direct write, we need to manually mark dirty
    v.mark_dirty();
    QVERIFY(v.is_dirty());
}

// ============================================================================
// Test 8: float template specialization (uses FloatComparator)
// ============================================================================
void TestTrackedValue::testFloatSpecialization()
{
    QImTrackedValue<float> v(1.5f);
    QVERIFY(qFuzzyCompare(v.value(), 1.5f));

    v = 1.5f + 1e-7f;  // Within default float epsilon (1e-6)
    // FloatComparator with epsilon=1e-6 should consider this NOT dirty
    QVERIFY(!v.is_dirty());
    QVERIFY(qFuzzyCompare(v.value(), 1.5f + 1e-7f));

    v = 2.0f;  // Clearly different
    QVERIFY(v.is_dirty());
    QCOMPARE(v.value(), 2.0f);
}

// ============================================================================
// Test 9: double template specialization (uses FloatComparator)
// ============================================================================
void TestTrackedValue::testDoubleSpecialization()
{
    QImTrackedValue<double> v(1.0);
    QVERIFY(qFuzzyCompare(v.value(), 1.0));

    v = 1.0 + 1e-9;  // Within default double epsilon (1e-6)
    // FloatComparator with epsilon=1e-6 should consider this NOT dirty
    QVERIFY(!v.is_dirty());

    v = 5.0;  // Clearly different
    QVERIFY(v.is_dirty());
    QCOMPARE(v.value(), 5.0);
}

// ============================================================================
// Test 10: ImVec4 template specialization
// ============================================================================
void TestTrackedValue::testImVec4Specialization()
{
    QImTrackedValue<ImVec4> v(ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
    QCOMPARE(v.value().x, 0.0f);
    QCOMPARE(v.value().y, 0.0f);
    QCOMPARE(v.value().z, 0.0f);
    QCOMPARE(v.value().w, 1.0f);
    QVERIFY(!v.is_dirty());

    v = ImVec4(1.0f, 0.5f, 0.0f, 0.8f);
    QVERIFY(v.is_dirty());
    QCOMPARE(v.value().x, 1.0f);
    QCOMPARE(v.value().y, 0.5f);
    QCOMPARE(v.value().z, 0.0f);
    QCOMPARE(v.value().w, 0.8f);

    // Same value should not mark dirty
    v.clear();
    v = ImVec4(1.0f, 0.5f, 0.0f, 0.8f);
    QVERIFY(!v.is_dirty());
}

// ============================================================================
// Test 11: operator bool() returns is_dirty()
// ============================================================================
void TestTrackedValue::testBoolOperator()
{
    QImTrackedValue<int> v(0);
    QVERIFY(!static_cast<bool>(v));

    v = 1;
    QVERIFY(static_cast<bool>(v));

    v.clear();
    QVERIFY(!static_cast<bool>(v));
}

// ============================================================================
// Test 12: get_and_clear returns value and clears dirty
// ============================================================================
void TestTrackedValue::testGetAndClear()
{
    QImTrackedValue<int> v(0);
    v = 42;
    QVERIFY(v.is_dirty());

    int val = v.get_and_clear();
    QCOMPARE(val, 42);
    QVERIFY(!v.is_dirty());   // Dirty flag should be cleared
    QCOMPARE(v.value(), 42);  // Value should remain
}

// ============================================================================
// Test 13: reset() returns to default-constructed value
// ============================================================================
void TestTrackedValue::testReset()
{
    QImTrackedValue<int> v(100);
    v = 200;
    QVERIFY(v.is_dirty());

    v.reset();
    QCOMPARE(v.value(), 0);   // T{} for int is 0
    QVERIFY(!v.is_dirty());   // Dirty flag cleared
}

// ============================================================================
// Test 14: reset(const T&) sets new value
// ============================================================================
void TestTrackedValue::testResetWithValue()
{
    QImTrackedValue<int> v(10);
    v.reset(99);
    QCOMPARE(v.value(), 99);
    QVERIFY(v.is_dirty());  // Value changed, should be dirty

    v.mark_clean();
    v.reset(99);  // Same value
    QVERIFY(!v.is_dirty());
}

// ============================================================================
// Test 15: swap exchanges contents
// ============================================================================
void TestTrackedValue::testSwap()
{
    QImTrackedValue<int> a(10);
    QImTrackedValue<int> b(20);

    a = 10;  // Ensure a knows its value
    b = 20;

    a.mark_clean();
    b.mark_dirty();

    a.swap(b);

    QCOMPARE(a.value(), 20);
    QCOMPARE(b.value(), 10);
    QVERIFY(a.is_dirty());    // b's dirty flag moved to a
    QVERIFY(!b.is_dirty());   // a's clean flag moved to b
}

// ============================================================================
// Test 16: Equality comparison operators
// ============================================================================
void TestTrackedValue::testEqualityComparison()
{
    QImTrackedValue<int> a(42);
    QImTrackedValue<int> b(42);
    QImTrackedValue<int> c(99);

    QVERIFY(a == b);
    QVERIFY(!(a == c));
    QVERIFY(a != c);
    QVERIFY(!(a != b));

    // Compare with raw value
    QVERIFY(a == 42);
    QVERIFY(!(a == 99));
    QVERIFY(a != 99);
    QVERIFY(!(a != 42));

    // Raw value on left side
    QVERIFY(42 == a);
    QVERIFY(99 != a);
}

QTEST_MAIN(TestTrackedValue)
#include "tst_tracked_value.moc"
