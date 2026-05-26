#ifndef QIMSUBPLOTSNODE_H
#define QIMSUBPLOTSNODE_H

#include "QImAbstractNode.h"
#include <QSize>
#include <vector>

namespace QIM
{
class QImPlotNode;

/**
 * \if ENGLISH
 * @brief ImPlot subplot grid node
 *
 * Manages a grid of 2D plot cells using ImPlot::BeginSubplots API.
 * Each cell contains a QImPlotNode that renders independently.
 *
 * Key features:
 * - Automatic cell management via BeginSubplots (no manual layout)
 * - Flag properties with Qt-affirmative semantics (xxxEnabled)
 * - Link axis behavior with mutex handling
 * - Grid ratio tracking for resize detection
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot 子图网格节点
 *
 * 使用 ImPlot::BeginSubplots API 管理 2D 绘图单元格网格。
 * 每个单元格包含一个独立渲染的 QImPlotNode。
 *
 * 主要特性：
 * - 通过 BeginSubplots 自动管理单元格（无需手动布局）
 * - Qt 肯定语义的标志属性（xxxEnabled）
 * - 具有互斥处理的坐标轴链接行为
 * - 网格比例追踪用于检测调整大小
 * \endif
 */
class QIM_CORE_API QImSubplotsNode : public QImAbstractNode
{
    Q_OBJECT

    // == Basic properties ==
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(int rows READ rows WRITE setRows NOTIFY gridInfoChanged)
    Q_PROPERTY(int cols READ columns WRITE setColumns NOTIFY gridInfoChanged)
    Q_PROPERTY(QSizeF size READ size WRITE setSize)

    // == Flag properties (Qt-affirmative semantics, mapping from ImPlotSubplotFlags) ==
    Q_PROPERTY(bool isTitleEnabled READ isTitleEnabled WRITE setTitleEnabled NOTIFY titleChanged)
    Q_PROPERTY(bool isLegendEnabled READ isLegendEnabled WRITE setLegendEnabled NOTIFY subplotFlagChanged)
    Q_PROPERTY(bool isDefaultMenusEnabled READ isDefaultMenusEnabled WRITE setDefaultMenusEnabled NOTIFY subplotFlagChanged)
    Q_PROPERTY(bool isResizable READ isResizable WRITE setResizable NOTIFY subplotFlagChanged)
    Q_PROPERTY(bool isAlignedEnabled READ isAlignedEnabled WRITE setAlignedEnabled NOTIFY subplotFlagChanged)
    Q_PROPERTY(bool isShareItemsEnabled READ isShareItemsEnabled WRITE setShareItemsEnabled NOTIFY subplotFlagChanged)

    // == Link behavior (automatic mutex handling) ==
    Q_PROPERTY(bool isLinkRows READ isLinkRows WRITE setLinkRows NOTIFY subplotFlagChanged)
    Q_PROPERTY(bool isLinkColumns READ isLinkColumns WRITE setLinkColumns NOTIFY subplotFlagChanged)
    Q_PROPERTY(bool isLinkAllX READ isLinkAllX WRITE setLinkAllX NOTIFY subplotFlagChanged)
    Q_PROPERTY(bool isLinkAllY READ isLinkAllY WRITE setLinkAllY NOTIFY subplotFlagChanged)

    // == Layout direction ==
    Q_PROPERTY(bool isColumnMajor READ isColumnMajor WRITE setColumnMajor NOTIFY subplotFlagChanged)

    Q_DISABLE_COPY(QImSubplotsNode)
    QIM_DECLARE_PRIVATE(QImSubplotsNode)

public:
    // Construct a subplot node with default settings
    explicit QImSubplotsNode(QObject* parent = nullptr);

    // Construct a subplot node with a title
    explicit QImSubplotsNode(const QString& title, QObject* parent = nullptr);

    // Destructor
    ~QImSubplotsNode();

    // == Title ==
    QString title() const;
    void setTitle(const QString& title);

    // == Grid dimensions ==
    int rows() const;
    void setRows(int r);

    int columns() const;
    void setColumns(int c);

    // == Grid ratios ==
    std::vector<float> rowRatios() const;
    void setRowRatios(const std::vector<float>& row_ratios);

    std::vector<float> columnRatios() const;
    void setColumnRatios(const std::vector<float>& col_ratios);

    // Set grid dimensions and ratios in one call
    void setGrid(int r,
                 int c,
                 const std::vector<float>& row_ratios = std::vector<float>(),
                 const std::vector<float>& col_ratios = std::vector<float>());

    // == Size ==
    QSizeF size() const;
    void setSize(const QSizeF& size);

    // == Flag properties (Qt-affirmative semantics) ==
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

    // == Link behavior (mutex handling) ==
    bool isLinkRows() const;
    void setLinkRows(bool on);

    bool isLinkColumns() const;
    void setLinkColumns(bool on);

    bool isLinkAllX() const;
    void setLinkAllX(bool on);

    bool isLinkAllY() const;
    void setLinkAllY(bool on);

    // == Layout direction ==
    bool isColumnMajor() const;
    void setColumnMajor(bool on);

    // Get total number of grid cells
    int gridCount() const;

    // Get all plot nodes in this subplot
    QList<QImPlotNode*> plotNodes() const;

    // Create a new plot node as direct child
    QImPlotNode* createPlotNode();

    // Add an existing plot node as direct child
    void addPlotNode(QImPlotNode* plot);

    // Insert a plot node at specified position
    void insertPlotNode(int plotIndex, QImPlotNode* plot);

    // Get index of a plot node among plot nodes
    int plotNodeSubplotIndex(QImPlotNode* plot) const;

    // Take a plot node out without destroying it
    bool takePlotNode(QImPlotNode* plot);

    // Remove and destroy a plot node
    void removePlotNode(QImPlotNode* plot);

    // Get count of plot nodes
    int plotCount() const;

    // Check if grid ratio tracking is enabled
    bool isTrackGridRatiosEnabled() const;

    // Enable/disable grid ratio tracking
    void setTrackGridRatiosEnabled(bool on);

Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Title changed signal
     * @param title New title
     * \endif
     *
     * \if CHINESE
     * @brief 标题更改信号
     * @param title 新标题
     * \endif
     */
    void titleChanged(const QString& title);

    /**
     * \if ENGLISH
     * @brief ImPlotSubplotFlags changed signal
     * \endif
     *
     * \if CHINESE
     * @brief ImPlotSubplotFlags 更改信号
     * \endif
     */
    void subplotFlagChanged();

    /**
     * \if ENGLISH
     * @brief Grid info changed signal (rows, cols, ratios)
     * \endif
     *
     * \if CHINESE
     * @brief 网格信息更改信号（行、列、比例）
     * \endif
     */
    void gridInfoChanged();

protected:
    // Begin drawing - calls ImPlot::BeginSubplots
    bool beginDraw() override;

    // End drawing - calls ImPlot::EndSubplots
    void endDraw() override;
};

}  // namespace QIM

#endif  // QIMSUBPLOTSNODE_H