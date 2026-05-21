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
    struct SortState {
        int column = 1;  // Default sort by CPU% (column 1)
        Qt::SortOrder order = Qt::DescendingOrder;  // Default descending
    };

    QStandardItemModel* model_;
    HistoryBuffer* historyBuffer_;
    SortState sortState_;

    void setupColumns();
    QStandardItem* createNumberItem(double value, const QString& suffix = "");
    QStandardItem* createTextItem(const QString& text);
    bool compareProcesses(const AggregatedProcessInfo& a, const AggregatedProcessInfo& b) const;
};