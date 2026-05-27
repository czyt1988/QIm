#include "QImPlotStemsItemNode.h"
#include <optional>
#include "implot.h"
#include "implot_internal.h"
#include "QImTrackedValue.hpp"
#include "QtImGuiUtils.h"

namespace QIM
{

class QImPlotStemsItemNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlotStemsItemNode)
public:
    PrivateData(QImPlotStemsItemNode* p);

    ImPlotStemsFlags flags { ImPlotStemsFlags_None };
    double referenceValue { 0.0 };  ///< Reference value (baseline)
    // Style tracking values
    std::optional<QImTrackedValue<ImVec4, QIM::ImVecComparator<ImVec4>>> color;
};

QImPlotStemsItemNode::PrivateData::PrivateData(QImPlotStemsItemNode* p) : q_ptr(p)
{
}

QImPlotStemsItemNode::QImPlotStemsItemNode(QObject* parent) 
    : QImAbstractXYSeriesItemNode(parent), QIM_PIMPL_CONSTRUCT
{
}

QImPlotStemsItemNode::~QImPlotStemsItemNode()
{
}

double QImPlotStemsItemNode::referenceValue() const
{
    QIM_DC(d);
    return d->referenceValue;
}

void QImPlotStemsItemNode::setReferenceValue(double value)
{
    QIM_D(d);
    if (d->referenceValue != value) {
        d->referenceValue = value;
        Q_EMIT referenceValueChanged(value);
    }
}

bool QImPlotStemsItemNode::isHorizontal() const
{
    QIM_DC(d);
    return (d->flags & ImPlotStemsFlags_Horizontal) != 0;
}

void QImPlotStemsItemNode::setHorizontal(bool horizontal)
{
    QIM_D(d);
    const ImPlotStemsFlags oldFlags = d->flags;
    if (horizontal) {
        d->flags |= ImPlotStemsFlags_Horizontal;
    } else {
        d->flags &= ~ImPlotStemsFlags_Horizontal;
    }
    if (d->flags != oldFlags) {
        Q_EMIT orientationChanged(horizontal);
        Q_EMIT stemsFlagChanged();
    }
}

QColor QImPlotStemsItemNode::color() const
{
    QIM_DC(d);
    return (d->color.has_value()) ? toQColor(d->color->value()) : QColor();
}

void QImPlotStemsItemNode::setColor(const QColor& c)
{
    QIM_D(d);
    d->color = toImVec4(c);
    Q_EMIT colorChanged(c);
}

int QImPlotStemsItemNode::stemsFlags() const
{
    QIM_DC(d);
    return d->flags;
}

void QImPlotStemsItemNode::setStemsFlags(int flags)
{
    QIM_D(d);
    if (d->flags != flags) {
        d->flags = static_cast<ImPlotStemsFlags>(flags);
        Q_EMIT stemsFlagChanged();
    }
}

bool QImPlotStemsItemNode::beginDraw()
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
            ImPlot::PlotStems(
                labelConstData(),
                xData,
                yData,
                size,
                d->referenceValue,
                d->flags,
                0,
                sizeof(double));
        } else {
            ImPlot::PlotStems(
                labelConstData(),
                yData,
                size,
                d->referenceValue,
                rawData->xScale(),
                rawData->xStart(),
                d->flags,
                0,
                sizeof(double));
        }
    } else {
        int size = rawData->size();
        std::vector<double> xValues(size);
        std::vector<double> yValues(size);
        for (int i = 0; i < size; ++i) {
            xValues[i] = rawData->xValue(i);
            yValues[i] = rawData->yValue(i);
        }
        ImPlot::PlotStems(
            labelConstData(),
            xValues.data(),
            yValues.data(),
            size,
            d->referenceValue,
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