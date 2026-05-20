#pragma once

#include "collector/ProcessInfo.h"
#include <QList>
#include <vector>

namespace QIM {
class QImFigureWidget;
class QImPlot3DNode;
class QImPlot3DSurfaceItemNode;
}

class HistoryBuffer;

class CpuTimeline3DView {
public:
    void buildView(QIM::QImFigureWidget* figure, const QList<AggregatedProcessInfo>& data);
    void updateData(const QList<AggregatedProcessInfo>& data);
    void setHistoryBuffer(HistoryBuffer* buffer);

private:
    QIM::QImPlot3DNode* plotNode_ = nullptr;
    QIM::QImPlot3DSurfaceItemNode* surfaceNode_ = nullptr;
    HistoryBuffer* buffer_ = nullptr;
};