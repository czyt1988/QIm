#pragma once

#include "collector/ProcessInfo.h"
#include <vector>

struct SystemTimeSeries {
    std::vector<qint64> timestamps;
    std::vector<double> cpuPercent;
    std::vector<double> memoryPercent;
    std::vector<double> gpuPercent;
    std::vector<double> diskReadMBps;
    std::vector<double> diskWriteMBps;
    std::vector<double> networkRecvMBps;
    std::vector<double> networkSendMBps;
    int pointCount = 0;
};

class HistoryBuffer {
public:
    HistoryBuffer(int maxDurationSeconds = 60, int sampleIntervalMs = 1000);

    void addSnapshot(const ProcessSnapshot& snapshot);
    QList<ProcessSnapshot> getTimeRange(qint64 startMs, qint64 endMs) const;

    // Time-series extraction for stacked charts
    StackedTimeSeries getStackedCpuData() const;
    StackedTimeSeries getStackedMemoryData() const;
    StackedTimeSeries getStackedGpuData() const;

    // System-level time-series data
    SystemTimeSeries getSystemTimeSeries() const;

    int maxDuration() const;
    void setMaxDuration(int seconds);
    int pointCount() const;

private:
    void trimExpired(qint64 cutoff);

    std::vector<ProcessSnapshot> buffer_;
    int maxDurationSeconds_;
    int sampleIntervalMs_;
};
