#include "QImPlot3DTriangleItemNode.h"
#include "QImAPI.h"
#include "QtImGuiUtils.h"
#include "implot3d.h"
#include "implot3d_internal.h"
#include "QImPlot3DDataSeries.h"

namespace QIM
{

// ===============================================================
// PrivateData Implementation
// ===============================================================

class QImPlot3DTriangleItemNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlot3DTriangleItemNode)

public:
    explicit PrivateData(QImPlot3DTriangleItemNode* q) : q_ptr(q) {}

    int flags{0};
    int markerShape{ImPlot3DMarker_None};
    float markerSize{4.0f};
    float markerWeight{1.0f};
    QImOptional3DColor fillColor;
    QImOptional3DColor lineColor;
    QImOptional3DColor markerFillColor;
    QImOptional3DColor markerOutlineColor;
    float lineWidth{1.0f};
    float fillAlpha{-1.0f};  // IMPLOT3D_AUTO
    QImAbstractXYZDataSeries* dataSeries = nullptr;
};

// ===============================================================
// QImPlot3DTriangleItemNode Implementation
// ===============================================================

QImPlot3DTriangleItemNode::QImPlot3DTriangleItemNode(QObject* parent)
    : QImPlot3DItemNode(parent)
    , QIM_PIMPL_CONSTRUCT
{
}

QImPlot3DTriangleItemNode::~QImPlot3DTriangleItemNode()
{
    QIM_D(d);
    delete d->dataSeries;
}

void QImPlot3DTriangleItemNode::setData(QImAbstractXYZDataSeries* series)
{
    QIM_D(d);
    delete d->dataSeries;
    d->dataSeries = series;
}

QImAbstractXYZDataSeries* QImPlot3DTriangleItemNode::dataSeries() const
{
    QIM_DC(d);
    return d->dataSeries;
}

QIMPLOT3D_FLAG_ENABLED_ACCESSOR(QImPlot3DTriangleItemNode, LinesVisible, ImPlot3DTriangleFlags_NoLines, triangleFlagChanged)
QIMPLOT3D_FLAG_ENABLED_ACCESSOR(QImPlot3DTriangleItemNode, FillVisible, ImPlot3DTriangleFlags_NoFill, triangleFlagChanged)
QIMPLOT3D_FLAG_ENABLED_ACCESSOR(QImPlot3DTriangleItemNode, MarkersVisible, ImPlot3DTriangleFlags_NoMarkers, triangleFlagChanged)

int QImPlot3DTriangleItemNode::markerShape() const
{
    QIM_DC(d);
    return d->markerShape;
}

void QImPlot3DTriangleItemNode::setMarkerShape(int shape)
{
    QIM_D(d);
    if (d->markerShape != shape) {
        d->markerShape = shape;
        Q_EMIT markerShapeChanged(shape);
    }
}

float QImPlot3DTriangleItemNode::markerSize() const
{
    QIM_DC(d);
    return d->markerSize;
}

void QImPlot3DTriangleItemNode::setMarkerSize(float size)
{
    QIM_D(d);
    if (!qFuzzyCompare(d->markerSize, size)) {
        d->markerSize = size;
        Q_EMIT markerStyleChanged();
    }
}

float QImPlot3DTriangleItemNode::markerWeight() const
{
    QIM_DC(d);
    return d->markerWeight;
}

void QImPlot3DTriangleItemNode::setMarkerWeight(float weight)
{
    QIM_D(d);
    if (!qFuzzyCompare(d->markerWeight, weight)) {
        d->markerWeight = weight;
        Q_EMIT markerStyleChanged();
    }
}

QColor QImPlot3DTriangleItemNode::fillColor() const
{
    QIM_DC(d);
    return (d->fillColor.has_value()) ? toQColor(d->fillColor->value()) : QColor();
}

void QImPlot3DTriangleItemNode::setFillColor(const QColor& color)
{
    QIM_D(d);
    d->fillColor = toImVec4(color);
    Q_EMIT fillColorChanged(color);
}

QColor QImPlot3DTriangleItemNode::lineColor() const
{
    QIM_DC(d);
    return (d->lineColor.has_value()) ? toQColor(d->lineColor->value()) : QColor();
}

void QImPlot3DTriangleItemNode::setLineColor(const QColor& color)
{
    QIM_D(d);
    d->lineColor = toImVec4(color);
    Q_EMIT lineColorChanged(color);
}

QColor QImPlot3DTriangleItemNode::markerFillColor() const
{
    QIM_DC(d);
    return (d->markerFillColor.has_value()) ? toQColor(d->markerFillColor->value()) : QColor();
}

void QImPlot3DTriangleItemNode::setMarkerFillColor(const QColor& color)
{
    QIM_D(d);
    d->markerFillColor = toImVec4(color);
    Q_EMIT markerFillColorChanged(color);
}

QColor QImPlot3DTriangleItemNode::markerOutlineColor() const
{
    QIM_DC(d);
    return (d->markerOutlineColor.has_value()) ? toQColor(d->markerOutlineColor->value()) : QColor();
}

void QImPlot3DTriangleItemNode::setMarkerOutlineColor(const QColor& color)
{
    QIM_D(d);
    d->markerOutlineColor = toImVec4(color);
    Q_EMIT markerOutlineColorChanged(color);
}

float QImPlot3DTriangleItemNode::lineWidth() const
{
    QIM_DC(d);
    return d->lineWidth;
}

void QImPlot3DTriangleItemNode::setLineWidth(float width)
{
    QIM_D(d);
    if (!qFuzzyCompare(d->lineWidth, width)) {
        d->lineWidth = width;
        Q_EMIT lineWidthChanged(width);
    }
}

/**
 * \if ENGLISH
 * @brief Returns the current fill alpha value
 * @return Fill alpha value (0.0 to 1.0, or -1.0 for auto)
 * @details Returns the fill transparency value. A value of -1.0 indicates
 *          IMPLOT3D_AUTO, meaning the default style value should be used.
 * \endif
 *
 * \if CHINESE
 * @brief 返回当前填充透明度值
 * @return 填充透明度值（0.0到1.0，或-1.0表示自动）
 * @details 返回填充透明度值。-1.0表示IMPLOT3D_AUTO，应使用默认样式值。
 * \endif
 */
float QImPlot3DTriangleItemNode::fillAlpha() const
{
    QIM_DC(d);
    return d->fillAlpha;
}

/**
 * \if ENGLISH
 * @brief Sets the fill alpha value
 * @param[in] alpha Fill alpha value (0.0 to 1.0, or -1.0 for auto)
 * @details Sets the fill transparency value. A value of -1.0 indicates
 *          IMPLOT3D_AUTO, meaning the default style value should be used.
 *          Values outside the valid range are clamped to [0.0, 1.0].
 * \endif
 *
 * \if CHINESE
 * @brief 设置填充透明度值
 * @param[in] alpha 填充透明度值（0.0到1.0，或-1.0表示自动）
 * @details 设置填充透明度值。-1.0表示IMPLOT3D_AUTO，应使用默认样式值。
 *          超出有效范围的值将被限制在[0.0, 1.0]范围内。
 * \endif
 */
void QImPlot3DTriangleItemNode::setFillAlpha(float alpha)
{
    QIM_D(d);
    // Clamp to valid range, but allow -1.0 for IMPLOT3D_AUTO
    float clampedAlpha = alpha;
    if (alpha >= 0.0f) {
        // Only clamp non-negative values (auto is -1.0)
        clampedAlpha = qBound(0.0f, alpha, 1.0f);
    }
    
    if (!qFuzzyCompare(d->fillAlpha, clampedAlpha)) {
        d->fillAlpha = clampedAlpha;
        Q_EMIT fillAlphaChanged(clampedAlpha);
    }
}

int QImPlot3DTriangleItemNode::triangleFlags() const
{
    QIM_DC(d);
    return d->flags;
}

void QImPlot3DTriangleItemNode::setTriangleFlags(int flags)
{
    QIM_D(d);
    if (d->flags != flags) {
        d->flags = flags;
        Q_EMIT triangleFlagChanged();
    }
}

bool QImPlot3DTriangleItemNode::beginDraw()
{
    QIM_D(d);
    if (!d->dataSeries || !d->dataSeries->isValid()) {
        return false;
    }
    const int count = d->dataSeries->size();
    if (count < 3) {
        return false;
    }

    if (d->fillColor.has_value()) {
        ImVec4 fillColor = d->fillColor->value();
        // Apply fill alpha if explicitly set (not auto)
        if (d->fillAlpha >= 0.0f) {
            fillColor.w = d->fillAlpha;
        }
        ImPlot3D::SetNextFillStyle(fillColor);
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

    ImPlot3D::PlotTriangle(
        labelConstData(),
        d->dataSeries->xRawData(),
        d->dataSeries->yRawData(),
        d->dataSeries->zRawData(),
        count,
        static_cast<ImPlot3DTriangleFlags>(d->flags)
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
