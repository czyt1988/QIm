// MSVC requires _USE_MATH_DEFINES for M_PI
#ifndef _USE_MATH_DEFINES
#    define _USE_MATH_DEFINES
#endif

#include "QImPlot3DTextItemNode.h"
#include <cmath>
#include "implot3d.h"
#include "implot3d_internal.h"
#include "QtImGuiUtils.h"
#include "QImOptionalColor.h"
namespace QIM
{

class QImPlot3DTextItemNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlot3DTextItemNode)
public:
    PrivateData(QImPlot3DTextItemNode* p);

    QByteArray textUtf8;                   ///< Text content (UTF8, used directly by ImGui)
    double posX { 0.0 };                   ///< X coordinate in plot space
    double posY { 0.0 };                   ///< Y coordinate in plot space
    double posZ { 0.0 };                   ///< Z coordinate in plot space
    double angleRad { 0.0 };               ///< Rotation angle in radians (internally stored)
    double pixelOffsetX { 0.0 };           ///< Pixel offset X
    double pixelOffsetY { 0.0 };           ///< Pixel offset Y
    QImOptionalColor color;              ///< Text color (lazy capture)
};

QImPlot3DTextItemNode::PrivateData::PrivateData(QImPlot3DTextItemNode* p) : q_ptr(p)
{
}

/**
 * \if ENGLISH
 * @brief Constructor for QImPlot3DTextItemNode
 * @param parent Parent QObject
 * \endif
 *
 * \if CHINESE
 * @brief QImPlot3DTextItemNode 的构造函数
 * @param parent 父 QObject
 * \endif
 */
QImPlot3DTextItemNode::QImPlot3DTextItemNode(QObject* parent)
    : QImPlot3DItemNode(parent), QIM_PIMPL_CONSTRUCT
{
}

/**
 * \if ENGLISH
 * @brief Destructor for QImPlot3DTextItemNode
 * \endif
 *
 * \if CHINESE
 * @brief QImPlot3DTextItemNode 的析构函数
 * \endif
 */
QImPlot3DTextItemNode::~QImPlot3DTextItemNode()
{
}

/**
 * \if ENGLISH
 * @brief Get text content
 * @return Current text string
 * \endif
 *
 * \if CHINESE
 * @brief 获取文本内容
 * @return 当前的文本字符串
 * \endif
 */
QString QImPlot3DTextItemNode::text() const
{
    QIM_DC(d);
    return QString::fromUtf8(d->textUtf8.constData(), d->textUtf8.size());
}

/**
 * \if ENGLISH
 * @brief Set text content
 * @param text New text string to display
 * \endif
 *
 * \if CHINESE
 * @brief 设置文本内容
 * @param text 要显示的新文本字符串
 * \endif
 */
void QImPlot3DTextItemNode::setText(const QString& text)
{
    QIM_D(d);
    QByteArray utf8 = text.toUtf8();
    if (d->textUtf8 != utf8) {
        d->textUtf8 = utf8;
        Q_EMIT textChanged(text);
    }
}

/**
 * \if ENGLISH
 * @brief Get X coordinate
 * @return Current X position in plot space
 * \endif
 *
 * \if CHINESE
 * @brief 获取 X 坐标
 * @return 绘图空间中的当前 X 位置
 * \endif
 */
double QImPlot3DTextItemNode::x() const
{
    QIM_DC(d);
    return d->posX;
}

/**
 * \if ENGLISH
 * @brief Set X coordinate
 * @param x New X position in plot space
 * \endif
 *
 * \if CHINESE
 * @brief 设置 X 坐标
 * @param x 绘图空间中的新 X 位置
 * \endif
 */
void QImPlot3DTextItemNode::setX(double x)
{
    QIM_D(d);
    if (d->posX != x) {
        d->posX = x;
        Q_EMIT positionChanged(d->posX, d->posY, d->posZ);
    }
}

/**
 * \if ENGLISH
 * @brief Get Y coordinate
 * @return Current Y position in plot space
 * \endif
 *
 * \if CHINESE
 * @brief 获取 Y 坐标
 * @return 绘图空间中的当前 Y 位置
 * \endif
 */
double QImPlot3DTextItemNode::y() const
{
    QIM_DC(d);
    return d->posY;
}

/**
 * \if ENGLISH
 * @brief Set Y coordinate
 * @param y New Y position in plot space
 * \endif
 *
 * \if CHINESE
 * @brief 设置 Y 坐标
 * @param y 绘图空间中的新 Y 位置
 * \endif
 */
void QImPlot3DTextItemNode::setY(double y)
{
    QIM_D(d);
    if (d->posY != y) {
        d->posY = y;
        Q_EMIT positionChanged(d->posX, d->posY, d->posZ);
    }
}

/**
 * \if ENGLISH
 * @brief Get Z coordinate
 * @return Current Z position in plot space
 * \endif
 *
 * \if CHINESE
 * @brief 获取 Z 坐标
 * @return 绘图空间中的当前 Z 位置
 * \endif
 */
double QImPlot3DTextItemNode::z() const
{
    QIM_DC(d);
    return d->posZ;
}

/**
 * \if ENGLISH
 * @brief Set Z coordinate
 * @param z New Z position in plot space
 * \endif
 *
 * \if CHINESE
 * @brief 设置 Z 坐标
 * @param z 绘图空间中的新 Z 位置
 * \endif
 */
void QImPlot3DTextItemNode::setZ(double z)
{
    QIM_D(d);
    if (d->posZ != z) {
        d->posZ = z;
        Q_EMIT positionChanged(d->posX, d->posY, d->posZ);
    }
}

/**
 * \if ENGLISH
 * @brief Set 3D position (convenience overload)
 * @param x X coordinate in plot space
 * @param y Y coordinate in plot space
 * @param z Z coordinate in plot space
 * \endif
 *
 * \if CHINESE
 * @brief 设置 3D 位置（便捷重载）
 * @param x 绘图空间中的 X 坐标
 * @param y 绘图空间中的 Y 坐标
 * @param z 绘图空间中的 Z 坐标
 * \endif
 */
void QImPlot3DTextItemNode::setPosition(double x, double y, double z)
{
    QIM_D(d);
    if (d->posX != x || d->posY != y || d->posZ != z) {
        d->posX = x;
        d->posY = y;
        d->posZ = z;
        Q_EMIT positionChanged(x, y, z);
    }
}

/**
 * \if ENGLISH
 * @brief Get rotation angle in degrees
 * @return Current angle in degrees (converted from internal radians)
 * \endif
 *
 * \if CHINESE
 * @brief 获取旋转角度（度）
 * @return 当前的角度（度）（从内部弧度转换）
 * \endif
 */
double QImPlot3DTextItemNode::angle() const
{
    QIM_DC(d);
    return d->angleRad * 180.0 / M_PI;
}

/**
 * \if ENGLISH
 * @brief Set rotation angle in degrees
 * @param angleDeg New angle in degrees (will be converted to radians internally)
 * @details Converts degrees to radians for internal storage.
 *          Formula: radians = degrees * PI / 180
 * \endif
 *
 * \if CHINESE
 * @brief 设置旋转角度（度）
 * @param angleDeg 新的角度（度）（内部将转换为弧度）
 * @details 将度转换为弧度进行内部存储。
 *          公式：弧度 = 度 * PI / 180
 * \endif
 */
void QImPlot3DTextItemNode::setAngle(double angleDeg)
{
    QIM_D(d);
    double angleRad = angleDeg * M_PI / 180.0;
    if (d->angleRad != angleRad) {
        d->angleRad = angleRad;
        Q_EMIT angleChanged(angleDeg);
    }
}

/**
 * \if ENGLISH
 * @brief Get pixel offset X
 * @return Current pixel offset X
 * \endif
 *
 * \if CHINESE
 * @brief 获取像素偏移 X
 * @return 当前的像素偏移 X
 * \endif
 */
double QImPlot3DTextItemNode::pixelOffsetX() const
{
    QIM_DC(d);
    return d->pixelOffsetX;
}

/**
 * \if ENGLISH
 * @brief Set pixel offset X
 * @param offset New pixel offset X
 * \endif
 *
 * \if CHINESE
 * @brief 设置像素偏移 X
 * @param offset 新的像素偏移 X
 * \endif
 */
void QImPlot3DTextItemNode::setPixelOffsetX(double offset)
{
    QIM_D(d);
    if (d->pixelOffsetX != offset) {
        d->pixelOffsetX = offset;
        Q_EMIT pixelOffsetChanged(d->pixelOffsetX, d->pixelOffsetY);
    }
}

/**
 * \if ENGLISH
 * @brief Get pixel offset Y
 * @return Current pixel offset Y
 * \endif
 *
 * \if CHINESE
 * @brief 获取像素偏移 Y
 * @return 当前的像素偏移 Y
 * \endif
 */
double QImPlot3DTextItemNode::pixelOffsetY() const
{
    QIM_DC(d);
    return d->pixelOffsetY;
}

/**
 * \if ENGLISH
 * @brief Set pixel offset Y
 * @param offset New pixel offset Y
 * \endif
 *
 * \if CHINESE
 * @brief 设置像素偏移 Y
 * @param offset 新的像素偏移 Y
 * \endif
 */
void QImPlot3DTextItemNode::setPixelOffsetY(double offset)
{
    QIM_D(d);
    if (d->pixelOffsetY != offset) {
        d->pixelOffsetY = offset;
        Q_EMIT pixelOffsetChanged(d->pixelOffsetX, d->pixelOffsetY);
    }
}

/**
 * \if ENGLISH
 * @brief Set pixel offset (convenience overload)
 * @param offsetX Horizontal offset in pixels
 * @param offsetY Vertical offset in pixels
 * \endif
 *
 * \if CHINESE
 * @brief 设置像素偏移（便捷重载）
 * @param offsetX 水平像素偏移
 * @param offsetY 垂直像素偏移
 * \endif
 */
void QImPlot3DTextItemNode::setPixelOffset(double offsetX, double offsetY)
{
    QIM_D(d);
    if (d->pixelOffsetX != offsetX || d->pixelOffsetY != offsetY) {
        d->pixelOffsetX = offsetX;
        d->pixelOffsetY = offsetY;
        Q_EMIT pixelOffsetChanged(offsetX, offsetY);
    }
}

/**
 * \if ENGLISH
 * @brief Get text color
 * @return Current QColor, or invalid QColor() if using ImPlot3D default
 * \endif
 *
 * \if CHINESE
 * @brief 获取文本颜色
 * @return 当前的 QColor，使用 ImPlot3D 默认颜色时返回无效 QColor()
 * \endif
 */
QColor QImPlot3DTextItemNode::color() const
{
    QIM_DC(d);
    return (d->color.has_value()) ? toQColor(d->color->value()) : QColor();
}

/**
 * \if ENGLISH
 * @brief Set text color
 * @param c QColor for the text
 * \endif
 *
 * \if CHINESE
 * @brief 设置文本颜色
 * @param c 文本的 QColor
 * \endif
 */
void QImPlot3DTextItemNode::setColor(const QColor& c)
{
    QIM_D(d);
    d->color = toImVec4(c);
    Q_EMIT colorChanged(c);
}

/**
 * \if ENGLISH
 * @brief Begin drawing implementation
 * @return false to prevent endDraw from being called
 * @details Calls ImPlot3D::PlotText with appropriate parameters.
 *          Returns false if text is empty.
 *          Applies color style if explicitly set.
 * \endif
 *
 * \if CHINESE
 * @brief 开始绘制实现
 * @return false 以防止调用 endDraw
 * @details 使用适当的参数调用 ImPlot3D::PlotText。
 *          如果文本为空则返回 false。
 *          如果设置了显式颜色则应用颜色样式。
 * \endif
 */
bool QImPlot3DTextItemNode::beginDraw()
{
    QIM_D(d);
    if (d->textUtf8.isEmpty()) {
        return false;
    }

    // Track if we pushed style color to ensure matching pop
    bool pushedStyleColor = false;

    // Apply style - push if color is set
    if (d->color) {
        ImPlot3D::PushStyleColor(ImPlot3DCol_InlayText, d->color->value());
        pushedStyleColor = true;
    }

    // Call ImPlot3D API - use UTF8 directly (stored in setter for performance)
    // Note: PlotText has no flags parameter, no label_id parameter
    ImPlot3D::PlotText(
        d->textUtf8.constData(),
        d->posX,
        d->posY,
        d->posZ,
        d->angleRad,
        ImVec2(static_cast<float>(d->pixelOffsetX), static_cast<float>(d->pixelOffsetY)));

    // Pop style only if we pushed it
    if (pushedStyleColor) {
        ImPlot3D::PopStyleColor();
    }

    // Capture default color on first render when no explicit color was set
    if (!d->color) {
        d->color = captureItemColor();
    }

    return false;
}

}  // namespace QIM
