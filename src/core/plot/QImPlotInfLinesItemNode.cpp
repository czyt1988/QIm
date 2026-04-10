#include "QImPlotInfLinesItemNode.h"
#include <optional>
#include "QImTrackedValue.hpp"
#include "QtImGuiUtils.h"
#include "implot.h"
#include "implot_internal.h"

namespace QIM
{

class QImPlotInfLinesItemNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlotInfLinesItemNode)
public:
    explicit PrivateData(QImPlotInfLinesItemNode* p) : q_ptr(p)
    {
    }

    std::vector< double > values;
    ImPlotInfLinesFlags flags { ImPlotInfLinesFlags_None };
    std::optional< QImTrackedValue< ImVec4, QIM::ImVecComparator< ImVec4 > > > color;
};

/**
 * \if ENGLISH
 * @brief Constructs an infinite lines plot item node.
 * @param[in] parent Parent QObject.
 * \endif
 *
 * \if CHINESE
 * @brief 构造一个无限线绘图项节点。
 * @param[in] parent 父QObject对象。
 * \endif
 */
QImPlotInfLinesItemNode::QImPlotInfLinesItemNode(QObject* parent)
    : QImPlotItemNode(parent), QIM_PIMPL_CONSTRUCT
{
}

/**
 * \if ENGLISH
 * @brief Destroys the infinite lines plot item node.
 * \endif
 *
 * \if CHINESE
 * @brief 销毁无限线绘图项节点。
 * \endif
 */
QImPlotInfLinesItemNode::~QImPlotInfLinesItemNode()
{
}

/**
 * \if ENGLISH
 * @brief Replaces the full coordinate list of infinite lines.
 * @param[in] values Coordinate list used by PlotInfLines.
 * \endif
 *
 * \if CHINESE
 * @brief 替换无限线的完整坐标列表。
 * @param[in] values 传递给PlotInfLines使用的坐标列表。
 * \endif
 */
void QImPlotInfLinesItemNode::setValues(const std::vector< double >& values)
{
    QIM_D(d);
    d->values = values;
    Q_EMIT valuesChanged();
}

/**
 * \if ENGLISH
 * @brief Replaces the full coordinate list of infinite lines with move semantics.
 * @param[in] values Coordinate list used by PlotInfLines.
 * \endif
 *
 * \if CHINESE
 * @brief 使用移动语义替换无限线的完整坐标列表。
 * @param[in] values 传递给PlotInfLines使用的坐标列表。
 * \endif
 */
void QImPlotInfLinesItemNode::setValues(std::vector< double >&& values)
{
    QIM_D(d);
    d->values = std::move(values);
    Q_EMIT valuesChanged();
}

/**
 * \if ENGLISH
 * @brief Returns the stored infinite line coordinates.
 * @return Internal coordinate list.
 * \endif
 *
 * \if CHINESE
 * @brief 返回当前存储的无限线坐标。
 * @return 内部坐标列表。
 * \endif
 */
const std::vector< double >& QImPlotInfLinesItemNode::values() const
{
    return d_ptr->values;
}

/**
 * \if ENGLISH
 * @brief Checks whether the infinite lines are rendered horizontally.
 * @return True if horizontal mode is enabled.
 * \endif
 *
 * \if CHINESE
 * @brief 检查无限线是否以水平方向渲染。
 * @return 启用水平模式时返回true。
 * \endif
 */
bool QImPlotInfLinesItemNode::isHorizontal() const
{
    QIM_DC(d);
    return (d->flags & ImPlotInfLinesFlags_Horizontal) != 0;
}

/**
 * \if ENGLISH
 * @brief Enables or disables horizontal mode.
 * @param[in] on True to render lines horizontally.
 * \endif
 *
 * \if CHINESE
 * @brief 启用或禁用水平模式。
 * @param[in] on 为true时按水平线渲染。
 * \endif
 */
void QImPlotInfLinesItemNode::setHorizontal(bool on)
{
    QIM_D(d);
    const ImPlotInfLinesFlags oldFlags = d->flags;
    if (on) {
        d->flags |= ImPlotInfLinesFlags_Horizontal;
    } else {
        d->flags &= ~ImPlotInfLinesFlags_Horizontal;
    }
    if (oldFlags != d->flags) {
        Q_EMIT infLinesFlagChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Returns the raw ImPlotInfLinesFlags bitmask.
 * @return Current ImPlotInfLinesFlags value.
 * \endif
 *
 * \if CHINESE
 * @brief 返回原始ImPlotInfLinesFlags位掩码。
 * @return 当前的ImPlotInfLinesFlags值。
 * \endif
 */
int QImPlotInfLinesItemNode::infLinesFlags() const
{
    QIM_DC(d);
    return d->flags;
}

/**
 * \if ENGLISH
 * @brief Sets the raw ImPlotInfLinesFlags bitmask.
 * @param[in] flags New ImPlotInfLinesFlags value.
 * \endif
 *
 * \if CHINESE
 * @brief 设置原始ImPlotInfLinesFlags位掩码。
 * @param[in] flags 新的ImPlotInfLinesFlags值。
 * \endif
 */
void QImPlotInfLinesItemNode::setInfLinesFlags(int flags)
{
    QIM_D(d);
    if (d->flags != flags) {
        d->flags = static_cast< ImPlotInfLinesFlags >(flags);
        Q_EMIT infLinesFlagChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Sets the explicit line color.
 * @param[in] c Qt color used for rendering.
 * \endif
 *
 * \if CHINESE
 * @brief 设置显式线条颜色。
 * @param[in] c 用于渲染的Qt颜色。
 * \endif
 */
void QImPlotInfLinesItemNode::setColor(const QColor& c)
{
    d_ptr->color = toImVec4(c);
    Q_EMIT colorChanged(c);
}

/**
 * \if ENGLISH
 * @brief Returns the effective line color.
 * @return Explicit color if set; otherwise an invalid QColor.
 * \endif
 *
 * \if CHINESE
 * @brief 返回当前线条颜色。
 * @return 如果已设置显式颜色则返回该颜色，否则返回无效QColor。
 * \endif
 */
QColor QImPlotInfLinesItemNode::color() const
{
    return (d_ptr->color.has_value()) ? toQColor(d_ptr->color->value()) : QColor();
}

/**
 * \if ENGLISH
 * @brief Draws the infinite lines through ImPlot::PlotInfLines.
 * @return Always returns false because the item has no child scope to recurse into.
 * \endif
 *
 * \if CHINESE
 * @brief 通过ImPlot::PlotInfLines绘制无限线。
 * @return 始终返回false，因为该绘图项没有子作用域需要递归渲染。
 * \endif
 */
bool QImPlotInfLinesItemNode::beginDraw()
{
    QIM_D(d);
    if (d->values.empty()) {
        return false;
    }

    if (d->color && d->color->is_dirty()) {
        ImPlot::SetNextLineStyle(d->color->value());
    }

    ImPlot::PlotInfLines(labelConstData(),
                         d->values.data(),
                         static_cast< int >(d->values.size()),
                         d->flags,
                         0,
                         sizeof(double));

    ImPlotContext* ct = ImPlot::GetCurrentContext();
    ImPlotItem* plotItem = ct->PreviousItem;
    setImPlotItem(plotItem);
    if (plotItem && plotItem->Show != QImAbstractNode::isVisible()) {
        QImAbstractNode::setVisible(plotItem->Show);
    }
    if (!d->color) {
        d->color = ImPlot::GetLastItemColor();
    }
    return false;
}

}  // namespace QIM
