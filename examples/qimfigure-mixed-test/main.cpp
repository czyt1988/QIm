// MSVC requires _USE_MATH_DEFINES for M_PI
#ifndef _USE_MATH_DEFINES
#    define _USE_MATH_DEFINES
#endif

#include "QImFigureWidget.h"
#include "QImGridNode.h"
#include "implot.h"
#include "plot/QImPlotNode.h"
#include "plot/QImPlotScatterItemNode.h"
#include "plot3d/QImPlot3DNode.h"
#include "plot3d/QImPlot3DLineItemNode.h"
#include "plot3d/QImPlot3DScatterItemNode.h"
#include <QApplication>
#include <QOpenGLContext>
#include <QSurfaceFormat>
#include <QMainWindow>
#include <cmath>
#include <vector>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QSurfaceFormat glFormat;
    if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL) {
        glFormat.setVersion(3, 3);
        glFormat.setProfile(QSurfaceFormat::CoreProfile);
    }
    QSurfaceFormat::setDefaultFormat(glFormat);

    QMainWindow window;
    window.setWindowTitle("QIm Figure Mixed 2D 3D Example");
    window.resize(1400, 900);

    QIM::QImFigureWidget* figure = new QIM::QImFigureWidget(&window);
    figure->setRenderMode(QIM::QImWidget::RenderOnDemand);
    window.setCentralWidget(figure);

    // Create 2x2 mixed grid
    auto* grid = new QIM::QImGridNode("Mixed 2D+3D", figure);
    figure->addRenderNode(grid);
    grid->setGrid(2, 2);

    // Cell 0: 2D Line (damped sine)
    if (auto* cell = grid->createCellNode()) {
        QIM::QImPlotNode* plot = new QIM::QImPlotNode("Damped Sine", cell);
        plot->setLegendEnabled(true);
        std::vector<double> xData, yData;
        xData.reserve(600);
        yData.reserve(600);
        for (int i = 0; i < 600; ++i) {
            const double x = static_cast<double>(i) * 0.02;
            xData.push_back(x);
            yData.push_back(std::sin(x) * std::exp(-0.03 * x));
        }
        plot->addLine(xData, yData, "damped sine");
    }

    // Cell 1: 3D Scatter
    if (auto* cell = grid->createCellNode()) {
        QIM::QImPlot3DNode* plot3d = new QIM::QImPlot3DNode("3D Scatter", cell);
        plot3d->setLegendEnabled(true);
        std::vector<double> xs, ys, zs;
        xs.reserve(120);
        ys.reserve(120);
        zs.reserve(120);
        for (int i = 0; i < 120; ++i) {
            const double t = static_cast<double>(i) * 0.11;
            xs.push_back(std::cos(t) * 1.4 + 0.2 * std::sin(t * 2.0));
            ys.push_back(std::sin(t) * 0.9 + 0.4 * std::cos(t * 1.7));
            zs.push_back(std::sin(t * 0.7) * 0.5);
        }
        auto* scatter = new QIM::QImPlot3DScatterItemNode(plot3d);
        scatter->setLabel("cluster");
        scatter->setData(xs, ys, zs);
        scatter->setMarkerSize(4.5f);
        scatter->setMarkerFillColor(QColor(0, 114, 189));
    }

    // Cell 2: 2D Scatter
    if (auto* cell = grid->createCellNode()) {
        QIM::QImPlotNode* plot = new QIM::QImPlotNode("2D Scatter", cell);
        plot->setLegendEnabled(true);
        std::vector<double> scatterX, scatterY;
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

    // Cell 3: 3D Line (helix)
    if (auto* cell = grid->createCellNode()) {
        QIM::QImPlot3DNode* plot3d = new QIM::QImPlot3DNode("3D Line", cell);
        plot3d->setLegendEnabled(true);
        std::vector<double> xs, ys, zs;
        xs.reserve(200);
        ys.reserve(200);
        zs.reserve(200);
        for (int i = 0; i < 200; ++i) {
            double t = i * 0.05;
            xs.push_back(std::cos(t));
            ys.push_back(std::sin(t));
            zs.push_back(t * 0.1);
        }
        auto* line = new QIM::QImPlot3DLineItemNode(plot3d);
        line->setLabel("helix");
        line->setData(xs, ys, zs);
        line->setColor(QColor(0, 114, 189));
        line->setLineWeight(2.0f);
    }

    window.show();
    return app.exec();
}
