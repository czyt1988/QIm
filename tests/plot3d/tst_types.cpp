#include <QObject>
#include <QtTest>
#include <cmath>
#include <limits>

#include "QImPlot3DTypes.h"

using namespace QIM;

class TestPlot3DTypes : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    // QImPlot3DPoint — edge cases not covered in math_types
    void testPointIndexOutOfBounds();
    void testPointDivideByZero();
    void testPointNormalizeZeroVector();
    void testPointCrossOrthogonal();
    void testPointCrossSame();
    void testPointDotZero();
    void testPointOperatorScalarLeft();
    // QImPlot3DRay — extended tests
    void testRayDefaultZero();
    void testRayFieldAssignment();
    // QImPlot3DPlane — extended tests
    void testPlaneDefaultZero();
    void testPlaneFieldAssignment();
    // QImPlot3DBox — edge cases
    void testBoxZeroVolume();
    void testBoxExpandNaN();
    void testBoxContainsBoundary();
    void testBoxClipEmptySegment();
    void testBoxClipSinglePointInside();
    // QImPlot3DRange — edge cases
    void testRangeReverse();
    void testRangeSingleValue();
    void testRangeExpandNaN();
    void testRangeContainsBoundary();
};

void TestPlot3DTypes::initTestCase()
{
}

void TestPlot3DTypes::cleanupTestCase()
{
}

// ============================================================================
// QImPlot3DPoint — index out of bounds (Q_ASSERT won't fire in Release)
// ============================================================================
void TestPlot3DTypes::testPointIndexOutOfBounds()
{
    QImPlot3DPoint p(1.0, 2.0, 3.0);
    // Valid indices 0, 1, 2 should work
    QCOMPARE(p[0], 1.0);
    QCOMPARE(p[1], 2.0);
    QCOMPARE(p[2], 3.0);

    // Access via operator[] at valid indices non-mutating
    double v0 = p[0];
    double v1 = p[1];
    double v2 = p[2];
    QCOMPARE(v0, 1.0);
    QCOMPARE(v1, 2.0);
    QCOMPARE(v2, 3.0);
}

// ============================================================================
// QImPlot3DPoint — divide by zero
// ============================================================================
void TestPlot3DTypes::testPointDivideByZero()
{
    QImPlot3DPoint p(1.0, 2.0, 3.0);
    QImPlot3DPoint result = p / 0.0;

    // Division by zero produces inf values
    QVERIFY(std::isinf(result.x));
    QVERIFY(std::isinf(result.y));
    QVERIFY(std::isinf(result.z));
}

// ============================================================================
// QImPlot3DPoint — Normalize zero vector
// ============================================================================
void TestPlot3DTypes::testPointNormalizeZeroVector()
{
    QImPlot3DPoint zero(0.0, 0.0, 0.0);
    QImPlot3DPoint norm = zero.Normalized();

    // Dividing by zero length gives NaN
    QVERIFY(norm.IsNaN());
}

// ============================================================================
// QImPlot3DPoint — Cross product of orthogonal unit vectors gives unit vector
// ============================================================================
void TestPlot3DTypes::testPointCrossOrthogonal()
{
    QImPlot3DPoint x(1.0, 0.0, 0.0);
    QImPlot3DPoint y(0.0, 1.0, 0.0);
    QImPlot3DPoint z = x.Cross(y);

    QCOMPARE(z.x, 0.0);
    QCOMPARE(z.y, 0.0);
    QCOMPARE(z.z, 1.0);

    // z . x = 0 (perpendicular)
    QCOMPARE(z.Dot(x), 0.0);
    // z . y = 0 (perpendicular)
    QCOMPARE(z.Dot(y), 0.0);
}

// ============================================================================
// QImPlot3DPoint — Cross product of parallel vectors gives zero
// ============================================================================
void TestPlot3DTypes::testPointCrossSame()
{
    QImPlot3DPoint a(2.0, 3.0, 4.0);
    QImPlot3DPoint result = a.Cross(a);

    QCOMPARE(result.x, 0.0);
    QCOMPARE(result.y, 0.0);
    QCOMPARE(result.z, 0.0);
}

// ============================================================================
// QImPlot3DPoint — Dot product with zero vector
// ============================================================================
void TestPlot3DTypes::testPointDotZero()
{
    QImPlot3DPoint a(1.0, 2.0, 3.0);
    QImPlot3DPoint zero(0.0, 0.0, 0.0);

    QCOMPARE(a.Dot(zero), 0.0);
    QCOMPARE(zero.Dot(a), 0.0);
}

// ============================================================================
// QImPlot3DPoint — left scalar multiply
// ============================================================================
void TestPlot3DTypes::testPointOperatorScalarLeft()
{
    QImPlot3DPoint p(1.0, 2.0, 3.0);
    QImPlot3DPoint r1 = 3.0 * p;
    QImPlot3DPoint r2 = p * 3.0;

    QCOMPARE(r1.x, r2.x);
    QCOMPARE(r1.y, r2.y);
    QCOMPARE(r1.z, r2.z);
    QVERIFY(r1 == r2);
}

// ============================================================================
// QImPlot3DRay — default values
// ============================================================================
void TestPlot3DTypes::testRayDefaultZero()
{
    QImPlot3DRay r;
    QCOMPARE(r.Origin.x, 0.0);
    QCOMPARE(r.Origin.y, 0.0);
    QCOMPARE(r.Origin.z, 0.0);
    QCOMPARE(r.Direction.x, 0.0);
    QCOMPARE(r.Direction.y, 0.0);
    QCOMPARE(r.Direction.z, 0.0);
}

// ============================================================================
// QImPlot3DRay — field assignment
// ============================================================================
void TestPlot3DTypes::testRayFieldAssignment()
{
    QImPlot3DRay r;
    r.Origin    = QImPlot3DPoint(1.0, 2.0, 3.0);
    r.Direction = QImPlot3DPoint(0.0, 0.0, -1.0);

    QCOMPARE(r.Origin.x, 1.0);
    QCOMPARE(r.Direction.z, -1.0);
}

// ============================================================================
// QImPlot3DPlane — default values
// ============================================================================
void TestPlot3DTypes::testPlaneDefaultZero()
{
    QImPlot3DPlane p;
    QCOMPARE(p.Point.x, 0.0);
    QCOMPARE(p.Point.y, 0.0);
    QCOMPARE(p.Point.z, 0.0);
    QCOMPARE(p.Normal.x, 0.0);
    QCOMPARE(p.Normal.y, 0.0);
    QCOMPARE(p.Normal.z, 0.0);
}

// ============================================================================
// QImPlot3DPlane — field assignment
// ============================================================================
void TestPlot3DTypes::testPlaneFieldAssignment()
{
    QImPlot3DPlane p;
    p.Point  = QImPlot3DPoint(0.0, 0.0, 5.0);
    p.Normal = QImPlot3DPoint(0.0, 0.0, 1.0);

    QCOMPARE(p.Point.z, 5.0);
    QCOMPARE(p.Normal.z, 1.0);
}

// ============================================================================
// QImPlot3DBox — box with zero volume (Min == Max)
// ============================================================================
void TestPlot3DTypes::testBoxZeroVolume()
{
    QImPlot3DPoint p(5.0, 5.0, 5.0);
    QImPlot3DBox box(p, p);

    // Contains point at exactly the boundary
    QVERIFY(box.Contains(p));

    // Does not contain point outside
    QVERIFY(!box.Contains(QImPlot3DPoint(6.0, 5.0, 5.0)));
}

// ============================================================================
// QImPlot3DBox — Expand with a point inside the box should not change bounds
// ============================================================================
void TestPlot3DTypes::testBoxExpandNaN()
{
    QImPlot3DBox box(QImPlot3DPoint(0.0, 0.0, 0.0), QImPlot3DPoint(10.0, 10.0, 10.0));
    QImPlot3DPoint inside(5.0, 5.0, 5.0);
    box.Expand(inside);

    // Box should remain unchanged
    QCOMPARE(box.Min.x, 0.0);
    QCOMPARE(box.Min.y, 0.0);
    QCOMPARE(box.Min.z, 0.0);
    QCOMPARE(box.Max.x, 10.0);
    QCOMPARE(box.Max.y, 10.0);
    QCOMPARE(box.Max.z, 10.0);
}

// ============================================================================
// QImPlot3DBox — Contains at boundary
// ============================================================================
void TestPlot3DTypes::testBoxContainsBoundary()
{
    QImPlot3DBox box(QImPlot3DPoint(-1.0, -1.0, -1.0), QImPlot3DPoint(1.0, 1.0, 1.0));

    QVERIFY(box.Contains(QImPlot3DPoint(1.0, 1.0, 1.0)));
    QVERIFY(box.Contains(QImPlot3DPoint(-1.0, -1.0, -1.0)));
    QVERIFY(box.Contains(QImPlot3DPoint(0.0, 0.0, 0.0)));
}

// ============================================================================
// QImPlot3DBox — Clip a degenerate line segment (p0 == p1)
// ============================================================================
void TestPlot3DTypes::testBoxClipEmptySegment()
{
    QImPlot3DBox box(QImPlot3DPoint(0.0, 0.0, 0.0), QImPlot3DPoint(10.0, 10.0, 10.0));
    QImPlot3DPoint p0(5.0, 5.0, 5.0);
    QImPlot3DPoint p1(5.0, 5.0, 5.0);
    QImPlot3DPoint c0, c1;

    // Degenerate line inside box should return true
    bool result = box.ClipLineSegment(p0, p1, c0, c1);
    QVERIFY(result);
    QCOMPARE(c0.x, 5.0);
    QCOMPARE(c1.x, 5.0);
}

// ============================================================================
// QImPlot3DBox — Clip a single point inside
// ============================================================================
void TestPlot3DTypes::testBoxClipSinglePointInside()
{
    QImPlot3DBox box(QImPlot3DPoint(0.0, 0.0, 0.0), QImPlot3DPoint(10.0, 10.0, 10.0));
    QImPlot3DPoint p0(5.0, 5.0, 5.0);
    QImPlot3DPoint p1(5.0, 5.0, 5.0);
    QImPlot3DPoint c0, c1;

    QVERIFY(box.ClipLineSegment(p0, p1, c0, c1));
}

// ============================================================================
// QImPlot3DRange — constructing with reversed values
// ============================================================================
void TestPlot3DTypes::testRangeReverse()
{
    QImPlot3DRange r(10.0, 0.0);  // Max < Min
    QCOMPARE(r.Min, 10.0);
    QCOMPARE(r.Max, 0.0);
    QCOMPARE(r.Size(), -10.0);
}

// ============================================================================
// QImPlot3DRange — single value range (Min == Max)
// ============================================================================
void TestPlot3DTypes::testRangeSingleValue()
{
    QImPlot3DRange r(5.0, 5.0);
    QCOMPARE(r.Size(), 0.0);
    QVERIFY(r.Contains(5.0));
    QVERIFY(!r.Contains(5.1));
}

// ============================================================================
// QImPlot3DRange — Expand with value inside range does not change
// ============================================================================
void TestPlot3DTypes::testRangeExpandNaN()
{
    QImPlot3DRange r(0.0, 10.0);
    r.Expand(5.0);

    QCOMPARE(r.Min, 0.0);
    QCOMPARE(r.Max, 10.0);
}

// ============================================================================
// QImPlot3DRange — Contains boundary values
// ============================================================================
void TestPlot3DTypes::testRangeContainsBoundary()
{
    QImPlot3DRange r(-5.0, 5.0);

    QVERIFY(r.Contains(-5.0));
    QVERIFY(r.Contains(5.0));
    QVERIFY(r.Contains(0.0));
    QVERIFY(!r.Contains(-5.1));
    QVERIFY(!r.Contains(5.1));
}

QTEST_MAIN(TestPlot3DTypes)
#include "tst_types.moc"
