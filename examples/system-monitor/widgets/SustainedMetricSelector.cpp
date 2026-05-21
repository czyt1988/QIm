#include "SustainedMetricSelector.h"

#include <QHBoxLayout>
#include <QButtonGroup>

SustainedMetricSelector::SustainedMetricSelector(QWidget* parent)
    : QWidget(parent)
    , btnCpuTime_(new QRadioButton(tr("CPU Time"), this))
    , btnGpuTime_(new QRadioButton(tr("GPU Time"), this))
    , btnAvgMemory_(new QRadioButton(tr("Avg Memory"), this))
    , btnDiskRead_(new QRadioButton(tr("Disk Read"), this))
    , btnDiskWrite_(new QRadioButton(tr("Disk Write"), this))
{
    auto* layout = new QHBoxLayout(this);
    layout->addWidget(btnCpuTime_);
    layout->addWidget(btnGpuTime_);
    layout->addWidget(btnAvgMemory_);
    layout->addWidget(btnDiskRead_);
    layout->addWidget(btnDiskWrite_);
    layout->addStretch();

    auto* buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(btnCpuTime_);
    buttonGroup->addButton(btnGpuTime_);
    buttonGroup->addButton(btnAvgMemory_);
    buttonGroup->addButton(btnDiskRead_);
    buttonGroup->addButton(btnDiskWrite_);

    connect(btnCpuTime_, &QRadioButton::toggled, this, &SustainedMetricSelector::onMetricChanged);
    connect(btnGpuTime_, &QRadioButton::toggled, this, &SustainedMetricSelector::onMetricChanged);
    connect(btnAvgMemory_, &QRadioButton::toggled, this, &SustainedMetricSelector::onMetricChanged);
    connect(btnDiskRead_, &QRadioButton::toggled, this, &SustainedMetricSelector::onMetricChanged);
    connect(btnDiskWrite_, &QRadioButton::toggled, this, &SustainedMetricSelector::onMetricChanged);

    btnCpuTime_->setChecked(true);
}

SustainedMetric SustainedMetricSelector::currentMetric() const
{
    if (btnCpuTime_->isChecked()) {
        return SustainedMetric::CpuTime;
    } else if (btnGpuTime_->isChecked()) {
        return SustainedMetric::GpuTime;
    } else if (btnAvgMemory_->isChecked()) {
        return SustainedMetric::AvgMemory;
    } else if (btnDiskRead_->isChecked()) {
        return SustainedMetric::DiskReadTotal;
    } else if (btnDiskWrite_->isChecked()) {
        return SustainedMetric::DiskWriteTotal;
    }
    return SustainedMetric::CpuTime;
}

void SustainedMetricSelector::setCurrentMetric(SustainedMetric metric)
{
    switch (metric) {
    case SustainedMetric::CpuTime:
        btnCpuTime_->setChecked(true);
        break;
    case SustainedMetric::GpuTime:
        btnGpuTime_->setChecked(true);
        break;
    case SustainedMetric::AvgMemory:
        btnAvgMemory_->setChecked(true);
        break;
    case SustainedMetric::DiskReadTotal:
        btnDiskRead_->setChecked(true);
        break;
    case SustainedMetric::DiskWriteTotal:
        btnDiskWrite_->setChecked(true);
        break;
    }
}

void SustainedMetricSelector::onMetricChanged()
{
    Q_EMIT metricChanged(currentMetric());
}