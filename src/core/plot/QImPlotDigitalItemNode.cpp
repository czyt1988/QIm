#include "QImPlotDigitalItemNode.h"
#include <optional>
#include "implot.h"
#include "implot_internal.h"
#include "QImTrackedValue.hpp"
#include "../QtImGuiUtils.h"

namespace QIM
{

class QImPlotDigitalItemNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlotDigitalItemNode)
public:
    PrivateData(QImPlotDigitalItemNode* p);

    std::unique_ptr<QImAbstractXYDataSeries> data;  ///< Data series (X, Y values)
    ImPlotDigitalFlags flags { ImPlotDigitalFlags_None };
    // Style tracking values
    std::optional<QImTrackedValue<ImVec4, QIM::ImVecComparator<ImVec4>>> color;
};

QImPlotDigitalItemNode::PrivateData::PrivateData(QImPlotDigitalItemNode* p) : q_ptr(p)
{
}

/**
 * \if ENGLISH
 * @brief Constructor for QImPlotDigitalItemNode
 * @param parent Parent QObject
 * \endif
 *
 * \if CHINESE
 * @brief QImPlotDigitalItemNode的构造函数
 * @param parent 父QObject
 * \endif
 */
QImPlotDigitalItemNode::QImPlotDigitalItemNode(QObject* parent) 
    : QImPlotItemNode(parent), QIM_PIMPL_CONSTRUCT
{
}

/**
 * \if ENGLISH
 * @brief Destructor for QImPlotDigitalItemNode
 * \endif
 *
 * \if CHINESE
 * @brief QImPlotDigitalItemNode的析构函数
 * \endif
 */
QImPlotDigitalItemNode::~QImPlotDigitalItemNode()
{
}

/**
 * \if ENGLISH
 * @brief Set data series for digital signal
 * @param series Pointer to QImAbstractXYDataSeries containing X, Y values
 * \endif
 *
 * \if CHINESE
 * @brief 设置数字信号的数据系列
 * @param series 包含X、Y值的QImAbstractXYDataSeries指针
 * \endif
 */
void QImPlotDigitalItemNode::setData(QImAbstractXYDataSeries* series)
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
QImAbstractXYDataSeries* QImPlotDigitalItemNode::data() const
{
    return d_ptr->data.get();
}

/**
 * \if ENGLISH
 * @brief Get digital signal color
 * @return Current QColor
 * \endif
 *
 * \if CHINESE
 * @brief 获取数字信号颜色
 * @return 当前的QColor
 * \endif
 */
QColor QImPlotDigitalItemNode::color() const
{
    return (d_ptr->color.has_value()) ? toQColor(d_ptr->color->value()) : QColor();
}

/**
 * \if ENGLISH
 * @brief Set digital signal color
 * @param c QColor for the digital signal lines
 * \endif
 *
 * \if CHINESE
 * @brief 设置数字信号颜色
 * @param c 数字信号线的QColor
 * \endif
 */
void QImPlotDigitalItemNode::setColor(const QColor& c)
{
    d_ptr->color = toImVec4(c);
    emit colorChanged(c);
}

/**
 * \if ENGLISH
 * @brief Get raw ImPlotDigitalFlags
 * @return Current ImPlotDigitalFlags
 * \endif
 *
 * \if CHINESE
 * @brief 获取原始的ImPlotDigitalFlags
 * @return 当前的ImPlotDigitalFlags
 * \endif
 */
int QImPlotDigitalItemNode::digitalFlags() const
{
    QIM_DC(d);
    return d->flags;
}

/**
 * \if ENGLISH
 * @brief Set raw ImPlotDigitalFlags
 * @param flags New ImPlotDigitalFlags
 * \endif
 *
 * \if CHINESE
 * @brief 设置原始的ImPlotDigitalFlags
 * @param flags 新的ImPlotDigitalFlags
 * \endif
 */
void QImPlotDigitalItemNode::setDigitalFlags(int flags)
{
    QIM_D(d);
    if (d->flags != flags) {
        d->flags = static_cast<ImPlotDigitalFlags>(flags);
        emit digitalFlagChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Begin drawing implementation
 * @return false to prevent endDraw from being called
 * @details Calls ImPlot::PlotDigital with appropriate parameters.
 * \endif
 *
 * \if CHINESE
 * @brief 开始绘制实现
 * @return false以防止调用endDraw
 * @details 使用适当的参数调用ImPlot::PlotDigital。
 * \endif
 */
bool QImPlotDigitalItemNode::beginDraw()
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
            ImPlot::PlotDigital(
                labelConstData(),
                xData,
                yData,
                size,
                d->flags,
                0,
                sizeof(double));
        } else {
            // Y-only mode
            // PlotDigital does not have Y-only overload, need to convert to XY.
            // We'll generate X values using xStart and xScale.
            std::vector<double> xValues(size);
            double xStart = d->data->xStart();
            double xScale = d->data->xScale();
            for (int i = 0; i < size; ++i) {
                xValues[i] = xStart + i * xScale;
            }
            ImPlot::PlotDigital(
                labelConstData(),
                xValues.data(),
                yData,
                size,
                d->flags,
                0,
                sizeof(double));
        }
    } else {
        // Non-contiguous memory mode: copy data
        int size = d->data->size();
        std::vector<double> xValues(size);
        std::vector<double> yValues(size);
        for (int i = 0; i < size; ++i) {
            xValues[i] = d->data->xValue(i);
            yValues[i] = d->data->yValue(i);
        }
        ImPlot::PlotDigital(
            labelConstData(),
            xValues.data(),
            yValues.data(),
            size,
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