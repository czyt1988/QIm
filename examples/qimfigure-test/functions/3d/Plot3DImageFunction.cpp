#include "Plot3DImageFunction.h"
#include "QImFigureWidget.h"
#include "plot3d/QImPlot3DNode.h"
#include "plot3d/QImPlot3DAxisInfo.h"
#include "plot3d/QImPlot3DImageItemNode.h"

/**
 * \if ENGLISH
 * @brief Constructor for Plot3DImageFunction
 * @param parent Parent object
 * @details Initializes the test function and registers properties for interactive editing.
 * Properties are organized in categories:
 * - "Plot"/"Basic Info"/"Title" - Plot title
 * - "Plot"/"Axis"/"X Label" - X axis label
 * - "Plot"/"Axis"/"Y Label" - Y axis label
 * - "Plot"/"Axis"/"Z Label" - Z axis label
 * - "Image"/"Center"/"X" - Image center X coordinate
 * - "Image"/"Center"/"Y" - Image center Y coordinate
 * - "Image"/"Center"/"Z" - Image center Z coordinate
 * - "Image"/"Axis U"/"X" - Image U axis X component
 * - "Image"/"Axis U"/"Y" - Image U axis Y component
 * - "Image"/"Axis U"/"Z" - Image U axis Z component
 * - "Image"/"Axis V"/"X" - Image V axis X component
 * - "Image"/"Axis V"/"Y" - Image V axis Y component
 * - "Image"/"Axis V"/"Z" - Image V axis Z component
 * - "Image"/"Tint"/"Color" - Image tint color
 * - "Image"/"UV0"/"X" - Image lower-left UV X coordinate
 * - "Image"/"UV0"/"Y" - Image lower-left UV Y coordinate
 * - "Image"/"UV1"/"X" - Image upper-right UV X coordinate
 * - "Image"/"UV1"/"Y" - Image upper-right UV Y coordinate
 * \endif
 * 
 * \if CHINESE
 * @brief Plot3DImageFunction 构造函数
 * @param parent 父对象
 * @details 初始化测试函数并注册属性用于交互式编辑。
 * 属性按类别组织：
 * - "Plot"/"Basic Info"/"Title" - 图表标题
 * - "Plot"/"Axis"/"X Label" - X 轴标签
 * - "Plot"/"Axis"/"Y Label" - Y 轴标签
 * - "Plot"/"Axis"/"Z Label" - Z 轴标签
 * - "Image"/"Center"/"X" - 图像中心X坐标
 * - "Image"/"Center"/"Y" - 图像中心Y坐标
 * - "Image"/"Center"/"Z" - 图像中心Z坐标
 * - "Image"/"Axis U"/"X" - 图像U轴X分量
 * - "Image"/"Axis U"/"Y" - 图像U轴Y分量
 * - "Image"/"Axis U"/"Z" - 图像U轴Z分量
 * - "Image"/"Axis V"/"X" - 图像V轴X分量
 * - "Image"/"Axis V"/"Y" - 图像V轴Y分量
 * - "Image"/"Axis V"/"Z" - 图像V轴Z分量
 * - "Image"/"Tint"/"Color" - 图像色调颜色
 * - "Image"/"UV0"/"X" - 图像左下角UV X坐标
 * - "Image"/"UV0"/"Y" - 图像左下角UV Y坐标
 * - "Image"/"UV1"/"X" - 图像右上角UV X坐标
 * - "Image"/"UV1"/"Y" - 图像右上角UV Y坐标
 * \endif
 */
Plot3DImageFunction::Plot3DImageFunction(QObject* parent)
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
    
    // Register center X property
    PropertyRegistration centerXReg;
    centerXReg.category = tr("Image");
    centerXReg.subcategory = tr("Center");
    centerXReg.displayName = tr("X");
    centerXReg.briefDesc = tr("Center X coordinate");
    centerXReg.detailDesc = tr("Sets the X coordinate of the image center in 3D space");
    centerXReg.editorType = EditorType::DoubleSpinBox;
    centerXReg.defaultValue = m_centerX;
    centerXReg.minValue = -10.0;
    centerXReg.maxValue = 10.0;
    centerXReg.stepValue = 0.1;
    centerXReg.propertyName = "centerX";
    centerXReg.target = this;
    registerProperty(centerXReg);
    
    // Register center Y property
    PropertyRegistration centerYReg;
    centerYReg.category = tr("Image");
    centerYReg.subcategory = tr("Center");
    centerYReg.displayName = tr("Y");
    centerYReg.briefDesc = tr("Center Y coordinate");
    centerYReg.detailDesc = tr("Sets the Y coordinate of the image center in 3D space");
    centerYReg.editorType = EditorType::DoubleSpinBox;
    centerYReg.defaultValue = m_centerY;
    centerYReg.minValue = -10.0;
    centerYReg.maxValue = 10.0;
    centerYReg.stepValue = 0.1;
    centerYReg.propertyName = "centerY";
    centerYReg.target = this;
    registerProperty(centerYReg);
    
    // Register center Z property
    PropertyRegistration centerZReg;
    centerZReg.category = tr("Image");
    centerZReg.subcategory = tr("Center");
    centerZReg.displayName = tr("Z");
    centerZReg.briefDesc = tr("Center Z coordinate");
    centerZReg.detailDesc = tr("Sets the Z coordinate of the image center in 3D space");
    centerZReg.editorType = EditorType::DoubleSpinBox;
    centerZReg.defaultValue = m_centerZ;
    centerZReg.minValue = -10.0;
    centerZReg.maxValue = 10.0;
    centerZReg.stepValue = 0.1;
    centerZReg.propertyName = "centerZ";
    centerZReg.target = this;
    registerProperty(centerZReg);
    
    // Register axis U X property
    PropertyRegistration axisUXReg;
    axisUXReg.category = tr("Image");
    axisUXReg.subcategory = tr("Axis U");
    axisUXReg.displayName = tr("X");
    axisUXReg.briefDesc = tr("Axis U X component");
    axisUXReg.detailDesc = tr("Sets the X component of the U axis direction vector");
    axisUXReg.editorType = EditorType::DoubleSpinBox;
    axisUXReg.defaultValue = m_axisUX;
    axisUXReg.minValue = -5.0;
    axisUXReg.maxValue = 5.0;
    axisUXReg.stepValue = 0.1;
    axisUXReg.propertyName = "axisUX";
    axisUXReg.target = this;
    registerProperty(axisUXReg);
    
    // Register axis U Y property
    PropertyRegistration axisUYReg;
    axisUYReg.category = tr("Image");
    axisUYReg.subcategory = tr("Axis U");
    axisUYReg.displayName = tr("Y");
    axisUYReg.briefDesc = tr("Axis U Y component");
    axisUYReg.detailDesc = tr("Sets the Y component of the U axis direction vector");
    axisUYReg.editorType = EditorType::DoubleSpinBox;
    axisUYReg.defaultValue = m_axisUY;
    axisUYReg.minValue = -5.0;
    axisUYReg.maxValue = 5.0;
    axisUYReg.stepValue = 0.1;
    axisUYReg.propertyName = "axisUY";
    axisUYReg.target = this;
    registerProperty(axisUYReg);
    
    // Register axis U Z property
    PropertyRegistration axisUZReg;
    axisUZReg.category = tr("Image");
    axisUZReg.subcategory = tr("Axis U");
    axisUZReg.displayName = tr("Z");
    axisUZReg.briefDesc = tr("Axis U Z component");
    axisUZReg.detailDesc = tr("Sets the Z component of the U axis direction vector");
    axisUZReg.editorType = EditorType::DoubleSpinBox;
    axisUZReg.defaultValue = m_axisUZ;
    axisUZReg.minValue = -5.0;
    axisUZReg.maxValue = 5.0;
    axisUZReg.stepValue = 0.1;
    axisUZReg.propertyName = "axisUZ";
    axisUZReg.target = this;
    registerProperty(axisUZReg);
    
    // Register axis V X property
    PropertyRegistration axisVXReg;
    axisVXReg.category = tr("Image");
    axisVXReg.subcategory = tr("Axis V");
    axisVXReg.displayName = tr("X");
    axisVXReg.briefDesc = tr("Axis V X component");
    axisVXReg.detailDesc = tr("Sets the X component of the V axis direction vector");
    axisVXReg.editorType = EditorType::DoubleSpinBox;
    axisVXReg.defaultValue = m_axisVX;
    axisVXReg.minValue = -5.0;
    axisVXReg.maxValue = 5.0;
    axisVXReg.stepValue = 0.1;
    axisVXReg.propertyName = "axisVX";
    axisVXReg.target = this;
    registerProperty(axisVXReg);
    
    // Register axis V Y property
    PropertyRegistration axisVYReg;
    axisVYReg.category = tr("Image");
    axisVYReg.subcategory = tr("Axis V");
    axisVYReg.displayName = tr("Y");
    axisVYReg.briefDesc = tr("Axis V Y component");
    axisVYReg.detailDesc = tr("Sets the Y component of the V axis direction vector");
    axisVYReg.editorType = EditorType::DoubleSpinBox;
    axisVYReg.defaultValue = m_axisVY;
    axisVYReg.minValue = -5.0;
    axisVYReg.maxValue = 5.0;
    axisVYReg.stepValue = 0.1;
    axisVYReg.propertyName = "axisVY";
    axisVYReg.target = this;
    registerProperty(axisVYReg);
    
    // Register axis V Z property
    PropertyRegistration axisVZReg;
    axisVZReg.category = tr("Image");
    axisVZReg.subcategory = tr("Axis V");
    axisVZReg.displayName = tr("Z");
    axisVZReg.briefDesc = tr("Axis V Z component");
    axisVZReg.detailDesc = tr("Sets the Z component of the V axis direction vector");
    axisVZReg.editorType = EditorType::DoubleSpinBox;
    axisVZReg.defaultValue = m_axisVZ;
    axisVZReg.minValue = -5.0;
    axisVZReg.maxValue = 5.0;
    axisVZReg.stepValue = 0.1;
    axisVZReg.propertyName = "axisVZ";
    axisVZReg.target = this;
    registerProperty(axisVZReg);
    
    // Register tint color property
    PropertyRegistration tintColorReg;
    tintColorReg.category = tr("Image");
    tintColorReg.subcategory = tr("Tint");
    tintColorReg.displayName = tr("Color");
    tintColorReg.briefDesc = tr("Tint color");
    tintColorReg.detailDesc = tr("Sets the tint color applied to the image texture");
    tintColorReg.editorType = EditorType::ColorPicker;
    tintColorReg.defaultValue = m_tintColor;
    tintColorReg.propertyName = "tintColor";
    tintColorReg.target = this;
    registerProperty(tintColorReg);
    
    // Register UV0 X property
    PropertyRegistration uv0XReg;
    uv0XReg.category = tr("Image");
    uv0XReg.subcategory = tr("UV0");
    uv0XReg.displayName = tr("X");
    uv0XReg.briefDesc = tr("UV0 X coordinate");
    uv0XReg.detailDesc = tr("Sets the X component of the lower-left texture coordinate");
    uv0XReg.editorType = EditorType::DoubleSpinBox;
    uv0XReg.defaultValue = m_uv0X;
    uv0XReg.minValue = 0.0;
    uv0XReg.maxValue = 1.0;
    uv0XReg.stepValue = 0.01;
    uv0XReg.propertyName = "uv0X";
    uv0XReg.target = this;
    registerProperty(uv0XReg);
    
    // Register UV0 Y property
    PropertyRegistration uv0YReg;
    uv0YReg.category = tr("Image");
    uv0YReg.subcategory = tr("UV0");
    uv0YReg.displayName = tr("Y");
    uv0YReg.briefDesc = tr("UV0 Y coordinate");
    uv0YReg.detailDesc = tr("Sets the Y component of the lower-left texture coordinate");
    uv0YReg.editorType = EditorType::DoubleSpinBox;
    uv0YReg.defaultValue = m_uv0Y;
    uv0YReg.minValue = 0.0;
    uv0YReg.maxValue = 1.0;
    uv0YReg.stepValue = 0.01;
    uv0YReg.propertyName = "uv0Y";
    uv0YReg.target = this;
    registerProperty(uv0YReg);
    
    // Register UV1 X property
    PropertyRegistration uv1XReg;
    uv1XReg.category = tr("Image");
    uv1XReg.subcategory = tr("UV1");
    uv1XReg.displayName = tr("X");
    uv1XReg.briefDesc = tr("UV1 X coordinate");
    uv1XReg.detailDesc = tr("Sets the X component of the upper-right texture coordinate");
    uv1XReg.editorType = EditorType::DoubleSpinBox;
    uv1XReg.defaultValue = m_uv1X;
    uv1XReg.minValue = 0.0;
    uv1XReg.maxValue = 1.0;
    uv1XReg.stepValue = 0.01;
    uv1XReg.propertyName = "uv1X";
    uv1XReg.target = this;
    registerProperty(uv1XReg);
    
    // Register UV1 Y property
    PropertyRegistration uv1YReg;
    uv1YReg.category = tr("Image");
    uv1YReg.subcategory = tr("UV1");
    uv1YReg.displayName = tr("Y");
    uv1YReg.briefDesc = tr("UV1 Y coordinate");
    uv1YReg.detailDesc = tr("Sets the Y component of the upper-right texture coordinate");
    uv1YReg.editorType = EditorType::DoubleSpinBox;
    uv1YReg.defaultValue = m_uv1Y;
    uv1YReg.minValue = 0.0;
    uv1YReg.maxValue = 1.0;
    uv1YReg.stepValue = 0.01;
    uv1YReg.propertyName = "uv1Y";
    uv1YReg.target = this;
    registerProperty(uv1YReg);
}

/**
 * \if ENGLISH
 * @brief Destructor for Plot3DImageFunction
 * \endif
 * 
 * \if CHINESE
 * @brief Plot3DImageFunction 析构函数
 * \endif
 */
Plot3DImageFunction::~Plot3DImageFunction()
{
}

/**
 * \if ENGLISH
 * @brief Create plot nodes in the given figure widget
 * @param figure Pointer to the figure widget where plots will be created
 * @details Creates a 3D plot with an image item using ImGui's built-in font texture
 * as the test texture, configures axes, and sets default image properties.
 * 
 * @note The textureId is set to 0 initially. Users must set a valid GPU texture ID
 * at runtime via the property panel after the ImGui context is initialized.
 * The texture ID must be obtained from the rendering backend (e.g., ImGui::GetIO().Fonts->TexID).
 * \endif
 * 
 * \if CHINESE
 * @brief 在指定的图表控件中创建绘图节点
 * @param figure 将要创建绘图的图表控件指针
 * @details 创建一个包含图像元素的 3D 图表，使用 ImGui 内置的字体纹理作为
 * 测试纹理，配置坐标轴，并设置默认图像属性。
 * 
 * @note textureId 初始设置为 0。用户必须在 ImGui 上下文初始化后，
 * 通过属性面板在运行时设置有效的 GPU 纹理 ID。
 * 纹理 ID 必须从渲染后端获取（例如 ImGui::GetIO().Fonts->TexID）。
 * \endif
 */
void Plot3DImageFunction::createPlot(QIM::QImFigureWidget* figure)
{
    if (!figure) {
        return;
    }
    
    // Create 3D plot node as a top-level render node via QImFigureWidget API
    m_plot3DNode = figure->createPlot3DNode();
    
    // Configure axes and title
    m_plot3DNode->xAxis()->setLabel(m_xLabel);
    m_plot3DNode->yAxis()->setLabel(m_yLabel);
    m_plot3DNode->zAxis()->setLabel(m_zLabel);
    m_plot3DNode->setTitle(m_title);
    
    // Set default isometric view for better 3D visualization
    m_plot3DNode->setBoxRotation(-35.264, 45.0);  // Isometric view (elevation: -35.264°, azimuth: 45°)
    
    // Create 3D image item node
    m_image3DNode = new QIM::QImPlot3DImageItemNode(m_plot3DNode);
    
    // Note: texture ID must be a valid ImTextureID obtained from the rendering backend.
    // The ImGui font texture is not yet available when createPlot() is called,
    // so we set textureId to 0 initially. Users can set a valid texture ID at runtime
    // via the property panel after the ImGui context is initialized.
    m_image3DNode->setTextureId(static_cast<quintptr>(0));
    
    // Set default properties
    m_image3DNode->setCenterX(m_centerX);
    m_image3DNode->setCenterY(m_centerY);
    m_image3DNode->setCenterZ(m_centerZ);
    
    m_image3DNode->setAxisUX(m_axisUX);
    m_image3DNode->setAxisUY(m_axisUY);
    m_image3DNode->setAxisUZ(m_axisUZ);
    
    m_image3DNode->setAxisVX(m_axisVX);
    m_image3DNode->setAxisVY(m_axisVY);
    m_image3DNode->setAxisVZ(m_axisVZ);
    
    m_image3DNode->setTintColor(m_tintColor);
    
    m_image3DNode->setUv0X(m_uv0X);
    m_image3DNode->setUv0Y(m_uv0Y);
    m_image3DNode->setUv1X(m_uv1X);
    m_image3DNode->setUv1Y(m_uv1Y);
}

void Plot3DImageFunction::setTitle(const QString& title)
{
    if (m_title != title) {
        m_title = title;
        Q_EMIT titleChanged(title);
        if (m_plot3DNode) {
            m_plot3DNode->setTitle(title);
        }
    }
}

void Plot3DImageFunction::setXLabel(const QString& label)
{
    if (m_xLabel != label) {
        m_xLabel = label;
        Q_EMIT xLabelChanged(label);
        if (m_plot3DNode) {
            m_plot3DNode->xAxis()->setLabel(label);
        }
    }
}

void Plot3DImageFunction::setYLabel(const QString& label)
{
    if (m_yLabel != label) {
        m_yLabel = label;
        Q_EMIT yLabelChanged(label);
        if (m_plot3DNode) {
            m_plot3DNode->yAxis()->setLabel(label);
        }
    }
}

void Plot3DImageFunction::setZLabel(const QString& label)
{
    if (m_zLabel != label) {
        m_zLabel = label;
        Q_EMIT zLabelChanged(label);
        if (m_plot3DNode) {
            m_plot3DNode->zAxis()->setLabel(label);
        }
    }
}

void Plot3DImageFunction::setCenterX(double x)
{
    if (m_centerX != x) {
        m_centerX = x;
        Q_EMIT centerXChanged(x);
        if (m_image3DNode) {
            m_image3DNode->setCenterX(x);
        }
    }
}

void Plot3DImageFunction::setCenterY(double y)
{
    if (m_centerY != y) {
        m_centerY = y;
        Q_EMIT centerYChanged(y);
        if (m_image3DNode) {
            m_image3DNode->setCenterY(y);
        }
    }
}

void Plot3DImageFunction::setCenterZ(double z)
{
    if (m_centerZ != z) {
        m_centerZ = z;
        Q_EMIT centerZChanged(z);
        if (m_image3DNode) {
            m_image3DNode->setCenterZ(z);
        }
    }
}

void Plot3DImageFunction::setAxisUX(double x)
{
    if (m_axisUX != x) {
        m_axisUX = x;
        Q_EMIT axisUXChanged(x);
        if (m_image3DNode) {
            m_image3DNode->setAxisUX(x);
        }
    }
}

void Plot3DImageFunction::setAxisUY(double y)
{
    if (m_axisUY != y) {
        m_axisUY = y;
        Q_EMIT axisUYChanged(y);
        if (m_image3DNode) {
            m_image3DNode->setAxisUY(y);
        }
    }
}

void Plot3DImageFunction::setAxisUZ(double z)
{
    if (m_axisUZ != z) {
        m_axisUZ = z;
        Q_EMIT axisUZChanged(z);
        if (m_image3DNode) {
            m_image3DNode->setAxisUZ(z);
        }
    }
}

void Plot3DImageFunction::setAxisVX(double x)
{
    if (m_axisVX != x) {
        m_axisVX = x;
        Q_EMIT axisVXChanged(x);
        if (m_image3DNode) {
            m_image3DNode->setAxisVX(x);
        }
    }
}

void Plot3DImageFunction::setAxisVY(double y)
{
    if (m_axisVY != y) {
        m_axisVY = y;
        Q_EMIT axisVYChanged(y);
        if (m_image3DNode) {
            m_image3DNode->setAxisVY(y);
        }
    }
}

void Plot3DImageFunction::setAxisVZ(double z)
{
    if (m_axisVZ != z) {
        m_axisVZ = z;
        Q_EMIT axisVZChanged(z);
        if (m_image3DNode) {
            m_image3DNode->setAxisVZ(z);
        }
    }
}

void Plot3DImageFunction::setTintColor(const QColor& color)
{
    if (m_tintColor != color) {
        m_tintColor = color;
        Q_EMIT tintColorChanged(color);
        if (m_image3DNode) {
            m_image3DNode->setTintColor(color);
        }
    }
}

void Plot3DImageFunction::setUv0X(double x)
{
    if (m_uv0X != x) {
        m_uv0X = x;
        Q_EMIT uv0XChanged(x);
        if (m_image3DNode) {
            m_image3DNode->setUv0X(x);
        }
    }
}

void Plot3DImageFunction::setUv0Y(double y)
{
    if (m_uv0Y != y) {
        m_uv0Y = y;
        Q_EMIT uv0YChanged(y);
        if (m_image3DNode) {
            m_image3DNode->setUv0Y(y);
        }
    }
}

void Plot3DImageFunction::setUv1X(double x)
{
    if (m_uv1X != x) {
        m_uv1X = x;
        Q_EMIT uv1XChanged(x);
        if (m_image3DNode) {
            m_image3DNode->setUv1X(x);
        }
    }
}

void Plot3DImageFunction::setUv1Y(double y)
{
    if (m_uv1Y != y) {
        m_uv1Y = y;
        Q_EMIT uv1YChanged(y);
        if (m_image3DNode) {
            m_image3DNode->setUv1Y(y);
        }
    }
}
