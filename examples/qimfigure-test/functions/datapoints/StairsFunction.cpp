#include "StairsFunction.h"
#include "QImFigureWidget.h"
#include "plot/QImPlotNode.h"
#include "plot/QImPlotAxisInfo.h"
#include "plot/QImPlotStairsItemNode.h"
#include "plot/QImPlotValueTrackerNode.h"
#include <QColor>

/**
 * \if ENGLISH
 * @brief Constructor for StairsFunction
 * @param parent Parent object
 * @details Initializes the test function and registers properties for interactive editing.
 * Properties are organized in categories:
 * - "Plot"/"Basic Info"/"Title" - Plot title
 * - "Plot"/"Axis"/"X Label" - X axis label
 * - "Plot"/"Axis"/"Y Label" - Y axis label
 * - "Stairs"/"Style"/"Color" - Stairs color
 * - "Stairs"/"Style"/"Shaded" - Shaded mode toggle
 * \endif
 * 
 * \if CHINESE
 * @brief StairsFunction 构造函数
 * @param parent 父对象
 * @details 初始化测试函数并注册属性用于交互式编辑。
 * 属性按类别组织：
 * - "Plot"/"Basic Info"/"Title" - 图表标题
 * - "Plot"/"Axis"/"X Label" - X 轴标签
 * - "Plot"/"Axis"/"Y Label" - Y 轴标签
 * - "Stairs"/"Style"/"Color" - 阶梯颜色
 * - "Stairs"/"Style"/"Shaded" - 填充模式开关
 * \endif
 */
StairsFunction::StairsFunction(QObject* parent)
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
    
    // Register stairs color property
    PropertyRegistration colorReg;
    colorReg.category = tr("Stairs");
    colorReg.subcategory = tr("Style");
    colorReg.displayName = tr("Color");
    colorReg.briefDesc = tr("Stairs color");
    colorReg.detailDesc = tr("Sets the color of the stairs plot");
    colorReg.editorType = EditorType::ColorPicker;
    colorReg.defaultValue = m_stairsColor;
    colorReg.propertyName = "stairsColor";
    colorReg.target = this;
    registerProperty(colorReg);
    
    // Register shaded property
    PropertyRegistration shadedReg;
    shadedReg.category = tr("Stairs");
    shadedReg.subcategory = tr("Style");
    shadedReg.displayName = tr("Shaded");
    shadedReg.briefDesc = tr("Shaded mode");
    shadedReg.detailDesc = tr("Enables or disables shaded mode for the stairs plot");
    shadedReg.editorType = EditorType::CheckBox;
    shadedReg.defaultValue = m_shaded;
    shadedReg.propertyName = "shaded";
    shadedReg.target = this;
    registerProperty(shadedReg);
}

/**
 * \if ENGLISH
 * @brief Destructor for StairsFunction
 * \endif
 * 
 * \if CHINESE
 * @brief StairsFunction 析构函数
 * \endif
 */
StairsFunction::~StairsFunction()
{
}

/**
 * \if ENGLISH
 * @brief Create plot nodes in the given figure widget
 * @param figure Pointer to the figure widget where plots will be created
 * @details Creates a plot with 10 stair points, configures axes,
 * adds a stairs item, and attaches a value tracker node.
 * \endif
 * 
 * \if CHINESE
 * @brief 在指定的图表控件中创建绘图节点
 * @param figure 将要创建绘图的图表控件指针
 * @details 创建一个包含 10 个阶梯点的图表，配置坐标轴，
 * 添加阶梯项，并附加一个值追踪器节点。
 * \endif
 */
void StairsFunction::createPlot(QIM::QImFigureWidget* figure)
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
    m_plotNode->x1Axis()->setLabel(m_xLabel);
    m_plotNode->y1Axis()->setLabel(m_yLabel);
    m_plotNode->setTitle(m_title);
    m_plotNode->setLegendEnabled(true);
    
    // Generate 10-point stair data
    const int numPoints = 10;
    std::vector<double> xData(numPoints);
    std::vector<double> yData(numPoints);
    
    for (int i = 0; i < numPoints; ++i) {
        xData[i] = i;
        yData[i] = static_cast<double>(i % 3) + 1.0;
    }
    
    // Create stairs item node and set data
    m_stairsNode = new QIM::QImPlotStairsItemNode(m_plotNode);
    m_stairsNode->setLabel(tr("Stairs Plot"));
    m_stairsNode->setData(xData, yData);
    m_stairsNode->setColor(m_stairsColor);
    m_stairsNode->setShaded(m_shaded);
    m_plotNode->addChildNode(m_stairsNode);
    
    // Create and attach value tracker
    m_trackerNode = new QIM::QImPlotValueTrackerNode(m_plotNode);
    m_trackerNode->setGroup(nullptr);  // No group by default
    m_plotNode->addChildNode(m_trackerNode);
}

void StairsFunction::setTitle(const QString& title)
{
    if (m_title != title) {
        m_title = title;
        emit titleChanged(title);
        if (m_plotNode) {
            m_plotNode->setTitle(title);
        }
    }
}

void StairsFunction::setXLabel(const QString& label)
{
    if (m_xLabel != label) {
        m_xLabel = label;
        emit xLabelChanged(label);
        if (m_plotNode) {
            m_plotNode->x1Axis()->setLabel(label);
        }
    }
}

void StairsFunction::setYLabel(const QString& label)
{
    if (m_yLabel != label) {
        m_yLabel = label;
        emit yLabelChanged(label);
        if (m_plotNode) {
            m_plotNode->y1Axis()->setLabel(label);
        }
    }
}

void StairsFunction::setStairsColor(const QColor& color)
{
    if (m_stairsColor != color) {
        m_stairsColor = color;
        emit stairsColorChanged(color);
        if (m_stairsNode) {
            m_stairsNode->setColor(color);
        }
    }
}

void StairsFunction::setShaded(bool shaded)
{
    if (m_shaded != shaded) {
        m_shaded = shaded;
        emit shadedChanged(shaded);
        if (m_stairsNode) {
            m_stairsNode->setShaded(shaded);
        }
    }
}
