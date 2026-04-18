#include "QImPlotImageItemNode.h"
#include <memory>
#include <optional>
#include "implot.h"
#include "implot_internal.h"
#include "QImTrackedValue.hpp"
#include "QtImGuiUtils.h"
#include <QDebug>

namespace QIM
{

class QImPlotImageItemNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlotImageItemNode)
public:
    PrivateData(QImPlotImageItemNode* p);

    quintptr textureId { 0 };
    ImPlotPoint boundsMin { 0.0, 0.0 };
    ImPlotPoint boundsMax { 10.0, 10.0 };
    ImVec2 uv0 { 0.0f, 0.0f };
    ImVec2 uv1 { 1.0f, 1.0f };
    QImOptionalColor tintColor;
    ImPlotImageFlags flags { ImPlotImageFlags_None };
};

QImPlotImageItemNode::PrivateData::PrivateData(QImPlotImageItemNode* p) : q_ptr(p)
{
}

/**
 * \if ENGLISH
 * @brief Constructor for QImPlotImageItemNode
 * @param parent Parent QObject
 * \endif
 *
 * \if CHINESE
 * @brief QImPlotImageItemNode的构造函数
 * @param parent 父QObject
 * \endif
 */
QImPlotImageItemNode::QImPlotImageItemNode(QObject* parent) : QImPlotItemNode(parent), QIM_PIMPL_CONSTRUCT
{
}

/**
 * \if ENGLISH
 * @brief Destructor for QImPlotImageItemNode
 * \endif
 *
 * \if CHINESE
 * @brief QImPlotImageItemNode的析构函数
 * \endif
 */
QImPlotImageItemNode::~QImPlotImageItemNode()
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
quintptr QImPlotImageItemNode::textureId() const
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
void QImPlotImageItemNode::setTextureId(quintptr id)
{
    QIM_D(d);
    if (d->textureId != id) {
        d->textureId = id;
        Q_EMIT textureIdChanged(id);
    }
}

/**
 * \if ENGLISH
 * @brief Get lower-left bounds
 * @return Current bounds minimum as QPointF
 * \endif
 *
 * \if CHINESE
 * @brief 获取左下角边界
 * @return 当前边界最小值（QPointF类型）
 * \endif
 */
QPointF QImPlotImageItemNode::boundsMin() const
{
    QIM_DC(d);
    return toQPointF(d->boundsMin);
}

/**
 * \if ENGLISH
 * @brief Set lower-left bounds
 * @param min New bounds minimum position
 * \endif
 *
 * \if CHINESE
 * @brief 设置左下角边界
 * @param min 新的边界最小值位置
 * \endif
 */
void QImPlotImageItemNode::setBoundsMin(const QPointF& min)
{
    QIM_D(d);
    const ImPlotPoint newMin = toImPlotPoint(min);
    if (d->boundsMin.x != newMin.x || d->boundsMin.y != newMin.y) {
        d->boundsMin = newMin;
        Q_EMIT boundsMinChanged(min);
    }
}

/**
 * \if ENGLISH
 * @brief Get upper-right bounds
 * @return Current bounds maximum as QPointF
 * \endif
 *
 * \if CHINESE
 * @brief 获取右上角边界
 * @return 当前边界最大值（QPointF类型）
 * \endif
 */
QPointF QImPlotImageItemNode::boundsMax() const
{
    QIM_DC(d);
    return toQPointF(d->boundsMax);
}

/**
 * \if ENGLISH
 * @brief Set upper-right bounds
 * @param max New bounds maximum position
 * \endif
 *
 * \if CHINESE
 * @brief 设置右上角边界
 * @param max 新的边界最大值位置
 * \endif
 */
void QImPlotImageItemNode::setBoundsMax(const QPointF& max)
{
    QIM_D(d);
    const ImPlotPoint newMax = toImPlotPoint(max);
    if (d->boundsMax.x != newMax.x || d->boundsMax.y != newMax.y) {
        d->boundsMax = newMax;
        Q_EMIT boundsMaxChanged(max);
    }
}

/**
 * \if ENGLISH
 * @brief Get lower-left texture coordinate UV0
 * @return Current UV0 as QPointF
 * \endif
 *
 * \if CHINESE
 * @brief 获取左下角纹理坐标 UV0
 * @return 当前UV0坐标（QPointF类型）
 * \endif
 */
QPointF QImPlotImageItemNode::uv0() const
{
    QIM_DC(d);
    return toQPointF(d->uv0);
}

/**
 * \if ENGLISH
 * @brief Set lower-left texture coordinate UV0
 * @param uv New UV0 coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 设置左下角纹理坐标 UV0
 * @param uv 新的UV0坐标
 * \endif
 */
void QImPlotImageItemNode::setUv0(const QPointF& uv)
{
    QIM_D(d);
    const ImVec2 newUv = toImVec2(uv);
    if (!fuzzyEqual(d->uv0.x, newUv.x) || !fuzzyEqual(d->uv0.y, newUv.y)) {
        d->uv0 = newUv;
        Q_EMIT uv0Changed(uv);
    }
}

/**
 * \if ENGLISH
 * @brief Get upper-right texture coordinate UV1
 * @return Current UV1 as QPointF
 * \endif
 *
 * \if CHINESE
 * @brief 获取右上角纹理坐标 UV1
 * @return 当前UV1坐标（QPointF类型）
 * \endif
 */
QPointF QImPlotImageItemNode::uv1() const
{
    QIM_DC(d);
    return toQPointF(d->uv1);
}

/**
 * \if ENGLISH
 * @brief Set upper-right texture coordinate UV1
 * @param uv New UV1 coordinate
 * \endif
 *
 * \if CHINESE
 * @brief 设置右上角纹理坐标 UV1
 * @param uv 新的UV1坐标
 * \endif
 */
void QImPlotImageItemNode::setUv1(const QPointF& uv)
{
    QIM_D(d);
    const ImVec2 newUv = toImVec2(uv);
    if (!fuzzyEqual(d->uv1.x, newUv.x) || !fuzzyEqual(d->uv1.y, newUv.y)) {
        d->uv1 = newUv;
        Q_EMIT uv1Changed(uv);
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
QColor QImPlotImageItemNode::tintColor() const
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
void QImPlotImageItemNode::setTintColor(const QColor& color)
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
 * @brief Get raw ImPlotImageFlags
 * @return Current ImPlotImageFlags as integer
 * \endif
 *
 * \if CHINESE
 * @brief 获取原始的ImPlotImageFlags
 * @return 当前的ImPlotImageFlags（整数形式）
 * \endif
 */
int QImPlotImageItemNode::imageFlags() const
{
    QIM_DC(d);
    return d->flags;
}

/**
 * \if ENGLISH
 * @brief Set raw ImPlotImageFlags
 * @param flags New ImPlotImageFlags
 * \endif
 *
 * \if CHINESE
 * @brief 设置原始的ImPlotImageFlags
 * @param flags 新的ImPlotImageFlags
 * \endif
 */
void QImPlotImageItemNode::setImageFlags(int flags)
{
    QIM_D(d);
    if (d->flags != flags) {
        d->flags = static_cast<ImPlotImageFlags>(flags);
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
bool QImPlotImageItemNode::beginDraw()
{
    QIM_D(d);
    if (d->textureId == 0) {
        // No valid texture ID, skip drawing
        return false;
    }

    // Use pre-converted data directly
    ImTextureID texRef = (ImTextureID)d->textureId;
    const ImVec4 tint_col = d->tintColor.has_value() ? d->tintColor->value() : ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

    // Call ImPlot API
    ImPlot::PlotImage(
        labelConstData(),
        texRef,
        d->boundsMin,
        d->boundsMax,
        d->uv0,
        d->uv1,
        tint_col,
        d->flags
    );

    // Update item status
    ImPlotContext* ct    = ImPlot::GetCurrentContext();
    if (!ct) {
        return false;
    }
    ImPlotItem* plotItem = ct->PreviousItem;
    if (!plotItem) {
        return false;
    }
    setImPlotItem(plotItem);
    if (plotItem->Show != QImAbstractNode::isVisible()) {
        QImAbstractNode::setVisible(plotItem->Show);
    }

    return false;
}

}  // namespace QIM
