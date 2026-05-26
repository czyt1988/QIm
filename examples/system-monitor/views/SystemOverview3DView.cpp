#include "SystemOverview3DView.h"

#include "QImFigureWidget.h"
#include "plot3d/QImPlot3DNode.h"
#include "plot3d/QImPlot3DSurfaceItemNode.h"
#include "plot3d/QImPlot3D.h"
#include "aggregator/HistoryBuffer.h"

#include <algorithm>
#include <cmath>

void SystemOverview3DView::setHistoryBuffer(HistoryBuffer* buffer)
{
    buffer_ = buffer;
}

void SystemOverview3DView::buildView(QIM::QImFigureWidget* figure, const QList<AggregatedProcessInfo>& data)
{
    if (!figure)
        return;

    figure->setSubplot3DGrid(1, 1);
    plotNode_ = figure->createPlot3DNode();
    if (!plotNode_)
        return;

    plotNode_->setTitle(QStringLiteral("System Overview"));
    plotNode_->setupAxes(QByteArray("Time (s)"), QByteArray("Metric"), QByteArray("Normalized"));

    // Y-axis tick labels for 5 metric lanes
    QList<double> yTicks = {0.0, 1.0, 2.0, 3.0, 4.0};
    QList<QByteArray> yLabels = {QByteArray("CPU"), QByteArray("Memory"), QByteArray("GPU"), QByteArray("NetIn"), QByteArray("NetOut")};
    plotNode_->yAxis()->setAxisTicks(yTicks, yLabels);

    surfaceNode_ = new QIM::QImPlot3DSurfaceItemNode(plotNode_);
    surfaceNode_->setColormapEnabled(true);
    surfaceNode_->setColormap(static_cast<int>(QIM::QImPlot3DColormap::Viridis));
    surfaceNode_->setMarkersVisible(false);

    updateData(data);
}

void SystemOverview3DView::updateData(const QList<AggregatedProcessInfo>& data)
{
    if (!surfaceNode_ || !buffer_ || buffer_->pointCount() < 2)
        return;

    SystemTimeSeries ts = buffer_->getSystemTimeSeries();

    if (ts.pointCount < 2 || ts.timestamps.empty())
        return;

    const int xCount = ts.pointCount;
    const int yCount = 5;
    const int totalPoints = xCount * yCount;

    std::vector<double> xs(totalPoints);
    std::vector<double> ys(totalPoints);
    std::vector<double> zs(totalPoints);

    const qint64 t0 = ts.timestamps[0];

    for (int i = 0; i < xCount; ++i) {
        double xVal = (ts.timestamps[i] - t0) / 1000.0;

        for (int j = 0; j < yCount; ++j) {
            const int idx = i * yCount + j;
            xs[idx] = xVal;
            ys[idx] = static_cast<double>(j);

            switch (j) {
            case 0:  // CPU - already 0-100
                zs[idx] = ts.cpuPercent[i];
                break;
            case 1:  // Memory - already 0-100
                zs[idx] = ts.memoryPercent[i];
                break;
            case 2:  // GPU - already 0-100
                zs[idx] = ts.gpuPercent[i];
                break;
            case 3:  // Network recv - clamp 0-10 MB/s then *10 for 0-100
                zs[idx] = std::clamp(ts.networkRecvMBps[i], 0.0, 10.0) * 10.0;
                break;
            case 4:  // Network send - clamp 0-10 MB/s then *10 for 0-100
                zs[idx] = std::clamp(ts.networkSendMBps[i], 0.0, 10.0) * 10.0;
                break;
            }

            // Sanitize NaN / Inf
            if (!std::isfinite(zs[idx]))
                zs[idx] = 0.0;
        }
    }

    surfaceNode_->setData(xs, ys, zs, xCount, yCount);

    // Sliding X-axis window: show last 300 seconds
    const double xMax = (ts.timestamps[xCount - 1] - t0) / 1000.0;
    const double xMin = std::max(0.0, xMax - 300.0);
    plotNode_->xAxis()->setLimits(xMin, xMax, QIM::QImPlot3DCondition::Always);
}
