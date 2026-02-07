#include "QImPlotTheme.h"
#include "implot.h"
#include "QtImGuiUtils.h"
namespace QIM
{

class QImPlotTheme::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlotTheme)
public:
    PrivateData(QImPlotTheme* p);
    void copyColorsFrom(const QImPlotTheme& other)
    {
        // Copy all 13 non-AUTO color components (POD types - no exception risk)
        m_frameBg      = other.d_ptr->m_frameBg;
        m_plotBg       = other.d_ptr->m_plotBg;
        m_plotBorder   = other.d_ptr->m_plotBorder;
        m_legendBg     = other.d_ptr->m_legendBg;
        m_legendBorder = other.d_ptr->m_legendBorder;
        m_legendText   = other.d_ptr->m_legendText;
        m_titleText    = other.d_ptr->m_titleText;
        m_inlayText    = other.d_ptr->m_inlayText;
        m_axisText     = other.d_ptr->m_axisText;
        m_axisGrid     = other.d_ptr->m_axisGrid;
        m_axisTick     = other.d_ptr->m_axisTick;
        m_selection    = other.d_ptr->m_selection;
        m_crosshairs   = other.d_ptr->m_crosshairs;
    }
    // Internal color storage (only for non-AUTO colors)
    ImVec4 m_frameBg;
    ImVec4 m_plotBg;
    ImVec4 m_plotBorder;
    ImVec4 m_legendBg;
    ImVec4 m_legendBorder;
    ImVec4 m_legendText;
    ImVec4 m_titleText;
    ImVec4 m_inlayText;
    ImVec4 m_axisText;
    ImVec4 m_axisGrid;
    ImVec4 m_axisTick;
    ImVec4 m_selection;
    ImVec4 m_crosshairs;
};

QImPlotTheme::PrivateData::PrivateData(QImPlotTheme* p)
    : q_ptr(p)
    , m_frameBg(1.00f, 1.00f, 1.00f, 1.00f)
    , m_plotBg(0.42f, 0.57f, 1.00f, 0.13f)
    , m_plotBorder(0.00f, 0.00f, 0.00f, 0.00f)
    , m_legendBg(1.00f, 1.00f, 1.00f, 0.98f)
    , m_legendBorder(0.82f, 0.82f, 0.82f, 0.80f)
    , m_legendText(0.00f, 0.00f, 0.00f, 1.00f)
    , m_titleText(0.00f, 0.00f, 0.00f, 1.00f)
    , m_inlayText(0.00f, 0.00f, 0.00f, 1.00f)
    , m_axisText(0.00f, 0.00f, 0.00f, 1.00f)
    , m_axisGrid(1.00f, 1.00f, 1.00f, 1.00f)
    , m_axisTick(0.00f, 0.00f, 0.00f, 0.25f)
    , m_selection(0.82f, 0.64f, 0.03f, 1.00f)
    , m_crosshairs(0.00f, 0.00f, 0.00f, 0.50f)
{
}
//----------------------------------------------------
// QImFigureTheme
//----------------------------------------------------
QImPlotTheme::QImPlotTheme() : QIM_PIMPL_CONSTRUCT
{
}

/**
 * \if ENGLISH
 * @brief Copy constructor - performs deep copy of theme colors
 * @param other Source theme to copy from
 * @note All color components are duplicated; q_ptr in PrivateData is rebound to new instance
 * \endif
 *
 * \if CHINESE
 * @brief 拷贝构造函数 - 执行主题颜色的深拷贝
 * @param other 源主题对象
 * @note 所有颜色分量均被复制；PrivateData 中的 q_ptr 会重新绑定到新实例
 * \endif
 */
QImPlotTheme::QImPlotTheme(const QImPlotTheme& other)
{
    // Deep copy all color components from source
    d_ptr->copyColorsFrom(other);
}

/**
 * \if ENGLISH
 * @brief Move constructor - transfers ownership of theme data with zero-copy semantics
 * @param other Rvalue reference to source theme
 * @note Invalidates source object (leaves it in valid but unspecified state)
 * \endif
 *
 * \if CHINESE
 * @brief 移动构造函数 - 以零拷贝语义转移主题数据所有权
 * @param other 源主题的右值引用
 * @note 使源对象失效（处于合法但未指定状态）
 * \endif
 */
QImPlotTheme::QImPlotTheme(QImPlotTheme&& other) noexcept : d_ptr(std::move(other.d_ptr))
{
    // Rebind q_ptr in moved PrivateData to new owner
    if (d_ptr) {
        d_ptr->q_ptr = this;
    }
}

QImPlotTheme::~QImPlotTheme()
{
}

// ===== Frame Colors =====
/**
 * \if ENGLISH
 * @brief Set plot frame background color
 * @param c QColor value (RGBA components mapped to [0.0, 1.0])
 * \endif
 *
 * \if CHINESE
 * @brief 设置图表框架背景颜色
 * @param c QColor 值（RGBA 分量映射到 [0.0, 1.0] 范围）
 * \endif
 */
void QImPlotTheme::setFrameBackgroundColor(const QColor& c)
{
    d_ptr->m_frameBg = QIM::toImVec4(c);
}

/**
 * \if ENGLISH
 * @brief Get current plot frame background color
 * @return Current background color as QColor
 * \endif
 *
 * \if CHINESE
 * @brief 获取当前图表框架背景颜色
 * @return 当前背景颜色（QColor 格式）
 * \endif
 */
QColor QImPlotTheme::frameBackgroundColor() const
{
    return QIM::toQColor(d_ptr->m_frameBg);
}

// ===== Plot Area Colors =====
/**
 * \if ENGLISH
 * @brief Set plot canvas background color (with transparency support)
 * @param c QColor value with alpha channel
 * \endif
 *
 * \if CHINESE
 * @brief 设置绘图区域背景颜色（支持透明度）
 * @param c 带 Alpha 通道的 QColor 值
 * \endif
 */
void QImPlotTheme::setPlotBackgroundColor(const QColor& c)
{
    d_ptr->m_plotBg = QIM::toImVec4(c);
}
QColor QImPlotTheme::plotBackgroundColor() const
{
    return QIM::toQColor(d_ptr->m_plotBg);
}

/**
 * \if ENGLISH
 * @brief Set plot canvas border color
 * @param c Border color (fully transparent by default)
 * \endif
 *
 * \if CHINESE
 * @brief 设置绘图区域边框颜色
 * @param c 边框颜色（默认完全透明）
 * \endif
 */
void QImPlotTheme::setPlotBorderColor(const QColor& c)
{
    d_ptr->m_plotBorder = QIM::toImVec4(c);
}
QColor QImPlotTheme::plotBorderColor() const
{
    return QIM::toQColor(d_ptr->m_plotBorder);
}

// ===== Legend Colors =====
/**
 * \if ENGLISH
 * @brief Set legend background color
 * @param c Background color with optional transparency
 * \endif
 *
 * \if CHINESE
 * @brief 设置图例背景颜色
 * @param c 可选透明度的背景颜色
 * \endif
 */
void QImPlotTheme::setLegendBackgroundColor(const QColor& c)
{
    d_ptr->m_legendBg = QIM::toImVec4(c);
}
QColor QImPlotTheme::legendBackgroundColor() const
{
    return QIM::toQColor(d_ptr->m_legendBg);
}

/**
 * \if ENGLISH
 * @brief Set legend border color
 * @param c Border color for legend box
 * \endif
 *
 * \if CHINESE
 * @brief 设置图例边框颜色
 * @param c 图例框的边框颜色
 * \endif
 */
void QImPlotTheme::setLegendBorderColor(const QColor& c)
{
    d_ptr->m_legendBorder = QIM::toImVec4(c);
}
QColor QImPlotTheme::legendBorderColor() const
{
    return QIM::toQColor(d_ptr->m_legendBorder);
}

/**
 * \if ENGLISH
 * @brief Set legend text color
 * @param c Text color for legend items
 * \endif
 *
 * \if CHINESE
 * @brief 设置图例文本颜色
 * @param c 图例项的文本颜色
 * \endif
 */
void QImPlotTheme::setLegendTextColor(const QColor& c)
{
    d_ptr->m_legendText = QIM::toImVec4(c);
}
QColor QImPlotTheme::legendTextColor() const
{
    return QIM::toQColor(d_ptr->m_legendText);
}

// ===== Text Colors =====
/**
 * \if ENGLISH
 * @brief Set plot title text color
 * @param c Color for main plot title
 * \endif
 *
 * \if CHINESE
 * @brief 设置图表标题文本颜色
 * @param c 主标题文本颜色
 * \endif
 */
void QImPlotTheme::setTitleTextColor(const QColor& c)
{
    d_ptr->m_titleText = QIM::toImVec4(c);
}
QColor QImPlotTheme::titleTextColor() const
{
    return QIM::toQColor(d_ptr->m_titleText);
}

/**
 * \if ENGLISH
 * @brief Set inlay text color (annotations/labels inside plot area)
 * @param c Color for embedded text elements
 * \endif
 *
 * \if CHINESE
 * @brief 设置内嵌文本颜色（绘图区域内的注释/标签）
 * @param c 嵌入式文本元素的颜色
 * \endif
 */
void QImPlotTheme::setInlayTextColor(const QColor& c)
{
    d_ptr->m_inlayText = QIM::toImVec4(c);
}
QColor QImPlotTheme::inlayTextColor() const
{
    return QIM::toQColor(d_ptr->m_inlayText);
}

/**
 * \if ENGLISH
 * @brief Set axis label text color
 * @param c Color for X/Y axis labels and values
 * \endif
 *
 * \if CHINESE
 * @brief 设置坐标轴标签文本颜色
 * @param c X/Y 轴标签和刻度值的颜色
 * \endif
 */
void QImPlotTheme::setAxisTextColor(const QColor& c)
{
    d_ptr->m_axisText = QIM::toImVec4(c);
}
QColor QImPlotTheme::axisTextColor() const
{
    return QIM::toQColor(d_ptr->m_axisText);
}

// ===== Grid & Ticks =====
/**
 * \if ENGLISH
 * @brief Set major grid line color
 * @param c Color for primary grid lines (X/Y axes)
 * \endif
 *
 * \if CHINESE
 * @brief 设置主网格线颜色
 * @param c 主网格线（X/Y 轴）的颜色
 * \endif
 */
void QImPlotTheme::setAxisGridColor(const QColor& c)
{
    d_ptr->m_axisGrid = QIM::toImVec4(c);
}
QColor QImPlotTheme::axisGridColor() const
{
    return QIM::toQColor(d_ptr->m_axisGrid);
}

/**
 * \if ENGLISH
 * @brief Set axis tick mark color
 * @param c Color for small tick marks on axes (typically semi-transparent)
 * \endif
 *
 * \if CHINESE
 * @brief 设置坐标轴刻度标记颜色
 * @param c 坐标轴上小刻度标记的颜色（通常为半透明）
 * \endif
 */
void QImPlotTheme::setAxisTickColor(const QColor& c)
{
    d_ptr->m_axisTick = QIM::toImVec4(c);
}
QColor QImPlotTheme::axisTickColor() const
{
    return QIM::toQColor(d_ptr->m_axisTick);
}

// ===== Interactive Elements =====
/**
 * \if ENGLISH
 * @brief Set selection rectangle color
 * @param c Color for rubber-band selection area
 * \endif
 *
 * \if CHINESE
 * @brief 设置选择矩形颜色
 * @param c 框选区域（rubber-band）的颜色
 * \endif
 */
void QImPlotTheme::setSelectionColor(const QColor& c)
{
    d_ptr->m_selection = QIM::toImVec4(c);
}
QColor QImPlotTheme::selectionColor() const
{
    return QIM::toQColor(d_ptr->m_selection);
}

/**
 * \if ENGLISH
 * @brief Set crosshairs cursor color
 * @param c Color for crosshair lines during mouse hover
 * \endif
 *
 * \if CHINESE
 * @brief 设置十字线光标颜色
 * @param c 鼠标悬停时十字线的颜色
 * \endif
 */
void QImPlotTheme::setCrosshairsColor(const QColor& c)
{
    d_ptr->m_crosshairs = QIM::toImVec4(c);
}
QColor QImPlotTheme::crosshairsColor() const
{
    return QIM::toQColor(d_ptr->m_crosshairs);
}

// ===== Comparison Operators =====
/**
 * \if ENGLISH
 * @brief Equality comparison operator for theme objects
 * @param other Another QImPlotTheme instance to compare with
 * @return true if all color components match within floating-point tolerance (1e-5), false otherwise
 * @note Uses epsilon-based comparison for robust floating-point equality checks
 * \endif
 *
 * \if CHINESE
 * @brief 主题对象的相等比较操作符
 * @param other 用于比较的另一个 QImPlotTheme 实例
 * @return 若所有颜色分量在浮点容差范围内（1e-5）匹配则返回 true，否则返回 false
 * @note 采用基于 epsilon 的比较方式确保浮点数相等性检查的健壮性
 * \endif
 */
bool QImPlotTheme::operator==(const QImPlotTheme& other) const
{
    // Self-comparison shortcut
    if (this == &other) {
        return true;
    }

    // Compare all 13 non-AUTO color components with epsilon tolerance
    return QIM::fuzzyEqual(d_ptr->m_frameBg, other.d_ptr->m_frameBg)
           && QIM::fuzzyEqual(d_ptr->m_plotBg, other.d_ptr->m_plotBg)
           && QIM::fuzzyEqual(d_ptr->m_plotBorder, other.d_ptr->m_plotBorder)
           && QIM::fuzzyEqual(d_ptr->m_legendBg, other.d_ptr->m_legendBg)
           && QIM::fuzzyEqual(d_ptr->m_legendBorder, other.d_ptr->m_legendBorder)
           && QIM::fuzzyEqual(d_ptr->m_legendText, other.d_ptr->m_legendText)
           && QIM::fuzzyEqual(d_ptr->m_titleText, other.d_ptr->m_titleText)
           && QIM::fuzzyEqual(d_ptr->m_inlayText, other.d_ptr->m_inlayText)
           && QIM::fuzzyEqual(d_ptr->m_axisText, other.d_ptr->m_axisText)
           && QIM::fuzzyEqual(d_ptr->m_axisGrid, other.d_ptr->m_axisGrid)
           && QIM::fuzzyEqual(d_ptr->m_axisTick, other.d_ptr->m_axisTick)
           && QIM::fuzzyEqual(d_ptr->m_selection, other.d_ptr->m_selection)
           && QIM::fuzzyEqual(d_ptr->m_crosshairs, other.d_ptr->m_crosshairs);
}

/**
 * \if ENGLISH
 * @brief Inequality comparison operator (negation of operator==)
 * @param other Another QImPlotTheme instance to compare with
 * @return true if themes differ, false if identical
 * \endif
 *
 * \if CHINESE
 * @brief 不等比较操作符（operator== 的取反）
 * @param other 用于比较的另一个 QImPlotTheme 实例
 * @return 若主题不同则返回 true，相同则返回 false
 * \endif
 */
bool QImPlotTheme::operator!=(const QImPlotTheme& other) const
{
    return !(*this == other);
}

/**
 * \if ENGLISH
 * @brief Copy assignment operator - replaces current theme with deep copy of other
 * @param other Source theme to copy from
 * @return Reference to this object after assignment
 * @note Self-assignment safe; preserves PIMPL encapsulation integrity
 * \endif
 *
 * \if CHINESE
 * @brief 拷贝赋值运算符 - 用 other 的深拷贝替换当前主题
 * @param other 源主题对象
 * @return 赋值后的当前对象引用
 * @note 自赋值安全；保持 PIMPL 封装完整性
 * \endif
 */
QImPlotTheme& QImPlotTheme::operator=(const QImPlotTheme& other)
{
    if (this != &other && other.d_ptr) {
        // Copy all color components (d_ptr already exists from construction)
        d_ptr->copyColorsFrom(other);
    }
    return *this;
}

/**
 * \if ENGLISH
 * @brief Move assignment operator - efficiently replaces current theme via resource transfer
 * @param other Rvalue reference to source theme
 * @return Reference to this object after move
 * @note Self-assignment safe; preferred for temporary objects/performance-critical paths
 * \endif
 *
 * \if CHINESE
 * @brief 移动赋值运算符 - 通过资源转移高效替换当前主题
 * @param other 源主题的右值引用
 * @return 移动后的当前对象引用
 * @note 自赋值安全；适用于临时对象/性能关键路径
 * \endif
 */
QImPlotTheme& QImPlotTheme::operator=(QImPlotTheme&& other) noexcept
{
    if (this != &other) {
        // Swap unique_ptr handles (efficient zero-copy transfer)
        d_ptr.swap(other.d_ptr);

        // Rebind q_ptr in both objects to maintain PIMPL integrity
        if (d_ptr)
            d_ptr->q_ptr = this;
        if (other.d_ptr)
            other.d_ptr->q_ptr = &other;
    }
    return *this;
}
// ===== Theme Application =====
/**
 * \if ENGLISH
 * @brief Apply custom theme colors to ImPlotStyle
 * @param style Pointer to ImPlotStyle structure to modify
 * @note Only modifies non-AUTO colors; preserves ImPlot's automatic coloring for lines/markers
 * \endif
 *
 * \if CHINESE
 * @brief 应用自定义主题颜色到 ImPlotStyle
 * @param style 指向待修改的 ImPlotStyle 结构的指针
 * @note 仅修改非 AUTO 颜色；保留 ImPlot 对线条/标记的自动配色
 * \endif
 */
void QImPlotTheme::apply(ImPlotStyle* style) const
{
    if (!style) {
        return;
    }
    QIM_DC(d);
    ImVec4* colors = style->Colors;

    colors[ ImPlotCol_FrameBg ]      = d->m_frameBg;
    colors[ ImPlotCol_PlotBg ]       = d->m_plotBg;
    colors[ ImPlotCol_PlotBorder ]   = d->m_plotBorder;
    colors[ ImPlotCol_LegendBg ]     = d->m_legendBg;
    colors[ ImPlotCol_LegendBorder ] = d->m_legendBorder;
    colors[ ImPlotCol_LegendText ]   = d->m_legendText;
    colors[ ImPlotCol_TitleText ]    = d->m_titleText;
    colors[ ImPlotCol_InlayText ]    = d->m_inlayText;
    colors[ ImPlotCol_AxisText ]     = d->m_axisText;
    colors[ ImPlotCol_AxisGrid ]     = d->m_axisGrid;
    colors[ ImPlotCol_AxisTick ]     = d->m_axisTick;
    colors[ ImPlotCol_Selection ]    = d->m_selection;
    colors[ ImPlotCol_Crosshairs ]   = d->m_crosshairs;

    // Note: AUTO colors (Line, Fill, Marker, etc.) are intentionally not set
    // to preserve ImPlot's automatic color behavior
}

/**
 * \if ENGLISH
 * @brief Apply built-in ImPlot theme preset
 * @param style Pointer to ImPlotStyle structure to modify
 * @param theme Theme preset to apply (Auto/Classic/Dark/Light)
 * @note This completely overwrites the style with ImPlot's default theme
 * \endif
 *
 * \if CHINESE
 * @brief 应用 ImPlot 内置主题预设
 * @param style 指向待修改的 ImPlotStyle 结构的指针
 * @param theme 要应用的主题预设（自动/经典/暗色/亮色）
 * @note 此操作将用 ImPlot 默认主题完全覆盖样式
 * \endif
 */
void QImPlotTheme::apply(ImPlotStyle* style, StyleColorTheme theme)
{
    switch (theme) {
    case StyleColorsAuto:
        ImPlot::StyleColorsAuto(style);
        break;
    case StyleColorsClassic:
        ImPlot::StyleColorsClassic(style);
        break;
    case StyleColorsDark:
        ImPlot::StyleColorsDark(style);
        break;
    case StyleColorsLight:
        ImPlot::StyleColorsLight(style);
        break;
    default:
        break;
    }
}

}  // end namespace QIM
