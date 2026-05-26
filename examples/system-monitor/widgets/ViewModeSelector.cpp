#include "ViewModeSelector.h"

#include <QHBoxLayout>
#include <QButtonGroup>

ViewModeSelector::ViewModeSelector(QWidget* parent)
    : QWidget(parent)
    , btnCpuUsage_(new QRadioButton(tr("CPU Usage"), this))
    , btnResourcePie_(new QRadioButton(tr("Resource Pies"), this))
    , btnProcessResource3D_(new QRadioButton(tr("Process 3D"), this))
    , btnSystemMetrics_(new QRadioButton(tr("System Metrics"), this))
    , btnSustainedMetrics_(new QRadioButton(tr("Sustained Metrics"), this))
{
    auto* layout = new QHBoxLayout(this);
    layout->addWidget(btnCpuUsage_);
    layout->addWidget(btnResourcePie_);
    layout->addWidget(btnProcessResource3D_);
    layout->addWidget(btnSystemMetrics_);
    layout->addWidget(btnSustainedMetrics_);
    layout->addStretch();

    auto* buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(btnCpuUsage_);
    buttonGroup->addButton(btnResourcePie_);
    buttonGroup->addButton(btnProcessResource3D_);
    buttonGroup->addButton(btnSystemMetrics_);
    buttonGroup->addButton(btnSustainedMetrics_);

    connect(btnCpuUsage_, &QRadioButton::toggled, this, &ViewModeSelector::onModeChanged);
    connect(btnResourcePie_, &QRadioButton::toggled, this, &ViewModeSelector::onModeChanged);
    connect(btnProcessResource3D_, &QRadioButton::toggled, this, &ViewModeSelector::onModeChanged);
    connect(btnSystemMetrics_, &QRadioButton::toggled, this, &ViewModeSelector::onModeChanged);
    connect(btnSustainedMetrics_, &QRadioButton::toggled, this, &ViewModeSelector::onModeChanged);

    btnCpuUsage_->setChecked(true);
}

ViewMode ViewModeSelector::currentMode() const
{
    if (btnCpuUsage_->isChecked()) {
        return ViewMode::CpuUsage;
    } else if (btnResourcePie_->isChecked()) {
        return ViewMode::ResourcePie;
    } else if (btnProcessResource3D_->isChecked()) {
        return ViewMode::ProcessResource3D;
    } else if (btnSystemMetrics_->isChecked()) {
        return ViewMode::SystemMetrics;
    } else if (btnSustainedMetrics_->isChecked()) {
        return ViewMode::SustainedMetrics;
    }
    return ViewMode::CpuUsage;
}

void ViewModeSelector::setCurrentMode(ViewMode mode)
{
    switch (mode) {
    case ViewMode::CpuUsage:
        btnCpuUsage_->setChecked(true);
        break;
    case ViewMode::ResourcePie:
        btnResourcePie_->setChecked(true);
        break;
    case ViewMode::ProcessResource3D:
        btnProcessResource3D_->setChecked(true);
        break;
    case ViewMode::SystemMetrics:
        btnSystemMetrics_->setChecked(true);
        break;
    case ViewMode::SustainedMetrics:
        btnSustainedMetrics_->setChecked(true);
        break;
    }
}

void ViewModeSelector::onModeChanged()
{
    Q_EMIT modeChanged(currentMode());
}
