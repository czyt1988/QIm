#include "SystemMetricsView.h"

#include <algorithm>
#include <vector>

#include "QImFigureWidget.h"
#include "plot/QImPlotNode.h"
#include "plot/QImPlotLineItemNode.h"
#include "plot/QImPlotAxisInfo.h"
#include "plot/QImPlot.h"
#include "plot/QImPlotLegendNode.h"
#include "plot/QImPlotValueTrackerNode.h"
#include "aggregator/HistoryBuffer.h"

// -----------------------------------------------------------
// setHistoryBuffer
// -----------------------------------------------------------
void SystemMetricsView::setHistoryBuffer(HistoryBuffer* buffer)
{
    buffer_ = buffer;
}

// -----------------------------------------------------------
// buildView
// -----------------------------------------------------------
void SystemMetricsView::buildView(QIM::QImFigureWidget* figure, const QList<AggregatedProcessInfo>& /*data*/)
{
    if (!figure)
        return;

    figure->setSubplotGrid(1, 1);
    plotNode_ = figure->createPlotNode();
    if (!plotNode_)
        return;

    plotNode_->setTitle(QStringLiteral("System Overview"));
    plotNode_->x1Axis()->setLabel(QStringLiteral("Time (s)"));

    // Y1: Usage % (CPU + Memory)
    QIM::QImPlotAxisInfo* y1 = plotNode_->y1Axis();
    y1->setLabel(QStringLiteral("Usage %"));
    y1->setLimits(0.0, 110.0, QIM::QImPlotCondition::Always);
    y1->setAutoFit(true);

    // Y2: MB/s (Disk IO + Network)
    QIM::QImPlotAxisInfo* y2 = plotNode_->y2Axis();
    y2->setEnabled(true);
    y2->setLabel(QStringLiteral("MB/s"));
    y2->setAutoFit(true);

    // Y3: GPU %
    QIM::QImPlotAxisInfo* y3 = plotNode_->y3Axis();
    y3->setEnabled(true);
    y3->setLabel(QStringLiteral("GPU %"));
    y3->setLimits(0.0, 110.0, QIM::QImPlotCondition::Always);
    y3->setAutoFit(true);

    // ---- y1 items: CPU + Memory ----
    cpuLine_ = new QIM::QImPlotLineItemNode(plotNode_);
    cpuLine_->setLabel(QStringLiteral("CPU"));
    cpuLine_->bindAxis(QIM::QImPlotAxisId::X1, QIM::QImPlotAxisId::Y1);
    cpuLine_->setColor(QColor("#332288"));

    memLine_ = new QIM::QImPlotLineItemNode(plotNode_);
    memLine_->setLabel(QStringLiteral("Memory"));
    memLine_->bindAxis(QIM::QImPlotAxisId::X1, QIM::QImPlotAxisId::Y1);
    memLine_->setColor(QColor("#117733"));

    // ---- y2 items: Disk IO + Network ----
    diskRLine_ = new QIM::QImPlotLineItemNode(plotNode_);
    diskRLine_->setLabel(QStringLiteral("Disk Read"));
    diskRLine_->bindAxis(QIM::QImPlotAxisId::X1, QIM::QImPlotAxisId::Y2);
    diskRLine_->setColor(QColor("#44AA99"));

    diskWLine_ = new QIM::QImPlotLineItemNode(plotNode_);
    diskWLine_->setLabel(QStringLiteral("Disk Write"));
    diskWLine_->bindAxis(QIM::QImPlotAxisId::X1, QIM::QImPlotAxisId::Y2);
    diskWLine_->setColor(QColor("#CC9944"));

    netRLine_ = new QIM::QImPlotLineItemNode(plotNode_);
    netRLine_->setLabel(QStringLiteral("Net Recv"));
    netRLine_->bindAxis(QIM::QImPlotAxisId::X1, QIM::QImPlotAxisId::Y2);
    netRLine_->setColor(QColor("#AA4499"));

    netSLine_ = new QIM::QImPlotLineItemNode(plotNode_);
    netSLine_->setLabel(QStringLiteral("Net Sent"));
    netSLine_->bindAxis(QIM::QImPlotAxisId::X1, QIM::QImPlotAxisId::Y2);
    netSLine_->setColor(QColor("#CC6677"));

    // ---- y3 items: GPU ----
    gpuLine_ = new QIM::QImPlotLineItemNode(plotNode_);
    gpuLine_->setLabel(QStringLiteral("GPU"));
    gpuLine_->bindAxis(QIM::QImPlotAxisId::X1, QIM::QImPlotAxisId::Y3);
    gpuLine_->setColor(QColor("#882255"));

    // Legend outside at bottom
    QIM::QImPlotLegendNode* legend = plotNode_->legendNode();
    if (legend) {
        legend->setOutside(true);
        legend->setLocation(QIM::QImPlotLegendLocation::South);
    }

    // Value tracker: hover-data tooltip showing all 7 series at cursor position
    valueTracker_ = new QIM::QImPlotValueTrackerNode(plotNode_);
    valueTracker_->setFixedWidth(220.0f);
    valueTracker_->setSkipNanFiniteValues(true);
    plotNode_->addChildNode(valueTracker_);
}

// -----------------------------------------------------------
// updateData
// -----------------------------------------------------------
void SystemMetricsView::updateData(const QList<AggregatedProcessInfo>& /*data*/)
{
    if (!plotNode_ || !buffer_ || buffer_->pointCount() < 1)
        return;

    SystemTimeSeries ts = buffer_->getSystemTimeSeries();
    if (ts.pointCount < 1 || ts.timestamps.empty())
        return;

    int numPoints = ts.pointCount;

    // Build X axis: seconds since first timestamp
    qint64 firstTs = ts.timestamps.front();
    std::vector<double> xValues(numPoints);
    for (int i = 0; i < numPoints; ++i) {
        xValues[i] = static_cast<double>(ts.timestamps[i] - firstTs) / 1000.0;
    }

    // Set data on all 7 line items
    cpuLine_->setData(xValues, ts.cpuPercent);
    memLine_->setData(xValues, ts.memoryPercent);
    diskRLine_->setData(xValues, ts.diskReadMBps);
    diskWLine_->setData(xValues, ts.diskWriteMBps);
    netRLine_->setData(xValues, ts.networkRecvMBps);
    netSLine_->setData(xValues, ts.networkSendMBps);
    gpuLine_->setData(xValues, ts.gpuPercent);

    // Sliding window: show last 600 seconds
    double totalElapsed = xValues.back();
    double xMin = std::max(0.0, totalElapsed - static_cast<double>(kWindowDurationSec));
    double xMax = std::max(xMin + 1.0, totalElapsed);
    plotNode_->x1Axis()->setLimits(xMin, xMax, QIM::QImPlotCondition::Always);
}
