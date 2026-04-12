#include "DummyFunction.h"
#include "QImFigureWidget.h"
#include "plot/QImPlotNode.h"
#include "plot/QImPlotAxisInfo.h"
#include "plot/QImPlotLineItemNode.h"
#include "plot/QImPlotDummyItemNode.h"
#include "plot/QImPlotValueTrackerNode.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/**
 * \if ENGLISH
 * @brief Constructor for DummyFunction
 * @param parent Parent object
 * @details Initializes the test function and registers properties for interactive editing.
 * Properties are organized in categories:
 * - "Plot"/"Basic Info"/"Title" - Plot title
 * - "Plot"/"Axis"/"X Label" - X axis label
 * - "Plot"/"Axis"/"Y Label" - Y axis label
 * - "Line"/"Style"/"Color" - Background line color
 * - "Dummy"/"Style"/"Color" - Dummy item color
 * \endif
 * 
 * \if CHINESE
 * @brief DummyFunction 构造函数
 * @param parent 父对象
 * @details 初始化测试函数并注册属性用于交互式编辑。
 * 属性按类别组织：
 * - "Plot"/"Basic Info"/"Title" - 图表标题
 * - "Plot"/"Axis"/"X Label" - X 轴标签
 * - "Plot"/"Axis"/"Y Label" - Y 轴标签
 * - "Line"/"Style"/"Color" - 背景线颜色
 * - "Dummy"/"Style"/"Color" - 虚拟项颜色
 * \endif
 */
DummyFunction::DummyFunction(QObject* parent)
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
    
    // Register background line color property
    PropertyRegistration lineColorReg;
    lineColorReg.category = tr("Line");
    lineColorReg.subcategory = tr("Style");
    lineColorReg.displayName = tr("Color");
    lineColorReg.briefDesc = tr("Background line color");
    lineColorReg.detailDesc = tr("Sets the color of the background sine wave line");
    lineColorReg.editorType = EditorType::ColorPicker;
    lineColorReg.defaultValue = m_lineColor;
    lineColorReg.propertyName = "lineColor";
    lineColorReg.target = this;
    registerProperty(lineColorReg);
    
    // Register dummy item color property
    PropertyRegistration dummyColorReg;
    dummyColorReg.category = tr("Dummy");
    dummyColorReg.subcategory = tr("Style");
    dummyColorReg.displayName = tr("Color");
    dummyColorReg.briefDesc = tr("Dummy item color");
    dummyColorReg.detailDesc = tr("Sets the color of the dummy item legend icon");
    dummyColorReg.editorType = EditorType::ColorPicker;
    dummyColorReg.defaultValue = m_dummyColor;
    dummyColorReg.propertyName = "dummyColor";
    dummyColorReg.target = this;
    registerProperty(dummyColorReg);
}

/**
 * \if ENGLISH
 * @brief Destructor for DummyFunction
 * \endif
 * 
 * \if CHINESE
 * @brief DummyFunction 析构函数
 * \endif
 */
DummyFunction::~DummyFunction()
{
}

/**
 * \if ENGLISH
 * @brief Create plot nodes in the given figure widget
 * @param figure Pointer to the figure widget where plots will be created
 * @details Creates a plot with 200-point sine wave as background,
 * adds dummy items for custom legend entries, and attaches a value tracker node.
 * \endif
 * 
 * \if CHINESE
 * @brief 在指定的图表控件中创建绘图节点
 * @param figure 将要创建绘图的图表控件指针
 * @details 创建一个包含 200 点正弦波作为背景的图表，
 * 添加虚拟项用于自定义图例条目，并附加一个值追踪器节点。
 * \endif
 */
void DummyFunction::createPlot(QIM::QImFigureWidget* figure)
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
    
    // Generate 200 points sine wave data
    const int numPoints = 200;
    std::vector<double> xData(numPoints);
    std::vector<double> yData(numPoints);
    
    for (int i = 0; i < numPoints; ++i) {
        xData[i] = i * 0.05;
        yData[i] = std::sin(xData[i]) * 5.0;
    }
    
    // Create background line item
    m_lineNode = new QIM::QImPlotLineItemNode(m_plotNode);
    m_lineNode->setLabel("Sine Wave");
    m_lineNode->setData(xData, yData);
    m_lineNode->setColor(m_lineColor);
    
    // Create dummy items for legend
    m_dummyNode = new QIM::QImPlotDummyItemNode(m_plotNode);
    m_dummyNode->setLabel("Reference");
    m_dummyNode->setColor(m_dummyColor);
    
    // Create and attach value tracker
    m_trackerNode = new QIM::QImPlotValueTrackerNode(m_plotNode);
    m_trackerNode->setGroup(nullptr);  // No group by default
    m_plotNode->addChildNode(m_trackerNode);
}

void DummyFunction::setTitle(const QString& title)
{
    if (m_title != title) {
        m_title = title;
        emit titleChanged(title);
        if (m_plotNode) {
            m_plotNode->setTitle(title);
        }
    }
}

void DummyFunction::setXLabel(const QString& label)
{
    if (m_xLabel != label) {
        m_xLabel = label;
        emit xLabelChanged(label);
        if (m_plotNode) {
            m_plotNode->x1Axis()->setLabel(label);
        }
    }
}

void DummyFunction::setYLabel(const QString& label)
{
    if (m_yLabel != label) {
        m_yLabel = label;
        emit yLabelChanged(label);
        if (m_plotNode) {
            m_plotNode->y1Axis()->setLabel(label);
        }
    }
}

void DummyFunction::setLineColor(const QColor& color)
{
    if (m_lineColor != color) {
        m_lineColor = color;
        emit lineColorChanged(color);
        if (m_lineNode) {
            m_lineNode->setColor(color);
        }
    }
}

void DummyFunction::setDummyColor(const QColor& color)
{
    if (m_dummyColor != color) {
        m_dummyColor = color;
        emit dummyColorChanged(color);
        if (m_dummyNode) {
            m_dummyNode->setColor(color);
        }
    }
}
