#pragma once

#include "collector/ProcessInfo.h"
#include "core/ColorPalette.h"
#include <QHash>
#include <QList>
#include <QSet>
#include <vector>

namespace QIM {
class QImFigureWidget;
class QImPlotNode;
class QImPlotShadedItemNode;
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

    // Stable stacking order: first-seen processes listed first, never removed
    QStringList orderedNames_;
    // processName → shaded item (persistent: never removed once created)
    QHash<QString, QIM::QImPlotShadedItemNode*> shadedItems_;
    // Color manager for deterministic process colors
    QImSystemMonitor::ColorManager colorManager_;
};
