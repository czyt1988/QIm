#ifndef QIMFONTFILEHELPER_H
#define QIMFONTFILEHELPER_H
#include <QFont>
#include "QImAPI.h"
namespace QIM
{
/**
 * @brief Font file helper class
 *
 * Used to find font file paths based on QFont objects.
 *
 * At program startup, preloadCommonFonts scans OS font directories and caches
 * font family names and file paths in a static object.
 *
 * Subsequently, QFont objects can be used to look up corresponding TTF files.
 */
class QIM_CORE_API QImFontFileHelper
{
    QIM_DECLARE_PRIVATE(QImFontFileHelper)
public:
    /**
     * @brief Constructor
     */
    QImFontFileHelper();

    /**
     * @brief Destructor
     */
    ~QImFontFileHelper();

    /**
     * @brief Get all font file paths for the specified font
     * @param font Qt font object
     * @return List of font file paths (deduplicated)
     *
     * Retrieves all available font files for the specified font family,
     * including bold, italic, and other variants.
     * Supports caching for improved performance.
     */
    static QList< QString > getFontFiles(const QFont& font);

    /**
     * @brief Get all cached font families
     * @return List of font family names
     */
    static QList< QString > getAvailableFamilies();

    /**
     * @brief Clear font cache
     */
    static void clearCache();

    /**
     * @brief Preload common fonts
     */
    static void preloadCommonFonts();

    /**
     * @brief Get pixel size from QFont, auto-converting from point size if needed
     * @param qtFont
     * @return Pixel size as float
     */
    static float getFontPixelSize(const QFont& qtFont);

    /**
     * @brief Recommended Chinese font file path
     * @return Path to recommended Chinese font
     */
    static std::string getRecommendedChineseFontPath();
};

}
#endif  // QIMFONTFILEHELPER_H
