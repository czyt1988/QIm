#include "QImPlot3DTriangleItemNode.h"
#include "QImAPI.h"
#include "QtImGuiUtils.h"
#include "implot3d.h"
#include <algorithm>

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

    int triangleFlags{0};
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
// QImPlot3DTriangleItemNode Implementation
// ===============================================================

QImPlot3DTriangleItemNode::QImPlot3DTriangleItemNode(QObject* parent)
    : QImPlot3DItemNode(parent)
    , QIM_PIMPL_CONSTRUCT
{
}

QImPlot3DTriangleItemNode::~QImPlot3DTriangleItemNode()
{
}

const std::vector<double>& QImPlot3DTriangleItemNode::xData() const
{
    return xData_vec;
}

const std::vector<double>& QImPlot3DTriangleItemNode::yData() const
{
    return yData_vec;
}

const std::vector<double>& QImPlot3DTriangleItemNode::zData() const
{
    return zData_vec;
}

bool QImPlot3DTriangleItemNode::isLinesVisible() const
{
    return (d_ptr->triangleFlags & ImPlot3DTriangleFlags_NoLines) == 0;
}

void QImPlot3DTriangleItemNode::setLinesVisible(bool visible)
{
    QIM_D(d);
    const int oldFlags = d->triangleFlags;
    if (visible) {
        d->triangleFlags &= ~ImPlot3DTriangleFlags_NoLines;
    } else {
        d->triangleFlags |= ImPlot3DTriangleFlags_NoLines;
    }
    if (d->triangleFlags != oldFlags) {
        Q_EMIT triangleFlagChanged();
    }
}

bool QImPlot3DTriangleItemNode::isFillVisible() const
{
    return (d_ptr->triangleFlags & ImPlot3DTriangleFlags_NoFill) == 0;
}

void QImPlot3DTriangleItemNode::setFillVisible(bool visible)
{
    QIM_D(d);
    const int oldFlags = d->triangleFlags;
    if (visible) {
        d->triangleFlags &= ~ImPlot3DTriangleFlags_NoFill;
    } else {
        d->triangleFlags |= ImPlot3DTriangleFlags_NoFill;
    }
    if (d->triangleFlags != oldFlags) {
        Q_EMIT triangleFlagChanged();
    }
}

bool QImPlot3DTriangleItemNode::isMarkersVisible() const
{
    return (d_ptr->triangleFlags & ImPlot3DTriangleFlags_NoMarkers) == 0;
}

void QImPlot3DTriangleItemNode::setMarkersVisible(bool visible)
{
    QIM_D(d);
    const int oldFlags = d->triangleFlags;
    if (visible) {
        d->triangleFlags &= ~ImPlot3DTriangleFlags_NoMarkers;
    } else {
        d->triangleFlags |= ImPlot3DTriangleFlags_NoMarkers;
    }
    if (d->triangleFlags != oldFlags) {
        Q_EMIT triangleFlagChanged();
    }
}

int QImPlot3DTriangleItemNode::markerShape() const
{
    return d_ptr->markerShape;
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
    return d_ptr->markerSize;
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
    return d_ptr->markerWeight;
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
    return d_ptr->fillColor;
}

void QImPlot3DTriangleItemNode::setFillColor(const QColor& color)
{
    QIM_D(d);
    if (d->fillColor != color) {
        d->fillColor = color;
        Q_EMIT fillColorChanged(color);
    }
}

QColor QImPlot3DTriangleItemNode::lineColor() const
{
    return d_ptr->lineColor;
}

void QImPlot3DTriangleItemNode::setLineColor(const QColor& color)
{
    QIM_D(d);
    if (d->lineColor != color) {
        d->lineColor = color;
        Q_EMIT lineColorChanged(color);
    }
}

QColor QImPlot3DTriangleItemNode::markerFillColor() const
{
    return d_ptr->markerFillColor;
}

void QImPlot3DTriangleItemNode::setMarkerFillColor(const QColor& color)
{
    QIM_D(d);
    if (d->markerFillColor != color) {
        d->markerFillColor = color;
        Q_EMIT markerFillColorChanged(color);
    }
}

QColor QImPlot3DTriangleItemNode::markerOutlineColor() const
{
    return d_ptr->markerOutlineColor;
}

void QImPlot3DTriangleItemNode::setMarkerOutlineColor(const QColor& color)
{
    QIM_D(d);
    if (d->markerOutlineColor != color) {
        d->markerOutlineColor = color;
        Q_EMIT markerOutlineColorChanged(color);
    }
}

float QImPlot3DTriangleItemNode::lineWidth() const
{
    return d_ptr->lineWidth;
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
    return d_ptr->triangleFlags;
}

void QImPlot3DTriangleItemNode::setTriangleFlags(int flags)
{
    QIM_D(d);
    if (d->triangleFlags != flags) {
        d->triangleFlags = flags;
        Q_EMIT triangleFlagChanged();
    }
}

bool QImPlot3DTriangleItemNode::beginDraw()
{
    const int count = static_cast<int>(std::min({xData_vec.size(), yData_vec.size(), zData_vec.size()}));
    if (count < 3) {
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

    ImPlot3D::PlotTriangle(
        labelConstData(),
        xData_vec.data(),
        yData_vec.data(),
        zData_vec.data(),
        count,
        static_cast<ImPlot3DTriangleFlags>(d_ptr->triangleFlags)
    );
    return false;
}

void QImPlot3DTriangleItemNode::trimDataToCommonSize()
{
    const std::size_t commonSize = std::min({xData_vec.size(), yData_vec.size(), zData_vec.size()});
    xData_vec.resize(commonSize);
    yData_vec.resize(commonSize);
    zData_vec.resize(commonSize);
}

}  // namespace QIM
