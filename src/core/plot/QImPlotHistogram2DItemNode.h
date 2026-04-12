#ifndef QIMPLOTHISTOGRAM2DITEMNODE_H
#define QIMPLOTHISTOGRAM2DITEMNODE_H

#include "../../QImAPI.h"
#include <QColor>
#include <QPointF>
#include "QImPlotItemNode.h"
#include "QImPlotDataSeries.h"

namespace QIM
{
class QImAbstractXYDataSeries;

/**
 * \if ENGLISH
 * @brief Qt-style wrapper for ImPlot 2D histogram visualization
 *
 * @class QImPlotHistogram2DItemNode
 * @ingroup plot_items
 *
 * @details Provides Qt-style retained mode encapsulation for ImPlot 2D histograms (bivariate).
 *          Supports customizable binning in X and Y dimensions, range filtering,
 *          density normalization, outlier exclusion, and column-major data layout.
 *          Inherits from QImPlotItemNode and follows the same PIMPL design pattern
 *          as QImPlotHeatmapItemNode for consistency.
 *
 * @note 2D histograms visualize joint distribution of two variables as a heatmap of binned counts.
 *       Useful for correlation analysis, density estimation, and 2D data exploration.
 * @warning Large bin counts (>100x100) may impact performance.
 *
 * @param[in] parent Parent QObject (optional)
 *
 * @see QImPlotItemNode
 * @see QImPlotHeatmapItemNode
 * @see QImPlotHistogramItemNode
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot二维直方图可视化的Qt风格封装
 *
 * @class QImPlotHistogram2DItemNode
 * @ingroup plot_items
 *
 * @details 为ImPlot二维直方图（双变量）提供Qt风格的保留模式封装。
 *          支持可自定义的X和Y维度装箱、范围过滤、
 *          密度归一化、异常值排除和列主序数据布局。
 *          继承自QImPlotItemNode，并遵循与QImPlotHeatmapItemNode相同的PIMPL设计模式以保持一致性。
 *
 * @note 二维直方图将两个变量的联合分布可视化为装箱计数的热力图。
 *       适用于相关性分析、密度估计和二维数据探索。
 * @warning 大量箱数（>100x100）可能影响性能。
 *
 * @param[in] parent 父QObject对象（可选）
 *
 * @see QImPlotItemNode
 * @see QImPlotHeatmapItemNode
 * @see QImPlotHistogramItemNode
 * \endif
 */
class QIM_CORE_API QImPlotHistogram2DItemNode : public QImPlotItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotHistogram2DItemNode)

    /**
     * \if ENGLISH
     * @property QImPlotHistogram2DItemNode::xBins
     * @brief Number of bins in X dimension or automatic binning method
     *
     * @details Controls the binning strategy for X dimension.
     *          Positive integer: exact bin count.
     *          ImPlotBin enumeration: automatic method.
     *          Default value is ImPlotBin_Sturges (-2).
     * @accessors READ xBins WRITE setXBins NOTIFY xBinsChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotHistogram2DItemNode::xBins
     * @brief X维度的箱数或自动装箱方法
     *
     * @details 控制X维度的装箱策略。
     *          正整数：精确箱数。
     *          ImPlotBin枚举：自动方法。
     *          默认值为ImPlotBin_Sturges (-2)。
     * @accessors READ xBins WRITE setXBins NOTIFY xBinsChanged
     * \endif
     */
    Q_PROPERTY(int xBins READ xBins WRITE setXBins NOTIFY xBinsChanged)

    /**
     * \if ENGLISH
     * @property QImPlotHistogram2DItemNode::yBins
     * @brief Number of bins in Y dimension or automatic binning method
     *
     * @details Controls the binning strategy for Y dimension.
     *          Positive integer: exact bin count.
     *          ImPlotBin enumeration: automatic method.
     *          Default value is ImPlotBin_Sturges (-2).
     * @accessors READ yBins WRITE setYBins NOTIFY yBinsChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotHistogram2DItemNode::yBins
     * @brief Y维度的箱数或自动装箱方法
     *
     * @details 控制Y维度的装箱策略。
     *          正整数：精确箱数。
     *          ImPlotBin枚举：自动方法。
     *          默认值为ImPlotBin_Sturges (-2)。
     * @accessors READ yBins WRITE setYBins NOTIFY yBinsChanged
     * \endif
     */
    Q_PROPERTY(int yBins READ yBins WRITE setYBins NOTIFY yBinsChanged)

    /**
     * \if ENGLISH
     * @property QImPlotHistogram2DItemNode::xRangeMin
     * @brief Minimum value for X binning range
     *
     * @details Defines the lower bound of the X histogram range.
     *          If equal to xRangeMax (default 0), automatic range from data is used.
     * @accessors READ xRangeMin WRITE setXRangeMin NOTIFY xRangeChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotHistogram2DItemNode::xRangeMin
     * @brief X装箱范围的最小值
     *
     * @details 定义X直方图范围的下界。
     *          如果等于xRangeMax（默认0），则使用数据的自动范围。
     * @accessors READ xRangeMin WRITE setXRangeMin NOTIFY xRangeChanged
     * \endif
     */
    Q_PROPERTY(double xRangeMin READ xRangeMin WRITE setXRangeMin NOTIFY xRangeChanged)

    /**
     * \if ENGLISH
     * @property QImPlotHistogram2DItemNode::xRangeMax
     * @brief Maximum value for X binning range
     *
     * @details Defines the upper bound of the X histogram range.
     *          If equal to xRangeMin (default 0), automatic range from data is used.
     * @accessors READ xRangeMax WRITE setXRangeMax NOTIFY xRangeChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotHistogram2DItemNode::xRangeMax
     * @brief X装箱范围的最大值
     *
     * @details 定义X直方图范围的上界。
     *          如果等于xRangeMin（默认0），则使用数据的自动范围。
     * @accessors READ xRangeMax WRITE setXRangeMax NOTIFY xRangeChanged
     * \endif
     */
    Q_PROPERTY(double xRangeMax READ xRangeMax WRITE setXRangeMax NOTIFY xRangeChanged)

    /**
     * \if ENGLISH
     * @property QImPlotHistogram2DItemNode::yRangeMin
     * @brief Minimum value for Y binning range
     *
     * @details Defines the lower bound of the Y histogram range.
     *          If equal to yRangeMax (default 0), automatic range from data is used.
     * @accessors READ yRangeMin WRITE setYRangeMin NOTIFY yRangeChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotHistogram2DItemNode::yRangeMin
     * @brief Y装箱范围的最小值
     *
     * @details 定义Y直方图范围的下界。
     *          如果等于yRangeMax（默认0），则使用数据的自动范围。
     * @accessors READ yRangeMin WRITE setYRangeMin NOTIFY yRangeChanged
     * \endif
     */
    Q_PROPERTY(double yRangeMin READ yRangeMin WRITE setYRangeMin NOTIFY yRangeChanged)

    /**
     * \if ENGLISH
     * @property QImPlotHistogram2DItemNode::yRangeMax
     * @brief Maximum value for Y binning range
     *
     * @details Defines the upper bound of the Y histogram range.
     *          If equal to yRangeMin (default 0), automatic range from data is used.
     * @accessors READ yRangeMax WRITE setYRangeMax NOTIFY yRangeChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotHistogram2DItemNode::yRangeMax
     * @brief Y装箱范围的最大值
     *
     * @details 定义Y直方图范围的上界。
     *          如果等于yRangeMin（默认0），则使用数据的自动范围。
     * @accessors READ yRangeMax WRITE setYRangeMax NOTIFY yRangeChanged
     * \endif
     */
    Q_PROPERTY(double yRangeMax READ yRangeMax WRITE setYRangeMax NOTIFY yRangeChanged)

    /**
     * \if ENGLISH
     * @property QImPlotHistogram2DItemNode::density
     * @brief Density normalization flag
     *
     * @details When true, counts are normalized to represent probability density (PDF).
     *          Corresponds to ImPlotHistogramFlags_Density.
     * @accessors READ isDensity WRITE setDensity NOTIFY densityChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotHistogram2DItemNode::density
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
     * @property QImPlotHistogram2DItemNode::noOutliers
     * @brief Exclude outliers flag
     *
     * @details When true, values outside the specified histogram range are excluded from normalization and cumulative counts.
     *          Corresponds to ImPlotHistogramFlags_NoOutliers.
     * @accessors READ isNoOutliers WRITE setNoOutliers NOTIFY noOutliersChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotHistogram2DItemNode::noOutliers
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
     * @property QImPlotHistogram2DItemNode::colMajor
     * @brief Column-major data layout flag
     *
     * @details When true, data is assumed to be stored in column-major order.
     *          Corresponds to ImPlotHistogramFlags_ColMajor.
     * @accessors READ isColMajor WRITE setColMajor NOTIFY colMajorChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotHistogram2DItemNode::colMajor
     * @brief 列主序数据布局标志
     *
     * @details 为true时，假定数据按列主序存储。
     *          对应于ImPlotHistogramFlags_ColMajor。
     * @accessors READ isColMajor WRITE setColMajor NOTIFY colMajorChanged
     * \endif
     */
    Q_PROPERTY(bool colMajor READ isColMajor WRITE setColMajor NOTIFY colMajorChanged)

public:
    // Unique type identifier for QImPlotHistogram2DItemNode
    enum
    {
        Type = InnerType + 12   // Ensure unique
    };

    // Returns the type identifier of this plot item
    virtual int type() const override
    {
        return Type;
    }

    // Constructs a QImPlotHistogram2DItemNode with optional parent
    QImPlotHistogram2DItemNode(QObject* parent = nullptr);

    // Destroys the QImPlotHistogram2DItemNode
    ~QImPlotHistogram2DItemNode();

    //----------------------------------------------------
    // Data setting interface
    //----------------------------------------------------

    // Sets the data series for the 2D histogram (X and Y arrays)
    void setData(QImAbstractXYDataSeries* series);

    // Sets 2D histogram data from X and Y containers
    template< typename ContainerX, typename ContainerY >
    QImAbstractXYDataSeries* setData(const ContainerX& xs, const ContainerY& ys);

    // Sets 2D histogram data from X and Y containers (move semantics)
    template< typename ContainerX, typename ContainerY >
    QImAbstractXYDataSeries* setData(ContainerX&& xs, ContainerY&& ys);

    // Gets the current data series
    QImAbstractXYDataSeries* data() const;

    //----------------------------------------------------
    // Style property accessors
    //----------------------------------------------------

    // Gets the X bin count or automatic method
    int xBins() const;

    // Sets the X bin count or automatic method
    void setXBins(int bins);

    // Gets the Y bin count or automatic method
    int yBins() const;

    // Sets the Y bin count or automatic method
    void setYBins(int bins);

    // Gets the X range minimum (0 = auto)
    double xRangeMin() const;

    // Sets the X range minimum (0 = auto)
    void setXRangeMin(double min);

    // Gets the X range maximum (0 = auto)
    double xRangeMax() const;

    // Sets the X range maximum (0 = auto)
    void setXRangeMax(double max);

    // Gets the Y range minimum (0 = auto)
    double yRangeMin() const;

    // Sets the Y range minimum (0 = auto)
    void setYRangeMin(double min);

    // Gets the Y range maximum (0 = auto)
    double yRangeMax() const;

    // Sets the Y range maximum (0 = auto)
    void setYRangeMax(double max);

    // Checks if density normalization is enabled
    bool isDensity() const;

    // Sets density normalization
    void setDensity(bool density);

    // Checks if outliers are excluded
    bool isNoOutliers() const;

    // Sets exclude outliers flag
    void setNoOutliers(bool noOutliers);

    // Checks if column-major layout is enabled
    bool isColMajor() const;

    // Sets column-major layout
    void setColMajor(bool colMajor);

    // Gets the raw ImPlotHistogramFlags
    int histogramFlags() const;

    // Sets the raw ImPlotHistogramFlags
    void setHistogramFlags(int flags);

Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when X bin count changes
     * @param[in] bins New X bin count or method
     * @details Triggered by setXBins() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief X箱数更改时触发
     * @param[in] bins 新X箱数或方法
     * @details 当值实际更改时由setXBins()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void xBinsChanged(int bins);

    /**
     * \if ENGLISH
     * @brief Emitted when Y bin count changes
     * @param[in] bins New Y bin count or method
     * @details Triggered by setYBins() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief Y箱数更改时触发
     * @param[in] bins 新Y箱数或方法
     * @details 当值实际更改时由setYBins()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void yBinsChanged(int bins);

    /**
     * \if ENGLISH
     * @brief Emitted when X or Y range changes
     * @details Triggered by setXRangeMin(), setXRangeMax(), setYRangeMin(), setYRangeMax() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief X或Y范围更改时触发
     * @details 当值实际更改时由setXRangeMin()、setXRangeMax()、setYRangeMin()、setYRangeMax()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void xRangeChanged();
    void yRangeChanged();

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
     * @brief Emitted when column-major layout flag changes
     * @param[in] colMajor New column-major state
     * @details Triggered by setColMajor() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 列主序布局标志更改时触发
     * @param[in] colMajor 新列主序状态
     * @details 当值实际更改时由setColMajor()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void colMajorChanged(bool colMajor);

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
     * @details Triggered by any flag property change (density, noOutliers, colMajor).
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 直方图标志更改时触发
     * @details 任何标志属性更改（密度、排除异常值、列主序）时触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void histogramFlagChanged();

protected:
    // Begins drawing the 2D histogram
    virtual bool beginDraw() override;
};

// Template function implementation
template< typename ContainerX, typename ContainerY >
inline QImAbstractXYDataSeries* QImPlotHistogram2DItemNode::setData(const ContainerX& xs, const ContainerY& ys)
{
    using ConcreteSeries = QImVectorXYDataSeries<ContainerX, ContainerY>;
    ConcreteSeries* d = new ConcreteSeries(xs, ys);
    setData(d);
    return d;
}

template< typename ContainerX, typename ContainerY >
inline QImAbstractXYDataSeries* QImPlotHistogram2DItemNode::setData(ContainerX&& xs, ContainerY&& ys)
{
    using ConcreteSeries = QImVectorXYDataSeries<ContainerX, ContainerY>;
    ConcreteSeries* d = new ConcreteSeries(std::move(xs), std::move(ys));
    setData(d);
    return d;
}

}  // end namespace QIM

#endif  // QIMPLOTHISTOGRAM2DITEMNODE_H