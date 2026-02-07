#ifndef IMWIDGET3D_H
#define IMWIDGET3D_H
#include "QImWidget.h"
struct ImPlot3DContext;
class ImWidget3D : public QIM::QImWidget
{
public:
    ImWidget3D(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

public:
    virtual void afterRenderImNodes() override;

protected:
    void initializeGL() override;

private:
    ImPlot3DContext* m_context { nullptr };
};

#endif  // IMWIDGET3D_H
