#include "QImPlotPieChartItemNode.h"
#include <memory>
#include <optional>
#include "implot.h"
#include "implot_internal.h"
#include "QImTrackedValue.hpp"
#include "QtImGuiUtils.h"
#include <QDebug>

namespace QIM
{

class QImPlotPieChartItemNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlotPieChartItemNode)
public:
    PrivateData(QImPlotPieChartItemNode* p);

    std::unique_ptr< QImAbstractPieChartDataSeries > data;
    ImPlotPieChartFlags flags { ImPlotPieChartFlags_None };
    double centerX { 0.0 };          ///< Center X coordinate
    double centerY { 0.0 };          ///< Center Y coordinate
    double radius { 1.0 };           ///< Pie chart radius
    QByteArray labelFormatUtf8 { "%.1f" }; ///< Label format (UTF8, used directly by ImGui)
    double startAngle { 90.0 };      ///< Starting angle in degrees
};

QImPlotPieChartItemNode::PrivateData::PrivateData(QImPlotPieChartItemNode* p) : q_ptr(p)
{
}

/**
 * \if ENGLISH
 * @brief Constructor for QImPlotPieChartItemNode
 * @param parent Parent QObject
 * \endif
 *
 * \if CHINESE
 * @brief QImPlotPieChartItemNode的构造函数
 * @param parent 父QObject
 * \endif
 */
QImPlotPieChartItemNode::QImPlotPieChartItemNode(QObject* parent) : QImPlotItemNode(parent), QIM_PIMPL_CONSTRUCT
{
}

/**
 * \if ENGLISH
 * @brief Destructor for QImPlotPieChartItemNode
 * \endif
 *
 * \if CHINESE
 * @brief QImPlotPieChartItemNode的析构函数
 * \endif
 */
QImPlotPieChartItemNode::~QImPlotPieChartItemNode()
{
}

/**
 * \if ENGLISH
 * @brief Set data series for the pie chart
 * @param series Pointer to QImAbstractPieChartDataSeries
 * \endif
 *
 * \if CHINESE
 * @brief 设置饼图的数据系列
 * @param series QImAbstractPieChartDataSeries指针
 * \endif
 */
void QImPlotPieChartItemNode::setData(QImAbstractPieChartDataSeries* series)
{
    QIM_D(d);
    d->data.reset(series);
    Q_EMIT dataChanged();
}

/**
 * \if ENGLISH
 * @brief Get current data series
 * @return Pointer to QImAbstractPieChartDataSeries
 * \endif
 *
 * \if CHINESE
 * @brief 获取当前数据系列
 * @return QImAbstractPieChartDataSeries指针
 * \endif
 */
QImAbstractPieChartDataSeries* QImPlotPieChartItemNode::data() const
{
    return d_ptr->data.get();
}

/**
 * \if ENGLISH
 * @brief Get center position in plot units
 * @return Current center position as QPointF
 * \endif
 *
 * \if CHINESE
 * @brief 获取中心位置（绘图单位）
 * @return 当前中心位置（QPointF）
 * \endif
 */
QPointF QImPlotPieChartItemNode::center() const
{
    QIM_DC(d);
    return QPointF(d->centerX, d->centerY);
}

/**
 * \if ENGLISH
 * @brief Set center position in plot units
 * @param center New center position
 * \endif
 *
 * \if CHINESE
 * @brief 设置中心位置（绘图单位）
 * @param center 新中心位置
 * \endif
 */
void QImPlotPieChartItemNode::setCenter(const QPointF& center)
{
    QIM_D(d);
    if (d->centerX != center.x() || d->centerY != center.y()) {
        d->centerX = center.x();
        d->centerY = center.y();
        emit centerChanged(center);
    }
}

/**
 * \if ENGLISH
 * @brief Get radius in plot units
 * @return Current radius
 * \endif
 *
 * \if CHINESE
 * @brief 获取半径（绘图单位）
 * @return 当前半径
 * \endif
 */
double QImPlotPieChartItemNode::radius() const
{
    QIM_DC(d);
    return d->radius;
}

/**
 * \if ENGLISH
 * @brief Set radius in plot units
 * @param radius New radius
 * \endif
 *
 * \if CHINESE
 * @brief 设置半径（绘图单位）
 * @param radius 新半径
 * \endif
 */
void QImPlotPieChartItemNode::setRadius(double radius)
{
    QIM_D(d);
    if (d->radius != radius) {
        d->radius = radius;
        emit radiusChanged(radius);
    }
}

/**
 * \if ENGLISH
 * @brief Get label format string
 * @return Current label format string
 * \endif
 *
 * \if CHINESE
 * @brief 获取标签格式字符串
 * @return 当前标签格式字符串
 * \endif
 */
QString QImPlotPieChartItemNode::labelFormat() const
{
    QIM_DC(d);
    return QString::fromUtf8(d->labelFormatUtf8);
}

/**
 * \if ENGLISH
 * @brief Set label format string
 * @param format New label format string
 * \endif
 *
 * \if CHINESE
 * @brief 设置标签格式字符串
 * @param format 新标签格式字符串
 * \endif
 */
void QImPlotPieChartItemNode::setLabelFormat(const QString& format)
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
 * @brief Get starting angle in degrees
 * @return Current start angle
 * \endif
 *
 * \if CHINESE
 * @brief 获取起始角度（度）
 * @return 当前起始角度
 * \endif
 */
double QImPlotPieChartItemNode::startAngle() const
{
    QIM_DC(d);
    return d->startAngle;
}

/**
 * \if ENGLISH
 * @brief Set starting angle in degrees
 * @param angle New start angle
 * \endif
 *
 * \if CHINESE
 * @brief 设置起始角度（度）
 * @param angle 新起始角度
 * \endif
 */
void QImPlotPieChartItemNode::setStartAngle(double angle)
{
    QIM_D(d);
    if (d->startAngle != angle) {
        d->startAngle = angle;
        emit startAngleChanged(angle);
    }
}

/**
 * \if ENGLISH
 * @brief Check if normalization is enabled
 * @return true if normalization mode is enabled
 * \endif
 *
 * \if CHINESE
 * @brief 检查是否启用归一化
 * @return 如果启用归一化模式则返回true
 * \endif
 */
bool QImPlotPieChartItemNode::isNormalized() const
{
    QIM_DC(d);
    return (d->flags & ImPlotPieChartFlags_Normalize) != 0;
}

/**
 * \if ENGLISH
 * @brief Set normalization
 * @param normalize true to enable normalization mode
 * \endif
 *
 * \if CHINESE
 * @brief 设置归一化
 * @param normalize true表示启用归一化模式
 * \endif
 */
void QImPlotPieChartItemNode::setNormalize(bool normalize)
{
    QIM_D(d);
    const ImPlotPieChartFlags oldFlags = d->flags;
    if (normalize) {
        d->flags |= ImPlotPieChartFlags_Normalize;
    } else {
        d->flags &= ~ImPlotPieChartFlags_Normalize;
    }
    if (d->flags != oldFlags) {
        emit normalizeChanged(normalize);
        emit pieChartFlagChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Check if hidden slices are ignored
 * @return true if ignore hidden mode is enabled
 * \endif
 *
 * \if CHINESE
 * @brief 检查是否启用忽略隐藏切片
 * @return 如果启用忽略隐藏切片模式则返回true
 * \endif
 */
bool QImPlotPieChartItemNode::isIgnoreHidden() const
{
    QIM_DC(d);
    return (d->flags & ImPlotPieChartFlags_IgnoreHidden) != 0;
}

/**
 * \if ENGLISH
 * @brief Set ignore hidden slices
 * @param ignore true to enable ignore hidden mode
 * \endif
 *
 * \if CHINESE
 * @brief 设置忽略隐藏切片
 * @param ignore true表示启用忽略隐藏切片模式
 * \endif
 */
void QImPlotPieChartItemNode::setIgnoreHidden(bool ignore)
{
    QIM_D(d);
    const ImPlotPieChartFlags oldFlags = d->flags;
    if (ignore) {
        d->flags |= ImPlotPieChartFlags_IgnoreHidden;
    } else {
        d->flags &= ~ImPlotPieChartFlags_IgnoreHidden;
    }
    if (d->flags != oldFlags) {
        emit ignoreHiddenChanged(ignore);
        emit pieChartFlagChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Check if exploding slices are enabled
 * @return true if exploding mode is enabled
 * \endif
 *
 * \if CHINESE
 * @brief 检查是否启用爆炸切片
 * @return 如果启用爆炸切片模式则返回true
 * \endif
 */
bool QImPlotPieChartItemNode::isExploding() const
{
    QIM_DC(d);
    return (d->flags & ImPlotPieChartFlags_Exploding) != 0;
}

/**
 * \if ENGLISH
 * @brief Set exploding slices
 * @param exploding true to enable exploding mode
 * \endif
 *
 * \if CHINESE
 * @brief 设置爆炸切片
 * @param exploding true表示启用爆炸切片模式
 * \endif
 */
void QImPlotPieChartItemNode::setExploding(bool exploding)
{
    QIM_D(d);
    const ImPlotPieChartFlags oldFlags = d->flags;
    if (exploding) {
        d->flags |= ImPlotPieChartFlags_Exploding;
    } else {
        d->flags &= ~ImPlotPieChartFlags_Exploding;
    }
    if (d->flags != oldFlags) {
        emit explodingChanged(exploding);
        emit pieChartFlagChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Get raw ImPlotPieChartFlags
 * @return Current ImPlotPieChartFlags
 * \endif
 *
 * \if CHINESE
 * @brief 获取原始的ImPlotPieChartFlags
 * @return 当前的ImPlotPieChartFlags
 * \endif
 */
int QImPlotPieChartItemNode::pieChartFlags() const
{
    QIM_DC(d);
    return d->flags;
}

/**
 * \if ENGLISH
 * @brief Set raw ImPlotPieChartFlags
 * @param flags New ImPlotPieChartFlags
 * \endif
 *
 * \if CHINESE
 * @brief 设置原始的ImPlotPieChartFlags
 * @param flags 新的ImPlotPieChartFlags
 * \endif
 */
void QImPlotPieChartItemNode::setPieChartFlags(int flags)
{
    QIM_D(d);
    if (d->flags != flags) {
        d->flags = static_cast< ImPlotPieChartFlags >(flags);
        emit pieChartFlagChanged();
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
bool QImPlotPieChartItemNode::beginDraw()
{
    QIM_D(d);
    if (!d->data || d->data->sliceCount() == 0) {
        return false;
    }

    // Prepare label pointers and values
    std::vector<const char*> labelPtrs = d->data->labelPtrs();
    const double* values = d->data->valuesRawData();
    int count = d->data->sliceCount();

    // Prepare label format C-string (use UTF8 directly from setter)
    const char* fmt = nullptr;
    if (!d->labelFormatUtf8.isEmpty()) {
        fmt = d->labelFormatUtf8.constData();
    }

    // Call ImPlot API
    if (d->data->isContiguous() && values) {
        ImPlot::PlotPieChart(
            labelPtrs.data(), values, count, d->centerX, d->centerY, d->radius,
            fmt, d->startAngle, d->flags);
    } else {
        // Non-contiguous memory mode: not supported by ImPlot's PlotPieChart, fallback to manual drawing?
        // For now, just skip drawing
        qWarning() << "[PieChartItemNode] Non-contiguous data not supported for pie chart";
        return false;
    }

    // Update item status
    if(!imPlotItem()){
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
    }
    return false;
}

}  // namespace QIM
