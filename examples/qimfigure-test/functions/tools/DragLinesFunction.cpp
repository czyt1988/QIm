#include "DragLinesFunction.h"
#include "QImFigureWidget.h"
#include "plot/QImPlotNode.h"
#include "plot/QImPlotAxisInfo.h"
#include "plot/QImPlotLineItemNode.h"
#include "plot/QImPlotDragLineXNode.h"
#include "plot/QImPlotDragLineYNode.h"
#include <vector>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/**
 * \if ENGLISH
 * @brief Constructor for DragLinesFunction
 * @param parent Parent object
 * @details Initializes the test function and registers properties for interactive editing.
 * Properties are organized in categories:
 * - "Plot"/"Basic Info"/"Title" - Plot title
 * - "DragLineX"/"Position"/"X Value" - Vertical line X position
 * - "DragLineX"/"Style"/"Color" - Vertical line color
 * - "DragLineY"/"Position"/"Y Value" - Horizontal line Y position
 * - "DragLineY"/"Style"/"Color" - Horizontal line color
 * - "Common"/"Style"/"Thickness" - Line thickness for both
 * \endif
 * 
 * \if CHINESE
 * @brief DragLinesFunction 构造函数
 * @param parent 父对象
 * @details 初始化测试函数并注册属性用于交互式编辑。
 * 属性按类别组织：
 * - "Plot"/"Basic Info"/"Title" - 图表标题
 * - "DragLineX"/"Position"/"X Value" - 垂直线 X 位置
 * - "DragLineX"/"Style"/"Color" - 垂直线颜色
 * - "DragLineY"/"Position"/"Y Value" - 水平线 Y 位置
 * - "DragLineY"/"Style"/"Color" - 水平线颜色
 * - "Common"/"Style"/"Thickness" - 线条粗细
 * \endif
 */
DragLinesFunction::DragLinesFunction(QObject* parent)
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
    
    // Register DragLineX value property
    PropertyRegistration lineXValueReg;
    lineXValueReg.category = tr("DragLineX");
    lineXValueReg.subcategory = tr("Position");
    lineXValueReg.displayName = tr("X Value");
    lineXValueReg.briefDesc = tr("Vertical line position");
    lineXValueReg.detailDesc = tr("Sets the X coordinate of the vertical draggable line");
    lineXValueReg.editorType = EditorType::DoubleSpinBox;
    lineXValueReg.defaultValue = m_lineXValue;
    lineXValueReg.minValue = 0.0;
    lineXValueReg.maxValue = 10.0;
    lineXValueReg.stepValue = 0.1;
    lineXValueReg.propertyName = "lineXValue";
    lineXValueReg.target = this;
    registerProperty(lineXValueReg);
    
    // Register DragLineX color property
    PropertyRegistration lineXColorReg;
    lineXColorReg.category = tr("DragLineX");
    lineXColorReg.subcategory = tr("Style");
    lineXColorReg.displayName = tr("Color");
    lineXColorReg.briefDesc = tr("Vertical line color");
    lineXColorReg.detailDesc = tr("Sets the color of the vertical draggable line");
    lineXColorReg.editorType = EditorType::ColorPicker;
    lineXColorReg.defaultValue = m_lineXColor;
    lineXColorReg.propertyName = "lineXColor";
    lineXColorReg.target = this;
    registerProperty(lineXColorReg);
    
    // Register DragLineY value property
    PropertyRegistration lineYValueReg;
    lineYValueReg.category = tr("DragLineY");
    lineYValueReg.subcategory = tr("Position");
    lineYValueReg.displayName = tr("Y Value");
    lineYValueReg.briefDesc = tr("Horizontal line position");
    lineYValueReg.detailDesc = tr("Sets the Y coordinate of the horizontal draggable line");
    lineYValueReg.editorType = EditorType::DoubleSpinBox;
    lineYValueReg.defaultValue = m_lineYValue;
    lineYValueReg.minValue = 0.0;
    lineYValueReg.maxValue = 10.0;
    lineYValueReg.stepValue = 0.1;
    lineYValueReg.propertyName = "lineYValue";
    lineYValueReg.target = this;
    registerProperty(lineYValueReg);
    
    // Register DragLineY color property
    PropertyRegistration lineYColorReg;
    lineYColorReg.category = tr("DragLineY");
    lineYColorReg.subcategory = tr("Style");
    lineYColorReg.displayName = tr("Color");
    lineYColorReg.briefDesc = tr("Horizontal line color");
    lineYColorReg.detailDesc = tr("Sets the color of the horizontal draggable line");
    lineYColorReg.editorType = EditorType::ColorPicker;
    lineYColorReg.defaultValue = m_lineYColor;
    lineYColorReg.propertyName = "lineYColor";
    lineYColorReg.target = this;
    registerProperty(lineYColorReg);
    
    // Register line thickness property
    PropertyRegistration thicknessReg;
    thicknessReg.category = tr("Common");
    thicknessReg.subcategory = tr("Style");
    thicknessReg.displayName = tr("Thickness");
    thicknessReg.briefDesc = tr("Line thickness");
    thicknessReg.detailDesc = tr("Sets the thickness of both draggable lines");
    thicknessReg.editorType = EditorType::DoubleSpinBox;
    thicknessReg.defaultValue = m_lineThickness;
    thicknessReg.minValue = 1.0;
    thicknessReg.maxValue = 10.0;
    thicknessReg.stepValue = 0.5;
    thicknessReg.propertyName = "lineThickness";
    thicknessReg.target = this;
    registerProperty(thicknessReg);
}

/**
 * \if ENGLISH
 * @brief Destructor for DragLinesFunction
 * \endif
 * 
 * \if CHINESE
 * @brief DragLinesFunction 析构函数
 * \endif
 */
DragLinesFunction::~DragLinesFunction()
{
}

/**
 * \if ENGLISH
 * @brief Create plot nodes in the given figure widget
 * @param figure Pointer to the figure widget where plots will be created
 * @details Creates a plot with a sine curve and two draggable lines (vertical and horizontal).
 * The lines intersect to form a crosshair marker.
 * \endif
 * 
 * \if CHINESE
 * @brief 在指定的图表控件中创建绘图节点
 * @param figure 将要创建绘图的图表控件指针
 * @details 创建一个包含正弦曲线和两条可拖拽线（垂直和水平）的图表。
 * 线条相交形成十字线标记。
 * \endif
 */
void DragLinesFunction::createPlot(QIM::QImFigureWidget* figure)
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
    m_plotNode->x1Axis()->setLabel(tr("x"));
    m_plotNode->y1Axis()->setLabel(tr("y"));
    m_plotNode->setTitle(m_title);
    
    // Generate sine curve
    const int numPoints = 100;
    std::vector<double> xData(numPoints);
    std::vector<double> yData(numPoints);
    
    for (int i = 0; i < numPoints; ++i) {
        xData[i] = i * 0.1;
        yData[i] = std::sin(xData[i] * M_PI) * 3.0 + 5.0;
    }
    
    // Create line item node
    m_lineNode = new QIM::QImPlotLineItemNode(m_plotNode);
    m_lineNode->setData(xData, yData);
    m_lineNode->setColor(QColor(100, 150, 255));
    m_plotNode->addChildNode(m_lineNode);
    
    // Create vertical draggable line (DragLineX)
    m_dragLineXNode = new QIM::QImPlotDragLineXNode(m_plotNode);
    m_dragLineXNode->setValue(m_lineXValue);
    m_dragLineXNode->setColor(m_lineXColor);
    m_dragLineXNode->setThickness(m_lineThickness);
    m_dragLineXNode->setId(0);
    m_plotNode->addChildNode(m_dragLineXNode);
    
    // Create horizontal draggable line (DragLineY)
    m_dragLineYNode = new QIM::QImPlotDragLineYNode(m_plotNode);
    m_dragLineYNode->setValue(m_lineYValue);
    m_dragLineYNode->setColor(m_lineYColor);
    m_dragLineYNode->setThickness(m_lineThickness);
    m_dragLineYNode->setId(1);
    m_plotNode->addChildNode(m_dragLineYNode);
}

void DragLinesFunction::setTitle(const QString& title)
{
    if (m_title != title) {
        m_title = title;
        emit titleChanged(title);
        if (m_plotNode) {
            m_plotNode->setTitle(title);
        }
    }
}

void DragLinesFunction::setLineXValue(double value)
{
    if (m_lineXValue != value) {
        m_lineXValue = value;
        emit lineXValueChanged(value);
        if (m_dragLineXNode) {
            m_dragLineXNode->setValue(value);
        }
    }
}

void DragLinesFunction::setLineYValue(double value)
{
    if (m_lineYValue != value) {
        m_lineYValue = value;
        emit lineYValueChanged(value);
        if (m_dragLineYNode) {
            m_dragLineYNode->setValue(value);
        }
    }
}

void DragLinesFunction::setLineXColor(const QColor& color)
{
    if (m_lineXColor != color) {
        m_lineXColor = color;
        emit lineXColorChanged(color);
        if (m_dragLineXNode) {
            m_dragLineXNode->setColor(color);
        }
    }
}

void DragLinesFunction::setLineYColor(const QColor& color)
{
    if (m_lineYColor != color) {
        m_lineYColor = color;
        emit lineYColorChanged(color);
        if (m_dragLineYNode) {
            m_dragLineYNode->setColor(color);
        }
    }
}

void DragLinesFunction::setLineThickness(float thickness)
{
    if (m_lineThickness != thickness) {
        m_lineThickness = thickness;
        emit lineThicknessChanged(thickness);
        if (m_dragLineXNode) {
            m_dragLineXNode->setThickness(thickness);
        }
        if (m_dragLineYNode) {
            m_dragLineYNode->setThickness(thickness);
        }
    }
}