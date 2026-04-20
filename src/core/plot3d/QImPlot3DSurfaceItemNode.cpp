#include "QImPlot3DSurfaceItemNode.h"
#include "QImAPI.h"
#include "QtImGuiUtils.h"
#include "implot3d.h"
#include "implot3d_internal.h"
#include <QByteArray>
#include <algorithm>

namespace QIM
{

// ===============================================================
// PrivateData Implementation
// ===============================================================

class QImPlot3DSurfaceItemNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlot3DSurfaceItemNode)

public:
    explicit PrivateData(QImPlot3DSurfaceItemNode* q) : q_ptr(q) {}

    int flags{0};
    int markerShape{ImPlot3DMarker_None};
    float markerSize{4.0f};
    float markerWeight{1.0f};
    QImOptional3DColor fillColor;
    QImOptional3DColor lineColor;
    QImOptional3DColor markerFillColor;
    QImOptional3DColor markerOutlineColor;
    float lineWidth{1.0f};
    bool colormapEnabled{false};
    int colormap{ImPlot3DColormap_Viridis};
    QImAbstractXYZDataSeries* dataSeries = nullptr;
    int xCount = 0;
    int yCount = 0;
    QVector<QByteArray> edgeLabelsRow;
    QVector<QByteArray> edgeLabelsCol;
};

// ===============================================================
// QImPlot3DSurfaceItemNode Implementation
// ===============================================================

QImPlot3DSurfaceItemNode::QImPlot3DSurfaceItemNode(QObject* parent)
    : QImPlot3DItemNode(parent)
    , QIM_PIMPL_CONSTRUCT
{
}

QImPlot3DSurfaceItemNode::~QImPlot3DSurfaceItemNode()
{
    QIM_D(d);
    delete d->dataSeries;
}

void QImPlot3DSurfaceItemNode::setData(QImAbstractXYZDataSeries* series, int xCount, int yCount)
{
    QIM_D(d);
    delete d->dataSeries;
    d->dataSeries = series;
    d->xCount = xCount;
    d->yCount = yCount;
    regenerateEdgeLabels();
    Q_EMIT gridShapeChanged();
}

QImAbstractXYZDataSeries* QImPlot3DSurfaceItemNode::dataSeries() const
{
    QIM_DC(d);
    return d->dataSeries;
}

int QImPlot3DSurfaceItemNode::xCount() const
{
    QIM_DC(d);
    return d->xCount;
}

void QImPlot3DSurfaceItemNode::setXCount(int count)
{
    QIM_D(d);
    if (count > 0 && d->xCount != count) {
        d->xCount = count;
        regenerateEdgeLabels();
        Q_EMIT gridShapeChanged();
    }
}

int QImPlot3DSurfaceItemNode::yCount() const
{
    QIM_DC(d);
    return d->yCount;
}

void QImPlot3DSurfaceItemNode::setYCount(int count)
{
    QIM_D(d);
    if (count > 0 && d->yCount != count) {
        d->yCount = count;
        regenerateEdgeLabels();
        Q_EMIT gridShapeChanged();
    }
}

void QImPlot3DSurfaceItemNode::regenerateEdgeLabels()
{
    QIM_D(d);
    // Row edges: horizontal connections
    d->edgeLabelsRow.clear();
    int xMinus1 = qMax(0, d->xCount - 1);
    d->edgeLabelsRow.reserve(d->yCount * xMinus1);
    for (int yi = 0; yi < d->yCount; ++yi) {
        for (int xi = 0; xi + 1 < d->xCount; ++xi) {
            QByteArray label = QByteArray("##surface_")
                + QByteArray::number(reinterpret_cast<quintptr>(d->q_func()), 16)
                + "_row_" + QByteArray::number(yi) + "_" + QByteArray::number(xi);
            d->edgeLabelsRow.append(label);
        }
    }
    // Column edges: vertical connections
    d->edgeLabelsCol.clear();
    int yMinus1 = qMax(0, d->yCount - 1);
    d->edgeLabelsCol.reserve(yMinus1 * d->xCount);
    for (int yi = 0; yi + 1 < d->yCount; ++yi) {
        for (int xi = 0; xi < d->xCount; ++xi) {
            QByteArray label = QByteArray("##surface_")
                + QByteArray::number(reinterpret_cast<quintptr>(d->q_func()), 16)
                + "_col_" + QByteArray::number(yi) + "_" + QByteArray::number(xi);
            d->edgeLabelsCol.append(label);
        }
    }
}

QIMPLOT3D_FLAG_ENABLED_ACCESSOR(QImPlot3DSurfaceItemNode, LinesVisible, ImPlot3DSurfaceFlags_NoLines, surfaceFlagChanged)
QIMPLOT3D_FLAG_ENABLED_ACCESSOR(QImPlot3DSurfaceItemNode, FillVisible, ImPlot3DSurfaceFlags_NoFill, surfaceFlagChanged)
QIMPLOT3D_FLAG_ENABLED_ACCESSOR(QImPlot3DSurfaceItemNode, MarkersVisible, ImPlot3DSurfaceFlags_NoMarkers, surfaceFlagChanged)

int QImPlot3DSurfaceItemNode::markerShape() const
{
    QIM_DC(d);
    return d->markerShape;
}

void QImPlot3DSurfaceItemNode::setMarkerShape(int shape)
{
    QIM_D(d);
    if (d->markerShape != shape) {
        d->markerShape = shape;
        Q_EMIT markerShapeChanged(shape);
    }
}

float QImPlot3DSurfaceItemNode::markerSize() const
{
    QIM_DC(d);
    return d->markerSize;
}

void QImPlot3DSurfaceItemNode::setMarkerSize(float size)
{
    QIM_D(d);
    if (!qFuzzyCompare(d->markerSize, size)) {
        d->markerSize = size;
        Q_EMIT markerStyleChanged();
    }
}

float QImPlot3DSurfaceItemNode::markerWeight() const
{
    QIM_DC(d);
    return d->markerWeight;
}

void QImPlot3DSurfaceItemNode::setMarkerWeight(float weight)
{
    QIM_D(d);
    if (!qFuzzyCompare(d->markerWeight, weight)) {
        d->markerWeight = weight;
        Q_EMIT markerStyleChanged();
    }
}

QColor QImPlot3DSurfaceItemNode::fillColor() const
{
    QIM_DC(d);
    return (d->fillColor.has_value()) ? toQColor(d->fillColor->value()) : QColor();
}

void QImPlot3DSurfaceItemNode::setFillColor(const QColor& color)
{
    QIM_D(d);
    d->fillColor = toImVec4(color);
    Q_EMIT fillColorChanged(color);
}

QColor QImPlot3DSurfaceItemNode::lineColor() const
{
    QIM_DC(d);
    return (d->lineColor.has_value()) ? toQColor(d->lineColor->value()) : QColor();
}

void QImPlot3DSurfaceItemNode::setLineColor(const QColor& color)
{
    QIM_D(d);
    d->lineColor = toImVec4(color);
    Q_EMIT lineColorChanged(color);
}

QColor QImPlot3DSurfaceItemNode::markerFillColor() const
{
    QIM_DC(d);
    return (d->markerFillColor.has_value()) ? toQColor(d->markerFillColor->value()) : QColor();
}

void QImPlot3DSurfaceItemNode::setMarkerFillColor(const QColor& color)
{
    QIM_D(d);
    d->markerFillColor = toImVec4(color);
    Q_EMIT markerFillColorChanged(color);
}

QColor QImPlot3DSurfaceItemNode::markerOutlineColor() const
{
    QIM_DC(d);
    return (d->markerOutlineColor.has_value()) ? toQColor(d->markerOutlineColor->value()) : QColor();
}

void QImPlot3DSurfaceItemNode::setMarkerOutlineColor(const QColor& color)
{
    QIM_D(d);
    d->markerOutlineColor = toImVec4(color);
    Q_EMIT markerOutlineColorChanged(color);
}

float QImPlot3DSurfaceItemNode::lineWidth() const
{
    QIM_DC(d);
    return d->lineWidth;
}

void QImPlot3DSurfaceItemNode::setLineWidth(float width)
{
    QIM_D(d);
    if (!qFuzzyCompare(d->lineWidth, width)) {
        d->lineWidth = width;
        Q_EMIT lineWidthChanged(width);
    }
}

bool QImPlot3DSurfaceItemNode::isColormapEnabled() const
{
    QIM_DC(d);
    return d->colormapEnabled;
}

void QImPlot3DSurfaceItemNode::setColormapEnabled(bool enabled)
{
    QIM_D(d);
    if (d->colormapEnabled != enabled) {
        d->colormapEnabled = enabled;
        Q_EMIT colormapChanged();
    }
}

int QImPlot3DSurfaceItemNode::colormap() const
{
    QIM_DC(d);
    return d->colormap;
}

void QImPlot3DSurfaceItemNode::setColormap(int colormap)
{
    QIM_D(d);
    if (d->colormap != colormap) {
        d->colormap = colormap;
        Q_EMIT colormapChanged();
    }
}

int QImPlot3DSurfaceItemNode::surfaceFlags() const
{
    QIM_DC(d);
    return d->flags;
}

void QImPlot3DSurfaceItemNode::setSurfaceFlags(int flags)
{
    QIM_D(d);
    if (d->flags != flags) {
        d->flags = flags;
        Q_EMIT surfaceFlagChanged();
    }
}

bool QImPlot3DSurfaceItemNode::beginDraw()
{
    QIM_D(d);
    if (!d->dataSeries || !d->dataSeries->isValid()) {
        return false;
    }
    const int expectedCount = d->xCount * d->yCount;
    if (d->xCount < 2 || d->yCount < 2 || expectedCount <= 0) {
        return false;
    }
    if (d->dataSeries->size() < expectedCount) {
        return false;
    }

    const bool useGradientWireframe =
        d->colormapEnabled &&
        !isFillVisible() &&
        isLinesVisible() &&
        !d->lineColor.has_value();

    if (useGradientWireframe) {
        const double* zData = d->dataSeries->zRawData();
        const auto [zMinIt, zMaxIt] = std::minmax_element(zData, zData + expectedCount);
        const double zMin = *zMinIt;
        const double zMax = *zMaxIt;
        const double zRange = std::max(1e-12, zMax - zMin);

        auto sampleColor = [&](double value) {
            const float t = static_cast<float>((value - zMin) / zRange);
            return ImPlot3D::SampleColormap(t, static_cast<ImPlot3DColormap>(d->colormap));
        };

        const double* xData = d->dataSeries->xRawData();
        const double* yData = d->dataSeries->yRawData();
        const int xCount = d->xCount;
        const int yCount = d->yCount;

        // Draw each grid edge independently so wireframe colors track local height
        // distribution instead of using one averaged color per row/column.
        // Use pre-generated edge label cache for performance (no per-frame QString allocation).
        int labelIdx = 0;
        for (int yi = 0; yi < yCount; ++yi) {
            for (int xi = 0; xi + 1 < xCount; ++xi) {
                const int idx0 = yi * xCount + xi;
                const int idx1 = idx0 + 1;
                const double edgeZ = 0.5 * (zData[idx0] + zData[idx1]);
                const double xs[2] = {xData[idx0], xData[idx1]};
                const double ys[2] = {yData[idx0], yData[idx1]};
                const double zs[2] = {zData[idx0], zData[idx1]};
                const QByteArray& edgeLabel = d->edgeLabelsRow[labelIdx++];
                ImPlot3D::SetNextLineStyle(sampleColor(edgeZ), d->lineWidth);
                ImPlot3D::PlotLine(edgeLabel.constData(), xs, ys, zs, 2);
            }
        }

        labelIdx = 0;
        for (int yi = 0; yi + 1 < yCount; ++yi) {
            for (int xi = 0; xi < xCount; ++xi) {
                const int idx0 = yi * xCount + xi;
                const int idx1 = idx0 + xCount;
                const double edgeZ = 0.5 * (zData[idx0] + zData[idx1]);
                const double xs[2] = {xData[idx0], xData[idx1]};
                const double ys[2] = {yData[idx0], yData[idx1]};
                const double zs[2] = {zData[idx0], zData[idx1]};
                const QByteArray& edgeLabel = d->edgeLabelsCol[labelIdx++];
                ImPlot3D::SetNextLineStyle(sampleColor(edgeZ), d->lineWidth);
                ImPlot3D::PlotLine(edgeLabel.constData(), xs, ys, zs, 2);
            }
        }
        return false;
    }

    const bool useColormap = d->colormapEnabled && !d->fillColor.has_value();
    if (useColormap) {
        ImPlot3D::PushColormap(static_cast<ImPlot3DColormap>(d->colormap));
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

    ImPlot3D::PlotSurface(
        labelConstData(),
        d->dataSeries->xRawData(),
        d->dataSeries->yRawData(),
        d->dataSeries->zRawData(),
        d->xCount,
        d->yCount,
        0.0,
        0.0,
        static_cast<ImPlot3DSurfaceFlags>(d->flags)
    );
    if (useColormap) {
        ImPlot3D::PopColormap();
    }

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
