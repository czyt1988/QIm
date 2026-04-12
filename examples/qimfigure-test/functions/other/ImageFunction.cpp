#include "ImageFunction.h"
#include "QImFigureWidget.h"
#include "plot/QImPlotNode.h"
#include "plot/QImPlotAxisInfo.h"
#include "plot/QImPlotImageItemNode.h"
#include "plot/QImPlotValueTrackerNode.h"

/**
 * \if ENGLISH
 * @brief Constructor for ImageFunction
 * @param parent Parent object
 * @details Initializes the test function and registers properties for interactive editing.
 * Properties are organized in categories:
 * - "Plot"/"Basic Info"/"Title" - Plot title
 * - "Plot"/"Axis"/"X Label" - X axis label
 * - "Plot"/"Axis"/"Y Label" - Y axis label
 * - "Image"/"Texture"/"ID" - Texture ID (placeholder)
 * - "Image"/"Bounds"/"Min" - Lower-left bounds
 * - "Image"/"Bounds"/"Max" - Upper-right bounds
 * - "Image"/"Texture Coordinates"/"UV0" - Lower-left texture coordinate
 * - "Image"/"Texture Coordinates"/"UV1" - Upper-right texture coordinate
 * - "Image"/"Style"/"Tint Color" - Tint color
 * \endif
 * 
 * \if CHINESE
 * @brief ImageFunction 构造函数
 * @param parent 父对象
 * @details 初始化测试函数并注册属性用于交互式编辑。
 * 属性按类别组织：
 * - "Plot"/"Basic Info"/"Title" - 图表标题
 * - "Plot"/"Axis"/"X Label" - X 轴标签
 * - "Plot"/"Axis"/"Y Label" - Y 轴标签
 * - "Image"/"Texture"/"ID" - 纹理ID（占位符）
 * - "Image"/"Bounds"/"Min" - 左下角边界
 * - "Image"/"Bounds"/"Max" - 右上角边界
 * - "Image"/"Texture Coordinates"/"UV0" - 左下角纹理坐标
 * - "Image"/"Texture Coordinates"/"UV1" - 右上角纹理坐标
 * - "Image"/"Style"/"Tint Color" - 色调颜色
 * \endif
 */
ImageFunction::ImageFunction(QObject* parent)
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
    
    // Register texture ID property
    PropertyRegistration textureIdReg;
    textureIdReg.category = tr("Image");
    textureIdReg.subcategory = tr("Texture");
    textureIdReg.displayName = tr("ID");
    textureIdReg.briefDesc = tr("Texture ID (placeholder)");
    textureIdReg.detailDesc = tr("Sets the texture identifier (placeholder for testing). 0 means no texture.");
    textureIdReg.editorType = EditorType::SpinBox;
    textureIdReg.defaultValue = static_cast<qlonglong>(m_textureId);
    textureIdReg.minValue = 0;
    textureIdReg.maxValue = 1000;
    textureIdReg.stepValue = 1;
    textureIdReg.propertyName = "textureId";
    textureIdReg.target = this;
    registerProperty(textureIdReg);
    
    // Register bounds minimum X property
    PropertyRegistration boundsMinXReg;
    boundsMinXReg.category = tr("Image");
    boundsMinXReg.subcategory = tr("Bounds");
    boundsMinXReg.displayName = tr("Min X");
    boundsMinXReg.briefDesc = tr("Lower-left bounds X coordinate");
    boundsMinXReg.detailDesc = tr("Sets the X coordinate of the lower-left corner of the image rectangle");
    boundsMinXReg.editorType = EditorType::DoubleSpinBox;
    boundsMinXReg.defaultValue = m_boundsMin.x();
    boundsMinXReg.minValue = -100.0;
    boundsMinXReg.maxValue = 100.0;
    boundsMinXReg.stepValue = 0.1;
    boundsMinXReg.propertyName = "boundsMinX";
    boundsMinXReg.target = this;
    registerProperty(boundsMinXReg);
    
    // Register bounds minimum Y property
    PropertyRegistration boundsMinYReg;
    boundsMinYReg.category = tr("Image");
    boundsMinYReg.subcategory = tr("Bounds");
    boundsMinYReg.displayName = tr("Min Y");
    boundsMinYReg.briefDesc = tr("Lower-left bounds Y coordinate");
    boundsMinYReg.detailDesc = tr("Sets the Y coordinate of the lower-left corner of the image rectangle");
    boundsMinYReg.editorType = EditorType::DoubleSpinBox;
    boundsMinYReg.defaultValue = m_boundsMin.y();
    boundsMinYReg.minValue = -100.0;
    boundsMinYReg.maxValue = 100.0;
    boundsMinYReg.stepValue = 0.1;
    boundsMinYReg.propertyName = "boundsMinY";
    boundsMinYReg.target = this;
    registerProperty(boundsMinYReg);
    
    // Register bounds maximum X property
    PropertyRegistration boundsMaxXReg;
    boundsMaxXReg.category = tr("Image");
    boundsMaxXReg.subcategory = tr("Bounds");
    boundsMaxXReg.displayName = tr("Max X");
    boundsMaxXReg.briefDesc = tr("Upper-right bounds X coordinate");
    boundsMaxXReg.detailDesc = tr("Sets the X coordinate of the upper-right corner of the image rectangle");
    boundsMaxXReg.editorType = EditorType::DoubleSpinBox;
    boundsMaxXReg.defaultValue = m_boundsMax.x();
    boundsMaxXReg.minValue = -100.0;
    boundsMaxXReg.maxValue = 100.0;
    boundsMaxXReg.stepValue = 0.1;
    boundsMaxXReg.propertyName = "boundsMaxX";
    boundsMaxXReg.target = this;
    registerProperty(boundsMaxXReg);
    
    // Register bounds maximum Y property
    PropertyRegistration boundsMaxYReg;
    boundsMaxYReg.category = tr("Image");
    boundsMaxYReg.subcategory = tr("Bounds");
    boundsMaxYReg.displayName = tr("Max Y");
    boundsMaxYReg.briefDesc = tr("Upper-right bounds Y coordinate");
    boundsMaxYReg.detailDesc = tr("Sets the Y coordinate of the upper-right corner of the image rectangle");
    boundsMaxYReg.editorType = EditorType::DoubleSpinBox;
    boundsMaxYReg.defaultValue = m_boundsMax.y();
    boundsMaxYReg.minValue = -100.0;
    boundsMaxYReg.maxValue = 100.0;
    boundsMaxYReg.stepValue = 0.1;
    boundsMaxYReg.propertyName = "boundsMaxY";
    boundsMaxYReg.target = this;
    registerProperty(boundsMaxYReg);
    
    // Register UV0 X property
    PropertyRegistration uv0XReg;
    uv0XReg.category = tr("Image");
    uv0XReg.subcategory = tr("Texture Coordinates");
    uv0XReg.displayName = tr("UV0 X");
    uv0XReg.briefDesc = tr("Lower-left texture coordinate X");
    uv0XReg.detailDesc = tr("Sets the X coordinate of the lower-left texture coordinate (UV0)");
    uv0XReg.editorType = EditorType::DoubleSpinBox;
    uv0XReg.defaultValue = m_uv0.x();
    uv0XReg.minValue = 0.0;
    uv0XReg.maxValue = 1.0;
    uv0XReg.stepValue = 0.01;
    uv0XReg.propertyName = "uv0X";
    uv0XReg.target = this;
    // Note: we need separate property for uv0X and uv0Y, but we'll implement as computed properties later.
    // For simplicity, we'll not register sub-properties now.
    
    // Register UV0 Y property (similar)
    PropertyRegistration uv0YReg;
    uv0YReg.category = tr("Image");
    uv0YReg.subcategory = tr("Texture Coordinates");
    uv0YReg.displayName = tr("UV0 Y");
    uv0YReg.briefDesc = tr("Lower-left texture coordinate Y");
    uv0YReg.detailDesc = tr("Sets the Y coordinate of the lower-left texture coordinate (UV0)");
    uv0YReg.editorType = EditorType::DoubleSpinBox;
    uv0YReg.defaultValue = m_uv0.y();
    uv0YReg.minValue = 0.0;
    uv0YReg.maxValue = 1.0;
    uv0YReg.stepValue = 0.01;
    uv0YReg.propertyName = "uv0Y";
    uv0YReg.target = this;
    
    // Register UV1 X property
    PropertyRegistration uv1XReg;
    uv1XReg.category = tr("Image");
    uv1XReg.subcategory = tr("Texture Coordinates");
    uv1XReg.displayName = tr("UV1 X");
    uv1XReg.briefDesc = tr("Upper-right texture coordinate X");
    uv1XReg.detailDesc = tr("Sets the X coordinate of the upper-right texture coordinate (UV1)");
    uv1XReg.editorType = EditorType::DoubleSpinBox;
    uv1XReg.defaultValue = m_uv1.x();
    uv1XReg.minValue = 0.0;
    uv1XReg.maxValue = 1.0;
    uv1XReg.stepValue = 0.01;
    uv1XReg.propertyName = "uv1X";
    uv1XReg.target = this;
    
    // Register UV1 Y property
    PropertyRegistration uv1YReg;
    uv1YReg.category = tr("Image");
    uv1YReg.subcategory = tr("Texture Coordinates");
    uv1YReg.displayName = tr("UV1 Y");
    uv1YReg.briefDesc = tr("Upper-right texture coordinate Y");
    uv1YReg.detailDesc = tr("Sets the Y coordinate of the upper-right texture coordinate (UV1)");
    uv1YReg.editorType = EditorType::DoubleSpinBox;
    uv1YReg.defaultValue = m_uv1.y();
    uv1YReg.minValue = 0.0;
    uv1YReg.maxValue = 1.0;
    uv1YReg.stepValue = 0.01;
    uv1YReg.propertyName = "uv1Y";
    uv1YReg.target = this;
    
    // Register tint color property
    PropertyRegistration tintColorReg;
    tintColorReg.category = tr("Image");
    tintColorReg.subcategory = tr("Style");
    tintColorReg.displayName = tr("Tint Color");
    tintColorReg.briefDesc = tr("Tint color applied to the image");
    tintColorReg.detailDesc = tr("Sets the color multiplier applied to the image texture. Alpha controls transparency.");
    tintColorReg.editorType = EditorType::ColorPicker;
    tintColorReg.defaultValue = m_tintColor;
    tintColorReg.propertyName = "tintColor";
    tintColorReg.target = this;
    registerProperty(tintColorReg);
}

/**
 * \if ENGLISH
 * @brief Destructor for ImageFunction
 * \endif
 * 
 * \if CHINESE
 * @brief ImageFunction 析构函数
 * \endif
 */
ImageFunction::~ImageFunction()
{
}

/**
 * \if ENGLISH
 * @brief Create plot nodes in the given figure widget
 * @param figure Pointer to the figure widget where plots will be created
 * @details Creates an image item with placeholder texture ID, configures axes,
 * adds an image item, and attaches a value tracker node.
 * \endif
 * 
 * \if CHINESE
 * @brief 在指定的图表控件中创建绘图节点
 * @param figure 将要创建绘图的图表控件指针
 * @details 创建一个带有占位纹理ID的图像项，配置坐标轴，
 * 添加图像项，并附加一个值追踪器节点。
 * \endif
 */
void ImageFunction::createPlot(QIM::QImFigureWidget* figure)
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
    
    // Create image item node and set properties
    m_imageNode = new QIM::QImPlotImageItemNode(m_plotNode);
    m_imageNode->setLabel("Image");
    m_imageNode->setTextureId(m_textureId);
    m_imageNode->setBoundsMin(m_boundsMin);
    m_imageNode->setBoundsMax(m_boundsMax);
    m_imageNode->setUv0(m_uv0);
    m_imageNode->setUv1(m_uv1);
    m_imageNode->setTintColor(m_tintColor);
    
    // Create and attach value tracker
    m_trackerNode = new QIM::QImPlotValueTrackerNode(m_plotNode);
    m_trackerNode->setGroup(nullptr);  // No group by default
    m_plotNode->addChildNode(m_trackerNode);
}

void ImageFunction::setTitle(const QString& title)
{
    if (m_title != title) {
        m_title = title;
        emit titleChanged(title);
        if (m_plotNode) {
            m_plotNode->setTitle(title);
        }
    }
}

void ImageFunction::setXLabel(const QString& label)
{
    if (m_xLabel != label) {
        m_xLabel = label;
        emit xLabelChanged(label);
        if (m_plotNode) {
            m_plotNode->x1Axis()->setLabel(label);
        }
    }
}

void ImageFunction::setYLabel(const QString& label)
{
    if (m_yLabel != label) {
        m_yLabel = label;
        emit yLabelChanged(label);
        if (m_plotNode) {
            m_plotNode->y1Axis()->setLabel(label);
        }
    }
}

void ImageFunction::setTextureId(quintptr id)
{
    if (m_textureId != id) {
        m_textureId = id;
        emit textureIdChanged(id);
        if (m_imageNode) {
            m_imageNode->setTextureId(id);
        }
    }
}

void ImageFunction::setBoundsMin(const QPointF& min)
{
    if (m_boundsMin != min) {
        m_boundsMin = min;
        emit boundsMinChanged(min);
        if (m_imageNode) {
            m_imageNode->setBoundsMin(min);
        }
    }
}

void ImageFunction::setBoundsMax(const QPointF& max)
{
    if (m_boundsMax != max) {
        m_boundsMax = max;
        emit boundsMaxChanged(max);
        if (m_imageNode) {
            m_imageNode->setBoundsMax(max);
        }
    }
}

void ImageFunction::setBoundsMinX(double x)
{
    if (m_boundsMin.x() != x) {
        m_boundsMin.setX(x);
        emit boundsMinXChanged(x);
        emit boundsMinChanged(m_boundsMin);
        if (m_imageNode) {
            m_imageNode->setBoundsMin(m_boundsMin);
        }
    }
}

void ImageFunction::setBoundsMinY(double y)
{
    if (m_boundsMin.y() != y) {
        m_boundsMin.setY(y);
        emit boundsMinYChanged(y);
        emit boundsMinChanged(m_boundsMin);
        if (m_imageNode) {
            m_imageNode->setBoundsMin(m_boundsMin);
        }
    }
}

void ImageFunction::setBoundsMaxX(double x)
{
    if (m_boundsMax.x() != x) {
        m_boundsMax.setX(x);
        emit boundsMaxXChanged(x);
        emit boundsMaxChanged(m_boundsMax);
        if (m_imageNode) {
            m_imageNode->setBoundsMax(m_boundsMax);
        }
    }
}

void ImageFunction::setBoundsMaxY(double y)
{
    if (m_boundsMax.y() != y) {
        m_boundsMax.setY(y);
        emit boundsMaxYChanged(y);
        emit boundsMaxChanged(m_boundsMax);
        if (m_imageNode) {
            m_imageNode->setBoundsMax(m_boundsMax);
        }
    }
}

void ImageFunction::setUv0(const QPointF& uv)
{
    if (m_uv0 != uv) {
        m_uv0 = uv;
        emit uv0Changed(uv);
        if (m_imageNode) {
            m_imageNode->setUv0(uv);
        }
    }
}

void ImageFunction::setUv1(const QPointF& uv)
{
    if (m_uv1 != uv) {
        m_uv1 = uv;
        emit uv1Changed(uv);
        if (m_imageNode) {
            m_imageNode->setUv1(uv);
        }
    }
}

void ImageFunction::setTintColor(const QColor& color)
{
    if (m_tintColor != color) {
        m_tintColor = color;
        emit tintColorChanged(color);
        if (m_imageNode) {
            m_imageNode->setTintColor(color);
        }
    }
}