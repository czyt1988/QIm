#include "QImPlotDragRectNode.h"
#include <optional>
#include "implot.h"
#include "implot_internal.h"
#include "QImTrackedValue.hpp"
#include "../QtImGuiUtils.h"

namespace QIM
{

class QImPlotDragRectNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlotDragRectNode)
public:
    PrivateData(QImPlotDragRectNode* p);

    // Rectangle coordinates (stored as doubles for ImPlot pointer access)
    double x1 { 0.0 };
    double y1 { 0.0 };
    double x2 { 1.0 };
    double y2 { 1.0 };
    
    // Style and configuration
    std::optional<QImTrackedValue<ImVec4, QIM::ImVecComparator<ImVec4>>> color;  ///< Rectangle border color
    int id { 0 };                                                                 ///< Unique identifier
    ImPlotDragToolFlags flags { ImPlotDragToolFlags_None };                      ///< Drag tool flags
    
    // Interaction state (updated after each render)
    bool clicked { false };
    bool hovered { false };
    bool held { false };
    bool wasModified { false };  ///< True if position was modified in last render
};

QImPlotDragRectNode::PrivateData::PrivateData(QImPlotDragRectNode* p) : q_ptr(p)
{
}

/**
 * \if ENGLISH
 * @brief Constructor for QImPlotDragRectNode
 * @param parent Parent QObject
 * \endif
 *
 * \if CHINESE
 * @brief QImPlotDragRectNode的构造函数
 * @param parent 父QObject
 * \endif
 */
QImPlotDragRectNode::QImPlotDragRectNode(QObject* parent)
    : QImPlotItemNode(parent), QIM_PIMPL_CONSTRUCT
{
    // Generate a default ID based on object address
    static int nextId = 3000;  // Start from 3000 to avoid conflicts with points and lines
    setId(nextId++);
    // Register with default ID "2d_tools_dragrect"
    setObjectName("2d_tools_dragrect");
}

/**
 * \if ENGLISH
 * @brief Destructor for QImPlotDragRectNode
 * \endif
 *
 * \if CHINESE
 * @brief QImPlotDragRectNode的析构函数
 * \endif
 */
QImPlotDragRectNode::~QImPlotDragRectNode()
{
}

/**
 * \if ENGLISH
 * @brief Get rectangle coordinates
 * @return Current rectangle in plot coordinates
 * \endif
 *
 * \if CHINESE
 * @brief 获取矩形坐标
 * @return 绘图坐标中的当前矩形
 * \endif
 */
QRectF QImPlotDragRectNode::rect() const
{
    return QRectF(d_ptr->x1, d_ptr->y1, d_ptr->x2 - d_ptr->x1, d_ptr->y2 - d_ptr->y1);
}

/**
 * \if ENGLISH
 * @brief Set rectangle coordinates
 * @param rect New rectangle in plot coordinates
 * \endif
 *
 * \if CHINESe
 * @brief 设置矩形坐标
 * @param rect 绘图坐标中的新矩形
 * \endif
 */
void QImPlotDragRectNode::setRect(const QRectF& rect)
{
    setRect(rect.left(), rect.top(), rect.right(), rect.bottom());
}

/**
 * \if ENGLISH
 * @brief Set rectangle coordinates (convenience overload)
 * @param x1 Left x coordinate in plot space
 * @param y1 Top y coordinate in plot space
 * @param x2 Right x coordinate in plot space
 * @param y2 Bottom y coordinate in plot space
 * \endif
 *
 * \if CHINESE
 * @brief 设置矩形坐标（便捷重载）
 * @param x1 左侧x坐标（绘图空间）
 * @param y1 顶部y坐标（绘图空间）
 * @param x2 右侧x坐标（绘图空间）
 * @param y2 底部y坐标（绘图空间）
 * \endif
 */
void QImPlotDragRectNode::setRect(double x1, double y1, double x2, double y2)
{
    QIM_D(d);
    // Ensure x1 <= x2, y1 <= y2 for consistent rectangle representation
    if (x1 > x2) std::swap(x1, x2);
    if (y1 > y2) std::swap(y1, y2);
    
    if (d->x1 != x1 || d->y1 != y1 || d->x2 != x2 || d->y2 != y2) {
        d->x1 = x1;
        d->y1 = y1;
        d->x2 = x2;
        d->y2 = y2;
        emit rectChanged(QRectF(x1, y1, x2 - x1, y2 - y1));
    }
}

/**
 * \if ENGLISH
 * @brief Get rectangle border color
 * @return Current QColor
 * \endif
 *
 * \if CHINESE
 * @brief 获取矩形边框颜色
 * @return 当前的QColor
 * \endif
 */
QColor QImPlotDragRectNode::color() const
{
    return (d_ptr->color.has_value()) ? toQColor(d_ptr->color->value()) : QColor();
}

/**
 * \if ENGLISH
 * @brief Set rectangle border color
 * @param c QColor for the rectangle border
 * \endif
 *
 * \if CHINESE
 * @brief 设置矩形边框颜色
 * @param c 矩形边框的QColor
 * \endif
 */
void QImPlotDragRectNode::setColor(const QColor& c)
{
    d_ptr->color = toImVec4(c);
    emit colorChanged(c);
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
int QImPlotDragRectNode::id() const
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
void QImPlotDragRectNode::setId(int id)
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
int QImPlotDragRectNode::flags() const
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
void QImPlotDragRectNode::setFlags(int flags)
{
    if (d_ptr->flags != flags) {
        d_ptr->flags = static_cast<ImPlotDragToolFlags>(flags);
        emit flagsChanged(flags);
    }
}

/**
 * \if ENGLISH
 * @brief Check if rectangle was clicked
 * @return true if rectangle was clicked in last render
 * \endif
 *
 * \if CHINESE
 * @brief 检查矩形是否被点击
 * @return 如果矩形在最后一次渲染中被点击，则为true
 * \endif
 */
bool QImPlotDragRectNode::clicked() const
{
    return d_ptr->clicked;
}

/**
 * \if ENGLISH
 * @brief Check if rectangle is hovered
 * @return true if rectangle is hovered in last render
 * \endif
 *
 * \if CHINESE
 * @brief 检查矩形是否被悬停
 * @return 如果矩形在最后一次渲染中被悬停，则为true
 * \endif
 */
bool QImPlotDragRectNode::hovered() const
{
    return d_ptr->hovered;
}

/**
 * \if ENGLISH
 * @brief Check if rectangle is being held
 * @return true if rectangle is being held in last render
 * \endif
 *
 * \if CHINESE
 * @brief 检查矩形是否被按住
 * @return 如果矩形在最后一次渲染中被按住，则为true
 * \endif
 */
bool QImPlotDragRectNode::held() const
{
    return d_ptr->held;
}

/**
 * \if ENGLISH
 * @brief Check if rectangle was modified by user
 * @return true if rectangle position was modified in last render
 * \endif
 *
 * \if CHINESE
 * @brief 检查矩形是否被用户修改
 * @return 如果矩形位置在最后一次渲染中被修改，则为true
 * \endif
 */
bool QImPlotDragRectNode::wasModified() const
{
    return d_ptr->wasModified;
}

/**
 * \if ENGLISH
 * @brief Begin drawing implementation
 * @return false to prevent endDraw from being called
 * @details Calls ImPlot::DragRect with appropriate parameters.
 *          Updates interaction state (clicked/hovered/held) after call.
 *          Emits rectChanged() if rectangle was modified by user.
 * \endif
 *
 * \if CHINESE
 * @brief 开始绘制实现
 * @return false以防止调用endDraw
 * @details 使用适当的参数调用ImPlot::DragRect。
 *          调用后更新交互状态（clicked/hovered/held）。
 *          如果用户修改了矩形，则发出rectChanged()信号。
 * \endif
 */
bool QImPlotDragRectNode::beginDraw()
{
    QIM_D(d);
    
    // Store previous coordinates to detect changes
    const double prevX1 = d->x1;
    const double prevY1 = d->y1;
    const double prevX2 = d->x2;
    const double prevY2 = d->y2;
    
    // Convert color to ImVec4
    ImVec4 colorVec = d->color.has_value() ? d->color->value() : ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
    
    // Call ImPlot DragRect API
    bool modified = ImPlot::DragRect(
        d->id,
        &d->x1,
        &d->y1,
        &d->x2,
        &d->y2,
        colorVec,
        d->flags,
        &d->clicked,
        &d->hovered,
        &d->held
    );
    
    // Update modification state
    d->wasModified = modified;
    
    // Emit signals if rectangle changed
    if (modified) {
        if (d->x1 != prevX1 || d->y1 != prevY1 || d->x2 != prevX2 || d->y2 != prevY2) {
            // Ensure consistent ordering
            if (d->x1 > d->x2) std::swap(d->x1, d->x2);
            if (d->y1 > d->y2) std::swap(d->y1, d->y2);
            emit rectChanged(QRectF(d->x1, d->y1, d->x2 - d->x1, d->y2 - d->y1));
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
    
    return false; // DragRect doesn't need endDraw
}

}  // namespace QIM