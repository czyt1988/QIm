#include "QImPlotInfLinesItemNode.h"
#include <optional>
#include <vector>
#include "implot.h"
#include "implot_internal.h"
#include "QImTrackedValue.hpp"
#include "QtImGuiUtils.h"

namespace QIM
{

class QImPlotInfLinesItemNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlotInfLinesItemNode)
public:
    PrivateData(QImPlotInfLinesItemNode* p);

    std::vector< double > values;  ///< Values for infinite lines (x for vertical, y for horizontal)
    ImPlotInfLinesFlags flags { ImPlotInfLinesFlags_None };
    // Style tracking values
    std::optional< QImTrackedValue< ImVec4, QIM::ImVecComparator< ImVec4 > > > color;
};

QImPlotInfLinesItemNode::PrivateData::PrivateData(QImPlotInfLinesItemNode* p) : q_ptr(p)
{
}

/**
 * \if ENGLISH
 * @brief Constructor for QImPlotInfLinesItemNode
 * @param parent Parent QObject
 * \endif
 *
 * \if CHINESE
 * @brief QImPlotInfLinesItemNode的构造函数
 * @param parent 父QObject
 * \endif
 */
QImPlotInfLinesItemNode::QImPlotInfLinesItemNode(QObject* parent) : QImPlotItemNode(parent), QIM_PIMPL_CONSTRUCT
{
}

/**
 * \if ENGLISH
 * @brief Destructor for QImPlotInfLinesItemNode
 * \endif
 *
 * \if CHINESE
 * @brief QImPlotInfLinesItemNode的析构函数
 * \endif
 */
QImPlotInfLinesItemNode::~QImPlotInfLinesItemNode()
{
}

/**
 * \if ENGLISH
 * @brief Set a single value for one infinite line
 * @param value Position value for the infinite line
 * \endif
 *
 * \if CHINESE
 * @brief 设置单条无限线的值
 * @param value 无限线的位置值
 * \endif
 */
void QImPlotInfLinesItemNode::setValue(double value)
{
    QIM_D(d);
    d->values.clear();
    d->values.push_back(value);
    emit dataChanged();
}

/**
 * \if ENGLISH
 * @brief Set infinite lines data from raw pointer and size
 * @param values Pointer to array of values
 * @param count Number of values
 * \endif
 *
 * \if CHINESE
 * @brief 从原始指针和大小设置无限线数据
 * @param values 值数组指针
 * @param count 值的数量
 * \endif
 */
void QImPlotInfLinesItemNode::setValues(const double* values, int count)
{
    QIM_D(d);
    d->values.assign(values, values + count);
    emit dataChanged();
}

/**
 * \if ENGLISH
 * @brief Set infinite lines data from initializer list
 * @param values Initializer list of values
 * \endif
 *
 * \if CHINESE
 * @brief 从初始化列表设置无限线数据
 * @param values 值的初始化列表
 * \endif
 */
void QImPlotInfLinesItemNode::setValues(std::initializer_list< double > values)
{
    QIM_D(d);
    d->values.assign(values);
    emit dataChanged();
}

/**
 * \if ENGLISH
 * @brief Set infinite lines data from std::vector (move semantics)
 * @param values std::vector of values (rvalue reference)
 * \endif
 *
 * \if CHINESE
 * @brief 从std::vector设置无限线数据（移动语义）
 * @param values 值的std::vector（右值引用）
 * \endif
 */
void QImPlotInfLinesItemNode::setValues(std::vector< double >&& values)
{
    QIM_D(d);
    d->values = std::move(values);
    emit dataChanged();
}

/**
 * \if ENGLISH
 * @brief Get the number of values
 * @return Number of values
 * \endif
 *
 * \if CHINESE
 * @brief 获取值的数量
 * @return 值的数量
 * \endif
 */
int QImPlotInfLinesItemNode::count() const
{
    return static_cast< int >(d_ptr->values.size());
}

/**
 * \if ENGLISH
 * @brief Get the value at specified index
 * @param index Index of the value
 * @return Value at the specified index
 * \endif
 *
 * \if CHINESE
 * @brief 获取指定索引处的值
 * @param index 值的索引
 * @return 指定索引处的值
 * \endif
 */
double QImPlotInfLinesItemNode::value(int index) const
{
    return d_ptr->values.at(index);
}

/**
 * \if ENGLISH
 * @brief Check if horizontal orientation is enabled
 * @return true if horizontal mode is enabled
 * \endif
 *
 * \if CHINESE
 * @brief 检查是否启用水平方向
 * @return 如果启用水平模式则返回true
 * \endif
 */
bool QImPlotInfLinesItemNode::isHorizontal() const
{
    QIM_DC(d);
    return (d->flags & ImPlotInfLinesFlags_Horizontal) != 0;
}

/**
 * \if ENGLISH
 * @brief Set horizontal orientation
 * @param horizontal true to enable horizontal mode
 * \endif
 *
 * \if CHINESE
 * @brief 设置水平方向
 * @param horizontal true表示启用水平模式
 * \endif
 */
void QImPlotInfLinesItemNode::setHorizontal(bool horizontal)
{
    QIM_D(d);
    const ImPlotInfLinesFlags oldFlags = d->flags;
    if (horizontal) {
        d->flags |= ImPlotInfLinesFlags_Horizontal;
    } else {
        d->flags &= ~ImPlotInfLinesFlags_Horizontal;
    }
    if (d->flags != oldFlags) {
        emit orientationChanged(horizontal);
        emit infLinesFlagChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Get infinite lines color
 * @return Current QColor
 * \endif
 *
 * \if CHINESE
 * @brief 获取无限线颜色
 * @return 当前的QColor
 * \endif
 */
QColor QImPlotInfLinesItemNode::color() const
{
    return (d_ptr->color.has_value()) ? toQColor(d_ptr->color->value()) : QColor();
}

/**
 * \if ENGLISH
 * @brief Set infinite lines color
 * @param c QColor for the infinite lines
 * \endif
 *
 * \if CHINESE
 * @brief 设置无限线颜色
 * @param c 无限线的QColor
 * \endif
 */
void QImPlotInfLinesItemNode::setColor(const QColor& c)
{
    d_ptr->color = toImVec4(c);
    emit colorChanged(c);
}

/**
 * \if ENGLISH
 * @brief Get raw ImPlotInfLinesFlags
 * @return Current ImPlotInfLinesFlags
 * \endif
 *
 * \if CHINESE
 * @brief 获取原始的ImPlotInfLinesFlags
 * @return 当前的ImPlotInfLinesFlags
 * \endif
 */
int QImPlotInfLinesItemNode::infLinesFlags() const
{
    QIM_DC(d);
    return d->flags;
}

/**
 * \if ENGLISH
 * @brief Set raw ImPlotInfLinesFlags
 * @param flags New ImPlotInfLinesFlags
 * \endif
 *
 * \if CHINESE
 * @brief 设置原始的ImPlotInfLinesFlags
 * @param flags 新的ImPlotInfLinesFlags
 * \endif
 */
void QImPlotInfLinesItemNode::setInfLinesFlags(int flags)
{
    QIM_D(d);
    if (d->flags != flags) {
        d->flags = static_cast< ImPlotInfLinesFlags >(flags);
        emit infLinesFlagChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Begin drawing implementation
 * @return false to prevent endDraw from being called
 * @details Handles both vertical and horizontal orientations.
 * \endif
 *
 * \if CHINESE
 * @brief 开始绘制实现
 * @return false以防止调用endDraw
 * @details 处理垂直和水平方向。
 * \endif
 */
bool QImPlotInfLinesItemNode::beginDraw()
{
    QIM_D(d);
    if (d->values.empty()) {
        return false;
    }

    // Apply style
    if (d->color && d->color->is_dirty()) {
        ImPlot::SetNextLineStyle(d->color->value());
    }

    // Call ImPlot API
    ImPlot::PlotInfLines(
        labelConstData(), d->values.data(), static_cast< int >(d->values.size()), d->flags, 0, sizeof(double));

    // Update item status
    ImPlotContext* ct    = ImPlot::GetCurrentContext();
    ImPlotItem* plotItem = ct->PreviousItem;
    setImPlotItem(plotItem);
    if (plotItem && plotItem->Show != QImAbstractNode::isVisible()) {
        QImAbstractNode::setVisible(plotItem->Show);
    }
    if (!d->color) {
        // First render without explicit color, get default color from ImPlot
        d->color = ImPlot::GetLastItemColor();
    }

    return false;
}

}  // namespace QIM
