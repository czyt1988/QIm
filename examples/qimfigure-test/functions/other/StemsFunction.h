#ifndef STEMSFUNCTION_H
#define STEMSFUNCTION_H

#include "../TestFunction.h"
#include <QObject>
#include <QColor>

namespace QIM {
class QImFigureWidget;
class QImPlotNode;
class QImPlotStemsItemNode;
class QImPlotValueTrackerNode;
}

/**
 * \if ENGLISH
 * @brief Test function demonstrating stems plot with property registration
 * 
 * StemsFunction creates a plot with stem lines from a reference value (baseline)
 * to each data point, useful for visualizing discrete data and their deviation
 * from a baseline. It registers properties for:
 * - Plot title
 * - X and Y axis labels
 * - Stems color
 * - Stems label
 * - Reference value (baseline)
 * - Horizontal orientation
 * 
 * @section usage_example Usage Example
 * 
 * \code
 * auto func = new StemsFunction(this);
 * func->createPlot(figureWidget);
 * // Properties will be available via getRegisteredProperties()
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotNode
 * @see QImPlotStemsItemNode
 * \endif
 * 
 * \if CHINESE
 * @brief 演示茎叶图的测试函数，带有属性注册
 * 
 * StemsFunction 创建一个茎叶图，从参考值（基线）到每个数据点绘制茎线，
 * 适用于可视化离散数据及其与基线的偏差。它注册了以下属性：
 * - 图表标题
 * - X 和 Y 轴标签
 * - 茎线颜色
 * - 茎线标签
 * - 参考值（基线）
 * - 水平方向
 * 
 * @section usage_example 使用示例
 * 
 * \code
 * auto func = new StemsFunction(this);
 * func->createPlot(figureWidget);
 * // 属性可通过 getRegisteredProperties() 获取
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotNode
 * @see QImPlotStemsItemNode
 * \endif
 */
class StemsFunction : public TestFunction {
    Q_OBJECT
    
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString xLabel READ xLabel WRITE setXLabel NOTIFY xLabelChanged)
    Q_PROPERTY(QString yLabel READ yLabel WRITE setYLabel NOTIFY yLabelChanged)
    Q_PROPERTY(QColor stemsColor READ stemsColor WRITE setStemsColor NOTIFY stemsColorChanged)
    Q_PROPERTY(QString stemsLabel READ stemsLabel WRITE setStemsLabel NOTIFY stemsLabelChanged)
    Q_PROPERTY(double referenceValue READ referenceValue WRITE setReferenceValue NOTIFY referenceValueChanged)
    Q_PROPERTY(bool horizontal READ isHorizontal WRITE setHorizontal NOTIFY horizontalChanged)
    
public:
    /**
     * \if ENGLISH
     * @brief Constructor for StemsFunction
     * @param parent Parent object
     * \endif
     * 
     * \if CHINESE
     * @brief StemsFunction 构造函数
     * @param parent 父对象
     * \endif
     */
    explicit StemsFunction(QObject* parent = nullptr);
    
    /**
     * \if ENGLISH
     * @brief Destructor for StemsFunction
     * \endif
     * 
     * \if CHINESE
     * @brief StemsFunction 析构函数
     * \endif
     */
    ~StemsFunction() override;
    
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
    
    // Stems color property accessors
    QColor stemsColor() const { return m_stemsColor; }
    void setStemsColor(const QColor& color);
    
    // Stems label property accessors
    QString stemsLabel() const { return m_stemsLabel; }
    void setStemsLabel(const QString& label);
    
    // Reference value property accessors
    double referenceValue() const { return m_referenceValue; }
    void setReferenceValue(double value);
    
    // Horizontal orientation property accessors
    bool isHorizontal() const { return m_horizontal; }
    void setHorizontal(bool horizontal);
    
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
     * @brief Signal emitted when stems color changes
     * @param color New stems color value
     * \endif
     * 
     * \if CHINESE
     * @brief 茎线颜色改变时发出的信号
     * @param color 新的茎线颜色值
     * \endif
     */
    void stemsColorChanged(const QColor& color);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when stems label changes
     * @param label New stems label value
     * \endif
     * 
     * \if CHINESE
     * @brief 茎线标签改变时发出的信号
     * @param label 新的茎线标签值
     * \endif
     */
    void stemsLabelChanged(const QString& label);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when reference value changes
     * @param value New reference value
     * \endif
     * 
     * \if CHINESE
     * @brief 参考值改变时发出的信号
     * @param value 新的参考值
     * \endif
     */
    void referenceValueChanged(double value);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when horizontal orientation changes
     * @param horizontal New horizontal state
     * \endif
     * 
     * \if CHINESE
     * @brief 水平方向改变时发出的信号
     * @param horizontal 新的水平状态
     * \endif
     */
    void horizontalChanged(bool horizontal);
    
private:
    QString m_title = QStringLiteral("Stems Plot");
    QString m_xLabel = QStringLiteral("x");
    QString m_yLabel = QStringLiteral("y");
    QColor m_stemsColor = Qt::blue;
    QString m_stemsLabel = QStringLiteral("Stems");
    double m_referenceValue = 0.0;
    bool m_horizontal = false;
    
    QIM::QImPlotNode* m_plotNode = nullptr;
    QIM::QImPlotStemsItemNode* m_stemsNode = nullptr;
    QIM::QImPlotValueTrackerNode* m_trackerNode = nullptr;
};

#endif // STEMSFUNCTION_H
