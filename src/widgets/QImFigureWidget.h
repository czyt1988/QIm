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
 * QImFigureWidget内部会创建一个QImSubplotsNode
 *
 * 绘图的节点都会作为QImSubplotsNode的子节点，默认QImSubplotsNode会有1行1列的布局
 *
 * 你也可以直接调用@ref addRenderNode 把节点挂在顶层窗口下面，这样你可以创建任意渲染节点在subplot上面
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
};
}
#endif  // QIMFIGUREWIDGET_H
