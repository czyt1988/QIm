#include "QtImGuiUtils.h"
#include <QGuiApplication>
#include <QPalette>
#include <QDebug>

namespace QIM
{

/**
 * @brief 操作系统是否为暗色主题
 * @return
 */
bool isSystemDarkTheme()
{
    QPalette palette  = QGuiApplication::palette();
    QColor background = palette.color(QPalette::Window);
    // 计算亮度（YIQ 公式或简单 RGB 平均）使用 YIQ 亮度公式（权重 0.299R + 0.587G + 0.114B）更符合人眼感知
    int brightness = (background.red() * 299 + background.green() * 587 + background.blue() * 114) / 1000;
    return brightness < 128;  // 阈值 128（0-255 范围）
}

/**
 * @brief QColor和ImVec4的转换
 * @param color
 * @return
 */
ImVec4 toImVec4(const QColor& color)
{
    return ImVec4(color.redF(), color.greenF(), color.blueF(), color.alphaF());
}

/**
 * @brief ImVec4和QColor的转换
 * @param color
 * @return
 */
QColor toQColor(const ImVec4& color)
{
    return QColor::fromRgbF(color.x, color.y, color.z, color.w);
}

/**
 * @brief imVec2 cast to QSize
 * @param s
 * @return
 */
QSize toQSize(const ImVec2& v)
{
    return QSize(static_cast< int >(v.x), static_cast< int >(v.y));
}

/**
 * @brief QSize To imVec2
 * @param v
 * @return
 */
ImVec2 toImVec2(const QSize& v)
{
    return ImVec2(v.width(), v.height());
}

/**
 * @brief QSizeF To imVec2
 * @param v
 * @return
 */
ImVec2 toImVec2(const QSizeF& v)
{
    return ImVec2(v.width(), v.height());
}

/**
 * @brief imVec2 cast to QSizeF
 * @param s
 * @return
 */
QSizeF toQSizeF(const ImVec2& v)
{
    return QSizeF(static_cast< qreal >(v.x), static_cast< qreal >(v.y));
}

/**
 * @brief 模糊判断两个 float 是否相等（支持自定义精度）
 * @param a 第一个值
 * @param b 第二个值
 * @param epsilon 容差（默认 1e-5f）
 * @return 若 |a - b| <= epsilon，则返回 true
 */
bool fuzzyEqual(float a, float b, float epsilon)
{
    return std::abs(a - b) <= epsilon;
}

/**
 * @brief 模糊判断两个 double 是否相等（支持自定义精度）
 * @param a 第一个值
 * @param b 第二个值
 * @param epsilon 容差（默认 1e-9）
 * @return 若 |a - b| <= epsilon，则返回 true
 */
bool fuzzyEqual(double a, double b, double epsilon)
{
    return std::abs(a - b) <= epsilon;
}

/**
 * @brief 模糊判断是否相等
 * @param a
 * @param b
 * @return
 */
bool fuzzyEqual(const ImVec2& a, const ImVec2& b, float epsilon)
{
    return fuzzyEqual(a.x, b.x, epsilon) && fuzzyEqual(a.y, b.y, epsilon);
}

/**
 * @brief 模糊判断是否相等
 * @param a
 * @param b
 * @return
 */
bool fuzzyEqual(const ImVec4& a, const ImVec4& b, float epsilon)
{
    return fuzzyEqual(a.x, b.x, epsilon) && fuzzyEqual(a.y, b.y, epsilon) && fuzzyEqual(a.z, b.z, epsilon)
           && fuzzyEqual(a.w, b.w, epsilon);
}

Qt::Alignment toQAlignment(ImPlotLocation location)
{
    switch (location) {
    case ImPlotLocation_North:
        return Qt::AlignHCenter | Qt::AlignTop;
    case ImPlotLocation_South:
        return Qt::AlignHCenter | Qt::AlignBottom;
    case ImPlotLocation_West:
        return Qt::AlignLeft | Qt::AlignVCenter;
    case ImPlotLocation_East:
        return Qt::AlignRight | Qt::AlignVCenter;
    case ImPlotLocation_NorthWest:
        return Qt::AlignLeft | Qt::AlignTop;
    case ImPlotLocation_NorthEast:
        return Qt::AlignRight | Qt::AlignTop;
    case ImPlotLocation_SouthWest:
        return Qt::AlignLeft | Qt::AlignBottom;
    case ImPlotLocation_SouthEast:
        return Qt::AlignRight | Qt::AlignBottom;
    case ImPlotLocation_Center:
        return Qt::AlignHCenter | Qt::AlignVCenter;
    default:
        break;
    }
    return Qt::AlignRight | Qt::AlignVCenter;
}

ImPlotLocation toImPlotLocation(Qt::Alignment alignment)
{
    if (alignment & Qt::AlignLeft) {
        if (alignment & Qt::AlignTop)
            return ImPlotLocation_NorthWest;
        if (alignment & Qt::AlignBottom)
            return ImPlotLocation_SouthWest;
        return ImPlotLocation_West;
    } else if (alignment & Qt::AlignRight) {
        if (alignment & Qt::AlignTop)
            return ImPlotLocation_NorthEast;
        if (alignment & Qt::AlignBottom)
            return ImPlotLocation_SouthEast;
        return ImPlotLocation_East;
    } else if (alignment & Qt::AlignHCenter) {
        if (alignment & Qt::AlignTop)
            return ImPlotLocation_North;
        if (alignment & Qt::AlignBottom)
            return ImPlotLocation_South;
        return ImPlotLocation_Center;
    } else if (alignment & Qt::AlignTop) {
        return ImPlotLocation_North;
    } else if (alignment & Qt::AlignBottom) {
        return ImPlotLocation_South;
    }

    // 默认右侧
    return ImPlotLocation_East;
}

ImPlotPoint toImPlotPoint(const QPointF& v)
{
    return ImPlotPoint(v.x(), v.y());
}

QPointF toQPointF(const ImPlotPoint& v)
{
    return QPointF(v.x, v.y);
}

ImVec2 toImVec2(const QPointF& v)
{
    return ImVec2(v.x(), v.y());
}

QPointF toQPointF(const ImVec2& v)
{
    return QPointF(v.x, v.y);
}

/**
 * @brief 将 QColor (ARGB32: 0xAARRGGBB) 转换为 ImGui 的 ImU32 (ABGR32: 0xAABBGGRR)
 *
 * 处理逻辑：
 * - 提取 Qt 的 ARGB 分量 (Alpha, Red, Green, Blue)
 * - 重组为 ImGui 期望的 ABGR 顺序 (Alpha, Blue, Green, Red)
 * - 保留 Alpha 通道完整性
 */
ImU32 toImU32(const QColor& color)
{
    return IM_COL32(color.red(), color.green(), color.blue(), color.alpha());
}

/**
 * @brief 将 ImGui 的 ImU32 (ABGR32: 0xAABBGGRR) 转换为 QColor (ARGB32: 0xAARRGGBB)
 *
 * 处理逻辑：
 * - 提取 ImGui 的 ABGR 分量
 * - 重组为 Qt 期望的 ARGB 顺序
 */
QColor toQColor(ImU32 color)
{
    // 提取 ABGR 分量 (ImGui: 0xAABBGGRR)
    quint8 a = (color >> 24) & 0xFF;
    quint8 b = (color >> 16) & 0xFF;
    quint8 g = (color >> 8) & 0xFF;
    quint8 r = color & 0xFF;

    // 重组为 ARGB (Qt: 0xAARRGGBB)
    return QColor(r, g, b, a);
}

QDebug operator<<(QDebug debug, const ImVec2& v)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << '(' << v.x << "," << v.y << ')';

    return debug;
}

QDebug operator<<(QDebug debug, const ImVec4& v)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << '(' << v.x << "," << v.y << "," << v.z << "," << v.w << ')';

    return debug;
}

QDebug operator<<(QDebug debug, const ImPlotPoint& v)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << '(' << v.x << "," << v.y << ')';

    return debug;
}

QDebug operator<<(QDebug debug, const ImRect& v)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << "TL:" << v.GetTL() << ",Size:" << v.GetSize();

    return debug;
}

}
