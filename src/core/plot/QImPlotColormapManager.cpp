#include "QImPlotColormapManager.h"
#include "../QtImGuiUtils.h"
#include "implot.h"
#include <vector>

namespace QIM
{

//===============================================================
// Query Methods
//===============================================================

/**
 * \if ENGLISH
 * @brief Returns the number of available colormaps
 * @return Count of registered colormaps
 * @details Wraps ImPlot::GetColormapCount()
 * \endif
 *
 * \if CHINESE
 * @brief 返回可用色彩映射的数量
 * @return 已注册色彩映射的数量
 * @details 封装 ImPlot::GetColormapCount()
 * \endif
 */
int QImPlotColormapManager::colormapCount()
{
    return ImPlot::GetColormapCount();
}

/**
 * \if ENGLISH
 * @brief Returns the name of the colormap at the given enum index
 * @param colormap Colormap enum value to query
 * @return Colormap name as UTF-8 QByteArray
 * @details Wraps ImPlot::GetColormapName(toImPlotColormap(colormap))
 * \endif
 *
 * \if CHINESE
 * @brief 返回给定枚举索引的色彩映射名称
 * @param colormap 要查询的色彩映射枚举值
 * @return 色彩映射名称，UTF-8 编码的 QByteArray
 * @details 封装 ImPlot::GetColormapName(toImPlotColormap(colormap))
 * \endif
 */
QByteArray QImPlotColormapManager::colormapName(QImPlotColormap colormap)
{
    const char* name = ImPlot::GetColormapName(toImPlotColormap(colormap));
    return QByteArray(name);
}

/**
 * \if ENGLISH
 * @brief Returns the colormap enum value for the given name
 * @param name Colormap name to look up (UTF-8 encoded)
 * @return Corresponding QImPlotColormap enum value
 * @details Wraps ImPlot::GetColormapIndex(name.constData()) then
 *          converts to QIm enum via toQImPlotColormap()
 * \endif
 *
 * \if CHINESE
 * @brief 返回给定名称对应的色彩映射枚举值
 * @param name 要查找的色彩映射名称（UTF-8 编码）
 * @return 对应的 QImPlotColormap 枚举值
 * @details 封装 ImPlot::GetColormapIndex(name.constData())，
 *          然后通过 toQImPlotColormap() 转换为 QIm 枚举
 * \endif
 */
QImPlotColormap QImPlotColormapManager::colormapIndex(const QByteArray& name)
{
    ImPlotColormap idx = ImPlot::GetColormapIndex(name.constData());
    return toQImPlotColormap(idx);
}

/**
 * \if ENGLISH
 * @brief Returns the number of colors in the specified colormap
 * @param colormap Colormap enum value to query
 * @return Number of discrete colors in the colormap
 * @details Wraps ImPlot::GetColormapSize(toImPlotColormap(colormap))
 * \endif
 *
 * \if CHINESE
 * @brief 返回指定色彩映射中的颜色数量
 * @param colormap 要查询的色彩映射枚举值
 * @return 色彩映射中离散颜色的数量
 * @details 封装 ImPlot::GetColormapSize(toImPlotColormap(colormap))
 * \endif
 */
int QImPlotColormapManager::colormapSize(QImPlotColormap colormap)
{
    return ImPlot::GetColormapSize(toImPlotColormap(colormap));
}

/**
 * \if ENGLISH
 * @brief Returns the color at the given index within the specified colormap
 * @param colormap Colormap enum value to query
 * @param index Color index within the colormap
 * @return QColor at the specified position
 * @details Wraps ImPlot::GetColormapColor(index, toImPlotColormap(colormap)),
 *          converts ImVec4 result to QColor via QtImGuiUtils::toQColor()
 * \endif
 *
 * \if CHINESE
 * @brief 返回指定色彩映射中给定索引处的颜色
 * @param colormap 要查询的色彩映射枚举值
 * @param index 色彩映射中的颜色索引
 * @return 指定位置的 QColor
 * @details 封装 ImPlot::GetColormapColor(index, toImPlotColormap(colormap))，
 *          通过 QtImGuiUtils::toQColor() 将 ImVec4 结果转换为 QColor
 * \endif
 */
QColor QImPlotColormapManager::colormapColor(QImPlotColormap colormap, int index)
{
    ImVec4 col = ImPlot::GetColormapColor(index, toImPlotColormap(colormap));
    return toQColor(col);
}

/**
 * \if ENGLISH
 * @brief Samples the colormap at the given t value (0.0 to 1.0)
 * @param colormap Colormap enum value to sample
 * @param t Sampling position in [0.0, 1.0] range
 * @return Interpolated QColor at the specified position
 * @details Wraps ImPlot::SampleColormap(static_cast<float>(t),
 *          toImPlotColormap(colormap)), converts ImVec4 result to QColor
 *          via QtImGuiUtils::toQColor()
 * \endif
 *
 * \if CHINESE
 * @brief 在给定 t 值（0.0 到 1.0）处采样色彩映射
 * @param colormap 要采样的色彩映射枚举值
 * @param t [0.0, 1.0] 范围内的采样位置
 * @return 指定位置的插值 QColor
 * @details 封装 ImPlot::SampleColormap(static_cast<float>(t),
 *          toImPlotColormap(colormap))，通过 QtImGuiUtils::toQColor()
 *          将 ImVec4 结果转换为 QColor
 * \endif
 */
QColor QImPlotColormapManager::sampleColormap(QImPlotColormap colormap, double t)
{
    ImVec4 col = ImPlot::SampleColormap(static_cast<float>(t), toImPlotColormap(colormap));
    return toQColor(col);
}

/**
 * \if ENGLISH
 * @brief Returns the next automatic colormap color
 * @return Next color in the automatic colormap cycle as QColor
 * @details Wraps ImPlot::NextColormapColor(), converts ImVec4 result
 *          to QColor via QtImGuiUtils::toQColor()
 * \endif
 *
 * \if CHINESE
 * @brief 返回下一个自动色彩映射颜色
 * @return 自动色彩映射循环中的下一个颜色，类型为 QColor
 * @details 封装 ImPlot::NextColormapColor()，通过 QtImGuiUtils::toQColor()
 *          将 ImVec4 结果转换为 QColor
 * \endif
 */
QColor QImPlotColormapManager::nextColormapColor()
{
    ImVec4 col = ImPlot::NextColormapColor();
    return toQColor(col);
}

//===============================================================
// Registration Methods
//===============================================================

/**
 * \if ENGLISH
 * @brief Registers a new colormap from a list of QColor values
 * @param name Colormap name (UTF-8 encoded QByteArray)
 * @param colors List of QColor values defining the colormap
 * @param qualitative Whether this is a qualitative (discrete) colormap (default false)
 * @details Converts each QColor to ImVec4 via QtImGuiUtils::toImVec4(),
 *          stores in a std::vector<ImVec4>, then calls
 *          ImPlot::AddColormap(name, vector.data(), vector.size(), qualitative)
 * \endif
 *
 * \if CHINESE
 * @brief 从 QColor 值列表注册新的色彩映射
 * @param name 色彩映射名称（UTF-8 编码的 QByteArray）
 * @param colors 定义色彩映射的 QColor 值列表
 * @param qualitative 是否为定性（离散）色彩映射（默认 false）
 * @details 通过 QtImGuiUtils::toImVec4() 将每个 QColor 转换为 ImVec4，
 *          存入 std::vector<ImVec4>，然后调用
 *          ImPlot::AddColormap(name, vector.data(), vector.size(), qualitative)
 * \endif
 */
void QImPlotColormapManager::addColormap(const QByteArray& name, const QList<QColor>& colors, bool qualitative)
{
    std::vector<ImVec4> imColors;
    imColors.reserve(colors.size());
    for (const QColor& c : colors) {
        imColors.push_back(toImVec4(c));
    }
    ImPlot::AddColormap(name.constData(), imColors.data(), static_cast<int>(imColors.size()), qualitative);
}

/**
 * \if ENGLISH
 * @brief Registers a new colormap from a list of quint32 color values
 * @param name Colormap name (UTF-8 encoded QByteArray)
 * @param colors List of packed RGBA quint32 values (ImU32-compatible format)
 * @param qualitative Whether this is a qualitative (discrete) colormap (default false)
 * @details Passes quint32 data directly to ImPlot::AddColormap since
 *          ImU32 is quint32-compatible
 * \endif
 *
 * \if CHINESE
 * @brief 从 quint32 颜色值列表注册新的色彩映射
 * @param name 色彩映射名称（UTF-8 编码的 QByteArray）
 * @param colors 打包 RGBA quint32 值列表（ImU32 兼容格式）
 * @param qualitative 是否为定性（离散）色彩映射（默认 false）
 * @details 直接将 quint32 数据传递给 ImPlot::AddColormap，
 *          因为 ImU32 与 quint32 兼容
 * \endif
 */
void QImPlotColormapManager::addColormap(const QByteArray& name, const QList<quint32>& colors, bool qualitative)
{
    // ImU32 is quint32-compatible, pass directly
    std::vector<ImU32> imColors;
    imColors.reserve(colors.size());
    for (quint32 c : colors) {
        imColors.push_back(static_cast<ImU32>(c));
    }
    ImPlot::AddColormap(name.constData(), imColors.data(), static_cast<int>(imColors.size()), qualitative);
}

}  // namespace QIM