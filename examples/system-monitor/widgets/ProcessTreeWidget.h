#pragma once

#include <QTreeView>
#include <QStandardItemModel>
#include "collector/ProcessInfo.h"

class HistoryBuffer;

class ProcessTreeWidget : public QTreeView {
    Q_OBJECT
public:
    explicit ProcessTreeWidget(QWidget* parent = nullptr);

    void updateData(const QList<AggregatedProcessInfo>& data);
    void setHistoryBuffer(HistoryBuffer* buffer);

private:
    QStandardItemModel* model_;
    HistoryBuffer* historyBuffer_;

    void setupColumns();
    QStandardItem* createNumberItem(double value, const QString& suffix = "");
    QStandardItem* createTextItem(const QString& text);
};