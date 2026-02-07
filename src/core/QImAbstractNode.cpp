#include "QImAbstractNode.h"
#include "imgui.h"

namespace QIM
{
QImAbstractNode::QImAbstractNode(QObject* parent) : QObject(parent)
{
    // 设置默认 objectName（用于 ImGui ID）
    if (objectName().isEmpty()) {
        setObjectName(QStringLiteral("QImAbstractNode"));
    }

    // 如果传入的 parent 是 QImAbstractNode，自动建立逻辑父子关系
    if (auto nodeParent = qobject_cast< QImAbstractNode* >(parent)) {
        nodeParent->addChildNode(this);
    }
}

QImAbstractNode::~QImAbstractNode()
{
    // 1. 先移除所有子节点的逻辑关系（触发子节点的 removeFromParentList）
    clearChildrenNodes();

    // 2. 从逻辑父节点的子列表中移除自己
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

void QImAbstractNode::addChildNode(QImAbstractNode* child)
{
    insertChildNode(m_children.size(), child);
}

void QImAbstractNode::insertChildNode(int index, QImAbstractNode* child)
{
    if (!child || child == this) {
        return;
    }

    // 处理边界情况
    if (index < 0) {
        index = 0;
    }
    if (index > m_children.size()) {
        index = m_children.size();
    }

    // 避免重复添加
    if (m_children.contains(child)) {
        // 如果已在列表中，先移除再插入到新位置
        m_children.removeOne(child);
    } else {
        // 处理旧父节点
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
 * @brief 移除子节点
 *
 * 此函数会发射@ref childNodeRemoved 信号，此信号发射的时候，节点还未删除
 * @note 子节点会被delete
 * @param child
 */
void QImAbstractNode::removeChildNode(QImAbstractNode* child)
{
    if (takeChildNode(child)) {
        child->deleteLater();
    }
}

/**
 * @brief 拿出子节点
 *
 * 此函数会发射@ref childNodeRemoved 信号
 * @note 子节点不会被delete，你可以继续操作子节点
 * @note 如果子节点的parentObject为父节点，那么此函数会解除和父节点的QObject的父子关系
 * @param child
 * @return
 */
bool QImAbstractNode::takeChildNode(QImAbstractNode* child)
{
    if (!child || !m_children.contains(child)) {
        return false;
    }

    // 从子列表移除
    m_children.removeOne(child);

    // 解除逻辑父子关系
    child->m_parent.clear();  // QPointer 自动处理

    // 解除 QObject 父子关系（转移所有权给调用者）
    if (child->parentNode() == this) {
        child->setParent(nullptr);
    }
    updateZOrderedList();
    Q_EMIT childNodeRemoved(child);
    return true;
}

void QImAbstractNode::clearChildrenNodes()
{
    if (m_children.isEmpty()) {
        return;
    }

    // 创建子节点副本（避免在操作过程中列表被修改）
    // 调用std::move后此时：
    //   - childrenCopy 拥有原 m_children 的所有数据
    //   - m_children 被置为空列表（Qt 保证移动后源容器为空）
    //   - m_children 仍然有效，可安全调用 clear()/isEmpty() 等
    // QList/QVector 等容器的移动构造/赋值后，源容器变为空,这是 Qt 的明确设计（不同于标准库容器的"有效但未指定状态"）
    QList< QImAbstractNode* > childrenCopy = std::move(m_children);

    // 批量处理子节点：解除父子关系 + 内存管理
    for (QImAbstractNode* child : childrenCopy) {
        if (!child) {
            continue;
        }
        child->m_parent.clear();    // 解除逻辑父子关系
        child->setParent(nullptr);  // 解除 QObject 父子关系
        child->deleteLater();       // 安全延迟删除
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
    m_autoId = on;
}

bool QImAbstractNode::isAutoIdEnabled() const
{
    return m_autoId;
}

int QImAbstractNode::zOrder() const
{
    return m_zOrder;
}

void QImAbstractNode::setZOrder(int z)
{
    if (m_zOrder != z) {
        m_zOrder = z;
        // 通知父节点：子节点 zOrder 变更，需要更新排序
        if (m_parent) {
            m_parent->updateZOrderedList();
        }
    }
}

void QImAbstractNode::render()
{
    // 对于imgui，不可见就直接不渲染就行
    if (!m_visible) {
        return;
    }
    // === 处理禁用状态 ===
    bool isDisable = !m_enabled;
    beginDisabled(isDisable);
    if (m_autoId) {
        ImGui::PushID(this);  // ImGui 原生支持 void* 重载，高效且唯一
    }
    if (beginDraw()) {
        // 子节点
        for (QImAbstractNode* child : std::as_const(m_childrenZordered)) {
            if (child) {  // 安全检查（防悬空指针）
                child->render();
            }
        }
        endDraw();
    }
    if (m_autoId) {
        ImGui::PopID();  // 严格匹配 PushID
    }
    endDisabled(isDisable);
}

void QImAbstractNode::endDraw()
{

}

void QImAbstractNode::beginDisabled(bool isDisable)
{
    Q_UNUSED(isDisable);
}

void QImAbstractNode::endDisabled(bool isDisable)
{
    Q_UNUSED(isDisable);
}

void QImAbstractNode::removeFromParentList()
{
    // 仅从父节点子列表移除引用（不改变 QObject 父子关系）
    // 由析构函数调用，此时父节点可能正在析构，需安全检查
    if (m_parent && m_parent->m_children.contains(this)) {
        m_parent->m_children.removeOne(this);
    }
    m_parent.clear();
}

void QImAbstractNode::updateZOrderedList()
{
    // 0-1 个子节点无需排序
    if (m_children.size() <= 1) {
        m_childrenZordered = m_children;
        return;
    }

    // 创建 (节点, 原始索引) 临时列表，跳过空指针
    struct ChildWithIndex
    {
        QImAbstractNode* node;
        int originalIndex;
    };
    QList< ChildWithIndex > temp;
    temp.reserve(m_children.size());

    for (int i = 0; i < m_children.size(); ++i) {
        QImAbstractNode* child = m_children.at(i);
        if (child) {  // 安全过滤空指针
            temp.append({ child, i });
        }
    }

    // 排序规则：1) zOrder 升序 2) 原始索引升序
    std::sort(temp.begin(), temp.end(), [](const ChildWithIndex& a, const ChildWithIndex& b) {
        if (a.node->m_zOrder != b.node->m_zOrder) {
            return a.node->m_zOrder < b.node->m_zOrder;  // 小 zOrder 先渲染（下层）
        }
        return a.originalIndex < b.originalIndex;  // 同 zOrder 时按添加顺序
    });

    // 生成预排序列表
    m_childrenZordered.clear();
    m_childrenZordered.reserve(temp.size());
    for (const auto& item : temp) {
        m_childrenZordered.append(item.node);
    }
}

}
