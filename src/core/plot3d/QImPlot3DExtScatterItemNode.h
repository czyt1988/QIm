#ifndef QIMPLOT3DEXTSCATTERITEMNODE_H
#define QIMPLOT3DEXTSCATTERITEMNODE_H

#include "QImPlot3DExtItemNode.h"
#include "QImPlot3DDataSeries.h"

namespace QIM
{

class QImAbstractXYZDataSeries;

/**
 * \if ENGLISH
 * @brief Qt-style wrapper for ImPlot3D scatter plot visualization
 *
 * @class QImPlot3DExtScatterItemNode
 * @ingroup plot3d_items
 *
 * @details Provides Qt-style retained mode encapsulation for ImPlot3D scatter plots.
 *          Supports marker customization (shape, size, color) and full Qt property
 *          system integration with signal-slot event handling.
 *          Inherits from QImPlot3DExtItemNode and follows the same PIMPL design pattern.
 *
 * @note Scatter plots visualize discrete data points in 3D space as markers.
 *       Use QImPlot3DExtLineItemNode for continuous line plots.
 * @warning Marker size and shape changes require redraw to take effect.
 *
 * @param[in] parent Parent QObject (optional, typically a QImPlot3DExtNode)
 *
 * @see QImPlot3DExtItemNode
 * @see QImAbstractXYZDataSeries
 * @see QImPlot3DExtNode
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot3D散点图可视化的Qt风格封装
 *
 * @class QImPlot3DExtScatterItemNode
 * @ingroup plot3d_items
 *
 * @details 为ImPlot3D散点图提供Qt风格的保留模式封装。
 *          支持标记自定义（形状、大小、颜色）以及完整的Qt属性系统集成和信号槽事件处理。
 *          继承自QImPlot3DExtItemNode，并遵循相同的PIMPL设计模式。
 *
 * @note 散点图以标记形式可视化3D空间中的离散数据点。
 *       连续线图请使用QImPlot3DExtLineItemNode。
 * @warning 标记大小和形状更改需要重绘才能生效。
 *
 * @param[in] parent 父QObject对象（可选，通常是QImPlot3DExtNode）
 *
 * @see QImPlot3DExtItemNode
 * @see QImAbstractXYZDataSeries
 * @see QImPlot3DExtNode
 * \endif
 */
class QIM_CORE_API QImPlot3DExtScatterItemNode : public QImPlot3DExtItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlot3DExtScatterItemNode)

    /**
     * \if ENGLISH
     * @property QImPlot3DExtScatterItemNode::color
     * @brief Color of scatter plot markers
     *
     * @details Defines the primary color for scatter plot markers.
     *          When not set, uses ImPlot3D's default color sequence.
     * @accessors READ color WRITE setColor NOTIFY colorChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlot3DExtScatterItemNode::color
     * @brief 散点图标记的颜色
     *
     * @details 定义散点图标记的主要颜色。
     *          未设置时，使用ImPlot3D的默认颜色序列。
     * @accessors READ color WRITE setColor NOTIFY colorChanged
     * \endif
     */
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

    /**
     * \if ENGLISH
     * @property QImPlot3DExtScatterItemNode::markerSize
     * @brief Size of scatter plot markers in pixels
     *
     * @details Controls the visual size of markers in the scatter plot.
     *          Value is in pixels and affects all markers uniformly.
     *          Default value is IMPLOT3D_AUTO (auto-sized).
     * @accessors READ markerSize WRITE setMarkerSize NOTIFY markerSizeChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlot3DExtScatterItemNode::markerSize
     * @brief 散点图标记的大小（像素）
     *
     * @details 控制散点图中标记的视觉大小。
     *          值为像素，统一影响所有标记。
     *          默认值为IMPLOT3D_AUTO（自动大小）。
     * @accessors READ markerSize WRITE setMarkerSize NOTIFY markerSizeChanged
     * \endif
     */
    Q_PROPERTY(float markerSize READ markerSize WRITE setMarkerSize NOTIFY markerSizeChanged)

    /**
     * \if ENGLISH
     * @property QImPlot3DExtScatterItemNode::markerShape
     * @brief Shape of scatter plot markers
     *
     * @details Defines the geometric shape used for scatter plot markers.
     *          Supported shapes include circle, square, diamond, triangles, etc.
     *          Value corresponds to QImPlot3DMarkerShape enumeration.
     *          Default shape is QImPlot3DMarkerShape::Circle.
     * @accessors READ markerShape WRITE setMarkerShape NOTIFY markerShapeChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlot3DExtScatterItemNode::markerShape
     * @brief 散点图标记的形状
     *
     * @details 定义散点图标记使用的几何形状。
     *          支持的形状包括圆形、正方形、菱形、三角形等。
     *          值对应QImPlot3DMarkerShape枚举。
     *          默认形状为QImPlot3DMarkerShape::Circle。
     * @accessors READ markerShape WRITE setMarkerShape NOTIFY markerShapeChanged
     * \endif
     */
    Q_PROPERTY(int markerShape READ markerShape WRITE setMarkerShape NOTIFY markerShapeChanged)

public:
    /**
     * \if ENGLISH
     * @brief Unique type identifier for QImPlot3DExtScatterItemNode
     * @details InnerType3D + 1 distinguishes this from other 3D item types.
     * \endif
     *
     * \if CHINESE
     * @brief QImPlot3DExtScatterItemNode的唯一类型标识符
     * @details InnerType3D + 1 用于与其他3D元素类型区分。
     * \endif
     */
    enum
    {
        Type = InnerType3D + 1
    };

    /**
     * \if ENGLISH
     * @brief Returns the type identifier of this plot item
     * @return Type value (InnerType3D + 1)
     * \endif
     *
     * \if CHINESE
     * @brief 返回此绘图元素的类型标识符
     * @return 类型值（InnerType3D + 1）
     * \endif
     */
    virtual int type() const override
    {
        return Type;
    }

    /**
     * \if ENGLISH
     * @brief Constructs a QImPlot3DExtScatterItemNode with optional parent
     * @param parent Parent QObject (typically a QImPlot3DExtNode)
     * \endif
     *
     * \if CHINESE
     * @brief 构造QImPlot3DExtScatterItemNode
     * @param parent 父QObject对象（通常是QImPlot3DExtNode）
     * \endif
     */
    explicit QImPlot3DExtScatterItemNode(QObject* parent = nullptr);

    /**
     * \if ENGLISH
     * @brief Destructor
     * \endif
     *
     * \if CHINESE
     * @brief 析构函数
     * \endif
     */
    ~QImPlot3DExtScatterItemNode() override;

    //----------------------------------------------------
    // Data setting interface
    //----------------------------------------------------

    /**
     * \if ENGLISH
     * @brief Sets the XYZ data series for the scatter plot
     * @param series Pointer to QImAbstractXYZDataSeries (ownership transferred)
     * \endif
     *
     * \if CHINESE
     * @brief 设置散点图的XYZ数据系列
     * @param series QImAbstractXYZDataSeries指针（所有权转移）
     * \endif
     */
    void setData(QImAbstractXYZDataSeries* series);

    /**
     * \if ENGLISH
     * @brief Sets scatter plot data from X, Y, and Z containers
     * @tparam CX Container type for X coordinates (must store double)
     * @tparam CY Container type for Y coordinates (must store double)
     * @tparam CZ Container type for Z coordinates (must store double)
     * @param x X coordinate container
     * @param y Y coordinate container
     * @param z Z coordinate container
     * @return Pointer to the created data series
     * \endif
     *
     * \if CHINESE
     * @brief 从X、Y、Z容器设置散点图数据
     * @tparam CX X坐标容器类型（必须存储double）
     * @tparam CY Y坐标容器类型（必须存储double）
     * @tparam CZ Z坐标容器类型（必须存储double）
     * @param x X坐标容器
     * @param y Y坐标容器
     * @param z Z坐标容器
     * @return 创建的数据系列指针
     * \endif
     */
    template<typename CX, typename CY, typename CZ>
    QImAbstractXYZDataSeries* setData(const CX& x, const CY& y, const CZ& z);

    /**
     * \if ENGLISH
     * @brief Gets the current XYZ data series
     * @return Pointer to the data series, or nullptr if not set
     * \endif
     *
     * \if CHINESE
     * @brief 获取当前XYZ数据系列
     * @return 数据系列指针，若未设置则返回nullptr
     * \endif
     */
    QImAbstractXYZDataSeries* data() const;

    //----------------------------------------------------
    // Style property accessors
    //----------------------------------------------------

    /**
     * \if ENGLISH
     * @brief Gets the marker color
     * @return Current marker color as QColor
     * \endif
     *
     * \if CHINESE
     * @brief 获取标记颜色
     * @return 当前标记颜色（QColor）
     * \endif
     */
    QColor color() const;

    /**
     * \if ENGLISH
     * @brief Sets the marker color
     * @param c New marker color
     * \endif
     *
     * \if CHINESE
     * @brief 设置标记颜色
     * @param c 新标记颜色
     * \endif
     */
    void setColor(const QColor& c);

    /**
     * \if ENGLISH
     * @brief Gets the marker size in pixels
     * @return Current marker size, IMPLOT3D_AUTO (-1) means auto-sized
     * \endif
     *
     * \if CHINESE
     * @brief 获取标记大小（像素）
     * @return 当前标记大小，IMPLOT3D_AUTO (-1) 表示自动大小
     * \endif
     */
    float markerSize() const;

    /**
     * \if ENGLISH
     * @brief Sets the marker size in pixels
     * @param size New marker size, use IMPLOT3D_AUTO (-1) for auto-size
     * \endif
     *
     * \if CHINESE
     * @brief 设置标记大小（像素）
     * @param size 新标记大小，使用IMPLOT3D_AUTO (-1) 表示自动大小
     * \endif
     */
    void setMarkerSize(float size);

    /**
     * \if ENGLISH
     * @brief Gets the marker shape
     * @return Current marker shape as int (corresponds to QImPlot3DMarkerShape)
     * \endif
     *
     * \if CHINESE
     * @brief 获取标记形状
     * @return 当前标记形状（int，对应QImPlot3DMarkerShape）
     * \endif
     */
    int markerShape() const;

    /**
     * \if ENGLISH
     * @brief Sets the marker shape
     * @param shape New marker shape (QImPlot3DMarkerShape enum value)
     * \endif
     *
     * \if CHINESE
     * @brief 设置标记形状
     * @param shape 新标记形状（QImPlot3DMarkerShape枚举值）
     * \endif
     */
    void setMarkerShape(int shape);

    //----------------------------------------------------
    // ImPlot3DScatterFlags
    //----------------------------------------------------

    /**
     * \if ENGLISH
     * @brief Gets the raw ImPlot3DScatterFlags
     * @return Current scatter flags value
     * \endif
     *
     * \if CHINESE
     * @brief 获取原始ImPlot3DScatterFlags
     * @return 当前散点图标志值
     * \endif
     */
    int scatterFlags() const;

    /**
     * \if ENGLISH
     * @brief Sets the raw ImPlot3DScatterFlags
     * @param flags New scatter flags value
     * \endif
     *
     * \if CHINESE
     * @brief 设置原始ImPlot3DScatterFlags
     * @param flags 新散点图标志值
     * \endif
     */
    void setScatterFlags(int flags);

Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when marker color changes
     * @param color New marker color
     * \endif
     *
     * \if CHINESE
     * @brief 标记颜色更改时触发
     * @param color 新标记颜色
     * \endif
     */
    void colorChanged(const QColor& color);

    /**
     * \if ENGLISH
     * @brief Emitted when marker size changes
     * @param size New marker size in pixels
     * \endif
     *
     * \if CHINESE
     * @brief 标记大小更改时触发
     * @param size 新标记大小（像素）
     * \endif
     */
    void markerSizeChanged(float size);

    /**
     * \if ENGLISH
     * @brief Emitted when marker shape changes
     * @param shape New marker shape as QImPlot3DMarkerShape enum value
     * \endif
     *
     * \if CHINESE
     * @brief 标记形状更改时触发
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
     * @brief 数据系列更改时触发
     * \endif
     */
    void dataChanged();

protected:
    /**
     * \if ENGLISH
     * @brief Begins drawing the scatter plot
     * @return true if drawing succeeded, false if no valid data
     * @details Calls SetNextMarkerStyle and PlotScatter with XYZ data.
     *          All conversions are done in setters for minimal beginDraw overhead.
     * \endif
     *
     * \if CHINESE
     * @brief 开始绘制散点图
     * @return true表示绘制成功，false表示无有效数据
     * @details 调用SetNextMarkerStyle和PlotScatter绘制XYZ数据。
     *          所有转换都在setter中完成，以最小化beginDraw开销。
     * \endif
     */
    virtual bool beginDraw() override;
};

// Template function implementation
template<typename CX, typename CY, typename CZ>
inline QImAbstractXYZDataSeries* QImPlot3DExtScatterItemNode::setData(const CX& x, const CY& y, const CZ& z)
{
    QImAbstractXYZDataSeries* s = new QImVectorXYZDataSeries<CX, CY, CZ>(x, y, z);
    setData(s);
    return s;
}

}  // end namespace QIM

#endif  // QIMPLOT3DEXTSCATTERITEMNODE_H

