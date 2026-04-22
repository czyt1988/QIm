#include <QTest>
#include <QObject>
#include <cmath>
#include "plot3d/QImPlot3DAxisFormatter.h"
#include "plot3d/QImPlot3DAxisTransform.h"
#include "plot3d/QImPlot3D.h"

using namespace QIM;

class TestPlot3DSetupApi : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase() { }
    void cleanupTestCase() { }
    void init() { }
    void cleanup() { }

    // ---- AxisFormatter: Default formatter behavior ----

    void testDefaultFormatterFormat()
    {
        QImPlot3DDefaultAxisFormatter formatter;
        QByteArray result = formatter.format(3.14159, "");
        QVERIFY(!result.isEmpty());
        QCOMPARE(result, QByteArray::number(3.14159, 'g', 6));
    }

    void testDefaultFormatterWithPrefix()
    {
        QImPlot3DDefaultAxisFormatter formatter;
        QByteArray result = formatter.format(42.0, "val=");
        QCOMPARE(result, QByteArray("val=") + QByteArray::number(42.0, 'g', 6));
    }

    void testDefaultFormatterZero()
    {
        QImPlot3DDefaultAxisFormatter formatter;
        QByteArray result = formatter.format(0.0, "");
        QCOMPARE(result, QByteArray::number(0.0, 'g', 6));
    }

    void testDefaultFormatterNegative()
    {
        QImPlot3DDefaultAxisFormatter formatter;
        QByteArray result = formatter.format(-1.5, "");
        QVERIFY(result.startsWith("-"));
    }

    void testDefaultFormatterLargeValue()
    {
        QImPlot3DDefaultAxisFormatter formatter;
        QByteArray result = formatter.format(1e6, "");
        QVERIFY(!result.isEmpty());
    }

    void testDefaultFormatterSmallValue()
    {
        QImPlot3DDefaultAxisFormatter formatter;
        QByteArray result = formatter.format(0.001, "");
        QVERIFY(!result.isEmpty());
    }

    void testDefaultFormatterEmptyPrefix()
    {
        QImPlot3DDefaultAxisFormatter formatter;
        QByteArray noPrefix = formatter.format(5.0, "");
        QByteArray emptyPrefix = formatter.format(5.0, QByteArray());
        QCOMPARE(noPrefix, emptyPrefix);
    }

    // ---- AxisTransform: Interface type system ----

    void testAxisTransformInterface()
    {
        // Verify QImPlot3DAxisTransform is a pure virtual interface.
        // Create a test subclass to verify the type system works.
        struct TestTransform : QImPlot3DAxisTransform {
            double forward(double v) override { return v * 2.0; }
            double inverse(double v) override { return v / 2.0; }
        };
        TestTransform t;
        QCOMPARE(t.forward(5.0), 10.0);
        QCOMPARE(t.inverse(10.0), 5.0);
    }

    void testAxisTransformIdentity()
    {
        struct IdentityTransform : QImPlot3DAxisTransform {
            double forward(double v) override { return v; }
            double inverse(double v) override { return v; }
        };
        IdentityTransform t;
        QCOMPARE(t.forward(42.0), 42.0);
        QCOMPARE(t.inverse(42.0), 42.0);
    }

    void testAxisTransformLogScale()
    {
        struct LogTransform : QImPlot3DAxisTransform {
            double forward(double v) override { return std::log10(v); }
            double inverse(double v) override { return std::pow(10.0, v); }
        };
        LogTransform t;
        QCOMPARE(t.forward(100.0), 2.0);
        QCOMPARE(t.inverse(2.0), 100.0);
    }

    // ---- AxisInfo API: Enum values (no object creation needed) ----

    void testAxisIdEnumValues()
    {
        QCOMPARE(int(QImPlot3DAxisId::X1), 0);
        QCOMPARE(int(QImPlot3DAxisId::Y1), 1);
        QCOMPARE(int(QImPlot3DAxisId::Z1), 2);
    }

    void testScaleTypeEnumValues()
    {
        QCOMPARE(int(QImPlot3DScaleType::Linear), 0);
        QCOMPARE(int(QImPlot3DScaleType::Log10), 1);
        QCOMPARE(int(QImPlot3DScaleType::SymLog), 2);
    }

    void testConditionEnumValues()
    {
        QCOMPARE(int(QImPlot3DCondition::None), 0);
        QCOMPARE(int(QImPlot3DCondition::Always), 1);
        QCOMPARE(int(QImPlot3DCondition::Once), 2);
    }

    void testLocationEnumValues()
    {
        QCOMPARE(int(QImPlot3DLocation::Center), 0);
        QCOMPARE(int(QImPlot3DLocation::NorthWest), 5);
        QCOMPARE(int(QImPlot3DLocation::SouthEast), 10);
    }

    void testLegendFlagsEnumValues()
    {
        QCOMPARE(int(QImPlot3DLegendFlags::None), 0);
        QCOMPARE(int(QImPlot3DLegendFlags::NoButtons), 1);
        QCOMPARE(int(QImPlot3DLegendFlags::NoHighlightItem), 2);
        QCOMPARE(int(QImPlot3DLegendFlags::Horizontal), 4);
    }

    void testPlane3DEnumValues()
    {
        QCOMPARE(int(QImPlane3D::YZ), 0);
        QCOMPARE(int(QImPlane3D::XZ), 1);
        QCOMPARE(int(QImPlane3D::XY), 2);
    }

    // ---- Conversion functions roundtrip for axis-related enums ----

    void testAxisIdConversionRoundtrip()
    {
        for (int i = 0; i <= 2; ++i) {
            QImPlot3DAxisId qtVal = toQImPlot3DAxisId(i);
            int imVal = toImAxis3D(qtVal);
            QCOMPARE(imVal, i);
        }
    }

    void testScaleTypeConversionRoundtrip()
    {
        for (int i = 0; i <= 2; ++i) {
            QImPlot3DScaleType qtVal = toQImPlot3DScaleType(i);
            int imVal = toImPlot3DScale(qtVal);
            QCOMPARE(imVal, i);
        }
    }

    void testConditionConversionRoundtrip()
    {
        for (int i = 0; i <= 2; ++i) {
            QImPlot3DCondition qtVal = toQImPlot3DCondition(i);
            int imVal = toImPlot3DCond(qtVal);
            QCOMPARE(imVal, i);
        }
    }

    void testMarkerShapeConversionRoundtrip()
    {
        for (int i = -1; i <= 9; ++i) {
            QImPlot3DMarkerShape qtVal = toQImPlot3DMarkerShape(i);
            int imVal = toImPlot3DMarker(qtVal);
            QCOMPARE(imVal, i);
        }
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

QTEST_MAIN(TestPlot3DSetupApi)
#include "tst_plot3d_setup_api.moc"