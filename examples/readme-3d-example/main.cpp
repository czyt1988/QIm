#include "QImFigureWidget.h"
#include "plot/QImPlot3DLineItemNode.h"
#include "plot/QImPlot3DNode.h"
#include "plot/QImPlot3DScatterItemNode.h"
#include "plot/QImPlot3DSurfaceItemNode.h"

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
    window.setWindowTitle("3D Plot Example");

    QIM::QImFigureWidget* figure3D = new QIM::QImFigureWidget(&window);
    figure3D->setSubplot3DGrid(2, 2);
    figure3D->setRenderMode(QIM::QImWidget::RenderOnDemand);
    window.setCentralWidget(figure3D);

    if (QIM::QImPlot3DNode* plot = figure3D->createPlot3DNode()) {
        plot->setTitle("3D Line");
        std::vector< double > xs, ys, zs;
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
        line->setLineWidth(2.0f);
    }

    if (QIM::QImPlot3DNode* plot = figure3D->createPlot3DNode()) {
        plot->setTitle("3D Scatter");
        std::vector< double > xs, ys, zs;
        for (int i = 0; i < 200; ++i) {
            double t = i * 0.05;
            xs.push_back(std::cos(t) * 0.8);
            ys.push_back(std::sin(t) * 0.8);
            zs.push_back(std::sin(t * 0.5));
        }
        auto* scatter = new QIM::QImPlot3DScatterItemNode(plot);
        scatter->setLabel("samples");
        scatter->setData(xs, ys, zs);
        scatter->setMarkerSize(4.0f);
        scatter->setFillColor(QColor(217, 83, 25));
        scatter->setOutlineColor(QColor(120, 45, 10));
    }

    if (QIM::QImPlot3DNode* plot = figure3D->createPlot3DNode()) {
        plot->setTitle("3D Surface");
        constexpr int rows = 40;
        constexpr int cols = 40;
        std::vector< double > xs(rows * cols);
        std::vector< double > ys(rows * cols);
        std::vector< double > zs(rows * cols);
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                int index = r * cols + c;
                double x = -3.0 + 6.0 * c / (cols - 1);
                double y = -3.0 + 6.0 * r / (rows - 1);
                xs[index] = x;
                ys[index] = y;
                zs[index] = std::sin(x) * std::cos(y);
            }
        }
        auto* surface = new QIM::QImPlot3DSurfaceItemNode(plot);
        surface->setLabel("surface");
        surface->setData(xs, ys, zs, rows, cols);
        surface->setColormapEnabled(true);
        surface->setColormap(ImPlot3DColormap_Viridis);
    }

    if (QIM::QImPlot3DNode* plot = figure3D->createPlot3DNode()) {
        plot->setTitle("3D Wireframe");
        constexpr int rows = 40;
        constexpr int cols = 40;
        std::vector< double > xs(rows * cols);
        std::vector< double > ys(rows * cols);
        std::vector< double > zs(rows * cols);
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                int index = r * cols + c;
                double x = -3.0 + 6.0 * c / (cols - 1);
                double y = -3.0 + 6.0 * r / (rows - 1);
                xs[index] = x;
                ys[index] = y;
                zs[index] = std::sin(x) * std::cos(y);
            }
        }
        auto* wireframe = new QIM::QImPlot3DSurfaceItemNode(plot);
        wireframe->setLabel("wireframe");
        wireframe->setData(xs, ys, zs, rows, cols);
        wireframe->setColormapEnabled(true);
        wireframe->setColormap(ImPlot3DColormap_Viridis);
        wireframe->setFillVisible(false);
        wireframe->setMarkersVisible(false);
        wireframe->setLineWidth(1.2f);
    }

window.resize(1280, 900);
    window.show();
    return app.exec();
}
