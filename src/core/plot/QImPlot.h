#ifndef QIMPLOT_H
#define QIMPLOT_H
#include <QMetaType>
#include "QImAPI.h"
namespace QIM
{
/**
 * \if ENGLISH
 * @brief Corresponds to ImAxis
 * \endif
 *
 * \if CHINESE
 * @brief 对应ImAxis
 * \endif
 */
enum class QImPlotAxisId
{
    X1 = 0,     ///< ImAxis_::ImAxis_X1
    X2,         ///< ImAxis_::ImAxis_X2
    X3,         ///< ImAxis_::ImAxis_X3
    Y1,         ///< ImAxis_::ImAxis_Y1
    Y2,         ///< ImAxis_::ImAxis_Y2
    Y3,         ///< ImAxis_::ImAxis_Y3
    AxisCount,  ///< ImAxis_::ImAxis_COUNT
    Auto        ///< Auto
};

/**
 * \if ENGLISH
 * @brief Qt-style wrapper enum for ImPlot scale types
 * @details Maps ImPlot's ImPlotScale enumeration to Qt-compatible enum class values.
 *          Defines axis scale transformation modes: Linear (default), Time (date/time axis),
 *          Log10 (base-10 logarithmic, requires positive values), and SymLog (symmetric
 *          logarithmic that handles negative values near zero).
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot刻度类型的Qt风格封装枚举
 * @details 将ImPlot的ImPlotScale枚举映射为Qt兼容的enum class值。
 *          定义坐标轴刻度变换模式：Linear（默认线性）、Time（日期/时间轴）、
 *          Log10（以10为底的对数，要求正值）和SymLog（对称对数，可处理零附近的负值）。
 * \endif
 */
enum class QImPlotScaleType
{
    Linear = 0,  ///< ImPlotScale_::ImPlotScale_Linear
    Time,        ///< ImPlotScale_::ImPlotScale_Time
    Log10,       ///< ImPlotScale_::ImPlotScale_Log10
    SymLog       ///< ImPlotScale_::ImPlotScale_SymLog
};

/**
 * \if ENGLISH
 * @brief Qt-style wrapper enum for ImPlot condition types
 * @details Maps ImPlot's ImPlotCond enumeration to Qt-compatible enum class values.
 *          Controls how axis limits and constraints are applied:
 *          - None: No constraint applied
 *          - Always: Constraint applied every frame
 *          - Once: Constraint applied only on the first frame
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot条件类型的Qt风格封装枚举
 * @details 将ImPlot的ImPlotCond枚举映射为Qt兼容的enum class值。
 *          控制坐标轴限制和约束的应用方式：
 *          - None：不应用约束
 *          - Always：每帧都应用约束
 *          - Once：仅在首帧应用约束
 * \endif
 */
enum class QImPlotCondition
{
    None = 0,  ///< ImPlotCond_::ImPlotCond_None
    Always,    ///< ImPlotCond_::ImPlotCond_Always
    Once       ///< ImPlotCond_::ImPlotCond_Once
};

/**
 * \if ENGLISH
 * @brief Qt-style wrapper enum for ImPlot legend location
 * @details Maps ImPlot's ImPlotLocation enumeration to Qt-compatible enum class values
 *          with bitwise flag semantics. Defines where the legend is positioned within
 *          the plot area: North/South/West/East edges, corner combinations (NorthWest,
 *          NorthEast, SouthWest, SouthEast), and Center placement.
 *          Values are bit flags that can be combined for custom positioning.
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot图例位置的Qt风格封装枚举
 * @details 将ImPlot的ImPlotLocation枚举映射为Qt兼容的enum class值，
 *          采用位标志语义。定义图例在绘图区域中的位置：
 *          North/South/West/East边缘、角落组合（NorthWest、NorthEast、
 *          SouthWest、SouthEast）和Center居中放置。
 *          值为位标志，可组合使用实现自定义定位。
 * \endif
 */
enum class QImPlotLegendLocation
{
    North     = 1 << 0,  ///< Top center
    South     = 1 << 1,  ///< Bottom center
    West      = 1 << 2,  ///< Left center
    East      = 1 << 3,  ///< Right center
    NorthWest = 1 << 4,  ///< Top-left corner
    NorthEast = 1 << 5,  ///< Top-right corner
    SouthWest = 1 << 6,  ///< Bottom-left corner
    SouthEast = 1 << 7,  ///< Bottom-right corner
    Center    = 1 << 8   ///< Center (custom extension)
};

// Check if axis ID is X
QIM_CORE_API bool isXAxisId(QImPlotAxisId id);
// Check if axis ID is Y
QIM_CORE_API bool isYAxisId(QImPlotAxisId id);

// Converts QImPlotAxisId enumeration to ImPlot's ImAxis enumeration value
QIM_CORE_API int toImAxis(QImPlotAxisId axis);
// Converts ImPlot's ImAxis enumeration to QImPlotAxisId wrapper enum
QIM_CORE_API QImPlotAxisId toQImPlotAxisId(int axis);

// Converts QImPlotScaleType wrapper enum to ImPlot's native scale enumeration value
QIM_CORE_API int toImPlotScale(QImPlotScaleType v);
// Converts ImPlot's scale enumeration value to QImPlotScaleType wrapper enum
QIM_CORE_API QImPlotScaleType toQImPlotScaleType(int v);

// Converts QImPlotScaleType wrapper enum to ImPlot's native scale enumeration value
QIM_CORE_API int toImPlotCond(QImPlotCondition v);
// Converts ImPlot's scale enumeration value to QImPlotScaleType wrapper enum
QIM_CORE_API QImPlotCondition toQImPlotCondition(int v);

// Converts ImPlotLocation to QImPlotLegendLocation
QIM_CORE_API QImPlotLegendLocation toQImPlotLegendLocation(int v);
QIM_CORE_API int toImPlotLocation(QImPlotLegendLocation v);

}

Q_DECLARE_METATYPE(QIM::QImPlotAxisId)
Q_DECLARE_METATYPE(QIM::QImPlotScaleType)
Q_DECLARE_METATYPE(QIM::QImPlotCondition)
Q_DECLARE_METATYPE(QIM::QImPlotLegendLocation)

// ===== Helper macros =====
#ifndef QIMPLOT_FLAG_ACCESSOR
#define QIMPLOT_FLAG_ACCESSOR(ClassName, FlagName, FlagEnum, emitFunName)                                              \
    bool ClassName::is##FlagName() const                                                                               \
    {                                                                                                                  \
        return (d_ptr->flags & FlagEnum) != 0;                                                                         \
    }                                                                                                                  \
    void ClassName::set##FlagName(bool on)                                                                             \
    {                                                                                                                  \
        QIM_D(d);                                                                                                      \
        const auto oldFlags = d->flags;                                                                                \
        if (on) {                                                                                                      \
            d->flags |= FlagEnum;                                                                                      \
        } else {                                                                                                       \
            d->flags &= ~FlagEnum;                                                                                     \
        }                                                                                                              \
        if (d->flags != oldFlags) {                                                                                    \
            Q_EMIT emitFunName();                                                                                      \
        }                                                                                                              \
    }
#endif
// Inverts NoXxx flags to affirmative semantics
#ifndef QIMPLOT_FLAG_ENABLED_ACCESSOR
#define QIMPLOT_FLAG_ENABLED_ACCESSOR(ClassName, PropName, FlagEnum, emitFunName)                                      \
    bool ClassName::is##PropName() const                                                                               \
    {                                                                                                                  \
        return (d_ptr->flags & FlagEnum) == 0;                                                                         \
    }                                                                                                                  \
    void ClassName::set##PropName(bool enabled)                                                                        \
    {                                                                                                                  \
        QIM_D(d);                                                                                                      \
        const auto oldFlags = d->flags;                                                                                \
        if (enabled) {                                                                                                 \
            d->flags &= ~FlagEnum;                                                                                     \
        } else {                                                                                                       \
            d->flags |= FlagEnum;                                                                                      \
        }                                                                                                              \
        if (d->flags != oldFlags) {                                                                                    \
            Q_EMIT emitFunName();                                                                                      \
        }                                                                                                              \
    }
#endif

#endif  // QIMPLOT_H
