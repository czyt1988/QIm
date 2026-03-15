#include "QImPlotStairsItemNode.h"
#include <optional>
#include "implot.h"
#include "implot_internal.h"
#include "QImTrackedValue.hpp"
#include "QtImGuiUtils.h"

namespace QIM
{

class QImPlotStairsItemNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlotStairsItemNode)
public:
    PrivateData(QImPlotStairsItemNode* p);

    std::unique_ptr< QImAbstractXYDataSeries > data;
    ImPlotStairsFlags flags { ImPlotStairsFlags_None };
    // 样式跟踪值
    std::optional< QImTrackedValue< ImVec4, QIM::ImVecComparator< ImVec4 > > > color;
};

QImPlotStairsItemNode::PrivateData::PrivateData(QImPlotStairsItemNode* p) : q_ptr(p)
{
}

/**
 * \if ENGLISH
 * @brief Constructor for QImPlotStairsItemNode
 * @param parent Parent QObject
 * \endif
 *
 * \if CHINESE
 * @brief QImPlotStairsItemNode的构造函数
 * @param parent 父QObject
 * \endif
 */
QImPlotStairsItemNode::QImPlotStairsItemNode(QObject* parent) : QImPlotItemNode(parent), QIM_PIMPL_CONSTRUCT
{
}

/**
 * \if ENGLISH
 * @brief Destructor for QImPlotStairsItemNode
 * \endif
 *
 * \if CHINESE
 * @brief QImPlotStairsItemNode的析构函数
 * \endif
 */
QImPlotStairsItemNode::~QImPlotStairsItemNode()
{
}

/**
 * \if ENGLISH
 * @brief Set data series for the stairs plot
 * @param series Pointer to QImAbstractXYDataSeries
 * \endif
 *
 * \if CHINESE
 * @brief 设置阶梯图的数据系列
 * @param series QImAbstractXYDataSeries指针
 * \endif
 */
void QImPlotStairsItemNode::setData(QImAbstractXYDataSeries* series)
{
    QIM_D(d);
    d->data.reset(series);
}

/**
 * \if ENGLISH
 * @brief Get current data series
 * @return Pointer to QImAbstractXYDataSeries
 * \endif
 *
 * \if CHINESE
 * @brief 获取当前数据系列
 * @return QImAbstractXYDataSeries指针
 * \endif
 */
QImAbstractXYDataSeries* QImPlotStairsItemNode::data() const
{
    return d_ptr->data.get();
}

/**
 * \if ENGLISH
 * @brief Check if shaded mode is enabled
 * @return true if shaded mode is enabled
 * \endif
 *
 * \if CHINESE
 * @brief 检查是否启用填充模式
 * @return 如果启用填充模式则返回true
 * \endif
 */
bool QImPlotStairsItemNode::isShaded() const
{
    QIM_DC(d);
    return (d->flags & ImPlotStairsFlags_Shaded) != 0;
}

/**
 * \if ENGLISH
 * @brief Set shaded mode
 * @param on true to enable shaded mode
 * \endif
 *
 * \if CHINESE
 * @brief 设置填充模式
 * @param on true表示启用填充模式
 * \endif
 */
void QImPlotStairsItemNode::setShaded(bool on)
{
    QIM_D(d);
    const ImPlotStairsFlags oldFlags = d->flags;
    if (on) {
        d->flags |= ImPlotStairsFlags_Shaded;
    } else {
        d->flags &= ~ImPlotStairsFlags_Shaded;
    }
    if (d->flags != oldFlags) {
        emit stairsFlagChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Get raw ImPlotStairsFlags
 * @return Current ImPlotStairsFlags
 * \endif
 *
 * \if CHINESE
 * @brief 获取原始的ImPlotStairsFlags
 * @return 当前的ImPlotStairsFlags
 * \endif
 */
int QImPlotStairsItemNode::stairsFlags() const
{
    QIM_DC(d);
    return d->flags;
}

/**
 * \if ENGLISH
 * @brief Set raw ImPlotStairsFlags
 * @param flags New ImPlotStairsFlags
 * \endif
 *
 * \if CHINESE
 * @brief 设置原始的ImPlotStairsFlags
 * @param flags 新的ImPlotStairsFlags
 * \endif
 */
void QImPlotStairsItemNode::setStairsFlags(int flags)
{
    QIM_D(d);
    if (d->flags != flags) {
        d->flags = static_cast< ImPlotStairsFlags >(flags);
        emit stairsFlagChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Set line color
 * @param c QColor for the line
 * \endif
 *
 * \if CHINESE
 * @brief 设置线条颜色
 * @param c 线条的QColor
 * \endif
 */
void QImPlotStairsItemNode::setColor(const QColor& c)
{
    d_ptr->color = toImVec4(c);
}

/**
 * \if ENGLISH
 * @brief Get line color
 * @return Current QColor
 * \endif
 *
 * \if CHINESE
 * @brief 获取线条颜色
 * @return 当前的QColor
 * \endif
 */
QColor QImPlotStairsItemNode::color() const
{
    return (d_ptr->color.has_value()) ? toQColor(d_ptr->color->value()) : QColor();
}

/**
 * \if ENGLISH
 * @brief Begin drawing implementation
 * @return false to prevent endDraw from being called
 * \endif
 *
 * \if CHINESE
 * @brief 开始绘制实现
 * @return false以防止调用endDraw
 * \endif
 */
bool QImPlotStairsItemNode::beginDraw()
{
    QIM_D(d);
    if (!d->data || d->data->size() == 0) {
        return false;
    }

    // 应用样式
    if (d->color && d->color->is_dirty()) {
        ImPlot::SetNextLineStyle(d->color->value());
    }

    // 调用 ImPlot API
    if (d->data->isContiguous()) {
        // 连续内存模式：使用零拷贝快速路径
        const double* xData = d->data->xRawData();
        const double* yData = d->data->yRawData();
        int size            = d->data->size();

        if (xData) {
            // XY模式
            ImPlot::PlotStairs(labelConstData(), xData, yData, size, d->flags, 0, sizeof(double));
        } else {
            // Y-only模式
            ImPlot::PlotStairs(
                labelConstData(), yData, size, d->data->xStart(), d->data->xScale(), d->flags, 0, sizeof(double));
        }
    } else {
        // 非连续内存模式：使用回调
        ImPlot::PlotStairsG(
            labelConstData(),
            [](int idx, void* data) -> ImPlotPoint {
                QImAbstractXYDataSeries* series = static_cast< QImAbstractXYDataSeries* >(data);
                return ImPlotPoint(series->xValue(idx), series->yValue(idx));
            },
            d->data.get(),
            d->data->size(),
            d->flags);
    }

    // 更新item的状态
    ImPlotContext* ct    = ImPlot::GetCurrentContext();
    ImPlotItem* plotItem = ct->PreviousItem;
    setImPlotItem(plotItem);
    if (plotItem->Show != QImAbstractNode::isVisible()) {
        QImAbstractNode::setVisible(plotItem->Show);
    }
    if (!d->color) {
        d->color = ImPlot::GetLastItemColor();
    }

    return false;
}

}  // namespace QIM