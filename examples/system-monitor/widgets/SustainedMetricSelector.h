#pragma once

#include <QWidget>
#include <QRadioButton>
#include "aggregator/SustainedMetricsTracker.h"

class SustainedMetricSelector : public QWidget {
    Q_OBJECT
public:
    explicit SustainedMetricSelector(QWidget* parent = nullptr);
    SustainedMetric currentMetric() const;
    void setCurrentMetric(SustainedMetric metric);

Q_SIGNALS:
    void metricChanged(SustainedMetric metric);

private Q_SLOTS:
    void onMetricChanged();

private:
    QRadioButton* btnCpuTime_;
    QRadioButton* btnGpuTime_;
    QRadioButton* btnAvgMemory_;
    QRadioButton* btnDiskRead_;
    QRadioButton* btnDiskWrite_;
};