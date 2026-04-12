#include "StemsFunction.h"
#include "QImFigureWidget.h"
#include "plot/QImPlotNode.h"
#include "plot/QImPlotAxisInfo.h"
#include "plot/QImPlotStemsItemNode.h"
#include "plot/QImWaveformGenerator.hpp"
#include "plot/QImPlotValueTrackerNode.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/**
 * \if ENGLISH
 * @brief Constructor for StemsFunction
 * @param parent Parent object
 * @details Initializes the test function and registers properties for interactive editing.
 * Properties are organized in categories:
 * - "Plot"/"Basic Info"/"Title" - Plot title
 * - "Plot"/"Axis"/"X Label" - X axis label
 * - "Plot"/"Axis"/"Y Label" - Y axis label
 * - "Stems"/"Style"/"Color" - Stems color
 * - "Stems"/"Style"/"Label" - Stems label
 * - "Stems"/"Data"/"Reference Value" - Reference value (baseline)
 * - "Stems"/"Orientation"/"Horizontal" - Horizontal orientation
 * \endif
 * 
 * \if CHINESE
 * @brief StemsFunction 构造函数
 * @param parent 父对象
 * @details 初始化测试函数并注册属性用于交互式编辑。
 * 属性按类别组织：
 * - "Plot"/"Basic Info"/"Title" - 图表标题
 * - "Plot"/"Axis"/"X Label" - X 轴标签
 * - "Plot"/"Axis"/"Y Label" - Y 轴标签
 * - "Stems"/"Style"/"Color" - 茎线颜色
 * - "Stems"/"Style"/"Label" - 茎线标签
 * - "Stems"/"Data"/"Reference Value" - 参考值（基线）
 * - "Stems"/"Orientation"/"Horizontal" - 水平方向
 * \endif
 */
StemsFunction::StemsFunction(QObject* parent)
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
    
    // Register stems color property
    PropertyRegistration colorReg;
    colorReg.category = tr("Stems");
    colorReg.subcategory = tr("Style");
    colorReg.displayName = tr("Color");
    colorReg.briefDesc = tr("Stems color");
    colorReg.detailDesc = tr("Sets the color of the stems");
    colorReg.editorType = EditorType::ColorPicker;
    colorReg.defaultValue = m_stemsColor;
    colorReg.propertyName = "stemsColor";
    colorReg.target = this;
    registerProperty(colorReg);
    
    // Register stems label property
    PropertyRegistration labelReg;
    labelReg.category = tr("Stems");
    labelReg.subcategory = tr("Style");
    labelReg.displayName = tr("Label");
    labelReg.briefDesc = tr("Stems label");
    labelReg.detailDesc = tr("Sets the label text for the stems (shown in legend)");
    labelReg.editorType = EditorType::LineEdit;
    labelReg.defaultValue = m_stemsLabel;
    labelReg.propertyName = "stemsLabel";
    labelReg.target = this;
    registerProperty(labelReg);
    
    // Register reference value property
    PropertyRegistration refValueReg;
    refValueReg.category = tr("Stems");
    refValueReg.subcategory = tr("Data");
    refValueReg.displayName = tr("Reference Value");
    refValueReg.briefDesc = tr("Reference value (baseline)");
    refValueReg.detailDesc = tr("Sets the baseline value from which stems are drawn");
    refValueReg.editorType = EditorType::DoubleSpinBox;
    refValueReg.defaultValue = m_referenceValue;
    refValueReg.minValue = -100.0;
    refValueReg.maxValue = 100.0;
    refValueReg.stepValue = 0.1;
    refValueReg.propertyName = "referenceValue";
    refValueReg.target = this;
    registerProperty(refValueReg);
    
    // Register horizontal orientation property
    PropertyRegistration horizontalReg;
    horizontalReg.category = tr("Stems");
    horizontalReg.subcategory = tr("Orientation");
    horizontalReg.displayName = tr("Horizontal");
    horizontalReg.briefDesc = tr("Horizontal orientation");
    horizontalReg.detailDesc = tr("When enabled, stems are rendered horizontally");
    horizontalReg.editorType = EditorType::CheckBox;
    horizontalReg.defaultValue = m_horizontal;
    horizontalReg.propertyName = "horizontal";
    horizontalReg.target = this;
    registerProperty(horizontalReg);
}

/**
 * \if ENGLISH
 * @brief Destructor for StemsFunction
 * \endif
 * 
 * \if CHINESE
 * @brief StemsFunction 析构函数
 * \endif
 */
StemsFunction::~StemsFunction()
{
}

/**
 * \if ENGLISH
 * @brief Create plot nodes in the given figure widget
 * @param figure Pointer to the figure widget where plots will be created
 * @details Creates a plot with 20 stem points, configures axes,
 * adds a stems item, and attaches a value tracker node.
 * \endif
 * 
 * \if CHINESE
 * @brief 在指定的图表控件中创建绘图节点
 * @param figure 将要创建绘图的图表控件指针
 * @details 创建一个包含 20 个茎点的图表，配置坐标轴，
 * 添加茎叶图项，并附加一个值追踪器节点。
 * \endif
 */
void StemsFunction::createPlot(QIM::QImFigureWidget* figure)
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
    
    // Generate 20 points data - decaying sine wave
    const int numPoints = 20;
    std::vector<double> xData(numPoints);
    std::vector<double> yData(numPoints);
    
    for (int i = 0; i < numPoints; ++i) {
        xData[i] = i;
        yData[i] = std::sin(i * 0.5) * std::exp(-i * 0.1) * 10.0;
    }
    
    // Create stems item node and set data
    m_stemsNode = new QIM::QImPlotStemsItemNode(m_plotNode);
    m_stemsNode->setLabel(m_stemsLabel);
    m_stemsNode->setData(xData, yData);
    m_stemsNode->setReferenceValue(m_referenceValue);
    m_stemsNode->setHorizontal(m_horizontal);
    m_stemsNode->setColor(m_stemsColor);
    
    // Create and attach value tracker
    m_trackerNode = new QIM::QImPlotValueTrackerNode(m_plotNode);
    m_trackerNode->setGroup(nullptr);  // No group by default
    m_plotNode->addChildNode(m_trackerNode);
}

void StemsFunction::setTitle(const QString& title)
{
    if (m_title != title) {
        m_title = title;
        emit titleChanged(title);
        if (m_plotNode) {
            m_plotNode->setTitle(title);
        }
    }
}

void StemsFunction::setXLabel(const QString& label)
{
    if (m_xLabel != label) {
        m_xLabel = label;
        emit xLabelChanged(label);
        if (m_plotNode) {
            m_plotNode->x1Axis()->setLabel(label);
        }
    }
}

void StemsFunction::setYLabel(const QString& label)
{
    if (m_yLabel != label) {
        m_yLabel = label;
        emit yLabelChanged(label);
        if (m_plotNode) {
            m_plotNode->y1Axis()->setLabel(label);
        }
    }
}

void StemsFunction::setStemsColor(const QColor& color)
{
    if (m_stemsColor != color) {
        m_stemsColor = color;
        emit stemsColorChanged(color);
        if (m_stemsNode) {
            m_stemsNode->setColor(color);
        }
    }
}

void StemsFunction::setStemsLabel(const QString& label)
{
    if (m_stemsLabel != label) {
        m_stemsLabel = label;
        emit stemsLabelChanged(label);
        if (m_stemsNode) {
            m_stemsNode->setLabel(label);
        }
    }
}

void StemsFunction::setReferenceValue(double value)
{
    if (m_referenceValue != value) {
        m_referenceValue = value;
        emit referenceValueChanged(value);
        if (m_stemsNode) {
            m_stemsNode->setReferenceValue(value);
        }
    }
}

void StemsFunction::setHorizontal(bool horizontal)
{
    if (m_horizontal != horizontal) {
        m_horizontal = horizontal;
        emit horizontalChanged(horizontal);
        if (m_stemsNode) {
            m_stemsNode->setHorizontal(horizontal);
        }
    }
}
