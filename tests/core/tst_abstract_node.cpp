#include <QObject>
#include <QtTest>

#include "QImAbstractNode.h"

using namespace QIM;

// Concrete test subclass for testing QImAbstractNode non-virtual API
class TestNode : public QImAbstractNode
{
public:
    explicit TestNode(QObject* parent = nullptr)
        : QImAbstractNode(parent)
    {
    }

    // Track how many times beginDraw/endDraw are called
    int beginDrawCount{0};
    int endDrawCount{0};
    bool beginDrawReturnValue{true};

    bool beginDraw() override
    {
        ++beginDrawCount;
        return beginDrawReturnValue;
    }

    void endDraw() override
    {
        ++endDrawCount;
    }
};

class TestAbstractNode : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    // Default construction: visible=true, enabled=true
    void testDefaultConstruction();
    // Node ID uniqueness
    void testNodeIdUniqueness();
    // Parent-child: addChildNode
    void testAddChildNode();
    // Parent-child: removeChildNode
    void testRemoveChildNode();
    // Parent-child: takeChildNode
    void testTakeChildNode();
    // parentNode returns logical parent
    void testParentNode();
    // Auto parent-child when constructed with QImAbstractNode* parent
    void testAutoParentChild();
    // z-order getter/setter
    void testZOrder();
    // z-order affects childrenZOrdered
    void testZOrderedList();
    // visible property
    void testVisibleProperty();
    // visibleChanged signal
    void testVisibleChangedSignal();
    // enabled property
    void testEnabledProperty();
    // enabledChanged signal
    void testEnabledChangedSignal();
    // childNodeCount and childNodeAt
    void testChildCountAndAt();
    // childNodeRemoved signal on removeChildNode
    void testChildNodeRemovedSignal();
    // childNodeAdded signal on addChildNode
    void testChildNodeAddedSignal();
    // clearChildrenNodes
    void testClearChildrenNodes();
    // insertChildNode at specific index
    void testInsertChildNode();
    // indexOfChildNode
    void testIndexOfChildNode();
    // auto-ID enabled/disabled
    void testAutoIdEnabled();
    // render option flags
    void testRenderOptionFlags();
};

void TestAbstractNode::initTestCase()
{
}

void TestAbstractNode::cleanupTestCase()
{
}

// ============================================================================
// Test 1: Default construction
// ============================================================================
void TestAbstractNode::testDefaultConstruction()
{
    TestNode node;
    QVERIFY(node.isVisible());
    QVERIFY(node.isEnabled());
    QCOMPARE(node.zOrder(), 0);
    QCOMPARE(node.childNodeCount(), 0);
    QVERIFY(node.parentNode() == nullptr);
}

// ============================================================================
// Test 2: Each node gets a unique ID via pointer address
// ============================================================================
void TestAbstractNode::testNodeIdUniqueness()
{
    TestNode a;
    TestNode b;
    // In QImAbstractNode, ImGui::PushID(this) is used, so each node
    // has a unique void* ID. Verify that two nodes have different addresses.
    QVERIFY(&a != &b);
}

// ============================================================================
// Test 3: addChildNode adds to parent's child list
// ============================================================================
void TestAbstractNode::testAddChildNode()
{
    TestNode parent;
    auto* child = new TestNode();

    parent.addChildNode(child);

    QCOMPARE(parent.childNodeCount(), 1);
    QCOMPARE(parent.childNodeAt(0), child);
    QVERIFY(parent.childrenNodes().contains(child));
    QCOMPARE(parent.indexOfChildNode(child), 0);
}

// ============================================================================
// Test 4: removeChildNode schedules child for deletion
// ============================================================================
void TestAbstractNode::testRemoveChildNode()
{
    TestNode parent;
    auto* child = new TestNode();
    parent.addChildNode(child);

    QSignalSpy removedSpy(&parent, &QImAbstractNode::childNodeRemoved);

    parent.removeChildNode(child);

    QCOMPARE(removedSpy.count(), 1);
    QCOMPARE(removedSpy.at(0).at(0).value<QImAbstractNode*>(), child);
    QCOMPARE(parent.childNodeCount(), 0);
    QVERIFY(!parent.childrenNodes().contains(child));
}

// ============================================================================
// Test 5: takeChildNode removes child without deletion
// ============================================================================
void TestAbstractNode::testTakeChildNode()
{
    TestNode parent;
    auto* child = new TestNode();
    parent.addChildNode(child);

    QSignalSpy removedSpy(&parent, &QImAbstractNode::childNodeRemoved);

    bool result = parent.takeChildNode(child);
    QVERIFY(result);
    QCOMPARE(removedSpy.count(), 1);
    QCOMPARE(parent.childNodeCount(), 0);
    QVERIFY(child->parentNode() == nullptr);

    // takeChildNode returns false for non-child
    TestNode orphan;
    bool fail = parent.takeChildNode(&orphan);
    QVERIFY(!fail);

    delete child;
}

// ============================================================================
// Test 6: parentNode returns correct logical parent
// ============================================================================
void TestAbstractNode::testParentNode()
{
    TestNode parent;
    auto* child = new TestNode();

    parent.addChildNode(child);
    QCOMPARE(child->parentNode(), &parent);

    parent.takeChildNode(child);
    QVERIFY(child->parentNode() == nullptr);

    delete child;
}

// ============================================================================
// Test 7: Constructor with QImAbstractNode* parent auto-establishes relationship
// ============================================================================
void TestAbstractNode::testAutoParentChild()
{
    TestNode parent;
    auto* child = new TestNode(&parent);  // QObject parent is QImAbstractNode

    QCOMPARE(parent.childNodeCount(), 1);
    QCOMPARE(child->parentNode(), &parent);
    QVERIFY(parent.childrenNodes().contains(child));
}

// ============================================================================
// Test 8: z-order getter and setter
// ============================================================================
void TestAbstractNode::testZOrder()
{
    TestNode parent;
    auto* child1 = new TestNode(&parent);
    auto* child2 = new TestNode(&parent);

    QCOMPARE(child1->zOrder(), 0);
    QCOMPARE(child2->zOrder(), 0);

    child1->setZOrder(10);
    QCOMPARE(child1->zOrder(), 10);

    child2->setZOrder(5);
    QCOMPARE(child2->zOrder(), 5);

    // Set same value — no change
    child1->setZOrder(10);
    QCOMPARE(child1->zOrder(), 10);
}

// ============================================================================
// Test 9: z-order sorted list returns children in correct order
// ============================================================================
void TestAbstractNode::testZOrderedList()
{
    TestNode parent;
    auto* child1 = new TestNode(&parent);
    auto* child2 = new TestNode(&parent);
    auto* child3 = new TestNode(&parent);

    // Same z-order → insertion order
    const auto& zOrdered = parent.childrenNodesZOrdered();
    QCOMPARE(zOrdered.size(), 3);
    QCOMPARE(zOrdered.at(0), child1);  // First inserted
    QCOMPARE(zOrdered.at(1), child2);  // Second
    QCOMPARE(zOrdered.at(2), child3);  // Third

    // Change z-order: child3 should appear first (z=-1)
    child3->setZOrder(-1);
    const auto& zOrdered2 = parent.childrenNodesZOrdered();
    QCOMPARE(zOrdered2.size(), 3);
    QCOMPARE(zOrdered2.at(0), child3);  // Lowest z
    QCOMPARE(zOrdered2.at(1), child1);  // Same z, first inserted
    QCOMPARE(zOrdered2.at(2), child2);  // Same z, second inserted

    // Change z-order: child1 should come after child2 (child1 z=0, child2 z=-0.5)
    child1->setZOrder(100);
    child2->setZOrder(50);
    const auto& zOrdered3 = parent.childrenNodesZOrdered();
    QCOMPARE(zOrdered3.at(0), child3);  // z=-1
    QCOMPARE(zOrdered3.at(1), child2);  // z=50
    QCOMPARE(zOrdered3.at(2), child1);  // z=100
}

// ============================================================================
// Test 10: visible property getter/setter
// ============================================================================
void TestAbstractNode::testVisibleProperty()
{
    TestNode node;
    QVERIFY(node.isVisible());

    node.setVisible(false);
    QVERIFY(!node.isVisible());

    node.setVisible(true);
    QVERIFY(node.isVisible());

    // Setting same value doesn't change state
    node.setVisible(true);
    QVERIFY(node.isVisible());
}

// ============================================================================
// Test 11: visibleChanged signal
// ============================================================================
void TestAbstractNode::testVisibleChangedSignal()
{
    TestNode node;
    QSignalSpy spy(&node, &QImAbstractNode::visibleChanged);

    node.setVisible(false);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toBool(), false);

    // Setting same value does NOT emit signal
    node.setVisible(false);
    QCOMPARE(spy.count(), 1);

    node.setVisible(true);
    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.at(1).at(0).toBool(), true);
}

// ============================================================================
// Test 12: enabled property getter/setter
// ============================================================================
void TestAbstractNode::testEnabledProperty()
{
    TestNode node;
    QVERIFY(node.isEnabled());

    node.setEnabled(false);
    QVERIFY(!node.isEnabled());

    node.setEnabled(true);
    QVERIFY(node.isEnabled());
}

// ============================================================================
// Test 13: enabledChanged signal
// ============================================================================
void TestAbstractNode::testEnabledChangedSignal()
{
    TestNode node;
    QSignalSpy spy(&node, &QImAbstractNode::enabledChanged);

    node.setEnabled(false);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toBool(), false);

    node.setEnabled(false);  // Same value — no signal
    QCOMPARE(spy.count(), 1);

    node.setEnabled(true);
    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.at(1).at(0).toBool(), true);
}

// ============================================================================
// Test 14: childCount and childNodeAt
// ============================================================================
void TestAbstractNode::testChildCountAndAt()
{
    TestNode parent;
    QCOMPARE(parent.childNodeCount(), 0);
    QVERIFY(parent.childNodeAt(0) == nullptr);
    QVERIFY(parent.childNodeAt(-1) == nullptr);

    auto* child = new TestNode(&parent);
    QCOMPARE(parent.childNodeCount(), 1);
    QCOMPARE(parent.childNodeAt(0), child);
    QVERIFY(parent.childNodeAt(1) == nullptr);
    QVERIFY(parent.childNodeAt(-1) == nullptr);
}

// ============================================================================
// Test 15: childNodeRemoved signal on removeChildNode
// ============================================================================
void TestAbstractNode::testChildNodeRemovedSignal()
{
    TestNode parent;
    auto* child = new TestNode(&parent);

    QSignalSpy spy(&parent, &QImAbstractNode::childNodeRemoved);

    // removeChildNode should emit childNodeRemoved exactly once
    parent.removeChildNode(child);
    QCOMPARE(spy.count(), 1);
}

// ============================================================================
// Test 16: childNodeAdded signal on addChildNode
// ============================================================================
void TestAbstractNode::testChildNodeAddedSignal()
{
    TestNode parent;
    QSignalSpy spy(&parent, &QImAbstractNode::childNodeAdded);

    auto* child = new TestNode();
    QCOMPARE(spy.count(), 0);  // Constructor should NOT emit

    parent.addChildNode(child);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).value<QImAbstractNode*>(), child);
}

// ============================================================================
// Test 17: clearChildrenNodes removes all children
// ============================================================================
void TestAbstractNode::testClearChildrenNodes()
{
    TestNode parent;
    auto* c1 = new TestNode(&parent);
    auto* c2 = new TestNode(&parent);
    auto* c3 = new TestNode(&parent);
    QCOMPARE(parent.childNodeCount(), 3);

    QSignalSpy spy(&parent, &QImAbstractNode::childNodeRemoved);

    parent.clearChildrenNodes();

    // Each remove emits childNodeRemoved
    QCOMPARE(spy.count(), 3);
    QCOMPARE(parent.childNodeCount(), 0);
    QCOMPARE(parent.childrenNodesZOrdered().size(), 0);
}

// ============================================================================
// Test 18: insertChildNode at specific index
// ============================================================================
void TestAbstractNode::testInsertChildNode()
{
    TestNode parent;
    auto* c1 = new TestNode(&parent);
    auto* c2 = new TestNode();

    // Insert at front
    parent.insertChildNode(0, c2);

    QCOMPARE(parent.childNodeCount(), 2);
    QCOMPARE(parent.childNodeAt(0), c2);
    QCOMPARE(parent.childNodeAt(1), c1);

    // Insert at end (same as addChildNode)
    auto* c3 = new TestNode();
    parent.insertChildNode(999, c3);  // Out of bounds → append
    QCOMPARE(parent.childNodeCount(), 3);
    QCOMPARE(parent.childNodeAt(2), c3);

    // Negative index → insert at 0
    auto* c4 = new TestNode();
    parent.insertChildNode(-5, c4);
    QCOMPARE(parent.childNodeAt(0), c4);
}

// ============================================================================
// Test 19: indexOfChildNode
// ============================================================================
void TestAbstractNode::testIndexOfChildNode()
{
    TestNode parent;
    auto* c1 = new TestNode(&parent);
    auto* c2 = new TestNode(&parent);

    QCOMPARE(parent.indexOfChildNode(c1), 0);
    QCOMPARE(parent.indexOfChildNode(c2), 1);

    TestNode orphan;
    QCOMPARE(parent.indexOfChildNode(&orphan), -1);  // Not found
}

// ============================================================================
// Test 20: auto-ID enabled/disabled
// ============================================================================
void TestAbstractNode::testAutoIdEnabled()
{
    TestNode node;
    QVERIFY(node.isAutoIdEnabled());  // Default is enabled

    node.setAutoIdEnabled(false);
    QVERIFY(!node.isAutoIdEnabled());

    node.setAutoIdEnabled(true);
    QVERIFY(node.isAutoIdEnabled());
}

// ============================================================================
// Test 21: render option flags
// ============================================================================
void TestAbstractNode::testRenderOptionFlags()
{
    TestNode node;

    // Default: no flags
    QCOMPARE(node.renderOptionFlags(), QImAbstractNode::RenderOptionFlags());

    node.setRenderOption(QImAbstractNode::RenderIgnoreVisible, true);
    QVERIFY(node.testRenderOption(QImAbstractNode::RenderIgnoreVisible));
    QVERIFY(!node.testRenderOption(QImAbstractNode::RenderNotAutoID));

    node.setRenderOption(QImAbstractNode::RenderIgnoreVisible, false);
    QVERIFY(!node.testRenderOption(QImAbstractNode::RenderIgnoreVisible));

    // setRenderOptionFlags replaces all
    node.setRenderOptionFlags(QImAbstractNode::RenderNotAutoID);
    QVERIFY(node.testRenderOption(QImAbstractNode::RenderNotAutoID));
    QVERIFY(!node.testRenderOption(QImAbstractNode::RenderIgnoreVisible));
}

QTEST_MAIN(TestAbstractNode)
#include "tst_abstract_node.moc"
