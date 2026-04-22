#include <QTest>
#include <QObject>
#include <QColor>
#include <QSizeF>
#include "plot3d/QImPlot3D.h"

using namespace QIM;

class TestPlot3DStyles : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase() { }
    void cleanupTestCase() { }
    void init() { }
    void cleanup() { }

    // ---- StyleVar enum values ----

    void testStyleVarEnumValues()
    {
        QCOMPARE(int(QImPlot3DStyleVar::LineWeight), 0);
        QCOMPARE(int(QImPlot3DStyleVar::Marker), 1);
        QCOMPARE(int(QImPlot3DStyleVar::MarkerSize), 2);
        QCOMPARE(int(QImPlot3DStyleVar::MarkerWeight), 3);
        QCOMPARE(int(QImPlot3DStyleVar::FillAlpha), 4);
        QCOMPARE(int(QImPlot3DStyleVar::PlotDefaultSize), 5);
        QCOMPARE(int(QImPlot3DStyleVar::PlotMinSize), 6);
        QCOMPARE(int(QImPlot3DStyleVar::PlotPadding), 7);
        QCOMPARE(int(QImPlot3DStyleVar::LabelPadding), 8);
        QCOMPARE(int(QImPlot3DStyleVar::ViewScaleFactor), 9);
        QCOMPARE(int(QImPlot3DStyleVar::LegendPadding), 10);
        QCOMPARE(int(QImPlot3DStyleVar::LegendInnerPadding), 11);
        QCOMPARE(int(QImPlot3DStyleVar::LegendSpacing), 12);
        QCOMPARE(int(QImPlot3DStyleVar::COUNT), 13);
    }

    // ---- StyleVar conversion roundtrip ----

    void testStyleVarConversionRoundtrip()
    {
        for (int i = 0; i <= 12; ++i) {
            QImPlot3DStyleVar qtVal = toQImPlot3DStyleVar(i);
            int imVal = toImPlot3DStyleVar(qtVal);
            QCOMPARE(imVal, i);
        }
    }

    // ---- Col enum values (style colors) ----

    void testColEnumValues()
    {
        QCOMPARE(int(QImPlot3DCol::Line), 0);
        QCOMPARE(int(QImPlot3DCol::Fill), 1);
        QCOMPARE(int(QImPlot3DCol::MarkerOutline), 2);
        QCOMPARE(int(QImPlot3DCol::MarkerFill), 3);
        QCOMPARE(int(QImPlot3DCol::TitleText), 4);
        QCOMPARE(int(QImPlot3DCol::InlayText), 5);
        QCOMPARE(int(QImPlot3DCol::FrameBg), 6);
        QCOMPARE(int(QImPlot3DCol::PlotBg), 7);
        QCOMPARE(int(QImPlot3DCol::PlotBorder), 8);
        QCOMPARE(int(QImPlot3DCol::LegendBg), 9);
        QCOMPARE(int(QImPlot3DCol::LegendBorder), 10);
        QCOMPARE(int(QImPlot3DCol::LegendText), 11);
        QCOMPARE(int(QImPlot3DCol::AxisText), 12);
        QCOMPARE(int(QImPlot3DCol::AxisGrid), 13);
        QCOMPARE(int(QImPlot3DCol::AxisTick), 14);
        QCOMPARE(int(QImPlot3DCol::COUNT), 15);
    }

    // ---- Col conversion roundtrip ----

    void testColConversionRoundtrip()
    {
        for (int i = 0; i <= 14; ++i) {
            QImPlot3DCol qtVal = toQImPlot3DCol(i);
            int imVal = toImPlot3DCol(qtVal);
            QCOMPARE(imVal, i);
        }
    }

    // ---- MarkerShape enum values ----

    void testMarkerShapeEnumValues()
    {
        QCOMPARE(int(QImPlot3DMarkerShape::None), -1);
        QCOMPARE(int(QImPlot3DMarkerShape::Circle), 0);
        QCOMPARE(int(QImPlot3DMarkerShape::Square), 1);
        QCOMPARE(int(QImPlot3DMarkerShape::Diamond), 2);
        QCOMPARE(int(QImPlot3DMarkerShape::Up), 3);
        QCOMPARE(int(QImPlot3DMarkerShape::Down), 4);
        QCOMPARE(int(QImPlot3DMarkerShape::Left), 5);
        QCOMPARE(int(QImPlot3DMarkerShape::Right), 6);
        QCOMPARE(int(QImPlot3DMarkerShape::Cross), 7);
        QCOMPARE(int(QImPlot3DMarkerShape::Plus), 8);
        QCOMPARE(int(QImPlot3DMarkerShape::Asterisk), 9);
    }

    // ---- MarkerShape conversion roundtrip ----

    void testMarkerShapeConversionRoundtrip()
    {
        for (int i = -1; i <= 9; ++i) {
            QImPlot3DMarkerShape qtVal = toQImPlot3DMarkerShape(i);
            int imVal = toImPlot3DMarker(qtVal);
            QCOMPARE(imVal, i);
        }
    }

    // ---- QColor operations (style node colors are QColors) ----

    void testQColorConstruction()
    {
        QColor red(255, 0, 0);
        QVERIFY(red.isValid());
        QCOMPARE(red.red(), 255);
        QCOMPARE(red.green(), 0);
        QCOMPARE(red.blue(), 0);
    }

    void testQColorWithAlpha()
    {
        QColor semiRed(255, 0, 0, 128);
        QVERIFY(semiRed.isValid());
        QCOMPARE(semiRed.alpha(), 128);
    }

    void testQSizeFOperations()
    {
        QSizeF size(400.0, 300.0);
        QCOMPARE(size.width(), 400.0);
        QCOMPARE(size.height(), 300.0);
    }

    void testQSizeFDefault()
    {
        QSizeF size;
        QCOMPARE(size.width(), -1.0);
        QCOMPARE(size.height(), -1.0);
    }

    // ---- Location combined flags ----

    void testLocationCombinedFlags()
    {
        QCOMPARE(int(QImPlot3DLocation::NorthWest), int(QImPlot3DLocation::North) | int(QImPlot3DLocation::West));
        QCOMPARE(int(QImPlot3DLocation::NorthEast), int(QImPlot3DLocation::North) | int(QImPlot3DLocation::East));
        QCOMPARE(int(QImPlot3DLocation::SouthWest), int(QImPlot3DLocation::South) | int(QImPlot3DLocation::West));
        QCOMPARE(int(QImPlot3DLocation::SouthEast), int(QImPlot3DLocation::South) | int(QImPlot3DLocation::East));
    }
};

QTEST_MAIN(TestPlot3DStyles)
#include "tst_plot3d_styles.moc"