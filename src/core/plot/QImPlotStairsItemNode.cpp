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

    std::unique_ptr<QImAbstractXYDataSeries> data;
    ImPlotStairsFlags flags { ImPlotStairsFlags_None };
    // 样式跟踪值
    std::optional<QImTrackedValue<ImVec4, QIM::ImVecComparator<ImVec4>>> color;
};

QImPlotStairsItemNode::PrivateData::PrivateData(QImPlotStairsItemNode* p) : q_ptr(p)
{
}

// 构造函数
QImPlotStairsItemNode::QImPlotStairsItemNode(QObject* parent) 
    : QImPlotItemNode(parent), QIM_PIMPL_CONSTRUCT
{
}

// 析构函数
QImPlotStairsItemNode::~QImPlotStairsItemNode()
{
}

// 数据设置
void QImPlotStairsItemNode::setData(QImAbstractXYDataSeries* series)
{
    QIM_D(d);
    d->data.reset(series);
}

// 获取数据
QImAbstractXYDataSeries* QImPlotStairsItemNode::data() const
{
    return d_ptr->data.get();
}

// 标志访问器
bool QImPlotStairsItemNode::isShaded() const
{
    QIM_DC(d);
    return (d->flags & ImPlotStairsFlags_Shaded) != 0;
}

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

int QImPlotStairsItemNode::stairsFlags() const
{
    QIM_DC(d);
    return d->flags;
}

void QImPlotStairsItemNode::setStairsFlags(int flags)
{
    QIM_D(d);
    if (d->flags != flags) {
        d->flags = static_cast<ImPlotStairsFlags>(flags);
        emit stairsFlagChanged();
    }
}

// 样式设置
void QImPlotStairsItemNode::setColor(const QColor& c)
{
    d_ptr->color = toImVec4(c);
}

QColor QImPlotStairsItemNode::color() const
{
    return (d_ptr->color.has_value()) ? toQColor(d_ptr->color->value()) : QColor();
}

// 绘制实现
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
        int size = d->data->size();

        if (xData) {
            // XY模式
            ImPlot::PlotStairs(labelConstData(), xData, yData, size, d->flags, 0, sizeof(double));
        } else {
            // Y-only模式
            ImPlot::PlotStairs(labelConstData(), yData, size, d->data->xStart(), d->data->xScale(), d->flags, 0, sizeof(double));
        }
    } else {
        // 非连续内存模式：使用回调
        ImPlot::PlotStairsG(labelConstData(), [](int idx, void* data) -> ImPlotPoint {
            QImAbstractXYDataSeries* series = static_cast<QImAbstractXYDataSeries*>(data);
            return ImPlotPoint(series->xValue(idx), series->yValue(idx));
        }, d->data.get(), d->data->size(), d->flags);
    }

    // 更新item的状态
    ImPlotContext* ct = ImPlot::GetCurrentContext();
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

} // namespace QIM