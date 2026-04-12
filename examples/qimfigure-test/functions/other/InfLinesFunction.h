#ifndef INFLINESFUNCTION_H
#define INFLINESFUNCTION_H

#include "TestFunction.h"
#include <QObject>
#include <QColor>

namespace QIM {
class QImFigureWidget;
}

/**
 * \if ENGLISH
 * @brief Test function demonstrating infinite lines plot with property registration
 * 
 * InfLinesFunction creates a plot with infinite reference lines (both vertical and horizontal)
 * overlaid on a sine wave background curve. It registers properties for:
 * - Plot title
 * - X and Y axis labels
 * - Background line color
 * - Vertical infinite lines color and values
 * - Horizontal infinite lines color and values
 * 
 * @section usage_example Usage Example
 * 
 * \code
 * auto func = new InfLinesFunction(this);
 * func->createPlot(figureWidget);
 * // Properties will be available via getRegisteredProperties()
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotNode
 * @see QImPlotInfLinesItemNode
 * \endif
 * 
 * \if CHINESE
 * @brief 演示无限线图的测试函数，带有属性注册
 * 
 * InfLinesFunction 创建一个在正弦波背景曲线上叠加无限参考线（垂直和水平）的图表。
 * 它注册了以下属性：
 * - 图表标题
 * - X 和 Y 轴标签
 * - 背景线颜色
 * - 垂直无限线颜色和值
 * - 水平无限线颜色和值
 * 
 * @section usage_example 使用示例
 * 
 * \code
 * auto func = new InfLinesFunction(this);
 * func->createPlot(figureWidget);
 * // 属性可通过 getRegisteredProperties() 获取
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotNode
 * @see QImPlotInfLinesItemNode
 * \endif
 */
class InfLinesFunction : public TestFunction {
    Q_OBJECT
    
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString xLabel READ xLabel WRITE setXLabel NOTIFY xLabelChanged)
    Q_PROPERTY(QString yLabel READ yLabel WRITE setYLabel NOTIFY yLabelChanged)
    Q_PROPERTY(QColor lineColor READ lineColor WRITE setLineColor NOTIFY lineColorChanged)
    Q_PROPERTY(QColor verticalInfLinesColor READ verticalInfLinesColor WRITE setVerticalInfLinesColor NOTIFY verticalInfLinesColorChanged)
    Q_PROPERTY(QColor horizontalInfLinesColor READ horizontalInfLinesColor WRITE setHorizontalInfLinesColor NOTIFY horizontalInfLinesColorChanged)
    Q_PROPERTY(QVector<double> verticalValues READ verticalValues WRITE setVerticalValues NOTIFY verticalValuesChanged)
    Q_PROPERTY(QVector<double> horizontalValues READ horizontalValues WRITE setHorizontalValues NOTIFY horizontalValuesChanged)
    
public:
    /**
     * \if ENGLISH
     * @brief Constructor for InfLinesFunction
     * @param parent Parent object
     * \endif
     * 
     * \if CHINESE
     * @brief InfLinesFunction 构造函数
     * @param parent 父对象
     * \endif
     */
    explicit InfLinesFunction(QObject* parent = nullptr);
    
    /**
     * \if ENGLISH
     * @brief Destructor for InfLinesFunction
     * \endif
     * 
     * \if CHINESE
     * @brief InfLinesFunction 析构函数
     * \endif
     */
    ~InfLinesFunction() override;
    
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
    
    // Background line color property accessors
    QColor lineColor() const { return m_lineColor; }
    void setLineColor(const QColor& color);
    
    // Vertical infinite lines color property accessors
    QColor verticalInfLinesColor() const { return m_verticalInfLinesColor; }
    void setVerticalInfLinesColor(const QColor& color);
    
    // Horizontal infinite lines color property accessors
    QColor horizontalInfLinesColor() const { return m_horizontalInfLinesColor; }
    void setHorizontalInfLinesColor(const QColor& color);
    
    // Vertical values property accessors
    QVector<double> verticalValues() const { return m_verticalValues; }
    void setVerticalValues(const QVector<double>& values);
    
    // Horizontal values property accessors
    QVector<double> horizontalValues() const { return m_horizontalValues; }
    void setHorizontalValues(const QVector<double>& values);
    
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
     * @brief Signal emitted when background line color changes
     * @param color New line color value
     * \endif
     * 
     * \if CHINESE
     * @brief 背景线颜色改变时发出的信号
     * @param color 新的线颜色值
     * \endif
     */
    void lineColorChanged(const QColor& color);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when vertical infinite lines color changes
     * @param color New vertical infinite lines color value
     * \endif
     * 
     * \if CHINESE
     * @brief 垂直无限线颜色改变时发出的信号
     * @param color 新的垂直无限线颜色值
     * \endif
     */
    void verticalInfLinesColorChanged(const QColor& color);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when horizontal infinite lines color changes
     * @param color New horizontal infinite lines color value
     * \endif
     * 
     * \if CHINESE
     * @brief 水平无限线颜色改变时发出的信号
     * @param color 新的水平无限线颜色值
     * \endif
     */
    void horizontalInfLinesColorChanged(const QColor& color);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when vertical values change
     * @param values New vertical values
     * \endif
     * 
     * \if CHINESE
     * @brief 垂直值改变时发出的信号
     * @param values 新的垂直值
     * \endif
     */
    void verticalValuesChanged(const QVector<double>& values);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when horizontal values change
     * @param values New horizontal values
     * \endif
     * 
     * \if CHINESE
     * @brief 水平值改变时发出的信号
     * @param values 新的水平值
     * \endif
     */
    void horizontalValuesChanged(const QVector<double>& values);
    
private:
    QString m_title = QStringLiteral("Infinite Lines Plot");
    QString m_xLabel = QStringLiteral("x");
    QString m_yLabel = QStringLiteral("y");
    QColor m_lineColor = Qt::blue;
    QColor m_verticalInfLinesColor = Qt::red;
    QColor m_horizontalInfLinesColor = Qt::darkGreen;
    QVector<double> m_verticalValues = {1.0, 3.0, 5.0, 7.0};
    QVector<double> m_horizontalValues = {-3.0, 0.0, 3.0};
    
    QIM::QImPlotNode* m_plotNode = nullptr;
    QIM::QImPlotLineItemNode* m_lineNode = nullptr;
    QIM::QImPlotInfLinesItemNode* m_verticalInfLinesNode = nullptr;
    QIM::QImPlotInfLinesItemNode* m_horizontalInfLinesNode = nullptr;
    QIM::QImPlotValueTrackerNode* m_trackerNode = nullptr;
};

#endif // INFLINESFUNCTION_H
