#ifndef QIMPLOTSCATTERITEMNODE_H
#define QIMPLOTSCATTERITEMNODE_H
#include "QImPlotItemNode.h"
#include "QImPlotDataSeries.h"

namespace QIM
{
class QImAbstractXYDataSeries;

/**
 * \if ENGLISH
 * @brief Qt-style wrapper for ImPlot scatter plot visualization
 *
 * @class QImPlotScatterItemNode
 * @ingroup plot_items
 *
 * @details Provides Qt-style retained mode encapsulation for ImPlot scatter plots.
 *          Supports marker customization (shape, size, fill), adaptive sampling,
 *          and full Qt property system integration with signal-slot event handling.
 *          Inherits from QImPlotItemNode and follows the same PIMPL design pattern
 *          as QImPlotLineItemNode for consistency.
 *
 * @note Scatter plots visualize discrete data points without connecting lines.
 *       Use QImPlotLineItemNode for continuous line plots.
 * @warning Marker size and shape changes require redraw to take effect.
 *
 * @param[in] parent Parent QObject (optional)
 *
 * @see QImPlotItemNode
 * @see QImPlotLineItemNode
 * @see QImPlotNode
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot散点图可视化的Qt风格封装
 *
 * @class QImPlotScatterItemNode
 * @ingroup plot_items
 *
 * @details 为ImPlot散点图提供Qt风格的保留模式封装。
 *          支持标记自定义（形状、大小、填充）、自适应采样，
 *          以及完整的Qt属性系统集成和信号槽事件处理。
 *          继承自QImPlotItemNode，并遵循与QImPlotLineItemNode相同的PIMPL设计模式以保持一致性。
 *
 * @note 散点图可视化离散数据点而不连接线。
 *       连续线图请使用QImPlotLineItemNode。
 * @warning 标记大小和形状更改需要重绘才能生效。
 *
 * @param[in] parent 父QObject对象（可选）
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

    /**
     * \if ENGLISH
     * @property QImPlotScatterItemNode::markerSize
     * @brief Size of scatter plot markers in pixels
     *
     * @details Controls the visual size of markers in the scatter plot.
     *          Value is in pixels and affects all markers uniformly.
     *          Default value is 4.0 pixels.
     * @accessors READ markerSize WRITE setMarkerSize NOTIFY markerSizeChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotScatterItemNode::markerSize
     * @brief 散点图标记的大小（像素）
     *
     * @details 控制散点图中标记的视觉大小。
     *          值为像素，统一影响所有标记。
     *          默认值为4.0像素。
     * @accessors READ markerSize WRITE setMarkerSize NOTIFY markerSizeChanged
     * \endif
     */
    Q_PROPERTY(float markerSize READ markerSize WRITE setMarkerSize NOTIFY markerSizeChanged)

    /**
     * \if ENGLISH
     * @property QImPlotScatterItemNode::markerShape
     * @brief Shape of scatter plot markers
     *
     * @details Defines the geometric shape used for scatter plot markers.
     *          Supported shapes include circle, square, diamond, triangles, etc.
     *          Value corresponds to ImPlotMarker enumeration.
     *          Default shape is ImPlotMarker_Circle.
     * @accessors READ markerShape WRITE setMarkerShape NOTIFY markerShapeChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotScatterItemNode::markerShape
     * @brief 散点图标记的形状
     *
     * @details 定义散点图标记使用的几何形状。
     *          支持的形状包括圆形、正方形、菱形、三角形等。
     *          值对应ImPlotMarker枚举。
     *          默认形状为ImPlotMarker_Circle。
     * @accessors READ markerShape WRITE setMarkerShape NOTIFY markerShapeChanged
     * \endif
     */
    Q_PROPERTY(int markerShape READ markerShape WRITE setMarkerShape NOTIFY markerShapeChanged)

    /**
     * \if ENGLISH
     * @property QImPlotScatterItemNode::markerFill
     * @brief Fill style for scatter plot markers
     *
     * @details Controls whether markers are filled or outlined only.
     *          When true, markers are filled with color.
     *          When false, only marker outlines are drawn.
     *          Default value is true (filled markers).
     * @accessors READ isMarkerFill WRITE setMarkerFill NOTIFY markerFillChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotScatterItemNode::markerFill
     * @brief 散点图标记的填充样式
     *
     * @details 控制标记是填充还是仅轮廓。
     *          为true时，标记用颜色填充。
     *          为false时，仅绘制标记轮廓。
     *          默认值为true（填充标记）。
     * @accessors READ isMarkerFill WRITE setMarkerFill NOTIFY markerFillChanged
     * \endif
     */
    Q_PROPERTY(bool markerFill READ isMarkerFill WRITE setMarkerFill NOTIFY markerFillChanged)

    /**
     * \if ENGLISH
     * @property QImPlotScatterItemNode::adaptiveSampling
     * @brief Enable/disable adaptive sampling for large datasets
     *
     * @details When enabled, applies downsampling algorithms (LTTB/MinMaxLTTB)
     *          to reduce rendering load for datasets exceeding downsampleThreshold.
     *          Preserves visual characteristics while improving performance.
     *          Default value is true (adaptive sampling enabled).
     * @accessors READ isAdaptiveSampling WRITE setAdaptiveSampling NOTIFY adaptiveSamplingChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotScatterItemNode::adaptiveSampling
     * @brief 启用/禁用大数据集的自适应采样
     *
     * @details 启用时，对超过downsampleThreshold的数据集应用降采样算法（LTTB/MinMaxLTTB），
     *          以减少渲染负载。在提高性能的同时保留视觉特征。
     *          默认值为true（启用自适应采样）。
     * @accessors READ isAdaptiveSampling WRITE setAdaptiveSampling NOTIFY adaptiveSamplingChanged
     * \endif
     */
    Q_PROPERTY(bool adaptiveSampling READ isAdaptiveSampling WRITE setAdaptiveSampling NOTIFY adaptiveSamplingChanged)

    /**
     * \if ENGLISH
     * @property QImPlotScatterItemNode::downsampleThreshold
     * @brief Dataset size threshold for triggering adaptive sampling
     *
     * @details When dataset size exceeds this value and adaptive sampling is enabled,
     *          downsampling algorithms are applied to reduce data points.
     *          Value represents number of data points.
     *          Default value is 20000 points.
     * @accessors READ downsampleThreshold WRITE setDownsampleThreshold NOTIFY downsampleThresholdChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotScatterItemNode::downsampleThreshold
     * @brief 触发自适应采样的数据集大小阈值
     *
     * @details 当数据集大小超过此值且自适应采样启用时，
     *          应用降采样算法以减少数据点。
     *          值表示数据点数量。
     *          默认值为20000点。
     * @accessors READ downsampleThreshold WRITE setDownsampleThreshold NOTIFY downsampleThresholdChanged
     * \endif
     */
    Q_PROPERTY(int downsampleThreshold READ downsampleThreshold WRITE setDownsampleThreshold NOTIFY downsampleThresholdChanged)

    /**
     * \if ENGLISH
     * @property QImPlotScatterItemNode::color
     * @brief Color of scatter plot markers
     *
     * @details Defines the primary color for scatter plot markers.
     *          Affects both fill and outline colors based on markerFill setting.
     *          When not set, uses ImPlot's default color sequence.
     * @accessors READ color WRITE setColor NOTIFY colorChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotScatterItemNode::color
     * @brief 散点图标记的颜色
     *
     * @details 定义散点图标记的主要颜色。
     *          根据markerFill设置影响填充和轮廓颜色。
     *          未设置时，使用ImPlot的默认颜色序列。
     * @accessors READ color WRITE setColor NOTIFY colorChanged
     * \endif
     */
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

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
    // 数据设置接口
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
    // 样式属性访问器
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

    // Checks if adaptive sampling is enabled
    bool isAdaptiveSampling() const;

    // Enables or disables adaptive sampling
    void setAdaptiveSampling(bool enabled);

    // Gets the downsample threshold
    int downsampleThreshold() const;

    // Sets the downsample threshold
    void setDownsampleThreshold(int threshold);

    // Gets the marker color
    QColor color() const;

    // Sets the marker color
    void setColor(const QColor& color);

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
     * @brief 标记填充状态更改时触发
     * @param[in] fill 新标记填充状态（true = 填充，false = 轮廓）
     * @details 当值实际更改时由setMarkerFill()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void markerFillChanged(bool fill);

    /**
     * \if ENGLISH
     * @brief Emitted when adaptive sampling state changes
     * @param[in] enabled New adaptive sampling state
     * @details Triggered by setAdaptiveSampling() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 自适应采样状态更改时触发
     * @param[in] enabled 新自适应采样状态
     * @details 当值实际更改时由setAdaptiveSampling()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void adaptiveSamplingChanged(bool enabled);

    /**
     * \if ENGLISH
     * @brief Emitted when downsample threshold changes
     * @param[in] threshold New downsample threshold in data points
     * @details Triggered by setDownsampleThreshold() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 降采样阈值更改时触发
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
     * @brief 散点图标志更改时触发
     * @details 任何标志属性更改（标记填充等）时触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void scatterFlagChanged();

protected:
    // Begins drawing the scatter plot
    virtual bool beginDraw() override;
};

// 模板函数实现
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
    QImAbstractXYDataSeries* d = new QImVectorXYDataSeries(std::move(x), std::move(y));
    setData(d);
    return d;
}

}  // end namespace QIM

#endif  // QIMPLOTSCATTERITEMNODE_H