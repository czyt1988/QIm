#include "QImPlotDigitalItemNode.h"
#include <optional>
#include "implot.h"
#include "implot_internal.h"
#include "QImTrackedValue.hpp"
#include "../QtImGuiUtils.h"

namespace QIM
{

class QImPlotDigitalItemNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlotDigitalItemNode)
public:
    PrivateData(QImPlotDigitalItemNode* p);

    ImPlotDigitalFlags flags { ImPlotDigitalFlags_None };
    // Style tracking values
    std::optional<QImTrackedValue<ImVec4, QIM::ImVecComparator<ImVec4>>> color;
};

QImPlotDigitalItemNode::PrivateData::PrivateData(QImPlotDigitalItemNode* p) : q_ptr(p)
{
}

QImPlotDigitalItemNode::QImPlotDigitalItemNode(QObject* parent) 
    : QImAbstractXYSeriesItemNode(parent), QIM_PIMPL_CONSTRUCT
{
}

QImPlotDigitalItemNode::~QImPlotDigitalItemNode()
{
}

QColor QImPlotDigitalItemNode::color() const
{
    QIM_DC(d);
    return (d->color.has_value()) ? toQColor(d->color->value()) : QColor();
}

void QImPlotDigitalItemNode::setColor(const QColor& c)
{
    QIM_D(d);
    d->color = toImVec4(c);
    Q_EMIT colorChanged(c);
}

int QImPlotDigitalItemNode::digitalFlags() const
{
    QIM_DC(d);
    return d->flags;
}

void QImPlotDigitalItemNode::setDigitalFlags(int flags)
{
    QIM_D(d);
    if (d->flags != flags) {
        d->flags = static_cast<ImPlotDigitalFlags>(flags);
        Q_EMIT digitalFlagChanged();
    }
}

bool QImPlotDigitalItemNode::beginDraw()
{
    QIM_D(d);
    QImAbstractXYDataSeries* rawData = this->data();
    if (!rawData || rawData->size() == 0) {
        return false;
    }

    // Apply style
    if (d->color && d->color->is_dirty()) {
        ImPlot::SetNextLineStyle(d->color->value());
    }

    // Call ImPlot API
    if (rawData->isContiguous()) {
        const double* xData = rawData->xRawData();
        const double* yData = rawData->yRawData();
        int size = rawData->size();

        if (xData) {
            // XY mode with explicit X coordinates
            ImPlot::PlotDigital(
                labelConstData(),
                xData,
                yData,
                size,
                d->flags,
                0,
                sizeof(double));
        } else {
            // Y-only mode - need to generate X values
            std::vector<double> xValues(size);
            double xStart = rawData->xStart();
            double xScale = rawData->xScale();
            for (int i = 0; i < size; ++i) {
                xValues[i] = xStart + i * xScale;
            }
            ImPlot::PlotDigital(
                labelConstData(),
                xValues.data(),
                yData,
                size,
                d->flags,
                0,
                sizeof(double));
        }
    } else {
        // Non-contiguous memory mode: copy data
        int size = rawData->size();
        std::vector<double> xValues(size);
        std::vector<double> yValues(size);
        for (int i = 0; i < size; ++i) {
            xValues[i] = rawData->xValue(i);
            yValues[i] = rawData->yValue(i);
        }
        ImPlot::PlotDigital(
            labelConstData(),
            xValues.data(),
            yValues.data(),
            size,
            d->flags,
            0,
            sizeof(double));
    }

    // Update item status
    if(!imPlotItem()){
        ImPlotContext* ct = ImPlot::GetCurrentContext();
        if (!ct) {
            return false;
        }
        ImPlotItem* plotItem = ct->PreviousItem;
        setImPlotItem(plotItem);
        if (plotItem && plotItem->Show != QImAbstractNode::isVisible()) {
            QImAbstractNode::setVisible(plotItem->Show);
        }
    }
    if (!d->color) {
        d->color = ImPlot::GetLastItemColor();
    }

    return false;
}

}  // namespace QIM