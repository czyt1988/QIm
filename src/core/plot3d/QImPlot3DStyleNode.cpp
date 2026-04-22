#include "QImPlot3DStyleNode.h"
#include "QImPlot3DNode.h"
#include "QtImGuiUtils.h"
#include "implot3d.h"

namespace QIM
{

//===============================================================
// PrivateData Implementation
//===============================================================

class QImPlot3DStyleNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlot3DStyleNode)

public:
    PrivateData(QImPlot3DStyleNode* q);

public:
    // Style variables (defaults match ImPlot3DStyle constructor)
    float lineWeight {1.0f};
    int marker {-1};  // ImPlot3DMarker_None = -1
    float markerSize {4.0f};
    float markerWeight {1.0f};
    float fillAlpha {1.0f};
    ImVec2 plotDefaultSize {400.0f, 400.0f};
    ImVec2 plotMinSize {200.0f, 200.0f};
    ImVec2 plotPadding {10.0f, 10.0f};
    ImVec2 labelPadding {5.0f, 5.0f};
    float viewScaleFactor {1.0f};
    ImVec2 legendPadding {10.0f, 10.0f};
    ImVec2 legendInnerPadding {5.0f, 5.0f};
    ImVec2 legendSpacing {5.0f, 0.0f};

    // Colors (QColor stored for explicit values; isAuto flags control IMPLOT3D_AUTO_COL)
    QColor titleTextColor;
    QColor inlayTextColor;
    QColor frameBgColor;
    QColor plotBgColor;
    QColor plotBorderColor;
    QColor legendBgColor;
    QColor legendBorderColor;
    QColor legendTextColor;
    QColor axisTextColor;
    QColor axisGridColor;
    QColor axisTickColor;

    // isAuto flags: true = write IMPLOT3D_AUTO_COL in applyStyle(), false = write toImVec4(color)
    mutable bool isAutoTitleTextColor {true};
    mutable bool isAutoInlayTextColor {true};
    mutable bool isAutoFrameBgColor {true};
    mutable bool isAutoPlotBgColor {true};
    mutable bool isAutoPlotBorderColor {true};
    mutable bool isAutoLegendBgColor {true};
    mutable bool isAutoLegendBorderColor {true};
    mutable bool isAutoLegendTextColor {true};
    mutable bool isAutoAxisTextColor {true};
    mutable bool isAutoAxisGridColor {true};
    mutable bool isAutoAxisTickColor {true};
};

QImPlot3DStyleNode::PrivateData::PrivateData(QImPlot3DStyleNode* q) : q_ptr(q)
{
    // All colors default to IMPLOT3D_AUTO_COL (isAuto=true).
    // QColor values are left default-initialized — they are "dead code" when isAuto=true,
    // since applyStyle() writes IMPLOT3D_AUTO_COL directly without reading the QColor.
}

//===============================================================
// QImPlot3DStyleNode Implementation
//===============================================================

/**
 * \if ENGLISH
 * @brief Constructs a QImPlot3DStyleNode with optional parent
 * @param parent QImPlot3DNode parent for ownership and style association
 * @details All style variables and colors are initialized with ImPlot3D default values.
 *          The parent must be a QImPlot3DNode for proper style integration.
 * \endif
 *
 * \if CHINESE
 * @brief 构造 QImPlot3DStyleNode，可指定父对象
 * @param parent QImPlot3DNode 父对象，用于所有权和样式关联
 * @details 所有样式变量和颜色使用 ImPlot3D 默认值初始化。
 *          父对象必须是 QImPlot3DNode 以实现正确的样式集成。
 * \endif
 */
QImPlot3DStyleNode::QImPlot3DStyleNode(QImPlot3DNode* parent)
    : QObject(parent)
    , QIM_PIMPL_CONSTRUCT
{
    setObjectName(QStringLiteral("Plot3DStyleNode"));
}

QImPlot3DStyleNode::~QImPlot3DStyleNode()
{
}

//----------------------------------------------------
// Style variables
//----------------------------------------------------

/**
 * \if ENGLISH
 * @brief Returns the line weight in pixels
 * @return Current line weight value
 * \endif
 *
 * \if CHINESE
 * @brief 返回线条粗细（像素）
 * @return 当前线条粗细值
 * \endif
 */
float QImPlot3DStyleNode::lineWeight() const
{
    QIM_DC(d);
    return d->lineWeight;
}

/**
 * \if ENGLISH
 * @brief Sets the line weight in pixels
 * @param weight New line weight value
 * \endif
 *
 * \if CHINESE
 * @brief 设置线条粗细（像素）
 * @param weight 新的线条粗细值
 * \endif
 */
void QImPlot3DStyleNode::setLineWeight(float weight)
{
    QIM_D(d);
    if (d->lineWeight != weight) {
        d->lineWeight = weight;
        Q_EMIT styleChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Returns the marker type as QImPlot3DMarkerShape enum value
 * @return Current marker type (int corresponding to QImPlot3DMarkerShape)
 * \endif
 *
 * \if CHINESE
 * @brief 返回标记类型，对应 QImPlot3DMarkerShape 枚举值
 * @return 当前标记类型（对应 QImPlot3DMarkerShape 的 int 值）
 * \endif
 */
int QImPlot3DStyleNode::marker() const
{
    QIM_DC(d);
    return d->marker;
}

/**
 * \if ENGLISH
 * @brief Sets the marker type from QImPlot3DMarkerShape enum value
 * @param marker New marker type (QImPlot3DMarkerShape enum value as int)
 * \endif
 *
 * \if CHINESE
 * @brief 设置标记类型，使用 QImPlot3DMarkerShape 枚举值
 * @param marker 新的标记类型（QImPlot3DMarkerShape 枚举值转为 int）
 * \endif
 */
void QImPlot3DStyleNode::setMarker(int marker)
{
    QIM_D(d);
    if (d->marker != marker) {
        d->marker = marker;
        Q_EMIT styleChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Returns the marker size in pixels
 * @return Current marker size value
 * \endif
 *
 * \if CHINESE
 * @brief 返回标记大小（像素）
 * @return 当前标记大小值
 * \endif
 */
float QImPlot3DStyleNode::markerSize() const
{
    QIM_DC(d);
    return d->markerSize;
}

/**
 * \if ENGLISH
 * @brief Sets the marker size in pixels
 * @param size New marker size value
 * \endif
 *
 * \if CHINESE
 * @brief 设置标记大小（像素）
 * @param size 新的标记大小值
 * \endif
 */
void QImPlot3DStyleNode::setMarkerSize(float size)
{
    QIM_D(d);
    if (d->markerSize != size) {
        d->markerSize = size;
        Q_EMIT styleChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Returns the marker outline weight in pixels
 * @return Current marker outline weight
 * \endif
 *
 * \if CHINESE
 * @brief 返回标记轮廓粗细（像素）
 * @return 当前标记轮廓粗细值
 * \endif
 */
float QImPlot3DStyleNode::markerWeight() const
{
    QIM_DC(d);
    return d->markerWeight;
}

/**
 * \if ENGLISH
 * @brief Sets the marker outline weight in pixels
 * @param weight New marker outline weight
 * \endif
 *
 * \if CHINESE
 * @brief 设置标记轮廓粗细（像素）
 * @param weight 新的标记轮廓粗细值
 * \endif
 */
void QImPlot3DStyleNode::setMarkerWeight(float weight)
{
    QIM_D(d);
    if (d->markerWeight != weight) {
        d->markerWeight = weight;
        Q_EMIT styleChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Returns the fill alpha modifier
 * @return Current fill alpha value
 * \endif
 *
 * \if CHINESE
 * @brief 返回填充透明度修正值
 * @return 当前填充透明度值
 * \endif
 */
float QImPlot3DStyleNode::fillAlpha() const
{
    QIM_DC(d);
    return d->fillAlpha;
}

/**
 * \if ENGLISH
 * @brief Sets the fill alpha modifier
 * @param alpha New fill alpha value (typically 0.0 to 1.0)
 * \endif
 *
 * \if CHINESE
 * @brief 设置填充透明度修正值
 * @param alpha 新的填充透明度值（通常为 0.0 到 1.0）
 * \endif
 */
void QImPlot3DStyleNode::setFillAlpha(float alpha)
{
    QIM_D(d);
    if (d->fillAlpha != alpha) {
        d->fillAlpha = alpha;
        Q_EMIT styleChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Returns the default plot size
 * @return Default plot size as QSizeF
 * \endif
 *
 * \if CHINESE
 * @brief 返回默认绘图尺寸
 * @return 默认绘图尺寸，类型为 QSizeF
 * \endif
 */
QSizeF QImPlot3DStyleNode::plotDefaultSize() const
{
    QIM_DC(d);
    return toQSizeF(d->plotDefaultSize);
}

/**
 * \if ENGLISH
 * @brief Sets the default plot size
 * @param size New default plot size
 * \endif
 *
 * \if CHINESE
 * @brief 设置默认绘图尺寸
 * @param size 新的默认绘图尺寸
 * \endif
 */
void QImPlot3DStyleNode::setPlotDefaultSize(const QSizeF& size)
{
    QIM_D(d);
    ImVec2 newVal = toImVec2(size);
    if (!fuzzyEqual(d->plotDefaultSize, newVal)) {
        d->plotDefaultSize = newVal;
        Q_EMIT styleChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Returns the minimum plot size
 * @return Minimum plot size as QSizeF
 * \endif
 *
 * \if CHINESE
 * @brief 返回最小绘图尺寸
 * @return 最小绘图尺寸，类型为 QSizeF
 * \endif
 */
QSizeF QImPlot3DStyleNode::plotMinSize() const
{
    QIM_DC(d);
    return toQSizeF(d->plotMinSize);
}

/**
 * \if ENGLISH
 * @brief Sets the minimum plot size
 * @param size New minimum plot size
 * \endif
 *
 * \if CHINESE
 * @brief 设置最小绘图尺寸
 * @param size 新的最小绘图尺寸
 * \endif
 */
void QImPlot3DStyleNode::setPlotMinSize(const QSizeF& size)
{
    QIM_D(d);
    ImVec2 newVal = toImVec2(size);
    if (!fuzzyEqual(d->plotMinSize, newVal)) {
        d->plotMinSize = newVal;
        Q_EMIT styleChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Returns the plot padding
 * @return Plot padding as QSizeF
 * \endif
 *
 * \if CHINESE
 * @brief 返回绘图内边距
 * @return 绘图内边距，类型为 QSizeF
 * \endif
 */
QSizeF QImPlot3DStyleNode::plotPadding() const
{
    QIM_DC(d);
    return toQSizeF(d->plotPadding);
}

/**
 * \if ENGLISH
 * @brief Sets the plot padding
 * @param padding New plot padding
 * \endif
 *
 * \if CHINESE
 * @brief 设置绘图内边距
 * @param padding 新的绘图内边距
 * \endif
 */
void QImPlot3DStyleNode::setPlotPadding(const QSizeF& padding)
{
    QIM_D(d);
    ImVec2 newVal = toImVec2(padding);
    if (!fuzzyEqual(d->plotPadding, newVal)) {
        d->plotPadding = newVal;
        Q_EMIT styleChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Returns the label padding
 * @return Label padding as QSizeF
 * \endif
 *
 * \if CHINESE
 * @brief 返回标签内边距
 * @return 标签内边距，类型为 QSizeF
 * \endif
 */
QSizeF QImPlot3DStyleNode::labelPadding() const
{
    QIM_DC(d);
    return toQSizeF(d->labelPadding);
}

/**
 * \if ENGLISH
 * @brief Sets the label padding
 * @param padding New label padding
 * \endif
 *
 * \if CHINESE
 * @brief 设置标签内边距
 * @param padding 新的标签内边距
 * \endif
 */
void QImPlot3DStyleNode::setLabelPadding(const QSizeF& padding)
{
    QIM_D(d);
    ImVec2 newVal = toImVec2(padding);
    if (!fuzzyEqual(d->labelPadding, newVal)) {
        d->labelPadding = newVal;
        Q_EMIT styleChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Returns the 3D view scale factor
 * @return Current view scale factor value
 * \endif
 *
 * \if CHINESE
 * @brief 返回三维视图缩放因子
 * @return 当前视图缩放因子值
 * \endif
 */
float QImPlot3DStyleNode::viewScaleFactor() const
{
    QIM_DC(d);
    return d->viewScaleFactor;
}

/**
 * \if ENGLISH
 * @brief Sets the 3D view scale factor
 * @param factor New view scale factor (1.0 = default)
 * \endif
 *
 * \if CHINESE
 * @brief 设置三维视图缩放因子
 * @param factor 新的视图缩放因子（1.0 为默认值）
 * \endif
 */
void QImPlot3DStyleNode::setViewScaleFactor(float factor)
{
    QIM_D(d);
    if (d->viewScaleFactor != factor) {
        d->viewScaleFactor = factor;
        Q_EMIT styleChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Returns the legend padding from plot edges
 * @return Legend padding as QSizeF
 * \endif
 *
 * \if CHINESE
 * @brief 返回图例与绘图边缘的内边距
 * @return 图例内边距，类型为 QSizeF
 * \endif
 */
QSizeF QImPlot3DStyleNode::legendPadding() const
{
    QIM_DC(d);
    return toQSizeF(d->legendPadding);
}

/**
 * \if ENGLISH
 * @brief Sets the legend padding from plot edges
 * @param padding New legend padding
 * \endif
 *
 * \if CHINESE
 * @brief 设置图例与绘图边缘的内边距
 * @param padding 新的图例内边距
 * \endif
 */
void QImPlot3DStyleNode::setLegendPadding(const QSizeF& padding)
{
    QIM_D(d);
    ImVec2 newVal = toImVec2(padding);
    if (!fuzzyEqual(d->legendPadding, newVal)) {
        d->legendPadding = newVal;
        Q_EMIT styleChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Returns the legend inner padding
 * @return Legend inner padding as QSizeF
 * \endif
 *
 * \if CHINESE
 * @brief 返回图例内部内边距
 * @return 图例内部内边距，类型为 QSizeF
 * \endif
 */
QSizeF QImPlot3DStyleNode::legendInnerPadding() const
{
    QIM_DC(d);
    return toQSizeF(d->legendInnerPadding);
}

/**
 * \if ENGLISH
 * @brief Sets the legend inner padding
 * @param padding New legend inner padding
 * \endif
 *
 * \if CHINESE
 * @brief 设置图例内部内边距
 * @param padding 新的图例内部内边距
 * \endif
 */
void QImPlot3DStyleNode::setLegendInnerPadding(const QSizeF& padding)
{
    QIM_D(d);
    ImVec2 newVal = toImVec2(padding);
    if (!fuzzyEqual(d->legendInnerPadding, newVal)) {
        d->legendInnerPadding = newVal;
        Q_EMIT styleChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Returns the spacing between legend entries
 * @return Legend spacing as QSizeF
 * \endif
 *
 * \if CHINESE
 * @brief 返回图例条目间距
 * @return 图例条目间距，类型为 QSizeF
 * \endif
 */
QSizeF QImPlot3DStyleNode::legendSpacing() const
{
    QIM_DC(d);
    return toQSizeF(d->legendSpacing);
}

/**
 * \if ENGLISH
 * @brief Sets the spacing between legend entries
 * @param spacing New legend entry spacing
 * \endif
 *
 * \if CHINESE
 * @brief 设置图例条目间距
 * @param spacing 新的图例条目间距
 * \endif
 */
void QImPlot3DStyleNode::setLegendSpacing(const QSizeF& spacing)
{
    QIM_D(d);
    ImVec2 newVal = toImVec2(spacing);
    if (!fuzzyEqual(d->legendSpacing, newVal)) {
        d->legendSpacing = newVal;
        Q_EMIT styleChanged();
    }
}

//----------------------------------------------------
// Plot colors
//----------------------------------------------------

/**
 * \if ENGLISH
 * @brief Returns the title text color
 * @return Current title text color
 * \endif
 *
 * \if CHINESE
 * @brief 返回标题文本颜色
 * @return 当前标题文本颜色
 * \endif
 */
QColor QImPlot3DStyleNode::titleTextColor() const
{
    QIM_DC(d);
    return d->titleTextColor;
}

/**
 * \if ENGLISH
 * @brief Sets the title text color
 * @param color New title text color
 * \endif
 *
 * \if CHINESE
 * @brief 设置标题文本颜色
 * @param color 新的标题文本颜色
 * \endif
 */
void QImPlot3DStyleNode::setTitleTextColor(const QColor& color)
{
    QIM_D(d);
    if (d->titleTextColor != color) {
        d->titleTextColor = color;
        d->isAutoTitleTextColor = false;
        Q_EMIT styleChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Returns the inlay text color
 * @return Current inlay text color
 * \endif
 *
 * \if CHINESE
 * @brief 返回内嵌文本颜色
 * @return 当前内嵌文本颜色
 * \endif
 */
QColor QImPlot3DStyleNode::inlayTextColor() const
{
    QIM_DC(d);
    return d->inlayTextColor;
}

/**
 * \if ENGLISH
 * @brief Sets the inlay text color
 * @param color New inlay text color
 * \endif
 *
 * \if CHINESE
 * @brief 设置内嵌文本颜色
 * @param color 新的内嵌文本颜色
 * \endif
 */
void QImPlot3DStyleNode::setInlayTextColor(const QColor& color)
{
    QIM_D(d);
    if (d->inlayTextColor != color) {
        d->inlayTextColor = color;
        d->isAutoInlayTextColor = false;
        Q_EMIT styleChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Returns the frame background color
 * @return Current frame background color
 * \endif
 *
 * \if CHINESE
 * @brief 返回边框背景颜色
 * @return 当前边框背景颜色
 * \endif
 */
QColor QImPlot3DStyleNode::frameBgColor() const
{
    QIM_DC(d);
    return d->frameBgColor;
}

/**
 * \if ENGLISH
 * @brief Sets the frame background color
 * @param color New frame background color
 * \endif
 *
 * \if CHINESE
 * @brief 设置边框背景颜色
 * @param color 新的边框背景颜色
 * \endif
 */
void QImPlot3DStyleNode::setFrameBgColor(const QColor& color)
{
    QIM_D(d);
    if (d->frameBgColor != color) {
        d->frameBgColor = color;
        d->isAutoFrameBgColor = false;
        Q_EMIT styleChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Returns the plot background color
 * @return Current plot background color
 * \endif
 *
 * \if CHINESE
 * @brief 返回绘图区域背景颜色
 * @return 当前绘图区域背景颜色
 * \endif
 */
QColor QImPlot3DStyleNode::plotBgColor() const
{
    QIM_DC(d);
    return d->plotBgColor;
}

/**
 * \if ENGLISH
 * @brief Sets the plot background color
 * @param color New plot background color
 * \endif
 *
 * \if CHINESE
 * @brief 设置绘图区域背景颜色
 * @param color 新的绘图区域背景颜色
 * \endif
 */
void QImPlot3DStyleNode::setPlotBgColor(const QColor& color)
{
    QIM_D(d);
    if (d->plotBgColor != color) {
        d->plotBgColor = color;
        d->isAutoPlotBgColor = false;
        Q_EMIT styleChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Returns the plot border color
 * @return Current plot border color
 * \endif
 *
 * \if CHINESE
 * @brief 返回绘图区域边框颜色
 * @return 当前绘图区域边框颜色
 * \endif
 */
QColor QImPlot3DStyleNode::plotBorderColor() const
{
    QIM_DC(d);
    return d->plotBorderColor;
}

/**
 * \if ENGLISH
 * @brief Sets the plot border color
 * @param color New plot border color
 * \endif
 *
 * \if CHINESE
 * @brief 设置绘图区域边框颜色
 * @param color 新的绘图区域边框颜色
 * \endif
 */
void QImPlot3DStyleNode::setPlotBorderColor(const QColor& color)
{
    QIM_D(d);
    if (d->plotBorderColor != color) {
        d->plotBorderColor = color;
        d->isAutoPlotBorderColor = false;
        Q_EMIT styleChanged();
    }
}

//----------------------------------------------------
// Legend colors
//----------------------------------------------------

/**
 * \if ENGLISH
 * @brief Returns the legend background color
 * @return Current legend background color
 * \endif
 *
 * \if CHINESE
 * @brief 返回图例背景颜色
 * @return 当前图例背景颜色
 * \endif
 */
QColor QImPlot3DStyleNode::legendBgColor() const
{
    QIM_DC(d);
    return d->legendBgColor;
}

/**
 * \if ENGLISH
 * @brief Sets the legend background color
 * @param color New legend background color
 * \endif
 *
 * \if CHINESE
 * @brief 设置图例背景颜色
 * @param color 新的图例背景颜色
 * \endif
 */
void QImPlot3DStyleNode::setLegendBgColor(const QColor& color)
{
    QIM_D(d);
    if (d->legendBgColor != color) {
        d->legendBgColor = color;
        d->isAutoLegendBgColor = false;
        Q_EMIT styleChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Returns the legend border color
 * @return Current legend border color
 * \endif
 *
 * \if CHINESE
 * @brief 返回图例边框颜色
 * @return 当前图例边框颜色
 * \endif
 */
QColor QImPlot3DStyleNode::legendBorderColor() const
{
    QIM_DC(d);
    return d->legendBorderColor;
}

/**
 * \if ENGLISH
 * @brief Sets the legend border color
 * @param color New legend border color
 * \endif
 *
 * \if CHINESE
 * @brief 设置图例边框颜色
 * @param color 新的图例边框颜色
 * \endif
 */
void QImPlot3DStyleNode::setLegendBorderColor(const QColor& color)
{
    QIM_D(d);
    if (d->legendBorderColor != color) {
        d->legendBorderColor = color;
        d->isAutoLegendBorderColor = false;
        Q_EMIT styleChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Returns the legend text color
 * @return Current legend text color
 * \endif
 *
 * \if CHINESE
 * @brief 返回图例文本颜色
 * @return 当前图例文本颜色
 * \endif
 */
QColor QImPlot3DStyleNode::legendTextColor() const
{
    QIM_DC(d);
    return d->legendTextColor;
}

/**
 * \if ENGLISH
 * @brief Sets the legend text color
 * @param color New legend text color
 * \endif
 *
 * \if CHINESE
 * @brief 设置图例文本颜色
 * @param color 新的图例文本颜色
 * \endif
 */
void QImPlot3DStyleNode::setLegendTextColor(const QColor& color)
{
    QIM_D(d);
    if (d->legendTextColor != color) {
        d->legendTextColor = color;
        d->isAutoLegendTextColor = false;
        Q_EMIT styleChanged();
    }
}

//----------------------------------------------------
// Axis colors
//----------------------------------------------------

/**
 * \if ENGLISH
 * @brief Returns the axis text color
 * @return Current axis text color
 * \endif
 *
 * \if CHINESE
 * @brief 返回坐标轴文本颜色
 * @return 当前坐标轴文本颜色
 * \endif
 */
QColor QImPlot3DStyleNode::axisTextColor() const
{
    QIM_DC(d);
    return d->axisTextColor;
}

/**
 * \if ENGLISH
 * @brief Sets the axis text color
 * @param color New axis text color
 * \endif
 *
 * \if CHINESE
 * @brief 设置坐标轴文本颜色
 * @param color 新的坐标轴文本颜色
 * \endif
 */
void QImPlot3DStyleNode::setAxisTextColor(const QColor& color)
{
    QIM_D(d);
    if (d->axisTextColor != color) {
        d->axisTextColor = color;
        d->isAutoAxisTextColor = false;
        Q_EMIT styleChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Returns the axis grid color
 * @return Current axis grid color
 * \endif
 *
 * \if CHINESE
 * @brief 返回坐标轴网格颜色
 * @return 当前坐标轴网格颜色
 * \endif
 */
QColor QImPlot3DStyleNode::axisGridColor() const
{
    QIM_DC(d);
    return d->axisGridColor;
}

/**
 * \if ENGLISH
 * @brief Sets the axis grid color
 * @param color New axis grid color
 * \endif
 *
 * \if CHINESE
 * @brief 设置坐标轴网格颜色
 * @param color 新的坐标轴网格颜色
 * \endif
 */
void QImPlot3DStyleNode::setAxisGridColor(const QColor& color)
{
    QIM_D(d);
    if (d->axisGridColor != color) {
        d->axisGridColor = color;
        d->isAutoAxisGridColor = false;
        Q_EMIT styleChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Returns the axis tick color
 * @return Current axis tick color
 * \endif
 *
 * \if CHINESE
 * @brief 返回坐标轴刻度颜色
 * @return 当前坐标轴刻度颜色
 * \endif
 */
QColor QImPlot3DStyleNode::axisTickColor() const
{
    QIM_DC(d);
    return d->axisTickColor;
}

/**
 * \if ENGLISH
 * @brief Sets the axis tick color
 * @param color New axis tick color
 * \endif
 *
 * \if CHINESE
 * @brief 设置坐标轴刻度颜色
 * @param color 新的坐标轴刻度颜色
 * \endif
 */
void QImPlot3DStyleNode::setAxisTickColor(const QColor& color)
{
    QIM_D(d);
    if (d->axisTickColor != color) {
        d->axisTickColor = color;
        d->isAutoAxisTickColor = false;
        Q_EMIT styleChanged();
    }
}

//----------------------------------------------------
// Theme convenience methods
//----------------------------------------------------

/**
 * \if ENGLISH
 * @brief Applies the Auto theme (colors derived from current ImGui style)
 * @details Calls ImPlot3D::StyleColorsAuto() and reads all resulting color values
 *          to update the internal Q_PROPERTY color storage.
 * \endif
 *
 * \if CHINESE
 * @brief 应用自动主题（颜色从当前 ImGui 样式派生）
 * @details 调用 ImPlot3D::StyleColorsAuto() 并读取所有结果颜色值
 *          以更新内部的 Q_PROPERTY 颜色存储。
 * \endif
 */
void QImPlot3DStyleNode::styleColorsAuto()
{
    QIM_D(d);
    // All colors become IMPLOT3D_AUTO_COL — set isAuto flags, don't call toQColor on AUTO_COL
    d->isAutoTitleTextColor = true;
    d->isAutoInlayTextColor = true;
    d->isAutoFrameBgColor = true;
    d->isAutoPlotBgColor = true;
    d->isAutoPlotBorderColor = true;
    d->isAutoLegendBgColor = true;
    d->isAutoLegendBorderColor = true;
    d->isAutoLegendTextColor = true;
    d->isAutoAxisTextColor = true;
    d->isAutoAxisGridColor = true;
    d->isAutoAxisTickColor = true;
    Q_EMIT styleChanged();
}

/**
 * \if ENGLISH
 * @brief Applies the Dark theme
 * @details Calls ImPlot3D::StyleColorsDark() and reads all resulting color values
 *          to update the internal Q_PROPERTY color storage.
 * \endif
 *
 * \if CHINESE
 * @brief 应用暗色主题
 * @details 调用 ImPlot3D::StyleColorsDark() 并读取所有结果颜色值
 *          以更新内部的 Q_PROPERTY 颜色存储。
 * \endif
 */
void QImPlot3DStyleNode::styleColorsDark()
{
    QIM_D(d);
    ImPlot3DStyle tempStyle;
    ImPlot3D::StyleColorsDark(&tempStyle);
    // Lambda to detect IMPLOT3D_AUTO_COL sentinel before toQColor conversion
    auto isAutoCol = [](const ImVec4& c) -> bool {
        return c.x == 0.0f && c.y == 0.0f && c.z == 0.0f && c.w == -1.0f;
    };
    // For each color: if AUTO_COL, set isAuto=true and skip toQColor (which corrupts it);
    // otherwise, set isAuto=false and store the explicit QColor value.
    const ImVec4& titleTextCol = tempStyle.Colors[ImPlot3DCol_TitleText];
    d->isAutoTitleTextColor = isAutoCol(titleTextCol);
    if (!d->isAutoTitleTextColor) { d->titleTextColor = toQColor(titleTextCol); }

    const ImVec4& inlayTextCol = tempStyle.Colors[ImPlot3DCol_InlayText];
    d->isAutoInlayTextColor = isAutoCol(inlayTextCol);
    if (!d->isAutoInlayTextColor) { d->inlayTextColor = toQColor(inlayTextCol); }

    const ImVec4& frameBgCol = tempStyle.Colors[ImPlot3DCol_FrameBg];
    d->isAutoFrameBgColor = isAutoCol(frameBgCol);
    if (!d->isAutoFrameBgColor) { d->frameBgColor = toQColor(frameBgCol); }

    const ImVec4& plotBgCol = tempStyle.Colors[ImPlot3DCol_PlotBg];
    d->isAutoPlotBgColor = isAutoCol(plotBgCol);
    if (!d->isAutoPlotBgColor) { d->plotBgColor = toQColor(plotBgCol); }

    const ImVec4& plotBorderCol = tempStyle.Colors[ImPlot3DCol_PlotBorder];
    d->isAutoPlotBorderColor = isAutoCol(plotBorderCol);
    if (!d->isAutoPlotBorderColor) { d->plotBorderColor = toQColor(plotBorderCol); }

    const ImVec4& legendBgCol = tempStyle.Colors[ImPlot3DCol_LegendBg];
    d->isAutoLegendBgColor = isAutoCol(legendBgCol);
    if (!d->isAutoLegendBgColor) { d->legendBgColor = toQColor(legendBgCol); }

    const ImVec4& legendBorderCol = tempStyle.Colors[ImPlot3DCol_LegendBorder];
    d->isAutoLegendBorderColor = isAutoCol(legendBorderCol);
    if (!d->isAutoLegendBorderColor) { d->legendBorderColor = toQColor(legendBorderCol); }

    const ImVec4& legendTextCol = tempStyle.Colors[ImPlot3DCol_LegendText];
    d->isAutoLegendTextColor = isAutoCol(legendTextCol);
    if (!d->isAutoLegendTextColor) { d->legendTextColor = toQColor(legendTextCol); }

    const ImVec4& axisTextCol = tempStyle.Colors[ImPlot3DCol_AxisText];
    d->isAutoAxisTextColor = isAutoCol(axisTextCol);
    if (!d->isAutoAxisTextColor) { d->axisTextColor = toQColor(axisTextCol); }

    const ImVec4& axisGridCol = tempStyle.Colors[ImPlot3DCol_AxisGrid];
    d->isAutoAxisGridColor = isAutoCol(axisGridCol);
    if (!d->isAutoAxisGridColor) { d->axisGridColor = toQColor(axisGridCol); }

    const ImVec4& axisTickCol = tempStyle.Colors[ImPlot3DCol_AxisTick];
    d->isAutoAxisTickColor = isAutoCol(axisTickCol);
    if (!d->isAutoAxisTickColor) { d->axisTickColor = toQColor(axisTickCol); }

    Q_EMIT styleChanged();
}

/**
 * \if ENGLISH
 * @brief Applies the Light theme
 * @details Calls ImPlot3D::StyleColorsLight() and reads all resulting color values
 *          to update the internal Q_PROPERTY color storage.
 * \endif
 *
 * \if CHINESE
 * @brief 应用亮色主题
 * @details 调用 ImPlot3D::StyleColorsLight() 并读取所有结果颜色值
 *          以更新内部的 Q_PROPERTY 颜色存储。
 * \endif
 */
void QImPlot3DStyleNode::styleColorsLight()
{
    QIM_D(d);
    ImPlot3DStyle tempStyle;
    ImPlot3D::StyleColorsLight(&tempStyle);
    auto isAutoCol = [](const ImVec4& c) -> bool {
        return c.x == 0.0f && c.y == 0.0f && c.z == 0.0f && c.w == -1.0f;
    };
    const ImVec4& titleTextCol = tempStyle.Colors[ImPlot3DCol_TitleText];
    d->isAutoTitleTextColor = isAutoCol(titleTextCol);
    if (!d->isAutoTitleTextColor) { d->titleTextColor = toQColor(titleTextCol); }

    const ImVec4& inlayTextCol = tempStyle.Colors[ImPlot3DCol_InlayText];
    d->isAutoInlayTextColor = isAutoCol(inlayTextCol);
    if (!d->isAutoInlayTextColor) { d->inlayTextColor = toQColor(inlayTextCol); }

    const ImVec4& frameBgCol = tempStyle.Colors[ImPlot3DCol_FrameBg];
    d->isAutoFrameBgColor = isAutoCol(frameBgCol);
    if (!d->isAutoFrameBgColor) { d->frameBgColor = toQColor(frameBgCol); }

    const ImVec4& plotBgCol = tempStyle.Colors[ImPlot3DCol_PlotBg];
    d->isAutoPlotBgColor = isAutoCol(plotBgCol);
    if (!d->isAutoPlotBgColor) { d->plotBgColor = toQColor(plotBgCol); }

    const ImVec4& plotBorderCol = tempStyle.Colors[ImPlot3DCol_PlotBorder];
    d->isAutoPlotBorderColor = isAutoCol(plotBorderCol);
    if (!d->isAutoPlotBorderColor) { d->plotBorderColor = toQColor(plotBorderCol); }

    const ImVec4& legendBgCol = tempStyle.Colors[ImPlot3DCol_LegendBg];
    d->isAutoLegendBgColor = isAutoCol(legendBgCol);
    if (!d->isAutoLegendBgColor) { d->legendBgColor = toQColor(legendBgCol); }

    const ImVec4& legendBorderCol = tempStyle.Colors[ImPlot3DCol_LegendBorder];
    d->isAutoLegendBorderColor = isAutoCol(legendBorderCol);
    if (!d->isAutoLegendBorderColor) { d->legendBorderColor = toQColor(legendBorderCol); }

    const ImVec4& legendTextCol = tempStyle.Colors[ImPlot3DCol_LegendText];
    d->isAutoLegendTextColor = isAutoCol(legendTextCol);
    if (!d->isAutoLegendTextColor) { d->legendTextColor = toQColor(legendTextCol); }

    const ImVec4& axisTextCol = tempStyle.Colors[ImPlot3DCol_AxisText];
    d->isAutoAxisTextColor = isAutoCol(axisTextCol);
    if (!d->isAutoAxisTextColor) { d->axisTextColor = toQColor(axisTextCol); }

    const ImVec4& axisGridCol = tempStyle.Colors[ImPlot3DCol_AxisGrid];
    d->isAutoAxisGridColor = isAutoCol(axisGridCol);
    if (!d->isAutoAxisGridColor) { d->axisGridColor = toQColor(axisGridCol); }

    const ImVec4& axisTickCol = tempStyle.Colors[ImPlot3DCol_AxisTick];
    d->isAutoAxisTickColor = isAutoCol(axisTickCol);
    if (!d->isAutoAxisTickColor) { d->axisTickColor = toQColor(axisTickCol); }

    Q_EMIT styleChanged();
}

/**
 * \if ENGLISH
 * @brief Applies the Classic theme
 * @details Calls ImPlot3D::StyleColorsClassic() and reads all resulting color values
 *          to update the internal Q_PROPERTY color storage.
 * \endif
 *
 * \if CHINESE
 * @brief 应用经典主题
 * @details 调用 ImPlot3D::StyleColorsClassic() 并读取所有结果颜色值
 *          以更新内部的 Q_PROPERTY 颜色存储。
 * \endif
 */
void QImPlot3DStyleNode::styleColorsClassic()
{
    QIM_D(d);
    ImPlot3DStyle tempStyle;
    ImPlot3D::StyleColorsClassic(&tempStyle);
    auto isAutoCol = [](const ImVec4& c) -> bool {
        return c.x == 0.0f && c.y == 0.0f && c.z == 0.0f && c.w == -1.0f;
    };
    const ImVec4& titleTextCol = tempStyle.Colors[ImPlot3DCol_TitleText];
    d->isAutoTitleTextColor = isAutoCol(titleTextCol);
    if (!d->isAutoTitleTextColor) { d->titleTextColor = toQColor(titleTextCol); }

    const ImVec4& inlayTextCol = tempStyle.Colors[ImPlot3DCol_InlayText];
    d->isAutoInlayTextColor = isAutoCol(inlayTextCol);
    if (!d->isAutoInlayTextColor) { d->inlayTextColor = toQColor(inlayTextCol); }

    const ImVec4& frameBgCol = tempStyle.Colors[ImPlot3DCol_FrameBg];
    d->isAutoFrameBgColor = isAutoCol(frameBgCol);
    if (!d->isAutoFrameBgColor) { d->frameBgColor = toQColor(frameBgCol); }

    const ImVec4& plotBgCol = tempStyle.Colors[ImPlot3DCol_PlotBg];
    d->isAutoPlotBgColor = isAutoCol(plotBgCol);
    if (!d->isAutoPlotBgColor) { d->plotBgColor = toQColor(plotBgCol); }

    const ImVec4& plotBorderCol = tempStyle.Colors[ImPlot3DCol_PlotBorder];
    d->isAutoPlotBorderColor = isAutoCol(plotBorderCol);
    if (!d->isAutoPlotBorderColor) { d->plotBorderColor = toQColor(plotBorderCol); }

    const ImVec4& legendBgCol = tempStyle.Colors[ImPlot3DCol_LegendBg];
    d->isAutoLegendBgColor = isAutoCol(legendBgCol);
    if (!d->isAutoLegendBgColor) { d->legendBgColor = toQColor(legendBgCol); }

    const ImVec4& legendBorderCol = tempStyle.Colors[ImPlot3DCol_LegendBorder];
    d->isAutoLegendBorderColor = isAutoCol(legendBorderCol);
    if (!d->isAutoLegendBorderColor) { d->legendBorderColor = toQColor(legendBorderCol); }

    const ImVec4& legendTextCol = tempStyle.Colors[ImPlot3DCol_LegendText];
    d->isAutoLegendTextColor = isAutoCol(legendTextCol);
    if (!d->isAutoLegendTextColor) { d->legendTextColor = toQColor(legendTextCol); }

    const ImVec4& axisTextCol = tempStyle.Colors[ImPlot3DCol_AxisText];
    d->isAutoAxisTextColor = isAutoCol(axisTextCol);
    if (!d->isAutoAxisTextColor) { d->axisTextColor = toQColor(axisTextCol); }

    const ImVec4& axisGridCol = tempStyle.Colors[ImPlot3DCol_AxisGrid];
    d->isAutoAxisGridColor = isAutoCol(axisGridCol);
    if (!d->isAutoAxisGridColor) { d->axisGridColor = toQColor(axisGridCol); }

    const ImVec4& axisTickCol = tempStyle.Colors[ImPlot3DCol_AxisTick];
    d->isAutoAxisTickColor = isAutoCol(axisTickCol);
    if (!d->isAutoAxisTickColor) { d->axisTickColor = toQColor(axisTickCol); }

    Q_EMIT styleChanged();
}

//----------------------------------------------------
// Style application
//----------------------------------------------------

/**
 * \if ENGLISH
 * @brief Applies all stored style properties to ImPlot3D::GetStyle()
 * @details Called inside BeginPlot/EndPlot block before child items render.
 *          Sets all style variables and colors directly on the current ImPlot3DStyle
 *          via direct field assignment — no PushStyleVar/PopStyleVar needed.
 * \endif
 *
 * \if CHINESE
 * @brief 将所有存储的样式属性应用到 ImPlot3D::GetStyle()
 * @details 在 BeginPlot/EndPlot 块内调用，在子元素渲染之前。
 *          通过直接字段赋值设置当前 ImPlot3DStyle 的所有样式变量和颜色
 *          — 不需要 PushStyleVar/PopStyleVar。
 * \endif
 */
void QImPlot3DStyleNode::applyStyle() const
{
    QIM_DC(d);
    ImPlot3DStyle& style = ImPlot3D::GetStyle();

    // Style variables
    style.LineWeight = d->lineWeight;
    style.Marker = toImPlot3DMarker(static_cast<QImPlot3DMarkerShape>(d->marker));
    style.MarkerSize = d->markerSize;
    style.MarkerWeight = d->markerWeight;
    style.FillAlpha = d->fillAlpha;
    style.PlotDefaultSize = d->plotDefaultSize;
    style.PlotMinSize = d->plotMinSize;
    style.PlotPadding = d->plotPadding;
    style.LabelPadding = d->labelPadding;
    style.ViewScaleFactor = d->viewScaleFactor;
    style.LegendPadding = d->legendPadding;
    style.LegendInnerPadding = d->legendInnerPadding;
    style.LegendSpacing = d->legendSpacing;

    // Plot colors
    if (d->isAutoTitleTextColor) {
        style.Colors[ImPlot3DCol_TitleText] = IMPLOT3D_AUTO_COL;
    } else {
        style.Colors[ImPlot3DCol_TitleText] = toImVec4(d->titleTextColor);
    }
    if (d->isAutoInlayTextColor) {
        style.Colors[ImPlot3DCol_InlayText] = IMPLOT3D_AUTO_COL;
    } else {
        style.Colors[ImPlot3DCol_InlayText] = toImVec4(d->inlayTextColor);
    }
    if (d->isAutoFrameBgColor) {
        style.Colors[ImPlot3DCol_FrameBg] = IMPLOT3D_AUTO_COL;
    } else {
        style.Colors[ImPlot3DCol_FrameBg] = toImVec4(d->frameBgColor);
    }
    if (d->isAutoPlotBgColor) {
        style.Colors[ImPlot3DCol_PlotBg] = IMPLOT3D_AUTO_COL;
    } else {
        style.Colors[ImPlot3DCol_PlotBg] = toImVec4(d->plotBgColor);
    }
    if (d->isAutoPlotBorderColor) {
        style.Colors[ImPlot3DCol_PlotBorder] = IMPLOT3D_AUTO_COL;
    } else {
        style.Colors[ImPlot3DCol_PlotBorder] = toImVec4(d->plotBorderColor);
    }

    // Legend colors
    if (d->isAutoLegendBgColor) {
        style.Colors[ImPlot3DCol_LegendBg] = IMPLOT3D_AUTO_COL;
    } else {
        style.Colors[ImPlot3DCol_LegendBg] = toImVec4(d->legendBgColor);
    }
    if (d->isAutoLegendBorderColor) {
        style.Colors[ImPlot3DCol_LegendBorder] = IMPLOT3D_AUTO_COL;
    } else {
        style.Colors[ImPlot3DCol_LegendBorder] = toImVec4(d->legendBorderColor);
    }
    if (d->isAutoLegendTextColor) {
        style.Colors[ImPlot3DCol_LegendText] = IMPLOT3D_AUTO_COL;
    } else {
        style.Colors[ImPlot3DCol_LegendText] = toImVec4(d->legendTextColor);
    }

    // Axis colors
    if (d->isAutoAxisTextColor) {
        style.Colors[ImPlot3DCol_AxisText] = IMPLOT3D_AUTO_COL;
    } else {
        style.Colors[ImPlot3DCol_AxisText] = toImVec4(d->axisTextColor);
    }
    if (d->isAutoAxisGridColor) {
        style.Colors[ImPlot3DCol_AxisGrid] = IMPLOT3D_AUTO_COL;
    } else {
        style.Colors[ImPlot3DCol_AxisGrid] = toImVec4(d->axisGridColor);
    }
    if (d->isAutoAxisTickColor) {
        style.Colors[ImPlot3DCol_AxisTick] = IMPLOT3D_AUTO_COL;
    } else {
        style.Colors[ImPlot3DCol_AxisTick] = toImVec4(d->axisTickColor);
    }
}

}  // namespace QIM