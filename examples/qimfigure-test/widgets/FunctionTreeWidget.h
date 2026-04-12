#ifndef FUNCTIONTREEWIDGET_H
#define FUNCTIONTREEWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QString>

/**
 * \if ENGLISH
 * @brief Tree navigation widget for test functions
 * 
 * Provides a hierarchical tree view for organizing and selecting test functions.
 * Functions are grouped by categories (2D, 3D, etc.) with expandable nodes.
 * Clicking a leaf node emits functionSelected() signal with the function ID.
 * 
 * @section usage Usage Example
 * @code
 * FunctionTreeWidget *treeWidget = new FunctionTreeWidget(parent);
 * treeWidget->initializeTree();
 * connect(treeWidget, &FunctionTreeWidget::functionSelected,
 *         this, &MainWindow::onFunctionSelected);
 * @endcode
 * 
 * @see QTreeWidget, QTreeWidgetItem
 * \endif
 * 
 * \if CHINESE
 * @brief 测试函数的树形导航控件
 * 
 * 提供层次化的树形视图用于组织与选择测试函数。
 * 函数按类别（2D、3D 等）分组，节点可展开。
 * 点击叶节点时发射 functionSelected() 信号并附带函数 ID。
 * 
 * @section usage 使用示例
 * @code
 * FunctionTreeWidget *treeWidget = new FunctionTreeWidget(parent);
 * treeWidget->initializeTree();
 * connect(treeWidget, &FunctionTreeWidget::functionSelected,
 *         this, &MainWindow::onFunctionSelected);
 * @endcode
 * 
 * @see QTreeWidget, QTreeWidgetItem
 * \endif
 */
class FunctionTreeWidget : public QTreeWidget
{
    Q_OBJECT

public:
    // Constructor for FunctionTreeWidget (English only)
    explicit FunctionTreeWidget(QWidget *parent = nullptr);

    // Destructor for FunctionTreeWidget (English only)
    ~FunctionTreeWidget() override = default;

    // Initialize tree with hardcoded structure (English only)
    void initializeTree();

    // Get the function ID of currently selected node (English only)
    QString selectedFunctionId() const;

Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when a function node is selected
     * @param functionId The ID of the selected function
     * \endif
     * 
     * \if CHINESE
     * @brief 选择函数节点时触发
     * @param functionId 选中函数的 ID
     * \endif
     */
    void functionSelected(const QString &functionId);

protected Q_SLOTS:
    // Handle item click to emit signal for leaf nodes (English only)
    void onItemClicked(QTreeWidgetItem *item, int column);

private:
    // Build the complete tree hierarchy (English only)
    void buildTree();

    // Add a category node (expandable parent node) (English only)
    QTreeWidgetItem* addCategoryNode(QTreeWidgetItem *parent, const QString &name);

    // Add a function node (leaf node with functionId stored) (English only)
    QTreeWidgetItem* addFunctionNode(QTreeWidgetItem *parent, const QString &name, const QString &functionId);
};

#endif // FUNCTIONTREEWIDGET_H
