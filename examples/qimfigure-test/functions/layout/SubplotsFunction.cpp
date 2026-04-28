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
    
    // Register subplot width property
    PropertyRegistration widthReg;
    widthReg.category = tr("Subplots");
    widthReg.subcategory = tr("Layout");
    widthReg.displayName = tr("Width");
    widthReg.briefDesc = tr("Subplot width");
    widthReg.detailDesc = tr("Sets the width of each subplot cell (-1 for auto)");
    widthReg.editorType = EditorType::DoubleSpinBox;
    widthReg.defaultValue = m_size.width();
    widthReg.minValue = -1;
    widthReg.maxValue = 2000;
    widthReg.stepValue = 10;
    widthReg.propertyName = "subplotWidth";
    widthReg.target = this;
    registerProperty(widthReg);
    
    // Register subplot height property
    PropertyRegistration heightReg;
    heightReg.category = tr("Subplots");
    heightReg.subcategory = tr("Layout");
    heightReg.displayName = tr("Height");
    heightReg.briefDesc = tr("Subplot height");
    heightReg.detailDesc = tr("Sets the height of each subplot cell (-1 for auto)");
    heightReg.editorType = EditorType::DoubleSpinBox;
    heightReg.defaultValue = m_size.height();
    heightReg.minValue = -1;
    heightReg.maxValue = 2000;
    heightReg.stepValue = 10;
    heightReg.propertyName = "subplotHeight";
    heightReg.target = this;
    registerProperty(heightReg);
    
    // Register title enabled property
    PropertyRegistration titleEnabledReg;
    titleEnabledReg.category = tr("Subplots");
    titleEnabledReg.subcategory = tr("Features");
    titleEnabledReg.displayName = tr("Title Enabled");
    titleEnabledReg.briefDesc = tr("Enable subplot title");
    titleEnabledReg.detailDesc = tr("When enabled, the subplot title is displayed");
    titleEnabledReg.editorType = EditorType::CheckBox;
    titleEnabledReg.defaultValue = m_isTitleEnabled;
    titleEnabledReg.propertyName = "isTitleEnabled";
    titleEnabledReg.target = this;
    registerProperty(titleEnabledReg);
    
    // Register default menus enabled property
    PropertyRegistration defaultMenusReg;
    defaultMenusReg.category = tr("Subplots");
    defaultMenusReg.subcategory = tr("Features");
    defaultMenusReg.displayName = tr("Default Menus");
    defaultMenusReg.briefDesc = tr("Enable default menus");
    defaultMenusReg.detailDesc = tr("When enabled, ImPlot default context menus are available");
    defaultMenusReg.editorType = EditorType::CheckBox;
    defaultMenusReg.defaultValue = m_isDefaultMenusEnabled;
    defaultMenusReg.propertyName = "isDefaultMenusEnabled";
    defaultMenusReg.target = this;
    registerProperty(defaultMenusReg);
    
    // Register aligned enabled property
    PropertyRegistration alignedReg;
    alignedReg.category = tr("Subplots");
    alignedReg.subcategory = tr("Features");
    alignedReg.displayName = tr("Aligned");
    alignedReg.briefDesc = tr("Enable aligned plots");
    alignedReg.detailDesc = tr("When enabled, subplot cells are aligned to pixel boundaries");
    alignedReg.editorType = EditorType::CheckBox;
    alignedReg.defaultValue = m_isAlignedEnabled;
    alignedReg.propertyName = "isAlignedEnabled";
    alignedReg.target = this;
    registerProperty(alignedReg);
    
    // Register share items enabled property
    PropertyRegistration shareItemsReg;
    shareItemsReg.category = tr("Subplots");
    shareItemsReg.subcategory = tr("Features");
    shareItemsReg.displayName = tr("Share Items");
    shareItemsReg.briefDesc = tr("Enable shared items");
    shareItemsReg.detailDesc = tr("When enabled, plot items are shared across subplot cells");
    shareItemsReg.editorType = EditorType::CheckBox;
    shareItemsReg.defaultValue = m_isShareItemsEnabled;
    shareItemsReg.propertyName = "isShareItemsEnabled";
    shareItemsReg.target = this;
    registerProperty(shareItemsReg);
    
    // Register link rows property
    PropertyRegistration linkRowsReg;
    linkRowsReg.category = tr("Subplots");
    linkRowsReg.subcategory = tr("Link");
    linkRowsReg.displayName = tr("Link Rows");
    linkRowsReg.briefDesc = tr("Link rows");
    linkRowsReg.detailDesc = tr("When enabled, all plots in the same row share linked axes");
    linkRowsReg.editorType = EditorType::CheckBox;
    linkRowsReg.defaultValue = m_isLinkRows;
    linkRowsReg.propertyName = "isLinkRows";
    linkRowsReg.target = this;
    registerProperty(linkRowsReg);
    
    // Register link columns property
    PropertyRegistration linkColsReg;
    linkColsReg.category = tr("Subplots");
    linkColsReg.subcategory = tr("Link");
    linkColsReg.displayName = tr("Link Columns");
    linkColsReg.briefDesc = tr("Link columns");
    linkColsReg.detailDesc = tr("When enabled, all plots in the same column share linked axes");
    linkColsReg.editorType = EditorType::CheckBox;
    linkColsReg.defaultValue = m_isLinkColumns;
    linkColsReg.propertyName = "isLinkColumns";
    linkColsReg.target = this;
    registerProperty(linkColsReg);
    
    // Register column major property
    PropertyRegistration columnMajorReg;
    columnMajorReg.category = tr("Subplots");
    columnMajorReg.subcategory = tr("Layout");
    columnMajorReg.displayName = tr("Column Major");
    columnMajorReg.briefDesc = tr("Enable column major order");
    columnMajorReg.detailDesc = tr("When enabled, subplot cells are filled in column-major order instead of row-major");
    columnMajorReg.editorType = EditorType::CheckBox;
    columnMajorReg.defaultValue = m_isColumnMajor;
    columnMajorReg.propertyName = "isColumnMajor";
    columnMajorReg.target = this;
    registerProperty(columnMajorReg);
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
 * @brief Cleanup plot nodes
 * @details Clears plot node references. Does NOT delete the subplots node
 * because it's the figure widget's internal subplotNode, not a newly created one.
 * Plot nodes are managed by the figure widget and will be removed when
 * MainWindow clears the figure.
 * \endif
 * 
 * \if CHINESE
 * @brief 清理绘图节点
 * @details 清空绘图节点引用。不删除子图节点，因为它不是新创建的，
 * 而是 figure 控件内部的 subplotNode。绘图节点由 figure 控件管理，
 * MainWindow 清空 figure 时会被移除。
 * \endif
 */
void SubplotsFunction::cleanupPlot()
{
    // Clear references - don't delete m_subplotsNode as it's figure's internal node
    m_subplotsNode = nullptr;
    m_plotNodes.clear();
}

/**
 * \if ENGLISH
 * @brief Create plot nodes in the given figure widget
 * @param figure Pointer to the figure widget where plots will be created
 * @details Uses the figure's internal subplotNode instead of creating a new one.
 * This prevents nested SubplotsNode which would corrupt ImPlot's style stack.
 * Creates a subplot grid with different plot types in each cell:
 * - Top-left: Line plot
 * - Top-right: Scatter plot
 * - Bottom-left: Bar chart
 * - Bottom-right: Stairs plot
 * \endif
 * 
 * \if CHINESE
 * @brief 在指定的图表控件中创建绘图节点
 * @param figure 将要创建绘图的图表控件指针
 * @details 使用 figure 内部的 subplotNode 而不是创建新的。
 * 这可以防止嵌套的 SubplotsNode 打乱 ImPlot 的样式栈。
 * 创建一个子图网格，每个单元格展示不同的绘图类型：
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
    
    // Create subplot grid (this creates QImSubplotsNode on demand)
    // With the new lazy architecture, subplot only exists when explicitly requested
    figure->setSubplotGrid(m_rows, m_cols);
    
    m_subplotsNode = figure->subplotNode();
    if (!m_subplotsNode) {
        return;
    }
    
    // Configure subplot properties (grid is already set via setSubplotGrid above)
    m_subplotsNode->setTitle(m_title);
    m_subplotsNode->setLinkAllX(m_linkAllX);
    m_subplotsNode->setLinkAllY(m_linkAllY);
    m_subplotsNode->setLegendEnabled(m_legendEnabled);
    m_subplotsNode->setResizable(m_resizable);
    
    // Create plots for each subplot cell using figure->createPlotNode()
    const int numPoints = 50;
    std::vector<double> xData(numPoints);
    std::vector<double> yData(numPoints);
    
    for (int i = 0; i < numPoints; ++i) {
        xData[i] = i * 0.2;
        yData[i] = std::sin(xData[i] * M_PI);
    }
    
    // Plot 1: Line plot
    QIM::QImPlotNode* plot1 = figure->createPlotNode();
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
    QIM::QImPlotNode* plot2 = figure->createPlotNode();
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
    QIM::QImPlotNode* plot3 = figure->createPlotNode();
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
    QIM::QImPlotNode* plot4 = figure->createPlotNode();
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
        Q_EMIT titleChanged(title);
        if (m_subplotsNode) {
            m_subplotsNode->setTitle(title);
        }
    }
}

void SubplotsFunction::setRows(int rows)
{
    if (m_rows != rows && rows > 0) {
        m_rows = rows;
        Q_EMIT rowsChanged(rows);
        // Update subplot grid in real-time
        if (m_subplotsNode) {
            m_subplotsNode->setRows(rows);
        }
    }
}

void SubplotsFunction::setCols(int cols)
{
    if (m_cols != cols && cols > 0) {
        m_cols = cols;
        Q_EMIT colsChanged(cols);
        // Update subplot grid in real-time
        if (m_subplotsNode) {
            m_subplotsNode->setColumns(cols);
        }
    }
}

void SubplotsFunction::setLinkAllX(bool enabled)
{
    if (m_linkAllX != enabled) {
        m_linkAllX = enabled;
        Q_EMIT linkAllXChanged(enabled);
        if (m_subplotsNode) {
            m_subplotsNode->setLinkAllX(enabled);
        }
    }
}

void SubplotsFunction::setLinkAllY(bool enabled)
{
    if (m_linkAllY != enabled) {
        m_linkAllY = enabled;
        Q_EMIT linkAllYChanged(enabled);
        if (m_subplotsNode) {
            m_subplotsNode->setLinkAllY(enabled);
        }
    }
}

void SubplotsFunction::setLegendEnabled(bool enabled)
{
    if (m_legendEnabled != enabled) {
        m_legendEnabled = enabled;
        Q_EMIT legendEnabledChanged(enabled);
        if (m_subplotsNode) {
            m_subplotsNode->setLegendEnabled(enabled);
        }
    }
}

void SubplotsFunction::setResizable(bool enabled)
{
    if (m_resizable != enabled) {
        m_resizable = enabled;
        Q_EMIT resizableChanged(enabled);
        if (m_subplotsNode) {
            m_subplotsNode->setResizable(enabled);
        }
    }
}

void SubplotsFunction::setSubplotWidth(qreal width)
{
    if (m_size.width() != width) {
        m_size.setWidth(width);
        Q_EMIT subplotWidthChanged(width);
        if (m_subplotsNode) {
            m_subplotsNode->setSize(m_size);
        }
    }
}

void SubplotsFunction::setSubplotHeight(qreal height)
{
    if (m_size.height() != height) {
        m_size.setHeight(height);
        Q_EMIT subplotHeightChanged(height);
        if (m_subplotsNode) {
            m_subplotsNode->setSize(m_size);
        }
    }
}

void SubplotsFunction::setTitleEnabled(bool enabled)
{
    if (m_isTitleEnabled != enabled) {
        m_isTitleEnabled = enabled;
        Q_EMIT titleEnabledChanged(enabled);
        if (m_subplotsNode) {
            m_subplotsNode->setTitleEnabled(enabled);
        }
    }
}

void SubplotsFunction::setDefaultMenusEnabled(bool enabled)
{
    if (m_isDefaultMenusEnabled != enabled) {
        m_isDefaultMenusEnabled = enabled;
        Q_EMIT defaultMenusEnabledChanged(enabled);
        if (m_subplotsNode) {
            m_subplotsNode->setDefaultMenusEnabled(enabled);
        }
    }
}

void SubplotsFunction::setAlignedEnabled(bool enabled)
{
    if (m_isAlignedEnabled != enabled) {
        m_isAlignedEnabled = enabled;
        Q_EMIT alignedEnabledChanged(enabled);
        if (m_subplotsNode) {
            m_subplotsNode->setAlignedEnabled(enabled);
        }
    }
}

void SubplotsFunction::setShareItemsEnabled(bool enabled)
{
    if (m_isShareItemsEnabled != enabled) {
        m_isShareItemsEnabled = enabled;
        Q_EMIT shareItemsEnabledChanged(enabled);
        if (m_subplotsNode) {
            m_subplotsNode->setShareItemsEnabled(enabled);
        }
    }
}

void SubplotsFunction::setLinkRows(bool enabled)
{
    if (m_isLinkRows != enabled) {
        m_isLinkRows = enabled;
        Q_EMIT linkRowsChanged(enabled);
        if (m_subplotsNode) {
            m_subplotsNode->setLinkRows(enabled);
        }
    }
}

void SubplotsFunction::setLinkColumns(bool enabled)
{
    if (m_isLinkColumns != enabled) {
        m_isLinkColumns = enabled;
        Q_EMIT linkColumnsChanged(enabled);
        if (m_subplotsNode) {
            m_subplotsNode->setLinkColumns(enabled);
        }
    }
}

void SubplotsFunction::setColumnMajor(bool enabled)
{
    if (m_isColumnMajor != enabled) {
        m_isColumnMajor = enabled;
        Q_EMIT columnMajorChanged(enabled);
        if (m_subplotsNode) {
            m_subplotsNode->setColumnMajor(enabled);
        }
    }
}