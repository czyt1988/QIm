#include "QImPlotScatterItemNode.h"
#include <optional>
#include "QImPlotDataSeries.h"
#include "QImLTTBDownsampler.h"
#include "QImMinMaxLTTBDownsampler.h"
#include "implot.h"
#include "implot_internal.h"
#include "QImTrackedValue.hpp"
#include "QtImGuiUtils.h"
#include <QDebug>

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
    void resetDownSamplerData();
    std::unique_ptr< QImAbstractXYDataSeries > data;
    std::unique_ptr< QImAbstractXYDataSeries > dataLTTB;
    bool isAdaptiveSampling { true };
    int downsampleThreshold { 20000 };
    QImTrackedValue< int > markerShape { ImPlotMarker_Circle };                      ///< 标记形状
    bool markerFill { true };
    std::optional< QImTrackedValue< ImVec4, ImVecComparator< ImVec4 > > > color;  ///< 颜色
    QImTrackedValue< float > markerSize { 4.0f };                                 ///< 标记大小
    bool isPlotItemVisible;
};

QImPlotScatterItemNode::PrivateData::PrivateData(QImPlotScatterItemNode* p) : q_ptr(p)
{
}

/**
 * @brief 重置降采样数据，在设置数据后或者
 */
void QImPlotScatterItemNode::PrivateData::resetDownSamplerData()
{
    if (isAdaptiveSampling) {
        if (data && (data->size() > downsampleThreshold)) {
#if 0
            QImLTTBDownsampler* lttb = new QImLTTBDownsampler(data.get(), downsampleThreshold);
            dataLTTB.reset(lttb);
#else
            QImMinMaxLTTBDownsampler* lttb = new QImMinMaxLTTBDownsampler(data.get(), downsampleThreshold);
            dataLTTB.reset(lttb);
#endif
        }
    } else {
        dataLTTB.reset(nullptr);
    }
}
//----------------------------------------------------
// QImPlotScatterItemNode
//----------------------------------------------------
QImPlotScatterItemNode::QImPlotScatterItemNode(QObject* par) : QImPlotItemNode(par), QIM_PIMPL_CONSTRUCT
{
}

QImPlotScatterItemNode::~QImPlotScatterItemNode()
{
}

void QImPlotScatterItemNode::setData(QImAbstractXYDataSeries* series)
{
    QIM_D(d);
    d->data.reset(series);
    if (d->isAdaptiveSampling) {
        d->resetDownSamplerData();
    }
    emit dataChanged();
}

QImAbstractXYDataSeries* QImPlotScatterItemNode::data() const
{
    return d_ptr->data.get();
}

// ===== 在 CPP 文件顶部添加辅助宏定义 =====
#ifndef QImPlotScatterItemNode_FLAG_ACCESSOR
#define QImPlotScatterItemNode_FLAG_ACCESSOR(FlagName, FlagEnum)                                                       \
    bool QImPlotScatterItemNode::is##FlagName() const                                                                  \
    {                                                                                                                  \
        QIM_DC(d);                                                                                                     \
        return (d->FlagName);                                                                                          \
    }                                                                                                                  \
    void QImPlotScatterItemNode::set##FlagName(bool on)                                                                \
    {                                                                                                                  \
        QIM_D(d);                                                                                                      \
        if (d->FlagName != on) {                                                                                       \
            d->FlagName = on;                                                                                          \
            emit scatterFlagChanged();                                                                                 \
        }                                                                                                              \
    }
#endif

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
        emit markerSizeChanged(size);
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
        emit markerShapeChanged(shape);
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
        emit markerFillChanged(fill);
    }
}

/**
 * \if ENGLISH
 * @brief Checks if adaptive sampling is enabled
 * @return true if adaptive sampling is enabled
 * @details Returns current adaptive sampling state.
 *          Default is true (enabled).
 * @see setAdaptiveSampling()
 * \endif
 *
 * \if CHINESE
 * @brief 检查自适应采样是否启用
 * @return true表示自适应采样启用
 * @details 返回当前自适应采样状态。
 *          默认为true（启用）。
 * @see setAdaptiveSampling()
 * \endif
 */
bool QImPlotScatterItemNode::isAdaptiveSampling() const
{
    QIM_DC(d);
    return d->isAdaptiveSampling;
}

/**
 * \if ENGLISH
 * @brief Enables or disables adaptive sampling
 * @param[in] enabled true to enable adaptive sampling
 * @details Updates adaptive sampling state and regenerates downsampled data if needed.
 *          Emits adaptiveSamplingChanged() signal if value changed.
 * @note Disabling adaptive sampling removes downsampled data, using original data for rendering.
 * @see isAdaptiveSampling()
 * \endif
 *
 * \if CHINESE
 * @brief 启用或禁用自适应采样
 * @param[in] enabled true启用自适应采样
 * @details 更新自适应采样状态，并在需要时重新生成降采样数据。
 *          如果值更改，触发adaptiveSamplingChanged()信号。
 * @note 禁用自适应采样会移除降采样数据，使用原始数据进行渲染。
 * @see isAdaptiveSampling()
 * \endif
 */
void QImPlotScatterItemNode::setAdaptiveSampling(bool enabled)
{
    QIM_D(d);
    if (d->isAdaptiveSampling != enabled) {
        d->isAdaptiveSampling = enabled;
        d->resetDownSamplerData();
        emit adaptiveSamplingChanged(enabled);
    }
}

/**
 * \if ENGLISH
 * @brief Gets the downsample threshold
 * @return Current downsample threshold in data points
 * @details Returns the dataset size threshold for triggering adaptive sampling.
 *          Default is 20000 points.
 * @see setDownsampleThreshold()
 * \endif
 *
 * \if CHINESE
 * @brief 获取降采样阈值
 * @return 当前降采样阈值（数据点）
 * @details 返回触发自适应采样的数据集大小阈值。
 *          默认为20000点。
 * @see setDownsampleThreshold()
 * \endif
 */
int QImPlotScatterItemNode::downsampleThreshold() const
{
    QIM_DC(d);
    return d->downsampleThreshold;
}

/**
 * \if ENGLISH
 * @brief Sets the downsample threshold
 * @param[in] threshold New downsample threshold in data points
 * @details Updates downsample threshold and regenerates downsampled data if needed.
 *          Emits downsampleThresholdChanged() signal if value changed.
 *          Value must be positive.
 * @note Lower thresholds increase downsampling frequency but may reduce visual quality.
 * @see downsampleThreshold()
 * \endif
 *
 * \if CHINESE
 * @brief 设置降采样阈值
 * @param[in] threshold 新降采样阈值（数据点）
 * @details 更新降采样阈值，并在需要时重新生成降采样数据。
 *          如果值更改，触发downsampleThresholdChanged()信号。
 *          值必须为正数。
 * @note 较低的阈值增加降采样频率但可能降低视觉质量。
 * @see downsampleThreshold()
 * \endif
 */
void QImPlotScatterItemNode::setDownsampleThreshold(int threshold)
{
    QIM_D(d);
    if (d->downsampleThreshold != threshold && threshold > 0) {
        d->downsampleThreshold = threshold;
        d->resetDownSamplerData();
        emit downsampleThresholdChanged(threshold);
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
    return (d_ptr->color.has_value()) ? toQColor(d_ptr->color->value()) : QColor();
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
    d_ptr->color = toImVec4(c);
    emit colorChanged(c);
}

/**
 * @brief 绘图
 * @return  这里直接返回false，避免调用endDraw
 */
bool QImPlotScatterItemNode::beginDraw()
{
    QIM_D(d);
    if (!d->data) {
        // 没有数据
        return false;
    }
    QImAbstractXYDataSeries* series = d->data.get();
    if (d->isAdaptiveSampling && d->dataLTTB) {
        series = d->dataLTTB.get();
    }
    if (!series) {
        return false;
    }

    // 准备标记样式
    ImPlotMarker marker = static_cast<ImPlotMarker>(d->markerShape.value());
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
                                0,  // ImPlotScatterFlags
                                series->offset(),
                                series->stride());
        } else {
            // x指针没有说明是yonly
            ImPlot::PlotScatter(labelConstData(),
                                series->yRawData(),
                                series->size(),
                                series->xScale(),
                                series->xStart(),
                                0,  // ImPlotScatterFlags
                                series->offset(),
                                series->stride());
        }
    } else {
        // TODO:非连续内存
    }

    // 更新item的状态
    ImPlotContext* ct    = ImPlot::GetCurrentContext();
    ImPlotItem* plotItem = ct->PreviousItem;  // 通过源码，PlotScatter结束后，ImPlotItem就是PreviousItem
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