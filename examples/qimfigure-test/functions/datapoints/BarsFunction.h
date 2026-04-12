#ifndef BARSFUNCTION_H
#define BARSFUNCTION_H

#include "../TestFunction.h"
#include <QObject>
#include <QColor>

namespace QIM {
class QImFigureWidget;
class QImPlotNode;
class QImPlotBarsItemNode;
class QImPlotValueTrackerNode;
}

/**
 * \if ENGLISH
 * @brief Test function demonstrating bar chart with property registration
 * 
 * BarsFunction creates a bar chart with 12 bars to test bar chart rendering
 * capabilities. It registers properties for:
 * - Plot title
 * - X and Y axis labels
 * - Bar width
 * - Bar color
 * 
 * @section usage_example Usage Example
 * 
 * \code
 * auto func = new BarsFunction(this);
 * func->createPlot(figureWidget);
 * // Properties will be available via getRegisteredProperties()
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotNode
 * @see QImPlotBarsItemNode
 * \endif
 * 
 * \if CHINESE
 * @brief 演示柱状图的测试函数，带有属性注册
 * 
 * BarsFunction 创建一个包含 12 个柱子的图表，用于测试柱状图渲染能力。
 * 它注册了以下属性：
 * - 图表标题
 * - X 和 Y 轴标签
 * - 柱子宽度
 * - 柱子颜色
 * 
 * @section usage_example 使用示例
 * 
 * \code
 * auto func = new BarsFunction(this);
 * func->createPlot(figureWidget);
 * // 属性可通过 getRegisteredProperties() 获取
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotNode
 * @see QImPlotBarsItemNode
 * \endif
 */
class BarsFunction : public TestFunction {
    Q_OBJECT
    
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString xLabel READ xLabel WRITE setXLabel NOTIFY xLabelChanged)
    Q_PROPERTY(QString yLabel READ yLabel WRITE setYLabel NOTIFY yLabelChanged)
    Q_PROPERTY(double barWidth READ barWidth WRITE setBarWidth NOTIFY barWidthChanged)
    Q_PROPERTY(QColor barColor READ barColor WRITE setBarColor NOTIFY barColorChanged)
    
public:
    /**
     * \if ENGLISH
     * @brief Constructor for BarsFunction
     * @param parent Parent object
     * \endif
     * 
     * \if CHINESE
     * @brief BarsFunction 构造函数
     * @param parent 父对象
     * \endif
     */
    explicit BarsFunction(QObject* parent = nullptr);
    
    /**
     * \if ENGLISH
     * @brief Destructor for BarsFunction
     * \endif
     * 
     * \if CHINESE
     * @brief BarsFunction 析构函数
     * \endif
     */
    ~BarsFunction() override;
    
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
    
    // Bar width property accessors
    double barWidth() const { return m_barWidth; }
    void setBarWidth(double width);
    
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
     * @brief Signal emitted when bar width changes
     * @param width New bar width value
     * \endif
     * 
     * \if CHINESE
     * @brief 柱子宽度改变时发出的信号
     * @param width 新的柱子宽度值
     * \endif
     */
    void barWidthChanged(double width);
    
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
    QString m_title = QStringLiteral("Bar Chart");
    QString m_xLabel = QStringLiteral("x");
    QString m_yLabel = QStringLiteral("y");
    double m_barWidth = 0.6;
    QColor m_barColor = Qt::green;
    
    QIM::QImPlotNode* m_plotNode = nullptr;
    QIM::QImPlotBarsItemNode* m_barsNode = nullptr;
    QIM::QImPlotValueTrackerNode* m_trackerNode = nullptr;
};

#endif // BARSFUNCTION_H
