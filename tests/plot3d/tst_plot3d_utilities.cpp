#include <QTest>
#include <QObject>
#include <cmath>
#include "plot3d/QImPlot3DTypes.h"
#include "plot3d/QImPlot3D.h"

using namespace QIM;

class TestPlot3DUtilities : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase() { }
    void cleanupTestCase() { }
    void init() { }
    void cleanup() { }

    // ---- QImPlot3DPoint operations (coordinate transform foundation) ----

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
    }

    void testPointAddition()
    {
        QImPlot3DPoint a(1.0, 2.0, 3.0);
        QImPlot3DPoint b(4.0, 5.0, 6.0);
        QImPlot3DPoint c = a + b;
        QCOMPARE(c.x, 5.0);
        QCOMPARE(c.y, 7.0);
        QCOMPARE(c.z, 9.0);
    }

    void testPointSubtraction()
    {
        QImPlot3DPoint a(4.0, 5.0, 6.0);
        QImPlot3DPoint b(1.0, 2.0, 3.0);
        QImPlot3DPoint c = a - b;
        QCOMPARE(c.x, 3.0);
        QCOMPARE(c.y, 3.0);
        QCOMPARE(c.z, 3.0);
    }

    void testPointScalarMultiplication()
    {
        QImPlot3DPoint p(1.0, 2.0, 3.0);
        QImPlot3DPoint r = p * 2.0;
        QCOMPARE(r.x, 2.0);
        QCOMPARE(r.y, 4.0);
        QCOMPARE(r.z, 6.0);
    }

    void testPointDotProduct()
    {
        QImPlot3DPoint a(1.0, 0.0, 0.0);
        QImPlot3DPoint b(0.0, 1.0, 0.0);
        QCOMPARE(a.Dot(b), 0.0);

        QImPlot3DPoint c(1.0, 1.0, 0.0);
        QCOMPARE(a.Dot(c), 1.0);
    }

    void testPointCrossProduct()
    {
        QImPlot3DPoint a(1.0, 0.0, 0.0);
        QImPlot3DPoint b(0.0, 1.0, 0.0);
        QImPlot3DPoint c = a.Cross(b);
        QCOMPARE(c.x, 0.0);
        QCOMPARE(c.y, 0.0);
        QCOMPARE(c.z, 1.0);
    }

    void testPointLength()
    {
        QImPlot3DPoint p(3.0, 4.0, 0.0);
        QCOMPARE(p.Length(), 5.0);
    }

    void testPointLengthSquared()
    {
        QImPlot3DPoint p(3.0, 4.0, 0.0);
        QCOMPARE(p.LengthSquared(), 25.0);
    }

    void testPointNormalize()
    {
        QImPlot3DPoint p(3.0, 4.0, 0.0);
        p.Normalize();
        QCOMPARE(p.Length(), 1.0);
    }

    void testPointNormalized()
    {
        QImPlot3DPoint p(3.0, 4.0, 0.0);
        QImPlot3DPoint n = p.Normalized();
        QCOMPARE(n.Length(), 1.0);
        // Original unchanged
        QCOMPARE(p.x, 3.0);
    }

    void testPointIsNaN()
    {
        QImPlot3DPoint p(1.0, 2.0, 3.0);
        QVERIFY(!p.IsNaN());

        QImPlot3DPoint nanPoint(std::nan(""), 2.0, 3.0);
        QVERIFY(nanPoint.IsNaN());
    }

    // ---- QImPlot3DRay (coordinate transform ray type) ----

    void testRayConstruction()
    {
        QImPlot3DRay ray;
        ray.Origin = QImPlot3DPoint(0, 0, 0);
        ray.Direction = QImPlot3DPoint(1, 0, 0);
        QCOMPARE(ray.Origin.x, 0.0);
        QCOMPARE(ray.Direction.x, 1.0);
    }

    // ---- QImPlot3DPlane (coordinate transform plane type) ----

    void testPlaneConstruction()
    {
        QImPlot3DPlane plane;
        plane.Point = QImPlot3DPoint(0, 0, 0);
        plane.Normal = QImPlot3DPoint(0, 1, 0);
        QCOMPARE(plane.Normal.y, 1.0);
    }

    // ---- QImPlot3DBox (bounding box type) ----

    void testBoxDefaultConstructor()
    {
        QImPlot3DBox box;
        QCOMPARE(box.Min.x, 0.0);
        QCOMPARE(box.Max.x, 0.0);
    }

    void testBoxMinMaxConstructor()
    {
        QImPlot3DBox box(QImPlot3DPoint(-1, -1, -1), QImPlot3DPoint(1, 1, 1));
        QCOMPARE(box.Min.x, -1.0);
        QCOMPARE(box.Max.x, 1.0);
    }

    void testBoxContains()
    {
        QImPlot3DBox box(QImPlot3DPoint(-1, -1, -1), QImPlot3DPoint(1, 1, 1));
        QVERIFY(box.Contains(QImPlot3DPoint(0, 0, 0)));
        QVERIFY(!box.Contains(QImPlot3DPoint(2, 0, 0)));
    }

    void testBoxExpand()
    {
        QImPlot3DBox box(QImPlot3DPoint(0, 0, 0), QImPlot3DPoint(1, 1, 1));
        box.Expand(QImPlot3DPoint(2, 0, 0));
        QCOMPARE(box.Max.x, 2.0);
    }

    // ---- QImPlot3DRange (1D range type) ----

    void testRangeDefaultConstructor()
    {
        QImPlot3DRange range;
        QCOMPARE(range.Min, 0.0);
        QCOMPARE(range.Max, 0.0);
    }

    void testRangeMinMaxConstructor()
    {
        QImPlot3DRange range(0.0, 10.0);
        QCOMPARE(range.Min, 0.0);
        QCOMPARE(range.Max, 10.0);
    }

    void testRangeSize()
    {
        QImPlot3DRange range(2.0, 8.0);
        QCOMPARE(range.Size(), 6.0);
    }

    void testRangeContains()
    {
        QImPlot3DRange range(0.0, 10.0);
        QVERIFY(range.Contains(5.0));
        QVERIFY(!range.Contains(15.0));
    }

    void testRangeExpand()
    {
        QImPlot3DRange range(0.0, 10.0);
        range.Expand(15.0);
        QCOMPARE(range.Max, 15.0);
        range.Expand(-5.0);
        QCOMPARE(range.Min, -5.0);
    }

    // ---- Enum conversions for coordinate-related types ----

    void testPlane3DConversionRoundtrip()
    {
        for (int i = 0; i <= 2; ++i) {
            QImPlane3D qtVal = toQImPlane3D(i);
            int imVal = toImPlane3D(qtVal);
            QCOMPARE(imVal, i);
        }
    }

    // ---- Type registration (metatype system) ----

    void testPointMetatype()
    {
        int id = qMetaTypeId<QImPlot3DPoint>();
        QVERIFY(id >= 0);
    }

    void testRayMetatype()
    {
        int id = qMetaTypeId<QImPlot3DRay>();
        QVERIFY(id >= 0);
    }

    void testPlaneMetatype()
    {
        int id = qMetaTypeId<QImPlot3DPlane>();
        QVERIFY(id >= 0);
    }

    void testBoxMetatype()
    {
        int id = qMetaTypeId<QImPlot3DBox>();
        QVERIFY(id >= 0);
    }

    void testRangeMetatype()
    {
        int id = qMetaTypeId<QImPlot3DRange>();
        QVERIFY(id >= 0);
    }
};

QTEST_MAIN(TestPlot3DUtilities)
#include "tst_plot3d_utilities.moc"