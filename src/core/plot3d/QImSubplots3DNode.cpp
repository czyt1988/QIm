#include "QImSubplots3DNode.h"
#include "QImAPI.h"
#include "QImPlot3DNode.h"
#include "imgui.h"

namespace QIM
{

// ===============================================================
// PrivateData Implementation
// ===============================================================

class QImSubplots3DNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImSubplots3DNode)

public:
    explicit PrivateData(QImSubplots3DNode* q) : q_ptr(q) {}

    QByteArray titleUtf8;
    int rows{1};
    int cols{1};
    QSizeF size{-1.0, -1.0};
    QPointF origin{0.0, 0.0};
    QSizeF availableSize{0.0, 0.0};
};

// ===============================================================
// CellNode Implementation (private nested class)
// ===============================================================

class QImSubplots3DNode::CellNode : public QImAbstractNode
{
public:
    explicit CellNode(int subplotIndex, QObject* parent = nullptr) : QImAbstractNode(parent), subplotIndex(subplotIndex)
    {
        setAutoIdEnabled(false);
    }

    bool beginDraw() override
    {
        QImSubplots3DNode* subplots = qobject_cast<QImSubplots3DNode*>(parentNode());
        if (!subplots) {
            return true;
        }
        const QPoint cellPos = subplots->cellPosition(subplotIndex);
        const QSizeF cellSz  = subplots->cellSize();
        ImGui::SetCursorPos(ImVec2(static_cast<float>(cellPos.x()), static_cast<float>(cellPos.y())));

        for (QImAbstractNode* child : childrenNodes()) {
            if (QImPlot3DNode* plot = qobject_cast<QImPlot3DNode*>(child)) {
                plot->setAutoSize(false);
                plot->setSize(cellSz);
            }
        }
        return true;
    }

    void endDraw() override
    {
    }

private:
    int subplotIndex{0};
};

// ===============================================================
// QImSubplots3DNode Implementation
// ===============================================================

QImSubplots3DNode::QImSubplots3DNode(QObject* parent)
    : QImAbstractNode(parent)
    , QIM_PIMPL_CONSTRUCT
{
    setTitle(QStringLiteral("##Subplots3D"));
}

QImSubplots3DNode::QImSubplots3DNode(const QString& title, QObject* parent)
    : QImAbstractNode(parent)
    , QIM_PIMPL_CONSTRUCT
{
    setTitle(title);
}

QImSubplots3DNode::~QImSubplots3DNode()
{
}

QString QImSubplots3DNode::title() const
{
    QIM_DC(d);
    return QString::fromUtf8(d->titleUtf8);
}

void QImSubplots3DNode::setTitle(const QString& title)
{
    QIM_D(d);
    const QByteArray utf8 = title.toUtf8();
    if (d->titleUtf8 != utf8) {
        d->titleUtf8 = utf8;
        Q_EMIT titleChanged(title);
    }
}

int QImSubplots3DNode::rows() const
{
    QIM_DC(d);
    return d->rows;
}

void QImSubplots3DNode::setRows(int rows)
{
    QIM_D(d);
    if (rows > 0 && d->rows != rows) {
        d->rows = rows;
        Q_EMIT gridInfoChanged();
    }
}

int QImSubplots3DNode::columns() const
{
    QIM_DC(d);
    return d->cols;
}

void QImSubplots3DNode::setColumns(int columns)
{
    QIM_D(d);
    if (columns > 0 && d->cols != columns) {
        d->cols = columns;
        Q_EMIT gridInfoChanged();
    }
}

void QImSubplots3DNode::setGrid(int rows, int cols)
{
    {
        QSignalBlocker blocker(static_cast<QObject*>(this));
        setRows(rows);
        setColumns(cols);
    }
    Q_EMIT gridInfoChanged();
}

QSizeF QImSubplots3DNode::size() const
{
    QIM_DC(d);
    return d->size;
}

void QImSubplots3DNode::setSize(const QSizeF& size)
{
    QIM_D(d);
    if (d->size != size) {
        d->size = size;
        Q_EMIT sizeChanged(size);
    }
}

int QImSubplots3DNode::gridCount() const
{
    QIM_DC(d);
    return d->rows * d->cols;
}

QImSubplots3DNode::CellNode* QImSubplots3DNode::createCellNode()
{
    CellNode* cell = new CellNode(plotCount(), static_cast<QObject*>(this));
    this->addChildNode(cell);
    return cell;
}

QImPlot3DNode* QImSubplots3DNode::createPlotNode()
{
    if (plotCount() >= gridCount()) {
        return nullptr;
    }
    CellNode* cell      = createCellNode();
    QImPlot3DNode* plot = new QImPlot3DNode(cell);
    return plot;
}

QList<QImPlot3DNode*> QImSubplots3DNode::plotNodes() const
{
    QList<QImPlot3DNode*> plots;
    const QList<QImAbstractNode*>& cells = this->childrenNodes();
    for (QImAbstractNode* child : cells) {
        if (!child) {
            continue;
        }
        const QList<QImAbstractNode*>& cellChildren = child->childrenNodes();
        for (QImAbstractNode* cellChild : cellChildren) {
            if (QImPlot3DNode* plot = qobject_cast<QImPlot3DNode*>(cellChild)) {
                plots.push_back(plot);
            }
        }
    }
    return plots;
}

int QImSubplots3DNode::plotCount() const
{
    return plotNodes().size();
}

bool QImSubplots3DNode::beginDraw()
{
    QIM_D(d);
    const ImVec2 cursorPos = ImGui::GetCursorPos();
    const ImVec2 availSize = ImGui::GetContentRegionAvail();
    d->origin = QPointF(cursorPos.x, cursorPos.y);
    d->availableSize = QSizeF(
        d->size.width() > 0 ? d->size.width() : static_cast<double>(availSize.x),
        d->size.height() > 0 ? d->size.height() : static_cast<double>(availSize.y)
    );
    return true;
}

void QImSubplots3DNode::endDraw()
{
    QIM_D(d);
    ImGui::SetCursorPos(ImVec2(static_cast<float>(d->origin.x()), static_cast<float>(d->origin.y())));
    ImGui::Dummy(ImVec2(static_cast<float>(d->availableSize.width()), static_cast<float>(d->availableSize.height())));
}

QPoint QImSubplots3DNode::cellPosition(int index) const
{
    QIM_DC(d);
    const QSizeF sz = cellSize();
    const int row   = (d->cols > 0) ? (index / d->cols) : 0;
    const int col   = (d->cols > 0) ? (index % d->cols) : 0;
    return QPoint(
        static_cast<int>(d->origin.x() + static_cast<double>(col) * sz.width()),
        static_cast<int>(d->origin.y() + static_cast<double>(row) * sz.height())
    );
}

QSizeF QImSubplots3DNode::cellSize() const
{
    QIM_DC(d);
    const double width = (d->cols > 0) ? (d->availableSize.width() / static_cast<double>(d->cols)) : d->availableSize.width();
    const double height = (d->rows > 0) ? (d->availableSize.height() / static_cast<double>(d->rows)) : d->availableSize.height();
    return QSizeF(width, height);
}

}  // namespace QIM
