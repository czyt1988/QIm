#ifndef QTIMGUIUTILS_H
#define QTIMGUIUTILS_H
#include <QString>
#include <QFont>
#include <QColor>
#include <QSize>
#include <QDebug>
#include <QPointF>
#include <vector>
#include "QImAPI.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "implot.h"
/**
 *@file 这个文件处理qt和imgui的一些类型转换和一些辅助工具
 */
namespace QIM
{
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
// 模糊判断相等
// 更简单的fuzzyEqual，可指定精度，不使用qFuzzyCompare，当前场景不需要使用相对误差或ULP-based 比较
QIM_CORE_API bool fuzzyEqual(float a, float b, float epsilon = 1e-5f);
QIM_CORE_API bool fuzzyEqual(double a, double b, double epsilon = 1e-9);
QIM_CORE_API bool fuzzyEqual(const ImVec2& a, const ImVec2& b, float epsilon = 1e-5f);
QIM_CORE_API bool fuzzyEqual(const ImVec4& a, const ImVec4& b, float epsilon = 1e-5f);
template< typename T >
bool fuzzyEqual(const std::vector< T >& a, const std::vector< T >& b);
// QDebug Support
QIM_CORE_API QDebug operator<<(QDebug debug, const ImVec2& v);
QIM_CORE_API QDebug operator<<(QDebug debug, const ImVec4& v);
QIM_CORE_API QDebug operator<<(QDebug debug, const ImPlotPoint& v);
QIM_CORE_API QDebug operator<<(QDebug debug, const ImRect& v);
// ImVec2/ImVec4的模糊比较器
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
bool fuzzyEqual(const std::vector< T >& a, const std::vector< T >& b)
{
    if (a.size() != b.size()) {
        return false;
    }

    for (size_t i = 0; i < a.size(); ++i) {
        if (!qFuzzyCompare(a[ i ], b[ i ])) {
            return false;
        }
    }

    return true;
}

template< typename T >
inline ImVecComparator< T >::ImVecComparator()
{
}

template< typename T >
inline bool ImVecComparator< T >::operator()(const T& a, const T& b) const
{
    return fuzzyEqual(a, b);
}
}
#endif  // QTIMGUIUTILS_H
