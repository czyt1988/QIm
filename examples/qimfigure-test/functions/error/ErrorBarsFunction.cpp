#include "ErrorBarsFunction.h"
#include "QImFigureWidget.h"
#include "plot/QImPlotNode.h"
#include "plot/QImPlotScatterItemNode.h"
#include "plot/QImPlotErrorBarsItemNode.h"
#include "plot/QImPlotValueTrackerNode.h"

/**
 * \if ENGLISH
 * @brief Constructor for ErrorBarsFunction
 * @param parent Parent object
 * @details Initializes the test function and registers properties for interactive editing.
 * Properties are organized in categories:
 * - "Plot"/"Basic Info"/"Title" - Plot title
 * - "Plot"/"Axis"/"X Label" - X axis label
 * - "Plot"/"Axis"/"Y Label" - Y axis label
 * - "Error Bars"/"Style"/"Color" - Error bars color
 * - "Error Bars"/"Mode"/"Horizontal" - Horizontal mode toggle
 * \endif
 * 
 * \if CHINESE
 * @brief ErrorBarsFunction 构造函数
 * @param parent 父对象
 * @details 初始化测试函数并注册属性用于交互式编辑。
 * 属性按类别组织：
 * - "Plot"/"Basic Info"/"Title" - 图表标题
 * - "Plot"/"Axis"/"X Label" - X 轴标签
 * - "Plot"/"Axis"/"Y Label" - Y 轴标签
 * - "Error Bars"/"Style"/"Color" - 误差棒颜色
 * - "Error Bars"/"Mode"/"Horizontal" - 水平模式切换
 * \endif
 */
ErrorBarsFunction::ErrorBarsFunction(QObject* parent)
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
    
    // Register error color property
    PropertyRegistration colorReg;
    colorReg.category = tr("Error Bars");
    colorReg.subcategory = tr("Style");
    colorReg.displayName = tr("Color");
    colorReg.briefDesc = tr("Error bars color");
    colorReg.detailDesc = tr("Sets the color of the error bars");
    colorReg.editorType = EditorType::ColorPicker;
    colorReg.defaultValue = m_errorColor;
    colorReg.propertyName = "errorColor";
    colorReg.target = this;
    registerProperty(colorReg);
    
    // Register horizontal mode property
    PropertyRegistration horizontalReg;
    horizontalReg.category = tr("Error Bars");
    horizontalReg.subcategory = tr("Mode");
    horizontalReg.displayName = tr("Horizontal");
    horizontalReg.briefDesc = tr("Horizontal mode");
    horizontalReg.detailDesc = tr("When enabled, error bars are displayed horizontally instead of vertically");
    horizontalReg.editorType = EditorType::CheckBox;
    horizontalReg.defaultValue = m_horizontalMode;
    horizontalReg.propertyName = "horizontalMode";
    horizontalReg.target = this;
    registerProperty(horizontalReg);
}

/**
 * \if ENGLISH
 * @brief Destructor for ErrorBarsFunction
 * \endif
 * 
 * \if CHINESE
 * @brief ErrorBarsFunction 析构函数
 * \endif
 */
ErrorBarsFunction::~ErrorBarsFunction()
{
}

/**
 * \if ENGLISH
 * @brief Create plot nodes in the given figure widget
 * @param figure Pointer to the figure widget where plots will be created
 * @details Creates a plot with 10 data points showing:
 * - Symmetric error bars (same error in both directions)
 * - Asymmetric error bars with horizontal mode
 * - Two scatter plots with different marker shapes
 * - Value tracker for interactive data inspection
 * \endif
 * 
 * \if CHINESE
 * @brief 在指定的图表控件中创建绘图节点
 * @param figure 将要创建绘图的图表控件指针
 * @details 创建一个包含 10 个数据点的图表，展示：
 * - 对称误差棒（两个方向误差相同）
 * - 非对称误差棒及水平模式
 * - 两个使用不同标记形状的散点图
 * - 用于交互式数据检查的值追踪器
 * \endif
 */
void ErrorBarsFunction::createPlot(QIM::QImFigureWidget* figure)
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
    
    // Generate data - 10 points with error bars
    const int numPoints = 10;
    std::vector<double> xData(numPoints);
    std::vector<double> yData(numPoints);
    std::vector<double> errors(numPoints);
    std::vector<double> negErrors(numPoints);
    std::vector<double> posErrors(numPoints);
    
    for (int i = 0; i < numPoints; ++i) {
        xData[i] = i;
        yData[i] = static_cast<double>(i * i) / 5.0 + 2.0;
        errors[i] = 0.5 + static_cast<double>(i) * 0.1;           // Symmetric error
        negErrors[i] = 0.3 + static_cast<double>(i) * 0.05;       // Negative error (smaller)
        posErrors[i] = 0.7 + static_cast<double>(i) * 0.15;       // Positive error (larger)
    }
    
    // Add first scatter plot as base data
    m_scatterNode1 = new QIM::QImPlotScatterItemNode(m_plotNode);
    m_scatterNode1->setLabel(tr("Data Points"));
    m_scatterNode1->setData(xData, yData);
    m_scatterNode1->setMarkerSize(6.0f);
    m_scatterNode1->setMarkerShape(ImPlotMarker_Circle);
    m_scatterNode1->setColor(Qt::blue);
    
    // Add symmetric error bars
    m_errorBarsNode1 = new QIM::QImPlotErrorBarsItemNode(m_plotNode);
    m_errorBarsNode1->setLabel(tr("Symmetric Errors"));
    m_errorBarsNode1->setData(xData, yData, errors);
    m_errorBarsNode1->setColor(m_errorColor);
    
    // Add second scatter plot with X offset to avoid overlap
    std::vector<double> xOffset(numPoints);
    for (int i = 0; i < numPoints; ++i) {
        xOffset[i] = xData[i] + 0.3;
    }
    
    m_scatterNode2 = new QIM::QImPlotScatterItemNode(m_plotNode);
    m_scatterNode2->setLabel(tr("Data Points 2"));
    m_scatterNode2->setData(xOffset, yData);
    m_scatterNode2->setMarkerSize(6.0f);
    m_scatterNode2->setMarkerShape(ImPlotMarker_Square);
    m_scatterNode2->setColor(Qt::green);
    
    // Add asymmetric horizontal error bars
    m_errorBarsNode2 = new QIM::QImPlotErrorBarsItemNode(m_plotNode);
    m_errorBarsNode2->setLabel(tr("Asymmetric Horizontal"));
    m_errorBarsNode2->setData(xOffset, yData, negErrors, posErrors);
    m_errorBarsNode2->setHorizontal(m_horizontalMode);
    m_errorBarsNode2->setColor(Qt::darkGreen);
    
    // Add value tracker
    m_trackerNode = new QIM::QImPlotValueTrackerNode(m_plotNode);
    m_trackerNode->setGroup(nullptr);  // No group by default
    m_plotNode->addChildNode(m_trackerNode);
}

void ErrorBarsFunction::setTitle(const QString& title)
{
    if (m_title != title) {
        m_title = title;
        emit titleChanged(title);
        if (m_plotNode) {
            m_plotNode->setTitle(title);
        }
    }
}

void ErrorBarsFunction::setXLabel(const QString& label)
{
    if (m_xLabel != label) {
        m_xLabel = label;
        emit xLabelChanged(label);
        if (m_plotNode) {
            m_plotNode->x1Axis()->setLabel(label);
        }
    }
}

void ErrorBarsFunction::setYLabel(const QString& label)
{
    if (m_yLabel != label) {
        m_yLabel = label;
        emit yLabelChanged(label);
        if (m_plotNode) {
            m_plotNode->y1Axis()->setLabel(label);
        }
    }
}

void ErrorBarsFunction::setErrorColor(const QColor& color)
{
    if (m_errorColor != color) {
        m_errorColor = color;
        emit errorColorChanged(color);
        if (m_errorBarsNode1) {
            m_errorBarsNode1->setColor(color);
        }
    }
}

void ErrorBarsFunction::setHorizontalMode(bool horizontal)
{
    if (m_horizontalMode != horizontal) {
        m_horizontalMode = horizontal;
        emit horizontalModeChanged(horizontal);
        if (m_errorBarsNode2) {
            m_errorBarsNode2->setHorizontal(horizontal);
        }
    }
}
