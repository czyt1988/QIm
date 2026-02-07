#ifndef IMTESTWIDGET_H
#define IMTESTWIDGET_H
#include "QImWidget.h"

class ImTestWidget : public QIM::QImWidget
{
    Q_OBJECT
public:
    ImTestWidget(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

public:
    virtual void afterRenderImNodes() override;
};

#endif  // IMTESTWIDGET_H
