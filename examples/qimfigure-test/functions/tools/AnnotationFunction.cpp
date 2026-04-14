#include "AnnotationFunction.h"
#include "QImFigureWidget.h"
#include "plot/QImPlotNode.h"
#include "plot/QImPlotAxisInfo.h"
#include "plot/QImPlotLineItemNode.h"
#include "plot/QImPlotDragPointNode.h"
#include "plot/QImPlotAnnotationNode.h"
#include <vector>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/**
 * \if ENGLISH
 * @brief Constructor for AnnotationFunction
 * @param parent Parent object
 * @details Initializes the test function and registers properties for interactive editing.
 * Properties are organized in categories:
 * - "Plot"/"Basic Info"/"Title" - Plot title
 * - "Annotation"/"Content"/"Text" - Annotation text label
 * - "Annotation"/"Style"/"Color" - Annotation text color
 * - "Annotation"/"Options"/"Clamp" - Clamp annotation within plot area
 * \endif
 * 
 * \if CHINESE
 * @brief AnnotationFunction 构造函数
 * @param parent 父对象
 * @details 初始化测试函数并注册属性用于交互式编辑。
 * 属性按类别组织：
 * - "Plot"/"Basic Info"/"Title" - 图表标题
 * - "Annotation"/"Content"/"Text" - 注释文本标签
 * - "Annotation"/"Style"/"Color" - 注释文本颜色
 * - "Annotation"/"Options"/"Clamp" - 将注释钳位在绘图区域内
 * \endif
 */
AnnotationFunction::AnnotationFunction(QObject* parent)
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
    
    // Register annotation text property
    PropertyRegistration textReg;
    textReg.category = tr("Annotation");
    textReg.subcategory = tr("Content");
    textReg.displayName = tr("Text");
    textReg.briefDesc = tr("Annotation text");
    textReg.detailDesc = tr("Sets the text label displayed by the annotation");
    textReg.editorType = EditorType::LineEdit;
    textReg.defaultValue = m_annotationText;
    textReg.propertyName = "annotationText";
    textReg.target = this;
    registerProperty(textReg);
    
    // Register annotation color property
    PropertyRegistration colorReg;
    colorReg.category = tr("Annotation");
    colorReg.subcategory = tr("Style");
    colorReg.displayName = tr("Color");
    colorReg.briefDesc = tr("Annotation color");
    colorReg.detailDesc = tr("Sets the color of the annotation text");
    colorReg.editorType = EditorType::ColorPicker;
    colorReg.defaultValue = m_annotationColor;
    colorReg.propertyName = "annotationColor";
    colorReg.target = this;
    registerProperty(colorReg);
    
    // Register clamp property
    PropertyRegistration clampReg;
    clampReg.category = tr("Annotation");
    clampReg.subcategory = tr("Options");
    clampReg.displayName = tr("Clamp");
    clampReg.briefDesc = tr("Clamp within plot");
    clampReg.detailDesc = tr("When enabled, annotation stays within the plot area bounds");
    clampReg.editorType = EditorType::CheckBox;
    clampReg.defaultValue = m_clamp;
    clampReg.propertyName = "clamp";
    clampReg.target = this;
    registerProperty(clampReg);
}

/**
 * \if ENGLISH
 * @brief Destructor for AnnotationFunction
 * \endif
 * 
 * \if CHINESE
 * @brief AnnotationFunction 析构函数
 * \endif
 */
AnnotationFunction::~AnnotationFunction()
{
}

/**
 * \if ENGLISH
 * @brief Create plot nodes in the given figure widget
 * @param figure Pointer to the figure widget where plots will be created
 * @details Creates a plot with a sine curve, a draggable point, and an annotation.
 * The annotation position is linked to the drag point via signal-slot connection.
 * \endif
 * 
 * \if CHINESE
 * @brief 在指定的图表控件中创建绘图节点
 * @param figure 将要创建绘图的图表控件指针
 * @details 创建一个包含正弦曲线、可拖拽点和注释的图表。
 * 注释位置通过信号槽连接与拖拽点联动。
 * \endif
 */
void AnnotationFunction::createPlot(QIM::QImFigureWidget* figure)
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
    m_dragPointNode->setColor(QColor(255, 100, 100));
    m_dragPointNode->setSize(8.0f);
    m_dragPointNode->setId(0);
    
    // Connect drag point position change signal to update annotation
    connect(m_dragPointNode, &QIM::QImPlotDragPointNode::positionChanged,
            this, &AnnotationFunction::onDragPointMoved);
    m_plotNode->addChildNode(m_dragPointNode);
    
    // Create annotation linked to drag point
    m_annotationNode = new QIM::QImPlotAnnotationNode(m_plotNode);
    m_annotationNode->setPosition(m_pointPosition);
    m_annotationNode->setText(m_annotationText);
    m_annotationNode->setColor(m_annotationColor);
    m_annotationNode->setPixelOffset(20.0, -20.0);
    m_annotationNode->setClamp(m_clamp);
    m_plotNode->addChildNode(m_annotationNode);
}

void AnnotationFunction::onDragPointMoved(const QPointF& pos)
{
    m_pointPosition = pos;
    if (m_annotationNode) {
        m_annotationNode->setPosition(pos);
    }
}

void AnnotationFunction::setTitle(const QString& title)
{
    if (m_title != title) {
        m_title = title;
        emit titleChanged(title);
        if (m_plotNode) {
            m_plotNode->setTitle(title);
        }
    }
}

void AnnotationFunction::setAnnotationText(const QString& text)
{
    if (m_annotationText != text) {
        m_annotationText = text;
        emit annotationTextChanged(text);
        if (m_annotationNode) {
            m_annotationNode->setText(text);
        }
    }
}

void AnnotationFunction::setAnnotationColor(const QColor& color)
{
    if (m_annotationColor != color) {
        m_annotationColor = color;
        emit annotationColorChanged(color);
        if (m_annotationNode) {
            m_annotationNode->setColor(color);
        }
    }
}

void AnnotationFunction::setClamp(bool enabled)
{
    if (m_clamp != enabled) {
        m_clamp = enabled;
        emit clampChanged(enabled);
        if (m_annotationNode) {
            m_annotationNode->setClamp(enabled);
        }
    }
}