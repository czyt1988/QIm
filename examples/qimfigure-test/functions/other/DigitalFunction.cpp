#include "DigitalFunction.h"
#include "QImFigureWidget.h"
#include "plot/QImPlotNode.h"
#include "plot/QImPlotAxisInfo.h"
#include "plot/QImPlotDigitalItemNode.h"
#include "plot/QImWaveformGenerator.hpp"
#include "plot/QImPlotValueTrackerNode.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/**
 * \if ENGLISH
 * @brief Constructor for DigitalFunction
 * @param parent Parent object
 * @details Initializes the test function and registers properties for interactive editing.
 * Properties are organized in categories:
 * - "Plot"/"Basic Info"/"Title" - Plot title
 * - "Plot"/"Axis"/"X Label" - X axis label
 * - "Plot"/"Axis"/"Y Label" - Y axis label
 * - "Digital"/"Style"/"Color" - Digital signal color
 * - "Digital"/"Style"/"Label" - Digital signal label
 * \endif
 * 
 * \if CHINESE
 * @brief DigitalFunction 构造函数
 * @param parent 父对象
 * @details 初始化测试函数并注册属性用于交互式编辑。
 * 属性按类别组织：
 * - "Plot"/"Basic Info"/"Title" - 图表标题
 * - "Plot"/"Axis"/"X Label" - X 轴标签
 * - "Plot"/"Axis"/"Y Label" - Y 轴标签
 * - "Digital"/"Style"/"Color" - 数字信号颜色
 * - "Digital"/"Style"/"Label" - 数字信号标签
 * \endif
 */
DigitalFunction::DigitalFunction(QObject* parent)
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
    
    // Register digital color property
    PropertyRegistration colorReg;
    colorReg.category = tr("Digital");
    colorReg.subcategory = tr("Style");
    colorReg.displayName = tr("Color");
    colorReg.briefDesc = tr("Digital signal color");
    colorReg.detailDesc = tr("Sets the color of the digital signal lines");
    colorReg.editorType = EditorType::ColorPicker;
    colorReg.defaultValue = m_digitalColor;
    colorReg.propertyName = "digitalColor";
    colorReg.target = this;
    registerProperty(colorReg);
    
    // Register digital label property
    PropertyRegistration labelReg;
    labelReg.category = tr("Digital");
    labelReg.subcategory = tr("Style");
    labelReg.displayName = tr("Label");
    labelReg.briefDesc = tr("Digital signal label");
    labelReg.detailDesc = tr("Sets the label text for the digital signal (shown in legend)");
    labelReg.editorType = EditorType::LineEdit;
    labelReg.defaultValue = m_digitalLabel;
    labelReg.propertyName = "digitalLabel";
    labelReg.target = this;
    registerProperty(labelReg);
}

/**
 * \if ENGLISH
 * @brief Destructor for DigitalFunction
 * \endif
 * 
 * \if CHINESE
 * @brief DigitalFunction 析构函数
 * \endif
 */
DigitalFunction::~DigitalFunction()
{
}

/**
 * \if ENGLISH
 * @brief Create plot nodes in the given figure widget
 * @param figure Pointer to the figure widget where plots will be created
 * @details Creates a plot with digital signal data, configures axes,
 * adds a digital item, and attaches a value tracker node.
 * \endif
 * 
 * \if CHINESE
 * @brief 在指定的图表控件中创建绘图节点
 * @param figure 将要创建绘图的图表控件指针
 * @details 创建一个包含数字信号数据的图表，配置坐标轴，
 * 添加数字信号项，并附加一个值追踪器节点。
 * \endif
 */
void DigitalFunction::createPlot(QIM::QImFigureWidget* figure)
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
    
    // Generate digital signal data: 0 and 1 values
    QVector<double> xs = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    QVector<double> ys = {0, 1, 1, 0, 0, 1, 1, 0, 1};  // Digital transitions
    
    // Create digital item node and set data
    m_digitalNode = new QIM::QImPlotDigitalItemNode(m_plotNode);
    m_digitalNode->setLabel(m_digitalLabel);
    m_digitalNode->setData(xs, ys);
    m_digitalNode->setColor(m_digitalColor);
    
    // Create and attach value tracker
    m_trackerNode = new QIM::QImPlotValueTrackerNode(m_plotNode);
    m_trackerNode->setGroup(nullptr);  // No group by default
    m_plotNode->addChildNode(m_trackerNode);
}

void DigitalFunction::setTitle(const QString& title)
{
    if (m_title != title) {
        m_title = title;
        emit titleChanged(title);
        if (m_plotNode) {
            m_plotNode->setTitle(title);
        }
    }
}

void DigitalFunction::setXLabel(const QString& label)
{
    if (m_xLabel != label) {
        m_xLabel = label;
        emit xLabelChanged(label);
        if (m_plotNode) {
            m_plotNode->x1Axis()->setLabel(label);
        }
    }
}

void DigitalFunction::setYLabel(const QString& label)
{
    if (m_yLabel != label) {
        m_yLabel = label;
        emit yLabelChanged(label);
        if (m_plotNode) {
            m_plotNode->y1Axis()->setLabel(label);
        }
    }
}

void DigitalFunction::setDigitalColor(const QColor& color)
{
    if (m_digitalColor != color) {
        m_digitalColor = color;
        emit digitalColorChanged(color);
        if (m_digitalNode) {
            m_digitalNode->setColor(color);
        }
    }
}

void DigitalFunction::setDigitalLabel(const QString& label)
{
    if (m_digitalLabel != label) {
        m_digitalLabel = label;
        emit digitalLabelChanged(label);
        if (m_digitalNode) {
            m_digitalNode->setLabel(label);
        }
    }
}