#include "TextFunction.h"
#include "QImFigureWidget.h"
#include "plot/QImPlotNode.h"
#include "plot/QImPlotAxisInfo.h"
#include "plot/QImPlotTextItemNode.h"

/**
 * \if ENGLISH
 * @brief Constructor for TextFunction
 * @param parent Parent object
 * @details Initializes the test function and registers properties for interactive editing.
 * Properties are organized in categories:
 * - "Plot"/"Basic Info"/"Title" - Plot title
 * - "Plot"/"Axis"/"X Label" - X axis label
 * - "Plot"/"Axis"/"Y Label" - Y axis label
 * - "Text"/"Content"/"Text" - Text content
 * - "Text"/"Position"/"X" - Text X position
 * - "Text"/"Position"/"Y" - Text Y position
 * - "Text"/"Offset"/"Pixel X" - Pixel offset X
 * - "Text"/"Offset"/"Pixel Y" - Pixel offset Y
 * - "Text"/"Style"/"Color" - Text color
 * - "Text"/"Style"/"Vertical" - Vertical orientation
 * \endif
 * 
 * \if CHINESE
 * @brief TextFunction 构造函数
 * @param parent 父对象
 * @details 初始化测试函数并注册属性用于交互式编辑。
 * 属性按类别组织：
 * - "Plot"/"Basic Info"/"Title" - 图表标题
 * - "Plot"/"Axis"/"X Label" - X 轴标签
 * - "Plot"/"Axis"/"Y Label" - Y 轴标签
 * - "Text"/"Content"/"Text" - 文本内容
 * - "Text"/"Position"/"X" - 文本 X 位置
 * - "Text"/"Position"/"Y" - 文本 Y 位置
 * - "Text"/"Offset"/"Pixel X" - 像素偏移 X
 * - "Text"/"Offset"/"Pixel Y" - 像素偏移 Y
 * - "Text"/"Style"/"Color" - 文本颜色
 * - "Text"/"Style"/"Vertical" - 垂直方向
 * \endif
 */
TextFunction::TextFunction(QObject* parent)
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
    
    // Register text content property
    PropertyRegistration textReg;
    textReg.category = tr("Text");
    textReg.subcategory = tr("Content");
    textReg.displayName = tr("Text");
    textReg.briefDesc = tr("Text content");
    textReg.detailDesc = tr("Sets the text content to display at the specified position");
    textReg.editorType = EditorType::LineEdit;
    textReg.defaultValue = m_text;
    textReg.propertyName = "text";
    textReg.target = this;
    registerProperty(textReg);
    
    // Register text X position property
    PropertyRegistration textXReg;
    textXReg.category = tr("Text");
    textXReg.subcategory = tr("Position");
    textXReg.displayName = tr("X");
    textXReg.briefDesc = tr("Text X position");
    textXReg.detailDesc = tr("Sets the X coordinate in plot space where the text will be rendered");
    textXReg.editorType = EditorType::DoubleSpinBox;
    textXReg.defaultValue = m_textX;
    textXReg.propertyName = "textX";
    textXReg.target = this;
    registerProperty(textXReg);
    
    // Register text Y position property
    PropertyRegistration textYReg;
    textYReg.category = tr("Text");
    textYReg.subcategory = tr("Position");
    textYReg.displayName = tr("Y");
    textYReg.briefDesc = tr("Text Y position");
    textYReg.detailDesc = tr("Sets the Y coordinate in plot space where the text will be rendered");
    textYReg.editorType = EditorType::DoubleSpinBox;
    textYReg.defaultValue = m_textY;
    textYReg.propertyName = "textY";
    textYReg.target = this;
    registerProperty(textYReg);
    
    // Register pixel offset X property
    PropertyRegistration pixelOffsetXReg;
    pixelOffsetXReg.category = tr("Text");
    pixelOffsetXReg.subcategory = tr("Offset");
    pixelOffsetXReg.displayName = tr("Pixel X");
    pixelOffsetXReg.briefDesc = tr("Pixel offset X");
    pixelOffsetXReg.detailDesc = tr("Sets the horizontal pixel offset from the plot position");
    pixelOffsetXReg.editorType = EditorType::SpinBox;
    pixelOffsetXReg.defaultValue = static_cast<int>(m_pixelOffsetX);
    pixelOffsetXReg.propertyName = "pixelOffsetX";
    pixelOffsetXReg.target = this;
    registerProperty(pixelOffsetXReg);
    
    // Register pixel offset Y property
    PropertyRegistration pixelOffsetYReg;
    pixelOffsetYReg.category = tr("Text");
    pixelOffsetYReg.subcategory = tr("Offset");
    pixelOffsetYReg.displayName = tr("Pixel Y");
    pixelOffsetYReg.briefDesc = tr("Pixel offset Y");
    pixelOffsetYReg.detailDesc = tr("Sets the vertical pixel offset from the plot position");
    pixelOffsetYReg.editorType = EditorType::SpinBox;
    pixelOffsetYReg.defaultValue = static_cast<int>(m_pixelOffsetY);
    pixelOffsetYReg.propertyName = "pixelOffsetY";
    pixelOffsetYReg.target = this;
    registerProperty(pixelOffsetYReg);
    
    // Register text color property
    PropertyRegistration textColorReg;
    textColorReg.category = tr("Text");
    textColorReg.subcategory = tr("Style");
    textColorReg.displayName = tr("Color");
    textColorReg.briefDesc = tr("Text color");
    textColorReg.detailDesc = tr("Sets the color of the text label");
    textColorReg.editorType = EditorType::ColorPicker;
    textColorReg.defaultValue = m_textColor;
    textColorReg.propertyName = "textColor";
    textColorReg.target = this;
    registerProperty(textColorReg);
    
    // Register vertical orientation property
    PropertyRegistration verticalReg;
    verticalReg.category = tr("Text");
    verticalReg.subcategory = tr("Style");
    verticalReg.displayName = tr("Vertical");
    verticalReg.briefDesc = tr("Vertical orientation");
    verticalReg.detailDesc = tr("When enabled, text is rendered vertically (rotated 90 degrees)");
    verticalReg.editorType = EditorType::CheckBox;
    verticalReg.defaultValue = m_vertical;
    verticalReg.propertyName = "vertical";
    verticalReg.target = this;
    registerProperty(verticalReg);
}

/**
 * \if ENGLISH
 * @brief Destructor for TextFunction
 * \endif
 * 
 * \if CHINESE
 * @brief TextFunction 析构函数
 * \endif
 */
TextFunction::~TextFunction()
{
}

/**
 * \if ENGLISH
 * @brief Create plot nodes in the given figure widget
 * @param figure Pointer to the figure widget where plots will be created
 * @details Creates a plot with a text label at the specified position.
 * \endif
 * 
 * \if CHINESE
 * @brief 在指定的图表控件中创建绘图节点
 * @param figure 将要创建绘图的图表控件指针
 * @details 创建一个在指定位置带有文本标签的图表。
 * \endif
 */
void TextFunction::createPlot(QIM::QImFigureWidget* figure)
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
    m_plotNode->setLegendEnabled(false);
    
    // Set plot ranges for better visibility
    m_plotNode->x1Axis()->setLimits(-10.0, 10.0);
    m_plotNode->y1Axis()->setLimits(-10.0, 10.0);
    
    // Create text label item
    m_textNode = new QIM::QImPlotTextItemNode(m_plotNode);
    m_textNode->setLabel("Text Label");
    m_textNode->setText(m_text);
    m_textNode->setPosition(m_textX, m_textY);
    m_textNode->setPixelOffset(m_pixelOffsetX, m_pixelOffsetY);
    m_textNode->setVertical(m_vertical);
    m_textNode->setColor(m_textColor);
}

void TextFunction::setTitle(const QString& title)
{
    if (m_title != title) {
        m_title = title;
        emit titleChanged(title);
        if (m_plotNode) {
            m_plotNode->setTitle(title);
        }
    }
}

void TextFunction::setXLabel(const QString& label)
{
    if (m_xLabel != label) {
        m_xLabel = label;
        emit xLabelChanged(label);
        if (m_plotNode) {
            m_plotNode->x1Axis()->setLabel(label);
        }
    }
}

void TextFunction::setYLabel(const QString& label)
{
    if (m_yLabel != label) {
        m_yLabel = label;
        emit yLabelChanged(label);
        if (m_plotNode) {
            m_plotNode->y1Axis()->setLabel(label);
        }
    }
}

void TextFunction::setText(const QString& text)
{
    if (m_text != text) {
        m_text = text;
        emit textChanged(text);
        if (m_textNode) {
            m_textNode->setText(text);
        }
    }
}

void TextFunction::setTextX(double x)
{
    if (m_textX != x) {
        m_textX = x;
        emit textXChanged(x);
        if (m_textNode) {
            m_textNode->setPosition(x, m_textY);
        }
    }
}

void TextFunction::setTextY(double y)
{
    if (m_textY != y) {
        m_textY = y;
        emit textYChanged(y);
        if (m_textNode) {
            m_textNode->setPosition(m_textX, y);
        }
    }
}

void TextFunction::setPixelOffsetX(float x)
{
    if (m_pixelOffsetX != x) {
        m_pixelOffsetX = x;
        emit pixelOffsetXChanged(x);
        if (m_textNode) {
            m_textNode->setPixelOffset(x, m_pixelOffsetY);
        }
    }
}

void TextFunction::setPixelOffsetY(float y)
{
    if (m_pixelOffsetY != y) {
        m_pixelOffsetY = y;
        emit pixelOffsetYChanged(y);
        if (m_textNode) {
            m_textNode->setPixelOffset(m_pixelOffsetX, y);
        }
    }
}

void TextFunction::setTextColor(const QColor& color)
{
    if (m_textColor != color) {
        m_textColor = color;
        emit textColorChanged(color);
        if (m_textNode) {
            m_textNode->setColor(color);
        }
    }
}

void TextFunction::setVertical(bool vertical)
{
    if (m_vertical != vertical) {
        m_vertical = vertical;
        emit verticalChanged(vertical);
        if (m_textNode) {
            m_textNode->setVertical(vertical);
        }
    }
}
