#include "Histogram2DFunction.h"
#include "QImFigureWidget.h"
#include "plot/QImPlotNode.h"
#include "plot/QImPlotAxisInfo.h"
#include "plot/QImPlotHistogram2DItemNode.h"
#include "plot/QImPlotHistogram2DDataSeries.h"
#include <vector>
#include <QVector>
#include <random>
#include <utility>

/**
 * \if ENGLISH
 * @brief Constructor for Histogram2DFunction
 * @param parent Parent object
 * @details Initializes the test function and registers properties for interactive editing.
 * Properties are organized in categories:
 * - "Plot"/"Basic Info"/"Title" - Plot title
 * - "Plot"/"Axis"/"X Label" - X axis label
 * - "Plot"/"Axis"/"Y Label" - Y axis label
 * - "2D Histogram"/"Binning"/"X Bins" - Number of X bins or binning method
 * - "2D Histogram"/"Binning"/"Y Bins" - Number of Y bins or binning method
 * - "2D Histogram"/"Binning"/"X Range Min" - Minimum value for X binning range
 * - "2D Histogram"/"Binning"/"X Range Max" - Maximum value for X binning range
 * - "2D Histogram"/"Binning"/"Y Range Min" - Minimum value for Y binning range
 * - "2D Histogram"/"Binning"/"Y Range Max" - Maximum value for Y binning range
 * - "2D Histogram"/"Statistics"/"Density" - Normalize as probability density flag
 * - "2D Histogram"/"Outliers"/"No Outliers" - Exclude outliers flag
 * - "2D Histogram"/"Layout"/"Column-Major" - Column-major data layout flag
 * \endif
 * 
 * \if CHINESE
 * @brief Histogram2DFunction 构造函数
 * @param parent 父对象
 * @details 初始化测试函数并注册属性用于交互式编辑。
 * 属性按类别组织：
 * - "Plot"/"Basic Info"/"Title" - 图表标题
 * - "Plot"/"Axis"/"X Label" - X 轴标签
 * - "Plot"/"Axis"/"Y Label" - Y 轴标签
 * - "2D Histogram"/"Binning"/"X Bins" - X 箱数或分箱方法
 * - "2D Histogram"/"Binning"/"Y Bins" - Y 箱数或分箱方法
 * - "2D Histogram"/"Binning"/"X Range Min" - X 分箱范围最小值
 * - "2D Histogram"/"Binning"/"X Range Max" - X 分箱范围最大值
 * - "2D Histogram"/"Binning"/"Y Range Min" - Y 分箱范围最小值
 * - "2D Histogram"/"Binning"/"Y Range Max" - Y 分箱范围最大值
 * - "2D Histogram"/"Statistics"/"Density" - 归一化为概率密度标志
 * - "2D Histogram"/"Outliers"/"No Outliers" - 排除异常值标志
 * - "2D Histogram"/"Layout"/"Column-Major" - 列主序数据布局标志
 * \endif
 */
Histogram2DFunction::Histogram2DFunction(QObject* parent)
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
    
    // Register X bins property
    PropertyRegistration xBinsReg;
    xBinsReg.category = tr("2D Histogram");
    xBinsReg.subcategory = tr("Binning");
    xBinsReg.displayName = tr("X Bins");
    xBinsReg.briefDesc = tr("Number of X bins or binning method");
    xBinsReg.detailDesc = tr("Positive integer for number of X bins, or negative for predefined methods:\n"
                             "  -2: Sturges (default)\n"
                             "  -3: Scott\n"
                             "  -4: Freedman-Diaconis (F-D)\n"
                             "  -5: Square-root (Sqrt)\n"
                             "  -6: Doane\n");
    xBinsReg.editorType = EditorType::SpinBox;
    xBinsReg.defaultValue = m_xBins;
    xBinsReg.propertyName = "xBins";
    xBinsReg.target = this;
    xBinsReg.minValue = -10;
    xBinsReg.maxValue = 100;
    registerProperty(xBinsReg);
    
    // Register Y bins property
    PropertyRegistration yBinsReg;
    yBinsReg.category = tr("2D Histogram");
    yBinsReg.subcategory = tr("Binning");
    yBinsReg.displayName = tr("Y Bins");
    yBinsReg.briefDesc = tr("Number of Y bins or binning method");
    yBinsReg.detailDesc = tr("Positive integer for number of Y bins, or negative for predefined methods:\n"
                             "  -2: Sturges (default)\n"
                             "  -3: Scott\n"
                             "  -4: Freedman-Diaconis (F-D)\n"
                             "  -5: Square-root (Sqrt)\n"
                             "  -6: Doane\n");
    yBinsReg.editorType = EditorType::SpinBox;
    yBinsReg.defaultValue = m_yBins;
    yBinsReg.propertyName = "yBins";
    yBinsReg.target = this;
    yBinsReg.minValue = -10;
    yBinsReg.maxValue = 100;
    registerProperty(yBinsReg);
    
    // Register X range min property
    PropertyRegistration xRangeMinReg;
    xRangeMinReg.category = tr("2D Histogram");
    xRangeMinReg.subcategory = tr("Binning");
    xRangeMinReg.displayName = tr("X Range Min");
    xRangeMinReg.briefDesc = tr("Minimum value for X binning range");
    xRangeMinReg.detailDesc = tr("Sets the lower bound of the X histogram range.\n"
                                 "If equal to X Range Max (default 0), automatic range from data is used.");
    xRangeMinReg.editorType = EditorType::DoubleSpinBox;
    xRangeMinReg.defaultValue = m_xRangeMin;
    xRangeMinReg.propertyName = "xRangeMin";
    xRangeMinReg.target = this;
    xRangeMinReg.minValue = -10.0;
    xRangeMinReg.maxValue = 10.0;
    registerProperty(xRangeMinReg);
    
    // Register X range max property
    PropertyRegistration xRangeMaxReg;
    xRangeMaxReg.category = tr("2D Histogram");
    xRangeMaxReg.subcategory = tr("Binning");
    xRangeMaxReg.displayName = tr("X Range Max");
    xRangeMaxReg.briefDesc = tr("Maximum value for X binning range");
    xRangeMaxReg.detailDesc = tr("Sets the upper bound of the X histogram range.\n"
                                 "If equal to X Range Min (default 0), automatic range from data is used.");
    xRangeMaxReg.editorType = EditorType::DoubleSpinBox;
    xRangeMaxReg.defaultValue = m_xRangeMax;
    xRangeMaxReg.propertyName = "xRangeMax";
    xRangeMaxReg.target = this;
    xRangeMaxReg.minValue = -10.0;
    xRangeMaxReg.maxValue = 10.0;
    registerProperty(xRangeMaxReg);
    
    // Register Y range min property
    PropertyRegistration yRangeMinReg;
    yRangeMinReg.category = tr("2D Histogram");
    yRangeMinReg.subcategory = tr("Binning");
    yRangeMinReg.displayName = tr("Y Range Min");
    yRangeMinReg.briefDesc = tr("Minimum value for Y binning range");
    yRangeMinReg.detailDesc = tr("Sets the lower bound of the Y histogram range.\n"
                                 "If equal to Y Range Max (default 0), automatic range from data is used.");
    yRangeMinReg.editorType = EditorType::DoubleSpinBox;
    yRangeMinReg.defaultValue = m_yRangeMin;
    yRangeMinReg.propertyName = "yRangeMin";
    yRangeMinReg.target = this;
    yRangeMinReg.minValue = -10.0;
    yRangeMinReg.maxValue = 10.0;
    registerProperty(yRangeMinReg);
    
    // Register Y range max property
    PropertyRegistration yRangeMaxReg;
    yRangeMaxReg.category = tr("2D Histogram");
    yRangeMaxReg.subcategory = tr("Binning");
    yRangeMaxReg.displayName = tr("Y Range Max");
    yRangeMaxReg.briefDesc = tr("Maximum value for Y binning range");
    yRangeMaxReg.detailDesc = tr("Sets the upper bound of the Y histogram range.\n"
                                 "If equal to Y Range Min (default 0), automatic range from data is used.");
    yRangeMaxReg.editorType = EditorType::DoubleSpinBox;
    yRangeMaxReg.defaultValue = m_yRangeMax;
    yRangeMaxReg.propertyName = "yRangeMax";
    yRangeMaxReg.target = this;
    yRangeMaxReg.minValue = -10.0;
    yRangeMaxReg.maxValue = 10.0;
    registerProperty(yRangeMaxReg);
    
    // Register density flag property
    PropertyRegistration densityReg;
    densityReg.category = tr("2D Histogram");
    densityReg.subcategory = tr("Statistics");
    densityReg.displayName = tr("Density");
    densityReg.briefDesc = tr("Normalize as probability density");
    densityReg.detailDesc = tr("When enabled, counts are normalized to represent probability density (PDF).\n"
                               "Corresponds to ImPlotHistogramFlags_Density.");
    densityReg.editorType = EditorType::CheckBox;
    densityReg.defaultValue = m_density;
    densityReg.propertyName = "density";
    densityReg.target = this;
    registerProperty(densityReg);
    
    // Register exclude outliers flag property
    PropertyRegistration noOutliersReg;
    noOutliersReg.category = tr("2D Histogram");
    noOutliersReg.subcategory = tr("Outliers");
    noOutliersReg.displayName = tr("No Outliers");
    noOutliersReg.briefDesc = tr("Exclude outliers from normalization");
    noOutliersReg.detailDesc = tr("When enabled, values outside the specified histogram range are excluded\n"
                                  "from normalization and cumulative counts.\n"
                                  "Corresponds to ImPlotHistogramFlags_NoOutliers.");
    noOutliersReg.editorType = EditorType::CheckBox;
    noOutliersReg.defaultValue = m_noOutliers;
    noOutliersReg.propertyName = "noOutliers";
    noOutliersReg.target = this;
    registerProperty(noOutliersReg);
    
    // Register column-major layout flag property
    PropertyRegistration colMajorReg;
    colMajorReg.category = tr("2D Histogram");
    colMajorReg.subcategory = tr("Layout");
    colMajorReg.displayName = tr("Column-Major");
    colMajorReg.briefDesc = tr("Column-major data layout");
    colMajorReg.detailDesc = tr("When enabled, data is assumed to be stored in column-major order.\n"
                                "Corresponds to ImPlotHistogramFlags_ColMajor.");
    colMajorReg.editorType = EditorType::CheckBox;
    colMajorReg.defaultValue = m_colMajor;
    colMajorReg.propertyName = "colMajor";
    colMajorReg.target = this;
    registerProperty(colMajorReg);
}

/**
 * \if ENGLISH
 * @brief Destructor for Histogram2DFunction
 * \endif
 * 
 * \if CHINESE
 * @brief Histogram2DFunction 析构函数
 * \endif
 */
Histogram2DFunction::~Histogram2DFunction()
{
    cleanupPlot();
}

/**
 * \if ENGLISH
 * @brief Create 2D histogram plot in the given figure widget
 * @param figure Pointer to the figure widget where plots will be created
 * @details Generates 1000 correlated random points (X ~ N(0,1), Y ~ N(0,1) + 0.5*X)
 *          to demonstrate 2D histogram visualization.
 * \endif
 * 
 * \if CHINESE
 * @brief 在指定的图表控件中创建二维直方图
 * @param figure 将要创建绘图的图表控件指针
 * @details 生成 1000 个相关随机点（X ~ N(0,1)，Y ~ N(0,1) + 0.5*X）以演示二维直方图可视化。
 * \endif
 */
void Histogram2DFunction::createPlot(QIM::QImFigureWidget* figure)
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
    m_plotNode->setLegendEnabled(false); // 2D histogram doesn't use legend
    
    // Generate 1000 correlated random points
    const int numPoints = 1000;
    QVector<double> xs(numPoints);
    QVector<double> ys(numPoints);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> distX(0.0, 1.0); // mean=0, stddev=1
    std::normal_distribution<double> distY(0.0, 0.8); // slightly different stddev
    
    for (int i = 0; i < numPoints; ++i) {
        xs[i] = distX(gen);
        ys[i] = distY(gen) + 0.5 * xs[i]; // correlated Y with X
    }
    
    // Create 2D histogram item node and set data
    m_histogram2DNode = new QIM::QImPlotHistogram2DItemNode(m_plotNode);
    m_histogram2DNode->setLabel("Correlated 2D Normal");
    auto dataSeries = new QIM::QImVectorHistogram2DDataSeries<QVector<double>, QVector<double>>(std::move(xs), std::move(ys));
    m_histogram2DNode->setData(dataSeries);
    m_histogram2DNode->setXBins(m_xBins);
    m_histogram2DNode->setYBins(m_yBins);
    if (m_xRangeMin != 0.0 || m_xRangeMax != 0.0) {
        m_histogram2DNode->setXRangeMin(m_xRangeMin);
        m_histogram2DNode->setXRangeMax(m_xRangeMax);
    }
    if (m_yRangeMin != 0.0 || m_yRangeMax != 0.0) {
        m_histogram2DNode->setYRangeMin(m_yRangeMin);
        m_histogram2DNode->setYRangeMax(m_yRangeMax);
    }
    m_histogram2DNode->setDensity(m_density);
    m_histogram2DNode->setNoOutliers(m_noOutliers);
    m_histogram2DNode->setColMajor(m_colMajor);
}

void Histogram2DFunction::setTitle(const QString& title)
{
    if (m_title != title) {
        m_title = title;
        emit titleChanged(title);
        if (m_plotNode) {
            m_plotNode->setTitle(title);
        }
    }
}

void Histogram2DFunction::setXLabel(const QString& label)
{
    if (m_xLabel != label) {
        m_xLabel = label;
        emit xLabelChanged(label);
        if (m_plotNode) {
            m_plotNode->x1Axis()->setLabel(label);
        }
    }
}

void Histogram2DFunction::setYLabel(const QString& label)
{
    if (m_yLabel != label) {
        m_yLabel = label;
        emit yLabelChanged(label);
        if (m_plotNode) {
            m_plotNode->y1Axis()->setLabel(label);
        }
    }
}

void Histogram2DFunction::setXBins(int bins)
{
    if (m_xBins != bins) {
        m_xBins = bins;
        emit xBinsChanged(bins);
        if (m_histogram2DNode) {
            m_histogram2DNode->setXBins(bins);
        }
    }
}

void Histogram2DFunction::setYBins(int bins)
{
    if (m_yBins != bins) {
        m_yBins = bins;
        emit yBinsChanged(bins);
        if (m_histogram2DNode) {
            m_histogram2DNode->setYBins(bins);
        }
    }
}

void Histogram2DFunction::setXRangeMin(double min)
{
    if (m_xRangeMin != min) {
        m_xRangeMin = min;
        emit xRangeChanged();
        if (m_histogram2DNode) {
            m_histogram2DNode->setXRangeMin(min);
        }
    }
}

void Histogram2DFunction::setXRangeMax(double max)
{
    if (m_xRangeMax != max) {
        m_xRangeMax = max;
        emit xRangeChanged();
        if (m_histogram2DNode) {
            m_histogram2DNode->setXRangeMax(max);
        }
    }
}

void Histogram2DFunction::setYRangeMin(double min)
{
    if (m_yRangeMin != min) {
        m_yRangeMin = min;
        emit yRangeChanged();
        if (m_histogram2DNode) {
            m_histogram2DNode->setYRangeMin(min);
        }
    }
}

void Histogram2DFunction::setYRangeMax(double max)
{
    if (m_yRangeMax != max) {
        m_yRangeMax = max;
        emit yRangeChanged();
        if (m_histogram2DNode) {
            m_histogram2DNode->setYRangeMax(max);
        }
    }
}

void Histogram2DFunction::setDensity(bool density)
{
    if (m_density != density) {
        m_density = density;
        emit densityChanged(density);
        if (m_histogram2DNode) {
            m_histogram2DNode->setDensity(density);
        }
    }
}

void Histogram2DFunction::setNoOutliers(bool noOutliers)
{
    if (m_noOutliers != noOutliers) {
        m_noOutliers = noOutliers;
        emit noOutliersChanged(noOutliers);
        if (m_histogram2DNode) {
            m_histogram2DNode->setNoOutliers(noOutliers);
        }
    }
}

void Histogram2DFunction::setColMajor(bool colMajor)
{
    if (m_colMajor != colMajor) {
        m_colMajor = colMajor;
        emit colMajorChanged(colMajor);
        if (m_histogram2DNode) {
            m_histogram2DNode->setColMajor(colMajor);
        }
    }
}