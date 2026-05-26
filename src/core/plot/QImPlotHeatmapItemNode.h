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
 * @details Provides Qt-style retained mode encapsulation for ImPlot heatmap charts.
 *          Supports 2D matrix data with configurable color scaling, bounds,
 *          label formatting, and column-major data layout.
 *          Inherits from QImPlotItemNode and follows the same PIMPL design pattern
 *          as QImPlotBarsItemNode for consistency.
 *
 * @note Heatmaps visualize 2D data as a color grid, useful for matrix data,
 *       correlation matrices, and 2D density plots.
 * @note Large heatmaps (>1000x1000) may impact performance.
 *
 * @see QImPlotItemNode
 * @see QImPlotBarsItemNode
 * @see QImPlotNode
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot热力图可视化的Qt风格封装。
 *
 * @details 为ImPlot热力图提供Qt风格的保留模式封装。
 *          支持具有可配置颜色缩放、边界、标签格式和列主序数据布局的二维矩阵数据。
 *          继承自QImPlotItemNode，并遵循与QImPlotBarsItemNode相同的PIMPL设计模式以保持一致性。
 *
 * @note 热力图将二维数据可视化为颜色网格，适用于矩阵数据、相关矩阵和二维密度图。
 * @note 大型热力图（>1000x1000）可能影响性能。
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

    Q_PROPERTY(double scaleMin READ scaleMin WRITE setScaleMin NOTIFY scaleMinChanged)

    Q_PROPERTY(double scaleMax READ scaleMax WRITE setScaleMax NOTIFY scaleMaxChanged)

    Q_PROPERTY(QString labelFormat READ labelFormat WRITE setLabelFormat NOTIFY labelFormatChanged)

    Q_PROPERTY(QPointF boundsMin READ boundsMin WRITE setBoundsMin NOTIFY boundsMinChanged)

    Q_PROPERTY(QPointF boundsMax READ boundsMax WRITE setBoundsMax NOTIFY boundsMaxChanged)

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
     * @brief 最小缩放值更改时触发。
     * @param[in] min 新的最小缩放。
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
     * @brief 最大缩放值更改时触发。
     * @param[in] max 新的最大缩放。
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
     * @brief 左下角边界更改时触发。
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
     * @brief 右上角边界更改时触发。
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
     * @brief 热力图标志更改时触发。
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