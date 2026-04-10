#include "QImPlotPieChartItemNode.h"
#include "QtImGuiUtils.h"
#include "implot.h"
#include "implot_internal.h"

namespace QIM
{

class QImPlotPieChartItemNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlotPieChartItemNode)
public:
    explicit PrivateData(QImPlotPieChartItemNode* p) : q_ptr(p)
    {
    }

    void rebuildLabelCaches()
    {
        labelUtf8Cache.clear();
        labelPtrs.clear();
        labelUtf8Cache.reserve(labels.size());
        labelPtrs.reserve(labels.size());
        for (const QString& label : labels) {
            labelUtf8Cache.push_back(label.toUtf8());
        }
        for (const QByteArray& labelUtf8 : labelUtf8Cache) {
            labelPtrs.push_back(labelUtf8.constData());
        }
    }

    QStringList labels;
    std::vector< double > values;
    std::vector< QByteArray > labelUtf8Cache;
    std::vector< const char* > labelPtrs;
    QPointF center { 0.5, 0.5 };
    double radius { 0.4 };
    double startAngle { 90.0 };
    QByteArray labelFormat { "%.1f" };
    ImPlotPieChartFlags flags { ImPlotPieChartFlags_None };
};

/**
 * \if ENGLISH
 * @brief Constructs a pie chart item node.
 * @param[in] parent Parent QObject.
 * \endif
 *
 * \if CHINESE
 * @brief 构造一个饼图绘图项节点。
 * @param[in] parent 父QObject对象。
 * \endif
 */
QImPlotPieChartItemNode::QImPlotPieChartItemNode(QObject* parent)
    : QImPlotItemNode(parent), QIM_PIMPL_CONSTRUCT
{
}

/**
 * \if ENGLISH
 * @brief Destroys the pie chart item node.
 * \endif
 *
 * \if CHINESE
 * @brief 销毁饼图绘图项节点。
 * \endif
 */
QImPlotPieChartItemNode::~QImPlotPieChartItemNode()
{
}

/**
 * \if ENGLISH
 * @brief Sets slice labels and values.
 * @param[in] labels Slice labels used by the legend and slice registration.
 * @param[in] values Slice values used for angle calculation.
 * \endif
 *
 * \if CHINESE
 * @brief 设置切片标签和数值。
 * @param[in] labels 用于图例和切片注册的标签列表。
 * @param[in] values 用于角度计算的切片数值列表。
 * \endif
 */
void QImPlotPieChartItemNode::setData(const QStringList& labels, const std::vector< double >& values)
{
    QIM_D(d);
    d->labels = labels;
    d->values = values;
    d->rebuildLabelCaches();
    Q_EMIT dataChanged();
}

/**
 * \if ENGLISH
 * @brief Sets slice labels and values with move semantics.
 * @param[in] labels Slice labels used by the legend and slice registration.
 * @param[in] values Slice values used for angle calculation.
 * \endif
 *
 * \if CHINESE
 * @brief 使用移动语义设置切片标签和数值。
 * @param[in] labels 用于图例和切片注册的标签列表。
 * @param[in] values 用于角度计算的切片数值列表。
 * \endif
 */
void QImPlotPieChartItemNode::setData(QStringList labels, std::vector< double >&& values)
{
    QIM_D(d);
    d->labels = std::move(labels);
    d->values = std::move(values);
    d->rebuildLabelCaches();
    Q_EMIT dataChanged();
}

/**
 * \if ENGLISH
 * @brief Returns the current slice labels.
 * @return Slice labels.
 * \endif
 *
 * \if CHINESE
 * @brief 返回当前切片标签。
 * @return 切片标签列表。
 * \endif
 */
QStringList QImPlotPieChartItemNode::labels() const
{
    return d_ptr->labels;
}

/**
 * \if ENGLISH
 * @brief Returns the current slice values.
 * @return Slice value container.
 * \endif
 *
 * \if CHINESE
 * @brief 返回当前切片数值。
 * @return 切片数值容器。
 * \endif
 */
const std::vector< double >& QImPlotPieChartItemNode::values() const
{
    return d_ptr->values;
}

/**
 * \if ENGLISH
 * @brief Sets the pie chart center in plot coordinates.
 * @param[in] center New pie center.
 * \endif
 *
 * \if CHINESE
 * @brief 设置饼图在绘图坐标中的圆心。
 * @param[in] center 新的圆心位置。
 * \endif
 */
void QImPlotPieChartItemNode::setCenter(const QPointF& center)
{
    QIM_D(d);
    if (d->center != center) {
        d->center = center;
        Q_EMIT geometryChanged();
    }
}

QPointF QImPlotPieChartItemNode::center() const
{
    return d_ptr->center;
}

/**
 * \if ENGLISH
 * @brief Sets the pie chart radius in plot units.
 * @param[in] radius New radius.
 * \endif
 *
 * \if CHINESE
 * @brief 设置饼图半径（绘图单位）。
 * @param[in] radius 新的半径值。
 * \endif
 */
void QImPlotPieChartItemNode::setRadius(double radius)
{
    QIM_D(d);
    if (d->radius != radius) {
        d->radius = radius;
        Q_EMIT geometryChanged();
    }
}

double QImPlotPieChartItemNode::radius() const
{
    return d_ptr->radius;
}

/**
 * \if ENGLISH
 * @brief Sets the start angle in degrees.
 * @param[in] angle New start angle.
 * \endif
 *
 * \if CHINESE
 * @brief 设置起始角度（度）。
 * @param[in] angle 新的起始角度。
 * \endif
 */
void QImPlotPieChartItemNode::setStartAngle(double angle)
{
    QIM_D(d);
    if (d->startAngle != angle) {
        d->startAngle = angle;
        Q_EMIT geometryChanged();
    }
}

double QImPlotPieChartItemNode::startAngle() const
{
    return d_ptr->startAngle;
}

/**
 * \if ENGLISH
 * @brief Sets the slice label format string.
 * @param[in] format New printf-style format string. Empty disables labels.
 * \endif
 *
 * \if CHINESE
 * @brief 设置切片标签格式字符串。
 * @param[in] format 新的printf风格格式字符串。空字符串表示禁用标签。
 * \endif
 */
void QImPlotPieChartItemNode::setLabelFormat(const QString& format)
{
    QIM_D(d);
    const QByteArray newValue = format.toUtf8();
    if (d->labelFormat != newValue) {
        d->labelFormat = newValue;
        Q_EMIT labelFormatChanged(format);
    }
}

QString QImPlotPieChartItemNode::labelFormat() const
{
    return QString::fromUtf8(d_ptr->labelFormat);
}

bool QImPlotPieChartItemNode::isNormalize() const
{
    return (d_ptr->flags & ImPlotPieChartFlags_Normalize) != 0;
}

void QImPlotPieChartItemNode::setNormalize(bool on)
{
    QIM_D(d);
    const ImPlotPieChartFlags oldFlags = d->flags;
    if (on) {
        d->flags |= ImPlotPieChartFlags_Normalize;
    } else {
        d->flags &= ~ImPlotPieChartFlags_Normalize;
    }
    if (oldFlags != d->flags) {
        Q_EMIT pieChartFlagChanged();
    }
}

bool QImPlotPieChartItemNode::isIgnoreHidden() const
{
    return (d_ptr->flags & ImPlotPieChartFlags_IgnoreHidden) != 0;
}

void QImPlotPieChartItemNode::setIgnoreHidden(bool on)
{
    QIM_D(d);
    const ImPlotPieChartFlags oldFlags = d->flags;
    if (on) {
        d->flags |= ImPlotPieChartFlags_IgnoreHidden;
    } else {
        d->flags &= ~ImPlotPieChartFlags_IgnoreHidden;
    }
    if (oldFlags != d->flags) {
        Q_EMIT pieChartFlagChanged();
    }
}

bool QImPlotPieChartItemNode::isExploding() const
{
    return (d_ptr->flags & ImPlotPieChartFlags_Exploding) != 0;
}

void QImPlotPieChartItemNode::setExploding(bool on)
{
    QIM_D(d);
    const ImPlotPieChartFlags oldFlags = d->flags;
    if (on) {
        d->flags |= ImPlotPieChartFlags_Exploding;
    } else {
        d->flags &= ~ImPlotPieChartFlags_Exploding;
    }
    if (oldFlags != d->flags) {
        Q_EMIT pieChartFlagChanged();
    }
}

int QImPlotPieChartItemNode::pieChartFlags() const
{
    return d_ptr->flags;
}

void QImPlotPieChartItemNode::setPieChartFlags(int flags)
{
    QIM_D(d);
    if (d->flags != flags) {
        d->flags = static_cast< ImPlotPieChartFlags >(flags);
        Q_EMIT pieChartFlagChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Draws the pie chart using the currently stored labels and values.
 * @return Always returns false because the item has no child scope to recurse into.
 * \endif
 *
 * \if CHINESE
 * @brief 使用当前保存的标签和数值绘制饼图。
 * @return 始终返回false，因为该绘图项没有子作用域需要递归渲染。
 * \endif
 */
bool QImPlotPieChartItemNode::beginDraw()
{
    QIM_D(d);
    const int count = qMin(d->labels.size(), static_cast< int >(d->values.size()));
    if (count <= 0) {
        return false;
    }

    if (d->labelPtrs.size() != static_cast< std::size_t >(d->labels.size())) {
        d->rebuildLabelCaches();
    }

    const char* fmt = d->labelFormat.isEmpty() ? nullptr : d->labelFormat.constData();
    ImPlot::PlotPieChart(d->labelPtrs.data(),
                         d->values.data(),
                         count,
                         d->center.x(),
                         d->center.y(),
                         d->radius,
                         fmt,
                         d->startAngle,
                         d->flags);

    ImPlotContext* ct = ImPlot::GetCurrentContext();
    if (ct) {
        setImPlotItem(ct->PreviousItem);
    }
    return false;
}

}  // namespace QIM
