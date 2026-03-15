#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include "plot/QImPlotNode.h"
#include "plot/QImPlotAxisInfo.h"
#include "plot/QImWaveformGenerator.hpp"
#include "plot/QImPlotValueTrackerNode.h"
#include "plot/QImPlotValueTrackerNodeGroup.h"
#include "plot/QImPlotScatterItemNode.h"
#include "implot.h"
#include <cmath>
#include <random>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    drawPlot2D();
    drawPlot3D();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::drawPlot2D()
{
    QIM::QImFigureWidget* fig = ui->figureWidget1;
    fig->setRenderMode(QIM::QImWidget::RenderOnDemand);
    fig->setSubplotGrid(3, 1);
    QIM::QImPlotValueTrackerNodeGroup* trackerGroup = new QIM::QImPlotValueTrackerNodeGroup(this);
    if (QIM::QImPlotNode* plot1 = fig->createPlotNode()) {
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
    if (QIM::QImPlotNode* plot2 = fig->createPlotNode()) {
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

    // 添加散点图示例
    if (QIM::QImPlotNode* plot3 = fig->createPlotNode()) {
        plot3->x1Axis()->setLabel(u8"x3");
        plot3->y1Axis()->setLabel(u8"y3");
        plot3->setTitle("Scatter Plot");
        plot3->setLegendEnabled(true);

        // 生成随机散点数据
        int numPoints = 1000;
        std::vector< double > xData(numPoints);
        std::vector< double > yData(numPoints);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution< double > xDist(0.0, 1.0);
        std::normal_distribution< double > yDist(0.0, 1.0);

        for (int i = 0; i < numPoints; ++i) {
            xData[ i ] = xDist(gen);
            yData[ i ] = yDist(gen);
        }

        // 添加散点图
        QIM::QImPlotScatterItemNode* scatter = new QIM::QImPlotScatterItemNode(plot3);
        scatter->setLabel("Random Points");
        scatter->setData(xData, yData);
        scatter->setMarkerSize(4.0f);
        scatter->setMarkerShape(ImPlotMarker_Circle);
        scatter->setMarkerFill(true);
        scatter->setColor(Qt::blue);


        // 添加值跟踪器
        QIM::QImPlotValueTrackerNode* tracker = new QIM::QImPlotValueTrackerNode(plot3);
        tracker->setGroup(trackerGroup);
        plot3->addChildNode(tracker);
    }
}

void MainWindow::drawPlot3D()
{
}
