#include "QImPlotHeatmapItemNode.h"
#include <memory>
#include <optional>
#include "implot.h"
#include "implot_internal.h"
#include "QImTrackedValue.hpp"
#include "QtImGuiUtils.h"
#include <QDebug>

namespace QIM
{

class QImPlotHeatmapItemNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlotHeatmapItemNode)
public:
    PrivateData(QImPlotHeatmapItemNode* p);

    std::unique_ptr< QImAbstractHeatmapDataSeries > data;
    ImPlotHeatmapFlags flags { ImPlotHeatmapFlags_None };
    double scaleMin { 0.0 };
    double scaleMax { 0.0 };
    QByteArray labelFormatUtf8 { "%.1f" }; ///< Label format (UTF8, used directly by ImGui)
    QPointF boundsMin { 0.0, 0.0 };
    QPointF boundsMax { 1.0, 1.0 };
    // Style tracking values
    std::optional< QImTrackedValue< ImVec4, QIM::ImVecComparator< ImVec4 > > > color;
};

QImPlotHeatmapItemNode::PrivateData::PrivateData(QImPlotHeatmapItemNode* p) : q_ptr(p)
{
}

/**
 * \if ENGLISH
 * @brief Constructor for QImPlotHeatmapItemNode
 * @param parent Parent QObject
 * \endif
 *
 * \if CHINESE
 * @brief QImPlotHeatmapItemNode的构造函数
 * @param parent 父QObject
 * \endif
 */
QImPlotHeatmapItemNode::QImPlotHeatmapItemNode(QObject* parent) : QImPlotItemNode(parent), QIM_PIMPL_CONSTRUCT
{
}

/**
 * \if ENGLISH
 * @brief Destructor for QImPlotHeatmapItemNode
 * \endif
 *
 * \if CHINESE
 * @brief QImPlotHeatmapItemNode的析构函数
 * \endif
 */
QImPlotHeatmapItemNode::~QImPlotHeatmapItemNode()
{
}

/**
 * \if ENGLISH
 * @brief Set data series for the heatmap
 * @param series Pointer to QImAbstractHeatmapDataSeries
 * \endif
 *
 * \if CHINESE
 * @brief 设置热力图的数据系列
 * @param series QImAbstractHeatmapDataSeries指针
 * \endif
 */
void QImPlotHeatmapItemNode::setData(QImAbstractHeatmapDataSeries* series)
{
    QIM_D(d);
    d->data.reset(series);
    emit dataChanged();
}

/**
 * \if ENGLISH
 * @brief Get current data series
 * @return Pointer to QImAbstractHeatmapDataSeries
 * \endif
 *
 * \if CHINESE
 * @brief 获取当前数据系列
 * @return QImAbstractHeatmapDataSeries指针
 * \endif
 */
QImAbstractHeatmapDataSeries* QImPlotHeatmapItemNode::data() const
{
    return d_ptr->data.get();
}

/**
 * \if ENGLISH
 * @brief Get minimum scale value
 * @return Current scale minimum
 * \endif
 *
 * \if CHINESE
 * @brief 获取最小缩放值
 * @return 当前缩放最小值
 * \endif
 */
double QImPlotHeatmapItemNode::scaleMin() const
{
    QIM_DC(d);
    return d->scaleMin;
}

/**
 * \if ENGLISH
 * @brief Set minimum scale value
 * @param min New scale minimum
 * \endif
 *
 * \if CHINESE
 * @brief 设置最小缩放值
 * @param min 新缩放最小值
 * \endif
 */
void QImPlotHeatmapItemNode::setScaleMin(double min)
{
    QIM_D(d);
    if (d->scaleMin != min) {
        d->scaleMin = min;
        emit scaleMinChanged(min);
    }
}

/**
 * \if ENGLISH
 * @brief Get maximum scale value
 * @return Current scale maximum
 * \endif
 *
 * \if CHINESE
 * @brief 获取最大缩放值
 * @return 当前缩放最大值
 * \endif
 */
double QImPlotHeatmapItemNode::scaleMax() const
{
    QIM_DC(d);
    return d->scaleMax;
}

/**
 * \if ENGLISH
 * @brief Set maximum scale value
 * @param max New scale maximum
 * \endif
 *
 * \if CHINESE
 * @brief 设置最大缩放值
 * @param max 新缩放最大值
 * \endif
 */
void QImPlotHeatmapItemNode::setScaleMax(double max)
{
    QIM_D(d);
    if (d->scaleMax != max) {
        d->scaleMax = max;
        emit scaleMaxChanged(max);
    }
}

/**
 * \if ENGLISH
 * @brief Get label format string
 * @return Current label format
 * \endif
 *
 * \if CHINESE
 * @brief 获取标签格式字符串
 * @return 当前标签格式
 * \endif
 */
QString QImPlotHeatmapItemNode::labelFormat() const
{
    QIM_DC(d);
    return QString::fromUtf8(d->labelFormatUtf8);
}

/**
 * \if ENGLISH
 * @brief Set label format string
 * @param format New label format
 * \endif
 *
 * \if CHINESE
 * @brief 设置标签格式字符串
 * @param format 新标签格式
 * \endif
 */
void QImPlotHeatmapItemNode::setLabelFormat(const QString& format)
{
    QIM_D(d);
    QByteArray utf8 = format.toUtf8();
    if (d->labelFormatUtf8 != utf8) {
        d->labelFormatUtf8 = utf8;
        emit labelFormatChanged(format);
    }
}

/**
 * \if ENGLISH
 * @brief Get lower-left bounds
 * @return Current lower-left bounds
 * \endif
 *
 * \if CHINESE
 * @brief 获取左下角边界
 * @return 当前左下角边界
 * \endif
 */
QPointF QImPlotHeatmapItemNode::boundsMin() const
{
    QIM_DC(d);
    return d->boundsMin;
}

/**
 * \if ENGLISH
 * @brief Set lower-left bounds
 * @param min New lower-left bounds
 * \endif
 *
 * \if CHINESE
 * @brief 设置左下角边界
 * @param min 新左下角边界
 * \endif
 */
void QImPlotHeatmapItemNode::setBoundsMin(const QPointF& min)
{
    QIM_D(d);
    if (d->boundsMin != min) {
        d->boundsMin = min;
        emit boundsMinChanged(min);
    }
}

/**
 * \if ENGLISH
 * @brief Get upper-right bounds
 * @return Current upper-right bounds
 * \endif
 *
 * \if CHINESE
 * @brief 获取右上角边界
 * @return 当前右上角边界
 * \endif
 */
QPointF QImPlotHeatmapItemNode::boundsMax() const
{
    QIM_DC(d);
    return d->boundsMax;
}

/**
 * \if ENGLISH
 * @brief Set upper-right bounds
 * @param max New upper-right bounds
 * \endif
 *
 * \if CHINESE
 * @brief 设置右上角边界
 * @param max 新右上角边界
 * \endif
 */
void QImPlotHeatmapItemNode::setBoundsMax(const QPointF& max)
{
    QIM_D(d);
    if (d->boundsMax != max) {
        d->boundsMax = max;
        emit boundsMaxChanged(max);
    }
}

/**
 * \if ENGLISH
 * @brief Check if column-major layout is enabled
 * @return true if column-major, false if row-major
 * \endif
 *
 * \if CHINESE
 * @brief 检查是否启用列主序布局
 * @return true 表示列主序，false 表示行主序
 * \endif
 */
bool QImPlotHeatmapItemNode::isColMajor() const
{
    QIM_DC(d);
    return (d->flags & ImPlotHeatmapFlags_ColMajor) != 0;
}

/**
 * \if ENGLISH
 * @brief Set column-major layout
 * @param colMajor true for column-major, false for row-major
 * \endif
 *
 * \if CHINESE
 * @brief 设置列主序布局
 * @param colMajor true 表示列主序，false 表示行主序
 * \endif
 */
void QImPlotHeatmapItemNode::setColMajor(bool colMajor)
{
    QIM_D(d);
    const ImPlotHeatmapFlags oldFlags = d->flags;
    if (colMajor) {
        d->flags |= ImPlotHeatmapFlags_ColMajor;
    } else {
        d->flags &= ~ImPlotHeatmapFlags_ColMajor;
    }
    if (d->flags != oldFlags) {
        emit colMajorChanged(colMajor);
        emit heatmapFlagChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Get raw ImPlotHeatmapFlags
 * @return Current ImPlotHeatmapFlags
 * \endif
 *
 * \if CHINESE
 * @brief 获取原始的ImPlotHeatmapFlags
 * @return 当前的ImPlotHeatmapFlags
 * \endif
 */
int QImPlotHeatmapItemNode::heatmapFlags() const
{
    QIM_DC(d);
    return d->flags;
}

/**
 * \if ENGLISH
 * @brief Set raw ImPlotHeatmapFlags
 * @param flags New ImPlotHeatmapFlags
 * \endif
 *
 * \if CHINESE
 * @brief 设置原始的ImPlotHeatmapFlags
 * @param flags 新的ImPlotHeatmapFlags
 * \endif
 */
void QImPlotHeatmapItemNode::setHeatmapFlags(int flags)
{
    QIM_D(d);
    if (d->flags != flags) {
        d->flags = static_cast< ImPlotHeatmapFlags >(flags);
        emit heatmapFlagChanged();
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
bool QImPlotHeatmapItemNode::beginDraw()
{
    QIM_D(d);
    if (!d->data || d->data->size() == 0) {
        return false;
    }

    // Apply style - heatmap uses SetNextFillStyle? Actually heatmap uses colormap.
    // Color tracking not used for heatmap (colormap is separate). We'll ignore color for now.

    // Prepare parameters
    const double* values = d->data->valuesRawData();
    if (!values) {
        // Non-contiguous data not supported for heatmap (ImPlot doesn't have callback version)
        // Fallback to copying data? We'll just return false.
        qWarning() << "[HeatmapItemNode] Non-contiguous data not supported";
        return false;
    }

    int rows = d->data->rows();
    int cols = d->data->cols();
    double scale_min = d->scaleMin;
    double scale_max = d->scaleMax;
    // If both zero, auto-scale
    const char* label_fmt = d->labelFormatUtf8.isEmpty() ? nullptr : d->labelFormatUtf8.constData();
    ImPlotPoint bounds_min(d->boundsMin.x(), d->boundsMin.y());
    ImPlotPoint bounds_max(d->boundsMax.x(), d->boundsMax.y());
    ImPlotHeatmapFlags flags = d->flags;
    // If data is column-major but flag not set, adjust?
    if (d->data->isColMajor()) {
        flags |= ImPlotHeatmapFlags_ColMajor;
    } else {
        flags &= ~ImPlotHeatmapFlags_ColMajor;
    }

    // Call ImPlot API
    ImPlot::PlotHeatmap(
        labelConstData(),
        values,
        rows,
        cols,
        scale_min,
        scale_max,
        label_fmt,
        bounds_min,
        bounds_max,
        flags
    );

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

    return false;
}

}  // namespace QIM