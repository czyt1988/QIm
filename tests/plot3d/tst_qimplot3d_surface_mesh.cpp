#include <QObject>
#include <QtTest>
#include <QSignalSpy>
#include <memory>

#include "QImPlot3DSurfaceItemNode.h"
#include "QImPlot3DMeshItemNode.h"
#include "QImPlot3DTriangleItemNode.h"
#include "QImPlot3DQuadItemNode.h"
#include "../core/TestHelpers.h"

using namespace QIM;

class TestPlot3DSurfaceMesh : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    // Surface tests
    void testSurfaceConstruction();
    void testSurfaceFillVisible();
    void testSurfaceMarkersVisible();
    void testSurfaceLineWidth();
    void testSurfaceColormapEnabled();
    void testSurfaceColormap();
    void testSurfaceFlagSignalDedup();

    // Mesh tests
    void testMeshConstruction();
    void testMeshLinesVisible();
    void testMeshFillVisible();
    void testMeshMarkersVisible();
    void testMeshFlagSignalDedup();

    // Triangle tests
    void testTriangleConstruction();
    void testTriangleLinesVisible();
    void testTriangleFillVisible();
    void testTriangleFlagSignalDedup();

    // Quad tests
    void testQuadConstruction();
    void testQuadLinesVisible();
    void testQuadFillVisible();
    void testQuadFlagSignalDedup();
};

void TestPlot3DSurfaceMesh::initTestCase()
{
}

void TestPlot3DSurfaceMesh::cleanupTestCase()
{
}

// ============================================================================
// Surface Tests
// ============================================================================
void TestPlot3DSurfaceMesh::testSurfaceConstruction()
{
    auto node = std::make_unique<QImPlot3DSurfaceItemNode>();

    QCOMPARE(node->type(), QImPlot3DSurfaceItemNode::Type);
    QVERIFY(node->isFillVisible());
    QVERIFY(node->isLinesVisible());
    QVERIFY(node->isMarkersVisible());
    QVERIFY(!node->isColormapEnabled());
    QCOMPARE(node->lineWidth(), 1.0f);
}

void TestPlot3DSurfaceMesh::testSurfaceFillVisible()
{
    auto node = std::make_unique<QImPlot3DSurfaceItemNode>();

    QVERIFY(node->isFillVisible());

    node->setFillVisible(false);
    QVERIFY(!node->isFillVisible());

    node->setFillVisible(true);
    QVERIFY(node->isFillVisible());
}

void TestPlot3DSurfaceMesh::testSurfaceMarkersVisible()
{
    auto node = std::make_unique<QImPlot3DSurfaceItemNode>();

    QVERIFY(node->isMarkersVisible());

    node->setMarkersVisible(false);
    QVERIFY(!node->isMarkersVisible());

    node->setMarkersVisible(true);
    QVERIFY(node->isMarkersVisible());
}

void TestPlot3DSurfaceMesh::testSurfaceLineWidth()
{
    auto node = std::make_unique<QImPlot3DSurfaceItemNode>();

    QCOMPARE(node->lineWidth(), 1.0f);

    node->setLineWidth(2.5f);
    QCOMPARE(node->lineWidth(), 2.5f);

    node->setLineWidth(0.5f);
    QCOMPARE(node->lineWidth(), 0.5f);
}

void TestPlot3DSurfaceMesh::testSurfaceColormapEnabled()
{
    auto node = std::make_unique<QImPlot3DSurfaceItemNode>();

    QVERIFY(!node->isColormapEnabled());

    node->setColormapEnabled(true);
    QVERIFY(node->isColormapEnabled());

    node->setColormapEnabled(false);
    QVERIFY(!node->isColormapEnabled());
}

void TestPlot3DSurfaceMesh::testSurfaceColormap()
{
    auto node = std::make_unique<QImPlot3DSurfaceItemNode>();

    node->setColormap(2);
    QCOMPARE(node->colormap(), 2);

    node->setColormap(5);
    QCOMPARE(node->colormap(), 5);
}

void TestPlot3DSurfaceMesh::testSurfaceFlagSignalDedup()
{
    auto node = std::make_unique<QImPlot3DSurfaceItemNode>();
    QSignalSpy spy(node.get(), &QImPlot3DSurfaceItemNode::surfaceFlagChanged);

    node->setFillVisible(false);
    QCOMPARE(spy.count(), 1);

    node->setFillVisible(false);
    QCOMPARE(spy.count(), 1);

    node->setFillVisible(true);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// Mesh Tests
// ============================================================================
void TestPlot3DSurfaceMesh::testMeshConstruction()
{
    auto node = std::make_unique<QImPlot3DMeshItemNode>();

    QCOMPARE(node->type(), QImPlot3DMeshItemNode::Type);
    QVERIFY(node->isFillVisible());
    QVERIFY(node->isLinesVisible());
    QVERIFY(node->isMarkersVisible());
    QCOMPARE(node->lineWidth(), 1.0f);
}

void TestPlot3DSurfaceMesh::testMeshLinesVisible()
{
    auto node = std::make_unique<QImPlot3DMeshItemNode>();

    QVERIFY(node->isLinesVisible());

    node->setLinesVisible(false);
    QVERIFY(!node->isLinesVisible());

    node->setLinesVisible(true);
    QVERIFY(node->isLinesVisible());
}

void TestPlot3DSurfaceMesh::testMeshFillVisible()
{
    auto node = std::make_unique<QImPlot3DMeshItemNode>();

    QVERIFY(node->isFillVisible());

    node->setFillVisible(false);
    QVERIFY(!node->isFillVisible());

    node->setFillVisible(true);
    QVERIFY(node->isFillVisible());
}

void TestPlot3DSurfaceMesh::testMeshMarkersVisible()
{
    auto node = std::make_unique<QImPlot3DMeshItemNode>();

    QVERIFY(node->isMarkersVisible());

    node->setMarkersVisible(false);
    QVERIFY(!node->isMarkersVisible());

    node->setMarkersVisible(true);
    QVERIFY(node->isMarkersVisible());
}

void TestPlot3DSurfaceMesh::testMeshFlagSignalDedup()
{
    auto node = std::make_unique<QImPlot3DMeshItemNode>();
    QSignalSpy spy(node.get(), &QImPlot3DMeshItemNode::meshFlagChanged);

    node->setLinesVisible(false);
    QCOMPARE(spy.count(), 1);

    node->setLinesVisible(false);
    QCOMPARE(spy.count(), 1);

    node->setLinesVisible(true);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// Triangle Tests
// ============================================================================
void TestPlot3DSurfaceMesh::testTriangleConstruction()
{
    auto node = std::make_unique<QImPlot3DTriangleItemNode>();

    QCOMPARE(node->type(), QImPlot3DTriangleItemNode::Type);
    QVERIFY(node->isFillVisible());
    QVERIFY(node->isLinesVisible());
    QVERIFY(node->isMarkersVisible());
}

void TestPlot3DSurfaceMesh::testTriangleLinesVisible()
{
    auto node = std::make_unique<QImPlot3DTriangleItemNode>();

    QVERIFY(node->isLinesVisible());

    node->setLinesVisible(false);
    QVERIFY(!node->isLinesVisible());

    node->setLinesVisible(true);
    QVERIFY(node->isLinesVisible());
}

void TestPlot3DSurfaceMesh::testTriangleFillVisible()
{
    auto node = std::make_unique<QImPlot3DTriangleItemNode>();

    QVERIFY(node->isFillVisible());

    node->setFillVisible(false);
    QVERIFY(!node->isFillVisible());

    node->setFillVisible(true);
    QVERIFY(node->isFillVisible());
}

void TestPlot3DSurfaceMesh::testTriangleFlagSignalDedup()
{
    auto node = std::make_unique<QImPlot3DTriangleItemNode>();
    QSignalSpy spy(node.get(), &QImPlot3DTriangleItemNode::triangleFlagChanged);

    node->setFillVisible(false);
    QCOMPARE(spy.count(), 1);

    node->setFillVisible(false);
    QCOMPARE(spy.count(), 1);

    node->setFillVisible(true);
    QCOMPARE(spy.count(), 2);
}

// ============================================================================
// Quad Tests
// ============================================================================
void TestPlot3DSurfaceMesh::testQuadConstruction()
{
    auto node = std::make_unique<QImPlot3DQuadItemNode>();

    QCOMPARE(node->type(), QImPlot3DQuadItemNode::Type);
    QVERIFY(node->isFillVisible());
    QVERIFY(node->isLinesVisible());
    QVERIFY(node->isMarkersVisible());
}

void TestPlot3DSurfaceMesh::testQuadLinesVisible()
{
    auto node = std::make_unique<QImPlot3DQuadItemNode>();

    QVERIFY(node->isLinesVisible());

    node->setLinesVisible(false);
    QVERIFY(!node->isLinesVisible());

    node->setLinesVisible(true);
    QVERIFY(node->isLinesVisible());
}

void TestPlot3DSurfaceMesh::testQuadFillVisible()
{
    auto node = std::make_unique<QImPlot3DQuadItemNode>();

    QVERIFY(node->isFillVisible());

    node->setFillVisible(false);
    QVERIFY(!node->isFillVisible());

    node->setFillVisible(true);
    QVERIFY(node->isFillVisible());
}

void TestPlot3DSurfaceMesh::testQuadFlagSignalDedup()
{
    auto node = std::make_unique<QImPlot3DQuadItemNode>();
    QSignalSpy spy(node.get(), &QImPlot3DQuadItemNode::quadFlagChanged);

    node->setFillVisible(false);
    QCOMPARE(spy.count(), 1);

    node->setFillVisible(false);
    QCOMPARE(spy.count(), 1);

    node->setFillVisible(true);
    QCOMPARE(spy.count(), 2);
}

QTEST_GUILESS_MAIN(TestPlot3DSurfaceMesh)
#include "tst_qimplot3d_surface_mesh.moc"
