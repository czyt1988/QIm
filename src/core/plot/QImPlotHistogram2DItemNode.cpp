#include "QImPlotHistogram2DItemNode.h"
#include <memory>
#include <optional>
#include "implot.h"
#include "implot_internal.h"
#include "QImTrackedValue.hpp"
#include "QtImGuiUtils.h"
#include <QDebug>
#include "QImPlotHistogram2DDataSeries.h"

namespace QIM
{

class QImPlotHistogram2DItemNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlotHistogram2DItemNode)
public:
    PrivateData(QImPlotHistogram2DItemNode* p);

    ImPlotHistogramFlags flags { ImPlotHistogramFlags_None };
    int xBins { -2 };  // ImPlotBin_Sturges
    int yBins { -2 };
    double xRangeMin { 0.0 };  // 0 = auto
    double xRangeMax { 0.0 };
    double yRangeMin { 0.0 };
    double yRangeMax { 0.0 };
};

QImPlotHistogram2DItemNode::PrivateData::PrivateData(QImPlotHistogram2DItemNode* p) : q_ptr(p)
{
}

QImPlotHistogram2DItemNode::QImPlotHistogram2DItemNode(QObject* parent) : QImAbstractXYSeriesItemNode(parent), QIM_PIMPL_CONSTRUCT
{
}

QImPlotHistogram2DItemNode::~QImPlotHistogram2DItemNode()
{
}

int QImPlotHistogram2DItemNode::xBins() const
{
    QIM_DC(d);
    return d->xBins;
}

void QImPlotHistogram2DItemNode::setXBins(int bins)
{
    QIM_D(d);
    if (d->xBins != bins) {
        d->xBins = bins;
        Q_EMIT xBinsChanged(bins);
    }
}

int QImPlotHistogram2DItemNode::yBins() const
{
    QIM_DC(d);
    return d->yBins;
}

void QImPlotHistogram2DItemNode::setYBins(int bins)
{
    QIM_D(d);
    if (d->yBins != bins) {
        d->yBins = bins;
        Q_EMIT yBinsChanged(bins);
    }
}

double QImPlotHistogram2DItemNode::xRangeMin() const
{
    QIM_DC(d);
    return d->xRangeMin;
}

void QImPlotHistogram2DItemNode::setXRangeMin(double min)
{
    QIM_D(d);
    if (d->xRangeMin != min) {
        d->xRangeMin = min;
        Q_EMIT xRangeChanged();
    }
}

double QImPlotHistogram2DItemNode::xRangeMax() const
{
    QIM_DC(d);
    return d->xRangeMax;
}

void QImPlotHistogram2DItemNode::setXRangeMax(double max)
{
    QIM_D(d);
    if (d->xRangeMax != max) {
        d->xRangeMax = max;
        Q_EMIT xRangeChanged();
    }
}

double QImPlotHistogram2DItemNode::yRangeMin() const
{
    QIM_DC(d);
    return d->yRangeMin;
}

void QImPlotHistogram2DItemNode::setYRangeMin(double min)
{
    QIM_D(d);
    if (d->yRangeMin != min) {
        d->yRangeMin = min;
        Q_EMIT yRangeChanged();
    }
}

double QImPlotHistogram2DItemNode::yRangeMax() const
{
    QIM_DC(d);
    return d->yRangeMax;
}

void QImPlotHistogram2DItemNode::setYRangeMax(double max)
{
    QIM_D(d);
    if (d->yRangeMax != max) {
        d->yRangeMax = max;
        Q_EMIT yRangeChanged();
    }
}

bool QImPlotHistogram2DItemNode::isDensity() const
{
    QIM_DC(d);
    return (d->flags & ImPlotHistogramFlags_Density) != 0;
}

void QImPlotHistogram2DItemNode::setDensity(bool density)
{
    QIM_D(d);
    const ImPlotHistogramFlags oldFlags = d->flags;
    if (density) {
        d->flags |= ImPlotHistogramFlags_Density;
    } else {
        d->flags &= ~ImPlotHistogramFlags_Density;
    }
    if (d->flags != oldFlags) {
        Q_EMIT densityChanged(density);
        Q_EMIT histogramFlagChanged();
    }
}

bool QImPlotHistogram2DItemNode::isNoOutliers() const
{
    QIM_DC(d);
    return (d->flags & ImPlotHistogramFlags_NoOutliers) != 0;
}

void QImPlotHistogram2DItemNode::setNoOutliers(bool noOutliers)
{
    QIM_D(d);
    const ImPlotHistogramFlags oldFlags = d->flags;
    if (noOutliers) {
        d->flags |= ImPlotHistogramFlags_NoOutliers;
    } else {
        d->flags &= ~ImPlotHistogramFlags_NoOutliers;
    }
    if (d->flags != oldFlags) {
        Q_EMIT noOutliersChanged(noOutliers);
        Q_EMIT histogramFlagChanged();
    }
}

bool QImPlotHistogram2DItemNode::isColMajor() const
{
    QIM_DC(d);
    return (d->flags & ImPlotHistogramFlags_ColMajor) != 0;
}

void QImPlotHistogram2DItemNode::setColMajor(bool colMajor)
{
    QIM_D(d);
    const ImPlotHistogramFlags oldFlags = d->flags;
    if (colMajor) {
        d->flags |= ImPlotHistogramFlags_ColMajor;
    } else {
        d->flags &= ~ImPlotHistogramFlags_ColMajor;
    }
    if (d->flags != oldFlags) {
        Q_EMIT colMajorChanged(colMajor);
        Q_EMIT histogramFlagChanged();
    }
}

int QImPlotHistogram2DItemNode::histogramFlags() const
{
    QIM_DC(d);
    return d->flags;
}

void QImPlotHistogram2DItemNode::setHistogramFlags(int flags)
{
    QIM_D(d);
    if (d->flags != flags) {
        d->flags = static_cast< ImPlotHistogramFlags >(flags);
        Q_EMIT histogramFlagChanged();
    }
}

bool QImPlotHistogram2DItemNode::beginDraw()
{
    QIM_D(d);
    QImAbstractXYDataSeries* rawData = this->data();
    if (!rawData || rawData->size() == 0) {
        return false;
    }

    // 2D histogram uses colormap, not single fill style.

    // Prepare ImPlotRect
    ImPlotRect range;
    if (d->xRangeMin != 0.0 || d->xRangeMax != 0.0 || d->yRangeMin != 0.0 || d->yRangeMax != 0.0) {
        range.X.Min = d->xRangeMin;
        range.X.Max = d->xRangeMax;
        range.Y.Min = d->yRangeMin;
        range.Y.Max = d->yRangeMax;
    } else {
        range = ImPlotRect();
    }

    // Determine flags (exclude unsupported Horizontal and Cumulative)
    ImPlotHistogramFlags flags = d->flags;
    flags &= ~(ImPlotHistogramFlags_Horizontal | ImPlotHistogramFlags_Cumulative);

    // Call ImPlot API
    if (rawData->isContiguous()) {
        const double* xs = rawData->xRawData();
        const double* ys = rawData->yRawData();
        int count = rawData->size();
        if (!xs) {
            qWarning() << "[Histogram2DItemNode] X data missing, cannot plot 2D histogram";
            return false;
        }
        ImPlot::PlotHistogram2D(
            labelConstData(), xs, ys, count, d->xBins, d->yBins, range, flags);
    } else {
        int count = rawData->size();
        std::vector<double> xs(count), ys(count);
        for (int i = 0; i < count; ++i) {
            xs[i] = rawData->xValue(i);
            ys[i] = rawData->yValue(i);
        }
        ImPlot::PlotHistogram2D(
            labelConstData(), xs.data(), ys.data(), count, d->xBins, d->yBins, range, flags);
    }

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