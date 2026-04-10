#ifndef QIMFIGURE3DWIDGET_H
#define QIMFIGURE3DWIDGET_H

#include "QImWidget.h"

struct ImPlot3DContext;

namespace QIM
{
class QImPlot3DNode;

class QIM_WIDGETS_API QImFigure3DWidget : public QImWidget
{
    Q_OBJECT

public:
    explicit QImFigure3DWidget(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~QImFigure3DWidget() override;

    QImPlot3DNode* createPlotNode();
    QList< QImPlot3DNode* > plotNodes() const;

protected:
    void initializeGL() override;
    void beforeRenderImNodes() override;

private:
    ImPlot3DContext* m_context { nullptr };
};
}  // namespace QIM

#endif  // QIMFIGURE3DWIDGET_H
