#include "ScatterFunction.h"
#include "QImFigureWidget.h"
#include "plot/QImPlotNode.h"
#include "plot/QImPlotAxisInfo.h"
#include "plot/QImPlotScatterItemNode.h"
#include "plot/QImPlotValueTrackerNode.h"
#include <random>

/**
 * \if ENGLISH
 * @brief Constructor for ScatterFunction
 * @param parent Parent object
 * @details Initializes the test function and registers properties for interactive editing.
 * Properties are organized in categories:
 * - "Plot"/"Basic Info"/"Title" - Plot title
 * - "Plot"/"Axis"/"X Label" - X axis label
 * - "Plot"/"Axis"/"Y Label" - Y axis label
 * - "Scatter"/"Marker"/"Size" - Marker size
 * - "Scatter"/"Marker"/"Shape" - Marker shape
 * - "Scatter"/"Marker"/"Fill" - Marker fill
 * - "Scatter"/"Marker"/"Color" - Marker color
 * \endif
 * 
 * \if CHINESE
 * @brief ScatterFunction 构造函数
 * @param parent 父对象
 * @details 初始化测试函数并注册属性用于交互式编辑。
 * 属性按类别组织：
 * - "Plot"/"Basic Info"/"Title" - 图表标题
 * - "Plot"/"Axis"/"X Label" - X 轴标签
 * - "Plot"/"Axis"/"Y Label" - Y 轴标签
 * - "Scatter"/"Marker"/"Size" - 标记大小
 * - "Scatter"/"Marker"/"Shape" - 标记形状
 * - "Scatter"/"Marker"/"Fill" - 标记填充
 * - "Scatter"/"Marker"/"Color" - 标记颜色
 * \endif
 */
ScatterFunction::ScatterFunction(QObject* parent)
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
    
    // Register marker size property
    PropertyRegistration sizeReg;
    sizeReg.category = tr("Scatter");
    sizeReg.subcategory = tr("Marker");
    sizeReg.displayName = tr("Size");
    sizeReg.briefDesc = tr("Marker size");
    sizeReg.detailDesc = tr("Sets the size of scatter plot markers in pixels");
    sizeReg.editorType = EditorType::DoubleSpinBox;
    sizeReg.defaultValue = m_markerSize;
    sizeReg.minValue = 1.0;
    sizeReg.maxValue = 20.0;
    sizeReg.stepValue = 0.5;
    sizeReg.propertyName = "markerSize";
    sizeReg.target = this;
    registerProperty(sizeReg);
    
    // Register marker shape property
    PropertyRegistration shapeReg;
    shapeReg.category = tr("Scatter");
    shapeReg.subcategory = tr("Marker");
    shapeReg.displayName = tr("Shape");
    shapeReg.briefDesc = tr("Marker shape");
    shapeReg.detailDesc = tr("Sets the shape of scatter plot markers (0=Circle, 1=Square, 2=Diamond, etc.)");
    shapeReg.editorType = EditorType::SpinBox;
    shapeReg.defaultValue = m_markerShape;
    shapeReg.minValue = 0;
    shapeReg.maxValue = 10;
    shapeReg.stepValue = 1;
    shapeReg.propertyName = "markerShape";
    shapeReg.target = this;
    registerProperty(shapeReg);
    
    // Register marker fill property
    PropertyRegistration fillReg;
    fillReg.category = tr("Scatter");
    fillReg.subcategory = tr("Marker");
    fillReg.displayName = tr("Fill");
    fillReg.briefDesc = tr("Marker fill");
    fillReg.detailDesc = tr("Sets whether markers are filled or outline only");
    fillReg.editorType = EditorType::CheckBox;
    fillReg.defaultValue = m_markerFill;
    fillReg.propertyName = "markerFill";
    fillReg.target = this;
    registerProperty(fillReg);
    
    // Register marker color property
    PropertyRegistration colorReg;
    colorReg.category = tr("Scatter");
    colorReg.subcategory = tr("Marker");
    colorReg.displayName = tr("Color");
    colorReg.briefDesc = tr("Marker color");
    colorReg.detailDesc = tr("Sets the color of scatter plot markers");
    colorReg.editorType = EditorType::ColorPicker;
    colorReg.defaultValue = m_markerColor;
    colorReg.propertyName = "markerColor";
    colorReg.target = this;
    registerProperty(colorReg);
}

/**
 * \if ENGLISH
 * @brief Destructor for ScatterFunction
 * \endif
 * 
 * \if CHINESE
 * @brief ScatterFunction 析构函数
 * \endif
 */
ScatterFunction::~ScatterFunction()
{
}

/**
 * \if ENGLISH
 * @brief Create plot nodes in the given figure widget
 * @param figure Pointer to the figure widget where plots will be created
 * @details Creates a plot with 1000 random scatter points, configures axes,
 * adds a scatter item with marker settings, and attaches a value tracker node.
 * \endif
 * 
 * \if CHINESE
 * @brief 在指定的图表控件中创建绘图节点
 * @param figure 将要创建绘图的图表控件指针
 * @details 创建一个包含 1000 个随机散点的图表，配置坐标轴，
 * 添加带有标记设置的散点项，并附加一个值追踪器节点。
 * \endif
 */
void ScatterFunction::createPlot(QIM::QImFigureWidget* figure)
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
    
    // Generate 1000 random scatter points
    const int numPoints = 1000;
    std::vector<double> xData(numPoints);
    std::vector<double> yData(numPoints);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> xDist(0.0, 1.0);
    std::normal_distribution<double> yDist(0.0, 1.0);
    
    for (int i = 0; i < numPoints; ++i) {
        xData[i] = xDist(gen);
        yData[i] = yDist(gen);
    }
    
    // Create scatter item node and set data
    m_scatterNode = new QIM::QImPlotScatterItemNode(m_plotNode);
    m_scatterNode->setData(xData, yData);
    m_scatterNode->setMarkerSize(m_markerSize);
    m_scatterNode->setMarkerShape(m_markerShape);
    m_scatterNode->setMarkerFill(m_markerFill);
    m_scatterNode->setColor(m_markerColor);
    
    // Create and attach value tracker
    m_trackerNode = new QIM::QImPlotValueTrackerNode(m_plotNode);
    m_trackerNode->setGroup(nullptr);  // No group by default
    m_plotNode->addChildNode(m_trackerNode);
}

void ScatterFunction::setTitle(const QString& title)
{
    if (m_title != title) {
        m_title = title;
        emit titleChanged(title);
        if (m_plotNode) {
            m_plotNode->setTitle(title);
        }
    }
}

void ScatterFunction::setXLabel(const QString& label)
{
    if (m_xLabel != label) {
        m_xLabel = label;
        emit xLabelChanged(label);
        if (m_plotNode) {
            m_plotNode->x1Axis()->setLabel(label);
        }
    }
}

void ScatterFunction::setYLabel(const QString& label)
{
    if (m_yLabel != label) {
        m_yLabel = label;
        emit yLabelChanged(label);
        if (m_plotNode) {
            m_plotNode->y1Axis()->setLabel(label);
        }
    }
}

void ScatterFunction::setMarkerSize(float size)
{
    if (m_markerSize != size) {
        m_markerSize = size;
        emit markerSizeChanged(size);
        if (m_scatterNode) {
            m_scatterNode->setMarkerSize(size);
        }
    }
}

void ScatterFunction::setMarkerShape(int shape)
{
    if (m_markerShape != shape) {
        m_markerShape = shape;
        emit markerShapeChanged(shape);
        if (m_scatterNode) {
            m_scatterNode->setMarkerShape(shape);
        }
    }
}

void ScatterFunction::setMarkerFill(bool fill)
{
    if (m_markerFill != fill) {
        m_markerFill = fill;
        emit markerFillChanged(fill);
        if (m_scatterNode) {
            m_scatterNode->setMarkerFill(fill);
        }
    }
}

void ScatterFunction::setMarkerColor(const QColor& color)
{
    if (m_markerColor != color) {
        m_markerColor = color;
        emit markerColorChanged(color);
        if (m_scatterNode) {
            m_scatterNode->setColor(color);
        }
    }
}
