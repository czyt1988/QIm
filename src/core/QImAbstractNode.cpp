#include "QImAbstractNode.h"
#include "imgui.h"

namespace QIM
{
/**
 * \if ENGLISH
 * @brief Construct a QImAbstractNode with optional parent
 *
 * @details Sets default objectName for ImGui ID usage. If the parent is a
 *          QImAbstractNode, automatically establishes the logical parent-child
 *          relationship by calling addChildNode().
 * @param[in] parent Parent QObject (optional, nullptr by default)
 * \endif
 *
 * \if CHINESE
 * @brief 构造QImAbstractNode，可选指定父对象
 *
 * @details 设置默认objectName用于ImGui ID。如果传入的parent是QImAbstractNode，
 *          自动建立逻辑父子关系，调用addChildNode()。
 * @param[in] parent 父QObject对象（可选，默认nullptr）
 * \endif
 */
QImAbstractNode::QImAbstractNode(QObject* parent) : QObject(parent)
{
    // Set default objectName for ImGui ID
    if (objectName().isEmpty()) {
        setObjectName(QStringLiteral("QImAbstractNode"));
    }

    // If parent is QImAbstractNode, auto-establish logical parent-child relationship
    if (auto nodeParent = qobject_cast< QImAbstractNode* >(parent)) {
        nodeParent->addChildNode(this);
    }
}

/**
 * \if ENGLISH
 * @brief Destructor - cleans up child nodes and parent relationships
 *
 * @details Removes all child nodes first (triggers their removeFromParentList),
 *          then removes itself from its parent's child list.
 * \endif
 *
 * \if CHINESE
 * @brief 析构函数 - 清理子节点和父节点关系
 *
 * @details 先移除所有子节点的逻辑关系（触发子节点的removeFromParentList），
 *          然后从逻辑父节点的子列表中移除自己。
 * \endif
 */
QImAbstractNode::~QImAbstractNode()
{
    // 1. Remove all child logical relationships first
    clearChildrenNodes();

    // 2. Remove self from parent's child list
    if (m_parent) {
        m_parent->removeFromParentList();
    }
}

bool QImAbstractNode::isVisible() const
{
    return m_visible;
}

void QImAbstractNode::setVisible(bool visible)
{
    if (m_visible != visible) {
        m_visible = visible;
        Q_EMIT visibleChanged(visible);
    }
}

bool QImAbstractNode::isEnabled() const
{
    return m_enabled;
}

void QImAbstractNode::setEnabled(bool enabled)
{
    if (m_enabled != enabled) {
        m_enabled = enabled;
        Q_EMIT enabledChanged(enabled);
    }
}

/**
 * \if ENGLISH
 * @brief Add a child node at the end of the child list
 * @param[in] child Child node to add
 * @details Equivalent to insertChildNode(childCount(), child).
 * \endif
 *
 * \if CHINESE
 * @brief 在子节点列表末尾添加子节点
 * @param[in] child 要添加的子节点
 * @details 等价于insertChildNode(childCount(), child)。
 * \endif
 */
void QImAbstractNode::addChildNode(QImAbstractNode* child)
{
    insertChildNode(m_children.size(), child);
}

/**
 * \if ENGLISH
 * @brief Insert a child node at a specific index
 * @param[in] index Insertion position
 * @param[in] child Child node to insert
 * @details Handles boundary index adjustment, duplicate repositioning, and old parent
 *          detachment. If the child is already in the list, it is moved to the new position.
 *          If the child has a different parent, it is first detached from that parent.
 *          Emits childNodeAdded() on successful insertion.
 * \endif
 *
 * \if CHINESE
 * @brief 在指定位置插入子节点
 * @param[in] index 插入位置
 * @param[in] child 要插入的子节点
 * @details 处理边界索引调整、重复重新定位和旧父节点脱离。
 *          如果子节点已在列表中，移到新位置。
 *          如果子节点有不同的父节点，先脱离旧父节点。
 *          成功插入时发射childNodeAdded()信号。
 * \endif
 */
void QImAbstractNode::insertChildNode(int index, QImAbstractNode* child)
{
    if (!child || child == this) {
        return;
    }

    // Handle boundary cases
    if (index < 0) {
        index = 0;
    }
    if (index > m_children.size()) {
        index = m_children.size();
    }

    // Avoid duplicate addition
    if (m_children.contains(child)) {
        // If already in list, remove first then insert at new position
        m_children.removeOne(child);
        // Re-adjust index after removal to prevent overflow
        if (index > m_children.size()) {
            index = m_children.size();
        }
    } else {
        // Handle old parent node
        if (child->m_parent && child->m_parent != this) {
            child->m_parent->takeChildNode(child);
        }
        child->setParent(this);
        child->m_parent = this;
    }

    m_children.insert(index, child);
    updateZOrderedList();
    Q_EMIT childNodeAdded(child);
}

/**
 * \if ENGLISH
 * @brief Remove a child node and schedule its deletion
 *
 * @details Emits @ref childNodeRemoved signal. The node has not been destroyed yet
 *          when the signal is emitted; qobject_cast is still valid.
 * @note The child node will be deleted via deleteLater()
 * @param[in] child Child node to remove
 * \endif
 *
 * \if CHINESE
 * @brief 移除子节点并安排删除
 *
 * @details 此函数会发射@ref childNodeRemoved信号，此信号发射的时候，节点还未删除
 * @note 子节点会被deleteLater()删除
 * @param[in] child 要移除的子节点
 * \endif
 */
void QImAbstractNode::removeChildNode(QImAbstractNode* child)
{
    if (takeChildNode(child)) {
        child->deleteLater();
    }
}

/**
 * \if ENGLISH
 * @brief Take a child node out of the list without deleting it
 *
 * @details Emits @ref childNodeRemoved signal. The node has not been destroyed yet
 *          when the signal is emitted; qobject_cast is still valid.
 * @note The child node is NOT deleted; you can continue operating on it.
 * @note If the child's parentObject is this node, the QObject parent-child relationship
 *       is also detached, transferring ownership to the caller.
 * @param[in] child Child node to take
 * @return true if the child was successfully taken, false otherwise
 * \endif
 *
 * \if CHINESE
 * @brief 拿出子节点（不删除）
 *
 * @details 此函数会发射@ref childNodeRemoved信号
 * @note 子节点不会被delete，你可以继续操作子节点
 * @note 如果子节点的parentObject为父节点，那么此函数会解除和父节点的QObject的父子关系
 * @param[in] child 要拿出的子节点
 * @return true表示成功拿出，false表示失败
 * \endif
 */
bool QImAbstractNode::takeChildNode(QImAbstractNode* child)
{
    if (!child || !m_children.contains(child)) {
        return false;
    }

    // Remove from child list
    m_children.removeOne(child);

    // Detach logical parent-child relationship
    child->m_parent.clear();  // QPointer auto-handles

    // Detach QObject parent-child relationship (transfer ownership to caller)
    if (child->parentNode() == this) {
        child->setParent(nullptr);
    }
    updateZOrderedList();
    Q_EMIT childNodeRemoved(child);
    return true;
}

/**
 * \if ENGLISH
 * @brief Remove and delete all child nodes
 *
 * @details Creates a copy of the child list to avoid modification during iteration.
 *          Each child's logical and QObject parent relationships are detached,
 *          and the child is scheduled for delayed deletion via deleteLater().
 * \endif
 *
 * \if CHINESE
 * @brief 移除并删除所有子节点
 *
 * @details 创建子节点副本以避免在操作过程中列表被修改。
 *          每个子节点的逻辑和QObject父子关系被解除，
 *          子节点通过deleteLater()安全延迟删除。
 * \endif
 */
void QImAbstractNode::clearChildrenNodes()
{
    if (m_children.isEmpty()) {
        return;
    }

    // Create child copy (avoid list modification during iteration)
    // After std::move:
    //   - childrenCopy has all data from original m_children
    //   - m_children is set to empty list (Qt guarantees moved-from container is empty)
    //   - m_children remains valid, safe to call clear()/isEmpty() etc.
    // QList/QVector after move construction/assignment: source container becomes empty (Qt's explicit design, unlike std containers' "valid but unspecified state")
    QList< QImAbstractNode* > childrenCopy = std::move(m_children);

    // Batch process children: detach parent relationships + memory management
    for (QImAbstractNode* child : childrenCopy) {
        if (!child) {
            continue;
        }
        child->m_parent.clear();    // Detach logical parent-child relationship
        child->setParent(nullptr);  // Detach QObject parent-child relationship
        child->deleteLater();       // Safe delayed deletion
    }
    m_childrenZordered.clear();
}

int QImAbstractNode::indexOfChildNode(QImAbstractNode* child) const
{
    return m_children.indexOf(child);
}

const QList< QImAbstractNode* >& QImAbstractNode::childrenNodes() const
{
    return m_children;
}

const QList< QImAbstractNode* >& QImAbstractNode::childrenNodesZOrdered() const
{
    return m_childrenZordered;
}

int QImAbstractNode::childNodeCount() const
{
    return m_children.size();
}

QImAbstractNode* QImAbstractNode::childNodeAt(int index) const
{
    return (index >= 0 && index < m_children.size()) ? m_children.at(index) : nullptr;
}

QImAbstractNode* QImAbstractNode::parentNode() const
{
    return m_parent;
}

void QImAbstractNode::setAutoIdEnabled(bool on)
{
    setRenderOption(RenderNotAutoID, !on);
}

bool QImAbstractNode::isAutoIdEnabled() const
{
    return !testRenderOption(RenderNotAutoID);
}

int QImAbstractNode::zOrder() const
{
    return m_zOrder;
}

/**
 * \if ENGLISH
 * @brief Set the z-order value for this node
 * @param[in] z New z-order value
 * @details If the value changes, notifies the parent node to update its
 *          z-order sorted list. Lower z-order values are rendered first (bottom layer).
 * \endif
 *
 * \if CHINESE
 * @brief 设置节点的z-order值
 * @param[in] z 新的z-order值
 * @details 如果值发生变化，通知父节点更新其z-order排序列表。
 *          z-order值小的先渲染（底层）。
 * \endif
 */
void QImAbstractNode::setZOrder(int z)
{
    if (m_zOrder != z) {
        m_zOrder = z;
        // Notify parent node: child zOrder changed, need to update sorting
        if (m_parent) {
            m_parent->updateZOrderedList();
        }
    }
}

/**
 * \if ENGLISH
 * @brief Core render entry point - recursively renders this node and its children
 *
 * @details If RenderIgnoreVisible is not set and the node is not visible, rendering
 *          is skipped entirely. If auto-ID is enabled, ImGui::PushID/PopID is called
 *          around the rendering to provide unique identification. When beginDraw()
 *          returns true, child nodes are rendered in z-order, followed by endDraw().
 * \endif
 *
 * \if CHINESE
 * @brief 核心渲染入口 - 递归渲染此节点及其子节点
 *
 * @details 如果未设置RenderIgnoreVisible且节点不可见，则直接跳过渲染。
 *          如果启用了自动ID，在渲染前后调用ImGui::PushID/PopID提供唯一标识。
 *          当beginDraw()返回true时，子节点按z-order顺序渲染，然后调用endDraw()。
 * \endif
 */
void QImAbstractNode::render()
{
    if (!testRenderOption(RenderIgnoreVisible)) {
        if (!isVisible()) {
            // For ImGui, invisible nodes are simply not rendered
            return;
        }
    }
    bool autoID = isAutoIdEnabled();
    if (autoID) {
        ImGui::PushID(this);  // ImGui native void* overload, efficient and unique
    }
    if (beginDraw()) {
        // Render child nodes
        for (QImAbstractNode* child : std::as_const(m_childrenZordered)) {
            if (child) {  // Safety check (prevent dangling pointers)
                child->render();
            }
        }
        endDraw();
    }
    if (autoID) {
        ImGui::PopID();  // Strictly matching PushID
    }
}

void QImAbstractNode::setRenderOptionFlags(RenderOptionFlags f)
{
    m_renderFlags = f;
}

void QImAbstractNode::setRenderOption(RenderOption f, bool on)
{
    m_renderFlags.setFlag(f, on);
}

bool QImAbstractNode::testRenderOption(RenderOption f) const
{
    return m_renderFlags.testFlag(f);
}

QImAbstractNode::RenderOptionFlags QImAbstractNode::renderOptionFlags() const
{
    return m_renderFlags;
}

void QImAbstractNode::endDraw()
{
}


/**
 * \if ENGLISH
 * @brief Remove this node from its parent's child list (does not change QObject parent)
 *
 * @details Called by destructor. The parent may be destructing, so safety checks are needed.
 *          Only removes the reference from the parent's child list, without changing
 *          the QObject parent-child relationship.
 * \endif
 *
 * \if CHINESE
 * @brief 仅从父节点子列表移除引用（不改变QObject父子关系）
 *
 * @details 由析构函数调用，此时父节点可能正在析构，需安全检查。
 *          仅从父节点子列表移除引用，不改变QObject父子关系。
 * \endif
 */
void QImAbstractNode::removeFromParentList()
{
    // Only remove reference from parent's child list (does not change QObject parent-child relationship)
    // Called by destructor, parent may be destructing, need safety check
    if (m_parent && m_parent->m_children.contains(this)) {
        m_parent->m_children.removeOne(this);
    }
    m_parent.clear();
}

/**
 * \if ENGLISH
 * @brief Update the z-order pre-sorted child list
 *
 * @details For 0-1 children, no sorting is needed (direct copy). For more children,
 *          creates a temporary (node, originalIndex) list, filters null pointers,
 *          then sorts by: 1) zOrder ascending 2) original index ascending.
 *          Lower zOrder values render first (bottom layer).
 * \endif
 *
 * \if CHINESE
 * @brief 更新z-order预排序的子节点列表
 *
 * @details 0-1个子节点无需排序（直接拷贝）。多于1个子节点时，
 *          创建(node, originalIndex)临时列表，过滤空指针，
 *          然后按：1) zOrder升序 2) 原始索引升序 排序。
 *          zOrder值小的先渲染（底层）。
 * \endif
 */
void QImAbstractNode::updateZOrderedList()
{
    // 0-1 children need no sorting
    if (m_children.size() <= 1) {
        m_childrenZordered = m_children;
        return;
    }

    // Create (node, original index) temporary list, skip null pointers
    struct ChildWithIndex
    {
        QImAbstractNode* node;
        int originalIndex;
    };
    QList< ChildWithIndex > temp;
    temp.reserve(m_children.size());

    for (int i = 0; i < m_children.size(); ++i) {
        QImAbstractNode* child = m_children.at(i);
        if (child) {  // Safety filter for null pointers
            temp.append({ child, i });
        }
    }

    // Sort rule: 1) zOrder ascending 2) original index ascending
    std::sort(temp.begin(), temp.end(), [](const ChildWithIndex& a, const ChildWithIndex& b) {
        if (a.node->m_zOrder != b.node->m_zOrder) {
            return a.node->m_zOrder < b.node->m_zOrder;  // Lower zOrder renders first (bottom layer)
        }
        return a.originalIndex < b.originalIndex;  // Same zOrder: by insertion order
    });

    // Generate pre-sorted list
    m_childrenZordered.clear();
    m_childrenZordered.reserve(temp.size());
    for (const auto& item : temp) {
        m_childrenZordered.append(item.node);
    }
}

}