#include "QImFigureWidget.h"
#include "plot/QImPlotAxisInfo.h"
#include "plot/QImPlotBarsItemNode.h"
#include "plot/QImPlotNode.h"
#include "plot/QImPlotPieChartItemNode.h"
#include "plot/QImPlotScatterItemNode.h"

#include <QApplication>
#include <QMainWindow>
#include <QOpenGLContext>
#include <QSurfaceFormat>

#include <QtMath>
#include <vector>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QSurfaceFormat glFormat;
    if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL) {
        glFormat.setVersion(3, 3);
        glFormat.setProfile(QSurfaceFormat::CoreProfile);
    }
    QSurfaceFormat::setDefaultFormat(glFormat);

    QMainWindow window;
    window.setWindowTitle("QIm README 2D Example");

    QIM::QImFigureWidget* figure = new QIM::QImFigureWidget(&window);
    figure->setSubplotGrid(2, 2);
    figure->setRenderMode(QIM::QImWidget::RenderOnDemand);
    window.setCentralWidget(figure);

    if (QIM::QImPlotNode* plot = figure->createPlotNode()) {
        plot->setTitle("Sine Wave");
        plot->x1Axis()->setLabel("x");
        plot->y1Axis()->setLabel("sin(x)");
        plot->setLegendEnabled(true);

        std::vector< double > x;
        std::vector< double > y;
        x.reserve(400);
        y.reserve(400);
        for (int i = 0; i < 400; ++i) {
            double value = i * 2.0 * M_PI / 399.0;
            x.push_back(value);
            y.push_back(std::sin(value));
        }
        plot->addLine(x, y, "sin(x)");
    }

    if (QIM::QImPlotNode* plot = figure->createPlotNode()) {
        plot->setTitle("Scatter");
        plot->x1Axis()->setLabel("x");
        plot->y1Axis()->setLabel("y");
        plot->setLegendEnabled(true);
        std::vector< double > x {0.2, 0.5, 0.9, 1.3, 1.8, 2.1, 2.6, 3.0};
        std::vector< double > y {1.4, 1.0, 1.8, 1.3, 2.0, 1.7, 2.3, 2.1};
        auto* scatter = new QIM::QImPlotScatterItemNode(plot);
        scatter->setLabel("samples");
        scatter->setData(x, y);
        scatter->setMarkerSize(6.0f);
        scatter->setMarkerFill(true);
        scatter->setColor(QColor(0, 114, 189));
    }

    if (QIM::QImPlotNode* plot = figure->createPlotNode()) {
        plot->setTitle("Bars");
        plot->x1Axis()->setLabel("Quarter");
        plot->y1Axis()->setLabel("Revenue");
        plot->setLegendEnabled(true);
        std::vector< double > x {1, 2, 3, 4};
        std::vector< double > y {3.6, 5.1, 4.4, 6.2};
        auto* bars = new QIM::QImPlotBarsItemNode(plot);
        bars->setLabel("2026");
        bars->setData(x, y);
        bars->setBarWidth(0.6);
        bars->setColor(QColor(80, 170, 90));
    }

    if (QIM::QImPlotNode* plot = figure->createPlotNode()) {
        plot->setTitle("Pie Chart");
        plot->setEqual(true);
        plot->setMouseTextEnabled(false);
        plot->x1Axis()->setNoDecorations(true);
        plot->y1Axis()->setNoDecorations(true);
        plot->x1Axis()->setLimits(0.0, 1.0, QIM::QImPlotCondition::Always);
        plot->y1Axis()->setLimits(0.0, 1.0, QIM::QImPlotCondition::Always);

        auto* pie = new QIM::QImPlotPieChartItemNode(plot);
        pie->setData(QStringList() << "Desktop" << "Web" << "Embedded" << "Tools",
                     std::vector< double > {28.0, 34.0, 22.0, 16.0});
        pie->setCenter(QPointF(0.5, 0.5));
        pie->setRadius(0.40);
        pie->setLabelFormat("%.0f");
        pie->setExploding(true);
        pie->setIgnoreHidden(true);
    }

    window.resize(1280, 900);
    window.show();
    return app.exec();
}
