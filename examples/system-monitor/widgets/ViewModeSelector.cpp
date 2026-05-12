#include "ViewModeSelector.h"

#include <QHBoxLayout>
#include <QButtonGroup>

ViewModeSelector::ViewModeSelector(QWidget* parent)
    : QWidget(parent)
    , btnStackedArea_(new QRadioButton(tr("Stacked Area"), this))
    , btnPieChart_(new QRadioButton(tr("Pie Chart"), this))
    , btnScatter3D_(new QRadioButton(tr("3D Scatter"), this))
    , btnTimeSeries3D_(new QRadioButton(tr("3D Time Series"), this))
    , btnSystemOverview_(new QRadioButton(tr("System Overview"), this))
{
    auto* layout = new QHBoxLayout(this);
    layout->addWidget(btnStackedArea_);
    layout->addWidget(btnPieChart_);
    layout->addWidget(btnScatter3D_);
    layout->addWidget(btnTimeSeries3D_);
    layout->addWidget(btnSystemOverview_);
    layout->addStretch();

    auto* buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(btnStackedArea_);
    buttonGroup->addButton(btnPieChart_);
    buttonGroup->addButton(btnScatter3D_);
    buttonGroup->addButton(btnTimeSeries3D_);
    buttonGroup->addButton(btnSystemOverview_);

    connect(btnStackedArea_, &QRadioButton::toggled, this, &ViewModeSelector::onModeChanged);
    connect(btnPieChart_, &QRadioButton::toggled, this, &ViewModeSelector::onModeChanged);
    connect(btnScatter3D_, &QRadioButton::toggled, this, &ViewModeSelector::onModeChanged);
    connect(btnTimeSeries3D_, &QRadioButton::toggled, this, &ViewModeSelector::onModeChanged);
    connect(btnSystemOverview_, &QRadioButton::toggled, this, &ViewModeSelector::onModeChanged);

    btnStackedArea_->setChecked(true);
}

ViewMode ViewModeSelector::currentMode() const
{
    if (btnStackedArea_->isChecked()) {
        return ViewMode::StackedArea;
    } else if (btnPieChart_->isChecked()) {
        return ViewMode::PieChart;
    } else if (btnScatter3D_->isChecked()) {
        return ViewMode::Scatter3D;
    } else if (btnTimeSeries3D_->isChecked()) {
        return ViewMode::TimeSeries3D;
    } else if (btnSystemOverview_->isChecked()) {
        return ViewMode::SystemOverview;
    }
    return ViewMode::StackedArea;
}

void ViewModeSelector::setCurrentMode(ViewMode mode)
{
    switch (mode) {
    case ViewMode::StackedArea:
        btnStackedArea_->setChecked(true);
        break;
    case ViewMode::PieChart:
        btnPieChart_->setChecked(true);
        break;
    case ViewMode::Scatter3D:
        btnScatter3D_->setChecked(true);
        break;
    case ViewMode::TimeSeries3D:
        btnTimeSeries3D_->setChecked(true);
        break;
    case ViewMode::SystemOverview:
        btnSystemOverview_->setChecked(true);
        break;
    }
}

void ViewModeSelector::onModeChanged()
{
    Q_EMIT modeChanged(currentMode());
}
