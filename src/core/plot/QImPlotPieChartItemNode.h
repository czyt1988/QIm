#ifndef QIMPLOTPIECHARTITEMNODE_H
#define QIMPLOTPIECHARTITEMNODE_H
#include <QColor>
#include <QPointF>
#include <QStringList>
#include <vector>
#include "QImPlotItemNode.h"

namespace QIM
{

/**
 * \if ENGLISH
 * @brief Qt-style wrapper for ImPlot pie chart visualization
 *
 * @class QImPlotPieChartItemNode
 * @ingroup plot_items
 *
 * @details Provides Qt-style retained mode encapsulation for ImPlot pie charts.
 *          The node manages slice labels, slice values, pie center, radius,
 *          label formatting, start angle, and common pie-chart flags.
 *          Slice legend entries are still handled by ImPlot using the provided labels.
 *
 * @note Pie charts should generally be used inside an equal-aspect plot with
 *       hidden axis decorations for best visual results.
 *
 * @see QImPlotItemNode
 * @see QImPlotNode
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot饼图的Qt风格封装
 *
 * @class QImPlotPieChartItemNode
 * @ingroup plot_items
 *
 * @details 为ImPlot饼图提供Qt风格的保留模式封装。
 *          该节点管理切片标签、切片数值、圆心、半径、标签格式、起始角度以及常用饼图标志位。
 *          每个切片的图例项仍由ImPlot根据提供的标签原生管理。
 *
 * @note 饼图通常应放在等比例坐标且隐藏坐标轴装饰的绘图区域中，以获得更好的视觉效果。
 *
 * @see QImPlotItemNode
 * @see QImPlotNode
 * \endif
 */
class QIM_CORE_API QImPlotPieChartItemNode : public QImPlotItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotPieChartItemNode)

    Q_PROPERTY(QPointF center READ center WRITE setCenter NOTIFY geometryChanged)
    Q_PROPERTY(double radius READ radius WRITE setRadius NOTIFY geometryChanged)
    Q_PROPERTY(double startAngle READ startAngle WRITE setStartAngle NOTIFY geometryChanged)
    Q_PROPERTY(QString labelFormat READ labelFormat WRITE setLabelFormat NOTIFY labelFormatChanged)
    Q_PROPERTY(bool normalize READ isNormalize WRITE setNormalize NOTIFY pieChartFlagChanged)
    Q_PROPERTY(bool ignoreHidden READ isIgnoreHidden WRITE setIgnoreHidden NOTIFY pieChartFlagChanged)
    Q_PROPERTY(bool exploding READ isExploding WRITE setExploding NOTIFY pieChartFlagChanged)

public:
    enum
    {
        Type = InnerType + 9
    };

    virtual int type() const override
    {
        return Type;
    }

    // Constructs a pie chart item node.
    explicit QImPlotPieChartItemNode(QObject* parent = nullptr);

    // Destroys the pie chart item node.
    ~QImPlotPieChartItemNode();

    // Sets the pie chart labels and values.
    void setData(const QStringList& labels, const std::vector< double >& values);

    // Sets the pie chart labels and values with move semantics.
    void setData(QStringList labels, std::vector< double >&& values);

    // Returns the slice labels.
    QStringList labels() const;

    // Returns the slice values.
    const std::vector< double >& values() const;

    // Sets the pie chart center in plot coordinates.
    void setCenter(const QPointF& center);

    // Returns the pie chart center in plot coordinates.
    QPointF center() const;

    // Sets the pie chart radius in plot units.
    void setRadius(double radius);

    // Returns the pie chart radius in plot units.
    double radius() const;

    // Sets the start angle in degrees.
    void setStartAngle(double angle);

    // Returns the start angle in degrees.
    double startAngle() const;

    // Sets the label format string. Empty means no labels.
    void setLabelFormat(const QString& format);

    // Returns the label format string.
    QString labelFormat() const;

    // Returns true if values are normalized to a full circle.
    bool isNormalize() const;

    // Enables or disables normalization.
    void setNormalize(bool on);

    // Returns true if hidden slices are ignored.
    bool isIgnoreHidden() const;

    // Enables or disables hidden-slice ignoring.
    void setIgnoreHidden(bool on);

    // Returns true if legend-hovered slices explode.
    bool isExploding() const;

    // Enables or disables exploding behavior.
    void setExploding(bool on);

    // Returns the raw ImPlotPieChartFlags value.
    int pieChartFlags() const;

    // Sets the raw ImPlotPieChartFlags value.
    void setPieChartFlags(int flags);

Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when labels or values change.
     * \endif
     *
     * \if CHINESE
     * @brief 当标签或数值数据发生变化时触发。
     * \endif
     */
    void dataChanged();

    /**
     * \if ENGLISH
     * @brief Emitted when center, radius, or start angle changes.
     * \endif
     *
     * \if CHINESE
     * @brief 当圆心、半径或起始角度发生变化时触发。
     * \endif
     */
    void geometryChanged();

    /**
     * \if ENGLISH
     * @brief Emitted when the label format string changes.
     * @param format New label format string.
     * \endif
     *
     * \if CHINESE
     * @brief 当标签格式字符串发生变化时触发。
     * @param format 新的标签格式字符串。
     * \endif
     */
    void labelFormatChanged(const QString& format);

    /**
     * \if ENGLISH
     * @brief Emitted when pie chart flags change.
     * \endif
     *
     * \if CHINESE
     * @brief 当饼图标志位发生变化时触发。
     * \endif
     */
    void pieChartFlagChanged();

protected:
    // Draws the pie chart item.
    virtual bool beginDraw() override;
};

}  // namespace QIM

#endif  // QIMPLOTPIECHARTITEMNODE_H
