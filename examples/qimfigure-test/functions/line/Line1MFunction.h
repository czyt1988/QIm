#ifndef LINE1MFUNCTION_H
#define LINE1MFUNCTION_H

#include "TestFunction.h"
#include <QObject>
#include <QColor>

namespace QIM {
class QImFigureWidget;
}

/**
 * \if ENGLISH
 * @brief Test function demonstrating 1M point line plot with two curves
 * 
 * Line1MFunction creates a plot with 1,000,000 points using DampedSineWave and CosineWave
 * to test high-performance rendering capabilities with multiple curves. It registers 
 * properties for:
 * - Plot title
 * - X and Y axis labels
 * - Two line colors and labels
 * - Legend visibility
 * 
 * @section usage_example Usage Example
 * 
 * \code
 * auto func = new Line1MFunction(this);
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
 * @brief 演示 100 万点双曲线图的测试函数
 * 
 * Line1MFunction 使用 DampedSineWave 和 CosineWave 创建一个包含 1,000,000 个点的图表，
 * 用于测试多曲线的高性能渲染能力。它注册了以下属性：
 * - 图表标题
 * - X 和 Y 轴标签
 * - 两条曲线的颜色和标签
 * - 图例可见性
 * 
 * @section usage_example 使用示例
 * 
 * \code
 * auto func = new Line1MFunction(this);
 * func->createPlot(figureWidget);
 * // 属性可通过 getRegisteredProperties() 获取
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotNode
 * @see QImPlotLineItemNode
 * \endif
 */
class Line1MFunction : public TestFunction {
    Q_OBJECT
    
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString xLabel READ xLabel WRITE setXLabel NOTIFY xLabelChanged)
    Q_PROPERTY(QString yLabel READ yLabel WRITE setYLabel NOTIFY yLabelChanged)
    Q_PROPERTY(QColor line1Color READ line1Color WRITE setLine1Color NOTIFY line1ColorChanged)
    Q_PROPERTY(QString line1Label READ line1Label WRITE setLine1Label NOTIFY line1LabelChanged)
    Q_PROPERTY(QColor line2Color READ line2Color WRITE setLine2Color NOTIFY line2ColorChanged)
    Q_PROPERTY(QString line2Label READ line2Label WRITE setLine2Label NOTIFY line2LabelChanged)
    
public:
    /**
     * \if ENGLISH
     * @brief Constructor for Line1MFunction
     * @param parent Parent object
     * \endif
     * 
     * \if CHINESE
     * @brief Line1MFunction 构造函数
     * @param parent 父对象
     * \endif
     */
    explicit Line1MFunction(QObject* parent = nullptr);
    
    /**
     * \if ENGLISH
     * @brief Destructor for Line1MFunction
     * \endif
     * 
     * \if CHINESE
     * @brief Line1MFunction 析构函数
     * \endif
     */
    ~Line1MFunction() override;
    
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
    
    // Line 1 color property accessors
    QColor line1Color() const { return m_line1Color; }
    void setLine1Color(const QColor& color);
    
    // Line 1 label property accessors
    QString line1Label() const { return m_line1Label; }
    void setLine1Label(const QString& label);
    
    // Line 2 color property accessors
    QColor line2Color() const { return m_line2Color; }
    void setLine2Color(const QColor& color);
    
    // Line 2 label property accessors
    QString line2Label() const { return m_line2Label; }
    void setLine2Label(const QString& label);
    
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
     * @brief Signal emitted when line 1 color changes
     * @param color New line 1 color value
     * \endif
     * 
     * \if CHINESE
     * @brief 第一条曲线颜色改变时发出的信号
     * @param color 新的第一条曲线颜色值
     * \endif
     */
    void line1ColorChanged(const QColor& color);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when line 1 label changes
     * @param label New line 1 label value
     * \endif
     * 
     * \if CHINESE
     * @brief 第一条曲线标签改变时发出的信号
     * @param label 新的第一条曲线标签值
     * \endif
     */
    void line1LabelChanged(const QString& label);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when line 2 color changes
     * @param color New line 2 color value
     * \endif
     * 
     * \if CHINESE
     * @brief 第二条曲线颜色改变时发出的信号
     * @param color 新的第二条曲线颜色值
     * \endif
     */
    void line2ColorChanged(const QColor& color);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when line 2 label changes
     * @param label New line 2 label value
     * \endif
     * 
     * \if CHINESE
     * @brief 第二条曲线标签改变时发出的信号
     * @param label 新的第二条曲线标签值
     * \endif
     */
    void line2LabelChanged(const QString& label);
    
private:
    QString m_title = QStringLiteral("1M Points");
    QString m_xLabel = QStringLiteral("x2");
    QString m_yLabel = QStringLiteral("y2");
    QColor m_line1Color = Qt::blue;
    QString m_line1Label = QStringLiteral("curve 1");
    QColor m_line2Color = Qt::red;
    QString m_line2Label = QStringLiteral("curve 2");
    
    QIM::QImPlotNode* m_plotNode = nullptr;
    QIM::QImPlotLineItemNode* m_line1Node = nullptr;
    QIM::QImPlotLineItemNode* m_line2Node = nullptr;
    QIM::QImPlotValueTrackerNode* m_trackerNode = nullptr;
};

#endif // LINE1MFUNCTION_H
