#include "QImPlotTextItemNode.h"
#include <optional>
#include "implot.h"
#include "implot_internal.h"
#include "QImTrackedValue.hpp"
#include "../QtImGuiUtils.h"

namespace QIM
{

class QImPlotTextItemNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlotTextItemNode)
public:
    PrivateData(QImPlotTextItemNode* p);

    QString text;                          ///< Text content to display
    ImPlotPoint position { 0.0, 0.0 };     ///< Position in plot coordinates
    ImVec2 pixelOffset { 0.0f, 0.0f };     ///< Pixel offset from position
    ImPlotTextFlags flags { ImPlotTextFlags_None };  ///< Text flags
    // Style tracking values
    std::optional<QImTrackedValue<ImVec4, QIM::ImVecComparator<ImVec4>>> color;  ///< Text color
};

QImPlotTextItemNode::PrivateData::PrivateData(QImPlotTextItemNode* p) : q_ptr(p)
{
}

/**
 * \if ENGLISH
 * @brief Constructor for QImPlotTextItemNode
 * @param parent Parent QObject
 * \endif
 *
 * \if CHINESE
 * @brief QImPlotTextItemNode的构造函数
 * @param parent 父QObject
 * \endif
 */
QImPlotTextItemNode::QImPlotTextItemNode(QObject* parent)
    : QImPlotItemNode(parent), QIM_PIMPL_CONSTRUCT
{
}

/**
 * \if ENGLISH
 * @brief Destructor for QImPlotTextItemNode
 * \endif
 *
 * \if CHINESE
 * @brief QImPlotTextItemNode的析构函数
 * \endif
 */
QImPlotTextItemNode::~QImPlotTextItemNode()
{
}

/**
 * \if ENGLISH
 * @brief Get text content
 * @return Current text string
 * \endif
 *
 * \if CHINESE
 * @brief 获取文本内容
 * @return 当前的文本字符串
 * \endif
 */
QString QImPlotTextItemNode::text() const
{
    return d_ptr->text;
}

/**
 * \if ENGLISH
 * @brief Set text content
 * @param text New text string to display
 * \endif
 *
 * \if CHINESE
 * @brief 设置文本内容
 * @param text 要显示的新文本字符串
 * \endif
 */
void QImPlotTextItemNode::setText(const QString& text)
{
    if (d_ptr->text != text) {
        d_ptr->text = text;
        emit textChanged(text);
    }
}

/**
 * \if ENGLISH
 * @brief Get plot position
 * @return Current position in plot coordinates
 * \endif
 *
 * \if CHINESE
 * @brief 获取绘图位置
 * @return 绘图坐标中的当前位置
 * \endif
 */
QPointF QImPlotTextItemNode::position() const
{
    return QPointF(d_ptr->position.x, d_ptr->position.y);
}

/**
 * \if ENGLISH
 * @brief Set plot position
 * @param pos New position in plot coordinates
 * \endif
 *
 * \if CHINESE
 * @brief 设置绘图位置
 * @param pos 绘图坐标中的新位置
 * \endif
 */
void QImPlotTextItemNode::setPosition(const QPointF& pos)
{
    setPosition(pos.x(), pos.y());
}

/**
 * \if ENGLISH
 * @brief Set plot position (convenience overload)
 * @param x X coordinate in plot space
 * @param y Y coordinate in plot space
 * \endif
 *
 * \if CHINESE
 * @brief 设置绘图位置（便捷重载）
 * @param x 绘图空间中的X坐标
 * @param y 绘图空间中的Y坐标
 * \endif
 */
void QImPlotTextItemNode::setPosition(double x, double y)
{
    if (d_ptr->position.x != x || d_ptr->position.y != y) {
        d_ptr->position.x = x;
        d_ptr->position.y = y;
        emit positionChanged(QPointF(x, y));
    }
}

/**
 * \if ENGLISH
 * @brief Get pixel offset
 * @return Current pixel offset
 * \endif
 *
 * \if CHINESE
 * @brief 获取像素偏移
 * @return 当前的像素偏移
 * \endif
 */
QPointF QImPlotTextItemNode::pixelOffset() const
{
    return QPointF(d_ptr->pixelOffset.x, d_ptr->pixelOffset.y);
}

/**
 * \if ENGLISH
 * @brief Set pixel offset
 * @param offset New pixel offset
 * \endif
 *
 * \if CHINESE
 * @brief 设置像素偏移
 * @param offset 新的像素偏移
 * \endif
 */
void QImPlotTextItemNode::setPixelOffset(const QPointF& offset)
{
    setPixelOffset(static_cast<float>(offset.x()), static_cast<float>(offset.y()));
}

/**
 * \if ENGLISH
 * @brief Set pixel offset (convenience overload)
 * @param dx Horizontal offset in pixels
 * @param dy Vertical offset in pixels
 * \endif
 *
 * \if CHINESE
 * @brief 设置像素偏移（便捷重载）
 * @param dx 水平像素偏移
 * @param dy 垂直像素偏移
 * \endif
 */
void QImPlotTextItemNode::setPixelOffset(float dx, float dy)
{
    if (d_ptr->pixelOffset.x != dx || d_ptr->pixelOffset.y != dy) {
        d_ptr->pixelOffset.x = dx;
        d_ptr->pixelOffset.y = dy;
        emit pixelOffsetChanged(QPointF(dx, dy));
    }
}

/**
 * \if ENGLISH
 * @brief Check if vertical orientation is enabled
 * @return true if text is rendered vertically
 * \endif
 *
 * \if CHINESE
 * @brief 检查是否启用垂直方向
 * @return 如果文本垂直渲染则为true
 * \endif
 */
bool QImPlotTextItemNode::isVertical() const
{
    return d_ptr->flags & ImPlotTextFlags_Vertical;
}

/**
 * \if ENGLISH
 * @brief Set vertical orientation
 * @param vertical true to render text vertically
 * \endif
 *
 * \if CHINESE
 * @brief 设置垂直方向
 * @param vertical 为true时垂直渲染文本
 * \endif
 */
void QImPlotTextItemNode::setVertical(bool vertical)
{
    QIM_D(d);
    if (vertical) {
        d->flags |= ImPlotTextFlags_Vertical;
    } else {
        d->flags &= ~ImPlotTextFlags_Vertical;
    }
    emit verticalChanged(vertical);
    emit textFlagChanged();
}

/**
 * \if ENGLISH
 * @brief Get text color
 * @return Current QColor
 * \endif
 *
 * \if CHINESE
 * @brief 获取文本颜色
 * @return 当前的QColor
 * \endif
 */
QColor QImPlotTextItemNode::color() const
{
    return (d_ptr->color.has_value()) ? toQColor(d_ptr->color->value()) : QColor();
}

/**
 * \if ENGLISH
 * @brief Set text color
 * @param c QColor for the text
 * \endif
 *
 * \if CHINESE
 * @brief 设置文本颜色
 * @param c 文本的QColor
 * \endif
 */
void QImPlotTextItemNode::setColor(const QColor& c)
{
    d_ptr->color = toImVec4(c);
    emit colorChanged(c);
}

/**
 * \if ENGLISH
 * @brief Get raw ImPlotTextFlags
 * @return Current ImPlotTextFlags
 * \endif
 *
 * \if CHINESE
 * @brief 获取原始的ImPlotTextFlags
 * @return 当前的ImPlotTextFlags
 * \endif
 */
int QImPlotTextItemNode::textFlags() const
{
    QIM_DC(d);
    return d->flags;
}

/**
 * \if ENGLISH
 * @brief Set raw ImPlotTextFlags
 * @param flags New ImPlotTextFlags
 * \endif
 *
 * \if CHINESE
 * @brief 设置原始的ImPlotTextFlags
 * @param flags 新的ImPlotTextFlags
 * \endif
 */
void QImPlotTextItemNode::setTextFlags(int flags)
{
    QIM_D(d);
    if (d->flags != flags) {
        d->flags = static_cast<ImPlotTextFlags>(flags);
        emit textFlagChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Begin drawing implementation
 * @return false to prevent endDraw from being called
 * @details Calls ImPlot::PlotText with appropriate parameters.
 *          Returns false if text is empty.
 * \endif
 *
 * \if CHINESE
 * @brief 开始绘制实现
 * @return false以防止调用endDraw
 * @details 使用适当的参数调用ImPlot::PlotText。
 *          如果文本为空则返回false。
 * \endif
 */
bool QImPlotTextItemNode::beginDraw()
{
    QIM_D(d);
    if (d->text.isEmpty()) {
        return false;
    }

    // Track if we pushed style color to ensure matching pop
    bool pushedStyleColor = false;

    // Apply style - push if color is set
    if (d->color) {
        ImPlot::PushStyleColor(ImPlotCol_InlayText, d->color->value());
        pushedStyleColor = true;
    }

    // Call ImPlot API
    ImPlot::PlotText(
        d->text.toUtf8().constData(),
        d->position.x,
        d->position.y,
        d->pixelOffset,
        d->flags);

    // Pop style only if we pushed it
    if (pushedStyleColor) {
        ImPlot::PopStyleColor();
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
