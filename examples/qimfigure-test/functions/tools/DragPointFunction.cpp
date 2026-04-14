#include "DragPointFunction.h"
#include "QImFigureWidget.h"
#include "plot/QImPlotNode.h"
#include "plot/QImPlotAxisInfo.h"
#include "plot/QImPlotLineItemNode.h"
#include "plot/QImPlotDragPointNode.h"
#include <vector>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/**
 * \if ENGLISH
 * @brief Constructor for DragPointFunction
 * @param parent Parent object
 * @details Initializes the test function and registers properties for interactive editing.
 * Properties are organized in categories:
 * - "Plot"/"Basic Info"/"Title" - Plot title
 * - "DragPoint"/"Position"/"Position" - Point position (x, y)
 * - "DragPoint"/"Style"/"Color" - Point color
 * - "DragPoint"/"Style"/"Size" - Point size
 * \endif
 * 
 * \if CHINESE
 * @brief DragPointFunction 构造函数
 * @param parent 父对象
 * @details 初始化测试函数并注册属性用于交互式编辑。
 * 属性按类别组织：
 * - "Plot"/"Basic Info"/"Title" - 图表标题
 * - "DragPoint"/"Position"/"Position" - 点位置 (x, y)
 * - "DragPoint"/"Style"/"Color" - 点颜色
 * - "DragPoint"/"Style"/"Size" - 点大小
 * \endif
 */
DragPointFunction::DragPointFunction(QObject* parent)
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
    
    // Register point position X property
    PropertyRegistration posXReg;
    posXReg.category = tr("DragPoint");
    posXReg.subcategory = tr("Position");
    posXReg.displayName = tr("X Position");
    posXReg.briefDesc = tr("Point X coordinate");
    posXReg.detailDesc = tr("Sets the X coordinate of the draggable point in plot units");
    posXReg.editorType = EditorType::DoubleSpinBox;
    posXReg.defaultValue = m_pointPosition.x();
    posXReg.minValue = 0.0;
    posXReg.maxValue = 10.0;
    posXReg.stepValue = 0.1;
    posXReg.propertyName = "pointPositionX";
    posXReg.target = this;
    registerProperty(posXReg);
    
    // Register point position Y property
    PropertyRegistration posYReg;
    posYReg.category = tr("DragPoint");
    posYReg.subcategory = tr("Position");
    posYReg.displayName = tr("Y Position");
    posYReg.briefDesc = tr("Point Y coordinate");
    posYReg.detailDesc = tr("Sets the Y coordinate of the draggable point in plot units");
    posYReg.editorType = EditorType::DoubleSpinBox;
    posYReg.defaultValue = m_pointPosition.y();
    posYReg.minValue = 0.0;
    posYReg.maxValue = 10.0;
    posYReg.stepValue = 0.1;
    posYReg.propertyName = "pointPositionY";
    posYReg.target = this;
    registerProperty(posYReg);
    
    // Register point color property
    PropertyRegistration colorReg;
    colorReg.category = tr("DragPoint");
    colorReg.subcategory = tr("Style");
    colorReg.displayName = tr("Color");
    colorReg.briefDesc = tr("Point color");
    colorReg.detailDesc = tr("Sets the color of the draggable point marker");
    colorReg.editorType = EditorType::ColorPicker;
    colorReg.defaultValue = m_pointColor;
    colorReg.propertyName = "pointColor";
    colorReg.target = this;
    registerProperty(colorReg);
    
    // Register point size property
    PropertyRegistration sizeReg;
    sizeReg.category = tr("DragPoint");
    sizeReg.subcategory = tr("Style");
    sizeReg.displayName = tr("Size");
    sizeReg.briefDesc = tr("Point size");
    sizeReg.detailDesc = tr("Sets the visual size of the draggable point marker in pixels");
    sizeReg.editorType = EditorType::DoubleSpinBox;
    sizeReg.defaultValue = m_pointSize;
    sizeReg.minValue = 1.0;
    sizeReg.maxValue = 20.0;
    sizeReg.stepValue = 1.0;
    sizeReg.propertyName = "pointSize";
    sizeReg.target = this;
    registerProperty(sizeReg);
}

/**
 * \if ENGLISH
 * @brief Destructor for DragPointFunction
 * \endif
 * 
 * \if CHINESE
 * @brief DragPointFunction 析构函数
 * \endif
 */
DragPointFunction::~DragPointFunction()
{
}

/**
 * \if ENGLISH
 * @brief Create plot nodes in the given figure widget
 * @param figure Pointer to the figure widget where plots will be created
 * @details Creates a plot with a sine curve as reference and a draggable point.
 * The point can be dragged to any position within the plot area.
 * \endif
 * 
 * \if CHINESE
 * @brief 在指定的图表控件中创建绘图节点
 * @param figure 将要创建绘图的图表控件指针
 * @details 创建一个包含正弦曲线作为参考和可拖拽点的图表。
 * 点可以拖拽到绘图区域内的任意位置。
 * \endif
 */
void DragPointFunction::createPlot(QIM::QImFigureWidget* figure)
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
    
    // Create draggable point
    m_dragPointNode = new QIM::QImPlotDragPointNode(m_plotNode);
    m_dragPointNode->setPosition(m_pointPosition);
    m_dragPointNode->setColor(m_pointColor);
    m_dragPointNode->setSize(m_pointSize);
    m_dragPointNode->setId(0);
    m_plotNode->addChildNode(m_dragPointNode);
}

void DragPointFunction::setTitle(const QString& title)
{
    if (m_title != title) {
        m_title = title;
        emit titleChanged(title);
        if (m_plotNode) {
            m_plotNode->setTitle(title);
        }
    }
}

void DragPointFunction::setPointPosition(const QPointF& pos)
{
    if (m_pointPosition != pos) {
        m_pointPosition = pos;
        emit pointPositionChanged(pos);
        if (m_dragPointNode) {
            m_dragPointNode->setPosition(pos);
        }
    }
}

void DragPointFunction::setPointColor(const QColor& color)
{
    if (m_pointColor != color) {
        m_pointColor = color;
        emit pointColorChanged(color);
        if (m_dragPointNode) {
            m_dragPointNode->setColor(color);
        }
    }
}

void DragPointFunction::setPointSize(float size)
{
    if (m_pointSize != size) {
        m_pointSize = size;
        emit pointSizeChanged(size);
        if (m_dragPointNode) {
            m_dragPointNode->setSize(size);
        }
    }
}