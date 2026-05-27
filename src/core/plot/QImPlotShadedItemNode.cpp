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

    std::unique_ptr< QImAbstractXYDataSeries > data2;  ///< Secondary data series (for two-line mode)
    ImPlotShadedFlags flags { ImPlotShadedFlags_None };
    double referenceValue { 0.0 };  ///< Reference value for single-line fill mode
    // Style tracking values
    std::optional< QImTrackedValue< ImVec4, QIM::ImVecComparator< ImVec4 > > > color;
};

QImPlotShadedItemNode::PrivateData::PrivateData(QImPlotShadedItemNode* p) : q_ptr(p)
{
}

QImPlotShadedItemNode::QImPlotShadedItemNode(QObject* parent) : QImAbstractXYSeriesItemNode(parent), QIM_PIMPL_CONSTRUCT
{
}

QImPlotShadedItemNode::~QImPlotShadedItemNode()
{
}

/**
 * \if ENGLISH
 * @brief Set data series for single-line fill mode (override)
 * @param[in] series Pointer to QImAbstractXYDataSeries (ownership transferred)
 * @details In single-line mode, the area between the data line and referenceValue is filled.
 *          Calls base class setData() and clears any existing secondary data series.
 * \endif
 *
 * \if CHINESE
 * @brief 设置单线填充模式的数据系列（重写）
 * @param[in] series QImAbstractXYDataSeries指针（所有权转移）
 * @details 在单线模式下，数据线与referenceValue之间的区域被填充。
 *          调用基类setData()并清除辅助数据系列（用于双线模式）。
 * \endif
 */
void QImPlotShadedItemNode::setData(QImAbstractXYDataSeries* series)
{
    QIM_D(d);
    QImAbstractXYSeriesItemNode::setData(series);
    d->data2.reset();
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
    QImAbstractXYSeriesItemNode::setData(series1);
    d->data2.reset(series2);
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
    QIM_DC(d);
    return d->data2.get();
}

double QImPlotShadedItemNode::referenceValue() const
{
    QIM_DC(d);
    return d->referenceValue;
}

void QImPlotShadedItemNode::setReferenceValue(double value)
{
    QIM_D(d);
    if (d->referenceValue != value) {
        d->referenceValue = value;
        Q_EMIT referenceValueChanged(value);
    }
}

QColor QImPlotShadedItemNode::color() const
{
    QIM_DC(d);
    return (d->color.has_value()) ? toQColor(d->color->value()) : QColor();
}

void QImPlotShadedItemNode::setColor(const QColor& c)
{
    QIM_D(d);
    ImVec4 imColor = toImVec4(c);
    if (d->color.has_value()) {
        d->color->operator=(imColor);  // Trigger dirty flag via assignment
    } else {
        d->color.emplace(imColor);
        d->color->mark_dirty();  // Mark dirty for new color
    }
    Q_EMIT colorChanged(c);
}

int QImPlotShadedItemNode::shadedFlags() const
{
    QIM_DC(d);
    return d->flags;
}

void QImPlotShadedItemNode::setShadedFlags(int flags)
{
    QIM_D(d);
    if (d->flags != flags) {
        d->flags = static_cast< ImPlotShadedFlags >(flags);
        Q_EMIT shadedFlagChanged();
    }
}

bool QImPlotShadedItemNode::isTwoLineMode() const
{
    QIM_DC(d);
    return d->data2 != nullptr;
}

bool QImPlotShadedItemNode::beginDraw()
{
    QIM_D(d);
    QImAbstractXYDataSeries* rawData = this->data();
    if (!rawData || rawData->size() == 0) {
        return false;
    }

    // Apply fill style every frame (Shaded uses fill, not line style)
    if (d->color.has_value()) {
        ImPlot::SetNextFillStyle(d->color->value());
        d->color->clear();  // Clear dirty flag after applying
    }

    // Determine if we're in two-line mode
    bool twoLineMode = (d->data2 != nullptr && d->data2->size() > 0);

    if (twoLineMode) {
        // Two-line fill mode: fill between two lines
        if (rawData->isContiguous() && d->data2->isContiguous()) {
            const double* xData = rawData->xRawData();
            const double* y1Data = rawData->yRawData();
            const double* y2Data = d->data2->yRawData();
            int size = rawData->size();

            if (xData) {
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
                std::vector< double > xValues(size);
                double xStart = rawData->xStart();
                double xScale = rawData->xScale();
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
            struct ShadedData {
                QImAbstractXYDataSeries* series1;
                QImAbstractXYDataSeries* series2;
            };
            ShadedData shadedData{ rawData, d->data2.get() };

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
                rawData->size(),
                d->flags);
        }
    } else {
        // Single-line fill mode: fill between data line and reference value
        if (rawData->isContiguous()) {
            const double* xData = rawData->xRawData();
            const double* yData = rawData->yRawData();
            int size = rawData->size();

            if (xData) {
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
                ImPlot::PlotShaded(
                    labelConstData(),
                    yData,
                    size,
                    d->referenceValue,
                    rawData->xScale(),
                    rawData->xStart(),
                    d->flags,
                    0,
                    sizeof(double));
            }
        } else {
            ImPlot::PlotShadedG(
                labelConstData(),
                [](int idx, void* data) -> ImPlotPoint {
                    QImAbstractXYDataSeries* series = static_cast< QImAbstractXYDataSeries* >(data);
                    return ImPlotPoint(series->xValue(idx), series->yValue(idx));
                },
                rawData,
                [](int idx, void* data) -> ImPlotPoint {
                    QImPlotShadedItemNode::PrivateData* pd = static_cast< QImPlotShadedItemNode::PrivateData* >(data);
                    QImAbstractXYDataSeries* series = pd->data2 ? pd->data2.get() : static_cast<QImAbstractXYDataSeries*>(pd->q_func()->data());
                    return ImPlotPoint(series->xValue(idx), pd->referenceValue);
                },
                d,
                rawData->size(),
                d->flags);
        }
    }

    // Update item status
    if(!imPlotItem()){
        ImPlotContext* ct = ImPlot::GetCurrentContext();
        if (!ct){ 
            return false;
        }
        ImPlotItem* plotItem = ct->PreviousItem;
        setImPlotItem(plotItem);
        if (plotItem && plotItem->Show != QImAbstractNode::isVisible()) {
            QImAbstractNode::setVisible(plotItem->Show);
        }
    }
    if (!d->color) {
        d->color = ImPlot::GetLastItemColor();
    }

    return false;
}

}  // namespace QIM