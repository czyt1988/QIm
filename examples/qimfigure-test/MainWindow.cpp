#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include "plot/QImPlotNode.h"
#include "plot/QImPlotAxisInfo.h"
#include "plot/QImWaveformGenerator.hpp"
#include "plot/QImPlotValueTrackerNode.h"
#include "plot/QImPlotValueTrackerNodeGroup.h"
#include "plot/QImPlotScatterItemNode.h"
#include "plot/QImPlotStairsItemNode.h"
#include "plot/QImPlotBarsItemNode.h"
#include "plot/QImPlotShadedItemNode.h"
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
    fig->setSubplotGrid(6, 1);
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

    // 添加阶梯图示例
    if (QIM::QImPlotNode* plot4 = fig->createPlotNode()) {
        plot4->x1Axis()->setLabel(u8"x4");
        plot4->y1Axis()->setLabel(u8"y4");
        plot4->setTitle("Stairs Plot");
        plot4->setLegendEnabled(true);

        // 生成阶梯图数据
        int numPoints = 10;
        std::vector< double > xData(numPoints);
        std::vector< double > yData(numPoints);

        for (int i = 0; i < numPoints; ++i) {
            xData[ i ] = i;
            yData[ i ] = static_cast< double >(i % 3) + 1.0;
        }

        // 添加阶梯图
        QIM::QImPlotStairsItemNode* stairs = new QIM::QImPlotStairsItemNode(plot4);
        stairs->setLabel("Stairs Plot");
        stairs->setData(xData, yData);
        stairs->setColor(Qt::red);
        stairs->setShaded(true);

        // 添加值跟踪器
        QIM::QImPlotValueTrackerNode* tracker = new QIM::QImPlotValueTrackerNode(plot4);
        tracker->setGroup(trackerGroup);
        plot4->addChildNode(tracker);
    }

    // 添加柱状图示例
    if (QIM::QImPlotNode* plot5 = fig->createPlotNode()) {
        plot5->x1Axis()->setLabel(u8"x5");
        plot5->y1Axis()->setLabel(u8"y5");
        plot5->setTitle("Bars Plot");
        plot5->setLegendEnabled(true);

        // 生成柱状图数据
        int numBars = 12;
        std::vector< double > xData(numBars);
        std::vector< double > yData(numBars);

        for (int i = 0; i < numBars; ++i) {
            xData[ i ] = i;
            yData[ i ] = static_cast< double >(i * i) / 10.0 + 1.0;
        }

        // 添加柱状图
        QIM::QImPlotBarsItemNode* bars = new QIM::QImPlotBarsItemNode(plot5);
        bars->setLabel("Monthly Sales");
        bars->setData(xData, yData);
        bars->setBarWidth(0.6);
        bars->setColor(Qt::green);

        // 添加值跟踪器
        QIM::QImPlotValueTrackerNode* tracker = new QIM::QImPlotValueTrackerNode(plot5);
        tracker->setGroup(trackerGroup);
        plot5->addChildNode(tracker);
    }

    // 添加填充图示例
    if (QIM::QImPlotNode* plot6 = fig->createPlotNode()) {
        plot6->x1Axis()->setLabel(u8"x6");
        plot6->y1Axis()->setLabel(u8"y6");
        plot6->setTitle("Shaded Plot");
        plot6->setLegendEnabled(true);

        // 生成填充图数据 - 正弦波
        int numPoints = 100;
        std::vector< double > xData(numPoints);
        std::vector< double > yData(numPoints);

        for (int i = 0; i < numPoints; ++i) {
            xData[ i ] = i * 0.1;
            yData[ i ] = std::sin(xData[ i ]) * 5.0 + 5.0;  // 正弦波，范围0-10
        }

        // 添加填充图 - 单线填充模式（填充到参考值0）
        QIM::QImPlotShadedItemNode* shaded1 = new QIM::QImPlotShadedItemNode(plot6);
        shaded1->setLabel("Shaded to Zero");
        shaded1->setData(xData, yData);
        shaded1->setReferenceValue(0.0);
        shaded1->setColor(QColor(100, 150, 255, 180));

        // 添加第二条填充图 - 双线填充模式（上下边界）
        std::vector< double > yUpper(numPoints);
        std::vector< double > yLower(numPoints);
        for (int i = 0; i < numPoints; ++i) {
            yUpper[ i ] = yData[ i ] + 2.0;  // 上边界
            yLower[ i ] = yData[ i ] - 2.0;  // 下边界
        }

        // 添加第二条填充图 - 双线填充模式（上下边界）
        QIM::QImAbstractXYDataSeries* lowerSeries = new QIM::QImVectorXYDataSeries(xData, yLower);
        QIM::QImAbstractXYDataSeries* upperSeries = new QIM::QImVectorXYDataSeries(xData, yUpper);
        QIM::QImPlotShadedItemNode* shaded2 = new QIM::QImPlotShadedItemNode(plot6);
        shaded2->setLabel("Uncertainty Band");
        shaded2->setData(lowerSeries, upperSeries);
        shaded2->setColor(QColor(255, 100, 100, 120));

        // 添加值跟踪器
        QIM::QImPlotValueTrackerNode* tracker = new QIM::QImPlotValueTrackerNode(plot6);
        tracker->setGroup(trackerGroup);
        plot6->addChildNode(tracker);
    }
}

void MainWindow::drawPlot3D()
{
}
