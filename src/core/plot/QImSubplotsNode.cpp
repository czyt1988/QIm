#include "QImSubplotsNode.h"
#include "QImPlot3DNode.h"
#include "QImPlotNode.h"
#include "imgui.h"

namespace QIM
{
class QImSubplotsNode::CellNode : public QImAbstractNode
{
public:
    explicit CellNode(int subplotIndex, QObject* parent = nullptr) : QImAbstractNode(parent), m_subplotIndex(subplotIndex)
    {
        setAutoIdEnabled(false);
    }

    bool beginDraw() override
    {
        QImSubplotsNode* subplots = qobject_cast< QImSubplotsNode* >(parentNode());
        if (!subplots) {
            return true;
        }
        const QPoint cellPos = subplots->cellPosition(m_subplotIndex);
        const QSizeF cellSz  = subplots->cellSize();
        ImGui::SetCursorPos(ImVec2(static_cast< float >(cellPos.x()), static_cast< float >(cellPos.y())));

        for (QImAbstractNode* child : childrenNodes()) {
            if (QImPlotNode* plot2D = qobject_cast< QImPlotNode* >(child)) {
                plot2D->setAutoSize(false);
                plot2D->setSize(cellSz);
            } else if (QImPlot3DNode* plot3D = qobject_cast< QImPlot3DNode* >(child)) {
                plot3D->setAutoSize(false);
                plot3D->setSize(cellSz);
            }
        }
        return true;
    }

    void endDraw() override
    {
    }

    void setSubplotIndex(int subplotIndex)
    {
        m_subplotIndex = subplotIndex;
    }

private:
    int m_subplotIndex { 0 };
};

QImSubplotsNode::QImSubplotsNode(QObject* parent) : QImAbstractNode(parent)
{
    setTitle(QStringLiteral("##SubplotsNode"));
}

QImSubplotsNode::QImSubplotsNode(const QString& title, QObject* parent) : QImAbstractNode(parent)
{
    setTitle(title);
}

QImSubplotsNode::~QImSubplotsNode()
{
}

QString QImSubplotsNode::title() const
{
    return QString::fromUtf8(m_titleUtf8);
}

void QImSubplotsNode::setTitle(const QString& title)
{
    const QByteArray utf8 = title.toUtf8();
    if (m_titleUtf8 != utf8) {
        m_titleUtf8 = utf8;
        Q_EMIT titleChanged(title);
    }
}

int QImSubplotsNode::rows() const
{
    return m_rows;
}

void QImSubplotsNode::setRows(int rows)
{
    if (rows > 0 && m_rows != rows) {
        m_rows = rows;
        Q_EMIT gridInfoChanged();
    }
}

int QImSubplotsNode::columns() const
{
    return m_cols;
}

void QImSubplotsNode::setColumns(int columns)
{
    if (columns > 0 && m_cols != columns) {
        m_cols = columns;
        Q_EMIT gridInfoChanged();
    }
}

std::vector< float > QImSubplotsNode::rowRatios() const
{
    return {};
}

void QImSubplotsNode::setRowRatios(const std::vector< float >& row_ratios)
{
    Q_UNUSED(row_ratios);
}

std::vector< float > QImSubplotsNode::columnRatios() const
{
    return {};
}

void QImSubplotsNode::setColumnRatios(const std::vector< float >& col_ratios)
{
    Q_UNUSED(col_ratios);
}

void QImSubplotsNode::setGrid(int rows, int cols, const std::vector< float >& row_ratios, const std::vector< float >& col_ratios)
{
    Q_UNUSED(row_ratios);
    Q_UNUSED(col_ratios);
    {
        QSignalBlocker blocker(static_cast< QObject* >(this));
        setRows(rows);
        setColumns(cols);
    }
    Q_EMIT gridInfoChanged();
}

QSizeF QImSubplotsNode::size() const
{
    return m_size;
}

void QImSubplotsNode::setSize(const QSizeF& size)
{
    if (m_size != size) {
        m_size = size;
        Q_EMIT sizeChanged(size);
    }
}

int QImSubplotsNode::gridCount() const
{
    return m_rows * m_cols;
}

int QImSubplotsNode::itemCount() const
{
    return childrenNodes().size();
}

QImSubplotsNode::CellNode* QImSubplotsNode::createCellNode()
{
    CellNode* cell = new CellNode(itemCount(), static_cast< QObject* >(this));
    addChildNode(cell);
    return cell;
}

QImPlotNode* QImSubplotsNode::createPlotNode()
{
    if (itemCount() >= gridCount()) {
        return nullptr;
    }
    CellNode* cell    = createCellNode();
    QImPlotNode* plot = new QImPlotNode(cell);
    return plot;
}

void QImSubplotsNode::addPlotNode(QImPlotNode* plot)
{
    insertPlotNode(plotCount(), plot);
}

void QImSubplotsNode::insertPlotNode(int plotIndex, QImPlotNode* plot)
{
    if (!plot || itemCount() >= gridCount()) {
        return;
    }
    if (plotIndex < 0) {
        plotIndex = 0;
    }
    if (plotIndex > itemCount()) {
        plotIndex = itemCount();
    }
    CellNode* cell = new CellNode(plotIndex, static_cast< QObject* >(this));
    insertChildNode(plotIndex, cell);
    plot->setParent(cell);
    cell->addChildNode(plot);
    updateCellIndices();
}

int QImSubplotsNode::plotNodeSubplotIndex(QImPlotNode* plot) const
{
    return plotNodes().indexOf(plot);
}

bool QImSubplotsNode::takePlotNode(QImPlotNode* plot)
{
    if (!plot) {
        return false;
    }
    const QList< QImAbstractNode* >& cells = childrenNodes();
    for (QImAbstractNode* child : cells) {
        if (!child) {
            continue;
        }
        if (child->childrenNodes().contains(plot)) {
            child->takeChildNode(plot);
            removeChildNode(child);
            updateCellIndices();
            return true;
        }
    }
    return false;
}

void QImSubplotsNode::removePlotNode(QImPlotNode* plot)
{
    if (!plot) {
        return;
    }
    const QList< QImAbstractNode* >& cells = childrenNodes();
    for (QImAbstractNode* child : cells) {
        if (!child) {
            continue;
        }
        if (child->childrenNodes().contains(plot)) {
            child->removeChildNode(plot);
            removeChildNode(child);
            updateCellIndices();
            return;
        }
    }
}

QImPlot3DNode* QImSubplotsNode::createPlot3DNode()
{
    if (itemCount() >= gridCount()) {
        return nullptr;
    }
    CellNode* cell        = createCellNode();
    QImPlot3DNode* plot3D = new QImPlot3DNode(cell);
    return plot3D;
}

QList< QImPlotNode* > QImSubplotsNode::plotNodes() const
{
    QList< QImPlotNode* > plots;
    for (QImAbstractNode* child : childrenNodes()) {
        if (!child) {
            continue;
        }
        for (QImAbstractNode* cellChild : child->childrenNodes()) {
            if (QImPlotNode* plot = qobject_cast< QImPlotNode* >(cellChild)) {
                plots.push_back(plot);
            }
        }
    }
    return plots;
}

QList< QImPlot3DNode* > QImSubplotsNode::plot3DNodes() const
{
    QList< QImPlot3DNode* > plots;
    for (QImAbstractNode* child : childrenNodes()) {
        if (!child) {
            continue;
        }
        for (QImAbstractNode* cellChild : child->childrenNodes()) {
            if (QImPlot3DNode* plot = qobject_cast< QImPlot3DNode* >(cellChild)) {
                plots.push_back(plot);
            }
        }
    }
    return plots;
}

int QImSubplotsNode::plotCount() const
{
    return plotNodes().size();
}

int QImSubplotsNode::plot3DCount() const
{
    return plot3DNodes().size();
}

bool QImSubplotsNode::isTitleEnabled() const
{
    return true;
}

void QImSubplotsNode::setTitleEnabled(bool on)
{
    Q_UNUSED(on);
}

bool QImSubplotsNode::isLegendEnabled() const
{
    return true;
}

void QImSubplotsNode::setLegendEnabled(bool on)
{
    Q_UNUSED(on);
}

bool QImSubplotsNode::isDefaultMenusEnabled() const
{
    return true;
}

void QImSubplotsNode::setDefaultMenusEnabled(bool on)
{
    Q_UNUSED(on);
}

bool QImSubplotsNode::isResizable() const
{
    return true;
}

void QImSubplotsNode::setResizable(bool on)
{
    Q_UNUSED(on);
}

bool QImSubplotsNode::isAlignedEnabled() const
{
    return true;
}

void QImSubplotsNode::setAlignedEnabled(bool on)
{
    Q_UNUSED(on);
}

bool QImSubplotsNode::isShareItemsEnabled() const
{
    return false;
}

void QImSubplotsNode::setShareItemsEnabled(bool on)
{
    Q_UNUSED(on);
}

bool QImSubplotsNode::isLinkRows() const
{
    return false;
}

void QImSubplotsNode::setLinkRows(bool on)
{
    Q_UNUSED(on);
}

bool QImSubplotsNode::isLinkColumns() const
{
    return false;
}

void QImSubplotsNode::setLinkColumns(bool on)
{
    Q_UNUSED(on);
}

bool QImSubplotsNode::isLinkAllX() const
{
    return false;
}

void QImSubplotsNode::setLinkAllX(bool on)
{
    Q_UNUSED(on);
}

bool QImSubplotsNode::isLinkAllY() const
{
    return false;
}

void QImSubplotsNode::setLinkAllY(bool on)
{
    Q_UNUSED(on);
}

bool QImSubplotsNode::isColumnMajor() const
{
    return false;
}

void QImSubplotsNode::setColumnMajor(bool on)
{
    Q_UNUSED(on);
}

bool QImSubplotsNode::isTrackGridRatiosEnabled() const
{
    return false;
}

void QImSubplotsNode::setTrackGridRatiosEnabled(bool on)
{
    Q_UNUSED(on);
}

void QImSubplotsNode::updateCellIndices()
{
    const QList< QImAbstractNode* >& cells = childrenNodes();
    for (int i = 0; i < cells.size(); ++i) {
        if (CellNode* cellNode = dynamic_cast< CellNode* >(cells[i])) {
            cellNode->setSubplotIndex(i);
        }
    }
}

bool QImSubplotsNode::beginDraw()
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

void QImSubplotsNode::endDraw()
{
    ImGui::SetCursorPos(ImVec2(static_cast< float >(m_origin.x()), static_cast< float >(m_origin.y())));
    ImGui::Dummy(ImVec2(static_cast< float >(m_availableSize.width()), static_cast< float >(m_availableSize.height())));
}

QPoint QImSubplotsNode::cellPosition(int index) const
{
    const QSizeF sz = cellSize();
    const int row   = (m_cols > 0) ? (index / m_cols) : 0;
    const int col   = (m_cols > 0) ? (index % m_cols) : 0;
    return QPoint(
        static_cast< int >(m_origin.x() + static_cast< double >(col) * sz.width()),
        static_cast< int >(m_origin.y() + static_cast< double >(row) * sz.height())
    );
}

QSizeF QImSubplotsNode::cellSize() const
{
    const double width = (m_cols > 0) ? (m_availableSize.width() / static_cast< double >(m_cols)) : m_availableSize.width();
    const double height =
        (m_rows > 0) ? (m_availableSize.height() / static_cast< double >(m_rows)) : m_availableSize.height();
    return QSizeF(width, height);
}
}  // namespace QIM
