#pragma once

#include "collector/ProcessInfo.h"
#include <QHash>
#include <QList>
#include <QSet>
#include <vector>

namespace QIM {
class QImFigureWidget;
class QImPlotNode;
class QImPlotLineItemNode;
}

class HistoryBuffer;

class StackedAreaView {
public:
    void setHistoryBuffer(HistoryBuffer* buffer);
    void buildView(QIM::QImFigureWidget* figure, const QList<AggregatedProcessInfo>& data);
    void updateData(const QList<AggregatedProcessInfo>& data);

private:
    QIM::QImPlotNode* plotNode_ = nullptr;
    HistoryBuffer* buffer_ = nullptr;

    // processName → line item (persistent: items stay when process exits)
    QHash<QString, QIM::QImPlotLineItemNode*> lineItems_;
    // Track which processes were active in the last update (for cleanup)
    QSet<QString> lastActiveNames_;
};
