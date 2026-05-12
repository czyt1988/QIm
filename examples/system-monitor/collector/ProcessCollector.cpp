#ifdef _WIN32

#include "collector/ProcessCollector.h"
#include <QDateTime>
#include <QDebug>
#include <cwctype>
#include <algorithm>

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
    PDH_STATUS status = PdhOpenQueryW(nullptr, 0, &hQuery_);
    if (status == ERROR_SUCCESS) {
        initGPUCounters();
        initNetCounters();
    }
}

ProcessCollector::~ProcessCollector()
{
    if (hQuery_) {
        PdhCloseQuery(hQuery_);
        hQuery_ = nullptr;
    }
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

bool ProcessCollector::containsIgnoreCase(const std::wstring& str, const std::wstring& substr)
{
    auto it = std::search(str.begin(), str.end(), substr.begin(), substr.end(),
        [](wchar_t a, wchar_t b) { return std::towlower(a) == std::towlower(b); });
    return it != str.end();
}

// ─── PDH GPU counter initialization ──────────────────────────────────────────
// Enumerates \\GPU Engine(*)\\Utilization Percentage instances,
// keeps only those with "engtype_3D" in the instance name.

void ProcessCollector::initGPUCounters()
{
    const wchar_t* gpuWildcard = L"\\GPU Engine(*)\\Utilization Percentage";

    DWORD pathListLength = 0;
    PDH_STATUS status = PdhExpandWildCardPathW(nullptr, gpuWildcard, nullptr,
                                               &pathListLength, 0);
    if (status != PDH_MORE_DATA || pathListLength == 0) {
        return;
    }

    std::vector<wchar_t> pathBuffer(pathListLength);
    status = PdhExpandWildCardPathW(nullptr, gpuWildcard, pathBuffer.data(),
                                    &pathListLength, 0);
    if (status != ERROR_SUCCESS) {
        return;
    }

    // Parse MULTI_SZ: double-null-terminated array of null-terminated strings
    const wchar_t* path = pathBuffer.data();
    while (*path) {
        std::wstring pathStr(path);
        if (pathStr.find(L"engtype_3D") != std::wstring::npos) {
            PDHCounter counter;
            counter.instanceName = pathStr;
            PDH_STATUS addStatus = PdhAddCounterW(hQuery_, pathStr.c_str(), 0,
                                                  &counter.handle);
            if (addStatus == ERROR_SUCCESS) {
                gpuCounters_.push_back(counter);
            }
        }
        path += wcslen(path) + 1;
    }
}

// ─── PDH Network counter initialization ─────────────────────────────────────
// Enumerates \\Network Adapter(*)\\Bytes Received/sec and Bytes Sent/sec,
// filters out loopback / isatap / teredo pseudo-interfaces.

void ProcessCollector::initNetCounters()
{
    const wchar_t* recvWildcard = L"\\Network Adapter(*)\\Bytes Received/sec";
    const wchar_t* sendWildcard = L"\\Network Adapter(*)\\Bytes Sent/sec";

    // ── Receive counters ────────────────────────────────────────────────────
    DWORD pathListLength = 0;
    PDH_STATUS status = PdhExpandWildCardPathW(nullptr, recvWildcard, nullptr,
                                               &pathListLength, 0);
    if (status == PDH_MORE_DATA && pathListLength > 0) {
        std::vector<wchar_t> pathBuffer(pathListLength);
        status = PdhExpandWildCardPathW(nullptr, recvWildcard, pathBuffer.data(),
                                        &pathListLength, 0);
        if (status == ERROR_SUCCESS) {
            const wchar_t* path = pathBuffer.data();
            while (*path) {
                std::wstring pathStr(path);
                if (!containsIgnoreCase(pathStr, L"loopback") &&
                    !containsIgnoreCase(pathStr, L"isatap") &&
                    !containsIgnoreCase(pathStr, L"teredo")) {
                    PDHCounter counter;
                    counter.instanceName = pathStr;
                    PDH_STATUS addStatus = PdhAddCounterW(hQuery_, pathStr.c_str(), 0,
                                                          &counter.handle);
                    if (addStatus == ERROR_SUCCESS) {
                        netRecvCounters_.push_back(counter);
                    }
                }
                path += wcslen(path) + 1;
            }
        }
    }

    // ── Send counters ───────────────────────────────────────────────────────
    pathListLength = 0;
    status = PdhExpandWildCardPathW(nullptr, sendWildcard, nullptr,
                                    &pathListLength, 0);
    if (status == PDH_MORE_DATA && pathListLength > 0) {
        std::vector<wchar_t> pathBuffer(pathListLength);
        status = PdhExpandWildCardPathW(nullptr, sendWildcard, pathBuffer.data(),
                                        &pathListLength, 0);
        if (status == ERROR_SUCCESS) {
            const wchar_t* path = pathBuffer.data();
            while (*path) {
                std::wstring pathStr(path);
                if (!containsIgnoreCase(pathStr, L"loopback") &&
                    !containsIgnoreCase(pathStr, L"isatap") &&
                    !containsIgnoreCase(pathStr, L"teredo")) {
                    PDHCounter counter;
                    counter.instanceName = pathStr;
                    PDH_STATUS addStatus = PdhAddCounterW(hQuery_, pathStr.c_str(), 0,
                                                          &counter.handle);
                    if (addStatus == ERROR_SUCCESS) {
                        netSendCounters_.push_back(counter);
                    }
                }
                path += wcslen(path) + 1;
            }
        }
    }
}

// ─── PDH data collection ────────────────────────────────────────────────────
// Collects current values for all registered GPU and network counters.
// GPU: aggregates all engtype_3D utilization percentages (average).
// Network: sums Bytes Received/sec and Bytes Sent/sec across all adapters.

void ProcessCollector::collectPDHData(ProcessSnapshot& snapshot)
{
    if (!hQuery_ || (gpuCounters_.empty() && netRecvCounters_.empty() &&
                     netSendCounters_.empty())) {
        return;
    }

    PDH_STATUS status = PdhCollectQueryData(hQuery_);
    if (status != ERROR_SUCCESS) {
        return;
    }

    // ── GPU: aggregate all 3D engine utilization percentages ────────────────
    if (!gpuCounters_.empty()) {
        double totalGpu = 0.0;
        int gpuCount = 0;
        for (const auto& c : gpuCounters_) {
            PDH_FMT_COUNTERVALUE value;
            if (PdhGetFormattedCounterValue(c.handle, PDH_FMT_DOUBLE, nullptr,
                                            &value) == ERROR_SUCCESS) {
                totalGpu += value.doubleValue;
                ++gpuCount;
            }
        }
        snapshot.systemGpuPercent = (gpuCount > 0) ? totalGpu : 0.0;
    }

    // ── Network: sum Bytes/sec across all adapters ──────────────────────────
    if (!netRecvCounters_.empty()) {
        double totalRecv = 0.0;
        for (const auto& c : netRecvCounters_) {
            PDH_FMT_COUNTERVALUE value;
            if (PdhGetFormattedCounterValue(c.handle, PDH_FMT_DOUBLE, nullptr,
                                            &value) == ERROR_SUCCESS) {
                totalRecv += value.doubleValue;
            }
        }
        snapshot.systemNetworkRecvRate = totalRecv;
    }

    if (!netSendCounters_.empty()) {
        double totalSent = 0.0;
        for (const auto& c : netSendCounters_) {
            PDH_FMT_COUNTERVALUE value;
            if (PdhGetFormattedCounterValue(c.handle, PDH_FMT_DOUBLE, nullptr,
                                            &value) == ERROR_SUCCESS) {
                totalSent += value.doubleValue;
            }
        }
        snapshot.systemNetworkSendRate = totalSent;
    }
}

// ─── Main snapshot pipeline ─────────────────────────────────────────────────

ProcessSnapshot ProcessCollector::takeSnapshot()
{
    ProcessSnapshot snapshot;
    snapshot.timestamp = QDateTime::currentMSecsSinceEpoch();

    collectPDHData(snapshot);
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
        // GPU and network are already set by collectPDHData (called before this);
        // preserve those values on failure — only zero out if they weren't set.
        if (snapshot.systemGpuPercent == 0.0) {
            // collected by PDH; 0.0 means either no GPU or not yet collected
        }
        snapshot.systemDiskReadRate = 0.0;
        snapshot.systemDiskWriteRate = 0.0;
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

    // GPU and network are handled by collectPDHData — do NOT zero them out here.
    snapshot.systemDiskReadRate = 0.0;
    snapshot.systemDiskWriteRate = 0.0;
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
