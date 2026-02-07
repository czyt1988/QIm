#ifndef QIMPLOT_H
#define QIMPLOT_H
#include <QMetaType>
#include "QImAPI.h"
namespace QIM
{
/**
 * @brief 对应ImAxis
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
 * @brief 对应ImPlotScale
 */
enum class QImPlotScaleType
{
    Linear = 0,  ///< ImPlotScale_::ImPlotScale_Linear
    Time,        ///< ImPlotScale_::ImPlotScale_Time
    Log10,       ///< ImPlotScale_::ImPlotScale_Log10
    SymLog       ///< ImPlotScale_::ImPlotScale_SymLog
};

/**
 * @brief 对应ImPlotCond
 */
enum class QImPlotCondition
{
    None = 0,  ///< ImPlotCond_::ImPlotCond_None
    Always,    ///< ImPlotCond_::ImPlotCond_Always
    Once       ///< ImPlotCond_::ImPlotCond_Once
};

/**
 * @brief ImPlot 图例位置枚举
 * @details 对应 ImPlotLocation 枚举，用于设置图例在绘图区域中的位置
 */
enum class QImPlotLegendLocation
{
    North     = 1 << 0,  ///< 顶部居中
    South     = 1 << 1,  ///< 底部居中
    West      = 1 << 2,  ///< 左侧居中
    East      = 1 << 3,  ///< 右侧居中
    NorthWest = 1 << 4,  ///< 左上角
    NorthEast = 1 << 5,  ///< 右上角
    SouthWest = 1 << 6,  ///< 左下角
    SouthEast = 1 << 7,  ///< 右下角
    Center    = 1 << 8   ///< 居中（自定义扩展）
};

// 判断是否为x轴
QIM_CORE_API bool isXAxisId(QImPlotAxisId id);
// 判断是否为y轴
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

// ===== 辅助宏定义=====
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
// 这个针对 NOXXFlag 进行设置，反转为肯定语义
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
