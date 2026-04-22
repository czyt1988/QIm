#include "QImPlot3D.h"
#include "implot3d.h"
namespace QIM
{

/**
 * \if ENGLISH
 * @brief Converts QImPlot3DAxisId enumeration to ImPlot3D's ImAxis3D enumeration value
 * @param axis QImPlot3DAxisId enum value (X1/Y1/Z1/AxisCount)
 * @return Corresponding ImAxis3D enum value (ImAxis3D_X to ImAxis3D_Z), or ImAxis3D_COUNT for invalid input
 * @details Performs direct 1:1 mapping between wrapper enum and native ImPlot3D enum:
 *          - QImPlot3DAxisId::X1 → ImAxis3D_X (0)
 *          - QImPlot3DAxisId::Y1 → ImAxis3D_Y (1)
 *          - QImPlot3DAxisId::Z1 → ImAxis3D_Z (2)
 *          - QImPlot3DAxisId::AxisCount → ImAxis3D_COUNT (3)
 * @note Returns ImAxis3D_COUNT for unrecognized enum values as error indicator.
 *       ImAxis3D_COUNT is never a valid axis for plotting operations.
 * @see ImPlot3D::SetupAxis(), ImPlot3D::SetupAxisLimits()
 * \endif
 *
 * \if CHINESE
 * @brief 将 QImPlot3DAxisId 枚举转换为 ImPlot3D 的 ImAxis3D 枚举值
 * @param axis QImPlot3DAxisId 枚举值（X1/Y1/Z1/AxisCount）
 * @return 对应的 ImAxis3D 枚举值（ImAxis3D_X 至 ImAxis3D_Z），无效输入返回 ImAxis3D_COUNT
 * @details 执行包装枚举与原生 ImPlot3D 枚举之间的直接 1:1 映射：
 *          - QImPlot3DAxisId::X1 → ImAxis3D_X (0)
 *          - QImPlot3DAxisId::Y1 → ImAxis3D_Y (1)
 *          - QImPlot3DAxisId::Z1 → ImAxis3D_Z (2)
 *          - QImPlot3DAxisId::AxisCount → ImAxis3D_COUNT (3)
 * @note 无法识别的枚举值返回 ImAxis3D_COUNT 作为错误指示符。
 *       ImAxis3D_COUNT 永远不是有效的绘图轴标识符。
 * @see ImPlot3D::SetupAxis(), ImPlot3D::SetupAxisLimits()
 * \endif
 */
int toImAxis3D(QImPlot3DAxisId axis)
{
    switch (axis) {
    case QImPlot3DAxisId::X1:
        return ImAxis3D_X;
    case QImPlot3DAxisId::Y1:
        return ImAxis3D_Y;
    case QImPlot3DAxisId::Z1:
        return ImAxis3D_Z;
    case QImPlot3DAxisId::AxisCount:
        return ImAxis3D_COUNT;
    default:
        break;
    }
    return ImAxis3D_COUNT;
}

/**
 * \if ENGLISH
 * @brief Static conversion from ImPlot3D's ImAxis3D enumeration to QImPlot3DAxisId wrapper enum
 * @param axis Raw ImAxis3D enumeration value (0=ImAxis3D_X to 2=ImAxis3D_Z, 3=ImAxis3D_COUNT)
 * @return Corresponding QImPlot3DAxisId enum value
 * @details Performs direct 1:1 mapping between native ImPlot3D axis identifiers and Qt wrapper enum.
 *          See detailed mapping in non-static version documentation.
 * @note This is a static utility function - no instance required. Invalid inputs default to AxisCount.
 * @see toImAxis3D()
 * \endif
 *
 * \if CHINESE
 * @brief 静态转换：将 ImPlot3D 的 ImAxis3D 枚举转换为 QImPlot3DAxisId 包装枚举
 * @param axis 原始 ImAxis3D 枚举值（0=ImAxis3D_X 至 2=ImAxis3D_Z，3=ImAxis3D_COUNT）
 * @return 对应的 QImPlot3DAxisId 枚举值
 * @details 执行原生 ImPlot3D 轴标识符与 Qt 包装枚举之间的直接 1:1 映射。
 *          详细映射关系参见非静态版本文档。
 * @note 此为静态工具函数，无需实例。无效输入默认返回 AxisCount。
 * @see toImAxis3D()
 * \endif
 */
QImPlot3DAxisId toQImPlot3DAxisId(int axis)
{
    switch (axis) {
    case ImAxis3D_X:
        return QImPlot3DAxisId::X1;
    case ImAxis3D_Y:
        return QImPlot3DAxisId::Y1;
    case ImAxis3D_Z:
        return QImPlot3DAxisId::Z1;
    case ImAxis3D_COUNT:
        return QImPlot3DAxisId::AxisCount;
    default:
        break;
    }
    return QImPlot3DAxisId::AxisCount;
}

/**
 * \if ENGLISH
 * @brief Converts ImPlot3D's scale enumeration value to QImPlot3DScaleType wrapper enum
 * @param v Raw ImPlot3DScale_ enumeration value (ImPlot3DScale_Linear/Log10/SymLog)
 * @return Corresponding QImPlot3DScaleType enum value
 * @details Performs direct 1:1 mapping between native ImPlot3D scale types and Qt wrapper enum:
 *          - ImPlot3DScale_Linear (0) → QImPlot3DScaleType::Linear
 *          - ImPlot3DScale_Log10  (1) → QImPlot3DScaleType::Log10
 *          - ImPlot3DScale_SymLog (2) → QImPlot3DScaleType::SymLog
 * @note Invalid input values default to QImPlot3DScaleType::Linear for safety.
 *       This function is typically used internally when reading axis state from ImPlot3D context.
 *       ImPlot3D does not have Time scale type like ImPlot does.
 * @see toImPlot3DScale()
 * \endif
 *
 * \if CHINESE
 * @brief 将 ImPlot3D 的刻度枚举值转换为 QImPlot3DScaleType 包装枚举
 * @param v 原始 ImPlot3DScale_ 枚举值（ImPlot3DScale_Linear/Log10/SymLog）
 * @return 对应的 QImPlot3DScaleType 枚举值
 * @details 执行原生 ImPlot3D 刻度类型与 Qt 包装枚举之间的直接 1:1 映射：
 *          - ImPlot3DScale_Linear (0) → QImPlot3DScaleType::Linear
 *          - ImPlot3DScale_Log10  (1) → QImPlot3DScaleType::Log10
 *          - ImPlot3DScale_SymLog (2) → QImPlot3DScaleType::SymLog
 * @note 无效输入值默认返回 QImPlot3DScaleType::Linear 以保证安全。
 *       此函数通常在从 ImPlot3D 上下文读取轴状态时内部使用。
 *       ImPlot3D 没有 ImPlot 中的 Time 刻度类型。
 * @see toImPlot3DScale()
 * \endif
 */
QImPlot3DScaleType toQImPlot3DScaleType(int v)
{
    switch (v) {
    case ImPlot3DScale_Linear:
        return QImPlot3DScaleType::Linear;
    case ImPlot3DScale_Log10:
        return QImPlot3DScaleType::Log10;
    case ImPlot3DScale_SymLog:
        return QImPlot3DScaleType::SymLog;
    default:
        break;
    }
    return QImPlot3DScaleType::Linear;
}

/**
 * \if ENGLISH
 * @brief Converts QImPlot3DScaleType wrapper enum to ImPlot3D's native scale enumeration value
 * @param v QImPlot3DScaleType enum value (Linear/Log10/SymLog)
 * @return Corresponding ImPlot3DScale_ enumeration value as integer
 * @details Performs direct 1:1 mapping between Qt wrapper enum and native ImPlot3D scale types:
 *          - QImPlot3DScaleType::Linear → ImPlot3DScale_Linear (0)
 *          - QImPlot3DScaleType::Log10  → ImPlot3DScale_Log10  (1)
 *          - QImPlot3DScaleType::SymLog → ImPlot3DScale_SymLog (2)
 * @note Invalid enum values default to ImPlot3DScale_Linear for safety.
 *       The returned integer can be passed directly to ImPlot3D::SetupAxisScale().
 *       ImPlot3D does not have Time scale type like ImPlot does.
 * @see toQImPlot3DScaleType()
 * \endif
 *
 * \if CHINESE
 * @brief 将 QImPlot3DScaleType 包装枚举转换为 ImPlot3D 的原生刻度枚举值
 * @param v QImPlot3DScaleType 枚举值（Linear/Log10/SymLog）
 * @return 对应的 ImPlot3DScale_ 枚举值（整数形式）
 * @details 执行 Qt 包装枚举与原生 ImPlot3D 刻度类型之间的直接 1:1 映射：
 *          - QImPlot3DScaleType::Linear → ImPlot3DScale_Linear (0)
 *          - QImPlot3DScaleType::Log10  → ImPlot3DScale_Log10  (1)
 *          - QImPlot3DScaleType::SymLog → ImPlot3DScale_SymLog (2)
 * @note 无效枚举值默认返回 ImPlot3DScale_Linear 以保证安全。
 *       返回的整数值可直接传递给 ImPlot3D::SetupAxisScale()。
 *       ImPlot3D 没有 ImPlot 中的 Time 刻度类型。
 * @see toQImPlot3DScaleType()
 * \endif
 */
int toImPlot3DScale(QImPlot3DScaleType v)
{
    switch (v) {
    case QImPlot3DScaleType::Linear:
        return ImPlot3DScale_Linear;
    case QImPlot3DScaleType::Log10:
        return ImPlot3DScale_Log10;
    case QImPlot3DScaleType::SymLog:
        return ImPlot3DScale_SymLog;
    default:
        break;
    }
    return ImPlot3DScale_Linear;
}

/**
 * \if ENGLISH
 * @brief Converts QImPlot3DCondition wrapper enum to ImPlot3D's native condition enumeration value
 * @param v QImPlot3DCondition enum value (None/Always/Once)
 * @return Corresponding ImPlot3DCond_ enumeration value as integer
 * @details Performs direct 1:1 mapping between Qt wrapper enum and native ImPlot3D condition types:
 *          - QImPlot3DCondition::None   → ImPlot3DCond_None   (0)
 *          - QImPlot3DCondition::Always → ImPlot3DCond_Always (1)
 *          - QImPlot3DCondition::Once   → ImPlot3DCond_Once   (2)
 * @note Invalid enum values default to ImPlot3DCond_None for safety.
 *       The returned integer can be passed directly to ImPlot3D::SetupAxisLimits() etc.
 * @see toQImPlot3DCondition()
 * \endif
 *
 * \if CHINESE
 * @brief 将 QImPlot3DCondition 包装枚举转换为 ImPlot3D 的原生条件枚举值
 * @param v QImPlot3DCondition 枚举值（None/Always/Once）
 * @return 对应的 ImPlot3DCond_ 枚举值（整数形式）
 * @details 执行 Qt 包装枚举与原生 ImPlot3D 条件类型之间的直接 1:1 映射：
 *          - QImPlot3DCondition::None   → ImPlot3DCond_None   (0)
 *          - QImPlot3DCondition::Always → ImPlot3DCond_Always (1)
 *          - QImPlot3DCondition::Once   → ImPlot3DCond_Once   (2)
 * @note 无效枚举值默认返回 ImPlot3DCond_None 以保证安全。
 *       返回的整数值可直接传递给 ImPlot3D::SetupAxisLimits() 等函数。
 * @see toQImPlot3DCondition()
 * \endif
 */
int toImPlot3DCond(QImPlot3DCondition v)
{
    switch (v) {
    case QImPlot3DCondition::None:
        return ImPlot3DCond_None;
    case QImPlot3DCondition::Always:
        return ImPlot3DCond_Always;
    case QImPlot3DCondition::Once:
        return ImPlot3DCond_Once;
    default:
        break;
    }
    return ImPlot3DCond_None;
}

/**
 * \if ENGLISH
 * @brief Converts ImPlot3D's condition enumeration value to QImPlot3DCondition wrapper enum
 * @param v Raw ImPlot3DCond_ enumeration value (ImPlot3DCond_None/Always/Once)
 * @return Corresponding QImPlot3DCondition enum value
 * @details Performs direct 1:1 mapping between native ImPlot3D condition types and Qt wrapper enum:
 *          - ImPlot3DCond_None   (0) → QImPlot3DCondition::None
 *          - ImPlot3DCond_Always (1) → QImPlot3DCondition::Always
 *          - ImPlot3DCond_Once   (2) → QImPlot3DCondition::Once
 * @note Invalid input values default to QImPlot3DCondition::None for safety.
 * @see toImPlot3DCond()
 * \endif
 *
 * \if CHINESE
 * @brief 将 ImPlot3D 的条件枚举值转换为 QImPlot3DCondition 包装枚举
 * @param v 原始 ImPlot3DCond_ 枚举值（ImPlot3DCond_None/Always/Once）
 * @return 对应的 QImPlot3DCondition 枚举值
 * @details 执行原生 ImPlot3D 条件类型与 Qt 包装枚举之间的直接 1:1 映射：
 *          - ImPlot3DCond_None   (0) → QImPlot3DCondition::None
 *          - ImPlot3DCond_Always (1) → QImPlot3DCondition::Always
 *          - ImPlot3DCond_Once   (2) → QImPlot3DCondition::Once
 * @note 无效输入值默认返回 QImPlot3DCondition::None 以保证安全。
 * @see toImPlot3DCond()
 * \endif
 */
QImPlot3DCondition toQImPlot3DCondition(int v)
{
    switch (v) {
    case ImPlot3DCond_None:
        return QImPlot3DCondition::None;
    case ImPlot3DCond_Always:
        return QImPlot3DCondition::Always;
    case ImPlot3DCond_Once:
        return QImPlot3DCondition::Once;
    default:
        break;
    }
    return QImPlot3DCondition::None;
}

/**
 * \if ENGLISH
 * @brief Converts QImPlot3DMarkerShape wrapper enum to ImPlot3D's native marker enumeration value
 * @param v QImPlot3DMarkerShape enum value (None/Circle/Square/Diamond/Up/Down/Left/Right/Cross/Plus/Asterisk)
 * @return Corresponding ImPlot3DMarker_ enumeration value as integer
 * @details Performs direct 1:1 mapping between Qt wrapper enum and native ImPlot3D marker types:
 *          - QImPlot3DMarkerShape::None     → ImPlot3DMarker_None     (-1)
 *          - QImPlot3DMarkerShape::Circle   → ImPlot3DMarker_Circle   (0)
 *          - QImPlot3DMarkerShape::Square   → ImPlot3DMarker_Square   (1)
 *          - QImPlot3DMarkerShape::Diamond  → ImPlot3DMarker_Diamond  (2)
 *          - QImPlot3DMarkerShape::Up       → ImPlot3DMarker_Up       (3)
 *          - QImPlot3DMarkerShape::Down     → ImPlot3DMarker_Down     (4)
 *          - QImPlot3DMarkerShape::Left     → ImPlot3DMarker_Left     (5)
 *          - QImPlot3DMarkerShape::Right    → ImPlot3DMarker_Right    (6)
 *          - QImPlot3DMarkerShape::Cross    → ImPlot3DMarker_Cross    (7)
 *          - QImPlot3DMarkerShape::Plus     → ImPlot3DMarker_Plus     (8)
 *          - QImPlot3DMarkerShape::Asterisk → ImPlot3DMarker_Asterisk (9)
 * @note Invalid enum values default to ImPlot3DMarker_None for safety.
 *       The returned integer can be passed directly to ImPlot3D::SetNextMarkerStyle().
 * @see toQImPlot3DMarkerShape()
 * \endif
 *
 * \if CHINESE
 * @brief 将 QImPlot3DMarkerShape 包装枚举转换为 ImPlot3D 的原生标记枚举值
 * @param v QImPlot3DMarkerShape 枚举值（None/Circle/Square/Diamond/Up/Down/Left/Right/Cross/Plus/Asterisk）
 * @return 对应的 ImPlot3DMarker_ 枚举值（整数形式）
 * @details 执行 Qt 包装枚举与原生 ImPlot3D 标记类型之间的直接 1:1 映射：
 *          - QImPlot3DMarkerShape::None     → ImPlot3DMarker_None     (-1)
 *          - QImPlot3DMarkerShape::Circle   → ImPlot3DMarker_Circle   (0)
 *          - QImPlot3DMarkerShape::Square   → ImPlot3DMarker_Square   (1)
 *          - QImPlot3DMarkerShape::Diamond  → ImPlot3DMarker_Diamond  (2)
 *          - QImPlot3DMarkerShape::Up       → ImPlot3DMarker_Up       (3)
 *          - QImPlot3DMarkerShape::Down     → ImPlot3DMarker_Down     (4)
 *          - QImPlot3DMarkerShape::Left     → ImPlot3DMarker_Left     (5)
 *          - QImPlot3DMarkerShape::Right    → ImPlot3DMarker_Right    (6)
 *          - QImPlot3DMarkerShape::Cross    → ImPlot3DMarker_Cross    (7)
 *          - QImPlot3DMarkerShape::Plus     → ImPlot3DMarker_Plus     (8)
 *          - QImPlot3DMarkerShape::Asterisk → ImPlot3DMarker_Asterisk (9)
 * @note 无效枚举值默认返回 ImPlot3DMarker_None 以保证安全。
 *       返回的整数值可直接传递给 ImPlot3D::SetNextMarkerStyle()。
 * @see toQImPlot3DMarkerShape()
 * \endif
 */
int toImPlot3DMarker(QImPlot3DMarkerShape v)
{
    switch (v) {
    case QImPlot3DMarkerShape::None:
        return ImPlot3DMarker_None;
    case QImPlot3DMarkerShape::Circle:
        return ImPlot3DMarker_Circle;
    case QImPlot3DMarkerShape::Square:
        return ImPlot3DMarker_Square;
    case QImPlot3DMarkerShape::Diamond:
        return ImPlot3DMarker_Diamond;
    case QImPlot3DMarkerShape::Up:
        return ImPlot3DMarker_Up;
    case QImPlot3DMarkerShape::Down:
        return ImPlot3DMarker_Down;
    case QImPlot3DMarkerShape::Left:
        return ImPlot3DMarker_Left;
    case QImPlot3DMarkerShape::Right:
        return ImPlot3DMarker_Right;
    case QImPlot3DMarkerShape::Cross:
        return ImPlot3DMarker_Cross;
    case QImPlot3DMarkerShape::Plus:
        return ImPlot3DMarker_Plus;
    case QImPlot3DMarkerShape::Asterisk:
        return ImPlot3DMarker_Asterisk;
    default:
        break;
    }
    return ImPlot3DMarker_None;
}

/**
 * \if ENGLISH
 * @brief Converts ImPlot3D's marker enumeration value to QImPlot3DMarkerShape wrapper enum
 * @param v Raw ImPlot3DMarker_ enumeration value (-1=ImPlot3DMarker_None to 9=ImPlot3DMarker_Asterisk)
 * @return Corresponding QImPlot3DMarkerShape enum value
 * @details Performs direct 1:1 mapping between native ImPlot3D marker types and Qt wrapper enum.
 *          See detailed mapping in non-static version documentation.
 * @note Invalid input values default to QImPlot3DMarkerShape::None for safety.
 * @see toImPlot3DMarker()
 * \endif
 *
 * \if CHINESE
 * @brief 将 ImPlot3D 的标记枚举值转换为 QImPlot3DMarkerShape 包装枚举
 * @param v 原始 ImPlot3DMarker_ 枚举值（-1=ImPlot3DMarker_None 至 9=ImPlot3DMarker_Asterisk）
 * @return 对应的 QImPlot3DMarkerShape 枚举值
 * @details 执行原生 ImPlot3D 标记类型与 Qt 包装枚举之间的直接 1:1 映射。
 *          详细映射关系参见非静态版本文档。
 * @note 无效输入值默认返回 QImPlot3DMarkerShape::None 以保证安全。
 * @see toImPlot3DMarker()
 * \endif
 */
QImPlot3DMarkerShape toQImPlot3DMarkerShape(int v)
{
    switch (v) {
    case ImPlot3DMarker_None:
        return QImPlot3DMarkerShape::None;
    case ImPlot3DMarker_Circle:
        return QImPlot3DMarkerShape::Circle;
    case ImPlot3DMarker_Square:
        return QImPlot3DMarkerShape::Square;
    case ImPlot3DMarker_Diamond:
        return QImPlot3DMarkerShape::Diamond;
    case ImPlot3DMarker_Up:
        return QImPlot3DMarkerShape::Up;
    case ImPlot3DMarker_Down:
        return QImPlot3DMarkerShape::Down;
    case ImPlot3DMarker_Left:
        return QImPlot3DMarkerShape::Left;
    case ImPlot3DMarker_Right:
        return QImPlot3DMarkerShape::Right;
    case ImPlot3DMarker_Cross:
        return QImPlot3DMarkerShape::Cross;
    case ImPlot3DMarker_Plus:
        return QImPlot3DMarkerShape::Plus;
    case ImPlot3DMarker_Asterisk:
        return QImPlot3DMarkerShape::Asterisk;
    default:
        break;
    }
    return QImPlot3DMarkerShape::None;
}

/**
 * \if ENGLISH
 * @brief Converts QImPlot3DColormap wrapper enum to ImPlot3D's native colormap enumeration value
 * @param v QImPlot3DColormap enum value (Deep to Greys)
 * @return Corresponding ImPlot3DColormap_ enumeration value as integer
 * @details Performs direct 1:1 mapping between Qt wrapper enum and native ImPlot3D colormap types:
 *          - QImPlot3DColormap::Deep      → ImPlot3DColormap_Deep      (0)
 *          - QImPlot3DColormap::Dark      → ImPlot3DColormap_Dark      (1)
 *          - QImPlot3DColormap::Pastel    → ImPlot3DColormap_Pastel    (2)
 *          - QImPlot3DColormap::Paired    → ImPlot3DColormap_Paired    (3)
 *          - QImPlot3DColormap::Viridis   → ImPlot3DColormap_Viridis   (4)
 *          - QImPlot3DColormap::Plasma    → ImPlot3DColormap_Plasma    (5)
 *          - QImPlot3DColormap::Hot       → ImPlot3DColormap_Hot       (6)
 *          - QImPlot3DColormap::Cool      → ImPlot3DColormap_Cool      (7)
 *          - QImPlot3DColormap::Pink      → ImPlot3DColormap_Pink      (8)
 *          - QImPlot3DColormap::Jet       → ImPlot3DColormap_Jet       (9)
 *          - QImPlot3DColormap::Twilight  → ImPlot3DColormap_Twilight  (10)
 *          - QImPlot3DColormap::RdBu      → ImPlot3DColormap_RdBu      (11)
 *          - QImPlot3DColormap::BrBG      → ImPlot3DColormap_BrBG      (12)
 *          - QImPlot3DColormap::PiYG      → ImPlot3DColormap_PiYG      (13)
 *          - QImPlot3DColormap::Spectral  → ImPlot3DColormap_Spectral  (14)
 *          - QImPlot3DColormap::Greys     → ImPlot3DColormap_Greys     (15)
 * @note Invalid enum values default to ImPlot3DColormap_Deep for safety.
 *       The returned integer can be passed directly to ImPlot3D::SetNextColormap().
 * @see toQImPlot3DColormap()
 * \endif
 *
 * \if CHINESE
 * @brief 将 QImPlot3DColormap 包装枚举转换为 ImPlot3D 的原生色彩映射枚举值
 * @param v QImPlot3DColormap 枚举值（Deep 至 Greys）
 * @return 对应的 ImPlot3DColormap_ 枚举值（整数形式）
 * @details 执行 Qt 包装枚举与原生 ImPlot3D 色彩映射类型之间的直接 1:1 映射：
 *          - QImPlot3DColormap::Deep      → ImPlot3DColormap_Deep      (0)
 *          - QImPlot3DColormap::Dark      → ImPlot3DColormap_Dark      (1)
 *          - QImPlot3DColormap::Pastel    → ImPlot3DColormap_Pastel    (2)
 *          - QImPlot3DColormap::Paired    → ImPlot3DColormap_Paired    (3)
 *          - QImPlot3DColormap::Viridis   → ImPlot3DColormap_Viridis   (4)
 *          - QImPlot3DColormap::Plasma    → ImPlot3DColormap_Plasma    (5)
 *          - QImPlot3DColormap::Hot       → ImPlot3DColormap_Hot       (6)
 *          - QImPlot3DColormap::Cool      → ImPlot3DColormap_Cool      (7)
 *          - QImPlot3DColormap::Pink      → ImPlot3DColormap_Pink      (8)
 *          - QImPlot3DColormap::Jet       → ImPlot3DColormap_Jet       (9)
 *          - QImPlot3DColormap::Twilight  → ImPlot3DColormap_Twilight  (10)
 *          - QImPlot3DColormap::RdBu      → ImPlot3DColormap_RdBu      (11)
 *          - QImPlot3DColormap::BrBG      → ImPlot3DColormap_BrBG      (12)
 *          - QImPlot3DColormap::PiYG      → ImPlot3DColormap_PiYG      (13)
 *          - QImPlot3DColormap::Spectral  → ImPlot3DColormap_Spectral  (14)
 *          - QImPlot3DColormap::Greys     → ImPlot3DColormap_Greys     (15)
 * @note 无效枚举值默认返回 ImPlot3DColormap_Deep 以保证安全。
 *       返回的整数值可直接传递给 ImPlot3D::SetNextColormap()。
 * @see toQImPlot3DColormap()
 * \endif
 */
int toImPlot3DColormap(QImPlot3DColormap v)
{
    switch (v) {
    case QImPlot3DColormap::Deep:
        return ImPlot3DColormap_Deep;
    case QImPlot3DColormap::Dark:
        return ImPlot3DColormap_Dark;
    case QImPlot3DColormap::Pastel:
        return ImPlot3DColormap_Pastel;
    case QImPlot3DColormap::Paired:
        return ImPlot3DColormap_Paired;
    case QImPlot3DColormap::Viridis:
        return ImPlot3DColormap_Viridis;
    case QImPlot3DColormap::Plasma:
        return ImPlot3DColormap_Plasma;
    case QImPlot3DColormap::Hot:
        return ImPlot3DColormap_Hot;
    case QImPlot3DColormap::Cool:
        return ImPlot3DColormap_Cool;
    case QImPlot3DColormap::Pink:
        return ImPlot3DColormap_Pink;
    case QImPlot3DColormap::Jet:
        return ImPlot3DColormap_Jet;
    case QImPlot3DColormap::Twilight:
        return ImPlot3DColormap_Twilight;
    case QImPlot3DColormap::RdBu:
        return ImPlot3DColormap_RdBu;
    case QImPlot3DColormap::BrBG:
        return ImPlot3DColormap_BrBG;
    case QImPlot3DColormap::PiYG:
        return ImPlot3DColormap_PiYG;
    case QImPlot3DColormap::Spectral:
        return ImPlot3DColormap_Spectral;
    case QImPlot3DColormap::Greys:
        return ImPlot3DColormap_Greys;
    default:
        break;
    }
    return ImPlot3DColormap_Deep;
}

/**
 * \if ENGLISH
 * @brief Converts ImPlot3D's colormap enumeration value to QImPlot3DColormap wrapper enum
 * @param v Raw ImPlot3DColormap_ enumeration value (0=ImPlot3DColormap_Deep to 15=ImPlot3DColormap_Greys)
 * @return Corresponding QImPlot3DColormap enum value
 * @details Performs direct 1:1 mapping between native ImPlot3D colormap types and Qt wrapper enum.
 *          See detailed mapping in non-static version documentation.
 * @note Invalid input values default to QImPlot3DColormap::Deep for safety.
 * @see toImPlot3DColormap()
 * \endif
 *
 * \if CHINESE
 * @brief 将 ImPlot3D 的色彩映射枚举值转换为 QImPlot3DColormap 包装枚举
 * @param v 原始 ImPlot3DColormap_ 枚举值（0=ImPlot3DColormap_Deep 至 15=ImPlot3DColormap_Greys）
 * @return 对应的 QImPlot3DColormap 枚举值
 * @details 执行原生 ImPlot3D 色彩映射类型与 Qt 包装枚举之间的直接 1:1 映射。
 *          详细映射关系参见非静态版本文档。
 * @note 无效输入值默认返回 QImPlot3DColormap::Deep 以保证安全。
 * @see toImPlot3DColormap()
 * \endif
 */
QImPlot3DColormap toQImPlot3DColormap(int v)
{
    switch (v) {
    case ImPlot3DColormap_Deep:
        return QImPlot3DColormap::Deep;
    case ImPlot3DColormap_Dark:
        return QImPlot3DColormap::Dark;
    case ImPlot3DColormap_Pastel:
        return QImPlot3DColormap::Pastel;
    case ImPlot3DColormap_Paired:
        return QImPlot3DColormap::Paired;
    case ImPlot3DColormap_Viridis:
        return QImPlot3DColormap::Viridis;
    case ImPlot3DColormap_Plasma:
        return QImPlot3DColormap::Plasma;
    case ImPlot3DColormap_Hot:
        return QImPlot3DColormap::Hot;
    case ImPlot3DColormap_Cool:
        return QImPlot3DColormap::Cool;
    case ImPlot3DColormap_Pink:
        return QImPlot3DColormap::Pink;
    case ImPlot3DColormap_Jet:
        return QImPlot3DColormap::Jet;
    case ImPlot3DColormap_Twilight:
        return QImPlot3DColormap::Twilight;
    case ImPlot3DColormap_RdBu:
        return QImPlot3DColormap::RdBu;
    case ImPlot3DColormap_BrBG:
        return QImPlot3DColormap::BrBG;
    case ImPlot3DColormap_PiYG:
        return QImPlot3DColormap::PiYG;
    case ImPlot3DColormap_Spectral:
        return QImPlot3DColormap::Spectral;
    case ImPlot3DColormap_Greys:
        return QImPlot3DColormap::Greys;
    default:
        break;
    }
    return QImPlot3DColormap::Deep;
}

/**
 * \if ENGLISH
 * @brief Converts QImPlot3DCol wrapper enum to ImPlot3D's native color enumeration value
 * @param v QImPlot3DCol enum value (Line to COUNT)
 * @return Corresponding ImPlot3DCol_ enumeration value as integer
 * @details Performs direct 1:1 mapping between Qt wrapper enum and native ImPlot3D color types.
 * @note Invalid enum values default to ImPlot3DCol_Line for safety.
 * @see toQImPlot3DCol()
 * \endif
 *
 * \if CHINESE
 * @brief 将 QImPlot3DCol 包装枚举转换为 ImPlot3D 的原生颜色枚举值
 * @param v QImPlot3DCol 枚举值（Line 至 COUNT）
 * @return 对应的 ImPlot3DCol_ 枚举值（整数形式）
 * @details 执行 Qt 包装枚举与原生 ImPlot3D 颜色类型之间的直接 1:1 映射。
 * @note 无效枚举值默认返回 ImPlot3DCol_Line 以保证安全。
 * @see toQImPlot3DCol()
 * \endif
 */
int toImPlot3DCol(QImPlot3DCol v)
{
    switch (v) {
    case QImPlot3DCol::Line:
        return ImPlot3DCol_Line;
    case QImPlot3DCol::Fill:
        return ImPlot3DCol_Fill;
    case QImPlot3DCol::MarkerOutline:
        return ImPlot3DCol_MarkerOutline;
    case QImPlot3DCol::MarkerFill:
        return ImPlot3DCol_MarkerFill;
    case QImPlot3DCol::TitleText:
        return ImPlot3DCol_TitleText;
    case QImPlot3DCol::InlayText:
        return ImPlot3DCol_InlayText;
    case QImPlot3DCol::FrameBg:
        return ImPlot3DCol_FrameBg;
    case QImPlot3DCol::PlotBg:
        return ImPlot3DCol_PlotBg;
    case QImPlot3DCol::PlotBorder:
        return ImPlot3DCol_PlotBorder;
    case QImPlot3DCol::LegendBg:
        return ImPlot3DCol_LegendBg;
    case QImPlot3DCol::LegendBorder:
        return ImPlot3DCol_LegendBorder;
    case QImPlot3DCol::LegendText:
        return ImPlot3DCol_LegendText;
    case QImPlot3DCol::AxisText:
        return ImPlot3DCol_AxisText;
    case QImPlot3DCol::AxisGrid:
        return ImPlot3DCol_AxisGrid;
    case QImPlot3DCol::AxisTick:
        return ImPlot3DCol_AxisTick;
    case QImPlot3DCol::COUNT:
        return ImPlot3DCol_COUNT;
    default:
        break;
    }
    return ImPlot3DCol_Line;
}

/**
 * \if ENGLISH
 * @brief Converts ImPlot3D's color enumeration value to QImPlot3DCol wrapper enum
 * @param v Raw ImPlot3DCol_ enumeration value
 * @return Corresponding QImPlot3DCol enum value
 * @details Performs direct 1:1 mapping between native ImPlot3D color types and Qt wrapper enum.
 * @note Invalid input values default to QImPlot3DCol::Line for safety.
 * @see toImPlot3DCol()
 * \endif
 *
 * \if CHINESE
 * @brief 将 ImPlot3D 的颜色枚举值转换为 QImPlot3DCol 包装枚举
 * @param v 原始 ImPlot3DCol_ 枚举值
 * @return 对应的 QImPlot3DCol 枚举值
 * @details 执行原生 ImPlot3D 颜色类型与 Qt 包装枚举之间的直接 1:1 映射。
 * @note 无效输入值默认返回 QImPlot3DCol::Line 以保证安全。
 * @see toImPlot3DCol()
 * \endif
 */
QImPlot3DCol toQImPlot3DCol(int v)
{
    switch (v) {
    case ImPlot3DCol_Line:
        return QImPlot3DCol::Line;
    case ImPlot3DCol_Fill:
        return QImPlot3DCol::Fill;
    case ImPlot3DCol_MarkerOutline:
        return QImPlot3DCol::MarkerOutline;
    case ImPlot3DCol_MarkerFill:
        return QImPlot3DCol::MarkerFill;
    case ImPlot3DCol_TitleText:
        return QImPlot3DCol::TitleText;
    case ImPlot3DCol_InlayText:
        return QImPlot3DCol::InlayText;
    case ImPlot3DCol_FrameBg:
        return QImPlot3DCol::FrameBg;
    case ImPlot3DCol_PlotBg:
        return QImPlot3DCol::PlotBg;
    case ImPlot3DCol_PlotBorder:
        return QImPlot3DCol::PlotBorder;
    case ImPlot3DCol_LegendBg:
        return QImPlot3DCol::LegendBg;
    case ImPlot3DCol_LegendBorder:
        return QImPlot3DCol::LegendBorder;
    case ImPlot3DCol_LegendText:
        return QImPlot3DCol::LegendText;
    case ImPlot3DCol_AxisText:
        return QImPlot3DCol::AxisText;
    case ImPlot3DCol_AxisGrid:
        return QImPlot3DCol::AxisGrid;
    case ImPlot3DCol_AxisTick:
        return QImPlot3DCol::AxisTick;
    case ImPlot3DCol_COUNT:
        return QImPlot3DCol::COUNT;
    default:
        break;
    }
    return QImPlot3DCol::Line;
}

/**
 * \if ENGLISH
 * @brief Converts QImPlot3DStyleVar wrapper enum to ImPlot3D's native style variable enumeration value
 * @param v QImPlot3DStyleVar enum value (LineWeight to COUNT)
 * @return Corresponding ImPlot3DStyleVar_ enumeration value as integer
 * @details Performs direct 1:1 mapping between Qt wrapper enum and native ImPlot3D style variable types.
 * @note Invalid enum values default to ImPlot3DStyleVar_LineWeight for safety.
 * @see toQImPlot3DStyleVar()
 * \endif
 *
 * \if CHINESE
 * @brief 将 QImPlot3DStyleVar 包装枚举转换为 ImPlot3D 的原生样式变量枚举值
 * @param v QImPlot3DStyleVar 枚举值（LineWeight 至 COUNT）
 * @return 对应的 ImPlot3DStyleVar_ 枚举值（整数形式）
 * @details 执行 Qt 包装枚举与原生 ImPlot3D 样式变量类型之间的直接 1:1 映射。
 * @note 无效枚举值默认返回 ImPlot3DStyleVar_LineWeight 以保证安全。
 * @see toQImPlot3DStyleVar()
 * \endif
 */
int toImPlot3DStyleVar(QImPlot3DStyleVar v)
{
    switch (v) {
    case QImPlot3DStyleVar::LineWeight:
        return ImPlot3DStyleVar_LineWeight;
    case QImPlot3DStyleVar::Marker:
        return ImPlot3DStyleVar_Marker;
    case QImPlot3DStyleVar::MarkerSize:
        return ImPlot3DStyleVar_MarkerSize;
    case QImPlot3DStyleVar::MarkerWeight:
        return ImPlot3DStyleVar_MarkerWeight;
    case QImPlot3DStyleVar::FillAlpha:
        return ImPlot3DStyleVar_FillAlpha;
    case QImPlot3DStyleVar::PlotDefaultSize:
        return ImPlot3DStyleVar_PlotDefaultSize;
    case QImPlot3DStyleVar::PlotMinSize:
        return ImPlot3DStyleVar_PlotMinSize;
    case QImPlot3DStyleVar::PlotPadding:
        return ImPlot3DStyleVar_PlotPadding;
    case QImPlot3DStyleVar::LabelPadding:
        return ImPlot3DStyleVar_LabelPadding;
    case QImPlot3DStyleVar::ViewScaleFactor:
        return ImPlot3DStyleVar_ViewScaleFactor;
    case QImPlot3DStyleVar::LegendPadding:
        return ImPlot3DStyleVar_LegendPadding;
    case QImPlot3DStyleVar::LegendInnerPadding:
        return ImPlot3DStyleVar_LegendInnerPadding;
    case QImPlot3DStyleVar::LegendSpacing:
        return ImPlot3DStyleVar_LegendSpacing;
    case QImPlot3DStyleVar::COUNT:
        return ImPlot3DStyleVar_COUNT;
    default:
        break;
    }
    return ImPlot3DStyleVar_LineWeight;
}

/**
 * \if ENGLISH
 * @brief Converts ImPlot3D's style variable enumeration value to QImPlot3DStyleVar wrapper enum
 * @param v Raw ImPlot3DStyleVar_ enumeration value
 * @return Corresponding QImPlot3DStyleVar enum value
 * @details Performs direct 1:1 mapping between native ImPlot3D style variable types and Qt wrapper enum.
 * @note Invalid input values default to QImPlot3DStyleVar::LineWeight for safety.
 * @see toImPlot3DStyleVar()
 * \endif
 *
 * \if CHINESE
 * @brief 将 ImPlot3D 的样式变量枚举值转换为 QImPlot3DStyleVar 包装枚举
 * @param v 原始 ImPlot3DStyleVar_ 枚举值
 * @return 对应的 QImPlot3DStyleVar 枚举值
 * @details 执行原生 ImPlot3D 样式变量类型与 Qt 包装枚举之间的直接 1:1 映射。
 * @note 无效输入值默认返回 QImPlot3DStyleVar::LineWeight 以保证安全。
 * @see toImPlot3DStyleVar()
 * \endif
 */
QImPlot3DStyleVar toQImPlot3DStyleVar(int v)
{
    switch (v) {
    case ImPlot3DStyleVar_LineWeight:
        return QImPlot3DStyleVar::LineWeight;
    case ImPlot3DStyleVar_Marker:
        return QImPlot3DStyleVar::Marker;
    case ImPlot3DStyleVar_MarkerSize:
        return QImPlot3DStyleVar::MarkerSize;
    case ImPlot3DStyleVar_MarkerWeight:
        return QImPlot3DStyleVar::MarkerWeight;
    case ImPlot3DStyleVar_FillAlpha:
        return QImPlot3DStyleVar::FillAlpha;
    case ImPlot3DStyleVar_PlotDefaultSize:
        return QImPlot3DStyleVar::PlotDefaultSize;
    case ImPlot3DStyleVar_PlotMinSize:
        return QImPlot3DStyleVar::PlotMinSize;
    case ImPlot3DStyleVar_PlotPadding:
        return QImPlot3DStyleVar::PlotPadding;
    case ImPlot3DStyleVar_LabelPadding:
        return QImPlot3DStyleVar::LabelPadding;
    case ImPlot3DStyleVar_ViewScaleFactor:
        return QImPlot3DStyleVar::ViewScaleFactor;
    case ImPlot3DStyleVar_LegendPadding:
        return QImPlot3DStyleVar::LegendPadding;
    case ImPlot3DStyleVar_LegendInnerPadding:
        return QImPlot3DStyleVar::LegendInnerPadding;
    case ImPlot3DStyleVar_LegendSpacing:
        return QImPlot3DStyleVar::LegendSpacing;
    case ImPlot3DStyleVar_COUNT:
        return QImPlot3DStyleVar::COUNT;
    default:
        break;
    }
    return QImPlot3DStyleVar::LineWeight;
}

/**
 * \if ENGLISH
 * @brief Converts QImPlot3DLocation wrapper enum to ImPlot3D's native location enumeration value
 * @param v QImPlot3DLocation enum value (Center/North/South/West/East/NorthWest/NorthEast/SouthWest/SouthEast)
 * @return Corresponding ImPlot3DLocation_ enumeration value as integer
 * @details Performs direct 1:1 mapping between Qt wrapper enum and native ImPlot3D location types.
 * @note Invalid enum values default to ImPlot3DLocation_Center for safety.
 * @see toQImPlot3DLocation()
 * \endif
 *
 * \if CHINESE
 * @brief 将 QImPlot3DLocation 包装枚举转换为 ImPlot3D 的原生位置枚举值
 * @param v QImPlot3DLocation 枚举值（Center/North/South/West/East/NorthWest/NorthEast/SouthWest/SouthEast）
 * @return 对应的 ImPlot3DLocation_ 枚举值（整数形式）
 * @details 执行 Qt 包装枚举与原生 ImPlot3D 位置类型之间的直接 1:1 映射。
 * @note 无效枚举值默认返回 ImPlot3DLocation_Center 以保证安全。
 * @see toQImPlot3DLocation()
 * \endif
 */
int toImPlot3DLocation(QImPlot3DLocation v)
{
    return static_cast<int>(v);
}

/**
 * \if ENGLISH
 * @brief Converts ImPlot3D's location enumeration value to QImPlot3DLocation wrapper enum
 * @param v Raw ImPlot3DLocation_ enumeration value
 * @return Corresponding QImPlot3DLocation enum value
 * @details Performs direct 1:1 mapping between native ImPlot3D location types and Qt wrapper enum.
 * @note Invalid input values default to QImPlot3DLocation::Center for safety.
 * @see toImPlot3DLocation()
 * \endif
 *
 * \if CHINESE
 * @brief 将 ImPlot3D 的位置枚举值转换为 QImPlot3DLocation 包装枚举
 * @param v 原始 ImPlot3DLocation_ 枚举值
 * @return 对应的 QImPlot3DLocation 枚举值
 * @details 执行原生 ImPlot3D 位置类型与 Qt 包装枚举之间的直接 1:1 映射。
 * @note 无效输入值默认返回 QImPlot3DLocation::Center 以保证安全。
 * @see toImPlot3DLocation()
 * \endif
 */
QImPlot3DLocation toQImPlot3DLocation(int v)
{
    // Location is a flags-style enum with identical numeric values.
    // Valid bits: North(1<<0), South(1<<1), West(1<<2), East(1<<3)
    // Accept any valid combination (0..15); reject out-of-range values.
    const int validMask = ImPlot3DLocation_North | ImPlot3DLocation_South | ImPlot3DLocation_West | ImPlot3DLocation_East;
    if ((v & ~validMask) == 0) {
        return static_cast<QImPlot3DLocation>(v);
    }
    return QImPlot3DLocation::Center;
}

/**
 * \if ENGLISH
 * @brief Converts QImPlot3DLegendFlags wrapper enum to ImPlot3D's native legend flags enumeration value
 * @param v QImPlot3DLegendFlags enum value (None/NoButtons/NoHighlightItem/Horizontal)
 * @return Corresponding ImPlot3DLegendFlags_ enumeration value as integer
 * @details Performs direct 1:1 mapping between Qt wrapper enum and native ImPlot3D legend flags.
 * @note Invalid enum values default to ImPlot3DLegendFlags_None for safety.
 * @see toQImPlot3DLegendFlags()
 * \endif
 *
 * \if CHINESE
 * @brief 将 QImPlot3DLegendFlags 包装枚举转换为 ImPlot3D 的原生图例标志枚举值
 * @param v QImPlot3DLegendFlags 枚举值（None/NoButtons/NoHighlightItem/Horizontal）
 * @return 对应的 ImPlot3DLegendFlags_ 枚举值（整数形式）
 * @details 执行 Qt 包装枚举与原生 ImPlot3D 图例标志之间的直接 1:1 映射。
 * @note 无效枚举值默认返回 ImPlot3DLegendFlags_None 以保证安全。
 * @see toQImPlot3DLegendFlags()
 * \endif
 */
int toImPlot3DLegendFlags(QImPlot3DLegendFlags v)
{
    return static_cast<int>(v);
}

/**
 * \if ENGLISH
 * @brief Converts ImPlot3D's legend flags enumeration value to QImPlot3DLegendFlags wrapper enum
 * @param v Raw ImPlot3DLegendFlags_ enumeration value
 * @return Corresponding QImPlot3DLegendFlags enum value
 * @details Performs direct 1:1 mapping between native ImPlot3D legend flags and Qt wrapper enum.
 * @note Invalid input values default to QImPlot3DLegendFlags::None for safety.
 * @see toImPlot3DLegendFlags()
 * \endif
 *
 * \if CHINESE
 * @brief 将 ImPlot3D 的图例标志枚举值转换为 QImPlot3DLegendFlags 包装枚举
 * @param v 原始 ImPlot3DLegendFlags_ 枚举值
 * @return 对应的 QImPlot3DLegendFlags 枚举值
 * @details 执行原生 ImPlot3D 图例标志与 Qt 包装枚举之间的直接 1:1 映射。
 * @note 无效输入值默认返回 QImPlot3DLegendFlags::None 以保证安全。
 * @see toImPlot3DLegendFlags()
 * \endif
 */
QImPlot3DLegendFlags toQImPlot3DLegendFlags(int v)
{
    // Flags enum: numeric values are 1:1 identical (bit positions match).
    // Valid bits: NoButtons(1<<0), NoHighlightItem(1<<1), Horizontal(1<<2)
    // Accept any valid combination (0..7); reject out-of-range values.
    const int validMask = ImPlot3DLegendFlags_NoButtons | ImPlot3DLegendFlags_NoHighlightItem | ImPlot3DLegendFlags_Horizontal;
    if ((v & ~validMask) == 0) {
        return static_cast<QImPlot3DLegendFlags>(v);
    }
    return QImPlot3DLegendFlags::None;
}

/**
 * \if ENGLISH
 * @brief Converts QImPlane3D wrapper enum to ImPlot3D's native plane enumeration value
 * @param v QImPlane3D enum value (YZ/XZ/XY)
 * @return Corresponding ImPlane3D_ enumeration value as integer
 * @details Performs direct 1:1 mapping between Qt wrapper enum and native ImPlot3D plane types.
 * @note Invalid enum values default to ImPlane3D_YZ for safety.
 * @see toQImPlane3D()
 * \endif
 *
 * \if CHINESE
 * @brief 将 QImPlane3D 包装枚举转换为 ImPlot3D 的原生平面枚举值
 * @param v QImPlane3D 枚举值（YZ/XZ/XY）
 * @return 对应的 ImPlane3D_ 枚举值（整数形式）
 * @details 执行 Qt 包装枚举与原生 ImPlot3D 平面类型之间的直接 1:1 映射。
 * @note 无效枚举值默认返回 ImPlane3D_YZ 以保证安全。
 * @see toQImPlane3D()
 * \endif
 */
int toImPlane3D(QImPlane3D v)
{
    switch (v) {
    case QImPlane3D::YZ:
        return ImPlane3D_YZ;
    case QImPlane3D::XZ:
        return ImPlane3D_XZ;
    case QImPlane3D::XY:
        return ImPlane3D_XY;
    default:
        break;
    }
    return ImPlane3D_YZ;
}

/**
 * \if ENGLISH
 * @brief Converts ImPlot3D's plane enumeration value to QImPlane3D wrapper enum
 * @param v Raw ImPlane3D_ enumeration value
 * @return Corresponding QImPlane3D enum value
 * @details Performs direct 1:1 mapping between native ImPlot3D plane types and Qt wrapper enum.
 * @note Invalid input values default to QImPlane3D::YZ for safety.
 * @see toImPlane3D()
 * \endif
 *
 * \if CHINESE
 * @brief 将 ImPlot3D 的平面枚举值转换为 QImPlane3D 包装枚举
 * @param v 原始 ImPlane3D_ 枚举值
 * @return 对应的 QImPlane3D 枚举值
 * @details 执行原生 ImPlot3D 平面类型与 Qt 包装枚举之间的直接 1:1 映射。
 * @note 无效输入值默认返回 QImPlane3D::YZ 以保证安全。
 * @see toImPlane3D()
 * \endif
 */
QImPlane3D toQImPlane3D(int v)
{
    switch (v) {
    case ImPlane3D_YZ:
        return QImPlane3D::YZ;
    case ImPlane3D_XZ:
        return QImPlane3D::XZ;
    case ImPlane3D_XY:
        return QImPlane3D::XY;
    default:
        break;
    }
    return QImPlane3D::YZ;
}

}