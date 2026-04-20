#include "Plot3DTextFunction.h"
#include "QImFigureWidget.h"
#include "plot3d/QImPlot3DNode.h"
#include "plot3d/QImPlot3DAxisInfo.h"
#include "plot3d/QImPlot3DTextItemNode.h"
#include <QVector>

/**
 * \if ENGLISH
 * @brief Constructor for Plot3DTextFunction
 * @param parent Parent object
 * @details Initializes the test function and registers properties for interactive editing.
 * Properties are organized in categories:
 * - "Plot"/"Basic Info"/"Title" - Plot title
 * - "Plot"/"Axis"/"X Label" - X axis label
 * - "Plot"/"Axis"/"Y Label" - Y axis label
 * - "Plot"/"Axis"/"Z Label" - Z axis label
 * - "Text"/"Content"/"Text" - Text content
 * - "Text"/"Position"/"X", "Y", "Z" - Text position coordinates
 * - "Text"/"Style"/"Angle" - Text rotation angle
 * - "Text"/"Offset"/"Pixel X", "Pixel Y" - Pixel offset
 * - "Text"/"Style"/"Color" - Text color
 * \endif
 * 
 * \if CHINESE
 * @brief Plot3DTextFunction 构造函数
 * @param parent 父对象
 * @details 初始化测试函数并注册属性用于交互式编辑。
 * 属性按类别组织：
 * - "Plot"/"Basic Info"/"Title" - 图表标题
 * - "Plot"/"Axis"/"X Label" - X 轴标签
 * - "Plot"/"Axis"/"Y Label" - Y 轴标签
 * - "Plot"/"Axis"/"Z Label" - Z 轴标签
 * - "Text"/"Content"/"Text" - 文本内容
 * - "Text"/"Position"/"X", "Y", "Z" - 文本位置坐标
 * - "Text"/"Style"/"Angle" - 文本旋转角度
 * - "Text"/"Offset"/"Pixel X", "Pixel Y" - 像素偏移
 * - "Text"/"Style"/"Color" - 文本颜色
 * \endif
 */
Plot3DTextFunction::Plot3DTextFunction(QObject* parent)
    : TestFunction(parent)
{
    // Register title property
    PropertyRegistration titleReg;
    titleReg.category = tr("Plot");
    titleReg.subcategory = tr("Basic Info");
    titleReg.displayName = tr("Title");
    titleReg.briefDesc = tr("Plot title");
    titleReg.detailDesc = tr("Sets the title text displayed at the top of the 3D plot");
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
    
    // Register Z axis label property
    PropertyRegistration zLabelReg;
    zLabelReg.category = tr("Plot");
    zLabelReg.subcategory = tr("Axis");
    zLabelReg.displayName = tr("Z Label");
    zLabelReg.briefDesc = tr("Z axis label");
    zLabelReg.detailDesc = tr("Sets the label text for the Z axis");
    zLabelReg.editorType = EditorType::LineEdit;
    zLabelReg.defaultValue = m_zLabel;
    zLabelReg.propertyName = "zLabel";
    zLabelReg.target = this;
    registerProperty(zLabelReg);
    
    // Register text property
    PropertyRegistration textReg;
    textReg.category = tr("Text");
    textReg.subcategory = tr("Content");
    textReg.displayName = tr("Text");
    textReg.briefDesc = tr("Text content");
    textReg.detailDesc = tr("Sets the text content to display in 3D space");
    textReg.editorType = EditorType::LineEdit;
    textReg.defaultValue = m_text;
    textReg.propertyName = "text";
    textReg.target = this;
    registerProperty(textReg);
    
    // Register X position property
    PropertyRegistration xReg;
    xReg.category = tr("Text");
    xReg.subcategory = tr("Position");
    xReg.displayName = tr("X");
    xReg.briefDesc = tr("X coordinate");
    xReg.detailDesc = tr("Sets the X coordinate of the text in 3D space");
    xReg.editorType = EditorType::DoubleSpinBox;
    xReg.defaultValue = m_x;
    xReg.minValue = -10.0;
    xReg.maxValue = 10.0;
    xReg.stepValue = 0.1;
    xReg.propertyName = "x";
    xReg.target = this;
    registerProperty(xReg);
    
    // Register Y position property
    PropertyRegistration yReg;
    yReg.category = tr("Text");
    yReg.subcategory = tr("Position");
    yReg.displayName = tr("Y");
    yReg.briefDesc = tr("Y coordinate");
    yReg.detailDesc = tr("Sets the Y coordinate of the text in 3D space");
    yReg.editorType = EditorType::DoubleSpinBox;
    yReg.defaultValue = m_y;
    yReg.minValue = -10.0;
    yReg.maxValue = 10.0;
    yReg.stepValue = 0.1;
    yReg.propertyName = "y";
    yReg.target = this;
    registerProperty(yReg);
    
    // Register Z position property
    PropertyRegistration zReg;
    zReg.category = tr("Text");
    zReg.subcategory = tr("Position");
    zReg.displayName = tr("Z");
    zReg.briefDesc = tr("Z coordinate");
    zReg.detailDesc = tr("Sets the Z coordinate of the text in 3D space");
    zReg.editorType = EditorType::DoubleSpinBox;
    zReg.defaultValue = m_z;
    zReg.minValue = -10.0;
    zReg.maxValue = 10.0;
    zReg.stepValue = 0.1;
    zReg.propertyName = "z";
    zReg.target = this;
    registerProperty(zReg);
    
    // Register angle property
    PropertyRegistration angleReg;
    angleReg.category = tr("Text");
    angleReg.subcategory = tr("Style");
    angleReg.displayName = tr("Angle");
    angleReg.briefDesc = tr("Rotation angle");
    angleReg.detailDesc = tr("Sets the rotation angle of the text in degrees");
    angleReg.editorType = EditorType::DoubleSpinBox;
    angleReg.defaultValue = m_angle;
    angleReg.minValue = 0.0;
    angleReg.maxValue = 360.0;
    angleReg.stepValue = 1.0;
    angleReg.propertyName = "angle";
    angleReg.target = this;
    registerProperty(angleReg);
    
    // Register pixel offset X property
    PropertyRegistration pixelOffsetXReg;
    pixelOffsetXReg.category = tr("Text");
    pixelOffsetXReg.subcategory = tr("Offset");
    pixelOffsetXReg.displayName = tr("Pixel X");
    pixelOffsetXReg.briefDesc = tr("Pixel offset X");
    pixelOffsetXReg.detailDesc = tr("Sets the horizontal pixel offset for fine positioning");
    pixelOffsetXReg.editorType = EditorType::DoubleSpinBox;
    pixelOffsetXReg.defaultValue = m_pixelOffsetX;
    pixelOffsetXReg.minValue = -100.0;
    pixelOffsetXReg.maxValue = 100.0;
    pixelOffsetXReg.stepValue = 1.0;
    pixelOffsetXReg.propertyName = "pixelOffsetX";
    pixelOffsetXReg.target = this;
    registerProperty(pixelOffsetXReg);
    
    // Register pixel offset Y property
    PropertyRegistration pixelOffsetYReg;
    pixelOffsetYReg.category = tr("Text");
    pixelOffsetYReg.subcategory = tr("Offset");
    pixelOffsetYReg.displayName = tr("Pixel Y");
    pixelOffsetYReg.briefDesc = tr("Pixel offset Y");
    pixelOffsetYReg.detailDesc = tr("Sets the vertical pixel offset for fine positioning");
    pixelOffsetYReg.editorType = EditorType::DoubleSpinBox;
    pixelOffsetYReg.defaultValue = m_pixelOffsetY;
    pixelOffsetYReg.minValue = -100.0;
    pixelOffsetYReg.maxValue = 100.0;
    pixelOffsetYReg.stepValue = 1.0;
    pixelOffsetYReg.propertyName = "pixelOffsetY";
    pixelOffsetYReg.target = this;
    registerProperty(pixelOffsetYReg);
    
    // Register color property
    PropertyRegistration colorReg;
    colorReg.category = tr("Text");
    colorReg.subcategory = tr("Style");
    colorReg.displayName = tr("Color");
    colorReg.briefDesc = tr("Text color");
    colorReg.detailDesc = tr("Sets the color of the 3D text label");
    colorReg.editorType = EditorType::ColorPicker;
    colorReg.defaultValue = m_color;
    colorReg.propertyName = "color";
    colorReg.target = this;
    registerProperty(colorReg);
}

/**
 * \if ENGLISH
 * @brief Destructor for Plot3DTextFunction
 * \endif
 * 
 * \if CHINESE
 * @brief Plot3DTextFunction 析构函数
 * \endif
 */
Plot3DTextFunction::~Plot3DTextFunction()
{
}

/**
 * \if ENGLISH
 * @brief Create plot nodes in the given figure widget
 * @param figure Pointer to the figure widget where plots will be created
 * @details Creates a 3D plot with a text label at position (0, 0, 0.5),
 * configures axes, and adds a 3D text item.
 * The text displays "Hello QIm 3D!" with default rotation angle of 0 degrees.
 * \endif
 * 
 * \if CHINESE
 * @brief 在指定的图表控件中创建绘图节点
 * @param figure 将要创建绘图的图表控件指针
 * @details 创建一个包含文本标签的 3D 图表，配置坐标轴，
 * 并添加 3D 文本元素。
 * 文本在位置 (0, 0, 0.5) 处显示 "Hello QIm 3D!"，默认旋转角度为 0 度。
 * \endif
 */
void Plot3DTextFunction::createPlot(QIM::QImFigureWidget* figure)
{
    if (!figure) {
        return;
    }
    
    // Reset to single-plot mode to clear any previous subplot configuration
    figure->setSubplot3DGrid(1, 1);
    
    // Create 3D plot node as a top-level render node via QImFigureWidget API
    m_plot3DNode = figure->createPlot3DNode();
    
    // Configure axes and title
    m_plot3DNode->xAxis()->setLabel(m_xLabel);
    m_plot3DNode->yAxis()->setLabel(m_yLabel);
    m_plot3DNode->zAxis()->setLabel(m_zLabel);
    m_plot3DNode->setTitle(m_title);
    
    // Set default isometric view for better 3D visualization
    m_plot3DNode->setBoxRotation(35.264, 45.0);  // Isometric view (elevation: -35.264°, azimuth: 45°)
    
    // Create 3D text item node and set properties
    m_text3DNode = new QIM::QImPlot3DTextItemNode(m_plot3DNode);
    m_text3DNode->setText(m_text);
    m_text3DNode->setPosition(m_x, m_y, m_z);
    m_text3DNode->setAngle(m_angle);
    m_text3DNode->setPixelOffset(m_pixelOffsetX, m_pixelOffsetY);
    m_text3DNode->setColor(m_color);
}

void Plot3DTextFunction::setTitle(const QString& title)
{
    if (m_title != title) {
        m_title = title;
        Q_EMIT titleChanged(title);
        if (m_plot3DNode) {
            m_plot3DNode->setTitle(title);
        }
    }
}

void Plot3DTextFunction::setXLabel(const QString& label)
{
    if (m_xLabel != label) {
        m_xLabel = label;
        Q_EMIT xLabelChanged(label);
        if (m_plot3DNode) {
            m_plot3DNode->xAxis()->setLabel(label);
        }
    }
}

void Plot3DTextFunction::setYLabel(const QString& label)
{
    if (m_yLabel != label) {
        m_yLabel = label;
        Q_EMIT yLabelChanged(label);
        if (m_plot3DNode) {
            m_plot3DNode->yAxis()->setLabel(label);
        }
    }
}

void Plot3DTextFunction::setZLabel(const QString& label)
{
    if (m_zLabel != label) {
        m_zLabel = label;
        Q_EMIT zLabelChanged(label);
        if (m_plot3DNode) {
            m_plot3DNode->zAxis()->setLabel(label);
        }
    }
}

void Plot3DTextFunction::setText(const QString& text)
{
    if (m_text != text) {
        m_text = text;
        Q_EMIT textChanged(text);
        if (m_text3DNode) {
            m_text3DNode->setText(text);
        }
    }
}

void Plot3DTextFunction::setX(double x)
{
    if (m_x != x) {
        m_x = x;
        Q_EMIT positionChanged(x, m_y, m_z);
        if (m_text3DNode) {
            m_text3DNode->setX(x);
        }
    }
}

void Plot3DTextFunction::setY(double y)
{
    if (m_y != y) {
        m_y = y;
        Q_EMIT positionChanged(m_x, y, m_z);
        if (m_text3DNode) {
            m_text3DNode->setY(y);
        }
    }
}

void Plot3DTextFunction::setZ(double z)
{
    if (m_z != z) {
        m_z = z;
        Q_EMIT positionChanged(m_x, m_y, z);
        if (m_text3DNode) {
            m_text3DNode->setZ(z);
        }
    }
}

void Plot3DTextFunction::setAngle(double angle)
{
    if (m_angle != angle) {
        m_angle = angle;
        Q_EMIT angleChanged(angle);
        if (m_text3DNode) {
            m_text3DNode->setAngle(angle);
        }
    }
}

void Plot3DTextFunction::setPixelOffsetX(double offset)
{
    if (m_pixelOffsetX != offset) {
        m_pixelOffsetX = offset;
        Q_EMIT pixelOffsetChanged(offset, m_pixelOffsetY);
        if (m_text3DNode) {
            m_text3DNode->setPixelOffsetX(offset);
        }
    }
}

void Plot3DTextFunction::setPixelOffsetY(double offset)
{
    if (m_pixelOffsetY != offset) {
        m_pixelOffsetY = offset;
        Q_EMIT pixelOffsetChanged(m_pixelOffsetX, offset);
        if (m_text3DNode) {
            m_text3DNode->setPixelOffsetY(offset);
        }
    }
}

void Plot3DTextFunction::setColor(const QColor& color)
{
    if (m_color != color) {
        m_color = color;
        Q_EMIT colorChanged(color);
        if (m_text3DNode) {
            m_text3DNode->setColor(color);
        }
    }
}

void Plot3DTextFunction::cleanupPlot()
{
    TestFunction::cleanupPlot();
    m_plot3DNode = nullptr;
    m_text3DNode = nullptr;
}
