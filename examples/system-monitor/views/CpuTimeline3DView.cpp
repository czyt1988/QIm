#include "CpuTimeline3DView.h"

#include <algorithm>
#include <cmath>
#include <vector>

#include "QImFigureWidget.h"
#include "plot3d/QImPlot3DNode.h"
#include "plot3d/QImPlot3DSurfaceItemNode.h"
#include "plot3d/QImPlot3D.h"
#include "core/ColorPalette.h"
#include "collector/ProcessInfo.h"
#include "aggregator/HistoryBuffer.h"
#include "aggregator/ProcessAggregator.h"

static constexpr int kMaxProcesses = 8;

void CpuTimeline3DView::setHistoryBuffer(HistoryBuffer* buffer)
{
    buffer_ = buffer;
}

void CpuTimeline3DView::buildView(QIM::QImFigureWidget* figure, const QList<AggregatedProcessInfo>& data)
{
    if (!figure)
        return;

    figure->setSubplot3DGrid(1, 1);
    plotNode_ = figure->createPlot3DNode();
    if (!plotNode_)
        return;

    plotNode_->setTitle(QStringLiteral("Resource Over Time"));

    surfaceNode_ = new QIM::QImPlot3DSurfaceItemNode(plotNode_);
    surfaceNode_->setColormapEnabled(true);
    surfaceNode_->setColormap(static_cast<int>(QIM::QImPlot3DColormap::Viridis));
    surfaceNode_->setMarkersVisible(false);

    updateData(data);
}

void CpuTimeline3DView::updateData(const QList<AggregatedProcessInfo>& data)
{
    if (!surfaceNode_)
        return;

    if (buffer_ && buffer_->pointCount() >= 2) {
        // Time-series surface from history buffer
        StackedTimeSeries ts = buffer_->getStackedCpuData();

        if (ts.pointCount < 2 || ts.series.isEmpty())
            return;

        // Select top N processes by total CPU in latest data
        QList<AggregatedProcessInfo> sorted = data;
        std::sort(sorted.begin(), sorted.end(),
                  [](const AggregatedProcessInfo& a, const AggregatedProcessInfo& b) {
                      return a.totalCpuPercent > b.totalCpuPercent;
                  });

        int numProcesses = std::min(static_cast<int>(sorted.size()), kMaxProcesses);
        int numTimePoints = ts.pointCount;

        // Build grid: xCount = numTimePoints, yCount = numProcesses
        // Layout: index = timeIdx * numProcesses + procIdx
        // X varies with timeIdx, Y varies with procIdx, Z = cpu value
        std::vector<double> xs(numTimePoints * numProcesses);
        std::vector<double> ys(numTimePoints * numProcesses);
        std::vector<double> zs(numTimePoints * numProcesses);

        // Normalize time to 0..1 range
        qint64 tMin = ts.timestamps.front();
        qint64 tMax = ts.timestamps.back();
        double tRange = (tMax > tMin) ? static_cast<double>(tMax - tMin) : 1.0;

        for (int t = 0; t < numTimePoints; ++t) {
            double timeValue = static_cast<double>(ts.timestamps[t] - tMin) / tRange;
            for (int p = 0; p < numProcesses; ++p) {
                int index = t * numProcesses + p;
                xs[index] = timeValue;
                ys[index] = static_cast<double>(p);

                // Look up process name in time series
                const QString& procName = sorted[p].processName;
                auto it = ts.series.find(procName);
                if (it != ts.series.end() && t < static_cast<int>(it.value().size())) {
                    zs[index] = it.value()[t];
                } else {
                    zs[index] = 0.0;
                }
            }
        }

        surfaceNode_->setData(xs, ys, zs, numTimePoints, numProcesses);
    } else {
        // Simple snapshot surface: single-row grid showing current CPU usage
        QList<AggregatedProcessInfo> sorted = data;
        std::sort(sorted.begin(), sorted.end(),
                  [](const AggregatedProcessInfo& a, const AggregatedProcessInfo& b) {
                      return a.totalCpuPercent > b.totalCpuPercent;
                  });

        int n = std::min(static_cast<int>(sorted.size()), kMaxProcesses);
        if (n == 0)
            return;

        // Grid: 1 time point × N processes
        std::vector<double> xs(n);
        std::vector<double> ys(n);
        std::vector<double> zs(n);

        for (int i = 0; i < n; ++i) {
            xs[i] = 0.0;
            ys[i] = static_cast<double>(i);
            zs[i] = sorted[i].totalCpuPercent;
        }

        surfaceNode_->setData(xs, ys, zs, 1, n);
    }
}