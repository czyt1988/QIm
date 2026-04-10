#include "QImPlotTextItemNode.h"
#include <optional>
#include "QImTrackedValue.hpp"
#include "QtImGuiUtils.h"
#include "implot.h"

namespace QIM
{

class QImPlotTextItemNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlotTextItemNode)
public:
    explicit PrivateData(QImPlotTextItemNode* p) : q_ptr(p)
    {
    }

    QByteArray textUtf8;
    QPointF position { 0.0, 0.0 };
    QPointF pixelOffset { 0.0, 0.0 };
    ImPlotTextFlags flags { ImPlotTextFlags_None };
    std::optional< QImTrackedValue< ImVec4, QIM::ImVecComparator< ImVec4 > > > color;
};

/**
 * \if ENGLISH
 * @brief Constructs a text annotation item node.
 * @param[in] parent Parent QObject.
 * \endif
 *
 * \if CHINESE
 * @brief 构造一个文本标注绘图项节点。
 * @param[in] parent 父QObject对象。
 * \endif
 */
QImPlotTextItemNode::QImPlotTextItemNode(QObject* parent)
    : QImPlotItemNode(parent), QIM_PIMPL_CONSTRUCT
{
}

/**
 * \if ENGLISH
 * @brief Destroys the text annotation item node.
 * \endif
 *
 * \if CHINESE
 * @brief 销毁文本标注绘图项节点。
 * \endif
 */
QImPlotTextItemNode::~QImPlotTextItemNode()
{
}

/**
 * \if ENGLISH
 * @brief Sets the displayed text content.
 * @param[in] text New text content.
 * \endif
 *
 * \if CHINESE
 * @brief 设置显示文本内容。
 * @param[in] text 新的文本内容。
 * \endif
 */
void QImPlotTextItemNode::setText(const QString& text)
{
    QIM_D(d);
    const QByteArray newValue = text.toUtf8();
    if (d->textUtf8 != newValue) {
        d->textUtf8 = newValue;
        Q_EMIT textChanged(text);
    }
}

QString QImPlotTextItemNode::text() const
{
    return QString::fromUtf8(d_ptr->textUtf8);
}

/**
 * \if ENGLISH
 * @brief Sets the anchor position in plot coordinates.
 * @param[in] position New anchor position.
 * \endif
 *
 * \if CHINESE
 * @brief 设置绘图坐标中的锚点位置。
 * @param[in] position 新的锚点位置。
 * \endif
 */
void QImPlotTextItemNode::setPosition(const QPointF& position)
{
    QIM_D(d);
    if (d->position != position) {
        d->position = position;
        Q_EMIT geometryChanged();
    }
}

QPointF QImPlotTextItemNode::position() const
{
    return d_ptr->position;
}

/**
 * \if ENGLISH
 * @brief Sets the pixel offset relative to the anchor.
 * @param[in] offset New pixel offset.
 * \endif
 *
 * \if CHINESE
 * @brief 设置相对于锚点的像素偏移。
 * @param[in] offset 新的像素偏移。
 * \endif
 */
void QImPlotTextItemNode::setPixelOffset(const QPointF& offset)
{
    QIM_D(d);
    if (d->pixelOffset != offset) {
        d->pixelOffset = offset;
        Q_EMIT geometryChanged();
    }
}

QPointF QImPlotTextItemNode::pixelOffset() const
{
    return d_ptr->pixelOffset;
}

bool QImPlotTextItemNode::isVertical() const
{
    return (d_ptr->flags & ImPlotTextFlags_Vertical) != 0;
}

/**
 * \if ENGLISH
 * @brief Enables or disables vertical text rendering.
 * @param[in] on True to render text vertically.
 * \endif
 *
 * \if CHINESE
 * @brief 启用或禁用竖排文本渲染。
 * @param[in] on 为true时按竖排文字渲染。
 * \endif
 */
void QImPlotTextItemNode::setVertical(bool on)
{
    QIM_D(d);
    const ImPlotTextFlags oldFlags = d->flags;
    if (on) {
        d->flags |= ImPlotTextFlags_Vertical;
    } else {
        d->flags &= ~ImPlotTextFlags_Vertical;
    }
    if (oldFlags != d->flags) {
        Q_EMIT textFlagChanged();
    }
}

int QImPlotTextItemNode::textFlags() const
{
    return d_ptr->flags;
}

void QImPlotTextItemNode::setTextFlags(int flags)
{
    QIM_D(d);
    if (d->flags != flags) {
        d->flags = static_cast< ImPlotTextFlags >(flags);
        Q_EMIT textFlagChanged();
    }
}

void QImPlotTextItemNode::setColor(const QColor& color)
{
    d_ptr->color = toImVec4(color);
    Q_EMIT colorChanged(color);
}

QColor QImPlotTextItemNode::color() const
{
    return (d_ptr->color.has_value()) ? toQColor(d_ptr->color->value()) : QColor();
}

/**
 * \if ENGLISH
 * @brief Draws the annotation text through ImPlot::PlotText.
 * @return Always returns false because the item has no child scope to recurse into.
 * \endif
 *
 * \if CHINESE
 * @brief 通过ImPlot::PlotText绘制文本标注。
 * @return 始终返回false，因为该绘图项没有子作用域需要递归渲染。
 * \endif
 */
bool QImPlotTextItemNode::beginDraw()
{
    QIM_D(d);
    if (d->textUtf8.isEmpty()) {
        return false;
    }

    const bool hasColor = d->color.has_value();
    if (hasColor) {
        ImPlot::PushStyleColor(ImPlotCol_InlayText, d->color->value());
    }

    ImPlot::PlotText(d->textUtf8.constData(),
                     d->position.x(),
                     d->position.y(),
                     toImVec2(d->pixelOffset),
                     d->flags);

    if (hasColor) {
        ImPlot::PopStyleColor();
    }
    return false;
}

}  // namespace QIM
