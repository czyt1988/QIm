#pragma once

#include <QWidget>
#include <QRadioButton>

enum class ViewMode { StackedArea, PieChart, Scatter3D, TimeSeries3D };

class ViewModeSelector : public QWidget {
    Q_OBJECT
public:
    explicit ViewModeSelector(QWidget* parent = nullptr);
    ViewMode currentMode() const;
    void setCurrentMode(ViewMode mode);

Q_SIGNALS:
    void modeChanged(ViewMode mode);

private Q_SLOTS:
    void onModeChanged();

private:
    QRadioButton* btnStackedArea_;
    QRadioButton* btnPieChart_;
    QRadioButton* btnScatter3D_;
    QRadioButton* btnTimeSeries3D_;
};
