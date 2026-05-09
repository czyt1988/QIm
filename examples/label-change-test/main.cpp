/**
 * @brief 验证 label 变更与 needsItemPoolReset 机制
 *
 * 1 个 LineItem + 1 个 ShadedItem，同步每秒改变 label。
 *
 * Usage:
 *   .\build.ps1 configure; .\build.ps1 build
 *   .\build\bin\Release\LabelChangeTest.exe
 */
#include "QImFigureWidget.h"
#include "plot/QImPlotNode.h"
#include "plot/QImPlotLineItemNode.h"
#include "plot/QImPlotShadedItemNode.h"

#include <QApplication>
#include <QMainWindow>
#include <QOpenGLContext>
#include <QSurfaceFormat>
#include <QTimer>
#include <QElapsedTimer>
#include <QtMath>
#include <vector>
#include <iostream>

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
    window.setWindowTitle("Label Change Test — Changes label every 1s");

    QIM::QImFigureWidget* figure = new QIM::QImFigureWidget(&window);
    figure->setSubplotGrid(1, 1);
    figure->setRenderMode(QIM::QImWidget::RenderContinuous); // 持续渲染
    window.setCentralWidget(figure);

    // 创建 1 个 Line + 1 个 Shaded，同步改 label
    QIM::QImPlotNode* plot = figure->createPlotNode();
    plot->setTitle("Line + Shaded — both change label every 100ms");
    plot->setLegendEnabled(true);

    std::vector<double> x, y;
    for (int i = 0; i < 400; ++i) {
        double t = i * 2.0 * M_PI / 399.0;
        x.push_back(t);
        y.push_back(std::sin(t) * 0.5 + 0.5);
    }

    auto* line = new QIM::QImPlotLineItemNode(plot);
    line->setData(x, y);
    line->setLabel("Line");          // 初始 label

    auto* shaded = new QIM::QImPlotShadedItemNode(plot);
    shaded->setData(x, y);
    shaded->setReferenceValue(0.0);
    shaded->setLabel("Shaded");      // 初始 label

    // ================================================================
    // 每 100ms 同步改变 2 个 item 的 label
    // ================================================================
    int changeCount = 0;
    const QStringList labels = {
        "alpha", "beta",  "gamma", "delta",
        "epsilon","zeta",  "eta",   "theta",
        "iota",  "kappa", "lambda","mu",
        "nu",    "xi",    "omicron","pi",
        "rho",   "sigma", "tau",   "upsilon",
        "phi",   "chi",   "psi",   "omega"
    };

    QElapsedTimer elapsed;
    elapsed.start();

    QTimer* timer = new QTimer(&window);
    QObject::connect(timer, &QTimer::timeout, [&]() {
        const QString& newLabel = labels[changeCount % labels.size()];
        ++changeCount;

        std::cout << "[" << elapsed.elapsed() / 1000.0 << "s] #"
                  << changeCount << " → \"" << newLabel.toStdString() << "\""
                  << std::endl;

        line->setLabel(newLabel);
        shaded->setLabel(newLabel);
    });

    timer->start(100);

    std::cout << "=== Label Change Test (Line + Shaded) ===" << std::endl;
    std::cout << "Changing BOTH labels every 100ms." << std::endl;
    std::cout << "If pool reset is DISABLED, expect crash or anomaly." << std::endl;
    std::cout << "If pool reset is ENABLED,  runs indefinitely." << std::endl;
    std::cout << "===========================================" << std::endl;

    window.resize(800, 600);
    window.show();
    return app.exec();
}
