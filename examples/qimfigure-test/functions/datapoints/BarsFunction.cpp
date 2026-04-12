#include "BarsFunction.h"
#include "QImFigureWidget.h"
#include "plot/QImPlotNode.h"
#include "plot/QImPlotBarsItemNode.h"
#include "plot/QImPlotValueTrackerNode.h"
#include <vector>

/**
 * \if ENGLISH
 * @brief Constructor for BarsFunction
 * @param parent Parent object
 * @details Initializes the test function and registers properties for interactive editing.
 * Properties are organized in categories:
 * - "Plot"/"Basic Info"/"Title" - Plot title
 * - "Plot"/"Axis"/"X Label" - X axis label
 * - "Plot"/"Axis"/"Y Label" - Y axis label
 * - "Bars"/"Style"/"Width" - Bar width
 * - "Bars"/"Style"/"Color" - Bar color
 * \endif
 * 
 * \if CHINESE
 * @brief BarsFunction 构造函数
 * @param parent 父对象
 * @details 初始化测试函数并注册属性用于交互式编辑。
 * 属性按类别组织：
 * - "Plot"/"Basic Info"/"Title" - 图表标题
 * - "Plot"/"Axis"/"X Label" - X 轴标签
 * - "Plot"/"Axis"/"Y Label" - Y 轴标签
 * - "Bars"/"Style"/"Width" - 柱子宽度
 * - "Bars"/"Style"/"Color" - 柱子颜色
 * \endif
 */
BarsFunction::BarsFunction(QObject* parent)
    : TestFunction(parent)
{
    // Register title property
    PropertyRegistration titleReg;
    titleReg.category = tr("Plot");
    titleReg.subcategory = tr("Basic Info");
    titleReg.displayName = tr("Title");
    titleReg.briefDesc = tr("Plot title");
    titleReg.detailDesc = tr("Sets the title text displayed at the top of the plot");
    titleReg.editorType = EditorType::LineEdit;
    titleReg.defaultValue = m_title;
    titleReg.propertyName = "title";
    titleReg.target = this;
    registerProperty(titleReg);
    
    // Register X axis label property
    PropertyRegistration xLabelReg;
    xLabelReg.category = tr("Plot");
    xLabelReg.subcategory = tr("Axis");
    xLabelReg.displayName = tr("X Label");
    xLabelReg.briefDesc = tr("X axis label");
    xLabelReg.detailDesc = tr("Sets the label text for the X axis");
    xLabelReg.editorType = EditorType::LineEdit;
    xLabelReg.defaultValue = m_xLabel;
    xLabelReg.propertyName = "xLabel";
    xLabelReg.target = this;
    registerProperty(xLabelReg);
    
    // Register Y axis label property
    PropertyRegistration yLabelReg;
    yLabelReg.category = tr("Plot");
    yLabelReg.subcategory = tr("Axis");
    yLabelReg.displayName = tr("Y Label");
    yLabelReg.briefDesc = tr("Y axis label");
    yLabelReg.detailDesc = tr("Sets the label text for the Y axis");
    yLabelReg.editorType = EditorType::LineEdit;
    yLabelReg.defaultValue = m_yLabel;
    yLabelReg.propertyName = "yLabel";
    yLabelReg.target = this;
    registerProperty(yLabelReg);
    
    // Register bar width property
    PropertyRegistration widthReg;
    widthReg.category = tr("Bars");
    widthReg.subcategory = tr("Style");
    widthReg.displayName = tr("Width");
    widthReg.briefDesc = tr("Bar width");
    widthReg.detailDesc = tr("Sets the width of bars in plot units");
    widthReg.editorType = EditorType::DoubleSpinBox;
    widthReg.defaultValue = m_barWidth;
    widthReg.minValue = 0.1;
    widthReg.maxValue = 1.0;
    widthReg.stepValue = 0.05;
    widthReg.propertyName = "barWidth";
    widthReg.target = this;
    registerProperty(widthReg);
    
    // Register bar color property
    PropertyRegistration colorReg;
    colorReg.category = tr("Bars");
    colorReg.subcategory = tr("Style");
    colorReg.displayName = tr("Color");
    colorReg.briefDesc = tr("Bar color");
    colorReg.detailDesc = tr("Sets the color of the bars");
    colorReg.editorType = EditorType::ColorPicker;
    colorReg.defaultValue = m_barColor;
    colorReg.propertyName = "barColor";
    colorReg.target = this;
    registerProperty(colorReg);
}

/**
 * \if ENGLISH
 * @brief Destructor for BarsFunction
 * \endif
 * 
 * \if CHINESE
 * @brief BarsFunction 析构函数
 * \endif
 */
BarsFunction::~BarsFunction()
{
}

/**
 * \if ENGLISH
 * @brief Create plot nodes in the given figure widget
 * @param figure Pointer to the figure widget where plots will be created
 * @details Creates a bar chart with 12 bars, configures axes,
 * adds a bar item, and attaches a value tracker node.
 * \endif
 * 
 * \if CHINESE
 * @brief 在指定的图表控件中创建绘图节点
 * @param figure 将要创建绘图的图表控件指针
 * @details 创建一个包含 12 个柱子的图表，配置坐标轴，
 * 添加柱状图项，并附加一个值追踪器节点。
 * \endif
 */
void BarsFunction::createPlot(QIM::QImFigureWidget* figure)
{
    if (!figure) {
        return;
    }
    
    // Create plot node
    m_plotNode = figure->createPlotNode();
    if (!m_plotNode) {
        return;
    }
    
    // Configure axes and title
    m_plotNode->x1Axis()->setLabel(m_xLabel);
    m_plotNode->y1Axis()->setLabel(m_yLabel);
    m_plotNode->setTitle(m_title);
    m_plotNode->setLegendEnabled(true);
    
    // Generate bar chart data - 12 bars
    const int numBars = 12;
    std::vector<double> xData(numBars);
    std::vector<double> yData(numBars);
    
    for (int i = 0; i < numBars; ++i) {
        xData[i] = i;
        yData[i] = static_cast<double>(i * i) / 10.0 + 1.0;
    }
    
    // Create bar item node and set data
    m_barsNode = new QIM::QImPlotBarsItemNode(m_plotNode);
    m_barsNode->setLabel("Bars");
    m_barsNode->setData(xData, yData);
    m_barsNode->setBarWidth(m_barWidth);
    m_barsNode->setColor(m_barColor);
    
    // Create and attach value tracker
    m_trackerNode = new QIM::QImPlotValueTrackerNode(m_plotNode);
    m_trackerNode->setGroup(nullptr);  // No group by default
    m_plotNode->addChildNode(m_trackerNode);
}

void BarsFunction::setTitle(const QString& title)
{
    if (m_title != title) {
        m_title = title;
        emit titleChanged(title);
        if (m_plotNode) {
            m_plotNode->setTitle(title);
        }
    }
}

void BarsFunction::setXLabel(const QString& label)
{
    if (m_xLabel != label) {
        m_xLabel = label;
        emit xLabelChanged(label);
        if (m_plotNode) {
            m_plotNode->x1Axis()->setLabel(label);
        }
    }
}

void BarsFunction::setYLabel(const QString& label)
{
    if (m_yLabel != label) {
        m_yLabel = label;
        emit yLabelChanged(label);
        if (m_plotNode) {
            m_plotNode->y1Axis()->setLabel(label);
        }
    }
}

void BarsFunction::setBarWidth(double width)
{
    if (m_barWidth != width) {
        m_barWidth = width;
        emit barWidthChanged(width);
        if (m_barsNode) {
            m_barsNode->setBarWidth(width);
        }
    }
}

void BarsFunction::setBarColor(const QColor& color)
{
    if (m_barColor != color) {
        m_barColor = color;
        emit barColorChanged(color);
        if (m_barsNode) {
            m_barsNode->setColor(color);
        }
    }
}
