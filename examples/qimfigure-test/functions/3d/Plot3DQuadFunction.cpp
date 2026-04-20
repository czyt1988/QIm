#include "Plot3DQuadFunction.h"
#include "QImFigureWidget.h"
#include "plot3d/QImPlot3DNode.h"
#include "plot3d/QImPlot3DAxisInfo.h"
#include "plot3d/QImPlot3DQuadItemNode.h"
#include "plot3d/QImPlot3DDataSeries.h"
#include <QVector>

/**
 * \if ENGLISH
 * @brief Constructor for Plot3DQuadFunction
 * @param parent Parent object
 * @details Initializes the test function and registers properties for interactive editing.
 * Properties are organized in categories:
 * - "Plot"/"Basic Info"/"Title" - Plot title
 * - "Plot"/"Axis"/"X Label" - X axis label
 * - "Plot"/"Axis"/"Y Label" - Y axis label
 * - "Plot"/"Axis"/"Z Label" - Z axis label
 * - "Quad"/"Fill"/"Color" - Fill color
 * - "Quad"/"Line"/"Color" - Line color
 * - "Quad"/"Line"/"Width" - Line width
 * - "Quad"/"Visibility"/"Lines" - Lines visibility
 * - "Quad"/"Visibility"/"Fill" - Fill visibility
 * - "Quad"/"Visibility"/"Markers" - Markers visibility
 * \endif
 * 
 * \if CHINESE
 * @brief Plot3DQuadFunction 构造函数
 * @param parent 父对象
 * @details 初始化测试函数并注册属性用于交互式编辑。
 * 属性按类别组织：
 * - "Plot"/"Basic Info"/"Title" - 图表标题
 * - "Plot"/"Axis"/"X Label" - X 轴标签
 * - "Plot"/"Axis"/"Y Label" - Y 轴标签
 * - "Plot"/"Axis"/"Z Label" - Z 轴标签
 * - "Quad"/"Fill"/"Color" - 填充颜色
 * - "Quad"/"Line"/"Color" - 线条颜色
 * - "Quad"/"Line"/"Width" - 线宽
 * - "Quad"/"Visibility"/"Lines" - 线条可见性
 * - "Quad"/"Visibility"/"Fill" - 填充可见性
 * - "Quad"/"Visibility"/"Markers" - 标记点可见性
 * \endif
 */
Plot3DQuadFunction::Plot3DQuadFunction(QObject* parent)
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
    PropertyRegistration fillReg;
    fillReg.category = tr("Quad");
    fillReg.subcategory = tr("Fill");
    fillReg.displayName = tr("Color");
    fillReg.briefDesc = tr("Fill color");
    fillReg.detailDesc = tr("Sets the fill color of the 3D quad faces");
    fillReg.editorType = EditorType::ColorPicker;
    fillReg.defaultValue = m_fillColor;
    fillReg.propertyName = "fillColor";
    fillReg.target = this;
    registerProperty(fillReg);
    
    // Register line color property
    PropertyRegistration lineColorReg;
    lineColorReg.category = tr("Quad");
    lineColorReg.subcategory = tr("Line");
    lineColorReg.displayName = tr("Color");
    lineColorReg.briefDesc = tr("Line color");
    lineColorReg.detailDesc = tr("Sets the color of the 3D quad edges");
    lineColorReg.editorType = EditorType::ColorPicker;
    lineColorReg.defaultValue = m_lineColor;
    lineColorReg.propertyName = "lineColor";
    lineColorReg.target = this;
    registerProperty(lineColorReg);
    
    // Register line width property
    PropertyRegistration lineWidthReg;
    lineWidthReg.category = tr("Quad");
    lineWidthReg.subcategory = tr("Line");
    lineWidthReg.displayName = tr("Width");
    lineWidthReg.briefDesc = tr("Line width");
    lineWidthReg.detailDesc = tr("Sets the line width (thickness) of the 3D quad edges in pixels");
    lineWidthReg.editorType = EditorType::DoubleSpinBox;
    lineWidthReg.defaultValue = m_lineWidth;
    lineWidthReg.minValue = 0.1;
    lineWidthReg.maxValue = 10.0;
    lineWidthReg.stepValue = 0.1;
    lineWidthReg.propertyName = "lineWidth";
    lineWidthReg.target = this;
    registerProperty(lineWidthReg);
    
    // Register lines visibility property
    PropertyRegistration linesVisReg;
    linesVisReg.category = tr("Quad");
    linesVisReg.subcategory = tr("Visibility");
    linesVisReg.displayName = tr("Lines");
    linesVisReg.briefDesc = tr("Lines visibility");
    linesVisReg.detailDesc = tr("Controls whether the quad edges are visible");
    linesVisReg.editorType = EditorType::CheckBox;
    linesVisReg.defaultValue = m_linesVisible;
    linesVisReg.propertyName = "linesVisible";
    linesVisReg.target = this;
    registerProperty(linesVisReg);
    
    // Register fill visibility property
    PropertyRegistration fillVisReg;
    fillVisReg.category = tr("Quad");
    fillVisReg.subcategory = tr("Visibility");
    fillVisReg.displayName = tr("Fill");
    fillVisReg.briefDesc = tr("Fill visibility");
    fillVisReg.detailDesc = tr("Controls whether the quad faces are filled");
    fillVisReg.editorType = EditorType::CheckBox;
    fillVisReg.defaultValue = m_fillVisible;
    fillVisReg.propertyName = "fillVisible";
    fillVisReg.target = this;
    registerProperty(fillVisReg);
    
    // Register markers visibility property
    PropertyRegistration markersVisReg;
    markersVisReg.category = tr("Quad");
    markersVisReg.subcategory = tr("Visibility");
    markersVisReg.displayName = tr("Markers");
    markersVisReg.briefDesc = tr("Markers visibility");
    markersVisReg.detailDesc = tr("Controls whether the quad vertex markers are visible");
    markersVisReg.editorType = EditorType::CheckBox;
    markersVisReg.defaultValue = m_markersVisible;
    markersVisReg.propertyName = "markersVisible";
    markersVisReg.target = this;
    registerProperty(markersVisReg);
}

/**
 * \if ENGLISH
 * @brief Destructor for Plot3DQuadFunction
 * \endif
 * 
 * \if CHINESE
 * @brief Plot3DQuadFunction 析构函数
 * \endif
 */
Plot3DQuadFunction::~Plot3DQuadFunction()
{
}

/**
 * \if ENGLISH
 * @brief Create plot nodes in the given figure widget
 * @param figure Pointer to the figure widget where plots will be created
 * @details Creates a 3D plot with a quad item (unit cube with 6 faces).
 * The cube is defined by 24 points (4 points per face × 6 faces):
 * - Face 1 (front): (-1,-1,1), (1,-1,1), (1,1,1), (-1,1,1)
 * - Face 2 (back): (1,-1,-1), (-1,-1,-1), (-1,1,-1), (1,1,-1)
 * - Face 3 (top): (-1,1,1), (1,1,1), (1,1,-1), (-1,1,-1)
 * - Face 4 (bottom): (-1,-1,-1), (1,-1,-1), (1,-1,1), (-1,-1,1)
 * - Face 5 (right): (1,-1,1), (1,-1,-1), (1,1,-1), (1,1,1)
 * - Face 6 (left): (-1,-1,-1), (-1,-1,1), (-1,1,1), (-1,1,-1)
 * \endif
 * 
 * \if CHINESE
 * @brief 在指定的图表控件中创建绘图节点
 * @param figure 将要创建绘图的图表控件指针
 * @details 创建一个包含四边形元素（单位立方体，6 个面）的 3D 图表。
 * 立方体由 24 个点定义（每个面 4 个点 × 6 个面）：
 * - 面 1（前）：(-1,-1,1), (1,-1,1), (1,1,1), (-1,1,1)
 * - 面 2（后）：(1,-1,-1), (-1,-1,-1), (-1,1,-1), (1,1,-1)
 * - 面 3（上）：(-1,1,1), (1,1,1), (1,1,-1), (-1,1,-1)
 * - 面 4（下）：(-1,-1,-1), (1,-1,-1), (1,-1,1), (-1,-1,1)
 * - 面 5（右）：(1,-1,1), (1,-1,-1), (1,1,-1), (1,1,1)
 * - 面 6（左）：(-1,-1,-1), (-1,-1,1), (-1,1,1), (-1,1,-1)
 * \endif
 */
void Plot3DQuadFunction::createPlot(QIM::QImFigureWidget* figure)
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
    
    // Generate unit cube data: 6 faces × 4 vertices = 24 points
    QVector<double> xs, ys, zs;
    xs.reserve(24);
    ys.reserve(24);
    zs.reserve(24);
    
    // Face 1 (front): z = 1
    xs << -1 <<  1 <<  1 << -1;
    ys << -1 << -1 <<  1 <<  1;
    zs <<  1 <<  1 <<  1 <<  1;
    
    // Face 2 (back): z = -1
    xs <<  1 << -1 << -1 <<  1;
    ys << -1 << -1 <<  1 <<  1;
    zs << -1 << -1 << -1 << -1;
    
    // Face 3 (top): y = 1
    xs << -1 <<  1 <<  1 << -1;
    ys <<  1 <<  1 <<  1 <<  1;
    zs <<  1 <<  1 << -1 << -1;
    
    // Face 4 (bottom): y = -1
    xs << -1 <<  1 <<  1 << -1;
    ys << -1 << -1 << -1 << -1;
    zs << -1 << -1 <<  1 <<  1;
    
    // Face 5 (right): x = 1
    xs <<  1 <<  1 <<  1 <<  1;
    ys << -1 << -1 <<  1 <<  1;
    zs <<  1 << -1 << -1 <<  1;
    
    // Face 6 (left): x = -1
    xs << -1 << -1 << -1 << -1;
    ys << -1 << -1 <<  1 <<  1;
    zs << -1 <<  1 <<  1 << -1;
    
    // Create 3D quad item node and set data
    m_quad3DNode = new QIM::QImPlot3DQuadItemNode(m_plot3DNode);
    m_quad3DNode->setData(xs, ys, zs);
    m_quad3DNode->setFillColor(m_fillColor);
    m_quad3DNode->setLineColor(m_lineColor);
    m_quad3DNode->setLineWidth(m_lineWidth);
    m_quad3DNode->setLinesVisible(m_linesVisible);
    m_quad3DNode->setFillVisible(m_fillVisible);
    m_quad3DNode->setMarkersVisible(m_markersVisible);
}

void Plot3DQuadFunction::setTitle(const QString& title)
{
    if (m_title != title) {
        m_title = title;
        Q_EMIT titleChanged(title);
        if (m_plot3DNode) {
            m_plot3DNode->setTitle(title);
        }
    }
}

void Plot3DQuadFunction::setXLabel(const QString& label)
{
    if (m_xLabel != label) {
        m_xLabel = label;
        Q_EMIT xLabelChanged(label);
        if (m_plot3DNode) {
            m_plot3DNode->xAxis()->setLabel(label);
        }
    }
}

void Plot3DQuadFunction::setYLabel(const QString& label)
{
    if (m_yLabel != label) {
        m_yLabel = label;
        Q_EMIT yLabelChanged(label);
        if (m_plot3DNode) {
            m_plot3DNode->yAxis()->setLabel(label);
        }
    }
}

void Plot3DQuadFunction::setZLabel(const QString& label)
{
    if (m_zLabel != label) {
        m_zLabel = label;
        Q_EMIT zLabelChanged(label);
        if (m_plot3DNode) {
            m_plot3DNode->zAxis()->setLabel(label);
        }
    }
}

void Plot3DQuadFunction::setFillColor(const QColor& color)
{
    if (m_fillColor != color) {
        m_fillColor = color;
        Q_EMIT fillColorChanged(color);
        if (m_quad3DNode) {
            m_quad3DNode->setFillColor(color);
        }
    }
}

void Plot3DQuadFunction::setLineColor(const QColor& color)
{
    if (m_lineColor != color) {
        m_lineColor = color;
        Q_EMIT lineColorChanged(color);
        if (m_quad3DNode) {
            m_quad3DNode->setLineColor(color);
        }
    }
}

void Plot3DQuadFunction::setLineWidth(float width)
{
    if (m_lineWidth != width) {
        m_lineWidth = width;
        Q_EMIT lineWidthChanged(width);
        if (m_quad3DNode) {
            m_quad3DNode->setLineWidth(width);
        }
    }
}

void Plot3DQuadFunction::setLinesVisible(bool visible)
{
    if (m_linesVisible != visible) {
        m_linesVisible = visible;
        Q_EMIT linesVisibleChanged(visible);
        if (m_quad3DNode) {
            m_quad3DNode->setLinesVisible(visible);
        }
    }
}

void Plot3DQuadFunction::setFillVisible(bool visible)
{
    if (m_fillVisible != visible) {
        m_fillVisible = visible;
        Q_EMIT fillVisibleChanged(visible);
        if (m_quad3DNode) {
            m_quad3DNode->setFillVisible(visible);
        }
    }
}

void Plot3DQuadFunction::setMarkersVisible(bool visible)
{
    if (m_markersVisible != visible) {
        m_markersVisible = visible;
        Q_EMIT markersVisibleChanged(visible);
        if (m_quad3DNode) {
            m_quad3DNode->setMarkersVisible(visible);
        }
    }
}

void Plot3DQuadFunction::cleanupPlot()
{
    TestFunction::cleanupPlot();
    m_plot3DNode = nullptr;
    m_quad3DNode = nullptr;
}
