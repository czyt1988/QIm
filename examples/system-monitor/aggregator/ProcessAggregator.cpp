#include "ProcessAggregator.h"

#include <QList>
#include <QHash>
#include <QSet>
#include <QRegularExpression>
#include <algorithm>

QString ProcessAggregator::normalizeProcessName(const QString& name)
{
    // Strip PDH instance name suffixes: "chrome#1" -> "chrome", "svchost#2" -> "svchost"
    static const QRegularExpression pdhSuffix("#\\d+$");
    QString result = name;
    result.replace(pdhSuffix, QString());
    return result;
}

QList<AggregatedProcessInfo> ProcessAggregator::aggregate(const ProcessSnapshot& snapshot)
{
    QHash<QString, AggregatedProcessInfo> groups;

    for (const auto& proc : snapshot.processes) {
        QString normalizedName = normalizeProcessName(proc.processName);
        auto& group = groups[normalizedName];

        if (group.processName.isEmpty()) {
            group.processName = normalizedName;
        }
        group.pids.append(proc.pid);
        group.instanceCount++;
        group.totalCpuPercent += proc.cpuPercent;
        group.totalWorkingSetBytes += proc.workingSetBytes;
        group.totalPrivateBytes += proc.privateBytes;
        group.avgGpuPercent += proc.gpuPercent;
        group.totalDiskReadRate += proc.diskReadRate;
        group.totalDiskWriteRate += proc.diskWriteRate;
        group.totalNetworkRecvRate += proc.networkRecvRate;
        group.totalNetworkSendRate += proc.networkSendRate;
        group.totalThreadCount += proc.threadCount;
    }

    // Average GPU across instances (GPU is per-instance, not additive like CPU)
    for (auto& group : groups) {
        if (group.instanceCount > 0) {
            group.avgGpuPercent /= group.instanceCount;
        }
    }

    return groups.values();
}

QList<AggregatedProcessInfo> ProcessAggregator::getTopN(const QList<AggregatedProcessInfo>& all, int n, SortBy sortBy)
{
    if (all.size() <= n) {
        QList<AggregatedProcessInfo> sorted = all;
        std::sort(sorted.begin(), sorted.end(), [sortBy](const AggregatedProcessInfo& a, const AggregatedProcessInfo& b) {
            switch (sortBy) {
            case SortBy::ByCpu:       return a.totalCpuPercent > b.totalCpuPercent;
            case SortBy::ByMemory:    return a.totalWorkingSetBytes > b.totalWorkingSetBytes;
            case SortBy::ByGpu:       return a.avgGpuPercent > b.avgGpuPercent;
            case SortBy::ByDiskRead:  return a.totalDiskReadRate > b.totalDiskReadRate;
            case SortBy::ByDiskWrite: return a.totalDiskWriteRate > b.totalDiskWriteRate;
            }
            return false;
        });
        return sorted;
    }

    QList<AggregatedProcessInfo> sorted = all;
    std::sort(sorted.begin(), sorted.end(), [sortBy](const AggregatedProcessInfo& a, const AggregatedProcessInfo& b) {
        switch (sortBy) {
        case SortBy::ByCpu:       return a.totalCpuPercent > b.totalCpuPercent;
        case SortBy::ByMemory:    return a.totalWorkingSetBytes > b.totalWorkingSetBytes;
        case SortBy::ByGpu:       return a.avgGpuPercent > b.avgGpuPercent;
        case SortBy::ByDiskRead:  return a.totalDiskReadRate > b.totalDiskReadRate;
        case SortBy::ByDiskWrite: return a.totalDiskWriteRate > b.totalDiskWriteRate;
        }
        return false;
    });

    return sorted.mid(0, n);
}

AggregatedProcessInfo ProcessAggregator::getOthers(const QList<AggregatedProcessInfo>& topN, const QList<AggregatedProcessInfo>& all)
{
    // Collect names of top-N processes for quick lookup
    QSet<QString> topNames;
    for (const auto& item : topN) {
        topNames.insert(item.processName);
    }

    // Sum all metrics from processes NOT in topN
    AggregatedProcessInfo others;
    others.processName = QStringLiteral("Other");
    others.instanceCount = 0;
    others.pids.clear();
    others.totalCpuPercent = 0;
    others.totalWorkingSetBytes = 0;
    others.totalPrivateBytes = 0;
    others.avgGpuPercent = 0;
    others.totalDiskReadRate = 0;
    others.totalDiskWriteRate = 0;
    others.totalNetworkRecvRate = 0;
    others.totalNetworkSendRate = 0;
    others.totalThreadCount = 0;
    others.accumulatedCpuTime = 0;
    others.avgHeldMemory = 0;
    others.accumulatedGpuTime = 0;

    int gpuCount = 0;

    for (const auto& item : all) {
        if (!topNames.contains(item.processName)) {
            others.pids.append(item.pids);
            others.instanceCount += item.instanceCount;
            others.totalCpuPercent += item.totalCpuPercent;
            others.totalWorkingSetBytes += item.totalWorkingSetBytes;
            others.totalPrivateBytes += item.totalPrivateBytes;
            others.avgGpuPercent += item.avgGpuPercent;
            others.totalDiskReadRate += item.totalDiskReadRate;
            others.totalDiskWriteRate += item.totalDiskWriteRate;
            others.totalNetworkRecvRate += item.totalNetworkRecvRate;
            others.totalNetworkSendRate += item.totalNetworkSendRate;
            others.totalThreadCount += item.totalThreadCount;
            gpuCount++;
        }
    }

    // Average GPU across "Other" groups
    if (gpuCount > 0) {
        others.avgGpuPercent /= gpuCount;
    }

    return others;
}