#ifndef QIMABSTRACTNODE_H
#define QIMABSTRACTNODE_H
#include <QObject>
#include <QPointer>
#include "QImAPI.h"
namespace QIM
{
/**
 * @brief ImGui 绘图节点的抽象基类
 *
 * 设计理念：
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
 * 传统 ImGui（过程式）
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
 */
class QIM_CORE_API QImAbstractNode : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool visible READ isVisible WRITE setVisible NOTIFY visibleChanged)
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)
public:
    enum RenderOption
    {
        RenderIgnoreVisible = 1 << 0,  ///< render函数不考虑isVisible状态
        RenderNotAutoID     = 1 << 1   ///< 不自动推入id
    };
    Q_DECLARE_FLAGS(RenderOptionFlags, RenderOption)
    Q_FLAG(RenderOptionFlags)
public:
    explicit QImAbstractNode(QObject* parent = nullptr);
    ~QImAbstractNode() override;
    // === 状态控制 ===
    virtual bool isVisible() const;
    virtual void setVisible(bool visible);

    virtual bool isEnabled() const;
    virtual void setEnabled(bool enabled);

    // === 子节点管理 (O(1) 访问，无类型转换) ===
    void addChildNode(QImAbstractNode* child);
    void insertChildNode(int index, QImAbstractNode* child);
    void removeChildNode(QImAbstractNode* child);
    bool takeChildNode(QImAbstractNode* child);  // 移除并返回所有权
    void clearChildrenNodes();
    // 查找子节点的索引
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
    // 核心渲染入口 - 由QImWidget调用
    void render();
    // 设置渲染属性
    void setRenderOptionFlags(RenderOptionFlags f);
    RenderOptionFlags renderOptionFlags() const;
    void setRenderOption(RenderOption f, bool on);
    bool testRenderOption(RenderOption f) const;
Q_SIGNALS:
    void visibleChanged(bool visible);
    void enabledChanged(bool enabled);
    void childNodeRemoved(QIM::QImAbstractNode* c);
    void childNodeAdded(QIM::QImAbstractNode* c);

protected:
    /**
     * @brief 子类实现具体ImGui渲染逻辑
     *
     * 典型实现模式：
     * @code
     * bool QImPlotSectionNode::beginDraw()
     * {
     *     return ImPlot::BeginPlot(title.toUtf8().constData());
     * }
     *
     * void QImPlotSectionNode::endDraw()
     * {
     *     ImPlot::EndPlot();
     * }
     * @endcode
     *
     * @return true if section is open and children should be rendered
     */
    virtual bool beginDraw() = 0;

    /**
     * @brief 子类实现具体ImGui渲染结束逻辑
     * 典型实现模式：
     * @code
     * bool QImPlotSectionNode::beginDraw()
     * {
     *     return ImPlot::BeginPlot(title.toUtf8().constData());
     * }
     *
     * void QImPlotSectionNode::endDraw()
     * {
     *     ImPlot::EndPlot();
     * }
     * @endcode
     *
     * @return true if section is open and children should be rendered
     */
    virtual void endDraw();


private:
    // 仅移除子列表引用（不改变 QObject 父子关系），供析构函数使用
    void removeFromParentList();
    // 更新 z-order 排序列表
    void updateZOrderedList();

private:
    bool m_visible { true };
    bool m_enabled { true };
    int m_zOrder { 0 };  // z-order值
    QList< QImAbstractNode* > m_children;
    QList< QImAbstractNode* > m_childrenZordered;  // 按 z-order 预排序的子节点列表
    QPointer< QImAbstractNode > m_parent;          // 逻辑父节点（弱引用，避免循环）
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
