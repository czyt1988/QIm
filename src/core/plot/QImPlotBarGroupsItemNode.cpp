#include "QImPlotBarGroupsItemNode.h"
#include <memory>
#include <optional>
#include "implot.h"
#include "implot_internal.h"
#include "QImTrackedValue.hpp"
#include "QtImGuiUtils.h"
#include <QDebug>

namespace QIM
{

class QImPlotBarGroupsItemNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlotBarGroupsItemNode)
public:
    PrivateData(QImPlotBarGroupsItemNode* p);

    std::unique_ptr< QImAbstractBarGroupsDataSeries > data;
    ImPlotBarGroupsFlags flags { ImPlotBarGroupsFlags_None };
    double groupWidth { 0.67 };  ///< Group width in plot units
    double shift { 0.0 };        ///< Shift offset in plot units
    // Style tracking values
    std::optional< QImTrackedValue< ImVec4, QIM::ImVecComparator< ImVec4 > > > color;
};

QImPlotBarGroupsItemNode::PrivateData::PrivateData(QImPlotBarGroupsItemNode* p) : q_ptr(p)
{
}

/**
 * \if ENGLISH
 * @brief Constructor for QImPlotBarGroupsItemNode
 * @param parent Parent QObject
 * \endif
 *
 * \if CHINESE
 * @brief QImPlotBarGroupsItemNode的构造函数
 * @param parent 父QObject
 * \endif
 */
QImPlotBarGroupsItemNode::QImPlotBarGroupsItemNode(QObject* parent) : QImPlotItemNode(parent), QIM_PIMPL_CONSTRUCT
{
}

/**
 * \if ENGLISH
 * @brief Destructor for QImPlotBarGroupsItemNode
 * \endif
 *
 * \if CHINESE
 * @brief QImPlotBarGroupsItemNode的析构函数
 * \endif
 */
QImPlotBarGroupsItemNode::~QImPlotBarGroupsItemNode()
{
}

/**
 * \if ENGLISH
 * @brief Set data series for the grouped bar chart
 * @param series Pointer to QImAbstractBarGroupsDataSeries
 * \endif
 *
 * \if CHINESE
 * @brief 设置分组柱状图的数据系列
 * @param series QImAbstractBarGroupsDataSeries指针
 * \endif
 */
void QImPlotBarGroupsItemNode::setData(QImAbstractBarGroupsDataSeries* series)
{
    QIM_D(d);
    d->data.reset(series);
    emit dataChanged();
}

/**
 * \if ENGLISH
 * @brief Get current data series
 * @return Pointer to QImAbstractBarGroupsDataSeries
 * \endif
 *
 * \if CHINESE
 * @brief 获取当前数据系列
 * @return QImAbstractBarGroupsDataSeries指针
 * \endif
 */
QImAbstractBarGroupsDataSeries* QImPlotBarGroupsItemNode::data() const
{
    return d_ptr->data.get();
}

/**
 * \if ENGLISH
 * @brief Get group width in plot units
 * @return Current group width
 * \endif
 *
 * \if CHINESE
 * @brief 获取组宽（绘图单位）
 * @return 当前组宽
 * \endif
 */
double QImPlotBarGroupsItemNode::groupWidth() const
{
    QIM_DC(d);
    return d->groupWidth;
}

/**
 * \if ENGLISH
 * @brief Set group width in plot units
 * @param width New group width
 * \endif
 *
 * \if CHINESE
 * @brief 设置组宽（绘图单位）
 * @param width 新组宽
 * \endif
 */
void QImPlotBarGroupsItemNode::setGroupWidth(double width)
{
    QIM_D(d);
    if (d->groupWidth != width) {
        d->groupWidth = width;
        emit groupWidthChanged(width);
    }
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
bool QImPlotBarGroupsItemNode::isHorizontal() const
{
    QIM_DC(d);
    return (d->flags & ImPlotBarGroupsFlags_Horizontal) != 0;
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
void QImPlotBarGroupsItemNode::setHorizontal(bool horizontal)
{
    QIM_D(d);
    const ImPlotBarGroupsFlags oldFlags = d->flags;
    if (horizontal) {
        d->flags |= ImPlotBarGroupsFlags_Horizontal;
    } else {
        d->flags &= ~ImPlotBarGroupsFlags_Horizontal;
    }
    if (d->flags != oldFlags) {
        emit orientationChanged(horizontal);
        emit barGroupsFlagChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Check if stacked bars are enabled
 * @return true if stacked mode is enabled
 * \endif
 *
 * \if CHINESE
 * @brief 检查是否启用堆叠柱状图
 * @return 如果启用堆叠模式则返回true
 * \endif
 */
bool QImPlotBarGroupsItemNode::isStacked() const
{
    QIM_DC(d);
    return (d->flags & ImPlotBarGroupsFlags_Stacked) != 0;
}

/**
 * \if ENGLISH
 * @brief Set stacked bars
 * @param stacked true to enable stacked mode
 * \endif
 *
 * \if CHINESE
 * @brief 设置堆叠柱状图
 * @param stacked true表示启用堆叠模式
 * \endif
 */
void QImPlotBarGroupsItemNode::setStacked(bool stacked)
{
    QIM_D(d);
    const ImPlotBarGroupsFlags oldFlags = d->flags;
    if (stacked) {
        d->flags |= ImPlotBarGroupsFlags_Stacked;
    } else {
        d->flags &= ~ImPlotBarGroupsFlags_Stacked;
    }
    if (d->flags != oldFlags) {
        emit stackedChanged(stacked);
        emit barGroupsFlagChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Get shift offset in plot units
 * @return Current shift offset
 * \endif
 *
 * \if CHINESE
 * @brief 获取偏移量（绘图单位）
 * @return 当前偏移量
 * \endif
 */
double QImPlotBarGroupsItemNode::shift() const
{
    QIM_DC(d);
    return d->shift;
}

/**
 * \if ENGLISH
 * @brief Set shift offset in plot units
 * @param shift New shift offset
 * \endif
 *
 * \if CHINESE
 * @brief 设置偏移量（绘图单位）
 * @param shift 新偏移量
 * \endif
 */
void QImPlotBarGroupsItemNode::setShift(double shift)
{
    QIM_D(d);
    if (d->shift != shift) {
        d->shift = shift;
        emit shiftChanged(shift);
    }
}

/**
 * \if ENGLISH
 * @brief Set bar color
 * @param c QColor for the bars
 * \endif
 *
 * \if CHINESE
 * @brief 设置柱子颜色
 * @param c 柱子的QColor
 * \endif
 */
void QImPlotBarGroupsItemNode::setColor(const QColor& c)
{
    qDebug() << "[BarGroupsItemNode] setColor called with:" << c;
    
    ImVec4 imColor = toImVec4(c);
    
    if (d_ptr->color.has_value()) {
        qDebug() << "[BarGroupsItemNode] color optional has value, current dirty=" << d_ptr->color->is_dirty();
        qDebug() << "[BarGroupsItemNode] current color value:" << toQColor(d_ptr->color->value());
        d_ptr->color->operator=(imColor);  // Explicitly call assignment to trigger dirty
        qDebug() << "[BarGroupsItemNode] after assignment, dirty=" << d_ptr->color->is_dirty();
    } else {
        qDebug() << "[BarGroupsItemNode] color optional has NO value, creating new QImTrackedValue";
        // Create with value and then mark dirty
        d_ptr->color.emplace(imColor);
        d_ptr->color->mark_dirty();  // Force dirty because this is a new color being set
        qDebug() << "[BarGroupsItemNode] after emplace+mark_dirty, dirty=" << d_ptr->color->is_dirty();
    }
    
    emit colorChanged(c);
}

/**
 * \if ENGLISH
 * @brief Get bar color
 * @return Current QColor
 * \endif
 *
 * \if CHINESE
 * @brief 获取柱子颜色
 * @return 当前的QColor
 * \endif
 */
QColor QImPlotBarGroupsItemNode::color() const
{
    return (d_ptr->color.has_value()) ? toQColor(d_ptr->color->value()) : QColor();
}

/**
 * \if ENGLISH
 * @brief Get raw ImPlotBarGroupsFlags
 * @return Current ImPlotBarGroupsFlags
 * \endif
 *
 * \if CHINESE
 * @brief 获取原始的ImPlotBarGroupsFlags
 * @return 当前的ImPlotBarGroupsFlags
 * \endif
 */
int QImPlotBarGroupsItemNode::barGroupsFlags() const
{
    QIM_DC(d);
    return d->flags;
}

/**
 * \if ENGLISH
 * @brief Set raw ImPlotBarGroupsFlags
 * @param flags New ImPlotBarGroupsFlags
 * \endif
 *
 * \if CHINESE
 * @brief 设置原始的ImPlotBarGroupsFlags
 * @param flags 新的ImPlotBarGroupsFlags
 * \endif
 */
void QImPlotBarGroupsItemNode::setBarGroupsFlags(int flags)
{
    QIM_D(d);
    if (d->flags != flags) {
        d->flags = static_cast< ImPlotBarGroupsFlags >(flags);
        emit barGroupsFlagChanged();
    }
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
bool QImPlotBarGroupsItemNode::beginDraw()
{
    QIM_D(d);
    if (!d->data || d->data->size() == 0) {
        return false;
    }

    // Debug: Check color state before applying style
    qDebug() << "[BarGroupsItemNode] beginDraw - color has_value:" << d->color.has_value();
    if (d->color.has_value()) {
        qDebug() << "[BarGroupsItemNode] beginDraw - color is_dirty:" << d->color->is_dirty();
        qDebug() << "[BarGroupsItemNode] beginDraw - color value:" << toQColor(d->color->value());
    }

    // Apply style - use SetNextFillStyle for bars (not SetNextLineStyle)
    if (d->color.has_value() && d->color->is_dirty()) {
        qDebug() << "[BarGroupsItemNode] beginDraw - calling SetNextFillStyle with color:" << toQColor(d->color->value());
        ImPlot::SetNextFillStyle(d->color->value());
        d->color->clear();  // Clear dirty flag after applying
    } else {
        qDebug() << "[BarGroupsItemNode] beginDraw - NOT calling SetNextFillStyle (color not dirty or no value)";
    }

    // Prepare label pointers
    std::vector<const char*> labelPtrs = d->data->labelPtrs();
    const double* values = d->data->valuesRawData();
    int itemCount = d->data->itemCount();
    int groupCount = d->data->groupCount();

    // Call ImPlot API
    if (d->data->isContiguous() && values) {
        ImPlot::PlotBarGroups(
            labelPtrs.data(), values, itemCount, groupCount, d->groupWidth, d->shift, d->flags);
    } else {
        // Non-contiguous memory mode: not supported by ImPlot's PlotBarGroups, fallback to manual drawing?
        // For now, just skip drawing
        qWarning() << "[BarGroupsItemNode] Non-contiguous data not supported for grouped bars";
        return false;
    }

    // Update item status
    ImPlotContext* ct    = ImPlot::GetCurrentContext();
    if (!ct) {
        return false;
    }
    ImPlotItem* plotItem = ct->PreviousItem;
    if (!plotItem) {
        return false;
    }
    setImPlotItem(plotItem);
    if (plotItem->Show != QImAbstractNode::isVisible()) {
        QImAbstractNode::setVisible(plotItem->Show);
    }
    if (!d->color) {
        // First render without explicit color, get default color from ImPlot
        d->color = ImPlot::GetLastItemColor();
    }

    return false;
}

}  // namespace QIM