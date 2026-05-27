#ifndef QIMABSTRACTXYSSERIESITEMNODE_H
#define QIMABSTRACTXYSSERIESITEMNODE_H
#include "QImPlotItemNode.h"
#include "QImPlotDataSeries.h"
#include <memory>

namespace QIM
{

/**
 * \if ENGLISH
 * @brief Intermediate base class for all XY data series plot item nodes
 * @details Provides common data management (setData/data/dataChanged) for all
 *          plot item nodes that use QImAbstractXYDataSeries. Eliminates duplicated
 *          data member and accessor code across Line, Scatter, Bars, Stairs,
 *          Stems, Digital, Histogram, Histogram2D, and Shaded nodes.
 *          Inherits from QImPlotItemNode and uses PIMPL pattern with its own PrivateData.
 * @note Subclasses must still implement type() and beginDraw().
 * @note Line/Scatter keep QImDownsamplingController in their own PrivateData.
 * @note ShadedItemNode overrides setData() to also reset data2.
 * @see QImPlotItemNode, QImAbstractXYDataSeries, QImPlotLineItemNode
 * \endif
 *
 * \if CHINESE
 * @brief 所有XY数据系列绘图项节点的中间基类
 * @details 为所有使用QImAbstractXYDataSeries的绘图项节点提供通用数据管理
 *          (setData/data/dataChanged)。消除了Line、Scatter、Bars、Stairs、
 *          Stems、Digital、Histogram、Histogram2D和Shaded节点中重复的数据成员和访问器代码。
 *          继承自QImPlotItemNode，使用PIMPL模式并拥有自己的PrivateData。
 * @note 子类仍需实现type()和beginDraw()。
 * @note Line/Scatter在自己的PrivateData中保留QImDownsamplingController。
 * @note ShadedItemNode重写setData()以同时重置data2。
 * @see QImPlotItemNode, QImAbstractXYDataSeries, QImPlotLineItemNode
 * \endif
 */
class QIM_CORE_API QImAbstractXYSeriesItemNode : public QImPlotItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImAbstractXYSeriesItemNode)

public:
    // Constructs a QImAbstractXYSeriesItemNode with optional parent
    explicit QImAbstractXYSeriesItemNode(QObject* parent = nullptr);

    // Destroys the QImAbstractXYSeriesItemNode
    ~QImAbstractXYSeriesItemNode() override;

    // Sets the data series (takes ownership of series pointer)
    void setData(QImAbstractXYDataSeries* series);

    // Gets the current data series
    QImAbstractXYDataSeries* data() const;

    // Sets data from X and Y containers (convenience template)
    template<typename ContainerX, typename ContainerY>
    QImAbstractXYDataSeries* setData(const ContainerX& x, const ContainerY& y);

    // Sets data from X and Y containers with move semantics (convenience template)
    template<typename ContainerX, typename ContainerY>
    QImAbstractXYDataSeries* setData(ContainerX&& x, ContainerY&& y);

Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when data series changes
     * @details Triggered by setData() when new data is assigned.
     * \endif
     *
     * \if CHINESE
     * @brief 数据系列更改时触发
     * @details 当分配新数据时由setData()触发。
     * \endif
     */
    void dataChanged();
};

// Template convenience method implementations (inline in header)
template<typename ContainerX, typename ContainerY>
inline QImAbstractXYDataSeries* QImAbstractXYSeriesItemNode::setData(const ContainerX& x, const ContainerY& y)
{
    QImAbstractXYDataSeries* d = new QImVectorXYDataSeries(x, y);
    setData(d);
    return d;
}

template<typename ContainerX, typename ContainerY>
inline QImAbstractXYDataSeries* QImAbstractXYSeriesItemNode::setData(ContainerX&& x, ContainerY&& y)
{
    QImAbstractXYDataSeries* d = new QImVectorXYDataSeries(x, y);
    setData(d);
    return d;
}

}  // end namespace QIM

#endif  // QIMABSTRACTXYSSERIESITEMNODE_H