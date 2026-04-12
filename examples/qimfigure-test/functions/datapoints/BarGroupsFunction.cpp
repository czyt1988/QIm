#include "BarGroupsFunction.h"
#include "QImFigureWidget.h"
#include "plot/QImPlotNode.h"
#include "plot/QImPlotAxisInfo.h"
#include "plot/QImPlotBarGroupsItemNode.h"
#include "plot/QImPlotValueTrackerNode.h"
#include <vector>

/**
 * \if ENGLISH
 * @brief Constructor for BarGroupsFunction
 * @param parent Parent object
 * @details Initializes the test function and registers properties for interactive editing.
 * Properties are organized in categories:
 * - "Plot"/"Basic Info"/"Title" - Plot title
 * - "Plot"/"Axis"/"X Label" - X axis label
 * - "Plot"/"Axis"/"Y Label" - Y axis label
 * - "Bar Groups"/"Style"/"Group Width" - Group width
 * - "Bar Groups"/"Style"/"Horizontal" - Horizontal orientation
 * - "Bar Groups"/"Style"/"Stacked" - Stacked bars
 * - "Bar Groups"/"Style"/"Shift" - Shift offset
 * - "Bar Groups"/"Style"/"Color" - Bar color
 * \endif
 * 
 * \if CHINESE
 * @brief BarGroupsFunction 构造函数
 * @param parent 父对象
 * @details 初始化测试函数并注册属性用于交互式编辑。
 * 属性按类别组织：
 * - "Plot"/"Basic Info"/"Title" - 图表标题
 * - "Plot"/"Axis"/"X Label" - X 轴标签
 * - "Plot"/"Axis"/"Y Label" - Y 轴标签
 * - "Bar Groups"/"Style"/"Group Width" - 组宽
 * - "Bar Groups"/"Style"/"Horizontal" - 水平方向
 * - "Bar Groups"/"Style"/"Stacked" - 堆叠柱状图
 * - "Bar Groups"/"Style"/"Shift" - 偏移量
 * - "Bar Groups"/"Style"/"Color" - 柱子颜色
 * \endif
 */
BarGroupsFunction::BarGroupsFunction(QObject* parent)
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
    
    // Register group width property
    PropertyRegistration groupWidthReg;
    groupWidthReg.category = tr("Bar Groups");
    groupWidthReg.subcategory = tr("Style");
    groupWidthReg.displayName = tr("Group Width");
    groupWidthReg.briefDesc = tr("Width of groups in plot units");
    groupWidthReg.detailDesc = tr("Sets the visual width of groups in the grouped bar chart (0.1-2.0)");
    groupWidthReg.editorType = EditorType::DoubleSpinBox;
    groupWidthReg.defaultValue = m_groupWidth;
    groupWidthReg.minValue = 0.1;
    groupWidthReg.maxValue = 2.0;
    groupWidthReg.stepValue = 0.05;
    groupWidthReg.propertyName = "groupWidth";
    groupWidthReg.target = this;
    registerProperty(groupWidthReg);
    
    // Register horizontal orientation property
    PropertyRegistration horizontalReg;
    horizontalReg.category = tr("Bar Groups");
    horizontalReg.subcategory = tr("Style");
    horizontalReg.displayName = tr("Horizontal");
    horizontalReg.briefDesc = tr("Horizontal orientation");
    horizontalReg.detailDesc = tr("When checked, bars are rendered horizontally instead of vertically");
    horizontalReg.editorType = EditorType::CheckBox;
    horizontalReg.defaultValue = m_horizontal;
    horizontalReg.propertyName = "horizontal";
    horizontalReg.target = this;
    registerProperty(horizontalReg);
    
    // Register stacked bars property
    PropertyRegistration stackedReg;
    stackedReg.category = tr("Bar Groups");
    stackedReg.subcategory = tr("Style");
    stackedReg.displayName = tr("Stacked");
    stackedReg.briefDesc = tr("Stacked bars");
    stackedReg.detailDesc = tr("When checked, bars within each group are stacked on top of each other");
    stackedReg.editorType = EditorType::CheckBox;
    stackedReg.defaultValue = m_stacked;
    stackedReg.propertyName = "stacked";
    stackedReg.target = this;
    registerProperty(stackedReg);
    
    // Register shift offset property
    PropertyRegistration shiftReg;
    shiftReg.category = tr("Bar Groups");
    shiftReg.subcategory = tr("Style");
    shiftReg.displayName = tr("Shift");
    shiftReg.briefDesc = tr("Shift offset");
    shiftReg.detailDesc = tr("Sets the horizontal shift offset for groups in plot units (-1.0 to 1.0)");
    shiftReg.editorType = EditorType::DoubleSpinBox;
    shiftReg.defaultValue = m_shift;
    shiftReg.minValue = -1.0;
    shiftReg.maxValue = 1.0;
    shiftReg.stepValue = 0.05;
    shiftReg.propertyName = "shift";
    shiftReg.target = this;
    registerProperty(shiftReg);
    
    // Register bar color property
    PropertyRegistration colorReg;
    colorReg.category = tr("Bar Groups");
    colorReg.subcategory = tr("Style");
    colorReg.displayName = tr("Color");
    colorReg.briefDesc = tr("Bar color");
    colorReg.detailDesc = tr("Sets the color of the bars");
    colorReg.editorType = EditorType::ColorPicker;
    colorReg.defaultValue = m_barColor;
    colorReg.propertyName = "barColor";
    colorReg.target = this;
    registerProperty(colorReg);
}

/**
 * \if ENGLISH
 * @brief Destructor for BarGroupsFunction
 * \endif
 * 
 * \if CHINESE
 * @brief BarGroupsFunction 析构函数
 * \endif
 */
BarGroupsFunction::~BarGroupsFunction()
{
}

/**
 * \if ENGLISH
 * @brief Create plot nodes in the given figure widget
 * @param figure Pointer to the figure widget where plots will be created
 * @details Creates a grouped bar chart with 3 items (A, B, C) and 4 groups (Q1-Q4),
 * configures axes, adds a bar groups item, and attaches a value tracker node.
 * Sample data represents quarterly performance of 3 products.
 * \endif
 * 
 * \if CHINESE
 * @brief 在指定的图表控件中创建绘图节点
 * @param figure 将要创建绘图的图表控件指针
 * @details 创建一个包含 3 个项目（A、B、C）和 4 个组（Q1-Q4）的分组柱状图，
 * 配置坐标轴，添加分组柱状图项，并附加一个值追踪器节点。
 * 示例数据表示 3 个产品的季度性能。
 * \endif
 */
void BarGroupsFunction::createPlot(QIM::QImFigureWidget* figure)
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
    
    // Create grouped bar chart data - 3 items, 4 groups
    const int numItems = 3;
    const int numGroups = 4;
    
    // Item labels (A, B, C)
    QStringList itemLabels;
    itemLabels << "A" << "B" << "C";
    
    // Values matrix in row-major order: 3 items × 4 groups = 12 values
    // Item A: Q1=10, Q2=20, Q3=15, Q4=25
    // Item B: Q1=15, Q2=25, Q3=20, Q4=30
    // Item C: Q1=12, Q2=18, Q3=22, Q4=28
    QVector<double> values = {
        10.0, 20.0, 15.0, 25.0,  // Item A values for groups 1-4
        15.0, 25.0, 20.0, 30.0,  // Item B values for groups 1-4
        12.0, 18.0, 22.0, 28.0   // Item C values for groups 1-4
    };
    
    // Create bar groups item node and set data
    m_barGroupsNode = new QIM::QImPlotBarGroupsItemNode(m_plotNode);
    m_barGroupsNode->setLabel("Product Performance");
    m_barGroupsNode->setData(itemLabels, values, numItems, numGroups);
    m_barGroupsNode->setGroupWidth(m_groupWidth);
    m_barGroupsNode->setHorizontal(m_horizontal);
    m_barGroupsNode->setStacked(m_stacked);
    m_barGroupsNode->setShift(m_shift);
    m_barGroupsNode->setColor(m_barColor);
    
    // Create and attach value tracker
    m_trackerNode = new QIM::QImPlotValueTrackerNode(m_plotNode);
    m_trackerNode->setGroup(nullptr);  // No group by default
    m_plotNode->addChildNode(m_trackerNode);
}

void BarGroupsFunction::setTitle(const QString& title)
{
    if (m_title != title) {
        m_title = title;
        emit titleChanged(title);
        if (m_plotNode) {
            m_plotNode->setTitle(title);
        }
    }
}

void BarGroupsFunction::setXLabel(const QString& label)
{
    if (m_xLabel != label) {
        m_xLabel = label;
        emit xLabelChanged(label);
        if (m_plotNode) {
            m_plotNode->x1Axis()->setLabel(label);
        }
    }
}

void BarGroupsFunction::setYLabel(const QString& label)
{
    if (m_yLabel != label) {
        m_yLabel = label;
        emit yLabelChanged(label);
        if (m_plotNode) {
            m_plotNode->y1Axis()->setLabel(label);
        }
    }
}

void BarGroupsFunction::setGroupWidth(double width)
{
    if (m_groupWidth != width) {
        m_groupWidth = width;
        emit groupWidthChanged(width);
        if (m_barGroupsNode) {
            m_barGroupsNode->setGroupWidth(width);
        }
    }
}

void BarGroupsFunction::setHorizontal(bool horizontal)
{
    if (m_horizontal != horizontal) {
        m_horizontal = horizontal;
        emit horizontalChanged(horizontal);
        if (m_barGroupsNode) {
            m_barGroupsNode->setHorizontal(horizontal);
        }
    }
}

void BarGroupsFunction::setStacked(bool stacked)
{
    if (m_stacked != stacked) {
        m_stacked = stacked;
        emit stackedChanged(stacked);
        if (m_barGroupsNode) {
            m_barGroupsNode->setStacked(stacked);
        }
    }
}

void BarGroupsFunction::setShift(double shift)
{
    if (m_shift != shift) {
        m_shift = shift;
        emit shiftChanged(shift);
        if (m_barGroupsNode) {
            m_barGroupsNode->setShift(shift);
        }
    }
}

void BarGroupsFunction::setBarColor(const QColor& color)
{
    if (m_barColor != color) {
        m_barColor = color;
        emit barColorChanged(color);
        if (m_barGroupsNode) {
            m_barGroupsNode->setColor(color);
        }
    }
}