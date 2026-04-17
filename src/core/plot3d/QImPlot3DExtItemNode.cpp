#include "QImPlot3DExtItemNode.h"
#include "QImPlot3DExtNode.h"
#include "implot3d.h"

namespace QIM
{

class QImPlot3DExtItemNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlot3DExtItemNode)

public:
    PrivateData(QImPlot3DExtItemNode* p);

public:
    QByteArray labelUtf8;                    ///< UTF8-only storage for label
    ImPlot3DItemFlags flags { ImPlot3DItemFlags_None };  ///< Item flags (must be named 'flags' for macro access)
    bool userVisible { true };               ///< User-set visibility state (valid before first render)
};

QImPlot3DExtItemNode::PrivateData::PrivateData(QImPlot3DExtItemNode* p) : q_ptr(p)
{
}

//----------------------------------------------------
// QImPlot3DExtItemNode
//----------------------------------------------------

/**
 * \if ENGLISH
 * @brief Constructor - initializes with default settings
 * @param par Parent object (typically QImPlot3DExtNode)
 * @details Sets auto ID disabled and render option to ignore visible,
 *          allowing visibility to be controlled by ImPlot3D item state.
 * \endif
 *
 * \if CHINESE
 * @brief 构造函数 - 使用默认设置初始化
 * @param par 父对象（通常是 QImPlot3DExtNode）
 * @details 设置自动 ID 禁用，渲染选项忽略可见性，
 *          允许可见性由 ImPlot3D 元素状态控制。
 * \endif
 */
QImPlot3DExtItemNode::QImPlot3DExtItemNode(QObject* par)
    : QImAbstractNode(par)
    , QIM_PIMPL_CONSTRUCT
{
    setAutoIdEnabled(false);
    setRenderOption(RenderIgnoreVisible, true);  // 3D item visibility controlled by ImPlot3D item state
}

/**
 * \if ENGLISH
 * @brief Destructor
 * \endif
 *
 * \if CHINESE
 * @brief 析构函数
 * \endif
 */
QImPlot3DExtItemNode::~QImPlot3DExtItemNode()
{
}

//----------------------------------------------------
// Label property
//----------------------------------------------------

/**
 * \if ENGLISH
 * @brief Sets the label text
 * @param name New label text (converted to UTF8 for storage)
 * @details Emits labelChanged signal if the label actually changes.
 * \endif
 *
 * \if CHINESE
 * @brief 设置标签文本
 * @param name 新标签文本（转换为 UTF8 存储）
 * @details 如果标签实际改变则发射 labelChanged 信号。
 * \endif
 */
void QImPlot3DExtItemNode::setLabel(const QString& name)
{
    QIM_D(d);
    QByteArray nameUtf = name.toUtf8();
    if (nameUtf != d->labelUtf8) {
        d->labelUtf8 = nameUtf;
        Q_EMIT labelChanged(name);
    }
}

/**
 * \if ENGLISH
 * @brief Returns the label as QString
 * @return Label text converted from UTF8 storage
 * \endif
 *
 * \if CHINESE
 * @brief 返回标签（QString 格式）
 * @return 从 UTF8 存储转换的标签文本
 * \endif
 */
QString QImPlot3DExtItemNode::label() const
{
    return QString::fromUtf8(d_ptr->labelUtf8);
}

/**
 * \if ENGLISH
 * @brief Returns direct UTF8 pointer for rendering
 * @return Pointer to UTF8 label data, or default "##plot3dItem" if empty
 * @details Used in beginDraw() to pass label to ImPlot3D functions without conversion overhead.
 * \endif
 *
 * \if CHINESE
 * @brief 返回用于渲染的直接 UTF8 指针
 * @return UTF8 标签数据指针，若为空则返回默认 "##plot3dItem"
 * @details 在 beginDraw() 中使用，直接传递标签给 ImPlot3D 函数，无转换开销。
 * \endif
 */
const char* QImPlot3DExtItemNode::labelConstData() const
{
    return (d_ptr->labelUtf8.isEmpty() ? "##plot3dItem" : d_ptr->labelUtf8.constData());
}

//----------------------------------------------------
// Parent plot node
//----------------------------------------------------

/**
 * \if ENGLISH
 * @brief Returns the parent QImPlot3DExtNode
 * @return Pointer to parent 3D plot node, nullptr if not found
 * @details Searches up the parent chain for the first QImPlot3DExtNode.
 *          Similar pattern to 2D's QImPlotItemNode::plotNode().
 * \endif
 *
 * \if CHINESE
 * @brief 返回父 QImPlot3DExtNode
 * @return 父 3D 绘图节点指针，未找到则返回 nullptr
 * @details 向上搜索父链查找第一个 QImPlot3DExtNode。
 *          类似 2D 的 QImPlotItemNode::plotNode() 模式。
 * \endif
 */
QImPlot3DExtNode* QImPlot3DExtItemNode::plot3DNode() const
{
    QImAbstractNode* parent = parentNode();

    while (parent != nullptr) {
        if (auto* plot3DNode = qobject_cast<QImPlot3DExtNode*>(parent)) {
            return plot3DNode;
        }
        parent = parent->parentNode();
    }

    return nullptr;
}

//----------------------------------------------------
// Item flags (positive semantics via macro)
//----------------------------------------------------

// Generate isLegendEntryEnabled/setLegendEntryEnabled using QIMPLOT3D_FLAG_ENABLED_ACCESSOR
// Maps ImPlot3DItemFlags_NoLegend (negative) to positive semantics
QIMPLOT3D_FLAG_ENABLED_ACCESSOR(QImPlot3DExtItemNode, LegendEntryEnabled, ImPlot3DItemFlags_NoLegend, legendEntryEnabledChanged)

// Generate isFitEnabled/setFitEnabled using QIMPLOT3D_FLAG_ENABLED_ACCESSOR
// Maps ImPlot3DItemFlags_NoFit (negative) to positive semantics
QIMPLOT3D_FLAG_ENABLED_ACCESSOR(QImPlot3DExtItemNode, FitEnabled, ImPlot3DItemFlags_NoFit, fitEnabledChanged)

//----------------------------------------------------
// Visibility
//----------------------------------------------------

/**
 * \if ENGLISH
 * @brief Checks if the item is visible
 * @return true if visible (user's setting), false otherwise
 * @details Returns user-set visibility state. Unlike 2D which has ImPlotItem::Show,
 *          ImPlot3D doesn't expose a similar item structure.
 * \endif
 *
 * \if CHINESE
 * @brief 检查元素是否可见
 * @return true 表示可见（用户设置），false 表示不可见
 * @details 返回用户设置的可见性状态。与 2D 有 ImPlotItem::Show 不同，
 *          ImPlot3D 暴露类似的元素结构。
 * \endif
 */
bool QImPlot3DExtItemNode::isVisible() const
{
    // Check parent visibility first
    QImAbstractNode* parent = parentNode();
    if (parent && !parent->isVisible()) {
        return false;
    }
    // Return user-set visibility state
    return d_ptr->userVisible;
}

/**
 * \if ENGLISH
 * @brief Sets item visibility
 * @param visible true to show, false to hide
 * @details Stores user's visibility preference and calls base class setVisible.
 * \endif
 *
 * \if CHINESE
 * @brief 设置元素可见性
 * @param visible true 表示显示，false 表示隐藏
 * @details 存储用户可见性偏好并调用基类 setVisible。
 * \endif
 */
void QImPlot3DExtItemNode::setVisible(bool visible)
{
    if (d_ptr->userVisible != visible) {
        d_ptr->userVisible = visible;
        // Call base class to emit signal and update state
        QImAbstractNode::setVisible(visible);
    }
}

//----------------------------------------------------
// Protected methods
//----------------------------------------------------

/**
 * \if ENGLISH
 * @brief Called after beginDraw() completes
 * @details Override in derived classes for post-render cleanup.
 *          Default implementation is empty.
 * \endif
 *
 * \if CHINESE
 * @brief beginDraw() 完成后调用
 * @details 在派生类中重写以进行渲染后清理。
 *          默认实现为空。
 * \endif
 */
void QImPlot3DExtItemNode::endDraw()
{
}

}  // end namespace QIM

