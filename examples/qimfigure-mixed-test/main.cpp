#include "QImFigureWidget.h"
#include "plot/QImPlot3DLineItemNode.h"
#include "plot/QImPlot3DNode.h"
#include "plot/QImPlot3DSurfaceItemNode.h"
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
    figure->setSubplotGrid(2, 2);
    window.setCentralWidget(figure);

    if (QIM::QImPlotNode* plot = figure->createPlotNode()) {
        plot->setTitle("Mixed 2D Line");
        plot->x1Axis()->setLabel("time");
        plot->y1Axis()->setLabel("amplitude");
        plot->setLegendEnabled(true);

        std::vector< double > xData;
        std::vector< double > yData;
        xData.reserve(600);
        yData.reserve(600);
        for (int i = 0; i < 600; ++i) {
            const double x = static_cast< double >(i) * 0.02;
            xData.push_back(x);
            yData.push_back(std::sin(x) * std::exp(-0.03 * x));
        }
        plot->addLine(xData, yData, "damped sine");
    }

    if (QIM::QImPlot3DNode* plot = figure->createPlot3DNode()) {
        plot->setTitle("Mixed 3D Line");
        plot->setXAxisLabel("X");
        plot->setYAxisLabel("Y");
        plot->setZAxisLabel("Z");
        plot->setLegendEnabled(true);
        plot->setEqual(true);

        std::vector< double > xData;
        std::vector< double > yData;
        std::vector< double > zData;
        xData.reserve(240);
        yData.reserve(240);
        zData.reserve(240);
        for (int i = 0; i < 240; ++i) {
            const double t = static_cast< double >(i) * 0.08;
            xData.push_back(std::cos(t));
            yData.push_back(std::sin(t));
            zData.push_back(t * 0.3);
        }

        QIM::QImPlot3DLineItemNode* line = new QIM::QImPlot3DLineItemNode(plot);
        line->setLabel("helix");
        line->setData(xData, yData, zData);
        line->setColor(QColor(217, 83, 25));
        line->setLineWidth(2.0f);
    }

    if (QIM::QImPlotNode* plot = figure->createPlotNode()) {
        plot->setTitle("Mixed 2D Scatter");
        plot->x1Axis()->setLabel("feature x");
        plot->y1Axis()->setLabel("feature y");
        plot->setLegendEnabled(true);

        std::vector< double > scatterX;
        std::vector< double > scatterY;
        scatterX.reserve(120);
        scatterY.reserve(120);
        for (int i = 0; i < 120; ++i) {
            const double t = static_cast< double >(i) * 0.11;
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

    if (QIM::QImPlot3DNode* plot = figure->createPlot3DNode()) {
        plot->setTitle("Mixed 3D Surface");
        plot->setXAxisLabel("X");
        plot->setYAxisLabel("Y");
        plot->setZAxisLabel("Z");
        plot->setLegendEnabled(true);
        plot->setEqual(true);

        constexpr int xCount = 32;
        constexpr int yCount = 32;
        std::vector< double > xs;
        std::vector< double > ys;
        std::vector< double > zs;
        xs.reserve(xCount * yCount);
        ys.reserve(xCount * yCount);
        zs.reserve(xCount * yCount);

        for (int yi = 0; yi < yCount; ++yi) {
            const double y = -3.0 + 6.0 * static_cast< double >(yi) / static_cast< double >(yCount - 1);
            for (int xi = 0; xi < xCount; ++xi) {
                const double x = -3.0 + 6.0 * static_cast< double >(xi) / static_cast< double >(xCount - 1);
                const double r = std::sqrt(x * x + y * y);
                xs.push_back(x);
                ys.push_back(y);
                zs.push_back(std::sin(r * 2.0) / (1.0 + r * 0.6));
            }
        }

        QIM::QImPlot3DSurfaceItemNode* surface = new QIM::QImPlot3DSurfaceItemNode(plot);
        surface->setLabel("wave");
        surface->setData(xs, ys, zs, xCount, yCount);
        surface->setColormapEnabled(true);
        surface->setColormap(ImPlot3DColormap_Viridis);
        surface->setLineWidth(1.0f);
    }

    window.resize(1400, 900);
    window.show();
    return app.exec();
}
