#pragma once

#include "widgets/ViewModeSelector.h"
#include "collector/ProcessInfo.h"

#include <QObject>
#include <QList>

namespace QIM {
class QImFigureWidget;
}

class StackedAreaView;
class PieChartView;
class Scatter3DView;
class TimeSeries3DView;
class HistoryBuffer;

class ViewManager : public QObject {
    Q_OBJECT
public:
    explicit ViewManager(QIM::QImFigureWidget* figure, QObject* parent = nullptr);
    ~ViewManager() override;

    void switchTo(ViewMode mode, const QList<AggregatedProcessInfo>& data);
    void updateCurrentView(const QList<AggregatedProcessInfo>& data);
    void setHistoryBuffer(HistoryBuffer* buffer);

Q_SIGNALS:
    void viewSwitched(ViewMode mode);

private:
    QIM::QImFigureWidget* figure_;

    StackedAreaView* stackedAreaView_ = nullptr;
    PieChartView* pieChartView_ = nullptr;
    Scatter3DView* scatter3DView_ = nullptr;
    TimeSeries3DView* timeSeries3DView_ = nullptr;

    ViewMode currentMode_ = static_cast<ViewMode>(-1);
    bool viewBuilt_ = false;
};