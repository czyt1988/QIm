#ifndef QIMPLOTPIECHARTITEMNODE_H
#define QIMPLOTPIECHARTITEMNODE_H

#include <QColor>
#include <QPointF>
#include "QImPlotItemNode.h"
#include "QImPlotPieChartDataSeries.h"

namespace QIM
{
class QImAbstractPieChartDataSeries;

/**
 * \if ENGLISH
 * @brief Qt-style wrapper for ImPlot pie chart visualization
 *
 * @class QImPlotPieChartItemNode
 * @ingroup plot_items
 *
 * @details Provides Qt-style retained mode encapsulation for ImPlot pie charts.
 *          Supports customizable center position, radius, label formatting,
 *          start angle, and flags for normalization, hidden slice handling,
 *          and exploding slices.
 *          Inherits from QImPlotItemNode and follows the same PIMPL design pattern
 *          as QImPlotBarsItemNode for consistency.
 *
 * @note Pie charts visualize proportional data using circular slices.
 *       Use QImPlotBarGroupsItemNode for grouped bar charts.
 * @warning Radius changes require redraw to take effect.
 *
 * @param[in] parent Parent QObject (optional)
 *
 * @see QImPlotItemNode
 * @see QImPlotBarsItemNode
 * @see QImPlotNode
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot饼图可视化的Qt风格封装
 *
 * @class QImPlotPieChartItemNode
 * @ingroup plot_items
 *
 * @details 为ImPlot饼图提供Qt风格的保留模式封装。
 *          支持可自定义的中心位置、半径、标签格式化、
 *          起始角度，以及用于归一化、隐藏切片处理和爆炸切片的标志。
 *          继承自QImPlotItemNode，并遵循与QImPlotBarsItemNode相同的PIMPL设计模式以保持一致性。
 *
 * @note 饼图使用圆形切片可视化比例数据。
 *       分组柱状图请使用QImPlotBarGroupsItemNode。
 * @warning 半径更改需要重绘才能生效。
 *
 * @param[in] parent 父QObject对象（可选）
 *
 * @see QImPlotItemNode
 * @see QImPlotBarsItemNode
 * @see QImPlotNode
 * \endif
 */
class QIM_CORE_API QImPlotPieChartItemNode : public QImPlotItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotPieChartItemNode)
    Q_PROPERTY(QPointF center READ center WRITE setCenter NOTIFY centerChanged)
    Q_PROPERTY(double radius READ radius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(QString labelFormat READ labelFormat WRITE setLabelFormat NOTIFY labelFormatChanged)
    Q_PROPERTY(double startAngle READ startAngle WRITE setStartAngle NOTIFY startAngleChanged)
    Q_PROPERTY(bool normalize READ isNormalized WRITE setNormalize NOTIFY normalizeChanged)
    Q_PROPERTY(bool ignoreHidden READ isIgnoreHidden WRITE setIgnoreHidden NOTIFY ignoreHiddenChanged)
    Q_PROPERTY(bool exploding READ isExploding WRITE setExploding NOTIFY explodingChanged)
public:
    // Unique type identifier for QImPlotPieChartItemNode
    enum
    {
        Type = InnerType + 10
    };

    // Returns the type identifier of this plot item
    virtual int type() const override
    {
        return Type;
    }

    // Constructs a QImPlotPieChartItemNode with optional parent
    QImPlotPieChartItemNode(QObject* parent = nullptr);

    // Destroys the QImPlotPieChartItemNode
    ~QImPlotPieChartItemNode();

    //----------------------------------------------------
    // Data setting interface
    //----------------------------------------------------

    // Sets the data series for the pie chart
    void setData(QImAbstractPieChartDataSeries* series);

    // Sets pie chart data from labels and values containers
    template< typename ContainerValues >
    QImAbstractPieChartDataSeries* setData(const QStringList& labels, const ContainerValues& values);

    // Sets pie chart data from labels and values containers (move semantics)
    template< typename ContainerValues >
    QImAbstractPieChartDataSeries* setData(QStringList&& labels, ContainerValues&& values);

    // Gets the current data series
    QImAbstractPieChartDataSeries* data() const;

    //----------------------------------------------------
    // Style property accessors
    //----------------------------------------------------

    // Gets the center position in plot units
    QPointF center() const;

    // Sets the center position in plot units
    void setCenter(const QPointF& center);

    // Gets the radius in plot units
    double radius() const;

    // Sets the radius in plot units
    void setRadius(double radius);

    // Gets the label format string
    QString labelFormat() const;

    // Sets the label format string
    void setLabelFormat(const QString& format);

    // Gets the starting angle in degrees
    double startAngle() const;

    // Sets the starting angle in degrees
    void setStartAngle(double angle);

    // Checks if normalization is enabled
    bool isNormalized() const;

    // Sets normalization
    void setNormalize(bool normalize);

    // Checks if hidden slices are ignored
    bool isIgnoreHidden() const;

    // Sets ignore hidden slices
    void setIgnoreHidden(bool ignore);

    // Checks if exploding slices are enabled
    bool isExploding() const;

    // Sets exploding slices
    void setExploding(bool exploding);

    // Gets the raw ImPlotPieChartFlags
    int pieChartFlags() const;

    // Sets the raw ImPlotPieChartFlags
    void setPieChartFlags(int flags);

Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when center position changes
     * @param[in] center New center position
     * @details Triggered by setCenter() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 中心位置更改时触发
     * @param[in] center 新中心位置
     * @details 当值实际更改时由setCenter()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void centerChanged(const QPointF& center);

    /**
     * \if ENGLISH
     * @brief Emitted when radius changes
     * @param[in] radius New radius in plot units
     * @details Triggered by setRadius() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 半径更改时触发
     * @param[in] radius 新半径（绘图单位）
     * @details 当值实际更改时由setRadius()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void radiusChanged(double radius);

    /**
     * \if ENGLISH
     * @brief Emitted when label format changes
     * @param[in] format New label format string
     * @details Triggered by setLabelFormat() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 标签格式更改时触发
     * @param[in] format 新标签格式字符串
     * @details 当值实际更改时由setLabelFormat()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void labelFormatChanged(const QString& format);

    /**
     * \if ENGLISH
     * @brief Emitted when start angle changes
     * @param[in] angle New start angle in degrees
     * @details Triggered by setStartAngle() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 起始角度更改时触发
     * @param[in] angle 新起始角度（度）
     * @details 当值实际更改时由setStartAngle()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void startAngleChanged(double angle);

    /**
     * \if ENGLISH
     * @brief Emitted when normalization flag changes
     * @param[in] normalize New normalization state
     * @details Triggered by setNormalize() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 归一化标志更改时触发
     * @param[in] normalize 新归一化状态
     * @details 当值实际更改时由setNormalize()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void normalizeChanged(bool normalize);

    /**
     * \if ENGLISH
     * @brief Emitted when ignore hidden flag changes
     * @param[in] ignore New ignore hidden state
     * @details Triggered by setIgnoreHidden() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 忽略隐藏标志更改时触发
     * @param[in] ignore 新忽略隐藏状态
     * @details 当值实际更改时由setIgnoreHidden()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void ignoreHiddenChanged(bool ignore);

    /**
     * \if ENGLISH
     * @brief Emitted when exploding flag changes
     * @param[in] exploding New exploding state
     * @details Triggered by setExploding() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 爆炸标志更改时触发
     * @param[in] exploding 新爆炸状态
     * @details 当值实际更改时由setExploding()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void explodingChanged(bool exploding);

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
     * @brief Emitted when pie chart flags change
     * @details Triggered by any flag property change (normalize, ignoreHidden, exploding, etc.).
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 饼图标志更改时触发
     * @details 任何标志属性更改（归一化、忽略隐藏、爆炸等）时触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void pieChartFlagChanged();

protected:
    // Begins drawing the pie chart
    virtual bool beginDraw() override;
};

// Template function implementation
template< typename ContainerValues >
inline QImAbstractPieChartDataSeries* QImPlotPieChartItemNode::setData(const QStringList& labels, const ContainerValues& values)
{
    QImAbstractPieChartDataSeries* d = new QImVectorPieChartDataSeries<ContainerValues>(labels, values);
    setData(d);
    return d;
}

template< typename ContainerValues >
inline QImAbstractPieChartDataSeries* QImPlotPieChartItemNode::setData(QStringList&& labels, ContainerValues&& values)
{
    QImAbstractPieChartDataSeries* d = new QImVectorPieChartDataSeries<ContainerValues>(std::move(labels), std::move(values));
    setData(d);
    return d;
}

}  // end namespace QIM

#endif  // QIMPLOTPIECHARTITEMNODE_H
