#include "HistoryBuffer.h"
#include <QSet>

HistoryBuffer::HistoryBuffer(int maxDurationSeconds, int sampleIntervalMs)
    : maxDurationSeconds_(maxDurationSeconds)
    , sampleIntervalMs_(sampleIntervalMs)
{
    // Pre-allocate for efficiency (60 seconds at 1 sample/second = 60 samples,
    // plus some margin for variable intervals)
    buffer_.reserve(120);
}

void HistoryBuffer::addSnapshot(const ProcessSnapshot& snapshot)
{
    buffer_.push_back(snapshot);

    // Calculate cutoff time for expired data
    qint64 cutoff = snapshot.timestamp - static_cast<qint64>(maxDurationSeconds_) * 1000;
    trimExpired(cutoff);
}

void HistoryBuffer::trimExpired(qint64 cutoff)
{
    // Remove expired snapshots from the front (oldest first)
    while (!buffer_.empty() && buffer_.front().timestamp < cutoff) {
        buffer_.erase(buffer_.begin());
    }
}

QList<ProcessSnapshot> HistoryBuffer::getTimeRange(qint64 startMs, qint64 endMs) const
{
    QList<ProcessSnapshot> result;

    for (const auto& snapshot : buffer_) {
        if (snapshot.timestamp >= startMs && snapshot.timestamp <= endMs) {
            result.append(snapshot);
        }
    }

    return result;
}

StackedTimeSeries HistoryBuffer::getStackedCpuData() const
{
    StackedTimeSeries result;
    result.pointCount = static_cast<int>(buffer_.size());

    if (buffer_.empty()) {
        return result;
    }

    // Collect timestamps
    result.timestamps.reserve(buffer_.size());
    for (const auto& snapshot : buffer_) {
        result.timestamps.push_back(snapshot.timestamp);
    }

    // Find all unique process names across all snapshots
    QSet<QString> processNames;
    for (const auto& snapshot : buffer_) {
        for (const auto& proc : snapshot.processes) {
            processNames.insert(proc.processName);
        }
    }

    // Build time series for each process
    for (const QString& processName : processNames) {
        std::vector<double> values;
        values.reserve(buffer_.size());

        for (const auto& snapshot : buffer_) {
            double cpuPercent = 0.0;
            // Sum CPU percent for all instances of this process
            for (const auto& proc : snapshot.processes) {
                if (proc.processName == processName) {
                    cpuPercent += proc.cpuPercent;
                }
            }
            values.push_back(cpuPercent);
        }

        result.series.insert(processName, std::move(values));
    }

    return result;
}

StackedTimeSeries HistoryBuffer::getStackedMemoryData() const
{
    StackedTimeSeries result;
    result.pointCount = static_cast<int>(buffer_.size());

    if (buffer_.empty()) {
        return result;
    }

    // Collect timestamps
    result.timestamps.reserve(buffer_.size());
    for (const auto& snapshot : buffer_) {
        result.timestamps.push_back(snapshot.timestamp);
    }

    // Find all unique process names across all snapshots
    QSet<QString> processNames;
    for (const auto& snapshot : buffer_) {
        for (const auto& proc : snapshot.processes) {
            processNames.insert(proc.processName);
        }
    }

    // Build time series for each process (use workingSetBytes)
    for (const QString& processName : processNames) {
        std::vector<double> values;
        values.reserve(buffer_.size());

        for (const auto& snapshot : buffer_) {
            qint64 totalMemory = 0;
            // Sum memory for all instances of this process
            for (const auto& proc : snapshot.processes) {
                if (proc.processName == processName) {
                    totalMemory += proc.workingSetBytes;
                }
            }
            // Convert to MB for easier visualization
            values.push_back(static_cast<double>(totalMemory) / (1024.0 * 1024.0));
        }

        result.series.insert(processName, std::move(values));
    }

    return result;
}

StackedTimeSeries HistoryBuffer::getStackedGpuData() const
{
    StackedTimeSeries result;
    result.pointCount = static_cast<int>(buffer_.size());

    if (buffer_.empty()) {
        return result;
    }

    // Collect timestamps
    result.timestamps.reserve(buffer_.size());
    for (const auto& snapshot : buffer_) {
        result.timestamps.push_back(snapshot.timestamp);
    }

    // Find all unique process names across all snapshots
    QSet<QString> processNames;
    for (const auto& snapshot : buffer_) {
        for (const auto& proc : snapshot.processes) {
            processNames.insert(proc.processName);
        }
    }

    // Build time series for each process (use gpuPercent)
    for (const QString& processName : processNames) {
        std::vector<double> values;
        values.reserve(buffer_.size());

        for (const auto& snapshot : buffer_) {
            double gpuPercent = 0.0;
            // Sum GPU percent for all instances of this process
            for (const auto& proc : snapshot.processes) {
                if (proc.processName == processName) {
                    gpuPercent += proc.gpuPercent;
                }
            }
            values.push_back(gpuPercent);
        }

        result.series.insert(processName, std::move(values));
    }

    return result;
}

int HistoryBuffer::maxDuration() const
{
    return maxDurationSeconds_;
}

void HistoryBuffer::setMaxDuration(int seconds)
{
    maxDurationSeconds_ = seconds;

    // Trim expired data with new duration
    if (!buffer_.empty()) {
        qint64 latestTimestamp = buffer_.back().timestamp;
        qint64 cutoff = latestTimestamp - static_cast<qint64>(maxDurationSeconds_) * 1000;
        trimExpired(cutoff);
    }
}

int HistoryBuffer::pointCount() const
{
    return static_cast<int>(buffer_.size());
}

SystemTimeSeries HistoryBuffer::getSystemTimeSeries() const
{
    SystemTimeSeries result;
    result.pointCount = static_cast<int>(buffer_.size());

    if (buffer_.empty()) {
        return result;
    }

    // Reserve space for all vectors
    result.timestamps.reserve(buffer_.size());
    result.cpuPercent.reserve(buffer_.size());
    result.memoryPercent.reserve(buffer_.size());
    result.gpuPercent.reserve(buffer_.size());
    result.diskReadMBps.reserve(buffer_.size());
    result.diskWriteMBps.reserve(buffer_.size());
    result.networkRecvMBps.reserve(buffer_.size());
    result.networkSendMBps.reserve(buffer_.size());

    for (const auto& snapshot : buffer_) {
        result.timestamps.push_back(snapshot.timestamp);
        result.cpuPercent.push_back(snapshot.systemCpuPercent);
        result.memoryPercent.push_back(snapshot.systemUsedRamBytes * 100.0 / std::max(1LL, snapshot.systemTotalRamBytes));
        result.gpuPercent.push_back(snapshot.systemGpuPercent);
        result.diskReadMBps.push_back(snapshot.systemDiskReadRate / (1024.0 * 1024.0));
        result.diskWriteMBps.push_back(snapshot.systemDiskWriteRate / (1024.0 * 1024.0));
        result.networkRecvMBps.push_back(snapshot.systemNetworkRecvRate / (1024.0 * 1024.0));
        result.networkSendMBps.push_back(snapshot.systemNetworkSendRate / (1024.0 * 1024.0));
    }

    return result;
}
