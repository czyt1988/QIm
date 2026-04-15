#include "QImPlotAnnotationNode.h"
#include <optional>
#include <cstdarg>
#include "implot.h"
#include "implot_internal.h"
#include "QImTrackedValue.hpp"
#include "../QtImGuiUtils.h"

namespace QIM
{

class QImPlotAnnotationNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlotAnnotationNode)
public:
    PrivateData(QImPlotAnnotationNode* p);

    // Position values
    double x { 0.0 };
    double y { 0.0 };

    // Style and configuration
    std::optional< QImTrackedValue< ImVec4, QIM::ImVecComparator< ImVec4 > > > color;  ///< Text color
    QByteArray textUtf8;                                                               ///< Text content (UTF8, used directly by ImGui)
    ImVec2 pixelOffset { 0.0f, 0.0f };                                                 ///< Pixel offset
    bool clamp { false };                                                              ///< Clamp to plot area
    bool round { false };                                                              ///< Round to integer pixels
};

QImPlotAnnotationNode::PrivateData::PrivateData(QImPlotAnnotationNode* p) : q_ptr(p)
{
}

/**
 * \if ENGLISH
 * @brief Constructor for QImPlotAnnotationNode
 * @param parent Parent QObject
 * \endif
 *
 * \if CHINESE
 * @brief QImPlotAnnotationNode的构造函数
 * @param parent 父QObject
 * \endif
 */
QImPlotAnnotationNode::QImPlotAnnotationNode(QObject* parent) : QImPlotItemNode(parent), QIM_PIMPL_CONSTRUCT
{
    // Register with default ID "2d_tools_annotation"
    setObjectName("2d_tools_annotation");
}

/**
 * \if ENGLISH
 * @brief Destructor for QImPlotAnnotationNode
 * \endif
 *
 * \if CHINESE
 * @brief QImPlotAnnotationNode的析构函数
 * \endif
 */
QImPlotAnnotationNode::~QImPlotAnnotationNode()
{
}

/**
 * \if ENGLISH
 * @brief Get annotation position
 * @return Current position in plot coordinates
 * \endif
 *
 * \if CHINESE
 * @brief 获取注释位置
 * @return 绘图坐标中的当前位置
 * \endif
 */
QPointF QImPlotAnnotationNode::position() const
{
    return QPointF(d_ptr->x, d_ptr->y);
}

/**
 * \if ENGLISH
 * @brief Set annotation position
 * @param pos New position in plot coordinates
 * \endif
 *
 * \if CHINESE
 * @brief 设置注释位置
 * @param pos 绘图坐标中的新位置
 * \endif
 */
void QImPlotAnnotationNode::setPosition(const QPointF& pos)
{
    setPosition(pos.x(), pos.y());
}

/**
 * \if ENGLISH
 * @brief Set annotation position (convenience overload)
 * @param x X coordinate in plot space
 * @param y Y coordinate in plot space
 * \endif
 *
 * \if CHINESE
 * @brief 设置注释位置（便捷重载）
 * @param x 绘图空间中的X坐标
 * @param y 绘图空间中的Y坐标
 * \endif
 */
void QImPlotAnnotationNode::setPosition(double x, double y)
{
    QIM_D(d);
    if (d->x != x || d->y != y) {
        d->x = x;
        d->y = y;
        emit positionChanged(QPointF(x, y));
    }
}

/**
 * \if ENGLISH
 * @brief Get annotation text
 * @return Current text
 * \endif
 *
 * \if CHINESE
 * @brief 获取注释文本
 * @return 当前文本
 * \endif
 */
QString QImPlotAnnotationNode::text() const
{
    return QString::fromUtf8(d_ptr->textUtf8);
}

/**
 * \if ENGLISH
 * @brief Set annotation text
 * @param text New text
 * \endif
 *
 * \if CHINESE
 * @brief 设置注释文本
 * @param text 新文本
 * \endif
 */
void QImPlotAnnotationNode::setText(const QString& text)
{
    QIM_D(d);
    QByteArray utf8 = text.toUtf8();
    if (d->textUtf8 != utf8) {
        d->textUtf8 = utf8;
        emit textChanged(text);
    }
}

/**
 * \if ENGLISH
 * @brief Set annotation text with printf-style formatting
 * @param fmt Format string (printf-style)
 * @param ... Format arguments
 * \endif
 *
 * \if CHINESE
 * @brief 使用printf风格格式化设置注释文本
 * @param fmt 格式字符串（printf风格）
 * @param ... 格式参数
 * \endif
 */
void QImPlotAnnotationNode::setText(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char buffer[ 1024 ];
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    setText(QString::fromUtf8(buffer));
}

/**
 * \if ENGLISH
 * @brief Get annotation text color
 * @return Current QColor
 * \endif
 *
 * \if CHINESE
 * @brief 获取注释文本颜色
 * @return 当前的QColor
 * \endif
 */
QColor QImPlotAnnotationNode::color() const
{
    return (d_ptr->color.has_value()) ? toQColor(d_ptr->color->value()) : QColor();
}

/**
 * \if ENGLISH
 * @brief Set annotation text color
 * @param c QColor for the annotation text
 * \endif
 *
 * \if CHINESE
 * @brief 设置注释文本颜色
 * @param c 注释文本的QColor
 * \endif
 */
void QImPlotAnnotationNode::setColor(const QColor& c)
{
    d_ptr->color = toImVec4(c);
    emit colorChanged(c);
}

/**
 * \if ENGLISH
 * @brief Get pixel offset
 * @return Current pixel offset
 * \endif
 *
 * \if CHINESE
 * @brief 获取像素偏移
 * @return 当前像素偏移
 * \endif
 */
QPointF QImPlotAnnotationNode::pixelOffset() const
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
 * @param offset 新像素偏移
 * \endif
 */
void QImPlotAnnotationNode::setPixelOffset(const QPointF& offset)
{
    setPixelOffset(offset.x(), offset.y());
}

/**
 * \if ENGLISH
 * @brief Set pixel offset (convenience overload)
 * @param x X offset in pixels
 * @param y Y offset in pixels
 * \endif
 *
 * \if CHINESE
 * @brief 设置像素偏移（便捷重载）
 * @param x X偏移（像素）
 * @param y Y偏移（像素）
 * \endif
 */
void QImPlotAnnotationNode::setPixelOffset(double x, double y)
{
    QIM_D(d);
    if (d->pixelOffset.x != x || d->pixelOffset.y != y) {
        d->pixelOffset = ImVec2(static_cast< float >(x), static_cast< float >(y));
        emit pixelOffsetChanged(QPointF(x, y));
    }
}

/**
 * \if ENGLISH
 * @brief Get clamp setting
 * @return Current clamp setting
 * \endif
 *
 * \if CHINESE
 * @brief 获取钳位设置
 * @return 当前钳位设置
 * \endif
 */
bool QImPlotAnnotationNode::clamp() const
{
    return d_ptr->clamp;
}

/**
 * \if ENGLISH
 * @brief Set clamp setting
 * @param clamp New clamp setting
 * \endif
 *
 * \if CHINESE
 * @brief 设置钳位设置
 * @param clamp 新钳位设置
 * \endif
 */
void QImPlotAnnotationNode::setClamp(bool clamp)
{
    QIM_D(d);
    if (d->clamp != clamp) {
        d->clamp = clamp;
        emit clampChanged(clamp);
    }
}

/**
 * \if ENGLISH
 * @brief Get round setting
 * @return Current round setting
 * \endif
 *
 * \if CHINESE
 * @brief 获取舍入设置
 * @return 当前舍入设置
 * \endif
 */
bool QImPlotAnnotationNode::round() const
{
    return d_ptr->round;
}

/**
 * \if ENGLISH
 * @brief Set round setting
 * @param round New round setting
 * \endif
 *
 * \if CHINESE
 * @brief 设置舍入设置
 * @param round 新舍入设置
 * \endif
 */
void QImPlotAnnotationNode::setRound(bool round)
{
    QIM_D(d);
    if (d->round != round) {
        d->round = round;
        emit roundChanged(round);
    }
}

/**
 * \if ENGLISH
 * @brief Begin drawing implementation
 * @return false to prevent endDraw from being called
 * @details Calls ImPlot::Annotation with appropriate parameters.
 *          Annotation is a static element that doesn't need endDraw.
 * \endif
 *
 * \if CHINESE
 * @brief 开始绘制实现
 * @return false以防止调用endDraw
 * @details 使用适当的参数调用ImPlot::Annotation。
 *          注释是静态元素，不需要endDraw。
 * \endif
 */
bool QImPlotAnnotationNode::beginDraw()
{
    QIM_D(d);

    if (!isVisible()) {
        return false;
    }

    // Convert color to ImVec4
    ImVec4 colorVec = d->color.has_value() ? d->color->value() : ImVec4(1.0f, 1.0f, 1.0f, 1.0f);


    // Call ImPlot Annotation API
    if (d->textUtf8.isEmpty()) {
        ImPlot::Annotation(d->x, d->y, colorVec, d->pixelOffset, d->clamp, d->round);
    } else {
        // Use UTF8 text directly (stored in setter for performance)
        ImPlot::Annotation(d->x, d->y, colorVec, d->pixelOffset, d->clamp, "%s", d->textUtf8.constData());
    }

    // Update item status (for consistency with other plot items)
    ImPlotContext* ct = ImPlot::GetCurrentContext();
    if (ct) {
        ImPlotItem* plotItem = ct->PreviousItem;
        setImPlotItem(plotItem);
        if (plotItem && plotItem->Show != QImAbstractNode::isVisible()) {
            QImAbstractNode::setVisible(plotItem->Show);
        }
    }

    return false;  // Annotation doesn't need endDraw
}

}  // namespace QIM
