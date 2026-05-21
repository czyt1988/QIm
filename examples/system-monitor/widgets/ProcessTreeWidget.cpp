#include "ProcessTreeWidget.h"
#include "collector/ProcessInfo.h"
#include "aggregator/HistoryBuffer.h"

#include <QHeaderView>
#include <algorithm>

ProcessTreeWidget::ProcessTreeWidget(QWidget* parent)
    : QTreeView(parent)
    , model_(new QStandardItemModel(this))
    , historyBuffer_(nullptr)
{
    setupColumns();
    setModel(model_);
    setSortingEnabled(true);

    // Track sort indicator changes to remember user's sort preference
    connect(header(), &QHeaderView::sortIndicatorChanged,
            this, [this](int column, Qt::SortOrder order) {
                sortState_.column = column;
                sortState_.order = order;
            });
}

void ProcessTreeWidget::setupColumns()
{
    model_->setColumnCount(9);
    model_->setHeaderData(0, Qt::Horizontal, QStringLiteral("Process"));
    model_->setHeaderData(1, Qt::Horizontal, QStringLiteral("CPU%"));
    model_->setHeaderData(2, Qt::Horizontal, QStringLiteral("Memory(MB)"));
    model_->setHeaderData(3, Qt::Horizontal, QStringLiteral("GPU%"));
    model_->setHeaderData(4, Qt::Horizontal, QStringLiteral("DiskR(KB/s)"));
    model_->setHeaderData(5, Qt::Horizontal, QStringLiteral("DiskW(KB/s)"));
    model_->setHeaderData(6, Qt::Horizontal, QStringLiteral("NetR(KB/s)"));
    model_->setHeaderData(7, Qt::Horizontal, QStringLiteral("NetS(KB/s)"));
    model_->setHeaderData(8, Qt::Horizontal, QStringLiteral("Count"));
}

QStandardItem* ProcessTreeWidget::createNumberItem(double value, const QString& suffix)
{
    QStandardItem* item = new QStandardItem();
    item->setText(QString::number(value, 'f', 1) + suffix);
    item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    item->setEditable(false);
    return item;
}

QStandardItem* ProcessTreeWidget::createTextItem(const QString& text)
{
    QStandardItem* item = new QStandardItem(text);
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    item->setEditable(false);
    return item;
}

bool ProcessTreeWidget::compareProcesses(const AggregatedProcessInfo& a, const AggregatedProcessInfo& b) const
{
    // For descending order, swap a and b to reverse comparison direction
    // This preserves strict weak ordering (comp(a,a) must return false)
    const AggregatedProcessInfo& left = (sortState_.order == Qt::AscendingOrder) ? a : b;
    const AggregatedProcessInfo& right = (sortState_.order == Qt::AscendingOrder) ? b : a;

    switch (sortState_.column) {
        case 0: return left.processName < right.processName;
        case 1: return left.totalCpuPercent < right.totalCpuPercent;
        case 2: return left.totalWorkingSetBytes < right.totalWorkingSetBytes;
        case 3: return left.avgGpuPercent < right.avgGpuPercent;
        case 4: return left.totalDiskReadRate < right.totalDiskReadRate;
        case 5: return left.totalDiskWriteRate < right.totalDiskWriteRate;
        case 6: return left.totalNetworkRecvRate < right.totalNetworkRecvRate;
        case 7: return left.totalNetworkSendRate < right.totalNetworkSendRate;
        case 8: return left.instanceCount < right.instanceCount;
        default: return left.totalCpuPercent < right.totalCpuPercent;
    }
}

void ProcessTreeWidget::updateData(const QList<AggregatedProcessInfo>& data)
{
    // Save expanded state before clearing
    QSet<QString> expandedNames;
    for (int i = 0; i < model_->rowCount(); ++i) {
        QModelIndex index = model_->index(i, 0);
        if (isExpanded(index)) {
            expandedNames.insert(model_->item(i, 0)->text());
        }
    }

    model_->removeRows(0, model_->rowCount());

    // Sort data according to user's selected sort state
    QList<AggregatedProcessInfo> sorted = data;
    std::sort(sorted.begin(), sorted.end(),
              [this](const AggregatedProcessInfo& a, const AggregatedProcessInfo& b) {
                  return compareProcesses(a, b);
              });

    for (const AggregatedProcessInfo& info : sorted) {
        QList<QStandardItem*> parentRow;
        parentRow << createTextItem(info.processName)
                  << createNumberItem(info.totalCpuPercent, "%")
                  << createNumberItem(info.totalWorkingSetBytes / 1048576.0, "")
                  << createNumberItem(info.avgGpuPercent, "%")
                  << createNumberItem(info.totalDiskReadRate / 1024.0, "")
                  << createNumberItem(info.totalDiskWriteRate / 1024.0, "")
                  << createNumberItem(info.totalNetworkRecvRate / 1024.0, "")
                  << createNumberItem(info.totalNetworkSendRate / 1024.0, "")
                  << createNumberItem(info.instanceCount, "");

        model_->appendRow(parentRow);

        QStandardItem* parentItem = model_->item(model_->rowCount() - 1, 0);

        for (uint32_t pid : info.pids) {
            QList<QStandardItem*> childRow;
            childRow << createTextItem(QString::number(pid))
                     << createTextItem("-")
                     << createTextItem("-")
                     << createTextItem("-")
                     << createTextItem("-")
                     << createTextItem("-")
                     << createTextItem("-")
                     << createTextItem("-")
                     << createTextItem("-");
            parentItem->appendRow(childRow);
        }
    }

    // Restore expanded state for matching process names
    for (int i = 0; i < model_->rowCount(); ++i) {
        QModelIndex index = model_->index(i, 0);
        QString processName = model_->item(i, 0)->text();
        if (expandedNames.contains(processName)) {
            expand(index);
        }
    }
}

void ProcessTreeWidget::setHistoryBuffer(HistoryBuffer* buffer)
{
    historyBuffer_ = buffer;
}