#include <QTest>
#include <QObject>
#include <QtImGuiUtils.h>
#include "plot3d/QImPlot3DTypes.h"
#include <QQuaternion>
#include <cmath>

using namespace QIM;

class TestPlot3DConversions : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase() { }
    void cleanupTestCase() { }
    void init() { }
    void cleanup() { }

    void testQImPlot3DPointRoundtrip() {
        // Test normal values
        QImPlot3DPoint qpoint(1.5, 2.5, 3.5);
        ImPlot3DPoint implotPoint = toImPlot3DPoint(qpoint);
        QImPlot3DPoint qpointRoundtrip = toQImPlot3DPoint(implotPoint);
        
        QVERIFY(qpointRoundtrip == qpoint);
        QVERIFY(fuzzyEqual(qpointRoundtrip, qpoint));
        
        // Test NaN handling
        QImPlot3DPoint qpointNaN(std::numeric_limits<double>::quiet_NaN(), 2.0, 3.0);
        ImPlot3DPoint implotPointNaN = toImPlot3DPoint(qpointNaN);
        QImPlot3DPoint qpointNaNRoundtrip = toQImPlot3DPoint(implotPointNaN);
        
        QVERIFY(std::isnan(qpointNaNRoundtrip.x));
        QVERIFY(qpointNaNRoundtrip.y == 2.0);
        QVERIFY(qpointNaNRoundtrip.z == 3.0);
    }

    void testQQuaternionRoundtrip() {
        // Test normal quaternion
        QQuaternion q(0.5f, 0.5f, 0.5f, 0.5f); // (x, y, z, scalar)
        ImPlot3DQuat implotQuat = toImPlot3DQuat(q);
        QQuaternion qRoundtrip = toQQuaternion(implotQuat);
        
        // Due to float->double->float precision loss, use fuzzyEqual
        QVERIFY(fuzzyEqual(q.x(), qRoundtrip.x(), 1e-6f));
        QVERIFY(fuzzyEqual(q.y(), qRoundtrip.y(), 1e-6f));
        QVERIFY(fuzzyEqual(q.z(), qRoundtrip.z(), 1e-6f));
        QVERIFY(fuzzyEqual(q.scalar(), qRoundtrip.scalar(), 1e-6f));
        
        // Test identity quaternion
        QQuaternion qIdentity(1.0f, 0.0f, 0.0f, 0.0f);
        ImPlot3DQuat implotQuatIdentity = toImPlot3DQuat(qIdentity);
        QQuaternion qIdentityRoundtrip = toQQuaternion(implotQuatIdentity);
        
        QVERIFY(fuzzyEqual(qIdentity.x(), qIdentityRoundtrip.x(), 1e-6f));
        QVERIFY(fuzzyEqual(qIdentity.y(), qIdentityRoundtrip.y(), 1e-6f));
        QVERIFY(fuzzyEqual(qIdentity.z(), qIdentityRoundtrip.z(), 1e-6f));
        QVERIFY(fuzzyEqual(qIdentity.scalar(), qIdentityRoundtrip.scalar(), 1e-6f));
        
// Note: NaN in QQuaternion (float) converts to ImPlot3DQuat (double) and back,
        // but float->double->float NaN propagation is not guaranteed across all platforms.
        // So we test NaN conversion for ImPlot3DPoint (double-only) instead,
        // and verify normal quaternion roundtrip precision here.
    }

    void testQImPlot3DRayRoundtrip() {
        QImPlot3DRay qray;
        qray.Origin = QImPlot3DPoint(1.0, 2.0, 3.0);
        qray.Direction = QImPlot3DPoint(0.0, 1.0, 0.0);
        
        ImPlot3DRay implotRay = toImPlot3DRay(qray);
        QImPlot3DRay qrayRoundtrip = toQImPlot3DRay(implotRay);
        
        QVERIFY(qrayRoundtrip.Origin == qray.Origin);
        QVERIFY(qrayRoundtrip.Direction == qray.Direction);
        QVERIFY(fuzzyEqual(qrayRoundtrip.Origin, qray.Origin));
        QVERIFY(fuzzyEqual(qrayRoundtrip.Direction, qray.Direction));
    }

    void testQImPlot3DPlaneRoundtrip() {
        QImPlot3DPlane qplane;
        qplane.Point = QImPlot3DPoint(0.0, 0.0, 0.0);
        qplane.Normal = QImPlot3DPoint(0.0, 0.0, 1.0);
        
        ImPlot3DPlane implotPlane = toImPlot3DPlane(qplane);
        QImPlot3DPlane qplaneRoundtrip = toQImPlot3DPlane(implotPlane);
        
        QVERIFY(qplaneRoundtrip.Point == qplane.Point);
        QVERIFY(qplaneRoundtrip.Normal == qplane.Normal);
        QVERIFY(fuzzyEqual(qplaneRoundtrip.Point, qplane.Point));
        QVERIFY(fuzzyEqual(qplaneRoundtrip.Normal, qplane.Normal));
    }

    void testQImPlot3DBoxRoundtrip() {
        QImPlot3DBox qbox;
        qbox.Min = QImPlot3DPoint(-1.0, -1.0, -1.0);
        qbox.Max = QImPlot3DPoint(1.0, 1.0, 1.0);
        
        ImPlot3DBox implotBox = toImPlot3DBox(qbox);
        QImPlot3DBox qboxRoundtrip = toQImPlot3DBox(implotBox);
        
        QVERIFY(qboxRoundtrip.Min == qbox.Min);
        QVERIFY(qboxRoundtrip.Max == qbox.Max);
        QVERIFY(fuzzyEqual(qboxRoundtrip, qbox));
        
        // Test fuzzyEqual for boxes
        QImPlot3DBox qbox2;
        qbox2.Min = QImPlot3DPoint(-1.000000001, -1.000000001, -1.000000001);
        qbox2.Max = QImPlot3DPoint(1.000000001, 1.000000001, 1.000000001);
        
        QVERIFY(fuzzyEqual(qbox, qbox2, 1e-8));
        QVERIFY(!fuzzyEqual(qbox, qbox2, 1e-10));
    }

    void testQImPlot3DRangeRoundtrip() {
        QImPlot3DRange qrange(-10.0, 10.0);
        
        ImPlot3DRange implotRange = toImPlot3DRange(qrange);
        QImPlot3DRange qrangeRoundtrip = toQImPlot3DRange(implotRange);
        
        QVERIFY(qrangeRoundtrip.Min == qrange.Min);
        QVERIFY(qrangeRoundtrip.Max == qrange.Max);
        QVERIFY(fuzzyEqual(qrangeRoundtrip, qrange));
        
        // Test fuzzyEqual for ranges
        QImPlot3DRange qrange2(-10.000000001, 10.000000001);
        
        QVERIFY(fuzzyEqual(qrange, qrange2, 1e-8));
        QVERIFY(!fuzzyEqual(qrange, qrange2, 1e-10));
    }

    void testFuzzyEqualQImPlot3DPoint() {
        QImPlot3DPoint p1(1.0, 2.0, 3.0);
        QImPlot3DPoint p2(1.000000001, 2.000000001, 3.000000001);
        QImPlot3DPoint p3(1.1, 2.0, 3.0);
        
        QVERIFY(fuzzyEqual(p1, p2, 1e-8));
        QVERIFY(!fuzzyEqual(p1, p2, 1e-10));
        QVERIFY(!fuzzyEqual(p1, p3, 1e-1));
    }

    void testQDebugOperators() {
        // Test that QDebug operators compile and produce output
        QImPlot3DPoint point(1.5, 2.5, 3.5);
        
        QImPlot3DRay ray;
        ray.Origin = QImPlot3DPoint(1.0, 2.0, 3.0);
        ray.Direction = QImPlot3DPoint(0.0, 1.0, 0.0);
        
        QImPlot3DPlane plane;
        plane.Point = QImPlot3DPoint(0.0, 0.0, 0.0);
        plane.Normal = QImPlot3DPoint(0.0, 0.0, 1.0);
        
        QImPlot3DBox box;
        box.Min = QImPlot3DPoint(-1.0, -1.0, -1.0);
        box.Max = QImPlot3DPoint(1.0, 1.0, 1.0);
        
        QImPlot3DRange range(-10.0, 10.0);
        
        // Just verify they compile - actual output testing is not needed
        qDebug() << point;
        qDebug() << ray;
        qDebug() << plane;
        qDebug() << box;
        qDebug() << range;
        
        QVERIFY(true); // Just to have an assertion
    }
};

QTEST_MAIN(TestPlot3DConversions)
#include "tst_plot3d_conversions.moc"