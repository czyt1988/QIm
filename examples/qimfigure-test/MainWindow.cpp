#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include "QImFigure3DWidget.h"
#include "plot/QImPlotNode.h"
#include "plot/QImPlotHistogramItemNode.h"
#include "plot/QImPlot3DLineItemNode.h"
#include "plot/QImPlot3DNode.h"
#include "plot/QImPlot3DScatterItemNode.h"
#include "plot/QImPlot3DSurfaceItemNode.h"
#include "plot/QImPlot3DTriangleItemNode.h"
#include "plot/QImPlotAxisInfo.h"
#include "plot/QImWaveformGenerator.hpp"
#include "plot/QImPlotValueTrackerNode.h"
#include "plot/QImPlotValueTrackerNodeGroup.h"
#include "plot/QImPlotScatterItemNode.h"
#include "plot/QImPlotStairsItemNode.h"
#include "plot/QImPlotBarsItemNode.h"
#include "plot/QImPlotShadedItemNode.h"
#include "plot/QImPlotErrorBarsItemNode.h"
#include "plot/QImPlotStemsItemNode.h"
#include "plot/QImPlotInfLinesItemNode.h"
#include "plot/QImPlotPieChartItemNode.h"
#include "plot/QImPlotTextItemNode.h"
#include "plot/QImPlotDummyItemNode.h"
#include "implot.h"
#include <QVBoxLayout>
#include <cmath>
#include <random>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 1);
    ui->splitter->setSizes(QList< int >() << 1 << 1);
    drawPlot2D();
    drawPlot3D();
    showMaximized();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::drawPlot2D()
{
    QIM::QImFigureWidget* fig = ui->figureWidget1;
    fig->setRenderMode(QIM::QImWidget::RenderOnDemand);
    // 显示6x2=12个子图
    fig->setSubplotGrid(6, 2);
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
        QIM::QImPlotShadedItemNode* shaded2       = new QIM::QImPlotShadedItemNode(plot6);
        shaded2->setLabel("Uncertainty Band");
        shaded2->setData(lowerSeries, upperSeries);
        shaded2->setColor(QColor(255, 100, 100, 120));

        // 添加值跟踪器
        QIM::QImPlotValueTrackerNode* tracker = new QIM::QImPlotValueTrackerNode(plot6);
        tracker->setGroup(trackerGroup);
        plot6->addChildNode(tracker);
    }

    // 添加误差棒图示例
    if (QIM::QImPlotNode* plot7 = fig->createPlotNode()) {
        plot7->x1Axis()->setLabel(u8"x7");
        plot7->y1Axis()->setLabel(u8"y7");
        plot7->setTitle("Error Bars Plot");
        plot7->setLegendEnabled(true);

        // 生成数据 - 带误差的测量数据
        int numPoints = 10;
        std::vector<double> xData(numPoints);
        std::vector<double> yData(numPoints);
        std::vector<double> errors(numPoints);
        std::vector<double> negErrors(numPoints);
        std::vector<double> posErrors(numPoints);

        for (int i = 0; i < numPoints; ++i) {
            xData[i] = i;
            yData[i] = static_cast<double>(i * i) / 5.0 + 2.0;
            errors[i] = 0.5 + static_cast<double>(i) * 0.1;           // 对称误差
            negErrors[i] = 0.3 + static_cast<double>(i) * 0.05;       // 负误差（较小）
            posErrors[i] = 0.7 + static_cast<double>(i) * 0.15;       // 正误差（较大）
        }

        // 添加散点图作为基础数据
        QIM::QImPlotScatterItemNode* scatter = new QIM::QImPlotScatterItemNode(plot7);
        scatter->setLabel("Data Points");
        scatter->setData(xData, yData);
        scatter->setMarkerSize(6.0f);
        scatter->setMarkerShape(ImPlotMarker_Circle);
        scatter->setColor(Qt::blue);

        // 添加对称误差棒
        QIM::QImPlotErrorBarsItemNode* errorBars1 = new QIM::QImPlotErrorBarsItemNode(plot7);
        errorBars1->setLabel("Symmetric Errors");
        errorBars1->setData(xData, yData, errors);
        errorBars1->setColor(Qt::red);

        // 添加非对称水平误差棒（偏移X位置避免重叠）
        std::vector<double> xOffset(numPoints);
        for (int i = 0; i < numPoints; ++i) {
            xOffset[i] = xData[i] + 0.3;
        }
        QIM::QImPlotScatterItemNode* scatter2 = new QIM::QImPlotScatterItemNode(plot7);
        scatter2->setLabel("Data Points 2");
        scatter2->setData(xOffset, yData);
        scatter2->setMarkerSize(6.0f);
        scatter2->setMarkerShape(ImPlotMarker_Square);
        scatter2->setColor(Qt::green);

        QIM::QImPlotErrorBarsItemNode* errorBars2 = new QIM::QImPlotErrorBarsItemNode(plot7);
        errorBars2->setLabel("Asymmetric Horizontal");
        errorBars2->setData(xOffset, yData, negErrors, posErrors);
        errorBars2->setHorizontal(true);
        errorBars2->setColor(Qt::darkGreen);

        // 添加值跟踪器
        QIM::QImPlotValueTrackerNode* tracker2 = new QIM::QImPlotValueTrackerNode(plot7);
        tracker2->setGroup(trackerGroup);
        plot7->addChildNode(tracker2);
    }

    // 添加茎叶图示例
    if (QIM::QImPlotNode* plot8 = fig->createPlotNode()) {
        plot8->x1Axis()->setLabel(u8"x8");
        plot8->y1Axis()->setLabel(u8"y8");
        plot8->setTitle("Stems Plot");
        plot8->setLegendEnabled(true);

        // 生成数据 - 离散信号数据
        int numPoints = 20;
        std::vector<double> xData(numPoints);
        std::vector<double> yData(numPoints);

        for (int i = 0; i < numPoints; ++i) {
            xData[i] = i;
            // 生成一个衰减的正弦波
            yData[i] = std::sin(i * 0.5) * std::exp(-i * 0.1) * 10.0;
        }

        // 添加垂直茎叶图（默认基线为0）
        QIM::QImPlotStemsItemNode* stems1 = new QIM::QImPlotStemsItemNode(plot8);
        stems1->setLabel("Vertical Stems");
        stems1->setData(xData, yData);
        stems1->setReferenceValue(0.0);
        stems1->setColor(Qt::blue);

        // 添加水平茎叶图（偏移Y位置避免重叠）
        std::vector<double> yOffset(numPoints);
        for (int i = 0; i < numPoints; ++i) {
            yOffset[i] = yData[i] + 5.0;  // 向上偏移
        }
        QIM::QImPlotStemsItemNode* stems2 = new QIM::QImPlotStemsItemNode(plot8);
        stems2->setLabel("Horizontal Stems");
        stems2->setData(xData, yOffset);
        stems2->setReferenceValue(5.0);  // 基线也相应偏移
        stems2->setHorizontal(true);
        stems2->setColor(Qt::darkCyan);

        // 添加值跟踪器
        QIM::QImPlotValueTrackerNode* tracker3 = new QIM::QImPlotValueTrackerNode(plot8);
        tracker3->setGroup(trackerGroup);
        plot8->addChildNode(tracker3);
    }

    // 添加无限线示例
    if (QIM::QImPlotNode* plot9 = fig->createPlotNode()) {
        plot9->x1Axis()->setLabel(u8"x9");
        plot9->y1Axis()->setLabel(u8"y9");
        plot9->setTitle("InfLines Plot");
        plot9->setLegendEnabled(true);

        std::vector< double > xLineValues = { 1.5, 4.0, 7.2 };
        std::vector< double > yLineValues = { -2.0, 0.0, 3.5 };
        std::vector< double > xData(100);
        std::vector< double > yData(100);
        for (int i = 0; i < 100; ++i) {
            xData[ i ] = i * 0.1;
            yData[ i ] = std::sin(xData[ i ]) * 2.5;
        }

        plot9->addLine(xData, yData, "Reference Curve");

        QIM::QImPlotInfLinesItemNode* verticalLines = new QIM::QImPlotInfLinesItemNode(plot9);
        verticalLines->setLabel("Vertical Guides");
        verticalLines->setValues(xLineValues);
        verticalLines->setColor(Qt::darkRed);

        QIM::QImPlotInfLinesItemNode* horizontalLines = new QIM::QImPlotInfLinesItemNode(plot9);
        horizontalLines->setLabel("Horizontal Guides");
        horizontalLines->setValues(yLineValues);
        horizontalLines->setHorizontal(true);
        horizontalLines->setColor(Qt::darkGreen);

        QIM::QImPlotTextItemNode* text1 = new QIM::QImPlotTextItemNode(plot9);
        text1->setText("Peak Area");
        text1->setPosition(QPointF(1.6, 2.6));
        text1->setPixelOffset(QPointF(8.0, -8.0));
        text1->setColor(Qt::darkMagenta);

        QIM::QImPlotTextItemNode* text2 = new QIM::QImPlotTextItemNode(plot9);
        text2->setText("X = 4.0");
        text2->setPosition(QPointF(4.0, -1.5));
        text2->setVertical(true);
        text2->setColor(Qt::darkBlue);

        QIM::QImPlotDummyItemNode* dummy = new QIM::QImPlotDummyItemNode(plot9);
        dummy->setLabel("Legend Group Placeholder");

        QIM::QImPlotValueTrackerNode* tracker4 = new QIM::QImPlotValueTrackerNode(plot9);
        tracker4->setGroup(trackerGroup);
        plot9->addChildNode(tracker4);
    }

    // 添加饼图示例
    if (QIM::QImPlotNode* plot10 = fig->createPlotNode()) {
        plot10->setTitle("Pie Chart");
        plot10->setEqual(true);
        plot10->setMouseTextEnabled(false);
        plot10->x1Axis()->setNoDecorations(true);
        plot10->y1Axis()->setNoDecorations(true);
        plot10->x1Axis()->setLimits(0.0, 1.0, QIM::QImPlotCondition::Always);
        plot10->y1Axis()->setLimits(0.0, 1.0, QIM::QImPlotCondition::Always);

        QIM::QImPlotPieChartItemNode* pie = new QIM::QImPlotPieChartItemNode(plot10);
        pie->setData(QStringList() << "Alpha" << "Beta" << "Gamma" << "Delta",
                     std::vector< double > { 12.0, 18.0, 30.0, 40.0 });
        pie->setCenter(QPointF(0.5, 0.5));
        pie->setRadius(0.38);
        pie->setLabelFormat("%.0f");
        pie->setExploding(true);
        pie->setIgnoreHidden(true);
    }

    if (QIM::QImPlotNode* plot11 = fig->createPlotNode()) {
        plot11->x1Axis()->setLabel(u8"value");
        plot11->y1Axis()->setLabel(u8"density");
        plot11->setTitle("Histogram");
        plot11->setLegendEnabled(true);

        std::vector< double > values;
        values.reserve(4000);
        std::mt19937 gen(42);
        std::normal_distribution< double > dist1(-1.0, 0.9);
        std::normal_distribution< double > dist2(1.2, 0.5);

        for (int i = 0; i < 2500; ++i) {
            values.push_back(dist1(gen));
        }
        for (int i = 0; i < 1500; ++i) {
            values.push_back(dist2(gen));
        }

        QIM::QImPlotHistogramItemNode* hist = new QIM::QImPlotHistogramItemNode(plot11);
        hist->setLabel("Distribution");
        hist->setData(values);
        hist->setBinCount(60);
        hist->setDensity(true);
        hist->setFillColor(QColor(33, 150, 243, 180));
    }
}

void MainWindow::drawPlot3D()
{
    QIM::QImFigure3DWidget* figure3D = new QIM::QImFigure3DWidget(ui->widget_2);
    figure3D->setRenderMode(QIM::QImWidget::RenderOnDemand);
    figure3D->setSubplotGrid(3, 2);
    ui->widget_2->setLayout(new QVBoxLayout());
    ui->widget_2->layout()->setContentsMargins(0, 0, 0, 0);
    ui->widget_2->layout()->addWidget(figure3D);

    if (QIM::QImPlot3DNode* plot1 = figure3D->createPlotNode()) {
        plot1->setTitle("3D Line");
        plot1->setXAxisLabel("X");
        plot1->setYAxisLabel("Y");
        plot1->setZAxisLabel("Z");
        plot1->setLegendEnabled(true);
        plot1->setEqual(true);
        plot1->setAxisLimits(QIM::QImPlot3DNode::AxisX, -1.2, 1.2);
        plot1->setAxisLimits(QIM::QImPlot3DNode::AxisY, -1.2, 1.2);
        plot1->setAxisLimits(QIM::QImPlot3DNode::AxisZ, -0.2, 6.5);

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

        QIM::QImPlot3DLineItemNode* line = new QIM::QImPlot3DLineItemNode(plot1);
        line->setLabel("Helix");
        line->setData(xData, yData, zData);
        line->setColor(QColor(33, 150, 243));
        line->setLineWidth(2.0f);
    }

    if (QIM::QImPlot3DNode* plot2 = figure3D->createPlotNode()) {
        plot2->setTitle("3D Scatter");
        plot2->setXAxisLabel("X");
        plot2->setYAxisLabel("Y");
        plot2->setZAxisLabel("Z");
        plot2->setLegendEnabled(true);
        plot2->setEqual(true);
        plot2->setAxisLimits(QIM::QImPlot3DNode::AxisX, -1.2, 1.2);
        plot2->setAxisLimits(QIM::QImPlot3DNode::AxisY, -1.2, 1.2);
        plot2->setAxisLimits(QIM::QImPlot3DNode::AxisZ, -0.2, 2.8);

        std::vector< double > scatterX;
        std::vector< double > scatterY;
        std::vector< double > scatterZ;
        scatterX.reserve(40);
        scatterY.reserve(40);
        scatterZ.reserve(40);

        for (int i = 0; i < 40; ++i) {
            const double t = static_cast< double >(i) * 0.48;
            scatterX.push_back(std::cos(t) * 0.75);
            scatterY.push_back(std::sin(t) * 0.75);
            scatterZ.push_back(t * 0.12 + 0.2);
        }

        QIM::QImPlot3DScatterItemNode* scatter = new QIM::QImPlot3DScatterItemNode(plot2);
        scatter->setLabel("Samples");
        scatter->setData(scatterX, scatterY, scatterZ);
        scatter->setMarkerShape(ImPlot3DMarker_Square);
        scatter->setMarkerSize(5.0f);
        scatter->setMarkerWeight(1.5f);
        scatter->setFillColor(QColor(255, 140, 0));
        scatter->setOutlineColor(QColor(120, 50, 0));
    }

    if (QIM::QImPlot3DNode* plot3 = figure3D->createPlotNode()) {
        plot3->setTitle("3D Surface");
        plot3->setXAxisLabel("X");
        plot3->setYAxisLabel("Y");
        plot3->setZAxisLabel("Z");
        plot3->setLegendEnabled(true);
        plot3->setEqual(true);

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

        QIM::QImPlot3DSurfaceItemNode* surface = new QIM::QImPlot3DSurfaceItemNode(plot3);
        surface->setLabel("Wave Surface");
        surface->setData(xs, ys, zs, xCount, yCount);
        surface->setColormapEnabled(true);
        surface->setColormap(ImPlot3DColormap_Viridis);
        surface->setLineColor(QColor(20, 60, 120));
        surface->setLineWidth(1.0f);
    }

    if (QIM::QImPlot3DNode* plot4 = figure3D->createPlotNode()) {
        plot4->setTitle("3D Wireframe");
        plot4->setXAxisLabel("X");
        plot4->setYAxisLabel("Y");
        plot4->setZAxisLabel("Z");
        plot4->setLegendEnabled(true);
        plot4->setEqual(true);

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
                const double z = std::sin(r * 2.0) / (1.0 + r * 0.6);
                xs.push_back(x);
                ys.push_back(y);
                zs.push_back(z);
            }
        }

        QIM::QImPlot3DSurfaceItemNode* wireframe = new QIM::QImPlot3DSurfaceItemNode(plot4);
        wireframe->setLabel("Wave Wireframe");
        wireframe->setData(xs, ys, zs, xCount, yCount);
        wireframe->setColormapEnabled(true);
        wireframe->setColormap(ImPlot3DColormap_Viridis);
        wireframe->setFillVisible(false);
        wireframe->setMarkersVisible(false);
        wireframe->setLineWidth(1.1f);
    }

    if (QIM::QImPlot3DNode* plot5 = figure3D->createPlotNode()) {
        plot5->setTitle("3D Triangle");
        plot5->setXAxisLabel("X");
        plot5->setYAxisLabel("Y");
        plot5->setZAxisLabel("Z");
        plot5->setLegendEnabled(true);
        plot5->setEqual(true);

        std::vector< double > xs = {
            0.0, -0.8, 0.8,
            0.0, 0.8, 0.8,
            0.0, 0.8, -0.8,
            0.0, -0.8, -0.8,
            -0.8, 0.8, 0.8,
            -0.8, 0.8, -0.8
        };
        std::vector< double > ys = {
            0.0, -0.8, -0.8,
            0.0, -0.8, 0.8,
            0.0, 0.8, 0.8,
            0.0, 0.8, -0.8,
            -0.8, -0.8, 0.8,
            -0.8, 0.8, 0.8
        };
        std::vector< double > zs = {
            1.0, -0.6, -0.6,
            1.0, -0.6, -0.6,
            1.0, -0.6, -0.6,
            1.0, -0.6, -0.6,
            -0.6, -0.6, -0.6,
            -0.6, -0.6, -0.6
        };

        QIM::QImPlot3DTriangleItemNode* triangle = new QIM::QImPlot3DTriangleItemNode(plot5);
        triangle->setLabel("Pyramid");
        triangle->setData(xs, ys, zs);
        triangle->setFillColor(QColor(255, 193, 7, 180));
        triangle->setLineColor(QColor(120, 85, 10));
        triangle->setLineWidth(1.2f);
    }
}
