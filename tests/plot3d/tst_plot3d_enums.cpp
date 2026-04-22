#include <QTest>
#include <QObject>
#include "plot3d/QImPlot3D.h"
#include "implot3d.h"

using namespace QIM;

class TestPlot3DEnums : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase() { }
    void cleanupTestCase() { }
    void init() { }
    void cleanup() { }

    // ---- QImPlot3DColormap roundtrip ----

    void testColormapRoundtrip()
    {
        for (int i = 0; i <= 15; ++i) {
            QImPlot3DColormap qtVal = toQImPlot3DColormap(i);
            int imVal = toImPlot3DColormap(qtVal);
            QCOMPARE(imVal, i);
        }
    }

    // ---- QImPlot3DCol roundtrip ----

    void testColRoundtrip()
    {
        for (int i = 0; i <= 15; ++i) {
            QImPlot3DCol qtVal = toQImPlot3DCol(i);
            int imVal = toImPlot3DCol(qtVal);
            QCOMPARE(imVal, i);
        }
    }

    // ---- QImPlot3DStyleVar roundtrip ----

    void testStyleVarRoundtrip()
    {
        for (int i = 0; i <= 13; ++i) {
            QImPlot3DStyleVar qtVal = toQImPlot3DStyleVar(i);
            int imVal = toImPlot3DStyleVar(qtVal);
            QCOMPARE(imVal, i);
        }
    }

    // ---- QImPlot3DLocation roundtrip ----

    void testLocationRoundtrip()
    {
        // All 9 location values (including combined flags)
        QList<int> values = {0, 1, 2, 4, 8, 5, 9, 6, 10};
        for (int v : values) {
            QImPlot3DLocation qtVal = toQImPlot3DLocation(v);
            int imVal = toImPlot3DLocation(qtVal);
            QCOMPARE(imVal, v);
        }
    }

    // ---- QImPlot3DLegendFlags roundtrip ----

    void testLegendFlagsRoundtrip()
    {
        // Individual flag values
        QList<int> values = {0, 1, 2, 4};
        for (int v : values) {
            QImPlot3DLegendFlags qtVal = toQImPlot3DLegendFlags(v);
            int imVal = toImPlot3DLegendFlags(qtVal);
            QCOMPARE(imVal, v);
        }

        // Combined flags
        int combined = ImPlot3DLegendFlags_NoButtons | ImPlot3DLegendFlags_Horizontal;
        QImPlot3DLegendFlags qtCombined = toQImPlot3DLegendFlags(combined);
        int imCombined = toImPlot3DLegendFlags(qtCombined);
        QCOMPARE(imCombined, combined);
    }

    // ---- QImPlane3D roundtrip ----

    void testPlane3DRoundtrip()
    {
        for (int i = 0; i <= 2; ++i) {
            QImPlane3D qtVal = toQImPlane3D(i);
            int imVal = toImPlane3D(qtVal);
            QCOMPARE(imVal, i);
        }
    }

    // ---- Verify specific enum value names ----

    void testColormapValues()
    {
        QCOMPARE(int(QImPlot3DColormap::Deep), 0);
        QCOMPARE(int(QImPlot3DColormap::Dark), 1);
        QCOMPARE(int(QImPlot3DColormap::Pastel), 2);
        QCOMPARE(int(QImPlot3DColormap::Greys), 15);
    }

    void testColValues()
    {
        QCOMPARE(int(QImPlot3DCol::Line), 0);
        QCOMPARE(int(QImPlot3DCol::Fill), 1);
        QCOMPARE(int(QImPlot3DCol::COUNT), 15);
    }

    void testStyleVarValues()
    {
        QCOMPARE(int(QImPlot3DStyleVar::LineWeight), 0);
        QCOMPARE(int(QImPlot3DStyleVar::FillAlpha), 4);
        QCOMPARE(int(QImPlot3DStyleVar::COUNT), 13);
    }

    void testLocationValues()
    {
        QCOMPARE(int(QImPlot3DLocation::Center), 0);
        QCOMPARE(int(QImPlot3DLocation::North), 1);
        QCOMPARE(int(QImPlot3DLocation::South), 2);
        QCOMPARE(int(QImPlot3DLocation::West), 4);
        QCOMPARE(int(QImPlot3DLocation::East), 8);
        QCOMPARE(int(QImPlot3DLocation::NorthWest), 5);
        QCOMPARE(int(QImPlot3DLocation::NorthEast), 9);
        QCOMPARE(int(QImPlot3DLocation::SouthWest), 6);
        QCOMPARE(int(QImPlot3DLocation::SouthEast), 10);
    }

    void testLegendFlagsValues()
    {
        QCOMPARE(int(QImPlot3DLegendFlags::None), 0);
        QCOMPARE(int(QImPlot3DLegendFlags::NoButtons), 1);
        QCOMPARE(int(QImPlot3DLegendFlags::NoHighlightItem), 2);
        QCOMPARE(int(QImPlot3DLegendFlags::Horizontal), 4);
    }

    void testPlane3DValues()
    {
        QCOMPARE(int(QImPlane3D::YZ), 0);
        QCOMPARE(int(QImPlane3D::XZ), 1);
        QCOMPARE(int(QImPlane3D::XY), 2);
    }
};

QTEST_MAIN(TestPlot3DEnums)
#include "tst_plot3d_enums.moc"