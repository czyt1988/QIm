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
 * \endif
 * 
 * \if CHINESE
 * @brief Plot3DQuadFunction 构造函数
 * @param parent 父对象
 * @details 初始化测试函数并注册属性用于交互式编辑。
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
    
    // Register axis labels
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
    
    // Register visibility properties
    PropertyRegistration linesVisibleReg;
    linesVisibleReg.category = tr("Quad");
    linesVisibleReg.subcategory = tr("Visibility");
    linesVisibleReg.displayName = tr("Lines");
    linesVisibleReg.briefDesc = tr("Lines visible");
    linesVisibleReg.detailDesc = tr("Controls whether quad edges are visible");
    linesVisibleReg.editorType = EditorType::CheckBox;
    linesVisibleReg.defaultValue = m_linesVisible;
    linesVisibleReg.propertyName = "linesVisible";
    linesVisibleReg.target = this;
    registerProperty(linesVisibleReg);
    
    PropertyRegistration fillVisibleReg;
    fillVisibleReg.category = tr("Quad");
    fillVisibleReg.subcategory = tr("Visibility");
    fillVisibleReg.displayName = tr("Fill");
    fillVisibleReg.briefDesc = tr("Fill visible");
    fillVisibleReg.detailDesc = tr("Controls whether quad faces are filled");
    fillVisibleReg.editorType = EditorType::CheckBox;
    fillVisibleReg.defaultValue = m_fillVisible;
    fillVisibleReg.propertyName = "fillVisible";
    fillVisibleReg.target = this;
    registerProperty(fillVisibleReg);
    
    PropertyRegistration markersVisibleReg;
    markersVisibleReg.category = tr("Quad");
    markersVisibleReg.subcategory = tr("Visibility");
    markersVisibleReg.displayName = tr("Markers");
    markersVisibleReg.briefDesc = tr("Markers visible");
    markersVisibleReg.detailDesc = tr("Controls whether quad vertex markers are visible");
    markersVisibleReg.editorType = EditorType::CheckBox;
    markersVisibleReg.defaultValue = m_markersVisible;
    markersVisibleReg.propertyName = "markersVisible";
    markersVisibleReg.target = this;
    registerProperty(markersVisibleReg);
    
    // Register style properties
    PropertyRegistration fillColorReg;
    fillColorReg.category = tr("Quad");
    fillColorReg.subcategory = tr("Style");
    fillColorReg.displayName = tr("Fill Color");
    fillColorReg.briefDesc = tr("Fill color");
    fillColorReg.detailDesc = tr("Sets the fill color of the 3D quad faces");
    fillColorReg.editorType = EditorType::ColorPicker;
    fillColorReg.defaultValue = m_fillColor;
    fillColorReg.propertyName = "fillColor";
    fillColorReg.target = this;
    registerProperty(fillColorReg);
    
    PropertyRegistration lineColorReg;
    lineColorReg.category = tr("Quad");
    lineColorReg.subcategory = tr("Style");
    lineColorReg.displayName = tr("Line Color");
    lineColorReg.briefDesc = tr("Line color");
    lineColorReg.detailDesc = tr("Sets the color of the 3D quad edges");
    lineColorReg.editorType = EditorType::ColorPicker;
    lineColorReg.defaultValue = m_lineColor;
    lineColorReg.propertyName = "lineColor";
    lineColorReg.target = this;
    registerProperty(lineColorReg);
    
    PropertyRegistration lineWidthReg;
    lineWidthReg.category = tr("Quad");
    lineWidthReg.subcategory = tr("Style");
    lineWidthReg.displayName = tr("Line Width");
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
    
    PropertyRegistration fillAlphaReg;
    fillAlphaReg.category = tr("Quad");
    fillAlphaReg.subcategory = tr("Style");
    fillAlphaReg.displayName = tr("Fill Alpha");
    fillAlphaReg.briefDesc = tr("Fill alpha");
    fillAlphaReg.detailDesc = tr("Sets the fill transparency (0.0 = transparent, 1.0 = opaque)");
    fillAlphaReg.editorType = EditorType::DoubleSpinBox;
    fillAlphaReg.defaultValue = m_fillAlpha;
    fillAlphaReg.minValue = 0.0;
    fillAlphaReg.maxValue = 1.0;
    fillAlphaReg.stepValue = 0.1;
    fillAlphaReg.propertyName = "fillAlpha";
    fillAlphaReg.target = this;
    registerProperty(fillAlphaReg);
    
    // Register marker properties
    PropertyRegistration markerShapeReg;
    markerShapeReg.category = tr("Quad");
    markerShapeReg.subcategory = tr("Marker");
    markerShapeReg.displayName = tr("Shape");
    markerShapeReg.briefDesc = tr("Marker shape");
    markerShapeReg.detailDesc = tr("Sets the marker shape for quad vertices");
    markerShapeReg.editorType = EditorType::ComboBox;
    markerShapeReg.defaultValue = m_markerShape;
    markerShapeReg.comboBoxOptions = QStringList{"None", "Circle", "Square", "Diamond", "Up", "Down", "Left", "Right", "Cross", "Plus", "Asterisk"};
    markerShapeReg.propertyName = "markerShape";
    markerShapeReg.target = this;
    registerProperty(markerShapeReg);
    
    PropertyRegistration markerSizeReg;
    markerSizeReg.category = tr("Quad");
    markerSizeReg.subcategory = tr("Marker");
    markerSizeReg.displayName = tr("Size");
    markerSizeReg.briefDesc = tr("Marker size");
    markerSizeReg.detailDesc = tr("Sets the marker size in pixels");
    markerSizeReg.editorType = EditorType::DoubleSpinBox;
    markerSizeReg.defaultValue = m_markerSize;
    markerSizeReg.minValue = 1.0;
    markerSizeReg.maxValue = 20.0;
    markerSizeReg.stepValue = 0.5;
    markerSizeReg.propertyName = "markerSize";
    markerSizeReg.target = this;
    registerProperty(markerSizeReg);
    
    PropertyRegistration markerWeightReg;
    markerWeightReg.category = tr("Quad");
    markerWeightReg.subcategory = tr("Marker");
    markerWeightReg.displayName = tr("Weight");
    markerWeightReg.briefDesc = tr("Marker weight");
    markerWeightReg.detailDesc = tr("Sets the marker outline weight in pixels");
    markerWeightReg.editorType = EditorType::DoubleSpinBox;
    markerWeightReg.defaultValue = m_markerWeight;
    markerWeightReg.minValue = 0.5;
    markerWeightReg.maxValue = 10.0;
    markerWeightReg.stepValue = 0.5;
    markerWeightReg.propertyName = "markerWeight";
    markerWeightReg.target = this;
    registerProperty(markerWeightReg);
    
    PropertyRegistration markerFillColorReg;
    markerFillColorReg.category = tr("Quad");
    markerFillColorReg.subcategory = tr("Marker");
    markerFillColorReg.displayName = tr("Fill Color");
    markerFillColorReg.briefDesc = tr("Marker fill color");
    markerFillColorReg.detailDesc = tr("Sets the fill color of markers");
    markerFillColorReg.editorType = EditorType::ColorPicker;
    markerFillColorReg.defaultValue = m_markerFillColor;
    markerFillColorReg.propertyName = "markerFillColor";
    markerFillColorReg.target = this;
    registerProperty(markerFillColorReg);
    
    PropertyRegistration markerOutlineColorReg;
    markerOutlineColorReg.category = tr("Quad");
    markerOutlineColorReg.subcategory = tr("Marker");
    markerOutlineColorReg.displayName = tr("Outline Color");
    markerOutlineColorReg.briefDesc = tr("Marker outline color");
    markerOutlineColorReg.detailDesc = tr("Sets the outline color of markers");
    markerOutlineColorReg.editorType = EditorType::ColorPicker;
    markerOutlineColorReg.defaultValue = m_markerOutlineColor;
    markerOutlineColorReg.propertyName = "markerOutlineColor";
    markerOutlineColorReg.target = this;
    registerProperty(markerOutlineColorReg);
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
 * \endif
 * 
 * \if CHINESE
 * @brief 在指定的图表控件中创建绘图节点
 * @param figure 将要创建绘图的图表控件指针
 * @details 创建一个包含四边形元素（单位立方体，6 个面）的 3D 图表。
 * \endif
 */
void Plot3DQuadFunction::createPlot(QIM::QImFigureWidget* figure)
{
    if (!figure) {
        return;
    }
    
    figure->setSubplot3DGrid(1, 1);
    m_plot3DNode = figure->createPlot3DNode();
    
    m_plot3DNode->xAxis()->setLabel(m_xLabel);
    m_plot3DNode->yAxis()->setLabel(m_yLabel);
    m_plot3DNode->zAxis()->setLabel(m_zLabel);
    m_plot3DNode->setTitle(m_title);
    m_plot3DNode->setBoxRotation(35.264, 45.0);
    
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
    
    m_quad3DNode = new QIM::QImPlot3DQuadItemNode(m_plot3DNode);
    m_quad3DNode->setData(xs, ys, zs);
    m_quad3DNode->setFillColor(m_fillColor);
    m_quad3DNode->setLineColor(m_lineColor);
    m_quad3DNode->setLineWidth(m_lineWidth);
    m_quad3DNode->setLinesVisible(m_linesVisible);
    m_quad3DNode->setFillVisible(m_fillVisible);
    m_quad3DNode->setMarkersVisible(m_markersVisible);
    m_quad3DNode->setFillAlpha(m_fillAlpha);
}

void Plot3DQuadFunction::setTitle(const QString& title)
{
    if (m_title != title) {
        m_title = title;
        Q_EMIT titleChanged(title);
        if (m_plot3DNode) { m_plot3DNode->setTitle(title); }
    }
}

void Plot3DQuadFunction::setXLabel(const QString& label)
{
    if (m_xLabel != label) {
        m_xLabel = label;
        Q_EMIT xLabelChanged(label);
        if (m_plot3DNode) { m_plot3DNode->xAxis()->setLabel(label); }
    }
}

void Plot3DQuadFunction::setYLabel(const QString& label)
{
    if (m_yLabel != label) {
        m_yLabel = label;
        Q_EMIT yLabelChanged(label);
        if (m_plot3DNode) { m_plot3DNode->yAxis()->setLabel(label); }
    }
}

void Plot3DQuadFunction::setZLabel(const QString& label)
{
    if (m_zLabel != label) {
        m_zLabel = label;
        Q_EMIT zLabelChanged(label);
        if (m_plot3DNode) { m_plot3DNode->zAxis()->setLabel(label); }
    }
}

void Plot3DQuadFunction::setFillColor(const QColor& color)
{
    if (m_fillColor != color) {
        m_fillColor = color;
        Q_EMIT fillColorChanged(color);
        if (m_quad3DNode) { m_quad3DNode->setFillColor(color); }
    }
}

void Plot3DQuadFunction::setLineColor(const QColor& color)
{
    if (m_lineColor != color) {
        m_lineColor = color;
        Q_EMIT lineColorChanged(color);
        if (m_quad3DNode) { m_quad3DNode->setLineColor(color); }
    }
}

void Plot3DQuadFunction::setLineWidth(float width)
{
    if (m_lineWidth != width) {
        m_lineWidth = width;
        Q_EMIT lineWidthChanged(width);
        if (m_quad3DNode) { m_quad3DNode->setLineWidth(width); }
    }
}

void Plot3DQuadFunction::setLinesVisible(bool visible)
{
    if (m_linesVisible != visible) {
        m_linesVisible = visible;
        Q_EMIT linesVisibleChanged(visible);
        if (m_quad3DNode) { m_quad3DNode->setLinesVisible(visible); }
    }
}

void Plot3DQuadFunction::setFillVisible(bool visible)
{
    if (m_fillVisible != visible) {
        m_fillVisible = visible;
        Q_EMIT fillVisibleChanged(visible);
        if (m_quad3DNode) { m_quad3DNode->setFillVisible(visible); }
    }
}

void Plot3DQuadFunction::setMarkersVisible(bool visible)
{
    if (m_markersVisible != visible) {
        m_markersVisible = visible;
        Q_EMIT markersVisibleChanged(visible);
        if (m_quad3DNode) { m_quad3DNode->setMarkersVisible(visible); }
    }
}

void Plot3DQuadFunction::setMarkerShape(const QString& shape)
{
    if (m_markerShape != shape) {
        m_markerShape = shape;
        Q_EMIT markerShapeChanged(shape);
        if (m_quad3DNode) {
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
            m_quad3DNode->setMarkerShape(enumVal);
        }
    }
}

void Plot3DQuadFunction::setMarkerSize(float size)
{
    if (m_markerSize != size) {
        m_markerSize = size;
        Q_EMIT markerSizeChanged(size);
        if (m_quad3DNode) { m_quad3DNode->setMarkerSize(size); }
    }
}

void Plot3DQuadFunction::setMarkerWeight(float weight)
{
    if (m_markerWeight != weight) {
        m_markerWeight = weight;
        Q_EMIT markerWeightChanged(weight);
        if (m_quad3DNode) { m_quad3DNode->setMarkerWeight(weight); }
    }
}

void Plot3DQuadFunction::setMarkerFillColor(const QColor& color)
{
    if (m_markerFillColor != color) {
        m_markerFillColor = color;
        Q_EMIT markerFillColorChanged(color);
        if (m_quad3DNode) { m_quad3DNode->setMarkerFillColor(color); }
    }
}

void Plot3DQuadFunction::setMarkerOutlineColor(const QColor& color)
{
    if (m_markerOutlineColor != color) {
        m_markerOutlineColor = color;
        Q_EMIT markerOutlineColorChanged(color);
        if (m_quad3DNode) { m_quad3DNode->setMarkerOutlineColor(color); }
    }
}

void Plot3DQuadFunction::setFillAlpha(float alpha)
{
    if (m_fillAlpha != alpha) {
        m_fillAlpha = alpha;
        Q_EMIT fillAlphaChanged(alpha);
        if (m_quad3DNode) { m_quad3DNode->setFillAlpha(alpha); }
    }
}

void Plot3DQuadFunction::cleanupPlot()
{
    TestFunction::cleanupPlot();
    m_plot3DNode = nullptr;
    m_quad3DNode = nullptr;
}