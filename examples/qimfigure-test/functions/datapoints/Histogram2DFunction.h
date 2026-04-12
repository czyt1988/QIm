#ifndef HISTOGRAM2DFUNCTION_H
#define HISTOGRAM2DFUNCTION_H

#include "../TestFunction.h"
#include <QObject>
#include <QColor>

namespace QIM {
class QImFigureWidget;
class QImPlotNode;
class QImPlotHistogram2DItemNode;
}

/**
 * \if ENGLISH
 * @brief Test function demonstrating 2D histogram with property registration
 * 
 * Histogram2DFunction creates a 2D histogram (bivariate) with 1000 correlated random points
 * to test 2D histogram rendering capabilities. It registers properties for:
 * - Plot title
 * - X and Y axis labels
 * - X and Y bin counts/methods
 * - X and Y range min/max
 * - Density normalization flag
 * - Exclude outliers flag
 * - Column-major layout flag
 * 
 * @section usage_example Usage Example
 * 
 * \code
 * auto func = new Histogram2DFunction(this);
 * func->createPlot(figureWidget);
 * // Properties will be available via getRegisteredProperties()
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotNode
 * @see QImPlotHistogram2DItemNode
 * \endif
 * 
 * \if CHINESE
 * @brief 演示二维直方图的测试函数，带有属性注册
 * 
 * Histogram2DFunction 创建一个包含 1000 个相关随机点的二维直方图（双变量），用于测试二维直方图渲染能力。
 * 它注册了以下属性：
 * - 图表标题
 * - X 和 Y 轴标签
 * - X 和 Y 箱数/方法
 * - X 和 Y 范围最小/最大值
 * - 密度归一化标志
 * - 排除异常值标志
 * - 列主序布局标志
 * 
 * @section usage_example 使用示例
 * 
 * \code
 * auto func = new Histogram2DFunction(this);
 * func->createPlot(figureWidget);
 * // 属性可通过 getRegisteredProperties() 获取
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotNode
 * @see QImPlotHistogram2DItemNode
 * \endif
 */
class Histogram2DFunction : public TestFunction {
    Q_OBJECT
    
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString xLabel READ xLabel WRITE setXLabel NOTIFY xLabelChanged)
    Q_PROPERTY(QString yLabel READ yLabel WRITE setYLabel NOTIFY yLabelChanged)
    Q_PROPERTY(int xBins READ xBins WRITE setXBins NOTIFY xBinsChanged)
    Q_PROPERTY(int yBins READ yBins WRITE setYBins NOTIFY yBinsChanged)
    Q_PROPERTY(double xRangeMin READ xRangeMin WRITE setXRangeMin NOTIFY xRangeChanged)
    Q_PROPERTY(double xRangeMax READ xRangeMax WRITE setXRangeMax NOTIFY xRangeChanged)
    Q_PROPERTY(double yRangeMin READ yRangeMin WRITE setYRangeMin NOTIFY yRangeChanged)
    Q_PROPERTY(double yRangeMax READ yRangeMax WRITE setYRangeMax NOTIFY yRangeChanged)
    Q_PROPERTY(bool density READ isDensity WRITE setDensity NOTIFY densityChanged)
    Q_PROPERTY(bool noOutliers READ isNoOutliers WRITE setNoOutliers NOTIFY noOutliersChanged)
    Q_PROPERTY(bool colMajor READ isColMajor WRITE setColMajor NOTIFY colMajorChanged)
    
public:
    /**
     * \if ENGLISH
     * @brief Constructor for Histogram2DFunction
     * @param parent Parent object
     * \endif
     * 
     * \if CHINESE
     * @brief Histogram2DFunction 构造函数
     * @param parent 父对象
     * \endif
     */
    explicit Histogram2DFunction(QObject* parent = nullptr);
    
    /**
     * \if ENGLISH
     * @brief Destructor for Histogram2DFunction
     * \endif
     * 
     * \if CHINESE
     * @brief Histogram2DFunction 析构函数
     * \endif
     */
    ~Histogram2DFunction() override;
    
    /**
     * \if ENGLISH
     * @brief Create plot nodes in the given figure widget
     * @param figure Pointer to the figure widget where plots will be created
     * \endif
     * 
     * \if CHINESE
     * @brief 在指定的图表控件中创建绘图节点
     * @param figure 将要创建绘图的图表控件指针
     * \endif
     */
    void createPlot(QIM::QImFigureWidget* figure) override;
    
    // Title property accessors
    QString title() const { return m_title; }
    void setTitle(const QString& title);
    
    // X axis label property accessors
    QString xLabel() const { return m_xLabel; }
    void setXLabel(const QString& label);
    
    // Y axis label property accessors
    QString yLabel() const { return m_yLabel; }
    void setYLabel(const QString& label);
    
    // X bin count/method property accessors
    int xBins() const { return m_xBins; }
    void setXBins(int bins);
    
    // Y bin count/method property accessors
    int yBins() const { return m_yBins; }
    void setYBins(int bins);
    
    // X range min property accessors
    double xRangeMin() const { return m_xRangeMin; }
    void setXRangeMin(double min);
    
    // X range max property accessors
    double xRangeMax() const { return m_xRangeMax; }
    void setXRangeMax(double max);
    
    // Y range min property accessors
    double yRangeMin() const { return m_yRangeMin; }
    void setYRangeMin(double min);
    
    // Y range max property accessors
    double yRangeMax() const { return m_yRangeMax; }
    void setYRangeMax(double max);
    
    // Density flag property accessors
    bool isDensity() const { return m_density; }
    void setDensity(bool density);
    
    // Exclude outliers flag property accessors
    bool isNoOutliers() const { return m_noOutliers; }
    void setNoOutliers(bool noOutliers);
    
    // Column-major layout property accessors
    bool isColMajor() const { return m_colMajor; }
    void setColMajor(bool colMajor);
    
Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Signal emitted when title changes
     * @param title New title value
     * \endif
     * 
     * \if CHINESE
     * @brief 标题改变时发出的信号
     * @param title 新的标题值
     * \endif
     */
    void titleChanged(const QString& title);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when X axis label changes
     * @param label New X axis label value
     * \endif
     * 
     * \if CHINESE
     * @brief X 轴标签改变时发出的信号
     * @param label 新的 X 轴标签值
     * \endif
     */
    void xLabelChanged(const QString& label);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when Y axis label changes
     * @param label New Y axis label value
     * \endif
     * 
     * \if CHINESE
     * @brief Y 轴标签改变时发出的信号
     * @param label 新的 Y 轴标签值
     * \endif
     */
    void yLabelChanged(const QString& label);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when X bin count changes
     * @param bins New X bin count value
     * \endif
     * 
     * \if CHINESE
     * @brief X箱数改变时发出的信号
     * @param bins 新的X箱数值
     * \endif
     */
    void xBinsChanged(int bins);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when Y bin count changes
     * @param bins New Y bin count value
     * \endif
     * 
     * \if CHINESE
     * @brief Y箱数改变时发出的信号
     * @param bins 新的Y箱数值
     * \endif
     */
    void yBinsChanged(int bins);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when X or Y range changes
     * \endif
     * 
     * \if CHINESE
     * @brief X或Y范围改变时发出的信号
     * \endif
     */
    void xRangeChanged();
    void yRangeChanged();
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when density flag changes
     * @param density New density flag value
     * \endif
     * 
     * \if CHINESE
     * @brief 密度标志改变时发出的信号
     * @param density 新的密度标志值
     * \endif
     */
    void densityChanged(bool density);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when exclude outliers flag changes
     * @param noOutliers New exclude outliers flag value
     * \endif
     * 
     * \if CHINESE
     * @brief 排除异常值标志改变时发出的信号
     * @param noOutliers 新的排除异常值标志值
     * \endif
     */
    void noOutliersChanged(bool noOutliers);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when column-major layout flag changes
     * @param colMajor New column-major state
     * \endif
     * 
     * \if CHINESE
     * @brief 列主序布局标志改变时发出的信号
     * @param colMajor 新的列主序状态
     * \endif
     */
    void colMajorChanged(bool colMajor);
    
private:
    QString m_title = QStringLiteral("2D Histogram");
    QString m_xLabel = QStringLiteral("X");
    QString m_yLabel = QStringLiteral("Y");
    int m_xBins = -2;  // ImPlotBin_Sturges
    int m_yBins = -2;
    double m_xRangeMin = 0.0;
    double m_xRangeMax = 0.0;
    double m_yRangeMin = 0.0;
    double m_yRangeMax = 0.0;
    bool m_density = false;
    bool m_noOutliers = false;
    bool m_colMajor = false;
    
    QIM::QImPlotNode* m_plotNode = nullptr;
    QIM::QImPlotHistogram2DItemNode* m_histogram2DNode = nullptr;
};

#endif // HISTOGRAM2DFUNCTION_H