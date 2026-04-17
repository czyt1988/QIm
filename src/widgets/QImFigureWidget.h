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
class QImPlot3DNode;
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
    QImPlot3DNode* createPlot3DNode();
    QList< QImPlot3DNode* > plot3DNodes() const;
    int plot3DCount() const;
Q_SIGNALS:
    void plotNodeAttached(QIM::QImPlotNode* plot, bool attach);

protected:
    void initializeGL() override;
    void beforeRenderImNodes() override;

private:
    QImSubplotsNode* ensureSubplotNode();
private Q_SLOTS:
    void onSubplotChildNodeRemoved(QIM::QImAbstractNode* c);
    void onSubplotChildNodeAdded(QIM::QImAbstractNode* c);
};
}
#endif  // QIMFIGUREWIDGET_H
