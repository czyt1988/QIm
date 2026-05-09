#pragma once

#ifdef _WIN32

#include "collector/ProcessInfo.h"
#include <QString>
#include <QHash>
#include <windows.h>
#include <psapi.h>

class ProcessCollector {
public:
    static ProcessCollector* instance();

    ProcessSnapshot takeSnapshot();

private:
    ProcessCollector();

    struct PrevProcessData {
        qint64 kernelTime;
        qint64 userTime;
        qint64 diskReadBytes;
        qint64 diskWriteBytes;
        qint64 timestamp;
    };

    void collectProcessList(QList<ProcessInfo>& processes);
    void collectSystemCpuInfo(ProcessSnapshot& snapshot);
    void collectSystemMemoryInfo(ProcessSnapshot& snapshot);
    double calcCpuPercent(qint64 curKernel, qint64 curUser, qint64 prevKernel, qint64 prevUser, qint64 systemCpuTime);
    qint64 getSystemCpuTime();

    static qint64 fileTimeToInt64(const FILETIME& ft);

    QHash<uint32_t, PrevProcessData> prevProcData_;
    qint64 prevSystemCpuTime_;
    qint64 prevIdleTime_;
    qint64 prevTimestamp_;

    HANDLE hQuery_;
};

#endif // _WIN32