#include "QImPlotValueTrackerNode.h"
#include <algorithm>
#include <cmath>
#include <limits>
#include <QtGlobal>
#include <QPainter>
#include <QStyleOption>
#include <QDebug>
#include "implot.h"
#include "implot_internal.h"
#include "imgui.h"
#include "QImPlotNode.h"
#include "QImPlotItemNode.h"
#include "QImPlotLineItemNode.h"
#include "QImPlotDataSeries.h"
#include "QImPlotValueTrackerNodeGroup.h"
#include "QtImGuiUtils.h"
#include "QImPlotAxisInfo.h"
namespace QIM
{

class QImPlotValueTrackerNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlotValueTrackerNode)
public:
    PrivateData(QImPlotValueTrackerNode* p);
    void updateSupportSeries(QImPlotNode* plot);
    bool tryAddSeries(QImAbstractNode* n);
    bool tryRemoveSeries(QImAbstractNode* n);

    std::vector< TrackedValue > trackedValues;
    QImPlotNode* plotNode { nullptr };                ///< 绘图节点
    QList< QImAbstractXYDataSeries* > supportSeries;  ///< 记录支持的序列
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
};

QImPlotValueTrackerNode::PrivateData::PrivateData(QImPlotValueTrackerNode* p) : q_ptr(p)
{
}

/**
 * \if ENGLISH
 * @brief Clears and rebuilds the support series list from plot item nodes
 * @param[in] plot The plot node whose item nodes to scan
 * @details Iterates all child plot item nodes in the plot, extracting their
 *          data series via tryAddSeries().
 * \endif
 *
 * \if CHINESE
 * @brief 清除并重建支持序列列表
 * @param[in] plot 要扫描的绘图节点
 * @details 遍历绘图中的所有子绘图项目节点，通过tryAddSeries()提取其数据序列。
 * \endif
 */
void QImPlotValueTrackerNode::PrivateData::updateSupportSeries(QImPlotNode* plot)
{
    supportSeries.clear();
    const auto items = plot->plotItemNodes();
    for (QImAbstractNode* n : items) {
        tryAddSeries(n);
    }
}

/**
 * \if ENGLISH
 * @brief Attempts to add a data series from a child node
 * @param[in] n The node to extract a data series from
 * @return true if a series was successfully added, false otherwise
 * @details Checks if the node is a QImPlotLineItemNode and, if so, adds its
 *          data series to the support series list.
 * \endif
 *
 * \if CHINESE
 * @brief 尝试添加子节点的数据序列
 * @param[in] n 要提取数据序列的节点
 * @return 成功添加序列返回true，否则返回false
 * @details 检查节点是否为QImPlotLineItemNode，若是则将其数据序列添加到支持序列列表。
 * \endif
 */
bool QImPlotValueTrackerNode::PrivateData::tryAddSeries(QImAbstractNode* n)
{
    if (QImPlotLineItemNode* line = qobject_cast< QImPlotLineItemNode* >(n)) {
        supportSeries.push_back(line->data());
        return true;
    }
    return false;
}

/**
 * \if ENGLISH
 * @brief Attempts to remove a data series associated with a child node
 * @param[in] n The node whose data series to remove
 * @return true if a series was successfully removed, false otherwise
 * @details Checks if the node is a QImPlotLineItemNode and, if so, removes
 *          its data series from the support series list.
 * \endif
 *
 * \if CHINESE
 * @brief 尝试移除子节点关联的数据序列
 * @param[in] n 要移除数据序列的节点
 * @return 成功移除序列返回true，否则返回false
 * @details 检查节点是否为QImPlotLineItemNode，若是则从支持序列列表中移除其数据序列。
 * \endif
 */
bool QImPlotValueTrackerNode::PrivateData::tryRemoveSeries(QImAbstractNode* n)
{
    if (QImPlotLineItemNode* line = qobject_cast< QImPlotLineItemNode* >(n)) {
        return (supportSeries.removeAll(line->data()) > 0);
    }
    return false;
}

/**
 * \if ENGLISH
 * @brief Updates the tracking state and computes tracked values at the mouse position
 * @details Core state machine that determines whether the tracker is active based on
 *          mouse hover state, handles group synchronization for multi-plot tracking,
 *          and computes Y values for all visible line items at the current mouse X position.
 *          Updates tracked values only when the mouse has moved significantly.
 * \endif
 *
 * \if CHINESE
 * @brief 更新追踪状态并计算鼠标位置处的追踪值
 * @details 核心状态机，根据鼠标悬停状态确定追踪器是否激活，处理多子图联动的组同步，
 *          并计算当前鼠标X位置处所有可见折线项目的Y值。仅在鼠标发生显著移动时更新追踪值。
 * \endif
 */
void QImPlotValueTrackerNode::PrivateData::updateTrackingState()
{
    // bool mouseInPlot = (plotScreenSize.x > 0 && plotScreenSize.y > 0)
    //                    && (mouseScreenPos.x >= plotScreenPos.x && mouseScreenPos.x <= plotScreenPos.x + plotScreenSize.x
    //                        && mouseScreenPos.y >= plotScreenPos.y
    //                        && mouseScreenPos.y <= plotScreenPos.y + plotScreenSize.y);
    // qDebug() << "isMouseInCanvas=" << isMouseOnPlot << "mouseScreenPos=" << mouseScreenPos
    //          << ",plotScreenPos=" << plotScreenPos << ",plotScreenSize=" << plotScreenSize
    //          << ",plotCanvasRect=" << plotCanvasRect;
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
            // TODO:目前只有x有用
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

        // 优化：提前计算plot坐标，避免在循环内重复转换

        // 遍历所有可见且启用的序列
        for (QImAbstractNode* itemNode : plotNode->plotItemNodes()) {
            if (!itemNode->isVisible() || !itemNode->isEnabled()) {
                continue;
            }

            if (QImPlotLineItemNode* lineItem = qobject_cast< QImPlotLineItemNode* >(itemNode)) {
                QImAbstractXYDataSeries* series = lineItem->data();
                const int size                  = series->size();
                if (!series || size <= 0) {
                    continue;
                }
                QPointF plotPos = lineItem->pixelsToPlot(mouseScreenPos.x, mouseScreenPos.y);
                // 检查X值是否在数据范围内（优化：避免无效查询）

                double firstX = series->xValue(0);
                double lastX  = series->xValue(size - 1);
                if (plotPos.x() < firstX || plotPos.x() > lastX) {
                    continue;  // X值超出数据范围，跳过
                }

                // 获取Y值
                double yVal = series->yValueAtX(plotPos.x());

                // 严格检查Y值有效性（跳过NaN/Inf/超出范围值）
                if (skipNanFiniteValues) {
                    if (std::isnan(yVal) || std::isinf(yVal) || !std::isfinite(yVal)) {
                        continue;
                    }
                }

                // 获取曲线颜色
                QColor lineColor = lineItem->color();
                if (!lineColor.isValid()) {
                    ImVec4 lastItemColor = ImPlot::GetLastItemColor();
                    lineColor            = toQColor(lastItemColor);
                }

                TrackedValue trackedValue;
                trackedValue.label       = lineItem->labelConstData();
                trackedValue.color       = lineColor;
                trackedValue.xValue      = plotPos.x();  // 所有序列共享同一个X值
                trackedValue.yValue      = yVal;
                trackedValue.xValueLabel = plotNode->axisValueText(plotPos.x(), lineItem->xAxisId());
                trackedValue.yValueLabel = plotNode->axisValueText(yVal, lineItem->yAxisId());
                trackedValues.emplace_back(trackedValue);
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
    setZOrder(99999);  // 确保绘制在最上层
    setAutoIdEnabled(true);

    if (plotNode) {
        connect(plotNode, &QImPlotNode::childNodeAdded, this, &QImPlotValueTrackerNode::onChildNodeAdded);
        connect(plotNode, &QImPlotNode::childNodeRemoved, this, &QImPlotValueTrackerNode::onChildNodeRemoved);
        d->updateSupportSeries(plotNode);
    }
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
 * @brief Slot invoked when a child node is added to the plot
 * @param[in] n The newly added child node
 * @details Attempts to register the node's data series via tryAddSeries().
 * \endif
 *
 * \if CHINESE
 * @brief 子节点添加到绘图时触发的槽函数
 * @param[in] n 新添加的子节点
 * @details 通过tryAddSeries()尝试注册该节点的数据序列。
 * \endif
 */
void QImPlotValueTrackerNode::onChildNodeAdded(QImAbstractNode* n)
{
    QIM_D(d);
    d->tryAddSeries(n);
}

/**
 * \if ENGLISH
 * @brief Slot invoked when a child node is removed from the plot
 * @param[in] n The removed child node
 * @details Attempts to unregister the node's data series via tryRemoveSeries().
 * \endif
 *
 * \if CHINESE
 * @brief 子节点从绘图移除时触发的槽函数
 * @param[in] n 被移除的子节点
 * @details 通过tryRemoveSeries()尝试注销该节点的数据序列。
 * \endif
 */
void QImPlotValueTrackerNode::onChildNodeRemoved(QImAbstractNode* n)
{
    QIM_D(d);
    d->tryRemoveSeries(n);
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
        renderTooltip(d->trackedValues, QPointF(d->mouseScreenPos.x, d->mouseScreenPos.y));  // 移除 isHover 参数
    }

    return false;  // 不渲染子节点
}

void QImPlotValueTrackerNode::renderTooltip(const std::vector< TrackedValue >& values, const QPointF& mouseScreenPos)
{
#if 0
    QIM_D(d);

    if (!d->isActive || !d->plotNode) {
        return;
    }

    ImDrawList* drawList = ImPlot::GetPlotDrawList();

            // 如果没有数据，不显示tooltip
    if (values.empty()) {
        return;
    }

            // 准备绘制tooltip
    ImVec2 plotPos    = ImPlot::GetPlotPos();
    ImVec2 plotSize   = ImPlot::GetPlotSize();
    ImVec2 screenSize = ImGui::GetIO().DisplaySize;

    // 计算tooltip位置，避免超出屏幕
    float tooltipX = mouseScreenPos.x() + 10;
    float tooltipY = mouseScreenPos.y();

            // 检查是否超出右侧边界
    if (tooltipX + d->fixedWidth > screenSize.x) {
        tooltipX = (mouseScreenPos.x() > d->fixedWidth + 10) ? mouseScreenPos.x() - d->fixedWidth - 10 : 10;
    }

            // 按Y轴分组绘制
    float currentY = tooltipY;
    // 计算此Y轴组的tooltip尺寸
    float groupWidth = d->fixedWidth;
    float itemHeight = d->fontSize + d->tooltipPadding;
    float groupHeight = d->tooltipPadding * 2 + values.size() * itemHeight + d->fontSize  // 这里的fontSize是给x轴数据的
                        + 2;
    // 检查是否超出屏幕底部
    if (currentY + groupHeight > screenSize.y) {
        currentY = screenSize.y - groupHeight - 2;
    }
    // 检查是否超出屏幕顶部
    if (currentY < 0) {
        currentY = 2;
    }
    // 绘制背景
    ImVec2 bgMin(tooltipX, currentY);
    ImVec2 bgMax(tooltipX + groupWidth, currentY + groupHeight);
    drawList->AddRectFilled(bgMin, bgMax, toImU32(d->bgColor), 3.0f);
    drawList->AddRect(bgMin, bgMax, toImU32(d->borderColor), 3.0f);
    float itemY = currentY + d->tooltipPadding;
    for (auto yAxisIt = values.begin(); yAxisIt != values.end(); ++yAxisIt) {
        const TrackedValue& value = *yAxisIt;
        float textX               = tooltipX + d->tooltipPadding;
        // 绘制色块//这里应该垂直居中
        ImVec2 colorBoxMin(textX, itemY);
        ImVec2 colorBoxMax(textX + d->colorBoxSize, itemY + d->colorBoxSize);
        drawList->AddRectFilled(colorBoxMin, colorBoxMax, toImU32(value.color), 2.0f);
        // 绘制标签文本
        float labelX = textX + d->colorBoxSize + d->tooltipPadding;
        drawList->AddText(ImVec2(labelX, itemY - 1), toImU32(d->textColor), value.label);

                // 绘制Y值（使用ImFormatString避免QString转换）
        char yValueText[32];
        ImFormatString(yValueText, sizeof(yValueText), "%.3f", value.yValue);
        float valueWidth  = ImGui::CalcTextSize(yValueText).x;
        float valueX      = tooltipX + groupWidth - d->tooltipPadding - valueWidth;
        drawList->AddText(ImVec2(valueX, itemY - 1), toImU32(d->textColor), yValueText);

        itemY += itemHeight;
    }
    // 绘制分隔线
    drawList->AddLine(ImVec2(tooltipX + d->tooltipPadding, itemY),
                      ImVec2(tooltipX + groupWidth - d->tooltipPadding, itemY),
                      toImU32(d->borderColor),
                      1.0f);
    // 显示x值
    //  绘制垂直线
    ImVec2 lineStart(mouseScreenPos.x(), plotPos.y);
    ImVec2 lineEnd(mouseScreenPos.x(), plotPos.y + plotSize.y);
    drawList->AddLine(lineStart, lineEnd, d->trackerLineColor, 1.0f);
#else
    QIM_D(d);
    if (!d->isActive || values.empty() || !d->plotNode) {
        return;
    }

    ImDrawList* drawList = ImPlot::GetPlotDrawList();
    ImVec2 plotPos       = ImPlot::GetPlotPos();
    ImVec2 plotSize      = ImPlot::GetPlotSize();
    ImVec2 screenSize    = ImGui::GetIO().DisplaySize;

    // === 1. 动态计算tooltip宽度 ===
    bool showXValue    = d->plotNode->x1Axis()->isLabelEnabled();
    float xLabelHeight = -1.0f;
    float maxWidth     = 0;
    if (d->autoWidth) {
        // 计算每个数据行所需宽度（色块 + 标签 + Y值）
        for (const auto& value : values) {
            // 标签宽度
            float labelWidth  = ImGui::CalcTextSize(value.label).x;
            float yValueWidth = ImGui::CalcTextSize(value.yValueLabel.c_str()).x;

            // 行总宽度 = 色块 + 间距 + 标签 + 间距 + Y值
            float rowWidth = d->colorBoxSize + 2 * d->tooltipPadding + labelWidth + d->tooltipPadding + yValueWidth;
            maxWidth       = std::max(maxWidth, rowWidth);
        }
        // 计算x值宽度
        if (showXValue) {
            auto xfontSize = ImGui::CalcTextSize(values[ 0 ].xValueLabel.c_str());
            xLabelHeight   = xfontSize.y;
            maxWidth       = std::max(maxWidth, (d->tooltipPadding + xfontSize.x));
        }
    } else {
        maxWidth = d->fixedWidth;
    }

    // 限制最大宽度
    float tooltipWidth = maxWidth + 2 * d->tooltipPadding;  // 总宽度 = 内容宽度 + 两侧padding

    // === 2. 计算tooltip高度 ===
    float itemHeight    = d->fontSize + d->tooltipPadding;
    float contentHeight = values.size() * itemHeight;  // 数据行
    // 递增x值行高，x值的文字可能会换行，不能共用y的高度
    if (showXValue) {
        if (d->autoWidth) {
            contentHeight += xLabelHeight;
        } else {
            contentHeight += ImGui::CalcTextSize(values[ 0 ].xValueLabel.c_str()).y;
        }
    }
    float tooltipHeight = contentHeight + 2 * d->tooltipPadding + 2.0f;  // + 分隔线高度

    // === 3. 智能定位tooltip（避免超出屏幕）===
    float tooltipX = mouseScreenPos.x() + 10;
    float tooltipY = mouseScreenPos.y();

    // 检查右侧边界
    if (tooltipX + tooltipWidth > screenSize.x) {
        tooltipX = (mouseScreenPos.x() > tooltipWidth + 10) ? mouseScreenPos.x() - tooltipWidth - 10 : d->tooltipPadding;
    }

    // 检查底部边界
    if (tooltipY + tooltipHeight > screenSize.y) {
        tooltipY = screenSize.y - tooltipHeight - d->tooltipPadding;
    }

    // 检查顶部边界
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
        // 绘制色块（垂直居中）
        float colorBoxY = currentY + (itemHeight - d->colorBoxSize) * 0.5f;
        drawList->AddRectFilled(ImVec2(contentStartX, colorBoxY),
                                ImVec2(contentStartX + d->colorBoxSize, colorBoxY + d->colorBoxSize),
                                toImU32(value.color),
                                2.0f);

        // 绘制标签
        float labelX = contentStartX + d->colorBoxSize + d->tooltipPadding;
        drawList->AddText(ImVec2(labelX, currentY - 1.0f), toImU32(d->textColor), value.label);

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
    currentY += 2.0f;  // 分隔线高度

    // === 7. 绘制X值（底部）===

    if (showXValue) {
        // X值（紧随标签后）
        drawList->AddText(ImVec2(contentStartX, currentY - 1.0f), toImU32(d->textColor), values[ 0 ].xValueLabel.c_str());
    }

    // === 8. 绘制垂直跟踪线 ===
    drawList->AddLine(
        ImVec2(mouseScreenPos.x(), plotPos.y), ImVec2(mouseScreenPos.x(), plotPos.y + plotSize.y), d->trackerLineColor, 1.0f);
#endif
}

}  // namespace QIM
