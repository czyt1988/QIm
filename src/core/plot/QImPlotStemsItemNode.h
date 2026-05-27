#ifndef QIMPLOTSTEMSITEMNODE_H
#define QIMPLOTSTEMSITEMNODE_H
#include <QColor>
#include "QImAbstractXYSeriesItemNode.h"

namespace QIM
{

/**
 * \if ENGLISH
 * @brief Qt-style wrapper for ImPlot stems visualization
 *
 * @details Provides Qt-style retained mode encapsulation for ImPlot stems.
 *          Stems plot draws a line from a reference value (baseline) to each data point,
 *          useful for visualizing discrete data points and their deviation from a baseline.
 *          Supports vertical (default) and horizontal orientations.
 *          Inherits from QImAbstractXYSeriesItemNode for data management.
 *
 * @note Stems are typically used to visualize discrete data points and their
 *       relationship to a baseline (usually zero).
 * @note The reference value (baseline) should be set appropriately for your data.
 *
 * @see QImAbstractXYSeriesItemNode
 * @see QImPlotNode
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot茎叶图的Qt风格封装。
 *
 * @details 为ImPlot茎叶图提供Qt风格的保留模式封装。
 *          茎叶图从参考值（基线）绘制线条到每个数据点，
 *          适用于可视化离散数据点及其与基线的偏差。
 *          支持垂直（默认）和水平方向。
 *          继承自QImAbstractXYSeriesItemNode以获得数据管理。
 *
 * @note 茎叶图通常用于可视化离散数据点及其与基线（通常是零）的关系。
 * @note 应根据数据适当设置参考值（基线）。
 *
 * @see QImAbstractXYSeriesItemNode
 * @see QImPlotNode
 * \endif
 */
class QIM_CORE_API QImPlotStemsItemNode : public QImAbstractXYSeriesItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotStemsItemNode)

    Q_PROPERTY(double referenceValue READ referenceValue WRITE setReferenceValue NOTIFY referenceValueChanged)
    Q_PROPERTY(bool horizontal READ isHorizontal WRITE setHorizontal NOTIFY orientationChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    // Unique type identifier for QImPlotStemsItemNode
    enum
    {
        Type = InnerType + 7
    };

    // Returns the type identifier of this plot item
    virtual int type() const override
    {
        return Type;
    }

    // Constructs a QImPlotStemsItemNode with optional parent
    QImPlotStemsItemNode(QObject* parent = nullptr);

    // Destroys the QImPlotStemsItemNode
    ~QImPlotStemsItemNode();

    //----------------------------------------------------
    // Style property accessors
    //----------------------------------------------------

    // Gets the reference value (baseline)
    double referenceValue() const;

    // Sets the reference value (baseline)
    void setReferenceValue(double value);

    // Checks if horizontal orientation is enabled
    bool isHorizontal() const;

    // Sets horizontal orientation
    void setHorizontal(bool horizontal);

    // Gets the stems color
    QColor color() const;

    // Sets the stems color
    void setColor(const QColor& color);

    // Gets the raw ImPlotStemsFlags
    int stemsFlags() const;

    // Sets the raw ImPlotStemsFlags
    void setStemsFlags(int flags);

Q_SIGNALS:
    void referenceValueChanged(double value);
    void orientationChanged(bool horizontal);
    void colorChanged(const QColor& color);
    void stemsFlagChanged();

protected:
    // Begins drawing the stems
    virtual bool beginDraw() override;
};

}  // end namespace QIM

#endif  // QIMPLOTSTEMSITEMNODE_H