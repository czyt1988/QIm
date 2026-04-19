#include "Plot3DSubplotsFunction.h"
#include "QImFigureWidget.h"
#include "plot3d/QImPlot3DNode.h"
#include "plot3d/QImPlot3DAxisInfo.h"
#include "plot3d/QImPlot3DLineItemNode.h"
#include "plot3d/QImPlot3DScatterItemNode.h"
#include "plot3d/QImPlot3DSurfaceItemNode.h"
#include "plot3d/QImPlot3DDummyItemNode.h"
#include "plot3d/QImPlot3DDataSeries.h"
#include <QVector>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/**
 * \if ENGLISH
 * @brief Constructor for Plot3DSubplotsFunction
 * @param parent Parent object
 * @details Initializes the test function and registers properties for interactive editing.
 * Properties are organized in categories:
 * - "Subplots"/"Layout"/"Title 1" - First subplot title (3D Line)
 * - "Subplots"/"Layout"/"Title 2" - Second subplot title (3D Scatter)
 * - "Subplots"/"Layout"/"Title 3" - Third subplot title (3D Surface)
 * - "Subplots"/"Layout"/"Title 4" - Fourth subplot title (3D Legend)
 * \endif
 * 
 * \if CHINESE
 * @brief Plot3DSubplotsFunction 构造函数
 * @param parent 父对象
 * @details 初始化测试函数并注册属性用于交互式编辑。
 * 属性按类别组织：
 * - "Subplots"/"Layout"/"Title 1" - 第一个子图标题（3D线图）
 * - "Subplots"/"Layout"/"Title 2" - 第二个子图标题（3D散点图）
 * - "Subplots"/"Layout"/"Title 3" - 第三个子图标题（3D曲面图）
 * - "Subplots"/"Layout"/"Title 4" - 第四个子图标题（3D图例）
 * \endif
 */
Plot3DSubplotsFunction::Plot3DSubplotsFunction(QObject* parent)
    : TestFunction(parent)
{
    // Register Title 1 property
    PropertyRegistration title1Reg;
    title1Reg.category = tr("Subplots");
    title1Reg.subcategory = tr("Layout");
    title1Reg.displayName = tr("Title 1");
    title1Reg.briefDesc = tr("First subplot title");
    title1Reg.detailDesc = tr("Sets the title text for the first 3D subplot (Line)");
    title1Reg.editorType = EditorType::LineEdit;
    title1Reg.defaultValue = m_title1;
    title1Reg.propertyName = "title1";
    title1Reg.target = this;
    registerProperty(title1Reg);
    
    // Register Title 2 property
    PropertyRegistration title2Reg;
    title2Reg.category = tr("Subplots");
    title2Reg.subcategory = tr("Layout");
    title2Reg.displayName = tr("Title 2");
    title2Reg.briefDesc = tr("Second subplot title");
    title2Reg.detailDesc = tr("Sets the title text for the second 3D subplot (Scatter)");
    title2Reg.editorType = EditorType::LineEdit;
    title2Reg.defaultValue = m_title2;
    title2Reg.propertyName = "title2";
    title2Reg.target = this;
    registerProperty(title2Reg);
    
    // Register Title 3 property
    PropertyRegistration title3Reg;
    title3Reg.category = tr("Subplots");
    title3Reg.subcategory = tr("Layout");
    title3Reg.displayName = tr("Title 3");
    title3Reg.briefDesc = tr("Third subplot title");
    title3Reg.detailDesc = tr("Sets the title text for the third 3D subplot (Surface)");
    title3Reg.editorType = EditorType::LineEdit;
    title3Reg.defaultValue = m_title3;
    title3Reg.propertyName = "title3";
    title3Reg.target = this;
    registerProperty(title3Reg);
    
    // Register Title 4 property
    PropertyRegistration title4Reg;
    title4Reg.category = tr("Subplots");
    title4Reg.subcategory = tr("Layout");
    title4Reg.displayName = tr("Title 4");
    title4Reg.briefDesc = tr("Fourth subplot title");
    title4Reg.detailDesc = tr("Sets the title text for the fourth 3D subplot (Legend)");
    title4Reg.editorType = EditorType::LineEdit;
    title4Reg.defaultValue = m_title4;
    title4Reg.propertyName = "title4";
    title4Reg.target = this;
    registerProperty(title4Reg);
}

/**
 * \if ENGLISH
 * @brief Destructor for Plot3DSubplotsFunction
 * \endif
 * 
 * \if CHINESE
 * @brief Plot3DSubplotsFunction 析构函数
 * \endif
 */
Plot3DSubplotsFunction::~Plot3DSubplotsFunction()
{
}

/**
 * \if ENGLISH
 * @brief Create 2x2 3D subplot grid in the given figure widget
 * @param figure Pointer to the figure widget where plots will be created
 * @details Creates 4 different 3D plots:
 * 1. Subplot 1: 3D Line (helix spiral, 200 points)
 * 2. Subplot 2: 3D Scatter (100 points)
 * 3. Subplot 3: 3D Surface (sin(x)*cos(y), 20x20 grid)
 * 4. Subplot 4: 3D Dummy items for legend demonstration
 * \endif
 * 
 * \if CHINESE
 * @brief 在指定的图表控件中创建 2x2 3D子图网格
 * @param figure 将要创建绘图的图表控件指针
 * @details 创建4种不同的3D图表：
 * 1. 子图1：3D线图（螺旋线，200个点）
 * 2. 子图2：3D散点图（100个点）
 * 3. 子图3：3D曲面图（sin(x)*cos(y)，20x20网格）
 * 4. 子图4：用于图例演示的3D虚拟项
 * \endif
 */
void Plot3DSubplotsFunction::createPlot(QIM::QImFigureWidget* figure)
{
    if (!figure) {
        return;
    }
    
    // Set 2x2 subplot grid for 3D plots
    figure->setSubplot3DGrid(2, 2);
    
    // --------------------------
    // Subplot 1: 3D Line (Helix)
    // --------------------------
    m_plot3DNode1 = figure->createPlot3DNode();
    m_plot3DNode1->setTitle(m_title1);
    m_plot3DNode1->xAxis()->setLabel("X");
    m_plot3DNode1->yAxis()->setLabel("Y");
    m_plot3DNode1->zAxis()->setLabel("Z");
    
    // Generate shorter helix (200 points)
    const int numLinePoints = 200;
    QVector<double> xsLine, ysLine, zsLine;
    xsLine.reserve(numLinePoints);
    ysLine.reserve(numLinePoints);
    zsLine.reserve(numLinePoints);
    
    for (int i = 0; i < numLinePoints; ++i) {
        double t = i * 0.05 * M_PI;  // t from 0 to 10π
        xsLine.append(std::cos(t));
        ysLine.append(std::sin(t));
        zsLine.append(t * 0.1);
    }
    
    m_lineItem = new QIM::QImPlot3DLineItemNode(m_plot3DNode1);
    m_lineItem->setData(xsLine, ysLine, zsLine);
    m_lineItem->setColor(Qt::blue);
    m_lineItem->setLineWeight(2.0f);
    m_lineItem->setLabel("Helix");
    
    // --------------------------
    // Subplot 2: 3D Scatter
    // --------------------------
    m_plot3DNode2 = figure->createPlot3DNode();
    m_plot3DNode2->setTitle(m_title2);
    m_plot3DNode2->xAxis()->setLabel("X");
    m_plot3DNode2->yAxis()->setLabel("Y");
    m_plot3DNode2->zAxis()->setLabel("Z");
    
    // Generate scatter data (100 points)
    const int numScatterPoints = 100;
    QVector<double> xsScatter, ysScatter, zsScatter;
    xsScatter.reserve(numScatterPoints);
    ysScatter.reserve(numScatterPoints);
    zsScatter.reserve(numScatterPoints);
    
    for (int i = 0; i < numScatterPoints; ++i) {
        double t = i * 0.08 * M_PI;
        xsScatter.append(std::cos(t) * 0.8);
        ysScatter.append(std::sin(t) * 0.8);
        zsScatter.append(std::sin(t * 0.5));
    }
    
    m_scatterItem = new QIM::QImPlot3DScatterItemNode(m_plot3DNode2);
    m_scatterItem->setData(xsScatter, ysScatter, zsScatter);
    m_scatterItem->setMarkerFillColor(QColor(217, 83, 25));
    m_scatterItem->setMarkerOutlineColor(QColor(120, 45, 10));
    m_scatterItem->setMarkerSize(4.0f);
    m_scatterItem->setLabel("Samples");
    
    // --------------------------
    // Subplot 3: 3D Surface (sin(x)*cos(y))
    // --------------------------
    m_plot3DNode3 = figure->createPlot3DNode();
    m_plot3DNode3->setTitle(m_title3);
    m_plot3DNode3->xAxis()->setLabel("X");
    m_plot3DNode3->yAxis()->setLabel("Y");
    m_plot3DNode3->zAxis()->setLabel("Z");
    
    // Generate 20x20 surface grid
    constexpr int gridSize = 20;
    QVector<double> xsSurface, ysSurface, zsSurface;
    xsSurface.reserve(gridSize * gridSize);
    ysSurface.reserve(gridSize * gridSize);
    zsSurface.reserve(gridSize * gridSize);
    
    for (int r = 0; r < gridSize; ++r) {
        for (int c = 0; c < gridSize; ++c) {
            double x = -3.0 + 6.0 * c / (gridSize - 1);
            double y = -3.0 + 6.0 * r / (gridSize - 1);
            xsSurface.append(x);
            ysSurface.append(y);
            zsSurface.append(std::sin(x) * std::cos(y));
        }
    }
    
    m_surfaceItem = new QIM::QImPlot3DSurfaceItemNode(m_plot3DNode3);
    m_surfaceItem->setData(xsSurface, ysSurface, zsSurface, gridSize, gridSize);
    m_surfaceItem->setColormapEnabled(true);
    m_surfaceItem->setLabel("sin(x)*cos(y)");
    
    // --------------------------
    // Subplot 4: 3D Legend with Dummy Items
    // --------------------------
    m_plot3DNode4 = figure->createPlot3DNode();
    m_plot3DNode4->setTitle(m_title4);
    // Hide axes for legend demo
    m_plot3DNode4->xAxis()->setDecorationsEnabled(false);
    m_plot3DNode4->yAxis()->setDecorationsEnabled(false);
    m_plot3DNode4->zAxis()->setDecorationsEnabled(false);
    
    // Create 3 dummy items with different colors
    m_dummyItem1 = new QIM::QImPlot3DDummyItemNode(m_plot3DNode4);
    m_dummyItem1->setLabel("Red Series");
    m_dummyItem1->setColor(Qt::red);
    
    m_dummyItem2 = new QIM::QImPlot3DDummyItemNode(m_plot3DNode4);
    m_dummyItem2->setLabel("Green Series");
    m_dummyItem2->setColor(Qt::green);
    
    m_dummyItem3 = new QIM::QImPlot3DDummyItemNode(m_plot3DNode4);
    m_dummyItem3->setLabel("Blue Series");
    m_dummyItem3->setColor(Qt::blue);
}

void Plot3DSubplotsFunction::setTitle1(const QString& title)
{
    if (m_title1 != title) {
        m_title1 = title;
        Q_EMIT title1Changed(title);
        if (m_plot3DNode1) {
            m_plot3DNode1->setTitle(title);
        }
    }
}

void Plot3DSubplotsFunction::setTitle2(const QString& title)
{
    if (m_title2 != title) {
        m_title2 = title;
        Q_EMIT title2Changed(title);
        if (m_plot3DNode2) {
            m_plot3DNode2->setTitle(title);
        }
    }
}

void Plot3DSubplotsFunction::setTitle3(const QString& title)
{
    if (m_title3 != title) {
        m_title3 = title;
        Q_EMIT title3Changed(title);
        if (m_plot3DNode3) {
            m_plot3DNode3->setTitle(title);
        }
    }
}

void Plot3DSubplotsFunction::setTitle4(const QString& title)
{
    if (m_title4 != title) {
        m_title4 = title;
        Q_EMIT title4Changed(title);
        if (m_plot3DNode4) {
            m_plot3DNode4->setTitle(title);
        }
    }
}