#pragma once

#include "collector/ProcessInfo.h"
#include <QList>

namespace QIM {
class QImFigureWidget;
class QImPlotNode;
class QImPlotLineItemNode;
}

class HistoryBuffer;

class SystemOverviewView {
public:
    void setHistoryBuffer(HistoryBuffer* buffer);
    void buildView(QIM::QImFigureWidget* figure, const QList<AggregatedProcessInfo>& data);
    void updateData(const QList<AggregatedProcessInfo>& data);

private:
    QIM::QImPlotNode* plotNode_ = nullptr;
    HistoryBuffer* buffer_ = nullptr;

    // y1 items (CPU% + Memory%, left axis, 0-100%)
    QIM::QImPlotLineItemNode* cpuLine_ = nullptr;
    QIM::QImPlotLineItemNode* memLine_ = nullptr;

    // y2 items (DiskIO + Network, right axis, MB/s)
    QIM::QImPlotLineItemNode* diskRLine_ = nullptr;
    QIM::QImPlotLineItemNode* diskWLine_ = nullptr;
    QIM::QImPlotLineItemNode* netRLine_ = nullptr;
    QIM::QImPlotLineItemNode* netSLine_ = nullptr;

    // y3 items (GPU%, right axis, 0-100%)
    QIM::QImPlotLineItemNode* gpuLine_ = nullptr;

    static constexpr int kWindowDurationSec = 600;
};