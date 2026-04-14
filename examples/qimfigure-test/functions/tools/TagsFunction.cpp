#include "TagsFunction.h"
#include "QImFigureWidget.h"
#include "plot/QImPlotNode.h"
#include "plot/QImPlotAxisInfo.h"
#include "plot/QImPlotLineItemNode.h"
#include "plot/QImPlotTagXNode.h"
#include "plot/QImPlotTagYNode.h"
#include <vector>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/**
 * \if ENGLISH
 * @brief Constructor for TagsFunction
 * @param parent Parent object
 * @details Initializes the test function and registers properties for interactive editing.
 * Properties are organized in categories:
 * - "Plot"/"Basic Info"/"Title" - Plot title
 * - "TagX"/"Position"/"Value" - X-axis tag position
 * - "TagX"/"Content"/"Text" - X-axis tag text
 * - "TagX"/"Style"/"Color" - X-axis tag color
 * - "TagY"/"Position"/"Value" - Y-axis tag position
 * - "TagY"/"Content"/"Text" - Y-axis tag text
 * - "TagY"/"Style"/"Color" - Y-axis tag color
 * \endif
 * 
 * \if CHINESE
 * @brief TagsFunction 构造函数
 * @param parent 父对象
 * @details 初始化测试函数并注册属性用于交互式编辑。
 * 属性按类别组织：
 * - "Plot"/"Basic Info"/"Title" - 图表标题
 * - "TagX"/"Position"/"Value" - X轴标签位置
 * - "TagX"/"Content"/"Text" - X轴标签文本
 * - "TagX"/"Style"/"Color" - X轴标签颜色
 * - "TagY"/"Position"/"Value" - Y轴标签位置
 * - "TagY"/"Content"/"Text" - Y轴标签文本
 * - "TagY"/"Style"/"Color" - Y轴标签颜色
 * \endif
 */
TagsFunction::TagsFunction(QObject* parent)
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
    
    // Register TagX value property
    PropertyRegistration tagXValueReg;
    tagXValueReg.category = tr("TagX");
    tagXValueReg.subcategory = tr("Position");
    tagXValueReg.displayName = tr("Value");
    tagXValueReg.briefDesc = tr("X position");
    tagXValueReg.detailDesc = tr("Sets the X coordinate where the tag appears");
    tagXValueReg.editorType = EditorType::DoubleSpinBox;
    tagXValueReg.defaultValue = m_tagXValue;
    tagXValueReg.minValue = 0.0;
    tagXValueReg.maxValue = 10.0;
    tagXValueReg.stepValue = 0.1;
    tagXValueReg.propertyName = "tagXValue";
    tagXValueReg.target = this;
    registerProperty(tagXValueReg);
    
    // Register TagX text property
    PropertyRegistration tagXTextReg;
    tagXTextReg.category = tr("TagX");
    tagXTextReg.subcategory = tr("Content");
    tagXTextReg.displayName = tr("Text");
    tagXTextReg.briefDesc = tr("Tag label text");
    tagXTextReg.detailDesc = tr("Sets the text label displayed next to the tag");
    tagXTextReg.editorType = EditorType::LineEdit;
    tagXTextReg.defaultValue = m_tagXText;
    tagXTextReg.propertyName = "tagXText";
    tagXTextReg.target = this;
    registerProperty(tagXTextReg);
    
    // Register TagX color property
    PropertyRegistration tagXColorReg;
    tagXColorReg.category = tr("TagX");
    tagXColorReg.subcategory = tr("Style");
    tagXColorReg.displayName = tr("Color");
    tagXColorReg.briefDesc = tr("Tag line color");
    tagXColorReg.detailDesc = tr("Sets the color of the vertical tag line");
    tagXColorReg.editorType = EditorType::ColorPicker;
    tagXColorReg.defaultValue = m_tagXColor;
    tagXColorReg.propertyName = "tagXColor";
    tagXColorReg.target = this;
    registerProperty(tagXColorReg);
    
    // Register TagY value property
    PropertyRegistration tagYValueReg;
    tagYValueReg.category = tr("TagY");
    tagYValueReg.subcategory = tr("Position");
    tagYValueReg.displayName = tr("Value");
    tagYValueReg.briefDesc = tr("Y position");
    tagYValueReg.detailDesc = tr("Sets the Y coordinate where the tag appears");
    tagYValueReg.editorType = EditorType::DoubleSpinBox;
    tagYValueReg.defaultValue = m_tagYValue;
    tagYValueReg.minValue = 0.0;
    tagYValueReg.maxValue = 10.0;
    tagYValueReg.stepValue = 0.1;
    tagYValueReg.propertyName = "tagYValue";
    tagYValueReg.target = this;
    registerProperty(tagYValueReg);
    
    // Register TagY text property
    PropertyRegistration tagYTextReg;
    tagYTextReg.category = tr("TagY");
    tagYTextReg.subcategory = tr("Content");
    tagYTextReg.displayName = tr("Text");
    tagYTextReg.briefDesc = tr("Tag label text");
    tagYTextReg.detailDesc = tr("Sets the text label displayed next to the tag");
    tagYTextReg.editorType = EditorType::LineEdit;
    tagYTextReg.defaultValue = m_tagYText;
    tagYTextReg.propertyName = "tagYText";
    tagYTextReg.target = this;
    registerProperty(tagYTextReg);
    
    // Register TagY color property
    PropertyRegistration tagYColorReg;
    tagYColorReg.category = tr("TagY");
    tagYColorReg.subcategory = tr("Style");
    tagYColorReg.displayName = tr("Color");
    tagYColorReg.briefDesc = tr("Tag line color");
    tagYColorReg.detailDesc = tr("Sets the color of the horizontal tag line");
    tagYColorReg.editorType = EditorType::ColorPicker;
    tagYColorReg.defaultValue = m_tagYColor;
    tagYColorReg.propertyName = "tagYColor";
    tagYColorReg.target = this;
    registerProperty(tagYColorReg);
}

/**
 * \if ENGLISH
 * @brief Destructor for TagsFunction
 * \endif
 * 
 * \if CHINESE
 * @brief TagsFunction 析构函数
 * \endif
 */
TagsFunction::~TagsFunction()
{
}

/**
 * \if ENGLISH
 * @brief Create plot nodes in the given figure widget
 * @param figure Pointer to the figure widget where plots will be created
 * @details Creates a plot with a sine curve, an X-axis tag, and a Y-axis tag.
 * Tags mark specific values on the axes with labels.
 * \endif
 * 
 * \if CHINESE
 * @brief 在指定的图表控件中创建绘图节点
 * @param figure 将要创建绘图的图表控件指针
 * @details 创建一个包含正弦曲线、X轴标签和Y轴标签的图表。
 * 标签在轴上标记特定值并显示标签文本。
 * \endif
 */
void TagsFunction::createPlot(QIM::QImFigureWidget* figure)
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
    m_plotNode->x1Axis()->setLabel(tr("x"));
    m_plotNode->y1Axis()->setLabel(tr("y"));
    m_plotNode->setTitle(m_title);
    
    // Generate sine curve
    const int numPoints = 100;
    std::vector<double> xData(numPoints);
    std::vector<double> yData(numPoints);
    
    for (int i = 0; i < numPoints; ++i) {
        xData[i] = i * 0.1;
        yData[i] = std::sin(xData[i] * M_PI) * 3.0 + 5.0;
    }
    
    // Create line item node
    m_lineNode = new QIM::QImPlotLineItemNode(m_plotNode);
    m_lineNode->setData(xData, yData);
    m_lineNode->setColor(QColor(100, 150, 255));
    m_plotNode->addChildNode(m_lineNode);
    
    // Create X-axis tag
    m_tagXNode = new QIM::QImPlotTagXNode(m_plotNode);
    m_tagXNode->setValue(m_tagXValue);
    m_tagXNode->setText(m_tagXText);
    m_tagXNode->setColor(m_tagXColor);
    m_plotNode->addChildNode(m_tagXNode);
    
    // Create Y-axis tag
    m_tagYNode = new QIM::QImPlotTagYNode(m_plotNode);
    m_tagYNode->setValue(m_tagYValue);
    m_tagYNode->setText(m_tagYText);
    m_tagYNode->setColor(m_tagYColor);
    m_plotNode->addChildNode(m_tagYNode);
}

void TagsFunction::setTitle(const QString& title)
{
    if (m_title != title) {
        m_title = title;
        emit titleChanged(title);
        if (m_plotNode) {
            m_plotNode->setTitle(title);
        }
    }
}

void TagsFunction::setTagXValue(double value)
{
    if (m_tagXValue != value) {
        m_tagXValue = value;
        emit tagXValueChanged(value);
        if (m_tagXNode) {
            m_tagXNode->setValue(value);
        }
    }
}

void TagsFunction::setTagXText(const QString& text)
{
    if (m_tagXText != text) {
        m_tagXText = text;
        emit tagXTextChanged(text);
        if (m_tagXNode) {
            m_tagXNode->setText(text);
        }
    }
}

void TagsFunction::setTagXColor(const QColor& color)
{
    if (m_tagXColor != color) {
        m_tagXColor = color;
        emit tagXColorChanged(color);
        if (m_tagXNode) {
            m_tagXNode->setColor(color);
        }
    }
}

void TagsFunction::setTagYValue(double value)
{
    if (m_tagYValue != value) {
        m_tagYValue = value;
        emit tagYValueChanged(value);
        if (m_tagYNode) {
            m_tagYNode->setValue(value);
        }
    }
}

void TagsFunction::setTagYText(const QString& text)
{
    if (m_tagYText != text) {
        m_tagYText = text;
        emit tagYTextChanged(text);
        if (m_tagYNode) {
            m_tagYNode->setText(text);
        }
    }
}

void TagsFunction::setTagYColor(const QColor& color)
{
    if (m_tagYColor != color) {
        m_tagYColor = color;
        emit tagYColorChanged(color);
        if (m_tagYNode) {
            m_tagYNode->setColor(color);
        }
    }
}