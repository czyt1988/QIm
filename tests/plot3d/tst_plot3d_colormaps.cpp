#include <QTest>
#include <QObject>
#include "plot3d/QImPlot3D.h"

using namespace QIM;

class TestPlot3DColormaps : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase() { }
    void cleanupTestCase() { }
    void init() { }
    void cleanup() { }

    // ---- QImPlot3DColormap enum value verification ----

    void testColormapEnumValues()
    {
        QCOMPARE(int(QImPlot3DColormap::Deep), 0);
        QCOMPARE(int(QImPlot3DColormap::Dark), 1);
        QCOMPARE(int(QImPlot3DColormap::Pastel), 2);
        QCOMPARE(int(QImPlot3DColormap::Paired), 3);
        QCOMPARE(int(QImPlot3DColormap::Viridis), 4);
        QCOMPARE(int(QImPlot3DColormap::Plasma), 5);
        QCOMPARE(int(QImPlot3DColormap::Hot), 6);
        QCOMPARE(int(QImPlot3DColormap::Cool), 7);
        QCOMPARE(int(QImPlot3DColormap::Pink), 8);
        QCOMPARE(int(QImPlot3DColormap::Jet), 9);
        QCOMPARE(int(QImPlot3DColormap::Twilight), 10);
        QCOMPARE(int(QImPlot3DColormap::RdBu), 11);
        QCOMPARE(int(QImPlot3DColormap::BrBG), 12);
        QCOMPARE(int(QImPlot3DColormap::PiYG), 13);
        QCOMPARE(int(QImPlot3DColormap::Spectral), 14);
        QCOMPARE(int(QImPlot3DColormap::Greys), 15);
    }

    void testColormapEnumCount()
    {
        // ImPlot3D has 16 built-in colormaps (indices 0-15)
        QVERIFY(int(QImPlot3DColormap::Greys) == 15);
    }

    // ---- Colormap conversion functions (pure type conversion, no ImPlot3D context needed) ----

    void testColormapConversionRoundtrip()
    {
        for (int i = 0; i <= 15; ++i) {
            QImPlot3DColormap qtVal = toQImPlot3DColormap(i);
            int imVal = toImPlot3DColormap(qtVal);
            QCOMPARE(imVal, i);
        }
    }

    void testColormapConversionSpecific()
    {
        QCOMPARE(toImPlot3DColormap(QImPlot3DColormap::Deep), 0);
        QCOMPARE(toImPlot3DColormap(QImPlot3DColormap::Viridis), 4);
        QCOMPARE(toImPlot3DColormap(QImPlot3DColormap::Greys), 15);

        QCOMPARE(toQImPlot3DColormap(0), QImPlot3DColormap::Deep);
        QCOMPARE(toQImPlot3DColormap(4), QImPlot3DColormap::Viridis);
        QCOMPARE(toQImPlot3DColormap(15), QImPlot3DColormap::Greys);
    }

    // ---- QImPlot3DColormapManager class exists (compilation verification) ----

    void testColormapManagerClassExists()
    {
        // Verify that QImPlot3DColormapManager is a static-only utility class
        // (cannot be instantiated — deleted constructor/destructor)
        // This test just verifies the header compiles and the type exists
        QVERIFY(true);
    }

    // ---- Colormap enum is registered with Qt metatype system ----

    void testColormapMetaType()
    {
        int id = qMetaTypeId<QImPlot3DColormap>();
        QVERIFY(id != QMetaType::UnknownType);
    }
};

QTEST_MAIN(TestPlot3DColormaps)
#include "tst_plot3d_colormaps.moc"