#ifndef QIMGRIDNODE_H
#define QIMGRIDNODE_H

#include "QImAbstractNode.h"
#include <QPoint>
#include <QPointF>
#include <QSizeF>
#include <QVariant>
#include "imgui.h"

namespace QIM
{

/**
 * @brief 通用网格布局节点，支持在统一网格中混合放置任意类型的子图节点
 * @brief Generic grid layout node that enables mixing any plot node types in a uniform grid
 *
 * 设计理念：\if ENGLISH
 * QImGridNode provides a generalized grid layout that can host both 2D (QImPlotNode)
 * and 3D (QImPlot3DNode) plots in the same grid. Unlike QImSubplots3DNode which is
 * specific to 3D plots, QImGridNode uses Qt's property system for cell size notification,
 * making it type-agnostic. Both QImPlotNode and QImPlot3DNode expose Q_PROPERTY(QSizeF size)
 * and Q_PROPERTY(bool autoSize), so property-based notification works seamlessly.
 * \endif \if CHINESE
 * QImGridNode 提供一个通用网格布局，可在同一网格中混合放置 2D（QImPlotNode）和
 * 3D（QImPlot3DNode）子图。与专用于 3D 子图的 QImSubplots3DNode 不同，QImGridNode
 * 使用 Qt 属性系统进行单元格尺寸通知，因此与具体类型无关。QImPlotNode 和 QImPlot3DNode
 * 均暴露了 Q_PROPERTY(QSizeF size) 和 Q_PROPERTY(bool autoSize)，因此基于属性的通知
 * 可以无缝工作。
 * \endif
 *
 * 使用示例：\if CHINESE
 * @code
 * auto* grid = new QImGridNode("My Grid");
 * grid->setGrid(2, 2);  // 2x2 layout
 * auto* cell = grid->createCellNode();
 * auto* plot2D = new QImPlotNode(cell);    // 2D plot
 * auto* cell2 = grid->createCellNode();
 * auto* plot3D = new QImPlot3DNode(cell2); // 3D plot
 * @endcode
 * \endif \if ENGLISH
 * @code
 * auto* grid = new QImGridNode("My Grid");
 * grid->setGrid(2, 2);  // 2x2 layout
 * auto* cell = grid->createCellNode();
 * auto* plot2D = new QImPlotNode(cell);    // 2D plot
 * auto* cell2 = grid->createCellNode();
 * auto* plot3D = new QImPlot3DNode(cell2); // 3D plot
 * @endcode
 * \endif
 *
 * @see QImSubplots3DNode
 */
class QIM_CORE_API QImGridNode : public QImAbstractNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImGridNode)

    // Title of the grid
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    // Number of rows in the grid
    Q_PROPERTY(int rows READ rows WRITE setRows NOTIFY gridInfoChanged)
    // Number of columns in the grid
    Q_PROPERTY(int cols READ columns WRITE setColumns NOTIFY gridInfoChanged)
    // Size of the grid area (-1 means auto-fill available space)
    Q_PROPERTY(QSizeF size READ size WRITE setSize NOTIFY sizeChanged)

    Q_DISABLE_COPY(QImGridNode)

public:
    // ===============================================================
    // CellNode — private nested class that wraps a grid cell
    // ===============================================================
    class QIM_CORE_API CellNode : public QImAbstractNode
    {
        // No Q_OBJECT needed — no independent signals/slots

    public:
        explicit CellNode(int subplotIndex, QObject* parent = nullptr)
            : QImAbstractNode(parent), m_subplotIndex(subplotIndex)
        {
            setAutoIdEnabled(false);
        }

        bool beginDraw() override;
        void endDraw() override;

    private:
        int m_subplotIndex{0};
    };

    // Constructor with optional parent
    explicit QImGridNode(QObject* parent = nullptr);
    // Constructor with title and optional parent
    explicit QImGridNode(const QString& title, QObject* parent = nullptr);
    // Destructor
    ~QImGridNode() override;

    // Gets the grid title
    QString title() const;
    // Sets the grid title
    void setTitle(const QString& title);

    // Gets the number of rows
    int rows() const;
    // Sets the number of rows
    void setRows(int rows);

    // Gets the number of columns
    int columns() const;
    // Sets the number of columns
    void setColumns(int columns);

    // Sets grid dimensions in one call
    void setGrid(int rows, int cols);

    // Gets the grid area size
    QSizeF size() const;
    // Sets the grid area size
    void setSize(const QSizeF& size);

    // Gets total number of cells
    int gridCount() const;

    // Creates a new cell node at next available position
    QImGridNode::CellNode* createCellNode();

    // Gets all cell nodes
    QList<QImGridNode::CellNode*> cellNodes() const;

    // Gets the number of populated cells
    int cellCount() const;

Q_SIGNALS:
    /**
     * @brief Emitted when the grid title changes
     * @brief 当网格标题改变时发出
     * @param title The new title
     */
    void titleChanged(const QString& title);

    /**
     * \if ENGLISH
     * @brief Emitted when grid rows or columns change
     * @details Triggered when the number of rows or columns in the grid layout changes.
     * \endif
     *
     * \if CHINESE
     * @brief 当网格行数或列数改变时发射
     * @details 当网格布局中的行数或列数发生变更时触发。
     * \endif
     */
    void gridInfoChanged();

    /**
     * @brief Emitted when the grid size changes
     * @brief 当网格尺寸改变时发出
     * @param size The new size
     */
    void sizeChanged(const QSizeF& size);

protected:
    // Begins grid rendering - captures layout origin and available size
    bool beginDraw() override;
    // Ends grid rendering - restores cursor and occupies grid area
    void endDraw() override;

    // Calculates pixel position for a cell by index
    QPoint cellPosition(int index) const;
    // Calculates the size of each uniform cell
    QSizeF cellSize() const;

};

}  // namespace QIM

#endif  // QIMGRIDNODE_H
