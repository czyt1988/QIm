#ifndef LINE10KFUNCTION_H
#define LINE10KFUNCTION_H

#include "TestFunction.h"
#include <QObject>
#include <QColor>

namespace QIM {
class QImFigureWidget;
}

/**
 * \if ENGLISH
 * @brief Test function demonstrating 10K point line plot with property registration
 * 
 * Line10KFunction creates a plot with 10,000 cosine wave points to test
 * high-performance rendering capabilities. It registers properties for:
 * - Plot title
 * - X and Y axis labels
 * - Line color
 * - Line label
 * 
 * @section usage_example Usage Example
 * 
 * \code
 * auto func = new Line10KFunction(this);
 * func->createPlot(figureWidget);
 * // Properties will be available via getRegisteredProperties()
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotNode
 * @see QImPlotLineItemNode
 * \endif
 * 
 * \if CHINESE
 * @brief 演示 10K 点线图的测试函数，带有属性注册
 * 
 * Line10KFunction 创建一个包含 10,000 个余弦波点的图表，用于测试
 * 高性能渲染能力。它注册了以下属性：
 * - 图表标题
 * - X 和 Y 轴标签
 * - 线条颜色
 * - 线条标签
 * 
 * @section usage_example 使用示例
 * 
 * \code
 * auto func = new Line10KFunction(this);
 * func->createPlot(figureWidget);
 * // 属性可通过 getRegisteredProperties() 获取
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotNode
 * @see QImPlotLineItemNode
 * \endif
 */
class Line10KFunction : public TestFunction {
    Q_OBJECT
    
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString xLabel READ xLabel WRITE setXLabel NOTIFY xLabelChanged)
    Q_PROPERTY(QString yLabel READ yLabel WRITE setYLabel NOTIFY yLabelChanged)
    Q_PROPERTY(QColor lineColor READ lineColor WRITE setLineColor NOTIFY lineColorChanged)
    Q_PROPERTY(QString lineLabel READ lineLabel WRITE setLineLabel NOTIFY lineLabelChanged)
    
public:
    /**
     * \if ENGLISH
     * @brief Constructor for Line10KFunction
     * @param parent Parent object
     * \endif
     * 
     * \if CHINESE
     * @brief Line10KFunction 构造函数
     * @param parent 父对象
     * \endif
     */
    explicit Line10KFunction(QObject* parent = nullptr);
    
    /**
     * \if ENGLISH
     * @brief Destructor for Line10KFunction
     * \endif
     * 
     * \if CHINESE
     * @brief Line10KFunction 析构函数
     * \endif
     */
    ~Line10KFunction() override;
    
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
    
    // Line color property accessors
    QColor lineColor() const { return m_lineColor; }
    void setLineColor(const QColor& color);
    
    // Line label property accessors
    QString lineLabel() const { return m_lineLabel; }
    void setLineLabel(const QString& label);
    
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
     * @brief Signal emitted when line color changes
     * @param color New line color value
     * \endif
     * 
     * \if CHINESE
     * @brief 线条颜色改变时发出的信号
     * @param color 新的线条颜色值
     * \endif
     */
    void lineColorChanged(const QColor& color);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when line label changes
     * @param label New line label value
     * \endif
     * 
     * \if CHINESE
     * @brief 线条标签改变时发出的信号
     * @param label 新的线条标签值
     * \endif
     */
    void lineLabelChanged(const QString& label);
    
private:
    QString m_title = QStringLiteral("10K Points");
    QString m_xLabel = QStringLiteral("x1");
    QString m_yLabel = QStringLiteral("y1");
    QColor m_lineColor = Qt::blue;
    QString m_lineLabel = QStringLiteral("curve a");
    
    QIM::QImPlotNode* m_plotNode = nullptr;
    QIM::QImPlotLineItemNode* m_lineNode = nullptr;
    QIM::QImPlotValueTrackerNode* m_trackerNode = nullptr;
};

#endif // LINE10KFUNCTION_H
