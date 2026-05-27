#ifndef QIMPLOTSHADEDITEMNODE_H
#define QIMPLOTSHADEDITEMNODE_H
#include <QColor>
#include "QImAbstractXYSeriesItemNode.h"
#include "QImPlotDataSeries.h"

namespace QIM
{

/**
 * \if ENGLISH
 * @brief Qt-style wrapper for ImPlot shaded (filled) region visualization
 *
 * @details Provides Qt-style retained mode encapsulation for ImPlot shaded/filled regions.
 *          Supports filling between a line and a horizontal reference value,
 *          or filling between two lines (upper and lower bounds).
 *          Useful for area charts, confidence intervals, and uncertainty visualization.
 *          Inherits from QImAbstractXYSeriesItemNode for data management.
 *
 * @note Shaded plots visualize filled regions between lines or between a line and a reference.
 *       Use QImPlotLineItemNode for simple line plots.
 * @note For two-line fill mode, both data series must have the same X coordinates.
 *
 * @see QImAbstractXYSeriesItemNode
 * @see QImPlotNode
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot填充区域（阴影）可视化的Qt风格封装。
 *
 * @details 为ImPlot填充/阴影区域提供Qt风格的保留模式封装。
 *          支持线条与水平参考值之间的填充，或两条线之间（上下边界）的填充。
 *          适用于面积图、置信区间和不确定性可视化。
 *          继承自QImAbstractXYSeriesItemNode以获得数据管理。
 *
 * @note 填充图可视化线条之间或线条与参考值之间的填充区域。
 *       简单线图请使用QImPlotLineItemNode。
 * @note 对于双线填充模式，两个数据系列必须具有相同的X坐标。
 *
 * @see QImAbstractXYSeriesItemNode
 * @see QImPlotNode
 * \endif
 */
class QIM_CORE_API QImPlotShadedItemNode : public QImAbstractXYSeriesItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotShadedItemNode)

    Q_PROPERTY(double referenceValue READ referenceValue WRITE setReferenceValue NOTIFY referenceValueChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    // Unique type identifier for QImPlotShadedItemNode
    enum
    {
        Type = InnerType + 5
    };

    // Returns the type identifier of this plot item
    virtual int type() const override
    {
        return Type;
    }

    // Constructs a QImPlotShadedItemNode with optional parent
    QImPlotShadedItemNode(QObject* parent = nullptr);

    // Destroys the QImPlotShadedItemNode
    ~QImPlotShadedItemNode();

    //----------------------------------------------------
    // Data setting interface - Two-line fill mode (Shaded-specific)
    //----------------------------------------------------

    using QImAbstractXYSeriesItemNode::setData;  // Bring base class templates into scope

    // Sets a single data series (overrides base to clear data2)
    void setData(QImAbstractXYDataSeries* series);

    // Sets two data series for filling between two lines (upper and lower bounds)
    void setData(QImAbstractXYDataSeries* series1, QImAbstractXYDataSeries* series2);

    // Sets two-line fill data from X, Y1, and Y2 containers
    template< typename ContainerX, typename ContainerY1, typename ContainerY2 >
    void setData(const ContainerX& x, const ContainerY1& y1, const ContainerY2& y2);

    // Sets two-line fill data from X, Y1, and Y2 containers (move semantics)
    template< typename ContainerX, typename ContainerY1, typename ContainerY2 >
    void setData(ContainerX&& x, ContainerY1&& y1, ContainerY2&& y2);

    // Gets the secondary data series (for two-line fill mode)
    QImAbstractXYDataSeries* data2() const;

    //----------------------------------------------------
    // Style property accessors
    //----------------------------------------------------

    // Gets the reference value for single-line fill mode
    double referenceValue() const;

    // Sets the reference value for single-line fill mode
    void setReferenceValue(double value);

    // Gets the fill color
    QColor color() const;

    // Sets the fill color
    void setColor(const QColor& color);

    // Gets the raw ImPlotShadedFlags
    int shadedFlags() const;

    // Sets the raw ImPlotShadedFlags
    void setShadedFlags(int flags);

    // Checks if two-line fill mode is active
    bool isTwoLineMode() const;

Q_SIGNALS:
    void referenceValueChanged(double value);
    void colorChanged(const QColor& color);
    void shadedFlagChanged();

protected:
    // Begins drawing the shaded region
    virtual bool beginDraw() override;
};

// Template function implementation - two line mode
template< typename ContainerX, typename ContainerY1, typename ContainerY2 >
inline void QImPlotShadedItemNode::setData(const ContainerX& x, const ContainerY1& y1, const ContainerY2& y2)
{
    QImAbstractXYDataSeries* d1 = new QImVectorXYDataSeries(x, y1);
    QImAbstractXYDataSeries* d2 = new QImVectorXYDataSeries(x, y2);
    setData(d1, d2);
}

template< typename ContainerX, typename ContainerY1, typename ContainerY2 >
inline void QImPlotShadedItemNode::setData(ContainerX&& x, ContainerY1&& y1, ContainerY2&& y2)
{
    QImAbstractXYDataSeries* d1 = new QImVectorXYDataSeries(x, y1);
    QImAbstractXYDataSeries* d2 = new QImVectorXYDataSeries(x, y2);
    setData(d1, d2);
}

}  // end namespace QIM

#endif  // QIMPLOTSHADEDITEMNODE_H