#include "QImPlotDragLineYNode.h"
#include <optional>
#include "implot.h"
#include "implot_internal.h"
#include "QImTrackedValue.hpp"
#include "../QtImGuiUtils.h"

namespace QIM
{

class QImPlotDragLineYNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlotDragLineYNode)
public:
    PrivateData(QImPlotDragLineYNode* p);

    // Position value (stored as double for ImPlot pointer access)
    double y { 0.0 };
    
    // Style and configuration
    std::optional<QImTrackedValue<ImVec4, QIM::ImVecComparator<ImVec4>>> color;  ///< Line color
    float thickness { 1.0f };                                                     ///< Line thickness in pixels
    int id { 0 };                                                                 ///< Unique identifier
    ImPlotDragToolFlags flags { ImPlotDragToolFlags_None };                      ///< Drag tool flags
    
    // Interaction state (updated after each render)
    bool clicked { false };
    bool hovered { false };
    bool held { false };
    bool wasModified { false };  ///< True if position was modified in last render
};

QImPlotDragLineYNode::PrivateData::PrivateData(QImPlotDragLineYNode* p) : q_ptr(p)
{
}

/**
 * \if ENGLISH
 * @brief Constructor for QImPlotDragLineYNode
 * @param parent Parent QObject
 * \endif
 *
 * \if CHINESE
 * @brief QImPlotDragLineYNode的构造函数
 * @param parent 父QObject
 * \endif
 */
QImPlotDragLineYNode::QImPlotDragLineYNode(QObject* parent)
    : QImPlotItemNode(parent), QIM_PIMPL_CONSTRUCT
{
    // Generate a default ID based on object address
    static int nextId = 2000;  // Start from 2000 to avoid conflicts with points and X lines
    setId(nextId++);
}

/**
 * \if ENGLISH
 * @brief Destructor for QImPlotDragLineYNode
 * \endif
 *
 * \if CHINESE
 * @brief QImPlotDragLineYNode的析构函数
 * \endif
 */
QImPlotDragLineYNode::~QImPlotDragLineYNode()
{
}

/**
 * \if ENGLISH
 * @brief Get line y position
 * @return Current y position in plot coordinates
 * \endif
 *
 * \if CHINESE
 * @brief 获取线y位置
 * @return 绘图坐标中的当前y位置
 * \endif
 */
double QImPlotDragLineYNode::value() const
{
    return d_ptr->y;
}

/**
 * \if ENGLISH
 * @brief Set line y position
 * @param value New y position in plot coordinates
 * \endif
 *
 * \if CHINESE
 * @brief 设置线y位置
 * @param value 绘图坐标中的新y位置
 * \endif
 */
void QImPlotDragLineYNode::setValue(double value)
{
    QIM_D(d);
    if (d->y != value) {
        d->y = value;
        emit valueChanged(value);
    }
}

/**
 * \if ENGLISH
 * @brief Get line color
 * @return Current QColor
 * \endif
 *
 * \if CHINESE
 * @brief 获取线颜色
 * @return 当前的QColor
 * \endif
 */
QColor QImPlotDragLineYNode::color() const
{
    return (d_ptr->color.has_value()) ? toQColor(d_ptr->color->value()) : QColor();
}

/**
 * \if ENGLISH
 * @brief Set line color
 * @param c QColor for the line
 * \endif
 *
 * \if CHINESE
 * @brief 设置线颜色
 * @param c 线的QColor
 * \endif
 */
void QImPlotDragLineYNode::setColor(const QColor& c)
{
    d_ptr->color = toImVec4(c);
    emit colorChanged(c);
}

/**
 * \if ENGLISH
 * @brief Get line thickness
 * @return Current thickness in pixels
 * \endif
 *
 * \if CHINESE
 * @brief 获取线厚度
 * @return 当前的像素厚度
 * \endif
 */
float QImPlotDragLineYNode::thickness() const
{
    return d_ptr->thickness;
}

/**
 * \if ENGLISH
 * @brief Set line thickness
 * @param thickness New thickness in pixels
 * \endif
 *
 * \if CHINESE
 * @brief 设置线厚度
 * @param thickness 新的像素厚度
 * \endif
 */
void QImPlotDragLineYNode::setThickness(float thickness)
{
    if (d_ptr->thickness != thickness) {
        d_ptr->thickness = thickness;
        emit thicknessChanged(thickness);
    }
}

/**
 * \if ENGLISH
 * @brief Get drag tool ID
 * @return Current ID
 * \endif
 *
 * \if CHINESE
 * @brief 获取拖拽工具ID
 * @return 当前ID
 * \endif
 */
int QImPlotDragLineYNode::id() const
{
    return d_ptr->id;
}

/**
 * \if ENGLISH
 * @brief Set drag tool ID
 * @param id New ID
 * \endif
 *
 * \if CHINESE
 * @brief 设置拖拽工具ID
 * @param id 新ID
 * \endif
 */
void QImPlotDragLineYNode::setId(int id)
{
    if (d_ptr->id != id) {
        d_ptr->id = id;
        emit idChanged(id);
    }
}

/**
 * \if ENGLISH
 * @brief Get ImPlotDragToolFlags
 * @return Current flags
 * \endif
 *
 * \if CHINESE
 * @brief 获取ImPlotDragToolFlags
 * @return 当前标志
 * \endif
 */
int QImPlotDragLineYNode::flags() const
{
    return d_ptr->flags;
}

/**
 * \if ENGLISH
 * @brief Set ImPlotDragToolFlags
 * @param flags New flags
 * \endif
 *
 * \if CHINESE
 * @brief 设置ImPlotDragToolFlags
 * @param flags 新标志
 * \endif
 */
void QImPlotDragLineYNode::setFlags(int flags)
{
    if (d_ptr->flags != flags) {
        d_ptr->flags = static_cast<ImPlotDragToolFlags>(flags);
        emit flagsChanged(flags);
    }
}

bool QImPlotDragLineYNode::isCursorsEnabled() const
{
    QIM_DC(d);
    return (d->flags & ImPlotDragToolFlags_NoCursors) == 0;
}

void QImPlotDragLineYNode::setCursorsEnabled(bool enabled)
{
    QIM_D(d);
    const ImPlotDragToolFlags oldFlags = d->flags;
    if (enabled) { d->flags &= ~ImPlotDragToolFlags_NoCursors; } else { d->flags |= ImPlotDragToolFlags_NoCursors; }
    if (d->flags != oldFlags) { Q_EMIT dragToolFlagChanged(); }
}

bool QImPlotDragLineYNode::isFitEnabled() const
{
    QIM_DC(d);
    return (d->flags & ImPlotDragToolFlags_NoFit) == 0;
}

void QImPlotDragLineYNode::setFitEnabled(bool enabled)
{
    QIM_D(d);
    const ImPlotDragToolFlags oldFlags = d->flags;
    if (enabled) { d->flags &= ~ImPlotDragToolFlags_NoFit; } else { d->flags |= ImPlotDragToolFlags_NoFit; }
    if (d->flags != oldFlags) { Q_EMIT dragToolFlagChanged(); }
}

bool QImPlotDragLineYNode::isInputsEnabled() const
{
    QIM_DC(d);
    return (d->flags & ImPlotDragToolFlags_NoInputs) == 0;
}

void QImPlotDragLineYNode::setInputsEnabled(bool enabled)
{
    QIM_D(d);
    const ImPlotDragToolFlags oldFlags = d->flags;
    if (enabled) { d->flags &= ~ImPlotDragToolFlags_NoInputs; } else { d->flags |= ImPlotDragToolFlags_NoInputs; }
    if (d->flags != oldFlags) { Q_EMIT dragToolFlagChanged(); }
}

bool QImPlotDragLineYNode::isDelayed() const
{
    QIM_DC(d);
    return (d->flags & ImPlotDragToolFlags_Delayed) != 0;
}

void QImPlotDragLineYNode::setDelayed(bool on)
{
    QIM_D(d);
    const ImPlotDragToolFlags oldFlags = d->flags;
    if (on) { d->flags |= ImPlotDragToolFlags_Delayed; } else { d->flags &= ~ImPlotDragToolFlags_Delayed; }
    if (d->flags != oldFlags) { Q_EMIT dragToolFlagChanged(); }
}

/**
 * \if ENGLISH
 * @brief Check if line was clicked
 * @return true if line was clicked in last render
 * \endif
 *
 * \if CHINESE
 * @brief 检查线是否被点击
 * @return 如果线在最后一次渲染中被点击，则为true
 * \endif
 */
bool QImPlotDragLineYNode::clicked() const
{
    return d_ptr->clicked;
}

/**
 * \if ENGLISH
 * @brief Check if line is hovered
 * @return true if line is hovered in last render
 * \endif
 *
 * \if CHINESE
 * @brief 检查线是否被悬停
 * @return 如果线在最后一次渲染中被悬停，则为true
 * \endif
 */
bool QImPlotDragLineYNode::hovered() const
{
    return d_ptr->hovered;
}

/**
 * \if ENGLISH
 * @brief Check if line is being held
 * @return true if line is being held in last render
 * \endif
 *
 * \if CHINESE
 * @brief 检查线是否被按住
 * @return 如果线在最后一次渲染中被按住，则为true
 * \endif
 */
bool QImPlotDragLineYNode::held() const
{
    return d_ptr->held;
}

/**
 * \if ENGLISH
 * @brief Check if line was modified by user
 * @return true if line position was modified in last render
 * \endif
 *
 * \if CHINESE
 * @brief 检查线是否被用户修改
 * @return 如果线位置在最后一次渲染中被修改，则为true
 * \endif
 */
bool QImPlotDragLineYNode::wasModified() const
{
    return d_ptr->wasModified;
}

/**
 * \if ENGLISH
 * @brief Begin drawing implementation
 * @return false to prevent endDraw from being called
 * @details Calls ImPlot::DragLineY with appropriate parameters.
 *          Updates interaction state (clicked/hovered/held) after call.
 *          Emits valueChanged() if position was modified by user.
 * \endif
 *
 * \if CHINESE
 * @brief 开始绘制实现
 * @return false以防止调用endDraw
 * @details 使用适当的参数调用ImPlot::DragLineY。
 *          调用后更新交互状态（clicked/hovered/held）。
 *          如果用户修改了位置，则发出valueChanged()信号。
 * \endif
 */
bool QImPlotDragLineYNode::beginDraw()
{
    QIM_D(d);
    
    // Store previous position to detect changes
    const double prevY = d->y;
    
    // Convert color to ImVec4
    ImVec4 colorVec = d->color.has_value() ? d->color->value() : ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
    
    // Call ImPlot DragLineY API
    bool modified = ImPlot::DragLineY(
        d->id,
        &d->y,
        colorVec,
        d->thickness,
        d->flags,
        &d->clicked,
        &d->hovered,
        &d->held
    );
    
    // Update modification state
    d->wasModified = modified;
    
    // Emit signals if position changed
    if (modified) {
        if (d->y != prevY) {
            emit valueChanged(d->y);
        }
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
    
    return false; // DragLineY doesn't need endDraw
}

}  // namespace QIM