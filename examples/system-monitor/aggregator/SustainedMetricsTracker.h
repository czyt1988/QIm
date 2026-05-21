#pragma once

#include "collector/ProcessInfo.h"
#include <QHash>
#include <QVector>
#include <QList>
#include <QPair>
#include <QStringList>

enum class SustainedMetric {
    CpuTime,
    GpuTime,
    AvgMemory,
    DiskReadTotal,
    DiskWriteTotal
};

struct SustainedProcessMetrics {
    double accumulatedCpuTime = 0.0;
    double accumulatedGpuTime = 0.0;
    double memorySumMB = 0.0;
    int memoryTickCount = 0;
    double accumulatedDiskReadKB = 0.0;
    double accumulatedDiskWriteKB = 0.0;
};

struct SustainedMetricsSnapshot {
    qint64 timestamp;
    QHash<QString, double> cpuTime;
    QHash<QString, double> gpuTime;
    QHash<QString, double> avgMemory;
    QHash<QString, double> diskReadKB;
    QHash<QString, double> diskWriteKB;
};

struct SustainedTimelineData {
    QVector<double> timestamps;
    QHash<QString, QVector<double>> values;
    QStringList topNames;
    int pointCount = 0;
};

class SustainedMetricsTracker {
public:
    void addTick(const QList<AggregatedProcessInfo>& processes, qint64 timestamp);
    void reset();

    QList<QPair<QString, double>> getRanking(SustainedMetric metric, int topN = -1) const;
    SustainedTimelineData getTimeline(SustainedMetric metric, int topN = 10) const;
    qint64 startTimestamp() const;

private:
    double metricValue(const SustainedProcessMetrics& metrics, SustainedMetric metric) const;

    QHash<QString, SustainedProcessMetrics> runningSums_;
    QList<SustainedMetricsSnapshot> history_;
    qint64 startTimestamp_ = 0;
};