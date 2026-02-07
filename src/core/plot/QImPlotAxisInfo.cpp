#include "QImPlotAxisInfo.h"
#include <QDebug>
#include "QImTrackedValue.hpp"
#include "implot.h"
#include "QImPlotNode.h"
namespace QIM
{
class QImPlotAxisInfo::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlotAxisInfo)
public:
    PrivateData(QImPlotAxisInfo* p);

public:
    ImAxis axisId;
    QImTrackedValue< QByteArray > labelUtf8;  // 缓存 UTF-8
    QImTrackedValue< double > minLimits { 0.0 };
    QImTrackedValue< double > maxLimits { 1.0 };
    ImPlotAxisFlags flags { ImPlotAxisFlags_None };
    ImPlotAxisFlags tempFlags { ImPlotAxisFlags_None };  ///< 针对X1和Y1设置enable==false时，把原有的flag保存下来，以便设置为true时还原
    QImTrackedValue< ImPlotScale > scale { ImPlotScale_Linear };
    ImPlotCond limitCond { ImPlotCond_Once };
    bool enable { false };  ///< 对于X1和Y1设置为无效是把flag设置为
    QImPlotNode* plot { nullptr };
};

QImPlotAxisInfo::PrivateData::PrivateData(QImPlotAxisInfo* p) : q_ptr(p)
{
}

//===============================================================
// QImPlotAxisInfo
//===============================================================
/**
 * \if ENGLISH
 * @brief Constructor for QImPlotAxisInfo
 * @param axis The ImPlot axis identifier (e.g., ImAxis_X1, ImAxis_Y1)
 * @param parent Optional QObject parent for memory management
 * @details Initializes axis with default settings:
 *          - Label: empty string (will use default axis name)
 *          - Limits: [0.0, 1.0] range
 *          - Flags: ImPlotAxisFlags_None (all decorations enabled, no inversion)
 * \endif
 *
 * \if CHINESE
 * @brief QImPlotAxisInfo 构造函数
 * @param axis ImPlot 轴标识符（例如 ImAxis_X1, ImAxis_Y1）
 * @param parent 可选的 QObject 父对象，用于内存管理
 * @details 初始化轴的默认设置：
 *          - 标签：空字符串（将使用默认轴名称）
 *          - 范围：[0.0, 1.0]
 *          - 标志：ImPlotAxisFlags_None（启用所有装饰元素，无反转）
 * \endif
 */
QImPlotAxisInfo::QImPlotAxisInfo(QImPlotAxisId axis, QImPlotNode* plot) : QObject(plot), QIM_PIMPL_CONSTRUCT
{
    d_ptr->plot   = plot;
    d_ptr->axisId = toImAxis(axis);
}

QImPlotAxisInfo::~QImPlotAxisInfo()
{
}

/**
 * \if ENGLISH
 * @brief Returns the underlying ImPlot axis identifier
 * @return QImPlotAxisId enum value
 * @note This value should not passed directly to ImPlot::SetupAxis() functions,use @ref plotAxisIdToImAxis to cast
 * \endif
 *
 * \if CHINESE
 * @brief 返回 QImPlotAxisId 轴标识符
 * @return QImPlotAxisId 枚举值（X1对应 ImAxis_X1...）
 * @note 该值应不能直接传递给 ImPlot::SetupAxis() 等函数，可以使用@ref plotAxisIdToImAxis 去转换
 * \endif
 */
QImPlotAxisId QImPlotAxisInfo::axisId() const
{
    return toQImPlotAxisId(d_ptr->axisId);
}

/**
 * @brief Returns the raw ImAxis value for direct ImPlot API usage
 * @return
 */
int QImPlotAxisInfo::imAxis() const
{
    return d_ptr->axisId;
}

/**
 * \if ENGLISH
 * @brief Gets the axis label text displayed next to the axis
 * @return Current label string (empty string means default axis name is used)
 * \endif
 *
 * \if CHINESE
 * @brief 获取显示在坐标轴旁边的标签文本
 * @return 当前标签字符串（空字符串表示使用默认轴名称）
 * \endif
 */
QString QImPlotAxisInfo::label() const
{
    return QString::fromUtf8(d_ptr->labelUtf8.value());
}

/**
 * \if ENGLISH
 * @brief Sets the axis label text
 * @param label New label string. Empty string restores default axis name.
 * @see ImPlot::SetupAxis()
 * \endif
 *
 * \if CHINESE
 * @brief 设置坐标轴标签文本
 * @param label 新标签字符串。空字符串将恢复默认轴名称。
 * @see ImPlot::SetupAxis()
 * \endif
 */
void QImPlotAxisInfo::setLabel(const QString& label)
{
    QIM_D(d);
    d_ptr->labelUtf8 = label.toUtf8();
    if (d_ptr->labelUtf8.is_dirty()) {
        Q_EMIT labelChanged(label);
    }
}

/**
 * \if ENGLISH
 * @brief Gets the current minimum limit of the axis range
 * @return Minimum value of the axis visible range
 * @note This value is used with SetupAxisLimits() to constrain panning/zooming
 * \endif
 *
 * \if CHINESE
 * @brief 获取坐标轴范围的当前最小限制值
 * @return 坐标轴可见范围的最小值
 * @note 该值与 SetupAxisLimits() 配合使用，用于约束平移/缩放操作
 * \endif
 */
double QImPlotAxisInfo::minLimits() const
{
    return d_ptr->minLimits.value();
}

/**
 * \if ENGLISH
 * @brief Sets the minimum limit of the axis range
 * @param min New minimum value for the axis
 * @note Changing limits does NOT immediately affect the plot; must call SetupAxisLimits() during plot setup
 * @see setLimits(), maxLimits()
 * \endif
 *
 * \if CHINESE
 * @brief 设置坐标轴范围的最小限制值
 * @param min 坐标轴的新最小值
 * @note 修改限制值不会立即影响绘图；必须在绘图设置阶段调用 SetupAxisLimits()
 * @see setLimits(), maxLimits()
 * \endif
 */

void QImPlotAxisInfo::setMinLimits(double min)
{
    QIM_D(d);
    d->minLimits = min;
    if (d->minLimits.is_dirty()) {
        Q_EMIT limitsChanged(d->maxLimits.value(), d->maxLimits.value());
    }
}

/**
 * \if ENGLISH
 * @brief Gets the current maximum limit of the axis range
 * @return Maximum value of the axis visible range
 * @note This value is used with SetupAxisLimits() to constrain panning/zooming
 * \endif
 *
 * \if CHINESE
 * @brief 获取坐标轴范围的当前最大限制值
 * @return 坐标轴可见范围的最大值
 * @note 该值与 SetupAxisLimits() 配合使用，用于约束平移/缩放操作
 * \endif
 */
double QImPlotAxisInfo::maxLimits() const
{
    return d_ptr->maxLimits.value();
}

/**
 * \if ENGLISH
 * @brief Sets the maximum limit of the axis range
 * @param max New maximum value for the axis
 * @note Changing limits does NOT immediately affect the plot; must call SetupAxisLimits() during plot setup
 * @see setLimits(), minLimits()
 * \endif
 *
 * \if CHINESE
 * @brief 设置坐标轴范围的最大限制值
 * @param max 坐标轴的新最大值
 * @note 修改限制值不会立即影响绘图；必须在绘图设置阶段调用 SetupAxisLimits()
 * @see setLimits(), minLimits()
 * \endif
 */
void QImPlotAxisInfo::setMaxLimits(double max)
{
    QIM_D(d);
    d->maxLimits = max;
    if (d->maxLimits.is_dirty()) {
        Q_EMIT limitsChanged(d->maxLimits.value(), d->maxLimits.value());
    }
}

QImPlotCondition QImPlotAxisInfo::limitsCondition() const
{
    return toQImPlotCondition(d_ptr->limitCond);
}

void QImPlotAxisInfo::setLimitsCondition(QImPlotCondition v)
{
    d_ptr->limitCond = toImPlotCond(v);
}

/**
 * \if ENGLISH
 * @brief Sets both minimum and maximum limits of the axis range simultaneously
 * @param min New minimum value
 * @param max New maximum value
 * @note Ensures min < max; values are not swapped automatically
 * @see SetupAxisLimits()
 * \endif
 *
 * \if CHINESE
 * @brief 同时设置坐标轴范围的最小和最大限制值
 * @param min 新最小值
 * @param max 新最大值
 * @param cond 应用条件，对应ImPlotCond
 * @note 不自动交换 min/max 顺序；需确保 min < max
 * @see SetupAxisLimits()
 * \endif
 */
void QImPlotAxisInfo::setLimits(double min, double max, QImPlotCondition cond)
{
    QIM_D(d);
    d->minLimits = min;
    d->maxLimits = max;
    d->limitCond = toImPlotCond(cond);
    if (d->minLimits.is_dirty() || d->maxLimits.is_dirty()) {
        Q_EMIT limitsChanged(d->maxLimits.value(), d->maxLimits.value());
    }
}

// ===== 标志访问器实现 =====

/**
 * \if ENGLISH
 * @fn bool QImPlotAxisInfo::isAutoFit() const
 * @brief Checks if axis automatically fits to data extents
 * @return true if auto-fitting is enabled (ImPlotAxisFlags_AutoFit set)
 * @details When enabled, axis range dynamically adjusts to show all plotted data during interaction.
 *          Overrides manual limits set via setLimits() when user pans/zooms.
 * @note Corresponds to ImPlotAxisFlags_AutoFit flag. Conflicts with LockMin/LockMax flags; auto-fit takes precedence
 * when active. \endif
 *
 * \if CHINESE
 * @fn bool QImPlotAxisInfo::isAutoFit() const
 * @brief 检查坐标轴是否自动适配数据范围
 * @return true 表示启用自动适配（设置了 ImPlotAxisFlags_AutoFit）
 * @details 启用后，交互过程中坐标轴范围会动态调整以显示所有绘制的数据。
 *          用户平移/缩放时会覆盖通过 setLimits() 设置的手动限制。
 * @note 对应 ImPlotAxisFlags_AutoFit 标志。与 LockMin/LockMax 标志冲突；激活时自动适配优先级更高。
 * \endif
 */

/**
 * \if ENGLISH
 * @fn void QImPlotAxisInfo::setAutoFit(bool on)
 * @brief Enables/disables automatic axis fitting to data extents
 * @param on true to enable auto-fitting, false to use fixed limits
 * @details When disabled, axis maintains fixed range defined by setLimits().
 *          User can still pan/zoom unless LockMin/LockMax are set.
 * @note Sets/clears ImPlotAxisFlags_AutoFit flag. Prefer individual property setters for type safety.
 * @see setLimits(), isLockMin(), isLockMax()
 * \endif
 *
 * \if CHINESE
 * @fn void QImPlotAxisInfo::setAutoFit(bool on)
 * @brief 启用/禁用坐标轴自动适配数据范围
 * @param on true 启用自动适配，false 使用固定范围
 * @details 禁用后，坐标轴将保持 setLimits() 定义的固定范围。
 *          除非设置了 LockMin/LockMax，否则用户仍可平移/缩放。
 * @note 设置/清除 ImPlotAxisFlags_AutoFit 标志。为类型安全，建议优先使用属性设置器。
 * @see setLimits(), isLockMin(), isLockMax()
 * \endif
 */

/**
 * \if ENGLISH
 * @fn bool QImPlotAxisInfo::isInverted() const
 * @brief Checks if axis direction is inverted (high values at bottom/left)
 * @return true if axis is inverted (ImPlotAxisFlags_Invert set)
 * @details Normally: X-axis increases left→right, Y-axis increases bottom→top.
 *          When inverted: X-axis increases right→left, Y-axis increases top→bottom.
 * @note Corresponds to ImPlotAxisFlags_Invert flag (note enum name is "Invert", property is "inverted" for Qt
 * convention). \endif
 *
 * \if CHINESE
 * @fn bool QImPlotAxisInfo::isInverted() const
 * @brief 检查坐标轴方向是否反转（高值在底部/左侧）
 * @return true 表示轴已反转（设置了 ImPlotAxisFlags_Invert）
 * @details 正常情况：X轴从左到右递增，Y轴从下到上递增。
 *          反转后：X轴从右到左递增，Y轴从上到下递增。
 * @note 对应 ImPlotAxisFlags_Invert 标志（注意枚举名为"Invert"，属性名为"inverted"以符合 Qt 命名规范）。
 * \endif
 */

/**
 * \if ENGLISH
 * @fn void QImPlotAxisInfo::setInverted(bool on)
 * @brief Enables/disables axis inversion
 * @param on true to invert axis direction, false for normal direction
 * @note Sets/clears ImPlotAxisFlags_Invert flag.
 * @see isInverted()
 * \endif
 *
 * \if CHINESE
 * @fn void QImPlotAxisInfo::setInverted(bool on)
 * @brief 启用/禁用坐标轴反转
 * @param on true 反转轴方向，false 为正常方向
 * @note 设置/清除 ImPlotAxisFlags_Invert 标志。
 * @see isInverted()
 * \endif
 */

/**
 * \if ENGLISH
 * @fn bool QImPlotAxisInfo::isLabelEnabled() const
 * @brief Checks if axis label is visible
 * @return true if label is enabled (ImPlotAxisFlags_NoLabel NOT set)
 * @details When disabled, axis name text is hidden but ticks/grid may remain visible.
 *          Also hidden if label string is empty regardless of this flag.
 * @note Corresponds to ImPlotAxisFlags_NoLabel flag with inverted logic:
 *       enabled = flag NOT set, disabled = flag set.
 * @note Distinct from tick labels (isTickLabelsEnabled())
 * \endif
 *
 * \if CHINESE
 * @fn bool QImPlotAxisInfo::isLabelEnabled() const
 * @brief 检查坐标轴标签是否可见
 * @return true 表示标签启用（未设置 ImPlotAxisFlags_NoLabel）
 * @details 禁用后，轴名称文本将隐藏，但刻度/网格可能仍可见。
 *          如果标签字符串为空，无论此标志如何都会隐藏标签。
 * @note 对应 ImPlotAxisFlags_NoLabel 标志，逻辑反转：
 *       启用 = 未设置标志位，禁用 = 设置标志位。
 * @note 与刻度标签不同（isTickLabelsEnabled()）
 * \endif
 */

/**
 * \if ENGLISH
 * @fn void QImPlotAxisInfo::setLabelEnabled(bool enabled)
 * @brief Enables/disables axis label visibility
 * @param enabled true to show axis name text, false to hide it
 * @note Controls ImPlotAxisFlags_NoLabel flag with inverted logic:
 *       enabled → clear flag, disabled → set flag.
 * @note Hiding label does NOT affect tick marks or grid lines
 * @see setTickLabelsEnabled(), setDecorationsEnabled()
 * \endif
 *
 * \if CHINESE
 * @fn void QImPlotAxisInfo::setLabelEnabled(bool enabled)
 * @brief 启用/禁用坐标轴标签可见性
 * @param enabled true 显示轴名称文本，false 隐藏
 * @note 控制 ImPlotAxisFlags_NoLabel 标志，逻辑反转：
 *       启用 → 清除标志位，禁用 → 设置标志位。
 * @note 隐藏标签不会影响刻度线或网格线
 * @see setTickLabelsEnabled(), setDecorationsEnabled()
 * \endif
 */

/**
 * \if ENGLISH
 * @fn bool QImPlotAxisInfo::isGridLinesEnabled() const
 * @brief Checks if grid lines are visible for this axis
 * @return true if grid lines are enabled (ImPlotAxisFlags_NoGridLines NOT set)
 * @details Grid lines extend from axis ticks across the plot area.
 *          Controlled independently for X and Y axes.
 * @note Corresponds to ImPlotAxisFlags_NoGridLines flag with inverted logic:
 *       enabled = flag NOT set, disabled = flag set.
 * @note Foreground grid lines (isForeground()) render above data when enabled
 * \endif
 *
 * \if CHINESE
 * @fn bool QImPlotAxisInfo::isGridLinesEnabled() const
 * @brief 检查此坐标轴的网格线是否可见
 * @return true 表示网格线启用（未设置 ImPlotAxisFlags_NoGridLines）
 * @details 网格线从轴刻度延伸穿过绘图区域。
 *          X轴和Y轴的网格线独立控制。
 * @note 对应 ImPlotAxisFlags_NoGridLines 标志，逻辑反转：
 *       启用 = 未设置标志位，禁用 = 设置标志位。
 * @note 启用前景网格线（isForeground()）时，网格线会渲染在数据上方
 * \endif
 */

/**
 * \if ENGLISH
 * @fn void QImPlotAxisInfo::setGridLinesEnabled(bool enabled)
 * @brief Enables/disables grid lines for this axis
 * @param enabled true to show grid lines, false to hide them
 * @note Controls ImPlotAxisFlags_NoGridLines flag with inverted logic:
 *       enabled → clear flag, disabled → set flag.
 * @note Grid lines visibility is independent per axis (X vs Y)
 * @see setForeground()
 * \endif
 *
 * \if CHINESE
 * @fn void QImPlotAxisInfo::setGridLinesEnabled(bool enabled)
 * @brief 启用/禁用此坐标轴的网格线
 * @param enabled true 显示网格线，false 隐藏
 * @note 控制 ImPlotAxisFlags_NoGridLines 标志，逻辑反转：
 *       启用 → 清除标志位，禁用 → 设置标志位。
 * @note 网格线可见性按轴独立控制（X轴与Y轴）
 * @see setForeground()
 * \endif
 */

/**
 * \if ENGLISH
 * @fn bool QImPlotAxisInfo::isTickMarksEnabled() const
 * @brief Checks if tick marks are visible on the axis
 * @return true if tick marks are enabled (ImPlotAxisFlags_NoTickMarks NOT set)
 * @details Tick marks are small lines perpendicular to axis at major/minor intervals.
 *          Required for tick labels to be meaningful.
 * @note Corresponds to ImPlotAxisFlags_NoTickMarks flag with inverted logic:
 *       enabled = flag NOT set, disabled = flag set.
 * @note Hiding tick marks does NOT hide tick labels (controlled separately)
 * \endif
 *
 * \if CHINESE
 * @fn bool QImPlotAxisInfo::isTickMarksEnabled() const
 * @brief 检查坐标轴上的刻度线是否可见
 * @return true 表示刻度线启用（未设置 ImPlotAxisFlags_NoTickMarks）
 * @details 刻度线是在主/次间隔处垂直于轴的小线段。
 *          刻度标签需要刻度线才有意义。
 * @note 对应 ImPlotAxisFlags_NoTickMarks 标志，逻辑反转：
 *       启用 = 未设置标志位，禁用 = 设置标志位。
 * @note 隐藏刻度线不会隐藏刻度标签（单独控制）
 * \endif
 */

/**
 * \if ENGLISH
 * @fn void QImPlotAxisInfo::setTickMarksEnabled(bool enabled)
 * @brief Enables/disables tick marks on the axis
 * @param enabled true to show tick marks, false to hide them
 * @note Controls ImPlotAxisFlags_NoTickMarks flag with inverted logic:
 *       enabled → clear flag, disabled → set flag.
 * @note Tick labels may still be visible even when tick marks are hidden
 * @see setTickLabelsEnabled()
 * \endif
 *
 * \if CHINESE
 * @fn void QImPlotAxisInfo::setTickMarksEnabled(bool enabled)
 * @brief 启用/禁用坐标轴上的刻度线
 * @param enabled true 显示刻度线，false 隐藏
 * @note 控制 ImPlotAxisFlags_NoTickMarks 标志，逻辑反转：
 *       启用 → 清除标志位，禁用 → 设置标志位。
 * @note 即使隐藏刻度线，刻度标签仍可能可见
 * @see setTickLabelsEnabled()
 * \endif
 */

/**
 * \if ENGLISH
 * @fn bool QImPlotAxisInfo::isTickLabelsEnabled() const
 * @brief Checks if numeric tick labels are visible
 * @return true if tick labels are enabled (ImPlotAxisFlags_NoTickLabels NOT set)
 * @details Shows numeric values at major tick positions (e.g., "0.0", "1.0", "2.0").
 *          Format controlled by ImPlot::SetAxisFormat() or default formatting.
 * @note Corresponds to ImPlotAxisFlags_NoTickLabels flag with inverted logic:
 *       enabled = flag NOT set, disabled = flag set.
 * @note Requires tick marks to be meaningful; can be hidden independently
 * \endif
 *
 * \if CHINESE
 * @fn bool QImPlotAxisInfo::isTickLabelsEnabled() const
 * @brief 检查数字刻度标签是否可见
 * @return true 表示刻度标签启用（未设置 ImPlotAxisFlags_NoTickLabels）
 * @details 在主刻度位置显示数字值（例如 "0.0"、"1.0"、"2.0"）。
 *          格式由 ImPlot::SetAxisFormat() 或默认格式控制。
 * @note 对应 ImPlotAxisFlags_NoTickLabels 标志，逻辑反转：
 *       启用 = 未设置标志位，禁用 = 设置标志位。
 * @note 需要刻度线才有意义；可独立隐藏
 * \endif
 */

/**
 * \if ENGLISH
 * @fn void QImPlotAxisInfo::setTickLabelsEnabled(bool enabled)
 * @brief Enables/disables numeric tick labels
 * @param enabled true to show numeric values at ticks, false to hide them
 * @note Controls ImPlotAxisFlags_NoTickLabels flag with inverted logic:
 *       enabled → clear flag, disabled → set flag.
 * @note Hiding tick labels does NOT hide tick marks or grid lines
 * @see setTickMarksEnabled(), setGridLinesEnabled()
 * \endif
 *
 * \if CHINESE
 * @fn void QImPlotAxisInfo::setTickLabelsEnabled(bool enabled)
 * @brief 启用/禁用数字刻度标签
 * @param enabled true 在刻度处显示数字值，false 隐藏
 * @note 控制 ImPlotAxisFlags_NoTickLabels 标志，逻辑反转：
 *       启用 → 清除标志位，禁用 → 设置标志位。
 * @note 隐藏刻度标签不会隐藏刻度线或网格线
 * @see setTickMarksEnabled(), setGridLinesEnabled()
 * \endif
 */

/**
 * \if ENGLISH
 * @fn bool QImPlotAxisInfo::isInitialFitEnabled() const
 * @brief Checks if axis fits to data on first render frame
 * @return true if initial fit is enabled (ImPlotAxisFlags_NoInitialFit NOT set)
 * @details When enabled, axis automatically adjusts range to show all data on first plot render.
 *          Subsequent behavior controlled by isAutoFit().
 * @note Corresponds to ImPlotAxisFlags_NoInitialFit flag with inverted logic:
 *       enabled = flag NOT set, disabled = flag set.
 * @note Only affects the very first render after plot creation
 * \endif
 *
 * \if CHINESE
 * @fn bool QImPlotAxisInfo::isInitialFitEnabled() const
 * @brief 检查坐标轴是否在首帧渲染时自动适配数据
 * @return true 表示启用初始适配（未设置 ImPlotAxisFlags_NoInitialFit）
 * @details 启用后，坐标轴在首次绘图渲染时自动调整范围以显示所有数据。
 *          后续行为由 isAutoFit() 控制。
 * @note 对应 ImPlotAxisFlags_NoInitialFit 标志，逻辑反转：
 *       启用 = 未设置标志位，禁用 = 设置标志位。
 * @note 仅影响绘图创建后的首次渲染
 * \endif
 */

/**
 * \if ENGLISH
 * @fn void QImPlotAxisInfo::setInitialFitEnabled(bool enabled)
 * @brief Enables/disables automatic fitting on first render frame
 * @param enabled true to fit to data on first frame, false to use setLimits() immediately
 * @note Controls ImPlotAxisFlags_NoInitialFit flag with inverted logic:
 *       enabled → clear flag, disabled → set flag.
 * @note Only affects the very first render after plot creation; ignored thereafter
 * @see setAutoFit()
 * \endif
 *
 * \if CHINESE
 * @fn void QImPlotAxisInfo::setInitialFitEnabled(bool enabled)
 * @brief 启用/禁用首帧渲染时的自动适配
 * @param enabled true 首帧适配数据，false 立即使用 setLimits() 设置的范围
 * @note 控制 ImPlotAxisFlags_NoInitialFit 标志，逻辑反转：
 *       启用 → 清除标志位，禁用 → 设置标志位。
 * @note 仅影响绘图创建后的首次渲染；之后被忽略
 * @see setAutoFit()
 * \endif
 */

/**
 * \if ENGLISH
 * @fn bool QImPlotAxisInfo::isMenusEnabled() const
 * @brief Checks if context menu is accessible via right-click
 * @return true if menus are enabled (ImPlotAxisFlags_NoMenus NOT set)
 * @details Right-clicking axis opens menu with options: lock, invert, log scale, etc.
 *          Essential for user-driven axis configuration at runtime.
 * @note Corresponds to ImPlotAxisFlags_NoMenus flag with inverted logic:
 *       enabled = flag NOT set, disabled = flag set.
 * @note Disabling menus prevents all interactive axis configuration
 * \endif
 *
 * \if CHINESE
 * @fn bool QImPlotAxisInfo::isMenusEnabled() const
 * @brief 检查是否可通过右键单击访问上下文菜单
 * @return true 表示菜单启用（未设置 ImPlotAxisFlags_NoMenus）
 * @details 右键单击坐标轴会打开包含锁轴、反转、对数刻度等选项的菜单。
 *          对于运行时用户驱动的轴配置至关重要。
 * @note 对应 ImPlotAxisFlags_NoMenus 标志，逻辑反转：
 *       启用 = 未设置标志位，禁用 = 设置标志位。
 * @note 禁用菜单会阻止所有交互式轴配置
 * \endif
 */

/**
 * \if ENGLISH
 * @fn void QImPlotAxisInfo::setMenusEnabled(bool enabled)
 * @brief Enables/disables right-click context menu on axis
 * @param enabled true to allow right-click menu, false to disable all axis interactions
 * @note Controls ImPlotAxisFlags_NoMenus flag with inverted logic:
 *       enabled → clear flag, disabled → set flag.
 * @note Disabling menus also disables side-switching and lock toggling via UI
 * @see setSideSwitchEnabled()
 * \endif
 *
 * \if CHINESE
 * @fn void QImPlotAxisInfo::setMenusEnabled(bool enabled)
 * @brief 启用/禁用坐标轴上的右键上下文菜单
 * @param enabled true 允许右键菜单，false 禁用所有轴交互
 * @note 控制 ImPlotAxisFlags_NoMenus 标志，逻辑反转：
 *       启用 → 清除标志位，禁用 → 设置标志位。
 * @note 禁用菜单也会禁用通过 UI 切换轴侧和锁定切换
 * @see setSideSwitchEnabled()
 * \endif
 */

/**
 * \if ENGLISH
 * @fn bool QImPlotAxisInfo::isSideSwitchEnabled() const
 * @brief Checks if user can drag axis to opposite side
 * @return true if side-switching is enabled (ImPlotAxisFlags_NoSideSwitch NOT set)
 * @details Allows dragging X-axis from bottom to top, or Y-axis from left to right.
 *          Visual feedback shown during drag operation.
 * @note Corresponds to ImPlotAxisFlags_NoSideSwitch flag with inverted logic:
 *       enabled = flag NOT set, disabled = flag set.
 * @note Requires menus to be enabled (isMenusEnabled() must be true)
 * \endif
 *
 * \if CHINESE
 * @fn bool QImPlotAxisInfo::isSideSwitchEnabled() const
 * @brief 检查用户是否可通过拖拽将轴切换到对侧
 * @return true 表示启用轴侧切换（未设置 ImPlotAxisFlags_NoSideSwitch）
 * @details 允许将 X 轴从底部拖到顶部，或将 Y 轴从左侧拖到右侧。
 *          拖拽操作期间显示视觉反馈。
 * @note 对应 ImPlotAxisFlags_NoSideSwitch 标志，逻辑反转：
 *       启用 = 未设置标志位，禁用 = 设置标志位。
 * @note 需要启用菜单（isMenusEnabled() 必须为 true）
 * \endif
 */

/**
 * \if ENGLISH
 * @fn void QImPlotAxisInfo::setSideSwitchEnabled(bool enabled)
 * @brief Enables/disables dragging axis to opposite side
 * @param enabled true to allow side-switching via drag, false to lock axis position
 * @note Controls ImPlotAxisFlags_NoSideSwitch flag with inverted logic:
 *       enabled → clear flag, disabled → set flag.
 * @note Axis position can still be changed programmatically via isOpposite()
 * @see setOpposite()
 * \endif
 *
 * \if CHINESE
 * @fn void QImPlotAxisInfo::setSideSwitchEnabled(bool enabled)
 * @brief 启用/禁用通过拖拽切换轴到对侧
 * @param enabled true 允许通过拖拽切换轴侧，false 锁定轴位置
 * @note 控制 ImPlotAxisFlags_NoSideSwitch 标志，逻辑反转：
 *       启用 → 清除标志位，禁用 → 设置标志位。
 * @note 仍可通过 isOpposite() 以编程方式更改轴位置
 * @see setOpposite()
 * \endif
 */

/**
 * \if ENGLISH
 * @fn bool QImPlotAxisInfo::isHighlightEnabled() const
 * @brief Checks if axis background highlights on hover/interaction
 * @return true if highlighting is enabled (ImPlotAxisFlags_NoHighlight NOT set)
 * @details Visual feedback when hovering axis: background tint changes color.
 *          Helps users identify which axis they are interacting with.
 * @note Corresponds to ImPlotAxisFlags_NoHighlight flag with inverted logic:
 *       enabled = flag NOT set, disabled = flag set.
 * @note Purely visual effect; does not affect functionality
 * \endif
 *
 * \if CHINESE
 * @fn bool QImPlotAxisInfo::isHighlightEnabled() const
 * @brief 检查悬停/交互时坐标轴背景是否高亮
 * @return true 表示启用高亮（未设置 ImPlotAxisFlags_NoHighlight）
 * @details 悬停坐标轴时的视觉反馈：背景色调改变颜色。
 *          帮助用户识别正在交互的坐标轴。
 * @note 对应 ImPlotAxisFlags_NoHighlight 标志，逻辑反转：
 *       启用 = 未设置标志位，禁用 = 设置标志位。
 * @note 纯视觉效果；不影响功能
 * \endif
 */

/**
 * \if ENGLISH
 * @fn void QImPlotAxisInfo::setHighlightEnabled(bool enabled)
 * @brief Enables/disables visual highlighting on axis interaction
 * @param enabled true to show hover highlight, false for no visual feedback
 * @note Controls ImPlotAxisFlags_NoHighlight flag with inverted logic:
 *       enabled → clear flag, disabled → set flag.
 * @note Disabling improves performance slightly on very complex plots
 * \endif
 *
 * \if CHINESE
 * @fn void QImPlotAxisInfo::setHighlightEnabled(bool enabled)
 * @brief 启用/禁用坐标轴交互时的视觉高亮
 * @param enabled true 显示悬停高亮，false 无视觉反馈
 * @note 控制 ImPlotAxisFlags_NoHighlight 标志，逻辑反转：
 *       启用 → 清除标志位，禁用 → 设置标志位。
 * @note 禁用可在非常复杂的绘图上略微提升性能
 * \endif
 */

/**
 * \if ENGLISH
 * @fn bool QImPlotAxisInfo::isOpposite() const
 * @brief Checks if axis renders on opposite side (top/right instead of bottom/left)
 * @return true if opposite side rendering is enabled (ImPlotAxisFlags_Opposite set)
 * @details Default: X-axis at bottom, Y-axis at left.
 *          Opposite: X-axis at top, Y-axis at right.
 * @note Corresponds to ImPlotAxisFlags_Opposite flag (direct mapping, no inversion).
 * @note Commonly used for secondary Y-axes in multi-series plots
 * \endif
 *
 * \if CHINESE
 * @fn bool QImPlotAxisInfo::isOpposite() const
 * @brief 检查坐标轴是否渲染在对侧（顶部/右侧而非底部/左侧）
 * @return true 表示启用对侧渲染（设置了 ImPlotAxisFlags_Opposite）
 * @details 默认：X轴在底部，Y轴在左侧。
 *          对侧：X轴在顶部，Y轴在右侧。
 * @note 对应 ImPlotAxisFlags_Opposite 标志（直接映射，无逻辑反转）。
 * @note 常用于多序列绘图中的次级 Y 轴
 * \endif
 */

/**
 * \if ENGLISH
 * @fn void QImPlotAxisInfo::setOpposite(bool on)
 * @brief Enables/disables opposite-side axis rendering
 * @param on true to render axis on top (X) or right (Y), false for default side
 * @note Sets/clears ImPlotAxisFlags_Opposite flag (direct mapping).
 * @note Does not affect data rendering; only changes axis UI position
 * \endif
 *
 * \if CHINESE
 * @fn void QImPlotAxisInfo::setOpposite(bool on)
 * @brief 启用/禁用对侧坐标轴渲染
 * @param on true 将轴渲染在顶部（X）或右侧（Y），false 为默认侧
 * @note 设置/清除 ImPlotAxisFlags_Opposite 标志（直接映射）。
 * @note 不影响数据渲染；仅改变轴的 UI 位置
 * \endif
 */

/**
 * \if ENGLISH
 * @fn bool QImPlotAxisInfo::isForeground() const
 * @brief Checks if grid lines render in foreground (above data)
 * @return true if foreground rendering is enabled (ImPlotAxisFlags_Foreground set)
 * @details Default: grid lines render in background (behind data).
 *          Foreground: grid lines render on top of all plotted data.
 * @note Corresponds to ImPlotAxisFlags_Foreground flag (direct mapping, no inversion).
 * @note Useful when data obscures grid lines (e.g., dense scatter plots)
 * \endif
 *
 * \if CHINESE
 * @fn bool QImPlotAxisInfo::isForeground() const
 * @brief 检查网格线是否在前景渲染（数据上方）
 * @return true 表示启用前景渲染（设置了 ImPlotAxisFlags_Foreground）
 * @details 默认：网格线在背景渲染（数据后方）。
 *          前景：网格线在所有绘制数据上方渲染。
 * @note 对应 ImPlotAxisFlags_Foreground 标志（直接映射，无逻辑反转）。
 * @note 当数据遮挡网格线时很有用（例如密集散点图）
 * \endif
 */

/**
 * \if ENGLISH
 * @fn void QImPlotAxisInfo::setForeground(bool on)
 * @brief Enables/disables foreground grid line rendering
 * @param on true to render grid lines above data, false for background rendering
 * @note Sets/clears ImPlotAxisFlags_Foreground flag (direct mapping).
 * @note Affects visual layering only; does not change grid line visibility
 * @see setGridLinesEnabled()
 * \endif
 *
 * \if CHINESE
 * @fn void QImPlotAxisInfo::setForeground(bool on)
 * @brief 启用/禁用前景网格线渲染
 * @param on true 将网格线渲染在数据上方，false 为背景渲染
 * @note 设置/清除 ImPlotAxisFlags_Foreground 标志（直接映射）。
 * @note 仅影响视觉层级；不改变网格线可见性
 * @see setGridLinesEnabled()
 * \endif
 */

/**
 * \if ENGLISH
 * @fn bool QImPlotAxisInfo::isRangeFit() const
 * @brief Checks if axis only fits points within orthogonal axis range
 * @return true if range-fit is enabled (ImPlotAxisFlags_RangeFit set)
 * @details When enabled during zoom/pan: only considers data points visible on the
 *          perpendicular axis when auto-fitting this axis.
 *          Example: When zooming X-axis, only fits Y-axis to points visible in current X range.
 * @note Corresponds to ImPlotAxisFlags_RangeFit flag (direct mapping).
 * @note Requires isAutoFit() to be true to have effect
 * \endif
 *
 * \if CHINESE
 * @fn bool QImPlotAxisInfo::isRangeFit() const
 * @brief 检查坐标轴是否仅适配正交轴范围内的点
 * @return true 表示启用范围适配（设置了 ImPlotAxisFlags_RangeFit）
 * @details 启用后，在缩放/平移时：自动适配此轴时仅考虑在垂直轴上可见的数据点。
 *          示例：缩放 X 轴时，Y 轴仅适配当前 X 范围内可见的点。
 * @note 对应 ImPlotAxisFlags_RangeFit 标志（直接映射）。
 * @note 需要 isAutoFit() 为 true 才生效
 * \endif
 */

/**
 * \if ENGLISH
 * @fn void QImPlotAxisInfo::setRangeFit(bool on)
 * @brief Enables/disables range-constrained auto-fitting
 * @param on true to fit only to orthogonally visible data, false for full data set
 * @note Sets/clears ImPlotAxisFlags_RangeFit flag (direct mapping).
 * @note Only active when auto-fit is enabled (isAutoFit() == true)
 * @see setAutoFit()
 * \endif
 *
 * \if CHINESE
 * @fn void QImPlotAxisInfo::setRangeFit(bool on)
 * @brief 启用/禁用范围约束的自动适配
 * @param on true 仅适配正交可见数据，false 适配完整数据集
 * @note 设置/清除 ImPlotAxisFlags_RangeFit 标志（直接映射）。
 * @note 仅在启用自动适配时生效（isAutoFit() == true）
 * @see setAutoFit()
 * \endif
 */

/**
 * \if ENGLISH
 * @fn bool QImPlotAxisInfo::isPanStretch() const
 * @brief Checks if axis stretches during constrained panning
 * @return true if pan-stretch is enabled (ImPlotAxisFlags_PanStretch set)
 * @details When panning with locked/constrained axes: instead of clamping movement,
 *          the unlocked axis stretches its range to accommodate the pan operation.
 * @note Corresponds to ImPlotAxisFlags_PanStretch flag (direct mapping).
 * @note Primarily useful for specialized interaction scenarios
 * \endif
 *
 * \if CHINESE
 * @fn bool QImPlotAxisInfo::isPanStretch() const
 * @brief 检查约束平移时坐标轴是否拉伸
 * @return true 表示启用拉伸平移（设置了 ImPlotAxisFlags_PanStretch）
 * @details 平移时若轴被锁定/约束：不解锁移动，而是拉伸未锁定轴的范围以适应平移操作。
 * @note 对应 ImPlotAxisFlags_PanStretch 标志（直接映射）。
 * @note 主要用于特殊交互场景
 * \endif
 */

/**
 * \if ENGLISH
 * @fn void QImPlotAxisInfo::setPanStretch(bool on)
 * @brief Enables/disables axis stretching during constrained panning
 * @param on true to allow range stretching, false to clamp panning at limits
 * @note Sets/clears ImPlotAxisFlags_PanStretch flag (direct mapping).
 * @note Advanced feature; most applications should leave disabled (default)
 * \endif
 *
 * \if CHINESE
 * @fn void QImPlotAxisInfo::setPanStretch(bool on)
 * @brief 启用/禁用约束平移时的轴拉伸
 * @param on true 允许范围拉伸，false 在限制处钳制平移
 * @note 设置/清除 ImPlotAxisFlags_PanStretch 标志（直接映射）。
 * @note 高级功能；大多数应用应保持禁用（默认）
 * \endif
 */

/**
 * \if ENGLISH
 * @fn bool QImPlotAxisInfo::isLockMin() const
 * @brief Checks if axis minimum value is locked during interaction
 * @return true if min value is locked (ImPlotAxisFlags_LockMin set)
 * @details When locked: panning/zooming cannot change the minimum axis value.
 *          User can still change maximum value unless LockMax is also set.
 * @note Corresponds to ImPlotAxisFlags_LockMin flag (direct mapping).
 * @note Locking both min and max completely disables panning/zooming on this axis
 * \endif
 *
 * \if CHINESE
 * @fn bool QImPlotAxisInfo::isLockMin() const
 * @brief 检查交互时坐标轴最小值是否锁定
 * @return true 表示最小值锁定（设置了 ImPlotAxisFlags_LockMin）
 * @details 锁定后：平移/缩放无法改变最小轴值。
 *          除非同时锁定最大值，否则用户仍可改变最大值。
 * @note 对应 ImPlotAxisFlags_LockMin 标志（直接映射）。
 * @note 同时锁定最小和最大值将完全禁用此轴的平移/缩放
 * \endif
 */

/**
 * \if ENGLISH
 * @fn void QImPlotAxisInfo::setLockMin(bool on)
 * @brief Locks/unlocks the minimum axis value during user interaction
 * @param on true to prevent min value changes during pan/zoom, false to allow changes
 * @note Sets/clears ImPlotAxisFlags_LockMin flag (direct mapping).
 * @note Locking min does NOT prevent programmatic changes via setLimits()
 * @see setLockMax(), setLock()
 * \endif
 *
 * \if CHINESE
 * @fn void QImPlotAxisInfo::setLockMin(bool on)
 * @brief 锁定/解锁用户交互时的最小轴值
 * @param on true 防止平移/缩放时改变最小值，false 允许改变
 * @note 设置/清除 ImPlotAxisFlags_LockMin 标志（直接映射）。
 * @note 锁定最小值不会阻止通过 setLimits() 的编程式更改
 * @see setLockMax(), setLock()
 * \endif
 */

/**
 * \if ENGLISH
 * @fn bool QImPlotAxisInfo::isLockMax() const
 * @brief Checks if axis maximum value is locked during interaction
 * @return true if max value is locked (ImPlotAxisFlags_LockMax set)
 * @details When locked: panning/zooming cannot change the maximum axis value.
 *          User can still change minimum value unless LockMin is also set.
 * @note Corresponds to ImPlotAxisFlags_LockMax flag (direct mapping).
 * @note Locking both min and max completely disables panning/zooming on this axis
 * \endif
 *
 * \if CHINESE
 * @fn bool QImPlotAxisInfo::isLockMax() const
 * @brief 检查交互时坐标轴最大值是否锁定
 * @return true 表示最大值锁定（设置了 ImPlotAxisFlags_LockMax）
 * @details 锁定后：平移/缩放无法改变最大轴值。
 *          除非同时锁定最小值，否则用户仍可改变最小值。
 * @note 对应 ImPlotAxisFlags_LockMax 标志（直接映射）。
 * @note 同时锁定最小和最大值将完全禁用此轴的平移/缩放
 * \endif
 */

/**
 * \if ENGLISH
 * @fn void QImPlotAxisInfo::setLockMax(bool on)
 * @brief Locks/unlocks the maximum axis value during user interaction
 * @param on true to prevent max value changes during pan/zoom, false to allow changes
 * @note Sets/clears ImPlotAxisFlags_LockMax flag (direct mapping).
 * @note Locking max does NOT prevent programmatic changes via setLimits()
 * @see setLockMin(), setLock()
 * \endif
 *
 * \if CHINESE
 * @fn void QImPlotAxisInfo::setLockMax(bool on)
 * @brief 锁定/解锁用户交互时的最大轴值
 * @param on true 防止平移/缩放时改变最大值，false 允许改变
 * @note 设置/清除 ImPlotAxisFlags_LockMax 标志（直接映射）。
 * @note 锁定最大值不会阻止通过 setLimits() 的编程式更改
 * @see setLockMin(), setLock()
 * \endif
 */


// 辅助宏：处理肯定语义标志（无需反转）
#ifndef QImPlotAxisInfo_FLAG_ACCESSOR
#define QImPlotAxisInfo_FLAG_ACCESSOR(FlagName, FlagEnum)                                                              \
    bool QImPlotAxisInfo::is##FlagName() const                                                                         \
    {                                                                                                                  \
        QIM_DC(d);                                                                                                     \
        return (d->flags & FlagEnum) != 0;                                                                             \
    }                                                                                                                  \
    void QImPlotAxisInfo::set##FlagName(bool on)                                                                       \
    {                                                                                                                  \
        QIM_D(d);                                                                                                      \
        const ImPlotAxisFlags oldFlags = d->flags;                                                                     \
        if (on)                                                                                                        \
            d->flags |= FlagEnum;                                                                                      \
        else                                                                                                           \
            d->flags &= ~FlagEnum;                                                                                     \
        if (d->flags != oldFlags)                                                                                      \
            Q_EMIT axisFlagChanged();                                                                                  \
    }
#endif

// 辅助宏：处理否定语义标志（需要反转逻辑：enabled = !hasFlag）
#ifndef QImPlotAxisInfo_ENABLED_ACCESSOR
#define QImPlotAxisInfo_ENABLED_ACCESSOR(PropName, FlagEnum)                                                           \
    bool QImPlotAxisInfo::is##PropName() const                                                                         \
    {                                                                                                                  \
        QIM_DC(d);                                                                                                     \
        return (d->flags & FlagEnum) == 0; /* enabled = 无标志位 */                                                \
    }                                                                                                                  \
    void QImPlotAxisInfo::set##PropName(bool enabled)                                                                  \
    {                                                                                                                  \
        QIM_D(d);                                                                                                      \
        const ImPlotAxisFlags oldFlags = d->flags;                                                                     \
        if (enabled)                                                                                                   \
            d->flags &= ~FlagEnum; /* 启用 = 清除标志位 */                                                      \
        else                                                                                                           \
            d->flags |= FlagEnum; /* 禁用 = 设置标志位 */                                                       \
        if (d->flags != oldFlags)                                                                                      \
            Q_EMIT axisFlagChanged();                                                                                  \
    }
#endif


// clang-format off
// ===== 肯定语义标志（直接映射）=====
QImPlotAxisInfo_FLAG_ACCESSOR(AutoFit, ImPlotAxisFlags_AutoFit)
QImPlotAxisInfo_FLAG_ACCESSOR(Inverted, ImPlotAxisFlags_Invert)
QImPlotAxisInfo_FLAG_ACCESSOR(Opposite, ImPlotAxisFlags_Opposite)
QImPlotAxisInfo_FLAG_ACCESSOR(Foreground, ImPlotAxisFlags_Foreground)
QImPlotAxisInfo_FLAG_ACCESSOR(RangeFit, ImPlotAxisFlags_RangeFit)
QImPlotAxisInfo_FLAG_ACCESSOR(PanStretch, ImPlotAxisFlags_PanStretch)
QImPlotAxisInfo_FLAG_ACCESSOR(LockMin, ImPlotAxisFlags_LockMin)
QImPlotAxisInfo_FLAG_ACCESSOR(LockMax, ImPlotAxisFlags_LockMax)

// ===== 否定语义标志（反转逻辑：enabled = !hasFlag）=====
QImPlotAxisInfo_ENABLED_ACCESSOR(LabelEnabled, ImPlotAxisFlags_NoLabel)
QImPlotAxisInfo_ENABLED_ACCESSOR(GridLinesEnabled, ImPlotAxisFlags_NoGridLines)
QImPlotAxisInfo_ENABLED_ACCESSOR(TickMarksEnabled, ImPlotAxisFlags_NoTickMarks)
QImPlotAxisInfo_ENABLED_ACCESSOR(TickLabelsEnabled, ImPlotAxisFlags_NoTickLabels)
QImPlotAxisInfo_ENABLED_ACCESSOR(InitialFitEnabled, ImPlotAxisFlags_NoInitialFit)
QImPlotAxisInfo_ENABLED_ACCESSOR(MenusEnabled, ImPlotAxisFlags_NoMenus)
QImPlotAxisInfo_ENABLED_ACCESSOR(SideSwitchEnabled, ImPlotAxisFlags_NoSideSwitch)
QImPlotAxisInfo_ENABLED_ACCESSOR(HighlightEnabled, ImPlotAxisFlags_NoHighlight)

    // clang-format on

    // clang-format off
    // clang-format on


    // ===== 组合标志：Lock (LockMin | LockMax) =====


    /**
     * \if ENGLISH
     * @brief Checks if both min and max values are locked
     * @return true if both LockMin and LockMax flags are set (ImPlotAxisFlags_Lock)
     * @details When locked: axis range becomes completely fixed; no panning or zooming possible.
     *          Equivalent to (isLockMin() && isLockMax()).
     * @note Corresponds to ImPlotAxisFlags_Lock flag (combination of LockMin | LockMax).
     * @note Programmatic changes via setLimits() still work when locked
     * \endif
     *
     * \if CHINESE
     * @brief 检查最小和最大值是否同时锁定
     * @return true 表示同时设置了 LockMin 和 LockMax 标志（ImPlotAxisFlags_Lock）
     * @details 锁定后：轴范围完全固定；无法进行平移或缩放。
     *          等效于 (isLockMin() && isLockMax())。
     * @note 对应 ImPlotAxisFlags_Lock 标志（LockMin 与 LockMax 的组合）。
     * @note 锁定时仍可通过 setLimits() 进行编程式更改
     * \endif
     */

    bool QImPlotAxisInfo::isLock() const
{
    QIM_DC(d);
    return (d->flags & ImPlotAxisFlags_Lock) == ImPlotAxisFlags_Lock;
}

/**
 * \if ENGLISH
 * @brief Locks/unlocks both min and max axis values simultaneously
 * @param on true to completely lock axis range, false to unlock both ends
 * @details Convenience method equivalent to:
 *          setLockMin(on); setLockMax(on);
 * @note Sets/clears ImPlotAxisFlags_Lock flag (combination flag).
 * @note Does not affect programmatic limit changes via setLimits()
 * \endif
 *
 * \if CHINESE
 * @brief 同时锁定/解锁最小和最大轴值
 * @param on true 完全锁定轴范围，false 解锁两端
 * @details 便捷方法，等效于：
 *          setLockMin(on); setLockMax(on);
 * @note 设置/清除 ImPlotAxisFlags_Lock 标志（组合标志）。
 * @note 不影响通过 setLimits() 的编程式范围更改
 * \endif
 */

void QImPlotAxisInfo::setLock(bool on)
{
    QIM_D(d);
    const ImPlotAxisFlags oldFlags = d->flags;
    if (on)
        d->flags |= ImPlotAxisFlags_Lock;
    else
        d->flags &= ~ImPlotAxisFlags_Lock;
    if (d->flags != oldFlags)
        Q_EMIT axisFlagChanged();
}

// ===== 组合标志：Decorations (NoLabel|NoGridLines|NoTickMarks|NoTickLabels) =====
/**
 * \if ENGLISH
 * @brief Checks if all decorative elements are visible (label, grid, ticks, tick labels)
 * @return true if all decorations is hide (ImPlotAxisFlags_NoDecorations set)
 * @note Corresponds to ImPlotAxisFlags_NoDecorations flag (combination of NoLabel|NoGridLines|NoTickMarks|NoTickLabels)
 * \endif
 *
 * \if CHINESE
 * @brief 检查所有装饰元素是否可见（标签、网格、刻度线、刻度标签）
 * @return true 表示所有装饰隐藏（设置 ImPlotAxisFlags_NoDecorations）
 * @note 对应 ImPlotAxisFlags_NoDecorations 标志（NoLabel|NoGridLines|NoTickMarks|NoTickLabels 的组合），
 * \endif
 */
bool QImPlotAxisInfo::isNoDecorations() const
{
    QIM_DC(d);
    return (d->flags & ImPlotAxisFlags_NoDecorations) != 0;
}

/**
 * \if ENGLISH
 * @brief Enables/disables no decorative elements
 * @param enabled true to hide all (label/grid/ticks/labels)
 * @details Convenience method equivalent to:
 *          setLabelEnabled(!enabled);
 *          setGridLinesEnabled(!enabled);
 *          setTickMarksEnabled(!enabled);
 *          setTickLabelsEnabled(!enabled);
 * \endif
 *
 * \if CHINESE
 * @brief 同时启用/禁用所有装饰元素
 * @param enabled true 隐藏所有（标签/网格/刻度线/标签）
 * @details 便捷方法，等效于：
 *          setLabelEnabled(!enabled);
 *          setGridLinesEnabled(!enabled);
 *          setTickMarksEnabled(!enabled);
 *          setTickLabelsEnabled(!enabled);
 * \endif
 */
void QImPlotAxisInfo::setNoDecorations(bool enabled)
{
    QIM_D(d);
    const ImPlotAxisFlags oldFlags = d->flags;
    if (enabled) {
        d->flags |= ImPlotAxisFlags_NoDecorations;  // 启用 = 设置所有装饰禁用标志
    } else {
        d->flags &= ~ImPlotAxisFlags_NoDecorations;  // 启用 = 清除所有装饰禁用标志
    }
    if (d->flags != oldFlags) {
        Q_EMIT axisFlagChanged();
    }
}

/**
 * @brief 获取ImPlotAxisFlags值，作为int返回
 * @return ImPlotAxisFlags值，作为int返回
 */
int QImPlotAxisInfo::axisFlags() const
{
    return d_ptr->flags;
}

/**
 * @brief 设置ImPlotAxisFlags值
 * @param flags
 */
void QImPlotAxisInfo::setAxisFlags(int flags)
{
    if (d_ptr->flags != flags) {
        d_ptr->flags = flags;
        Q_EMIT axisFlagChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Gets the current axis scale type (linear, logarithmic, time, etc.)
 * @return Current scale type as QImPlotScaleType enum value
 * @details Returns the scale type configured for this axis:
 *          - Linear: Standard linear scale (default)
 *          - Time:  Time-based scale with automatic formatting (requires ImPlot::SetTimeFormat())
 *          - Log10: Base-10 logarithmic scale (values must be > 0)
 *          - SymLog: Symmetric logarithmic scale (handles negative values)
 * @note This is a convenience wrapper around imPlotScaleToPlotScaleType(d_ptr->scale).
 *       The underlying storage is an integer matching ImPlotScale_ enumeration values.
 * @see setScaleType(), imPlotScale()
 * \endif
 *
 * \if CHINESE
 * @brief 获取当前坐标轴刻度类型（线性、对数、时间等）
 * @return 当前刻度类型，以 QImPlotScaleType 枚举值返回
 * @details 返回为此轴配置的刻度类型：
 *          - Linear: 标准线性刻度（默认）
 *          - Time:  基于时间的刻度，带自动格式化（需配合 ImPlot::SetTimeFormat() 使用）
 *          - Log10: 以 10 为底的对数刻度（数值必须 > 0）
 *          - SymLog: 对称对数刻度（可处理负值）
 * @note 此函数是 imPlotScaleToPlotScaleType(d_ptr->scale) 的便捷包装。
 *       底层存储为与 ImPlotScale_ 枚举值匹配的整数。
 * @see setScaleType(), imPlotScale()
 * \endif
 */
QImPlotScaleType QImPlotAxisInfo::scaleType() const
{
    return toQImPlotScaleType(d_ptr->scale.value());
}

/**
 * \if ENGLISH
 * @brief Sets the axis scale type (linear, logarithmic, time, etc.)
 * @param t New scale type as QImPlotScaleType enum value
 * @details Configures how axis values are transformed and displayed:
 *          - Linear: Standard linear mapping (y = x)
 *          - Time:  Values interpreted as Unix timestamps, formatted as dates/times
 *          - Log10: Logarithmic mapping (y = log10(x)), requires positive values
 *          - SymLog: Hybrid mapping (linear near zero, logarithmic elsewhere)
 * @note This method updates the internal scale storage and emits scaleTypeChanged() signal.
 *       Actual scale change takes effect when SetupAxisScale() is called during plot setup.
 * @warning Logarithmic scales require all data values to be strictly positive.
 *          SymLog scale requires ImPlot v0.14+; earlier versions fall back to Linear.
 * @see scaleType(), ImPlot::SetupAxisScale()
 * \endif
 *
 * \if CHINESE
 * @brief 设置坐标轴刻度类型（线性、对数、时间等）
 * @param t 新的刻度类型，以 QImPlotScaleType 枚举值指定
 * @details 配置坐标轴值的变换和显示方式：
 *          - Linear: 标准线性映射（y = x）
 *          - Time:  值被解释为 Unix 时间戳，格式化为日期/时间
 *          - Log10: 对数映射（y = log10(x)），要求值严格为正
 *          - SymLog: 混合映射（零点附近线性，其他区域对数）
 * @note 此方法更新内部刻度存储并触发 scaleTypeChanged() 信号。
 *       实际刻度变更需在绘图设置阶段调用 SetupAxisScale() 后生效。
 * @warning 对数刻度要求所有数据值严格为正。
 *          SymLog 刻度需要 ImPlot v0.14+；早期版本会回退到 Linear。
 * @see scaleType(), ImPlot::SetupAxisScale()
 * \endif
 */
void QImPlotAxisInfo::setScaleType(QImPlotScaleType t)
{
    d_ptr->scale = toImPlotScale(t);
}

/**
 * \if ENGLISH
 * @brief Returns the raw ImPlotScale_ enumeration value for direct ImPlot API usage
 * @return Current scale type as integer matching ImPlotScale_ enumeration values
 * @details Provides direct access to the underlying scale type storage.
 *          The integer value can be passed directly to ImPlot::SetupAxisScale():
 * @note Although return type is int (for Qt property compatibility), the value represents
 *       an ImPlotScale_ enumeration constant. Valid values: 0=Linear, 1=Time, 2=Log10, 3=SymLog.
 * @see scaleType(), plotScaleTypeToImPlotScale()
 * \endif
 *
 * \if CHINESE
 * @brief 返回原始 ImPlotScale_ 枚举值，用于直接调用 ImPlot API
 * @return 当前刻度类型，整数值匹配 ImPlotScale_ 枚举
 * @details 提供对底层刻度类型存储的直接访问。
 *          该整数值可直接传递给 ImPlot::SetupAxisScale()：
 * @note 虽然返回类型为 int（为兼容 Qt 属性系统），但该值实际表示 ImPlotScale_ 枚举常量。
 *       有效值：0=Linear, 1=Time, 2=Log10, 3=SymLog。
 * @see scaleType(), plotScaleTypeToImPlotScale()
 * \endif
 */
int QImPlotAxisInfo::imPlotScale() const
{
    return d_ptr->scale.value();
}

bool QImPlotAxisInfo::isEnabled() const
{
    return d_ptr->enable;
}

void QImPlotAxisInfo::setEnabled(bool on)
{
    QIM_D(d);
    d->enable = on;
    if (d->axisId == ImAxis_::ImAxis_X1 || d->axisId == ImAxis_::ImAxis_Y1) {
        if (on) {
            setAxisFlags(d->tempFlags);
        } else {
            d->tempFlags = d->flags;
            setNoDecorations(true);
        }
    }
}

/**
 * @brief QImPlotAxisInfo::render
 */
void QImPlotAxisInfo::render()
{
    QIM_D(d);
    // x1和y1不受enable影响
    if (d->axisId != ImAxis_::ImAxis_X1 && d->axisId != ImAxis_::ImAxis_Y1) {
        if (!isEnabled()) {
            return;
        }
    }
    const char* label = (d->labelUtf8->isEmpty() ? nullptr : d->labelUtf8->constData());
    ImPlot::SetupAxis(d->axisId, label, d->flags);
    if (d->minLimits.is_dirty() || d->maxLimits.is_dirty()) {
        ImPlot::SetupAxisLimits(d->axisId, d->minLimits.value(), d->maxLimits.value(), d->limitCond);
    }
    if (d->scale.is_dirty()) {
        ImPlot::SetupAxisScale(d->axisId, d->scale.value());
    }
}

}
