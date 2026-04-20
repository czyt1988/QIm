// SystemInfoCollector_win.h
#ifndef SYSTEMINFOCOLLECTOR_WIN_H
#define SYSTEMINFOCOLLECTOR_WIN_H

#include <QString>

/**
 * @brief Get CPU model string from Windows registry
 *
 * Reads ProcessorNameString from
 * HKLM\HARDWARE\DESCRIPTION\System\CentralProcessor\0
 *
 * @return CPU model string (e.g. "Intel(R) Core(TM) i7-10700K CPU @ 3.80GHz"),
 *         or empty QString if registry read fails
 */
QString qimGetCpuModel();

/**
 * @brief Get total physical RAM size in MB via Windows API
 *
 * Uses GlobalMemoryStatusEx() to retrieve ullTotalPhys.
 *
 * @return Total physical RAM in MB (e.g. 32768.0), or 0.0 if API call fails
 */
double qimGetRamTotalMB();

#endif // SYSTEMINFOCOLLECTOR_WIN_H