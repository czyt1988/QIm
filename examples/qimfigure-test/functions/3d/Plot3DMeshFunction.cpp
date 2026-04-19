#include "Plot3DMeshFunction.h"
#include "QImFigureWidget.h"
#include "plot3d/QImPlot3DNode.h"
#include "plot3d/QImPlot3DAxisInfo.h"
#include "plot3d/QImPlot3DMeshItemNode.h"
#include "implot3d.h"
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
 * - "Mesh"/"Fill"/"Color" - Mesh fill color
 * - "Mesh"/"Fill"/"Visible" - Mesh fill visibility
 * - "Mesh"/"Line"/"Color" - Mesh line color
 * - "Mesh"/"Line"/"Width" - Mesh line width
 * - "Mesh"/"Line"/"Visible" - Mesh line visibility
 * - "Mesh"/"Markers"/"Visible" - Mesh markers visibility
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
 * - "Mesh"/"Fill"/"Color" - 网格填充颜色
 * - "Mesh"/"Fill"/"Visible" - 网格填充可见性
 * - "Mesh"/"Line"/"Color" - 网格线条颜色
 * - "Mesh"/"Line"/"Width" - 网格线宽
 * - "Mesh"/"Line"/"Visible" - 网格线条可见性
 * - "Mesh"/"Markers"/"Visible" - 网格标记点可见性
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
    fillColorReg.subcategory = tr("Fill");
    fillColorReg.displayName = tr("Color");
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
    fillVisibleReg.subcategory = tr("Fill");
    fillVisibleReg.displayName = tr("Visible");
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
    lineColorReg.subcategory = tr("Line");
    lineColorReg.displayName = tr("Color");
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
    lineWidthReg.subcategory = tr("Line");
    lineWidthReg.displayName = tr("Width");
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
    linesVisibleReg.subcategory = tr("Line");
    linesVisibleReg.displayName = tr("Visible");
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
    markersVisibleReg.subcategory = tr("Markers");
    markersVisibleReg.displayName = tr("Visible");
    markersVisibleReg.briefDesc = tr("Markers visible");
    markersVisibleReg.detailDesc = tr("Sets whether mesh vertex markers are visible");
    markersVisibleReg.editorType = EditorType::CheckBox;
    markersVisibleReg.defaultValue = m_markersVisible;
    markersVisibleReg.propertyName = "markersVisible";
    markersVisibleReg.target = this;
    registerProperty(markersVisibleReg);
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
    
    // Create 3D plot node as a top-level render node via QImFigureWidget API
    m_plot3DNode = figure->createPlot3DNode();
    
    // Configure axes and title
    m_plot3DNode->xAxis()->setLabel(m_xLabel);
    m_plot3DNode->yAxis()->setLabel(m_yLabel);
    m_plot3DNode->zAxis()->setLabel(m_zLabel);
    m_plot3DNode->setTitle(m_title);
    
    // Set default isometric view for better 3D visualization
    m_plot3DNode->setBoxRotation(-35.264, 45.0);  // Isometric view (elevation: -35.264°, azimuth: 45°)
    
    // Create cube vertices: 8 corners of unit cube centered at origin
    std::vector<ImPlot3DPoint> vertices;
    vertices.reserve(8);
    vertices.emplace_back(-1.0f, -1.0f, -1.0f); // v0
    vertices.emplace_back(1.0f, -1.0f, -1.0f);  // v1
    vertices.emplace_back(1.0f, 1.0f, -1.0f);   // v2
    vertices.emplace_back(-1.0f, 1.0f, -1.0f);  // v3
    vertices.emplace_back(-1.0f, -1.0f, 1.0f);  // v4
    vertices.emplace_back(1.0f, -1.0f, 1.0f);   // v5
    vertices.emplace_back(1.0f, 1.0f, 1.0f);    // v6
    vertices.emplace_back(-1.0f, 1.0f, 1.0f);   // v7
    
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
