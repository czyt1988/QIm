#include "QImPlotHistogramItemNode.h"
#include <optional>
#include "implot.h"
#include "implot_internal.h"
#include "QImTrackedValue.hpp"
#include "QtImGuiUtils.h"

namespace QIM
{

class QImPlotHistogramItemNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlotHistogramItemNode)
public:
    PrivateData(QImPlotHistogramItemNode* p);

    ImPlotHistogramFlags flags { ImPlotHistogramFlags_None };
    int bins { -2 };  // ImPlotBin_Sturges
    double barScale { 1.0 };
    double rangeMin { 0.0 };  // 0 = auto
    double rangeMax { 0.0 };  // 0 = auto
    // Style tracking values
    std::optional< QImTrackedValue< ImVec4, QIM::ImVecComparator< ImVec4 > > > color;
};

QImPlotHistogramItemNode::PrivateData::PrivateData(QImPlotHistogramItemNode* p) : q_ptr(p)
{
}

QImPlotHistogramItemNode::QImPlotHistogramItemNode(QObject* parent) : QImAbstractXYSeriesItemNode(parent), QIM_PIMPL_CONSTRUCT
{
}

QImPlotHistogramItemNode::~QImPlotHistogramItemNode()
{
}

int QImPlotHistogramItemNode::bins() const
{
    QIM_DC(d);
    return d->bins;
}

void QImPlotHistogramItemNode::setBins(int bins)
{
    QIM_D(d);
    if (d->bins != bins) {
        d->bins = bins;
        Q_EMIT binsChanged(bins);
    }
}

double QImPlotHistogramItemNode::barScale() const
{
    QIM_DC(d);
    return d->barScale;
}

void QImPlotHistogramItemNode::setBarScale(double scale)
{
    QIM_D(d);
    if (d->barScale != scale) {
        d->barScale = scale;
        Q_EMIT barScaleChanged(scale);
    }
}

double QImPlotHistogramItemNode::rangeMin() const
{
    QIM_DC(d);
    return d->rangeMin;
}

void QImPlotHistogramItemNode::setRangeMin(double min)
{
    QIM_D(d);
    if (d->rangeMin != min) {
        d->rangeMin = min;
        Q_EMIT rangeChanged();
    }
}

double QImPlotHistogramItemNode::rangeMax() const
{
    QIM_DC(d);
    return d->rangeMax;
}

void QImPlotHistogramItemNode::setRangeMax(double max)
{
    QIM_D(d);
    if (d->rangeMax != max) {
        d->rangeMax = max;
        Q_EMIT rangeChanged();
    }
}

bool QImPlotHistogramItemNode::isCumulative() const
{
    QIM_DC(d);
    return (d->flags & ImPlotHistogramFlags_Cumulative) != 0;
}

void QImPlotHistogramItemNode::setCumulative(bool cumulative)
{
    QIM_D(d);
    const ImPlotHistogramFlags oldFlags = d->flags;
    if (cumulative) {
        d->flags |= ImPlotHistogramFlags_Cumulative;
    } else {
        d->flags &= ~ImPlotHistogramFlags_Cumulative;
    }
    if (d->flags != oldFlags) {
        Q_EMIT cumulativeChanged(cumulative);
        Q_EMIT histogramFlagChanged();
    }
}

bool QImPlotHistogramItemNode::isDensity() const
{
    QIM_DC(d);
    return (d->flags & ImPlotHistogramFlags_Density) != 0;
}

void QImPlotHistogramItemNode::setDensity(bool density)
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

bool QImPlotHistogramItemNode::isHorizontal() const
{
    QIM_DC(d);
    return (d->flags & ImPlotHistogramFlags_Horizontal) != 0;
}

void QImPlotHistogramItemNode::setHorizontal(bool horizontal)
{
    QIM_D(d);
    const ImPlotHistogramFlags oldFlags = d->flags;
    if (horizontal) {
        d->flags |= ImPlotHistogramFlags_Horizontal;
    } else {
        d->flags &= ~ImPlotHistogramFlags_Horizontal;
    }
    if (d->flags != oldFlags) {
        Q_EMIT orientationChanged(horizontal);
        Q_EMIT histogramFlagChanged();
    }
}

bool QImPlotHistogramItemNode::isOutliersIncluded() const
{
    QIM_DC(d);
    return (d->flags & ImPlotHistogramFlags_NoOutliers) == 0;
}

void QImPlotHistogramItemNode::setOutliersIncluded(bool included)
{
    QIM_D(d);
    const ImPlotHistogramFlags oldFlags = d->flags;
    if (included) {
        d->flags &= ~ImPlotHistogramFlags_NoOutliers;
    } else {
        d->flags |= ImPlotHistogramFlags_NoOutliers;
    }
    if (d->flags != oldFlags) {
        Q_EMIT outliersIncludedChanged(included);
        Q_EMIT histogramFlagChanged();
    }
}

bool QImPlotHistogramItemNode::isColMajor() const
{
    QIM_DC(d);
    return (d->flags & ImPlotHistogramFlags_ColMajor) != 0;
}

void QImPlotHistogramItemNode::setColMajor(bool on)
{
    QIM_D(d);
    const ImPlotHistogramFlags oldFlags = d->flags;
    if (on) {
        d->flags |= ImPlotHistogramFlags_ColMajor;
    } else {
        d->flags &= ~ImPlotHistogramFlags_ColMajor;
    }
    if (d->flags != oldFlags) {
        Q_EMIT histogramFlagChanged();
    }
}

QColor QImPlotHistogramItemNode::color() const
{
    QIM_DC(d);
    return (d->color.has_value()) ? toQColor(d->color->value()) : QColor();
}

void QImPlotHistogramItemNode::setColor(const QColor& c)
{
    QIM_D(d);
    ImVec4 imColor = toImVec4(c);
    if (d->color.has_value()) {
        d->color->operator=(imColor);
    } else {
        d->color.emplace(imColor);
        d->color->mark_dirty();
    }
    Q_EMIT colorChanged(c);
}

int QImPlotHistogramItemNode::histogramFlags() const
{
    QIM_DC(d);
    return d->flags;
}

void QImPlotHistogramItemNode::setHistogramFlags(int flags)
{
    QIM_D(d);
    if (d->flags != flags) {
        d->flags = static_cast< ImPlotHistogramFlags >(flags);
        Q_EMIT histogramFlagChanged();
    }
}

bool QImPlotHistogramItemNode::beginDraw()
{
    QIM_D(d);
    QImAbstractXYDataSeries* rawData = this->data();
    if (!rawData || rawData->size() == 0) {
        return false;
    }

    // Apply style - use SetNextFillStyle for histogram bars
    if (d->color.has_value() && d->color->is_dirty()) {
        ImPlot::SetNextFillStyle(d->color->value());
        d->color->clear();  // Clear dirty flag after applying
    }

    // Prepare ImPlotRange
    ImPlotRange range;
    if (d->rangeMin != 0.0 || d->rangeMax != 0.0) {
        range.Min = d->rangeMin;
        range.Max = d->rangeMax;
    } else {
        range = ImPlotRange();
    }

    // Call ImPlot API
    if (rawData->isContiguous()) {
        const double* values = rawData->yRawData();
        int count = rawData->size();
        ImPlot::PlotHistogram(
            labelConstData(), values, count, d->bins, d->barScale, range, d->flags);
    } else {
        int count = rawData->size();
        std::vector<double> values(count);
        for (int i = 0; i < count; ++i) {
            values[i] = rawData->yValue(i);
        }
        ImPlot::PlotHistogram(
            labelConstData(), values.data(), count, d->bins, d->barScale, range, d->flags);
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
    if (!d->color) {
        d->color = ImPlot::GetLastItemColor();
    }

    return false;
}

}  // namespace QIM