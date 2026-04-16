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

}