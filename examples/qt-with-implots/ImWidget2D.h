#ifndef IMWIDGET2D_H
#define IMWIDGET2D_H
#include "QImWidget.h"
#include <QOpenGLExtraFunctions>

struct ImPlotContext;

class ImWidget2D : public QIM::QImWidget
{
    Q_OBJECT
public:
    ImWidget2D(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~ImWidget2D();

public:
    virtual void afterRenderImNodes() override;

protected:
    void initializeGL() override;
    // void paintGL() override;

private:
    ImPlotContext* m_context { nullptr };
};

#endif  // IMWIDGET2D_H
