#include "QImPlot3DMeshItemNode.h"
#include "QImAPI.h"
#include "QtImGuiUtils.h"
#include "implot3d.h"
#include "implot3d_internal.h"

namespace QIM
{

// ===============================================================
// PrivateData Implementation
// ===============================================================

class QImPlot3DMeshItemNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlot3DMeshItemNode)

public:
    explicit PrivateData(QImPlot3DMeshItemNode* q) : q_ptr(q) {}

    std::vector<ImPlot3DPoint> vertices;
    std::vector<unsigned int> indices;
    int flags{0};
    int markerShape{ImPlot3DMarker_None};
    float markerSize{4.0f};
    float markerWeight{1.0f};
    QImOptional3DColor fillColor;
    QImOptional3DColor lineColor;
    QImOptional3DColor markerFillColor;
    QImOptional3DColor markerOutlineColor;
    float lineWidth{1.0f};
};

// ===============================================================
// QImPlot3DMeshItemNode Implementation
// ===============================================================

QImPlot3DMeshItemNode::QImPlot3DMeshItemNode(QObject* parent)
    : QImPlot3DItemNode(parent)
    , QIM_PIMPL_CONSTRUCT
{
}

QImPlot3DMeshItemNode::~QImPlot3DMeshItemNode()
{
}

void QImPlot3DMeshItemNode::setMeshData(const std::vector<ImPlot3DPoint>& vertices, const std::vector<unsigned int>& indices)
{
    QIM_D(d);
    d->vertices = vertices;
    d->indices = indices;
    Q_EMIT dataChanged();
}

const std::vector<ImPlot3DPoint>& QImPlot3DMeshItemNode::vertices() const
{
    QIM_DC(d);
    return d->vertices;
}

const std::vector<unsigned int>& QImPlot3DMeshItemNode::indices() const
{
    QIM_DC(d);
    return d->indices;
}

QIMPLOT3D_FLAG_ENABLED_ACCESSOR(QImPlot3DMeshItemNode, LinesVisible, ImPlot3DMeshFlags_NoLines, meshFlagChanged)
QIMPLOT3D_FLAG_ENABLED_ACCESSOR(QImPlot3DMeshItemNode, FillVisible, ImPlot3DMeshFlags_NoFill, meshFlagChanged)
QIMPLOT3D_FLAG_ENABLED_ACCESSOR(QImPlot3DMeshItemNode, MarkersVisible, ImPlot3DMeshFlags_NoMarkers, meshFlagChanged)

int QImPlot3DMeshItemNode::markerShape() const
{
    QIM_DC(d);
    return d->markerShape;
}

void QImPlot3DMeshItemNode::setMarkerShape(int shape)
{
    QIM_D(d);
    if (d->markerShape != shape) {
        d->markerShape = shape;
        Q_EMIT markerShapeChanged(shape);
    }
}

float QImPlot3DMeshItemNode::markerSize() const
{
    QIM_DC(d);
    return d->markerSize;
}

void QImPlot3DMeshItemNode::setMarkerSize(float size)
{
    QIM_D(d);
    if (!qFuzzyCompare(d->markerSize, size)) {
        d->markerSize = size;
        Q_EMIT markerStyleChanged();
    }
}

float QImPlot3DMeshItemNode::markerWeight() const
{
    QIM_DC(d);
    return d->markerWeight;
}

void QImPlot3DMeshItemNode::setMarkerWeight(float weight)
{
    QIM_D(d);
    if (!qFuzzyCompare(d->markerWeight, weight)) {
        d->markerWeight = weight;
        Q_EMIT markerStyleChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Returns the fill color, or invalid QColor if not set
 * @details When no fill color has been explicitly set, returns an invalid QColor().
 *          After the first render, unset colors capture the ImPlot3D default.
 * \endif
 *
 * \if CHINESE
 * @brief 返回填充颜色，若未设置则返回无效QColor
 * @details 当未显式设置填充颜色时，返回无效的QColor()。
 *          首次渲染后，未设置的颜色会捕获ImPlot3D默认值。
 * \endif
 */
QColor QImPlot3DMeshItemNode::fillColor() const
{
    QIM_DC(d);
    return (d->fillColor.has_value()) ? toQColor(d->fillColor->value()) : QColor();
}

/**
 * \if ENGLISH
 * @brief Sets the fill color and emits fillColorChanged
 * @param[in] color The new fill color
 * @details Stores the color as QImOptional3DColor for lazy initialization.
 *          Always emits fillColorChanged to satisfy Q_PROPERTY NOTIFY.
 * \endif
 *
 * \if CHINESE
 * @brief 设置填充颜色并发射fillColorChanged信号
 * @param[in] color 新的填充颜色
 * @details 将颜色存储为QImOptional3DColor以支持延迟初始化。
 *          总是发射fillColorChanged以满足Q_PROPERTY NOTIFY。
 * \endif
 */
void QImPlot3DMeshItemNode::setFillColor(const QColor& color)
{
    QIM_D(d);
    d->fillColor = toImVec4(color);
    Q_EMIT fillColorChanged(color);
}

/**
 * \if ENGLISH
 * @brief Returns the line color, or invalid QColor if not set
 * @details When no line color has been explicitly set, returns an invalid QColor().
 *          After the first render, unset colors capture the ImPlot3D default.
 * \endif
 *
 * \if CHINESE
 * @brief 返回线条颜色，若未设置则返回无效QColor
 * @details 当未显式设置线条颜色时，返回无效的QColor()。
 *          首次渲染后，未设置的颜色会捕获ImPlot3D默认值。
 * \endif
 */
QColor QImPlot3DMeshItemNode::lineColor() const
{
    QIM_DC(d);
    return (d->lineColor.has_value()) ? toQColor(d->lineColor->value()) : QColor();
}

/**
 * \if ENGLISH
 * @brief Sets the line color and emits lineColorChanged
 * @param[in] color The new line color
 * @details Stores the color as QImOptional3DColor for lazy initialization.
 *          Always emits lineColorChanged to satisfy Q_PROPERTY NOTIFY.
 * \endif
 *
 * \if CHINESE
 * @brief 设置线条颜色并发射lineColorChanged信号
 * @param[in] color 新的线条颜色
 * @details 将颜色存储为QImOptional3DColor以支持延迟初始化。
 *          总是发射lineColorChanged以满足Q_PROPERTY NOTIFY。
 * \endif
 */
void QImPlot3DMeshItemNode::setLineColor(const QColor& color)
{
    QIM_D(d);
    d->lineColor = toImVec4(color);
    Q_EMIT lineColorChanged(color);
}

/**
 * \if ENGLISH
 * @brief Returns the marker fill color, or invalid QColor if not set
 * @details When no marker fill color has been explicitly set, returns an invalid QColor().
 *          After the first render, unset colors capture the ImPlot3D default.
 * \endif
 *
 * \if CHINESE
 * @brief 返回标记填充颜色，若未设置则返回无效QColor
 * @details 当未显式设置标记填充颜色时，返回无效的QColor()。
 *          首次渲染后，未设置的颜色会捕获ImPlot3D默认值。
 * \endif
 */
QColor QImPlot3DMeshItemNode::markerFillColor() const
{
    QIM_DC(d);
    return (d->markerFillColor.has_value()) ? toQColor(d->markerFillColor->value()) : QColor();
}

/**
 * \if ENGLISH
 * @brief Sets the marker fill color and emits markerFillColorChanged
 * @param[in] color The new marker fill color
 * @details Stores the color as QImOptional3DColor for lazy initialization.
 *          Always emits markerFillColorChanged to satisfy Q_PROPERTY NOTIFY.
 * \endif
 *
 * \if CHINESE
 * @brief 设置标记填充颜色并发射markerFillColorChanged信号
 * @param[in] color 新的标记填充颜色
 * @details 将颜色存储为QImOptional3DColor以支持延迟初始化。
 *          总是发射markerFillColorChanged以满足Q_PROPERTY NOTIFY。
 * \endif
 */
void QImPlot3DMeshItemNode::setMarkerFillColor(const QColor& color)
{
    QIM_D(d);
    d->markerFillColor = toImVec4(color);
    Q_EMIT markerFillColorChanged(color);
}

/**
 * \if ENGLISH
 * @brief Returns the marker outline color, or invalid QColor if not set
 * @details When no marker outline color has been explicitly set, returns an invalid QColor().
 *          After the first render, unset colors capture the ImPlot3D default.
 * \endif
 *
 * \if CHINESE
 * @brief 返回标记轮廓颜色，若未设置则返回无效QColor
 * @details 当未显式设置标记轮廓颜色时，返回无效的QColor()。
 *          首次渲染后，未设置的颜色会捕获ImPlot3D默认值。
 * \endif
 */
QColor QImPlot3DMeshItemNode::markerOutlineColor() const
{
    QIM_DC(d);
    return (d->markerOutlineColor.has_value()) ? toQColor(d->markerOutlineColor->value()) : QColor();
}

/**
 * \if ENGLISH
 * @brief Sets the marker outline color and emits markerOutlineColorChanged
 * @param[in] color The new marker outline color
 * @details Stores the color as QImOptional3DColor for lazy initialization.
 *          Always emits markerOutlineColorChanged to satisfy Q_PROPERTY NOTIFY.
 * \endif
 *
 * \if CHINESE
 * @brief 设置标记轮廓颜色并发射markerOutlineColorChanged信号
 * @param[in] color 新的标记轮廓颜色
 * @details 将颜色存储为QImOptional3DColor以支持延迟初始化。
 *          总是发射markerOutlineColorChanged以满足Q_PROPERTY NOTIFY。
 * \endif
 */
void QImPlot3DMeshItemNode::setMarkerOutlineColor(const QColor& color)
{
    QIM_D(d);
    d->markerOutlineColor = toImVec4(color);
    Q_EMIT markerOutlineColorChanged(color);
}

float QImPlot3DMeshItemNode::lineWidth() const
{
    QIM_DC(d);
    return d->lineWidth;
}

void QImPlot3DMeshItemNode::setLineWidth(float width)
{
    QIM_D(d);
    if (!qFuzzyCompare(d->lineWidth, width)) {
        d->lineWidth = width;
        Q_EMIT lineWidthChanged(width);
    }
}

int QImPlot3DMeshItemNode::meshFlags() const
{
    QIM_DC(d);
    return d->flags;
}

void QImPlot3DMeshItemNode::setMeshFlags(int flags)
{
    QIM_D(d);
    if (d->flags != flags) {
        d->flags = flags;
        Q_EMIT meshFlagChanged();
    }
}

bool QImPlot3DMeshItemNode::beginDraw()
{
    QIM_D(d);
    if (d->vertices.empty() || d->indices.size() < 3 || (d->indices.size() % 3) != 0) {
        return false;
    }

    if (d->fillColor.has_value()) {
        ImPlot3D::SetNextFillStyle(d->fillColor->value());
    }
    if (d->lineColor.has_value()) {
        ImPlot3D::SetNextLineStyle(d->lineColor->value(), d->lineWidth);
    } else {
        ImPlot3D::SetNextLineStyle(IMPLOT3D_AUTO_COL, d->lineWidth);
    }
    if (d->markerShape != ImPlot3DMarker_None) {
        const ImVec4 fill = d->markerFillColor.has_value() ? d->markerFillColor->value() : IMPLOT3D_AUTO_COL;
        const ImVec4 outline = d->markerOutlineColor.has_value() ? d->markerOutlineColor->value() : IMPLOT3D_AUTO_COL;
        ImPlot3D::SetNextMarkerStyle(static_cast<ImPlot3DMarker>(d->markerShape), d->markerSize, fill, d->markerWeight, outline);
    }

    ImPlot3D::PlotMesh(
        labelConstData(),
        d->vertices.data(),
        d->indices.data(),
        static_cast<int>(d->vertices.size()),
        static_cast<int>(d->indices.size()),
        static_cast<ImPlot3DMeshFlags>(d->flags)
    );

    // Capture defaults for unset colors
    if (!d->fillColor.has_value()) {
        d->fillColor = captureItemColor();
    }
    if (!d->lineColor.has_value()) {
        d->lineColor = captureItemColor();
    }
    if (!d->markerFillColor.has_value()) {
        d->markerFillColor = captureItemColor();
    }
    if (!d->markerOutlineColor.has_value()) {
        d->markerOutlineColor = captureItemColor();
    }

    return false;
}

}  // namespace QIM
