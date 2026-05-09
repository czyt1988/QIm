#include "StackedAreaView.h"

#include <algorithm>
#include <vector>

#include "QImFigureWidget.h"
#include "plot/QImPlotNode.h"
#include "plot/QImPlotShadedItemNode.h"
#include "plot/QImPlotAxisInfo.h"
#include "plot/QImPlot.h"
#include "core/ColorPalette.h"
#include "aggregator/HistoryBuffer.h"
#include "aggregator/ProcessAggregator.h"

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

    shadedItems_.clear();
    orderedNames_.clear();

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

    // Build X values: seconds from first snapshot (shared across all processes)
    std::vector<double> xValues(numPoints);
    for (int t = 0; t < numPoints; ++t) {
        xValues[t] = static_cast<double>(ts.timestamps[t] - firstTimestamp) / 1000.0;
    }

    // Create shaded items for any processes that don't have one yet
    // (never remove existing items — once created, they persist)
    for (int idx = 0; idx < orderedNames_.size(); ++idx) {
        const QString& name = orderedNames_[idx];
        if (!shadedItems_.contains(name)) {
            auto* shaded = new QIM::QImPlotShadedItemNode(plotNode_);
            shaded->setLabel(name);
            shaded->setColor(getColorForIndex(idx));
            shadedItems_.insert(name, shaded);
        }
    }

    // Pre-compute stacked lower/upper values for each process in stable order
    QHash<QString, std::vector<double>> lowerValues;
    QHash<QString, std::vector<double>> upperValues;
    for (const QString& name : orderedNames_) {
        lowerValues[name].resize(numPoints, 0.0);
        upperValues[name].resize(numPoints, 0.0);
    }

    for (int i = 0; i < numPoints; ++i) {
        double cumulative = 0.0;
        for (const QString& name : orderedNames_) {
            const auto& rawValues = ts.series[name];
            double rawValue = (i < static_cast<int>(rawValues.size())) ? rawValues[i] : 0.0;
            double lower = cumulative;
            double upper = cumulative + rawValue;
            lowerValues[name][i] = lower;
            upperValues[name][i] = upper;
            cumulative = upper;
        }
    }

    // Update data for all shaded items with pre-computed stacked values
    for (const QString& name : orderedNames_) {
        auto* shaded = shadedItems_.value(name, nullptr);
        if (!shaded)
            continue;
        shaded->setData(xValues, lowerValues[name], upperValues[name]);
    }

    // Sliding-window X axis:
    //   - While elapsed < kWindowDurationSec: X shows 0 → elapsed (growing)
    //   - After kWindowDurationSec:     X shows (elapsed - kWindowDurationSec) → elapsed (fixed-width slide)
    double totalElapsed = static_cast<double>(ts.timestamps.back() - firstTimestamp) / 1000.0;
    double xMin = std::max(0.0, totalElapsed - kWindowDurationSec);
    double xMax = std::max(xMin + 1.0, totalElapsed);
    plotNode_->x1Axis()->setLimits(xMin, xMax, QIM::QImPlotCondition::Always);
}
