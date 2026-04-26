#include "QImPlotScatterItemNode.h"
#include "QImPlotDataSeries.h"
#include "QImDownsamplingController.h"
#include "implot.h"
#include "implot_internal.h"
#include <QDebug>
#include <cmath>

namespace QIM
{
// ImPlotMarker_None   ->   无标记
// ImPlotMarker_Circle   ->   ● 圆形
// ImPlotMarker_Square   ->   ■ 正方形
// ImPlotMarker_Diamond   ->   ◆ 菱形
// ImPlotMarker_Up   ->   ▲ 上三角
// ImPlotMarker_Down   ->   ▼ 下三角
// ImPlotMarker_Left   ->   ◀ 左三角
// ImPlotMarker_Right   ->   ▶ 右三角
// ImPlotMarker_Cross   ->   ✕ 叉形
// ImPlotMarker_Plus   ->   ＋ 加号
// ImPlotMarker_Asterisk   ->   ✻ 星形

class QImPlotScatterItemNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlotScatterItemNode)
public:
    PrivateData(QImPlotScatterItemNode* p);
    std::unique_ptr< QImAbstractXYDataSeries > data;
    QImDownsamplingController m_sampling;
    QImTrackedValue< int > markerShape { ImPlotMarker_Circle };  ///< 标记形状
    bool markerFill { true };
    QImOptionalColor color;                                       ///< 颜色（延迟初始化：首次渲染时捕获ImPlot默认颜色）
    QImTrackedValue< float > markerSize { 4.0f };                 ///< 标记大小
    ImPlotScatterFlags scatterFlags { ImPlotScatterFlags_None };  ///< 散点图标志位
};

QImPlotScatterItemNode::PrivateData::PrivateData(QImPlotScatterItemNode* p) : q_ptr(p)
{
}

//----------------------------------------------------
// QImPlotScatterItemNode
//----------------------------------------------------
/**
 * \if ENGLISH
 * @brief Constructs a QImPlotScatterItemNode with optional parent
 * @param[in] par Parent QObject (typically a QImPlotNode)
 * \endif
 *
 * \if CHINESE
 * @brief 构造 QImPlotScatterItemNode，可选指定父对象
 * @param[in] par 父 QObject（通常为 QImPlotNode）
 * \endif
 */
QImPlotScatterItemNode::QImPlotScatterItemNode(QObject* par) : QImPlotItemNode(par), QIM_PIMPL_CONSTRUCT
{
}

/**
 * \if ENGLISH
 * @brief Destructor for QImPlotScatterItemNode
 * \endif
 *
 * \if CHINESE
 * @brief QImPlotScatterItemNode 的析构函数
 * \endif
 */
QImPlotScatterItemNode::~QImPlotScatterItemNode()
{
}

/**
 * \if ENGLISH
 * @brief Sets the data series for the scatter plot
 * @param[in] series Pointer to QImAbstractXYDataSeries (ownership transferred)
 * @details Stores the series, triggers adaptive sampling if enabled, and emits dataChanged() signal.
 * \endif
 *
 * \if CHINESE
 * @brief 设置散点图的数据系列
 * @param[in] series QImAbstractXYDataSeries 指针（所有权转移）
 * @details 存储数据系列，如果启用自适应采样则触发降采样，并触发 dataChanged() 信号。
 * \endif
 */
void QImPlotScatterItemNode::setData(QImAbstractXYDataSeries* series)
{
    QIM_D(d);
    d->data.reset(series);
    d->m_sampling.setSource(series);
    d->m_sampling.invalidate();
    Q_EMIT dataChanged();
}

/**
 * \if ENGLISH
 * @brief Gets the current data series
 * @return Pointer to QImAbstractXYDataSeries, nullptr if no data set
 * \endif
 *
 * \if CHINESE
 * @brief 获取当前数据系列
 * @return QImAbstractXYDataSeries 指针，无数据时返回 nullptr
 * \endif
 */
QImAbstractXYDataSeries* QImPlotScatterItemNode::data() const
{
    QIM_DC(d);
    return d->data.get();
}

/**
 * \if ENGLISH
 * @brief Gets the marker size in pixels
 * @return Current marker size in pixels
 * @details Returns the uniform size applied to all scatter markers.
 *          Default is 4.0 pixels.
 * @see setMarkerSize()
 * \endif
 *
 * \if CHINESE
 * @brief 获取标记大小（像素）
 * @return 当前标记大小（像素）
 * @details 返回应用于所有散点标记的统一大小。
 *          默认为4.0像素。
 * @see setMarkerSize()
 * \endif
 */
float QImPlotScatterItemNode::markerSize() const
{
    QIM_DC(d);
    return d->markerSize.value();
}

/**
 * \if ENGLISH
 * @brief Sets the marker size in pixels
 * @param[in] size New marker size in pixels
 * @details Updates marker size and triggers visual update.
 *          Emits markerSizeChanged() signal if value changed.
 *          Value must be positive.
 * @note Size change requires redraw to be visible.
 * @see markerSize()
 * \endif
 *
 * \if CHINESE
 * @brief 设置标记大小（像素）
 * @param[in] size 新标记大小（像素）
 * @details 更新标记大小并触发视觉更新。
 *          如果值更改，触发markerSizeChanged()信号。
 *          值必须为正数。
 * @note 大小更改需要重绘才能可见。
 * @see markerSize()
 * \endif
 */
void QImPlotScatterItemNode::setMarkerSize(float size)
{
    QIM_D(d);
    d->markerSize = size;
    if (d->markerSize.is_dirty()) {
        Q_EMIT markerSizeChanged(size);
    }
}

/**
 * \if ENGLISH
 * @brief Gets the marker shape
 * @return Current marker shape as ImPlotMarker enum value
 * @details Returns the geometric shape used for scatter markers.
 *          Default is ImPlotMarker_Circle.
 * @see setMarkerShape()
 * \endif
 *
 * \if CHINESE
 * @brief 获取标记形状
 * @return 当前标记形状（ImPlotMarker枚举值）
 * @details 返回用于散点标记的几何形状。
 *          默认为ImPlotMarker_Circle。
 * @see setMarkerShape()
 * \endif
 */
int QImPlotScatterItemNode::markerShape() const
{
    QIM_DC(d);
    return d->markerShape.value();
}

/**
 * \if ENGLISH
 * @brief Sets the marker shape
 * @param[in] shape New marker shape as ImPlotMarker enum value
 * @details Updates marker shape and triggers visual update.
 *          Emits markerShapeChanged() signal if value changed.
 *          Valid values are ImPlotMarker enumeration members.
 * @note Shape change requires redraw to be visible.
 * @see markerShape()
 * \endif
 *
 * \if CHINESE
 * @brief 设置标记形状
 * @param[in] shape 新标记形状（ImPlotMarker枚举值）
 * @details 更新标记形状并触发视觉更新。
 *          如果值更改，触发markerShapeChanged()信号。
 *          有效值为ImPlotMarker枚举成员。
 * @note 形状更改需要重绘才能可见。
 * @see markerShape()
 * \endif
 */
void QImPlotScatterItemNode::setMarkerShape(int shape)
{
    QIM_D(d);
    d->markerShape = shape;
    if (d->markerShape.is_dirty()) {
        Q_EMIT markerShapeChanged(shape);
    }
}

/**
 * \if ENGLISH
 * @brief Checks if markers are filled
 * @return true if markers are filled, false if outlined only
 * @details Returns current fill state of scatter markers.
 *          Default is true (filled markers).
 * @see setMarkerFill()
 * \endif
 *
 * \if CHINESE
 * @brief 检查标记是否填充
 * @return true表示标记填充，false表示仅轮廓
 * @details 返回散点标记的当前填充状态。
 *          默认为true（填充标记）。
 * @see setMarkerFill()
 * \endif
 */
bool QImPlotScatterItemNode::isMarkerFill() const
{
    QIM_DC(d);
    return d->markerFill;
}

/**
 * \if ENGLISH
 * @brief Sets marker fill state
 * @param[in] fill true to fill markers, false for outline only
 * @details Updates marker fill state and triggers visual update.
 *          Emits markerFillChanged() signal if value changed.
 * @note Fill change requires redraw to be visible.
 * @see isMarkerFill()
 * \endif
 *
 * \if CHINESE
 * @brief 设置标记填充状态
 * @param[in] fill true填充标记，false仅轮廓
 * @details 更新标记填充状态并触发视觉更新。
 *          如果值更改，触发markerFillChanged()信号。
 * @note 填充更改需要重绘才能可见。
 * @see isMarkerFill()
 * \endif
 */
void QImPlotScatterItemNode::setMarkerFill(bool fill)
{
    QIM_D(d);
    if (d->markerFill != fill) {
        d->markerFill = fill;
        Q_EMIT markerFillChanged(fill);
    }
}

/**
 * \if ENGLISH
 * @brief Gets the downsampling algorithm
 * @return Current downsampling algorithm
 * @details Returns the current downsampling algorithm for this scatter plot.
 *          Default is QImDownsampleAlgorithm::Auto.
 * \endif
 *
 * \if CHINESE
 * @brief 获取降采样算法
 * @return 当前降采样算法
 * @details 返回此散点图的当前降采样算法。
 *          默认为 QImDownsampleAlgorithm::Auto。
 * \endif
 */
QImDownsampleAlgorithm QImPlotScatterItemNode::downsampleAlgorithm() const
{
    QIM_DC(d);
    return d->m_sampling.algorithm();
}

/**
 * \if ENGLISH
 * @brief Sets the downsampling algorithm
 * @param[in] algo The new downsampling algorithm
 * @details Updates the downsampling algorithm and triggers re-downsampling.
 *          Emits downsampleAlgorithmChanged() signal if value changed.
 * \endif
 *
 * \if CHINESE
 * @brief 设置降采样算法
 * @param[in] algo 新的降采样算法
 * @details 更新降采样算法并触发重新降采样。
 *          如果值变更，触发 downsampleAlgorithmChanged() 信号。
 * \endif
 */
void QImPlotScatterItemNode::setDownsampleAlgorithm(QImDownsampleAlgorithm algo)
{
    QIM_D(d);
    QImDownsampleAlgorithm old = d->m_sampling.algorithm();
    d->m_sampling.setAlgorithm(algo);
    if (d->m_sampling.algorithm() != old) {
        Q_EMIT downsampleAlgorithmChanged(d->m_sampling.algorithm());
    }
}

/**
 * \if ENGLISH
 * @brief Gets the downsample threshold
 * @return Current downsample threshold in data points
 * @details Returns the dataset size threshold for triggering downsampling.
 *          Default is 20000 points.
 * \endif
 *
 * \if CHINESE
 * @brief 获取降采样阈值
 * @return 当前降采样阈值（数据点）
 * @details 返回触发降采样的数据集大小阈值。
 *          默认为 20000 点。
 * \endif
 */
int QImPlotScatterItemNode::downsampleThreshold() const
{
    QIM_DC(d);
    return d->m_sampling.threshold();
}

/**
 * \if ENGLISH
 * @brief Sets the downsample threshold
 * @param[in] threshold New downsample threshold in data points (min: 100)
 * @details Updates downsample threshold and triggers re-downsampling if needed.
 *          Emits downsampleThresholdChanged() signal if value changed.
 * \endif
 *
 * \if CHINESE
 * @brief 设置降采样阈值
 * @param[in] threshold 新降采样阈值（数据点，最小 100）
 * @details 更新降采样阈值，如果需要则触发重新降采样。
 *          如果值变更，触发 downsampleThresholdChanged() 信号。
 * \endif
 */
void QImPlotScatterItemNode::setDownsampleThreshold(int threshold)
{
    QIM_D(d);
    int old = d->m_sampling.threshold();
    d->m_sampling.setThreshold(threshold);
    if (d->m_sampling.threshold() != old) {
        Q_EMIT downsampleThresholdChanged(d->m_sampling.threshold());
    }
}

/**
 * \if ENGLISH
 * @brief Gets the marker color
 * @return Current marker color as QColor
 * @details Returns the primary color for scatter markers.
 *          Returns default QColor() if no color is explicitly set.
 * @see setColor()
 * \endif
 *
 * \if CHINESE
 * @brief 获取标记颜色
 * @return 当前标记颜色（QColor）
 * @details 返回散点标记的主要颜色。
 *          如果未显式设置颜色，返回默认QColor()。
 * @see setColor()
 * \endif
 */
QColor QImPlotScatterItemNode::color() const
{
    QIM_DC(d);
    return (d->color.has_value()) ? toQColor(d->color->value()) : QColor();
}

/**
 * \if ENGLISH
 * @brief Sets the marker color
 * @param[in] color New marker color
 * @details Updates marker color and triggers visual update.
 *          Emits colorChanged() signal if value changed.
 *          Affects both fill and outline colors based on markerFill setting.
 * @note Color change requires redraw to be visible.
 * @see color()
 * \endif
 *
 * \if CHINESE
 * @brief 设置标记颜色
 * @param[in] color 新标记颜色
 * @details 更新标记颜色并触发视觉更新。
 *          如果值更改，触发colorChanged()信号。
 *          根据markerFill设置影响填充和轮廓颜色。
 * @note 颜色更改需要重绘才能可见。
 * @see color()
 * \endif
 */
void QImPlotScatterItemNode::setColor(const QColor& c)
{
    QIM_D(d);
    d->color = toImVec4(c);
    Q_EMIT colorChanged(c);
}

/**
 * \if ENGLISH
 * @brief Checks if clipping is enabled
 * @return true if clipping is enabled (ImPlotScatterFlags_NoClip is NOT set)
 * @details Returns the clipping state. When true, markers at plot edges are clipped.
 *          When false, markers may extend beyond the plot area.
 *          Default is true (clipping enabled).
 * @see setClippingEnabled()
 * \endif
 *
 * \if CHINESE
 * @brief 检查裁剪是否启用
 * @return true表示裁剪启用（ImPlotScatterFlags_NoClip未设置）
 * @details 返回裁剪状态。为true时，绘图边缘标记将被裁剪。
 *          为false时，标记可能超出绘图区域。
 *          默认为true（启用裁剪）。
 * @see setClippingEnabled()
 * \endif
 */
bool QImPlotScatterItemNode::isClippingEnabled() const
{
    QIM_DC(d);
    return (d->scatterFlags & ImPlotScatterFlags_NoClip) == 0;
}

/**
 * \if ENGLISH
 * @brief Sets clipping enabled state
 * @param[in] enabled true to enable clipping, false to disable (sets NoClip)
 * @details When enabled, markers at plot edges are clipped.
 *          When disabled (NoClip), markers may extend beyond the plot area.
 *          Emits scatterFlagChanged() signal if value changed.
 * @see isClippingEnabled()
 * \endif
 *
 * \if CHINESE
 * @brief 设置裁剪启用状态
 * @param[in] enabled true启用裁剪，false禁用（设置NoClip）
 * @details 启用时，绘图边缘标记将被裁剪。
 *          禁用时（NoClip），标记可能超出绘图区域。
 *          如果值更改，触发scatterFlagChanged()信号。
 * @see isClippingEnabled()
 * \endif
 */
void QImPlotScatterItemNode::setClippingEnabled(bool enabled)
{
    QIM_D(d);
    const ImPlotScatterFlags oldFlags = d->scatterFlags;
    if (enabled) {
        d->scatterFlags &= ~ImPlotScatterFlags_NoClip;
    } else {
        d->scatterFlags |= ImPlotScatterFlags_NoClip;
    }
    if (d->scatterFlags != oldFlags) {
        Q_EMIT scatterFlagChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Gets the raw ImPlotScatterFlags
 * @return Current ImPlotScatterFlags value
 * @details Returns the raw ImPlot scatter flags for advanced usage.
 * @see setScatterFlags()
 * \endif
 *
 * \if CHINESE
 * @brief 获取原始ImPlotScatterFlags
 * @return 当前的ImPlotScatterFlags值
 * @details 返回原始ImPlot散点图标志，供高级用户使用。
 * @see setScatterFlags()
 * \endif
 */
int QImPlotScatterItemNode::scatterFlags() const
{
    QIM_DC(d);
    return d->scatterFlags;
}

/**
 * \if ENGLISH
 * @brief Sets the raw ImPlotScatterFlags
 * @param[in] flags New ImPlotScatterFlags value
 * @details Sets the raw ImPlot scatter flags for advanced usage.
 *          Emits scatterFlagChanged() signal if value changed.
 * @see scatterFlags()
 * \endif
 *
 * \if CHINESE
 * @brief 设置原始ImPlotScatterFlags
 * @param[in] flags 新的ImPlotScatterFlags值
 * @details 设置原始ImPlot散点图标志，供高级用户使用。
 *          如果值更改，触发scatterFlagChanged()信号。
 * @see scatterFlags()
 * \endif
 */
void QImPlotScatterItemNode::setScatterFlags(int flags)
{
    QIM_D(d);
    if (d->scatterFlags != static_cast< ImPlotScatterFlags >(flags)) {
        d->scatterFlags = static_cast< ImPlotScatterFlags >(flags);
        Q_EMIT scatterFlagChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Renders the scatter plot with pre-converted data
 * @return false (no endDraw needed)
 * @details Calls ImPlot::PlotScatter with XY data. All conversions done in setters.
 *          Uses adaptive sampling (LTTB) when enabled for large datasets.
 * \endif
 *
 * \if CHINESE
 * @brief 使用预转换数据渲染散点图
 * @return false（无需调用endDraw）
 * @details 调用 ImPlot::PlotScatter 处理 XY 数据。所有转换在 setter 中完成。
 *          启用自适应采样时对大数据集使用 LTTB 算法。
 * \endif
 */
bool QImPlotScatterItemNode::beginDraw()
{
    QIM_D(d);
    if (!d->data) {
        // 没有数据
        return false;
    }
    QImAbstractXYDataSeries* series = d->data.get();

    // Adaptive downsampling: resolve data series via controller
    if (d->m_sampling.algorithm() != QImDownsampleAlgorithm::None && d->data && d->data->size() > d->m_sampling.threshold()) {
        ImPlotRect limits = ImPlot::GetPlotLimits(ImAxis_X1, ImAxis_Y1);
        int pixelWidth    = static_cast<int>(ImPlot::GetPlotSize().x);
        series = d->m_sampling.resolve(pixelWidth, limits.X.Min, limits.X.Max);
    } else {
        series = d->data.get();
    }
    if (!series) {
        return false;
    }

    // 准备标记样式
    ImPlotMarker marker = static_cast< ImPlotMarker >(d->markerShape.value());
    float size          = d->markerSize.value();
    ImVec4 col          = d->color.has_value() ? d->color->value() : ImVec4(0, 0, 0, -1);

    // 每次渲染都应用样式 (SetNextMarkerStyle 只影响下一次绘制)
    // ImPlot 需要每帧重新设置样式
    ImPlot::SetNextMarkerStyle(marker, size, col, IMPLOT_AUTO, d->markerFill ? col : ImVec4(0, 0, 0, 0));

    // Clear dirty flags after applying (for signal emission tracking)
    d->markerSize.clear();
    d->markerShape.clear();
    if (d->color.has_value()) {
        d->color->clear();
    }

    if (series->isContiguous()) {
        if (series->xRawData()) {
            // 有x指针，说明不是yonly
            ImPlot::PlotScatter(labelConstData(),
                                series->xRawData(),
                                series->yRawData(),
                                series->size(),
                                d->scatterFlags,
                                series->offset(),
                                series->stride());
        } else {
            // x指针没有说明是yonly
            ImPlot::PlotScatter(labelConstData(),
                                series->yRawData(),
                                series->size(),
                                series->xScale(),
                                series->xStart(),
                                d->scatterFlags,
                                series->offset(),
                                series->stride());
        }
    } else {
        // TODO:非连续内存
    }

    // 更新item的状态
    ImPlotContext* ct = ImPlot::GetCurrentContext();
    if (!ct) {
        return false;
    }
    ImPlotItem* plotItem = ct->PreviousItem;  // 通过源码，PlotScatter结束后，ImPlotItem就是PreviousItem
    if (!plotItem) {
        return false;
    }
    setImPlotItem(plotItem);
    if (plotItem->Show != QImAbstractNode::isVisible()) {
        // 状态发生了变化，这种情况是label点击，设置了show状态和QImAbstractNode记录的状态不一致
        // 这时要同步状态
        QImAbstractNode::setVisible(plotItem->Show);  // 此函数会触发信号
    }
    if (!d->color) {
        // 一般是首次渲染，且没设定颜色，这时是implot给的默认颜色，把这个默认颜色获取到
        d->color = ImPlot::GetLastItemColor();
    }
    // 绘图之后，更新状态

    return false;
}

}  // end namespace QIM