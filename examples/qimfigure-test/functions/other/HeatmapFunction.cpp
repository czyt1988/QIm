#include "HeatmapFunction.h"
#include "QImFigureWidget.h"
#include "plot/QImPlotNode.h"
#include "plot/QImPlotAxisInfo.h"
#include "plot/QImPlotHeatmapItemNode.h"
#include "plot/QImPlotValueTrackerNode.h"
#include <vector>

/**
 * \if ENGLISH
 * @brief Constructor for HeatmapFunction
 * @param parent Parent object
 * @details Initializes the test function and registers properties for interactive editing.
 * Properties are organized in categories:
 * - "Plot"/"Basic Info"/"Title" - Plot title
 * - "Plot"/"Axis"/"X Label" - X axis label
 * - "Plot"/"Axis"/"Y Label" - Y axis label
 * - "Heatmap"/"Scale"/"Min" - Scale minimum value
 * - "Heatmap"/"Scale"/"Max" - Scale maximum value
 * - "Heatmap"/"Labels"/"Format" - Label format string
 * - "Heatmap"/"Bounds"/"Min" - Lower-left bounds
 * - "Heatmap"/"Bounds"/"Max" - Upper-right bounds
 * - "Heatmap"/"Layout"/"Column-major" - Column-major data layout flag
 * \endif
 * 
 * \if CHINESE
 * @brief HeatmapFunction 构造函数
 * @param parent 父对象
 * @details 初始化测试函数并注册属性用于交互式编辑。
 * 属性按类别组织：
 * - "Plot"/"Basic Info"/"Title" - 图表标题
 * - "Plot"/"Axis"/"X Label" - X 轴标签
 * - "Plot"/"Axis"/"Y Label" - Y 轴标签
 * - "Heatmap"/"Scale"/"Min" - 缩放最小值
 * - "Heatmap"/"Scale"/"Max" - 缩放最大值
 * - "Heatmap"/"Labels"/"Format" - 标签格式字符串
 * - "Heatmap"/"Bounds"/"Min" - 左下角边界
 * - "Heatmap"/"Bounds"/"Max" - 右上角边界
 * - "Heatmap"/"Layout"/"Column-major" - 列主序数据布局标志
 * \endif
 */
HeatmapFunction::HeatmapFunction(QObject* parent)
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
    
    // Register scale minimum property
    PropertyRegistration scaleMinReg;
    scaleMinReg.category = tr("Heatmap");
    scaleMinReg.subcategory = tr("Scale");
    scaleMinReg.displayName = tr("Min");
    scaleMinReg.briefDesc = tr("Scale minimum value");
    scaleMinReg.detailDesc = tr("Sets the minimum value for color scaling (0 = auto)");
    scaleMinReg.editorType = EditorType::DoubleSpinBox;
    scaleMinReg.defaultValue = m_scaleMin;
    scaleMinReg.minValue = -100.0;
    scaleMinReg.maxValue = 100.0;
    scaleMinReg.stepValue = 0.1;
    scaleMinReg.propertyName = "scaleMin";
    scaleMinReg.target = this;
    registerProperty(scaleMinReg);
    
    // Register scale maximum property
    PropertyRegistration scaleMaxReg;
    scaleMaxReg.category = tr("Heatmap");
    scaleMaxReg.subcategory = tr("Scale");
    scaleMaxReg.displayName = tr("Max");
    scaleMaxReg.briefDesc = tr("Scale maximum value");
    scaleMaxReg.detailDesc = tr("Sets the maximum value for color scaling (0 = auto)");
    scaleMaxReg.editorType = EditorType::DoubleSpinBox;
    scaleMaxReg.defaultValue = m_scaleMax;
    scaleMaxReg.minValue = -100.0;
    scaleMaxReg.maxValue = 100.0;
    scaleMaxReg.stepValue = 0.1;
    scaleMaxReg.propertyName = "scaleMax";
    scaleMaxReg.target = this;
    registerProperty(scaleMaxReg);
    
    // Register label format property
    PropertyRegistration labelFmtReg;
    labelFmtReg.category = tr("Heatmap");
    labelFmtReg.subcategory = tr("Labels");
    labelFmtReg.displayName = tr("Format");
    labelFmtReg.briefDesc = tr("Label format string");
    labelFmtReg.detailDesc = tr("Sets the format string for value labels (e.g., \"%.1f\"). Empty string disables labels.");
    labelFmtReg.editorType = EditorType::LineEdit;
    labelFmtReg.defaultValue = m_labelFormat;
    labelFmtReg.propertyName = "labelFormat";
    labelFmtReg.target = this;
    registerProperty(labelFmtReg);
    
    // Register bounds minimum X property
    PropertyRegistration boundsMinXReg;
    boundsMinXReg.category = tr("Heatmap");
    boundsMinXReg.subcategory = tr("Bounds");
    boundsMinXReg.displayName = tr("Min X");
    boundsMinXReg.briefDesc = tr("Lower-left bounds X coordinate");
    boundsMinXReg.detailDesc = tr("Sets the X coordinate of the lower-left corner of the heatmap rectangle");
    boundsMinXReg.editorType = EditorType::DoubleSpinBox;
    boundsMinXReg.defaultValue = m_boundsMin.x();
    boundsMinXReg.minValue = -10.0;
    boundsMinXReg.maxValue = 10.0;
    boundsMinXReg.stepValue = 0.1;
    boundsMinXReg.propertyName = "boundsMinX";
    boundsMinXReg.target = this;
    registerProperty(boundsMinXReg);
    
    // Register bounds minimum Y property
    PropertyRegistration boundsMinYReg;
    boundsMinYReg.category = tr("Heatmap");
    boundsMinYReg.subcategory = tr("Bounds");
    boundsMinYReg.displayName = tr("Min Y");
    boundsMinYReg.briefDesc = tr("Lower-left bounds Y coordinate");
    boundsMinYReg.detailDesc = tr("Sets the Y coordinate of the lower-left corner of the heatmap rectangle");
    boundsMinYReg.editorType = EditorType::DoubleSpinBox;
    boundsMinYReg.defaultValue = m_boundsMin.y();
    boundsMinYReg.minValue = -10.0;
    boundsMinYReg.maxValue = 10.0;
    boundsMinYReg.stepValue = 0.1;
    boundsMinYReg.propertyName = "boundsMinY";
    boundsMinYReg.target = this;
    registerProperty(boundsMinYReg);
    
    // Register bounds maximum X property
    PropertyRegistration boundsMaxXReg;
    boundsMaxXReg.category = tr("Heatmap");
    boundsMaxXReg.subcategory = tr("Bounds");
    boundsMaxXReg.displayName = tr("Max X");
    boundsMaxXReg.briefDesc = tr("Upper-right bounds X coordinate");
    boundsMaxXReg.detailDesc = tr("Sets the X coordinate of the upper-right corner of the heatmap rectangle");
    boundsMaxXReg.editorType = EditorType::DoubleSpinBox;
    boundsMaxXReg.defaultValue = m_boundsMax.x();
    boundsMaxXReg.minValue = -10.0;
    boundsMaxXReg.maxValue = 10.0;
    boundsMaxXReg.stepValue = 0.1;
    boundsMaxXReg.propertyName = "boundsMaxX";
    boundsMaxXReg.target = this;
    registerProperty(boundsMaxXReg);
    
    // Register bounds maximum Y property
    PropertyRegistration boundsMaxYReg;
    boundsMaxYReg.category = tr("Heatmap");
    boundsMaxYReg.subcategory = tr("Bounds");
    boundsMaxYReg.displayName = tr("Max Y");
    boundsMaxYReg.briefDesc = tr("Upper-right bounds Y coordinate");
    boundsMaxYReg.detailDesc = tr("Sets the Y coordinate of the upper-right corner of the heatmap rectangle");
    boundsMaxYReg.editorType = EditorType::DoubleSpinBox;
    boundsMaxYReg.defaultValue = m_boundsMax.y();
    boundsMaxYReg.minValue = -10.0;
    boundsMaxYReg.maxValue = 10.0;
    boundsMaxYReg.stepValue = 0.1;
    boundsMaxYReg.propertyName = "boundsMaxY";
    boundsMaxYReg.target = this;
    registerProperty(boundsMaxYReg);
    
    // Register column-major property
    PropertyRegistration colMajorReg;
    colMajorReg.category = tr("Heatmap");
    colMajorReg.subcategory = tr("Layout");
    colMajorReg.displayName = tr("Column-major");
    colMajorReg.briefDesc = tr("Column-major data layout");
    colMajorReg.detailDesc = tr("When enabled, data is assumed to be stored in column-major order (default: row-major)");
    colMajorReg.editorType = EditorType::CheckBox;
    colMajorReg.defaultValue = m_colMajor;
    colMajorReg.propertyName = "colMajor";
    colMajorReg.target = this;
    registerProperty(colMajorReg);
}

/**
 * \if ENGLISH
 * @brief Destructor for HeatmapFunction
 * \endif
 * 
 * \if CHINESE
 * @brief HeatmapFunction 析构函数
 * \endif
 */
HeatmapFunction::~HeatmapFunction()
{
}

/**
 * \if ENGLISH
 * @brief Create plot nodes in the given figure widget
 * @param figure Pointer to the figure widget where plots will be created
 * @details Creates a 10x10 heatmap with sinusoidal pattern, configures axes,
 * adds a heatmap item, and attaches a value tracker node.
 * \endif
 * 
 * \if CHINESE
 * @brief 在指定的图表控件中创建绘图节点
 * @param figure 将要创建绘图的图表控件指针
 * @details 创建一个 10x10 正弦图案的热力图，配置坐标轴，
 * 添加热力图项，并附加一个值追踪器节点。
 * \endif
 */
void HeatmapFunction::createPlot(QIM::QImFigureWidget* figure)
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
    
    // Generate heatmap data - 10x10 sinusoidal pattern
    const int rows = 10;
    const int cols = 10;
    std::vector<double> values(rows * cols);
    
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            values[i * cols + j] = sin(i * 0.5) * cos(j * 0.5);
        }
    }
    
    // Create heatmap item node and set data
    m_heatmapNode = new QIM::QImPlotHeatmapItemNode(m_plotNode);
    m_heatmapNode->setLabel("Heatmap");
    m_heatmapNode->setData(values, rows, cols, m_colMajor);
    m_heatmapNode->setScaleMin(m_scaleMin);
    m_heatmapNode->setScaleMax(m_scaleMax);
    m_heatmapNode->setLabelFormat(m_labelFormat);
    m_heatmapNode->setBoundsMin(m_boundsMin);
    m_heatmapNode->setBoundsMax(m_boundsMax);
    m_heatmapNode->setColMajor(m_colMajor);
    
    // Create and attach value tracker
    m_trackerNode = new QIM::QImPlotValueTrackerNode(m_plotNode);
    m_trackerNode->setGroup(nullptr);  // No group by default
    m_plotNode->addChildNode(m_trackerNode);
}

void HeatmapFunction::setTitle(const QString& title)
{
    if (m_title != title) {
        m_title = title;
        emit titleChanged(title);
        if (m_plotNode) {
            m_plotNode->setTitle(title);
        }
    }
}

void HeatmapFunction::setXLabel(const QString& label)
{
    if (m_xLabel != label) {
        m_xLabel = label;
        emit xLabelChanged(label);
        if (m_plotNode) {
            m_plotNode->x1Axis()->setLabel(label);
        }
    }
}

void HeatmapFunction::setYLabel(const QString& label)
{
    if (m_yLabel != label) {
        m_yLabel = label;
        emit yLabelChanged(label);
        if (m_plotNode) {
            m_plotNode->y1Axis()->setLabel(label);
        }
    }
}

void HeatmapFunction::setScaleMin(double min)
{
    if (m_scaleMin != min) {
        m_scaleMin = min;
        emit scaleMinChanged(min);
        if (m_heatmapNode) {
            m_heatmapNode->setScaleMin(min);
        }
    }
}

void HeatmapFunction::setScaleMax(double max)
{
    if (m_scaleMax != max) {
        m_scaleMax = max;
        emit scaleMaxChanged(max);
        if (m_heatmapNode) {
            m_heatmapNode->setScaleMax(max);
        }
    }
}

void HeatmapFunction::setLabelFormat(const QString& format)
{
    if (m_labelFormat != format) {
        m_labelFormat = format;
        emit labelFormatChanged(format);
        if (m_heatmapNode) {
            m_heatmapNode->setLabelFormat(format);
        }
    }
}

void HeatmapFunction::setBoundsMin(const QPointF& min)
{
    if (m_boundsMin != min) {
        m_boundsMin = min;
        emit boundsMinChanged(min);
        if (m_heatmapNode) {
            m_heatmapNode->setBoundsMin(min);
        }
    }
}

void HeatmapFunction::setBoundsMax(const QPointF& max)
{
    if (m_boundsMax != max) {
        m_boundsMax = max;
        emit boundsMaxChanged(max);
        if (m_heatmapNode) {
            m_heatmapNode->setBoundsMax(max);
        }
    }
}

void HeatmapFunction::setBoundsMinX(double x)
{
    if (m_boundsMin.x() != x) {
        m_boundsMin.setX(x);
        emit boundsMinXChanged(x);
        emit boundsMinChanged(m_boundsMin);
        if (m_heatmapNode) {
            m_heatmapNode->setBoundsMin(m_boundsMin);
        }
    }
}

void HeatmapFunction::setBoundsMinY(double y)
{
    if (m_boundsMin.y() != y) {
        m_boundsMin.setY(y);
        emit boundsMinYChanged(y);
        emit boundsMinChanged(m_boundsMin);
        if (m_heatmapNode) {
            m_heatmapNode->setBoundsMin(m_boundsMin);
        }
    }
}

void HeatmapFunction::setBoundsMaxX(double x)
{
    if (m_boundsMax.x() != x) {
        m_boundsMax.setX(x);
        emit boundsMaxXChanged(x);
        emit boundsMaxChanged(m_boundsMax);
        if (m_heatmapNode) {
            m_heatmapNode->setBoundsMax(m_boundsMax);
        }
    }
}

void HeatmapFunction::setBoundsMaxY(double y)
{
    if (m_boundsMax.y() != y) {
        m_boundsMax.setY(y);
        emit boundsMaxYChanged(y);
        emit boundsMaxChanged(m_boundsMax);
        if (m_heatmapNode) {
            m_heatmapNode->setBoundsMax(m_boundsMax);
        }
    }
}

void HeatmapFunction::setColMajor(bool colMajor)
{
    if (m_colMajor != colMajor) {
        m_colMajor = colMajor;
        emit colMajorChanged(colMajor);
        if (m_heatmapNode) {
            m_heatmapNode->setColMajor(colMajor);
        }
    }
}