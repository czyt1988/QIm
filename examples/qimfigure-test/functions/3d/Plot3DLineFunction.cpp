#include "Plot3DLineFunction.h"
#include "QImFigureWidget.h"
#include "plot3d/QImPlot3DExtNode.h"
#include "plot3d/QImPlot3DAxisInfo.h"
#include "plot3d/QImPlot3DExtLineItemNode.h"
#include "plot3d/QImPlot3DDataSeries.h"
#include <QVector>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/**
 * \if ENGLISH
 * @brief Constructor for Plot3DLineFunction
 * @param parent Parent object
 * @details Initializes the test function and registers properties for interactive editing.
 * Properties are organized in categories:
 * - "Plot"/"Basic Info"/"Title" - Plot title
 * - "Plot"/"Axis"/"X Label" - X axis label
 * - "Plot"/"Axis"/"Y Label" - Y axis label
 * - "Plot"/"Axis"/"Z Label" - Z axis label
 * - "Line"/"Style"/"Color" - Line color
 * - "Line"/"Style"/"Weight" - Line weight
 * \endif
 * 
 * \if CHINESE
 * @brief Plot3DLineFunction 构造函数
 * @param parent 父对象
 * @details 初始化测试函数并注册属性用于交互式编辑。
 * 属性按类别组织：
 * - "Plot"/"Basic Info"/"Title" - 图表标题
 * - "Plot"/"Axis"/"X Label" - X 轴标签
 * - "Plot"/"Axis"/"Y Label" - Y 轴标签
 * - "Plot"/"Axis"/"Z Label" - Z 轴标签
 * - "Line"/"Style"/"Color" - 线条颜色
 * - "Line"/"Style"/"Weight" - 线宽
 * \endif
 */
Plot3DLineFunction::Plot3DLineFunction(QObject* parent)
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
    
    // Register line color property
    PropertyRegistration colorReg;
    colorReg.category = tr("Line");
    colorReg.subcategory = tr("Style");
    colorReg.displayName = tr("Color");
    colorReg.briefDesc = tr("Line color");
    colorReg.detailDesc = tr("Sets the color of the 3D line curve");
    colorReg.editorType = EditorType::ColorPicker;
    colorReg.defaultValue = m_lineColor;
    colorReg.propertyName = "lineColor";
    colorReg.target = this;
    registerProperty(colorReg);
    
    // Register line weight property
    PropertyRegistration weightReg;
    weightReg.category = tr("Line");
    weightReg.subcategory = tr("Style");
    weightReg.displayName = tr("Weight");
    weightReg.briefDesc = tr("Line weight");
    weightReg.detailDesc = tr("Sets the line weight (thickness) in pixels");
    weightReg.editorType = EditorType::DoubleSpinBox;
    weightReg.defaultValue = m_lineWeight;
    weightReg.minValue = 0.1;
    weightReg.maxValue = 10.0;
    weightReg.stepValue = 0.1;
    weightReg.propertyName = "lineWeight";
    weightReg.target = this;
    registerProperty(weightReg);
}

/**
 * \if ENGLISH
 * @brief Destructor for Plot3DLineFunction
 * \endif
 * 
 * \if CHINESE
 * @brief Plot3DLineFunction 析构函数
 * \endif
 */
Plot3DLineFunction::~Plot3DLineFunction()
{
}

/**
 * \if ENGLISH
 * @brief Create plot nodes in the given figure widget
 * @param figure Pointer to the figure widget where plots will be created
 * @details Creates a 3D plot with a spiral line curve, configures axes,
 * and adds a 3D line item.
 * The spiral is generated as a parametric curve:
 * - x = cos(t)
 * - y = sin(t)
 * - z = t/10
 * for t in [0, 10π] (1000 points)
 * \endif
 * 
 * \if CHINESE
 * @brief 在指定的图表控件中创建绘图节点
 * @param figure 将要创建绘图的图表控件指针
 * @details 创建一个包含螺旋线的 3D 图表，配置坐标轴，
 * 并添加 3D 线条元素。
 * 螺旋线作为参数曲线生成：
 * - x = cos(t)
 * - y = sin(t)
 * - z = t/10
 * t 范围为 [0, 10π]（1000 个点）
 * \endif
 */
void Plot3DLineFunction::createPlot(QIM::QImFigureWidget* figure)
{
    if (!figure) {
        return;
    }
    
    // Create 3D plot node as a top-level render node via QImFigureWidget API
    m_plot3DNode = figure->createPlotNode3D();
    
    // Configure axes and title
    m_plot3DNode->xAxis()->setLabel(m_xLabel);
    m_plot3DNode->yAxis()->setLabel(m_yLabel);
    m_plot3DNode->zAxis()->setLabel(m_zLabel);
    m_plot3DNode->setTitle(m_title);
    
    // Generate 3D spiral data: x = cos(t), y = sin(t), z = t/10 for t in [0, 10π]
    const int numPoints = 1000;
    QVector<double> xs, ys, zs;
    xs.reserve(numPoints);
    ys.reserve(numPoints);
    zs.reserve(numPoints);
    
    for (int i = 0; i < numPoints; ++i) {
        double t = i * 0.01 * M_PI * 10;  // t from 0 to 10π
        xs.append(std::cos(t));
        ys.append(std::sin(t));
        zs.append(t / 10.0);
    }
    
    // Create 3D line item node and set data
    m_line3DNode = new QIM::QImPlot3DExtLineItemNode(m_plot3DNode);
    m_line3DNode->setData(xs, ys, zs);
    m_line3DNode->setColor(m_lineColor);
    m_line3DNode->setLineWeight(m_lineWeight);
}

void Plot3DLineFunction::setTitle(const QString& title)
{
    if (m_title != title) {
        m_title = title;
        Q_EMIT titleChanged(title);
        if (m_plot3DNode) {
            m_plot3DNode->setTitle(title);
        }
    }
}

void Plot3DLineFunction::setXLabel(const QString& label)
{
    if (m_xLabel != label) {
        m_xLabel = label;
        Q_EMIT xLabelChanged(label);
        if (m_plot3DNode) {
            m_plot3DNode->xAxis()->setLabel(label);
        }
    }
}

void Plot3DLineFunction::setYLabel(const QString& label)
{
    if (m_yLabel != label) {
        m_yLabel = label;
        Q_EMIT yLabelChanged(label);
        if (m_plot3DNode) {
            m_plot3DNode->yAxis()->setLabel(label);
        }
    }
}

void Plot3DLineFunction::setZLabel(const QString& label)
{
    if (m_zLabel != label) {
        m_zLabel = label;
        Q_EMIT zLabelChanged(label);
        if (m_plot3DNode) {
            m_plot3DNode->zAxis()->setLabel(label);
        }
    }
}

void Plot3DLineFunction::setLineColor(const QColor& color)
{
    if (m_lineColor != color) {
        m_lineColor = color;
        Q_EMIT lineColorChanged(color);
        if (m_line3DNode) {
            m_line3DNode->setColor(color);
        }
    }
}

void Plot3DLineFunction::setLineWeight(float weight)
{
    if (m_lineWeight != weight) {
        m_lineWeight = weight;
        Q_EMIT lineWeightChanged(weight);
        if (m_line3DNode) {
            m_line3DNode->setLineWeight(weight);
        }
    }
}
