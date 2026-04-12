#include "InfLinesFunction.h"
#include "QImFigureWidget.h"
#include "plot/QImPlotNode.h"
#include "plot/QImPlotLineItemNode.h"
#include "plot/QImPlotInfLinesItemNode.h"
#include "plot/QImPlotValueTrackerNode.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/**
 * \if ENGLISH
 * @brief Constructor for InfLinesFunction
 * @param parent Parent object
 * @details Initializes the test function and registers properties for interactive editing.
 * Properties are organized in categories:
 * - "Plot"/"Basic Info"/"Title" - Plot title
 * - "Plot"/"Axis"/"X Label" - X axis label
 * - "Plot"/"Axis"/"Y Label" - Y axis label
 * - "Line"/"Style"/"Color" - Background line color
 * - "InfLines"/"Vertical"/"Color" - Vertical infinite lines color
 * - "InfLines"/"Vertical"/"Values" - Vertical infinite lines values
 * - "InfLines"/"Horizontal"/"Color" - Horizontal infinite lines color
 * - "InfLines"/"Horizontal"/"Values" - Horizontal infinite lines values
 * \endif
 * 
 * \if CHINESE
 * @brief InfLinesFunction 构造函数
 * @param parent 父对象
 * @details 初始化测试函数并注册属性用于交互式编辑。
 * 属性按类别组织：
 * - "Plot"/"Basic Info"/"Title" - 图表标题
 * - "Plot"/"Axis"/"X Label" - X 轴标签
 * - "Plot"/"Axis"/"Y Label" - Y 轴标签
 * - "Line"/"Style"/"Color" - 背景线颜色
 * - "InfLines"/"Vertical"/"Color" - 垂直无限线颜色
 * - "InfLines"/"Vertical"/"Values" - 垂直无限线值
 * - "InfLines"/"Horizontal"/"Color" - 水平无限线颜色
 * - "InfLines"/"Horizontal"/"Values" - 水平无限线值
 * \endif
 */
InfLinesFunction::InfLinesFunction(QObject* parent)
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
    
    // Register vertical infinite lines color property
    PropertyRegistration verticalColorReg;
    verticalColorReg.category = tr("InfLines");
    verticalColorReg.subcategory = tr("Vertical");
    verticalColorReg.displayName = tr("Color");
    verticalColorReg.briefDesc = tr("Vertical infinite lines color");
    verticalColorReg.detailDesc = tr("Sets the color of the vertical infinite reference lines");
    verticalColorReg.editorType = EditorType::ColorPicker;
    verticalColorReg.defaultValue = m_verticalInfLinesColor;
    verticalColorReg.propertyName = "verticalInfLinesColor";
    verticalColorReg.target = this;
    registerProperty(verticalColorReg);
    
    // Register vertical values property
    PropertyRegistration verticalValuesReg;
    verticalValuesReg.category = tr("InfLines");
    verticalValuesReg.subcategory = tr("Vertical");
    verticalValuesReg.displayName = tr("Values");
    verticalValuesReg.briefDesc = tr("Vertical infinite lines values");
    verticalValuesReg.detailDesc = tr("Sets the X-axis positions of vertical infinite lines (comma-separated)");
    verticalValuesReg.editorType = EditorType::LineEdit;
    verticalValuesReg.defaultValue = QString();  // Will be handled specially
    verticalValuesReg.propertyName = "verticalValues";
    verticalValuesReg.target = this;
    registerProperty(verticalValuesReg);
    
    // Register horizontal infinite lines color property
    PropertyRegistration horizontalColorReg;
    horizontalColorReg.category = tr("InfLines");
    horizontalColorReg.subcategory = tr("Horizontal");
    horizontalColorReg.displayName = tr("Color");
    horizontalColorReg.briefDesc = tr("Horizontal infinite lines color");
    horizontalColorReg.detailDesc = tr("Sets the color of the horizontal infinite reference lines");
    horizontalColorReg.editorType = EditorType::ColorPicker;
    horizontalColorReg.defaultValue = m_horizontalInfLinesColor;
    horizontalColorReg.propertyName = "horizontalInfLinesColor";
    horizontalColorReg.target = this;
    registerProperty(horizontalColorReg);
    
    // Register horizontal values property
    PropertyRegistration horizontalValuesReg;
    horizontalValuesReg.category = tr("InfLines");
    horizontalValuesReg.subcategory = tr("Horizontal");
    horizontalValuesReg.displayName = tr("Values");
    horizontalValuesReg.briefDesc = tr("Horizontal infinite lines values");
    horizontalValuesReg.detailDesc = tr("Sets the Y-axis positions of horizontal infinite lines (comma-separated)");
    horizontalValuesReg.editorType = EditorType::LineEdit;
    horizontalValuesReg.defaultValue = QString();  // Will be handled specially
    horizontalValuesReg.propertyName = "horizontalValues";
    horizontalValuesReg.target = this;
    registerProperty(horizontalValuesReg);
}

/**
 * \if ENGLISH
 * @brief Destructor for InfLinesFunction
 * \endif
 * 
 * \if CHINESE
 * @brief InfLinesFunction 析构函数
 * \endif
 */
InfLinesFunction::~InfLinesFunction()
{
}

/**
 * \if ENGLISH
 * @brief Create plot nodes in the given figure widget
 * @param figure Pointer to the figure widget where plots will be created
 * @details Creates a plot with 200-point sine wave as background,
 * adds vertical and horizontal infinite reference lines, and attaches a value tracker node.
 * \endif
 * 
 * \if CHINESE
 * @brief 在指定的图表控件中创建绘图节点
 * @param figure 将要创建绘图的图表控件指针
 * @details 创建一个包含 200 点正弦波作为背景的图表，
 * 添加垂直和水平无限参考线，并附加一个值追踪器节点。
 * \endif
 */
void InfLinesFunction::createPlot(QIM::QImFigureWidget* figure)
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
    
    // Create vertical infinite lines
    m_verticalInfLinesNode = new QIM::QImPlotInfLinesItemNode(m_plotNode);
    m_verticalInfLinesNode->setLabel("Vertical Ref");
    std::vector<double> verticalValuesStd;
    for (double val : m_verticalValues) {
        verticalValuesStd.push_back(val);
    }
    m_verticalInfLinesNode->setValues(verticalValuesStd);
    m_verticalInfLinesNode->setHorizontal(false);
    m_verticalInfLinesNode->setColor(m_verticalInfLinesColor);
    
    // Create horizontal infinite lines
    m_horizontalInfLinesNode = new QIM::QImPlotInfLinesItemNode(m_plotNode);
    m_horizontalInfLinesNode->setLabel("Horizontal Ref");
    std::vector<double> horizontalValuesStd;
    for (double val : m_horizontalValues) {
        horizontalValuesStd.push_back(val);
    }
    m_horizontalInfLinesNode->setValues(horizontalValuesStd);
    m_horizontalInfLinesNode->setHorizontal(true);
    m_horizontalInfLinesNode->setColor(m_horizontalInfLinesColor);
    
    // Create and attach value tracker
    m_trackerNode = new QIM::QImPlotValueTrackerNode(m_plotNode);
    m_trackerNode->setGroup(nullptr);  // No group by default
    m_plotNode->addChildNode(m_trackerNode);
}

void InfLinesFunction::setTitle(const QString& title)
{
    if (m_title != title) {
        m_title = title;
        emit titleChanged(title);
        if (m_plotNode) {
            m_plotNode->setTitle(title);
        }
    }
}

void InfLinesFunction::setXLabel(const QString& label)
{
    if (m_xLabel != label) {
        m_xLabel = label;
        emit xLabelChanged(label);
        if (m_plotNode) {
            m_plotNode->x1Axis()->setLabel(label);
        }
    }
}

void InfLinesFunction::setYLabel(const QString& label)
{
    if (m_yLabel != label) {
        m_yLabel = label;
        emit yLabelChanged(label);
        if (m_plotNode) {
            m_plotNode->y1Axis()->setLabel(label);
        }
    }
}

void InfLinesFunction::setLineColor(const QColor& color)
{
    if (m_lineColor != color) {
        m_lineColor = color;
        emit lineColorChanged(color);
        if (m_lineNode) {
            m_lineNode->setColor(color);
        }
    }
}

void InfLinesFunction::setVerticalInfLinesColor(const QColor& color)
{
    if (m_verticalInfLinesColor != color) {
        m_verticalInfLinesColor = color;
        emit verticalInfLinesColorChanged(color);
        if (m_verticalInfLinesNode) {
            m_verticalInfLinesNode->setColor(color);
        }
    }
}

void InfLinesFunction::setHorizontalInfLinesColor(const QColor& color)
{
    if (m_horizontalInfLinesColor != color) {
        m_horizontalInfLinesColor = color;
        emit horizontalInfLinesColorChanged(color);
        if (m_horizontalInfLinesNode) {
            m_horizontalInfLinesNode->setColor(color);
        }
    }
}

void InfLinesFunction::setVerticalValues(const QVector<double>& values)
{
    if (m_verticalValues != values) {
        m_verticalValues = values;
        emit verticalValuesChanged(values);
        if (m_verticalInfLinesNode) {
            std::vector<double> valuesStd;
            for (double val : values) {
                valuesStd.push_back(val);
            }
            m_verticalInfLinesNode->setValues(valuesStd);
        }
    }
}

void InfLinesFunction::setHorizontalValues(const QVector<double>& values)
{
    if (m_horizontalValues != values) {
        m_horizontalValues = values;
        emit horizontalValuesChanged(values);
        if (m_horizontalInfLinesNode) {
            std::vector<double> valuesStd;
            for (double val : values) {
                valuesStd.push_back(val);
            }
            m_horizontalInfLinesNode->setValues(valuesStd);
        }
    }
}
