#ifndef HISTOGRAMFUNCTION_H
#define HISTOGRAMFUNCTION_H

#include "../TestFunction.h"
#include <QObject>
#include <QColor>

namespace QIM {
class QImFigureWidget;
class QImPlotNode;
class QImPlotHistogramItemNode;
class QImPlotValueTrackerNode;
}

/**
 * \if ENGLISH
 * @brief Test function demonstrating histogram with property registration
 * 
 * HistogramFunction creates a histogram with 1000 random values (normal distribution)
 * to test histogram rendering capabilities. It registers properties for:
 * - Plot title
 * - X and Y axis labels
 * - Bin count/method
 * - Bar scale factor
 * - Range min/max
 * - Cumulative flag
 * - Density flag
 * - Horizontal orientation flag
 * - Exclude outliers flag
 * - Bar color
 * 
 * @section usage_example Usage Example
 * 
 * \code
 * auto func = new HistogramFunction(this);
 * func->createPlot(figureWidget);
 * // Properties will be available via getRegisteredProperties()
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotNode
 * @see QImPlotHistogramItemNode
 * \endif
 * 
 * \if CHINESE
 * @brief 演示直方图的测试函数，带有属性注册
 * 
 * HistogramFunction 创建一个包含 1000 个随机值（正态分布）的直方图，用于测试直方图渲染能力。
 * 它注册了以下属性：
 * - 图表标题
 * - X 和 Y 轴标签
 * - 箱数/方法
 * - 柱条缩放因子
 * - 范围最小/最大值
 * - 累积标志
 * - 密度标志
 * - 水平方向标志
 * - 排除异常值标志
 * - 柱条颜色
 * 
 * @section usage_example 使用示例
 * 
 * \code
 * auto func = new HistogramFunction(this);
 * func->createPlot(figureWidget);
 * // 属性可通过 getRegisteredProperties() 获取
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotNode
 * @see QImPlotHistogramItemNode
 * \endif
 */
class HistogramFunction : public TestFunction {
    Q_OBJECT
    
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString xLabel READ xLabel WRITE setXLabel NOTIFY xLabelChanged)
    Q_PROPERTY(QString yLabel READ yLabel WRITE setYLabel NOTIFY yLabelChanged)
    Q_PROPERTY(int bins READ bins WRITE setBins NOTIFY binsChanged)
    Q_PROPERTY(double barScale READ barScale WRITE setBarScale NOTIFY barScaleChanged)
    Q_PROPERTY(double rangeMin READ rangeMin WRITE setRangeMin NOTIFY rangeChanged)
    Q_PROPERTY(double rangeMax READ rangeMax WRITE setRangeMax NOTIFY rangeChanged)
    Q_PROPERTY(bool cumulative READ isCumulative WRITE setCumulative NOTIFY cumulativeChanged)
    Q_PROPERTY(bool density READ isDensity WRITE setDensity NOTIFY densityChanged)
    Q_PROPERTY(bool horizontal READ isHorizontal WRITE setHorizontal NOTIFY orientationChanged)
    Q_PROPERTY(bool outliersIncluded READ isOutliersIncluded WRITE setOutliersIncluded NOTIFY outliersIncludedChanged)
    Q_PROPERTY(QColor barColor READ barColor WRITE setBarColor NOTIFY barColorChanged)
    
public:
    /**
     * \if ENGLISH
     * @brief Constructor for HistogramFunction
     * @param parent Parent object
     * \endif
     * 
     * \if CHINESE
     * @brief HistogramFunction 构造函数
     * @param parent 父对象
     * \endif
     */
    explicit HistogramFunction(QObject* parent = nullptr);
    
    /**
     * \if ENGLISH
     * @brief Destructor for HistogramFunction
     * \endif
     * 
     * \if CHINESE
     * @brief HistogramFunction 析构函数
     * \endif
     */
    ~HistogramFunction() override;
    
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
    
    // Bin count/method property accessors
    int bins() const { return m_bins; }
    void setBins(int bins);
    
    // Bar scale property accessors
    double barScale() const { return m_barScale; }
    void setBarScale(double scale);
    
    // Range min property accessors
    double rangeMin() const { return m_rangeMin; }
    void setRangeMin(double min);
    
    // Range max property accessors
    double rangeMax() const { return m_rangeMax; }
    void setRangeMax(double max);
    
    // Cumulative flag property accessors
    bool isCumulative() const { return m_cumulative; }
    void setCumulative(bool cumulative);
    
    // Density flag property accessors
    bool isDensity() const { return m_density; }
    void setDensity(bool density);
    
    // Horizontal orientation property accessors
    bool isHorizontal() const { return m_horizontal; }
    void setHorizontal(bool horizontal);
    
    // Outliers inclusion flag property accessors
    bool isOutliersIncluded() const { return m_outliersIncluded; }
    void setOutliersIncluded(bool included);
    
    // Bar color property accessors
    QColor barColor() const { return m_barColor; }
    void setBarColor(const QColor& color);
    
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
     * @brief Signal emitted when bin count changes
     * @param bins New bin count value
     * \endif
     * 
     * \if CHINESE
     * @brief 箱数改变时发出的信号
     * @param bins 新的箱数值
     * \endif
     */
    void binsChanged(int bins);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when bar scale changes
     * @param scale New bar scale value
     * \endif
     * 
     * \if CHINESE
     * @brief 柱条缩放因子改变时发出的信号
     * @param scale 新的柱条缩放因子值
     * \endif
     */
    void barScaleChanged(double scale);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when histogram range changes
     * \endif
     * 
     * \if CHINESE
     * @brief 直方图范围改变时发出的信号
     * \endif
     */
    void rangeChanged();
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when cumulative flag changes
     * @param cumulative New cumulative flag value
     * \endif
     * 
     * \if CHINESE
     * @brief 累积标志改变时发出的信号
     * @param cumulative 新的累积标志值
     * \endif
     */
    void cumulativeChanged(bool cumulative);
    
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
     * @brief Signal emitted when orientation changes
     * @param horizontal New horizontal orientation value
     * \endif
     * 
     * \if CHINESE
     * @brief 方向改变时发出的信号
     * @param horizontal 新的水平方向值
     * \endif
     */
    void orientationChanged(bool horizontal);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when outliers inclusion flag changes
     * @param included New outliers inclusion flag value
     * \endif
     *
     * \if CHINESE
     * @brief 异常值包含标志改变时发出的信号
     * @param included 新的异常值包含标志值
     * \endif
     */
    void outliersIncludedChanged(bool included);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when bar color changes
     * @param color New bar color value
     * \endif
     * 
     * \if CHINESE
     * @brief 柱条颜色改变时发出的信号
     * @param color 新的柱条颜色值
     * \endif
     */
    void barColorChanged(const QColor& color);
    
private:
    QString m_title = QStringLiteral("Histogram");
    QString m_xLabel = QStringLiteral("x");
    QString m_yLabel = QStringLiteral("Frequency");
    int m_bins = -2;  // ImPlotBin_Sturges
    double m_barScale = 1.0;
    double m_rangeMin = 0.0;
    double m_rangeMax = 0.0;
    bool m_cumulative = false;
    bool m_density = false;
    bool m_horizontal = false;
    bool m_outliersIncluded = true;
    QColor m_barColor = Qt::green;
    
    QIM::QImPlotNode* m_plotNode = nullptr;
    QIM::QImPlotHistogramItemNode* m_histogramNode = nullptr;
    QIM::QImPlotValueTrackerNode* m_trackerNode = nullptr;
};

#endif // HISTOGRAMFUNCTION_H