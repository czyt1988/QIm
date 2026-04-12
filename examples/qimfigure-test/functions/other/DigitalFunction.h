#ifndef DIGITALFUNCTION_H
#define DIGITALFUNCTION_H

#include "../TestFunction.h"
#include <QObject>
#include <QColor>

namespace QIM {
class QImFigureWidget;
class QImPlotNode;
class QImPlotDigitalItemNode;
class QImPlotValueTrackerNode;
}

/**
 * \if ENGLISH
 * @brief Test function demonstrating digital signal plot with property registration
 * 
 * DigitalFunction creates a plot with digital signal lines (on/off states).
 * It registers properties for:
 * - Plot title
 * - X and Y axis labels
 * - Digital signal color
 * - Digital signal label
 * 
 * @section usage_example Usage Example
 * 
 * \code
 * auto func = new DigitalFunction(this);
 * func->createPlot(figureWidget);
 * // Properties will be available via getRegisteredProperties()
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotNode
 * @see QImPlotDigitalItemNode
 * \endif
 * 
 * \if CHINESE
 * @brief 演示数字信号图的测试函数，带有属性注册
 * 
 * DigitalFunction 创建一个数字信号图（开/关状态）。
 * 它注册了以下属性：
 * - 图表标题
 * - X 和 Y 轴标签
 * - 数字信号颜色
 * - 数字信号标签
 * 
 * @section usage_example 使用示例
 * 
 * \code
 * auto func = new DigitalFunction(this);
 * func->createPlot(figureWidget);
 * // 属性可通过 getRegisteredProperties() 获取
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotNode
 * @see QImPlotDigitalItemNode
 * \endif
 */
class DigitalFunction : public TestFunction {
    Q_OBJECT
    
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString xLabel READ xLabel WRITE setXLabel NOTIFY xLabelChanged)
    Q_PROPERTY(QString yLabel READ yLabel WRITE setYLabel NOTIFY yLabelChanged)
    Q_PROPERTY(QColor digitalColor READ digitalColor WRITE setDigitalColor NOTIFY digitalColorChanged)
    Q_PROPERTY(QString digitalLabel READ digitalLabel WRITE setDigitalLabel NOTIFY digitalLabelChanged)
    
public:
    /**
     * \if ENGLISH
     * @brief Constructor for DigitalFunction
     * @param parent Parent object
     * \endif
     * 
     * \if CHINESE
     * @brief DigitalFunction 构造函数
     * @param parent 父对象
     * \endif
     */
    explicit DigitalFunction(QObject* parent = nullptr);
    
    /**
     * \if ENGLISH
     * @brief Destructor for DigitalFunction
     * \endif
     * 
     * \if CHINESE
     * @brief DigitalFunction 析构函数
     * \endif
     */
    ~DigitalFunction() override;
    
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
    
    // Digital color property accessors
    QColor digitalColor() const { return m_digitalColor; }
    void setDigitalColor(const QColor& color);
    
    // Digital label property accessors
    QString digitalLabel() const { return m_digitalLabel; }
    void setDigitalLabel(const QString& label);
    
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
     * @brief Signal emitted when digital color changes
     * @param color New digital color value
     * \endif
     * 
     * \if CHINESE
     * @brief 数字信号颜色改变时发出的信号
     * @param color 新的数字信号颜色值
     * \endif
     */
    void digitalColorChanged(const QColor& color);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when digital label changes
     * @param label New digital label value
     * \endif
     * 
     * \if CHINESE
     * @brief 数字信号标签改变时发出的信号
     * @param label 新的数字信号标签值
     * \endif
     */
    void digitalLabelChanged(const QString& label);
    
private:
    QString m_title = QStringLiteral("Digital Signal Plot");
    QString m_xLabel = QStringLiteral("Time");
    QString m_yLabel = QStringLiteral("State");
    QColor m_digitalColor = Qt::blue;
    QString m_digitalLabel = QStringLiteral("Digital Signal");
    
    QIM::QImPlotNode* m_plotNode = nullptr;
    QIM::QImPlotDigitalItemNode* m_digitalNode = nullptr;
    QIM::QImPlotValueTrackerNode* m_trackerNode = nullptr;
};

#endif // DIGITALFUNCTION_H