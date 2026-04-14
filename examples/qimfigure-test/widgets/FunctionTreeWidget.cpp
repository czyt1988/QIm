#include "FunctionTreeWidget.h"

/**
 * \if ENGLISH
 * @brief Construct a new Function Tree Widget object
 * @param parent Parent widget
 * \endif
 * 
 * \if CHINESE
 * @brief 构造函数树控件对象
 * @param parent 父控件
 * \endif
 */
FunctionTreeWidget::FunctionTreeWidget(QWidget *parent)
    : QTreeWidget(parent)
{
    // Configure tree widget properties
    setHeaderHidden(true);
    setIndentation(20);
    setAnimated(true);
    setExpandsOnDoubleClick(false);
    
    // Connect click signal to handle item selection
    connect(this, &QTreeWidget::itemClicked, this, &FunctionTreeWidget::onItemClicked);
}

/**
 * \if ENGLISH
 * @brief Initialize tree with hardcoded structure
 * 
 * Builds the complete tree hierarchy with categories and test functions.
 * This is a placeholder implementation - later MainWindow will call setMetadata().
 * \endif
 * 
 * \if CHINESE
 * @brief 使用硬编码结构初始化树
 * 
 * 构建包含类别和测试函数的完整树形层次结构。
 * 这是一个占位符实现 - 稍后 MainWindow 将调用 setMetadata()。
 * \endif
 */
void FunctionTreeWidget::initializeTree()
{
    clear();
    buildTree();
    
    // Expand all categories by default
    expandAll();
}

/**
 * \if ENGLISH
 * @brief Get the function ID of currently selected node
 * @return Function ID string, or empty string if no valid selection
 * \endif
 * 
 * \if CHINESE
 * @brief 获取当前选中节点的函数 ID
 * @return 函数 ID 字符串，如果没有有效选择则返回空字符串
 * \endif
 */
QString FunctionTreeWidget::selectedFunctionId() const
{
    QTreeWidgetItem *selectedItem = currentItem();
    if (!selectedItem) {
        return QString();
    }
    
    // Only leaf nodes (function nodes) have functionId
    return selectedItem->data(0, Qt::UserRole).toString();
}

/**
 * \if ENGLISH
 * @brief Build the complete tree hierarchy
 * 
 * Creates the root node and all category/function nodes according to the specification:
 * Root
 * ├── 2D
 * │   ├── Line
 * │   │   ├── 10K Points
 * │   │   └── 1M Points
 * │   ├── Data Points
 * │   │   ├── Scatter
 * │   │   ├── Stairs
 * │   │   └── Bars
 * │   ├── Shaded
 * │   │   └── Shaded Plot
 * │   ├── Error
 * │   │   └── Error Bars
 * │   └── Other
 * │       ├── Stems
 * │       └── Infinite Lines
 * └── 3D
 *     └── (reserved for future)
 * \endif
 * 
 * \if CHINESE
 * @brief 构建完整的树形层次结构
 * 
 * 根据规范创建根节点和所有类别/函数节点：
 * Root
 * ├── 2D
 * │   ├── Line
 * │   │   ├── 10K Points
 * │   │   └── 1M Points
 * │   ├── Data Points
 * │   │   ├── Scatter
 * │   │   ├── Stairs
 * │   │   └── Bars
 * │   ├── Shaded
 * │   │   └── Shaded Plot
 * │   ├── Error
 * │   │   └── Error Bars
 * │   └── Other
 * │       ├── Stems
 * │       └── Infinite Lines
 * └── 3D
 *     └── (reserved for future)
 * \endif
 */
void FunctionTreeWidget::buildTree()
{
    // Create root node (hidden)
    QTreeWidgetItem *root = new QTreeWidgetItem();
    root->setText(0, tr("Root"));
    addTopLevelItem(root);
    
    // === 2D Category ===
    QTreeWidgetItem *category2D = addCategoryNode(root, tr("2D"));
    
    // Line subcategory
    QTreeWidgetItem *lineCategory = addCategoryNode(category2D, tr("Line"));
    addFunctionNode(lineCategory, tr("10K Points"), QStringLiteral("2d_line_10k"));
    addFunctionNode(lineCategory, tr("1M Points"), QStringLiteral("2d_line_1m"));
    
    // Data Points subcategory
    QTreeWidgetItem *dataPointsCategory = addCategoryNode(category2D, tr("Data Points"));
    addFunctionNode(dataPointsCategory, tr("Scatter"), QStringLiteral("2d_datapoints_scatter"));
    addFunctionNode(dataPointsCategory, tr("Stairs"), QStringLiteral("2d_datapoints_stairs"));
    addFunctionNode(dataPointsCategory, tr("Bars"), QStringLiteral("2d_datapoints_bars"));
    addFunctionNode(dataPointsCategory, tr("Bar Groups"), QStringLiteral("2d_datapoints_bargroups"));
    addFunctionNode(dataPointsCategory, tr("Histogram"), QStringLiteral("2d_datapoints_histogram"));
    addFunctionNode(dataPointsCategory, tr("2D Histogram"), QStringLiteral("2d_datapoints_histogram2d"));
    
    // Shaded subcategory
    QTreeWidgetItem *shadedCategory = addCategoryNode(category2D, tr("Shaded"));
    addFunctionNode(shadedCategory, tr("Shaded Plot"), QStringLiteral("2d_shaded_shaded"));
    
    // Error subcategory
    QTreeWidgetItem *errorCategory = addCategoryNode(category2D, tr("Error"));
    addFunctionNode(errorCategory, tr("Error Bars"), QStringLiteral("2d_errorbars"));
    
    // Other subcategory
    QTreeWidgetItem *otherCategory = addCategoryNode(category2D, tr("Other"));
    addFunctionNode(otherCategory, tr("Stems"), QStringLiteral("2d_stems"));
    addFunctionNode(otherCategory, tr("Infinite Lines"), QStringLiteral("2d_inflines"));
    addFunctionNode(otherCategory, tr("Pie Chart"), QStringLiteral("2d_other_piechart"));
    addFunctionNode(otherCategory, tr("Heatmap"), QStringLiteral("2d_other_heatmap"));
    addFunctionNode(otherCategory, tr("Digital"), QStringLiteral("2d_other_digital"));
    addFunctionNode(otherCategory, tr("Image"), QStringLiteral("2d_other_image"));
    addFunctionNode(otherCategory, tr("Text"), QStringLiteral("2d_other_text"));
    addFunctionNode(otherCategory, tr("Dummy"), QStringLiteral("2d_other_dummy"));
    
    // Tools subcategory - Interactive drag tools
    QTreeWidgetItem *toolsCategory = addCategoryNode(category2D, tr("Tools"));
    addFunctionNode(toolsCategory, tr("Drag Point"), QStringLiteral("2d_tools_dragpoint"));
    addFunctionNode(toolsCategory, tr("Drag Lines"), QStringLiteral("2d_tools_draglines"));
    addFunctionNode(toolsCategory, tr("Drag Rect"), QStringLiteral("2d_tools_dragrect"));
    addFunctionNode(toolsCategory, tr("Annotation"), QStringLiteral("2d_tools_annotation"));
    addFunctionNode(toolsCategory, tr("Tags"), QStringLiteral("2d_tools_tags"));
    
    // Layout subcategory - Subplots
    QTreeWidgetItem *layoutCategory = addCategoryNode(category2D, tr("Layout"));
    addFunctionNode(layoutCategory, tr("Subplots"), QStringLiteral("2d_layout_subplots"));
    
    // === 3D Category (reserved for future) ===
    QTreeWidgetItem *category3D = addCategoryNode(root, tr("3D"));
    QTreeWidgetItem *reservedNode = new QTreeWidgetItem();
    reservedNode->setText(0, tr("(reserved for future)"));
    reservedNode->setFlags(reservedNode->flags() & ~Qt::ItemIsSelectable);
    category3D->addChild(reservedNode);
}

/**
 * \if ENGLISH
 * @brief Add a category node (expandable parent node)
 * @param parent Parent node to add this category under
 * @param name Display name for the category
 * @return Pointer to the created category node
 * \endif
 * 
 * \if CHINESE
 * @brief 添加类别节点（可展开的父节点）
 * @param parent 要添加此类别的父节点
 * @param name 类别的显示名称
 * @return 指向创建的类别节点的指针
 * \endif
 */
QTreeWidgetItem* FunctionTreeWidget::addCategoryNode(QTreeWidgetItem *parent, const QString &name)
{
    QTreeWidgetItem *categoryNode = new QTreeWidgetItem();
    categoryNode->setText(0, name);
    
    // Set flags: selectable but primarily for expansion
    categoryNode->setFlags(categoryNode->flags() | Qt::ItemIsEditable);
    
    parent->addChild(categoryNode);
    return categoryNode;
}

/**
 * \if ENGLISH
 * @brief Add a function node (leaf node with functionId stored)
 * @param parent Parent node to add this function under
 * @param name Display name for the function
 * @param functionId Unique identifier for the function (stored in UserRole)
 * @return Pointer to the created function node
 * \endif
 * 
 * \if CHINESE
 * @brief 添加函数节点（叶节点，存储 functionId）
 * @param parent 要添加此函数的父节点
 * @param name 函数的显示名称
 * @param functionId 函数的唯一标识符（存储在 UserRole 中）
 * @return 指向创建的函数节点的指针
 * \endif
 */
QTreeWidgetItem* FunctionTreeWidget::addFunctionNode(QTreeWidgetItem *parent, const QString &name, const QString &functionId)
{
    QTreeWidgetItem *functionNode = new QTreeWidgetItem();
    functionNode->setText(0, name);
    
    // Store functionId in UserRole for retrieval
    functionNode->setData(0, Qt::UserRole, functionId);
    
    // Leaf node flags: selectable but not editable
    functionNode->setFlags(functionNode->flags() & ~Qt::ItemIsEditable);
    
    parent->addChild(functionNode);
    return functionNode;
}

/**
 * \if ENGLISH
 * @brief Handle item click to emit signal for leaf nodes
 * @param item The clicked tree widget item
 * @param column The column that was clicked
 * 
 * Emits functionSelected() signal only if the clicked item is a leaf node
 * (i.e., has a valid functionId stored in UserRole).
 * \endif
 * 
 * \if CHINESE
 * @brief 处理项目点击，为叶节点发射信号
 * @param item 被点击的树控件项目
 * @param column 被点击的列
 * 
 * 仅当点击的项目是叶节点（即在 UserRole 中存储了有效的 functionId）时，
 * 才会发射 functionSelected() 信号。
 * \endif
 */
void FunctionTreeWidget::onItemClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    
    // Check if this is a leaf node (function node) with a valid functionId
    QString functionId = item->data(0, Qt::UserRole).toString();
    
    if (!functionId.isEmpty()) {
        emit functionSelected(functionId);
    }
    // Category nodes (empty functionId) do not emit signal
}
