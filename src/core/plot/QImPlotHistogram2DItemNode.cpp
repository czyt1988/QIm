#include "QImPlotHistogram2DItemNode.h"
#include <memory>
#include <optional>
#include "implot.h"
#include "implot_internal.h"
#include "QImTrackedValue.hpp"
#include "QtImGuiUtils.h"
#include <QDebug>
#include "QImPlotHistogram2DDataSeries.h"

namespace QIM
{

class QImPlotHistogram2DItemNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlotHistogram2DItemNode)
public:
    PrivateData(QImPlotHistogram2DItemNode* p);

    std::unique_ptr< QImAbstractXYDataSeries > data;
    ImPlotHistogramFlags flags { ImPlotHistogramFlags_None };
    int xBins { -2 };  // ImPlotBin_Sturges
    int yBins { -2 };
    double xRangeMin { 0.0 };  // 0 = auto
    double xRangeMax { 0.0 };
    double yRangeMin { 0.0 };
    double yRangeMax { 0.0 };
    // Style tracking values (color not used for 2D histogram - uses colormap)
};

QImPlotHistogram2DItemNode::PrivateData::PrivateData(QImPlotHistogram2DItemNode* p) : q_ptr(p)
{
}

/**
 * \if ENGLISH
 * @brief Constructor for QImPlotHistogram2DItemNode
 * @param parent Parent QObject
 * \endif
 *
 * \if CHINESE
 * @brief QImPlotHistogram2DItemNode的构造函数
 * @param parent 父QObject
 * \endif
 */
QImPlotHistogram2DItemNode::QImPlotHistogram2DItemNode(QObject* parent) : QImPlotItemNode(parent), QIM_PIMPL_CONSTRUCT
{
}

/**
 * \if ENGLISH
 * @brief Destructor for QImPlotHistogram2DItemNode
 * \endif
 *
 * \if CHINESE
 * @brief QImPlotHistogram2DItemNode的析构函数
 * \endif
 */
QImPlotHistogram2DItemNode::~QImPlotHistogram2DItemNode()
{
}

/**
 * \if ENGLISH
 * @brief Set data series for the 2D histogram
 * @param series Pointer to QImAbstractXYDataSeries
 * \endif
 *
 * \if CHINESE
 * @brief 设置二维直方图的数据系列
 * @param series QImAbstractXYDataSeries指针
 * \endif
 */
void QImPlotHistogram2DItemNode::setData(QImAbstractXYDataSeries* series)
{
    QIM_D(d);
    d->data.reset(series);
    emit dataChanged();
}

/**
 * \if ENGLISH
 * @brief Get current data series
 * @return Pointer to QImAbstractXYDataSeries
 * \endif
 *
 * \if CHINESE
 * @brief 获取当前数据系列
 * @return QImAbstractXYDataSeries指针
 * \endif
 */
QImAbstractXYDataSeries* QImPlotHistogram2DItemNode::data() const
{
    return d_ptr->data.get();
}

/**
 * \if ENGLISH
 * @brief Get X bin count or automatic method
 * @return Current X bin count or ImPlotBin enumeration
 * \endif
 *
 * \if CHINESE
 * @brief 获取X箱数或自动方法
 * @return 当前X箱数或ImPlotBin枚举
 * \endif
 */
int QImPlotHistogram2DItemNode::xBins() const
{
    QIM_DC(d);
    return d->xBins;
}

/**
 * \if ENGLISH
 * @brief Set X bin count or automatic method
 * @param bins New X bin count or ImPlotBin enumeration
 * \endif
 *
 * \if CHINESE
 * @brief 设置X箱数或自动方法
 * @param bins 新X箱数或ImPlotBin枚举
 * \endif
 */
void QImPlotHistogram2DItemNode::setXBins(int bins)
{
    QIM_D(d);
    if (d->xBins != bins) {
        d->xBins = bins;
        emit xBinsChanged(bins);
    }
}

/**
 * \if ENGLISH
 * @brief Get Y bin count or automatic method
 * @return Current Y bin count or ImPlotBin enumeration
 * \endif
 *
 * \if CHINESE
 * @brief 获取Y箱数或自动方法
 * @return 当前Y箱数或ImPlotBin枚举
 * \endif
 */
int QImPlotHistogram2DItemNode::yBins() const
{
    QIM_DC(d);
    return d->yBins;
}

/**
 * \if ENGLISH
 * @brief Set Y bin count or automatic method
 * @param bins New Y bin count or ImPlotBin enumeration
 * \endif
 *
 * \if CHINESE
 * @brief 设置Y箱数或自动方法
 * @param bins 新Y箱数或ImPlotBin枚举
 * \endif
 */
void QImPlotHistogram2DItemNode::setYBins(int bins)
{
    QIM_D(d);
    if (d->yBins != bins) {
        d->yBins = bins;
        emit yBinsChanged(bins);
    }
}

/**
 * \if ENGLISH
 * @brief Get X range minimum (0 = auto)
 * @return Current X range minimum
 * \endif
 *
 * \if CHINESE
 * @brief 获取X范围最小值（0表示自动）
 * @return 当前X范围最小值
 * \endif
 */
double QImPlotHistogram2DItemNode::xRangeMin() const
{
    QIM_DC(d);
    return d->xRangeMin;
}

/**
 * \if ENGLISH
 * @brief Set X range minimum (0 = auto)
 * @param min New X range minimum
 * \endif
 *
 * \if CHINESE
 * @brief 设置X范围最小值（0表示自动）
 * @param min 新X范围最小值
 * \endif
 */
void QImPlotHistogram2DItemNode::setXRangeMin(double min)
{
    QIM_D(d);
    if (d->xRangeMin != min) {
        d->xRangeMin = min;
        emit xRangeChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Get X range maximum (0 = auto)
 * @return Current X range maximum
 * \endif
 *
 * \if CHINESE
 * @brief 获取X范围最大值（0表示自动）
 * @return 当前X范围最大值
 * \endif
 */
double QImPlotHistogram2DItemNode::xRangeMax() const
{
    QIM_DC(d);
    return d->xRangeMax;
}

/**
 * \if ENGLISH
 * @brief Set X range maximum (0 = auto)
 * @param max New X range maximum
 * \endif
 *
 * \if CHINESE
 * @brief 设置X范围最大值（0表示自动）
 * @param max 新X范围最大值
 * \endif
 */
void QImPlotHistogram2DItemNode::setXRangeMax(double max)
{
    QIM_D(d);
    if (d->xRangeMax != max) {
        d->xRangeMax = max;
        emit xRangeChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Get Y range minimum (0 = auto)
 * @return Current Y range minimum
 * \endif
 *
 * \if CHINESE
 * @brief 获取Y范围最小值（0表示自动）
 * @return 当前Y范围最小值
 * \endif
 */
double QImPlotHistogram2DItemNode::yRangeMin() const
{
    QIM_DC(d);
    return d->yRangeMin;
}

/**
 * \if ENGLISH
 * @brief Set Y range minimum (0 = auto)
 * @param min New Y range minimum
 * \endif
 *
 * \if CHINESE
 * @brief 设置Y范围最小值（0表示自动）
 * @param min 新Y范围最小值
 * \endif
 */
void QImPlotHistogram2DItemNode::setYRangeMin(double min)
{
    QIM_D(d);
    if (d->yRangeMin != min) {
        d->yRangeMin = min;
        emit yRangeChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Get Y range maximum (0 = auto)
 * @return Current Y range maximum
 * \endif
 *
 * \if CHINESE
 * @brief 获取Y范围最大值（0表示自动）
 * @return 当前Y范围最大值
 * \endif
 */
double QImPlotHistogram2DItemNode::yRangeMax() const
{
    QIM_DC(d);
    return d->yRangeMax;
}

/**
 * \if ENGLISH
 * @brief Set Y range maximum (0 = auto)
 * @param max New Y range maximum
 * \endif
 *
 * \if CHINESE
 * @brief 设置Y范围最大值（0表示自动）
 * @param max 新Y范围最大值
 * \endif
 */
void QImPlotHistogram2DItemNode::setYRangeMax(double max)
{
    QIM_D(d);
    if (d->yRangeMax != max) {
        d->yRangeMax = max;
        emit yRangeChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Check if density normalization is enabled
 * @return true if density normalization is enabled
 * \endif
 *
 * \if CHINESE
 * @brief 检查是否启用密度归一化
 * @return 如果启用密度归一化则返回true
 * \endif
 */
bool QImPlotHistogram2DItemNode::isDensity() const
{
    QIM_DC(d);
    return (d->flags & ImPlotHistogramFlags_Density) != 0;
}

/**
 * \if ENGLISH
 * @brief Set density normalization
 * @param density true to enable density normalization
 * \endif
 *
 * \if CHINESE
 * @brief 设置密度归一化
 * @param density true表示启用密度归一化
 * \endif
 */
void QImPlotHistogram2DItemNode::setDensity(bool density)
{
    QIM_D(d);
    const ImPlotHistogramFlags oldFlags = d->flags;
    if (density) {
        d->flags |= ImPlotHistogramFlags_Density;
    } else {
        d->flags &= ~ImPlotHistogramFlags_Density;
    }
    if (d->flags != oldFlags) {
        emit densityChanged(density);
        emit histogramFlagChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Check if outliers are excluded
 * @return true if outliers are excluded
 * \endif
 *
 * \if CHINESE
 * @brief 检查是否排除异常值
 * @return 如果排除异常值则返回true
 * \endif
 */
bool QImPlotHistogram2DItemNode::isNoOutliers() const
{
    QIM_DC(d);
    return (d->flags & ImPlotHistogramFlags_NoOutliers) != 0;
}

/**
 * \if ENGLISH
 * @brief Set exclude outliers flag
 * @param noOutliers true to exclude outliers
 * \endif
 *
 * \if CHINESE
 * @brief 设置排除异常值标志
 * @param noOutliers true表示排除异常值
 * \endif
 */
void QImPlotHistogram2DItemNode::setNoOutliers(bool noOutliers)
{
    QIM_D(d);
    const ImPlotHistogramFlags oldFlags = d->flags;
    if (noOutliers) {
        d->flags |= ImPlotHistogramFlags_NoOutliers;
    } else {
        d->flags &= ~ImPlotHistogramFlags_NoOutliers;
    }
    if (d->flags != oldFlags) {
        emit noOutliersChanged(noOutliers);
        emit histogramFlagChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Check if column-major layout is enabled
 * @return true if column-major, false if row-major (default)
 * \endif
 *
 * \if CHINESE
 * @brief 检查是否启用列主序布局
 * @return true 表示列主序，false 表示行主序（默认）
 * \endif
 */
bool QImPlotHistogram2DItemNode::isColMajor() const
{
    QIM_DC(d);
    return (d->flags & ImPlotHistogramFlags_ColMajor) != 0;
}

/**
 * \if ENGLISH
 * @brief Set column-major layout
 * @param colMajor true for column-major, false for row-major
 * \endif
 *
 * \if CHINESE
 * @brief 设置列主序布局
 * @param colMajor true 表示列主序，false 表示行主序
 * \endif
 */
void QImPlotHistogram2DItemNode::setColMajor(bool colMajor)
{
    QIM_D(d);
    const ImPlotHistogramFlags oldFlags = d->flags;
    if (colMajor) {
        d->flags |= ImPlotHistogramFlags_ColMajor;
    } else {
        d->flags &= ~ImPlotHistogramFlags_ColMajor;
    }
    if (d->flags != oldFlags) {
        emit colMajorChanged(colMajor);
        emit histogramFlagChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Get raw ImPlotHistogramFlags
 * @return Current ImPlotHistogramFlags
 * \endif
 *
 * \if CHINESE
 * @brief 获取原始的ImPlotHistogramFlags
 * @return 当前的ImPlotHistogramFlags
 * \endif
 */
int QImPlotHistogram2DItemNode::histogramFlags() const
{
    QIM_DC(d);
    return d->flags;
}

/**
 * \if ENGLISH
 * @brief Set raw ImPlotHistogramFlags
 * @param flags New ImPlotHistogramFlags
 * \endif
 *
 * \if CHINESE
 * @brief 设置原始的ImPlotHistogramFlags
 * @param flags 新的ImPlotHistogramFlags
 * \endif
 */
void QImPlotHistogram2DItemNode::setHistogramFlags(int flags)
{
    QIM_D(d);
    if (d->flags != flags) {
        d->flags = static_cast< ImPlotHistogramFlags >(flags);
        emit histogramFlagChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Begin drawing implementation
 * @return false to prevent endDraw from being called
 * \endif
 *
 * \if CHINESE
 * @brief 开始绘制实现
 * @return false以防止调用endDraw
 * \endif
 */
bool QImPlotHistogram2DItemNode::beginDraw()
{
    QIM_D(d);
    if (!d->data || d->data->size() == 0) {
        return false;
    }

    // 2D histogram uses colormap, not single fill style.
    // No color tracking needed.

    // Prepare ImPlotRect
    ImPlotRect range;
    if (d->xRangeMin != 0.0 || d->xRangeMax != 0.0 || d->yRangeMin != 0.0 || d->yRangeMax != 0.0) {
        range.X.Min = d->xRangeMin;
        range.X.Max = d->xRangeMax;
        range.Y.Min = d->yRangeMin;
        range.Y.Max = d->yRangeMax;
    } else {
        // Auto range: ImPlotRect() default constructor sets zero ranges (auto)
        range = ImPlotRect();
    }

    // Determine flags (exclude unsupported Horizontal and Cumulative)
    ImPlotHistogramFlags flags = d->flags;
    // Ensure Horizontal and Cumulative are cleared (they are not supported by PlotHistogram2D)
    flags &= ~(ImPlotHistogramFlags_Horizontal | ImPlotHistogramFlags_Cumulative);

    // Call ImPlot API
    if (d->data->isContiguous()) {
        // Continuous memory mode: zero-copy fast path
        const double* xs = d->data->xRawData();
        const double* ys = d->data->yRawData();
        int count = d->data->size();
        if (!xs) {
            // Y-only mode (X values are indices) not suitable for 2D histogram.
            // We'll need to generate X indices? Actually PlotHistogram2D expects both X and Y arrays.
            // Since we cannot proceed, we'll fallback to extracting values.
            qWarning() << "[Histogram2DItemNode] X data missing, cannot plot 2D histogram";
            return false;
        }
        ImPlot::PlotHistogram2D(
            labelConstData(), xs, ys, count, d->xBins, d->yBins, range, flags);
    } else {
        // Non-contiguous memory mode: extract values into temporary arrays
        int count = d->data->size();
        std::vector<double> xs(count), ys(count);
        for (int i = 0; i < count; ++i) {
            xs[i] = d->data->xValue(i);
            ys[i] = d->data->yValue(i);
        }
        ImPlot::PlotHistogram2D(
            labelConstData(), xs.data(), ys.data(), count, d->xBins, d->yBins, range, flags);
    }

    // Update item status
    ImPlotContext* ct    = ImPlot::GetCurrentContext();
    if (!ct) {
        return false;
    }
    ImPlotItem* plotItem = ct->PreviousItem;
    if (!plotItem) {
        return false;
    }
    setImPlotItem(plotItem);
    if (plotItem->Show != QImAbstractNode::isVisible()) {
        QImAbstractNode::setVisible(plotItem->Show);
    }

    return false;
}

}  // namespace QIM