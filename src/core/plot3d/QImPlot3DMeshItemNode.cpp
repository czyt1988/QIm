#include "QImPlot3DMeshItemNode.h"
#include "QImAPI.h"
#include "QtImGuiUtils.h"
#include "implot3d.h"

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
    int meshFlags{0};
    int markerShape{ImPlot3DMarker_None};
    float markerSize{4.0f};
    float markerWeight{1.0f};
    QColor fillColor;
    QColor lineColor;
    QColor markerFillColor;
    QColor markerOutlineColor;
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
    return d_ptr->vertices;
}

const std::vector<unsigned int>& QImPlot3DMeshItemNode::indices() const
{
    return d_ptr->indices;
}

bool QImPlot3DMeshItemNode::isLinesVisible() const
{
    return (d_ptr->meshFlags & ImPlot3DMeshFlags_NoLines) == 0;
}

void QImPlot3DMeshItemNode::setLinesVisible(bool visible)
{
    QIM_D(d);
    const int oldFlags = d->meshFlags;
    if (visible) {
        d->meshFlags &= ~ImPlot3DMeshFlags_NoLines;
    } else {
        d->meshFlags |= ImPlot3DMeshFlags_NoLines;
    }
    if (d->meshFlags != oldFlags) {
        Q_EMIT meshFlagChanged();
    }
}

bool QImPlot3DMeshItemNode::isFillVisible() const
{
    return (d_ptr->meshFlags & ImPlot3DMeshFlags_NoFill) == 0;
}

void QImPlot3DMeshItemNode::setFillVisible(bool visible)
{
    QIM_D(d);
    const int oldFlags = d->meshFlags;
    if (visible) {
        d->meshFlags &= ~ImPlot3DMeshFlags_NoFill;
    } else {
        d->meshFlags |= ImPlot3DMeshFlags_NoFill;
    }
    if (d->meshFlags != oldFlags) {
        Q_EMIT meshFlagChanged();
    }
}

bool QImPlot3DMeshItemNode::isMarkersVisible() const
{
    return (d_ptr->meshFlags & ImPlot3DMeshFlags_NoMarkers) == 0;
}

void QImPlot3DMeshItemNode::setMarkersVisible(bool visible)
{
    QIM_D(d);
    const int oldFlags = d->meshFlags;
    if (visible) {
        d->meshFlags &= ~ImPlot3DMeshFlags_NoMarkers;
    } else {
        d->meshFlags |= ImPlot3DMeshFlags_NoMarkers;
    }
    if (d->meshFlags != oldFlags) {
        Q_EMIT meshFlagChanged();
    }
}

int QImPlot3DMeshItemNode::markerShape() const
{
    return d_ptr->markerShape;
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
    return d_ptr->markerSize;
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
    return d_ptr->markerWeight;
}

void QImPlot3DMeshItemNode::setMarkerWeight(float weight)
{
    QIM_D(d);
    if (!qFuzzyCompare(d->markerWeight, weight)) {
        d->markerWeight = weight;
        Q_EMIT markerStyleChanged();
    }
}

QColor QImPlot3DMeshItemNode::fillColor() const
{
    return d_ptr->fillColor;
}

void QImPlot3DMeshItemNode::setFillColor(const QColor& color)
{
    QIM_D(d);
    if (d->fillColor != color) {
        d->fillColor = color;
        Q_EMIT fillColorChanged(color);
    }
}

QColor QImPlot3DMeshItemNode::lineColor() const
{
    return d_ptr->lineColor;
}

void QImPlot3DMeshItemNode::setLineColor(const QColor& color)
{
    QIM_D(d);
    if (d->lineColor != color) {
        d->lineColor = color;
        Q_EMIT lineColorChanged(color);
    }
}

QColor QImPlot3DMeshItemNode::markerFillColor() const
{
    return d_ptr->markerFillColor;
}

void QImPlot3DMeshItemNode::setMarkerFillColor(const QColor& color)
{
    QIM_D(d);
    if (d->markerFillColor != color) {
        d->markerFillColor = color;
        Q_EMIT markerFillColorChanged(color);
    }
}

QColor QImPlot3DMeshItemNode::markerOutlineColor() const
{
    return d_ptr->markerOutlineColor;
}

void QImPlot3DMeshItemNode::setMarkerOutlineColor(const QColor& color)
{
    QIM_D(d);
    if (d->markerOutlineColor != color) {
        d->markerOutlineColor = color;
        Q_EMIT markerOutlineColorChanged(color);
    }
}

float QImPlot3DMeshItemNode::lineWidth() const
{
    return d_ptr->lineWidth;
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
    return d_ptr->meshFlags;
}

void QImPlot3DMeshItemNode::setMeshFlags(int flags)
{
    QIM_D(d);
    if (d->meshFlags != flags) {
        d->meshFlags = flags;
        Q_EMIT meshFlagChanged();
    }
}

bool QImPlot3DMeshItemNode::beginDraw()
{
    if (d_ptr->vertices.empty() || d_ptr->indices.size() < 3 || (d_ptr->indices.size() % 3) != 0) {
        return false;
    }

    if (d_ptr->fillColor.isValid()) {
        ImPlot3D::SetNextFillStyle(toImVec4(d_ptr->fillColor));
    }
    if (d_ptr->lineColor.isValid()) {
        ImPlot3D::SetNextLineStyle(toImVec4(d_ptr->lineColor), d_ptr->lineWidth);
    } else {
        ImPlot3D::SetNextLineStyle(IMPLOT3D_AUTO_COL, d_ptr->lineWidth);
    }
    if (d_ptr->markerShape != ImPlot3DMarker_None) {
        const ImVec4 fill = d_ptr->markerFillColor.isValid() ? toImVec4(d_ptr->markerFillColor) : IMPLOT3D_AUTO_COL;
        const ImVec4 outline = d_ptr->markerOutlineColor.isValid() ? toImVec4(d_ptr->markerOutlineColor) : IMPLOT3D_AUTO_COL;
        ImPlot3D::SetNextMarkerStyle(static_cast<ImPlot3DMarker>(d_ptr->markerShape), d_ptr->markerSize, fill, d_ptr->markerWeight, outline);
    }

    ImPlot3D::PlotMesh(
        labelConstData(),
        d_ptr->vertices.data(),
        d_ptr->indices.data(),
        static_cast<int>(d_ptr->vertices.size()),
        static_cast<int>(d_ptr->indices.size()),
        static_cast<ImPlot3DMeshFlags>(d_ptr->meshFlags)
    );
    return false;
}

}  // namespace QIM
