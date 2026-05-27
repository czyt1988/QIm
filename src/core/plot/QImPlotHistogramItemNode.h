#ifndef QIMPLOTHISTOGRAMITEMNODE_H
#define QIMPLOTHISTOGRAMITEMNODE_H

#include <QColor>
#include <type_traits>
#include "QImAPI.h"
#include "QImAbstractXYSeriesItemNode.h"
#include "QImPlotHistogramDataSeries.h"

namespace QIM
{
class QImAbstractHistogramDataSeries;

/**
 * \if ENGLISH
 * @brief Qt-style wrapper for ImPlot histogram visualization
 *
 * @details Provides Qt-style retained mode encapsulation for ImPlot histograms.
 *          Supports customizable binning, bar scaling, range filtering,
 *          and full Qt property system integration with signal-slot event handling.
 *          Inherits from QImAbstractXYSeriesItemNode for data management.
 *
 * @note Histograms visualize distribution of single-value data by binning values into bars.
 *       Use QImPlotBarsItemNode for categorical or discrete XY bar charts.
 * @note Bin count changes require redraw to take effect.
 *
 * @see QImAbstractXYSeriesItemNode
 * @see QImPlotBarsItemNode
 * @see QImPlotNode
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot直方图可视化的Qt风格封装。
 *
 * @details 为ImPlot直方图提供Qt风格的保留模式封装。
 *          支持可自定义的装箱、柱条缩放、范围过滤，
 *          以及完整的Qt属性系统集成和信号槽事件处理。
 *          继承自QImAbstractXYSeriesItemNode以获得数据管理。
 *
 * @note 直方图通过将单值数据装箱到柱条中来可视化数据分布。
 *       分类或离散XY柱状图请使用QImPlotBarsItemNode。
 * @note 箱数更改需要重绘才能生效。
 *
 * @see QImAbstractXYSeriesItemNode
 * @see QImPlotBarsItemNode
 * @see QImPlotNode
 * \endif
 */
class QIM_CORE_API QImPlotHistogramItemNode : public QImAbstractXYSeriesItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotHistogramItemNode)

    
    Q_PROPERTY(int bins READ bins WRITE setBins NOTIFY binsChanged)
    Q_PROPERTY(double barScale READ barScale WRITE setBarScale NOTIFY barScaleChanged)
    Q_PROPERTY(double rangeMin READ rangeMin WRITE setRangeMin NOTIFY rangeChanged)
    Q_PROPERTY(double rangeMax READ rangeMax WRITE setRangeMax NOTIFY rangeChanged)
    Q_PROPERTY(bool cumulative READ isCumulative WRITE setCumulative NOTIFY cumulativeChanged)
    Q_PROPERTY(bool density READ isDensity WRITE setDensity NOTIFY densityChanged)
    Q_PROPERTY(bool horizontal READ isHorizontal WRITE setHorizontal NOTIFY orientationChanged)
    Q_PROPERTY(bool outliersIncluded READ isOutliersIncluded WRITE setOutliersIncluded NOTIFY outliersIncludedChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(bool colMajor READ isColMajor WRITE setColMajor NOTIFY histogramFlagChanged)

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
    // Data setting interface - Y-only mode (Histogram-specific)
    //----------------------------------------------------

    using QImAbstractXYSeriesItemNode::setData;  // Bring base setData(pointer) into scope

    // Sets histogram data from single value container (Y-only mode)
    template< typename ContainerY >
    std::enable_if_t< !std::is_pointer< ContainerY >::value, QImAbstractHistogramDataSeries* >
    setData(const ContainerY& y);

    // Sets histogram data from single value container (move semantics)
    template< typename ContainerY >
    std::enable_if_t< !std::is_pointer< typename std::remove_reference< ContainerY >::type >::value, QImAbstractHistogramDataSeries* >
    setData(ContainerY&& y);

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

    // Checks if outliers are included (inverse of ImPlotHistogramFlags_NoOutliers)
    bool isOutliersIncluded() const;

    // Sets whether outliers are included
    void setOutliersIncluded(bool included);

    // Checks if column-major data order is enabled
    bool isColMajor() const;

    // Sets column-major data order
    void setColMajor(bool on);

    // Gets the histogram bar color
    QColor color() const;

    // Sets the histogram bar color
    void setColor(const QColor& color);

    // Gets the raw ImPlotHistogramFlags
    int histogramFlags() const;

    // Sets the raw ImPlotHistogramFlags
    void setHistogramFlags(int flags);

Q_SIGNALS:
    void binsChanged(int bins);
    void barScaleChanged(double scale);
    void rangeChanged();
    void cumulativeChanged(bool cumulative);
    void densityChanged(bool density);
    void orientationChanged(bool horizontal);
    void outliersIncludedChanged(bool included);
    void colorChanged(const QColor& color);
    void histogramFlagChanged();

protected:
    // Begins drawing the histogram
    virtual bool beginDraw() override;
};

// Template function implementation - Y-only mode (Histogram-specific)
template< typename ContainerY >
inline std::enable_if_t< !std::is_pointer< ContainerY >::value, QImAbstractHistogramDataSeries* >
QImPlotHistogramItemNode::setData(const ContainerY& y)
{
    using ConcreteSeries = QImVectorHistogramDataSeries<ContainerY>;
    ConcreteSeries* d = new ConcreteSeries(y);
    QImAbstractXYSeriesItemNode::setData(d);
    return d;
}

template< typename ContainerY >
inline std::enable_if_t< !std::is_pointer< typename std::remove_reference< ContainerY >::type >::value, QImAbstractHistogramDataSeries* >
QImPlotHistogramItemNode::setData(ContainerY&& y)
{
    using ConcreteSeries = QImVectorHistogramDataSeries<typename std::remove_reference< ContainerY >::type>;
    ConcreteSeries* d = new ConcreteSeries(std::move(y));
    QImAbstractXYSeriesItemNode::setData(d);
    return d;
}

}  // end namespace QIM

#endif  // QIMPLOTHISTOGRAMITEMNODE_H