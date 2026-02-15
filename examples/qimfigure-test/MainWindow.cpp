#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include "plot/QImPlotNode.h"
#include "plot/QImPlotAxisInfo.h"
#include "plot/QImWaveformGenerator.hpp"
#include "plot/QImPlotValueTrackerNode.h"
#include "plot/QImPlotValueTrackerNodeGroup.h"
#include <cmath>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->figureWidget1->setRenderMode(QIM::QImWidget::RenderOnDemand);
    ui->figureWidget1->setSubplotGrid(2, 1);
    QIM::QImPlotValueTrackerNodeGroup* trackerGroup = new QIM::QImPlotValueTrackerNodeGroup(this);
    if (QIM::QImPlotNode* plot1 = ui->figureWidget1->createPlotNode()) {
        plot1->x1Axis()->setLabel(u8"x1");
        plot1->y1Axis()->setLabel(u8"y1");
        plot1->setTitle("10K Points");
        int numPoints = 100000;
        auto wave     = QIM::make_waveform< QIM::CosineWave >(15.0, 0.001);
        auto datas    = wave.generate(numPoints, 0.0, 20 * M_PI);
        plot1->addLine(datas.first, datas.second, "curve a");
        QIM::QImPlotValueTrackerNode* tracker = new QIM::QImPlotValueTrackerNode(plot1);
        tracker->setGroup(trackerGroup);
        plot1->addChildNode(tracker);
    }
    if (QIM::QImPlotNode* plot2 = ui->figureWidget1->createPlotNode()) {
        plot2->x1Axis()->setLabel(u8"x2");
        plot2->y1Axis()->setLabel(u8"y2");
        plot2->setTitle("1M Points");
        plot2->setLegendEnabled(true);
        int numPoints = 1000000;
        auto wave     = QIM::make_waveform< QIM::DampedSineWave >(3.0, 0.15);
        auto datas    = wave.generate(numPoints, 0.0, 4.0 * M_PI);
        plot2->addLine(datas.first, datas.second, "curve 1");

        auto cos_wave = QIM::make_waveform< QIM::CosineWave >(2.0, M_PI / 4.0);  // 振幅=2.0，相位=π/4
        datas         = cos_wave.generate(numPoints, -M_PI, M_PI);
        plot2->addLine(datas.first, datas.second, "curve 2");
        QIM::QImPlotValueTrackerNode* tracker = new QIM::QImPlotValueTrackerNode(plot2);
        tracker->setGroup(trackerGroup);
        plot2->addChildNode(tracker);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::drawPlot()
{
}
