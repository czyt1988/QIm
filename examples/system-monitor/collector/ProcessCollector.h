#pragma once

#ifdef _WIN32

#include "collector/ProcessInfo.h"
#include <QString>
#include <QHash>
#include <windows.h>
#include <psapi.h>
#include <pdh.h>
#include <pdhmsg.h>
#include <vector>
#include <string>

class ProcessCollector {
public:
    static ProcessCollector* instance();

    ProcessSnapshot takeSnapshot();

private:
    ProcessCollector();
    ~ProcessCollector();

    struct PrevProcessData {
        qint64 kernelTime;
        qint64 userTime;
        qint64 diskReadBytes;
        qint64 diskWriteBytes;
        qint64 timestamp;
    };

    struct PDHCounter {
        PDH_HCOUNTER handle;
        std::wstring instanceName;
    };

    void collectProcessList(QList<ProcessInfo>& processes);
    void collectSystemCpuInfo(ProcessSnapshot& snapshot);
    void collectSystemMemoryInfo(ProcessSnapshot& snapshot);
    double calcCpuPercent(qint64 curKernel, qint64 curUser, qint64 prevKernel, qint64 prevUser, qint64 systemCpuTime);
    qint64 getSystemCpuTime();

    void initGPUCounters();
    void initNetCounters();
    void collectPDHData(ProcessSnapshot& snapshot);

    static qint64 fileTimeToInt64(const FILETIME& ft);
    static bool containsIgnoreCase(const std::wstring& str, const std::wstring& substr);

    QHash<uint32_t, PrevProcessData> prevProcData_;
    qint64 prevSystemCpuTime_;
    qint64 prevIdleTime_;
    qint64 prevTimestamp_;

    HANDLE hQuery_;
    std::vector<PDHCounter> gpuCounters_;
    std::vector<PDHCounter> netRecvCounters_;
    std::vector<PDHCounter> netSendCounters_;
};

#endif // _WIN32
