#ifndef QIMFONTFILEHELPER_H
#define QIMFONTFILEHELPER_H
#include <QFont>
#include "QImAPI.h"
namespace QIM
{
/**
 * @brief 字体文件帮助类
 *
 * 用于根据 QFont 对象查找对应的字体文件路径
 *
 * 在程序启动的时候，调用preloadCommonFonts，将扫描操作系统下的字体目录，把字体的家族名字和字体文件缓存到一个静态对象中
 *
 * 后续可以通过QFont来查找这个字体对应哪些ttf文件
 */
class QIM_CORE_API QImFontFileHelper
{
    QIM_DECLARE_PRIVATE(QImFontFileHelper)
public:
    /**
     * @brief 构造函数
     */
    QImFontFileHelper();

    /**
     * @brief 析构函数
     */
    ~QImFontFileHelper();

    /**
     * @brief 获取指定字体的所有字体文件路径
     * @param font Qt字体对象
     * @return 字体文件路径列表（自动去重）
     *
     * 获取指定字体家族的所有可用字体文件，包括粗体、斜体等变体。
     * 支持缓存以提高性能。
     */
    static QList< QString > getFontFiles(const QFont& font);

    /**
     * @brief 获取所有已缓存的字体家族
     * @return 字体家族名称列表
     */
    static QList< QString > getAvailableFamilies();

    /**
     * @brief 清空字体缓存
     */
    static void clearCache();

    /**
     * @brief 预加载常用字体
     */
    static void preloadCommonFonts();

    /**
     * @brief 获取QFont的像素大小，如果是点大小会自动转换
     * @param qtFont
     * @return
     */
    static float getFontPixelSize(const QFont& qtFont);

    /**
     * @brief 推荐的中文字体
     * @return
     */
    static std::string getRecommendedChineseFontPath();
};

}
#endif  // QIMFONTFILEHELPER_H
