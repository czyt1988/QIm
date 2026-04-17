#include "QImSubplotsNode.h"
#include "implot.h"
#include "QtImGuiUtils.h"
#include <QList>
#include <QDebug>

// qim
#include "QImPlotNode.h"

namespace QIM
{

// PIMPL implementation (completely hides ImPlot dependency)
class QImSubplotsNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImSubplotsNode)

public:
    explicit PrivateData(QImSubplotsNode* q) : q_ptr(q)
    {
    }

    // Property storage (using QByteArray for UTF-8, avoiding conversion during render)
    QByteArray titleUtf8;
    int rows = 1;
    int cols = 1;
    std::vector<float> rowRatios;
    std::vector<float> columnRatios;
    std::vector<float> lastRowRatios;
    std::vector<float> lastColumnRatios;
    constexpr static const float epsilon = 0.001f;

    /**
     * \if ENGLISH
     * @brief Track grid ratio changes during resize
     * @details When enabled, emits gridInfoChanged signal if ratios change.
     * This checks ratios every frame, so use sparingly.
     * \endif
     *
     * \if CHINESE
     * @brief 追踪调整大小期间的网格比例变化
     * @details 启用后，如果比例发生变化则发出 gridInfoChanged 信号。
     * 每帧都会检查比例，因此请谨慎使用。
     * \endif
     */
    bool trackGridRatios {false};

    ImVec2 size = ImVec2(-1, -1);
    ImPlotSubplotFlags subplotFlags {ImPlotSubplotFlags_None};
};

QImSubplotsNode::QImSubplotsNode(QObject* parent)
    : QImAbstractNode(parent)
    , QIM_PIMPL_CONSTRUCT
{
    setObjectName(QStringLiteral("SubplotsNode"));
    setTitle(QStringLiteral("##SubplotsNode"));
}

QImSubplotsNode::QImSubplotsNode(const QString& title, QObject* parent)
    : QImAbstractNode(parent)
    , QIM_PIMPL_CONSTRUCT
{
    setObjectName(QStringLiteral("SubplotsNode"));
    setTitle(title);
}

QImSubplotsNode::~QImSubplotsNode()
{
}

// === Property implementations ===

QString QImSubplotsNode::title() const
{
    QIM_DC(d);
    return QString::fromUtf8(d->titleUtf8);
}

void QImSubplotsNode::setTitle(const QString& title)
{
    QIM_D(d);
    const QByteArray utf8 = title.toUtf8();
    if (d->titleUtf8 != utf8) {
        d->titleUtf8 = utf8;
        Q_EMIT titleChanged(title);
    }
}

int QImSubplotsNode::rows() const
{
    QIM_DC(d);
    return d->rows;
}

void QImSubplotsNode::setRows(int r)
{
    QIM_D(d);
    if (d->rows != r && r > 0) {
        d->rows = r;
        Q_EMIT gridInfoChanged();
    }
}

int QImSubplotsNode::columns() const
{
    QIM_DC(d);
    return d->cols;
}

void QImSubplotsNode::setColumns(int c)
{
    QIM_D(d);
    if (d->cols != c && c > 0) {
        d->cols = c;
        Q_EMIT gridInfoChanged();
    }
}

std::vector<float> QImSubplotsNode::rowRatios() const
{
    return d_ptr->rowRatios;
}

void QImSubplotsNode::setRowRatios(const std::vector<float>& row_ratios)
{
    QIM_D(d);
    if (!fuzzyEqual<float>(d->rowRatios, row_ratios, PrivateData::epsilon)) {
        d->rowRatios = row_ratios;
        d->lastRowRatios = row_ratios;
        Q_EMIT gridInfoChanged();
    }
}

std::vector<float> QImSubplotsNode::columnRatios() const
{
    return d_ptr->columnRatios;
}

void QImSubplotsNode::setColumnRatios(const std::vector<float>& col_ratios)
{
    QIM_D(d);
    if (!fuzzyEqual<float>(d->columnRatios, col_ratios, PrivateData::epsilon)) {
        d->columnRatios = col_ratios;
        d->lastColumnRatios = col_ratios;
        Q_EMIT gridInfoChanged();
    }
}

void QImSubplotsNode::setGrid(int r, int c, const std::vector<float>& row_ratios, const std::vector<float>& col_ratios)
{
    {
        QSignalBlocker b(this);
        setRows(r);
        setColumns(c);
        setRowRatios(row_ratios);
        setColumnRatios(col_ratios);
    }
    // Emit signal only once
    Q_EMIT gridInfoChanged();
}

QSizeF QImSubplotsNode::size() const
{
    QIM_DC(d);
    return toQSizeF(d->size);
}

/**
 * \if ENGLISH
 * @brief Set subplot grid size
 * @param size Grid size specification
 * @details ImPlot::BeginSubplots size parameter specifies total grid size:
 * - ImVec2(w, h) where w>0, h>0: Exact fixed size in pixels
 * - ImVec2(-1, h): Width fills parent, height fixed
 * - ImVec2(w, -1): Width fixed, height fills parent
 * - ImVec2(-1, -1): Fill parent client area (most common)
 * - ImVec2(0, 0): Use default size
 * \endif
 *
 * \if CHINESE
 * @brief 设置子图网格大小
 * @param size 网格大小规格
 * @details ImPlot::BeginSubplots 的 size 参数指定网格总尺寸：
 * - ImVec2(w, h) 其中 w>0, h>0：精确固定像素大小
 * - ImVec2(-1, h)：宽度填满父窗口，高度固定
 * - ImVec2(w, -1)：宽度固定，高度填满父窗口
 * - ImVec2(-1, -1)：填满父窗口客户区（最常用）
 * - ImVec2(0, 0)：使用默认大小
 * \endif
 */
void QImSubplotsNode::setSize(const QSizeF& size)
{
    QIM_D(d);
    ImVec2 newSize = toImVec2(size);

    if (fuzzyEqual(d->size, newSize)) {
        d->size = newSize;
    }
}

// === Semantic flag properties ===

bool QImSubplotsNode::isTitleEnabled() const
{
    QIM_DC(d);
    return !(d->subplotFlags & ImPlotSubplotFlags_NoTitle);
}

void QImSubplotsNode::setTitleEnabled(bool on)
{
    QIM_D(d);
    const bool current = isTitleEnabled();
    if (current != on) {
        if (on) {
            d->subplotFlags &= ~ImPlotSubplotFlags_NoTitle;  // Clear NoTitle flag
        } else {
            d->subplotFlags |= ImPlotSubplotFlags_NoTitle;  // Set NoTitle flag
        }
        Q_EMIT subplotFlagChanged();
    }
}

bool QImSubplotsNode::isLegendEnabled() const
{
    QIM_DC(d);
    return !(d->subplotFlags & ImPlotSubplotFlags_NoLegend);
}

void QImSubplotsNode::setLegendEnabled(bool on)
{
    QIM_D(d);
    const bool current = isLegendEnabled();
    if (current != on) {
        if (on) {
            d->subplotFlags &= ~ImPlotSubplotFlags_NoLegend;
        } else {
            d->subplotFlags |= ImPlotSubplotFlags_NoLegend;
        }
        Q_EMIT subplotFlagChanged();
    }
}

bool QImSubplotsNode::isDefaultMenusEnabled() const
{
    QIM_DC(d);
    return !(d->subplotFlags & ImPlotSubplotFlags_NoMenus);
}

void QImSubplotsNode::setDefaultMenusEnabled(bool on)
{
    QIM_D(d);
    const bool current = isDefaultMenusEnabled();
    if (current != on) {
        if (on) {
            d->subplotFlags &= ~ImPlotSubplotFlags_NoMenus;
        } else {
            d->subplotFlags |= ImPlotSubplotFlags_NoMenus;
        }
        Q_EMIT subplotFlagChanged();
    }
}

bool QImSubplotsNode::isResizable() const
{
    QIM_DC(d);
    return !(d->subplotFlags & ImPlotSubplotFlags_NoResize);
}

void QImSubplotsNode::setResizable(bool on)
{
    QIM_D(d);
    const bool current = isResizable();
    if (current != on) {
        if (on) {
            d->subplotFlags &= ~ImPlotSubplotFlags_NoResize;
        } else {
            d->subplotFlags |= ImPlotSubplotFlags_NoResize;
        }
        Q_EMIT subplotFlagChanged();
    }
}

bool QImSubplotsNode::isAlignedEnabled() const
{
    QIM_DC(d);
    return !(d->subplotFlags & ImPlotSubplotFlags_NoAlign);
}

void QImSubplotsNode::setAlignedEnabled(bool on)
{
    QIM_D(d);
    const bool current = isAlignedEnabled();
    if (current != on) {
        if (on) {
            d->subplotFlags &= ~ImPlotSubplotFlags_NoAlign;
        } else {
            d->subplotFlags |= ImPlotSubplotFlags_NoAlign;
        }
        Q_EMIT subplotFlagChanged();
    }
}

bool QImSubplotsNode::isShareItemsEnabled() const
{
    QIM_DC(d);
    return d->subplotFlags & ImPlotSubplotFlags_ShareItems;
}

void QImSubplotsNode::setShareItemsEnabled(bool on)
{
    QIM_D(d);
    const bool current = isShareItemsEnabled();
    if (current != on) {
        if (on) {
            d->subplotFlags |= ImPlotSubplotFlags_ShareItems;
        } else {
            d->subplotFlags &= ~ImPlotSubplotFlags_ShareItems;
        }
        Q_EMIT subplotFlagChanged();
    }
}

// === Link behavior (smart mutex handling) ===

bool QImSubplotsNode::isLinkRows() const
{
    QIM_DC(d);
    // LinkAllY has higher priority than LinkRows, LinkRows is ineffective when LinkAllY exists
    return (d->subplotFlags & ImPlotSubplotFlags_LinkRows) && !(d->subplotFlags & ImPlotSubplotFlags_LinkAllY);
}

void QImSubplotsNode::setLinkRows(bool on)
{
    QIM_D(d);
    const bool current = isLinkRows();
    if (current != on) {
        if (on) {
            // Enable LinkRows, disable mutex LinkAllY
            d->subplotFlags |= ImPlotSubplotFlags_LinkRows;
            d->subplotFlags &= ~ImPlotSubplotFlags_LinkAllY;
        } else {
            d->subplotFlags &= ~ImPlotSubplotFlags_LinkRows;
        }
        Q_EMIT subplotFlagChanged();
    }
}

bool QImSubplotsNode::isLinkColumns() const
{
    QIM_DC(d);
    // LinkAllX has higher priority than LinkCols, LinkCols is ineffective when LinkAllX exists
    return (d->subplotFlags & ImPlotSubplotFlags_LinkCols) && !(d->subplotFlags & ImPlotSubplotFlags_LinkAllX);
}

void QImSubplotsNode::setLinkColumns(bool on)
{
    QIM_D(d);
    const bool current = isLinkColumns();
    if (current != on) {
        if (on) {
            // Enable LinkCols, disable mutex LinkAllX
            d->subplotFlags |= ImPlotSubplotFlags_LinkCols;
            d->subplotFlags &= ~ImPlotSubplotFlags_LinkAllX;
        } else {
            d->subplotFlags &= ~ImPlotSubplotFlags_LinkCols;
        }
        Q_EMIT subplotFlagChanged();
    }
}

bool QImSubplotsNode::isLinkAllX() const
{
    QIM_DC(d);
    return d->subplotFlags & ImPlotSubplotFlags_LinkAllX;
}

void QImSubplotsNode::setLinkAllX(bool on)
{
    QIM_D(d);
    const bool current = isLinkAllX();
    if (current != on) {
        if (on) {
            // Enable LinkAllX, disable mutex LinkCols
            d->subplotFlags |= ImPlotSubplotFlags_LinkAllX;
            d->subplotFlags &= ~ImPlotSubplotFlags_LinkCols;
        } else {
            d->subplotFlags &= ~ImPlotSubplotFlags_LinkAllX;
        }
        Q_EMIT subplotFlagChanged();
    }
}

bool QImSubplotsNode::isLinkAllY() const
{
    QIM_DC(d);
    return d->subplotFlags & ImPlotSubplotFlags_LinkAllY;
}

void QImSubplotsNode::setLinkAllY(bool on)
{
    QIM_D(d);
    const bool current = isLinkAllY();
    if (current != on) {
        if (on) {
            // Enable LinkAllY, disable mutex LinkRows
            d->subplotFlags |= ImPlotSubplotFlags_LinkAllY;
            d->subplotFlags &= ~ImPlotSubplotFlags_LinkRows;
        } else {
            d->subplotFlags &= ~ImPlotSubplotFlags_LinkAllY;
        }
        Q_EMIT subplotFlagChanged();
    }
}

int QImSubplotsNode::gridCount() const
{
    return (d_ptr->cols * d_ptr->rows);
}

// === Layout direction ===

bool QImSubplotsNode::isColumnMajor() const
{
    QIM_DC(d);
    return d->subplotFlags & ImPlotSubplotFlags_ColMajor;
}

void QImSubplotsNode::setColumnMajor(bool on)
{
    QIM_D(d);
    const bool current = isColumnMajor();
    if (current != on) {
        if (on) {
            d->subplotFlags |= ImPlotSubplotFlags_ColMajor;
        } else {
            d->subplotFlags &= ~ImPlotSubplotFlags_ColMajor;
        }
        Q_EMIT subplotFlagChanged();
    }
}

// === Plot node management ===

QList<QImPlotNode*> QImSubplotsNode::plotNodes() const
{
    const QList<QImAbstractNode*> cns = childrenNodes();
    QList<QImPlotNode*> res;
    for (QImAbstractNode* n : cns) {
        if (QImPlotNode* pn = qobject_cast<QImPlotNode*>(n)) {
            res.push_back(pn);
        }
    }
    return res;
}

void QImSubplotsNode::addPlotNode(QImPlotNode* plot)
{
    if (!plot) {
        return;
    }
    insertChildNode(childNodeCount(), plot);
}

void QImSubplotsNode::insertPlotNode(int plotIndex, QImPlotNode* plot)
{
    if (!plot) {
        return;
    }
    QList<QImPlotNode*> ps = plotNodes();
    if (plotIndex < ps.size()) {
        if (plotIndex < 0) {
            insertChildNode(0, plot);
        } else {
            int t = indexOfChildNode(ps[plotIndex]);
            if (t >= 0) {
                plotIndex = t + 1;
            } else {
                // Unreachable position
                plotIndex = 0;
            }
        }
    } else {
        plotIndex = childNodeCount();
    }
    insertChildNode(plotIndex, plot);
}

int QImSubplotsNode::plotCount() const
{
    return plotNodes().size();
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
    // Check if plot is a direct child
    if (!childrenNodes().contains(plot)) {
        return false;
    }
    // Take child without destroying it
    takeChildNode(plot);
    return true;
}

void QImSubplotsNode::removePlotNode(QImPlotNode* plot)
{
    if (!plot) {
        return;
    }
    // Check if plot is a direct child
    if (!childrenNodes().contains(plot)) {
        return;
    }
    // Remove and destroy child
    removeChildNode(plot);
}

bool QImSubplotsNode::isTrackGridRatiosEnabled() const
{
    return d_ptr->trackGridRatios;
}

void QImSubplotsNode::setTrackGridRatiosEnabled(bool on)
{
    d_ptr->trackGridRatios = on;
}

QImPlotNode* QImSubplotsNode::createPlotNode()
{
    const QList<QImPlotNode*> pns = plotNodes();
    if (pns.size() >= gridCount()) {
        // Plot count equals grid count, cannot add more
        return nullptr;
    }
    QImPlotNode* p = new QImPlotNode();
    addPlotNode(p);
    return p;
}

// === Render implementation ===

bool QImSubplotsNode::beginDraw()
{
    // Call ImPlot API (UTF-8 cache, zero overhead)
    QIM_D(d);

    float* row_ratios = nullptr;
    float* col_ratios = nullptr;

    if (!d->rowRatios.empty() && (static_cast<int>(d->rowRatios.size()) == d->rows)) {
        row_ratios = d->rowRatios.data();
    }
    if (!d->columnRatios.empty() && (static_cast<int>(d->columnRatios.size()) == d->cols)) {
        col_ratios = d->columnRatios.data();
    }

    bool on = ImPlot::BeginSubplots(
        d->titleUtf8.isEmpty() ? "##Subplots" : d->titleUtf8.constData(),
        d->rows,
        d->cols,
        d->size,
        d->subplotFlags,
        row_ratios,
        col_ratios
    );

    // Detect if row_ratios and col_ratios changed, emit gridInfoChanged signal
    if (d->trackGridRatios) {
        bool isGridChanged {false};
        if (row_ratios) {
            if (!fuzzyEqual(d->rowRatios, d->lastRowRatios, PrivateData::epsilon)) {
                d->lastRowRatios = d->rowRatios;
                isGridChanged = true;
            }
        }
        if (col_ratios) {
            if (!fuzzyEqual(d->columnRatios, d->lastColumnRatios, PrivateData::epsilon)) {
                d->lastColumnRatios = d->columnRatios;
                isGridChanged = true;
            }
        }
        if (isGridChanged) {
            Q_EMIT gridInfoChanged();
        }
    }

    return on;
}

void QImSubplotsNode::endDraw()
{
    ImPlot::EndSubplots();
}

}  // namespace QIM