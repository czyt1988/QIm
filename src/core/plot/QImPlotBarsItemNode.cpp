#include "QImPlotBarsItemNode.h"
#include <optional>
#include "implot.h"
#include "implot_internal.h"
#include "QImTrackedValue.hpp"
#include "QtImGuiUtils.h"
#include <QDebug>

namespace QIM
{

class QImPlotBarsItemNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlotBarsItemNode)
public:
    PrivateData(QImPlotBarsItemNode* p);

    std::unique_ptr< QImAbstractXYDataSeries > data;
    ImPlotBarsFlags flags { ImPlotBarsFlags_None };
    double barWidth { 0.67 };  ///< Bar width in plot units
    // Style tracking values
    std::optional< QImTrackedValue< ImVec4, QIM::ImVecComparator< ImVec4 > > > color;
};

QImPlotBarsItemNode::PrivateData::PrivateData(QImPlotBarsItemNode* p) : q_ptr(p)
{
}

/**
 * \if ENGLISH
 * @brief Constructor for QImPlotBarsItemNode
 * @param parent Parent QObject
 * \endif
 *
 * \if CHINESE
 * @brief QImPlotBarsItemNode的构造函数
 * @param parent 父QObject
 * \endif
 */
QImPlotBarsItemNode::QImPlotBarsItemNode(QObject* parent) : QImPlotItemNode(parent), QIM_PIMPL_CONSTRUCT
{
}

/**
 * \if ENGLISH
 * @brief Destructor for QImPlotBarsItemNode
 * \endif
 *
 * \if CHINESE
 * @brief QImPlotBarsItemNode的析构函数
 * \endif
 */
QImPlotBarsItemNode::~QImPlotBarsItemNode()
{
}

/**
 * \if ENGLISH
 * @brief Set data series for the bar chart
 * @param series Pointer to QImAbstractXYDataSeries
 * \endif
 *
 * \if CHINESE
 * @brief 设置柱状图的数据系列
 * @param series QImAbstractXYDataSeries指针
 * \endif
 */
void QImPlotBarsItemNode::setData(QImAbstractXYDataSeries* series)
{
    QIM_D(d);
    d->data.reset(series);
    emit dataChanged();
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
QImAbstractXYDataSeries* QImPlotBarsItemNode::data() const
{
    return d_ptr->data.get();
}

/**
 * \if ENGLISH
 * @brief Get bar width in plot units
 * @return Current bar width
 * \endif
 *
 * \if CHINESE
 * @brief 获取柱宽（绘图单位）
 * @return 当前柱宽
 * \endif
 */
double QImPlotBarsItemNode::barWidth() const
{
    QIM_DC(d);
    return d->barWidth;
}

/**
 * \if ENGLISH
 * @brief Set bar width in plot units
 * @param width New bar width
 * \endif
 *
 * \if CHINESE
 * @brief 设置柱宽（绘图单位）
 * @param width 新柱宽
 * \endif
 */
void QImPlotBarsItemNode::setBarWidth(double width)
{
    QIM_D(d);
    if (d->barWidth != width) {
        d->barWidth = width;
        emit barWidthChanged(width);
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
bool QImPlotBarsItemNode::isHorizontal() const
{
    QIM_DC(d);
    return (d->flags & ImPlotBarsFlags_Horizontal) != 0;
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
void QImPlotBarsItemNode::setHorizontal(bool horizontal)
{
    QIM_D(d);
    const ImPlotBarsFlags oldFlags = d->flags;
    if (horizontal) {
        d->flags |= ImPlotBarsFlags_Horizontal;
    } else {
        d->flags &= ~ImPlotBarsFlags_Horizontal;
    }
    if (d->flags != oldFlags) {
        emit orientationChanged(horizontal);
        emit barsFlagChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Get raw ImPlotBarsFlags
 * @return Current ImPlotBarsFlags
 * \endif
 *
 * \if CHINESE
 * @brief 获取原始的ImPlotBarsFlags
 * @return 当前的ImPlotBarsFlags
 * \endif
 */
int QImPlotBarsItemNode::barsFlags() const
{
    QIM_DC(d);
    return d->flags;
}

/**
 * \if ENGLISH
 * @brief Set raw ImPlotBarsFlags
 * @param flags New ImPlotBarsFlags
 * \endif
 *
 * \if CHINESE
 * @brief 设置原始的ImPlotBarsFlags
 * @param flags 新的ImPlotBarsFlags
 * \endif
 */
void QImPlotBarsItemNode::setBarsFlags(int flags)
{
    QIM_D(d);
    if (d->flags != flags) {
        d->flags = static_cast< ImPlotBarsFlags >(flags);
        emit barsFlagChanged();
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
void QImPlotBarsItemNode::setColor(const QColor& c)
{
    qDebug() << "[BarsItemNode] setColor called with:" << c;
    
    ImVec4 imColor = toImVec4(c);
    
    if (d_ptr->color.has_value()) {
        qDebug() << "[BarsItemNode] color optional has value, current dirty=" << d_ptr->color->is_dirty();
        qDebug() << "[BarsItemNode] current color value:" << toQColor(d_ptr->color->value());
        d_ptr->color->operator=(imColor);  // Explicitly call assignment to trigger dirty
        qDebug() << "[BarsItemNode] after assignment, dirty=" << d_ptr->color->is_dirty();
    } else {
        qDebug() << "[BarsItemNode] color optional has NO value, creating new QImTrackedValue";
        // Create with value and then mark dirty
        d_ptr->color.emplace(imColor);
        d_ptr->color->mark_dirty();  // Force dirty because this is a new color being set
        qDebug() << "[BarsItemNode] after emplace+mark_dirty, dirty=" << d_ptr->color->is_dirty();
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
QColor QImPlotBarsItemNode::color() const
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
bool QImPlotBarsItemNode::beginDraw()
{
    QIM_D(d);
    if (!d->data || d->data->size() == 0) {
        return false;
    }

    // Debug: Check color state before applying style
    qDebug() << "[BarsItemNode] beginDraw - color has_value:" << d->color.has_value();
    if (d->color.has_value()) {
        qDebug() << "[BarsItemNode] beginDraw - color is_dirty:" << d->color->is_dirty();
        qDebug() << "[BarsItemNode] beginDraw - color value:" << toQColor(d->color->value());
    }

    // Apply style - use SetNextFillStyle for bars (not SetNextLineStyle)
    if (d->color.has_value() && d->color->is_dirty()) {
        qDebug() << "[BarsItemNode] beginDraw - calling SetNextFillStyle with color:" << toQColor(d->color->value());
        ImPlot::SetNextFillStyle(d->color->value());
        d->color->clear();  // Clear dirty flag after applying
    } else {
        qDebug() << "[BarsItemNode] beginDraw - NOT calling SetNextFillStyle (color not dirty or no value)";
    }

    // Call ImPlot API
    if (d->data->isContiguous()) {
        // Continuous memory mode: use zero-copy fast path
        const double* xData = d->data->xRawData();
        const double* yData = d->data->yRawData();
        int size            = d->data->size();

        if (xData) {
            // XY mode
            ImPlot::PlotBars(
                labelConstData(), xData, yData, size, d->barWidth, d->flags, 0, sizeof(double));
        } else {
            // Y-only mode - use xStart and xScale to generate X values
            // For Y-only mode, we need to create X values based on index
            std::vector< double > xValues(size);
            double xStart = d->data->xStart();
            double xScale = d->data->xScale();
            for (int i = 0; i < size; ++i) {
                xValues[ i ] = xStart + i * xScale;
            }
            ImPlot::PlotBars(
                labelConstData(), xValues.data(), yData, size, d->barWidth, d->flags, 0, sizeof(double));
        }
    } else {
        // Non-contiguous memory mode: use callback
        ImPlot::PlotBarsG(
            labelConstData(),
            [](int idx, void* data) -> ImPlotPoint {
                QImAbstractXYDataSeries* series = static_cast< QImAbstractXYDataSeries* >(data);
                return ImPlotPoint(series->xValue(idx), series->yValue(idx));
            },
            d->data.get(),
            d->data->size(),
            d->barWidth,
            d->flags);
    }

    // Update item status
    ImPlotContext* ct    = ImPlot::GetCurrentContext();
    ImPlotItem* plotItem = ct->PreviousItem;
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
