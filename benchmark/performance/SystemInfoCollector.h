// SystemInfoCollector.h
#ifndef SYSTEMINFOCOLLECTOR_H
#define SYSTEMINFOCOLLECTOR_H

#include <QString>
#include <QSysInfo>
#include <thread>

/**
 * @brief System hardware and software information struct
 * 
 * Contains OS, CPU, RAM, GPU, OpenGL, screen, disk, Qt, and compiler
 * information collected from the running system.
 */
struct SystemInfo
{
    QString osName;           ///< OS name (from QSysInfo::prettyProductName())
    QString osVersion;        ///< OS version (from QSysInfo::productVersion())
    QString cpuModel;         ///< CPU model string (platform-specific)
    int     cpuCoreCount     = 0;    ///< Number of CPU cores (from std::thread::hardware_concurrency)
    double  ramTotalMB       = 0.0;  ///< Total physical RAM in MB (platform-specific)
    QString gpuName;         ///< GPU name (OpenGL renderer string, filled by collectGPUInfo)
    double  gpuVramMB        = 0.0;  ///< GPU VRAM in MB (best-effort, 0.0 if unknown)
    QString openglVersion;   ///< OpenGL version string (filled by collectGPUInfo)
    QString openglRenderer;  ///< OpenGL renderer string (filled by collectGPUInfo)
    bool    isSoftwareOpenGL = false; ///< True if software OpenGL detected (llvmpipe/swrast/softpipe)
    QString screenResolution; ///< Screen resolution as "WxH" string
    QString diskType;        ///< Disk type: "SSD", "HDD", or "Unknown"
    QString qtVersion;       ///< Qt version string (compile-time + runtime)
    QString compilerInfo;    ///< Compiler identification string
};

/**
 * @brief Cross-platform system information collector
 * 
 * Static utility class for collecting hardware/OS information.
 * GPU/OpenGL info requires an active GL context — call collectGPUInfo()
 * separately after the GL context is initialized.
 */
class SystemInfoCollector
{
public:
    /**
     * @brief Collect system information (OS, CPU, RAM, screen, Qt, compiler, disk)
     * @return Filled SystemInfo struct (GPU fields remain empty until collectGPUInfo is called)
     */
    static SystemInfo collectSystemInfo();

    /**
     * @brief Collect GPU and OpenGL information
     * 
     * Must be called after an OpenGL context is active (e.g., after QOpenGLWidget initialization).
     * Fills gpuName, gpuVramMB, openglVersion, openglRenderer, and isSoftwareOpenGL fields.
     * 
     * @param info SystemInfo struct to fill GPU fields into
     */
    static void collectGPUInfo(SystemInfo& info);
};

Q_DECLARE_METATYPE(SystemInfo)

#endif // SYSTEMINFOCOLLECTOR_H