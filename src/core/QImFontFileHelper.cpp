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

QImFontFileHelper::QImFontFileHelper() : QIM_PIMPL_CONSTRUCT
{
}

QImFontFileHelper::~QImFontFileHelper()
{
}

// ============================================================================
// 公共接口实现
// ============================================================================

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

QList< QString > QImFontFileHelper::getAvailableFamilies()
{
    QMutexLocker locker(&g_cacheMutex);
    return g_fontCache.keys();
}

void QImFontFileHelper::clearCache()
{
    QMutexLocker locker(&g_cacheMutex);
    g_fontCache.clear();
}

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
    // macOS: PingFang 是默认中文字体（从 macOS 10.11 起）
    // 注意：PingFang 是 .ttc 格式，包含多个字重
    return "/System/Library/Fonts/PingFang.ttc";

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
        return std::std::string(path);
    }
    return "";

#else
    // 未知平台，返回空或默认
    return "";
#endif
}

}  // namespace QIM
