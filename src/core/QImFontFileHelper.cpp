#include "QImFontFileHelper.h"
#include <QFont>
#include <QFontDatabase>
#include <QFontInfo>
#include <QDir>
#include <QFileInfoList>
#include <QDebug>
#include <QCoreApplication>
#include <QStandardPaths>
#include <QMutex>
#include <QScreen>
#include <QApplication>
#include <QFontMetrics>
#ifdef Q_OS_WIN
#include <windows.h>
#include <shlobj.h>
#elif defined(Q_OS_MACOS)
#include <CoreFoundation/CoreFoundation.h>
#include <QRegularExpression>
#endif

namespace QIM
{

// 静态缓存：字体家族名称 -> 字体文件路径列表
static QHash< QString, QList< QString > > g_fontCache;
static QMutex g_cacheMutex;

class QImFontFileHelper::PrivateData
{
    QIM_DECLARE_PUBLIC(QImFontFileHelper)
public:
    explicit PrivateData(QImFontFileHelper* p) : q_ptr(p)
    {
    }

    // 获取系统字体目录路径
    static QStringList getSystemFontDirectories();

    // 扫描单个字体文件
    static void scanFontFile(const QString& fontFilePath);

    // 递归扫描目录
    static void scanFontDirectory(const QString& directory);

    // 根据QFont属性筛选最合适的字体文件
    static QList< QString > filterFontFilesByFont(const QString& family, const QFont& font);
};

QStringList QImFontFileHelper::PrivateData::getSystemFontDirectories()
{
    QStringList fontDirs;

#ifdef Q_OS_WIN
    // Windows字体目录
    wchar_t fontsPath[ MAX_PATH ];
    if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_FONTS, NULL, 0, fontsPath))) {
        fontDirs << QString::fromWCharArray(fontsPath);
    }
    // 备选方案
    if (fontDirs.isEmpty()) {
        fontDirs << "C:/Windows/Fonts";
    }
#elif defined(Q_OS_MACOS)
    // macOS字体目录
    fontDirs << "/System/Library/Fonts"
             << "/Library/Fonts" << QDir::homePath() + "/Library/Fonts";

    // 通过CoreFoundation获取字体目录
    CFURLRef url =
        CFURLCreateWithFileSystemPath(kCFAllocatorDefault, CFSTR("/System/Library/Fonts"), kCFURLPOSIXPathStyle, false);
    if (url) {
        CFStringRef path = CFURLCopyFileSystemPath(url, kCFURLPOSIXPathStyle);
        if (path) {
            fontDirs << QString::fromCFString(path);
            CFRelease(path);
        }
        CFRelease(url);
    }
#else
    // Linux字体目录
    fontDirs << "/usr/share/fonts"
             << "/usr/local/share/fonts" << QDir::homePath() + "/.local/share/fonts" << QDir::homePath() + "/.fonts";

    // 通过fontconfig配置获取额外目录
    QString fontConfigPath =
        QStandardPaths::locate(QStandardPaths::ConfigLocation, "fontconfig/fonts.conf", QStandardPaths::LocateFile);
    if (!fontConfigPath.isEmpty()) {
        // 可以解析fontconfig获取更多字体目录
        // 简化处理，使用常见目录
    }
#endif

    return fontDirs;
}

void QImFontFileHelper::PrivateData::scanFontFile(const QString& fontFilePath)
{
    // 使用QFontDatabase加载字体文件
    int fontId = QFontDatabase::addApplicationFont(fontFilePath);
    if (fontId != -1) {
        QStringList families = QFontDatabase::applicationFontFamilies(fontId);
        foreach (const QString& family, families) {
            // 添加到缓存（自动去重）
            auto ite = g_fontCache.find(family);
            if (ite == g_fontCache.end()) {
                ite = g_fontCache.insert(family, QList< QString >());
            }
            if (!ite.value().contains(fontFilePath)) {
                ite.value().append(fontFilePath);
            }
        }
        // 移除临时加载的字体
        QFontDatabase::removeApplicationFont(fontId);
    }
}

void QImFontFileHelper::PrivateData::scanFontDirectory(const QString& directory)
{
    QDir dir(directory);
    if (!dir.exists()) {
        return;
    }

    // 支持的字体文件扩展名
    const QStringList filters = { "*.ttf", "*.otf" };

    // 获取所有字体文件
    const QFileInfoList fileList = dir.entryInfoList(filters, QDir::Files | QDir::Readable);
    QSet< QString > scanedFiles;
    for (const QFileInfo& fileInfo : fileList) {
        QString fontFile = fileInfo.absoluteFilePath();
        if (scanedFiles.contains(fontFile)) {
            continue;
        }
        scanFontFile(fontFile);
    }
}

QList< QString > QImFontFileHelper::PrivateData::filterFontFilesByFont(const QString& family, const QFont& font)
{
    QList< QString > result;
    if (!g_fontCache.contains(family)) {
        // 尝试从备用家族查找
        QStringList availableFamilies = g_fontCache.keys();
        for (const QString& availFamily : availableFamilies) {
            if (availFamily.contains(family, Qt::CaseInsensitive) || family.contains(availFamily, Qt::CaseInsensitive)) {
                result.append(g_fontCache[ availFamily ]);
            }
        }
        // 去重
        QSet< QString > uniqueFiles(result.begin(), result.end());
        result = QList< QString >(uniqueFiles.begin(), uniqueFiles.end());
        return result;
    }

    // 获取所有该家族的字体文件
    QList< QString > allFiles = g_fontCache[ family ];

    // 如果只有1个文件，直接返回
    if (allFiles.size() <= 1) {
        return allFiles;
    }

    // TODO: 根据字体的粗细、斜体等属性筛选最匹配的字体文件
    // 这里简化处理，返回所有文件
    // 在Windows上可以通过IDWriteFontFamily等API进行精确匹配
    // 在Linux上可以通过fontconfig匹配

    return allFiles;
}

// ============================================================================
// 构造函数和析构函数
// ============================================================================

/**
 * \if ENGLISH
 * @brief Default constructor
 * \endif
 * \if CHINESE
 * @brief 默认构造函数
 * \endif
 */
QImFontFileHelper::QImFontFileHelper() : QIM_PIMPL_CONSTRUCT
{
}

/**
 * \if ENGLISH
 * @brief Destructor
 * \endif
 * \if CHINESE
 * @brief 析构函数
 * \endif
 */
QImFontFileHelper::~QImFontFileHelper()
{
}

// ============================================================================
// 公共接口实现
// ============================================================================

/**
 * \if ENGLISH
 * @brief Gets font file paths matching a QFont's family name
 * @param[in] font Qt font object whose family name is used for lookup
 * @return List of font file paths that match the font family, empty if cache is not loaded
 * @details Requires preloadCommonFonts() to be called first. Falls back to
 *          fuzzy family name matching if exact match is not found.
 * \endif
 * \if CHINESE
 * @brief 获取与 QFont 字体家族名称匹配的字体文件路径
 * @param[in] font Qt 字体对象，使用其家族名称进行查找
 * @return 匹配字体家族的文件路径列表，若缓存未加载则返回空列表
 * @details 需要先调用 preloadCommonFonts()。若精确匹配未找到，
 *          会尝试模糊匹配字体家族名称。
 * \endif
 */
QList< QString > QImFontFileHelper::getFontFiles(const QFont& font)
{
    QMutexLocker locker(&g_cacheMutex);
    if (g_fontCache.isEmpty()) {
        qWarning() << "Font cache is empty. Call preloadCommonFonts() first.";
        return {};
    }

    QString family = font.family();
    if (family.isEmpty()) {
        // 获取默认字体
        family = QFontInfo(font).family();
    }

    return PrivateData::filterFontFilesByFont(family, font);
}

/**
 * \if ENGLISH
 * @brief Gets all available font family names from cache
 * @return List of font family names that have been scanned
 * \endif
 * \if CHINESE
 * @brief 获取缓存中所有可用的字体家族名称
 * @return 已扫描的字体家族名称列表
 * \endif
 */
QList< QString > QImFontFileHelper::getAvailableFamilies()
{
    QMutexLocker locker(&g_cacheMutex);
    return g_fontCache.keys();
}

/**
 * \if ENGLISH
 * @brief Clears the font file cache, requiring reload via preloadCommonFonts()
 * \endif
 * \if CHINESE
 * @brief 清空字体文件缓存，需要通过 preloadCommonFonts() 重新加载
 * \endif
 */
void QImFontFileHelper::clearCache()
{
    QMutexLocker locker(&g_cacheMutex);
    g_fontCache.clear();
}

/**
 * \if ENGLISH
 * @brief Scans system font directories and populates the font cache
 * @details Scans platform-specific font directories (Windows Fonts folder,
 *          macOS /System/Library/Fonts, Linux /usr/share/fonts) and indexes
 *          all .ttf/.otf files by family name. Only runs once; subsequent
 *          calls are skipped if cache is already populated.
 * \endif
 * \if CHINESE
 * @brief 扫描系统字体目录并填充字体缓存
 * @details 扫描平台特定的字体目录（Windows Fonts 目录、macOS
 *          /System/Library/Fonts、Linux /usr/share/fonts），并按家族名称
 *          索引所有 .ttf/.otf 文件。仅执行一次；缓存已填充后再次调用将被跳过。
 * \endif
 */
void QImFontFileHelper::preloadCommonFonts()
{
    QMutexLocker locker(&g_cacheMutex);

    if (!g_fontCache.isEmpty()) {
        // 已经加载过
        return;
    }

    QImFontFileHelper helper;

    // 获取系统字体目录
    const QStringList fontDirs = PrivateData::getSystemFontDirectories();

    qDebug() << "Scanning font directories:" << fontDirs;

    // 扫描所有字体目录
    for (const QString& fontDir : fontDirs) {
        helper.d_ptr->scanFontDirectory(fontDir);
    }

    qDebug() << "Font scanning completed.";
    qDebug() << "Found" << g_fontCache.size() << "font families";
    if (!g_fontCache.isEmpty()) {
        qDebug() << "Sample families:" << g_fontCache.keys().mid(0, 10);
    }
}

/**
 * \if ENGLISH
 * @brief Converts a QFont's size to pixel size suitable for ImGui font loading
 * @param[in] qtFont Qt font object to extract size from
 * @return Pixel size in float, converted from point size using screen DPI if needed
 * @details If the font has pixelSize set, uses it directly. If pointSize is set,
 *          converts using screen DPI (formula: points * dpi / 72). Otherwise
 *          uses QFontMetrics height as fallback. Minimum value is 1.0.
 * \endif
 * \if CHINESE
 * @brief 将 QFont 的字体大小转换为适用于 ImGui 字体加载的像素大小
 * @param[in] qtFont Qt 字体对象，用于提取字体大小
 * @return 像素大小（float），如有需要会使用屏幕 DPI 从磅值转换
 * @details 若字体设置了 pixelSize，直接使用；若设置了 pointSize，
 *          使用屏幕 DPI 转换（公式：磅值 * DPI / 72）。否则使用
 *          QFontMetrics 高度作为回退。最小值为 1.0。
 * \endif
 */
float QImFontFileHelper::getFontPixelSize(const QFont& qtFont)
{
    float pixelSize = 0.0f;

    if (qtFont.pixelSize() > 0) {
        // Qt 中已经设置了像素大小，直接使用
        pixelSize = static_cast< float >(qtFont.pixelSize());
    } else if (qtFont.pointSize() > 0) {
        // Qt 中设置了点大小，需要转换为像素
        // 获取 DPI
        int dpi = 96;  // 默认 DPI

        // 尝试获取屏幕 DPI
        QScreen* screen = QGuiApplication::primaryScreen();
        if (screen) {
            dpi = screen->logicalDotsPerInchY();
        }

        // 点转像素公式：points * dpi / 72
        pixelSize = qtFont.pointSize() * dpi / 72.0f;
    } else {
        // 没有设置大小，使用默认值
        QFontMetrics metrics(qtFont);
        pixelSize = static_cast< float >(metrics.height());
    }

    // 确保大小合理
    pixelSize = qMax(pixelSize, 1.0f);  // 最小 6 像素

    return pixelSize;
}

/**
 * \if ENGLISH
 * @brief Gets the recommended Chinese font file path for the current platform
 * @return Absolute path to the best available Chinese font file, or empty string if none found
 * @details Tries platform-specific candidates in priority order:
 *          - Windows: msyh.ttc (Microsoft YaHei), msyhbd.ttc, simhei.ttf, simsun.ttc
 *          - macOS: PingFang.ttc, Hiragino Sans GB.ttc, STHeiti
 *          - Linux: NotoSansCJK, WenQuanYi Micro Hei, DroidSansFallback
 * \endif
 * \if CHINESE
 * @brief 获取当前平台推荐的中文字体文件路径
 * @return 最佳可用中文字体文件的绝对路径，若未找到则返回空字符串
 * @details 按优先级顺序尝试平台特定的候选字体：
 *          - Windows：msyh.ttc（微软雅黑）、msyhbd.ttc、simhei.ttf、simsun.ttc
 *          - macOS：PingFang.ttc、Hiragino Sans GB.ttc、STHeiti
 *          - Linux：NotoSansCJK、文泉驿微米黑、DroidSansFallback
 * \endif
 */
std::string QImFontFileHelper::getRecommendedChineseFontPath()
{
#if defined(_WIN32)
    // Windows: 优先使用微软雅黑（清晰现代），其次黑体、宋体
    static const char* candidates[] = {
        "C:/Windows/Fonts/msyh.ttc",    // 微软雅黑
        "C:/Windows/Fonts/msyhbd.ttc",  // 微软雅黑 Bold（备用）
        "C:/Windows/Fonts/simhei.ttf",  // 黑体
        "C:/Windows/Fonts/simsun.ttc"   // 宋体（注意是 .ttc）
    };
    for (const char* path : candidates) {
        if (!QFileInfo::exists(path)) {
            continue;
        }
        return std::string(path);
    }
    return "";  // 不应到达此处

#elif defined(__APPLE__)
    // macOS: 系统字体路径会随版本变化，按常见中文字体顺序探测
    static const char* candidates[] = {
        "/System/Library/Fonts/PingFang.ttc",
        "/System/Library/Fonts/Supplemental/PingFang.ttc",
        "/System/Library/Fonts/Hiragino Sans GB.ttc",
        "/System/Library/Fonts/STHeiti Medium.ttc",
        "/System/Library/Fonts/STHeiti Light.ttc"
    };
    for (const char* path : candidates) {
        if (!QFileInfo::exists(path)) {
            continue;
        }
        return std::string(path);
    }
    return "";

#elif defined(__linux__) || defined(__unix__)
    // Linux: 常见中文字体（根据发行版可能不同）
    static const char* candidates[] = {
        "/usr/share/fonts/opentype/noto/NotoSansCJK-Regular.ttc",  // Noto Sans CJK（Google 推荐）
        "/usr/share/fonts/wqy-microhei/wqy-microhei.ttc",          // 文泉驿微米黑
        "/usr/share/fonts/truetype/wqy/wqy-microhei.ttc",
        "/usr/share/fonts/truetype/droid/DroidSansFallbackFull.ttf"  // Droid Sans Fallback（较旧）
    };
    for (const char* path : candidates) {
        if (!QFileInfo::exists(path)) {
            continue;
        }
        return std::string(path);
    }
    return "";

#else
    // 未知平台，返回空或默认
    return "";
#endif
}

}  // namespace QIM
