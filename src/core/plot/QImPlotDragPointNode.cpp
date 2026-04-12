#include "QImPlotDragPointNode.h"
#include <optional>
#include "implot.h"
#include "implot_internal.h"
#include "QImTrackedValue.hpp"
#include "../QtImGuiUtils.h"

namespace QIM
{

class QImPlotDragPointNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlotDragPointNode)
public:
    PrivateData(QImPlotDragPointNode* p);

    // Position values (stored as doubles for ImPlot pointer access)
    double x { 0.0 };
    double y { 0.0 };
    
    // Style and configuration
    std::optional<QImTrackedValue<ImVec4, QIM::ImVecComparator<ImVec4>>> color;  ///< Point color
    float size { 4.0f };                                                         ///< Point size in pixels
    int id { 0 };                                                                ///< Unique identifier
    ImPlotDragToolFlags flags { ImPlotDragToolFlags_None };                      ///< Drag tool flags
    
    // Interaction state (updated after each render)
    bool clicked { false };
    bool hovered { false };
    bool held { false };
    bool wasModified { false };  ///< True if position was modified in last render
};

QImPlotDragPointNode::PrivateData::PrivateData(QImPlotDragPointNode* p) : q_ptr(p)
{
}

/**
 * \if ENGLISH
 * @brief Constructor for QImPlotDragPointNode
 * @param parent Parent QObject
 * \endif
 *
 * \if CHINESE
 * @brief QImPlotDragPointNode的构造函数
 * @param parent 父QObject
 * \endif
 */
QImPlotDragPointNode::QImPlotDragPointNode(QObject* parent)
    : QImPlotItemNode(parent), QIM_PIMPL_CONSTRUCT
{
    // Generate a default ID based on object address
    static int nextId = 1;
    setId(nextId++);
}

/**
 * \if ENGLISH
 * @brief Destructor for QImPlotDragPointNode
 * \endif
 *
 * \if CHINESE
 * @brief QImPlotDragPointNode的析构函数
 * \endif
 */
QImPlotDragPointNode::~QImPlotDragPointNode()
{
}

/**
 * \if ENGLISH
 * @brief Get point position
 * @return Current position in plot coordinates
 * \endif
 *
 * \if CHINESE
 * @brief 获取点位置
 * @return 绘图坐标中的当前位置
 * \endif
 */
QPointF QImPlotDragPointNode::position() const
{
    return QPointF(d_ptr->x, d_ptr->y);
}

/**
 * \if ENGLISH
 * @brief Set point position
 * @param pos New position in plot coordinates
 * \endif
 *
 * \if CHINESE
 * @brief 设置点位置
 * @param pos 绘图坐标中的新位置
 * \endif
 */
void QImPlotDragPointNode::setPosition(const QPointF& pos)
{
    setPosition(pos.x(), pos.y());
}

/**
 * \if ENGLISH
 * @brief Set point position (convenience overload)
 * @param x X coordinate in plot space
 * @param y Y coordinate in plot space
 * \endif
 *
 * \if CHINESE
 * @brief 设置点位置（便捷重载）
 * @param x 绘图空间中的X坐标
 * @param y 绘图空间中的Y坐标
 * \endif
 */
void QImPlotDragPointNode::setPosition(double x, double y)
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
 * @brief Get point color
 * @return Current QColor
 * \endif
 *
 * \if CHINESE
 * @brief 获取点颜色
 * @return 当前的QColor
 * \endif
 */
QColor QImPlotDragPointNode::color() const
{
    return (d_ptr->color.has_value()) ? toQColor(d_ptr->color->value()) : QColor();
}

/**
 * \if ENGLISH
 * @brief Set point color
 * @param c QColor for the point
 * \endif
 *
 * \if CHINESE
 * @brief 设置点颜色
 * @param c 点的QColor
 * \endif
 */
void QImPlotDragPointNode::setColor(const QColor& c)
{
    d_ptr->color = toImVec4(c);
    emit colorChanged(c);
}

/**
 * \if ENGLISH
 * @brief Get point size
 * @return Current size in pixels
 * \endif
 *
 * \if CHINESE
 * @brief 获取点大小
 * @return 当前的像素大小
 * \endif
 */
float QImPlotDragPointNode::size() const
{
    return d_ptr->size;
}

/**
 * \if ENGLISH
 * @brief Set point size
 * @param size New size in pixels
 * \endif
 *
 * \if CHINESE
 * @brief 设置点大小
 * @param size 新的像素大小
 * \endif
 */
void QImPlotDragPointNode::setSize(float size)
{
    if (d_ptr->size != size) {
        d_ptr->size = size;
        emit sizeChanged(size);
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
int QImPlotDragPointNode::id() const
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
void QImPlotDragPointNode::setId(int id)
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
int QImPlotDragPointNode::flags() const
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
void QImPlotDragPointNode::setFlags(int flags)
{
    if (d_ptr->flags != flags) {
        d_ptr->flags = static_cast<ImPlotDragToolFlags>(flags);
        emit flagsChanged(flags);
    }
}

/**
 * \if ENGLISH
 * @brief Check if point was clicked
 * @return true if point was clicked in last render
 * \endif
 *
 * \if CHINESE
 * @brief 检查点是否被点击
 * @return 如果点在最后一次渲染中被点击，则为true
 * \endif
 */
bool QImPlotDragPointNode::clicked() const
{
    return d_ptr->clicked;
}

/**
 * \if ENGLISH
 * @brief Check if point is hovered
 * @return true if point is hovered in last render
 * \endif
 *
 * \if CHINESE
 * @brief 检查点是否被悬停
 * @return 如果点在最后一次渲染中被悬停，则为true
 * \endif
 */
bool QImPlotDragPointNode::hovered() const
{
    return d_ptr->hovered;
}

/**
 * \if ENGLISH
 * @brief Check if point is being held
 * @return true if point is being held in last render
 * \endif
 *
 * \if CHINESE
 * @brief 检查点是否被按住
 * @return 如果点在最后一次渲染中被按住，则为true
 * \endif
 */
bool QImPlotDragPointNode::held() const
{
    return d_ptr->held;
}

/**
 * \if ENGLISH
 * @brief Check if point was modified by user
 * @return true if point position was modified in last render
 * \endif
 *
 * \if CHINESE
 * @brief 检查点是否被用户修改
 * @return 如果点位置在最后一次渲染中被修改，则为true
 * \endif
 */
bool QImPlotDragPointNode::wasModified() const
{
    return d_ptr->wasModified;
}

/**
 * \if ENGLISH
 * @brief Begin drawing implementation
 * @return false to prevent endDraw from being called
 * @details Calls ImPlot::DragPoint with appropriate parameters.
 *          Updates interaction state (clicked/hovered/held) after call.
 *          Emits positionChanged() if position was modified by user.
 * \endif
 *
 * \if CHINESE
 * @brief 开始绘制实现
 * @return false以防止调用endDraw
 * @details 使用适当的参数调用ImPlot::DragPoint。
 *          调用后更新交互状态（clicked/hovered/held）。
 *          如果用户修改了位置，则发出positionChanged()信号。
 * \endif
 */
bool QImPlotDragPointNode::beginDraw()
{
    QIM_D(d);
    
    // Store previous position to detect changes
    const double prevX = d->x;
    const double prevY = d->y;
    
    // Convert color to ImVec4
    ImVec4 colorVec = d->color.has_value() ? d->color->value() : ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
    
    // Call ImPlot DragPoint API
    bool modified = ImPlot::DragPoint(
        d->id,
        &d->x,
        &d->y,
        colorVec,
        d->size,
        d->flags,
        &d->clicked,
        &d->hovered,
        &d->held
    );
    
    // Update modification state
    d->wasModified = modified;
    
    // Emit signals if position changed
    if (modified) {
        if (d->x != prevX || d->y != prevY) {
            emit positionChanged(QPointF(d->x, d->y));
        }
    }
    
    // Emit interaction state signals if changed
    // Note: We could emit signals here, but to avoid excessive signals,
    // we rely on property bindings or manual polling
    
    // Update item status (for consistency with other plot items)
    ImPlotContext* ct = ImPlot::GetCurrentContext();
    if (ct) {
        ImPlotItem* plotItem = ct->PreviousItem;
        setImPlotItem(plotItem);
        if (plotItem && plotItem->Show != QImAbstractNode::isVisible()) {
            QImAbstractNode::setVisible(plotItem->Show);
        }
    }
    
    return false; // DragPoint doesn't need endDraw
}

}  // namespace QIM