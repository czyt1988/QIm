#ifndef QIMPLOTHISTOGRAMITEMNODE_H
#define QIMPLOTHISTOGRAMITEMNODE_H

#include <QColor>
#include <type_traits>
#include "QImAPI.h"
#include "QImPlotItemNode.h"
#include "QImPlotDataSeries.h"
#include "QImPlotHistogramDataSeries.h"

namespace QIM
{
class QImAbstractHistogramDataSeries;

/**
 * \if ENGLISH
 * @brief Qt-style wrapper for ImPlot histogram visualization
 *
 * @class QImPlotHistogramItemNode
 * @ingroup plot_items
 *
 * @details Provides Qt-style retained mode encapsulation for ImPlot histograms.
 *          Supports customizable binning, bar scaling, range filtering,
 *          and full Qt property system integration with signal-slot event handling.
 *          Inherits from QImPlotItemNode and follows the same PIMPL design pattern
 *          as QImPlotBarsItemNode for consistency.
 *
 * @note Histograms visualize distribution of single-value data by binning values into bars.
 *       Use QImPlotBarsItemNode for categorical or discrete XY bar charts.
 * @warning Bin count changes require redraw to take effect.
 *
 * @param[in] parent Parent QObject (optional)
 *
 * @see QImPlotItemNode
 * @see QImPlotBarsItemNode
 * @see QImPlotNode
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot直方图可视化的Qt风格封装
 *
 * @class QImPlotHistogramItemNode
 * @ingroup plot_items
 *
 * @details 为ImPlot直方图提供Qt风格的保留模式封装。
 *          支持可自定义的装箱、柱条缩放、范围过滤，
 *          以及完整的Qt属性系统集成和信号槽事件处理。
 *          继承自QImPlotItemNode，并遵循与QImPlotBarsItemNode相同的PIMPL设计模式以保持一致性。
 *
 * @note 直方图通过将单值数据装箱到柱条中来可视化数据分布。
 *       分类或离散XY柱状图请使用QImPlotBarsItemNode。
 * @warning 箱数更改需要重绘才能生效。
 *
 * @param[in] parent 父QObject对象（可选）
 *
 * @see QImPlotItemNode
 * @see QImPlotBarsItemNode
 * @see QImPlotNode
 * \endif
 */
class QIM_CORE_API QImPlotHistogramItemNode : public QImPlotItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotHistogramItemNode)

    /**
     * \if ENGLISH
     * @property QImPlotHistogramItemNode::bins
     * @brief Number of bins or automatic binning method
     *
     * @details Controls the binning strategy for histogram.
     *          Positive integer: exact bin count.
     *          ImPlotBin enumeration: automatic method.
     *          Default value is ImPlotBin_Sturges (-2).
     * @accessors READ bins WRITE setBins NOTIFY binsChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotHistogramItemNode::bins
     * @brief 箱数或自动装箱方法
     *
     * @details 控制直方图的装箱策略。
     *          正整数：精确箱数。
     *          ImPlotBin枚举：自动方法。
     *          默认值为ImPlotBin_Sturges (-2)。
     * @accessors READ bins WRITE setBins NOTIFY binsChanged
     * \endif
     */
    Q_PROPERTY(int bins READ bins WRITE setBins NOTIFY binsChanged)

    /**
     * \if ENGLISH
     * @property QImPlotHistogramItemNode::barScale
     * @brief Scale factor for bar width
     *
     * @details Controls the visual width of histogram bars.
     *          Value is a scale factor (default 1.0).
     * @accessors READ barScale WRITE setBarScale NOTIFY barScaleChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotHistogramItemNode::barScale
     * @brief 柱条宽度的缩放因子
     *
     * @details 控制直方图柱条的视觉宽度。
     *          值为缩放因子（默认1.0）。
     * @accessors READ barScale WRITE setBarScale NOTIFY barScaleChanged
     * \endif
     */
    Q_PROPERTY(double barScale READ barScale WRITE setBarScale NOTIFY barScaleChanged)

    /**
     * \if ENGLISH
     * @property QImPlotHistogramItemNode::rangeMin
     * @brief Minimum value for binning range
     *
     * @details Defines the lower bound of the histogram range.
     *          If equal to rangeMax (default 0), automatic range from data is used.
     * @accessors READ rangeMin WRITE setRangeMin NOTIFY rangeChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotHistogramItemNode::rangeMin
     * @brief 装箱范围的最小值
     *
     * @details 定义直方图范围的下界。
     *          如果等于rangeMax（默认0），则使用数据的自动范围。
     * @accessors READ rangeMin WRITE setRangeMin NOTIFY rangeChanged
     * \endif
     */
    Q_PROPERTY(double rangeMin READ rangeMin WRITE setRangeMin NOTIFY rangeChanged)

    /**
     * \if ENGLISH
     * @property QImPlotHistogramItemNode::rangeMax
     * @brief Maximum value for binning range
     *
     * @details Defines the upper bound of the histogram range.
     *          If equal to rangeMin (default 0), automatic range from data is used.
     * @accessors READ rangeMax WRITE setRangeMax NOTIFY rangeChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotHistogramItemNode::rangeMax
     * @brief 装箱范围的最大值
     *
     * @details 定义直方图范围的上界。
     *          如果等于rangeMin（默认0），则使用数据的自动范围。
     * @accessors READ rangeMax WRITE setRangeMax NOTIFY rangeChanged
     * \endif
     */
    Q_PROPERTY(double rangeMax READ rangeMax WRITE setRangeMax NOTIFY rangeChanged)

    /**
     * \if ENGLISH
     * @property QImPlotHistogramItemNode::cumulative
     * @brief Cumulative histogram flag
     *
     * @details When true, each bin contains its count plus the counts of all previous bins.
     *          Corresponds to ImPlotHistogramFlags_Cumulative.
     * @accessors READ isCumulative WRITE setCumulative NOTIFY cumulativeChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotHistogramItemNode::cumulative
     * @brief 累积直方图标志
     *
     * @details 为true时，每个箱包含其计数加上之前所有箱的计数。
     *          对应于ImPlotHistogramFlags_Cumulative。
     * @accessors READ isCumulative WRITE setCumulative NOTIFY cumulativeChanged
     * \endif
     */
    Q_PROPERTY(bool cumulative READ isCumulative WRITE setCumulative NOTIFY cumulativeChanged)

    /**
     * \if ENGLISH
     * @property QImPlotHistogramItemNode::density
     * @brief Density normalization flag
     *
     * @details When true, counts are normalized to represent probability density (PDF).
     *          Corresponds to ImPlotHistogramFlags_Density.
     * @accessors READ isDensity WRITE setDensity NOTIFY densityChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotHistogramItemNode::density
     * @brief 密度归一化标志
     *
     * @details 为true时，计数被归一化以表示概率密度（PDF）。
     *          对应于ImPlotHistogramFlags_Density。
     * @accessors READ isDensity WRITE setDensity NOTIFY densityChanged
     * \endif
     */
    Q_PROPERTY(bool density READ isDensity WRITE setDensity NOTIFY densityChanged)

    /**
     * \if ENGLISH
     * @property QImPlotHistogramItemNode::horizontal
     * @brief Horizontal orientation flag
     *
     * @details When true, histogram bars are rendered horizontally.
     *          Corresponds to ImPlotHistogramFlags_Horizontal.
     * @accessors READ isHorizontal WRITE setHorizontal NOTIFY orientationChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotHistogramItemNode::horizontal
     * @brief 水平方向标志
     *
     * @details 为true时，直方图柱条水平渲染。
     *          对应于ImPlotHistogramFlags_Horizontal。
     * @accessors READ isHorizontal WRITE setHorizontal NOTIFY orientationChanged
     * \endif
     */
    Q_PROPERTY(bool horizontal READ isHorizontal WRITE setHorizontal NOTIFY orientationChanged)

    /**
     * \if ENGLISH
     * @property QImPlotHistogramItemNode::noOutliers
     * @brief Exclude outliers flag
     *
     * @details When true, values outside the specified histogram range are excluded from normalization and cumulative counts.
     *          Corresponds to ImPlotHistogramFlags_NoOutliers.
     * @accessors READ isNoOutliers WRITE setNoOutliers NOTIFY noOutliersChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotHistogramItemNode::noOutliers
     * @brief 排除异常值标志
     *
     * @details 为true时，指定直方图范围之外的值将从归一化和累积计数中排除。
     *          对应于ImPlotHistogramFlags_NoOutliers。
     * @accessors READ isNoOutliers WRITE setNoOutliers NOTIFY noOutliersChanged
     * \endif
     */
    Q_PROPERTY(bool noOutliers READ isNoOutliers WRITE setNoOutliers NOTIFY noOutliersChanged)

    /**
     * \if ENGLISH
     * @property QImPlotHistogramItemNode::color
     * @brief Color of histogram bars
     *
     * @details Defines the primary color for histogram bars.
     *          When not set, uses ImPlot's default color sequence.
     * @accessors READ color WRITE setColor NOTIFY colorChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotHistogramItemNode::color
     * @brief 直方图柱条的颜色
     *
     * @details 定义直方图柱条的主要颜色。
     *          未设置时，使用ImPlot的默认颜色序列。
     * @accessors READ color WRITE setColor NOTIFY colorChanged
     * \endif
     */
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    // Unique type identifier for QImPlotHistogramItemNode
    enum
    {
        Type = InnerType + 11
    };

    // Returns the type identifier of this plot item
    virtual int type() const override
    {
        return Type;
    }

    // Constructs a QImPlotHistogramItemNode with optional parent
    QImPlotHistogramItemNode(QObject* parent = nullptr);

    // Destroys the QImPlotHistogramItemNode
    ~QImPlotHistogramItemNode();

    //----------------------------------------------------
    // Data setting interface
    //----------------------------------------------------

    // Sets the data series for the histogram (Y values only)
    void setData(QImAbstractXYDataSeries* series);

    // Sets histogram data from single value container (Y-only mode)
    template< typename ContainerY >
    std::enable_if_t< !std::is_pointer< ContainerY >::value, QImAbstractHistogramDataSeries* >
    setData(const ContainerY& y);

    // Sets histogram data from single value container (move semantics)
    template< typename ContainerY >
    std::enable_if_t< !std::is_pointer< typename std::remove_reference< ContainerY >::type >::value, QImAbstractHistogramDataSeries* >
    setData(ContainerY&& y);

    // Gets the current data series
    QImAbstractXYDataSeries* data() const;

    //----------------------------------------------------
    // Style property accessors
    //----------------------------------------------------

    // Gets the bin count or automatic method
    int bins() const;

    // Sets the bin count or automatic method
    void setBins(int bins);

    // Gets the bar scale factor
    double barScale() const;

    // Sets the bar scale factor
    void setBarScale(double scale);

    // Gets the range minimum (0 = auto)
    double rangeMin() const;

    // Sets the range minimum (0 = auto)
    void setRangeMin(double min);

    // Gets the range maximum (0 = auto)
    double rangeMax() const;

    // Sets the range maximum (0 = auto)
    void setRangeMax(double max);

    // Checks if cumulative mode is enabled
    bool isCumulative() const;

    // Sets cumulative mode
    void setCumulative(bool cumulative);

    // Checks if density normalization is enabled
    bool isDensity() const;

    // Sets density normalization
    void setDensity(bool density);

    // Checks if horizontal orientation is enabled
    bool isHorizontal() const;

    // Sets horizontal orientation
    void setHorizontal(bool horizontal);

    // Checks if outliers are excluded
    bool isNoOutliers() const;

    // Sets exclude outliers flag
    void setNoOutliers(bool noOutliers);

    // Gets the histogram bar color
    QColor color() const;

    // Sets the histogram bar color
    void setColor(const QColor& color);

    // Gets the raw ImPlotHistogramFlags
    int histogramFlags() const;

    // Sets the raw ImPlotHistogramFlags
    void setHistogramFlags(int flags);

Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when bin count changes
     * @param[in] bins New bin count or method
     * @details Triggered by setBins() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 箱数更改时触发
     * @param[in] bins 新箱数或方法
     * @details 当值实际更改时由setBins()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void binsChanged(int bins);

    /**
     * \if ENGLISH
     * @brief Emitted when bar scale changes
     * @param[in] scale New bar scale factor
     * @details Triggered by setBarScale() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 柱条缩放因子更改时触发
     * @param[in] scale 新柱条缩放因子
     * @details 当值实际更改时由setBarScale()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void barScaleChanged(double scale);

    /**
     * \if ENGLISH
     * @brief Emitted when histogram range changes
     * @details Triggered by setRangeMin() or setRangeMax() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 直方图范围更改时触发
     * @details 当值实际更改时由setRangeMin()或setRangeMax()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void rangeChanged();

    /**
     * \if ENGLISH
     * @brief Emitted when cumulative flag changes
     * @param[in] cumulative New cumulative flag state
     * @details Triggered by setCumulative() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 累积标志更改时触发
     * @param[in] cumulative 新累积标志状态
     * @details 当值实际更改时由setCumulative()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void cumulativeChanged(bool cumulative);

    /**
     * \if ENGLISH
     * @brief Emitted when density flag changes
     * @param[in] density New density flag state
     * @details Triggered by setDensity() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 密度标志更改时触发
     * @param[in] density 新密度标志状态
     * @details 当值实际更改时由setDensity()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void densityChanged(bool density);

    /**
     * \if ENGLISH
     * @brief Emitted when orientation changes
     * @param[in] horizontal New horizontal orientation state
     * @details Triggered by setHorizontal() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 方向更改时触发
     * @param[in] horizontal 新水平方向状态
     * @details 当值实际更改时由setHorizontal()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void orientationChanged(bool horizontal);

    /**
     * \if ENGLISH
     * @brief Emitted when exclude outliers flag changes
     * @param[in] noOutliers New exclude outliers flag state
     * @details Triggered by setNoOutliers() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 排除异常值标志更改时触发
     * @param[in] noOutliers 新排除异常值标志状态
     * @details 当值实际更改时由setNoOutliers()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void noOutliersChanged(bool noOutliers);

    /**
     * \if ENGLISH
     * @brief Emitted when histogram bar color changes
     * @param[in] color New histogram bar color
     * @details Triggered by setColor() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 直方图柱条颜色更改时触发
     * @param[in] color 新直方图柱条颜色
     * @details 当值实际更改时由setColor()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void colorChanged(const QColor& color);

    /**
     * \if ENGLISH
     * @brief Emitted when data series changes
     * @details Triggered by setData() when new data is assigned.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 数据系列更改时触发
     * @details 当分配新数据时由setData()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void dataChanged();

    /**
     * \if ENGLISH
     * @brief Emitted when histogram flags change
     * @details Triggered by any flag property change (cumulative, density, horizontal, noOutliers).
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 直方图标志更改时触发
     * @details 任何标志属性更改（累积、密度、水平方向、排除异常值）时触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void histogramFlagChanged();

protected:
    // Begins drawing the histogram
    virtual bool beginDraw() override;
};

// Template function implementation
template< typename ContainerY >
inline std::enable_if_t< !std::is_pointer< ContainerY >::value, QImAbstractHistogramDataSeries* >
QImPlotHistogramItemNode::setData(const ContainerY& y)
{
    using ConcreteSeries = QImVectorHistogramDataSeries<ContainerY>;
    ConcreteSeries* d = new ConcreteSeries(y);
    setData(d);
    return d;
}

template< typename ContainerY >
inline std::enable_if_t< !std::is_pointer< typename std::remove_reference< ContainerY >::type >::value, QImAbstractHistogramDataSeries* >
QImPlotHistogramItemNode::setData(ContainerY&& y)
{
    using ConcreteSeries = QImVectorHistogramDataSeries<typename std::remove_reference< ContainerY >::type>;
    ConcreteSeries* d = new ConcreteSeries(std::move(y));
    setData(d);
    return d;
}

}  // end namespace QIM

#endif  // QIMPLOTHISTOGRAMITEMNODE_H