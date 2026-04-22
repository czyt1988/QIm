#include "QtImGuiUtils.h"
#include <QGuiApplication>
#include <QPalette>
#include <QDebug>
#include "plot3d/QImPlot3DTypes.h"

namespace QIM
{

/**
 * \if ENGLISH
 * @brief Checks whether the operating system is using a dark theme
 * @return true if system theme is dark (brightness < 128 using YIQ formula)
 * @details Uses YIQ brightness formula (0.299R + 0.587G + 0.114B) on the
 *          QPalette::Window background color to determine theme darkness.
 * \endif
 * \if CHINESE
 * @brief 检查操作系统是否使用暗色主题
 * @return true 如果系统主题为暗色（使用 YIQ 公式亮度 < 128）
 * @details 使用 YIQ 亮度公式（0.299R + 0.587G + 0.114B）对
 *          QPalette::Window 背景色进行计算以判断主题深浅。
 * \endif
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
 * \if ENGLISH
 * @brief Converts a QColor to ImVec4 (float RGBA)
 * @param[in] color Qt color to convert
 * @return ImVec4 with normalized float components (0.0-1.0 range)
 * \endif
 * \if CHINESE
 * @brief 将 QColor 转换为 ImVec4（浮点 RGBA）
 * @param[in] color Qt 颜色对象
 * @return ImVec4，包含归一化浮点分量（0.0-1.0 范围）
 * \endif
 */
ImVec4 toImVec4(const QColor& color)
{
    return ImVec4(color.redF(), color.greenF(), color.blueF(), color.alphaF());
}

/**
 * \if ENGLISH
 * @brief Converts an ImVec4 to QColor
 * @param[in] color ImGui float color to convert
 * @return QColor with the same RGBA values
 * \endif
 * \if CHINESE
 * @brief 将 ImVec4 转换为 QColor
 * @param[in] color ImGui 浮点颜色对象
 * @return QColor，包含相同的 RGBA 值
 * \endif
 */
QColor toQColor(const ImVec4& color)
{
    return QColor::fromRgbF(color.x, color.y, color.z, color.w);
}

/**
 * \if ENGLISH
 * @brief Converts an ImVec2 to QSize (integer dimensions)
 * @param[in] v ImVec2 to convert
 * @return QSize with integer width and height
 * \endif
 * \if CHINESE
 * @brief 将 ImVec2 转换为 QSize（整数尺寸）
 * @param[in] v 待转换的 ImVec2
 * @return QSize，包含整型宽度和高度
 * \endif
 */
QSize toQSize(const ImVec2& v)
{
    return QSize(static_cast< int >(v.x), static_cast< int >(v.y));
}

/**
 * \if ENGLISH
 * @brief Converts a QSize to ImVec2 (integer dimensions)
 * @param[in] v QSize to convert
 * @return ImVec2 with width and height as float values
 * \endif
 * \if CHINESE
 * @brief 将 QSize 转换为 ImVec2（整数尺寸）
 * @param[in] v 待转换的 QSize
 * @return ImVec2，宽度和高度作为浮点值
 * \endif
 */
ImVec2 toImVec2(const QSize& v)
{
    return ImVec2(v.width(), v.height());
}

/**
 * \if ENGLISH
 * @brief Converts a QSizeF to ImVec2 (floating-point dimensions)
 * @param[in] v QSizeF to convert
 * @return ImVec2 with qreal width and height as float values
 * \endif
 * \if CHINESE
 * @brief 将 QSizeF 转换为 ImVec2（浮点尺寸）
 * @param[in] v 待转换的 QSizeF
 * @return ImVec2，宽度和高度作为浮点值
 * \endif
 */
ImVec2 toImVec2(const QSizeF& v)
{
    return ImVec2(v.width(), v.height());
}

/**
 * \if ENGLISH
 * @brief Converts an ImVec2 to QSizeF (floating-point dimensions)
 * @param[in] v ImVec2 to convert
 * @return QSizeF with qreal width and height
 * \endif
 * \if CHINESE
 * @brief 将 ImVec2 转换为 QSizeF（浮点尺寸）
 * @param[in] v 待转换的 ImVec2
 * @return QSizeF，包含 qreal 宽度和高度
 * \endif
 */
QSizeF toQSizeF(const ImVec2& v)
{
    return QSizeF(static_cast< qreal >(v.x), static_cast< qreal >(v.y));
}

/**
 * \if ENGLISH
 * @brief Fuzzy comparison of two float values with configurable tolerance
 * @param[in] a First value
 * @param[in] b Second value
 * @param[in] epsilon Tolerance (default 1e-5f)
 * @return true if |a - b| <= epsilon
 * \endif
 * \if CHINESE
 * @brief 两个浮点数的模糊比较（支持自定义精度）
 * @param[in] a 第一个值
 * @param[in] b 第二个值
 * @param[in] epsilon 容差（默认 1e-5f）
 * @return 若 |a - b| <= epsilon，则返回 true
 * \endif
 */
bool fuzzyEqual(float a, float b, float epsilon)
{
    return std::abs(a - b) <= epsilon;
}

/**
 * \if ENGLISH
 * @brief Fuzzy comparison of two double values with configurable tolerance
 * @param[in] a First value
 * @param[in] b Second value
 * @param[in] epsilon Tolerance (default 1e-9)
 * @return true if |a - b| <= epsilon
 * \endif
 * \if CHINESE
 * @brief 两个双精度浮点数的模糊比较（支持自定义精度）
 * @param[in] a 第一个值
 * @param[in] b 第二个值
 * @param[in] epsilon 容差（默认 1e-9）
 * @return 若 |a - b| <= epsilon，则返回 true
 * \endif
 */
bool fuzzyEqual(double a, double b, double epsilon)
{
    return std::abs(a - b) <= epsilon;
}

/**
 * \if ENGLISH
 * @brief Fuzzy comparison of two ImVec2 values component-wise
 * @param[in] a First ImVec2
 * @param[in] b Second ImVec2
 * @param[in] epsilon Tolerance (default 1e-5f)
 * @return true if both x and y components are within epsilon
 * \endif
 * \if CHINESE
 * @brief 两个 ImVec2 的逐分量模糊比较
 * @param[in] a 第一个 ImVec2
 * @param[in] b 第二个 ImVec2
 * @param[in] epsilon 容差（默认 1e-5f）
 * @return 若 x 和 y 分量均在容差范围内，则返回 true
 * \endif
 */
bool fuzzyEqual(const ImVec2& a, const ImVec2& b, float epsilon)
{
    return fuzzyEqual(a.x, b.x, epsilon) && fuzzyEqual(a.y, b.y, epsilon);
}

/**
 * \if ENGLISH
 * @brief Fuzzy comparison of two ImVec4 values component-wise
 * @param[in] a First ImVec4
 * @param[in] b Second ImVec4
 * @param[in] epsilon Tolerance (default 1e-5f)
 * @return true if all four components (x, y, z, w) are within epsilon
 * \endif
 * \if CHINESE
 * @brief 两个 ImVec4 的逐分量模糊比较
 * @param[in] a 第一个 ImVec4
 * @param[in] b 第二个 ImVec4
 * @param[in] epsilon 容差（默认 1e-5f）
 * @return 若所有四个分量（x、y、z、w）均在容差范围内，则返回 true
 * \endif
 */
bool fuzzyEqual(const ImVec4& a, const ImVec4& b, float epsilon)
{
    return fuzzyEqual(a.x, b.x, epsilon) && fuzzyEqual(a.y, b.y, epsilon) && fuzzyEqual(a.z, b.z, epsilon)
           && fuzzyEqual(a.w, b.w, epsilon);
}

/**
 * \if ENGLISH
 * @brief Converts an ImPlotLocation enum to Qt::Alignment flags
 * @param[in] location ImPlot location enum value
 * @return Qt::Alignment combining horizontal and vertical alignment flags
 * \endif
 * \if CHINESE
 * @brief 将 ImPlotLocation 枚举转换为 Qt::Alignment 标志
 * @param[in] location ImPlot 位置枚举值
 * @return Qt::Alignment，组合了水平和垂直对齐标志
 * \endif
 */
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

/**
 * \if ENGLISH
 * @brief Converts Qt::Alignment flags to ImPlotLocation enum
 * @param[in] alignment Qt alignment flags
 * @return ImPlotLocation corresponding to the alignment combination
 * \endif
 * \if CHINESE
 * @brief 将 Qt::Alignment 标志转换为 ImPlotLocation 枚举
 * @param[in] alignment Qt 对齐标志
 * @return ImPlotLocation，与对齐组合对应
 * \endif
 */
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

/**
 * \if ENGLISH
 * @brief Converts a QPointF to ImPlotPoint
 * @param[in] v QPointF to convert
 * @return ImPlotPoint with the same x and y values
 * \endif
 * \if CHINESE
 * @brief 将 QPointF 转换为 ImPlotPoint
 * @param[in] v 待转换的 QPointF
 * @return ImPlotPoint，包含相同的 x 和 y 值
 * \endif
 */
ImPlotPoint toImPlotPoint(const QPointF& v)
{
    return ImPlotPoint(v.x(), v.y());
}

/**
 * \if ENGLISH
 * @brief Converts an ImPlotPoint to QPointF
 * @param[in] v ImPlotPoint to convert
 * @return QPointF with the same x and y values
 * \endif
 * \if CHINESE
 * @brief 将 ImPlotPoint 转换为 QPointF
 * @param[in] v 待转换的 ImPlotPoint
 * @return QPointF，包含相同的 x 和 y 值
 * \endif
 */
QPointF toQPointF(const ImPlotPoint& v)
{
    return QPointF(v.x, v.y);
}

/**
 * \if ENGLISH
 * @brief Converts a QPointF to ImVec2
 * @param[in] v QPointF to convert
 * @return ImVec2 with the same x and y values as float
 * \endif
 * \if CHINESE
 * @brief 将 QPointF 转换为 ImVec2
 * @param[in] v 待转换的 QPointF
 * @return ImVec2，包含相同的 x 和 y 浮点值
 * \endif
 */
ImVec2 toImVec2(const QPointF& v)
{
    return ImVec2(v.x(), v.y());
}

/**
 * \if ENGLISH
 * @brief Converts an ImVec2 to QPointF
 * @param[in] v ImVec2 to convert
 * @return QPointF with x and y as double values
 * \endif
 * \if CHINESE
 * @brief 将 ImVec2 转换为 QPointF
 * @param[in] v 待转换的 ImVec2
 * @return QPointF，包含 x 和 y 的双精度值
 * \endif
 */
QPointF toQPointF(const ImVec2& v)
{
    return QPointF(v.x, v.y);
}

/**
 * \if ENGLISH
 * @brief Converts a QColor (ARGB32) to ImGui ImU32 (ABGR32 packed color)
 * @param[in] color Qt color to convert
 * @return ImU32 packed color in ABGR format (0xAABBGGRR)
 * @details Extracts Qt's ARGB components and reassembles them into ImGui's
 *          expected ABGR order via IM_COL32 macro. Alpha channel is preserved.
 * \endif
 * \if CHINESE
 * @brief 将 QColor（ARGB32：0xAARRGGBB）转换为 ImGui 的 ImU32（ABGR32：0xAABBGGRR）
 * @param[in] color Qt 颜色对象
 * @return ImU32 打包颜色，ABGR 格式（0xAABBGGRR）
 * @details 通过 IM_COL32 宏提取 Qt 的 ARGB 分量并重组为 ImGui 期望的
 *          ABGR 顺序。Alpha 通道完整保留。
 * \endif
 */
ImU32 toImU32(const QColor& color)
{
    return IM_COL32(color.red(), color.green(), color.blue(), color.alpha());
}

/**
 * \if ENGLISH
 * @brief Converts an ImGui ImU32 (ABGR32) to QColor (ARGB32)
 * @param[in] color ImGui packed color in ABGR format
 * @return QColor with ARGB components extracted from ABGR layout
 * @details Extracts ABGR components from ImGui format and reassembles into
 *          Qt's ARGB order. Alpha channel is preserved.
 * \endif
 * \if CHINESE
 * @brief 将 ImGui 的 ImU32（ABGR32：0xAABBGGRR）转换为 QColor（ARGB32：0xAARRGGBB）
 * @param[in] color ImGui 打包颜色，ABGR 格式
 * @return QColor，从 ABGR 布局中提取的 ARGB 分量
 * @details 从 ImGui 格式中提取 ABGR 分量并重组为 Qt 期望的
 *          ARGB 顺序。Alpha 通道完整保留。
 * \endif
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

/**
 * \if ENGLISH
 * @brief QDebug stream operator for ImVec2
 * @param[in] debug QDebug stream
 * @param[in] v ImVec2 value to output
 * @return QDebug stream with formatted ImVec2
 * \endif
 * \if CHINESE
 * @brief ImVec2 的 QDebug 流输出操作符
 * @param[in] debug QDebug 流
 * @param[in] v 待输出的 ImVec2 值
 * @return 包含格式化 ImVec2 的 QDebug 流
 * \endif
 */
QDebug operator<<(QDebug debug, const ImVec2& v)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << '(' << v.x << "," << v.y << ')';

    return debug;
}

/**
 * \if ENGLISH
 * @brief QDebug stream operator for ImVec4
 * @param[in] debug QDebug stream
 * @param[in] v ImVec4 value to output
 * @return QDebug stream with formatted ImVec4
 * \endif
 * \if CHINESE
 * @brief ImVec4 的 QDebug 流输出操作符
 * @param[in] debug QDebug 流
 * @param[in] v 待输出的 ImVec4 值
 * @return 包含格式化 ImVec4 的 QDebug 流
 * \endif
 */
QDebug operator<<(QDebug debug, const ImVec4& v)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << '(' << v.x << "," << v.y << "," << v.z << "," << v.w << ')';

    return debug;
}

/**
 * \if ENGLISH
 * @brief QDebug stream operator for ImPlotPoint
 * @param[in] debug QDebug stream
 * @param[in] v ImPlotPoint value to output
 * @return QDebug stream with formatted ImPlotPoint
 * \endif
 * \if CHINESE
 * @brief ImPlotPoint 的 QDebug 流输出操作符
 * @param[in] debug QDebug 流
 * @param[in] v 待输出的 ImPlotPoint 值
 * @return 包含格式化 ImPlotPoint 的 QDebug 流
 * \endif
 */
QDebug operator<<(QDebug debug, const ImPlotPoint& v)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << '(' << v.x << "," << v.y << ')';

    return debug;
}

/**
 * \if ENGLISH
 * @brief QDebug stream operator for ImRect
 * @param[in] debug QDebug stream
 * @param[in] v ImRect value to output
 * @return QDebug stream with formatted ImRect (top-left and size)
 * \endif
 * \if CHINESE
 * @brief ImRect 的 QDebug 流输出操作符
 * @param[in] debug QDebug 流
 * @param[in] v 待输出的 ImRect 值
 * @return 包含格式化 ImRect（左上角和尺寸）的 QDebug 流
 * \endif
 */
QDebug operator<<(QDebug debug, const ImRect& v)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << "TL:" << v.GetTL() << ",Size:" << v.GetSize();

    return debug;
}

/**
 * \if ENGLISH
 * @brief Converts a QImPlot3DPoint to ImPlot3DPoint
 * @param[in] v QImPlot3DPoint to convert
 * @return ImPlot3DPoint with the same x, y, z values
 * \endif
 * \if CHINESE
 * @brief 将 QImPlot3DPoint 转换为 ImPlot3DPoint
 * @param[in] v 待转换的 QImPlot3DPoint
 * @return ImPlot3DPoint，包含相同的 x、y、z 值
 * \endif
 */
ImPlot3DPoint toImPlot3DPoint(const QImPlot3DPoint& v)
{
    return ImPlot3DPoint(v.x, v.y, v.z);
}

/**
 * \if ENGLISH
 * @brief Converts an ImPlot3DPoint to QImPlot3DPoint
 * @param[in] v ImPlot3DPoint to convert
 * @return QImPlot3DPoint with the same x, y, z values
 * \endif
 * \if CHINESE
 * @brief 将 ImPlot3DPoint 转换为 QImPlot3DPoint
 * @param[in] v 待转换的 ImPlot3DPoint
 * @return QImPlot3DPoint，包含相同的 x、y、z 值
 * \endif
 */
QImPlot3DPoint toQImPlot3DPoint(const ImPlot3DPoint& v)
{
    return QImPlot3DPoint(v.x, v.y, v.z);
}

/**
 * \if ENGLISH
 * @brief Converts a QQuaternion to ImPlot3DQuat
 * @param[in] q QQuaternion to convert (float components)
 * @return ImPlot3DQuat with double precision components
 * @note QQuaternion stores components as (x, y, z, scalar) where scalar is w
 * \endif
 * \if CHINESE
 * @brief 将 QQuaternion 转换为 ImPlot3DQuat
 * @param[in] q 待转换的 QQuaternion（浮点分量）
 * @return ImPlot3DQuat，双精度分量
 * @note QQuaternion 以 (x, y, z, scalar) 存储分量，其中 scalar 对应 w
 * \endif
 */
ImPlot3DQuat toImPlot3DQuat(const QQuaternion& q)
{
    // QQuaternion: (x, y, z, scalar) where scalar is w
    // ImPlot3DQuat: (x, y, z, w)
    return ImPlot3DQuat(q.x(), q.y(), q.z(), q.scalar());
}

/**
 * \if ENGLISH
 * @brief Converts an ImPlot3DQuat to QQuaternion
 * @param[in] q ImPlot3DQuat to convert (double precision)
 * @return QQuaternion with float components
 * @note Precision loss from double to float is expected
 * \endif
 * \if CHINESE
 * @brief 将 ImPlot3DQuat 转换为 QQuaternion
 * @param[in] q 待转换的 ImPlot3DQuat（双精度）
 * @return QQuaternion，浮点分量
 * @note 从双精度到浮点会有精度损失
 * \endif
 */
QQuaternion toQQuaternion(const ImPlot3DQuat& q)
{
    // ImPlot3DQuat: (x, y, z, w)
    // QQuaternion: (x, y, z, scalar) where scalar is w
    return QQuaternion(static_cast<float>(q.w), static_cast<float>(q.x), static_cast<float>(q.y), static_cast<float>(q.z));
}

/**
 * \if ENGLISH
 * @brief Converts a QImPlot3DRay to ImPlot3DRay
 * @param[in] r QImPlot3DRay to convert
 * @return ImPlot3DRay with converted origin and direction
 * \endif
 * \if CHINESE
 * @brief 将 QImPlot3DRay 转换为 ImPlot3DRay
 * @param[in] r 待转换的 QImPlot3DRay
 * @return ImPlot3DRay，包含转换后的原点和方向
 * \endif
 */
ImPlot3DRay toImPlot3DRay(const QImPlot3DRay& r)
{
    ImPlot3DRay result;
    result.Origin = toImPlot3DPoint(r.Origin);
    result.Direction = toImPlot3DPoint(r.Direction);
    return result;
}

/**
 * \if ENGLISH
 * @brief Converts an ImPlot3DRay to QImPlot3DRay
 * @param[in] r ImPlot3DRay to convert
 * @return QImPlot3DRay with converted origin and direction
 * \endif
 * \if CHINESE
 * @brief 将 ImPlot3DRay 转换为 QImPlot3DRay
 * @param[in] r 待转换的 ImPlot3DRay
 * @return QImPlot3DRay，包含转换后的原点和方向
 * \endif
 */
QImPlot3DRay toQImPlot3DRay(const ImPlot3DRay& r)
{
    QImPlot3DRay result;
    result.Origin = toQImPlot3DPoint(r.Origin);
    result.Direction = toQImPlot3DPoint(r.Direction);
    return result;
}

/**
 * \if ENGLISH
 * @brief Converts a QImPlot3DPlane to ImPlot3DPlane
 * @param[in] p QImPlot3DPlane to convert
 * @return ImPlot3DPlane with converted point and normal
 * \endif
 * \if CHINESE
 * @brief 将 QImPlot3DPlane 转换为 ImPlot3DPlane
 * @param[in] p 待转换的 QImPlot3DPlane
 * @return ImPlot3DPlane，包含转换后的点和法向量
 * \endif
 */
ImPlot3DPlane toImPlot3DPlane(const QImPlot3DPlane& p)
{
    ImPlot3DPlane result;
    result.Point = toImPlot3DPoint(p.Point);
    result.Normal = toImPlot3DPoint(p.Normal);
    return result;
}

/**
 * \if ENGLISH
 * @brief Converts an ImPlot3DPlane to QImPlot3DPlane
 * @param[in] p ImPlot3DPlane to convert
 * @return QImPlot3DPlane with converted point and normal
 * \endif
 * \if CHINESE
 * @brief 将 ImPlot3DPlane 转换为 QImPlot3DPlane
 * @param[in] p 待转换的 ImPlot3DPlane
 * @return QImPlot3DPlane，包含转换后的点和法向量
 * \endif
 */
QImPlot3DPlane toQImPlot3DPlane(const ImPlot3DPlane& p)
{
    QImPlot3DPlane result;
    result.Point = toQImPlot3DPoint(p.Point);
    result.Normal = toQImPlot3DPoint(p.Normal);
    return result;
}

/**
 * \if ENGLISH
 * @brief Converts a QImPlot3DBox to ImPlot3DBox
 * @param[in] b QImPlot3DBox to convert
 * @return ImPlot3DBox with converted min and max corners
 * \endif
 * \if CHINESE
 * @brief 将 QImPlot3DBox 转换为 ImPlot3DBox
 * @param[in] b 待转换的 QImPlot3DBox
 * @return ImPlot3DBox，包含转换后的最小和最大角点
 * \endif
 */
ImPlot3DBox toImPlot3DBox(const QImPlot3DBox& b)
{
    ImPlot3DBox result;
    result.Min = toImPlot3DPoint(b.Min);
    result.Max = toImPlot3DPoint(b.Max);
    return result;
}

/**
 * \if ENGLISH
 * @brief Converts an ImPlot3DBox to QImPlot3DBox
 * @param[in] b ImPlot3DBox to convert
 * @return QImPlot3DBox with converted min and max corners
 * \endif
 * \if CHINESE
 * @brief 将 ImPlot3DBox 转换为 QImPlot3DBox
 * @param[in] b 待转换的 ImPlot3DBox
 * @return QImPlot3DBox，包含转换后的最小和最大角点
 * \endif
 */
QImPlot3DBox toQImPlot3DBox(const ImPlot3DBox& b)
{
    QImPlot3DBox result;
    result.Min = toQImPlot3DPoint(b.Min);
    result.Max = toQImPlot3DPoint(b.Max);
    return result;
}

/**
 * \if ENGLISH
 * @brief Converts a QImPlot3DRange to ImPlot3DRange
 * @param[in] r QImPlot3DRange to convert
 * @return ImPlot3DRange with the same min and max values
 * \endif
 * \if CHINESE
 * @brief 将 QImPlot3DRange 转换为 ImPlot3DRange
 * @param[in] r 待转换的 QImPlot3DRange
 * @return ImPlot3DRange，包含相同的最小和最大值
 * \endif
 */
ImPlot3DRange toImPlot3DRange(const QImPlot3DRange& r)
{
    return ImPlot3DRange(r.Min, r.Max);
}

/**
 * \if ENGLISH
 * @brief Converts an ImPlot3DRange to QImPlot3DRange
 * @param[in] r ImPlot3DRange to convert
 * @return QImPlot3DRange with the same min and max values
 * \endif
 * \if CHINESE
 * @brief 将 ImPlot3DRange 转换为 QImPlot3DRange
 * @param[in] r 待转换的 ImPlot3DRange
 * @return QImPlot3DRange，包含相同的最小和最大值
 * \endif
 */
QImPlot3DRange toQImPlot3DRange(const ImPlot3DRange& r)
{
    return QImPlot3DRange(r.Min, r.Max);
}

/**
 * \if ENGLISH
 * @brief Fuzzy comparison of two QImPlot3DPoint values component-wise
 * @param[in] a First QImPlot3DPoint
 * @param[in] b Second QImPlot3DPoint
 * @param[in] epsilon Tolerance (default 1e-9)
 * @return true if all three components (x, y, z) are within epsilon
 * \endif
 * \if CHINESE
 * @brief 两个 QImPlot3DPoint 的逐分量模糊比较
 * @param[in] a 第一个 QImPlot3DPoint
 * @param[in] b 第二个 QImPlot3DPoint
 * @param[in] epsilon 容差（默认 1e-9）
 * @return 若所有三个分量（x、y、z）均在容差范围内，则返回 true
 * \endif
 */
bool fuzzyEqual(const QImPlot3DPoint& a, const QImPlot3DPoint& b, double epsilon)
{
    return fuzzyEqual(a.x, b.x, epsilon) && fuzzyEqual(a.y, b.y, epsilon) && fuzzyEqual(a.z, b.z, epsilon);
}

/**
 * \if ENGLISH
 * @brief Fuzzy comparison of two QImPlot3DBox values
 * @param[in] a First QImPlot3DBox
 * @param[in] b Second QImPlot3DBox
 * @param[in] epsilon Tolerance (default 1e-9)
 * @return true if both min and max corners are within epsilon
 * \endif
 * \if CHINESE
 * @brief 两个 QImPlot3DBox 的模糊比较
 * @param[in] a 第一个 QImPlot3DBox
 * @param[in] b 第二个 QImPlot3DBox
 * @param[in] epsilon 容差（默认 1e-9）
 * @return 若最小和最大角点均在容差范围内，则返回 true
 * \endif
 */
bool fuzzyEqual(const QImPlot3DBox& a, const QImPlot3DBox& b, double epsilon)
{
    return fuzzyEqual(a.Min, b.Min, epsilon) && fuzzyEqual(a.Max, b.Max, epsilon);
}

/**
 * \if ENGLISH
 * @brief Fuzzy comparison of two QImPlot3DRange values
 * @param[in] a First QImPlot3DRange
 * @param[in] b Second QImPlot3DRange
 * @param[in] epsilon Tolerance (default 1e-9)
 * @return true if both min and max values are within epsilon
 * \endif
 * \if CHINESE
 * @brief 两个 QImPlot3DRange 的模糊比较
 * @param[in] a 第一个 QImPlot3DRange
 * @param[in] b 第二个 QImPlot3DRange
 * @param[in] epsilon 容差（默认 1e-9）
 * @return 若最小和最大值均在容差范围内，则返回 true
 * \endif
 */
bool fuzzyEqual(const QImPlot3DRange& a, const QImPlot3DRange& b, double epsilon)
{
    return fuzzyEqual(a.Min, b.Min, epsilon) && fuzzyEqual(a.Max, b.Max, epsilon);
}

/**
 * \if ENGLISH
 * @brief QDebug stream operator for QImPlot3DPoint
 * @param[in] debug QDebug stream
 * @param[in] v QImPlot3DPoint value to output
 * @return QDebug stream with formatted QImPlot3DPoint
 * \endif
 * \if CHINESE
 * @brief QImPlot3DPoint 的 QDebug 流输出操作符
 * @param[in] debug QDebug 流
 * @param[in] v 待输出的 QImPlot3DPoint 值
 * @return 包含格式化 QImPlot3DPoint 的 QDebug 流
 * \endif
 */
QDebug operator<<(QDebug debug, const QImPlot3DPoint& v)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << '(' << v.x << "," << v.y << "," << v.z << ')';
    return debug;
}

/**
 * \if ENGLISH
 * @brief QDebug stream operator for QImPlot3DRay
 * @param[in] debug QDebug stream
 * @param[in] v QImPlot3DRay value to output
 * @return QDebug stream with formatted QImPlot3DRay
 * \endif
 * \if CHINESE
 * @brief QImPlot3DRay 的 QDebug 流输出操作符
 * @param[in] debug QDebug 流
 * @param[in] v 待输出的 QImPlot3DRay 值
 * @return 包含格式化 QImPlot3DRay 的 QDebug 流
 * \endif
 */
QDebug operator<<(QDebug debug, const QImPlot3DRay& v)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << "Origin:" << v.Origin << ",Direction:" << v.Direction;
    return debug;
}

/**
 * \if ENGLISH
 * @brief QDebug stream operator for QImPlot3DPlane
 * @param[in] debug QDebug stream
 * @param[in] v QImPlot3DPlane value to output
 * @return QDebug stream with formatted QImPlot3DPlane
 * \endif
 * \if CHINESE
 * @brief QImPlot3DPlane 的 QDebug 流输出操作符
 * @param[in] debug QDebug 流
 * @param[in] v 待输出的 QImPlot3DPlane 值
 * @return 包含格式化 QImPlot3DPlane 的 QDebug 流
 * \endif
 */
QDebug operator<<(QDebug debug, const QImPlot3DPlane& v)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << "Point:" << v.Point << ",Normal:" << v.Normal;
    return debug;
}

/**
 * \if ENGLISH
 * @brief QDebug stream operator for QImPlot3DBox
 * @param[in] debug QDebug stream
 * @param[in] v QImPlot3DBox value to output
 * @return QDebug stream with formatted QImPlot3DBox
 * \endif
 * \if CHINESE
 * @brief QImPlot3DBox 的 QDebug 流输出操作符
 * @param[in] debug QDebug 流
 * @param[in] v 待输出的 QImPlot3DBox 值
 * @return 包含格式化 QImPlot3DBox 的 QDebug 流
 * \endif
 */
QDebug operator<<(QDebug debug, const QImPlot3DBox& v)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << "Min:" << v.Min << ",Max:" << v.Max;
    return debug;
}

/**
 * \if ENGLISH
 * @brief QDebug stream operator for QImPlot3DRange
 * @param[in] debug QDebug stream
 * @param[in] v QImPlot3DRange value to output
 * @return QDebug stream with formatted QImPlot3DRange
 * \endif
 * \if CHINESE
 * @brief QImPlot3DRange 的 QDebug 流输出操作符
 * @param[in] debug QDebug 流
 * @param[in] v 待输出的 QImPlot3DRange 值
 * @return 包含格式化 QImPlot3DRange 的 QDebug 流
 * \endif
 */
QDebug operator<<(QDebug debug, const QImPlot3DRange& v)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << '[' << v.Min << "," << v.Max << ']';
    return debug;
}

}  // namespace QIM
