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
    /**
     * \if ENGLISH
     * @brief Construct a subplot node with default settings
     * @param parent Parent object
     * \endif
     *
     * \if CHINESE
     * @brief 构造具有默认设置的子图节点
     * @param parent 父对象
     * \endif
     */
    explicit QImSubplotsNode(QObject* parent = nullptr);

    /**
     * \if ENGLISH
     * @brief Construct a subplot node with a title
     * @param title Subplot title
     * @param parent Parent object
     * \endif
     *
     * \if CHINESE
     * @brief 构造具有标题的子图节点
     * @param title 子图标题
     * @param parent 父对象
     * \endif
     */
    explicit QImSubplotsNode(const QString& title, QObject* parent = nullptr);

    /**
     * \if ENGLISH
     * @brief Destructor
     * \endif
     *
     * \if CHINESE
     * @brief 析构函数
     * \endif
     */
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

    /**
     * \if ENGLISH
     * @brief Set grid dimensions and ratios in one call
     * @param r Number of rows
     * @param c Number of columns
     * @param row_ratios Optional row height ratios
     * @param col_ratios Optional column width ratios
     * @details Emits gridInfoChanged signal only once after all changes.
     * \endif
     *
     * \if CHINESE
     * @brief 在一次调用中设置网格尺寸和比例
     * @param r 行数
     * @param c 列数
     * @param row_ratios 可选的行高度比例
     * @param col_ratios 可选的列宽度比例
     * @details 所有更改后仅发出一次 gridInfoChanged 信号。
     * \endif
     */
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

    /**
     * \if ENGLISH
     * @brief Get total number of grid cells
     * @return rows * columns
     * \endif
     *
     * \if CHINESE
     * @brief 获取网格单元格总数
     * @return 行数 * 列数
     * \endif
     */
    int gridCount() const;

    /**
     * \if ENGLISH
     * @brief Get all plot nodes in this subplot
     * @return List of QImPlotNode children
     * \endif
     *
     * \if CHINESE
     * @brief 获取此子图中的所有绘图节点
     * @return QImPlotNode 子节点列表
     * \endif
     */
    QList<QImPlotNode*> plotNodes() const;

    /**
     * \if ENGLISH
     * @brief Create a new plot node as direct child
     * @return New QImPlotNode, or nullptr if grid is full
     * @details Creates QImPlotNode as a direct child of this subplot node.
     * ImPlot::BeginSubplots handles cell positioning automatically.
     * \endif
     *
     * \if CHINESE
     * @brief 创建新的绘图节点作为直接子节点
     * @return 新的 QImPlotNode，如果网格已满则返回 nullptr
     * @details 创建 QImPlotNode 作为此子图节点的直接子节点。
     * ImPlot::BeginSubplots 自动处理单元格定位。
     * \endif
     */
    QImPlotNode* createPlotNode();

    /**
     * \if ENGLISH
     * @brief Add an existing plot node as direct child
     * @param plot Plot node to add
     * \endif
     *
     * \if CHINESE
     * @brief 将现有绘图节点添加为直接子节点
     * @param plot 要添加的绘图节点
     * \endif
     */
    void addPlotNode(QImPlotNode* plot);

    /**
     * \if ENGLISH
     * @brief Insert a plot node at specified position
     * @param plotIndex Index among plot nodes (not all children)
     * @param plot Plot node to insert
     * @details plotIndex can be -1 (insert at front) or >= size (append at end).
     * \endif
     *
     * \if CHINESE
     * @brief 在指定位置插入绘图节点
     * @param plotIndex 绘图节点中的索引（非所有子节点）
     * @param plot 要插入的绘图节点
     * @details plotIndex 可以是 -1（在最前面插入）或 >= size（在末尾追加）。
     * \endif
     */
    void insertPlotNode(int plotIndex, QImPlotNode* plot);

    /**
     * \if ENGLISH
     * @brief Get index of a plot node among plot nodes
     * @param plot Plot node to find
     * @return Index among plot nodes, or -1 if not found
     * \endif
     *
     * \if CHINESE
     * @brief 获取绘图节点在绘图节点中的索引
     * @param plot 要查找的绘图节点
     * @return 绘图节点中的索引，如果未找到则返回 -1
     * \endif
     */
    int plotNodeSubplotIndex(QImPlotNode* plot) const;

    /**
     * \if ENGLISH
     * @brief Take a plot node out without destroying it
     * @param plot Plot node to take
     * @return true if successfully taken, false otherwise
     * \endif
     *
     * \if CHINESE
     * @brief 取出绘图节点但不销毁它
     * @param plot 要取出的绘图节点
     * @return 如果成功取出返回 true，否则返回 false
     * \endif
     */
    bool takePlotNode(QImPlotNode* plot);

    /**
     * \if ENGLISH
     * @brief Remove and destroy a plot node
     * @param plot Plot node to remove
     * \endif
     *
     * \if CHINESE
     * @brief 移除并销毁绘图节点
     * @param plot 要移除的绘图节点
     * \endif
     */
    void removePlotNode(QImPlotNode* plot);

    /**
     * \if ENGLISH
     * @brief Get count of plot nodes
     * @return Number of QImPlotNode children
     * \endif
     *
     * \if CHINESE
     * @brief 获取绘图节点数量
     * @return QImPlotNode 子节点数量
     * \endif
     */
    int plotCount() const;

    /**
     * \if ENGLISH
     * @brief Check if grid ratio tracking is enabled
     * @return true if tracking enabled
     * \endif
     *
     * \if CHINESE
     * @brief 检查是否启用网格比例追踪
     * @return 如果启用追踪返回 true
     * \endif
     */
    bool isTrackGridRatiosEnabled() const;

    /**
     * \if ENGLISH
     * @brief Enable/disable grid ratio tracking
     * @param on Enable tracking
     * @details When enabled, emits gridInfoChanged if ratios change during resize.
     * This checks ratios every frame, so use sparingly.
     * \endif
     *
     * \if CHINESE
     * @brief 启用/禁用网格比例追踪
     * @param on 启用追踪
     * @details 启用后，如果比例在调整大小期间发生变化，则发出 gridInfoChanged。
     * 每帧都会检查比例，因此请谨慎使用。
     * \endif
     */
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
    /**
     * \if ENGLISH
     * @brief Begin drawing - calls ImPlot::BeginSubplots
     * @return true if subplot should render children
     * \endif
     *
     * \if CHINESE
     * @brief 开始绘制 - 调用 ImPlot::BeginSubplots
     * @return 如果子图应渲染子节点返回 true
     * \endif
     */
    bool beginDraw() override;

    /**
     * \if ENGLISH
     * @brief End drawing - calls ImPlot::EndSubplots
     * \endif
     *
     * \if CHINESE
     * @brief 结束绘制 - 调用 ImPlot::EndSubplots
     * \endif
     */
    void endDraw() override;
};

}  // namespace QIM

#endif  // QIMSUBPLOTSNODE_H