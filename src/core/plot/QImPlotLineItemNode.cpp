#include "QImPlotLineItemNode.h"
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

class QImPlotLineItemNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlotLineItemNode)
public:
    PrivateData(QImPlotLineItemNode* p);
    void resetDownSamplerData();
    std::unique_ptr< QImAbstractXYDataSeries > data;
    std::unique_ptr< QImAbstractXYDataSeries > dataLTTB;
    bool isAdaptiveSampling { true };
    int downsampleThreshold { 20000 };
    ImPlotLineFlags lineFlags { ImPlotLineFlags_None };
    std::optional< QImTrackedValue< ImVec4, ImVecComparator< ImVec4 > > > color;  ///< 颜色
    QImTrackedValue< float > lineWidth { 1.0f };                                  ///< 线宽
    bool isPlotItemVisible;
};

QImPlotLineItemNode::PrivateData::PrivateData(QImPlotLineItemNode* p) : q_ptr(p)
{
}

/**
 * @brief 重置降采样数据，在设置数据后或者
 */
void QImPlotLineItemNode::PrivateData::resetDownSamplerData()
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
// QImPlotLineItemNode
//----------------------------------------------------
QImPlotLineItemNode::QImPlotLineItemNode(QObject* par) : QImPlotItemNode(par), QIM_PIMPL_CONSTRUCT
{
}

QImPlotLineItemNode::~QImPlotLineItemNode()
{
}

void QImPlotLineItemNode::setData(QImAbstractXYDataSeries* series)
{
    QIM_D(d);
    d->data.reset(series);
    if (d->isAdaptiveSampling) {
        d->resetDownSamplerData();
    }
}


QImAbstractXYDataSeries* QImPlotLineItemNode::data() const
{
    return d_ptr->data.get();
}

// ===== 在 CPP 文件顶部添加辅助宏定义 =====
#ifndef QImPlotLineItemNode_FLAG_ACCESSOR
#define QImPlotLineItemNode_FLAG_ACCESSOR(FlagName, FlagEnum)                                                          \
    bool QImPlotLineItemNode::is##FlagName() const                                                                     \
    {                                                                                                                  \
        QIM_DC(d);                                                                                                     \
        return (d->lineFlags & FlagEnum) != 0;                                                                         \
    }                                                                                                                  \
    void QImPlotLineItemNode::set##FlagName(bool on)                                                                   \
    {                                                                                                                  \
        QIM_D(d);                                                                                                      \
        const ImPlotLineFlags oldFlags = d->lineFlags;                                                                 \
        if (on)                                                                                                        \
            d->lineFlags |= FlagEnum;                                                                                  \
        else                                                                                                           \
            d->lineFlags &= ~FlagEnum;                                                                                 \
        if (d->lineFlags != oldFlags)                                                                                  \
            emit lineFlagChanged();                                                                                    \
    }
#endif
#ifndef QImPlotLineItemNode_ENABLED_ACCESSOR
#define QImPlotLineItemNode_ENABLED_ACCESSOR(PropName, FlagEnum)                                                       \
    bool QImPlotLineItemNode::is##PropName() const                                                                     \
    {                                                                                                                  \
        QIM_DC(d);                                                                                                     \
        return (d->lineFlags & FlagEnum) == 0;                                                                         \
    }                                                                                                                  \
    void QImPlotLineItemNode::set##PropName(bool enabled)                                                              \
    {                                                                                                                  \
        QIM_D(d);                                                                                                      \
        const ImPlotLineFlags oldFlags = d->lineFlags;                                                                 \
        if (enabled)                                                                                                   \
            d->lineFlags &= ~FlagEnum;                                                                                 \
        else                                                                                                           \
            d->lineFlags |= FlagEnum;                                                                                  \
        if (d->lineFlags != oldFlags)                                                                                  \
            emit lineFlagChanged();                                                                                    \
    }
#endif

/**
 * \if ENGLISH
 * @brief Returns the raw ImPlotLineFlags bitmask value for direct ImPlot API usage
 * @return Current combined flags as ImPlotLineFlags bitmask
 * @details Provides direct access to the underlying ImPlotLineFlags bitmask stored internally.
 *          The value can be passed directly to ImPlot::PlotLine() as the flags parameter.
 * @see setLineFlags(), ImPlot::PlotLine()
 * \endif
 *
 * \if CHINESE
 * @brief 返回原始 ImPlotLineFlags 位掩码值，用于直接调用 ImPlot API
 * @return 当前组合标志的 ImPlotLineFlags 位掩码
 * @details 提供对内部存储的 ImPlotLineFlags 位掩码的直接访问。
 *          该值可直接作为 flags 参数传递给 ImPlot::PlotLine()。
 * @see setLineFlags(), ImPlot::PlotLine()
 * \endif
 */
int QImPlotLineItemNode::lineFlags() const
{
    QIM_DC(d);
    return d->lineFlags;
}

/**
 * \if ENGLISH
 * @brief Sets the raw ImPlotLineFlags bitmask value with signal emission on change
 * @param flags New combined flags as ImPlotLineFlags bitmask
 * @details Directly replaces the internal flag storage with the provided bitmask value.
 *          All individual property states (segments, loop, skipNaN, etc.) are conceptually
 *          updated to reflect the new flags value. Emits lineFlagChanged() signal ONLY if the
 *          new value differs from current value (checked via != comparison).
 * @note This method bypasses individual property validation logic but maintains signal emission
 *       for reactive UI updates. Prefer individual property setters for type-safe configuration.
 * @warning May set invalid flag combinations (though ImPlotLineFlags has few conflicts).
 * @see lineFlags()
 * \endif
 *
 * \if CHINESE
 * @brief 设置原始 ImPlotLineFlags 位掩码值，值变更时触发信号
 * @param flags 新的组合标志 ImPlotLineFlags 位掩码
 * @details 直接用提供的位掩码值替换内部标志存储。
 *          所有单独的属性状态（segments、loop、skipNaN 等）在概念上都会更新以反映新标志值。
 *          仅当新值与当前值不同时（通过 != 比较），才会触发 lineFlagChanged() 信号。
 * @note 此方法绕过单独的属性验证逻辑，但保留信号发射以支持响应式 UI 更新。
 *       为类型安全配置，建议优先使用单独的属性设置器。
 * @warning 可能设置无效标志组合（尽管 ImPlotLineFlags 冲突较少）。
 * @see lineFlags()
 * \endif
 */
void QImPlotLineItemNode::setLineFlags(int flags)
{
    QIM_D(d);
    if (d->lineFlags != flags) {
        d->lineFlags = flags;
        emit lineFlagChanged();
    }
}


void QImPlotLineItemNode::setColor(const QColor& c)
{
    d_ptr->color = toImVec4(c);
}

QColor QImPlotLineItemNode::color() const
{
    return (d_ptr->color.has_value()) ? toQColor(d_ptr->color->value()) : QColor();
}

void QImPlotLineItemNode::setAdaptivesSampling(bool on)
{
    d_ptr->isAdaptiveSampling = on;
    d_ptr->resetDownSamplerData();
}

bool QImPlotLineItemNode::isAdaptiveSampling() const
{
    return d_ptr->isAdaptiveSampling;
}

// ===== 标志访问器实现（带 Doxygen 注释）=====
// clang-format off

/**
 * \if ENGLISH
 * @brief Checks if line is rendered as discrete segments between consecutive points
 * @return true if segments mode is enabled (ImPlotLineFlags_Segments set)
 * @details When enabled, each pair of consecutive points forms an independent line segment.
 *          Gaps appear where data is missing (e.g., NaN values) even without SkipNaN flag.
 *          Default behavior (disabled) connects all points with a continuous polyline.
 * @note Corresponds to ImPlotLineFlags_Segments flag (direct mapping).
 * @warning Segments mode disables automatic gap detection; use SkipNaN flag for explicit NaN handling.
 * @see setSegments(), isSkipNaN()
 * \endif
 *
 * \if CHINESE
 * @brief 检查线条是否渲染为连续点之间的离散线段
 * @return true 表示启用线段模式（设置了 ImPlotLineFlags_Segments）
 * @details 启用后，每对连续点形成独立的线段。
 *          即使未设置 SkipNaN 标志，缺失数据处（例如 NaN 值）也会出现间隙。
 *          默认行为（禁用）用连续折线连接所有点。
 * @note 对应 ImPlotLineFlags_Segments 标志（直接映射）。
 * @warning 线段模式禁用自动间隙检测；使用 SkipNaN 标志进行显式的 NaN 处理。
 * @see setSegments(), isSkipNaN()
 * \endif
 */
QImPlotLineItemNode_FLAG_ACCESSOR(Segments, ImPlotLineFlags_Segments)

/**
 * \if ENGLISH
 * @brief Checks if line forms a closed loop by connecting last and first points
 * @return true if loop mode is enabled (ImPlotLineFlags_Loop set)
 * @details When enabled, an additional segment connects the last data point to the first point.
 *          Creates closed shapes suitable for polygons, orbits, or cyclic data visualization.
 *          Requires at least 3 points for visible effect (2 points form a line segment).
 * @note Corresponds to ImPlotLineFlags_Loop flag (direct mapping).
 * @warning Loop mode combined with Segments flag creates disconnected segments plus closing segment.
 * @see setLoop(), isSegments()
 * \endif
 *
 * \if CHINESE
 * @brief 检查线条是否通过连接最后和第一个点形成闭合环
 * @return true 表示启用环模式（设置了 ImPlotLineFlags_Loop）
 * @details 启用后，额外线段连接最后一个数据点和第一个点。
 *          创建适合多边形、轨道或循环数据可视化的闭合形状。
 *          需要至少 3 个点才能看到明显效果（2 个点形成线段）。
 * @note 对应 ImPlotLineFlags_Loop 标志（直接映射）。
 * @warning 环模式与线段模式组合时，会创建不相连的线段加上闭合线段。
 * @see setLoop(), isSegments()
 * \endif
 */
QImPlotLineItemNode_FLAG_ACCESSOR(Loop, ImPlotLineFlags_Loop)

/**
 * \if ENGLISH
 * @brief Checks if NaN values are skipped during rendering (no gap drawn)
 * @return true if NaN skipping is enabled (ImPlotLineFlags_SkipNaN set)
 * @details When enabled, points with NaN coordinates are silently skipped without breaking the line.
 *          Creates continuous appearance even with missing data points.
 *          When disabled, NaN values break the line into separate segments (gap appears).
 * @note Corresponds to ImPlotLineFlags_SkipNaN flag (direct mapping).
 * @warning Skipping NaNs may hide data quality issues; use with caution for scientific visualization.
 * @see setSkipNaN(), isSegments()
 * \endif
 *
 * \if CHINESE
 * @brief 检查渲染时是否跳过 NaN 值（不绘制间隙）
 * @return true 表示启用 NaN 跳过（设置了 ImPlotLineFlags_SkipNaN）
 * @details 启用后，具有 NaN 坐标的点会被静默跳过，不会中断线条。
 *          即使存在缺失数据点，也能创建连续外观。
 *          禁用时，NaN 值会将线条断开为独立线段（出现间隙）。
 * @note 对应 ImPlotLineFlags_SkipNaN 标志（直接映射）。
 * @warning 跳过 NaN 可能隐藏数据质量问题；科学可视化中请谨慎使用。
 * @see setSkipNaN(), isSegments()
 * \endif
 */
QImPlotLineItemNode_FLAG_ACCESSOR(SkipNaN, ImPlotLineFlags_SkipNaN)

/**
 * \if ENGLISH
 * @brief Checks if markers on plot edges are clipped to plot area boundaries
 * @return true if clipping is enabled (ImPlotLineFlags_NoClip NOT set)
 * @details When enabled, markers (e.g., circles at data points) are clipped at plot boundaries.
 *          Prevents visual artifacts when markers extend beyond visible area.
 *          When disabled (NoClip set), markers render fully even when partially outside plot area.
 * @note Corresponds to ImPlotLineFlags_NoClip flag with inverted logic:
 *       enabled = flag NOT set (clipping active), disabled = flag set (no clipping).
 * @warning Disabling clipping may cause markers to overlap adjacent UI elements or axes.
 * @see setClippingEnabled()
 * \endif
 *
 * \if CHINESE
 * @brief 检查绘图边缘的标记是否被裁剪到绘图区域边界内
 * @return true 表示启用裁剪（未设置 ImPlotLineFlags_NoClip）
 * @details 启用后，标记（例如数据点处的圆圈）在绘图边界处被裁剪。
 *          防止标记超出可见区域时产生视觉瑕疵。
 *          禁用时（设置 NoClip），即使部分超出绘图区域，标记也会完整渲染。
 * @note 对应 ImPlotLineFlags_NoClip 标志，逻辑反转：
 *       启用 = 未设置标志位（裁剪激活），禁用 = 设置标志位（无裁剪）。
 * @warning 禁用裁剪可能导致标记与相邻 UI 元素或坐标轴重叠。
 * @see setClippingEnabled()
 * \endif
 */
QImPlotLineItemNode_ENABLED_ACCESSOR(ClippingEnabled, ImPlotLineFlags_NoClip)

/**
 * \if ENGLISH
 * @brief Checks if area between line and horizontal origin is filled with color
 * @return true if shaded mode is enabled (ImPlotLineFlags_Shaded set)
 * @details When enabled, a filled region is rendered between the line and y=0 (horizontal origin).
 *          Creates area-chart appearance. Color matches line color with reduced opacity.
 *          For custom fill baselines or multi-line fills, use ImPlot::PlotShaded() directly.
 * @note Corresponds to ImPlotLineFlags_Shaded flag (direct mapping).
 * @warning Shaded mode requires non-negative Y values for predictable appearance.
 *          Negative values create fills extending downward from y=0.
 * @see setShaded(), ImPlot::PlotShaded()
 * \endif
 *
 * \if CHINESE
 * @brief 检查线条与水平原点之间的区域是否填充颜色
 * @return true 表示启用填充模式（设置了 ImPlotLineFlags_Shaded）
 * @details 启用后，线条与 y=0（水平原点）之间会渲染填充区域。
 *          创建面积图外观。颜色与线条颜色匹配但透明度降低。
 *          对于自定义填充基线或多线条填充，请直接使用 ImPlot::PlotShaded()。
 * @note 对应 ImPlotLineFlags_Shaded 标志（直接映射）。
 * @warning 填充模式要求 Y 值非负以获得可预测外观。
 *          负值会创建从 y=0 向下延伸的填充。
 * @see setShaded(), ImPlot::PlotShaded()
 * \endif
 */
QImPlotLineItemNode_FLAG_ACCESSOR(Shaded, ImPlotLineFlags_Shaded)
// clang-format off
    // clang-format on
    // clang-format on

    /**
     * @brief 绘图
     * @return  这里直接返回false，避免调用endDraw
     */
    bool QImPlotLineItemNode::beginDraw()
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
    if (d->color && (d->color->is_dirty() || d->lineWidth.is_dirty())) {
        ImPlot::SetNextLineStyle(d->color->value(), d->lineWidth.value());
    }
    if (series->isContiguous()) {
        if (series->xRawData()) {
            // 有x指针，说明不是yonly
            ImPlot::PlotLine(
                labelConstData(),
                series->xRawData(),
                series->yRawData(),
                series->size(),
                d->lineFlags,
                series->offset(),
                series->stride()
            );
        } else {
            // x指针没有说明是yonly
            ImPlot::PlotLine(
                labelConstData(),
                series->yRawData(),
                series->size(),
                series->xScale(),
                series->xStart(),
                d->lineFlags,
                series->offset(),
                series->stride()
            );
        }
    } else {
        // TODO:非连续内存
    }
    // 更新item的状态
    ImPlotContext* ct    = ImPlot::GetCurrentContext();
    ImPlotItem* plotItem = ct->PreviousItem;  // 通过源码，PlotLine结束后，ImPlotItem就是PreviousItem
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
