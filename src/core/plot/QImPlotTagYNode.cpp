#include "QImPlotTagYNode.h"
#include "implot.h"
#include "../QtImGuiUtils.h"

namespace QIM
{

class QImPlotTagYNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlotTagYNode)
public:
    PrivateData(QImPlotTagYNode* p);

    // Position value
    double value { 0.0 };
    
    // Style and configuration
    QColor color { Qt::white };  ///< Tag line color
    QByteArray textUtf8;         ///< Text label (UTF8, used directly by ImGui)
    bool round { false };        ///< Round position to integer pixels
};

QImPlotTagYNode::PrivateData::PrivateData(QImPlotTagYNode* p) : q_ptr(p)
{
}

/**
 * \if ENGLISH
 * @brief Constructor for QImPlotTagYNode
 * @param parent Parent QObject
 * \endif
 *
 * \if CHINESE
 * @brief QImPlotTagYNode 的构造函数
 * @param parent 父 QObject
 * \endif
 */
QImPlotTagYNode::QImPlotTagYNode(QObject* parent)
    : QImPlotItemNode(parent), QIM_PIMPL_CONSTRUCT
{
}

/**
 * \if ENGLISH
 * @brief Destructor for QImPlotTagYNode
 * \endif
 *
 * \if CHINESE
 * @brief QImPlotTagYNode 的析构函数
 * \endif
 */
QImPlotTagYNode::~QImPlotTagYNode()
{
}

/**
 * \if ENGLISH
 * @brief Get tag y position
 * @return Current y position in plot coordinates
 * \endif
 *
 * \if CHINESE
 * @brief 获取标签 y 位置
 * @return 绘图坐标中的当前 y 位置
 * \endif
 */
double QImPlotTagYNode::value() const
{
    return d_ptr->value;
}

/**
 * \if ENGLISH
 * @brief Set tag y position
 * @param value New y position in plot coordinates
 * \endif
 *
 * \if CHINESE
 * @brief 设置标签 y 位置
 * @param value 绘图坐标中的新 y 位置
 * \endif
 */
void QImPlotTagYNode::setValue(double value)
{
    QIM_D(d);
    if (d->value != value) {
        d->value = value;
        emit valueChanged(value);
    }
}

/**
 * \if ENGLISH
 * @brief Get tag text
 * @return Current text label
 * \endif
 *
 * \if CHINESE
 * @brief 获取标签文本
 * @return 当前文本标签
 * \endif
 */
QString QImPlotTagYNode::text() const
{
    return QString::fromUtf8(d_ptr->textUtf8);
}

/**
 * \if ENGLISH
 * @brief Set tag text
 * @param text New text label
 * \endif
 *
 * \if CHINESE
 * @brief 设置标签文本
 * @param text 新文本标签
 * \endif
 */
void QImPlotTagYNode::setText(const QString& text)
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
 * @brief Set tag text with printf-style formatting
 * @param fmt Format string
 * @param ... Format arguments
 * \endif
 *
 * \if CHINESE
 * @brief 使用 printf 风格格式化设置标签文本
 * @param fmt 格式字符串
 * @param ... 格式参数
 * \endif
 */
void QImPlotTagYNode::setText(const char* fmt, ...)
{
    if (!fmt) {
        return;
    }
    
    QIM_D(d);
    va_list args;
    va_start(args, fmt);
    QString text = QString::asprintf(fmt, args);
    va_end(args);
    
    QByteArray utf8 = text.toUtf8();
    if (d->textUtf8 != utf8) {
        d->textUtf8 = utf8;
        emit textChanged(text);
    }
}

/**
 * \if ENGLISH
 * @brief Get tag line color
 * @return Current QColor
 * \endif
 *
 * \if CHINESE
 * @brief 获取标签线颜色
 * @return 当前的 QColor
 * \endif
 */
QColor QImPlotTagYNode::color() const
{
    return d_ptr->color;
}

/**
 * \if ENGLISH
 * @brief Set tag line color
 * @param c QColor for the tag line
 * \endif
 *
 * \if CHINESE
 * @brief 设置标签线颜色
 * @param c 标签线的 QColor
 * \endif
 */
void QImPlotTagYNode::setColor(const QColor& c)
{
    QIM_D(d);
    if (d->color != c) {
        d->color = c;
        emit colorChanged(c);
    }
}

/**
 * \if ENGLISH
 * @brief Get round setting
 * @return true if position is rounded to integer pixels
 * \endif
 *
 * \if CHINESE
 * @brief 获取舍入设置
 * @return 如果位置舍入为整数像素，则为 true
 * \endif
 */
bool QImPlotTagYNode::round() const
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
void QImPlotTagYNode::setRound(bool round)
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
 * @details Calls ImPlot::TagY with appropriate parameters.
 *          If text is empty, uses the bool round version.
 *          If text has content, uses the format version.
 * \endif
 *
 * \if CHINESE
 * @brief 开始绘制实现
 * @return false 以防止调用 endDraw
 * @details 使用适当的参数调用 ImPlot::TagY。
 *          如果文本为空，使用 bool round 版本。
 *          如果文本有内容，使用格式版本。
 * \endif
 */
bool QImPlotTagYNode::beginDraw()
{
    QIM_DC(d);
    
    // Convert color to ImVec4
    ImVec4 colorVec = toImVec4(d->color);
    
    // Call ImPlot TagY API
    if (d->textUtf8.isEmpty()) {
        // Use the bool round version when no text
        ImPlot::TagY(d->value, colorVec, d->round);
    } else {
        // Use UTF8 text directly (stored in setter for performance)
        ImPlot::TagY(d->value, colorVec, "%s", d->textUtf8.constData());
    }
    
    return false; // TagY doesn't need endDraw
}

}  // namespace QIM
