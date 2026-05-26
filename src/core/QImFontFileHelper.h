#ifndef QIMFONTFILEHELPER_H
#define QIMFONTFILEHELPER_H
#include <QFont>
#include "QImAPI.h"
namespace QIM
{
/// \if ENGLISH
/// @brief Font file helper class
///
/// Used to find font file paths based on QFont objects.
///
/// At program startup, preloadCommonFonts scans OS font directories and caches
/// font family names and file paths in a static object.
///
/// Subsequently, QFont objects can be used to look up corresponding TTF files.
/// \endif
///
/// \if CHINESE
/// @brief 字体文件辅助类
///
/// 用于根据 QFont 对象查找字体文件路径。
///
/// 程序启动时，preloadCommonFonts 会扫描系统字体目录，
/// 将字体族名和文件路径缓存到静态对象中。
///
/// 之后可以通过 QFont 对象查询对应的 TTF 文件。
/// \endif
class QIM_CORE_API QImFontFileHelper
{
    QIM_DECLARE_PRIVATE(QImFontFileHelper)
public:
    // Constructs a QImFontFileHelper
    QImFontFileHelper();

    // Destroys the QImFontFileHelper
    ~QImFontFileHelper();

    // Gets all font file paths for the specified font
    static QList< QString > getFontFiles(const QFont& font);

    // Gets all cached font family names
    static QList< QString > getAvailableFamilies();

    // Clears the font cache
    static void clearCache();

    // Preloads common system fonts into the cache
    static void preloadCommonFonts();

    // Gets the pixel size from a QFont, auto-converting from point size if needed
    static float getFontPixelSize(const QFont& qtFont);

    // Gets the recommended Chinese font file path
    static std::string getRecommendedChineseFontPath();
};

}
#endif  // QIMFONTFILEHELPER_H
