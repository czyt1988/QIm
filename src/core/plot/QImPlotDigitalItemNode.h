#ifndef QIMPLOTDIGITALITEMNODE_H
#define QIMPLOTDIGITALITEMNODE_H
#include "QImAPI.h"
#include <QColor>
#include "QImAbstractXYSeriesItemNode.h"

namespace QIM
{

/**
 * \if ENGLISH
 * @brief Qt-style wrapper for ImPlot digital signal visualization
 *
 * @details Provides Qt-style retained mode encapsulation for ImPlot digital signals.
 *          Digital plots visualize binary/digital signals (on/off states).
 *          They don't respond to y-axis drag/zoom and are always referenced to plot bottom.
 *          Inherits from QImAbstractXYSeriesItemNode for data management.
 *
 * @note Digital signals are typically used for visualizing binary data, logic levels,
 *       or any signal that switches between discrete values (e.g., 0 and 1).
 * @note Digital plots are always referenced to the plot bottom and ignore y-axis scaling.
 *
 * @see QImAbstractXYSeriesItemNode
 * @see QImPlotNode
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot数字信号可视化的Qt风格封装。
 *
 * @details 为ImPlot数字信号提供Qt风格的保留模式封装。
 *          数字图可视化二进制/数字信号（开/关状态）。
 *          它们不响应y轴拖拽/缩放，并且始终参考绘图底部。
 *          继承自QImAbstractXYSeriesItemNode以获得数据管理。
 *
 * @note 数字信号通常用于可视化二进制数据、逻辑电平或任何在离散值之间切换的信号（例如 0 和 1）。
 * @note 数字图始终参考绘图底部并忽略y轴缩放。
 *
 * @see QImAbstractXYSeriesItemNode
 * @see QImPlotNode
 * \endif
 */
class QIM_CORE_API QImPlotDigitalItemNode : public QImAbstractXYSeriesItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotDigitalItemNode)

    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    // Unique type identifier for QImPlotDigitalItemNode
    enum
    {
        Type = InnerType + 8
    };

    // Returns the type identifier of this plot item
    virtual int type() const override
    {
        return Type;
    }

    // Constructs a QImPlotDigitalItemNode with optional parent
    QImPlotDigitalItemNode(QObject* parent = nullptr);

    // Destroys the QImPlotDigitalItemNode
    ~QImPlotDigitalItemNode();

    //----------------------------------------------------
    // Style property accessors
    //----------------------------------------------------

    // Gets the digital signal color
    QColor color() const;

    // Sets the digital signal color
    void setColor(const QColor& color);

    // Gets the raw ImPlotDigitalFlags
    int digitalFlags() const;

    // Sets the raw ImPlotDigitalFlags
    void setDigitalFlags(int flags);

Q_SIGNALS:
    void colorChanged(const QColor& color);
    void digitalFlagChanged();

protected:
    // Begins drawing the digital signal
    virtual bool beginDraw() override;
};

}  // end namespace QIM

#endif  // QIMPLOTDIGITALITEMNODE_H