#include "SubplotsFunction.h"
#include "QImFigureWidget.h"
#include "plot/QImSubplotsNode.h"
#include "plot/QImPlotNode.h"
#include "plot/QImPlotAxisInfo.h"
#include "plot/QImPlotLineItemNode.h"
#include "plot/QImPlotScatterItemNode.h"
#include "plot/QImPlotBarsItemNode.h"
#include "plot/QImPlotStairsItemNode.h"
#include <vector>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/**
 * \if ENGLISH
 * @brief Constructor for SubplotsFunction
 * @param parent Parent object
 * @details Initializes the test function and registers properties for interactive editing.
 * Properties are organized in categories:
 * - "Subplots"/"Basic Info"/"Title" - Subplots title
 * - "Subplots"/"Grid"/"Rows" - Number of rows
 * - "Subplots"/"Grid"/"Columns" - Number of columns
 * - "Subplots"/"Link"/"Link All X" - Link all X axes
 * - "Subplots"/"Link"/"Link All Y" - Link all Y axes
 * - "Subplots"/"Features"/"Legend Enabled" - Enable shared legend
 * - "Subplots"/"Features"/"Resizable" - Enable resizable panels
 * \endif
 * 
 * \if CHINESE
 * @brief SubplotsFunction 构造函数
 * @param parent 父对象
 * @details 初始化测试函数并注册属性用于交互式编辑。
 * 属性按类别组织：
 * - "Subplots"/"Basic Info"/"Title" - 子图标题
 * - "Subplots"/"Grid"/"Rows" - 行数
 * - "Subplots"/"Grid"/"Columns" - 列数
 * - "Subplots"/"Link"/"Link All X" - 链接所有X轴
 * - "Subplots"/"Link"/"Link All Y" - 链接所有Y轴
 * - "Subplots"/"Features"/"Legend Enabled" - 启用共享图例
 * - "Subplots"/"Features"/"Resizable" - 启用可调整面板
 * \endif
 */
SubplotsFunction::SubplotsFunction(QObject* parent)
    : TestFunction(parent)
{
    // Register title property
    PropertyRegistration titleReg;
    titleReg.category = tr("Subplots");
    titleReg.subcategory = tr("Basic Info");
    titleReg.displayName = tr("Title");
    titleReg.briefDesc = tr("Subplots title");
    titleReg.detailDesc = tr("Sets the title text displayed at the top of the subplots");
    titleReg.editorType = EditorType::LineEdit;
    titleReg.defaultValue = m_title;
    titleReg.propertyName = "title";
    titleReg.target = this;
    registerProperty(titleReg);
    
    // Register rows property
    PropertyRegistration rowsReg;
    rowsReg.category = tr("Subplots");
    rowsReg.subcategory = tr("Grid");
    rowsReg.displayName = tr("Rows");
    rowsReg.briefDesc = tr("Number of rows");
    rowsReg.detailDesc = tr("Sets the number of rows in the subplot grid");
    rowsReg.editorType = EditorType::SpinBox;
    rowsReg.defaultValue = m_rows;
    rowsReg.minValue = 1;
    rowsReg.maxValue = 4;
    rowsReg.stepValue = 1;
    rowsReg.propertyName = "rows";
    rowsReg.target = this;
    registerProperty(rowsReg);
    
    // Register columns property
    PropertyRegistration colsReg;
    colsReg.category = tr("Subplots");
    colsReg.subcategory = tr("Grid");
    colsReg.displayName = tr("Columns");
    colsReg.briefDesc = tr("Number of columns");
    colsReg.detailDesc = tr("Sets the number of columns in the subplot grid");
    colsReg.editorType = EditorType::SpinBox;
    colsReg.defaultValue = m_cols;
    colsReg.minValue = 1;
    colsReg.maxValue = 4;
    colsReg.stepValue = 1;
    colsReg.propertyName = "cols";
    colsReg.target = this;
    registerProperty(colsReg);
    
    // Register link all X axes property
    PropertyRegistration linkXReg;
    linkXReg.category = tr("Subplots");
    linkXReg.subcategory = tr("Link");
    linkXReg.displayName = tr("Link All X");
    linkXReg.briefDesc = tr("Link X axes");
    linkXReg.detailDesc = tr("When enabled, all subplots share the same X axis range");
    linkXReg.editorType = EditorType::CheckBox;
    linkXReg.defaultValue = m_linkAllX;
    linkXReg.propertyName = "linkAllX";
    linkXReg.target = this;
    registerProperty(linkXReg);
    
    // Register link all Y axes property
    PropertyRegistration linkYReg;
    linkYReg.category = tr("Subplots");
    linkYReg.subcategory = tr("Link");
    linkYReg.displayName = tr("Link All Y");
    linkYReg.briefDesc = tr("Link Y axes");
    linkYReg.detailDesc = tr("When enabled, all subplots share the same Y axis range");
    linkYReg.editorType = EditorType::CheckBox;
    linkYReg.defaultValue = m_linkAllY;
    linkYReg.propertyName = "linkAllY";
    linkYReg.target = this;
    registerProperty(linkYReg);
    
    // Register legend enabled property
    PropertyRegistration legendReg;
    legendReg.category = tr("Subplots");
    legendReg.subcategory = tr("Features");
    legendReg.displayName = tr("Legend Enabled");
    legendReg.briefDesc = tr("Enable legend");
    legendReg.detailDesc = tr("When enabled, a shared legend is displayed for all subplots");
    legendReg.editorType = EditorType::CheckBox;
    legendReg.defaultValue = m_legendEnabled;
    legendReg.propertyName = "legendEnabled";
    legendReg.target = this;
    registerProperty(legendReg);
    
    // Register resizable property
    PropertyRegistration resizableReg;
    resizableReg.category = tr("Subplots");
    resizableReg.subcategory = tr("Features");
    resizableReg.displayName = tr("Resizable");
    resizableReg.briefDesc = tr("Enable resizable panels");
    resizableReg.detailDesc = tr("When enabled, subplot panels can be resized by dragging");
    resizableReg.editorType = EditorType::CheckBox;
    resizableReg.defaultValue = m_resizable;
    resizableReg.propertyName = "resizable";
    resizableReg.target = this;
    registerProperty(resizableReg);
}

/**
 * \if ENGLISH
 * @brief Destructor for SubplotsFunction
 * \endif
 * 
 * \if CHINESE
 * @brief SubplotsFunction 析构函数
 * \endif
 */
SubplotsFunction::~SubplotsFunction()
{
}

/**
 * \if ENGLISH
 * @brief Create plot nodes in the given figure widget
 * @param figure Pointer to the figure widget where plots will be created
 * @details Creates a subplot grid with different plot types in each cell:
 * - Top-left: Line plot
 * - Top-right: Scatter plot
 * - Bottom-left: Bar chart
 * - Bottom-right: Stairs plot
 * \endif
 * 
 * \if CHINESE
 * @brief 在指定的图表控件中创建绘图节点
 * @param figure 将要创建绘图的图表控件指针
 * @details 创建一个子图网格，每个单元格展示不同的绘图类型：
 * - 左上：线图
 * - 右上：散点图
 * - 左下：柱状图
 * - 右下：阶梯图
 * \endif
 */
void SubplotsFunction::createPlot(QIM::QImFigureWidget* figure)
{
    if (!figure) {
        return;
    }
    
    // Create subplots node
    m_subplotsNode = new QIM::QImSubplotsNode(figure);
    m_subplotsNode->setTitle(m_title);
    m_subplotsNode->setGrid(m_rows, m_cols);
    m_subplotsNode->setLinkAllX(m_linkAllX);
    m_subplotsNode->setLinkAllY(m_linkAllY);
    m_subplotsNode->setLegendEnabled(m_legendEnabled);
    m_subplotsNode->setResizable(m_resizable);
    
    // Create plots for each subplot cell
    const int numPoints = 50;
    std::vector<double> xData(numPoints);
    std::vector<double> yData(numPoints);
    
    for (int i = 0; i < numPoints; ++i) {
        xData[i] = i * 0.2;
        yData[i] = std::sin(xData[i] * M_PI);
    }
    
    // Plot 1: Line plot
    QIM::QImPlotNode* plot1 = m_subplotsNode->createPlotNode();
    if (plot1) {
        plot1->setTitle(tr("Line"));
        plot1->x1Axis()->setLabel(tr("x"));
        plot1->y1Axis()->setLabel(tr("sin(x)"));
        
        QIM::QImPlotLineItemNode* lineNode = new QIM::QImPlotLineItemNode(plot1);
        lineNode->setData(xData, yData);
        lineNode->setColor(QColor(100, 150, 255));
        plot1->addChildNode(lineNode);
        
        m_plotNodes.append(plot1);
    }
    
    // Plot 2: Scatter plot
    QIM::QImPlotNode* plot2 = m_subplotsNode->createPlotNode();
    if (plot2) {
        plot2->setTitle(tr("Scatter"));
        plot2->x1Axis()->setLabel(tr("x"));
        plot2->y1Axis()->setLabel(tr("cos(x)"));
        
        QIM::QImPlotScatterItemNode* scatterNode = new QIM::QImPlotScatterItemNode(plot2);
        std::vector<double> yData2(numPoints);
        for (int i = 0; i < numPoints; ++i) {
            yData2[i] = std::cos(xData[i] * M_PI);
        }
        scatterNode->setData(xData, yData2);
        scatterNode->setColor(QColor(255, 100, 100));
        plot2->addChildNode(scatterNode);
        
        m_plotNodes.append(plot2);
    }
    
    // Plot 3: Bar chart
    QIM::QImPlotNode* plot3 = m_subplotsNode->createPlotNode();
    if (plot3) {
        plot3->setTitle(tr("Bars"));
        plot3->x1Axis()->setLabel(tr("x"));
        plot3->y1Axis()->setLabel(tr("y"));
        
        QIM::QImPlotBarsItemNode* barsNode = new QIM::QImPlotBarsItemNode(plot3);
        std::vector<double> xBars(10);
        std::vector<double> yBars(10);
        for (int i = 0; i < 10; ++i) {
            xBars[i] = i;
            yBars[i] = i * i * 0.1;
        }
        barsNode->setData(xBars, yBars);
        barsNode->setColor(QColor(100, 200, 100));
        plot3->addChildNode(barsNode);
        
        m_plotNodes.append(plot3);
    }
    
    // Plot 4: Stairs plot
    QIM::QImPlotNode* plot4 = m_subplotsNode->createPlotNode();
    if (plot4) {
        plot4->setTitle(tr("Stairs"));
        plot4->x1Axis()->setLabel(tr("x"));
        plot4->y1Axis()->setLabel(tr("y"));
        
        QIM::QImPlotStairsItemNode* stairsNode = new QIM::QImPlotStairsItemNode(plot4);
        stairsNode->setData(xData, yData);
        stairsNode->setColor(QColor(200, 100, 200));
        plot4->addChildNode(stairsNode);
        
        m_plotNodes.append(plot4);
    }
}

void SubplotsFunction::setTitle(const QString& title)
{
    if (m_title != title) {
        m_title = title;
        emit titleChanged(title);
        if (m_subplotsNode) {
            m_subplotsNode->setTitle(title);
        }
    }
}

void SubplotsFunction::setRows(int rows)
{
    if (m_rows != rows) {
        m_rows = rows;
        emit rowsChanged(rows);
        // Note: Grid changes require recreate
    }
}

void SubplotsFunction::setCols(int cols)
{
    if (m_cols != cols) {
        m_cols = cols;
        emit colsChanged(cols);
        // Note: Grid changes require recreate
    }
}

void SubplotsFunction::setLinkAllX(bool enabled)
{
    if (m_linkAllX != enabled) {
        m_linkAllX = enabled;
        emit linkAllXChanged(enabled);
        if (m_subplotsNode) {
            m_subplotsNode->setLinkAllX(enabled);
        }
    }
}

void SubplotsFunction::setLinkAllY(bool enabled)
{
    if (m_linkAllY != enabled) {
        m_linkAllY = enabled;
        emit linkAllYChanged(enabled);
        if (m_subplotsNode) {
            m_subplotsNode->setLinkAllY(enabled);
        }
    }
}

void SubplotsFunction::setLegendEnabled(bool enabled)
{
    if (m_legendEnabled != enabled) {
        m_legendEnabled = enabled;
        emit legendEnabledChanged(enabled);
        if (m_subplotsNode) {
            m_subplotsNode->setLegendEnabled(enabled);
        }
    }
}

void SubplotsFunction::setResizable(bool enabled)
{
    if (m_resizable != enabled) {
        m_resizable = enabled;
        emit resizableChanged(enabled);
        if (m_subplotsNode) {
            m_subplotsNode->setResizable(enabled);
        }
    }
}