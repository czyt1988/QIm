#include "QImPlot3DImageItemNode.h"
#include <memory>
#include <optional>
#include "implot3d.h"
#include "implot3d_internal.h"
#include "QImTrackedValue.hpp"
#include "QtImGuiUtils.h"
#include <QDebug>

namespace QIM
{

class QImPlot3DImageItemNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlot3DImageItemNode)
public:
    PrivateData(QImPlot3DImageItemNode* p);

    quintptr textureId { 0 };
    ImPlot3DPoint center { 0.0, 0.0, 0.0 };
    ImPlot3DPoint axisU { 1.0, 0.0, 0.0 };
    ImPlot3DPoint axisV { 0.0, 1.0, 0.0 };
    ImVec2 uv0 { 0.0f, 0.0f };
    ImVec2 uv1 { 1.0f, 1.0f };
    QImOptional3DColor tintColor;
    ImPlot3DImageFlags flags { ImPlot3DImageFlags_None };

    bool quadModeEnabled { false };
    ImPlot3DPoint p0 { 0.0, 0.0, 0.0 };
    ImPlot3DPoint p1 { 1.0, 0.0, 0.0 };
    ImPlot3DPoint p2 { 1.0, 1.0, 0.0 };
    ImPlot3DPoint p3 { 0.0, 1.0, 0.0 };
    ImVec2 uvP0 { 0.0f, 0.0f };
    ImVec2 uvP1 { 1.0f, 0.0f };
    ImVec2 uvP2 { 1.0f, 1.0f };
    ImVec2 uvP3 { 0.0f, 1.0f };
};

QImPlot3DImageItemNode::PrivateData::PrivateData(QImPlot3DImageItemNode* p) : q_ptr(p)
{
}

/**
 * \if ENGLISH
 * @brief Constructor for QImPlot3DImageItemNode
 * @param parent Parent QObject
 * \endif
 *
 * \if CHINESE
 * @brief QImPlot3DImageItemNode的构造函数
 * @param parent 父QObject
 * \endif
 */
QImPlot3DImageItemNode::QImPlot3DImageItemNode(QObject* parent) : QImPlot3DItemNode(parent), QIM_PIMPL_CONSTRUCT
{
}

/**
 * \if ENGLISH
 * @brief Destructor for QImPlot3DImageItemNode
 * \endif
 *
 * \if CHINESE
 * @brief QImPlot3DImageItemNode的析构函数
 * \endif
 */
QImPlot3DImageItemNode::~QImPlot3DImageItemNode()
{
}

/**
 * \if ENGLISH
 * @brief Get texture ID
 * @return Current texture ID as quintptr
 * \endif
 *
 * \if CHINESE
 * @brief 获取纹理ID
 * @return 当前纹理ID（quintptr类型）
 * \endif
 */
quintptr QImPlot3DImageItemNode::textureId() const
{
    QIM_DC(d);
    return d->textureId;
}

/**
 * \if ENGLISH
 * @brief Set texture ID
 * @param id New texture ID value
 * \endif
 *
 * \if CHINESE
 * @brief 设置纹理ID
 * @param id 新的纹理ID值
 * \endif
 */
void QImPlot3DImageItemNode::setTextureId(quintptr id)
{
    QIM_D(d);
    if (d->textureId != id) {
        d->textureId = id;
        Q_EMIT textureIdChanged(id);
    }
}

/**
 * \if ENGLISH
 * @brief Get center X coordinate
 * @return Current center X coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 获取中心X坐标
 * @return 当前中心X坐标
 * \endif
 */
double QImPlot3DImageItemNode::centerX() const
{
    QIM_DC(d);
    return d->center.x;
}

/**
 * \if ENGLISH
 * @brief Set center X coordinate
 * @param x New center X coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 设置中心X坐标
 * @param x 新的中心X坐标
 * \endif
 */
void QImPlot3DImageItemNode::setCenterX(double x)
{
    QIM_D(d);
    if (!fuzzyEqual(d->center.x, x)) {
        d->center.x = x;
        Q_EMIT centerChanged(d->center.x, d->center.y, d->center.z);
    }
}

/**
 * \if ENGLISH
 * @brief Get center Y coordinate
 * @return Current center Y coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 获取中心Y坐标
 * @return 当前中心Y坐标
 * \endif
 */
double QImPlot3DImageItemNode::centerY() const
{
    QIM_DC(d);
    return d->center.y;
}

/**
 * \if ENGLISH
 * @brief Set center Y coordinate
 * @param y New center Y coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 设置中心Y坐标
 * @param y 新的中心Y坐标
 * \endif
 */
void QImPlot3DImageItemNode::setCenterY(double y)
{
    QIM_D(d);
    if (!fuzzyEqual(d->center.y, y)) {
        d->center.y = y;
        Q_EMIT centerChanged(d->center.x, d->center.y, d->center.z);
    }
}

/**
 * \if ENGLISH
 * @brief Get center Z coordinate
 * @return Current center Z coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 获取中心Z坐标
 * @return 当前中心Z坐标
 * \endif
 */
double QImPlot3DImageItemNode::centerZ() const
{
    QIM_DC(d);
    return d->center.z;
}

/**
 * \if ENGLISH
 * @brief Set center Z coordinate
 * @param z New center Z coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 设置中心Z坐标
 * @param z 新的中心Z坐标
 * \endif
 */
void QImPlot3DImageItemNode::setCenterZ(double z)
{
    QIM_D(d);
    if (!fuzzyEqual(d->center.z, z)) {
        d->center.z = z;
        Q_EMIT centerChanged(d->center.x, d->center.y, d->center.z);
    }
}

/**
 * \if ENGLISH
 * @brief Get U axis X component
 * @return Current U axis X component
 * \endif
 *
 * \if CHINESE
 * @brief 获取U轴X分量
 * @return 当前U轴X分量
 * \endif
 */
double QImPlot3DImageItemNode::axisUX() const
{
    QIM_DC(d);
    return d->axisU.x;
}

/**
 * \if ENGLISH
 * @brief Set U axis X component
 * @param x New U axis X component
 * \endif
 *
 * \if CHINESE
 * @brief 设置U轴X分量
 * @param x 新的U轴X分量
 * \endif
 */
void QImPlot3DImageItemNode::setAxisUX(double x)
{
    QIM_D(d);
    if (!fuzzyEqual(d->axisU.x, x)) {
        d->axisU.x = x;
        Q_EMIT axisUChanged(d->axisU.x, d->axisU.y, d->axisU.z);
    }
}

/**
 * \if ENGLISH
 * @brief Get U axis Y component
 * @return Current U axis Y component
 * \endif
 *
 * \if CHINESE
 * @brief 获取U轴Y分量
 * @return 当前U轴Y分量
 * \endif
 */
double QImPlot3DImageItemNode::axisUY() const
{
    QIM_DC(d);
    return d->axisU.y;
}

/**
 * \if ENGLISH
 * @brief Set U axis Y component
 * @param y New U axis Y component
 * \endif
 *
 * \if CHINESE
 * @brief 设置U轴Y分量
 * @param y 新的U轴Y分量
 * \endif
 */
void QImPlot3DImageItemNode::setAxisUY(double y)
{
    QIM_D(d);
    if (!fuzzyEqual(d->axisU.y, y)) {
        d->axisU.y = y;
        Q_EMIT axisUChanged(d->axisU.x, d->axisU.y, d->axisU.z);
    }
}

/**
 * \if ENGLISH
 * @brief Get U axis Z component
 * @return Current U axis Z component
 * \endif
 *
 * \if CHINESE
 * @brief 获取U轴Z分量
 * @return 当前U轴Z分量
 * \endif
 */
double QImPlot3DImageItemNode::axisUZ() const
{
    QIM_DC(d);
    return d->axisU.z;
}

/**
 * \if ENGLISH
 * @brief Set U axis Z component
 * @param z New U axis Z component
 * \endif
 *
 * \if CHINESE
 * @brief 设置U轴Z分量
 * @param z 新的U轴Z分量
 * \endif
 */
void QImPlot3DImageItemNode::setAxisUZ(double z)
{
    QIM_D(d);
    if (!fuzzyEqual(d->axisU.z, z)) {
        d->axisU.z = z;
        Q_EMIT axisUChanged(d->axisU.x, d->axisU.y, d->axisU.z);
    }
}

/**
 * \if ENGLISH
 * @brief Get V axis X component
 * @return Current V axis X component
 * \endif
 *
 * \if CHINESE
 * @brief 获取V轴X分量
 * @return 当前V轴X分量
 * \endif
 */
double QImPlot3DImageItemNode::axisVX() const
{
    QIM_DC(d);
    return d->axisV.x;
}

/**
 * \if ENGLISH
 * @brief Set V axis X component
 * @param x New V axis X component
 * \endif
 *
 * \if CHINESE
 * @brief 设置V轴X分量
 * @param x 新的V轴X分量
 * \endif
 */
void QImPlot3DImageItemNode::setAxisVX(double x)
{
    QIM_D(d);
    if (!fuzzyEqual(d->axisV.x, x)) {
        d->axisV.x = x;
        Q_EMIT axisVChanged(d->axisV.x, d->axisV.y, d->axisV.z);
    }
}

/**
 * \if ENGLISH
 * @brief Get V axis Y component
 * @return Current V axis Y component
 * \endif
 *
 * \if CHINESE
 * @brief 获取V轴Y分量
 * @return 当前V轴Y分量
 * \endif
 */
double QImPlot3DImageItemNode::axisVY() const
{
    QIM_DC(d);
    return d->axisV.y;
}

/**
 * \if ENGLISH
 * @brief Set V axis Y component
 * @param y New V axis Y component
 * \endif
 *
 * \if CHINESE
 * @brief 设置V轴Y分量
 * @param y 新的V轴Y分量
 * \endif
 */
void QImPlot3DImageItemNode::setAxisVY(double y)
{
    QIM_D(d);
    if (!fuzzyEqual(d->axisV.y, y)) {
        d->axisV.y = y;
        Q_EMIT axisVChanged(d->axisV.x, d->axisV.y, d->axisV.z);
    }
}

/**
 * \if ENGLISH
 * @brief Get V axis Z component
 * @return Current V axis Z component
 * \endif
 *
 * \if CHINESE
 * @brief 获取V轴Z分量
 * @return 当前V轴Z分量
 * \endif
 */
double QImPlot3DImageItemNode::axisVZ() const
{
    QIM_DC(d);
    return d->axisV.z;
}

/**
 * \if ENGLISH
 * @brief Set V axis Z component
 * @param z New V axis Z component
 * \endif
 *
 * \if CHINESE
 * @brief 设置V轴Z分量
 * @param z 新的V轴Z分量
 * \endif
 */
void QImPlot3DImageItemNode::setAxisVZ(double z)
{
    QIM_D(d);
    if (!fuzzyEqual(d->axisV.z, z)) {
        d->axisV.z = z;
        Q_EMIT axisVChanged(d->axisV.x, d->axisV.y, d->axisV.z);
    }
}

/**
 * \if ENGLISH
 * @brief Get UV0 X coordinate
 * @return Current UV0 X coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 获取UV0 X坐标
 * @return 当前UV0 X坐标
 * \endif
 */
double QImPlot3DImageItemNode::uv0X() const
{
    QIM_DC(d);
    return static_cast<double>(d->uv0.x);
}

/**
 * \if ENGLISH
 * @brief Set UV0 X coordinate
 * @param x New UV0 X coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 设置UV0 X坐标
 * @param x 新的UV0 X坐标
 * \endif
 */
void QImPlot3DImageItemNode::setUv0X(double x)
{
    QIM_D(d);
    const float newX = static_cast<float>(x);
    if (!fuzzyEqual(d->uv0.x, newX)) {
        d->uv0.x = newX;
        Q_EMIT uv0Changed(static_cast<double>(d->uv0.x), static_cast<double>(d->uv0.y));
    }
}

/**
 * \if ENGLISH
 * @brief Get UV0 Y coordinate
 * @return Current UV0 Y coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 获取UV0 Y坐标
 * @return 当前UV0 Y坐标
 * \endif
 */
double QImPlot3DImageItemNode::uv0Y() const
{
    QIM_DC(d);
    return static_cast<double>(d->uv0.y);
}

/**
 * \if ENGLISH
 * @brief Set UV0 Y coordinate
 * @param y New UV0 Y coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 设置UV0 Y坐标
 * @param y 新的UV0 Y坐标
 * \endif
 */
void QImPlot3DImageItemNode::setUv0Y(double y)
{
    QIM_D(d);
    const float newY = static_cast<float>(y);
    if (!fuzzyEqual(d->uv0.y, newY)) {
        d->uv0.y = newY;
        Q_EMIT uv0Changed(static_cast<double>(d->uv0.x), static_cast<double>(d->uv0.y));
    }
}

/**
 * \if ENGLISH
 * @brief Get UV1 X coordinate
 * @return Current UV1 X coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 获取UV1 X坐标
 * @return 当前UV1 X坐标
 * \endif
 */
double QImPlot3DImageItemNode::uv1X() const
{
    QIM_DC(d);
    return static_cast<double>(d->uv1.x);
}

/**
 * \if ENGLISH
 * @brief Set UV1 X coordinate
 * @param x New UV1 X coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 设置UV1 X坐标
 * @param x 新的UV1 X坐标
 * \endif
 */
void QImPlot3DImageItemNode::setUv1X(double x)
{
    QIM_D(d);
    const float newX = static_cast<float>(x);
    if (!fuzzyEqual(d->uv1.x, newX)) {
        d->uv1.x = newX;
        Q_EMIT uv1Changed(static_cast<double>(d->uv1.x), static_cast<double>(d->uv1.y));
    }
}

/**
 * \if ENGLISH
 * @brief Get UV1 Y coordinate
 * @return Current UV1 Y coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 获取UV1 Y坐标
 * @return 当前UV1 Y坐标
 * \endif
 */
double QImPlot3DImageItemNode::uv1Y() const
{
    QIM_DC(d);
    return static_cast<double>(d->uv1.y);
}

/**
 * \if ENGLISH
 * @brief Set UV1 Y coordinate
 * @param y New UV1 Y coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 设置UV1 Y坐标
 * @param y 新的UV1 Y坐标
 * \endif
 */
void QImPlot3DImageItemNode::setUv1Y(double y)
{
    QIM_D(d);
    const float newY = static_cast<float>(y);
    if (!fuzzyEqual(d->uv1.y, newY)) {
        d->uv1.y = newY;
        Q_EMIT uv1Changed(static_cast<double>(d->uv1.x), static_cast<double>(d->uv1.y));
    }
}

/**
 * \if ENGLISH
 * @brief Get tint color
 * @return Current tint color as QColor
 * \endif
 *
 * \if CHINESE
 * @brief 获取色调颜色
 * @return 当前色调颜色（QColor类型）
 * \endif
 */
QColor QImPlot3DImageItemNode::tintColor() const
{
    QIM_DC(d);
    return (d->tintColor.has_value()) ? toQColor(d->tintColor->value()) : QColor(255, 255, 255);
}

/**
 * \if ENGLISH
 * @brief Set tint color
 * @param color New tint color
 * \endif
 *
 * \if CHINESE
 * @brief 设置色调颜色
 * @param color 新的色调颜色
 * \endif
 */
void QImPlot3DImageItemNode::setTintColor(const QColor& color)
{
    QIM_D(d);
    ImVec4 imColor = toImVec4(color);
    if (d->tintColor.has_value()) {
        d->tintColor->operator=(imColor);
    } else {
        d->tintColor.emplace(imColor);
        d->tintColor->mark_dirty();
    }
    Q_EMIT tintColorChanged(color);
}

/**
 * \if ENGLISH
 * @brief Get raw ImPlot3DImageFlags
 * @return Current ImPlot3DImageFlags as integer
 * \endif
 *
 * \if CHINESE
 * @brief 获取原始的ImPlot3DImageFlags
 * @return 当前的ImPlot3DImageFlags（整数形式）
 * \endif
 */
int QImPlot3DImageItemNode::imageFlags() const
{
    QIM_DC(d);
    return d->flags;
}

/**
 * \if ENGLISH
 * @brief Set raw ImPlot3DImageFlags
 * @param flags New ImPlot3DImageFlags
 * \endif
 *
 * \if CHINESE
 * @brief 设置原始的ImPlot3DImageFlags
 * @param flags 新的ImPlot3DImageFlags
 * \endif
 */
void QImPlot3DImageItemNode::setImageFlags(int flags)
{
    QIM_D(d);
    if (d->flags != flags) {
        d->flags = static_cast<ImPlot3DImageFlags>(flags);
        Q_EMIT imageFlagChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Get quad mode enabled state
 * @return true if quad mode is enabled, false otherwise
 * \endif
 *
 * \if CHINESE
 * @brief 获取四边形模式启用状态
 * @return 四边形模式是否启用
 * \endif
 */
bool QImPlot3DImageItemNode::quadModeEnabled() const
{
    QIM_DC(d);
    return d->quadModeEnabled;
}

/**
 * \if ENGLISH
 * @brief Set quad mode enabled state
 * @param enabled New quad mode state
 * \endif
 *
 * \if CHINESE
 * @brief 设置四边形模式启用状态
 * @param enabled 新的四边形模式状态
 * \endif
 */
void QImPlot3DImageItemNode::setQuadModeEnabled(bool enabled)
{
    QIM_D(d);
    if (d->quadModeEnabled != enabled) {
        d->quadModeEnabled = enabled;
        Q_EMIT quadModeEnabledChanged(enabled);
    }
}

/**
 * \if ENGLISH
 * @brief Get corner point 0 X coordinate
 * @return Current P0 X coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 获取角点0的X坐标
 * @return 当前P0 X坐标
 * \endif
 */
double QImPlot3DImageItemNode::p0x() const
{
    QIM_DC(d);
    return d->p0.x;
}

/**
 * \if ENGLISH
 * @brief Set corner point 0 X coordinate
 * @param x New P0 X coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 设置角点0的X坐标
 * @param x 新的P0 X坐标
 * \endif
 */
void QImPlot3DImageItemNode::setP0x(double x)
{
    QIM_D(d);
    if (!fuzzyEqual(d->p0.x, x)) {
        d->p0.x = x;
        Q_EMIT p0Changed(d->p0.x, d->p0.y, d->p0.z);
    }
}

/**
 * \if ENGLISH
 * @brief Get corner point 0 Y coordinate
 * @return Current P0 Y coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 获取角点0的Y坐标
 * @return 当前P0 Y坐标
 * \endif
 */
double QImPlot3DImageItemNode::p0y() const
{
    QIM_DC(d);
    return d->p0.y;
}

/**
 * \if ENGLISH
 * @brief Set corner point 0 Y coordinate
 * @param y New P0 Y coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 设置角点0的Y坐标
 * @param y 新的P0 Y坐标
 * \endif
 */
void QImPlot3DImageItemNode::setP0y(double y)
{
    QIM_D(d);
    if (!fuzzyEqual(d->p0.y, y)) {
        d->p0.y = y;
        Q_EMIT p0Changed(d->p0.x, d->p0.y, d->p0.z);
    }
}

/**
 * \if ENGLISH
 * @brief Get corner point 0 Z coordinate
 * @return Current P0 Z coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 获取角点0的Z坐标
 * @return 当前P0 Z坐标
 * \endif
 */
double QImPlot3DImageItemNode::p0z() const
{
    QIM_DC(d);
    return d->p0.z;
}

/**
 * \if ENGLISH
 * @brief Set corner point 0 Z coordinate
 * @param z New P0 Z coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 设置角点0的Z坐标
 * @param z 新的P0 Z坐标
 * \endif
 */
void QImPlot3DImageItemNode::setP0z(double z)
{
    QIM_D(d);
    if (!fuzzyEqual(d->p0.z, z)) {
        d->p0.z = z;
        Q_EMIT p0Changed(d->p0.x, d->p0.y, d->p0.z);
    }
}

/**
 * \if ENGLISH
 * @brief Get corner point 1 X coordinate
 * @return Current P1 X coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 获取角点1的X坐标
 * @return 当前P1 X坐标
 * \endif
 */
double QImPlot3DImageItemNode::p1x() const
{
    QIM_DC(d);
    return d->p1.x;
}

/**
 * \if ENGLISH
 * @brief Set corner point 1 X coordinate
 * @param x New P1 X coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 设置角点1的X坐标
 * @param x 新的P1 X坐标
 * \endif
 */
void QImPlot3DImageItemNode::setP1x(double x)
{
    QIM_D(d);
    if (!fuzzyEqual(d->p1.x, x)) {
        d->p1.x = x;
        Q_EMIT p1Changed(d->p1.x, d->p1.y, d->p1.z);
    }
}

/**
 * \if ENGLISH
 * @brief Get corner point 1 Y coordinate
 * @return Current P1 Y coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 获取角点1的Y坐标
 * @return 当前P1 Y坐标
 * \endif
 */
double QImPlot3DImageItemNode::p1y() const
{
    QIM_DC(d);
    return d->p1.y;
}

/**
 * \if ENGLISH
 * @brief Set corner point 1 Y coordinate
 * @param y New P1 Y coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 设置角点1的Y坐标
 * @param y 新的P1 Y坐标
 * \endif
 */
void QImPlot3DImageItemNode::setP1y(double y)
{
    QIM_D(d);
    if (!fuzzyEqual(d->p1.y, y)) {
        d->p1.y = y;
        Q_EMIT p1Changed(d->p1.x, d->p1.y, d->p1.z);
    }
}

/**
 * \if ENGLISH
 * @brief Get corner point 1 Z coordinate
 * @return Current P1 Z coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 获取角点1的Z坐标
 * @return 当前P1 Z坐标
 * \endif
 */
double QImPlot3DImageItemNode::p1z() const
{
    QIM_DC(d);
    return d->p1.z;
}

/**
 * \if ENGLISH
 * @brief Set corner point 1 Z coordinate
 * @param z New P1 Z coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 设置角点1的Z坐标
 * @param z 新的P1 Z坐标
 * \endif
 */
void QImPlot3DImageItemNode::setP1z(double z)
{
    QIM_D(d);
    if (!fuzzyEqual(d->p1.z, z)) {
        d->p1.z = z;
        Q_EMIT p1Changed(d->p1.x, d->p1.y, d->p1.z);
    }
}

/**
 * \if ENGLISH
 * @brief Get corner point 2 X coordinate
 * @return Current P2 X coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 获取角点2的X坐标
 * @return 当前P2 X坐标
 * \endif
 */
double QImPlot3DImageItemNode::p2x() const
{
    QIM_DC(d);
    return d->p2.x;
}

/**
 * \if ENGLISH
 * @brief Set corner point 2 X coordinate
 * @param x New P2 X coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 设置角点2的X坐标
 * @param x 新的P2 X坐标
 * \endif
 */
void QImPlot3DImageItemNode::setP2x(double x)
{
    QIM_D(d);
    if (!fuzzyEqual(d->p2.x, x)) {
        d->p2.x = x;
        Q_EMIT p2Changed(d->p2.x, d->p2.y, d->p2.z);
    }
}

/**
 * \if ENGLISH
 * @brief Get corner point 2 Y coordinate
 * @return Current P2 Y coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 获取角点2的Y坐标
 * @return 当前P2 Y坐标
 * \endif
 */
double QImPlot3DImageItemNode::p2y() const
{
    QIM_DC(d);
    return d->p2.y;
}

/**
 * \if ENGLISH
 * @brief Set corner point 2 Y coordinate
 * @param y New P2 Y coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 设置角点2的Y坐标
 * @param y 新的P2 Y坐标
 * \endif
 */
void QImPlot3DImageItemNode::setP2y(double y)
{
    QIM_D(d);
    if (!fuzzyEqual(d->p2.y, y)) {
        d->p2.y = y;
        Q_EMIT p2Changed(d->p2.x, d->p2.y, d->p2.z);
    }
}

/**
 * \if ENGLISH
 * @brief Get corner point 2 Z coordinate
 * @return Current P2 Z coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 获取角点2的Z坐标
 * @return 当前P2 Z坐标
 * \endif
 */
double QImPlot3DImageItemNode::p2z() const
{
    QIM_DC(d);
    return d->p2.z;
}

/**
 * \if ENGLISH
 * @brief Set corner point 2 Z coordinate
 * @param z New P2 Z coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 设置角点2的Z坐标
 * @param z 新的P2 Z坐标
 * \endif
 */
void QImPlot3DImageItemNode::setP2z(double z)
{
    QIM_D(d);
    if (!fuzzyEqual(d->p2.z, z)) {
        d->p2.z = z;
        Q_EMIT p2Changed(d->p2.x, d->p2.y, d->p2.z);
    }
}

/**
 * \if ENGLISH
 * @brief Get corner point 3 X coordinate
 * @return Current P3 X coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 获取角点3的X坐标
 * @return 当前P3 X坐标
 * \endif
 */
double QImPlot3DImageItemNode::p3x() const
{
    QIM_DC(d);
    return d->p3.x;
}

/**
 * \if ENGLISH
 * @brief Set corner point 3 X coordinate
 * @param x New P3 X coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 设置角点3的X坐标
 * @param x 新的P3 X坐标
 * \endif
 */
void QImPlot3DImageItemNode::setP3x(double x)
{
    QIM_D(d);
    if (!fuzzyEqual(d->p3.x, x)) {
        d->p3.x = x;
        Q_EMIT p3Changed(d->p3.x, d->p3.y, d->p3.z);
    }
}

/**
 * \if ENGLISH
 * @brief Get corner point 3 Y coordinate
 * @return Current P3 Y coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 获取角点3的Y坐标
 * @return 当前P3 Y坐标
 * \endif
 */
double QImPlot3DImageItemNode::p3y() const
{
    QIM_DC(d);
    return d->p3.y;
}

/**
 * \if ENGLISH
 * @brief Set corner point 3 Y coordinate
 * @param y New P3 Y coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 设置角点3的Y坐标
 * @param y 新的P3 Y坐标
 * \endif
 */
void QImPlot3DImageItemNode::setP3y(double y)
{
    QIM_D(d);
    if (!fuzzyEqual(d->p3.y, y)) {
        d->p3.y = y;
        Q_EMIT p3Changed(d->p3.x, d->p3.y, d->p3.z);
    }
}

/**
 * \if ENGLISH
 * @brief Get corner point 3 Z coordinate
 * @return Current P3 Z coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 获取角点3的Z坐标
 * @return 当前P3 Z坐标
 * \endif
 */
double QImPlot3DImageItemNode::p3z() const
{
    QIM_DC(d);
    return d->p3.z;
}

/**
 * \if ENGLISH
 * @brief Set corner point 3 Z coordinate
 * @param z New P3 Z coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 设置角点3的Z坐标
 * @param z 新的P3 Z坐标
 * \endif
 */
void QImPlot3DImageItemNode::setP3z(double z)
{
    QIM_D(d);
    if (!fuzzyEqual(d->p3.z, z)) {
        d->p3.z = z;
        Q_EMIT p3Changed(d->p3.x, d->p3.y, d->p3.z);
    }
}

/**
 * \if ENGLISH
 * @brief Get UV point 0 X coordinate
 * @return Current UV P0 X coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 获取UV点0的X坐标
 * @return 当前UV P0 X坐标
 * \endif
 */
double QImPlot3DImageItemNode::uvP0x() const
{
    QIM_DC(d);
    return static_cast<double>(d->uvP0.x);
}

/**
 * \if ENGLISH
 * @brief Set UV point 0 X coordinate
 * @param x New UV P0 X coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 设置UV点0的X坐标
 * @param x 新的UV P0 X坐标
 * \endif
 */
void QImPlot3DImageItemNode::setUvP0x(double x)
{
    QIM_D(d);
    const float newX = static_cast<float>(x);
    if (!fuzzyEqual(d->uvP0.x, newX)) {
        d->uvP0.x = newX;
        Q_EMIT uvP0Changed(static_cast<double>(d->uvP0.x), static_cast<double>(d->uvP0.y));
    }
}

/**
 * \if ENGLISH
 * @brief Get UV point 0 Y coordinate
 * @return Current UV P0 Y coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 获取UV点0的Y坐标
 * @return 当前UV P0 Y坐标
 * \endif
 */
double QImPlot3DImageItemNode::uvP0y() const
{
    QIM_DC(d);
    return static_cast<double>(d->uvP0.y);
}

/**
 * \if ENGLISH
 * @brief Set UV point 0 Y coordinate
 * @param y New UV P0 Y coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 设置UV点0的Y坐标
 * @param y 新的UV P0 Y坐标
 * \endif
 */
void QImPlot3DImageItemNode::setUvP0y(double y)
{
    QIM_D(d);
    const float newY = static_cast<float>(y);
    if (!fuzzyEqual(d->uvP0.y, newY)) {
        d->uvP0.y = newY;
        Q_EMIT uvP0Changed(static_cast<double>(d->uvP0.x), static_cast<double>(d->uvP0.y));
    }
}

/**
 * \if ENGLISH
 * @brief Get UV point 1 X coordinate
 * @return Current UV P1 X coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 获取UV点1的X坐标
 * @return 当前UV P1 X坐标
 * \endif
 */
double QImPlot3DImageItemNode::uvP1x() const
{
    QIM_DC(d);
    return static_cast<double>(d->uvP1.x);
}

/**
 * \if ENGLISH
 * @brief Set UV point 1 X coordinate
 * @param x New UV P1 X coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 设置UV点1的X坐标
 * @param x 新的UV P1 X坐标
 * \endif
 */
void QImPlot3DImageItemNode::setUvP1x(double x)
{
    QIM_D(d);
    const float newX = static_cast<float>(x);
    if (!fuzzyEqual(d->uvP1.x, newX)) {
        d->uvP1.x = newX;
        Q_EMIT uvP1Changed(static_cast<double>(d->uvP1.x), static_cast<double>(d->uvP1.y));
    }
}

/**
 * \if ENGLISH
 * @brief Get UV point 1 Y coordinate
 * @return Current UV P1 Y coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 获取UV点1的Y坐标
 * @return 当前UV P1 Y坐标
 * \endif
 */
double QImPlot3DImageItemNode::uvP1y() const
{
    QIM_DC(d);
    return static_cast<double>(d->uvP1.y);
}

/**
 * \if ENGLISH
 * @brief Set UV point 1 Y coordinate
 * @param y New UV P1 Y coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 设置UV点1的Y坐标
 * @param y 新的UV P1 Y坐标
 * \endif
 */
void QImPlot3DImageItemNode::setUvP1y(double y)
{
    QIM_D(d);
    const float newY = static_cast<float>(y);
    if (!fuzzyEqual(d->uvP1.y, newY)) {
        d->uvP1.y = newY;
        Q_EMIT uvP1Changed(static_cast<double>(d->uvP1.x), static_cast<double>(d->uvP1.y));
    }
}

/**
 * \if ENGLISH
 * @brief Get UV point 2 X coordinate
 * @return Current UV P2 X coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 获取UV点2的X坐标
 * @return 当前UV P2 X坐标
 * \endif
 */
double QImPlot3DImageItemNode::uvP2x() const
{
    QIM_DC(d);
    return static_cast<double>(d->uvP2.x);
}

/**
 * \if ENGLISH
 * @brief Set UV point 2 X coordinate
 * @param x New UV P2 X coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 设置UV点2的X坐标
 * @param x 新的UV P2 X坐标
 * \endif
 */
void QImPlot3DImageItemNode::setUvP2x(double x)
{
    QIM_D(d);
    const float newX = static_cast<float>(x);
    if (!fuzzyEqual(d->uvP2.x, newX)) {
        d->uvP2.x = newX;
        Q_EMIT uvP2Changed(static_cast<double>(d->uvP2.x), static_cast<double>(d->uvP2.y));
    }
}

/**
 * \if ENGLISH
 * @brief Get UV point 2 Y coordinate
 * @return Current UV P2 Y coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 获取UV点2的Y坐标
 * @return 当前UV P2 Y坐标
 * \endif
 */
double QImPlot3DImageItemNode::uvP2y() const
{
    QIM_DC(d);
    return static_cast<double>(d->uvP2.y);
}

/**
 * \if ENGLISH
 * @brief Set UV point 2 Y coordinate
 * @param y New UV P2 Y coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 设置UV点2的Y坐标
 * @param y 新的UV P2 Y坐标
 * \endif
 */
void QImPlot3DImageItemNode::setUvP2y(double y)
{
    QIM_D(d);
    const float newY = static_cast<float>(y);
    if (!fuzzyEqual(d->uvP2.y, newY)) {
        d->uvP2.y = newY;
        Q_EMIT uvP2Changed(static_cast<double>(d->uvP2.x), static_cast<double>(d->uvP2.y));
    }
}

/**
 * \if ENGLISH
 * @brief Get UV point 3 X coordinate
 * @return Current UV P3 X coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 获取UV点3的X坐标
 * @return 当前UV P3 X坐标
 * \endif
 */
double QImPlot3DImageItemNode::uvP3x() const
{
    QIM_DC(d);
    return static_cast<double>(d->uvP3.x);
}

/**
 * \if ENGLISH
 * @brief Set UV point 3 X coordinate
 * @param x New UV P3 X coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 设置UV点3的X坐标
 * @param x 新的UV P3 X坐标
 * \endif
 */
void QImPlot3DImageItemNode::setUvP3x(double x)
{
    QIM_D(d);
    const float newX = static_cast<float>(x);
    if (!fuzzyEqual(d->uvP3.x, newX)) {
        d->uvP3.x = newX;
        Q_EMIT uvP3Changed(static_cast<double>(d->uvP3.x), static_cast<double>(d->uvP3.y));
    }
}

/**
 * \if ENGLISH
 * @brief Get UV point 3 Y coordinate
 * @return Current UV P3 Y coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 获取UV点3的Y坐标
 * @return 当前UV P3 Y坐标
 * \endif
 */
double QImPlot3DImageItemNode::uvP3y() const
{
    QIM_DC(d);
    return static_cast<double>(d->uvP3.y);
}

/**
 * \if ENGLISH
 * @brief Set UV point 3 Y coordinate
 * @param y New UV P3 Y coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 设置UV点3的Y坐标
 * @param y 新的UV P3 Y坐标
 * \endif
 */
void QImPlot3DImageItemNode::setUvP3y(double y)
{
    QIM_D(d);
    const float newY = static_cast<float>(y);
    if (!fuzzyEqual(d->uvP3.y, newY)) {
        d->uvP3.y = newY;
        Q_EMIT uvP3Changed(static_cast<double>(d->uvP3.x), static_cast<double>(d->uvP3.y));
    }
}

/**
 * \if ENGLISH
 * @brief Convenience method to set all quad image parameters at once
 * @param textureId Texture identifier for the image
 * @param p0x Corner point 0 X coordinate
 * @param p0y Corner point 0 Y coordinate
 * @param p0z Corner point 0 Z coordinate
 * @param p1x Corner point 1 X coordinate
 * @param p1y Corner point 1 Y coordinate
 * @param p1z Corner point 1 Z coordinate
 * @param p2x Corner point 2 X coordinate
 * @param p2y Corner point 2 Y coordinate
 * @param p2z Corner point 2 Z coordinate
 * @param p3x Corner point 3 X coordinate
 * @param p3y Corner point 3 Y coordinate
 * @param p3z Corner point 3 Z coordinate
 * @param uvP0x UV point 0 X coordinate
 * @param uvP0y UV point 0 Y coordinate
 * @param uvP1x UV point 1 X coordinate
 * @param uvP1y UV point 1 Y coordinate
 * @param uvP2x UV point 2 X coordinate
 * @param uvP2y UV point 2 Y coordinate
 * @param uvP3x UV point 3 X coordinate
 * @param uvP3y UV point 3 Y coordinate
 * @param tintCol Tint color applied to the image (default: white)
 * \endif
 *
 * \if CHINESE
 * @brief 一次性设置所有四边形图像参数的便捷方法
 * @param textureId 图像纹理标识符
 * @param p0x 角点0的X坐标
 * @param p0y 角点0的Y坐标
 * @param p0z 角点0的Z坐标
 * @param p1x 角点1的X坐标
 * @param p1y 角点1的Y坐标
 * @param p1z 角点1的Z坐标
 * @param p2x 角点2的X坐标
 * @param p2y 角点2的Y坐标
 * @param p2z 角点2的Z坐标
 * @param p3x 角点3的X坐标
 * @param p3y 角点3的Y坐标
 * @param p3z 角点3的Z坐标
 * @param uvP0x UV点0的X坐标
 * @param uvP0y UV点0的Y坐标
 * @param uvP1x UV点1的X坐标
 * @param uvP1y UV点1的Y坐标
 * @param uvP2x UV点2的X坐标
 * @param uvP2y UV点2的Y坐标
 * @param uvP3x UV点3的X坐标
 * @param uvP3y UV点3的Y坐标
 * @param tintCol 应用于图像的色调颜色（默认：白色）
 * \endif
 */
void QImPlot3DImageItemNode::setQuadImage(quintptr textureId, double p0x, double p0y, double p0z, double p1x, double p1y, double p1z,
                                           double p2x, double p2y, double p2z, double p3x, double p3y, double p3z,
                                           double uvP0x, double uvP0y, double uvP1x, double uvP1y, double uvP2x, double uvP2y,
                                           double uvP3x, double uvP3y, const QColor& tintCol)
{
    setQuadModeEnabled(true);
    setTextureId(textureId);
    setP0x(p0x);
    setP0y(p0y);
    setP0z(p0z);
    setP1x(p1x);
    setP1y(p1y);
    setP1z(p1z);
    setP2x(p2x);
    setP2y(p2y);
    setP2z(p2z);
    setP3x(p3x);
    setP3y(p3y);
    setP3z(p3z);
    setUvP0x(uvP0x);
    setUvP0y(uvP0y);
    setUvP1x(uvP1x);
    setUvP1y(uvP1y);
    setUvP2x(uvP2x);
    setUvP2y(uvP2y);
    setUvP3x(uvP3x);
    setUvP3y(uvP3y);
    setTintColor(tintCol);
}

/**
 * \if ENGLISH
 * @brief Begin drawing implementation
 * @return false to prevent endDraw from being called
 * \endif
 *
 * \if CHINESE
 * @brief 开始绘制实现
 * @return false以防止调用endDraw
 * \endif
 */
bool QImPlot3DImageItemNode::beginDraw()
{
    QIM_D(d);
    if (d->textureId == 0) {
        // No valid texture ID, skip drawing
        return false;
    }

    // Use pre-converted data directly
    ImTextureRef texRef = (ImTextureID)(d->textureId);
    const ImVec4 tint_col = d->tintColor.has_value() ? d->tintColor->value() : ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

    if (d->quadModeEnabled) {
        // Call ImPlot3D API with 4-point quad overload
        ImPlot3D::PlotImage(
            labelConstData(),
            texRef,
            d->p0,
            d->p1,
            d->p2,
            d->p3,
            d->uvP0,
            d->uvP1,
            d->uvP2,
            d->uvP3,
            tint_col,
            d->flags
        );
    } else {
        // Call ImPlot3D API with center+axes overload
        ImPlot3D::PlotImage(
            labelConstData(),
            texRef,
            d->center,
            d->axisU,
            d->axisV,
            d->uv0,
            d->uv1,
            tint_col,
            d->flags
        );
    }

    return false;
}

}  // namespace QIM
