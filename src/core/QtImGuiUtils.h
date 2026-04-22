#ifndef QTIMGUIUTILS_H
#define QTIMGUIUTILS_H
#include <QString>
#include <QFont>
#include <QColor>
#include <QSize>
#include <QDebug>
#include <QPointF>
#include <QQuaternion>
#include <vector>
#include "QImAPI.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "implot.h"
#include "implot3d.h"
/**
 * \if ENGLISH
 * @file QtImGuiUtils.h
 * @brief Type conversion utilities between Qt and ImGui/ImPlot types
 *
 * Provides helpers for converting QColor ↔ ImVec4, QSize ↔ ImVec2,
 * QPointF ↔ ImPlotPoint, and other Qt-ImGui type bridging functions.
 * \endif
 *
 * \if CHINESE
 * @file QtImGuiUtils.h
 * @brief Qt与ImGui类型转换及辅助工具
 *
 * 这个文件处理qt和imgui的一些类型转换和一些辅助工具
 * \endif
 */
namespace QIM
{
// Forward declarations for 3D math types (Task 4 will add conversion functions)
struct QImPlot3DPoint;
struct QImPlot3DRay;
struct QImPlot3DPlane;
struct QImPlot3DBox;
struct QImPlot3DRange;

// 操作系统是否为暗色主题
QIM_CORE_API bool isSystemDarkTheme();
// QColor < - > ImVec4
QIM_CORE_API ImVec4 toImVec4(const QColor& color);
QIM_CORE_API QColor toQColor(const ImVec4& color);
// ImVec2 < - > QSize
QIM_CORE_API ImVec2 toImVec2(const QSize& v);
QIM_CORE_API QSize toQSize(const ImVec2& v);
// ImVec2 < - > QSizeF
QIM_CORE_API ImVec2 toImVec2(const QSizeF& v);
QIM_CORE_API QSizeF toQSizeF(const ImVec2& v);
// ImPlotPoint < - > QPointF
QIM_CORE_API ImPlotPoint toImPlotPoint(const QPointF& v);
QIM_CORE_API QPointF toQPointF(const ImPlotPoint& v);
// ImVec2 < - > QPointF
QIM_CORE_API ImVec2 toImVec2(const QPointF& v);
QIM_CORE_API QPointF toQPointF(const ImVec2& v);
// ImU32 < - > QColors
QIM_CORE_API ImU32 toImU32(const QColor& color);
QIM_CORE_API QColor toQColor(ImU32 color);
// ImPlotLocation < - > Qt::Alignment
QIM_CORE_API Qt::Alignment toQAlignment(ImPlotLocation location);
QIM_CORE_API ImPlotLocation toImPlotLocation(Qt::Alignment alignment);
// ImPlot3D < - > QImPlot3D type conversions
QIM_CORE_API ImPlot3DPoint toImPlot3DPoint(const QImPlot3DPoint& v);
QIM_CORE_API QImPlot3DPoint toQImPlot3DPoint(const ImPlot3DPoint& v);
QIM_CORE_API ImPlot3DQuat toImPlot3DQuat(const QQuaternion& q);
QIM_CORE_API QQuaternion toQQuaternion(const ImPlot3DQuat& q);
QIM_CORE_API ImPlot3DRay toImPlot3DRay(const QImPlot3DRay& r);
QIM_CORE_API QImPlot3DRay toQImPlot3DRay(const ImPlot3DRay& r);
QIM_CORE_API ImPlot3DPlane toImPlot3DPlane(const QImPlot3DPlane& p);
QIM_CORE_API QImPlot3DPlane toQImPlot3DPlane(const ImPlot3DPlane& p);
QIM_CORE_API ImPlot3DBox toImPlot3DBox(const QImPlot3DBox& b);
QIM_CORE_API QImPlot3DBox toQImPlot3DBox(const ImPlot3DBox& b);
QIM_CORE_API ImPlot3DRange toImPlot3DRange(const QImPlot3DRange& r);
QIM_CORE_API QImPlot3DRange toQImPlot3DRange(const ImPlot3DRange& r);
// 模糊判断相等
// 更简单的fuzzyEqual，可指定精度，不使用qFuzzyCompare，当前场景不需要使用相对误差或ULP-based 比较
QIM_CORE_API bool fuzzyEqual(float a, float b, float epsilon = 1e-5f);
QIM_CORE_API bool fuzzyEqual(double a, double b, double epsilon = 1e-9);
QIM_CORE_API bool fuzzyEqual(const ImVec2& a, const ImVec2& b, float epsilon = 1e-5f);
QIM_CORE_API bool fuzzyEqual(const ImVec4& a, const ImVec4& b, float epsilon = 1e-5f);
QIM_CORE_API bool fuzzyEqual(const QImPlot3DPoint& a, const QImPlot3DPoint& b, double epsilon = 1e-9);
QIM_CORE_API bool fuzzyEqual(const QImPlot3DBox& a, const QImPlot3DBox& b, double epsilon = 1e-9);
QIM_CORE_API bool fuzzyEqual(const QImPlot3DRange& a, const QImPlot3DRange& b, double epsilon = 1e-9);
template< typename T >
bool fuzzyEqual(const std::vector< T >& a, const std::vector< T >& b, T epsilon = 1e-5f);
// QDebug Support
QIM_CORE_API QDebug operator<<(QDebug debug, const ImVec2& v);
QIM_CORE_API QDebug operator<<(QDebug debug, const ImVec4& v);
QIM_CORE_API QDebug operator<<(QDebug debug, const ImPlotPoint& v);
QIM_CORE_API QDebug operator<<(QDebug debug, const ImRect& v);
QIM_CORE_API QDebug operator<<(QDebug debug, const QImPlot3DPoint& v);
QIM_CORE_API QDebug operator<<(QDebug debug, const QImPlot3DRay& v);
QIM_CORE_API QDebug operator<<(QDebug debug, const QImPlot3DPlane& v);
QIM_CORE_API QDebug operator<<(QDebug debug, const QImPlot3DBox& v);
QIM_CORE_API QDebug operator<<(QDebug debug, const QImPlot3DRange& v);
// ImVec2/ImVec4的模糊比较器 - 返回 true 表示"不相等"(需要更新)
// 用于 QImTrackedValue 的 comparator，语义与 DefaultComparator 一致
template< typename T >
class ImVecComparator
{
public:
    explicit ImVecComparator();
    bool operator()(const T& a, const T& b) const;
};

}

namespace QIM
{
template< typename T >
bool fuzzyEqual(const std::vector< T >& a, const std::vector< T >& b, T epsilon)
{
    if (a.size() != b.size()) {
        return false;
    }

    for (size_t i = 0; i < a.size(); ++i) {
        if (!fuzzyEqual(a[ i ], b[ i ], epsilon)) {
            return false;
        }
    }

    return true;
}

template< typename T >
inline ImVecComparator< T >::ImVecComparator()
{
}

// 返回 true 表示"不相等"(需要更新)，与 DefaultComparator 语义一致
template< typename T >
inline bool ImVecComparator< T >::operator()(const T& a, const T& b) const
{
    return !fuzzyEqual(a, b);  // 不相等返回 true
}
}
#endif  // QTIMGUIUTILS_H
