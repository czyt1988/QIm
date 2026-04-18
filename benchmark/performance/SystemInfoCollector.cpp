// SystemInfoCollector.cpp
#include "SystemInfoCollector.h"

#include <QGuiApplication>
#include <QScreen>
#include <QDebug>

// Platform-specific headers for CPU model, RAM, disk detection
#if defined(Q_OS_WIN)
#include <windows.h>
#include <psapi.h>
#elif defined(Q_OS_LINUX)
#include <unistd.h>
#include <sys/sysinfo.h>
#include <fstream>
#include <string>
#include <malloc.h>
#elif defined(Q_OS_MACOS)
#include <mach/mach.h>
#include <sys/sysctl.h>
#endif

// OpenGL headers for GPU detection
#include <GL/gl.h>

SystemInfo SystemInfoCollector::collectSystemInfo()
{
    SystemInfo info;

    // ── OS ──
    info.osName    = QSysInfo::prettyProductName();
    info.osVersion = QSysInfo::productVersion();

    // ── CPU cores ──
    info.cpuCoreCount = static_cast<int>(std::thread::hardware_concurrency());

    // ── Screen resolution ──
    try {
        if (QGuiApplication::primaryScreen()) {
            QSize screenSize = QGuiApplication::primaryScreen()->size();
            info.screenResolution = QString("%1x%2")
                .arg(screenSize.width())
                .arg(screenSize.height());
        }
    } catch (...) {
        info.screenResolution = QString();
    }

    // ── Qt version ──
    info.qtVersion = QString(QT_VERSION_STR) + " (runtime: " + qVersion() + ")";

    // ── Compiler info ──
#if defined(_MSC_VER)
    info.compilerInfo = "MSVC " + QString::number(_MSC_VER);
#elif defined(__GNUC__)
    info.compilerInfo = "GCC " + QString::number(__GNUC__) + "." + QString::number(__GNUC_MINOR__);
#elif defined(__clang__)
    info.compilerInfo = "Clang " + QString::number(__clang_major__) + "." + QString::number(__clang_minor__);
#else
    info.compilerInfo = "Unknown";
#endif

    // ── Platform-specific: CPU model, RAM, disk type ──
#if defined(Q_OS_WIN)
    // Windows: CPU model from registry
    try {
        HKEY hKey = nullptr;
        LONG result = RegOpenKeyExW(
            HKEY_LOCAL_MACHINE,
            L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
            0,
            KEY_READ,
            &hKey
        );
        if (result == ERROR_SUCCESS && hKey) {
            WCHAR buffer[256] = {0};
            DWORD bufferSize = sizeof(buffer);
            DWORD type = 0;
            result = RegQueryValueExW(
                hKey,
                L"ProcessorNameString",
                nullptr,
                &type,
                reinterpret_cast<LPBYTE>(buffer),
                &bufferSize
            );
            if (result == ERROR_SUCCESS && type == REG_SZ) {
                info.cpuModel = QString::fromWCharArray(buffer).trimmed();
            }
            RegCloseKey(hKey);
        }
    } catch (...) {
        info.cpuModel = QString();
    }

    // Windows: RAM from GlobalMemoryStatusEx
    try {
        MEMORYSTATUSEX memstatus;
        memstatus.dwLength = sizeof(memstatus);
        if (GlobalMemoryStatusEx(&memstatus)) {
            info.ramTotalMB = static_cast<double>(memstatus.ullTotalPhys) / (1024.0 * 1024.0);
        }
    } catch (...) {
        info.ramTotalMB = 0.0;
    }

    // Windows: Disk type — reliable detection too complex, use "Unknown"
    info.diskType = "Unknown";

#elif defined(Q_OS_LINUX)
    // Linux: CPU model from /proc/cpuinfo
    try {
        std::ifstream cpuinfo("/proc/cpuinfo");
        if (cpuinfo.is_open()) {
            std::string line;
            while (std::getline(cpuinfo, line)) {
                if (line.find("model name") == 0) {
                    size_t colonPos = line.find(':');
                    if (colonPos != std::string::npos && colonPos + 1 < line.size()) {
                        info.cpuModel = QString::fromStdString(line.substr(colonPos + 1)).trimmed();
                    }
                    break;
                }
            }
        }
    } catch (...) {
        info.cpuModel = QString();
    }

    // Linux: RAM from sysinfo
    try {
        struct sysinfo si;
        if (sysinfo(&si) == 0) {
            info.ramTotalMB = static_cast<double>(si.totalram) * static_cast<double>(si.mem_unit) / (1024.0 * 1024.0);
        }
    } catch (...) {
        info.ramTotalMB = 0.0;
    }

    // Linux: Disk type from /sys/block/sda/queue/rotational
    try {
        std::ifstream rotational("/sys/block/sda/queue/rotational");
        if (rotational.is_open()) {
            int value = 1;
            rotational >> value;
            info.diskType = (value == 0) ? "SSD" : "HDD";
        } else {
            info.diskType = "Unknown";
        }
    } catch (...) {
        info.diskType = "Unknown";
    }

#elif defined(Q_OS_MACOS)
    // macOS: CPU model from sysctlbyname
    try {
        char buffer[256] = {0};
        size_t size = sizeof(buffer);
        if (sysctlbyname("machdep.cpu.brand_string", buffer, &size, nullptr, 0) == 0) {
            info.cpuModel = QString::fromUtf8(buffer).trimmed();
        }
    } catch (...) {
        info.cpuModel = QString();
    }

    // macOS: RAM from sysctlbyname hw.memsize
    try {
        int64_t value = 0;
        size_t size = sizeof(value);
        if (sysctlbyname("hw.memsize", &value, &size, nullptr, 0) == 0) {
            info.ramTotalMB = static_cast<double>(value) / (1024.0 * 1024.0);
        }
    } catch (...) {
        info.ramTotalMB = 0.0;
    }

    // macOS: Disk type — reliable detection too complex
    info.diskType = "Unknown";

#else
    info.cpuModel   = QString();
    info.ramTotalMB = 0.0;
    info.diskType   = "Unknown";
#endif

    return info;
}

void SystemInfoCollector::collectGPUInfo(SystemInfo& info)
{
    // ── OpenGL version and renderer ──
    try {
        const GLubyte* versionStr  = glGetString(GL_VERSION);
        const GLubyte* rendererStr = glGetString(GL_RENDERER);

        if (versionStr) {
            info.openglVersion = QString::fromUtf8(reinterpret_cast<const char*>(versionStr));
        }
        if (rendererStr) {
            info.openglRenderer = QString::fromUtf8(reinterpret_cast<const char*>(rendererStr));
            info.gpuName        = info.openglRenderer;
        }
    } catch (...) {
        info.openglVersion  = QString();
        info.openglRenderer = QString();
        info.gpuName        = QString();
    }

    // ── Software OpenGL detection ──
    try {
        QString rendererLower = info.openglRenderer.toLower();
        if (rendererLower.contains("llvmpipe") ||
            rendererLower.contains("swrast") ||
            rendererLower.contains("softpipe")) {
            info.isSoftwareOpenGL = true;
        }
    } catch (...) {
        info.isSoftwareOpenGL = false;
    }

    // ── VRAM detection (best-effort, platform-specific) ──
#if defined(Q_OS_WIN)
    // Windows: VRAM detection via registry is unreliable; best-effort fallback to 0.0
    try {
        // Attempt to read from registry HKLM\SYSTEM\CurrentControlSet\Control\Video
        // This is unreliable as GPU VRAM is not consistently stored in registry
        // Fallback: 0.0
        info.gpuVramMB = 0.0;
    } catch (...) {
        info.gpuVramMB = 0.0;
    }

#elif defined(Q_OS_LINUX)
    // Linux: Try AMD VRAM from sysfs, then NVIDIA via nvidia-smi subprocess
    try {
        std::ifstream vramFile("/sys/class/drm/card0/device/mem_info_vram_total");
        if (vramFile.is_open()) {
            long long vramBytes = 0;
            vramFile >> vramBytes;
            if (vramBytes > 0) {
                info.gpuVramMB = static_cast<double>(vramBytes) / (1024.0 * 1024.0);
            }
        } else {
            // Try NVIDIA: read nvidia-smi output
            // Best-effort: subprocess may not be available; fallback to 0.0
            info.gpuVramMB = 0.0;
        }
    } catch (...) {
        info.gpuVramMB = 0.0;
    }

#elif defined(Q_OS_MACOS)
    // macOS: VRAM detection not easily available; fallback to 0.0
    info.gpuVramMB = 0.0;

#else
    info.gpuVramMB = 0.0;
#endif
}