#include "QImPlotShadedItemNode.h"
#include <optional>
#include <cmath>
#include "implot.h"
#include "implot_internal.h"
#include "QImTrackedValue.hpp"
#include "QtImGuiUtils.h"

namespace QIM
{

class QImPlotShadedItemNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlotShadedItemNode)
public:
    PrivateData(QImPlotShadedItemNode* p);

    std::unique_ptr< QImAbstractXYDataSeries > data;   ///< Primary data series
    std::unique_ptr< QImAbstractXYDataSeries > data2;  ///< Secondary data series (for two-line mode)
    ImPlotShadedFlags flags { ImPlotShadedFlags_None };
    double referenceValue { 0.0 };  ///< Reference value for single-line fill mode
    // Style tracking values
    std::optional< QImTrackedValue< ImVec4, QIM::ImVecComparator< ImVec4 > > > color;
};

QImPlotShadedItemNode::PrivateData::PrivateData(QImPlotShadedItemNode* p) : q_ptr(p)
{
}

/**
 * \if ENGLISH
 * @brief Constructor for QImPlotShadedItemNode
 * @param parent Parent QObject
 * \endif
 *
 * \if CHINESE
 * @brief QImPlotShadedItemNode的构造函数
 * @param parent 父QObject
 * \endif
 */
QImPlotShadedItemNode::QImPlotShadedItemNode(QObject* parent) : QImPlotItemNode(parent), QIM_PIMPL_CONSTRUCT
{
}

/**
 * \if ENGLISH
 * @brief Destructor for QImPlotShadedItemNode
 * \endif
 *
 * \if CHINESE
 * @brief QImPlotShadedItemNode的析构函数
 * \endif
 */
QImPlotShadedItemNode::~QImPlotShadedItemNode()
{
}

/**
 * \if ENGLISH
 * @brief Set data series for single-line fill mode
 * @param series Pointer to QImAbstractXYDataSeries
 * @details In single-line mode, the area between the data line and referenceValue is filled.
 *          Any existing secondary data series (for two-line mode) is cleared.
 * \endif
 *
 * \if CHINESE
 * @brief 设置单线填充模式的数据系列
 * @param series QImAbstractXYDataSeries指针
 * @details 在单线模式下，数据线与referenceValue之间的区域被填充。
 *          任何现有的辅助数据系列（用于双线模式）将被清除。
 * \endif
 */
void QImPlotShadedItemNode::setData(QImAbstractXYDataSeries* series)
{
    QIM_D(d);
    d->data.reset(series);
    d->data2.reset();  // Clear secondary data for two-line mode
    emit dataChanged();
}

/**
 * \if ENGLISH
 * @brief Set two data series for two-line fill mode
 * @param series1 Pointer to primary QImAbstractXYDataSeries (lower bound)
 * @param series2 Pointer to secondary QImAbstractXYDataSeries (upper bound)
 * @details In two-line mode, the area between the two data lines is filled.
 *          Both series must have the same X coordinates for correct rendering.
 * \endif
 *
 * \if CHINESE
 * @brief 设置双线填充模式的两个数据系列
 * @param series1 主QImAbstractXYDataSeries指针（下边界）
 * @param series2 辅助QImAbstractXYDataSeries指针（上边界）
 * @details 在双线模式下，两条数据线之间的区域被填充。
 *          两个系列必须具有相同的X坐标才能正确渲染。
 * \endif
 */
void QImPlotShadedItemNode::setData(QImAbstractXYDataSeries* series1, QImAbstractXYDataSeries* series2)
{
    QIM_D(d);
    d->data.reset(series1);
    d->data2.reset(series2);
    emit dataChanged();
}

/**
 * \if ENGLISH
 * @brief Set two-line fill data from X, Y1, and Y2 containers
 * @param x X coordinates container
 * @param y1 Primary Y values container (lower bound)
 * @param y2 Secondary Y values container (upper bound)
 * @details Creates two data series sharing the same X coordinates.
 *          The area between y1 and y2 is filled.
 * \endif
 *
 * \if CHINESE
 * @brief 从X、Y1和Y2容器设置双线填充数据
 * @param x X坐标容器
 * @param y1 主Y值容器（下边界）
 * @param y2 辅助Y值容器（上边界）
 * @details 创建两个共享相同X坐标的数据系列。
 *          y1和y2之间的区域被填充。
 * \endif
 */
template< typename ContainerX, typename ContainerY1, typename ContainerY2 >
void QImPlotShadedItemNode::setData(const ContainerX& x, const ContainerY1& y1, const ContainerY2& y2)
{
    QImAbstractXYDataSeries* d1 = new QImVectorXYDataSeries(x, y1);
    QImAbstractXYDataSeries* d2 = new QImVectorXYDataSeries(x, y2);
    setData(d1, d2);
}

/**
 * \if ENGLISH
 * @brief Set two-line fill data from X, Y1, and Y2 containers (move semantics)
 * @param x X coordinates container
 * @param y1 Primary Y values container (lower bound)
 * @param y2 Secondary Y values container (upper bound)
 * \endif
 *
 * \if CHINESE
 * @brief 从X、Y1和Y2容器设置双线填充数据（移动语义）
 * @param x X坐标容器
 * @param y1 主Y值容器（下边界）
 * @param y2 辅助Y值容器（上边界）
 * \endif
 */
template< typename ContainerX, typename ContainerY1, typename ContainerY2 >
void QImPlotShadedItemNode::setData(ContainerX&& x, ContainerY1&& y1, ContainerY2&& y2)
{
    QImAbstractXYDataSeries* d1 = new QImVectorXYDataSeries(x, y1);
    QImAbstractXYDataSeries* d2 = new QImVectorXYDataSeries(x, y2);
    setData(d1, d2);
}

/**
 * \if ENGLISH
 * @brief Get primary data series
 * @return Pointer to QImAbstractXYDataSeries
 * \endif
 *
 * \if CHINESE
 * @brief 获取主数据系列
 * @return QImAbstractXYDataSeries指针
 * \endif
 */
QImAbstractXYDataSeries* QImPlotShadedItemNode::data() const
{
    return d_ptr->data.get();
}

/**
 * \if ENGLISH
 * @brief Get secondary data series (for two-line fill mode)
 * @return Pointer to QImAbstractXYDataSeries, or nullptr if not in two-line mode
 * \endif
 *
 * \if CHINESE
 * @brief 获取辅助数据系列（用于双线填充模式）
 * @return QImAbstractXYDataSeries指针，如果不在双线模式则返回nullptr
 * \endif
 */
QImAbstractXYDataSeries* QImPlotShadedItemNode::data2() const
{
    return d_ptr->data2.get();
}

/**
 * \if ENGLISH
 * @brief Get reference value for single-line fill mode
 * @return Current reference value
 * \endif
 *
 * \if CHINESE
 * @brief 获取单线填充模式的参考值
 * @return 当前参考值
 * \endif
 */
double QImPlotShadedItemNode::referenceValue() const
{
    QIM_DC(d);
    return d->referenceValue;
}

/**
 * \if ENGLISH
 * @brief Set reference value for single-line fill mode
 * @param value New reference value
 * @details The reference value determines the horizontal line to which the fill extends.
 *          Use -INFINITY for fill extending to negative infinity,
 *          +INFINITY for fill extending to positive infinity.
 * \endif
 *
 * \if CHINESE
 * @brief 设置单线填充模式的参考值
 * @param value 新参考值
 * @details 参考值决定填充延伸到的水平线。
 *          使用-INFINITY表示填充延伸到负无穷，
 *          +INFINITY表示填充延伸到正无穷。
 * \endif
 */
void QImPlotShadedItemNode::setReferenceValue(double value)
{
    QIM_D(d);
    if (d->referenceValue != value) {
        d->referenceValue = value;
        emit referenceValueChanged(value);
    }
}

/**
 * \if ENGLISH
 * @brief Get fill color
 * @return Current QColor
 * \endif
 *
 * \if CHINESE
 * @brief 获取填充颜色
 * @return 当前的QColor
 * \endif
 */
QColor QImPlotShadedItemNode::color() const
{
    return (d_ptr->color.has_value()) ? toQColor(d_ptr->color->value()) : QColor();
}

/**
 * \if ENGLISH
 * @brief Set fill color
 * @param c QColor for the fill
 * \endif
 *
 * \if CHINESE
 * @brief 设置填充颜色
 * @param c 填充的QColor
 * \endif
 */
void QImPlotShadedItemNode::setColor(const QColor& c)
{
    ImVec4 imColor = toImVec4(c);
    if (d_ptr->color.has_value()) {
        d_ptr->color->operator=(imColor);  // Trigger dirty flag via assignment
    } else {
        d_ptr->color.emplace(imColor);
        d_ptr->color->mark_dirty();  // Mark dirty for new color
    }
    emit colorChanged(c);
}

/**
 * \if ENGLISH
 * @brief Get raw ImPlotShadedFlags
 * @return Current ImPlotShadedFlags
 * \endif
 *
 * \if CHINESE
 * @brief 获取原始的ImPlotShadedFlags
 * @return 当前的ImPlotShadedFlags
 * \endif
 */
int QImPlotShadedItemNode::shadedFlags() const
{
    QIM_DC(d);
    return d->flags;
}

/**
 * \if ENGLISH
 * @brief Set raw ImPlotShadedFlags
 * @param flags New ImPlotShadedFlags
 * \endif
 *
 * \if CHINESE
 * @brief 设置原始的ImPlotShadedFlags
 * @param flags 新的ImPlotShadedFlags
 * \endif
 */
void QImPlotShadedItemNode::setShadedFlags(int flags)
{
    QIM_D(d);
    if (d->flags != flags) {
        d->flags = static_cast< ImPlotShadedFlags >(flags);
        emit shadedFlagChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Check if two-line fill mode is active
 * @return true if two-line mode is active (data2 is not null)
 * \endif
 *
 * \if CHINESE
 * @brief 检查是否处于双线填充模式
 * @return 如果处于双线模式（data2不为null）则返回true
 * \endif
 */
bool QImPlotShadedItemNode::isTwoLineMode() const
{
    QIM_DC(d);
    return d->data2 != nullptr;
}

/**
 * \if ENGLISH
 * @brief Begin drawing implementation
 * @return false to prevent endDraw from being called
 * @details Handles both single-line and two-line fill modes.
 *          For single-line mode, fills between data line and reference value.
 *          For two-line mode, fills between two data lines.
 * \endif
 *
 * \if CHINESE
 * @brief 开始绘制实现
 * @return false以防止调用endDraw
 * @details 处理单线和双线填充模式。
 *          单线模式下，填充数据线与参考值之间的区域。
 *          双线模式下，填充两条数据线之间的区域。
 * \endif
 */
bool QImPlotShadedItemNode::beginDraw()
{
    QIM_D(d);
    if (!d->data || d->data->size() == 0) {
        return false;
    }

    // Apply fill style every frame (Shaded uses fill, not line style)
    // ImPlot::PlotShaded renders a filled area, so use SetNextFillStyle
    if (d->color.has_value()) {
        ImPlot::SetNextFillStyle(d->color->value());
        d->color->clear();  // Clear dirty flag after applying
    }

    // Determine if we're in two-line mode
    bool twoLineMode = (d->data2 != nullptr && d->data2->size() > 0);

    if (twoLineMode) {
        // Two-line fill mode: fill between two lines
        if (d->data->isContiguous() && d->data2->isContiguous()) {
            // Continuous memory mode: use zero-copy fast path
            const double* xData = d->data->xRawData();
            const double* y1Data = d->data->yRawData();
            const double* y2Data = d->data2->yRawData();
            int size = d->data->size();

            if (xData) {
                // XY mode with explicit X coordinates
                ImPlot::PlotShaded(
                    labelConstData(),
                    xData,
                    y1Data,
                    y2Data,
                    size,
                    d->flags,
                    0,
                    sizeof(double));
            } else {
                // Y-only mode - need to create X values based on index
                std::vector< double > xValues(size);
                double xStart = d->data->xStart();
                double xScale = d->data->xScale();
                for (int i = 0; i < size; ++i) {
                    xValues[i] = xStart + i * xScale;
                }
                ImPlot::PlotShaded(
                    labelConstData(),
                    xValues.data(),
                    y1Data,
                    y2Data,
                    size,
                    d->flags,
                    0,
                    sizeof(double));
            }
        } else {
            // Non-contiguous memory mode: use callback
            // For two-line mode with callbacks, we need to use PlotShadedG
            struct ShadedData {
                QImAbstractXYDataSeries* series1;
                QImAbstractXYDataSeries* series2;
            };
            ShadedData shadedData{ d->data.get(), d->data2.get() };

            ImPlot::PlotShadedG(
                labelConstData(),
                [](int idx, void* data) -> ImPlotPoint {
                    ShadedData* sd = static_cast< ShadedData* >(data);
                    return ImPlotPoint(sd->series1->xValue(idx), sd->series1->yValue(idx));
                },
                &shadedData,
                [](int idx, void* data) -> ImPlotPoint {
                    ShadedData* sd = static_cast< ShadedData* >(data);
                    return ImPlotPoint(sd->series2->xValue(idx), sd->series2->yValue(idx));
                },
                &shadedData,
                d->data->size(),
                d->flags);
        }
    } else {
        // Single-line fill mode: fill between data line and reference value
        if (d->data->isContiguous()) {
            // Continuous memory mode: use zero-copy fast path
            const double* xData = d->data->xRawData();
            const double* yData = d->data->yRawData();
            int size = d->data->size();

            if (xData) {
                // XY mode with explicit X coordinates
                ImPlot::PlotShaded(
                    labelConstData(),
                    xData,
                    yData,
                    size,
                    d->referenceValue,
                    d->flags,
                    0,
                    sizeof(double));
            } else {
                // Y-only mode
                ImPlot::PlotShaded(
                    labelConstData(),
                    yData,
                    size,
                    d->referenceValue,
                    d->data->xScale(),
                    d->data->xStart(),
                    d->flags,
                    0,
                    sizeof(double));
            }
        } else {
            // Non-contiguous memory mode: use callback
            ImPlot::PlotShadedG(
                labelConstData(),
                [](int idx, void* data) -> ImPlotPoint {
                    QImAbstractXYDataSeries* series = static_cast< QImAbstractXYDataSeries* >(data);
                    return ImPlotPoint(series->xValue(idx), series->yValue(idx));
                },
                d->data.get(),
                [](int idx, void* data) -> ImPlotPoint {
                    // For single-line mode, the second getter returns the reference value
                    QImPlotShadedItemNode::PrivateData* pd = static_cast< QImPlotShadedItemNode::PrivateData* >(data);
                    QImAbstractXYDataSeries* series = pd->data.get();
                    return ImPlotPoint(series->xValue(idx), pd->referenceValue);
                },
                d,
                d->data->size(),
                d->flags);
        }
    }

    // Update item status
    ImPlotContext* ct = ImPlot::GetCurrentContext();
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
