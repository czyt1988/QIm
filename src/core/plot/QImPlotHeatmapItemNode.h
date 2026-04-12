#ifndef QIMPLOTHEATMAPITEMNODE_H
#define QIMPLOTHEATMAPITEMNODE_H

#include "QImAPI.h"
#include <QColor>
#include <QPointF>
#include "QImPlotItemNode.h"
#include "QImPlotHeatmapDataSeries.h"

namespace QIM
{
class QImAbstractHeatmapDataSeries;

/**
 * \if ENGLISH
 * @brief Qt-style wrapper for ImPlot heatmap visualization
 *
 * @class QImPlotHeatmapItemNode
 * @ingroup plot_items
 *
 * @details Provides Qt-style retained mode encapsulation for ImPlot heatmap charts.
 *          Supports 2D matrix data with configurable color scaling, bounds,
 *          label formatting, and column-major data layout.
 *          Inherits from QImPlotItemNode and follows the same PIMPL design pattern
 *          as QImPlotBarsItemNode for consistency.
 *
 * @note Heatmaps visualize 2D data as a color grid, useful for matrix data,
 *       correlation matrices, and 2D density plots.
 * @warning Large heatmaps (>1000x1000) may impact performance.
 *
 * @param[in] parent Parent QObject (optional)
 *
 * @see QImPlotItemNode
 * @see QImPlotBarsItemNode
 * @see QImPlotNode
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot热力图可视化的Qt风格封装
 *
 * @class QImPlotHeatmapItemNode
 * @ingroup plot_items
 *
 * @details 为ImPlot热力图提供Qt风格的保留模式封装。
 *          支持具有可配置颜色缩放、边界、标签格式和列主序数据布局的二维矩阵数据。
 *          继承自QImPlotItemNode，并遵循与QImPlotBarsItemNode相同的PIMPL设计模式以保持一致性。
 *
 * @note 热力图将二维数据可视化为颜色网格，适用于矩阵数据、相关矩阵和二维密度图。
 * @warning 大型热力图（>1000x1000）可能影响性能。
 *
 * @param[in] parent 父QObject对象（可选）
 *
 * @see QImPlotItemNode
 * @see QImPlotBarsItemNode
 * @see QImPlotNode
 * \endif
 */
class QIM_CORE_API QImPlotHeatmapItemNode : public QImPlotItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotHeatmapItemNode)

    /**
     * \if ENGLISH
     * @property QImPlotHeatmapItemNode::scaleMin
     * @brief Minimum value for color scaling
     *
     * @details Defines the lower bound of the color scale.
     *          If set to 0 together with scaleMax = 0 (default),
     *          automatic scaling based on data min/max is used.
     * @accessors READ scaleMin WRITE setScaleMin NOTIFY scaleMinChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotHeatmapItemNode::scaleMin
     * @brief 颜色缩放的最小值
     *
     * @details 定义颜色尺度的下限。
     *          如果与 scaleMax = 0 一起设置为 0（默认），
     *          则使用基于数据最小/最大值的自动缩放。
     * @accessors READ scaleMin WRITE setScaleMin NOTIFY scaleMinChanged
     * \endif
     */
    Q_PROPERTY(double scaleMin READ scaleMin WRITE setScaleMin NOTIFY scaleMinChanged)

    /**
     * \if ENGLISH
     * @property QImPlotHeatmapItemNode::scaleMax
     * @brief Maximum value for color scaling
     *
     * @details Defines the upper bound of the color scale.
     *          If set to 0 together with scaleMin = 0 (default),
     *          automatic scaling based on data min/max is used.
     * @accessors READ scaleMax WRITE setScaleMax NOTIFY scaleMaxChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotHeatmapItemNode::scaleMax
     * @brief 颜色缩放的最大值
     *
     * @details 定义颜色尺度的上限。
     *          如果与 scaleMin = 0 一起设置为 0（默认），
     *          则使用基于数据最小/最大值的自动缩放。
     * @accessors READ scaleMax WRITE setScaleMax NOTIFY scaleMaxChanged
     * \endif
     */
    Q_PROPERTY(double scaleMax READ scaleMax WRITE setScaleMax NOTIFY scaleMaxChanged)

    /**
     * \if ENGLISH
     * @property QImPlotHeatmapItemNode::labelFormat
     * @brief Format string for value labels
     *
     * @details Controls the display format of value labels on the heatmap.
     *          Uses standard printf-style formatting (e.g., "%.1f", "%.2e").
     *          Set to empty string to disable labels.
     *          Default is "%.1f".
     * @accessors READ labelFormat WRITE setLabelFormat NOTIFY labelFormatChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotHeatmapItemNode::labelFormat
     * @brief 值标签的格式字符串
     *
     * @details 控制热力图上值标签的显示格式。
     *          使用标准的 printf 风格格式化（例如 "%.1f"、"%.2e"）。
     *          设置为空字符串以禁用标签。
     *          默认为 "%.1f"。
     * @accessors READ labelFormat WRITE setLabelFormat NOTIFY labelFormatChanged
     * \endif
     */
    Q_PROPERTY(QString labelFormat READ labelFormat WRITE setLabelFormat NOTIFY labelFormatChanged)

    /**
     * \if ENGLISH
     * @property QImPlotHeatmapItemNode::boundsMin
     * @brief Lower-left corner position of heatmap in plot coordinates
     *
     * @details Defines the plot coordinates of the lower-left corner
     *          of the heatmap rectangle. Default is (0, 0).
     * @accessors READ boundsMin WRITE setBoundsMin NOTIFY boundsMinChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotHeatmapItemNode::boundsMin
     * @brief 热力图左下角在绘图坐标中的位置
     *
     * @details 定义热力图矩形左下角的绘图坐标。
     *          默认为 (0, 0)。
     * @accessors READ boundsMin WRITE setBoundsMin NOTIFY boundsMinChanged
     * \endif
     */
    Q_PROPERTY(QPointF boundsMin READ boundsMin WRITE setBoundsMin NOTIFY boundsMinChanged)

    /**
     * \if ENGLISH
     * @property QImPlotHeatmapItemNode::boundsMax
     * @brief Upper-right corner position of heatmap in plot coordinates
     *
     * @details Defines the plot coordinates of the upper-right corner
     *          of the heatmap rectangle. Default is (1, 1).
     * @accessors READ boundsMax WRITE setBoundsMax NOTIFY boundsMaxChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotHeatmapItemNode::boundsMax
     * @brief 热力图右上角在绘图坐标中的位置
     *
     * @details 定义热力图矩形右上角的绘图坐标。
     *          默认为 (1, 1)。
     * @accessors READ boundsMax WRITE setBoundsMax NOTIFY boundsMaxChanged
     * \endif
     */
    Q_PROPERTY(QPointF boundsMax READ boundsMax WRITE setBoundsMax NOTIFY boundsMaxChanged)

    /**
     * \if ENGLISH
     * @property QImPlotHeatmapItemNode::colMajor
     * @brief Column-major data layout flag
     *
     * @details When true, data is assumed to be stored in column-major order.
     *          When false, data is assumed to be row-major (default).
     * @accessors READ isColMajor WRITE setColMajor NOTIFY colMajorChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotHeatmapItemNode::colMajor
     * @brief 列主序数据布局标志
     *
     * @details 为 true 时，假定数据按列主序存储。
     *          为 false 时，假定数据按行主序存储（默认）。
     * @accessors READ isColMajor WRITE setColMajor NOTIFY colMajorChanged
     * \endif
     */
    Q_PROPERTY(bool colMajor READ isColMajor WRITE setColMajor NOTIFY colMajorChanged)

public:
    // Unique type identifier for QImPlotHeatmapItemNode
    enum
    {
        Type = InnerType + 10   // Ensure unique, adjust if needed
    };

    // Returns the type identifier of this plot item
    virtual int type() const override
    {
        return Type;
    }

    // Constructs a QImPlotHeatmapItemNode with optional parent
    QImPlotHeatmapItemNode(QObject* parent = nullptr);

    // Destroys the QImPlotHeatmapItemNode
    ~QImPlotHeatmapItemNode();

    //----------------------------------------------------
    // Data setting interface
    //----------------------------------------------------

    // Sets the data series for the heatmap
    void setData(QImAbstractHeatmapDataSeries* series);

    // Sets heatmap data from values matrix
    template< typename ContainerValues >
    QImAbstractHeatmapDataSeries* setData(const ContainerValues& values,
                                          int rows, int cols,
                                          bool colMajor = false);

    // Sets heatmap data from values matrix (move semantics)
    template< typename ContainerValues >
    QImAbstractHeatmapDataSeries* setData(ContainerValues&& values,
                                          int rows, int cols,
                                          bool colMajor = false);

    // Gets the current data series
    QImAbstractHeatmapDataSeries* data() const;

    //----------------------------------------------------
    // Style property accessors
    //----------------------------------------------------

    // Gets the minimum scale value
    double scaleMin() const;

    // Sets the minimum scale value
    void setScaleMin(double min);

    // Gets the maximum scale value
    double scaleMax() const;

    // Sets the maximum scale value
    void setScaleMax(double max);

    // Gets the label format string
    QString labelFormat() const;

    // Sets the label format string
    void setLabelFormat(const QString& format);

    // Gets the lower-left bounds
    QPointF boundsMin() const;

    // Sets the lower-left bounds
    void setBoundsMin(const QPointF& min);

    // Gets the upper-right bounds
    QPointF boundsMax() const;

    // Sets the upper-right bounds
    void setBoundsMax(const QPointF& max);

    // Checks if column-major layout is enabled
    bool isColMajor() const;

    // Sets column-major layout
    void setColMajor(bool colMajor);

    // Gets the raw ImPlotHeatmapFlags
    int heatmapFlags() const;

    // Sets the raw ImPlotHeatmapFlags
    void setHeatmapFlags(int flags);

Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when minimum scale value changes
     * @param[in] min New minimum scale value
     * @details Triggered by setScaleMin() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 最小缩放值更改时触发
     * @param[in] min 新的最小缩放值
     * @details 当值实际更改时由setScaleMin()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void scaleMinChanged(double min);

    /**
     * \if ENGLISH
     * @brief Emitted when maximum scale value changes
     * @param[in] max New maximum scale value
     * @details Triggered by setScaleMax() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 最大缩放值更改时触发
     * @param[in] max 新的最大缩放值
     * @details 当值实际更改时由setScaleMax()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void scaleMaxChanged(double max);

    /**
     * \if ENGLISH
     * @brief Emitted when label format changes
     * @param[in] format New label format string
     * @details Triggered by setLabelFormat() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 标签格式更改时触发
     * @param[in] format 新的标签格式字符串
     * @details 当值实际更改时由setLabelFormat()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void labelFormatChanged(const QString& format);

    /**
     * \if ENGLISH
     * @brief Emitted when lower-left bounds change
     * @param[in] min New lower-left bounds position
     * @details Triggered by setBoundsMin() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 左下角边界更改时触发
     * @param[in] min 新的左下角边界位置
     * @details 当值实际更改时由setBoundsMin()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void boundsMinChanged(const QPointF& min);

    /**
     * \if ENGLISH
     * @brief Emitted when upper-right bounds change
     * @param[in] max New upper-right bounds position
     * @details Triggered by setBoundsMax() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 右上角边界更改时触发
     * @param[in] max 新的右上角边界位置
     * @details 当值实际更改时由setBoundsMax()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void boundsMaxChanged(const QPointF& max);

    /**
     * \if ENGLISH
     * @brief Emitted when column-major layout flag changes
     * @param[in] colMajor New column-major state
     * @details Triggered by setColMajor() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 列主序布局标志更改时触发
     * @param[in] colMajor 新的列主序状态
     * @details 当值实际更改时由setColMajor()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void colMajorChanged(bool colMajor);

    /**
     * \if ENGLISH
     * @brief Emitted when data series changes
     * @details Triggered by setData() when new data is assigned.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 数据系列更改时触发
     * @details 当分配新数据时由setData()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void dataChanged();

    /**
     * \if ENGLISH
     * @brief Emitted when heatmap flags change
     * @details Triggered by any flag property change (colMajor, etc.).
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 热力图标志更改时触发
     * @details 任何标志属性更改（列主序等）时触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void heatmapFlagChanged();

protected:
    // Begins drawing the heatmap
    virtual bool beginDraw() override;
};

// Template function implementation
template< typename ContainerValues >
inline QImAbstractHeatmapDataSeries* QImPlotHeatmapItemNode::setData(const ContainerValues& values,
                                                                     int rows, int cols,
                                                                     bool colMajor)
{
    QImAbstractHeatmapDataSeries* d = new QImVectorHeatmapDataSeries<ContainerValues>(values, rows, cols, colMajor);
    setData(d);
    return d;
}

template< typename ContainerValues >
inline QImAbstractHeatmapDataSeries* QImPlotHeatmapItemNode::setData(ContainerValues&& values,
                                                                     int rows, int cols,
                                                                     bool colMajor)
{
    QImAbstractHeatmapDataSeries* d = new QImVectorHeatmapDataSeries<ContainerValues>(std::move(values), rows, cols, colMajor);
    setData(d);
    return d;
}

}  // end namespace QIM

#endif  // QIMPLOTHEATMAPITEMNODE_H