#pragma once

#include "collector/ProcessInfo.h"

enum class SortBy {
    ByCpu,
    ByMemory,
    ByGpu,
    ByDiskRead,
    ByDiskWrite
};

class ProcessAggregator {
public:
    QList<AggregatedProcessInfo> aggregate(const ProcessSnapshot& snapshot);
    QList<AggregatedProcessInfo> getTopN(const QList<AggregatedProcessInfo>& all, int n, SortBy sortBy);
    AggregatedProcessInfo getOthers(const QList<AggregatedProcessInfo>& topN, const QList<AggregatedProcessInfo>& all);

private:
    QString normalizeProcessName(const QString& name);
};