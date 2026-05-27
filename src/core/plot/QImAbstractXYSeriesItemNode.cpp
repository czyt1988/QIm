#include "QImAbstractXYSeriesItemNode.h"
#include "QImPlotDataSeries.h"
#include <memory>

namespace QIM
{

class QImAbstractXYSeriesItemNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImAbstractXYSeriesItemNode)
public:
    PrivateData(QImAbstractXYSeriesItemNode* p);
    std::unique_ptr<QImAbstractXYDataSeries> data;
};

QImAbstractXYSeriesItemNode::PrivateData::PrivateData(QImAbstractXYSeriesItemNode* p) : q_ptr(p)
{
}

/**
 * \if ENGLISH
 * @brief Constructs a QImAbstractXYSeriesItemNode with optional parent
 * @param[in] parent Parent QObject (typically a QImPlotNode)
 * \endif
 *
 * \if CHINESE
 * @brief 构造 QImAbstractXYSeriesItemNode，可选指定父对象
 * @param[in] parent 父 QObject（通常为 QImPlotNode）
 * \endif
 */
QImAbstractXYSeriesItemNode::QImAbstractXYSeriesItemNode(QObject* parent)
    : QImPlotItemNode(parent), QIM_PIMPL_CONSTRUCT
{
}

/**
 * \if ENGLISH
 * @brief Destructor for QImAbstractXYSeriesItemNode
 * \endif
 *
 * \if CHINESE
 * @brief QImAbstractXYSeriesItemNode 的析构函数
 * \endif
 */
QImAbstractXYSeriesItemNode::~QImAbstractXYSeriesItemNode()
{
}

/**
 * \if ENGLISH
 * @brief Sets the data series for the plot item
 * @param[in] series Pointer to QImAbstractXYDataSeries (ownership transferred)
 * @details Stores the series and emits dataChanged() signal.
 * \endif
 *
 * \if CHINESE
 * @brief 设置绘图项的数据系列
 * @param[in] series QImAbstractXYDataSeries 指针（所有权转移）
 * @details 存储数据系列并触发 dataChanged() 信号。
 * \endif
 */
void QImAbstractXYSeriesItemNode::setData(QImAbstractXYDataSeries* series)
{
    QIM_D(d);
    d->data.reset(series);
    Q_EMIT dataChanged();
}

/**
 * \if ENGLISH
 * @brief Gets the current data series
 * @return Pointer to QImAbstractXYDataSeries, nullptr if no data set
 * \endif
 *
 * \if CHINESE
 * @brief 获取当前数据系列
 * @return QImAbstractXYDataSeries 指针，无数据时返回 nullptr
 * \endif
 */
QImAbstractXYDataSeries* QImAbstractXYSeriesItemNode::data() const
{
    QIM_DC(d);
    return d->data.get();
}

}  // end namespace QIM