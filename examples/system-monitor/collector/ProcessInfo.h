#pragma once

#include <QString>
#include <QList>
#include <QHash>
#include <QDateTime>
#include <vector>

struct ProcessInfo {
    uint32_t pid;
    QString processName;
    QString processPath;
    qint64 createTime;
    double cpuPercent;
    qint64 kernelTime;
    qint64 userTime;
    qint64 workingSetBytes;
    qint64 privateBytes;
    qint64 virtualBytes;
    double gpuPercent;
    qint64 gpuDedicatedBytes;
    qint64 gpuSharedBytes;
    qint64 diskReadBytes;
    qint64 diskWriteBytes;
    double diskReadRate;
    double diskWriteRate;
    qint64 networkRecvBytes;
    qint64 networkSendBytes;
    double networkRecvRate;
    double networkSendRate;
    int threadCount;
    int handleCount;
};

struct ProcessSnapshot {
    qint64 timestamp;
    QList<ProcessInfo> processes;
    double systemCpuPercent;
    qint64 systemTotalRamBytes;
    qint64 systemUsedRamBytes;
    double systemGpuPercent;
    double systemDiskReadRate;
    double systemDiskWriteRate;
    double systemNetworkRecvRate;
    double systemNetworkSendRate;
};

struct AggregatedProcessInfo {
    QString processName;
    int instanceCount;
    QList<uint32_t> pids;
    double totalCpuPercent;
    qint64 totalWorkingSetBytes;
    qint64 totalPrivateBytes;
    double avgGpuPercent;
    double totalDiskReadRate;
    double totalDiskWriteRate;
    double totalNetworkRecvRate;
    double totalNetworkSendRate;
    int totalThreadCount;
    double accumulatedCpuTime;
    qint64 avgHeldMemory;
    double accumulatedGpuTime;
};

struct StackedTimeSeries {
    QHash<QString, std::vector<double>> series;
    std::vector<qint64> timestamps;
    int pointCount;
};
