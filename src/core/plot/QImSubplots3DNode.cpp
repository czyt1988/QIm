#include "QImSubplots3DNode.h"
#include "QImPlot3DNode.h"
#include "imgui.h"

namespace QIM
{
class QImSubplots3DNode::CellNode : public QImAbstractNode
{
public:
    explicit CellNode(int subplotIndex, QObject* parent = nullptr) : QImAbstractNode(parent), m_subplotIndex(subplotIndex)
    {
        setAutoIdEnabled(false);
    }

    bool beginDraw() override
    {
        QImSubplots3DNode* subplots = qobject_cast< QImSubplots3DNode* >(parentNode());
        if (!subplots) {
            return true;
        }
        const QPoint cellPos = subplots->cellPosition(m_subplotIndex);
        const QSizeF cellSz  = subplots->cellSize();
        ImGui::SetCursorPos(ImVec2(static_cast< float >(cellPos.x()), static_cast< float >(cellPos.y())));

        for (QImAbstractNode* child : childrenNodes()) {
            if (QImPlot3DNode* plot = qobject_cast< QImPlot3DNode* >(child)) {
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
    int m_subplotIndex { 0 };
};

QImSubplots3DNode::QImSubplots3DNode(QObject* parent) : QImAbstractNode(parent)
{
    setTitle(QStringLiteral("##Subplots3D"));
}

QImSubplots3DNode::QImSubplots3DNode(const QString& title, QObject* parent) : QImAbstractNode(parent)
{
    setTitle(title);
}

QImSubplots3DNode::~QImSubplots3DNode()
{
}

QString QImSubplots3DNode::title() const
{
    return QString::fromUtf8(m_titleUtf8);
}

void QImSubplots3DNode::setTitle(const QString& title)
{
    const QByteArray utf8 = title.toUtf8();
    if (m_titleUtf8 != utf8) {
        m_titleUtf8 = utf8;
        Q_EMIT titleChanged(title);
    }
}

int QImSubplots3DNode::rows() const
{
    return m_rows;
}

void QImSubplots3DNode::setRows(int rows)
{
    if (rows > 0 && m_rows != rows) {
        m_rows = rows;
        Q_EMIT gridInfoChanged();
    }
}

int QImSubplots3DNode::columns() const
{
    return m_cols;
}

void QImSubplots3DNode::setColumns(int columns)
{
    if (columns > 0 && m_cols != columns) {
        m_cols = columns;
        Q_EMIT gridInfoChanged();
    }
}

void QImSubplots3DNode::setGrid(int rows, int cols)
{
    {
        QSignalBlocker blocker(static_cast< QObject* >(this));
        setRows(rows);
        setColumns(cols);
    }
    Q_EMIT gridInfoChanged();
}

QSizeF QImSubplots3DNode::size() const
{
    return m_size;
}

void QImSubplots3DNode::setSize(const QSizeF& size)
{
    if (m_size != size) {
        m_size = size;
        Q_EMIT sizeChanged(size);
    }
}

int QImSubplots3DNode::gridCount() const
{
    return m_rows * m_cols;
}

QImSubplots3DNode::CellNode* QImSubplots3DNode::createCellNode()
{
    CellNode* cell = new CellNode(plotCount(), static_cast< QObject* >(this));
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

QList< QImPlot3DNode* > QImSubplots3DNode::plotNodes() const
{
    QList< QImPlot3DNode* > plots;
    const QList< QImAbstractNode* >& cells = this->childrenNodes();
    for (QImAbstractNode* child : cells) {
        if (!child) {
            continue;
        }
        const QList< QImAbstractNode* >& cellChildren = child->childrenNodes();
        for (QImAbstractNode* cellChild : cellChildren) {
            if (QImPlot3DNode* plot = qobject_cast< QImPlot3DNode* >(cellChild)) {
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
    const ImVec2 cursorPos = ImGui::GetCursorPos();
    const ImVec2 availSize = ImGui::GetContentRegionAvail();
    m_origin = QPointF(cursorPos.x, cursorPos.y);
    m_availableSize = QSizeF(
        m_size.width() > 0 ? m_size.width() : static_cast< double >(availSize.x),
        m_size.height() > 0 ? m_size.height() : static_cast< double >(availSize.y)
    );
    return true;
}

void QImSubplots3DNode::endDraw()
{
    ImGui::SetCursorPos(ImVec2(static_cast< float >(m_origin.x()), static_cast< float >(m_origin.y())));
    ImGui::Dummy(ImVec2(static_cast< float >(m_availableSize.width()), static_cast< float >(m_availableSize.height())));
}

QPoint QImSubplots3DNode::cellPosition(int index) const
{
    const QSizeF sz = cellSize();
    const int row   = (m_cols > 0) ? (index / m_cols) : 0;
    const int col   = (m_cols > 0) ? (index % m_cols) : 0;
    return QPoint(
        static_cast< int >(m_origin.x() + static_cast< double >(col) * sz.width()),
        static_cast< int >(m_origin.y() + static_cast< double >(row) * sz.height())
    );
}

QSizeF QImSubplots3DNode::cellSize() const
{
    const double width = (m_cols > 0) ? (m_availableSize.width() / static_cast< double >(m_cols )) : m_availableSize.width();
    const double height = (m_rows > 0) ? (m_availableSize.height() / static_cast< double >(m_rows )) : m_availableSize.height();
    return QSizeF(width, height);
}
}  // namespace QIM
