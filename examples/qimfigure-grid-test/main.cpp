// MSVC requires _USE_MATH_DEFINES for M_PI
#ifndef _USE_MATH_DEFINES
#    define _USE_MATH_DEFINES
#endif

#include "QImFigureWidget.h"
#include "QImGridNode.h"
#include "plot/QImPlotNode.h"
#include "plot/QImPlotBarsItemNode.h"
#include "plot3d/QImPlot3DNode.h"
#include "plot3d/QImPlot3DLineItemNode.h"
#include "plot3d/QImPlot3DScatterItemNode.h"
#include "implot3d.h"

#include <QApplication>
#include <QOpenGLContext>
#include <QSurfaceFormat>
#include <QMainWindow>
#include <cmath>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSurfaceFormat glFormat;
    if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL) {
        glFormat.setVersion(3, 3);
        glFormat.setProfile(QSurfaceFormat::CoreProfile);
    }
    QSurfaceFormat::setDefaultFormat(glFormat);

    QMainWindow window;
    window.setWindowTitle("QImGridNode Demo - Mixed 2D/3D Plots");
    window.resize(1200, 900);

    // === Create the figure widget ===
    QIM::QImFigureWidget* figure = new QIM::QImFigureWidget(&window);
    figure->setRenderMode(QIM::QImWidget::RenderOnDemand);
    window.setCentralWidget(figure);

    // === Create the grid node with 2x2 layout ===
    auto* grid = new QIM::QImGridNode("Mixed Grid", figure);
    grid->setGrid(2, 2);

    // === Cell 0: 2D Line Plot (Sine Wave) ===
    if (auto* cell = grid->createCellNode()) {
        QIM::QImPlotNode* plot = new QIM::QImPlotNode("Sine Wave", cell);
        plot->setLegendEnabled(true);

        std::vector<double> x, y;
        x.reserve(400);
        y.reserve(400);
        for (int i = 0; i < 400; ++i) {
            double t = i * 2.0 * M_PI / 399.0;
            x.push_back(t);
            y.push_back(std::sin(t));
        }
        plot->addLine(x, y, "sin(x)");
    }

    // === Cell 1: 3D Scatter Plot ===
    if (auto* cell = grid->createCellNode()) {
        QIM::QImPlot3DNode* plot = new QIM::QImPlot3DNode("3D Scatter", cell);
        plot->setLegendEnabled(true);
        plot->setEqual(true);

        std::vector<double> xs, ys, zs;
        xs.reserve(200);
        ys.reserve(200);
        zs.reserve(200);
        for (int i = 0; i < 200; ++i) {
            double t = i * 0.05;
            xs.push_back(std::cos(t) * 0.8);
            ys.push_back(std::sin(t) * 0.8);
            zs.push_back(std::sin(t * 0.5));
        }

        auto* scatter = new QIM::QImPlot3DScatterItemNode(plot);
        scatter->setLabel("Samples");
        scatter->setData(xs, ys, zs);
        scatter->setMarkerSize(4.0f);
        scatter->setMarkerFillColor(QColor(217, 83, 25));
    }

    // === Cell 2: 2D Bar Chart ===
    if (auto* cell = grid->createCellNode()) {
        QIM::QImPlotNode* plot = new QIM::QImPlotNode("Bar Chart", cell);
        plot->setLegendEnabled(true);

        std::vector<double> x {1, 2, 3, 4, 5, 6};
        std::vector<double> y {3.6, 5.1, 4.4, 6.2, 3.8, 5.5};

        auto* bars = new QIM::QImPlotBarsItemNode(plot);
        bars->setLabel("2026");
        bars->setData(x, y);
        bars->setBarWidth(0.6);
        bars->setColor(QColor(80, 170, 90));
    }

    // === Cell 3: 3D Line Plot (Helix) ===
    if (auto* cell = grid->createCellNode()) {
        QIM::QImPlot3DNode* plot = new QIM::QImPlot3DNode("3D Line", cell);
        plot->setLegendEnabled(true);
        plot->setEqual(true);

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

        auto* line = new QIM::QImPlot3DLineItemNode(plot);
        line->setLabel("helix");
        line->setData(xs, ys, zs);
        line->setColor(QColor(0, 114, 189));
        line->setLineWeight(2.0f);
    }

    window.show();
    return a.exec();
}
