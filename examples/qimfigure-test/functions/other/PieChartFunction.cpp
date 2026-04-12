#include "PieChartFunction.h"
#include "QImFigureWidget.h"
#include "plot/QImPlotNode.h"
#include "plot/QImPlotAxisInfo.h"
#include "plot/QImPlotPieChartItemNode.h"
#include "plot/QImPlotValueTrackerNode.h"
#include <vector>

/**
 * \if ENGLISH
 * @brief Constructor for PieChartFunction
 * @param parent Parent object
 * @details Initializes the test function and registers properties for interactive editing.
 * Properties are organized in categories:
 * - "Plot"/"Basic Info"/"Title" - Plot title
 * - "Pie Chart"/"Position"/"Center X" - Center X coordinate
 * - "Pie Chart"/"Position"/"Center Y" - Center Y coordinate
 * - "Pie Chart"/"Size"/"Radius" - Pie chart radius
 * - "Pie Chart"/"Labels"/"Label Format" - Label format string
 * - "Pie Chart"/"Angles"/"Start Angle" - Starting angle in degrees
 * - "Pie Chart"/"Flags"/"Normalize" - Normalization flag
 * - "Pie Chart"/"Flags"/"Ignore Hidden" - Ignore hidden slices flag
 * - "Pie Chart"/"Flags"/"Exploding" - Exploding slices flag
 * \endif
 * 
 * \if CHINESE
 * @brief PieChartFunction 构造函数
 * @param parent 父对象
 * @details 初始化测试函数并注册属性用于交互式编辑。
 * 属性按类别组织：
 * - "Plot"/"Basic Info"/"Title" - 图表标题
 * - "Pie Chart"/"Position"/"Center X" - 中心X坐标
 * - "Pie Chart"/"Position"/"Center Y" - 中心Y坐标
 * - "Pie Chart"/"Size"/"Radius" - 饼图半径
 * - "Pie Chart"/"Labels"/"Label Format" - 标签格式字符串
 * - "Pie Chart"/"Angles"/"Start Angle" - 起始角度（度）
 * - "Pie Chart"/"Flags"/"Normalize" - 归一化标志
 * - "Pie Chart"/"Flags"/"Ignore Hidden" - 忽略隐藏切片标志
 * - "Pie Chart"/"Flags"/"Exploding" - 爆炸切片标志
 * \endif
 */
PieChartFunction::PieChartFunction(QObject* parent)
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
    
    // Register center X property
    PropertyRegistration centerXReg;
    centerXReg.category = tr("Pie Chart");
    centerXReg.subcategory = tr("Position");
    centerXReg.displayName = tr("Center X");
    centerXReg.briefDesc = tr("Center X coordinate");
    centerXReg.detailDesc = tr("Sets the X coordinate of the pie chart center in plot units");
    centerXReg.editorType = EditorType::DoubleSpinBox;
    centerXReg.defaultValue = m_centerX;
    centerXReg.minValue = -10.0;
    centerXReg.maxValue = 10.0;
    centerXReg.stepValue = 0.1;
    centerXReg.propertyName = "centerX";
    centerXReg.target = this;
    registerProperty(centerXReg);
    
    // Register center Y property
    PropertyRegistration centerYReg;
    centerYReg.category = tr("Pie Chart");
    centerYReg.subcategory = tr("Position");
    centerYReg.displayName = tr("Center Y");
    centerYReg.briefDesc = tr("Center Y coordinate");
    centerYReg.detailDesc = tr("Sets the Y coordinate of the pie chart center in plot units");
    centerYReg.editorType = EditorType::DoubleSpinBox;
    centerYReg.defaultValue = m_centerY;
    centerYReg.minValue = -10.0;
    centerXReg.maxValue = 10.0;
    centerXReg.stepValue = 0.1;
    centerYReg.propertyName = "centerY";
    centerYReg.target = this;
    registerProperty(centerYReg);
    
    // Register radius property
    PropertyRegistration radiusReg;
    radiusReg.category = tr("Pie Chart");
    radiusReg.subcategory = tr("Size");
    radiusReg.displayName = tr("Radius");
    radiusReg.briefDesc = tr("Pie chart radius");
    radiusReg.detailDesc = tr("Sets the radius of the pie chart in plot units");
    radiusReg.editorType = EditorType::DoubleSpinBox;
    radiusReg.defaultValue = m_radius;
    radiusReg.minValue = 0.1;
    radiusReg.maxValue = 5.0;
    radiusReg.stepValue = 0.1;
    radiusReg.propertyName = "radius";
    radiusReg.target = this;
    registerProperty(radiusReg);
    
    // Register label format property
    PropertyRegistration formatReg;
    formatReg.category = tr("Pie Chart");
    formatReg.subcategory = tr("Labels");
    formatReg.displayName = tr("Label Format");
    formatReg.briefDesc = tr("Label format string");
    formatReg.detailDesc = tr("Sets the printf-style format string for slice labels (e.g., \"%.1f\", \"%.2f%%\")");
    formatReg.editorType = EditorType::LineEdit;
    formatReg.defaultValue = m_labelFormat;
    formatReg.propertyName = "labelFormat";
    formatReg.target = this;
    registerProperty(formatReg);
    
    // Register start angle property
    PropertyRegistration angleReg;
    angleReg.category = tr("Pie Chart");
    angleReg.subcategory = tr("Angles");
    angleReg.displayName = tr("Start Angle");
    angleReg.briefDesc = tr("Starting angle in degrees");
    angleReg.detailDesc = tr("Sets the starting angle of the first slice in degrees (0 = 3 o'clock, 90 = 12 o'clock)");
    angleReg.editorType = EditorType::DoubleSpinBox;
    angleReg.defaultValue = m_startAngle;
    angleReg.minValue = -360.0;
    angleReg.maxValue = 360.0;
    angleReg.stepValue = 5.0;
    angleReg.propertyName = "startAngle";
    angleReg.target = this;
    registerProperty(angleReg);
    
    // Register normalize flag property
    PropertyRegistration normalizeReg;
    normalizeReg.category = tr("Pie Chart");
    normalizeReg.subcategory = tr("Flags");
    normalizeReg.displayName = tr("Normalize");
    normalizeReg.briefDesc = tr("Normalization flag");
    normalizeReg.detailDesc = tr("When enabled, forces normalization of pie chart values (always makes a full circle)");
    normalizeReg.editorType = EditorType::CheckBox;
    normalizeReg.defaultValue = m_normalize;
    normalizeReg.propertyName = "normalize";
    normalizeReg.target = this;
    registerProperty(normalizeReg);
    
    // Register ignore hidden flag property
    PropertyRegistration ignoreHiddenReg;
    ignoreHiddenReg.category = tr("Pie Chart");
    ignoreHiddenReg.subcategory = tr("Flags");
    ignoreHiddenReg.displayName = tr("Ignore Hidden");
    ignoreHiddenReg.briefDesc = tr("Ignore hidden slices flag");
    ignoreHiddenReg.detailDesc = tr("When enabled, ignores hidden slices when drawing (as if they were not there)");
    ignoreHiddenReg.editorType = EditorType::CheckBox;
    ignoreHiddenReg.defaultValue = m_ignoreHidden;
    ignoreHiddenReg.propertyName = "ignoreHidden";
    ignoreHiddenReg.target = this;
    registerProperty(ignoreHiddenReg);
    
    // Register exploding flag property
    PropertyRegistration explodingReg;
    explodingReg.category = tr("Pie Chart");
    explodingReg.subcategory = tr("Flags");
    explodingReg.displayName = tr("Exploding");
    explodingReg.briefDesc = tr("Exploding slices flag");
    explodingReg.detailDesc = tr("When enabled, legend-hovered slice is exploded (offset from center)");
    explodingReg.editorType = EditorType::CheckBox;
    explodingReg.defaultValue = m_exploding;
    explodingReg.propertyName = "exploding";
    explodingReg.target = this;
    registerProperty(explodingReg);
}

/**
 * \if ENGLISH
 * @brief Destructor for PieChartFunction
 * \endif
 * 
 * \if CHINESE
 * @brief PieChartFunction 析构函数
 * \endif
 */
PieChartFunction::~PieChartFunction()
{
}

/**
 * \if ENGLISH
 * @brief Create plot nodes in the given figure widget
 * @param figure Pointer to the figure widget where plots will be created
 * @details Creates a pie chart with 5 slices (Apple, Banana, Cherry, Date, Elderberry),
 * configures axes, adds a pie chart item, and attaches a value tracker node.
 * \endif
 * 
 * \if CHINESE
 * @brief 在指定的图表控件中创建绘图节点
 * @param figure 将要创建绘图的图表控件指针
 * @details 创建一个包含 5 个切片（Apple, Banana, Cherry, Date, Elderberry）的饼图，
 * 配置坐标轴，添加饼图项，并附加一个值追踪器节点。
 * \endif
 */
void PieChartFunction::createPlot(QIM::QImFigureWidget* figure)
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
    m_plotNode->x1Axis()->setLabel("X");
    m_plotNode->y1Axis()->setLabel("Y");
    m_plotNode->setTitle(m_title);
    m_plotNode->setLegendEnabled(true);
    
    // Generate pie chart data - 5 slices
    QStringList labels = {"Apple", "Banana", "Cherry", "Date", "Elderberry"};
    std::vector<double> values = {30.0, 25.0, 15.0, 20.0, 10.0};
    
    // Create pie chart item node and set data
    m_pieChartNode = new QIM::QImPlotPieChartItemNode(m_plotNode);
    m_pieChartNode->setLabel("Fruit Distribution");
    m_pieChartNode->setData(labels, values);
    m_pieChartNode->setCenter(QPointF(m_centerX, m_centerY));
    m_pieChartNode->setRadius(m_radius);
    m_pieChartNode->setLabelFormat(m_labelFormat);
    m_pieChartNode->setStartAngle(m_startAngle);
    m_pieChartNode->setNormalize(m_normalize);
    m_pieChartNode->setIgnoreHidden(m_ignoreHidden);
    m_pieChartNode->setExploding(m_exploding);
    
    // Create and attach value tracker
    m_trackerNode = new QIM::QImPlotValueTrackerNode(m_plotNode);
    m_trackerNode->setGroup(nullptr);  // No group by default
    m_plotNode->addChildNode(m_trackerNode);
}

void PieChartFunction::setTitle(const QString& title)
{
    if (m_title != title) {
        m_title = title;
        emit titleChanged(title);
        if (m_plotNode) {
            m_plotNode->setTitle(title);
        }
    }
}

void PieChartFunction::setCenterX(double x)
{
    if (m_centerX != x) {
        m_centerX = x;
        emit centerChanged(QPointF(x, m_centerY));
        if (m_pieChartNode) {
            m_pieChartNode->setCenter(QPointF(x, m_pieChartNode->center().y()));
        }
    }
}

void PieChartFunction::setCenterY(double y)
{
    if (m_centerY != y) {
        m_centerY = y;
        emit centerChanged(QPointF(m_centerX, y));
        if (m_pieChartNode) {
            m_pieChartNode->setCenter(QPointF(m_pieChartNode->center().x(), y));
        }
    }
}

void PieChartFunction::setRadius(double radius)
{
    if (m_radius != radius) {
        m_radius = radius;
        emit radiusChanged(radius);
        if (m_pieChartNode) {
            m_pieChartNode->setRadius(radius);
        }
    }
}

void PieChartFunction::setLabelFormat(const QString& format)
{
    if (m_labelFormat != format) {
        m_labelFormat = format;
        emit labelFormatChanged(format);
        if (m_pieChartNode) {
            m_pieChartNode->setLabelFormat(format);
        }
    }
}

void PieChartFunction::setStartAngle(double angle)
{
    if (m_startAngle != angle) {
        m_startAngle = angle;
        emit startAngleChanged(angle);
        if (m_pieChartNode) {
            m_pieChartNode->setStartAngle(angle);
        }
    }
}

void PieChartFunction::setNormalize(bool normalize)
{
    if (m_normalize != normalize) {
        m_normalize = normalize;
        emit normalizeChanged(normalize);
        if (m_pieChartNode) {
            m_pieChartNode->setNormalize(normalize);
        }
    }
}

void PieChartFunction::setIgnoreHidden(bool ignore)
{
    if (m_ignoreHidden != ignore) {
        m_ignoreHidden = ignore;
        emit ignoreHiddenChanged(ignore);
        if (m_pieChartNode) {
            m_pieChartNode->setIgnoreHidden(ignore);
        }
    }
}

void PieChartFunction::setExploding(bool exploding)
{
    if (m_exploding != exploding) {
        m_exploding = exploding;
        emit explodingChanged(exploding);
        if (m_pieChartNode) {
            m_pieChartNode->setExploding(exploding);
        }
    }
}