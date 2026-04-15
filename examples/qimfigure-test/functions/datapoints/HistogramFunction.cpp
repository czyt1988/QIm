#include "HistogramFunction.h"
#include "QImFigureWidget.h"
#include "plot/QImPlotNode.h"
#include "plot/QImPlotAxisInfo.h"
#include "plot/QImPlotHistogramItemNode.h"
#include "plot/QImPlotValueTrackerNode.h"
#include "plot/QImPlotHistogramDataSeries.h"
#include <vector>
#include <QVector>
#include <random>
#include <utility>

/**
 * \if ENGLISH
 * @brief Constructor for HistogramFunction
 * @param parent Parent object
 * @details Initializes the test function and registers properties for interactive editing.
 * Properties are organized in categories:
 * - "Plot"/"Basic Info"/"Title" - Plot title
 * - "Plot"/"Axis"/"X Label" - X axis label
 * - "Plot"/"Axis"/"Y Label" - Y axis label
 * - "Histogram"/"Binning"/"Bins" - Number of bins or binning method
 * - "Histogram"/"Binning"/"Bar Scale" - Bar width scaling factor
 * - "Histogram"/"Binning"/"Range Min" - Minimum value for binning range
 * - "Histogram"/"Binning"/"Range Max" - Maximum value for binning range
 * - "Histogram"/"Statistics"/"Cumulative" - Cumulative distribution flag
 * - "Histogram"/"Statistics"/"Density" - Normalize as probability density flag
 * - "Histogram"/"Orientation"/"Horizontal" - Horizontal orientation flag
 * - "Histogram"/"Outliers"/"No Outliers" - Exclude outliers flag
 * - "Histogram"/"Style"/"Color" - Bar color
 * \endif
 * 
 * \if CHINESE
 * @brief HistogramFunction 构造函数
 * @param parent 父对象
 * @details 初始化测试函数并注册属性用于交互式编辑。
 * 属性按类别组织：
 * - "Plot"/"Basic Info"/"Title" - 图表标题
 * - "Plot"/"Axis"/"X Label" - X 轴标签
 * - "Plot"/"Axis"/"Y Label" - Y 轴标签
 * - "Histogram"/"Binning"/"Bins" - 箱数或分箱方法
 * - "Histogram"/"Binning"/"Bar Scale" - 柱条宽度缩放因子
 * - "Histogram"/"Binning"/"Range Min" - 分箱范围最小值
 * - "Histogram"/"Binning"/"Range Max" - 分箱范围最大值
 * - "Histogram"/"Statistics"/"Cumulative" - 累积分布标志
 * - "Histogram"/"Statistics"/"Density" - 归一化为概率密度标志
 * - "Histogram"/"Orientation"/"Horizontal" - 水平方向标志
 * - "Histogram"/"Outliers"/"No Outliers" - 排除异常值标志
 * - "Histogram"/"Style"/"Color" - 柱条颜色
 * \endif
 */
HistogramFunction::HistogramFunction(QObject* parent)
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
    
    // Register bins property
    PropertyRegistration binsReg;
    binsReg.category = tr("Histogram");
    binsReg.subcategory = tr("Binning");
    binsReg.displayName = tr("Bins");
    binsReg.briefDesc = tr("Number of bins or binning method");
    binsReg.detailDesc = tr("Positive integer for number of bins, or negative for predefined methods:\n"
                           "-1 = SquareRoot, -2 = Sturges, -3 = Rice, -4 = Scott");
    binsReg.editorType = EditorType::SpinBox;
    binsReg.defaultValue = m_bins;
    binsReg.minValue = -4;
    binsReg.maxValue = 100;
    binsReg.stepValue = 1;
    binsReg.propertyName = "bins";
    binsReg.target = this;
    registerProperty(binsReg);
    
    // Register bar scale property
    PropertyRegistration barScaleReg;
    barScaleReg.category = tr("Histogram");
    barScaleReg.subcategory = tr("Binning");
    barScaleReg.displayName = tr("Bar Scale");
    barScaleReg.briefDesc = tr("Bar width scaling factor");
    barScaleReg.detailDesc = tr("Scaling factor for bar widths (0.1-5.0)");
    barScaleReg.editorType = EditorType::DoubleSpinBox;
    barScaleReg.defaultValue = m_barScale;
    barScaleReg.minValue = 0.1;
    barScaleReg.maxValue = 5.0;
    barScaleReg.stepValue = 0.05;
    barScaleReg.propertyName = "barScale";
    barScaleReg.target = this;
    registerProperty(barScaleReg);
    
    // Register range min property
    PropertyRegistration rangeMinReg;
    rangeMinReg.category = tr("Histogram");
    rangeMinReg.subcategory = tr("Binning");
    rangeMinReg.displayName = tr("Range Min");
    rangeMinReg.briefDesc = tr("Minimum value for binning range");
    rangeMinReg.detailDesc = tr("Minimum value for histogram binning range (leave 0 for auto)");
    rangeMinReg.editorType = EditorType::DoubleSpinBox;
    rangeMinReg.defaultValue = m_rangeMin;
    rangeMinReg.minValue = -1000.0;
    rangeMinReg.maxValue = 1000.0;
    rangeMinReg.stepValue = 0.1;
    rangeMinReg.propertyName = "rangeMin";
    rangeMinReg.target = this;
    registerProperty(rangeMinReg);
    
    // Register range max property
    PropertyRegistration rangeMaxReg;
    rangeMaxReg.category = tr("Histogram");
    rangeMaxReg.subcategory = tr("Binning");
    rangeMaxReg.displayName = tr("Range Max");
    rangeMaxReg.briefDesc = tr("Maximum value for binning range");
    rangeMaxReg.detailDesc = tr("Maximum value for histogram binning range (leave 0 for auto)");
    rangeMaxReg.editorType = EditorType::DoubleSpinBox;
    rangeMaxReg.defaultValue = m_rangeMax;
    rangeMaxReg.minValue = -1000.0;
    rangeMaxReg.maxValue = 1000.0;
    rangeMaxReg.stepValue = 0.1;
    rangeMaxReg.propertyName = "rangeMax";
    rangeMaxReg.target = this;
    registerProperty(rangeMaxReg);
    
    // Register cumulative flag property
    PropertyRegistration cumulativeReg;
    cumulativeReg.category = tr("Histogram");
    cumulativeReg.subcategory = tr("Statistics");
    cumulativeReg.displayName = tr("Cumulative");
    cumulativeReg.briefDesc = tr("Cumulative distribution");
    cumulativeReg.detailDesc = tr("Show cumulative distribution (running total) instead of frequency");
    cumulativeReg.editorType = EditorType::CheckBox;
    cumulativeReg.defaultValue = m_cumulative;
    cumulativeReg.propertyName = "cumulative";
    cumulativeReg.target = this;
    registerProperty(cumulativeReg);
    
    // Register density flag property
    PropertyRegistration densityReg;
    densityReg.category = tr("Histogram");
    densityReg.subcategory = tr("Statistics");
    densityReg.displayName = tr("Density");
    densityReg.briefDesc = tr("Normalize as probability density");
    densityReg.detailDesc = tr("Normalize histogram as probability density (area under curve = 1)");
    densityReg.editorType = EditorType::CheckBox;
    densityReg.defaultValue = m_density;
    densityReg.propertyName = "density";
    densityReg.target = this;
    registerProperty(densityReg);
    
    // Register horizontal orientation property
    PropertyRegistration horizontalReg;
    horizontalReg.category = tr("Histogram");
    horizontalReg.subcategory = tr("Orientation");
    horizontalReg.displayName = tr("Horizontal");
    horizontalReg.briefDesc = tr("Horizontal orientation");
    horizontalReg.detailDesc = tr("Display histogram bars horizontally instead of vertically");
    horizontalReg.editorType = EditorType::CheckBox;
    horizontalReg.defaultValue = m_horizontal;
    horizontalReg.propertyName = "horizontal";
    horizontalReg.target = this;
    registerProperty(horizontalReg);
    
    // Register outliers inclusion flag property
    PropertyRegistration outliersIncludedReg;
    outliersIncludedReg.category = tr("Histogram");
    outliersIncludedReg.subcategory = tr("Outliers");
    outliersIncludedReg.displayName = tr("Outliers Included");
    outliersIncludedReg.briefDesc = tr("Include outliers");
    outliersIncludedReg.detailDesc = tr("Include outliers in histogram normalization and cumulative counts");
    outliersIncludedReg.editorType = EditorType::CheckBox;
    outliersIncludedReg.defaultValue = m_outliersIncluded;
    outliersIncludedReg.propertyName = "outliersIncluded";
    outliersIncludedReg.target = this;
    registerProperty(outliersIncludedReg);
    
    // Register bar color property
    PropertyRegistration barColorReg;
    barColorReg.category = tr("Histogram");
    barColorReg.subcategory = tr("Style");
    barColorReg.displayName = tr("Color");
    barColorReg.briefDesc = tr("Bar color");
    barColorReg.detailDesc = tr("Color of histogram bars");
    barColorReg.editorType = EditorType::ColorPicker;
    barColorReg.defaultValue = m_barColor;
    barColorReg.propertyName = "barColor";
    barColorReg.target = this;
    registerProperty(barColorReg);
    
    // Register colMajor property
    PropertyRegistration colMajorReg;
    colMajorReg.category = tr("Histogram");
    colMajorReg.subcategory = tr("Flags");
    colMajorReg.displayName = tr("Col Major");
    colMajorReg.briefDesc = tr("Column-major data order");
    colMajorReg.detailDesc = tr("When enabled, data is interpreted in column-major order instead of row-major.");
    colMajorReg.editorType = EditorType::CheckBox;
    colMajorReg.defaultValue = m_colMajor;
    colMajorReg.propertyName = "colMajor";
    colMajorReg.target = this;
    registerProperty(colMajorReg);
}

/**
 * \if ENGLISH
 * @brief Destructor for HistogramFunction
 * \endif
 * 
 * \if CHINESE
 * @brief HistogramFunction 析构函数
 * \endif
 */
HistogramFunction::~HistogramFunction()
{
    // Plot nodes are managed by the figure widget
}

/**
 * \if ENGLISH
 * @brief Create plot nodes in the given figure widget
 * @param figure Pointer to the figure widget where plots will be created
 * @details Creates a histogram plot with 1000 random values following a normal distribution.
 * The histogram demonstrates various properties including binning methods, range specification,
 * cumulative/distribution flags, and styling options.
 * \endif
 * 
 * \if CHINESE
 * @brief 在指定的图表控件中创建绘图节点
 * @param figure 将要创建绘图的图表控件指针
 * @details 创建一个包含 1000 个遵循正态分布的随机值的直方图。
 * 直方图演示了各种属性，包括分箱方法、范围指定、累积/分布标志和样式选项。
 * \endif
 */
void HistogramFunction::createPlot(QIM::QImFigureWidget* figure)
{
    if (!figure) {
        return;
    }
    
    // Clear previous nodes (if any)
    cleanupPlot();
    
    // Create plot node with default 1x1 grid position
    m_plotNode = figure->createPlotNode();
    m_plotNode->setTitle(m_title);
    m_plotNode->x1Axis()->setLabel(m_xLabel);
    m_plotNode->y1Axis()->setLabel(m_yLabel);
    m_plotNode->setLegendEnabled(true);
    
    // Generate 1000 random values with normal distribution (mean=0, stddev=1)
    const int numValues = 1000;
    QVector<double> values(numValues);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> dist(0.0, 1.0); // mean=0, stddev=1
    
    for (int i = 0; i < numValues; ++i) {
        values[i] = dist(gen);
    }
    
    // Create histogram item node and set data
    m_histogramNode = new QIM::QImPlotHistogramItemNode(m_plotNode);
    m_histogramNode->setLabel("Normal Distribution");
    auto dataSeries = new QIM::QImVectorHistogramDataSeries<QVector<double>>(std::move(values));
    m_histogramNode->setData(dataSeries);
    m_histogramNode->setBins(m_bins);
    m_histogramNode->setBarScale(m_barScale);
    if (m_rangeMin != 0.0 || m_rangeMax != 0.0) {
        m_histogramNode->setRangeMin(m_rangeMin);
        m_histogramNode->setRangeMax(m_rangeMax);
    }
    m_histogramNode->setCumulative(m_cumulative);
    m_histogramNode->setDensity(m_density);
    m_histogramNode->setHorizontal(m_horizontal);
    m_histogramNode->setOutliersIncluded(m_outliersIncluded);
    m_histogramNode->setColMajor(m_colMajor);
    m_histogramNode->setColor(m_barColor);
    
    // Create and attach value tracker
    m_trackerNode = new QIM::QImPlotValueTrackerNode(m_plotNode);
    m_trackerNode->setGroup(nullptr);  // No group by default
    m_plotNode->addChildNode(m_trackerNode);
}

void HistogramFunction::setTitle(const QString& title)
{
    if (m_title != title) {
        m_title = title;
        emit titleChanged(title);
        if (m_plotNode) {
            m_plotNode->setTitle(title);
        }
    }
}

void HistogramFunction::setXLabel(const QString& label)
{
    if (m_xLabel != label) {
        m_xLabel = label;
        emit xLabelChanged(label);
        if (m_plotNode) {
            m_plotNode->x1Axis()->setLabel(label);
        }
    }
}

void HistogramFunction::setYLabel(const QString& label)
{
    if (m_yLabel != label) {
        m_yLabel = label;
        emit yLabelChanged(label);
        if (m_plotNode) {
            m_plotNode->y1Axis()->setLabel(label);
        }
    }
}

void HistogramFunction::setBins(int bins)
{
    if (m_bins != bins) {
        m_bins = bins;
        emit binsChanged(bins);
        if (m_histogramNode) {
            m_histogramNode->setBins(bins);
        }
    }
}

void HistogramFunction::setBarScale(double scale)
{
    if (m_barScale != scale) {
        m_barScale = scale;
        emit barScaleChanged(scale);
        if (m_histogramNode) {
            m_histogramNode->setBarScale(scale);
        }
    }
}

void HistogramFunction::setRangeMin(double min)
{
    if (m_rangeMin != min) {
        m_rangeMin = min;
        emit rangeChanged();
        if (m_histogramNode) {
            m_histogramNode->setRangeMin(min);
        }
    }
}

void HistogramFunction::setRangeMax(double max)
{
    if (m_rangeMax != max) {
        m_rangeMax = max;
        emit rangeChanged();
        if (m_histogramNode) {
            m_histogramNode->setRangeMax(max);
        }
    }
}

void HistogramFunction::setCumulative(bool cumulative)
{
    if (m_cumulative != cumulative) {
        m_cumulative = cumulative;
        emit cumulativeChanged(cumulative);
        if (m_histogramNode) {
            m_histogramNode->setCumulative(cumulative);
        }
    }
}

void HistogramFunction::setDensity(bool density)
{
    if (m_density != density) {
        m_density = density;
        emit densityChanged(density);
        if (m_histogramNode) {
            m_histogramNode->setDensity(density);
        }
    }
}

void HistogramFunction::setHorizontal(bool horizontal)
{
    if (m_horizontal != horizontal) {
        m_horizontal = horizontal;
        emit orientationChanged(horizontal);
        if (m_histogramNode) {
            m_histogramNode->setHorizontal(horizontal);
        }
    }
}

void HistogramFunction::setOutliersIncluded(bool included)
{
    if (m_outliersIncluded != included) {
        m_outliersIncluded = included;
        Q_EMIT outliersIncludedChanged(included);
        if (m_histogramNode) {
            m_histogramNode->setOutliersIncluded(included);
        }
    }
}

void HistogramFunction::setColMajor(bool on)
{
    if (m_colMajor != on) {
        m_colMajor = on;
        Q_EMIT colMajorChanged(on);
        if (m_histogramNode) {
            m_histogramNode->setColMajor(on);
        }
    }
}

void HistogramFunction::setBarColor(const QColor& color)
{
    if (m_barColor != color) {
        m_barColor = color;
        emit barColorChanged(color);
        if (m_histogramNode) {
            m_histogramNode->setColor(color);
        }
    }
}