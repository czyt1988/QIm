#include "QImGridNode.h"
#include "QImAPI.h"
#include <QVariant>
#include "imgui.h"

namespace QIM
{

// ===============================================================
// PrivateData Implementation
// ===============================================================

class QImGridNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImGridNode)

public:
    explicit PrivateData(QImGridNode* q) : q_ptr(q) {}

    QByteArray titleUtf8;
    int rows{1};
    int cols{1};
    QSizeF size{-1.0, -1.0};
    QPointF origin{0.0, 0.0};
    QSizeF availableSize{0.0, 0.0};
};

// ===============================================================
// CellNode Method Implementations (class definition in QImGridNode.h)
// ===============================================================

/**
 * \if ENGLISH
 * @brief Begins cell rendering, positions cursor and notifies child nodes of cell size
 *
 * Uses Qt property system for generic cell size notification instead of type-specific
 * qobject_cast. This allows mixing QImPlotNode (2D) and QImPlot3DNode (3D) in the
 * same grid. Both expose Q_PROPERTY(QSizeF size) and Q_PROPERTY(bool autoSize).
 *
 * CRITICAL: setAutoSize MUST be called BEFORE setSize because
 * QImPlotNode::setAutoSize(false) resets size to (0,0). If reversed, setSize would
 * be overwritten.
 * \endif
 * \if CHINESE
 * @brief 开始单元格渲染，定位光标并通知子节点单元格尺寸
 *
 * 使用 Qt 属性系统进行通用单元格尺寸通知，而非特定类型的 qobject_cast。这允许
 * 在同一网格中混合放置 QImPlotNode（2D）和 QImPlot3DNode（3D）。两者均暴露了
 * Q_PROPERTY(QSizeF size) 和 Q_PROPERTY(bool autoSize)。
 *
 * 关键：setAutoSize 必须在 setSize 之前调用，因为
 * QImPlotNode::setAutoSize(false) 会将尺寸重置为 (0,0)。如果顺序颠倒，
 * setSize 的结果将被覆盖。
 * \endif
 */
bool QImGridNode::CellNode::beginDraw()
{
    QImGridNode* grid = qobject_cast<QImGridNode*>(parentNode());
    if (!grid) {
        return true;
    }
    const QPoint cellPos = grid->cellPosition(m_subplotIndex);
    const QSizeF cellSz  = grid->cellSize();
    ImGui::SetCursorPos(ImVec2(static_cast<float>(cellPos.x()), static_cast<float>(cellPos.y())));

    // Generic cell size notification via Qt property system
    // CRITICAL: setAutoSize MUST be called BEFORE setSize
    // because QImPlotNode::setAutoSize(false) resets size to (0,0)
    for (QImAbstractNode* child : childrenNodes()) {
        const QMetaObject* meta = child->metaObject();
        if (meta->indexOfProperty("autoSize") >= 0) {
            child->setProperty("autoSize", QVariant(false));
        }
        if (meta->indexOfProperty("size") >= 0) {
            child->setProperty("size", QVariant::fromValue(cellSz));
        }
    }
    return true;
}

void QImGridNode::CellNode::endDraw()
{
}

// ===============================================================
// QImGridNode Implementation
// ===============================================================

/**
 * \if ENGLISH
 * @brief Constructor with optional parent
 * @param parent Parent QObject
 * \endif
 * \if CHINESE
 * @brief 构造函数（带可选父对象）
 * @param parent 父对象
 * \endif
 */
QImGridNode::QImGridNode(QObject* parent)
    : QImAbstractNode(parent)
    , QIM_PIMPL_CONSTRUCT
{
    setTitle(QStringLiteral("##Grid"));
}

/**
 * \if ENGLISH
 * @brief Constructor with title and optional parent
 * @param title Grid title
 * @param parent Parent QObject
 * \endif
 * \if CHINESE
 * @brief 构造函数（带标题和可选父对象）
 * @param title 网格标题
 * @param parent 父对象
 * \endif
 */
QImGridNode::QImGridNode(const QString& title, QObject* parent)
    : QImAbstractNode(parent)
    , QIM_PIMPL_CONSTRUCT
{
    setTitle(title);
}

/**
 * \if ENGLISH
 * @brief Destructor
 * \endif
 * \if CHINESE
 * @brief 析构函数
 * \endif
 */
QImGridNode::~QImGridNode()
{
}

/**
 * \if ENGLISH
 * @brief Gets the grid title
 * @return Current title as QString
 * \endif
 * \if CHINESE
 * @brief 获取网格标题
 * @return 当前标题（QString）
 * \endif
 */
QString QImGridNode::title() const
{
    QIM_DC(d);
    return QString::fromUtf8(d->titleUtf8);
}

/**
 * \if ENGLISH
 * @brief Sets the grid title
 * @param title New title
 * \endif
 * \if CHINESE
 * @brief 设置网格标题
 * @param title 新标题
 * \endif
 */
void QImGridNode::setTitle(const QString& title)
{
    QIM_D(d);
    const QByteArray utf8 = title.toUtf8();
    if (d->titleUtf8 != utf8) {
        d->titleUtf8 = utf8;
        Q_EMIT titleChanged(title);
    }
}

/**
 * \if ENGLISH
 * @brief Gets the number of rows
 * @return Row count
 * \endif
 * \if CHINESE
 * @brief 获取行数
 * @return 行数
 * \endif
 */
int QImGridNode::rows() const
{
    QIM_DC(d);
    return d->rows;
}

/**
 * \if ENGLISH
 * @brief Sets the number of rows
 * @param rows New row count (must be > 0)
 * \endif
 * \if CHINESE
 * @brief 设置行数
 * @param rows 新行数（必须 > 0）
 * \endif
 */
void QImGridNode::setRows(int rows)
{
    QIM_D(d);
    if (rows > 0 && d->rows != rows) {
        d->rows = rows;
        Q_EMIT gridInfoChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Gets the number of columns
 * @return Column count
 * \endif
 * \if CHINESE
 * @brief 获取列数
 * @return 列数
 * \endif
 */
int QImGridNode::columns() const
{
    QIM_DC(d);
    return d->cols;
}

/**
 * \if ENGLISH
 * @brief Sets the number of columns
 * @param columns New column count (must be > 0)
 * \endif
 * \if CHINESE
 * @brief 设置列数
 * @param columns 新列数（必须 > 0）
 * \endif
 */
void QImGridNode::setColumns(int columns)
{
    QIM_D(d);
    if (columns > 0 && d->cols != columns) {
        d->cols = columns;
        Q_EMIT gridInfoChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Sets grid dimensions in one call
 * @param rows Number of rows
 * @param cols Number of columns
 * \endif
 * \if CHINESE
 * @brief 一次性设置网格行列数
 * @param rows 行数
 * @param cols 列数
 * \endif
 */
void QImGridNode::setGrid(int rows, int cols)
{
    {
        QSignalBlocker blocker(static_cast<QObject*>(this));
        setRows(rows);
        setColumns(cols);
    }
    Q_EMIT gridInfoChanged();
}

/**
 * \if ENGLISH
 * @brief Gets the grid area size
 * @return Grid size (-1 means auto-fill)
 * \endif
 * \if CHINESE
 * @brief 获取网格区域尺寸
 * @return 网格尺寸（-1 表示自动填充）
 * \endif
 */
QSizeF QImGridNode::size() const
{
    QIM_DC(d);
    return d->size;
}

/**
 * \if ENGLISH
 * @brief Sets the grid area size
 * @param size New grid size
 * \endif
 * \if CHINESE
 * @brief 设置网格区域尺寸
 * @param size 新网格尺寸
 * \endif
 */
void QImGridNode::setSize(const QSizeF& size)
{
    QIM_D(d);
    if (d->size != size) {
        d->size = size;
        Q_EMIT sizeChanged(size);
    }
}

/**
 * \if ENGLISH
 * @brief Gets total number of cells
 * @return rows × columns
 * \endif
 * \if CHINESE
 * @brief 获取总单元格数
 * @return 行数 × 列数
 * \endif
 */
int QImGridNode::gridCount() const
{
    QIM_DC(d);
    return d->rows * d->cols;
}

/**
 * \if ENGLISH
 * @brief Creates a new cell node at next available position
 * @return Pointer to the new CellNode, or nullptr if grid is full
 * \endif
 * \if CHINESE
 * @brief 在下一个可用位置创建新的单元格节点
 * @return 指向新 CellNode 的指针，如果网格已满则返回 nullptr
 * \endif
 */
QImGridNode::CellNode* QImGridNode::createCellNode()
{
    CellNode* cell = new CellNode(cellCount(), static_cast<QObject*>(this));
    this->addChildNode(cell);
    return cell;
}

/**
 * \if ENGLISH
 * @brief Gets all cell nodes
 * @return List of CellNode pointers
 * \endif
 * \if CHINESE
 * @brief 获取所有单元格节点
 * @return CellNode 指针列表
 * \endif
 */
QList<QImGridNode::CellNode*> QImGridNode::cellNodes() const
{
    QList<CellNode*> cells;
    const QList<QImAbstractNode*>& children = this->childrenNodes();
    for (QImAbstractNode* child : children) {
        cells.push_back(static_cast<CellNode*>(child));
    }
    return cells;
}

/**
 * \if ENGLISH
 * @brief Gets the number of populated cells
 * @return Count of cell nodes
 * \endif
 * \if CHINESE
 * @brief 获取已填充的单元格数量
 * @return 单元格节点数量
 * \endif
 */
int QImGridNode::cellCount() const
{
    return cellNodes().size();
}

/**
 * \if ENGLISH
 * @brief Begins grid rendering - captures layout origin and available size
 * @return true to continue rendering children
 * \endif
 * \if CHINESE
 * @brief 开始网格渲染 - 捕获布局原点和可用尺寸
 * @return true 以继续渲染子节点
 * \endif
 */
bool QImGridNode::beginDraw()
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

/**
 * \if ENGLISH
 * @brief Ends grid rendering - restores cursor and occupies grid area
 * \endif
 * \if CHINESE
 * @brief 结束网格渲染 - 恢复光标并占据网格区域
 * \endif
 */
void QImGridNode::endDraw()
{
    QIM_D(d);
    ImGui::SetCursorPos(ImVec2(static_cast<float>(d->origin.x()), static_cast<float>(d->origin.y())));
    ImGui::Dummy(ImVec2(static_cast<float>(d->availableSize.width()), static_cast<float>(d->availableSize.height())));
}

/**
 * \if ENGLISH
 * @brief Calculates pixel position for a cell by index
 * @param index Cell index (row-major order)
 * @return Pixel position relative to grid origin
 * \endif
 * \if CHINESE
 * @brief 根据索引计算单元格的像素位置
 * @param index 单元格索引（行主序）
 * @return 相对于网格原点的像素位置
 * \endif
 */
QPoint QImGridNode::cellPosition(int index) const
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

/**
 * \if ENGLISH
 * @brief Calculates the size of each uniform cell
 * @return Cell size based on available area and grid dimensions
 * \endif
 * \if CHINESE
 * @brief 计算每个统一单元格的尺寸
 * @return 基于可用区域和网格尺寸的单元格尺寸
 * \endif
 */
QSizeF QImGridNode::cellSize() const
{
    QIM_DC(d);
    const double width = (d->cols > 0) ? (d->availableSize.width() / static_cast<double>(d->cols)) : d->availableSize.width();
    const double height = (d->rows > 0) ? (d->availableSize.height() / static_cast<double>(d->rows)) : d->availableSize.height();
    return QSizeF(width, height);
}

}  // namespace QIM
