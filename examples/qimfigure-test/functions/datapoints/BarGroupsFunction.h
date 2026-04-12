#ifndef BARGROUPSFUNCTION_H
#define BARGROUPSFUNCTION_H

#include "../TestFunction.h"
#include <QObject>
#include <QColor>

namespace QIM {
class QImFigureWidget;
class QImPlotNode;
class QImPlotBarGroupsItemNode;
class QImPlotValueTrackerNode;
}

/**
 * \if ENGLISH
 * @brief Test function demonstrating grouped bar chart with property registration
 * 
 * BarGroupsFunction creates a grouped bar chart with 3 items and 4 groups to test
 * grouped bar chart rendering capabilities. It registers properties for:
 * - Plot title
 * - X and Y axis labels
 * - Group width
 * - Horizontal orientation
 * - Stacked bars
 * - Shift offset
 * - Bar color
 * 
 * @section usage_example Usage Example
 * 
 * \code
 * auto func = new BarGroupsFunction(this);
 * func->createPlot(figureWidget);
 * // Properties will be available via getRegisteredProperties()
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotNode
 * @see QImPlotBarGroupsItemNode
 * \endif
 * 
 * \if CHINESE
 * @brief 演示分组柱状图的测试函数，带有属性注册
 * 
 * BarGroupsFunction 创建一个包含 3 个项目、4 个组的分组柱状图，用于测试分组柱状图渲染能力。
 * 它注册了以下属性：
 * - 图表标题
 * - X 和 Y 轴标签
 * - 组宽
 * - 水平方向
 * - 堆叠柱状图
 * - 偏移量
 * - 柱子颜色
 * 
 * @section usage_example 使用示例
 * 
 * \code
 * auto func = new BarGroupsFunction(this);
 * func->createPlot(figureWidget);
 * // 属性可通过 getRegisteredProperties() 获取
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotNode
 * @see QImPlotBarGroupsItemNode
 * \endif
 */
class BarGroupsFunction : public TestFunction {
    Q_OBJECT
    
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString xLabel READ xLabel WRITE setXLabel NOTIFY xLabelChanged)
    Q_PROPERTY(QString yLabel READ yLabel WRITE setYLabel NOTIFY yLabelChanged)
    Q_PROPERTY(double groupWidth READ groupWidth WRITE setGroupWidth NOTIFY groupWidthChanged)
    Q_PROPERTY(bool horizontal READ isHorizontal WRITE setHorizontal NOTIFY horizontalChanged)
    Q_PROPERTY(bool stacked READ isStacked WRITE setStacked NOTIFY stackedChanged)
    Q_PROPERTY(double shift READ shift WRITE setShift NOTIFY shiftChanged)
    Q_PROPERTY(QColor barColor READ barColor WRITE setBarColor NOTIFY barColorChanged)
    
public:
    /**
     * \if ENGLISH
     * @brief Constructor for BarGroupsFunction
     * @param parent Parent object
     * \endif
     * 
     * \if CHINESE
     * @brief BarGroupsFunction 构造函数
     * @param parent 父对象
     * \endif
     */
    explicit BarGroupsFunction(QObject* parent = nullptr);
    
    /**
     * \if ENGLISH
     * @brief Destructor for BarGroupsFunction
     * \endif
     * 
     * \if CHINESE
     * @brief BarGroupsFunction 析构函数
     * \endif
     */
    ~BarGroupsFunction() override;
    
    /**
     * \if ENGLISH
     * @brief Create plot nodes in the given figure widget
     * @param figure Pointer to the figure widget where plots will be created
     * \endif
     * 
     * \if CHINESE
     * @brief 在指定的图表控件中创建绘图节点
     * @param figure 将要创建绘图的图表控件指针
     * \endif
     */
    void createPlot(QIM::QImFigureWidget* figure) override;
    
    // Title property accessors
    QString title() const { return m_title; }
    void setTitle(const QString& title);
    
    // X axis label property accessors
    QString xLabel() const { return m_xLabel; }
    void setXLabel(const QString& label);
    
    // Y axis label property accessors
    QString yLabel() const { return m_yLabel; }
    void setYLabel(const QString& label);
    
    // Group width property accessors
    double groupWidth() const { return m_groupWidth; }
    void setGroupWidth(double width);
    
    // Horizontal orientation property accessors
    bool isHorizontal() const { return m_horizontal; }
    void setHorizontal(bool horizontal);
    
    // Stacked bars property accessors
    bool isStacked() const { return m_stacked; }
    void setStacked(bool stacked);
    
    // Shift offset property accessors
    double shift() const { return m_shift; }
    void setShift(double shift);
    
    // Bar color property accessors
    QColor barColor() const { return m_barColor; }
    void setBarColor(const QColor& color);
    
Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Signal emitted when title changes
     * @param title New title value
     * \endif
     * 
     * \if CHINESE
     * @brief 标题改变时发出的信号
     * @param title 新的标题值
     * \endif
     */
    void titleChanged(const QString& title);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when X axis label changes
     * @param label New X axis label value
     * \endif
     * 
     * \if CHINESE
     * @brief X 轴标签改变时发出的信号
     * @param label 新的 X 轴标签值
     * \endif
     */
    void xLabelChanged(const QString& label);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when Y axis label changes
     * @param label New Y axis label value
     * \endif
     * 
     * \if CHINESE
     * @brief Y 轴标签改变时发出的信号
     * @param label 新的 Y 轴标签值
     * \endif
     */
    void yLabelChanged(const QString& label);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when group width changes
     * @param width New group width value
     * \endif
     * 
     * \if CHINESE
     * @brief 组宽改变时发出的信号
     * @param width 新的组宽值
     * \endif
     */
    void groupWidthChanged(double width);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when horizontal orientation changes
     * @param horizontal New horizontal orientation value
     * \endif
     * 
     * \if CHINESE
     * @brief 水平方向改变时发出的信号
     * @param horizontal 新的水平方向值
     * \endif
     */
    void horizontalChanged(bool horizontal);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when stacked flag changes
     * @param stacked New stacked flag value
     * \endif
     * 
     * \if CHINESE
     * @brief 堆叠标志改变时发出的信号
     * @param stacked 新的堆叠标志值
     * \endif
     */
    void stackedChanged(bool stacked);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when shift offset changes
     * @param shift New shift offset value
     * \endif
     * 
     * \if CHINESE
     * @brief 偏移量改变时发出的信号
     * @param shift 新的偏移量值
     * \endif
     */
    void shiftChanged(double shift);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when bar color changes
     * @param color New bar color value
     * \endif
     * 
     * \if CHINESE
     * @brief 柱子颜色改变时发出的信号
     * @param color 新的柱子颜色值
     * \endif
     */
    void barColorChanged(const QColor& color);
    
private:
    QString m_title = QStringLiteral("Grouped Bar Chart");
    QString m_xLabel = QStringLiteral("Groups");
    QString m_yLabel = QStringLiteral("Values");
    double m_groupWidth = 0.67;
    bool m_horizontal = false;
    bool m_stacked = false;
    double m_shift = 0.0;
    QColor m_barColor = Qt::green;
    
    QIM::QImPlotNode* m_plotNode = nullptr;
    QIM::QImPlotBarGroupsItemNode* m_barGroupsNode = nullptr;
    QIM::QImPlotValueTrackerNode* m_trackerNode = nullptr;
};

#endif // BARGROUPSFUNCTION_H