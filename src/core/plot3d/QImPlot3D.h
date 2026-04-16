#ifndef QIMPLOT3D_H
#define QIMPLOT3D_H
#include <QMetaType>
#include "QImAPI.h"
namespace QIM
{
/**
 * @brief 对应ImAxis3D
 */
enum class QImPlot3DAxisId
{
    X1 = 0,     ///< ImAxis3D_::ImAxis3D_X
    Y1 = 1,     ///< ImAxis3D_::ImAxis3D_Y
    Z1 = 2,     ///< ImAxis3D_::ImAxis3D_Z
    AxisCount   ///< ImAxis3D_::ImAxis3D_COUNT
};

/**
 * @brief 对应ImPlot3DScale
 */
enum class QImPlot3DScaleType
{
    Linear = 0,  ///< ImPlot3DScale_::ImPlot3DScale_Linear
    Log10,       ///< ImPlot3DScale_::ImPlot3DScale_Log10
    SymLog       ///< ImPlot3DScale_::ImPlot3DScale_SymLog
};

/**
 * @brief 对应ImPlot3DCond
 */
enum class QImPlot3DCondition
{
    None = 0,  ///< ImPlot3DCond_::ImPlot3DCond_None
    Always,    ///< ImPlot3DCond_::ImPlot3DCond_Always
    Once       ///< ImPlot3DCond_::ImPlot3DCond_Once
};

/**
 * @brief 对应ImPlot3DMarker
 */
enum class QImPlot3DMarkerShape
{
    None = -1,     ///< ImPlot3DMarker_::ImPlot3DMarker_None
    Circle = 0,    ///< ImPlot3DMarker_::ImPlot3DMarker_Circle
    Square = 1,    ///< ImPlot3DMarker_::ImPlot3DMarker_Square
    Diamond = 2,   ///< ImPlot3DMarker_::ImPlot3DMarker_Diamond
    Up = 3,        ///< ImPlot3DMarker_::ImPlot3DMarker_Up
    Down = 4,      ///< ImPlot3DMarker_::ImPlot3DMarker_Down
    Left = 5,      ///< ImPlot3DMarker_::ImPlot3DMarker_Left
    Right = 6,     ///< ImPlot3DMarker_::ImPlot3DMarker_Right
    Cross = 7,     ///< ImPlot3DMarker_::ImPlot3DMarker_Cross
    Plus = 8,      ///< ImPlot3DMarker_::ImPlot3DMarker_Plus
    Asterisk = 9   ///< ImPlot3DMarker_::ImPlot3DMarker_Asterisk
};

// Converts QImPlot3DAxisId enumeration to ImPlot3D's ImAxis3D enumeration value
QIM_CORE_API int toImAxis3D(QImPlot3DAxisId axis);
// Converts ImPlot3D's ImAxis3D enumeration to QImPlot3DAxisId wrapper enum
QIM_CORE_API QImPlot3DAxisId toQImPlot3DAxisId(int axis);

// Converts QImPlot3DScaleType wrapper enum to ImPlot3D's native scale enumeration value
QIM_CORE_API int toImPlot3DScale(QImPlot3DScaleType v);
// Converts ImPlot3D's scale enumeration value to QImPlot3DScaleType wrapper enum
QIM_CORE_API QImPlot3DScaleType toQImPlot3DScaleType(int v);

// Converts QImPlot3DCondition wrapper enum to ImPlot3D's native condition enumeration value
QIM_CORE_API int toImPlot3DCond(QImPlot3DCondition v);
// Converts ImPlot3D's condition enumeration value to QImPlot3DCondition wrapper enum
QIM_CORE_API QImPlot3DCondition toQImPlot3DCondition(int v);

// Converts QImPlot3DMarkerShape wrapper enum to ImPlot3D's native marker enumeration value
QIM_CORE_API int toImPlot3DMarker(QImPlot3DMarkerShape v);
// Converts ImPlot3D's marker enumeration value to QImPlot3DMarkerShape wrapper enum
QIM_CORE_API QImPlot3DMarkerShape toQImPlot3DMarkerShape(int v);

}

Q_DECLARE_METATYPE(QIM::QImPlot3DAxisId)
Q_DECLARE_METATYPE(QIM::QImPlot3DScaleType)
Q_DECLARE_METATYPE(QIM::QImPlot3DCondition)
Q_DECLARE_METATYPE(QIM::QImPlot3DMarkerShape)

// ===== 辅助宏定义=====
#ifndef QIMPLOT3D_FLAG_ACCESSOR
#define QIMPLOT3D_FLAG_ACCESSOR(ClassName, FlagName, FlagEnum, emitFunName)                                              \
    bool ClassName::is##FlagName() const                                                                                 \
    {                                                                                                                    \
        return (d_ptr->flags & FlagEnum) != 0;                                                                           \
    }                                                                                                                    \
    void ClassName::set##FlagName(bool on)                                                                               \
    {                                                                                                                    \
        QIM_D(d);                                                                                                        \
        const auto oldFlags = d->flags;                                                                                  \
        if (on) {                                                                                                        \
            d->flags |= FlagEnum;                                                                                        \
        } else {                                                                                                         \
            d->flags &= ~FlagEnum;                                                                                       \
        }                                                                                                                \
        if (d->flags != oldFlags) {                                                                                      \
            Q_EMIT emitFunName();                                                                                        \
        }                                                                                                                \
    }
#endif
// 这个针对 NOXXFlag 进行设置，反转为肯定语义
#ifndef QIMPLOT3D_FLAG_ENABLED_ACCESSOR
#define QIMPLOT3D_FLAG_ENABLED_ACCESSOR(ClassName, PropName, FlagEnum, emitFunName)                                      \
    bool ClassName::is##PropName() const                                                                                 \
    {                                                                                                                    \
        return (d_ptr->flags & FlagEnum) == 0;                                                                           \
    }                                                                                                                    \
    void ClassName::set##PropName(bool enabled)                                                                          \
    {                                                                                                                    \
        QIM_D(d);                                                                                                        \
        const auto oldFlags = d->flags;                                                                                  \
        if (enabled) {                                                                                                   \
            d->flags &= ~FlagEnum;                                                                                       \
        } else {                                                                                                         \
            d->flags |= FlagEnum;                                                                                        \
        }                                                                                                                \
        if (d->flags != oldFlags) {                                                                                      \
            Q_EMIT emitFunName();                                                                                        \
        }                                                                                                                \
    }
#endif

#endif  // QIMPLOT3D_H