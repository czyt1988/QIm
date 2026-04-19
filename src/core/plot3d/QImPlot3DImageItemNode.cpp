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

    return false;
}

}  // namespace QIM
