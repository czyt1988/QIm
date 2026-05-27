#include "CpuUsageView.h"

#include <algorithm>
#include <vector>

#include "QImFigureWidget.h"
#include "plot/QImPlotNode.h"
#include "plot/QImPlotLegendNode.h"
#include "plot/QImPlotAxisInfo.h"
#include "plot/QImPlot.h"
#include "plot/QImPlotValueTrackerNode.h"
#include "aggregator/HistoryBuffer.h"
#include "aggregator/ProcessAggregator.h"

static constexpr int kHistoryDurationSec  = 300;       // 5 minutes
static constexpr int kWindowDurationSec   = 300;       // X-axis window: 5 minutes

void CpuUsageView::setHistoryBuffer(HistoryBuffer* buffer)
{
    buffer_ = buffer;
    if (buffer_) {
        buffer_->setMaxDuration(kHistoryDurationSec);
    }
}

void CpuUsageView::buildView(QIM::QImFigureWidget* figure, const QList<AggregatedProcessInfo>& data)
{
    if (!figure)
        return;

    orderedNames_.clear();

    figure->setSubplotGrid(1, 1);
    plotNode_ = figure->createPlotNode();
    if (!plotNode_)
        return;

    plotNode_->setTitle(QStringLiteral("CPU Usage Over Time"));
    plotNode_->setLegendEnabled(true);
    plotNode_->x1Axis()->setLabel("Time (s)");
    plotNode_->y1Axis()->setLabel("CPU %");
    plotNode_->y1Axis()->setAutoFit(true);

    // Create stacked bar groups node with deterministic colors
    barGroups_ = new ColoredBarGroupsNode(plotNode_);
    barGroups_->setStacked(true);
    barGroups_->setGroupWidth(0.8);

    // Colormap is registered once in ViewManager constructor.
    // Just push it at plot level — QImPlotNode auto-pops in endDraw()
    plotNode_->pushColormap("__bar_groups_custom__");

    // External legend at bottom
    QIM::QImPlotLegendNode* legend = plotNode_->legendNode();
    if (legend) {
        legend->setOutside(true);
        legend->setLocation(QIM::QImPlotLegendLocation::South);
    }

    // Value tracker: hover tooltip showing stacked bar values at cursor position
    valueTracker_ = new QIM::QImPlotValueTrackerNode(plotNode_);
    valueTracker_->setFixedWidth(240.0f);
    valueTracker_->setSkipNanFiniteValues(true);
    plotNode_->addChildNode(valueTracker_);

    updateData(data);
}

void CpuUsageView::updateData(const QList<AggregatedProcessInfo>& /*data*/)
{
    if (!plotNode_ || !barGroups_ || !buffer_ || buffer_->pointCount() < 1)
        return;

    // Get time-series data from history buffer
    StackedTimeSeries ts = buffer_->getStackedCpuData();
    if (ts.pointCount < 1 || ts.series.isEmpty())
        return;

    int numPoints = ts.pointCount;
    int groupCount = numPoints;

    // On first call, establish stable order: all process names sorted alphabetically.
    // On subsequent calls, append any newly-seen process names to the end.
    // Never remove processes — once added, they stay in the stack forever.
    if (orderedNames_.isEmpty()) {
        orderedNames_ = ts.series.keys();
        orderedNames_.sort();
    } else {
        QSet<QString> existing = QSet<QString>(orderedNames_.begin(), orderedNames_.end());
        for (auto it = ts.series.begin(); it != ts.series.end(); ++it) {
            if (!existing.contains(it.key())) {
                orderedNames_.append(it.key());
            }
        }
    }

    int itemCount = orderedNames_.size();

    // Build row-major values matrix: values[item * groupCount + group]
    // Each row corresponds to one process (item), columns are time groups
    QVector<double> values(itemCount * groupCount, 0.0);
    for (int item = 0; item < itemCount; ++item) {
        const auto& raw = ts.series[orderedNames_[item]];
        for (int group = 0; group < groupCount; ++group) {
            values[item * groupCount + group] = (group < static_cast<int>(raw.size())) ? raw[group] : 0.0;
        }
    }

    // Colormap is registered once in buildView() and pushed at plot level — no per-update work needed

    // Set data: orderedNames_ provides item labels, values is row-major matrix
    barGroups_->setData(orderedNames_, values, itemCount, groupCount);

    // Sliding-window X axis using group indices (not timestamps)
    double xMax = std::max(1.0, static_cast<double>(groupCount - 1));
    double xMin = std::max(0.0, xMax - kWindowDurationSec);
    plotNode_->x1Axis()->setLimits(xMin, xMax, QIM::QImPlotCondition::Always);
}
