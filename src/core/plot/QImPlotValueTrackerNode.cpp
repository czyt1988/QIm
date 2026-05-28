// MSVC requires _USE_MATH_DEFINES for M_PI
#ifndef _USE_MATH_DEFINES
#    define _USE_MATH_DEFINES
#endif
#include "QImPlotValueTrackerNode.h"
#include <algorithm>
#include <cmath>
#include <QtGlobal>
#include "implot.h"
#include "implot_internal.h"
#include "imgui.h"
#include "QImPlotNode.h"
#include "QImPlotItemNode.h"
#include "QImAbstractXYSeriesItemNode.h"
#include "QImPlotBarGroupsItemNode.h"
#include "QImPlotPieChartItemNode.h"
#include "QImPlotDataSeries.h"
#include "QImPlotValueTrackerNodeGroup.h"
#include "QtImGuiUtils.h"
#include "QImPlotAxisInfo.h"
namespace QIM
{

namespace {
constexpr double kPieCenterEpsilon  = 0.001;   // Threshold for atan2(0,0) guard at pie center
constexpr float  kTooltipOffsetPx   = 10.0f;   // Horizontal offset between cursor and tooltip
constexpr int    kTrackerZOrder      = 99999;   // Z-order to render tracker above all items
} // anonymous namespace

class QImPlotValueTrackerNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlotValueTrackerNode)
public:
    PrivateData(QImPlotValueTrackerNode* p);

    std::vector< TrackedValue > trackedValues;
    QImPlotNode* plotNode { nullptr };                ///< 绘图节点
    bool isActive { false };                          ///< 是否激活
    bool lastActiveState { false };  ///< 记录上次激活状态,这个是用于识别首次active状态变化的辅助变量
    bool skipNanFiniteValues { false };  ///< 是否跳过nan
    // 屏幕相关参数
    ImVec2 mouseScreenPos;
    ImVec2 plotScreenPos;   ///< 记录绘图尺寸位置
    ImVec2 plotScreenSize;  ///< 记录绘图尺寸
    ImRect plotCanvasRect;  ///< 记录canvas矩形，用于把tooltip放到顶部
    //
    ImVec2 lastCalcMouseScreenPos { -1.0f, -1.0f };  ///< 记录上次计算时的鼠标位置，避免不动也计算
    QImPlotValueTrackerNodeGroup* group { nullptr };

    // 样式设置
    bool autoWidth { true };
    float fixedWidth { 200.0f };
    float tooltipPadding { 3.0f };
    float colorBoxSize { 12.0f };
    float fontSize { 13.0f };
    QColor textColor { 255, 255, 255 };
    QColor bgColor { 30, 30, 30, 150 };
    QColor borderColor { 100, 100, 100, 255 };
    ImU32 trackerLineColor = IM_COL32(216, 234, 248, 255);
    // 纯数据状态更新（无任何 ImGui/ImPlot 调用！）
    void updateTrackingState();
    void processBarGroupsTracking(QImPlotBarGroupsItemNode* barItem);
    void processPieChartTracking(QImPlotPieChartItemNode* pieItem);
};

QImPlotValueTrackerNode::PrivateData::PrivateData(QImPlotValueTrackerNode* p) : q_ptr(p)
{
}

/**
 * \if ENGLISH
 * @brief Processes tracking for BarGroups (grouped bar chart) nodes
 * @details For each visible BarGroups node, computes the nearest group index
 *          from the mouse X position and emits tracked values with group labels.
 *          Accounts for shift offset, horizontal orientation, and colormap colors.
 * \endif
 *
 * \if CHINESE
 * @brief 处理分组柱状图节点的追踪
 * @details 对每个可见的分组柱状图节点，根据鼠标X位置计算最近的组索引，
 *          并使用组标签发出追踪值。考虑偏移量、水平方向和颜色映射。
 * \endif
 */
void QImPlotValueTrackerNode::PrivateData::processBarGroupsTracking(QImPlotBarGroupsItemNode* barItem)
{
    if (!barItem->isVisible() || !barItem->isEnabled()) {
        return;
    }
    auto* series = barItem->data();
    if (!series || series->itemCount() <= 0 || series->groupCount() <= 0) {
        return;
    }

    QPointF plotPos = barItem->pixelsToPlot(mouseScreenPos.x, mouseScreenPos.y);
    int itemCount   = series->itemCount();
    int groupCount  = series->groupCount();
    bool horizontal = barItem->isHorizontal();
    double shift    = barItem->shift();

    // Compute the nearest item index from the mouse plot position
    double mouseCoord = horizontal ? plotPos.y() : plotPos.x();
    int nearestItem   = static_cast< int >(std::round(mouseCoord - shift));
    nearestItem       = qBound(0, nearestItem, itemCount - 1);

    // Get labels with axis tick priority over series labels
    QStringList primaryLabels;
    const QImPlotAxisInfo* relevantAxis = horizontal
        ? plotNode->y1Axis()
        : plotNode->x1Axis();
    if (relevantAxis && !relevantAxis->tickLabels().isEmpty()) {
        QList<QByteArray> tickLabels = relevantAxis->tickLabels();
        for (const auto& tl : tickLabels) {
            primaryLabels << QString::fromUtf8(tl);
        }
    }
    if (primaryLabels.isEmpty()) {
        primaryLabels = series->labels();
    }

    // Build group labels as "Group 0", "Group 1", ...
    QStringList groupLabels;
    for (int g = 0; g < groupCount; ++g) {
        groupLabels << QStringLiteral("Group %1").arg(g);
    }

    for (int g = 0; g < groupCount; ++g) {
        double val = series->value(nearestItem, g);
        if (skipNanFiniteValues && (std::isnan(val) || std::isinf(val) || !std::isfinite(val))) {
            continue;
        }

        QColor color = toQColor(ImPlot::GetColormapColor(g));

        TrackedValue tv;
        tv.sourceType = SourceType::BarGroups;
        tv.label      = QStringLiteral("%1 [%2]").arg(primaryLabels.value(nearestItem), groupLabels.value(g)).toStdString();
        tv.color      = color;
        tv.xValue     = nearestItem;
        tv.yValue     = val;
        char buf[ 64 ];
        ImFormatString(buf, sizeof(buf), "%.3f", tv.yValue);
        tv.yValueLabel = buf;
        char xbuf[ 32 ];
        ImFormatString(xbuf, sizeof(xbuf), "%d", nearestItem);
        tv.xValueLabel = xbuf;
        trackedValues.emplace_back(tv);
    }
}

/**
 * \if ENGLISH
 * @brief Processes tracking for PieChart nodes using angle-based nearest-slice detection
 * @details For the given PieChart node, converts mouse position to plot coordinates,
 *          computes distance and angle from the pie center, and identifies the single
 *          hovered slice. Emits one TrackedValue for the hovered slice using colormap
 *          color. Guards against clicks outside the pie radius or at the exact center.
 * \endif
 *
 * \if CHINESE
 * @brief 使用角度最近扇区检测处理饼图节点的追踪
 * @details 对给定的饼图节点，将鼠标位置转换为绘图坐标，计算与饼图中心的距离和角度，
 *          识别鼠标悬停的单个扇区。使用颜色映射颜色发出一个追踪值。
 *          防止点击饼图半径外或中心点时的无效检测。
 * \endif
 */
void QImPlotValueTrackerNode::PrivateData::processPieChartTracking(QImPlotPieChartItemNode* pieItem)
{
    auto* series = pieItem->data();
    if (!series || series->sliceCount() <= 0) {
        return;
    }

    QPointF center      = pieItem->center();
    double radius       = pieItem->radius();
    double startAngle   = pieItem->startAngle();
    QString labelFmt    = pieItem->labelFormat();
    bool normalized     = pieItem->isNormalized();

    // Convert mouse screen position to plot coordinates
    QPointF mousePlot = pieItem->pixelsToPlot(mouseScreenPos.x, mouseScreenPos.y);

    // Compute distance from mouse to pie center
    double dx   = mousePlot.x() - center.x();
    double dy   = mousePlot.y() - center.y();
    double dist = std::sqrt(dx * dx + dy * dy);

    // Outside the pie radius or at the exact center (atan2 undefined)
    if (dist > radius || dist < kPieCenterEpsilon) {
        return;
    }

    // Compute mouse angle in degrees, normalized to [0, 360)
    double mouseAngleDeg = std::atan2(dy, dx) * 180.0 / M_PI;
    if (mouseAngleDeg < 0.0) {
        mouseAngleDeg += 360.0;
    }

    // Convert to ImPlot coordinate system (relative to start angle)
    double relativeAngle = mouseAngleDeg - startAngle;
    if (relativeAngle < 0.0) {
        relativeAngle += 360.0;
    }

    // Find which slice the mouse angle falls into
    int sliceCount    = series->sliceCount();
    double total      = 0.0;
    for (int i = 0; i < sliceCount; ++i) {
        total += series->value(i);
    }

    int hoveredSlice    = -1;
    double cumulativeAngle = 0.0;
    for (int i = 0; i < sliceCount; ++i) {
        double val      = series->value(i);
        double sliceArc = (total > 0.0) ? (val / total) * 360.0 : 0.0;
        if (relativeAngle >= cumulativeAngle && relativeAngle < (cumulativeAngle + sliceArc)) {
            hoveredSlice = i;
            break;
        }
        cumulativeAngle += sliceArc;
    }

    if (hoveredSlice < 0) {
        return;
    }

    double val = series->value(hoveredSlice);
    if (skipNanFiniteValues && (std::isnan(val) || std::isinf(val) || !std::isfinite(val))) {
        return;
    }

    // Build a single TrackedValue for the hovered slice (colormap color only)
    QColor color;
    ImVec4 c = ImPlot::GetColormapColor(hoveredSlice);
    color    = toQColor(c);

    TrackedValue tv;
    tv.sourceType = SourceType::PieChart;
    tv.color      = color;

    QStringList labels = series->labels();
    QString sliceLabel = labels.value(hoveredSlice, QStringLiteral("Slice %1").arg(hoveredSlice));
    if (labelFmt.isEmpty()) {
        tv.label = sliceLabel.toStdString();
    } else {
        double pct = (total > 0.0) ? (val / total * 100.0) : 0.0;
        char lblBuf[ 128 ];
        ImFormatString(lblBuf, sizeof(lblBuf), labelFmt.toUtf8().constData(), sliceLabel.toUtf8().constData(), pct);
        tv.label = lblBuf;
    }

    tv.xValue = hoveredSlice;
    tv.yValue = normalized ? (total > 0.0 ? val / total : 0.0) : val;

    char ybuf[ 64 ];
    ImFormatString(ybuf, sizeof(ybuf), "%.3f", tv.yValue);
    tv.yValueLabel = ybuf;
    char xbuf[ 32 ];
    ImFormatString(xbuf, sizeof(xbuf), "%d", hoveredSlice);
    tv.xValueLabel = xbuf;
    trackedValues.emplace_back(tv);
}

/**
 * \if ENGLISH
 * @brief Updates the tracking state and computes tracked values at the mouse position
 * @details Core state machine that determines whether the tracker is active based on
 *          mouse hover state, handles group synchronization for multi-plot tracking,
 *          and computes Y values for all visible line items at the current mouse X position.
 *          Also processes BarGroups and PieChart items.
 *          Updates tracked values only when the mouse has moved significantly.
 * \endif
 *
 * \if CHINESE
 * @brief 更新追踪状态并计算鼠标位置处的追踪值
 * @details 核心状态机，根据鼠标悬停状态确定追踪器是否激活，处理多子图联动的组同步，
 *          并计算当前鼠标X位置处所有可见折线项目的Y值。
 *          同时处理分组柱状图和饼图项目。
 *          仅在鼠标发生显著移动时更新追踪值。
 * \endif
 */
void QImPlotValueTrackerNode::PrivateData::updateTrackingState()
{
    bool isMouseOnPlot = plotNode->isPlotHovered();
    if (isMouseOnPlot) {
        this->isActive        = true;
        this->lastActiveState = true;
        // 更新Group状态（仅当有Group时）
        if (this->group) {
            // 计算x比例，其它绘图保持等比例x
            float pixelRatio = (plotScreenSize.x > 0)
                                   ? qBound(0.0f, (mouseScreenPos.x - plotScreenPos.x) / plotScreenSize.x, 1.0f)
                                   : 0.0f;
            this->group->updateActiveTracker(q_ptr, pixelRatio);
        }
    } else {
        this->isActive = false;
        if (this->lastActiveState) {
            this->lastActiveState = false;
            // 说明是从激活变到非激活
            if (this->group) {
                this->group->updateActiveTracker(nullptr, -1.0f);
            }
        }
    }
    if (!isMouseOnPlot && this->group && this->group->isActive()) {
        // 自身没hovered，但分组激活
        this->isActive        = true;  // 这时也属于active
        float groupPixelRatio = this->group->pixelRatio();

        if (groupPixelRatio >= 0.0f && groupPixelRatio <= 1.0f) {
            mouseScreenPos.x = plotScreenPos.x + (plotScreenSize.x * groupPixelRatio);
            mouseScreenPos.y = plotCanvasRect.GetTL().y;
        } else {
            this->isActive = false;
            return;
        }
    }

    // 仅当激活且鼠标移动时更新数据（优化：避免重复计算）
    if (isActive && !fuzzyEqual(lastCalcMouseScreenPos, mouseScreenPos, 0.5f)) {
        lastCalcMouseScreenPos = mouseScreenPos;
        trackedValues.clear();

        // === XY series tracking ===
        for (QImAbstractNode* itemNode : plotNode->plotItemNodes()) {
            if (!itemNode->isVisible() || !itemNode->isEnabled()) {
                continue;
            }

            if (auto* xyItem = qobject_cast< QImAbstractXYSeriesItemNode* >(itemNode)) {
                QImAbstractXYDataSeries* series = xyItem->data();
                if (!series || series->size() <= 0) {
                    continue;
                }
                QPointF plotPos = xyItem->pixelsToPlot(mouseScreenPos.x, mouseScreenPos.y);

                double firstX = series->xValue(0);
                double lastX  = series->xValue(series->size() - 1);
                if (plotPos.x() < firstX || plotPos.x() > lastX) {
                    continue;
                }

                double yVal = series->yValueAtX(plotPos.x());

                if (skipNanFiniteValues) {
                    if (std::isnan(yVal) || std::isinf(yVal) || !std::isfinite(yVal)) {
                        continue;
                    }
                }

                QColor lineColor = xyItem->itemColor();
                if (!lineColor.isValid()) {
                    ImVec4 lastItemColor = ImPlot::GetLastItemColor();
                    lineColor            = toQColor(lastItemColor);
                }

                TrackedValue trackedValue;
                trackedValue.sourceType  = SourceType::XY;
                trackedValue.label       = xyItem->labelConstData();
                trackedValue.color       = lineColor;
                trackedValue.xValue      = plotPos.x();
                trackedValue.yValue      = yVal;
                trackedValue.xValueLabel = plotNode->axisValueText(plotPos.x(), xyItem->xAxisId());
                trackedValue.yValueLabel = plotNode->axisValueText(yVal, xyItem->yAxisId());
                trackedValues.emplace_back(trackedValue);
            }
        }

        // === BarGroups tracking (dynamic) ===
        for (QImAbstractNode* itemNode : plotNode->plotItemNodes()) {
            if (!itemNode->isVisible() || !itemNode->isEnabled()) continue;
            if (auto* barItem = qobject_cast<QImPlotBarGroupsItemNode*>(itemNode)) {
                if (barItem->data() && barItem->data()->itemCount() > 0 && barItem->data()->groupCount() > 0)
                    processBarGroupsTracking(barItem);
            }
        }

        // === PieChart tracking (dynamic) ===
        for (QImAbstractNode* itemNode : plotNode->plotItemNodes()) {
            if (!itemNode->isVisible() || !itemNode->isEnabled()) continue;
            if (auto* pieItem = qobject_cast<QImPlotPieChartItemNode*>(itemNode)) {
                if (pieItem->data() && pieItem->data()->sliceCount() > 0)
                    processPieChartTracking(pieItem);
            }
        }
    }
}

//----------------------------------------------------
// QImPlotValueTrackerNode
//----------------------------------------------------
/**
 * \if ENGLISH
 * @brief Constructs a value tracker node attached to a plot node
 * @param[in] plotNode The plot node to track values in
 * @param[in] parent Optional parent QObject
 * @details Automatically connects to the plot's childNodeAdded/childNodeRemoved signals
 *          and initializes the support series list. Sets high Z-order to render on top.
 * \endif
 *
 * \if CHINESE
 * @brief 构造附着于绘图节点的值追踪器节点
 * @param[in] plotNode 要追踪值的绘图节点
 * @param[in] parent 可选的父QObject
 * @details 自动连接绘图的childNodeAdded/childNodeRemoved信号并初始化支持序列列表。
 *          设置高Z序以在最上层渲染。
 * \endif
 */
QImPlotValueTrackerNode::QImPlotValueTrackerNode(QImPlotNode* plotNode, QObject* parent)
    : QImAbstractNode(parent), QIM_PIMPL_CONSTRUCT
{
    QIM_D(d);
    d->plotNode = plotNode;
    setObjectName(QStringLiteral("QImPlotValueTrackerNode"));
    setZOrder(kTrackerZOrder);  // 确保绘制在最上层
    setAutoIdEnabled(true);
}

/**
 * \if ENGLISH
 * @brief Destroys the tracker and removes it from its group
 * \endif
 *
 * \if CHINESE
 * @brief 析构追踪器并从其所属组中移除
 * \endif
 */
QImPlotValueTrackerNode::~QImPlotValueTrackerNode()
{
    QIM_D(d);
    if (d->group) {
        d->group->removeTracker(this);
    }
}

/**
 * \if ENGLISH
 * @brief Assigns this tracker to a group for synchronized multi-plot tracking
 * @param[in] group The tracker group to join, or nullptr to leave the current group
 * \endif
 *
 * \if CHINESE
 * @brief 将此追踪器分配到组中，实现多子图联动追踪
 * @param[in] group 要加入的追踪器组，或nullptr退出当前组
 * \endif
 */
void QImPlotValueTrackerNode::setGroup(QImPlotValueTrackerNodeGroup* group)
{
    QIM_D(d);
    if (d->group == group) {
        return;
    }

    if (d->group) {
        d->group->removeTracker(this);
    }

    d->group = group;

    if (group) {
        group->addTracker(this);
    }
}

/**
 * \if ENGLISH
 * @brief Returns whether this tracker belongs to a group
 * @return true if the tracker is part of a tracker group
 * \endif
 *
 * \if CHINESE
 * @brief 返回此追踪器是否属于某个组
 * @return 追踪器属于追踪器组返回true
 * \endif
 */
bool QImPlotValueTrackerNode::hasGroup() const
{
    QIM_DC(d);
    return (d->group != nullptr);
}

/**
 * \if ENGLISH
 * @brief Returns the tracker group this tracker belongs to
 * @return The tracker group, or nullptr if not in a group
 * \endif
 *
 * \if CHINESE
 * @brief 返回此追踪器所属的追踪器组
 * @return 追踪器组，未加入任何组则返回nullptr
 * \endif
 */
QImPlotValueTrackerNodeGroup* QImPlotValueTrackerNode::group() const
{
    QIM_DC(d);
    return d->group;
}

/**
 * \if ENGLISH
 * @brief Enables or disables skipping NaN and non-finite values during tracking
 * @param[in] on true to skip invalid values, false to include them
 * \endif
 *
 * \if CHINESE
 * @brief 启用或禁用追踪时跳过NaN和非有限值
 * @param[in] on true跳过无效值，false包含无效值
 * \endif
 */
void QImPlotValueTrackerNode::setSkipNanFiniteValues(bool on)
{
    QIM_D(d);
    d->skipNanFiniteValues = on;
}

/**
 * \if ENGLISH
 * @brief Returns whether NaN and non-finite values are skipped during tracking
 * @return true if invalid values are skipped
 * \endif
 *
 * \if CHINESE
 * @brief 返回追踪时是否跳过NaN和非有限值
 * @return 跳过无效值返回true
 * \endif
 */
bool QImPlotValueTrackerNode::isSkipNanFiniteValues() const
{
    QIM_DC(d);
    return d->skipNanFiniteValues;
}

void QImPlotValueTrackerNode::setFixedWidth(float width)
{
    QIM_D(d);
    d->fixedWidth = width;
}

float QIM::QImPlotValueTrackerNode::fixedWidth() const
{
    QIM_DC(d);
    return d->fixedWidth;
}

void QImPlotValueTrackerNode::setAutoWidthEnabled(bool on)
{
    QIM_D(d);
    d->autoWidth = on;
}

bool QImPlotValueTrackerNode::isAutoWidthEnabled() const
{
    QIM_DC(d);
    return d->autoWidth;
}

void QImPlotValueTrackerNode::setTextColor(const QColor& color)
{
    QIM_D(d);
    d->textColor = color;
}

QColor QImPlotValueTrackerNode::textColor() const
{
    QIM_DC(d);
    return d->textColor;
}

void QImPlotValueTrackerNode::setBackgroundColor(const QColor& color)
{
    QIM_D(d);
    d->bgColor = color;
}

QColor QImPlotValueTrackerNode::backgroundColor() const
{
    QIM_DC(d);
    return d->bgColor;
}

void QImPlotValueTrackerNode::setBorderColor(const QColor& color)
{
    QIM_D(d);
    d->borderColor = color;
}

QColor QImPlotValueTrackerNode::borderColor() const
{
    QIM_DC(d);
    return d->borderColor;
}

void QImPlotValueTrackerNode::setTrackerLineColor(const QColor& color)
{
    QIM_D(d);
    d->trackerLineColor = toImU32(color);
}

QColor QImPlotValueTrackerNode::trackerLineColor() const
{
    QIM_DC(d);
    return d->trackerLineColor;
}

bool QImPlotValueTrackerNode::beginDraw()
{
    QIM_D(d);
    // 安全检查
    if (!d->plotNode || !d->plotNode->isVisible()) {
        d->isActive = false;
        return false;
    }
    // === 1. 采集所有 ImGui/ImPlot 状态（仅在此处调用！）===
    // 屏幕相关参数
    d->mouseScreenPos = ImGui::GetMousePos();                  // 鼠标屏幕坐标
    d->plotScreenPos  = ImPlot::GetPlotPos();                  // Plot左上角屏幕坐标
    d->plotScreenSize = ImPlot::GetPlotSize();                 // Plot屏幕尺寸
    d->plotCanvasRect = ImPlot::GetCurrentPlot()->CanvasRect;  // 画布的尺寸

    d->updateTrackingState();

    // 仅当需要显示时调用渲染（符合 ImGui 声明式原则）
    if (d->isActive) {
        renderTooltip(d->trackedValues, QPointF(d->mouseScreenPos.x, d->mouseScreenPos.y));
    }

    return false;  // 不渲染子节点
}

void QImPlotValueTrackerNode::renderTooltip(const std::vector< TrackedValue >& values, const QPointF& mouseScreenPos)
{
    QIM_D(d);
    if (!d->isActive || values.empty() || !d->plotNode) {
        return;
    }

    ImDrawList* drawList = ImPlot::GetPlotDrawList();
    ImVec2 plotPos       = ImPlot::GetPlotPos();
    ImVec2 plotSize      = ImPlot::GetPlotSize();
    ImVec2 screenSize    = ImGui::GetIO().DisplaySize;

    // Check if any value has an xValueLabel (XY series do, PieChart/BarGroups may not)
    bool showXValue = !values[ 0 ].xValueLabel.empty();

    // === 1. 动态计算tooltip宽度 ===
    float maxWidth = 0;
    if (d->autoWidth) {
        // 计算每个数据行所需宽度（色块 + 标签 + Y值）
        for (const auto& value : values) {
            float labelWidth  = ImGui::CalcTextSize(value.label.c_str()).x;
            float yValueWidth = ImGui::CalcTextSize(value.yValueLabel.c_str()).x;
            float rowWidth    = d->colorBoxSize + 2 * d->tooltipPadding + labelWidth + d->tooltipPadding + yValueWidth;
            maxWidth          = std::max(maxWidth, rowWidth);
        }
        // 计算x值宽度
        if (showXValue) {
            float xLabelWidth = ImGui::CalcTextSize(values[ 0 ].xValueLabel.c_str()).x;
            maxWidth          = std::max(maxWidth, (d->tooltipPadding + xLabelWidth));
        }
    } else {
        maxWidth = d->fixedWidth;
    }

    float tooltipWidth = maxWidth + 2 * d->tooltipPadding;

    // === 2. 计算tooltip高度 ===
    float itemHeight    = d->fontSize + d->tooltipPadding;
    float contentHeight = values.size() * itemHeight;
    if (showXValue) {
        contentHeight += d->fontSize;
    }
    float tooltipHeight = contentHeight + 2 * d->tooltipPadding + 2.0f;

    // === 3. 智能定位tooltip（避免超出屏幕）===
    float tooltipX = mouseScreenPos.x() + kTooltipOffsetPx;
    float tooltipY = mouseScreenPos.y();

    if (tooltipX + tooltipWidth > screenSize.x) {
        tooltipX = (mouseScreenPos.x() > tooltipWidth + kTooltipOffsetPx) ? mouseScreenPos.x() - tooltipWidth - kTooltipOffsetPx : d->tooltipPadding;
    }
    if (tooltipY + tooltipHeight > screenSize.y) {
        tooltipY = screenSize.y - tooltipHeight - d->tooltipPadding;
    }
    if (tooltipY < d->tooltipPadding) {
        tooltipY = d->tooltipPadding;
    }

    // === 4. 绘制背景 ===
    ImVec2 bgMin(tooltipX, tooltipY);
    ImVec2 bgMax(tooltipX + tooltipWidth, tooltipY + tooltipHeight);
    drawList->AddRectFilled(bgMin, bgMax, toImU32(d->bgColor), 3.0f);
    drawList->AddRect(bgMin, bgMax, toImU32(d->borderColor), 3.0f);

    // === 5. 绘制数据行 ===
    float currentY      = tooltipY + d->tooltipPadding;
    float contentStartX = tooltipX + d->tooltipPadding;

    for (const auto& value : values) {
        float colorBoxY = currentY + (itemHeight - d->colorBoxSize) * 0.5f;
        drawList->AddRectFilled(ImVec2(contentStartX, colorBoxY),
                                ImVec2(contentStartX + d->colorBoxSize, colorBoxY + d->colorBoxSize),
                                toImU32(value.color),
                                2.0f);

        float labelX = contentStartX + d->colorBoxSize + d->tooltipPadding;
        drawList->AddText(ImVec2(labelX, currentY - 1.0f), toImU32(d->textColor), value.label.c_str());

        // 绘制Y值（右对齐）
        char yValueText[ 32 ];
        if (std::fabs(value.yValue) > 1000.0 || (std::fabs(value.yValue) < 0.001 && value.yValue != 0.0)) {
            ImFormatString(yValueText, 32, "%.3e", value.yValue);
        } else {
            ImFormatString(yValueText, 32, "%.3f", value.yValue);
        }
        float yValueWidth = ImGui::CalcTextSize(yValueText).x;
        float valueX      = tooltipX + tooltipWidth - d->tooltipPadding - yValueWidth;
        drawList->AddText(ImVec2(valueX, currentY - 1.0f), toImU32(d->textColor), yValueText);

        currentY += itemHeight;
    }

    // === 6. 绘制分隔线 ===
    drawList->AddLine(ImVec2(tooltipX + d->tooltipPadding, currentY),
                      ImVec2(tooltipX + tooltipWidth - d->tooltipPadding, currentY),
                      toImU32(d->borderColor),
                      1.0f);
    currentY += 2.0f;

    // === 7. 绘制X值（底部，仅当有xValueLabel时）===
    if (showXValue) {
        drawList->AddText(ImVec2(contentStartX, currentY - 1.0f), toImU32(d->textColor), values[ 0 ].xValueLabel.c_str());
    }

    // === 8. 绘制垂直跟踪线 ===
    drawList->AddLine(
        ImVec2(mouseScreenPos.x(), plotPos.y), ImVec2(mouseScreenPos.x(), plotPos.y + plotSize.y), d->trackerLineColor, 1.0f);
}

}  // namespace QIM