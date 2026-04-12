#include "Line1MFunction.h"
#include "QImFigureWidget.h"
#include "plot/QImPlotNode.h"
#include "plot/QImPlotAxisInfo.h"
#include "plot/QImPlotLineItemNode.h"
#include "plot/QImWaveformGenerator.hpp"
#include "plot/QImPlotValueTrackerNode.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/**
 * \if ENGLISH
 * @brief Constructor for Line1MFunction
 * @param parent Parent object
 * @details Initializes the test function and registers properties for interactive editing.
 * Properties are organized in categories:
 * - "Plot"/"Basic Info"/"Title" - Plot title
 * - "Plot"/"Axis"/"X Label" - X axis label
 * - "Plot"/"Axis"/"Y Label" - Y axis label
 * - "Line 1"/"Style"/"Color" - First line color
 * - "Line 1"/"Style"/"Label" - First line label
 * - "Line 2"/"Style"/"Color" - Second line color
 * - "Line 2"/"Style"/"Label" - Second line label
 * \endif
 * 
 * \if CHINESE
 * @brief Line1MFunction 构造函数
 * @param parent 父对象
 * @details 初始化测试函数并注册属性用于交互式编辑。
 * 属性按类别组织：
 * - "Plot"/"Basic Info"/"Title" - 图表标题
 * - "Plot"/"Axis"/"X Label" - X 轴标签
 * - "Plot"/"Axis"/"Y Label" - Y 轴标签
 * - "Line 1"/"Style"/"Color" - 第一条曲线颜色
 * - "Line 1"/"Style"/"Label" - 第一条曲线标签
 * - "Line 2"/"Style"/"Color" - 第二条曲线颜色
 * - "Line 2"/"Style"/"Label" - 第二条曲线标签
 * \endif
 */
Line1MFunction::Line1MFunction(QObject* parent)
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
    
    // Register line 1 color property
    PropertyRegistration line1ColorReg;
    line1ColorReg.category = tr("Line 1");
    line1ColorReg.subcategory = tr("Style");
    line1ColorReg.displayName = tr("Color");
    line1ColorReg.briefDesc = tr("Line 1 color");
    line1ColorReg.detailDesc = tr("Sets the color of the first line curve (DampedSineWave)");
    line1ColorReg.editorType = EditorType::ColorPicker;
    line1ColorReg.defaultValue = m_line1Color;
    line1ColorReg.propertyName = "line1Color";
    line1ColorReg.target = this;
    registerProperty(line1ColorReg);
    
    // Register line 1 label property
    PropertyRegistration line1LabelReg;
    line1LabelReg.category = tr("Line 1");
    line1LabelReg.subcategory = tr("Style");
    line1LabelReg.displayName = tr("Label");
    line1LabelReg.briefDesc = tr("Line 1 label");
    line1LabelReg.detailDesc = tr("Sets the label text for the first line curve (shown in legend)");
    line1LabelReg.editorType = EditorType::LineEdit;
    line1LabelReg.defaultValue = m_line1Label;
    line1LabelReg.propertyName = "line1Label";
    line1LabelReg.target = this;
    registerProperty(line1LabelReg);
    
    // Register line 2 color property
    PropertyRegistration line2ColorReg;
    line2ColorReg.category = tr("Line 2");
    line2ColorReg.subcategory = tr("Style");
    line2ColorReg.displayName = tr("Color");
    line2ColorReg.briefDesc = tr("Line 2 color");
    line2ColorReg.detailDesc = tr("Sets the color of the second line curve (CosineWave)");
    line2ColorReg.editorType = EditorType::ColorPicker;
    line2ColorReg.defaultValue = m_line2Color;
    line2ColorReg.propertyName = "line2Color";
    line2ColorReg.target = this;
    registerProperty(line2ColorReg);
    
    // Register line 2 label property
    PropertyRegistration line2LabelReg;
    line2LabelReg.category = tr("Line 2");
    line2LabelReg.subcategory = tr("Style");
    line2LabelReg.displayName = tr("Label");
    line2LabelReg.briefDesc = tr("Line 2 label");
    line2LabelReg.detailDesc = tr("Sets the label text for the second line curve (shown in legend)");
    line2LabelReg.editorType = EditorType::LineEdit;
    line2LabelReg.defaultValue = m_line2Label;
    line2LabelReg.propertyName = "line2Label";
    line2LabelReg.target = this;
    registerProperty(line2LabelReg);
}

/**
 * \if ENGLISH
 * @brief Destructor for Line1MFunction
 * \endif
 * 
 * \if CHINESE
 * @brief Line1MFunction 析构函数
 * \endif
 */
Line1MFunction::~Line1MFunction()
{
}

/**
 * \if ENGLISH
 * @brief Create plot nodes in the given figure widget
 * @param figure Pointer to the figure widget where plots will be created
 * @details Creates a plot with 1,000,000 points using DampedSineWave and CosineWave,
 * configures axes, adds two line items with legend enabled, and attaches a value tracker node.
 * \endif
 * 
 * \if CHINESE
 * @brief 在指定的图表控件中创建绘图节点
 * @param figure 将要创建绘图的图表控件指针
 * @details 使用 DampedSineWave 和 CosineWave 创建一个包含 1,000,000 个点的图表，
 * 配置坐标轴，添加两条带图例的曲线，并附加一个值追踪器节点。
 * \endif
 */
void Line1MFunction::createPlot(QIM::QImFigureWidget* figure)
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
    
    // Generate 1M points: DampedSineWave for line 1
    const int numPoints = 1000000;
    auto dampedWave = QIM::make_waveform<QIM::DampedSineWave>(3.0, 0.15);
    auto datas = dampedWave.generate(numPoints, 0.0, 4.0 * M_PI);
    
    // Create first line item node and set data
    m_line1Node = new QIM::QImPlotLineItemNode(m_plotNode);
    m_line1Node->setData(datas.first, datas.second);
    m_line1Node->setColor(m_line1Color);
    
    // Generate 1M points: CosineWave for line 2
    auto cosWave = QIM::make_waveform<QIM::CosineWave>(2.0, M_PI / 4.0);
    datas = cosWave.generate(numPoints, -M_PI, M_PI);
    
    // Create second line item node and set data
    m_line2Node = new QIM::QImPlotLineItemNode(m_plotNode);
    m_line2Node->setData(datas.first, datas.second);
    m_line2Node->setColor(m_line2Color);
    
    // Create and attach value tracker
    m_trackerNode = new QIM::QImPlotValueTrackerNode(m_plotNode);
    m_trackerNode->setGroup(nullptr);  // No group by default
    m_plotNode->addChildNode(m_trackerNode);
}

void Line1MFunction::setTitle(const QString& title)
{
    if (m_title != title) {
        m_title = title;
        emit titleChanged(title);
        if (m_plotNode) {
            m_plotNode->setTitle(title);
        }
    }
}

void Line1MFunction::setXLabel(const QString& label)
{
    if (m_xLabel != label) {
        m_xLabel = label;
        emit xLabelChanged(label);
        if (m_plotNode) {
            m_plotNode->x1Axis()->setLabel(label);
        }
    }
}

void Line1MFunction::setYLabel(const QString& label)
{
    if (m_yLabel != label) {
        m_yLabel = label;
        emit yLabelChanged(label);
        if (m_plotNode) {
            m_plotNode->y1Axis()->setLabel(label);
        }
    }
}

void Line1MFunction::setLine1Color(const QColor& color)
{
    if (m_line1Color != color) {
        m_line1Color = color;
        emit line1ColorChanged(color);
        if (m_line1Node) {
            m_line1Node->setColor(color);
        }
    }
}

void Line1MFunction::setLine1Label(const QString& label)
{
    if (m_line1Label != label) {
        m_line1Label = label;
        emit line1LabelChanged(label);
        // Line label would typically be set via data series or legend
        // This is a placeholder for future implementation
    }
}

void Line1MFunction::setLine2Color(const QColor& color)
{
    if (m_line2Color != color) {
        m_line2Color = color;
        emit line2ColorChanged(color);
        if (m_line2Node) {
            m_line2Node->setColor(color);
        }
    }
}

void Line1MFunction::setLine2Label(const QString& label)
{
    if (m_line2Label != label) {
        m_line2Label = label;
        emit line2LabelChanged(label);
        // Line label would typically be set via data series or legend
        // This is a placeholder for future implementation
    }
}
