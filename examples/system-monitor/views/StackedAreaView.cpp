#include "StackedAreaView.h"

#include <algorithm>
#include <vector>

#include "QImFigureWidget.h"
#include "plot/QImPlotNode.h"
#include "plot/QImPlotLineItemNode.h"
#include "plot/QImPlotAxisInfo.h"
#include "plot/QImPlot.h"
#include "core/ColorPalette.h"
#include "aggregator/HistoryBuffer.h"
#include "aggregator/ProcessAggregator.h"

static constexpr int kMaxVisibleProcesses = 20;       // Show up to 20 processes
static constexpr int kHistoryDurationSec  = 600;       // 10 minutes
static constexpr int kWindowDurationSec   = 600;       // X-axis window: 10 minutes

void StackedAreaView::setHistoryBuffer(HistoryBuffer* buffer)
{
    buffer_ = buffer;
    if (buffer_) {
        buffer_->setMaxDuration(kHistoryDurationSec);
    }
}

void StackedAreaView::buildView(QIM::QImFigureWidget* figure, const QList<AggregatedProcessInfo>& data)
{
    if (!figure)
        return;

    lineItems_.clear();
    lastActiveNames_.clear();

    figure->setSubplotGrid(1, 1);
    plotNode_ = figure->createPlotNode();
    if (!plotNode_)
        return;

    plotNode_->setTitle(QStringLiteral("CPU Usage Over Time"));
    plotNode_->setLegendEnabled(true);
    plotNode_->x1Axis()->setLabel("Time (s)");
    plotNode_->y1Axis()->setLabel("CPU %");
    plotNode_->y1Axis()->setAutoFit(true);   // Y auto-scales to data

    updateData(data);
}

void StackedAreaView::updateData(const QList<AggregatedProcessInfo>& /*data*/)
{
    if (!plotNode_ || !buffer_ || buffer_->pointCount() < 1)
        return;

    // Get time-series data from history buffer
    StackedTimeSeries ts = buffer_->getStackedCpuData();
    if (ts.pointCount < 1 || ts.series.isEmpty())
        return;

    int numPoints = ts.pointCount;
    if (ts.timestamps.empty())
        return;

    qint64 firstTimestamp = ts.timestamps.front();

    // Select which processes to show (top N by latest CPU value)
    // Build a list of (name, latestCpu) for sorting
    struct ProcScore { QString name; double latestCpu; };
    QList<ProcScore> scores;
    for (auto it = ts.series.begin(); it != ts.series.end(); ++it) {
        const auto& values = it.value();
        double latest = (values.empty() ? 0.0 : values.back());
        if (latest > 0.001) {  // Only include currently-active processes
            scores.append({it.key(), latest});
        }
    }
    std::sort(scores.begin(), scores.end(),
              [](const ProcScore& a, const ProcScore& b) {
                  return a.latestCpu > b.latestCpu;
              });

    // Take top N
    QSet<QString> activeNames;
    int showCount = std::min(static_cast<int>(scores.size()), kMaxVisibleProcesses);
    for (int i = 0; i < showCount; ++i) {
        activeNames.insert(scores[i].name);
    }

    // Create line items for processes that don't have one yet
    for (const QString& name : activeNames) {
        if (!lineItems_.contains(name)) {
            auto* line = new QIM::QImPlotLineItemNode(plotNode_);
            line->setLabel(name);
            QColor color = getProcessColor(name);
            line->setColor(color);
            lineItems_.insert(name, line);
        }
    }

    // Update data for ALL tracked items (including exited processes)
    // Exited processes get 0.0 values from HistoryBuffer, so their curves flatline at 0
    for (auto it = lineItems_.begin(); it != lineItems_.end(); ++it) {
        const QString& name = it.key();
        auto* line = it.value();

        auto seriesIt = ts.series.find(name);
        if (seriesIt == ts.series.end())
            continue;

        const auto& yValues = seriesIt.value();
        if (yValues.empty())
            continue;

        // Build X values: seconds from first snapshot
        std::vector<double> xValues(numPoints);
        for (int t = 0; t < numPoints; ++t) {
            xValues[t] = static_cast<double>(ts.timestamps[t] - firstTimestamp) / 1000.0;
        }

        line->setData(xValues, yValues);
    }

    // Sliding-window X axis:
    //   - While elapsed < kWindowDurationSec: X shows 0 → elapsed (growing)
    //   - After kWindowDurationSec:     X shows (elapsed - kWindowDurationSec) → elapsed (fixed-width slide)
    double totalElapsed = static_cast<double>(ts.timestamps.back() - firstTimestamp) / 1000.0;
    double xMin = std::max(0.0, totalElapsed - kWindowDurationSec);
    double xMax = std::max(xMin + 1.0, totalElapsed);
    plotNode_->x1Axis()->setLimits(xMin, xMax, QIM::QImPlotCondition::Always);

    lastActiveNames_ = activeNames;
}
