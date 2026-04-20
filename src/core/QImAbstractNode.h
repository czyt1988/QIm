#ifndef QIMABSTRACTNODE_H
#define QIMABSTRACTNODE_H
#include <QObject>
#include <QPointer>
#include "QImAPI.h"
namespace QIM
{
/**
 * \if ENGLISH
 * @brief Abstract base class for ImGui drawing nodes
 *
 * @class QImAbstractNode
 * @ingroup core
 *
 * @details Design philosophy:
 * - Each node should correspond to an ImGui scope (Begin/End pair, some may have no End)
 * - Supports nested hierarchical structure with automatic ID stack handling
 * - Controls rendering behavior via visible/enabled properties
 * - Subclasses only need to focus on their own Begin/End logic
 * - Automatically manages ImGui IDs
 *
 * Solves these ImGui problems:
 * - Scope nesting chaos (indentation hell)
 * - Difficult state management (window positions, fold states, etc.)
 * - Poor code reuse (repetitive Begin/End template code)
 *
 * Makes code more Qt-like. For native ImGui code like this:
 *
 * ```cpp
 * // Traditional ImGui (procedural)
 * if (ImGui::Begin("Window")) {
 *    if (ImPlot::BeginPlot("Plot")) {
 *        ImPlot::PlotLine(...);
 *        ImPlot::EndPlot();
 *    }
 *    ImGui::End();
 * }
 * ```
 *
 * Using QImAbstractNode, it becomes:
 *
 * ```cpp
 * auto window = new QImWindowNode(root);
 * window->setTitle("Window");
 *
 * auto plot = new QImPlotNode(window);  // auto-nesting
 * plot->setTitle("Plot");
 *
 * auto line = new QImPlotLineNode(plot); // auto child of Plot
 * line->setData(...);
 * ```
 *
 * > **Core idea**: Each UI element = one object, UI structure = object tree, rendering = recursive tree traversal
 *
 * @see QImWindowNode
 * @see QImPlotNode
 * @see QImPlotLineNode
 * \endif
 *
 * \if CHINESE
 * @brief ImGui 绘图节点的抽象基类
 *
 * @class QImAbstractNode
 * @ingroup core
 *
 * @details 设计理念：
 * - 每个节点理论应该对应一个ImGui作用域（Begin/End对,某些可以没有end）
 * - 支持嵌套层次结构，自动处理ID栈
 * - 通过visible/enabled属性控制渲染行为
 * - 子类只需关注自身节点的Begin/End逻辑
 * - 自动管理ImGui的id
 *
 * 解决ImGui的这些问题：
 * - 作用域嵌套混乱（缩进地狱）
 * - 状态管理困难（窗口位置、折叠状态等）
 * - 代码复用性差（重复的 Begin/End 模板代码）
 *
 * 让代码更Qt
 * 对于原生ImGui的如下代码：
 *
 * ```cpp
 * // 传统 ImGui（过程式）
 * if (ImGui::Begin("Window")) {
 *    if (ImPlot::BeginPlot("Plot")) {
 *        ImPlot::PlotLine(...);
 *        ImPlot::EndPlot();
 *    }
 *    ImGui::End();
 * }
 * ```
 *
 * 使用QImAbstractNode的方式，将变为下面的方式：
 *
 * ```cpp
 * auto window = new QImWindowNode(root);
 * window->setTitle("Window");
 *
 * auto plot = new QImPlotNode(window);  // 自动嵌套
 * plot->setTitle("Plot");
 *
 * auto line = new QImPlotLineNode(plot); // 自动成为 Plot 的子节点
 * line->setData(...);
 * ```
 *
 * > **核心思想**：每个 UI 元素 = 一个对象，UI 结构 = 对象树，渲染 = 递归遍历树
 *
 * @see QImWindowNode
 * @see QImPlotNode
 * @see QImPlotLineNode
 * \endif
 */
class QIM_CORE_API QImAbstractNode : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool visible READ isVisible WRITE setVisible NOTIFY visibleChanged)
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)
public:
    enum RenderOption
    {
        RenderIgnoreVisible = 1 << 0,  ///< Render ignores isVisible state
        RenderNotAutoID     = 1 << 1   ///< Do not auto-push ID
    };
    Q_DECLARE_FLAGS(RenderOptionFlags, RenderOption)
    Q_FLAG(RenderOptionFlags)
public:
    explicit QImAbstractNode(QObject* parent = nullptr);
    ~QImAbstractNode() override;
    // === State control ===
    virtual bool isVisible() const;
    virtual void setVisible(bool visible);

    virtual bool isEnabled() const;
    virtual void setEnabled(bool enabled);

    // === Child node management (O(1) access, no type conversion) ===
    void addChildNode(QImAbstractNode* child);
    void insertChildNode(int index, QImAbstractNode* child);
    void removeChildNode(QImAbstractNode* child);
    bool takeChildNode(QImAbstractNode* child);  // Remove and return ownership
    void clearChildrenNodes();
    int indexOfChildNode(QImAbstractNode* child) const;
    const QList< QImAbstractNode* >& childrenNodes() const;
    const QList< QImAbstractNode* >& childrenNodesZOrdered() const;
    int childNodeCount() const;
    QImAbstractNode* childNodeAt(int index) const;
    QImAbstractNode* parentNode() const;
    //
    void setAutoIdEnabled(bool on);
    bool isAutoIdEnabled() const;
    int zOrder() const;
    void setZOrder(int z);
    //----------------------------------------------------
    //
    //----------------------------------------------------
    template< typename T >
    QList< T > findChildrenNodes() const;
    // Core render entry point - called by QImWidget
    void render();
    // Set render option flags
    void setRenderOptionFlags(RenderOptionFlags f);
    RenderOptionFlags renderOptionFlags() const;
    void setRenderOption(RenderOption f, bool on);
    bool testRenderOption(RenderOption f) const;
Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when node visibility changes
     * @param[in] visible New visibility state
     * @details Triggered by setVisible() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 节点可见性更改时触发
     * @param[in] visible 新的可见性状态
     * @details 当值实际更改时由setVisible()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void visibleChanged(bool visible);
    /**
     * \if ENGLISH
     * @brief Emitted when node enabled state changes
     * @param[in] enabled New enabled state
     * @details Triggered by setEnabled() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 节点启用状态更改时触发
     * @param[in] enabled 新的启用状态
     * @details 当值实际更改时由setEnabled()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void enabledChanged(bool enabled);
    /**
     * \if ENGLISH
     * @brief Emitted when a child node is removed
     * @param[in] c The removed child node
     * @details Triggered when a child node is removed via removeChildNode() or takeChildNode().
     * @note The node has not been destroyed yet; qobject_cast is still valid.
     * \endif
     *
     * \if CHINESE
     * @brief 子节点移除信号
     * @param[in] c 移除的子节点
     * @details 通过removeChildNode()或takeChildNode()移除子节点时触发。
     * @note 此时节点还未销毁，可以调用qobject_cast
     * \endif
     */
    void childNodeRemoved(QIM::QImAbstractNode* c);
    /**
     * \if ENGLISH
     * @brief Emitted when a child node is added
     * @param[in] c The added child node
     * @details Triggered when a child node is added via addChildNode() or insertChildNode().
     *          Connect to perform initialization or UI updates for the new child.
     * \endif
     *
     * \if CHINESE
     * @brief 子节点添加信号
     * @param[in] c 添加的子节点
     * @details 通过addChildNode()或insertChildNode()添加子节点时触发。
     *          连接到执行初始化或UI更新。
     * \endif
     */
    void childNodeAdded(QIM::QImAbstractNode* c);

protected:
    /**
     * \if ENGLISH
     * @brief Subclass implements specific ImGui rendering begin logic
     *
     * @details Typical implementation pattern:
     * @code
     * bool QImPlotSectionNode::beginDraw()
     * {
     *     return ImPlot::BeginPlot(title.toUtf8().constData());
     * }
     * @endcode
     *
     * @return true if section is open and children should be rendered
     * \endif
     *
     * \if CHINESE
     * @brief 子类实现具体ImGui渲染逻辑
     *
     * @details 典型实现模式：
     * @code
     * bool QImPlotSectionNode::beginDraw()
     * {
     *     return ImPlot::BeginPlot(title.toUtf8().constData());
     * }
     * @endcode
     *
     * @return true if section is open and children should be rendered
     * \endif
     */
    virtual bool beginDraw() = 0;

    /**
     * \if ENGLISH
     * @brief Subclass implements specific ImGui rendering end logic
     *
     * @details Typical implementation pattern:
     * @code
     * void QImPlotSectionNode::endDraw()
     * {
     *     ImPlot::EndPlot();
     * }
     * @endcode
     * \endif
     *
     * \if CHINESE
     * @brief 子类实现具体ImGui渲染结束逻辑
     *
     * @details 典型实现模式：
     * @code
     * void QImPlotSectionNode::endDraw()
     * {
     *     ImPlot::EndPlot();
     * }
     * @endcode
     * \endif
     */
    virtual void endDraw();


private:
    // Remove child list reference only (does not change QObject parent-child); used by destructor
    void removeFromParentList();
    // Update z-order sorted list
    void updateZOrderedList();

private:
    bool m_visible { true };
    bool m_enabled { true };
    int m_zOrder { 0 };  // z-order value
    QList< QImAbstractNode* > m_children;
    QList< QImAbstractNode* > m_childrenZordered;  // Pre-sorted child node list by z-order
    QPointer< QImAbstractNode > m_parent;          // Logical parent node (weak reference, avoid circular)
    RenderOptionFlags m_renderFlags;
};

template< typename T >
inline QList< T > QImAbstractNode::findChildrenNodes() const
{
    QList< T > res;
    for (QImAbstractNode* n : childrenNodesZOrdered()) {
        if (T item = qobject_cast< T >(n)) {
            res.push_back(item);
        }
    }
    return res;
}

}  // end namespace QIM
#endif  // QIMABSTRACTNODE_H