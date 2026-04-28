#include "Plot3DSurfaceFunction.h"
#include "QImFigureWidget.h"
#include "plot3d/QImPlot3DNode.h"
#include "plot3d/QImPlot3DAxisInfo.h"
#include "plot3d/QImPlot3DSurfaceItemNode.h"
#include "plot3d/QImPlot3DDataSeries.h"

#include <QVector>
#include <cmath>

/**
 * \if ENGLISH
 * @brief Constructor for Plot3DSurfaceFunction
 * @param parent Parent object
 * @details Initializes the test function and registers properties for interactive editing.
 * Properties are organized in categories:
 * - "Plot"/"Basic Info"/"Title" - Plot title
 * - "Plot"/"Axis"/"X Label" - X axis label
 * - "Plot"/"Axis"/"Y Label" - Y axis label
 * - "Plot"/"Axis"/"Z Label" - Z axis label
 * - "Surface"/"Fill"/"Color" - Surface fill color
 * - "Surface"/"Fill"/"Visible" - Surface fill visibility
 * - "Surface"/"Line"/"Color" - Surface line color
 * - "Surface"/"Line"/"Width" - Surface line width
 * - "Surface"/"Line"/"Visible" - Surface line visibility
 * - "Surface"/"Markers"/"Visible" - Markers visibility
 * - "Surface"/"Colormap"/"Enabled" - Colormap enable/disable
 * \endif
 * 
 * \if CHINESE
 * @brief Plot3DSurfaceFunction 构造函数
 * @param parent 父对象
 * @details 初始化测试函数并注册属性用于交互式编辑。
 * 属性按类别组织：
 * - "Plot"/"Basic Info"/"Title" - 图表标题
 * - "Plot"/"Axis"/"X Label" - X 轴标签
 * - "Plot"/"Axis"/"Y Label" - Y 轴标签
 * - "Plot"/"Axis"/"Z Label" - Z 轴标签
 * - "Surface"/"Fill"/"Color" - 曲面填充颜色
 * - "Surface"/"Fill"/"Visible" - 曲面填充可见性
 * - "Surface"/"Line"/"Color" - 曲面线条颜色
 * - "Surface"/"Line"/"Width" - 曲面线条宽度
 * - "Surface"/"Line"/"Visible" - 曲面线条可见性
 * - "Surface"/"Markers"/"Visible" - 标记点可见性
 * - "Surface"/"Colormap"/"Enabled" - 颜色映射启用/禁用
 * \endif
 */
Plot3DSurfaceFunction::Plot3DSurfaceFunction(QObject* parent)
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
    
    // Register fill color property
    PropertyRegistration fillColorReg;
    fillColorReg.category = tr("Surface");
    fillColorReg.subcategory = tr("Fill");
    fillColorReg.displayName = tr("Color");
    fillColorReg.briefDesc = tr("Surface fill color");
    fillColorReg.detailDesc = tr("Sets the color of the 3D surface fill");
    fillColorReg.editorType = EditorType::ColorPicker;
    fillColorReg.defaultValue = m_fillColor;
    fillColorReg.propertyName = "fillColor";
    fillColorReg.target = this;
    registerProperty(fillColorReg);
    
    // Register fill visible property
    PropertyRegistration fillVisibleReg;
    fillVisibleReg.category = tr("Surface");
    fillVisibleReg.subcategory = tr("Fill");
    fillVisibleReg.displayName = tr("Visible");
    fillVisibleReg.briefDesc = tr("Surface fill visibility");
    fillVisibleReg.detailDesc = tr("Controls whether the surface fill is visible");
    fillVisibleReg.editorType = EditorType::CheckBox;
    fillVisibleReg.defaultValue = m_fillVisible;
    fillVisibleReg.propertyName = "fillVisible";
    fillVisibleReg.target = this;
    registerProperty(fillVisibleReg);
    
    // Register line color property
    PropertyRegistration lineColorReg;
    lineColorReg.category = tr("Surface");
    lineColorReg.subcategory = tr("Line");
    lineColorReg.displayName = tr("Color");
    lineColorReg.briefDesc = tr("Surface line color");
    lineColorReg.detailDesc = tr("Sets the color of the 3D surface grid lines");
    lineColorReg.editorType = EditorType::ColorPicker;
    lineColorReg.defaultValue = m_lineColor;
    lineColorReg.propertyName = "lineColor";
    lineColorReg.target = this;
    registerProperty(lineColorReg);
    
    // Register line width property
    PropertyRegistration lineWidthReg;
    lineWidthReg.category = tr("Surface");
    lineWidthReg.subcategory = tr("Line");
    lineWidthReg.displayName = tr("Width");
    lineWidthReg.briefDesc = tr("Surface line width");
    lineWidthReg.detailDesc = tr("Sets the line width (thickness) of the surface grid lines in pixels");
    lineWidthReg.editorType = EditorType::DoubleSpinBox;
    lineWidthReg.defaultValue = m_lineWidth;
    lineWidthReg.minValue = 0.1;
    lineWidthReg.maxValue = 10.0;
    lineWidthReg.stepValue = 0.1;
    lineWidthReg.propertyName = "lineWidth";
    lineWidthReg.target = this;
    registerProperty(lineWidthReg);
    
    // Register lines visible property
    PropertyRegistration linesVisibleReg;
    linesVisibleReg.category = tr("Surface");
    linesVisibleReg.subcategory = tr("Line");
    linesVisibleReg.displayName = tr("Visible");
    linesVisibleReg.briefDesc = tr("Surface line visibility");
    linesVisibleReg.detailDesc = tr("Controls whether the surface grid lines are visible");
    linesVisibleReg.editorType = EditorType::CheckBox;
    linesVisibleReg.defaultValue = m_linesVisible;
    linesVisibleReg.propertyName = "linesVisible";
    linesVisibleReg.target = this;
    registerProperty(linesVisibleReg);
    
    // Register markers visible property
    PropertyRegistration markersVisibleReg;
    markersVisibleReg.category = tr("Surface");
    markersVisibleReg.subcategory = tr("Markers");
    markersVisibleReg.displayName = tr("Visible");
    markersVisibleReg.briefDesc = tr("Markers visibility");
    markersVisibleReg.detailDesc = tr("Controls whether the surface markers are visible");
    markersVisibleReg.editorType = EditorType::CheckBox;
    markersVisibleReg.defaultValue = m_markersVisible;
    markersVisibleReg.propertyName = "markersVisible";
    markersVisibleReg.target = this;
    registerProperty(markersVisibleReg);
    
    // Register colormap enabled property
    PropertyRegistration colormapEnabledReg;
    colormapEnabledReg.category = tr("Surface");
    colormapEnabledReg.subcategory = tr("Colormap");
    colormapEnabledReg.displayName = tr("Enabled");
    colormapEnabledReg.briefDesc = tr("Colormap enabled");
    colormapEnabledReg.detailDesc = tr("Controls whether the colormap is used to color the surface based on Z values");
    colormapEnabledReg.editorType = EditorType::CheckBox;
    colormapEnabledReg.defaultValue = m_colormapEnabled;
    colormapEnabledReg.propertyName = "colormapEnabled";
    colormapEnabledReg.target = this;
    registerProperty(colormapEnabledReg);
    
    // Register xCount property
    PropertyRegistration xCountReg;
    xCountReg.category = tr("Surface");
    xCountReg.subcategory = tr("Grid");
    xCountReg.displayName = tr("X Count");
    xCountReg.briefDesc = tr("X grid count");
    xCountReg.detailDesc = tr("Sets the number of grid points along the X axis for the surface");
    xCountReg.editorType = EditorType::SpinBox;
    xCountReg.defaultValue = m_xCount;
    xCountReg.minValue = 2;
    xCountReg.maxValue = 200;
    xCountReg.propertyName = "xCount";
    xCountReg.target = this;
    registerProperty(xCountReg);
    
    // Register yCount property
    PropertyRegistration yCountReg;
    yCountReg.category = tr("Surface");
    yCountReg.subcategory = tr("Grid");
    yCountReg.displayName = tr("Y Count");
    yCountReg.briefDesc = tr("Y grid count");
    yCountReg.detailDesc = tr("Sets the number of grid points along the Y axis for the surface");
    yCountReg.editorType = EditorType::SpinBox;
    yCountReg.defaultValue = m_yCount;
    yCountReg.minValue = 2;
    yCountReg.maxValue = 200;
    yCountReg.propertyName = "yCount";
    yCountReg.target = this;
    registerProperty(yCountReg);
    
    // Register marker shape property
    PropertyRegistration markerShapeReg;
    markerShapeReg.category = tr("Surface");
    markerShapeReg.subcategory = tr("Marker");
    markerShapeReg.displayName = tr("Shape");
    markerShapeReg.briefDesc = tr("Marker shape");
    markerShapeReg.detailDesc = tr("Sets the marker shape for surface vertices");
    markerShapeReg.editorType = EditorType::ComboBox;
    markerShapeReg.defaultValue = m_markerShape;
    markerShapeReg.comboBoxOptions = QStringList{"None", "Circle", "Square", "Diamond", "Up", "Down", "Left", "Right", "Cross", "Plus", "Asterisk"};
    markerShapeReg.propertyName = "markerShape";
    markerShapeReg.target = this;
    registerProperty(markerShapeReg);
    
    // Register marker size property
    PropertyRegistration markerSizeReg;
    markerSizeReg.category = tr("Surface");
    markerSizeReg.subcategory = tr("Marker");
    markerSizeReg.displayName = tr("Size");
    markerSizeReg.briefDesc = tr("Marker size");
    markerSizeReg.detailDesc = tr("Sets the marker size in pixels for surface vertices");
    markerSizeReg.editorType = EditorType::DoubleSpinBox;
    markerSizeReg.defaultValue = m_markerSize;
    markerSizeReg.minValue = 1.0;
    markerSizeReg.maxValue = 20.0;
    markerSizeReg.stepValue = 0.5;
    markerSizeReg.propertyName = "markerSize";
    markerSizeReg.target = this;
    registerProperty(markerSizeReg);
    
    // Register marker weight property
    PropertyRegistration markerWeightReg;
    markerWeightReg.category = tr("Surface");
    markerWeightReg.subcategory = tr("Marker");
    markerWeightReg.displayName = tr("Weight");
    markerWeightReg.briefDesc = tr("Marker weight");
    markerWeightReg.detailDesc = tr("Sets the marker outline weight in pixels for surface vertices");
    markerWeightReg.editorType = EditorType::DoubleSpinBox;
    markerWeightReg.defaultValue = m_markerWeight;
    markerWeightReg.minValue = 0.5;
    markerWeightReg.maxValue = 10.0;
    markerWeightReg.stepValue = 0.5;
    markerWeightReg.propertyName = "markerWeight";
    markerWeightReg.target = this;
    registerProperty(markerWeightReg);
    
    // Register marker fill color property
    PropertyRegistration markerFillColorReg;
    markerFillColorReg.category = tr("Surface");
    markerFillColorReg.subcategory = tr("Marker");
    markerFillColorReg.displayName = tr("Fill Color");
    markerFillColorReg.briefDesc = tr("Marker fill color");
    markerFillColorReg.detailDesc = tr("Sets the fill color of surface markers");
    markerFillColorReg.editorType = EditorType::ColorPicker;
    markerFillColorReg.defaultValue = m_markerFillColor;
    markerFillColorReg.propertyName = "markerFillColor";
    markerFillColorReg.target = this;
    registerProperty(markerFillColorReg);
    
    // Register marker outline color property
    PropertyRegistration markerOutlineColorReg;
    markerOutlineColorReg.category = tr("Surface");
    markerOutlineColorReg.subcategory = tr("Marker");
    markerOutlineColorReg.displayName = tr("Outline Color");
    markerOutlineColorReg.briefDesc = tr("Marker outline color");
    markerOutlineColorReg.detailDesc = tr("Sets the outline color of surface markers");
    markerOutlineColorReg.editorType = EditorType::ColorPicker;
    markerOutlineColorReg.defaultValue = m_markerOutlineColor;
    markerOutlineColorReg.propertyName = "markerOutlineColor";
    markerOutlineColorReg.target = this;
    registerProperty(markerOutlineColorReg);
    
    // Register fill alpha property
    PropertyRegistration fillAlphaReg;
    fillAlphaReg.category = tr("Surface");
    fillAlphaReg.subcategory = tr("Style");
    fillAlphaReg.displayName = tr("Fill Alpha");
    fillAlphaReg.briefDesc = tr("Surface fill alpha");
    fillAlphaReg.detailDesc = tr("Sets the fill transparency of the surface (0.0 = fully transparent, 1.0 = fully opaque)");
    fillAlphaReg.editorType = EditorType::DoubleSpinBox;
    fillAlphaReg.defaultValue = m_fillAlpha;
    fillAlphaReg.minValue = 0.0;
    fillAlphaReg.maxValue = 1.0;
    fillAlphaReg.stepValue = 0.1;
    fillAlphaReg.propertyName = "fillAlpha";
    fillAlphaReg.target = this;
    registerProperty(fillAlphaReg);
    
    // Register colormap property
    PropertyRegistration colormapReg;
    colormapReg.category = tr("Surface");
    colormapReg.subcategory = tr("Colormap");
    colormapReg.displayName = tr("Colormap");
    colormapReg.briefDesc = tr("Colormap type");
    colormapReg.detailDesc = tr("Sets the colormap used to color the surface based on Z values");
    colormapReg.editorType = EditorType::EnumComboBox;
    colormapReg.defaultValue = m_colormap;
    colormapReg.comboBoxOptions = QStringList{"Deep", "Dark", "Hot", "Cool", "Blues", "Viridis", "Plasma", "Inferno", "Cividis", "Sprint", "Turbo", "Rainbow", "Jet", "Pink", "Magma", "Greys"};
    colormapReg.propertyName = "colormap";
    colormapReg.target = this;
    registerProperty(colormapReg);
    
    // Register colormap scale min property
    PropertyRegistration colormapScaleMinReg;
    colormapScaleMinReg.category = tr("Surface");
    colormapScaleMinReg.subcategory = tr("Colormap");
    colormapScaleMinReg.displayName = tr("Scale Min");
    colormapScaleMinReg.briefDesc = tr("Colormap scale min");
    colormapScaleMinReg.detailDesc = tr("Sets the minimum value for the colormap scale range");
    colormapScaleMinReg.editorType = EditorType::DoubleSpinBox;
    colormapScaleMinReg.defaultValue = m_colormapScaleMin;
    colormapScaleMinReg.propertyName = "colormapScaleMin";
    colormapScaleMinReg.target = this;
    registerProperty(colormapScaleMinReg);
    
    // Register colormap scale max property
    PropertyRegistration colormapScaleMaxReg;
    colormapScaleMaxReg.category = tr("Surface");
    colormapScaleMaxReg.subcategory = tr("Colormap");
    colormapScaleMaxReg.displayName = tr("Scale Max");
    colormapScaleMaxReg.briefDesc = tr("Colormap scale max");
    colormapScaleMaxReg.detailDesc = tr("Sets the maximum value for the colormap scale range");
    colormapScaleMaxReg.editorType = EditorType::DoubleSpinBox;
    colormapScaleMaxReg.defaultValue = m_colormapScaleMax;
    colormapScaleMaxReg.propertyName = "colormapScaleMax";
    colormapScaleMaxReg.target = this;
    registerProperty(colormapScaleMaxReg);
}

/**
 * \if ENGLISH
 * @brief Destructor for Plot3DSurfaceFunction
 * \endif
 * 
 * \if CHINESE
 * @brief Plot3DSurfaceFunction 析构函数
 * \endif
 */
Plot3DSurfaceFunction::~Plot3DSurfaceFunction()
{
}

/**
 * \if ENGLISH
 * @brief Create plot nodes in the given figure widget
 * @param figure Pointer to the figure widget where plots will be created
 * @details Creates a 3D plot with a sin(x)*cos(y) surface, configures axes,
 * and adds a 3D surface item.
 * The surface is generated on a 40x40 grid:
 * - x ranges from -3 to 3
 * - y ranges from -3 to 3
 * - z = sin(x)*cos(y)
 * \endif
 * 
 * \if CHINESE
 * @brief 在指定的图表控件中创建绘图节点
 * @param figure 将要创建绘图的图表控件指针
 * @details 创建一个包含 sin(x)*cos(y) 曲面的 3D 图表，配置坐标轴，
 * 并添加 3D 曲面元素。
 * 曲面在 40x40 网格上生成：
 * - x 范围从 -3 到 3
 * - y 范围从 -3 到 3
 * - z = sin(x)*cos(y)
 * \endif
 */
void Plot3DSurfaceFunction::createPlot(QIM::QImFigureWidget* figure)
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
    
    // Generate 3D surface data: z = sin(x)*cos(y) on 40x40 grid
    constexpr int rows = 40;
    constexpr int cols = 40;
    QVector<double> xs, ys, zs;
    xs.reserve(rows * cols);
    ys.reserve(rows * cols);
    zs.reserve(rows * cols);
    
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            int index = r * cols + c;
            double x = -3.0 + 6.0 * c / (cols - 1);
            double y = -3.0 + 6.0 * r / (rows - 1);
            xs.append(x);
            ys.append(y);
            zs.append(std::sin(x) * std::cos(y));
        }
    }
    
    // Create 3D surface item node and set data
    m_surface3DNode = new QIM::QImPlot3DSurfaceItemNode(m_plot3DNode);
    m_surface3DNode->setData(xs, ys, zs, rows, cols);
    m_surface3DNode->setFillColor(m_fillColor);
    m_surface3DNode->setLineColor(m_lineColor);
    m_surface3DNode->setLineWidth(m_lineWidth);
    m_surface3DNode->setLinesVisible(m_linesVisible);
    m_surface3DNode->setFillVisible(m_fillVisible);
    m_surface3DNode->setMarkersVisible(m_markersVisible);
    m_surface3DNode->setColormapEnabled(m_colormapEnabled);
    if (m_colormapEnabled) {
        m_surface3DNode->setColormap(ImPlot3DColormap_Viridis);
    }
}

void Plot3DSurfaceFunction::setTitle(const QString& title)
{
    if (m_title != title) {
        m_title = title;
        Q_EMIT titleChanged(title);
        if (m_plot3DNode) {
            m_plot3DNode->setTitle(title);
        }
    }
}

void Plot3DSurfaceFunction::setXLabel(const QString& label)
{
    if (m_xLabel != label) {
        m_xLabel = label;
        Q_EMIT xLabelChanged(label);
        if (m_plot3DNode) {
            m_plot3DNode->xAxis()->setLabel(label);
        }
    }
}

void Plot3DSurfaceFunction::setYLabel(const QString& label)
{
    if (m_yLabel != label) {
        m_yLabel = label;
        Q_EMIT yLabelChanged(label);
        if (m_plot3DNode) {
            m_plot3DNode->yAxis()->setLabel(label);
        }
    }
}

void Plot3DSurfaceFunction::setZLabel(const QString& label)
{
    if (m_zLabel != label) {
        m_zLabel = label;
        Q_EMIT zLabelChanged(label);
        if (m_plot3DNode) {
            m_plot3DNode->zAxis()->setLabel(label);
        }
    }
}

void Plot3DSurfaceFunction::setFillColor(const QColor& color)
{
    if (m_fillColor != color) {
        m_fillColor = color;
        Q_EMIT fillColorChanged(color);
        if (m_surface3DNode) {
            m_surface3DNode->setFillColor(color);
        }
    }
}

void Plot3DSurfaceFunction::setLineColor(const QColor& color)
{
    if (m_lineColor != color) {
        m_lineColor = color;
        Q_EMIT lineColorChanged(color);
        if (m_surface3DNode) {
            m_surface3DNode->setLineColor(color);
        }
    }
}

void Plot3DSurfaceFunction::setLineWidth(float width)
{
    if (m_lineWidth != width) {
        m_lineWidth = width;
        Q_EMIT lineWidthChanged(width);
        if (m_surface3DNode) {
            m_surface3DNode->setLineWidth(width);
        }
    }
}

void Plot3DSurfaceFunction::setLinesVisible(bool visible)
{
    if (m_linesVisible != visible) {
        m_linesVisible = visible;
        Q_EMIT linesVisibleChanged(visible);
        if (m_surface3DNode) {
            m_surface3DNode->setLinesVisible(visible);
        }
    }
}

void Plot3DSurfaceFunction::setFillVisible(bool visible)
{
    if (m_fillVisible != visible) {
        m_fillVisible = visible;
        Q_EMIT fillVisibleChanged(visible);
        if (m_surface3DNode) {
            m_surface3DNode->setFillVisible(visible);
        }
    }
}

void Plot3DSurfaceFunction::setMarkersVisible(bool visible)
{
    if (m_markersVisible != visible) {
        m_markersVisible = visible;
        Q_EMIT markersVisibleChanged(visible);
        if (m_surface3DNode) {
            m_surface3DNode->setMarkersVisible(visible);
        }
    }
}

void Plot3DSurfaceFunction::setColormapEnabled(bool enabled)
{
    if (m_colormapEnabled != enabled) {
        m_colormapEnabled = enabled;
        Q_EMIT colormapEnabledChanged(enabled);
        if (m_surface3DNode) {
            m_surface3DNode->setColormapEnabled(enabled);
            if (enabled) {
                m_surface3DNode->setColormap(m_colormap);
            }
        }
    }
}

void Plot3DSurfaceFunction::setXCount(int count)
{
    if (m_xCount != count) {
        m_xCount = count;
        Q_EMIT gridShapeChanged();
        if (m_surface3DNode) {
            m_surface3DNode->setXCount(count);
        }
    }
}

void Plot3DSurfaceFunction::setYCount(int count)
{
    if (m_yCount != count) {
        m_yCount = count;
        Q_EMIT gridShapeChanged();
        if (m_surface3DNode) {
            m_surface3DNode->setYCount(count);
        }
    }
}

void Plot3DSurfaceFunction::setMarkerShape(const QString& shape)
{
    if (m_markerShape != shape) {
        m_markerShape = shape;
        Q_EMIT markerShapeChanged(shape);
        if (m_surface3DNode) {
            // Map QString to ImPlot3DMarker enum value (None=-1, Circle=0, Square=1, ...)
            int enumVal = -1;
            if (shape == tr("None") || shape == "None") enumVal = -1;
            else if (shape == tr("Circle") || shape == "Circle") enumVal = 0;
            else if (shape == tr("Square") || shape == "Square") enumVal = 1;
            else if (shape == tr("Diamond") || shape == "Diamond") enumVal = 2;
            else if (shape == tr("Up") || shape == "Up") enumVal = 3;
            else if (shape == tr("Down") || shape == "Down") enumVal = 4;
            else if (shape == tr("Left") || shape == "Left") enumVal = 5;
            else if (shape == tr("Right") || shape == "Right") enumVal = 6;
            else if (shape == tr("Cross") || shape == "Cross") enumVal = 7;
            else if (shape == tr("Plus") || shape == "Plus") enumVal = 8;
            else if (shape == tr("Asterisk") || shape == "Asterisk") enumVal = 9;
            m_surface3DNode->setMarkerShape(enumVal);
        }
    }
}

void Plot3DSurfaceFunction::setMarkerSize(float size)
{
    if (m_markerSize != size) {
        m_markerSize = size;
        Q_EMIT markerSizeChanged(size);
        if (m_surface3DNode) {
            m_surface3DNode->setMarkerSize(size);
        }
    }
}

void Plot3DSurfaceFunction::setMarkerWeight(float weight)
{
    if (m_markerWeight != weight) {
        m_markerWeight = weight;
        Q_EMIT markerWeightChanged(weight);
        if (m_surface3DNode) {
            m_surface3DNode->setMarkerWeight(weight);
        }
    }
}

void Plot3DSurfaceFunction::setMarkerFillColor(const QColor& color)
{
    if (m_markerFillColor != color) {
        m_markerFillColor = color;
        Q_EMIT markerFillColorChanged(color);
        if (m_surface3DNode) {
            m_surface3DNode->setMarkerFillColor(color);
        }
    }
}

void Plot3DSurfaceFunction::setMarkerOutlineColor(const QColor& color)
{
    if (m_markerOutlineColor != color) {
        m_markerOutlineColor = color;
        Q_EMIT markerOutlineColorChanged(color);
        if (m_surface3DNode) {
            m_surface3DNode->setMarkerOutlineColor(color);
        }
    }
}

void Plot3DSurfaceFunction::setFillAlpha(float alpha)
{
    if (m_fillAlpha != alpha) {
        m_fillAlpha = alpha;
        Q_EMIT fillAlphaChanged(alpha);
        if (m_surface3DNode) {
            m_surface3DNode->setFillAlpha(alpha);
        }
    }
}

void Plot3DSurfaceFunction::setColormap(int cmap)
{
    if (m_colormap != cmap) {
        m_colormap = cmap;
        Q_EMIT colormapChanged(cmap);
        if (m_surface3DNode) {
            m_surface3DNode->setColormap(cmap);
        }
    }
}

void Plot3DSurfaceFunction::setColormapScaleMin(double min)
{
    if (m_colormapScaleMin != min) {
        m_colormapScaleMin = min;
        Q_EMIT colormapScaleMinChanged(min);
        if (m_surface3DNode) {
            m_surface3DNode->setColormapScaleMin(min);
        }
    }
}

void Plot3DSurfaceFunction::setColormapScaleMax(double max)
{
    if (m_colormapScaleMax != max) {
        m_colormapScaleMax = max;
        Q_EMIT colormapScaleMaxChanged(max);
        if (m_surface3DNode) {
            m_surface3DNode->setColormapScaleMax(max);
        }
    }
}

void Plot3DSurfaceFunction::cleanupPlot()
{
    TestFunction::cleanupPlot();
    m_plot3DNode = nullptr;
    m_surface3DNode = nullptr;
}
