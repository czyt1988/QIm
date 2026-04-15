#include "DragRectFunction.h"
#include "QImFigureWidget.h"
#include "plot/QImPlotNode.h"
#include "plot/QImPlotAxisInfo.h"
#include "plot/QImPlotLineItemNode.h"
#include "plot/QImPlotDragRectNode.h"
#include <vector>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/**
 * \if ENGLISH
 * @brief Constructor for DragRectFunction
 * @param parent Parent object
 * @details Initializes the test function and registers properties for interactive editing.
 * Properties are organized in categories:
 * - "Plot"/"Basic Info"/"Title" - Plot title
 * - "DragRect"/"Position"/"X1" - Rectangle left X coordinate
 * - "DragRect"/"Position"/"Y1" - Rectangle bottom Y coordinate
 * - "DragRect"/"Position"/"X2" - Rectangle right X coordinate
 * - "DragRect"/"Position"/"Y2" - Rectangle top Y coordinate
 * - "DragRect"/"Style"/"Color" - Rectangle border color
 * \endif
 * 
 * \if CHINESE
 * @brief DragRectFunction 构造函数
 * @param parent 父对象
 * @details 初始化测试函数并注册属性用于交互式编辑。
 * 属性按类别组织：
 * - "Plot"/"Basic Info"/"Title" - 图表标题
 * - "DragRect"/"Position"/"X1" - 矩形左侧 X 坐标
 * - "DragRect"/"Position"/"Y1" - 矩形底部 Y 坐标
 * - "DragRect"/"Position"/"X2" - 矩形右侧 X 坐标
 * - "DragRect"/"Position"/"Y2" - 矩形顶部 Y 坐标
 * - "DragRect"/"Style"/"Color" - 矩形边框颜色
 * \endif
 */
DragRectFunction::DragRectFunction(QObject* parent)
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
    
    // Register X1 property
    PropertyRegistration x1Reg;
    x1Reg.category = tr("DragRect");
    x1Reg.subcategory = tr("Position");
    x1Reg.displayName = tr("X1");
    x1Reg.briefDesc = tr("Left X coordinate");
    x1Reg.detailDesc = tr("Sets the left X coordinate of the draggable rectangle");
    x1Reg.editorType = EditorType::DoubleSpinBox;
    x1Reg.defaultValue = m_rect.left();
    x1Reg.minValue = 0.0;
    x1Reg.maxValue = 10.0;
    x1Reg.stepValue = 0.1;
    x1Reg.propertyName = "rectX1";
    x1Reg.target = this;
    registerProperty(x1Reg);
    
    // Register Y1 property
    PropertyRegistration y1Reg;
    y1Reg.category = tr("DragRect");
    y1Reg.subcategory = tr("Position");
    y1Reg.displayName = tr("Y1");
    y1Reg.briefDesc = tr("Bottom Y coordinate");
    y1Reg.detailDesc = tr("Sets the bottom Y coordinate of the draggable rectangle");
    y1Reg.editorType = EditorType::DoubleSpinBox;
    y1Reg.defaultValue = m_rect.top();
    y1Reg.minValue = 0.0;
    y1Reg.maxValue = 10.0;
    y1Reg.stepValue = 0.1;
    y1Reg.propertyName = "rectY1";
    y1Reg.target = this;
    registerProperty(y1Reg);
    
    // Register X2 property
    PropertyRegistration x2Reg;
    x2Reg.category = tr("DragRect");
    x2Reg.subcategory = tr("Position");
    x2Reg.displayName = tr("X2");
    x2Reg.briefDesc = tr("Right X coordinate");
    x2Reg.detailDesc = tr("Sets the right X coordinate of the draggable rectangle");
    x2Reg.editorType = EditorType::DoubleSpinBox;
    x2Reg.defaultValue = m_rect.right();
    x2Reg.minValue = 0.0;
    x2Reg.maxValue = 10.0;
    x2Reg.stepValue = 0.1;
    x2Reg.propertyName = "rectX2";
    x2Reg.target = this;
    registerProperty(x2Reg);
    
    // Register Y2 property
    PropertyRegistration y2Reg;
    y2Reg.category = tr("DragRect");
    y2Reg.subcategory = tr("Position");
    y2Reg.displayName = tr("Y2");
    y2Reg.briefDesc = tr("Top Y coordinate");
    y2Reg.detailDesc = tr("Sets the top Y coordinate of the draggable rectangle");
    y2Reg.editorType = EditorType::DoubleSpinBox;
    y2Reg.defaultValue = m_rect.bottom();
    y2Reg.minValue = 0.0;
    y2Reg.maxValue = 10.0;
    y2Reg.stepValue = 0.1;
    y2Reg.propertyName = "rectY2";
    y2Reg.target = this;
    registerProperty(y2Reg);
    
    // Register rect color property
    PropertyRegistration colorReg;
    colorReg.category = tr("DragRect");
    colorReg.subcategory = tr("Style");
    colorReg.displayName = tr("Color");
    colorReg.briefDesc = tr("Rectangle border color");
    colorReg.detailDesc = tr("Sets the color of the draggable rectangle border");
    colorReg.editorType = EditorType::ColorPicker;
    colorReg.defaultValue = m_rectColor;
    colorReg.propertyName = "rectColor";
    colorReg.target = this;
    registerProperty(colorReg);
    
    // Register cursors enabled property
    PropertyRegistration cursorsEnabledReg;
    cursorsEnabledReg.category = tr("DragRect");
    cursorsEnabledReg.subcategory = tr("Flags");
    cursorsEnabledReg.displayName = tr("Cursors Enabled");
    cursorsEnabledReg.briefDesc = tr("Show cursor guides");
    cursorsEnabledReg.detailDesc = tr("When enabled, cursor guide lines are shown while dragging.");
    cursorsEnabledReg.editorType = EditorType::CheckBox;
    cursorsEnabledReg.defaultValue = m_cursorsEnabled;
    cursorsEnabledReg.propertyName = "cursorsEnabled";
    cursorsEnabledReg.target = this;
    registerProperty(cursorsEnabledReg);
    
    // Register fit enabled property
    PropertyRegistration fitEnabledReg;
    fitEnabledReg.category = tr("DragRect");
    fitEnabledReg.subcategory = tr("Flags");
    fitEnabledReg.displayName = tr("Fit Enabled");
    fitEnabledReg.briefDesc = tr("Auto-fit plot on drag");
    fitEnabledReg.detailDesc = tr("When enabled, the plot automatically fits to include the dragged rectangle.");
    fitEnabledReg.editorType = EditorType::CheckBox;
    fitEnabledReg.defaultValue = m_fitEnabled;
    fitEnabledReg.propertyName = "fitEnabled";
    fitEnabledReg.target = this;
    registerProperty(fitEnabledReg);
    
    // Register inputs enabled property
    PropertyRegistration inputsEnabledReg;
    inputsEnabledReg.category = tr("DragRect");
    inputsEnabledReg.subcategory = tr("Flags");
    inputsEnabledReg.displayName = tr("Inputs Enabled");
    inputsEnabledReg.briefDesc = tr("Enable mouse input");
    inputsEnabledReg.detailDesc = tr("When enabled, the drag tool responds to mouse input.");
    inputsEnabledReg.editorType = EditorType::CheckBox;
    inputsEnabledReg.defaultValue = m_inputsEnabled;
    inputsEnabledReg.propertyName = "inputsEnabled";
    inputsEnabledReg.target = this;
    registerProperty(inputsEnabledReg);
    
    // Register delayed property
    PropertyRegistration delayedReg;
    delayedReg.category = tr("DragRect");
    delayedReg.subcategory = tr("Flags");
    delayedReg.displayName = tr("Delayed");
    delayedReg.briefDesc = tr("Delayed rendering");
    delayedReg.detailDesc = tr("When enabled, the drag tool uses delayed rendering mode for smoother interaction.");
    delayedReg.editorType = EditorType::CheckBox;
    delayedReg.defaultValue = m_delayed;
    delayedReg.propertyName = "delayed";
    delayedReg.target = this;
    registerProperty(delayedReg);
}

/**
 * \if ENGLISH
 * @brief Destructor for DragRectFunction
 * \endif
 * 
 * \if CHINESE
 * @brief DragRectFunction 析构函数
 * \endif
 */
DragRectFunction::~DragRectFunction()
{
}

/**
 * \if ENGLISH
 * @brief Create plot nodes in the given figure widget
 * @param figure Pointer to the figure widget where plots will be created
 * @details Creates a plot with a sine curve as reference and a draggable rectangle.
 * The rectangle can be moved by dragging the center, and resized by dragging corners.
 * \endif
 * 
 * \if CHINESE
 * @brief 在指定的图表控件中创建绘图节点
 * @param figure 将要创建绘图的图表控件指针
 * @details 创建一个包含正弦曲线作为参考和可拖拽矩形的图表。
 * 矩形可以通过拖拽中心移动，通过拖拽角点调整大小。
 * \endif
 */
void DragRectFunction::createPlot(QIM::QImFigureWidget* figure)
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
    
    // Generate sine curve as reference
    const int numPoints = 100;
    std::vector<double> xData(numPoints);
    std::vector<double> yData(numPoints);
    
    for (int i = 0; i < numPoints; ++i) {
        xData[i] = i * 0.1;
        yData[i] = std::sin(xData[i] * M_PI) * 3.0 + 5.0;
    }
    
    // Create line item node as reference curve
    m_lineNode = new QIM::QImPlotLineItemNode(m_plotNode);
    m_lineNode->setData(xData, yData);
    m_lineNode->setColor(QColor(100, 150, 255));
    m_plotNode->addChildNode(m_lineNode);
    
    // Create draggable rectangle
    m_dragRectNode = new QIM::QImPlotDragRectNode(m_plotNode);
    m_dragRectNode->setRect(m_rect.left(), m_rect.top(), m_rect.right(), m_rect.bottom());
    m_dragRectNode->setColor(m_rectColor);
    m_dragRectNode->setId(0);
    m_dragRectNode->setCursorsEnabled(m_cursorsEnabled);
    m_dragRectNode->setFitEnabled(m_fitEnabled);
    m_dragRectNode->setInputsEnabled(m_inputsEnabled);
    m_dragRectNode->setDelayed(m_delayed);
    m_plotNode->addChildNode(m_dragRectNode);
}

void DragRectFunction::setTitle(const QString& title)
{
    if (m_title != title) {
        m_title = title;
        emit titleChanged(title);
        if (m_plotNode) {
            m_plotNode->setTitle(title);
        }
    }
}

void DragRectFunction::setRect(const QRectF& rect)
{
    if (m_rect != rect) {
        m_rect = rect;
        emit rectChanged(rect);
        if (m_dragRectNode) {
            m_dragRectNode->setRect(rect.left(), rect.top(), rect.right(), rect.bottom());
        }
    }
}

void DragRectFunction::setRectColor(const QColor& color)
{
    if (m_rectColor != color) {
        m_rectColor = color;
        emit rectColorChanged(color);
        if (m_dragRectNode) {
            m_dragRectNode->setColor(color);
        }
    }
}

void DragRectFunction::setCursorsEnabled(bool enabled)
{
    if (m_cursorsEnabled != enabled) {
        m_cursorsEnabled = enabled;
        Q_EMIT cursorsEnabledChanged(enabled);
        if (m_dragRectNode) {
            m_dragRectNode->setCursorsEnabled(enabled);
        }
    }
}

void DragRectFunction::setFitEnabled(bool enabled)
{
    if (m_fitEnabled != enabled) {
        m_fitEnabled = enabled;
        Q_EMIT fitEnabledChanged(enabled);
        if (m_dragRectNode) {
            m_dragRectNode->setFitEnabled(enabled);
        }
    }
}

void DragRectFunction::setInputsEnabled(bool enabled)
{
    if (m_inputsEnabled != enabled) {
        m_inputsEnabled = enabled;
        Q_EMIT inputsEnabledChanged(enabled);
        if (m_dragRectNode) {
            m_dragRectNode->setInputsEnabled(enabled);
        }
    }
}

void DragRectFunction::setDelayed(bool on)
{
    if (m_delayed != on) {
        m_delayed = on;
        Q_EMIT delayedChanged(on);
        if (m_dragRectNode) {
            m_dragRectNode->setDelayed(on);
        }
    }
}
