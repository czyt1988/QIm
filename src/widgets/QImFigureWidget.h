#ifndef QIMFIGUREWIDGET_H
#define QIMFIGUREWIDGET_H
#include "QImWidget.h"
#include <QWidget>
#include "QImPlotTheme.h"
#include <vector>
namespace QIM
{
class QImSubplotsNode;
class QImSubplots3DNode;
class QImPlotNode;
class QImPlot3DNode;
/**
 * \if ENGLISH
 * @brief Figure Widget for plot
 *
 * QImFigureWidget manages plot rendering with optional subplot layout.
 *
 * By default, single plots are rendered as root-level nodes (no subplot wrapper),
 * which allows them to fill the entire window naturally without ImPlot::BeginSubplots nesting.
 *
 * When setSubplotGrid() is called with rows*cols > 1, a QImSubplotsNode is created
 * to manage the grid layout. All subsequent createPlotNode() calls will place plots
 * inside the subplot grid.
 *
 * You can also call addRenderNode() to add any render node (including QImPlot3DNode)
 * as a root-level node alongside or instead of subplot-based plots.
 * \endif
 *
 * \if CHINESE
 * @brief 绘图图窗控件
 *
 * QImFigureWidget 管理绘图渲染，可选子图布局。
 *
 * 默认情况下，单个绘图作为根级节点渲染（无子图包装），使其自然填充整个窗口，
 * 而无需 ImPlot::BeginSubplots 嵌套。
 *
 * 当调用 setSubplotGrid() 且 rows*cols > 1 时，会创建 QImSubplotsNode 来管理网格布局，
 * 此后所有 createPlotNode() 调用都会将绘图放入子图网格中。
 *
 * 也可以调用 addRenderNode() 将任意渲染节点（包括 QImPlot3DNode）
 * 作为根级节点添加，与子图绘图并行或替代使用。
 * \endif
 */
class QIM_WIDGETS_API QImFigureWidget : public QImWidget
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImFigureWidget)
public:
    QImFigureWidget(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~QImFigureWidget();
    // ===========================
    //  theme
    // ===========================
    // Set/get plot theme
    void setPlotTheme(const QImPlotTheme& t);
    const QImPlotTheme& plotTheme() const;
    // ===========================
    //  subplot
    // ===========================
    void setSubplotGrid(
        int rows,
        int cols,
        const std::vector< float >& rowsRatios = std::vector< float >(),
        const std::vector< float >& colsRatios = std::vector< float >()
    );
    int subplotGridRows() const;
    int subplotGridColumns() const;
    std::vector< float > subplotGridRowRatios() const;
    std::vector< float > subplotGridColumnRatios() const;
    // Remove subplot grid entirely, return to single-plot mode
    // This destroys the QImSubplotsNode and all its child plot nodes
    void clearSubplotGrid();
    QImSubplotsNode* subplotNode() const;
    // ===========================
    //  plot
    // ===========================
    QImPlotNode* createPlotNode();
    QList< QImPlotNode* > plotNodes() const;
    int plotCount() const;
    void addPlotNode(QImPlotNode* plot);
    void insertPlotNode(int plotIndex, QImPlotNode* plot);
    int plotNodeSubplotIndex(QImPlotNode* plot);
    bool takePlotNode(QImPlotNode* plot);
    void removePlotNode(QImPlotNode* plot);
    // ===========================
    //  plot3D
    // ===========================
    // 3D subplot grid (using QImSubplots3DNode for manual cell layout)
    // ImPlot3D has no built-in subplot API, so we use CellNode for positioning
    void setSubplot3DGrid(int rows, int cols);
    QImSubplots3DNode* subplot3DNode() const;
    void clearSubplot3DGrid();
    // Create a 3D plot node inside the 3D subplot grid
    QImPlot3DNode* createPlot3DNode();
    // Get all 3D plot nodes from the 3D subplot grid
    QList< QImPlot3DNode* > plot3DNodes() const;
    int plot3DCount() const;
// Single 3D plot as top-level render node (fills entire window)
    // (Note: for subplot grid, use QImSubplots3DNode directly via addRenderNode())
Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when a plot node is attached or detached from the figure
     * @param[in] plot The plot node being attached or detached
     * @param[in] attach True if the node is being attached, false if being detached
     * @details Triggered when a QImPlotNode is added to or removed from the figure's plot management.
     * \endif
     *
     * \if CHINESE
     * @brief 绘图节点挂载或卸载时触发
     * @param[in] plot 正在挂载或卸载的绘图节点
     * @param[in] attach true表示节点正在挂载，false表示正在卸载
     * @details 当QImPlotNode被添加到或从绘图管理中移除时触发。
     * \endif
     */
    void plotNodeAttached(QIM::QImPlotNode* plot, bool attach);

protected:
    void initializeGL() override;
    void beforeRenderImNodes() override;

private:
    QImSubplotsNode* ensureSubplotNode();
    QImSubplots3DNode* ensureSubplot3DNode();
private Q_SLOTS:
    void onSubplotChildNodeRemoved(QIM::QImAbstractNode* c);
    void onSubplotChildNodeAdded(QIM::QImAbstractNode* c);
};
}
#endif  // QIMFIGUREWIDGET_H
