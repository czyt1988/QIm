#ifndef QIMSUBPLOTSNODE_H
#define QIMSUBPLOTSNODE_H

#include "QImAbstractNode.h"
#include <QPoint>
#include <QPointF>
#include <QSizeF>
#include <vector>

namespace QIM
{
class QImPlotNode;

class QIM_CORE_API QImSubplotsNode : public QImAbstractNode
{
    Q_OBJECT

    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(int rows READ rows WRITE setRows NOTIFY gridInfoChanged)
    Q_PROPERTY(int cols READ columns WRITE setColumns NOTIFY gridInfoChanged)
    Q_PROPERTY(QSizeF size READ size WRITE setSize NOTIFY sizeChanged)

public:
    explicit QImSubplotsNode(QObject* parent = nullptr);
    explicit QImSubplotsNode(const QString& title, QObject* parent = nullptr);
    ~QImSubplotsNode() override;

    QString title() const;
    void setTitle(const QString& title);

    int rows() const;
    void setRows(int rows);

    int columns() const;
    void setColumns(int columns);

    std::vector< float > rowRatios() const;
    void setRowRatios(const std::vector< float >& row_ratios);

    std::vector< float > columnRatios() const;
    void setColumnRatios(const std::vector< float >& col_ratios);

    void setGrid(
        int rows,
        int cols,
        const std::vector< float >& row_ratios = std::vector< float >(),
        const std::vector< float >& col_ratios = std::vector< float >()
    );

    QSizeF size() const;
    void setSize(const QSizeF& size);

    int gridCount() const;
    int itemCount() const;

    QImPlotNode* createPlotNode();
    void addPlotNode(QImPlotNode* plot);
    void insertPlotNode(int plotIndex, QImPlotNode* plot);
    int plotNodeSubplotIndex(QImPlotNode* plot) const;
    bool takePlotNode(QImPlotNode* plot);
    void removePlotNode(QImPlotNode* plot);

    QList< QImPlotNode* > plotNodes() const;
    int plotCount() const;

    bool isTitleEnabled() const;
    void setTitleEnabled(bool on);
    bool isLegendEnabled() const;
    void setLegendEnabled(bool on);
    bool isDefaultMenusEnabled() const;
    void setDefaultMenusEnabled(bool on);
    bool isResizable() const;
    void setResizable(bool on);
    bool isAlignedEnabled() const;
    void setAlignedEnabled(bool on);
    bool isShareItemsEnabled() const;
    void setShareItemsEnabled(bool on);
    bool isLinkRows() const;
    void setLinkRows(bool on);
    bool isLinkColumns() const;
    void setLinkColumns(bool on);
    bool isLinkAllX() const;
    void setLinkAllX(bool on);
    bool isLinkAllY() const;
    void setLinkAllY(bool on);
    bool isColumnMajor() const;
    void setColumnMajor(bool on);
    bool isTrackGridRatiosEnabled() const;
    void setTrackGridRatiosEnabled(bool on);

Q_SIGNALS:
    void titleChanged(const QString& title);
    void subplotFlagChanged();
    void gridInfoChanged();
    void sizeChanged(const QSizeF& size);

protected:
    bool beginDraw() override;
    void endDraw() override;

private:
    class CellNode;
    CellNode* createCellNode();
    void updateCellIndices();
    QPoint cellPosition(int index) const;
    QSizeF cellSize() const;

private:
    QByteArray m_titleUtf8;
    int m_rows { 1 };
    int m_cols { 1 };
    QSizeF m_size { -1.0, -1.0 };
    QPointF m_origin { 0.0, 0.0 };
    QSizeF m_availableSize { 0.0, 0.0 };
};
}  // namespace QIM

#endif  // QIMSUBPLOTSNODE_H
