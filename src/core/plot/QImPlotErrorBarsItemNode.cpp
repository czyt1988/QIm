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

    std::unique_ptr<QImAbstractXYDataSeries> data;  ///< Data series (X, Y values)
    std::vector<double> posErrors;                  ///< Positive error values
    std::vector<double> negErrors;                  ///< Negative error values (for asymmetric mode)
    bool asymmetricMode { false };                  ///< True if using different pos/neg errors
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
 * @brief Set data series with symmetric error values
 * @param dataSeries Pointer to QImAbstractXYDataSeries containing X, Y values
 * @param errors Vector of error values (same for positive and negative)
 * \endif
 *
 * \if CHINESE
 * @brief 设置带有对称误差值的数据系列
 * @param dataSeries 包含X、Y值的QImAbstractXYDataSeries指针
 * @param errors 误差值向量（正负误差相同）
 * \endif
 */
void QImPlotErrorBarsItemNode::setData(QImAbstractXYDataSeries* dataSeries, const std::vector<double>& errors)
{
    QIM_D(d);
    d->data.reset(dataSeries);
    d->posErrors = errors;
    d->negErrors.clear();
    d->asymmetricMode = false;
    emit dataChanged();
}

/**
 * \if ENGLISH
 * @brief Set data series with asymmetric error values
 * @param dataSeries Pointer to QImAbstractXYDataSeries containing X, Y values
 * @param negErrors Vector of negative error values
 * @param posErrors Vector of positive error values
 * \endif
 *
 * \if CHINESE
 * @brief 设置带有非对称误差值的数据系列
 * @param dataSeries 包含X、Y值的QImAbstractXYDataSeries指针
 * @param negErrors 负误差值向量
 * @param posErrors 正误差值向量
 * \endif
 */
void QImPlotErrorBarsItemNode::setData(QImAbstractXYDataSeries* dataSeries, const std::vector<double>& negErrors, const std::vector<double>& posErrors)
{
    QIM_D(d);
    d->data.reset(dataSeries);
    d->negErrors = negErrors;
    d->posErrors = posErrors;
    d->asymmetricMode = true;
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
QImAbstractXYDataSeries* QImPlotErrorBarsItemNode::data() const
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
    return d->asymmetricMode;
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
    if (d->asymmetricMode) {
        if (static_cast<int>(d->negErrors.size()) < dataSize || static_cast<int>(d->posErrors.size()) < dataSize) {
            return false;
        }
    } else {
        if (static_cast<int>(d->posErrors.size()) < dataSize) {
            return false;
        }
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

        if (xData) {
            // XY mode with explicit X coordinates
            if (d->asymmetricMode) {
                ImPlot::PlotErrorBars(
                    labelConstData(),
                    xData,
                    yData,
                    d->negErrors.data(),
                    d->posErrors.data(),
                    dataSize,
                    d->flags,
                    0,
                    sizeof(double));
            } else {
                ImPlot::PlotErrorBars(
                    labelConstData(),
                    xData,
                    yData,
                    d->posErrors.data(),
                    dataSize,
                    d->flags,
                    0,
                    sizeof(double));
            }
        } else {
            // Y-only mode - need to create X values based on index
            std::vector<double> xValues(dataSize);
            double xStart = d->data->xStart();
            double xScale = d->data->xScale();
            for (int i = 0; i < dataSize; ++i) {
                xValues[i] = xStart + i * xScale;
            }
            
            if (d->asymmetricMode) {
                ImPlot::PlotErrorBars(
                    labelConstData(),
                    xValues.data(),
                    yData,
                    d->negErrors.data(),
                    d->posErrors.data(),
                    dataSize,
                    d->flags,
                    0,
                    sizeof(double));
            } else {
                ImPlot::PlotErrorBars(
                    labelConstData(),
                    xValues.data(),
                    yData,
                    d->posErrors.data(),
                    dataSize,
                    d->flags,
                    0,
                    sizeof(double));
            }
        }
    } else {
        // Non-contiguous memory mode: not supported for error bars
        // Error bars require contiguous memory for efficient rendering
        return false;
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
