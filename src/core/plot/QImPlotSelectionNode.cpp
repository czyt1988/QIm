#include "QImPlotSelectionNode.h"
#include "implot.h"
#include "imgui.h"
#include "QImPlotNode.h"
#include "QImPlot.h"

namespace QIM
{

class QImPlotSelectionNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlotSelectionNode)
public:
    PrivateData(QImPlotSelectionNode* p) : q_ptr(p) {}

    QImPlotNode* plotNode { nullptr };
    QImPlotAxisId xAxisId { QImPlotAxisId::Auto };
    QImPlotAxisId yAxisId { QImPlotAxisId::Auto };
    bool selected { false };
    double x1 { 0.0 };
    double y1 { 0.0 };
    double x2 { 0.0 };
    double y2 { 0.0 };
    bool autoCancel { true };
    bool cancelPending { false };
    bool prevSelected { false };
};

QImPlotSelectionNode::QImPlotSelectionNode(QImPlotNode* plotNode, QObject* parent)
    : QImAbstractNode(parent), QIM_PIMPL_CONSTRUCT
{
    QIM_D(d);
    d->plotNode = plotNode;
    setZOrder(99999);
    setObjectName("2d_tools_selection");
}

QImPlotSelectionNode::~QImPlotSelectionNode() {}

QImPlotAxisId QImPlotSelectionNode::xAxis() const
{
    QIM_DC(d);
    return d->xAxisId;
}

void QImPlotSelectionNode::setXAxis(QImPlotAxisId axis)
{
    QIM_D(d);
    if (d->xAxisId != axis) {
        d->xAxisId = axis;
        Q_EMIT axisChanged();
    }
}

QImPlotAxisId QImPlotSelectionNode::yAxis() const
{
    QIM_DC(d);
    return d->yAxisId;
}

void QImPlotSelectionNode::setYAxis(QImPlotAxisId axis)
{
    QIM_D(d);
    if (d->yAxisId != axis) {
        d->yAxisId = axis;
        Q_EMIT axisChanged();
    }
}

QRectF QImPlotSelectionNode::selection() const
{
    QIM_DC(d);
    return QRectF(d->x1, d->y1, d->x2 - d->x1, d->y2 - d->y1);
}

bool QImPlotSelectionNode::isSelected() const
{
    QIM_DC(d);
    return d->selected;
}

bool QImPlotSelectionNode::isAutoCancel() const
{
    QIM_DC(d);
    return d->autoCancel;
}

void QImPlotSelectionNode::setAutoCancel(bool enabled)
{
    QIM_D(d);
    if (d->autoCancel != enabled) {
        d->autoCancel = enabled;
        Q_EMIT autoCancelChanged(enabled);
    }
}

void QImPlotSelectionNode::cancelSelection()
{
    QIM_D(d);
    d->cancelPending = true;
}

bool QImPlotSelectionNode::beginDraw()
{
    QIM_D(d);

    // Handle deferred cancel request
    if (d->cancelPending) {
        ImPlot::CancelPlotSelection();
        d->cancelPending = false;
    }

    // Query current selection state
    d->selected = ImPlot::IsPlotSelected();

    if (d->selected) {
        // Convert QImPlotAxisId to ImAxis for GetPlotSelection query
        ImAxis xAxisIm = (d->xAxisId == QImPlotAxisId::Auto)
                              ? IMPLOT_AUTO
                              : static_cast<ImAxis>(d->xAxisId);
        ImAxis yAxisIm = (d->yAxisId == QImPlotAxisId::Auto)
                              ? IMPLOT_AUTO
                              : static_cast<ImAxis>(d->yAxisId);

        ImPlotRect sel = ImPlot::GetPlotSelection(xAxisIm, yAxisIm);
        d->x1 = sel.X.Min;
        d->y1 = sel.Y.Min;
        d->x2 = sel.X.Max;
        d->y2 = sel.Y.Max;

        Q_EMIT selectionChanged(QRectF(d->x1, d->y1, d->x2 - d->x1, d->y2 - d->y1));

        // Auto-cancel: dismiss selection when SelectCancel button is clicked
        if (d->autoCancel && ImGui::IsMouseClicked(ImPlot::GetInputMap().SelectCancel)) {
            ImPlot::CancelPlotSelection();
        }
    }

    // Emit active state change signal only on transitions
    if (d->selected != d->prevSelected) {
        Q_EMIT selectionActiveChanged(d->selected);
    }
    d->prevSelected = d->selected;

    return false;
}

}  // namespace QIM