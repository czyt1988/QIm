#include "Plot3DTriangleFunction.h"
#include "QImFigureWidget.h"
#include "plot3d/QImPlot3DNode.h"
#include "plot3d/QImPlot3DAxisInfo.h"
#include "plot3d/QImPlot3DTriangleItemNode.h"
#include "plot3d/QImPlot3DDataSeries.h"
#include <QVector>

/**
 * \if ENGLISH
 * @brief Constructor for Plot3DTriangleFunction
 * @param parent Parent object
 * @details Initializes the test function and registers properties for interactive editing.
 * Properties are organized in categories:
 * - "Plot"/"Basic Info"/"Title" - Plot title
 * - "Plot"/"Axis"/"X Label" - X axis label
 * - "Plot"/"Axis"/"Y Label" - Y axis label
 * - "Plot"/"Axis"/"Z Label" - Z axis label
 * - "Triangle"/"Fill"/"Color" - Triangle fill color
 * - "Triangle"/"Fill"/"Visible" - Triangle fill visibility
 * - "Triangle"/"Line"/"Color" - Triangle line color
 * - "Triangle"/"Line"/"Width" - Triangle line width
 * - "Triangle"/"Line"/"Visible" - Triangle line visibility
 * - "Triangle"/"Markers"/"Visible" - Triangle markers visibility
 * \endif
 * 
 * \if CHINESE
 * @brief Plot3DTriangleFunction 构造函数
 * @param parent 父对象
 * @details 初始化测试函数并注册属性用于交互式编辑。
 * 属性按类别组织：
 * - "Plot"/"Basic Info"/"Title" - 图表标题
 * - "Plot"/"Axis"/"X Label" - X 轴标签
 * - "Plot"/"Axis"/"Y Label" - Y 轴标签
 * - "Plot"/"Axis"/"Z Label" - Z 轴标签
 * - "Triangle"/"Fill"/"Color" - 三角形填充颜色
 * - "Triangle"/"Fill"/"Visible" - 三角形填充可见性
 * - "Triangle"/"Line"/"Color" - 三角形线条颜色
 * - "Triangle"/"Line"/"Width" - 三角形线条宽度
 * - "Triangle"/"Line"/"Visible" - 三角形线条可见性
 * - "Triangle"/"Markers"/"Visible" - 三角形标记点可见性
 * \endif
 */
Plot3DTriangleFunction::Plot3DTriangleFunction(QObject* parent)
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
    fillColorReg.category = tr("Triangle");
    fillColorReg.subcategory = tr("Fill");
    fillColorReg.displayName = tr("Color");
    fillColorReg.briefDesc = tr("Triangle fill color");
    fillColorReg.detailDesc = tr("Sets the fill color of the 3D triangle faces");
    fillColorReg.editorType = EditorType::ColorPicker;
    fillColorReg.defaultValue = m_fillColor;
    fillColorReg.propertyName = "fillColor";
    fillColorReg.target = this;
    registerProperty(fillColorReg);
    
    // Register fill visible property
    PropertyRegistration fillVisibleReg;
    fillVisibleReg.category = tr("Triangle");
    fillVisibleReg.subcategory = tr("Fill");
    fillVisibleReg.displayName = tr("Visible");
    fillVisibleReg.briefDesc = tr("Triangle fill visibility");
    fillVisibleReg.detailDesc = tr("Controls whether triangle faces are filled");
    fillVisibleReg.editorType = EditorType::CheckBox;
    fillVisibleReg.defaultValue = m_fillVisible;
    fillVisibleReg.propertyName = "fillVisible";
    fillVisibleReg.target = this;
    registerProperty(fillVisibleReg);
    
    // Register line color property
    PropertyRegistration lineColorReg;
    lineColorReg.category = tr("Triangle");
    lineColorReg.subcategory = tr("Line");
    lineColorReg.displayName = tr("Color");
    lineColorReg.briefDesc = tr("Triangle line color");
    lineColorReg.detailDesc = tr("Sets the color of the triangle edges");
    lineColorReg.editorType = EditorType::ColorPicker;
    lineColorReg.defaultValue = m_lineColor;
    lineColorReg.propertyName = "lineColor";
    lineColorReg.target = this;
    registerProperty(lineColorReg);
    
    // Register line width property
    PropertyRegistration lineWidthReg;
    lineWidthReg.category = tr("Triangle");
    lineWidthReg.subcategory = tr("Line");
    lineWidthReg.displayName = tr("Width");
    lineWidthReg.briefDesc = tr("Triangle line width");
    lineWidthReg.detailDesc = tr("Sets the line width (thickness) of triangle edges in pixels");
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
    linesVisibleReg.category = tr("Triangle");
    linesVisibleReg.subcategory = tr("Line");
    linesVisibleReg.displayName = tr("Visible");
    linesVisibleReg.briefDesc = tr("Triangle lines visibility");
    linesVisibleReg.detailDesc = tr("Controls whether triangle edges are visible");
    linesVisibleReg.editorType = EditorType::CheckBox;
    linesVisibleReg.defaultValue = m_linesVisible;
    linesVisibleReg.propertyName = "linesVisible";
    linesVisibleReg.target = this;
    registerProperty(linesVisibleReg);
    
    // Register markers visible property
    PropertyRegistration markersVisibleReg;
    markersVisibleReg.category = tr("Triangle");
    markersVisibleReg.subcategory = tr("Markers");
    markersVisibleReg.displayName = tr("Visible");
    markersVisibleReg.briefDesc = tr("Triangle markers visibility");
    markersVisibleReg.detailDesc = tr("Controls whether markers are shown at triangle vertices");
    markersVisibleReg.editorType = EditorType::CheckBox;
    markersVisibleReg.defaultValue = m_markersVisible;
    markersVisibleReg.propertyName = "markersVisible";
    markersVisibleReg.target = this;
    registerProperty(markersVisibleReg);
}

/**
 * \if ENGLISH
 * @brief Destructor for Plot3DTriangleFunction
 * \endif
 * 
 * \if CHINESE
 * @brief Plot3DTriangleFunction 析构函数
 * \endif
 */
Plot3DTriangleFunction::~Plot3DTriangleFunction()
{
}

/**
 * \if ENGLISH
 * @brief Create plot nodes in the given figure widget
 * @param figure Pointer to the figure widget where plots will be created
 * @details Creates a 3D plot with a tetrahedron (4 triangular faces),
 * configures axes, and adds a 3D triangle item.
 * 
 * Tetrahedron vertices:
 * - V0 = (0, 0, 1)           (top)
 * - V1 = (0.943, 0, -0.333)  (base right)
 * - V2 = (-0.471, 0.816, -0.333)  (base back-left)
 * - V3 = (-0.471, -0.816, -0.333) (base front-left)
 * 
 * Faces (3 points each, counter-clockwise from outside):
 * - Face 0: V0-V1-V2
 * - Face 1: V0-V2-V3
 * - Face 2: V0-V3-V1
 * - Face 3: V1-V3-V2 (base)
 * 
 * Total: 12 points (4 faces * 3 points each)
 * \endif
 * 
 * \if CHINESE
 * @brief 在指定的图表控件中创建绘图节点
 * @param figure 将要创建绘图的图表控件指针
 * @details 创建一个包含四面体（4 个三角形面）的 3D 图表，
 * 配置坐标轴，并添加 3D 三角形元素。
 * 
 * 四面体顶点：
 * - V0 = (0, 0, 1)           (顶部)
 * - V1 = (0.943, 0, -0.333)  (底部右侧)
 * - V2 = (-0.471, 0.816, -0.333)  (底部后左)
 * - V3 = (-0.471, -0.816, -0.333) (底部前左)
 * 
 * 面（每个面 3 个点，从外部看逆时针）：
 * - 面 0: V0-V1-V2
 * - 面 1: V0-V2-V3
 * - 面 2: V0-V3-V1
 * - 面 3: V1-V3-V2 (底面)
 * 
 * 总计：12 个点（4 个面 * 每个面 3 个点）
 * \endif
 */
void Plot3DTriangleFunction::createPlot(QIM::QImFigureWidget* figure)
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
    m_plot3DNode->setBoxRotation(-35.264, 45.0);  // Isometric view (elevation: -35.264°, azimuth: 45°)
    
    // Generate tetrahedron data
    // Vertices:
    // V0 = (0, 0, 1)           (top)
    // V1 = (0.943, 0, -0.333)  (base right)
    // V2 = (-0.471, 0.816, -0.333)  (base back-left)
    // V3 = (-0.471, -0.816, -0.333) (base front-left)
    // Faces (3 points each): V0-V1-V2, V0-V2-V3, V0-V3-V1, V1-V3-V2
    // Total: 12 points (4 faces * 3 points each)
    QVector<double> xs, ys, zs;
    xs.reserve(12);
    ys.reserve(12);
    zs.reserve(12);
    
    // Face 0: V0-V1-V2
    xs.append(0.0);      ys.append(0.0);       zs.append(1.0);      // V0
    xs.append(0.943);    ys.append(0.0);       zs.append(-0.333);   // V1
    xs.append(-0.471);   ys.append(0.816);     zs.append(-0.333);   // V2
    
    // Face 1: V0-V2-V3
    xs.append(0.0);      ys.append(0.0);       zs.append(1.0);      // V0
    xs.append(-0.471);   ys.append(0.816);     zs.append(-0.333);   // V2
    xs.append(-0.471);   ys.append(-0.816);    zs.append(-0.333);   // V3
    
    // Face 2: V0-V3-V1
    xs.append(0.0);      ys.append(0.0);       zs.append(1.0);      // V0
    xs.append(-0.471);   ys.append(-0.816);    zs.append(-0.333);   // V3
    xs.append(0.943);    ys.append(0.0);       zs.append(-0.333);   // V1
    
    // Face 3: V1-V3-V2 (base)
    xs.append(0.943);    ys.append(0.0);       zs.append(-0.333);   // V1
    xs.append(-0.471);   ys.append(-0.816);    zs.append(-0.333);   // V3
    xs.append(-0.471);   ys.append(0.816);     zs.append(-0.333);   // V2
    
    // Create 3D triangle item node and set data
    m_triangle3DNode = new QIM::QImPlot3DTriangleItemNode(m_plot3DNode);
    m_triangle3DNode->setData(xs, ys, zs);
    m_triangle3DNode->setFillColor(m_fillColor);
    m_triangle3DNode->setLineColor(m_lineColor);
    m_triangle3DNode->setLineWidth(m_lineWidth);
    m_triangle3DNode->setLinesVisible(m_linesVisible);
    m_triangle3DNode->setFillVisible(m_fillVisible);
    m_triangle3DNode->setMarkersVisible(m_markersVisible);
}

void Plot3DTriangleFunction::setTitle(const QString& title)
{
    if (m_title != title) {
        m_title = title;
        Q_EMIT titleChanged(title);
        if (m_plot3DNode) {
            m_plot3DNode->setTitle(title);
        }
    }
}

void Plot3DTriangleFunction::setXLabel(const QString& label)
{
    if (m_xLabel != label) {
        m_xLabel = label;
        Q_EMIT xLabelChanged(label);
        if (m_plot3DNode) {
            m_plot3DNode->xAxis()->setLabel(label);
        }
    }
}

void Plot3DTriangleFunction::setYLabel(const QString& label)
{
    if (m_yLabel != label) {
        m_yLabel = label;
        Q_EMIT yLabelChanged(label);
        if (m_plot3DNode) {
            m_plot3DNode->yAxis()->setLabel(label);
        }
    }
}

void Plot3DTriangleFunction::setZLabel(const QString& label)
{
    if (m_zLabel != label) {
        m_zLabel = label;
        Q_EMIT zLabelChanged(label);
        if (m_plot3DNode) {
            m_plot3DNode->zAxis()->setLabel(label);
        }
    }
}

void Plot3DTriangleFunction::setFillColor(const QColor& color)
{
    if (m_fillColor != color) {
        m_fillColor = color;
        Q_EMIT fillColorChanged(color);
        if (m_triangle3DNode) {
            m_triangle3DNode->setFillColor(color);
        }
    }
}

void Plot3DTriangleFunction::setLineColor(const QColor& color)
{
    if (m_lineColor != color) {
        m_lineColor = color;
        Q_EMIT lineColorChanged(color);
        if (m_triangle3DNode) {
            m_triangle3DNode->setLineColor(color);
        }
    }
}

void Plot3DTriangleFunction::setLineWidth(float width)
{
    if (m_lineWidth != width) {
        m_lineWidth = width;
        Q_EMIT lineWidthChanged(width);
        if (m_triangle3DNode) {
            m_triangle3DNode->setLineWidth(width);
        }
    }
}

void Plot3DTriangleFunction::setLinesVisible(bool visible)
{
    if (m_linesVisible != visible) {
        m_linesVisible = visible;
        Q_EMIT linesVisibleChanged(visible);
        if (m_triangle3DNode) {
            m_triangle3DNode->setLinesVisible(visible);
        }
    }
}

void Plot3DTriangleFunction::setFillVisible(bool visible)
{
    if (m_fillVisible != visible) {
        m_fillVisible = visible;
        Q_EMIT fillVisibleChanged(visible);
        if (m_triangle3DNode) {
            m_triangle3DNode->setFillVisible(visible);
        }
    }
}

void Plot3DTriangleFunction::setMarkersVisible(bool visible)
{
    if (m_markersVisible != visible) {
        m_markersVisible = visible;
        Q_EMIT markersVisibleChanged(visible);
        if (m_triangle3DNode) {
            m_triangle3DNode->setMarkersVisible(visible);
        }
    }
}

void Plot3DTriangleFunction::cleanupPlot()
{
    TestFunction::cleanupPlot();
    m_plot3DNode = nullptr;
    m_triangle3DNode = nullptr;
}
