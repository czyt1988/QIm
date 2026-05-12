#pragma once

#include "plot/QImPlotBarGroupsItemNode.h"
#include "plot/QImPlot.h"
#include "implot.h"
#include <vector>

// ColoredBarGroupsNode: extends QImPlotBarGroupsItemNode with custom
// colormap support. Registers and pushes a per-item colormap before
// PlotBarGroups() renders, ensuring deterministic color assignment
// independent of process ordering changes.
// Note: No Q_OBJECT needed - no new signals/slots/properties defined.
class ColoredBarGroupsNode : public QIM::QImPlotBarGroupsItemNode {
public:
    using QImPlotBarGroupsItemNode::QImPlotBarGroupsItemNode;
    int type() const override { return QImPlotBarGroupsItemNode::Type; }

    // Set a custom colormap for the next render.
    // The colormap should match the number of bar groups (items).
    // Call this BEFORE setData() — it will be consumed in beginDraw().
    void setCustomColormap(const std::vector<ImVec4>& colors) {
        if (!colors.empty()) {
            // Register colormap with ImPlot (returns colormap index)
            // Use a fixed name for this node's colormap
            colormapIndex_ = ImPlot::AddColormap("__bar_groups_custom__",
                                                 colors.data(),
                                                 static_cast<int>(colors.size()),
                                                 true);  // qualitative
        }
    }

protected:
    bool beginDraw() override {
        bool pushed = false;
        if (colormapIndex_ >= 0) {
            ImPlot::PushColormap(colormapIndex_);
            pushed = true;
        }
        // Also switch to bound axes (for multi-axis compatibility)
        ImPlot::SetAxes(
            static_cast<ImAxis>(QIM::toImAxis(xAxisId())),
            static_cast<ImAxis>(QIM::toImAxis(yAxisId()))
        );
        bool result = QImPlotBarGroupsItemNode::beginDraw();
        if (pushed) {
            ImPlot::PopColormap();
        }
        return result;
    }

private:
    ImPlotColormap colormapIndex_ = -1;
};