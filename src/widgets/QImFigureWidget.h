#ifndef QIMFIGUREWIDGET_H
#define QIMFIGUREWIDGET_H
#include "QImWidget.h"
#include <QWidget>
#include "QImPlotTheme.h"
#include <vector>
namespace QIM
{
class QImSubplotsNode;
class QImPlotNode;
/**
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
    // 获取绘图的样式
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
    // 创建一个绘图，这个绘图会作为subplot的子节点，如果当前subplot的绘图已经和subplot的管理数量一致，此函数返回nullptr
    QImPlotNode* createPlotNode();
    // 获取所有绘图节点
    QList< QImPlotNode* > plotNodes() const;
    // 绘图的数量
    int plotCount() const;
    void addPlotNode(QImPlotNode* plot);
    // 插入绘图，注意plotIndex是subplot节点下面绘图节点的索引，其它节点会跳过,plotIndex可以是-1，则代表在最前面插入，可以大于等于size，代表最后插入
    void insertPlotNode(int plotIndex, QImPlotNode* plot);
    // plotNode在subplot下的索引
    int plotNodeSubplotIndex(QImPlotNode* plot);
    // 提取出QImPlotNode，不在此figure里管理
    bool takePlotNode(QImPlotNode* plot);
    // 移除绘图，plot会被删除
    void removePlotNode(QImPlotNode* plot);
Q_SIGNALS:
    /**
     * @brief QImPlotNode的添加或删除的信号，此信号等同绑定subplotNode的childNodeAdded/childNodeRemoved
     * @param plot
     * @param attach
     */
    void plotNodeAttached(QIM::QImPlotNode* plot, bool attach);

protected:
    void initializeGL() override;
private:
    QImSubplotsNode* ensureSubplotNode();
private Q_SLOTS:
    void onSubplotChildNodeRemoved(QIM::QImAbstractNode* c);
    void onSubplotChildNodeAdded(QIM::QImAbstractNode* c);
};
}
#endif  // QIMFIGUREWIDGET_H
