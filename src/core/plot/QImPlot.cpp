#include "QImPlot.h"
#include "implot.h"
namespace QIM
{

/**
 * \if ENGLISH
 * @brief Converts QImPlotAxisId enumeration to ImPlot's ImAxis enumeration value
 * @param axis QImPlotAxisId enum value (X1/X2/X3/Y1/Y2/Y3/AxisCount)
 * @return Corresponding ImAxis enum value (ImAxis_X1 to ImAxis_Y3), or ImAxis_COUNT for invalid input
 * @details Performs direct 1:1 mapping between wrapper enum and native ImPlot enum:
 *          - QImPlotAxisId::X1 → ImAxis_X1 (0)
 *          - QImPlotAxisId::X2 → ImAxis_X2 (1)
 *          - QImPlotAxisId::X3 → ImAxis_X3 (2)
 *          - QImPlotAxisId::Y1 → ImAxis_Y1 (3)
 *          - QImPlotAxisId::Y2 → ImAxis_Y2 (4)
 *          - QImPlotAxisId::Y3 → ImAxis_Y3 (5)
 *          - QImPlotAxisId::AxisCount → ImAxis_COUNT (6)
 * @note Returns ImAxis_COUNT for unrecognized enum values as error indicator.
 *       ImAxis_COUNT is never a valid axis for plotting operations.
 * @see ImPlot::SetupAxis(), ImPlot::SetupAxisLimits()
 * \endif
 *
 * \if CHINESE
 * @brief 将 QImPlotAxisId 枚举转换为 ImPlot 的 ImAxis 枚举值
 * @param axis QImPlotAxisId 枚举值（X1/X2/X3/Y1/Y2/Y3/AxisCount）
 * @return 对应的 ImAxis 枚举值（ImAxis_X1 至 ImAxis_Y3），无效输入返回 ImAxis_COUNT
 * @details 执行包装枚举与原生 ImPlot 枚举之间的直接 1:1 映射：
 *          - QImPlotAxisId::X1 → ImAxis_X1 (0)
 *          - QImPlotAxisId::X2 → ImAxis_X2 (1)
 *          - QImPlotAxisId::X3 → ImAxis_X3 (2)
 *          - QImPlotAxisId::Y1 → ImAxis_Y1 (3)
 *          - QImPlotAxisId::Y2 → ImAxis_Y2 (4)
 *          - QImPlotAxisId::Y3 → ImAxis_Y3 (5)
 *          - QImPlotAxisId::AxisCount → ImAxis_COUNT (6)
 * @note 无法识别的枚举值返回 ImAxis_COUNT 作为错误指示符。
 *       ImAxis_COUNT 永远不是有效的绘图轴标识符。
 * @see ImPlot::SetupAxis(), ImPlot::SetupAxisLimits()
 * \endif
 */
int toImAxis(QImPlotAxisId axis)
{
    switch (axis) {
    case QImPlotAxisId::X1:
        return ImAxis_X1;
    case QImPlotAxisId::X2:
        return ImAxis_X2;
    case QImPlotAxisId::X3:
        return ImAxis_X3;
    case QImPlotAxisId::Y1:
        return ImAxis_Y1;
    case QImPlotAxisId::Y2:
        return ImAxis_Y2;
    case QImPlotAxisId::Y3:
        return ImAxis_Y3;
    case QImPlotAxisId::AxisCount:
        return ImAxis_COUNT;
    case QImPlotAxisId::Auto:
        return IMPLOT_AUTO;
    default:
        break;
    }
    return ImAxis_COUNT;
}

/**
 * \if ENGLISH
 * @brief Static conversion from ImPlot's ImAxis enumeration to QImPlotAxisId wrapper enum
 * @param axis Raw ImAxis enumeration value (0=ImAxis_X1 to 5=ImAxis_Y3, 6=ImAxis_COUNT)
 * @return Corresponding QImPlotAxisId enum value
 * @details Performs direct 1:1 mapping between native ImPlot axis identifiers and Qt wrapper enum.
 *          See detailed mapping in non-static version documentation.
 * @note This is a static utility function - no instance required. Invalid inputs default to AxisCount.
 * @see plotAxisIdToImAxis()
 * \endif
 *
 * \if CHINESE
 * @brief 静态转换：将 ImPlot 的 ImAxis 枚举转换为 QImPlotAxisId 包装枚举
 * @param axis 原始 ImAxis 枚举值（0=ImAxis_X1 至 5=ImAxis_Y3，6=ImAxis_COUNT）
 * @return 对应的 QImPlotAxisId 枚举值
 * @details 执行原生 ImPlot 轴标识符与 Qt 包装枚举之间的直接 1:1 映射。
 *          详细映射关系参见非静态版本文档。
 * @note 此为静态工具函数，无需实例。无效输入默认返回 AxisCount。
 * @see plotAxisIdToImAxis()
 * \endif
 */
QImPlotAxisId toQImPlotAxisId(int axis)
{
    switch (axis) {
    case ImAxis_X1:
        return QImPlotAxisId::X1;
    case ImAxis_X2:
        return QImPlotAxisId::X2;
    case ImAxis_X3:
        return QImPlotAxisId::X3;
    case ImAxis_Y1:
        return QImPlotAxisId::Y1;
    case ImAxis_Y2:
        return QImPlotAxisId::Y2;
    case ImAxis_Y3:
        return QImPlotAxisId::Y3;
    case ImAxis_COUNT:
        return QImPlotAxisId::AxisCount;
    default:
        break;
    }
    return QImPlotAxisId::AxisCount;
}

/**
 * \if ENGLISH
 * @brief Converts ImPlot's scale enumeration value to QImPlotScaleType wrapper enum
 * @param v Raw ImPlotScale_ enumeration value (ImPlotScale_Linear/Time/Log10/SymLog)
 * @return Corresponding QImPlotScaleType enum value
 * @details Performs direct 1:1 mapping between native ImPlot scale types and Qt wrapper enum:
 *          - ImPlotScale_Linear (0) → QImPlotScaleType::Linear
 *          - ImPlotScale_Time   (1) → QImPlotScaleType::Time
 *          - ImPlotScale_Log10  (2) → QImPlotScaleType::Log10
 *          - ImPlotScale_SymLog (3) → QImPlotScaleType::SymLog
 * @note Invalid input values default to QImPlotScaleType::Linear for safety.
 *       This function is typically used internally when reading axis state from ImPlot context.
 * @see plotScaleTypeToImPlotScale(), scaleType()
 * \endif
 *
 * \if CHINESE
 * @brief 将 ImPlot 的刻度枚举值转换为 QImPlotScaleType 包装枚举
 * @param v 原始 ImPlotScale_ 枚举值（ImPlotScale_Linear/Time/Log10/SymLog）
 * @return 对应的 QImPlotScaleType 枚举值
 * @details 执行原生 ImPlot 刻度类型与 Qt 包装枚举之间的直接 1:1 映射：
 *          - ImPlotScale_Linear (0) → QImPlotScaleType::Linear
 *          - ImPlotScale_Time   (1) → QImPlotScaleType::Time
 *          - ImPlotScale_Log10  (2) → QImPlotScaleType::Log10
 *          - ImPlotScale_SymLog (3) → QImPlotScaleType::SymLog
 * @note 无效输入值默认返回 QImPlotScaleType::Linear 以保证安全。
 *       此函数通常在从 ImPlot 上下文读取轴状态时内部使用。
 * @see plotScaleTypeToImPlotScale(), scaleType()
 * \endif
 */
QImPlotScaleType toQImPlotScaleType(int v)
{
    switch (v) {
    case ImPlotScale_Linear:
        return QImPlotScaleType::Linear;
    case ImPlotScale_Time:
        return QImPlotScaleType::Time;
    case ImPlotScale_Log10:
        return QImPlotScaleType::Log10;
    case ImPlotScale_SymLog:
        return QImPlotScaleType::SymLog;
    default:
        break;
    }
    return QImPlotScaleType::Linear;
}

/**
 * \if ENGLISH
 * @brief Converts QImPlotScaleType wrapper enum to ImPlot's native scale enumeration value
 * @param v QImPlotScaleType enum value (Linear/Time/Log10/SymLog)
 * @return Corresponding ImPlotScale_ enumeration value as integer
 * @details Performs direct 1:1 mapping between Qt wrapper enum and native ImPlot scale types:
 *          - QImPlotScaleType::Linear → ImPlotScale_Linear (0)
 *          - QImPlotScaleType::Time   → ImPlotScale_Time   (1)
 *          - QImPlotScaleType::Log10  → ImPlotScale_Log10  (2)
 *          - QImPlotScaleType::SymLog → ImPlotScale_SymLog (3)
 * @note Invalid enum values default to ImPlotScale_Linear for safety.
 *       The returned integer can be passed directly to ImPlot::SetupAxisScale().
 * @see imPlotScaleToPlotScaleType(), setScaleType()
 * \endif
 *
 * \if CHINESE
 * @brief 将 QImPlotScaleType 包装枚举转换为 ImPlot 的原生刻度枚举值
 * @param v QImPlotScaleType 枚举值（Linear/Time/Log10/SymLog）
 * @return 对应的 ImPlotScale_ 枚举值（整数形式）
 * @details 执行 Qt 包装枚举与原生 ImPlot 刻度类型之间的直接 1:1 映射：
 *          - QImPlotScaleType::Linear → ImPlotScale_Linear (0)
 *          - QImPlotScaleType::Time   → ImPlotScale_Time   (1)
 *          - QImPlotScaleType::Log10  → ImPlotScale_Log10  (2)
 *          - QImPlotScaleType::SymLog → ImPlotScale_SymLog (3)
 * @note 无效枚举值默认返回 ImPlotScale_Linear 以保证安全。
 *       返回的整数值可直接传递给 ImPlot::SetupAxisScale()。
 * @see imPlotScaleToPlotScaleType(), setScaleType()
 * \endif
 */
int toImPlotScale(QImPlotScaleType v)
{
    switch (v) {
    case QImPlotScaleType::Linear:
        return ImPlotScale_Linear;
    case QImPlotScaleType::Time:
        return ImPlotScale_Time;
    case QImPlotScaleType::Log10:
        return ImPlotScale_Log10;
    case QImPlotScaleType::SymLog:
        return ImPlotScale_SymLog;
    default:
        break;
    }
    return ImPlotScale_Linear;
}

int toImPlotCond(QImPlotCondition v)
{
    switch (v) {
    case QImPlotCondition::None:
        return ImPlotCond_::ImPlotCond_None;
    case QImPlotCondition::Always:
        return ImPlotCond_::ImPlotCond_Always;
    case QImPlotCondition::Once:
        return ImPlotCond_::ImPlotCond_Once;
    default:
        break;
    }
    return ImPlotCond_::ImPlotCond_None;
}

QImPlotCondition toQImPlotCondition(int v)
{
    switch (v) {
    case ImPlotCond_::ImPlotCond_None:
        return QImPlotCondition::None;
    case ImPlotCond_::ImPlotCond_Always:
        return QImPlotCondition::Always;
    case ImPlotCond_::ImPlotCond_Once:
        return QImPlotCondition::Once;
    default:
        break;
    }
    return QImPlotCondition::None;
}

QImPlotLegendLocation toQImPlotLegendLocation(int v)
{
    switch (v) {
    case ImPlotLocation_North:
        return QImPlotLegendLocation::North;
    case ImPlotLocation_South:
        return QImPlotLegendLocation::South;
    case ImPlotLocation_West:
        return QImPlotLegendLocation::West;
    case ImPlotLocation_East:
        return QImPlotLegendLocation::East;
    case ImPlotLocation_NorthWest:
        return QImPlotLegendLocation::NorthWest;
    case ImPlotLocation_NorthEast:
        return QImPlotLegendLocation::NorthEast;
    case ImPlotLocation_SouthWest:
        return QImPlotLegendLocation::SouthWest;
    case ImPlotLocation_SouthEast:
        return QImPlotLegendLocation::SouthEast;
    case ImPlotLocation_Center:
        return QImPlotLegendLocation::Center;
    default:
        break;
    }
    return QImPlotLegendLocation::East;
}

int toImPlotLocation(QImPlotLegendLocation v)
{
    switch (v) {
    case QImPlotLegendLocation::North:
        return ImPlotLocation_North;
    case QImPlotLegendLocation::South:
        return ImPlotLocation_South;
    case QImPlotLegendLocation::West:
        return ImPlotLocation_West;
    case QImPlotLegendLocation::East:
        return ImPlotLocation_East;
    case QImPlotLegendLocation::NorthWest:
        return ImPlotLocation_NorthWest;
    case QImPlotLegendLocation::NorthEast:
        return ImPlotLocation_NorthEast;
    case QImPlotLegendLocation::SouthWest:
        return ImPlotLocation_SouthWest;
    case QImPlotLegendLocation::SouthEast:
        return ImPlotLocation_SouthEast;
    case QImPlotLegendLocation::Center:
        return ImPlotLocation_Center;
    default:
        break;
    }
    return ImPlotLocation_East;
}

/**
 * \if ENGLISH
 * @brief Converts ImPlot's colormap enumeration value to QImPlotColormap wrapper enum
 * @param v Raw ImPlotColormap_ enumeration value (ImPlotColormap_Deep through ImPlotColormap_Greys)
 * @return Corresponding QImPlotColormap enum value
 * @details Performs direct 1:1 mapping between native ImPlot colormap types and Qt wrapper enum:
 *          - ImPlotColormap_Deep     (0)  → QImPlotColormap::Deep
 *          - ImPlotColormap_Dark     (1)  → QImPlotColormap::Dark
 *          - ImPlotColormap_Pastel   (2)  → QImPlotColormap::Pastel
 *          - ImPlotColormap_Paired   (3)  → QImPlotColormap::Paired
 *          - ImPlotColormap_Viridis  (4)  → QImPlotColormap::Viridis
 *          - ImPlotColormap_Plasma   (5)  → QImPlotColormap::Plasma
 *          - ImPlotColormap_Hot      (6)  → QImPlotColormap::Hot
 *          - ImPlotColormap_Cool     (7)  → QImPlotColormap::Cool
 *          - ImPlotColormap_Pink     (8)  → QImPlotColormap::Pink
 *          - ImPlotColormap_Jet      (9)  → QImPlotColormap::Jet
 *          - ImPlotColormap_Twilight (10) → QImPlotColormap::Twilight
 *          - ImPlotColormap_RdBu     (11) → QImPlotColormap::RdBu
 *          - ImPlotColormap_BrBG     (12) → QImPlotColormap::BrBG
 *          - ImPlotColormap_PiYG     (13) → QImPlotColormap::PiYG
 *          - ImPlotColormap_Spectral (14) → QImPlotColormap::Spectral
 *          - ImPlotColormap_Greys    (15) → QImPlotColormap::Greys
 * @note Invalid input values default to QImPlotColormap::Deep for safety.
 *       This function is typically used internally when reading colormap state from ImPlot context.
 * \endif
 *
 * \if CHINESE
 * @brief 将 ImPlot 的色图枚举值转换为 QImPlotColormap 包装枚举
 * @param v 原始 ImPlotColormap_ 枚举值（ImPlotColormap_Deep 到 ImPlotColormap_Greys）
 * @return 对应的 QImPlotColormap 枚举值
 * @details 执行原生 ImPlot 色图类型与 Qt 包装枚举之间的直接 1:1 映射：
 *          - ImPlotColormap_Deep     (0)  → QImPlotColormap::Deep
 *          - ImPlotColormap_Dark     (1)  → QImPlotColormap::Dark
 *          - ImPlotColormap_Pastel   (2)  → QImPlotColormap::Pastel
 *          - ImPlotColormap_Paired   (3)  → QImPlotColormap::Paired
 *          - ImPlotColormap_Viridis  (4)  → QImPlotColormap::Viridis
 *          - ImPlotColormap_Plasma   (5)  → QImPlotColormap::Plasma
 *          - ImPlotColormap_Hot      (6)  → QImPlotColormap::Hot
 *          - ImPlotColormap_Cool     (7)  → QImPlotColormap::Cool
 *          - ImPlotColormap_Pink     (8)  → QImPlotColormap::Pink
 *          - ImPlotColormap_Jet      (9)  → QImPlotColormap::Jet
 *          - ImPlotColormap_Twilight (10) → QImPlotColormap::Twilight
 *          - ImPlotColormap_RdBu     (11) → QImPlotColormap::RdBu
 *          - ImPlotColormap_BrBG     (12) → QImPlotColormap::BrBG
 *          - ImPlotColormap_PiYG     (13) → QImPlotColormap::PiYG
 *          - ImPlotColormap_Spectral (14) → QImPlotColormap::Spectral
 *          - ImPlotColormap_Greys    (15) → QImPlotColormap::Greys
 * @note 无效输入值默认返回 QImPlotColormap::Deep 以保证安全。
 *       此函数通常在从 ImPlot 上下文读取色图状态时内部使用。
 * \endif
 */
QImPlotColormap toQImPlotColormap(int v)
{
    switch (v) {
    case ImPlotColormap_Deep:
        return QImPlotColormap::Deep;
    case ImPlotColormap_Dark:
        return QImPlotColormap::Dark;
    case ImPlotColormap_Pastel:
        return QImPlotColormap::Pastel;
    case ImPlotColormap_Paired:
        return QImPlotColormap::Paired;
    case ImPlotColormap_Viridis:
        return QImPlotColormap::Viridis;
    case ImPlotColormap_Plasma:
        return QImPlotColormap::Plasma;
    case ImPlotColormap_Hot:
        return QImPlotColormap::Hot;
    case ImPlotColormap_Cool:
        return QImPlotColormap::Cool;
    case ImPlotColormap_Pink:
        return QImPlotColormap::Pink;
    case ImPlotColormap_Jet:
        return QImPlotColormap::Jet;
    case ImPlotColormap_Twilight:
        return QImPlotColormap::Twilight;
    case ImPlotColormap_RdBu:
        return QImPlotColormap::RdBu;
    case ImPlotColormap_BrBG:
        return QImPlotColormap::BrBG;
    case ImPlotColormap_PiYG:
        return QImPlotColormap::PiYG;
    case ImPlotColormap_Spectral:
        return QImPlotColormap::Spectral;
    case ImPlotColormap_Greys:
        return QImPlotColormap::Greys;
    default:
        break;
    }
    return QImPlotColormap::Deep;
}

/**
 * \if ENGLISH
 * @brief Converts QImPlotColormap wrapper enum to ImPlot's native colormap enumeration value
 * @param v QImPlotColormap enum value (Deep through Greys)
 * @return Corresponding ImPlotColormap_ enumeration value as integer
 * @details Performs direct 1:1 mapping between Qt wrapper enum and native ImPlot colormap types:
 *          - QImPlotColormap::Deep     → ImPlotColormap_Deep     (0)
 *          - QImPlotColormap::Dark     → ImPlotColormap_Dark     (1)
 *          - QImPlotColormap::Pastel   → ImPlotColormap_Pastel   (2)
 *          - QImPlotColormap::Paired   → ImPlotColormap_Paired   (3)
 *          - QImPlotColormap::Viridis  → ImPlotColormap_Viridis  (4)
 *          - QImPlotColormap::Plasma   → ImPlotColormap_Plasma   (5)
 *          - QImPlotColormap::Hot      → ImPlotColormap_Hot      (6)
 *          - QImPlotColormap::Cool     → ImPlotColormap_Cool     (7)
 *          - QImPlotColormap::Pink     → ImPlotColormap_Pink     (8)
 *          - QImPlotColormap::Jet      → ImPlotColormap_Jet      (9)
 *          - QImPlotColormap::Twilight → ImPlotColormap_Twilight (10)
 *          - QImPlotColormap::RdBu     → ImPlotColormap_RdBu     (11)
 *          - QImPlotColormap::BrBG     → ImPlotColormap_BrBG     (12)
 *          - QImPlotColormap::PiYG     → ImPlotColormap_PiYG     (13)
 *          - QImPlotColormap::Spectral → ImPlotColormap_Spectral (14)
 *          - QImPlotColormap::Greys    → ImPlotColormap_Greys    (15)
 * @note Invalid enum values default to ImPlotColormap_Deep for safety.
 *       The returned integer can be passed directly to ImPlot colormap APIs.
 * \endif
 *
 * \if CHINESE
 * @brief 将 QImPlotColormap 包装枚举转换为 ImPlot 的原生色图枚举值
 * @param v QImPlotColormap 枚举值（Deep 到 Greys）
 * @return 对应的 ImPlotColormap_ 枚举值（整数形式）
 * @details 执行 Qt 包装枚举与原生 ImPlot 色图类型之间的直接 1:1 映射：
 *          - QImPlotColormap::Deep     → ImPlotColormap_Deep     (0)
 *          - QImPlotColormap::Dark     → ImPlotColormap_Dark     (1)
 *          - QImPlotColormap::Pastel   → ImPlotColormap_Pastel   (2)
 *          - QImPlotColormap::Paired   → ImPlotColormap_Paired   (3)
 *          - QImPlotColormap::Viridis  → ImPlotColormap_Viridis  (4)
 *          - QImPlotColormap::Plasma   → ImPlotColormap_Plasma   (5)
 *          - QImPlotColormap::Hot      → ImPlotColormap_Hot      (6)
 *          - QImPlotColormap::Cool     → ImPlotColormap_Cool     (7)
 *          - QImPlotColormap::Pink     → ImPlotColormap_Pink     (8)
 *          - QImPlotColormap::Jet      → ImPlotColormap_Jet      (9)
 *          - QImPlotColormap::Twilight → ImPlotColormap_Twilight (10)
 *          - QImPlotColormap::RdBu     → ImPlotColormap_RdBu     (11)
 *          - QImPlotColormap::BrBG     → ImPlotColormap_BrBG     (12)
 *          - QImPlotColormap::PiYG     → ImPlotColormap_PiYG     (13)
 *          - QImPlotColormap::Spectral → ImPlotColormap_Spectral (14)
 *          - QImPlotColormap::Greys    → ImPlotColormap_Greys    (15)
 * @note 无效枚举值默认返回 ImPlotColormap_Deep 以保证安全。
 *       返回的整数值可直接传递给 ImPlot 色图 API。
 * \endif
 */
int toImPlotColormap(QImPlotColormap v)
{
    switch (v) {
    case QImPlotColormap::Deep:
        return ImPlotColormap_Deep;
    case QImPlotColormap::Dark:
        return ImPlotColormap_Dark;
    case QImPlotColormap::Pastel:
        return ImPlotColormap_Pastel;
    case QImPlotColormap::Paired:
        return ImPlotColormap_Paired;
    case QImPlotColormap::Viridis:
        return ImPlotColormap_Viridis;
    case QImPlotColormap::Plasma:
        return ImPlotColormap_Plasma;
    case QImPlotColormap::Hot:
        return ImPlotColormap_Hot;
    case QImPlotColormap::Cool:
        return ImPlotColormap_Cool;
    case QImPlotColormap::Pink:
        return ImPlotColormap_Pink;
    case QImPlotColormap::Jet:
        return ImPlotColormap_Jet;
    case QImPlotColormap::Twilight:
        return ImPlotColormap_Twilight;
    case QImPlotColormap::RdBu:
        return ImPlotColormap_RdBu;
    case QImPlotColormap::BrBG:
        return ImPlotColormap_BrBG;
    case QImPlotColormap::PiYG:
        return ImPlotColormap_PiYG;
    case QImPlotColormap::Spectral:
        return ImPlotColormap_Spectral;
    case QImPlotColormap::Greys:
        return ImPlotColormap_Greys;
    default:
        break;
    }
    return ImPlotColormap_Deep;
}

bool isXAxisId(QImPlotAxisId id)
{
    return ((id >= QImPlotAxisId::X1) && (id <= QImPlotAxisId::X3));
}

bool isYAxisId(QImPlotAxisId id)
{
    return ((id >= QImPlotAxisId::Y1) && (id <= QImPlotAxisId::Y3));
}

}
