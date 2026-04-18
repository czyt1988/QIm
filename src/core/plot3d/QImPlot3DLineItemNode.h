#ifndef QIMPLOT3DLINEITEMNODE_H
#define QIMPLOT3DLINEITEMNODE_H

#include "QImPlot3DItemNode.h"
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
 * @see QImPlot3DItemNode, QImAbstractXYZDataSeries, ImPlot3D::PlotLine
 * \endif
 *
 * \if CHINESE
 * @brief 3D 线图元素，封装 ImPlot3D::PlotLine
 * @details 渲染连接连续 XYZ 数据点的 3D 线条。
 *          提供颜色、线宽和线条标志的 Qt 风格属性。
 *          支持线段、循环和跳过 NaN 渲染模式。
 *          使用 QImAbstractXYZDataSeries 作为数据输入（与 2D 的 XY 不同）。
 * @note ImPlot3DLineFlags 使用肯定语义（Segments、Loop、SkipNaN 不是 NoXxx 类型）。
 * @see QImPlot3DItemNode, QImAbstractXYZDataSeries, ImPlot3D::PlotLine
 * \endif
 */
class QIM_CORE_API QImPlot3DLineItemNode : public QImPlot3DItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlot3DLineItemNode)

    // Style properties
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(float lineWeight READ lineWeight WRITE setLineWeight NOTIFY lineWeightChanged)

    // Line flags (positive semantics - setting flag enables the feature)
    Q_PROPERTY(bool segmentsEnabled READ isSegmentsEnabled WRITE setSegmentsEnabled NOTIFY lineFlagChanged)
    Q_PROPERTY(bool loopEnabled READ isLoopEnabled WRITE setLoopEnabled NOTIFY lineFlagChanged)
    Q_PROPERTY(bool skipNaNEnabled READ isSkipNaNEnabled WRITE setSkipNaNEnabled NOTIFY lineFlagChanged)

public:
    // Constructor
    explicit QImPlot3DLineItemNode(QObject* par = nullptr);

    // Destructor
    ~QImPlot3DLineItemNode() override;

    // Item type for fast identification
    enum
    {
        Type = InnerType3D + 2
    };
    virtual int type() const override { return Type; }

    //----------------------------------------------------
    // Data setting
    //----------------------------------------------------

    // Sets XYZ data series for the line
    void setData(QImAbstractXYZDataSeries* series);

    // Sets XYZ data from containers (template convenience method)
    template<typename CX, typename CY, typename CZ>
    QImAbstractXYZDataSeries* setData(const CX& x, const CY& y, const CZ& z);

    // Returns the current data series
    QImAbstractXYZDataSeries* data() const;

    //----------------------------------------------------
    // Style properties
    //----------------------------------------------------

    // Sets the line color
    void setColor(const QColor& c);

    // Returns the line color
    QColor color() const;

    // Sets the line weight (thickness in pixels)
    void setLineWeight(float weight);

    // Returns the line weight
    float lineWeight() const;

    //----------------------------------------------------
    // Line flags (positive semantics)
    //----------------------------------------------------

    // Checks if segments mode is enabled
    bool isSegmentsEnabled() const;

    // Sets segments mode enabled state
    void setSegmentsEnabled(bool on);

    // Checks if loop mode is enabled
    bool isLoopEnabled() const;

    // Sets loop mode enabled state
    void setLoopEnabled(bool on);

    // Checks if NaN skipping is enabled
    bool isSkipNaNEnabled() const;

    // Sets NaN skipping enabled state
    void setSkipNaNEnabled(bool on);

    // Returns raw ImPlot3DLineFlags value
    int lineFlags() const;

    // Sets raw ImPlot3DLineFlags value
    void setLineFlags(int flags);

Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when data changes
     * \endif
     *
     * \if CHINESE
     * @brief 数据改变时发射
     * \endif
     */
    void dataChanged();

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
    // Renders the 3D line plot
    virtual bool beginDraw() override;
};

template<typename CX, typename CY, typename CZ>
inline QImAbstractXYZDataSeries* QImPlot3DLineItemNode::setData(const CX& x, const CY& y, const CZ& z)
{
    QImAbstractXYZDataSeries* s = new QImVectorXYZDataSeries<CX, CY, CZ>(x, y, z);
    setData(s);
    Q_EMIT dataChanged();
    return s;
}

}  // end namespace QIM

#endif  // QIMPLOT3DLINEITEMNODE_H