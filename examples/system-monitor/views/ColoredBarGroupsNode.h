#pragma once

#include "plot/QImPlotBarGroupsItemNode.h"
#include "plot/QImPlot.h"
#include "implot.h"

// ColoredBarGroupsNode: thin wrapper around QImPlotBarGroupsItemNode.
// Sets plot axes and delegates rendering to parent class.
// Colormap management is handled by the parent QImPlotNode via
// pushColormap() — this class no longer owns colormap logic.
// Note: No Q_OBJECT needed - no new signals/slots/properties defined.
class ColoredBarGroupsNode : public QIM::QImPlotBarGroupsItemNode {
public:
    using QImPlotBarGroupsItemNode::QImPlotBarGroupsItemNode;
    int type() const override { return QImPlotBarGroupsItemNode::Type; }

protected:
    bool beginDraw() override {
        ImPlot::SetAxes(
            static_cast<ImAxis>(QIM::toImAxis(xAxisId())),
            static_cast<ImAxis>(QIM::toImAxis(yAxisId()))
        );
        return QImPlotBarGroupsItemNode::beginDraw();
    }
};