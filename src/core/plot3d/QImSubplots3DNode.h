#ifndef QIMSUBPLOTS3DNODE_H
#define QIMSUBPLOTS3DNODE_H

#include "QImGridNode.h"

namespace QIM
{
class QImPlot3DNode;

/**
 * \if ENGLISH
 * @brief 3D plot grid node — specialized QImGridNode for ImPlot3D
 * @details Inherits QImGridNode's generic grid layout and adds 3D-specific
 *          factory method createPlotNode() that creates QImPlot3DNode instances.
 *          All grid layout properties (title, rows, cols, size) and signals
 *          are inherited from QImGridNode.
 * \endif
 *
 * \if CHINESE
 * @brief 3D绘图网格节点 — 专用于 ImPlot3D 的 QImGridNode 子类
 * @details 继承 QImGridNode 的通用网格布局，并添加 3D 专用的工厂方法
 *          createPlotNode()，用于创建 QImPlot3DNode 实例。
 *          所有网格布局属性（标题、行数、列数、尺寸）和信号均从 QImGridNode 继承。
 * \endif
 */
class QIM_CORE_API QImSubplots3DNode : public QImGridNode
{
    Q_OBJECT
    // QIM_DECLARE_PRIVATE — inherited from QImGridNode
    // Q_PROPERTY — all inherited from QImGridNode
    // Q_DISABLE_COPY — inherited from QImGridNode

public:
    explicit QImSubplots3DNode(QObject* parent = nullptr);
    explicit QImSubplots3DNode(const QString& title, QObject* parent = nullptr);
    ~QImSubplots3DNode() override;

    // Creates a new QImPlot3DNode in a grid cell
    QImPlot3DNode* createPlotNode();

    // Gets all 3D plot nodes in the grid
    QList<QImPlot3DNode*> plotNodes() const;

    // Gets the number of 3D plot nodes
    int plotCount() const;

    // NOTE: All grid methods are inherited from QImGridNode:
    // title/setTitle, rows/setRows, columns/setColumns, setGrid,
    // size/setSize, gridCount, createCellNode, cellNodes, cellCount,
    // cellPosition, cellSize, beginDraw, endDraw
    // All signals inherited: titleChanged, gridInfoChanged, sizeChanged
};

}  // namespace QIM

#endif  // QIMSUBPLOTS3DNODE_H
