#ifndef QIMPLOTSCATTERITEMNODE_H
#define QIMPLOTSCATTERITEMNODE_H
#include "QImPlotItemNode.h"
#include "QImPlotDataSeries.h"
#include "QImDownsamplingController.h"

namespace QIM
{
class QImAbstractXYDataSeries;

/**
 * \if ENGLISH
 * @brief Qt-style wrapper for ImPlot scatter plot visualization
 *
 * @details Provides Qt-style retained mode encapsulation for ImPlot scatter plots.
 *          Supports marker customization (shape, size, fill), adaptive sampling,
 *          and full Qt property system integration with signal-slot event handling.
 *          Inherits from QImPlotItemNode and follows the same PIMPL design pattern
 *          as QImPlotLineItemNode for consistency.
 *
 * @note Scatter plots visualize discrete data points without connecting lines.
 *       Use QImPlotLineItemNode for continuous line plots.
 * @note Marker size and shape changes require redraw to take effect.
 *
 * @see QImPlotItemNode
 * @see QImPlotLineItemNode
 * @see QImPlotNode
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot散点图可视化的Qt风格封装。
 *
 * @details 为ImPlot散点图提供Qt风格的保留模式封装。
 *          支持标记自定义（形状、大小、填充）、自适应采样。
 *          以及完整的Qt属性系统集成和信号槽事件处理。
 *          继承自QImPlotItemNode，并遵循与QImPlotLineItemNode相同的PIMPL设计模式以保持一致性。
 *
 * @note 散点图可视化离散数据点而不连接线。
 *       连续线图请使用QImPlotLineItemNode。
 * @note 标记大小和形状更改需要重绘才能生效。
 *
 *
 * @see QImPlotItemNode
 * @see QImPlotLineItemNode
 * @see QImPlotNode
 * \endif
 */
class QIM_CORE_API QImPlotScatterItemNode : public QImPlotItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotScatterItemNode)

    // Size of scatter plot markers in pixels (READ markerSize WRITE setMarkerSize NOTIFY markerSizeChanged)
    Q_PROPERTY(float markerSize READ markerSize WRITE setMarkerSize NOTIFY markerSizeChanged)

    // Shape of scatter plot markers, corresponds to ImPlotMarker enum (READ markerShape WRITE setMarkerShape NOTIFY markerShapeChanged)
    Q_PROPERTY(int markerShape READ markerShape WRITE setMarkerShape NOTIFY markerShapeChanged)

    // Fill style for markers, true=filled false=outline only (READ isMarkerFill WRITE setMarkerFill NOTIFY markerFillChanged)
    Q_PROPERTY(bool markerFill READ isMarkerFill WRITE setMarkerFill NOTIFY markerFillChanged)

    // Downsampling algorithm selection (READ downsampleAlgorithm WRITE setDownsampleAlgorithm NOTIFY downsampleAlgorithmChanged)
    Q_PROPERTY(QImDownsampleAlgorithm downsampleAlgorithm READ downsampleAlgorithm WRITE setDownsampleAlgorithm NOTIFY downsampleAlgorithmChanged)

    // Dataset size threshold for triggering downsampling (READ downsampleThreshold WRITE setDownsampleThreshold NOTIFY downsampleThresholdChanged)
    Q_PROPERTY(int downsampleThreshold READ downsampleThreshold WRITE setDownsampleThreshold NOTIFY downsampleThresholdChanged)

    // Color of scatter plot markers (READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

    // Enable/disable clipping of markers at plot edges, corresponds to negated ImPlotScatterFlags_NoClip (READ isClippingEnabled WRITE setClippingEnabled NOTIFY scatterFlagChanged)
    Q_PROPERTY(bool clippingEnabled READ isClippingEnabled WRITE setClippingEnabled NOTIFY scatterFlagChanged)

public:
    // Unique type identifier for QImPlotScatterItemNode
    enum
    {
        Type = InnerType + 2
    };

    // Returns the type identifier of this plot item
    virtual int type() const override
    {
        return Type;
    }

    // Constructs a QImPlotScatterItemNode with optional parent
    QImPlotScatterItemNode(QObject* parent = nullptr);

    // Destroys the QImPlotScatterItemNode
    ~QImPlotScatterItemNode();

    //----------------------------------------------------
    // Data setting interface
    //----------------------------------------------------

    // Sets the data series for the scatter plot
    void setData(QImAbstractXYDataSeries* series);

    // Sets scatter plot data from X and Y containers
    template< typename ContainerX, typename ContainerY >
    QImAbstractXYDataSeries* setData(const ContainerX& x, const ContainerY& y);

    // Sets scatter plot data from X and Y containers (move semantics)
    template< typename ContainerX, typename ContainerY >
    QImAbstractXYDataSeries* setData(ContainerX&& x, ContainerY&& y);

    // Gets the current data series
    QImAbstractXYDataSeries* data() const;

    //----------------------------------------------------
    // Style property accessors
    //----------------------------------------------------

    // Gets the marker size in pixels
    float markerSize() const;

    // Sets the marker size in pixels
    void setMarkerSize(float size);

    // Gets the marker shape
    int markerShape() const;

    // Sets the marker shape
    void setMarkerShape(int shape);

    // Checks if markers are filled
    bool isMarkerFill() const;

    // Sets marker fill state
    void setMarkerFill(bool fill);

    // Gets the downsampling algorithm
    QImDownsampleAlgorithm downsampleAlgorithm() const;

    // Sets the downsampling algorithm
    void setDownsampleAlgorithm(QImDownsampleAlgorithm algo);

    // Gets the downsample threshold
    int downsampleThreshold() const;

    // Sets the downsample threshold
    void setDownsampleThreshold(int threshold);

    // Gets the marker color
    QColor color() const;

    // Sets the marker color
    void setColor(const QColor& color);

    //----------------------------------------------------
    // ImPlotScatterFlags
    //----------------------------------------------------

    // Checks if clipping is enabled (corresponds to !ImPlotScatterFlags_NoClip)
    bool isClippingEnabled() const;

    // Sets clipping enabled state (true = clip markers, false = NoClip)
    void setClippingEnabled(bool enabled);

    // Gets the raw ImPlotScatterFlags
    int scatterFlags() const;

    // Sets the raw ImPlotScatterFlags
    void setScatterFlags(int flags);

Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when marker size changes
     * @param[in] size New marker size in pixels
     * @details Triggered by setMarkerSize() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 标记大小更改时触发
     * @param[in] size 新标记大小（像素）
     * @details 当值实际更改时由setMarkerSize()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void markerSizeChanged(float size);

    /**
     * \if ENGLISH
     * @brief Emitted when marker shape changes
     * @param[in] shape New marker shape as ImPlotMarker enum value
     * @details Triggered by setMarkerShape() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 标记形状更改时触发
     * @param[in] shape 新标记形状（ImPlotMarker枚举值）
     * @details 当值实际更改时由setMarkerShape()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void markerShapeChanged(int shape);

    /**
     * \if ENGLISH
     * @brief Emitted when marker fill state changes
     * @param[in] fill New marker fill state (true = filled, false = outline)
     * @details Triggered by setMarkerFill() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 标记填充状态更改时触发。
     * @param[in] fill 新标记填充状态（true = 填充，false = 轮廓。
     * @details 当值实际更改时由setMarkerFill()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void markerFillChanged(bool fill);

    /**
     * \if ENGLISH
     * @brief Emitted when downsample algorithm changes
     * @param[in] algo The new algorithm value
     * \endif
     *
     * \if CHINESE
     * @brief 降采样算法变更时发射
     * @param[in] algo 新的算法值
     * \endif
     */
    void downsampleAlgorithmChanged(QImDownsampleAlgorithm algo);

    /**
     * \if ENGLISH
     * @brief Emitted when downsample threshold changes
     * @param[in] threshold New downsample threshold in data points
     * @details Triggered by setDownsampleThreshold() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 降采样阈值更改时触发。
     * @param[in] threshold 新降采样阈值（数据点）
     * @details 当值实际更改时由setDownsampleThreshold()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void downsampleThresholdChanged(int threshold);

    /**
     * \if ENGLISH
     * @brief Emitted when marker color changes
     * @param[in] color New marker color
     * @details Triggered by setColor() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 标记颜色更改时触发
     * @param[in] color 新标记颜色
     * @details 当值实际更改时由setColor()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void colorChanged(const QColor& color);

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
     * @brief Emitted when scatter plot flags change
     * @details Triggered by any flag property change (marker fill, etc.).
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 散点图标志更改时触发。
     * @details 任何标志属性更改（标记填充等）时触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void scatterFlagChanged();

protected:
    // Begins drawing the scatter plot
    virtual bool beginDraw() override;
};

// Template function implementations
template< typename ContainerX, typename ContainerY >
inline QImAbstractXYDataSeries* QImPlotScatterItemNode::setData(const ContainerX& x, const ContainerY& y)
{
    QImAbstractXYDataSeries* d = new QImVectorXYDataSeries(x, y);
    setData(d);
    return d;
}

template< typename ContainerX, typename ContainerY >
inline QImAbstractXYDataSeries* QImPlotScatterItemNode::setData(ContainerX&& x, ContainerY&& y)
{
    QImAbstractXYDataSeries* d = new QImVectorXYDataSeries(x, y);
    setData(d);
    return d;
}

}  // end namespace QIM

#endif  // QIMPLOTSCATTERITEMNODE_H