#ifndef QIMPLOTCOLORMAPMANAGER_H
#define QIMPLOTCOLORMAPMANAGER_H

#include <QColor>
#include <QByteArray>
#include <QList>
#include "../QImAPI.h"
#include "QImPlot.h"

namespace QIM
{

/**
 * \if ENGLISH
 * @brief Static utility class providing ImPlot colormap management API
 * @details Wraps all ImPlot colormap query and registration functions:
 *          - Query: colormapCount, colormapName, colormapIndex, colormapSize,
 *            colormapColor, sampleColormap, nextColormapColor
 *          - Registration: addColormap (QColor list and quint32 list variants)
 *          Push/Pop colormap operations are on QImPlotNode, not here.
 * @note This is NOT a QObject — purely static utility class
 * @see QImPlotNode::pushColormap, QImPlotNode::popColormap
 * \endif
 *
 * \if CHINESE
 * @brief 静态工具类，提供 ImPlot 色彩映射管理 API
 * @details 封装所有 ImPlot 色彩映射查询和注册函数：
 *          - 查询：colormapCount、colormapName、colormapIndex、colormapSize、
 *            colormapColor、sampleColormap、nextColormapColor
 *          - 注册：addColormap（QColor 列表和 quint32 列表两种变体）
 *          Push/Pop 色彩映射操作在 QImPlotNode 上，不在此类中。
 * @note 此类不是 QObject — 纯静态工具类
 * @see QImPlotNode::pushColormap, QImPlotNode::popColormap
 * \endif
 */
class QIM_CORE_API QImPlotColormapManager
{
public:
    // Returns the number of available colormaps
    static int colormapCount();

    // Returns the name of the colormap at the given enum index
    static QByteArray colormapName(QImPlotColormap colormap);

    // Returns the colormap enum value for the given name
    static QImPlotColormap colormapIndex(const QByteArray& name);

    // Returns the number of colors in the specified colormap
    static int colormapSize(QImPlotColormap colormap);

    // Returns the color at the given index within the specified colormap
    static QColor colormapColor(QImPlotColormap colormap, int index);

    // Samples the colormap at the given t value (0.0 to 1.0)
    static QColor sampleColormap(QImPlotColormap colormap, double t);

    // Returns the next automatic colormap color
    static QColor nextColormapColor();

    // Registers a new colormap from a list of QColor values
    static void addColormap(const QByteArray& name, const QList<QColor>& colors, bool qualitative = false);

    // Registers a new colormap from a list of quint32 color values (packed RGBA)
    static void addColormap(const QByteArray& name, const QList<quint32>& colors, bool qualitative = false);

private:
    // Static-only class — no construction allowed
    QImPlotColormapManager() = delete;
    ~QImPlotColormapManager() = delete;
    QImPlotColormapManager(const QImPlotColormapManager&) = delete;
    QImPlotColormapManager& operator=(const QImPlotColormapManager&) = delete;
};

}  // namespace QIM

#endif  // QIMPLOTCOLORMAPMANAGER_H