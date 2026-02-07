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

void QImPlotValueTrackerNode::PrivateData::updateSupportSeries(QImPlotNode* plot)
{
    supportSeries.clear();
    const auto items = plot->plotItemNodes();
    for (QImAbstractNode* n : items) {
        tryAddSeries(n);
    }
}

bool QImPlotValueTrackerNode::PrivateData::tryAddSeries(QImAbstractNode* n)
{
    if (QImPlotLineItemNode* line = qobject_cast< QImPlotLineItemNode* >(n)) {
        supportSeries.push_back(line->data());
        return true;
    }
    return false;
}

bool QImPlotValueTrackerNode::PrivateData::tryRemoveSeries(QImAbstractNode* n)
{
    if (QImPlotLineItemNode* line = qobject_cast< QImPlotLineItemNode* >(n)) {
        return (supportSeries.removeAll(line->data()) > 0);
    }
    return false;
}

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
QImPlotValueTrackerNode::QImPlotValueTrackerNode(QImPlotNode* plotNode, QObject* parent)
    : QImAbstractNode(parent), QIM_PIMPL_CONSTRUCT
{
    d_ptr->plotNode = plotNode;
    setObjectName(QStringLiteral("QImPlotValueTrackerNode"));
    setZOrder(99999);  // 确保绘制在最上层
    setAutoIdEnabled(true);

    if (plotNode) {
        connect(plotNode, &QImPlotNode::childNodeAdded, this, &QImPlotValueTrackerNode::onChildNodeAdded);
        connect(plotNode, &QImPlotNode::childNodeRemoved, this, &QImPlotValueTrackerNode::onChildNodeRemoved);
        d_ptr->updateSupportSeries(plotNode);
    }
}

QImPlotValueTrackerNode::~QImPlotValueTrackerNode()
{
    if (d_ptr->group) {
        d_ptr->group->removeTracker(this);
    }
}

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

bool QImPlotValueTrackerNode::hasGroup() const
{
    return (d_ptr->group != nullptr);
}

QImPlotValueTrackerNodeGroup* QIM::QImPlotValueTrackerNode::group() const
{
    return d_ptr->group;
}

void QImPlotValueTrackerNode::onChildNodeAdded(QImAbstractNode* n)
{
    QIM_D(d);
    d->tryAddSeries(n);
}

void QImPlotValueTrackerNode::onChildNodeRemoved(QImAbstractNode* n)
{
    QIM_D(d);
    d->tryRemoveSeries(n);
}

void QImPlotValueTrackerNode::setSkipNanFiniteValues(bool on)
{
    d_ptr->skipNanFiniteValues = on;
}

bool QImPlotValueTrackerNode::isSkipNanFiniteValues() const
{
    return d_ptr->skipNanFiniteValues;
}

void QImPlotValueTrackerNode::setFixedWidth(float width)
{
    d_ptr->fixedWidth = width;
}

float QIM::QImPlotValueTrackerNode::fixedWidth() const
{
    return d_ptr->fixedWidth;
}

void QImPlotValueTrackerNode::setAutoWidthEnabled(bool on)
{
    d_ptr->autoWidth = on;
}

bool QImPlotValueTrackerNode::isAutoWidthEnabled() const
{
    return d_ptr->autoWidth;
}

void QImPlotValueTrackerNode::setTextColor(const QColor& color)
{
    QIM_D(d);
    d->textColor = color;
}

QColor QImPlotValueTrackerNode::textColor() const
{
    return d_ptr->textColor;
}

void QImPlotValueTrackerNode::setBackgroundColor(const QColor& color)
{
    d_ptr->bgColor = color;
}

QColor QImPlotValueTrackerNode::backgroundColor() const
{
    return d_ptr->bgColor;
}

void QImPlotValueTrackerNode::setBorderColor(const QColor& color)
{
    d_ptr->borderColor = color;
}

QColor QImPlotValueTrackerNode::borderColor() const
{
    return d_ptr->borderColor;
}

void QImPlotValueTrackerNode::setTrackerLineColor(const QColor& color)
{
    d_ptr->trackerLineColor = toImU32(color);
}

QColor QImPlotValueTrackerNode::trackerLineColor() const
{
    return d_ptr->trackerLineColor;
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

                // 绘制Y值
        QString valueText = QString::number(value.yValue, 'f', 3);
        float valueWidth  = ImGui::CalcTextSize(valueText.toUtf8().constData()).x;
        float valueX      = tooltipX + groupWidth - d->tooltipPadding - valueWidth;
        drawList->AddText(ImVec2(valueX, itemY - 1), toImU32(d->textColor), valueText.toUtf8().constData());

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
