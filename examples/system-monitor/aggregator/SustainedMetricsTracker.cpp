#include "SustainedMetricsTracker.h"
#include <algorithm>

void SustainedMetricsTracker::addTick(const QList<AggregatedProcessInfo>& processes, qint64 timestamp)
{
    if (startTimestamp_ == 0) {
        startTimestamp_ = timestamp;
    }

    for (const auto& proc : processes) {
        auto& sum = runningSums_[proc.processName];
        sum.accumulatedCpuTime += proc.totalCpuPercent;
        sum.accumulatedGpuTime += proc.avgGpuPercent;
        sum.memorySumMB += static_cast<double>(proc.totalWorkingSetBytes) / 1048576.0;
        sum.memoryTickCount++;
        sum.accumulatedDiskReadKB += proc.totalDiskReadRate;
        sum.accumulatedDiskWriteKB += proc.totalDiskWriteRate;
    }

    SustainedMetricsSnapshot snapshot;
    snapshot.timestamp = timestamp;

    for (auto it = runningSums_.constBegin(); it != runningSums_.constEnd(); ++it) {
        const auto& name = it.key();
        const auto& metrics = it.value();
        snapshot.cpuTime[name] = metrics.accumulatedCpuTime;
        snapshot.gpuTime[name] = metrics.accumulatedGpuTime;
        snapshot.avgMemory[name] = (metrics.memoryTickCount > 0)
            ? metrics.memorySumMB / static_cast<double>(metrics.memoryTickCount)
            : 0.0;
        snapshot.diskReadKB[name] = metrics.accumulatedDiskReadKB;
        snapshot.diskWriteKB[name] = metrics.accumulatedDiskWriteKB;
    }

    history_.append(snapshot);
}

void SustainedMetricsTracker::reset()
{
    runningSums_.clear();
    history_.clear();
    startTimestamp_ = 0;
}

double SustainedMetricsTracker::metricValue(const SustainedProcessMetrics& metrics, SustainedMetric metric) const
{
    switch (metric) {
    case SustainedMetric::CpuTime:
        return metrics.accumulatedCpuTime;
    case SustainedMetric::GpuTime:
        return metrics.accumulatedGpuTime;
    case SustainedMetric::AvgMemory:
        return (metrics.memoryTickCount > 0)
            ? metrics.memorySumMB / static_cast<double>(metrics.memoryTickCount)
            : 0.0;
    case SustainedMetric::DiskReadTotal:
        return metrics.accumulatedDiskReadKB;
    case SustainedMetric::DiskWriteTotal:
        return metrics.accumulatedDiskWriteKB;
    }
    return 0.0;
}

QList<QPair<QString, double>> SustainedMetricsTracker::getRanking(SustainedMetric metric, int topN) const
{
    QList<QPair<QString, double>> ranking;

    for (auto it = runningSums_.constBegin(); it != runningSums_.constEnd(); ++it) {
        ranking.append({it.key(), metricValue(it.value(), metric)});
    }

    std::sort(ranking.begin(), ranking.end(),
        [](const QPair<QString, double>& a, const QPair<QString, double>& b) {
            if (a.second != b.second) {
                return a.second > b.second;
            }
            return a.first < b.first;
        });

    if (topN > 0 && ranking.size() > topN) {
        ranking = ranking.mid(0, topN);
    }

    return ranking;
}

QList<QPair<QString, double>> SustainedMetricsTracker::getRankingWithOthers(SustainedMetric metric, int topN) const
{
    QList<QPair<QString, double>> all = getRanking(metric);
    if (all.size() <= topN) {
        return all;
    }

    QList<QPair<QString, double>> result = all.mid(0, topN);
    double othersSum = 0.0;
    for (int i = topN; i < all.size(); ++i) {
        othersSum += all[i].second;
    }
    result.append({QStringLiteral("Other"), othersSum});
    return result;
}

SustainedTimelineData SustainedMetricsTracker::getTimeline(SustainedMetric metric, int topN) const
{
    SustainedTimelineData result;

    QList<QPair<QString, double>> ranking = getRanking(metric, topN);

    result.topNames.reserve(ranking.size());
    for (const auto& pair : ranking) {
        result.topNames.append(pair.first);
    }

    result.pointCount = history_.size();
    if (result.pointCount == 0) {
        return result;
    }

    result.timestamps.reserve(result.pointCount);
    for (const auto& snapshot : history_) {
        double seconds = static_cast<double>(snapshot.timestamp - startTimestamp_) / 1000.0;
        result.timestamps.append(seconds);
    }

    for (const auto& name : result.topNames) {
        QVector<double> values;
        values.reserve(result.pointCount);

        for (const auto& snapshot : history_) {
            const QHash<QString, double>* metricHash = nullptr;
            switch (metric) {
            case SustainedMetric::CpuTime:
                metricHash = &snapshot.cpuTime;
                break;
            case SustainedMetric::GpuTime:
                metricHash = &snapshot.gpuTime;
                break;
            case SustainedMetric::AvgMemory:
                metricHash = &snapshot.avgMemory;
                break;
            case SustainedMetric::DiskReadTotal:
                metricHash = &snapshot.diskReadKB;
                break;
            case SustainedMetric::DiskWriteTotal:
                metricHash = &snapshot.diskWriteKB;
                break;
            }

            double value = metricHash->value(name, 0.0);
            values.append(value);
        }

        result.values.insert(name, values);
    }

    return result;
}

qint64 SustainedMetricsTracker::startTimestamp() const
{
    return startTimestamp_;
}