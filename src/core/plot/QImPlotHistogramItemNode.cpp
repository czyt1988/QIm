#include "QImPlotHistogramItemNode.h"
#include <optional>
#include "implot.h"
#include "implot_internal.h"
#include "QImTrackedValue.hpp"
#include "QtImGuiUtils.h"
#include <QDebug>

namespace QIM
{

class QImPlotHistogramItemNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlotHistogramItemNode)
public:
    PrivateData(QImPlotHistogramItemNode* p);

    std::unique_ptr< QImAbstractXYDataSeries > data;
    ImPlotHistogramFlags flags { ImPlotHistogramFlags_None };
    int bins { -2 };  // ImPlotBin_Sturges
    double barScale { 1.0 };
    double rangeMin { 0.0 };  // 0 = auto
    double rangeMax { 0.0 };  // 0 = auto
    // Style tracking values
    std::optional< QImTrackedValue< ImVec4, QIM::ImVecComparator< ImVec4 > > > color;
};

QImPlotHistogramItemNode::PrivateData::PrivateData(QImPlotHistogramItemNode* p) : q_ptr(p)
{
}

/**
 * \if ENGLISH
 * @brief Constructor for QImPlotHistogramItemNode
 * @param parent Parent QObject
 * \endif
 *
 * \if CHINESE
 * @brief QImPlotHistogramItemNode的构造函数
 * @param parent 父QObject
 * \endif
 */
QImPlotHistogramItemNode::QImPlotHistogramItemNode(QObject* parent) : QImPlotItemNode(parent), QIM_PIMPL_CONSTRUCT
{
}

/**
 * \if ENGLISH
 * @brief Destructor for QImPlotHistogramItemNode
 * \endif
 *
 * \if CHINESE
 * @brief QImPlotHistogramItemNode的析构函数
 * \endif
 */
QImPlotHistogramItemNode::~QImPlotHistogramItemNode()
{
}

/**
 * \if ENGLISH
 * @brief Set data series for the histogram
 * @param series Pointer to QImAbstractXYDataSeries
 * \endif
 *
 * \if CHINESE
 * @brief 设置直方图的数据系列
 * @param series QImAbstractXYDataSeries指针
 * \endif
 */
void QImPlotHistogramItemNode::setData(QImAbstractXYDataSeries* series)
{
    QIM_D(d);
    d->data.reset(series);
    Q_EMIT dataChanged();
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
QImAbstractXYDataSeries* QImPlotHistogramItemNode::data() const
{
    return d_ptr->data.get();
}

/**
 * \if ENGLISH
 * @brief Get bin count or automatic method
 * @return Current bin count or ImPlotBin enumeration
 * \endif
 *
 * \if CHINESE
 * @brief 获取箱数或自动方法
 * @return 当前箱数或ImPlotBin枚举
 * \endif
 */
int QImPlotHistogramItemNode::bins() const
{
    QIM_DC(d);
    return d->bins;
}

/**
 * \if ENGLISH
 * @brief Set bin count or automatic method
 * @param bins New bin count or ImPlotBin enumeration
 * \endif
 *
 * \if CHINESE
 * @brief 设置箱数或自动方法
 * @param bins 新箱数或ImPlotBin枚举
 * \endif
 */
void QImPlotHistogramItemNode::setBins(int bins)
{
    QIM_D(d);
    if (d->bins != bins) {
        d->bins = bins;
        Q_EMIT binsChanged(bins);
    }
}

/**
 * \if ENGLISH
 * @brief Get bar scale factor
 * @return Current bar scale factor
 * \endif
 *
 * \if CHINESE
 * @brief 获取柱条缩放因子
 * @return 当前柱条缩放因子
 * \endif
 */
double QImPlotHistogramItemNode::barScale() const
{
    QIM_DC(d);
    return d->barScale;
}

/**
 * \if ENGLISH
 * @brief Set bar scale factor
 * @param scale New bar scale factor
 * \endif
 *
 * \if CHINESE
 * @brief 设置柱条缩放因子
 * @param scale 新柱条缩放因子
 * \endif
 */
void QImPlotHistogramItemNode::setBarScale(double scale)
{
    QIM_D(d);
    if (d->barScale != scale) {
        d->barScale = scale;
        Q_EMIT barScaleChanged(scale);
    }
}

/**
 * \if ENGLISH
 * @brief Get range minimum (0 = auto)
 * @return Current range minimum
 * \endif
 *
 * \if CHINESE
 * @brief 获取范围最小值（0表示自动）
 * @return 当前范围最小值
 * \endif
 */
double QImPlotHistogramItemNode::rangeMin() const
{
    QIM_DC(d);
    return d->rangeMin;
}

/**
 * \if ENGLISH
 * @brief Set range minimum (0 = auto)
 * @param min New range minimum
 * \endif
 *
 * \if CHINESE
 * @brief 设置范围最小值（0表示自动）
 * @param min 新范围最小值
 * \endif
 */
void QImPlotHistogramItemNode::setRangeMin(double min)
{
    QIM_D(d);
    if (d->rangeMin != min) {
        d->rangeMin = min;
        Q_EMIT rangeChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Get range maximum (0 = auto)
 * @return Current range maximum
 * \endif
 *
 * \if CHINESE
 * @brief 获取范围最大值（0表示自动）
 * @return 当前范围最大值
 * \endif
 */
double QImPlotHistogramItemNode::rangeMax() const
{
    QIM_DC(d);
    return d->rangeMax;
}

/**
 * \if ENGLISH
 * @brief Set range maximum (0 = auto)
 * @param max New range maximum
 * \endif
 *
 * \if CHINESE
 * @brief 设置范围最大值（0表示自动）
 * @param max 新范围最大值
 * \endif
 */
void QImPlotHistogramItemNode::setRangeMax(double max)
{
    QIM_D(d);
    if (d->rangeMax != max) {
        d->rangeMax = max;
        Q_EMIT rangeChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Check if cumulative mode is enabled
 * @return true if cumulative mode is enabled
 * \endif
 *
 * \if CHINESE
 * @brief 检查是否启用累积模式
 * @return 如果启用累积模式则返回true
 * \endif
 */
bool QImPlotHistogramItemNode::isCumulative() const
{
    QIM_DC(d);
    return (d->flags & ImPlotHistogramFlags_Cumulative) != 0;
}

/**
 * \if ENGLISH
 * @brief Set cumulative mode
 * @param cumulative true to enable cumulative mode
 * \endif
 *
 * \if CHINESE
 * @brief 设置累积模式
 * @param cumulative true表示启用累积模式
 * \endif
 */
void QImPlotHistogramItemNode::setCumulative(bool cumulative)
{
    QIM_D(d);
    const ImPlotHistogramFlags oldFlags = d->flags;
    if (cumulative) {
        d->flags |= ImPlotHistogramFlags_Cumulative;
    } else {
        d->flags &= ~ImPlotHistogramFlags_Cumulative;
    }
    if (d->flags != oldFlags) {
        Q_EMIT cumulativeChanged(cumulative);
        Q_EMIT histogramFlagChanged();
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
bool QImPlotHistogramItemNode::isDensity() const
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
void QImPlotHistogramItemNode::setDensity(bool density)
{
    QIM_D(d);
    const ImPlotHistogramFlags oldFlags = d->flags;
    if (density) {
        d->flags |= ImPlotHistogramFlags_Density;
    } else {
        d->flags &= ~ImPlotHistogramFlags_Density;
    }
    if (d->flags != oldFlags) {
        Q_EMIT densityChanged(density);
        Q_EMIT histogramFlagChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Check if horizontal orientation is enabled
 * @return true if horizontal mode is enabled
 * \endif
 *
 * \if CHINESE
 * @brief 检查是否启用水平方向
 * @return 如果启用水平模式则返回true
 * \endif
 */
bool QImPlotHistogramItemNode::isHorizontal() const
{
    QIM_DC(d);
    return (d->flags & ImPlotHistogramFlags_Horizontal) != 0;
}

/**
 * \if ENGLISH
 * @brief Set horizontal orientation
 * @param horizontal true to enable horizontal mode
 * \endif
 *
 * \if CHINESE
 * @brief 设置水平方向
 * @param horizontal true表示启用水平模式
 * \endif
 */
void QImPlotHistogramItemNode::setHorizontal(bool horizontal)
{
    QIM_D(d);
    const ImPlotHistogramFlags oldFlags = d->flags;
    if (horizontal) {
        d->flags |= ImPlotHistogramFlags_Horizontal;
    } else {
        d->flags &= ~ImPlotHistogramFlags_Horizontal;
    }
    if (d->flags != oldFlags) {
        Q_EMIT orientationChanged(horizontal);
        Q_EMIT histogramFlagChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Check if outliers are included in histogram calculations
 * @return true if outliers are included (ImPlotHistogramFlags_NoOutliers is NOT set)
 * @details Default is true. When false, values outside range are excluded.
 * @see setOutliersIncluded()
 * \endif
 *
 * \if CHINESE
 * @brief 检查异常值是否包含在直方图计算中
 * @return true表示异常值包含（ImPlotHistogramFlags_NoOutliers未设置）
 * @details 默认为true。为false时，范围外的值被排除。
 * @see setOutliersIncluded()
 * \endif
 */
bool QImPlotHistogramItemNode::isOutliersIncluded() const
{
    QIM_DC(d);
    return (d->flags & ImPlotHistogramFlags_NoOutliers) == 0;
}

/**
 * \if ENGLISH
 * @brief Set whether outliers are included in histogram calculations
 * @param[in] included true to include outliers, false to exclude (sets NoOutliers)
 * @see isOutliersIncluded()
 * \endif
 *
 * \if CHINESE
 * @brief 设置异常值是否包含在直方图计算中
 * @param[in] included true包含异常值，false排除（设置NoOutliers）
 * @see isOutliersIncluded()
 * \endif
 */
void QImPlotHistogramItemNode::setOutliersIncluded(bool included)
{
    QIM_D(d);
    const ImPlotHistogramFlags oldFlags = d->flags;
    if (included) {
        d->flags &= ~ImPlotHistogramFlags_NoOutliers;
    } else {
        d->flags |= ImPlotHistogramFlags_NoOutliers;
    }
    if (d->flags != oldFlags) {
        Q_EMIT outliersIncludedChanged(included);
        Q_EMIT histogramFlagChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Check if column-major data order is enabled
 * @return true if column-major order is enabled (ImPlotHistogramFlags_ColMajor is set)
 * @details Default is false. Row-major is the default data layout.
 * @see setColMajor()
 * \endif
 *
 * \if CHINESE
 * @brief 检查列主序数据是否启用
 * @return true表示列主序启用（ImPlotHistogramFlags_ColMajor已设置）
 * @details 默认为false。行主序是默认的数据布局。
 * @see setColMajor()
 * \endif
 */
bool QImPlotHistogramItemNode::isColMajor() const
{
    QIM_DC(d);
    return (d->flags & ImPlotHistogramFlags_ColMajor) != 0;
}

/**
 * \if ENGLISH
 * @brief Set column-major data order
 * @param[in] on true to enable column-major, false for row-major (default)
 * @see isColMajor()
 * \endif
 *
 * \if CHINESE
 * @brief 设置列主序数据顺序
 * @param[in] on true启用列主序，false为行主序（默认）
 * @see isColMajor()
 * \endif
 */
void QImPlotHistogramItemNode::setColMajor(bool on)
{
    QIM_D(d);
    const ImPlotHistogramFlags oldFlags = d->flags;
    if (on) {
        d->flags |= ImPlotHistogramFlags_ColMajor;
    } else {
        d->flags &= ~ImPlotHistogramFlags_ColMajor;
    }
    if (d->flags != oldFlags) {
        Q_EMIT histogramFlagChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Get histogram bar color
 * @return Current QColor
 * \endif
 *
 * \if CHINESE
 * @brief 获取直方图柱条颜色
 * @return 当前的QColor
 * \endif
 */
QColor QImPlotHistogramItemNode::color() const
{
    return (d_ptr->color.has_value()) ? toQColor(d_ptr->color->value()) : QColor();
}

/**
 * \if ENGLISH
 * @brief Set histogram bar color
 * @param c QColor for the histogram bars
 * \endif
 *
 * \if CHINESE
 * @brief 设置直方图柱条颜色
 * @param c 直方图柱条的QColor
 * \endif
 */
void QImPlotHistogramItemNode::setColor(const QColor& c)
{
    qDebug() << "[HistogramItemNode] setColor called with:" << c;
    
    ImVec4 imColor = toImVec4(c);
    
    if (d_ptr->color.has_value()) {
        qDebug() << "[HistogramItemNode] color optional has value, current dirty=" << d_ptr->color->is_dirty();
        qDebug() << "[HistogramItemNode] current color value:" << toQColor(d_ptr->color->value());
        d_ptr->color->operator=(imColor);  // Explicitly call assignment to trigger dirty
        qDebug() << "[HistogramItemNode] after assignment, dirty=" << d_ptr->color->is_dirty();
    } else {
        qDebug() << "[HistogramItemNode] color optional has NO value, creating new QImTrackedValue";
        // Create with value and then mark dirty
        d_ptr->color.emplace(imColor);
        d_ptr->color->mark_dirty();  // Force dirty because this is a new color being set
        qDebug() << "[HistogramItemNode] after emplace+mark_dirty, dirty=" << d_ptr->color->is_dirty();
    }
    
    Q_EMIT colorChanged(c);
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
int QImPlotHistogramItemNode::histogramFlags() const
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
void QImPlotHistogramItemNode::setHistogramFlags(int flags)
{
    QIM_D(d);
    if (d->flags != flags) {
        d->flags = static_cast< ImPlotHistogramFlags >(flags);
        Q_EMIT histogramFlagChanged();
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
bool QImPlotHistogramItemNode::beginDraw()
{
    QIM_D(d);
    if (!d->data || d->data->size() == 0) {
        return false;
    }

    // Debug: Check color state before applying style
    qDebug() << "[HistogramItemNode] beginDraw - color has_value:" << d->color.has_value();
    if (d->color.has_value()) {
        qDebug() << "[HistogramItemNode] beginDraw - color is_dirty:" << d->color->is_dirty();
        qDebug() << "[HistogramItemNode] beginDraw - color value:" << toQColor(d->color->value());
    }

    // Apply style - use SetNextFillStyle for histogram bars
    if (d->color.has_value() && d->color->is_dirty()) {
        qDebug() << "[HistogramItemNode] beginDraw - calling SetNextFillStyle with color:" << toQColor(d->color->value());
        ImPlot::SetNextFillStyle(d->color->value());
        d->color->clear();  // Clear dirty flag after applying
    } else {
        qDebug() << "[HistogramItemNode] beginDraw - NOT calling SetNextFillStyle (color not dirty or no value)";
    }

    // Prepare ImPlotRange
    ImPlotRange range;
    if (d->rangeMin != 0.0 || d->rangeMax != 0.0) {
        range.Min = d->rangeMin;
        range.Max = d->rangeMax;
    } else {
        // Auto range: use data min/max
        // Since histogram uses only Y values, we need to compute min/max of Y data
        // For simplicity, let ImPlot compute automatically by passing ImPlotRange()
        range = ImPlotRange();
    }

    // Call ImPlot API
    if (d->data->isContiguous()) {
        // Continuous memory mode: use zero-copy fast path
        const double* values = d->data->yRawData();
        int count = d->data->size();
        ImPlot::PlotHistogram(
            labelConstData(), values, count, d->bins, d->barScale, range, d->flags);
    } else {
        // Non-contiguous memory mode: use callback (ImPlot doesn't provide PlotHistogramG, but we can use PlotHistogram with getter?)
        // Actually ImPlot::PlotHistogram has no getter version. We'll need to extract values into temporary array.
        // For simplicity, we'll extract values (might be inefficient for large data).
        // This is a limitation; we could implement a custom getter using ImPlot::PlotHistogramG? Not exist.
        // We'll just extract.
        int count = d->data->size();
        std::vector<double> values(count);
        for (int i = 0; i < count; ++i) {
            values[i] = d->data->yValue(i);
        }
        ImPlot::PlotHistogram(
            labelConstData(), values.data(), count, d->bins, d->barScale, range, d->flags);
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
    if (!d->color) {
        // First render without explicit color, get default color from ImPlot
        d->color = ImPlot::GetLastItemColor();
    }

    return false;
}

}  // namespace QIM
