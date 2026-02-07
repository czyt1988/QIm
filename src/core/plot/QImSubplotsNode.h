#ifndef QIMSUBPLOTSNODE_H
#define QIMSUBPLOTSNODE_H
#include "QImAbstractNode.h"
#include <QSize>
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
    Q_PROPERTY(QSizeF size READ size WRITE setSize)
    // 语义化标志属性（内部映射到单一 ImPlotSubplotFlags）
    Q_PROPERTY(bool isTitleEnabled READ isTitleEnabled WRITE setTitleEnabled NOTIFY titleChanged)
    Q_PROPERTY(bool isLegendEnabled READ isLegendEnabled WRITE setLegendEnabled NOTIFY subplotFlagChanged)
    Q_PROPERTY(bool isDefaultMenusEnabled READ isDefaultMenusEnabled WRITE setDefaultMenusEnabled NOTIFY subplotFlagChanged)
    Q_PROPERTY(bool isResizable READ isResizable WRITE setResizable NOTIFY subplotFlagChanged)
    Q_PROPERTY(bool isAlignedEnabled READ isAlignedEnabled WRITE setAlignedEnabled NOTIFY subplotFlagChanged)
    Q_PROPERTY(bool isShareItemsEnabled READ isShareItemsEnabled WRITE setShareItemsEnabled NOTIFY subplotFlagChanged)

    // 链接行为（自动处理互斥）
    Q_PROPERTY(bool isLinkRows READ isLinkRows WRITE setLinkRows NOTIFY subplotFlagChanged)
    Q_PROPERTY(bool isLinkColumns READ isLinkColumns WRITE setLinkColumns NOTIFY subplotFlagChanged)
    Q_PROPERTY(bool isLinkAllX READ isLinkAllX WRITE setLinkAllX NOTIFY subplotFlagChanged)
    Q_PROPERTY(bool isLinkAllY READ isLinkAllY WRITE setLinkAllY NOTIFY subplotFlagChanged)

    // 布局方向
    Q_PROPERTY(bool isColumnMajor READ isColumnMajor WRITE setColumnMajor NOTIFY subplotFlagChanged)

    Q_DISABLE_COPY(QImSubplotsNode)
    QIM_DECLARE_PRIVATE(QImSubplotsNode);

public:
    explicit QImSubplotsNode(QObject* parent = nullptr);
    explicit QImSubplotsNode(const QString& title, QObject* parent = nullptr);
    ~QImSubplotsNode();
    QString title() const;
    void setTitle(const QString& title);

    int rows() const;
    void setRows(int r);

    int columns() const;
    void setColumns(int c);

    std::vector< float > rowRatios() const;
    void setRowRatios(const std::vector< float >& row_ratios);

    std::vector< float > columnRatios() const;
    void setColumnRatios(const std::vector< float >& col_ratios);

    void setGrid(
        int r,
        int c,
        const std::vector< float >& row_ratios = std::vector< float >(),
        const std::vector< float >& col_ratios = std::vector< float >()
    );

    QSizeF size() const;
    void setSize(const QSizeF& size);

    // 语义化标志属性
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

    // 链接行为
    bool isLinkRows() const;
    void setLinkRows(bool on);

    bool isLinkColumns() const;
    void setLinkColumns(bool on);

    bool isLinkAllX() const;
    void setLinkAllX(bool on);

    bool isLinkAllY() const;
    void setLinkAllY(bool on);
    // 网格数量，plot数量不能超过网格数量
    int gridCount() const;
    // 布局方向
    bool isColumnMajor() const;
    void setColumnMajor(bool on);
    // 获取所有的绘图节点
    QList< QImPlotNode* > plotNodes() const;
    // 创建一个绘图，这个绘图会作为subplot的子节点，如果当前subplot的绘图已经和subplot的管理数量一致，此函数返回nullptr
    QImPlotNode* createPlotNode();
    void addPlotNode(QImPlotNode* plot);
    // 插入绘图，注意plotIndex是subplot节点下面绘图节点的索引，其它节点会跳过,plotIndex可以是-1，则代表在最前面插入，可以大于等于size，代表最后插入
    void insertPlotNode(int plotIndex, QImPlotNode* plot);
Q_SIGNALS:
    /**
     * @brief 标题发生了改变
     * @param title
     */
    void titleChanged(const QString& title);
    /**
     * @brief ImPlot::BeginSubplots对应的ImPlotSubplotFlags发生了改变
     */
    void subplotFlagChanged();
    /**
     * @brief 网格信息发生了改变，网格信息包括行、列、行占比、列占比
     */
    void gridInfoChanged();

protected:
    bool beginDraw() override;
    void endDraw() override;
};

}  // namespace QIM
#endif  // QIMSUBPLOTSNODE_H
