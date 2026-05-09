#pragma once

#include "collector/ProcessInfo.h"

#include <QObject>
#include <QTimer>

class ProcessCollector;
class ProcessAggregator;
class HistoryBuffer;

class ProcessMonitor : public QObject {
    Q_OBJECT
public:
    static ProcessMonitor* instance();

    void start();
    void stop();

    ProcessCollector* collector() const;
    ProcessAggregator* aggregator() const;
    HistoryBuffer* historyBuffer() const;

#ifndef _WIN32
    QList<AggregatedProcessInfo> currentAggregated() const;
#endif

Q_SIGNALS:
    void aggregatedReady(const QList<AggregatedProcessInfo>& data);

private Q_SLOTS:
    void onTimerTick();

private:
    explicit ProcessMonitor(QObject* parent = nullptr);

    ProcessCollector* m_collector;
    ProcessAggregator* m_aggregator;
    HistoryBuffer* m_historyBuffer;
    QTimer* m_timer;
    QList<AggregatedProcessInfo> m_currentAggregated;
};
