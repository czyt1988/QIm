#include <QTest>
#include <QObject>
#include <cmath>
#include <limits>
#include "plot3d/QImPlot3DTypes.h"

using namespace QIM;

class TestPlot3DMathTypes : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase() {}
    void cleanupTestCase() {}
    void init() {}
    void cleanup() {}

    // ---- QImPlot3DPoint Tests ----

    void testPointDefaultConstructor()
    {
        QImPlot3DPoint p;
        QCOMPARE(p.x, 0.0);
        QCOMPARE(p.y, 0.0);
        QCOMPARE(p.z, 0.0);
    }

    void testPointXYZConstructor()
    {
        QImPlot3DPoint p(1.0, 2.0, 3.0);
        QCOMPARE(p.x, 1.0);
        QCOMPARE(p.y, 2.0);
        QCOMPARE(p.z, 3.0);
    }

    void testPointIndexAccessor()
    {
        QImPlot3DPoint p(10.0, 20.0, 30.0);
        QCOMPARE(p[0], 10.0);
        QCOMPARE(p[1], 20.0);
        QCOMPARE(p[2], 30.0);

        p[0] = 100.0;
        QCOMPARE(p.x, 100.0);
        QCOMPARE(p[0], 100.0);
    }

    void testPointScalarMultiply()
    {
        QImPlot3DPoint p(1.0, 2.0, 3.0);
        QImPlot3DPoint result = p * 2.0;
        QCOMPARE(result.x, 2.0);
        QCOMPARE(result.y, 4.0);
        QCOMPARE(result.z, 6.0);
    }

    void testPointScalarDivide()
    {
        QImPlot3DPoint p(2.0, 4.0, 6.0);
        QImPlot3DPoint result = p / 2.0;
        QCOMPARE(result.x, 1.0);
        QCOMPARE(result.y, 2.0);
        QCOMPARE(result.z, 3.0);
    }

    void testPointAdd()
    {
        QImPlot3DPoint a(1.0, 2.0, 3.0);
        QImPlot3DPoint b(4.0, 5.0, 6.0);
        QImPlot3DPoint result = a + b;
        QCOMPARE(result.x, 5.0);
        QCOMPARE(result.y, 7.0);
        QCOMPARE(result.z, 9.0);
    }

    void testPointSubtract()
    {
        QImPlot3DPoint a(5.0, 7.0, 9.0);
        QImPlot3DPoint b(1.0, 2.0, 3.0);
        QImPlot3DPoint result = a - b;
        QCOMPARE(result.x, 4.0);
        QCOMPARE(result.y, 5.0);
        QCOMPARE(result.z, 6.0);
    }

    void testPointComponentMultiply()
    {
        QImPlot3DPoint a(1.0, 2.0, 3.0);
        QImPlot3DPoint b(4.0, 5.0, 6.0);
        QImPlot3DPoint result = a * b;
        QCOMPARE(result.x, 4.0);
        QCOMPARE(result.y, 10.0);
        QCOMPARE(result.z, 18.0);
    }

    void testPointComponentDivide()
    {
        QImPlot3DPoint a(4.0, 10.0, 18.0);
        QImPlot3DPoint b(2.0, 5.0, 6.0);
        QImPlot3DPoint result = a / b;
        QCOMPARE(result.x, 2.0);
        QCOMPARE(result.y, 2.0);
        QCOMPARE(result.z, 3.0);
    }

    void testPointUnaryNegate()
    {
        QImPlot3DPoint p(1.0, -2.0, 3.0);
        QImPlot3DPoint result = -p;
        QCOMPARE(result.x, -1.0);
        QCOMPARE(result.y, 2.0);
        QCOMPARE(result.z, -3.0);
    }

    void testPointCompoundScalarMultiply()
    {
        QImPlot3DPoint p(1.0, 2.0, 3.0);
        p *= 3.0;
        QCOMPARE(p.x, 3.0);
        QCOMPARE(p.y, 6.0);
        QCOMPARE(p.z, 9.0);
    }

    void testPointCompoundScalarDivide()
    {
        QImPlot3DPoint p(6.0, 12.0, 18.0);
        p /= 3.0;
        QCOMPARE(p.x, 2.0);
        QCOMPARE(p.y, 4.0);
        QCOMPARE(p.z, 6.0);
    }

    void testPointCompoundAdd()
    {
        QImPlot3DPoint p(1.0, 2.0, 3.0);
        QImPlot3DPoint rhs(4.0, 5.0, 6.0);
        p += rhs;
        QCOMPARE(p.x, 5.0);
        QCOMPARE(p.y, 7.0);
        QCOMPARE(p.z, 9.0);
    }

    void testPointCompoundSubtract()
    {
        QImPlot3DPoint p(5.0, 7.0, 9.0);
        QImPlot3DPoint rhs(1.0, 2.0, 3.0);
        p -= rhs;
        QCOMPARE(p.x, 4.0);
        QCOMPARE(p.y, 5.0);
        QCOMPARE(p.z, 6.0);
    }

    void testPointCompoundPointMultiply()
    {
        QImPlot3DPoint p(1.0, 2.0, 3.0);
        QImPlot3DPoint rhs(4.0, 5.0, 6.0);
        p *= rhs;
        QCOMPARE(p.x, 4.0);
        QCOMPARE(p.y, 10.0);
        QCOMPARE(p.z, 18.0);
    }

    void testPointCompoundPointDivide()
    {
        QImPlot3DPoint p(4.0, 10.0, 18.0);
        QImPlot3DPoint rhs(2.0, 5.0, 6.0);
        p /= rhs;
        QCOMPARE(p.x, 2.0);
        QCOMPARE(p.y, 2.0);
        QCOMPARE(p.z, 3.0);
    }

    void testPointEquality()
    {
        QImPlot3DPoint a(1.0, 2.0, 3.0);
        QImPlot3DPoint b(1.0, 2.0, 3.0);
        QImPlot3DPoint c(1.0, 2.0, 4.0);
        QVERIFY(a == b);
        QVERIFY(!(a == c));
    }

    void testPointInequality()
    {
        QImPlot3DPoint a(1.0, 2.0, 3.0);
        QImPlot3DPoint b(1.0, 2.0, 3.0);
        QImPlot3DPoint c(1.0, 2.0, 4.0);
        QVERIFY(!(a != b));
        QVERIFY(a != c);
    }

    void testPointDotProduct()
    {
        QImPlot3DPoint a(1.0, 2.0, 3.0);
        QImPlot3DPoint b(4.0, 5.0, 6.0);
        QCOMPARE(a.Dot(b), 32.0); // 1*4 + 2*5 + 3*6 = 32
    }

    void testPointCrossProduct()
    {
        QImPlot3DPoint a(1.0, 0.0, 0.0);
        QImPlot3DPoint b(0.0, 1.0, 0.0);
        QImPlot3DPoint result = a.Cross(b);
        QCOMPARE(result.x, 0.0);
        QCOMPARE(result.y, 0.0);
        QCOMPARE(result.z, 1.0);

        // Test another case: (2,3,4) × (5,6,7)
        QImPlot3DPoint c(2.0, 3.0, 4.0);
        QImPlot3DPoint d(5.0, 6.0, 7.0);
        QImPlot3DPoint r2 = c.Cross(d);
        QCOMPARE(r2.x, -3.0); // 3*7 - 4*6 = -3
        QCOMPARE(r2.y, 6.0);  // 4*5 - 2*7 = 6
        QCOMPARE(r2.z, -3.0); // 2*6 - 3*5 = -3
    }

    void testPointLength()
    {
        QImPlot3DPoint p(1.0, 2.0, 3.0);
        double expected = std::sqrt(1.0 + 4.0 + 9.0);
        QCOMPARE(p.Length(), expected);
    }

    void testPointLengthSquared()
    {
        QImPlot3DPoint p(1.0, 2.0, 3.0);
        QCOMPARE(p.LengthSquared(), 14.0);
    }

    void testPointNormalize()
    {
        QImPlot3DPoint p(3.0, 4.0, 0.0);
        p.Normalize();
        double len = p.Length();
        QVERIFY(qFuzzyCompare(len, 1.0));
        QCOMPARE(p.x, 0.6); // 3/5
        QCOMPARE(p.y, 0.8); // 4/5
        QCOMPARE(p.z, 0.0);
    }

    void testPointNormalized()
    {
        QImPlot3DPoint p(3.0, 4.0, 0.0);
        QImPlot3DPoint result = p.Normalized();
        // Original should be unchanged
        QCOMPARE(p.x, 3.0);
        QCOMPARE(p.y, 4.0);
        QCOMPARE(p.z, 0.0);
        // Result should be unit length
        double len = result.Length();
        QVERIFY(qFuzzyCompare(len, 1.0));
        QCOMPARE(result.x, 0.6);
        QCOMPARE(result.y, 0.8);
        QCOMPARE(result.z, 0.0);
    }

    void testPointIsNaN()
    {
        QImPlot3DPoint normal(1.0, 2.0, 3.0);
        QVERIFY(!normal.IsNaN());

        QImPlot3DPoint nanX(std::numeric_limits<double>::quiet_NaN(), 2.0, 3.0);
        QVERIFY(nanX.IsNaN());

        QImPlot3DPoint nanY(1.0, std::numeric_limits<double>::quiet_NaN(), 3.0);
        QVERIFY(nanY.IsNaN());

        QImPlot3DPoint nanZ(1.0, 2.0, std::numeric_limits<double>::quiet_NaN());
        QVERIFY(nanZ.IsNaN());

        QImPlot3DPoint nanAll(std::numeric_limits<double>::quiet_NaN(),
                               std::numeric_limits<double>::quiet_NaN(),
                               std::numeric_limits<double>::quiet_NaN());
        QVERIFY(nanAll.IsNaN());
    }

    void testPointFriendScalarMultiply()
    {
        QImPlot3DPoint p(1.0, 2.0, 3.0);
        QImPlot3DPoint result = 2.0 * p;
        QCOMPARE(result.x, 2.0);
        QCOMPARE(result.y, 4.0);
        QCOMPARE(result.z, 6.0);

        // Verify commutative: 2.0 * p == p * 2.0
        QImPlot3DPoint rhsResult = p * 2.0;
        QVERIFY(result == rhsResult);
    }

    // ---- QImPlot3DRay Tests ----

    void testRayDefault()
    {
        QImPlot3DRay r;
        QCOMPARE(r.Origin.x, 0.0);
        QCOMPARE(r.Origin.y, 0.0);
        QCOMPARE(r.Origin.z, 0.0);
        QCOMPARE(r.Direction.x, 0.0);
        QCOMPARE(r.Direction.y, 0.0);
        QCOMPARE(r.Direction.z, 0.0);
    }

    void testRayCustom()
    {
        QImPlot3DRay r;
        r.Origin = QImPlot3DPoint(1.0, 2.0, 3.0);
        r.Direction = QImPlot3DPoint(0.0, 1.0, 0.0);
        QCOMPARE(r.Origin.x, 1.0);
        QCOMPARE(r.Direction.y, 1.0);
    }

    // ---- QImPlot3DPlane Tests ----

    void testPlaneDefault()
    {
        QImPlot3DPlane p;
        QCOMPARE(p.Point.x, 0.0);
        QCOMPARE(p.Normal.x, 0.0);
    }

    void testPlaneCustom()
    {
        QImPlot3DPlane p;
        p.Point = QImPlot3DPoint(1.0, 0.0, 0.0);
        p.Normal = QImPlot3DPoint(1.0, 0.0, 0.0);
        QCOMPARE(p.Point.x, 1.0);
        QCOMPARE(p.Normal.x, 1.0);
    }

    // ---- QImPlot3DBox Tests ----

    void testBoxDefaultConstructor()
    {
        QImPlot3DBox box;
        QCOMPARE(box.Min.x, 0.0);
        QCOMPARE(box.Min.y, 0.0);
        QCOMPARE(box.Min.z, 0.0);
        QCOMPARE(box.Max.x, 0.0);
        QCOMPARE(box.Max.y, 0.0);
        QCOMPARE(box.Max.z, 0.0);
    }

    void testBoxTwoPointConstructor()
    {
        QImPlot3DPoint minP(-1.0, -2.0, -3.0);
        QImPlot3DPoint maxP(1.0, 2.0, 3.0);
        QImPlot3DBox box(minP, maxP);
        QCOMPARE(box.Min.x, -1.0);
        QCOMPARE(box.Min.y, -2.0);
        QCOMPARE(box.Min.z, -3.0);
        QCOMPARE(box.Max.x, 1.0);
        QCOMPARE(box.Max.y, 2.0);
        QCOMPARE(box.Max.z, 3.0);
    }

    void testBoxExpand()
    {
        QImPlot3DBox box(QImPlot3DPoint(0.0, 0.0, 0.0), QImPlot3DPoint(1.0, 1.0, 1.0));
        box.Expand(QImPlot3DPoint(2.0, -1.0, 0.5));
        QCOMPARE(box.Min.x, 0.0);   // min(0, 2) = 0
        QCOMPARE(box.Min.y, -1.0);  // min(0, -1) = -1
        QCOMPARE(box.Min.z, 0.0);   // min(0, 0.5) = 0
        QCOMPARE(box.Max.x, 2.0);   // max(1, 2) = 2
        QCOMPARE(box.Max.y, 1.0);   // max(1, -1) = 1
        QCOMPARE(box.Max.z, 1.0);   // max(1, 0.5) = 1
    }

    void testBoxContainsInside()
    {
        QImPlot3DBox box(QImPlot3DPoint(0.0, 0.0, 0.0), QImPlot3DPoint(10.0, 10.0, 10.0));
        QVERIFY(box.Contains(QImPlot3DPoint(5.0, 5.0, 5.0)));
        // Boundary points should also be contained (inclusive)
        QVERIFY(box.Contains(QImPlot3DPoint(0.0, 0.0, 0.0)));
        QVERIFY(box.Contains(QImPlot3DPoint(10.0, 10.0, 10.0)));
    }

    void testBoxContainsOutside()
    {
        QImPlot3DBox box(QImPlot3DPoint(0.0, 0.0, 0.0), QImPlot3DPoint(10.0, 10.0, 10.0));
        QVERIFY(!box.Contains(QImPlot3DPoint(11.0, 5.0, 5.0)));
        QVERIFY(!box.Contains(QImPlot3DPoint(-1.0, 5.0, 5.0)));
        QVERIFY(!box.Contains(QImPlot3DPoint(5.0, 11.0, 5.0)));
    }

    void testBoxClipLineSegmentInside()
    {
        QImPlot3DBox box(QImPlot3DPoint(0.0, 0.0, 0.0), QImPlot3DPoint(10.0, 10.0, 10.0));
        QImPlot3DPoint p0(2.0, 3.0, 4.0);
        QImPlot3DPoint p1(8.0, 7.0, 6.0);
        QImPlot3DPoint p0c, p1c;
        QVERIFY(box.ClipLineSegment(p0, p1, p0c, p1c));
        QCOMPARE(p0c.x, 2.0);
        QCOMPARE(p0c.y, 3.0);
        QCOMPARE(p0c.z, 4.0);
        QCOMPARE(p1c.x, 8.0);
        QCOMPARE(p1c.y, 7.0);
        QCOMPARE(p1c.z, 6.0);
    }

    void testBoxClipLineSegmentPartial()
    {
        QImPlot3DBox box(QImPlot3DPoint(0.0, 0.0, 0.0), QImPlot3DPoint(10.0, 10.0, 10.0));
        // Line from outside to inside
        QImPlot3DPoint p0(-5.0, 5.0, 5.0);
        QImPlot3DPoint p1(15.0, 5.0, 5.0);
        QImPlot3DPoint p0c, p1c;
        QVERIFY(box.ClipLineSegment(p0, p1, p0c, p1c));
        QCOMPARE(p0c.x, 0.0);
        QCOMPARE(p0c.y, 5.0);
        QCOMPARE(p0c.z, 5.0);
        QCOMPARE(p1c.x, 10.0);
        QCOMPARE(p1c.y, 5.0);
        QCOMPARE(p1c.z, 5.0);
    }

    void testBoxClipLineSegmentOutside()
    {
        QImPlot3DBox box(QImPlot3DPoint(0.0, 0.0, 0.0), QImPlot3DPoint(10.0, 10.0, 10.0));
        // Line entirely outside
        QImPlot3DPoint p0(-5.0, 15.0, 5.0);
        QImPlot3DPoint p1(15.0, 15.0, 5.0);
        QImPlot3DPoint p0c, p1c;
        QVERIFY(!box.ClipLineSegment(p0, p1, p0c, p1c));
    }

    // ---- QImPlot3DRange Tests ----

    void testRangeDefaultConstructor()
    {
        QImPlot3DRange r;
        QCOMPARE(r.Min, 0.0);
        QCOMPARE(r.Max, 0.0);
    }

    void testRangeTwoValueConstructor()
    {
        QImPlot3DRange r(-5.0, 10.0);
        QCOMPARE(r.Min, -5.0);
        QCOMPARE(r.Max, 10.0);
    }

    void testRangeExpand()
    {
        QImPlot3DRange r(0.0, 10.0);
        r.Expand(-5.0);
        QCOMPARE(r.Min, -5.0);
        QCOMPARE(r.Max, 10.0);

        r.Expand(15.0);
        QCOMPARE(r.Min, -5.0);
        QCOMPARE(r.Max, 15.0);

        // Expand with value already inside range
        r.Expand(5.0);
        QCOMPARE(r.Min, -5.0);
        QCOMPARE(r.Max, 15.0);
    }

    void testRangeContains()
    {
        QImPlot3DRange r(0.0, 10.0);
        QVERIFY(r.Contains(5.0));
        QVERIFY(r.Contains(0.0));    // Boundary inclusive
        QVERIFY(r.Contains(10.0));   // Boundary inclusive
        QVERIFY(!r.Contains(-1.0));
        QVERIFY(!r.Contains(11.0));
    }

    void testRangeSize()
    {
        QImPlot3DRange r(2.0, 8.0);
        QCOMPARE(r.Size(), 6.0);

        QImPlot3DRange r2(-5.0, 5.0);
        QCOMPARE(r2.Size(), 10.0);
    }

    // ---- Edge Case Tests ----

    void testPointNormalizeZeroVector()
    {
        // Normalizing a zero-length vector produces NaN/inf — verify behavior
        QImPlot3DPoint zero(0.0, 0.0, 0.0);
        QImPlot3DPoint result = zero.Normalized();
        QVERIFY(result.IsNaN());
    }

    void testPointDoublePrecision()
    {
        // Verify double precision is preserved (not float)
        double largeVal = 1e15;
        QImPlot3DPoint p(largeVal, largeVal, largeVal);
        QCOMPARE(p.x, largeVal);
        QCOMPARE(p.LengthSquared(), 3.0 * largeVal * largeVal);
    }

    void testPointCompoundAssignmentReturnsSelf()
    {
        QImPlot3DPoint p(1.0, 2.0, 3.0);
        QImPlot3DPoint& ref = (p += QImPlot3DPoint(1.0, 1.0, 1.0));
        QVERIFY(&ref == &p);
        QCOMPARE(p.x, 2.0);
    }
};

QTEST_MAIN(TestPlot3DMathTypes)
#include "tst_plot3d_math_types.moc"