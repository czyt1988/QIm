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
