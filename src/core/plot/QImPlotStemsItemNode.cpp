#include "QImPlotStemsItemNode.h"
#include <optional>
#include "implot.h"
#include "implot_internal.h"
#include "QImTrackedValue.hpp"
#include "QtImGuiUtils.h"

namespace QIM
{

class QImPlotStemsItemNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlotStemsItemNode)
public:
    PrivateData(QImPlotStemsItemNode* p);

    std::unique_ptr<QImAbstractXYDataSeries> data;  ///< Data series (X, Y values)
    ImPlotStemsFlags flags { ImPlotStemsFlags_None };
    double referenceValue { 0.0 };  ///< Reference value (baseline)
    // Style tracking values
    std::optional<QImTrackedValue<ImVec4, QIM::ImVecComparator<ImVec4>>> color;
};

QImPlotStemsItemNode::PrivateData::PrivateData(QImPlotStemsItemNode* p) : q_ptr(p)
{
}

/**
 * \if ENGLISH
 * @brief Constructor for QImPlotStemsItemNode
 * @param parent Parent QObject
 * \endif
 *
 * \if CHINESE
 * @brief QImPlotStemsItemNode的构造函数
 * @param parent 父QObject
 * \endif
 */
QImPlotStemsItemNode::QImPlotStemsItemNode(QObject* parent) 
    : QImPlotItemNode(parent), QIM_PIMPL_CONSTRUCT
{
}

/**
 * \if ENGLISH
 * @brief Destructor for QImPlotStemsItemNode
 * \endif
 *
 * \if CHINESE
 * @brief QImPlotStemsItemNode的析构函数
 * \endif
 */
QImPlotStemsItemNode::~QImPlotStemsItemNode()
{
}

/**
 * \if ENGLISH
 * @brief Set data series for stems
 * @param series Pointer to QImAbstractXYDataSeries containing X, Y values
 * \endif
 *
 * \if CHINESE
 * @brief 设置茎叶图的数据系列
 * @param series 包含X、Y值的QImAbstractXYDataSeries指针
 * \endif
 */
void QImPlotStemsItemNode::setData(QImAbstractXYDataSeries* series)
{
    QIM_D(d);
    d->data.reset(series);
    emit dataChanged();
}

/**
 * \if ENGLISH
 * @brief Get the data series
 * @return Pointer to QImAbstractXYDataSeries
 * \endif
 *
 * \if CHINESE
 * @brief 获取数据系列
 * @return QImAbstractXYDataSeries指针
 * \endif
 */
QImAbstractXYDataSeries* QImPlotStemsItemNode::data() const
{
    return d_ptr->data.get();
}

/**
 * \if ENGLISH
 * @brief Get reference value (baseline)
 * @return Current reference value
 * \endif
 *
 * \if CHINESE
 * @brief 获取参考值（基线）
 * @return 当前参考值
 * \endif
 */
double QImPlotStemsItemNode::referenceValue() const
{
    QIM_DC(d);
    return d->referenceValue;
}

/**
 * \if ENGLISH
 * @brief Set reference value (baseline)
 * @param value New reference value
 * \endif
 *
 * \if CHINESE
 * @brief 设置参考值（基线）
 * @param value 新参考值
 * \endif
 */
void QImPlotStemsItemNode::setReferenceValue(double value)
{
    QIM_D(d);
    if (d->referenceValue != value) {
        d->referenceValue = value;
        emit referenceValueChanged(value);
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
bool QImPlotStemsItemNode::isHorizontal() const
{
    QIM_DC(d);
    return (d->flags & ImPlotStemsFlags_Horizontal) != 0;
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
void QImPlotStemsItemNode::setHorizontal(bool horizontal)
{
    QIM_D(d);
    const ImPlotStemsFlags oldFlags = d->flags;
    if (horizontal) {
        d->flags |= ImPlotStemsFlags_Horizontal;
    } else {
        d->flags &= ~ImPlotStemsFlags_Horizontal;
    }
    if (d->flags != oldFlags) {
        emit orientationChanged(horizontal);
        emit stemsFlagChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Get stems color
 * @return Current QColor
 * \endif
 *
 * \if CHINESE
 * @brief 获取茎叶图颜色
 * @return 当前的QColor
 * \endif
 */
QColor QImPlotStemsItemNode::color() const
{
    return (d_ptr->color.has_value()) ? toQColor(d_ptr->color->value()) : QColor();
}

/**
 * \if ENGLISH
 * @brief Set stems color
 * @param c QColor for the stems
 * \endif
 *
 * \if CHINESE
 * @brief 设置茎叶图颜色
 * @param c 茎叶图的QColor
 * \endif
 */
void QImPlotStemsItemNode::setColor(const QColor& c)
{
    d_ptr->color = toImVec4(c);
    emit colorChanged(c);
}

/**
 * \if ENGLISH
 * @brief Get raw ImPlotStemsFlags
 * @return Current ImPlotStemsFlags
 * \endif
 *
 * \if CHINESE
 * @brief 获取原始的ImPlotStemsFlags
 * @return 当前的ImPlotStemsFlags
 * \endif
 */
int QImPlotStemsItemNode::stemsFlags() const
{
    QIM_DC(d);
    return d->flags;
}

/**
 * \if ENGLISH
 * @brief Set raw ImPlotStemsFlags
 * @param flags New ImPlotStemsFlags
 * \endif
 *
 * \if CHINESE
 * @brief 设置原始的ImPlotStemsFlags
 * @param flags 新的ImPlotStemsFlags
 * \endif
 */
void QImPlotStemsItemNode::setStemsFlags(int flags)
{
    QIM_D(d);
    if (d->flags != flags) {
        d->flags = static_cast<ImPlotStemsFlags>(flags);
        emit stemsFlagChanged();
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
bool QImPlotStemsItemNode::beginDraw()
{
    QIM_D(d);
    if (!d->data || d->data->size() == 0) {
        return false;
    }

    // Apply style
    if (d->color && d->color->is_dirty()) {
        ImPlot::SetNextLineStyle(d->color->value());
    }

    // Call ImPlot API
    if (d->data->isContiguous()) {
        // Continuous memory mode: use zero-copy fast path
        const double* xData = d->data->xRawData();
        const double* yData = d->data->yRawData();
        int size = d->data->size();

        if (xData) {
            // XY mode with explicit X coordinates
            ImPlot::PlotStems(
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
            ImPlot::PlotStems(
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
        // Note: ImPlot doesn't have PlotStemsG, so we need to copy data
        int size = d->data->size();
        std::vector<double> xValues(size);
        std::vector<double> yValues(size);
        for (int i = 0; i < size; ++i) {
            xValues[i] = d->data->xValue(i);
            yValues[i] = d->data->yValue(i);
        }
        ImPlot::PlotStems(
            labelConstData(),
            xValues.data(),
            yValues.data(),
            size,
            d->referenceValue,
            d->flags,
            0,
            sizeof(double));
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
