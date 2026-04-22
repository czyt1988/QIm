#ifndef QIMPLOT3DCOLORMAPMANAGER_H
#define QIMPLOT3DCOLORMAPMANAGER_H

#include <QColor>
#include <QByteArray>
#include <QList>
#include "QImAPI.h"
#include "QImPlot3D.h"

namespace QIM
{

/**
 * \if ENGLISH
 * @brief Static utility class providing ImPlot3D colormap management API
 * @details Wraps all ImPlot3D colormap query and registration functions:
 *          - Query: colormapCount, colormapName, colormapIndex, colormapSize,
 *            colormapColor, sampleColormap, nextColormapColor
 *          - Registration: addColormap (QColor list and quint32 list variants)
 *          Push/Pop colormap operations are on QImPlot3DNode, not here.
 * @note This is NOT a QObject — purely static utility class
 * @see QImPlot3DNode::pushColormap, QImPlot3DNode::popColormap
 * \endif
 *
 * \if CHINESE
 * @brief 静态工具类，提供 ImPlot3D 色彩映射管理 API
 * @details 封装所有 ImPlot3D 色彩映射查询和注册函数：
 *          - 查询：colormapCount、colormapName、colormapIndex、colormapSize、
 *            colormapColor、sampleColormap、nextColormapColor
 *          - 注册：addColormap（QColor 列表和 quint32 列表两种变体）
 *          Push/Pop 色彩映射操作在 QImPlot3DNode 上，不在此类中。
 * @note 此类不是 QObject — 纯静态工具类
 * @see QImPlot3DNode::pushColormap, QImPlot3DNode::popColormap
 * \endif
 */
class QIM_CORE_API QImPlot3DColormapManager
{
public:
    // Returns the number of available colormaps
    static int colormapCount();

    // Returns the name of the colormap at the given enum index
    static QByteArray colormapName(QImPlot3DColormap colormap);

    // Returns the colormap enum value for the given name
    static QImPlot3DColormap colormapIndex(const QByteArray& name);

    // Returns the number of colors in the specified colormap
    static int colormapSize(QImPlot3DColormap colormap);

    // Returns the color at the given index within the specified colormap
    static QColor colormapColor(QImPlot3DColormap colormap, int index);

    // Samples the colormap at the given t value (0.0 to 1.0)
    static QColor sampleColormap(QImPlot3DColormap colormap, double t);

    // Returns the next automatic colormap color
    static QColor nextColormapColor();

    // Registers a new colormap from a list of QColor values
    static void addColormap(const QByteArray& name, const QList<QColor>& colors, bool qualitative = false);

    // Registers a new colormap from a list of quint32 color values (packed RGBA)
    static void addColormap(const QByteArray& name, const QList<quint32>& colors, bool qualitative = false);

private:
    // Static-only class — no construction allowed
    QImPlot3DColormapManager() = delete;
    ~QImPlot3DColormapManager() = delete;
    QImPlot3DColormapManager(const QImPlot3DColormapManager&) = delete;
    QImPlot3DColormapManager& operator=(const QImPlot3DColormapManager&) = delete;
};

}  // namespace QIM

#endif  // QIMPLOT3DCOLORMAPMANAGER_H