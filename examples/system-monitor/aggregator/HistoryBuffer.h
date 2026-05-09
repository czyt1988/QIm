#pragma once

#include "collector/ProcessInfo.h"
#include <vector>

class HistoryBuffer {
public:
    HistoryBuffer(int maxDurationSeconds = 60, int sampleIntervalMs = 1000);

    void addSnapshot(const ProcessSnapshot& snapshot);
    QList<ProcessSnapshot> getTimeRange(qint64 startMs, qint64 endMs) const;

    // Time-series extraction for stacked charts
    StackedTimeSeries getStackedCpuData() const;
    StackedTimeSeries getStackedMemoryData() const;
    StackedTimeSeries getStackedGpuData() const;

    int maxDuration() const;
    void setMaxDuration(int seconds);
    int pointCount() const;

private:
    void trimExpired(qint64 cutoff);

    std::vector<ProcessSnapshot> buffer_;
    int maxDurationSeconds_;
    int sampleIntervalMs_;
};
