#include "QImFigureWidget.h"
#include "plot3d/QImPlot3DExtNode.h"
#include "plot3d/QImPlot3DAxisInfo.h"
#include "plot3d/QImPlot3DExtLineItemNode.h"
#include "plot/QImPlotAxisInfo.h"
#include "plot/QImPlotNode.h"
#include "plot/QImPlotScatterItemNode.h"

#include "implot.h"
#include "implot3d.h"

#include <QApplication>
#include <QMainWindow>
#include <QOpenGLContext>
#include <QSurfaceFormat>

#include <cmath>
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
    window.setWindowTitle("QIm Figure Mixed 2D 3D Example");

    QIM::QImFigureWidget* figure = new QIM::QImFigureWidget(&window);
    figure->setRenderMode(QIM::QImWidget::RenderOnDemand);
    // 2D plots in subplot grid, 3D as top-level render nodes
    figure->setSubplotGrid(2, 2);
    window.setCentralWidget(figure);

    // 2D plot: Damped sine
    if (QIM::QImPlotNode* plot = figure->createPlotNode()) {
        plot->setTitle("Mixed 2D Line");
        plot->x1Axis()->setLabel("time");
        plot->y1Axis()->setLabel("amplitude");
        plot->setLegendEnabled(true);

        std::vector<double> xData;
        std::vector<double> yData;
        xData.reserve(600);
        yData.reserve(600);
        for (int i = 0; i < 600; ++i) {
            const double x = static_cast<double>(i) * 0.02;
            xData.push_back(x);
            yData.push_back(std::sin(x) * std::exp(-0.03 * x));
        }
        plot->addLine(xData, yData, "damped sine");
    }

    // 2D plot: Scatter
    if (QIM::QImPlotNode* plot = figure->createPlotNode()) {
        plot->setTitle("Mixed 2D Scatter");
        plot->x1Axis()->setLabel("feature x");
        plot->y1Axis()->setLabel("feature y");
        plot->setLegendEnabled(true);

        std::vector<double> scatterX;
        std::vector<double> scatterY;
        scatterX.reserve(120);
        scatterY.reserve(120);
        for (int i = 0; i < 120; ++i) {
            const double t = static_cast<double>(i) * 0.11;
            scatterX.push_back(std::cos(t) * 1.4 + 0.2 * std::sin(t * 2.0));
            scatterY.push_back(std::sin(t) * 0.9 + 0.4 * std::cos(t * 1.7));
        }

        QIM::QImPlotScatterItemNode* scatter = new QIM::QImPlotScatterItemNode(plot);
        scatter->setLabel("cluster");
        scatter->setData(scatterX, scatterY);
        scatter->setMarkerShape(ImPlotMarker_Circle);
        scatter->setMarkerSize(4.5f);
        scatter->setMarkerFill(true);
        scatter->setColor(QColor(0, 114, 189));
    }

    window.resize(1400, 900);
    window.show();
    return app.exec();
}
