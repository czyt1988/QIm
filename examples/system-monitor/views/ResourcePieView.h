#pragma once

#include "collector/ProcessInfo.h"
#include <QList>

#include "plot/QImPlotPieChartItemNode.h"
#include "plot/QImPlotNode.h"
#include "QImFigureWidget.h"

class ResourcePieView {
public:
    void buildView(QIM::QImFigureWidget* figure, const QList<AggregatedProcessInfo>& data);
    void updateData(const QList<AggregatedProcessInfo>& data);

private:
    QIM::QImPlotNode* cpuPlot_ = nullptr;
    QIM::QImPlotNode* memPlot_ = nullptr;
    QIM::QImPlotNode* diskPlot_ = nullptr;
    QIM::QImPlotPieChartItemNode* cpuPie_ = nullptr;
    QIM::QImPlotPieChartItemNode* memPie_ = nullptr;
    QIM::QImPlotPieChartItemNode* diskPie_ = nullptr;

    void setupPiePlot(QIM::QImPlotNode* plot, const QString& title);
};