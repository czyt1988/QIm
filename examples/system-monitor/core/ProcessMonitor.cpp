#include "ProcessMonitor.h"
#include "collector/ProcessCollector.h"
#include "aggregator/ProcessAggregator.h"
#include "aggregator/HistoryBuffer.h"
#include "aggregator/SustainedMetricsTracker.h"

ProcessMonitor::ProcessMonitor(QObject* parent)
    : QObject(parent)
    , m_collector(nullptr)
    , m_aggregator(new ProcessAggregator())
    , m_historyBuffer(new HistoryBuffer(300))
    , m_sustainedMetricsTracker(new SustainedMetricsTracker())
    , m_timer(new QTimer(this))
{
#ifdef _WIN32
    m_collector = ProcessCollector::instance();
#endif
    connect(m_timer, &QTimer::timeout, this, &ProcessMonitor::onTimerTick);
}

ProcessMonitor* ProcessMonitor::instance()
{
    static ProcessMonitor* s_instance = new ProcessMonitor();
    return s_instance;
}

void ProcessMonitor::start()
{
    m_timer->start(1000);
}

void ProcessMonitor::stop()
{
    m_timer->stop();
}

ProcessCollector* ProcessMonitor::collector() const
{
    return m_collector;
}

ProcessAggregator* ProcessMonitor::aggregator() const
{
    return m_aggregator;
}

HistoryBuffer* ProcessMonitor::historyBuffer() const
{
    return m_historyBuffer;
}

SustainedMetricsTracker* ProcessMonitor::sustainedMetricsTracker() const
{
    return m_sustainedMetricsTracker;
}

#ifndef _WIN32
QList<AggregatedProcessInfo> ProcessMonitor::currentAggregated() const
{
    return QList<AggregatedProcessInfo>();
}
#endif

void ProcessMonitor::onTimerTick()
{
#ifdef _WIN32
    if (m_collector) {
        ProcessSnapshot snapshot = m_collector->takeSnapshot();
        m_currentAggregated = m_aggregator->aggregate(snapshot);
        m_sustainedMetricsTracker->addTick(m_currentAggregated, snapshot.timestamp);
        m_historyBuffer->addSnapshot(snapshot);
        Q_EMIT aggregatedReady(m_currentAggregated);
    }
#endif
}
