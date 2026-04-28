#include "Plot3DMeshFunction.h"
#include "QImFigureWidget.h"
#include "plot3d/QImPlot3DNode.h"
#include "plot3d/QImPlot3DAxisInfo.h"
#include "plot3d/QImPlot3DMeshItemNode.h"
#include "plot3d/QImPlot3DTypes.h"
#include <vector>

/**
 * \if ENGLISH
 * @brief Constructor for Plot3DMeshFunction
 * @param parent Parent object
 * @details Initializes the test function and registers properties for interactive editing.
 * Properties are organized in categories:
 * - "Plot"/"Basic Info"/"Title" - Plot title
 * - "Plot"/"Axis"/"X Label" - X axis label
 * - "Plot"/"Axis"/"Y Label" - Y axis label
 * - "Plot"/"Axis"/"Z Label" - Z axis label
 * - "Mesh"/"Style"/"Fill Color" - Mesh fill color
 * - "Mesh"/"Visibility"/"Fill Visible" - Mesh fill visibility
 * - "Mesh"/"Style"/"Line Color" - Mesh line color
 * - "Mesh"/"Style"/"Line Width" - Mesh line width
 * - "Mesh"/"Visibility"/"Lines Visible" - Mesh lines visibility
 * - "Mesh"/"Visibility"/"Markers Visible" - Mesh markers visibility
 * - "Mesh"/"Marker"/"Shape" - Marker shape
 * - "Mesh"/"Marker"/"Size" - Marker size
 * - "Mesh"/"Marker"/"Weight" - Marker weight
 * - "Mesh"/"Marker"/"Fill Color" - Marker fill color
 * - "Mesh"/"Marker"/"Outline Color" - Marker outline color
 * - "Mesh"/"Style"/"Fill Alpha" - Fill alpha
 * - "Mesh"/"Style"/"Per-Triangle Fill Color" - Per-triangle fill coloring
 * - "Mesh"/"Style"/"Per-Triangle Line Color" - Per-triangle line coloring
 * \endif
 * 
 * \if CHINESE
 * @brief Plot3DMeshFunction 构造函数
 * @param parent 父对象
 * @details 初始化测试函数并注册属性用于交互式编辑。
 * 属性按类别组织：
 * - "Plot"/"Basic Info"/"Title" - 图表标题
 * - "Plot"/"Axis"/"X Label" - X 轴标签
 * - "Plot"/"Axis"/"Y Label" - Y 轴标签
 * - "Plot"/"Axis"/"Z Label" - Z 轴标签
 * - "Mesh"/"Style"/"Fill Color" - 网格填充颜色
 * - "Mesh"/"Visibility"/"Fill Visible" - 网格填充可见性
 * - "Mesh"/"Style"/"Line Color" - 网格线条颜色
 * - "Mesh"/"Style"/"Line Width" - 网格线宽
 * - "Mesh"/"Visibility"/"Lines Visible" - 网格线条可见性
 * - "Mesh"/"Visibility"/"Markers Visible" - 网格标记点可见性
 * - "Mesh"/"Marker"/"Shape" - 标记点形状
 * - "Mesh"/"Marker"/"Size" - 标记点大小
 * - "Mesh"/"Marker"/"Weight" - 标记点权重
 * - "Mesh"/"Marker"/"Fill Color" - 标记点填充颜色
 * - "Mesh"/"Marker"/"Outline Color" - 标记点轮廓颜色
 * - "Mesh"/"Style"/"Fill Alpha" - 填充透明度
 * - "Mesh"/"Style"/"Per-Triangle Fill Color" - 逐三角面填充颜色
 * - "Mesh"/"Style"/"Per-Triangle Line Color" - 逐三角面线条颜色
 * \endif
 */
Plot3DMeshFunction::Plot3DMeshFunction(QObject* parent)
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
    fillColorReg.category = tr("Mesh");
    fillColorReg.subcategory = tr("Style");
    fillColorReg.displayName = tr("Fill Color");
    fillColorReg.briefDesc = tr("Fill color");
    fillColorReg.detailDesc = tr("Sets the fill color of the mesh faces");
    fillColorReg.editorType = EditorType::ColorPicker;
    fillColorReg.defaultValue = m_fillColor;
    fillColorReg.propertyName = "fillColor";
    fillColorReg.target = this;
    registerProperty(fillColorReg);
    
    // Register fill visible property
    PropertyRegistration fillVisibleReg;
    fillVisibleReg.category = tr("Mesh");
    fillVisibleReg.subcategory = tr("Visibility");
    fillVisibleReg.displayName = tr("Fill Visible");
    fillVisibleReg.briefDesc = tr("Fill visible");
    fillVisibleReg.detailDesc = tr("Sets whether mesh faces are visible");
    fillVisibleReg.editorType = EditorType::CheckBox;
    fillVisibleReg.defaultValue = m_fillVisible;
    fillVisibleReg.propertyName = "fillVisible";
    fillVisibleReg.target = this;
    registerProperty(fillVisibleReg);
    
    // Register line color property
    PropertyRegistration lineColorReg;
    lineColorReg.category = tr("Mesh");
    lineColorReg.subcategory = tr("Style");
    lineColorReg.displayName = tr("Line Color");
    lineColorReg.briefDesc = tr("Line color");
    lineColorReg.detailDesc = tr("Sets the color of the mesh edge lines");
    lineColorReg.editorType = EditorType::ColorPicker;
    lineColorReg.defaultValue = m_lineColor;
    lineColorReg.propertyName = "lineColor";
    lineColorReg.target = this;
    registerProperty(lineColorReg);
    
    // Register line width property
    PropertyRegistration lineWidthReg;
    lineWidthReg.category = tr("Mesh");
    lineWidthReg.subcategory = tr("Style");
    lineWidthReg.displayName = tr("Line Width");
    lineWidthReg.briefDesc = tr("Line width");
    lineWidthReg.detailDesc = tr("Sets the width of the mesh edge lines in pixels");
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
    linesVisibleReg.category = tr("Mesh");
    linesVisibleReg.subcategory = tr("Visibility");
    linesVisibleReg.displayName = tr("Lines Visible");
    linesVisibleReg.briefDesc = tr("Lines visible");
    linesVisibleReg.detailDesc = tr("Sets whether mesh edge lines are visible");
    linesVisibleReg.editorType = EditorType::CheckBox;
    linesVisibleReg.defaultValue = m_linesVisible;
    linesVisibleReg.propertyName = "linesVisible";
    linesVisibleReg.target = this;
    registerProperty(linesVisibleReg);
    
    // Register markers visible property
    PropertyRegistration markersVisibleReg;
    markersVisibleReg.category = tr("Mesh");
    markersVisibleReg.subcategory = tr("Visibility");
    markersVisibleReg.displayName = tr("Markers Visible");
    markersVisibleReg.briefDesc = tr("Markers visible");
    markersVisibleReg.detailDesc = tr("Sets whether mesh vertex markers are visible");
    markersVisibleReg.editorType = EditorType::CheckBox;
    markersVisibleReg.defaultValue = m_markersVisible;
    markersVisibleReg.propertyName = "markersVisible";
    markersVisibleReg.target = this;
    registerProperty(markersVisibleReg);
    
    // Register marker shape property
    PropertyRegistration markerShapeReg;
    markerShapeReg.category = tr("Mesh");
    markerShapeReg.subcategory = tr("Marker");
    markerShapeReg.displayName = tr("Shape");
    markerShapeReg.briefDesc = tr("Marker shape");
    markerShapeReg.detailDesc = tr("Sets the marker shape for mesh vertices");
    markerShapeReg.editorType = EditorType::ComboBox;
    markerShapeReg.defaultValue = m_markerShape;
    markerShapeReg.comboBoxOptions = QStringList{"None", "Circle", "Square", "Diamond", "Up", "Down", "Left", "Right", "Cross", "Plus", "Asterisk"};
    markerShapeReg.propertyName = "markerShape";
    markerShapeReg.target = this;
    registerProperty(markerShapeReg);
    
    // Register marker size property
    PropertyRegistration markerSizeReg;
    markerSizeReg.category = tr("Mesh");
    markerSizeReg.subcategory = tr("Marker");
    markerSizeReg.displayName = tr("Size");
    markerSizeReg.briefDesc = tr("Marker size");
    markerSizeReg.detailDesc = tr("Sets the marker size in pixels for mesh vertices");
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
    markerWeightReg.category = tr("Mesh");
    markerWeightReg.subcategory = tr("Marker");
    markerWeightReg.displayName = tr("Weight");
    markerWeightReg.briefDesc = tr("Marker weight");
    markerWeightReg.detailDesc = tr("Sets the marker outline weight in pixels for mesh vertices");
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
    markerFillColorReg.category = tr("Mesh");
    markerFillColorReg.subcategory = tr("Marker");
    markerFillColorReg.displayName = tr("Fill Color");
    markerFillColorReg.briefDesc = tr("Marker fill color");
    markerFillColorReg.detailDesc = tr("Sets the fill color of mesh markers");
    markerFillColorReg.editorType = EditorType::ColorPicker;
    markerFillColorReg.defaultValue = m_markerFillColor;
    markerFillColorReg.propertyName = "markerFillColor";
    markerFillColorReg.target = this;
    registerProperty(markerFillColorReg);
    
    // Register marker outline color property
    PropertyRegistration markerOutlineColorReg;
    markerOutlineColorReg.category = tr("Mesh");
    markerOutlineColorReg.subcategory = tr("Marker");
    markerOutlineColorReg.displayName = tr("Outline Color");
    markerOutlineColorReg.briefDesc = tr("Marker outline color");
    markerOutlineColorReg.detailDesc = tr("Sets the outline color of mesh markers");
    markerOutlineColorReg.editorType = EditorType::ColorPicker;
    markerOutlineColorReg.defaultValue = m_markerOutlineColor;
    markerOutlineColorReg.propertyName = "markerOutlineColor";
    markerOutlineColorReg.target = this;
    registerProperty(markerOutlineColorReg);
    
    // Register fill alpha property
    PropertyRegistration fillAlphaReg;
    fillAlphaReg.category = tr("Mesh");
    fillAlphaReg.subcategory = tr("Style");
    fillAlphaReg.displayName = tr("Fill Alpha");
    fillAlphaReg.briefDesc = tr("Fill alpha");
    fillAlphaReg.detailDesc = tr("Sets the fill transparency of the mesh (0.0 = fully transparent, 1.0 = fully opaque)");
    fillAlphaReg.editorType = EditorType::DoubleSpinBox;
    fillAlphaReg.defaultValue = m_fillAlpha;
    fillAlphaReg.minValue = 0.0;
    fillAlphaReg.maxValue = 1.0;
    fillAlphaReg.stepValue = 0.1;
    fillAlphaReg.propertyName = "fillAlpha";
    fillAlphaReg.target = this;
    registerProperty(fillAlphaReg);
    
    // Register per-triangle fill color enabled property
    PropertyRegistration perTriFillReg;
    perTriFillReg.category = tr("Mesh");
    perTriFillReg.subcategory = tr("Style");
    perTriFillReg.displayName = tr("Per-Triangle Fill");
    perTriFillReg.briefDesc = tr("Per-triangle fill color");
    perTriFillReg.detailDesc = tr("Enables per-triangle fill coloring for the mesh");
    perTriFillReg.editorType = EditorType::CheckBox;
    perTriFillReg.defaultValue = m_perTriangleFillColorEnabled;
    perTriFillReg.propertyName = "perTriangleFillColorEnabled";
    perTriFillReg.target = this;
    registerProperty(perTriFillReg);
    
    // Register per-triangle line color enabled property
    PropertyRegistration perTriLineReg;
    perTriLineReg.category = tr("Mesh");
    perTriLineReg.subcategory = tr("Style");
    perTriLineReg.displayName = tr("Per-Triangle Line");
    perTriLineReg.briefDesc = tr("Per-triangle line color");
    perTriLineReg.detailDesc = tr("Enables per-triangle line coloring for the mesh");
    perTriLineReg.editorType = EditorType::CheckBox;
    perTriLineReg.defaultValue = m_perTriangleLineColorEnabled;
    perTriLineReg.propertyName = "perTriangleLineColorEnabled";
    perTriLineReg.target = this;
    registerProperty(perTriLineReg);
}

/**
 * \if ENGLISH
 * @brief Destructor for Plot3DMeshFunction
 * \endif
 * 
 * \if CHINESE
 * @brief Plot3DMeshFunction 析构函数
 * \endif
 */
Plot3DMeshFunction::~Plot3DMeshFunction()
{
}

/**
 * \if ENGLISH
 * @brief Create plot nodes in the given figure widget
 * @param figure Pointer to the figure widget where plots will be created
 * @details Creates a 3D plot with a cube mesh, configures axes,
 * and adds a 3D mesh item.
 * The cube is a unit cube centered at origin with 8 vertices and 12 triangular faces.
 * \endif
 * 
 * \if CHINESE
 * @brief 在指定的图表控件中创建绘图节点
 * @param figure 将要创建绘图的图表控件指针
 * @details 创建一个包含立方体网格的 3D 图表，配置坐标轴，
 * 并添加 3D 网格元素。
 * 立方体是位于原点中心的单位立方体，包含8个顶点和12个三角面。
 * \endif
 */
void Plot3DMeshFunction::createPlot(QIM::QImFigureWidget* figure)
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
    
    // Create cube vertices: 8 corners of unit cube centered at origin
    std::vector<QIM::QImPlot3DPoint> vertices;
    vertices.reserve(8);
    vertices.emplace_back(-1.0, -1.0, -1.0); // v0
    vertices.emplace_back(1.0, -1.0, -1.0);  // v1
    vertices.emplace_back(1.0, 1.0, -1.0);   // v2
    vertices.emplace_back(-1.0, 1.0, -1.0);  // v3
    vertices.emplace_back(-1.0, -1.0, 1.0);  // v4
    vertices.emplace_back(1.0, -1.0, 1.0);   // v5
    vertices.emplace_back(1.0, 1.0, 1.0);    // v6
    vertices.emplace_back(-1.0, 1.0, 1.0);   // v7
    
    // Create cube indices: 12 triangle faces (36 indices total)
    std::vector<unsigned int> indices;
    indices.reserve(36);
    // Front face (z=1)
    indices.insert(indices.end(), {4, 5, 6, 4, 6, 7});
    // Back face (z=-1)
    indices.insert(indices.end(), {1, 0, 3, 1, 3, 2});
    // Top face (y=1)
    indices.insert(indices.end(), {7, 6, 2, 7, 2, 3});
    // Bottom face (y=-1)
    indices.insert(indices.end(), {0, 1, 5, 0, 5, 4});
    // Right face (x=1)
    indices.insert(indices.end(), {5, 1, 2, 5, 2, 6});
    // Left face (x=-1)
    indices.insert(indices.end(), {0, 4, 7, 0, 7, 3});
    
    // Create 3D mesh item node and set data
    m_mesh3DNode = new QIM::QImPlot3DMeshItemNode(m_plot3DNode);
    m_mesh3DNode->setMeshData(vertices, indices);
    m_mesh3DNode->setFillColor(m_fillColor);
    m_mesh3DNode->setLineColor(m_lineColor);
    m_mesh3DNode->setLineWidth(m_lineWidth);
    m_mesh3DNode->setLinesVisible(m_linesVisible);
    m_mesh3DNode->setFillVisible(m_fillVisible);
    m_mesh3DNode->setMarkersVisible(m_markersVisible);
    m_mesh3DNode->setFillAlpha(m_fillAlpha);
}

void Plot3DMeshFunction::setTitle(const QString& title)
{
    if (m_title != title) {
        m_title = title;
        Q_EMIT titleChanged(title);
        if (m_plot3DNode) {
            m_plot3DNode->setTitle(title);
        }
    }
}

void Plot3DMeshFunction::setXLabel(const QString& label)
{
    if (m_xLabel != label) {
        m_xLabel = label;
        Q_EMIT xLabelChanged(label);
        if (m_plot3DNode) {
            m_plot3DNode->xAxis()->setLabel(label);
        }
    }
}

void Plot3DMeshFunction::setYLabel(const QString& label)
{
    if (m_yLabel != label) {
        m_yLabel = label;
        Q_EMIT yLabelChanged(label);
        if (m_plot3DNode) {
            m_plot3DNode->yAxis()->setLabel(label);
        }
    }
}

void Plot3DMeshFunction::setZLabel(const QString& label)
{
    if (m_zLabel != label) {
        m_zLabel = label;
        Q_EMIT zLabelChanged(label);
        if (m_plot3DNode) {
            m_plot3DNode->zAxis()->setLabel(label);
        }
    }
}

void Plot3DMeshFunction::setFillColor(const QColor& color)
{
    if (m_fillColor != color) {
        m_fillColor = color;
        Q_EMIT fillColorChanged(color);
        if (m_mesh3DNode) {
            m_mesh3DNode->setFillColor(color);
        }
    }
}

void Plot3DMeshFunction::setLineColor(const QColor& color)
{
    if (m_lineColor != color) {
        m_lineColor = color;
        Q_EMIT lineColorChanged(color);
        if (m_mesh3DNode) {
            m_mesh3DNode->setLineColor(color);
        }
    }
}

void Plot3DMeshFunction::setLineWidth(float width)
{
    if (m_lineWidth != width) {
        m_lineWidth = width;
        Q_EMIT lineWidthChanged(width);
        if (m_mesh3DNode) {
            m_mesh3DNode->setLineWidth(width);
        }
    }
}

void Plot3DMeshFunction::setLinesVisible(bool visible)
{
    if (m_linesVisible != visible) {
        m_linesVisible = visible;
        Q_EMIT linesVisibleChanged(visible);
        if (m_mesh3DNode) {
            m_mesh3DNode->setLinesVisible(visible);
        }
    }
}

void Plot3DMeshFunction::setFillVisible(bool visible)
{
    if (m_fillVisible != visible) {
        m_fillVisible = visible;
        Q_EMIT fillVisibleChanged(visible);
        if (m_mesh3DNode) {
            m_mesh3DNode->setFillVisible(visible);
        }
    }
}

void Plot3DMeshFunction::setMarkersVisible(bool visible)
{
    if (m_markersVisible != visible) {
        m_markersVisible = visible;
        Q_EMIT markersVisibleChanged(visible);
        if (m_mesh3DNode) {
            m_mesh3DNode->setMarkersVisible(visible);
        }
    }
}

void Plot3DMeshFunction::setMarkerShape(const QString& shape)
{
    if (m_markerShape != shape) {
        m_markerShape = shape;
        Q_EMIT markerShapeChanged(shape);
        if (m_mesh3DNode) {
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
            m_mesh3DNode->setMarkerShape(enumVal);
        }
    }
}

void Plot3DMeshFunction::setMarkerSize(float size)
{
    if (m_markerSize != size) {
        m_markerSize = size;
        Q_EMIT markerSizeChanged(size);
        if (m_mesh3DNode) {
            m_mesh3DNode->setMarkerSize(size);
        }
    }
}

void Plot3DMeshFunction::setMarkerWeight(float weight)
{
    if (m_markerWeight != weight) {
        m_markerWeight = weight;
        Q_EMIT markerWeightChanged(weight);
        if (m_mesh3DNode) {
            m_mesh3DNode->setMarkerWeight(weight);
        }
    }
}

void Plot3DMeshFunction::setMarkerFillColor(const QColor& color)
{
    if (m_markerFillColor != color) {
        m_markerFillColor = color;
        Q_EMIT markerFillColorChanged(color);
        if (m_mesh3DNode) {
            m_mesh3DNode->setMarkerFillColor(color);
        }
    }
}

void Plot3DMeshFunction::setMarkerOutlineColor(const QColor& color)
{
    if (m_markerOutlineColor != color) {
        m_markerOutlineColor = color;
        Q_EMIT markerOutlineColorChanged(color);
        if (m_mesh3DNode) {
            m_mesh3DNode->setMarkerOutlineColor(color);
        }
    }
}

void Plot3DMeshFunction::setFillAlpha(float alpha)
{
    if (m_fillAlpha != alpha) {
        m_fillAlpha = alpha;
        Q_EMIT fillAlphaChanged(alpha);
        if (m_mesh3DNode) {
            m_mesh3DNode->setFillAlpha(alpha);
        }
    }
}

void Plot3DMeshFunction::setPerTriangleFillColorEnabled(bool enabled)
{
    if (m_perTriangleFillColorEnabled != enabled) {
        m_perTriangleFillColorEnabled = enabled;
        Q_EMIT perTriangleFillColorEnabledChanged(enabled);
        if (m_mesh3DNode) {
            m_mesh3DNode->setPerTriangleFillColorEnabled(enabled);
        }
    }
}

void Plot3DMeshFunction::setPerTriangleLineColorEnabled(bool enabled)
{
    if (m_perTriangleLineColorEnabled != enabled) {
        m_perTriangleLineColorEnabled = enabled;
        Q_EMIT perTriangleLineColorEnabledChanged(enabled);
        if (m_mesh3DNode) {
            m_mesh3DNode->setPerTriangleLineColorEnabled(enabled);
        }
    }
}

void Plot3DMeshFunction::cleanupPlot()
{
    TestFunction::cleanupPlot();
    m_plot3DNode = nullptr;
    m_mesh3DNode = nullptr;
}