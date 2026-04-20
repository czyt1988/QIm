// SystemInfoCollector_win.cpp
// Windows-specific system info collection (CPU model, RAM)
//
// CRITICAL: #define WIN32_LEAN_AND_MEAN and #include <windows.h> appear FIRST,
// before any Qt headers, to prevent Windows macro pollution (near, far, etc.)
// from corrupting Qt header parsing.

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "SystemInfoCollector_win.h"

QString qimGetCpuModel()
{
    // CPU model from registry: HKLM\HARDWARE\DESCRIPTION\System\CentralProcessor\0
    HKEY hKey = nullptr;
    if (RegOpenKeyExW(HKEY_LOCAL_MACHINE,
                      L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
                      0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        WCHAR buffer[256] = {0};
        DWORD bufferSize = sizeof(buffer);
        if (RegQueryValueExW(hKey, L"ProcessorNameString", nullptr, nullptr,
                              reinterpret_cast<LPBYTE>(buffer), &bufferSize) == ERROR_SUCCESS) {
            RegCloseKey(hKey);
            return QString::fromWCharArray(buffer).trimmed();
        }
        RegCloseKey(hKey);
    }
    return QString();
}

double qimGetRamTotalMB()
{
    // Total physical RAM from GlobalMemoryStatusEx
    MEMORYSTATUSEX memStatus;
    memStatus.dwLength = sizeof(memStatus);
    if (GlobalMemoryStatusEx(&memStatus)) {
        return static_cast<double>(memStatus.ullTotalPhys) / (1024.0 * 1024.0);
    }
    return 0.0;
}