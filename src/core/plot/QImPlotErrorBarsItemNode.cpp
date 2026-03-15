#include "QImPlotErrorBarsItemNode.h"
#include <optional>
#include "implot.h"
#include "implot_internal.h"
#include "QImTrackedValue.hpp"
#include "QtImGuiUtils.h"

namespace QIM
{

class QImPlotErrorBarsItemNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlotErrorBarsItemNode)
public:
    PrivateData(QImPlotErrorBarsItemNode* p);

    std::unique_ptr<QImAbstractErrorDataSeries> data;  ///< Error data series (X, Y, errors)
    ImPlotErrorBarsFlags flags { ImPlotErrorBarsFlags_None };
    // Style tracking values
    std::optional<QImTrackedValue<ImVec4, QIM::ImVecComparator<ImVec4>>> color;
};

QImPlotErrorBarsItemNode::PrivateData::PrivateData(QImPlotErrorBarsItemNode* p) : q_ptr(p)
{
}

/**
 * \if ENGLISH
 * @brief Constructor for QImPlotErrorBarsItemNode
 * @param parent Parent QObject
 * \endif
 *
 * \if CHINESE
 * @brief QImPlotErrorBarsItemNode的构造函数
 * @param parent 父QObject
 * \endif
 */
QImPlotErrorBarsItemNode::QImPlotErrorBarsItemNode(QObject* parent) 
    : QImPlotItemNode(parent), QIM_PIMPL_CONSTRUCT
{
}

/**
 * \if ENGLISH
 * @brief Destructor for QImPlotErrorBarsItemNode
 * \endif
 *
 * \if CHINESE
 * @brief QImPlotErrorBarsItemNode的析构函数
 * \endif
 */
QImPlotErrorBarsItemNode::~QImPlotErrorBarsItemNode()
{
}

/**
 * \if ENGLISH
 * @brief Set error data series
 * @param errorDataSeries Pointer to QImAbstractErrorDataSeries
 * \endif
 *
 * \if CHINESE
 * @brief 设置误差数据系列
 * @param errorDataSeries QImAbstractErrorDataSeries指针
 * \endif
 */
void QImPlotErrorBarsItemNode::setData(QImAbstractErrorDataSeries* errorDataSeries)
{
    QIM_D(d);
    d->data.reset(errorDataSeries);
    emit dataChanged();
}

/**
 * \if ENGLISH
 * @brief Get the error data series
 * @return Pointer to QImAbstractErrorDataSeries
 * \endif
 *
 * \if CHINESE
 * @brief 获取误差数据系列
 * @return QImAbstractErrorDataSeries指针
 * \endif
 */
QImAbstractErrorDataSeries* QImPlotErrorBarsItemNode::data() const
{
    return d_ptr->data.get();
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
bool QImPlotErrorBarsItemNode::isHorizontal() const
{
    QIM_DC(d);
    return (d->flags & ImPlotErrorBarsFlags_Horizontal) != 0;
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
void QImPlotErrorBarsItemNode::setHorizontal(bool horizontal)
{
    QIM_D(d);
    const ImPlotErrorBarsFlags oldFlags = d->flags;
    if (horizontal) {
        d->flags |= ImPlotErrorBarsFlags_Horizontal;
    } else {
        d->flags &= ~ImPlotErrorBarsFlags_Horizontal;
    }
    if (d->flags != oldFlags) {
        emit orientationChanged(horizontal);
        emit errorBarsFlagChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Get error bar color
 * @return Current QColor
 * \endif
 *
 * \if CHINESE
 * @brief 获取误差棒颜色
 * @return 当前的QColor
 * \endif
 */
QColor QImPlotErrorBarsItemNode::color() const
{
    return (d_ptr->color.has_value()) ? toQColor(d_ptr->color->value()) : QColor();
}

/**
 * \if ENGLISH
 * @brief Set error bar color
 * @param c QColor for the error bars
 * \endif
 *
 * \if CHINESE
 * @brief 设置误差棒颜色
 * @param c 误差棒的QColor
 * \endif
 */
void QImPlotErrorBarsItemNode::setColor(const QColor& c)
{
    d_ptr->color = toImVec4(c);
    emit colorChanged(c);
}

/**
 * \if ENGLISH
 * @brief Get raw ImPlotErrorBarsFlags
 * @return Current ImPlotErrorBarsFlags
 * \endif
 *
 * \if CHINESE
 * @brief 获取原始的ImPlotErrorBarsFlags
 * @return 当前的ImPlotErrorBarsFlags
 * \endif
 */
int QImPlotErrorBarsItemNode::errorBarsFlags() const
{
    QIM_DC(d);
    return d->flags;
}

/**
 * \if ENGLISH
 * @brief Set raw ImPlotErrorBarsFlags
 * @param flags New ImPlotErrorBarsFlags
 * \endif
 *
 * \if CHINESE
 * @brief 设置原始的ImPlotErrorBarsFlags
 * @param flags 新的ImPlotErrorBarsFlags
 * \endif
 */
void QImPlotErrorBarsItemNode::setErrorBarsFlags(int flags)
{
    QIM_D(d);
    if (d->flags != flags) {
        d->flags = static_cast<ImPlotErrorBarsFlags>(flags);
        emit errorBarsFlagChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Check if asymmetric error mode is active
 * @return true if using different positive and negative errors
 * \endif
 *
 * \if CHINESE
 * @brief 检查是否处于非对称误差模式
 * @return 如果使用不同的正负误差则返回true
 * \endif
 */
bool QImPlotErrorBarsItemNode::isAsymmetricMode() const
{
    QIM_DC(d);
    return d->data && d->data->isAsymmetric();
}

/**
 * \if ENGLISH
 * @brief Begin drawing implementation
 * @return false to prevent endDraw from being called
 * @details Handles both symmetric and asymmetric error modes,
 *          vertical and horizontal orientations.
 * \endif
 *
 * \if CHINESE
 * @brief 开始绘制实现
 * @return false以防止调用endDraw
 * @details 处理对称和非对称误差模式，垂直和水平方向。
 * \endif
 */
bool QImPlotErrorBarsItemNode::beginDraw()
{
    QIM_D(d);
    if (!d->data || d->data->size() == 0) {
        return false;
    }

    int dataSize = d->data->size();
    
    // Validate error array sizes
    if (d->data->isAsymmetric()) {
        // For asymmetric mode, we need both neg and pos errors
        // The data series should handle this internally
    }

    // Apply style
    if (d->color && d->color->is_dirty()) {
        ImPlot::SetNextLineStyle(d->color->value());
    }

    // Get raw pointers for fast rendering
    const double* xData = d->data->xRawData();
    const double* yData = d->data->yRawData();
    const double* negErrorData = d->data->negErrorRawData();
    const double* posErrorData = d->data->posErrorRawData();

    // Call ImPlot API
    if (xData && yData && negErrorData && posErrorData) {
        // Fast path: all data is contiguous
        if (d->data->isAsymmetric()) {
            ImPlot::PlotErrorBars(
                labelConstData(),
                xData,
                yData,
                negErrorData,
                posErrorData,
                dataSize,
                d->flags,
                0,
                sizeof(double));
        } else {
            ImPlot::PlotErrorBars(
                labelConstData(),
                xData,
                yData,
                posErrorData,
                dataSize,
                d->flags,
                0,
                sizeof(double));
        }
    } else {
        // Slow path: need to copy data to temporary buffers
        std::vector<double> xValues(dataSize);
        std::vector<double> yValues(dataSize);
        std::vector<double> negErrors(dataSize);
        std::vector<double> posErrors(dataSize);
        
        for (int i = 0; i < dataSize; ++i) {
            xValues[i] = d->data->xValue(i);
            yValues[i] = d->data->yValue(i);
            negErrors[i] = d->data->negError(i);
            posErrors[i] = d->data->posError(i);
        }
        
        if (d->data->isAsymmetric()) {
            ImPlot::PlotErrorBars(
                labelConstData(),
                xValues.data(),
                yValues.data(),
                negErrors.data(),
                posErrors.data(),
                dataSize,
                d->flags,
                0,
                sizeof(double));
        } else {
            ImPlot::PlotErrorBars(
                labelConstData(),
                xValues.data(),
                yValues.data(),
                posErrors.data(),
                dataSize,
                d->flags,
                0,
                sizeof(double));
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
