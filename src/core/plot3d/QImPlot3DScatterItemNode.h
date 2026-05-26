#ifndef QIMPLOT3DSCATTERITEMNODE_H
#define QIMPLOT3DSCATTERITEMNODE_H

#include "QImPlot3DItemNode.h"
#include "QImPlot3DDataSeries.h"

namespace QIM
{

class QImAbstractXYZDataSeries;

/**
 * \if ENGLISH
 * @brief Qt-style wrapper for ImPlot3D scatter plot visualization
 *
 * @details Provides Qt-style retained mode encapsulation for ImPlot3D scatter plots.
 *          Supports marker customization (shape, size, fill color, outline color, outline weight)
 *          and full Qt property system integration with signal-slot event handling.
 *          Inherits from QImPlot3DItemNode and follows the same PIMPL design pattern.
 *          Marker fill/outline colors use QImOptional3DColor for deferred initialization,
 *          capturing ImPlot3D defaults on first render when unset.
 *
 * @note Scatter plots visualize discrete data points in 3D space as markers.
 *       Use QImPlot3DLineItemNode for continuous line plots.
 * @note Marker size and shape changes require redraw to take effect.
 *
 * @see QImPlot3DItemNode
 * @see QImAbstractXYZDataSeries
 * @see QImPlot3DNode
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot3D散点图可视化的Qt风格封装
 *
 * @details 为ImPlot3D散点图提供Qt风格的保留模式封装�?
 *          支持标记自定义（形状、大小、填充颜色、描边颜色、描边粗细）以及完整的Qt属性系统集成和信号槽事件处理�?
 *          继承自QImPlot3DItemNode，并遵循相同的PIMPL设计模式�?
 *          标记填充/描边颜色使用QImOptional3DColor进行延迟初始化，
 *          未设置时在首次渲染时捕获ImPlot3D默认颜色�?
 *
 * @note 散点图以标记形式可视�?D空间中的离散数据点�?
 *       连续线图请使用QImPlot3DLineItemNode�?
 * @note 标记大小和形状更改需要重绘才能生效�?
 *
 * @see QImPlot3DItemNode
 * @see QImAbstractXYZDataSeries
 * @see QImPlot3DNode
 * \endif
 */
class QIM_CORE_API QImPlot3DScatterItemNode : public QImPlot3DItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlot3DScatterItemNode)

    // Marker fill color (auto-resolved from ImPlot3D default if unset)
    Q_PROPERTY(QColor markerFillColor READ markerFillColor WRITE setMarkerFillColor NOTIFY markerFillColorChanged)

    // Marker outline color (auto-resolved from ImPlot3D default if unset)
    Q_PROPERTY(QColor markerOutlineColor READ markerOutlineColor WRITE setMarkerOutlineColor NOTIFY markerOutlineColorChanged)

    // Size of scatter plot markers in pixels
    Q_PROPERTY(float markerSize READ markerSize WRITE setMarkerSize NOTIFY markerSizeChanged)

    // Weight (thickness) of marker outline in pixels
    Q_PROPERTY(float markerWeight READ markerWeight WRITE setMarkerWeight NOTIFY markerWeightChanged)

    // Shape of scatter plot markers
    Q_PROPERTY(int markerShape READ markerShape WRITE setMarkerShape NOTIFY markerShapeChanged)

public:
    // Unique type identifier for QImPlot3DScatterItemNode
    enum
    {
        Type = InnerType3D + 1
    };

    virtual int type() const override
    {
        return Type;
    }

    // Constructs a QImPlot3DScatterItemNode with optional parent
    explicit QImPlot3DScatterItemNode(QObject* parent = nullptr);

    // Destructor
    ~QImPlot3DScatterItemNode() override;

    //----------------------------------------------------
    // Data setting interface
    //----------------------------------------------------

    // Sets the XYZ data series for the scatter plot
    void setData(QImAbstractXYZDataSeries* series);

    // Sets scatter plot data from X, Y, and Z containers
    template<typename CX, typename CY, typename CZ>
    QImAbstractXYZDataSeries* setData(const CX& x, const CY& y, const CZ& z);

    // Gets the current XYZ data series
    QImAbstractXYZDataSeries* data() const;

    //----------------------------------------------------
    // Style property accessors
    //----------------------------------------------------

    // Gets the marker fill color
    QColor markerFillColor() const;

    // Sets the marker fill color
    void setMarkerFillColor(const QColor& c);

    // Gets the marker outline color
    QColor markerOutlineColor() const;

    // Sets the marker outline color
    void setMarkerOutlineColor(const QColor& c);

    // Gets the marker size in pixels
    float markerSize() const;

    // Sets the marker size in pixels
    void setMarkerSize(float size);

    // Gets the marker outline weight (thickness) in pixels
    float markerWeight() const;

    // Sets the marker outline weight (thickness) in pixels
    void setMarkerWeight(float weight);

    // Gets the marker shape
    int markerShape() const;

    // Sets the marker shape
    void setMarkerShape(int shape);

    //----------------------------------------------------
    // ImPlot3DScatterFlags
    //----------------------------------------------------

    // Gets the raw ImPlot3DScatterFlags
    int scatterFlags() const;

    // Sets the raw ImPlot3DScatterFlags
    void setScatterFlags(int flags);

Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when marker fill color changes
     * @param color New marker fill color
     * \endif
     *
     * \if CHINESE
     * @brief 标记填充颜色更改时触�?
     * @param color 新标记填充颜�?
     * \endif
     */
    void markerFillColorChanged(const QColor& color);

    /**
     * \if ENGLISH
     * @brief Emitted when marker outline color changes
     * @param color New marker outline color
     * \endif
     *
     * \if CHINESE
     * @brief 标记描边颜色更改时触�?
     * @param color 新标记描边颜�?
     * \endif
     */
    void markerOutlineColorChanged(const QColor& color);

    /**
     * \if ENGLISH
     * @brief Emitted when marker size changes
     * @param size New marker size in pixels
     * \endif
     *
     * \if CHINESE
     * @brief 标记大小更改时触�?
     * @param size 新标记大小（像素�?
     * \endif
     */
    void markerSizeChanged(float size);

    /**
     * \if ENGLISH
     * @brief Emitted when marker weight changes
     * @param weight New marker outline weight in pixels
     * \endif
     *
     * \if CHINESE
     * @brief 标记描边粗细更改时触�?
     * @param weight 新标记描边粗细（像素�?
     * \endif
     */
    void markerWeightChanged(float weight);

    /**
     * \if ENGLISH
     * @brief Emitted when marker shape changes
     * @param shape New marker shape as QImPlot3DMarkerShape enum value
     * \endif
     *
     * \if CHINESE
     * @brief 标记形状更改时触�?
     * @param shape 新标记形状（QImPlot3DMarkerShape枚举值）
     * \endif
     */
    void markerShapeChanged(int shape);

    /**
     * \if ENGLISH
     * @brief Emitted when scatter plot flags change
     * \endif
     *
     * \if CHINESE
     * @brief 散点图标志更改时触发
     * \endif
     */
    void scatterFlagChanged();

    /**
     * \if ENGLISH
     * @brief Emitted when data series changes
     * \endif
     *
     * \if CHINESE
     * @brief 数据系列更改时触�?
     * \endif
     */
    void dataChanged();

protected:
    // Begins drawing the scatter plot
    virtual bool beginDraw() override;
};

// Template function implementation
template<typename CX, typename CY, typename CZ>
inline QImAbstractXYZDataSeries* QImPlot3DScatterItemNode::setData(const CX& x, const CY& y, const CZ& z)
{
    QImAbstractXYZDataSeries* s = new QImVectorXYZDataSeries<CX, CY, CZ>(x, y, z);
    setData(s);
    return s;
}

}  // end namespace QIM

#endif  // QIMPLOT3DSCATTERITEMNODE_H