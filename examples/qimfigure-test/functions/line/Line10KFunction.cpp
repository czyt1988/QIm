#include "Line10KFunction.h"
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
 * @brief Constructor for Line10KFunction
 * @param parent Parent object
 * @details Initializes the test function and registers properties for interactive editing.
 * Properties are organized in categories:
 * - "Plot"/"Basic Info"/"Title" - Plot title
 * - "Plot"/"Axis"/"X Label" - X axis label
 * - "Plot"/"Axis"/"Y Label" - Y axis label
 * - "Line"/"Style"/"Color" - Line color
 * - "Line"/"Style"/"Label" - Line label
 * \endif
 * 
 * \if CHINESE
 * @brief Line10KFunction 构造函数
 * @param parent 父对象
 * @details 初始化测试函数并注册属性用于交互式编辑。
 * 属性按类别组织：
 * - "Plot"/"Basic Info"/"Title" - 图表标题
 * - "Plot"/"Axis"/"X Label" - X 轴标签
 * - "Plot"/"Axis"/"Y Label" - Y 轴标签
 * - "Line"/"Style"/"Color" - 线条颜色
 * - "Line"/"Style"/"Label" - 线条标签
 * \endif
 */
Line10KFunction::Line10KFunction(QObject* parent)
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
    
    // Register line color property
    PropertyRegistration colorReg;
    colorReg.category = tr("Line");
    colorReg.subcategory = tr("Style");
    colorReg.displayName = tr("Color");
    colorReg.briefDesc = tr("Line color");
    colorReg.detailDesc = tr("Sets the color of the line curve");
    colorReg.editorType = EditorType::ColorPicker;
    colorReg.defaultValue = m_lineColor;
    colorReg.propertyName = "lineColor";
    colorReg.target = this;
    registerProperty(colorReg);
    
    // Register line label property
    PropertyRegistration labelReg;
    labelReg.category = tr("Line");
    labelReg.subcategory = tr("Style");
    labelReg.displayName = tr("Label");
    labelReg.briefDesc = tr("Line label");
    labelReg.detailDesc = tr("Sets the label text for the line curve (shown in legend)");
    labelReg.editorType = EditorType::LineEdit;
    labelReg.defaultValue = m_lineLabel;
    labelReg.propertyName = "lineLabel";
    labelReg.target = this;
    registerProperty(labelReg);
}

/**
 * \if ENGLISH
 * @brief Destructor for Line10KFunction
 * \endif
 * 
 * \if CHINESE
 * @brief Line10KFunction 析构函数
 * \endif
 */
Line10KFunction::~Line10KFunction()
{
}

/**
 * \if ENGLISH
 * @brief Create plot nodes in the given figure widget
 * @param figure Pointer to the figure widget where plots will be created
 * @details Creates a plot with 10,000 cosine wave points, configures axes,
 * adds a line item, and attaches a value tracker node.
 * \endif
 * 
 * \if CHINESE
 * @brief 在指定的图表控件中创建绘图节点
 * @param figure 将要创建绘图的图表控件指针
 * @details 创建一个包含 10,000 个余弦波点的图表，配置坐标轴，
 * 添加线条项，并附加一个值追踪器节点。
 * \endif
 */
void Line10KFunction::createPlot(QIM::QImFigureWidget* figure)
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
    
    // Generate 10K cosine wave data
    const int numPoints = 10000;
    auto wave = QIM::make_waveform<QIM::CosineWave>(15.0, 0.001);
    auto datas = wave.generate(numPoints, 0.0, 20 * M_PI);
    
    // Create line item node and set data
    m_lineNode = new QIM::QImPlotLineItemNode(m_plotNode);
    m_lineNode->setData(datas.first, datas.second);
    m_lineNode->setColor(m_lineColor);
    
    // Create and attach value tracker
    m_trackerNode = new QIM::QImPlotValueTrackerNode(m_plotNode);
    m_trackerNode->setGroup(nullptr);  // No group by default
    m_plotNode->addChildNode(m_trackerNode);
}

void Line10KFunction::setTitle(const QString& title)
{
    if (m_title != title) {
        m_title = title;
        emit titleChanged(title);
        if (m_plotNode) {
            m_plotNode->setTitle(title);
        }
    }
}

void Line10KFunction::setXLabel(const QString& label)
{
    if (m_xLabel != label) {
        m_xLabel = label;
        emit xLabelChanged(label);
        if (m_plotNode) {
            m_plotNode->x1Axis()->setLabel(label);
        }
    }
}

void Line10KFunction::setYLabel(const QString& label)
{
    if (m_yLabel != label) {
        m_yLabel = label;
        emit yLabelChanged(label);
        if (m_plotNode) {
            m_plotNode->y1Axis()->setLabel(label);
        }
    }
}

void Line10KFunction::setLineColor(const QColor& color)
{
    if (m_lineColor != color) {
        m_lineColor = color;
        emit lineColorChanged(color);
        if (m_lineNode) {
            m_lineNode->setColor(color);
        }
    }
}

void Line10KFunction::setLineLabel(const QString& label)
{
    if (m_lineLabel != label) {
        m_lineLabel = label;
        emit lineLabelChanged(label);
        if (m_lineNode) {
            // Note: Line label would typically be set via data series or legend
            // This is a placeholder for future implementation
        }
    }
}
