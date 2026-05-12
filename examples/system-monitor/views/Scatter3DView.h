#pragma once

#include "collector/ProcessInfo.h"
#include "core/ColorPalette.h"
#include <QHash>
#include <QList>
#include <QSet>

namespace QIM {
class QImFigureWidget;
class QImPlot3DNode;
class QImPlot3DScatterItemNode;
}

class Scatter3DView {
public:
    void buildView(QIM::QImFigureWidget* figure, const QList<AggregatedProcessInfo>& data);
    void updateData(const QList<AggregatedProcessInfo>& data);

private:
    QIM::QImPlot3DNode* plotNode_ = nullptr;
    QImSystemMonitor::ColorManager colorManager_;

    // processName → scatter item (persistent: items stay when process exits)
    QHash<QString, QIM::QImPlot3DScatterItemNode*> scatterItems_;
};