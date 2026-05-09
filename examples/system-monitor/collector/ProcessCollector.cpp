#ifdef _WIN32

#include "collector/ProcessCollector.h"
#include <QDateTime>
#include <QDebug>

ProcessCollector* ProcessCollector::instance()
{
    static ProcessCollector inst;
    return &inst;
}

ProcessCollector::ProcessCollector()
    : prevSystemCpuTime_(0)
    , prevIdleTime_(0)
    , prevTimestamp_(0)
    , hQuery_(nullptr)
{
}

qint64 ProcessCollector::fileTimeToInt64(const FILETIME& ft)
{
    return (static_cast<__int64>(ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
}

qint64 ProcessCollector::getSystemCpuTime()
{
    FILETIME idleTime, kernelTime, userTime;
    if (!GetSystemTimes(&idleTime, &kernelTime, &userTime)) {
        return 0;
    }
    qint64 idle = fileTimeToInt64(idleTime);
    qint64 kernel = fileTimeToInt64(kernelTime);
    qint64 user = fileTimeToInt64(userTime);
    return idle + kernel + user;
}

double ProcessCollector::calcCpuPercent(qint64 curKernel, qint64 curUser,
                                        qint64 prevKernel, qint64 prevUser,
                                        qint64 systemCpuTime)
{
    qint64 deltaProc = (curKernel - prevKernel) + (curUser - prevUser);
    qint64 deltaSys = systemCpuTime - prevSystemCpuTime_;
    if (deltaSys <= 0) {
        return 0.0;
    }
    double percent = (100.0 * deltaProc) / deltaSys;
    return qBound(0.0, percent, 100.0);
}

ProcessSnapshot ProcessCollector::takeSnapshot()
{
    ProcessSnapshot snapshot;
    snapshot.timestamp = QDateTime::currentMSecsSinceEpoch();

    collectProcessList(snapshot.processes);
    collectSystemCpuInfo(snapshot);
    collectSystemMemoryInfo(snapshot);

    // Store system CPU time and idle time for next iteration's rate calculation
    FILETIME idleTime, kernelTime, userTime;
    if (GetSystemTimes(&idleTime, &kernelTime, &userTime)) {
        qint64 idle = fileTimeToInt64(idleTime);
        qint64 kernel = fileTimeToInt64(kernelTime);
        qint64 user = fileTimeToInt64(userTime);
        prevSystemCpuTime_ = idle + kernel + user;
        prevIdleTime_ = idle;
    }
    prevTimestamp_ = snapshot.timestamp;

    return snapshot;
}

void ProcessCollector::collectProcessList(QList<ProcessInfo>& processes)
{
    DWORD pids[4096];
    DWORD bytesReturned = 0;

    if (!EnumProcesses(pids, sizeof(pids), &bytesReturned)) {
        return;
    }

    DWORD pidCount = bytesReturned / sizeof(DWORD);
    qint64 currentSystemCpuTime = getSystemCpuTime();
    qint64 currentTimestamp = QDateTime::currentMSecsSinceEpoch();

    for (DWORD i = 0; i < pidCount; ++i) {
        DWORD pid = pids[i];

        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
        if (!hProcess) {
            continue;
        }

        ProcessInfo info;
        info.pid = static_cast<uint32_t>(pid);

        // Process name and path
        WCHAR processPath[MAX_PATH] = {0};
        DWORD pathSize = MAX_PATH;
        if (QueryFullProcessImageNameW(hProcess, 0, processPath, &pathSize)) {
            info.processPath = QString::fromWCharArray(processPath);
            // Extract just the filename from the full path
            int lastSlash = info.processPath.lastIndexOf(QLatin1Char('\\'));
            if (lastSlash >= 0) {
                info.processName = info.processPath.mid(lastSlash + 1);
            } else {
                info.processName = info.processPath;
            }
        } else {
            // Fallback: try GetModuleBaseName
            HMODULE hModule = nullptr;
            DWORD cbNeeded = 0;
            if (EnumProcessModules(hProcess, &hModule, sizeof(hModule), &cbNeeded)) {
                WCHAR moduleName[MAX_PATH] = {0};
                if (GetModuleBaseNameW(hProcess, hModule, moduleName, MAX_PATH)) {
                    info.processName = QString::fromWCharArray(moduleName);
                }
            }
            if (info.processName.isEmpty()) {
                info.processName = QString::number(pid);
            }
        }

        // Process times
        FILETIME creationTime, exitTime, kernelTime, userTime;
        if (GetProcessTimes(hProcess, &creationTime, &exitTime, &kernelTime, &userTime)) {
            info.kernelTime = fileTimeToInt64(kernelTime);
            info.userTime = fileTimeToInt64(userTime);
            info.createTime = fileTimeToInt64(creationTime);

            // Calculate CPU% if we have previous data
            auto it = prevProcData_.find(pid);
            if (it != prevProcData_.end() && prevSystemCpuTime_ > 0) {
                info.cpuPercent = calcCpuPercent(
                    info.kernelTime, info.userTime,
                    it->kernelTime, it->userTime,
                    currentSystemCpuTime);
            } else {
                info.cpuPercent = 0.0;
            }
        } else {
            info.kernelTime = 0;
            info.userTime = 0;
            info.createTime = 0;
            info.cpuPercent = 0.0;
        }

        // Memory info
        PROCESS_MEMORY_COUNTERS_EX pmc = {};
        pmc.cb = sizeof(pmc);
        if (GetProcessMemoryInfo(hProcess, reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&pmc), sizeof(pmc))) {
            info.workingSetBytes = static_cast<qint64>(pmc.WorkingSetSize);
            info.privateBytes = static_cast<qint64>(pmc.PrivateUsage);
            info.virtualBytes = static_cast<qint64>(pmc.PagefileUsage);
        } else {
            info.workingSetBytes = 0;
            info.privateBytes = 0;
            info.virtualBytes = 0;
        }

        // IO counters (disk I/O)
        IO_COUNTERS ioCounters = {};
        if (GetProcessIoCounters(hProcess, &ioCounters)) {
            info.diskReadBytes = static_cast<qint64>(ioCounters.ReadTransferCount);
            info.diskWriteBytes = static_cast<qint64>(ioCounters.WriteTransferCount);

            // Calculate disk rates if we have previous data
            auto it = prevProcData_.find(pid);
            if (it != prevProcData_.end() && prevTimestamp_ > 0) {
                qint64 deltaTimeMs = currentTimestamp - prevTimestamp_;
                if (deltaTimeMs > 0) {
                    double deltaSeconds = deltaTimeMs / 1000.0;
                    info.diskReadRate = (info.diskReadBytes - it->diskReadBytes) / deltaSeconds;
                    info.diskWriteRate = (info.diskWriteBytes - it->diskWriteBytes) / deltaSeconds;
                } else {
                    info.diskReadRate = 0.0;
                    info.diskWriteRate = 0.0;
                }
            } else {
                info.diskReadRate = 0.0;
                info.diskWriteRate = 0.0;
            }
        } else {
            info.diskReadBytes = 0;
            info.diskWriteBytes = 0;
            info.diskReadRate = 0.0;
            info.diskWriteRate = 0.0;
        }

        // GPU: system-level PDH only, per-process placeholder
        info.gpuPercent = 0.0;
        info.gpuDedicatedBytes = 0;
        info.gpuSharedBytes = 0;

        // Network: system-level PDH only, per-process placeholder
        info.networkRecvBytes = 0;
        info.networkSendBytes = 0;
        info.networkRecvRate = 0.0;
        info.networkSendRate = 0.0;

        // Thread and handle count
        info.threadCount = 0;
        info.handleCount = 0;

        // Store prev data for next iteration
        PrevProcessData prevData;
        prevData.kernelTime = info.kernelTime;
        prevData.userTime = info.userTime;
        prevData.diskReadBytes = info.diskReadBytes;
        prevData.diskWriteBytes = info.diskWriteBytes;
        prevData.timestamp = currentTimestamp;
        prevProcData_[pid] = prevData;

        CloseHandle(hProcess);
        processes.append(info);
    }

    // Remove prev data for processes that no longer exist
    QHash<uint32_t, PrevProcessData> cleanedPrevData;
    for (const auto& proc : processes) {
        auto it = prevProcData_.find(proc.pid);
        if (it != prevProcData_.end()) {
            cleanedPrevData[proc.pid] = it.value();
        }
    }
    prevProcData_ = cleanedPrevData;
}

void ProcessCollector::collectSystemCpuInfo(ProcessSnapshot& snapshot)
{
    FILETIME idleTime, kernelTime, userTime;
    if (!GetSystemTimes(&idleTime, &kernelTime, &userTime)) {
        snapshot.systemCpuPercent = 0.0;
        snapshot.systemGpuPercent = 0.0;
        snapshot.systemDiskReadRate = 0.0;
        snapshot.systemDiskWriteRate = 0.0;
        snapshot.systemNetworkRecvRate = 0.0;
        snapshot.systemNetworkSendRate = 0.0;
        return;
    }

    qint64 idle = fileTimeToInt64(idleTime);
    qint64 kernel = fileTimeToInt64(kernelTime);
    qint64 user = fileTimeToInt64(userTime);
    qint64 total = idle + kernel + user;

    if (prevSystemCpuTime_ > 0 && prevIdleTime_ > 0) {
        qint64 deltaTotal = total - prevSystemCpuTime_;
        qint64 deltaIdle = idle - prevIdleTime_;
        if (deltaTotal > 0) {
            snapshot.systemCpuPercent = 100.0 - (100.0 * deltaIdle / deltaTotal);
        } else {
            snapshot.systemCpuPercent = 0.0;
        }
    } else {
        snapshot.systemCpuPercent = 0.0;
    }

    // GPU: system-level placeholder (PDH counter in future)
    snapshot.systemGpuPercent = 0.0;
    snapshot.systemDiskReadRate = 0.0;
    snapshot.systemDiskWriteRate = 0.0;
    snapshot.systemNetworkRecvRate = 0.0;
    snapshot.systemNetworkSendRate = 0.0;
}

void ProcessCollector::collectSystemMemoryInfo(ProcessSnapshot& snapshot)
{
    MEMORYSTATUSEX memoryStatus = {};
    memoryStatus.dwLength = sizeof(memoryStatus);
    if (GlobalMemoryStatusEx(&memoryStatus)) {
        snapshot.systemTotalRamBytes = static_cast<qint64>(memoryStatus.ullTotalPhys);
        snapshot.systemUsedRamBytes = static_cast<qint64>(memoryStatus.ullTotalPhys - memoryStatus.ullAvailPhys);
    } else {
        snapshot.systemTotalRamBytes = 0;
        snapshot.systemUsedRamBytes = 0;
    }
}

#endif // _WIN32