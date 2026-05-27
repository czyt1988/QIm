#pragma once

#include "collector/ProcessInfo.h"
#include "core/ColorPalette.h"
#include <QHash>
#include <QList>
#include <QSet>
#include <vector>

namespace QIM {
class QImFigureWidget;
class QImPlot3DNode;
class QImPlot3DScatterItemNode;
class QImPlot3DMousePickerNode;
class QImAbstractXYZDataSeries;
}

class SustainedMetricsTracker;

class ProcessResource3DView {
public:
    void buildView(QIM::QImFigureWidget* figure, const QList<AggregatedProcessInfo>& data);
    void updateData(const QList<AggregatedProcessInfo>& data);
    void setTracker(SustainedMetricsTracker* tracker);

private:
    void updateDataInstantaneous(const QList<AggregatedProcessInfo>& data);
    void rebuildCombinedData();

    QIM::QImPlot3DNode* plotNode_ = nullptr;
    QImSystemMonitor::ColorManager colorManager_;
    SustainedMetricsTracker* tracker_ = nullptr;

    // Mouse picker for 3D hover coordinate display
    QIM::QImPlot3DMousePickerNode* mousePicker_ = nullptr;

    // Combined data series for the picker (all scatter points merged per-frame)
    QIM::QImAbstractXYZDataSeries* combinedDataSeries_ = nullptr;

    // Combined data vectors (rebuilt each updateData for the picker)
    std::vector<double> combinedXs_;
    std::vector<double> combinedYs_;
    std::vector<double> combinedZs_;

    // processName → scatter item (persistent: items stay when process exits)
    QHash<QString, QIM::QImPlot3DScatterItemNode*> scatterItems_;
};