#include "QImPlot3DSurfaceItemNode.h"
#include "QImAPI.h"
#include "QtImGuiUtils.h"
#include "implot3d.h"
#include <QString>
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

    int surfaceFlags{0};
    int markerShape{ImPlot3DMarker_None};
    float markerSize{4.0f};
    float markerWeight{1.0f};
    QColor fillColor;
    QColor lineColor;
    QColor markerFillColor;
    QColor markerOutlineColor;
    float lineWidth{1.0f};
    bool colormapEnabled{false};
    int colormap{ImPlot3DColormap_Viridis};
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
}

const std::vector<double>& QImPlot3DSurfaceItemNode::xData() const
{
    return xData_vec;
}

const std::vector<double>& QImPlot3DSurfaceItemNode::yData() const
{
    return yData_vec;
}

const std::vector<double>& QImPlot3DSurfaceItemNode::zData() const
{
    return zData_vec;
}

int QImPlot3DSurfaceItemNode::xCount() const
{
    return xCount_val;
}

void QImPlot3DSurfaceItemNode::setXCount(int count)
{
    if (count > 0 && xCount_val != count) {
        xCount_val = count;
        trimDataToGrid();
        Q_EMIT gridShapeChanged();
    }
}

int QImPlot3DSurfaceItemNode::yCount() const
{
    return yCount_val;
}

void QImPlot3DSurfaceItemNode::setYCount(int count)
{
    if (count > 0 && yCount_val != count) {
        yCount_val = count;
        trimDataToGrid();
        Q_EMIT gridShapeChanged();
    }
}

bool QImPlot3DSurfaceItemNode::isLinesVisible() const
{
    return (d_ptr->surfaceFlags & ImPlot3DSurfaceFlags_NoLines) == 0;
}

void QImPlot3DSurfaceItemNode::setLinesVisible(bool visible)
{
    QIM_D(d);
    const int oldFlags = d->surfaceFlags;
    if (visible) {
        d->surfaceFlags &= ~ImPlot3DSurfaceFlags_NoLines;
    } else {
        d->surfaceFlags |= ImPlot3DSurfaceFlags_NoLines;
    }
    if (d->surfaceFlags != oldFlags) {
        Q_EMIT surfaceFlagChanged();
    }
}

bool QImPlot3DSurfaceItemNode::isFillVisible() const
{
    return (d_ptr->surfaceFlags & ImPlot3DSurfaceFlags_NoFill) == 0;
}

void QImPlot3DSurfaceItemNode::setFillVisible(bool visible)
{
    QIM_D(d);
    const int oldFlags = d->surfaceFlags;
    if (visible) {
        d->surfaceFlags &= ~ImPlot3DSurfaceFlags_NoFill;
    } else {
        d->surfaceFlags |= ImPlot3DSurfaceFlags_NoFill;
    }
    if (d->surfaceFlags != oldFlags) {
        Q_EMIT surfaceFlagChanged();
    }
}

bool QImPlot3DSurfaceItemNode::isMarkersVisible() const
{
    return (d_ptr->surfaceFlags & ImPlot3DSurfaceFlags_NoMarkers) == 0;
}

void QImPlot3DSurfaceItemNode::setMarkersVisible(bool visible)
{
    QIM_D(d);
    const int oldFlags = d->surfaceFlags;
    if (visible) {
        d->surfaceFlags &= ~ImPlot3DSurfaceFlags_NoMarkers;
    } else {
        d->surfaceFlags |= ImPlot3DSurfaceFlags_NoMarkers;
    }
    if (d->surfaceFlags != oldFlags) {
        Q_EMIT surfaceFlagChanged();
    }
}

int QImPlot3DSurfaceItemNode::markerShape() const
{
    return d_ptr->markerShape;
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
    return d_ptr->markerSize;
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
    return d_ptr->markerWeight;
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
    return d_ptr->fillColor;
}

void QImPlot3DSurfaceItemNode::setFillColor(const QColor& color)
{
    QIM_D(d);
    if (d->fillColor != color) {
        d->fillColor = color;
        Q_EMIT fillColorChanged(color);
    }
}

QColor QImPlot3DSurfaceItemNode::lineColor() const
{
    return d_ptr->lineColor;
}

void QImPlot3DSurfaceItemNode::setLineColor(const QColor& color)
{
    QIM_D(d);
    if (d->lineColor != color) {
        d->lineColor = color;
        Q_EMIT lineColorChanged(color);
    }
}

QColor QImPlot3DSurfaceItemNode::markerFillColor() const
{
    return d_ptr->markerFillColor;
}

void QImPlot3DSurfaceItemNode::setMarkerFillColor(const QColor& color)
{
    QIM_D(d);
    if (d->markerFillColor != color) {
        d->markerFillColor = color;
        Q_EMIT markerFillColorChanged(color);
    }
}

QColor QImPlot3DSurfaceItemNode::markerOutlineColor() const
{
    return d_ptr->markerOutlineColor;
}

void QImPlot3DSurfaceItemNode::setMarkerOutlineColor(const QColor& color)
{
    QIM_D(d);
    if (d->markerOutlineColor != color) {
        d->markerOutlineColor = color;
        Q_EMIT markerOutlineColorChanged(color);
    }
}

float QImPlot3DSurfaceItemNode::lineWidth() const
{
    return d_ptr->lineWidth;
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
    return d_ptr->colormapEnabled;
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
    return d_ptr->colormap;
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
    return d_ptr->surfaceFlags;
}

void QImPlot3DSurfaceItemNode::setSurfaceFlags(int flags)
{
    QIM_D(d);
    if (d->surfaceFlags != flags) {
        d->surfaceFlags = flags;
        Q_EMIT surfaceFlagChanged();
    }
}

bool QImPlot3DSurfaceItemNode::beginDraw()
{
    const int expectedCount = xCount_val * yCount_val;
    if (xCount_val < 2 || yCount_val < 2 || expectedCount <= 0) {
        return false;
    }
    if (static_cast<int>(std::min({xData_vec.size(), yData_vec.size(), zData_vec.size()})) < expectedCount) {
        return false;
    }

    const bool useGradientWireframe =
        d_ptr->colormapEnabled &&
        !isFillVisible() &&
        isLinesVisible() &&
        !d_ptr->lineColor.isValid();

    if (useGradientWireframe) {
        const auto [zMinIt, zMaxIt] = std::minmax_element(zData_vec.begin(), zData_vec.begin() + expectedCount);
        const double zMin = *zMinIt;
        const double zMax = *zMaxIt;
        const double zRange = std::max(1e-12, zMax - zMin);

        auto sampleColor = [&](double value) {
            const float t = static_cast<float>((value - zMin) / zRange);
            return ImPlot3D::SampleColormap(t, static_cast<ImPlot3DColormap>(d_ptr->colormap));
        };

        // Draw each grid edge independently so wireframe colors track local height
        // distribution instead of using one averaged color per row/column.
        for (int yi = 0; yi < yCount_val; ++yi) {
            for (int xi = 0; xi + 1 < xCount_val; ++xi) {
                const int idx0 = yi * xCount_val + xi;
                const int idx1 = idx0 + 1;
                const double edgeZ = 0.5 * (zData_vec[idx0] + zData_vec[idx1]);
                const double xs[2] = {xData_vec[idx0], xData_vec[idx1]};
                const double ys[2] = {yData_vec[idx0], yData_vec[idx1]};
                const double zs[2] = {zData_vec[idx0], zData_vec[idx1]};
                const QByteArray edgeLabel =
                    QString("##surface_%1_row_%2_%3").arg(reinterpret_cast<quintptr>(this), 0, 16).arg(yi).arg(xi).toUtf8();
                ImPlot3D::SetNextLineStyle(sampleColor(edgeZ), d_ptr->lineWidth);
                ImPlot3D::PlotLine(edgeLabel.constData(), xs, ys, zs, 2);
            }
        }

        for (int yi = 0; yi + 1 < yCount_val; ++yi) {
            for (int xi = 0; xi < xCount_val; ++xi) {
                const int idx0 = yi * xCount_val + xi;
                const int idx1 = idx0 + xCount_val;
                const double edgeZ = 0.5 * (zData_vec[idx0] + zData_vec[idx1]);
                const double xs[2] = {xData_vec[idx0], xData_vec[idx1]};
                const double ys[2] = {yData_vec[idx0], yData_vec[idx1]};
                const double zs[2] = {zData_vec[idx0], zData_vec[idx1]};
                const QByteArray edgeLabel =
                    QString("##surface_%1_col_%2_%3").arg(reinterpret_cast<quintptr>(this), 0, 16).arg(yi).arg(xi).toUtf8();
                ImPlot3D::SetNextLineStyle(sampleColor(edgeZ), d_ptr->lineWidth);
                ImPlot3D::PlotLine(edgeLabel.constData(), xs, ys, zs, 2);
            }
        }
        return false;
    }

    const bool useColormap = d_ptr->colormapEnabled && !d_ptr->fillColor.isValid();
    if (useColormap) {
        ImPlot3D::PushColormap(static_cast<ImPlot3DColormap>(d_ptr->colormap));
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

    ImPlot3D::PlotSurface(
        labelConstData(),
        xData_vec.data(),
        yData_vec.data(),
        zData_vec.data(),
        xCount_val,
        yCount_val,
        0.0,
        0.0,
        static_cast<ImPlot3DSurfaceFlags>(d_ptr->surfaceFlags)
    );
    if (useColormap) {
        ImPlot3D::PopColormap();
    }
    return false;
}

void QImPlot3DSurfaceItemNode::trimDataToGrid()
{
    const std::size_t commonSize = std::min({xData_vec.size(), yData_vec.size(), zData_vec.size()});
    const std::size_t gridSize = (xCount_val > 0 && yCount_val > 0) ? static_cast<std::size_t>(xCount_val) * static_cast<std::size_t>(yCount_val)
                                                                 : commonSize;
    const std::size_t finalSize = std::min(commonSize, gridSize);
    xData_vec.resize(finalSize);
    yData_vec.resize(finalSize);
    zData_vec.resize(finalSize);
}

}  // namespace QIM
