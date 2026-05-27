#pragma once

#include "collector/ProcessInfo.h"
#include "core/ColorPalette.h"
#include "views/ColoredBarGroupsNode.h"
#include <QHash>
#include <QList>
#include <QSet>
#include <vector>

namespace QIM {
class QImFigureWidget;
class QImPlotNode;
class QImPlotValueTrackerNode;
}

class HistoryBuffer;

class CpuUsageView {
public:
    void setHistoryBuffer(HistoryBuffer* buffer);
    void buildView(QIM::QImFigureWidget* figure, const QList<AggregatedProcessInfo>& data);
    void updateData(const QList<AggregatedProcessInfo>& data);

private:
    QIM::QImPlotNode* plotNode_ = nullptr;
    HistoryBuffer* buffer_ = nullptr;

    // Stable stacking order: first-seen processes listed first, never removed
    QStringList orderedNames_;
    // Bar groups node for stacked bar chart with deterministic colors
    ColoredBarGroupsNode* barGroups_ = nullptr;
    // Value tracker for hover-data tooltip on bar groups
    QIM::QImPlotValueTrackerNode* valueTracker_ = nullptr;
};
