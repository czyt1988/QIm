#ifndef QIMPLOT3DEXTLINEITEMNODE_H
#define QIMPLOT3DEXTLINEITEMNODE_H

#include "QImPlot3DExtItemNode.h"
#include "QImPlot3DDataSeries.h"

namespace QIM
{

class QImAbstractXYZDataSeries;

/**
 * \if ENGLISH
 * @brief 3D line plot item that wraps ImPlot3D::PlotLine
 * @details Renders a 3D line connecting consecutive XYZ data points.
 *          Provides Qt-style properties for color, line weight, and line flags.
 *          Supports segments, loop, and skipNaN rendering modes.
 *          Uses QImAbstractXYZDataSeries for data input (not XY like 2D).
 * @note ImPlot3DLineFlags use positive semantics (Segments, Loop, SkipNaN are NOT NoXxx).
 * @see QImPlot3DExtItemNode, QImAbstractXYZDataSeries, ImPlot3D::PlotLine
 * \endif
 *
 * \if CHINESE
 * @brief 3D 线图元素，封装 ImPlot3D::PlotLine
 * @details 渲染连接连续 XYZ 数据点的 3D 线条。
 *          提供颜色、线宽和线条标志的 Qt 风格属性。
 *          支持线段、循环和跳过 NaN 渲染模式。
 *          使用 QImAbstractXYZDataSeries 作为数据输入（与 2D 的 XY 不同）。
 * @note ImPlot3DLineFlags 使用肯定语义（Segments、Loop、SkipNaN 不是 NoXxx 类型）。
 * @see QImPlot3DExtItemNode, QImAbstractXYZDataSeries, ImPlot3D::PlotLine
 * \endif
 */
class QIM_CORE_API QImPlot3DExtLineItemNode : public QImPlot3DExtItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlot3DExtLineItemNode)

    // Style properties
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(float lineWeight READ lineWeight WRITE setLineWeight NOTIFY lineWeightChanged)

    // Line flags (positive semantics - setting flag enables the feature)
    Q_PROPERTY(bool segmentsEnabled READ isSegmentsEnabled WRITE setSegmentsEnabled NOTIFY lineFlagChanged)
    Q_PROPERTY(bool loopEnabled READ isLoopEnabled WRITE setLoopEnabled NOTIFY lineFlagChanged)
    Q_PROPERTY(bool skipNaNEnabled READ isSkipNaNEnabled WRITE setSkipNaNEnabled NOTIFY lineFlagChanged)

public:
    /**
     * \if ENGLISH
     * @brief Constructor
     * @param par Parent object (typically a QImPlot3DExtNode)
     * \endif
     *
     * \if CHINESE
     * @brief 构造函数
     * @param par 父对象（通常是 QImPlot3DExtNode）
     * \endif
     */
    explicit QImPlot3DExtLineItemNode(QObject* par = nullptr);

    /**
     * \if ENGLISH
     * @brief Destructor
     * \endif
     *
     * \if CHINESE
     * @brief 析构函数
     * \endif
     */
    ~QImPlot3DExtLineItemNode() override;

    /**
     * \if ENGLISH
     * @brief Item type for fast identification
     * @return InnerType3D + 2 = 1002
     * \endif
     *
     * \if CHINESE
     * @brief 元素类型用于快速识别
     * @return InnerType3D + 2 = 1002
     * \endif
     */
    enum
    {
        Type = InnerType3D + 2
    };
    virtual int type() const override { return Type; }

    //----------------------------------------------------
    // Data setting
    //----------------------------------------------------

    /**
     * \if ENGLISH
     * @brief Sets XYZ data series for the line
     * @param series Data series to use (ownership transferred to this node)
     * \endif
     *
     * \if CHINESE
     * @brief 设置线图的 XYZ 数据系列
     * @param series 要使用的数据系列（所有权转移至此节点）
     * \endif
     */
    void setData(QImAbstractXYZDataSeries* series);

    /**
     * \if ENGLISH
     * @brief Sets XYZ data from containers (template convenience method)
     * @tparam CX Container type for X coordinates
     * @tparam CY Container type for Y coordinates
     * @tparam CZ Container type for Z coordinates
     * @param x X coordinate container
     * @param y Y coordinate container
     * @param z Z coordinate container
     * @return Created data series pointer
     * @details Creates a QImVectorXYZDataSeries internally and calls setData().
     * \endif
     *
     * \if CHINESE
     * @brief 从容器设置 XYZ 数据（模板便捷方法）
     * @tparam CX X 坐标容器类型
     * @tparam CY Y 坐标容器类型
     * @tparam CZ Z 坐标容器类型
     * @param x X 坐标容器
     * @param y Y 坐标容器
     * @param z Z 坐标容器
     * @return 创建的数据系列指针
     * @details 内部创建 QImVectorXYZDataSeries 并调用 setData()。
     * \endif
     */
    template<typename CX, typename CY, typename CZ>
    QImAbstractXYZDataSeries* setData(const CX& x, const CY& y, const CZ& z);

    /**
     * \if ENGLISH
     * @brief Returns the current data series
     * @return Pointer to data series, nullptr if not set
     * \endif
     *
     * \if CHINESE
     * @brief 返回当前数据系列
     * @return 数据系列指针，未设置则返回 nullptr
     * \endif
     */
    QImAbstractXYZDataSeries* data() const;

    //----------------------------------------------------
    // Style properties
    //----------------------------------------------------

    /**
     * \if ENGLISH
     * @brief Sets the line color
     * @param c New color (converted to ImVec4 internally for beginDraw minimization)
     * \endif
     *
     * \if CHINESE
     * @brief 设置线条颜色
     * @param c 新颜色（内部转换为 ImVec4 以最小化 beginDraw）
     * \endif
     */
    void setColor(const QColor& c);

    /**
     * \if ENGLISH
     * @brief Returns the line color
     * @return Current color as QColor
     * \endif
     *
     * \if CHINESE
     * @brief 返回线条颜色
     * @return 当前颜色（QColor 格式）
     * \endif
     */
    QColor color() const;

    /**
     * \if ENGLISH
     * @brief Sets the line weight (thickness in pixels)
     * @param weight New line weight value
     * \endif
     *
     * \if CHINESE
     * @brief 设置线宽（像素单位的粗细）
     * @param weight 新线宽值
     * \endif
     */
    void setLineWeight(float weight);

    /**
     * \if ENGLISH
     * @brief Returns the line weight
     * @return Current line weight value
     * \endif
     *
     * \if CHINESE
     * @brief 返回线宽
     * @return 当前线宽值
     * \endif
     */
    float lineWeight() const;

    //----------------------------------------------------
    // Line flags (positive semantics)
    //----------------------------------------------------

    /**
     * \if ENGLISH
     * @brief Checks if segments mode is enabled
     * @return true if ImPlot3DLineFlags_Segments is set
     * @details When enabled, each pair of consecutive points forms an independent segment.
     * @note This is a Positive→Positive flag (NOT NoSegments).
     * \endif
     *
     * \if CHINESE
     * @brief 检查是否启用线段模式
     * @return true 表示设置了 ImPlot3DLineFlags_Segments
     * @details 启用时，每对连续点形成独立的线段。
     * @note 这是一个肯定语义标志（不是 NoSegments 类型）。
     * \endif
     */
    bool isSegmentsEnabled() const;

    /**
     * \if ENGLISH
     * @brief Sets segments mode enabled state
     * @param on true to enable, false to disable
     * \endif
     *
     * \if CHINESE
     * @brief 设置线段模式的启用状态
     * @param on true 表示启用，false 表示禁用
     * \endif
     */
    void setSegmentsEnabled(bool on);

    /**
     * \if ENGLISH
     * @brief Checks if loop mode is enabled
     * @return true if ImPlot3DLineFlags_Loop is set
     * @details When enabled, last and first points are connected to form a closed loop.
     * @note This is a Positive→Positive flag (NOT NoLoop).
     * \endif
     *
     * \if CHINESE
     * @brief 检查是否启用循环模式
     * @return true 表示设置了 ImPlot3DLineFlags_Loop
     * @details 启用时，最后和第一个点连接形成闭合环。
     * @note 这是一个肯定语义标志（不是 NoLoop 类型）。
     * \endif
     */
    bool isLoopEnabled() const;

    /**
     * \if ENGLISH
     * @brief Sets loop mode enabled state
     * @param on true to enable, false to disable
     * \endif
     *
     * \if CHINESE
     * @brief 设置循环模式的启用状态
     * @param on true 表示启用，false 表示禁用
     * \endif
     */
    void setLoopEnabled(bool on);

    /**
     * \if ENGLISH
     * @brief Checks if NaN skipping is enabled
     * @return true if ImPlot3DLineFlags_SkipNaN is set
     * @details When enabled, NaN values are skipped without breaking the line.
     * @note This is a Positive→Positive flag (NOT NoSkipNaN).
     * \endif
     *
     * \if CHINESE
     * @brief 检查是否启用 NaN 跳过
     * @return true 表示设置了 ImPlot3DLineFlags_SkipNaN
     * @details 启用时，NaN 值被跳过而不中断线条。
     * @note 这是一个肯定语义标志（不是 NoSkipNaN 类型）。
     * \endif
     */
    bool isSkipNaNEnabled() const;

    /**
     * \if ENGLISH
     * @brief Sets NaN skipping enabled state
     * @param on true to enable, false to disable
     * \endif
     *
     * \if CHINESE
     * @brief 设置 NaN 跳过的启用状态
     * @param on true 表示启用，false 表示禁用
     * \endif
     */
    void setSkipNaNEnabled(bool on);

    /**
     * \if ENGLISH
     * @brief Returns raw ImPlot3DLineFlags value
     * @return Current combined line flags
     * \endif
     *
     * \if CHINESE
     * @brief 返回原始 ImPlot3DLineFlags 值
     * @return 当前组合的线条标志
     * \endif
     */
    int lineFlags() const;

    /**
     * \if ENGLISH
     * @brief Sets raw ImPlot3DLineFlags value
     * @param flags New combined flags value
     * \endif
     *
     * \if CHINESE
     * @brief 设置原始 ImPlot3DLineFlags 值
     * @param flags 新的组合标志值
     * \endif
     */
    void setLineFlags(int flags);

Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when color changes
     * @param c New color value
     * \endif
     *
     * \if CHINESE
     * @brief 颜色改变时发射
     * @param c 新的颜色值
     * \endif
     */
    void colorChanged(const QColor& c);

    /**
     * \if ENGLISH
     * @brief Emitted when line weight changes
     * @param weight New line weight value
     * \endif
     *
     * \if CHINESE
     * @brief 线宽改变时发射
     * @param weight 新的线宽值
     * \endif
     */
    void lineWeightChanged(float weight);

    /**
     * \if ENGLISH
     * @brief Emitted when any line flag changes
     * \endif
     *
     * \if CHINESE
     * @brief 任意线条标志改变时发射
     * \endif
     */
    void lineFlagChanged();

protected:
    /**
     * \if ENGLISH
     * @brief Renders the 3D line plot
     * @return false (no endDraw needed)
     * @details Calls SetNextLineStyle and PlotLine with XYZ data.
     *          All conversions done in setters for minimal beginDraw overhead.
     * \endif
     *
     * \if CHINESE
     * @brief 渲染 3D 线图
     * @return false（无需 endDraw）
     * @details 调用 SetNextLineStyle 和 PlotLine 处理 XYZ 数据。
     *          所有转换在 setter 中完成以最小化 beginDraw 开销。
     * \endif
     */
    virtual bool beginDraw() override;
};

template<typename CX, typename CY, typename CZ>
inline QImAbstractXYZDataSeries* QImPlot3DExtLineItemNode::setData(const CX& x, const CY& y, const CZ& z)
{
    QImAbstractXYZDataSeries* s = new QImVectorXYZDataSeries<CX, CY, CZ>(x, y, z);
    setData(s);
    return s;
}

}  // end namespace QIM

#endif  // QIMPLOT3DEXTLINEITEMNODE_H

