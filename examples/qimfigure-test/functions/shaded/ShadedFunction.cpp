#include "ShadedFunction.h"
#include "QImFigureWidget.h"
#include "plot/QImPlotNode.h"
#include "plot/QImPlotAxisInfo.h"
#include "plot/QImPlotShadedItemNode.h"
#include "plot/QImPlotValueTrackerNode.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/**
 * \if ENGLISH
 * @brief Constructor for ShadedFunction
 * @param parent Parent object
 * @details Initializes the test function and registers properties for interactive editing.
 * Properties are organized in categories:
 * - "Plot"/"Basic Info"/"Title" - Plot title
 * - "Plot"/"Axis"/"X Label" - X axis label
 * - "Plot"/"Axis"/"Y Label" - Y axis label
 * - "Shaded"/"Style"/"Color" - Fill color
 * - "Shaded"/"Style"/"Reference Value" - Reference value for single-line mode
 * - "Shaded"/"Mode"/"Two-Line Mode" - Toggle between single-line and double-line mode
 * \endif
 * 
 * \if CHINESE
 * @brief ShadedFunction 构造函数
 * @param parent 父对象
 * @details 初始化测试函数并注册属性用于交互式编辑。
 * 属性按类别组织：
 * - "Plot"/"Basic Info"/"Title" - 图表标题
 * - "Plot"/"Axis"/"X Label" - X 轴标签
 * - "Plot"/"Axis"/"Y Label" - Y 轴标签
 * - "Shaded"/"Style"/"Color" - 填充颜色
 * - "Shaded"/"Style"/"Reference Value" - 单线模式的参考值
 * - "Shaded"/"Mode"/"Two-Line Mode" - 单线和双线模式之间的切换
 * \endif
 */
ShadedFunction::ShadedFunction(QObject* parent)
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
    
    // Register fill color property
    PropertyRegistration colorReg;
    colorReg.category = tr("Shaded");
    colorReg.subcategory = tr("Style");
    colorReg.displayName = tr("Color");
    colorReg.briefDesc = tr("Fill color");
    colorReg.detailDesc = tr("Sets the color of the shaded/filled region");
    colorReg.editorType = EditorType::ColorPicker;
    colorReg.defaultValue = m_fillColor;
    colorReg.propertyName = "fillColor";
    colorReg.target = this;
    registerProperty(colorReg);
    
    // Register reference value property
    PropertyRegistration refValueReg;
    refValueReg.category = tr("Shaded");
    refValueReg.subcategory = tr("Style");
    refValueReg.displayName = tr("Reference Value");
    refValueReg.briefDesc = tr("Reference value for single-line mode");
    refValueReg.detailDesc = tr("Sets the reference value (Y-axis) to which the fill extends in single-line mode");
    refValueReg.editorType = EditorType::DoubleSpinBox;
    refValueReg.defaultValue = m_referenceValue;
    refValueReg.minValue = -100.0;
    refValueReg.maxValue = 100.0;
    refValueReg.stepValue = 0.1;
    refValueReg.propertyName = "referenceValue";
    refValueReg.target = this;
    registerProperty(refValueReg);
    
    // Register two-line mode property
    PropertyRegistration modeReg;
    modeReg.category = tr("Shaded");
    modeReg.subcategory = tr("Mode");
    modeReg.displayName = tr("Two-Line Mode");
    modeReg.briefDesc = tr("Enable two-line fill mode");
    modeReg.detailDesc = tr("When enabled, fills between two lines (upper and lower bounds). When disabled, fills to reference value.");
    modeReg.editorType = EditorType::CheckBox;
    modeReg.defaultValue = m_twoLineMode;
    modeReg.propertyName = "twoLineMode";
    modeReg.target = this;
    registerProperty(modeReg);
}

/**
 * \if ENGLISH
 * @brief Destructor for ShadedFunction
 * \endif
 * 
 * \if CHINESE
 * @brief ShadedFunction 析构函数
 * \endif
 */
ShadedFunction::~ShadedFunction()
{
}

/**
 * \if ENGLISH
 * @brief Create plot nodes in the given figure widget
 * @param figure Pointer to the figure widget where plots will be created
 * @details Creates a shaded plot with 100 sine wave points.
 *          Supports both single-line mode (fill to reference value)
 *          and double-line mode (fill between two lines).
 * \endif
 * 
 * \if CHINESE
 * @brief 在指定的图表控件中创建绘图节点
 * @param figure 将要创建绘图的图表控件指针
 * @details 创建一个包含 100 个正弦波点的填充图。
 *          支持单线模式（填充到参考值）和双线模式（两条线之间填充）。
 * \endif
 */
void ShadedFunction::createPlot(QIM::QImFigureWidget* figure)
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
    
    // Generate 100 sine wave data
    const int numPoints = 100;
    std::vector<double> xData(numPoints);
    std::vector<double> yData(numPoints);
    
    for (int i = 0; i < numPoints; ++i) {
        xData[i] = i * 0.1;
        yData[i] = std::sin(xData[i]) * 5.0 + 5.0;  // Sine wave, range 0-10
    }
    
    // Create shaded item node
    m_shadedNode = new QIM::QImPlotShadedItemNode(m_plotNode);
    m_shadedNode->setLabel("Shaded Area");
    
    if (m_twoLineMode) {
        // Two-line mode: fill between upper and lower bounds
        std::vector<double> yUpper(numPoints);
        std::vector<double> yLower(numPoints);
        for (int i = 0; i < numPoints; ++i) {
            yUpper[i] = yData[i] + 2.0;  // Upper bound
            yLower[i] = yData[i] - 2.0;  // Lower bound
        }
        
        QIM::QImAbstractXYDataSeries* lowerSeries = new QIM::QImVectorXYDataSeries(xData, yLower);
        QIM::QImAbstractXYDataSeries* upperSeries = new QIM::QImVectorXYDataSeries(xData, yUpper);
        m_shadedNode->setData(lowerSeries, upperSeries);
    } else {
        // Single-line mode: fill to reference value
        m_shadedNode->setData(xData, yData);
        m_shadedNode->setReferenceValue(m_referenceValue);
    }
    
    m_shadedNode->setColor(m_fillColor);
    m_plotNode->addChildNode(m_shadedNode);
    
    // Create and attach value tracker
    m_trackerNode = new QIM::QImPlotValueTrackerNode(m_plotNode);
    m_trackerNode->setGroup(nullptr);  // No group by default
    m_plotNode->addChildNode(m_trackerNode);
}

void ShadedFunction::setTitle(const QString& title)
{
    if (m_title != title) {
        m_title = title;
        emit titleChanged(title);
        if (m_plotNode) {
            m_plotNode->setTitle(title);
        }
    }
}

void ShadedFunction::setXLabel(const QString& label)
{
    if (m_xLabel != label) {
        m_xLabel = label;
        emit xLabelChanged(label);
        if (m_plotNode) {
            m_plotNode->x1Axis()->setLabel(label);
        }
    }
}

void ShadedFunction::setYLabel(const QString& label)
{
    if (m_yLabel != label) {
        m_yLabel = label;
        emit yLabelChanged(label);
        if (m_plotNode) {
            m_plotNode->y1Axis()->setLabel(label);
        }
    }
}

void ShadedFunction::setFillColor(const QColor& color)
{
    if (m_fillColor != color) {
        m_fillColor = color;
        emit fillColorChanged(color);
        if (m_shadedNode) {
            m_shadedNode->setColor(color);
        }
    }
}

void ShadedFunction::setReferenceValue(double value)
{
    if (m_referenceValue != value) {
        m_referenceValue = value;
        emit referenceValueChanged(value);
        if (m_shadedNode && !m_twoLineMode) {
            m_shadedNode->setReferenceValue(value);
        }
    }
}

void ShadedFunction::setTwoLineMode(bool enabled)
{
    if (m_twoLineMode != enabled) {
        m_twoLineMode = enabled;
        emit twoLineModeChanged(enabled);
        // Note: To apply the mode change, createPlot should be called again
        // This is a placeholder for future implementation
    }
}
