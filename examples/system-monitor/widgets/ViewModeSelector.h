#pragma once

#include <QWidget>
#include <QRadioButton>

enum class ViewMode { CpuUsage, ResourcePie, ProcessResource3D,     SystemOverview3D, SystemMetrics, SustainedMetrics };

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
    QRadioButton* btnCpuUsage_;
    QRadioButton* btnResourcePie_;
    QRadioButton* btnProcessResource3D_;
    QRadioButton* btnSystemOverview3D_;
    QRadioButton* btnSystemMetrics_;
    QRadioButton* btnSustainedMetrics_;
};
