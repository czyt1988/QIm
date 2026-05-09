#include "ProcessTreeWidget.h"
#include "collector/ProcessInfo.h"
#include "aggregator/HistoryBuffer.h"

#include <algorithm>

ProcessTreeWidget::ProcessTreeWidget(QWidget* parent)
    : QTreeView(parent)
    , model_(new QStandardItemModel(this))
    , historyBuffer_(nullptr)
{
    setupColumns();
    setModel(model_);
    setSortingEnabled(true);
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

void ProcessTreeWidget::updateData(const QList<AggregatedProcessInfo>& data)
{
    model_->removeRows(0, model_->rowCount());

    QList<AggregatedProcessInfo> sorted = data;
    std::sort(sorted.begin(), sorted.end(),
              [](const AggregatedProcessInfo& a, const AggregatedProcessInfo& b) {
                  return a.totalCpuPercent > b.totalCpuPercent;
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

    expandAll();
}

void ProcessTreeWidget::setHistoryBuffer(HistoryBuffer* buffer)
{
    historyBuffer_ = buffer;
}