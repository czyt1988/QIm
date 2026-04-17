#include "QImFigureWidget.h"
#include "plot3d/QImPlot3DExtNode.h"
#include "plot3d/QImPlot3DAxisInfo.h"
#include "plot3d/QImPlot3DExtScatterItemNode.h"
#include "plot3d/QImPlot3DExtLineItemNode.h"

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
    window.setWindowTitle("QIm README 3D Example");

    QIM::QImFigureWidget* figure3D = new QIM::QImFigureWidget(&window);
    // 3D plots are always top-level render nodes (ImPlot3D has no subplot API)
    // No setSubplotGrid() needed for 3D-only figures
    figure3D->setRenderMode(QIM::QImWidget::RenderOnDemand);
    window.setCentralWidget(figure3D);

    // Create a 3D line plot (helix curve)
    if (QIM::QImPlot3DExtNode* plot = figure3D->createPlotNode3D()) {
        plot->setTitle("3D Line - Helix");
        plot->xAxis()->setLabel("X");
        plot->yAxis()->setLabel("Y");
        plot->zAxis()->setLabel("Z");
        plot->setLegendEnabled(true);

        std::vector<double> xs, ys, zs;
        for (int i = 0; i < 200; ++i) {
            double t = i * 0.05;
            xs.push_back(std::cos(t));
            ys.push_back(std::sin(t));
            zs.push_back(t * 0.1);
        }
        auto* line = new QIM::QImPlot3DExtLineItemNode(plot);
        line->setLabel("helix");
        line->setData(xs, ys, zs);
        line->setColor(QColor(0, 114, 189));
        line->setLineWeight(2.0f);
    }

    window.resize(800, 600);
    window.show();
    return app.exec();
}
