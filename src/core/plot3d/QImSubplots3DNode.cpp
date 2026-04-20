#include "QImSubplots3DNode.h"
#include "QImPlot3DNode.h"

namespace QIM
{

// ===============================================================
// QImSubplots3DNode Implementation
// ===============================================================

/**
 * \if ENGLISH
 * @brief Constructor with optional parent
 * @param parent Parent QObject
 * \endif
 * \if CHINESE
 * @brief 构造函数（带可选父对象）
 * @param parent 父对象
 * \endif
 */
QImSubplots3DNode::QImSubplots3DNode(QObject* parent)
    : QImGridNode(parent)
{
    setTitle(QStringLiteral("##Subplots3D"));
}

/**
 * \if ENGLISH
 * @brief Constructor with title and optional parent
 * @param title 3D subplot grid title
 * @param parent Parent QObject
 * \endif
 * \if CHINESE
 * @brief 构造函数（带标题和可选父对象）
 * @param title 3D子图网格标题
 * @param parent 父对象
 * \endif
 */
QImSubplots3DNode::QImSubplots3DNode(const QString& title, QObject* parent)
    : QImGridNode(title, parent)
{
}

/**
 * \if ENGLISH
 * @brief Destructor
 * \endif
 * \if CHINESE
 * @brief 析构函数
 * \endif
 */
QImSubplots3DNode::~QImSubplots3DNode()
{
}

/**
 * \if ENGLISH
 * @brief Creates a new QImPlot3DNode in the next available grid cell
 * @return Pointer to the new QImPlot3DNode, or nullptr if grid is full
 * \endif
 * \if CHINESE
 * @brief 在下一个可用网格单元格中创建新的 QImPlot3DNode
 * @return 指向新 QImPlot3DNode 的指针，如果网格已满则返回 nullptr
 * \endif
 */
QImPlot3DNode* QImSubplots3DNode::createPlotNode()
{
    if (plotCount() >= gridCount()) {
        return nullptr;
    }
    QImGridNode::CellNode* cell = createCellNode();
    QImPlot3DNode* plot = new QImPlot3DNode(cell);
    return plot;
}

/**
 * \if ENGLISH
 * @brief Gets all QImPlot3DNode instances in the grid, traversing cell children
 * @return List of QImPlot3DNode pointers
 * \endif
 * \if CHINESE
 * @brief 获取网格中的所有 QImPlot3DNode 实例，遍历单元格子节点
 * @return QImPlot3DNode 指针列表
 * \endif
 */
QList<QImPlot3DNode*> QImSubplots3DNode::plotNodes() const
{
    QList<QImPlot3DNode*> plots;
    const QList<QImAbstractNode*>& cells = childrenNodes();
    for (QImAbstractNode* child : cells) {
        if (!child) {
            continue;
        }
        const QList<QImAbstractNode*>& cellChildren = child->childrenNodes();
        for (QImAbstractNode* cellChild : cellChildren) {
            if (QImPlot3DNode* plot = qobject_cast<QImPlot3DNode*>(cellChild)) {
                plots.push_back(plot);
            }
        }
    }
    return plots;
}

/**
 * \if ENGLISH
 * @brief Gets the number of 3D plot nodes in the grid
 * @return Count of QImPlot3DNode instances
 * \endif
 * \if CHINESE
 * @brief 获取网格中 3D 子图节点的数量
 * @return QImPlot3DNode 实例的数量
 * \endif
 */
int QImSubplots3DNode::plotCount() const
{
    return plotNodes().size();
}

}  // namespace QIM
