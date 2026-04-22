#ifndef QIMPLOT3D_H
#define QIMPLOT3D_H
#include <QMetaType>
#include "QImAPI.h"
#include "QImPlot3DTypes.h"

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

/**
 * @brief 对应ImPlot3DColormap
 */
enum class QImPlot3DColormap
{
    Deep = 0,      ///< ImPlot3DColormap_::ImPlot3DColormap_Deep
    Dark = 1,      ///< ImPlot3DColormap_::ImPlot3DColormap_Dark
    Pastel = 2,    ///< ImPlot3DColormap_::ImPlot3DColormap_Pastel
    Paired = 3,    ///< ImPlot3DColormap_::ImPlot3DColormap_Paired
    Viridis = 4,   ///< ImPlot3DColormap_::ImPlot3DColormap_Viridis
    Plasma = 5,    ///< ImPlot3DColormap_::ImPlot3DColormap_Plasma
    Hot = 6,       ///< ImPlot3DColormap_::ImPlot3DColormap_Hot
    Cool = 7,      ///< ImPlot3DColormap_::ImPlot3DColormap_Cool
    Pink = 8,      ///< ImPlot3DColormap_::ImPlot3DColormap_Pink
    Jet = 9,       ///< ImPlot3DColormap_::ImPlot3DColormap_Jet
    Twilight = 10, ///< ImPlot3DColormap_::ImPlot3DColormap_Twilight
    RdBu = 11,     ///< ImPlot3DColormap_::ImPlot3DColormap_RdBu
    BrBG = 12,     ///< ImPlot3DColormap_::ImPlot3DColormap_BrBG
    PiYG = 13,     ///< ImPlot3DColormap_::ImPlot3DColormap_PiYG
    Spectral = 14, ///< ImPlot3DColormap_::ImPlot3DColormap_Spectral
    Greys = 15     ///< ImPlot3DColormap_::ImPlot3DColormap_Greys
};

/**
 * @brief 对应ImPlot3DCol
 */
enum class QImPlot3DCol
{
    Line = 0,           ///< ImPlot3DCol_::ImPlot3DCol_Line
    Fill = 1,           ///< ImPlot3DCol_::ImPlot3DCol_Fill
    MarkerOutline = 2,  ///< ImPlot3DCol_::ImPlot3DCol_MarkerOutline
    MarkerFill = 3,     ///< ImPlot3DCol_::ImPlot3DCol_MarkerFill
    TitleText = 4,      ///< ImPlot3DCol_::ImPlot3DCol_TitleText
    InlayText = 5,      ///< ImPlot3DCol_::ImPlot3DCol_InlayText
    FrameBg = 6,        ///< ImPlot3DCol_::ImPlot3DCol_FrameBg
    PlotBg = 7,         ///< ImPlot3DCol_::ImPlot3DCol_PlotBg
    PlotBorder = 8,     ///< ImPlot3DCol_::ImPlot3DCol_PlotBorder
    LegendBg = 9,       ///< ImPlot3DCol_::ImPlot3DCol_LegendBg
    LegendBorder = 10,  ///< ImPlot3DCol_::ImPlot3DCol_LegendBorder
    LegendText = 11,    ///< ImPlot3DCol_::ImPlot3DCol_LegendText
    AxisText = 12,      ///< ImPlot3DCol_::ImPlot3DCol_AxisText
    AxisGrid = 13,      ///< ImPlot3DCol_::ImPlot3DCol_AxisGrid
    AxisTick = 14,      ///< ImPlot3DCol_::ImPlot3DCol_AxisTick
    COUNT = 15          ///< ImPlot3DCol_::ImPlot3DCol_COUNT
};

/**
 * @brief 对应ImPlot3DStyleVar
 */
enum class QImPlot3DStyleVar
{
    LineWeight = 0,         ///< ImPlot3DStyleVar_::ImPlot3DStyleVar_LineWeight
    Marker = 1,             ///< ImPlot3DStyleVar_::ImPlot3DStyleVar_Marker
    MarkerSize = 2,         ///< ImPlot3DStyleVar_::ImPlot3DStyleVar_MarkerSize
    MarkerWeight = 3,       ///< ImPlot3DStyleVar_::ImPlot3DStyleVar_MarkerWeight
    FillAlpha = 4,          ///< ImPlot3DStyleVar_::ImPlot3DStyleVar_FillAlpha
    PlotDefaultSize = 5,    ///< ImPlot3DStyleVar_::ImPlot3DStyleVar_PlotDefaultSize
    PlotMinSize = 6,        ///< ImPlot3DStyleVar_::ImPlot3DStyleVar_PlotMinSize
    PlotPadding = 7,        ///< ImPlot3DStyleVar_::ImPlot3DStyleVar_PlotPadding
    LabelPadding = 8,       ///< ImPlot3DStyleVar_::ImPlot3DStyleVar_LabelPadding
    ViewScaleFactor = 9,    ///< ImPlot3DStyleVar_::ImPlot3DStyleVar_ViewScaleFactor
    LegendPadding = 10,     ///< ImPlot3DStyleVar_::ImPlot3DStyleVar_LegendPadding
    LegendInnerPadding = 11,///< ImPlot3DStyleVar_::ImPlot3DStyleVar_LegendInnerPadding
    LegendSpacing = 12,     ///< ImPlot3DStyleVar_::ImPlot3DStyleVar_LegendSpacing
    COUNT = 13              ///< ImPlot3DStyleVar_::ImPlot3DStyleVar_COUNT
};

/**
 * @brief 对应ImPlot3DLocation (flags enum)
 */
enum class QImPlot3DLocation : int
{
    Center = 0,                     ///< ImPlot3DLocation_::ImPlot3DLocation_Center
    North = 1 << 0,                 ///< ImPlot3DLocation_::ImPlot3DLocation_North
    South = 1 << 1,                 ///< ImPlot3DLocation_::ImPlot3DLocation_South
    West = 1 << 2,                  ///< ImPlot3DLocation_::ImPlot3DLocation_West
    East = 1 << 3,                  ///< ImPlot3DLocation_::ImPlot3DLocation_East
    NorthWest = North | West,       ///< ImPlot3DLocation_::ImPlot3DLocation_NorthWest
    NorthEast = North | East,       ///< ImPlot3DLocation_::ImPlot3DLocation_NorthEast
    SouthWest = South | West,       ///< ImPlot3DLocation_::ImPlot3DLocation_SouthWest
    SouthEast = South | East        ///< ImPlot3DLocation_::ImPlot3DLocation_SouthEast
};

/**
 * @brief 对应ImPlot3DLegendFlags (flags enum)
 */
enum class QImPlot3DLegendFlags : int
{
    None = 0,                       ///< ImPlot3DLegendFlags_::ImPlot3DLegendFlags_None
    NoButtons = 1 << 0,             ///< ImPlot3DLegendFlags_::ImPlot3DLegendFlags_NoButtons
    NoHighlightItem = 1 << 1,       ///< ImPlot3DLegendFlags_::ImPlot3DLegendFlags_NoHighlightItem
    Horizontal = 1 << 2             ///< ImPlot3DLegendFlags_::ImPlot3DLegendFlags_Horizontal
};

/**
 * @brief 对应ImPlane3D
 */
enum class QImPlane3D
{
    YZ = 0,     ///< ImPlane3D_::ImPlane3D_YZ
    XZ = 1,     ///< ImPlane3D_::ImPlane3D_XZ
    XY = 2      ///< ImPlane3D_::ImPlane3D_XY
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

// Converts QImPlot3DColormap wrapper enum to ImPlot3D's native colormap enumeration value
QIM_CORE_API int toImPlot3DColormap(QImPlot3DColormap v);
// Converts ImPlot3D's colormap enumeration value to QImPlot3DColormap wrapper enum
QIM_CORE_API QImPlot3DColormap toQImPlot3DColormap(int v);

// Converts QImPlot3DCol wrapper enum to ImPlot3D's native color enumeration value
QIM_CORE_API int toImPlot3DCol(QImPlot3DCol v);
// Converts ImPlot3D's color enumeration value to QImPlot3DCol wrapper enum
QIM_CORE_API QImPlot3DCol toQImPlot3DCol(int v);

// Converts QImPlot3DStyleVar wrapper enum to ImPlot3D's native style variable enumeration value
QIM_CORE_API int toImPlot3DStyleVar(QImPlot3DStyleVar v);
// Converts ImPlot3D's style variable enumeration value to QImPlot3DStyleVar wrapper enum
QIM_CORE_API QImPlot3DStyleVar toQImPlot3DStyleVar(int v);

// Converts QImPlot3DLocation wrapper enum to ImPlot3D's native location enumeration value
QIM_CORE_API int toImPlot3DLocation(QImPlot3DLocation v);
// Converts ImPlot3D's location enumeration value to QImPlot3DLocation wrapper enum
QIM_CORE_API QImPlot3DLocation toQImPlot3DLocation(int v);

// Converts QImPlot3DLegendFlags wrapper enum to ImPlot3D's native legend flags enumeration value
QIM_CORE_API int toImPlot3DLegendFlags(QImPlot3DLegendFlags v);
// Converts ImPlot3D's legend flags enumeration value to QImPlot3DLegendFlags wrapper enum
QIM_CORE_API QImPlot3DLegendFlags toQImPlot3DLegendFlags(int v);

// Converts QImPlane3D wrapper enum to ImPlot3D's native plane enumeration value
QIM_CORE_API int toImPlane3D(QImPlane3D v);
// Converts ImPlot3D's plane enumeration value to QImPlane3D wrapper enum
QIM_CORE_API QImPlane3D toQImPlane3D(int v);

}

Q_DECLARE_METATYPE(QIM::QImPlot3DAxisId)
Q_DECLARE_METATYPE(QIM::QImPlot3DScaleType)
Q_DECLARE_METATYPE(QIM::QImPlot3DCondition)
Q_DECLARE_METATYPE(QIM::QImPlot3DMarkerShape)
Q_DECLARE_METATYPE(QIM::QImPlot3DColormap)
Q_DECLARE_METATYPE(QIM::QImPlot3DCol)
Q_DECLARE_METATYPE(QIM::QImPlot3DStyleVar)
Q_DECLARE_METATYPE(QIM::QImPlot3DLocation)
Q_DECLARE_METATYPE(QIM::QImPlot3DLegendFlags)
Q_DECLARE_METATYPE(QIM::QImPlane3D)
Q_DECLARE_METATYPE(QIM::QImPlot3DPoint)
Q_DECLARE_METATYPE(QIM::QImPlot3DRay)
Q_DECLARE_METATYPE(QIM::QImPlot3DPlane)
Q_DECLARE_METATYPE(QIM::QImPlot3DBox)
Q_DECLARE_METATYPE(QIM::QImPlot3DRange)

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