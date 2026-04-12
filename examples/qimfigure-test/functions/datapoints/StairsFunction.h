#ifndef STAIRSFUNCTION_H
#define STAIRSFUNCTION_H

#include "TestFunction.h"
#include <QObject>
#include <QColor>

namespace QIM {
class QImFigureWidget;
}

/**
 * \if ENGLISH
 * @brief Test function demonstrating 10-point stairs plot with property registration
 * 
 * StairsFunction creates a plot with 10 stair points to demonstrate
 * staircase plot capabilities. It registers properties for:
 * - Plot title
 * - X and Y axis labels
 * - Stairs color
 * - Shaded mode
 * 
 * @section usage_example Usage Example
 * 
 * \code
 * auto func = new StairsFunction(this);
 * func->createPlot(figureWidget);
 * // Properties will be available via getRegisteredProperties()
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotNode
 * @see QImPlotStairsItemNode
 * \endif
 * 
 * \if CHINESE
 * @brief 演示 10 点阶梯图的测试函数，带有属性注册
 * 
 * StairsFunction 创建一个包含 10 个阶梯点的图表，用于演示
 * 阶梯图功能。它注册了以下属性：
 * - 图表标题
 * - X 和 Y 轴标签
 * - 阶梯颜色
 * - 填充模式
 * 
 * @section usage_example 使用示例
 * 
 * \code
 * auto func = new StairsFunction(this);
 * func->createPlot(figureWidget);
 * // 属性可通过 getRegisteredProperties() 获取
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotNode
 * @see QImPlotStairsItemNode
 * \endif
 */
class StairsFunction : public TestFunction {
    Q_OBJECT
    
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString xLabel READ xLabel WRITE setXLabel NOTIFY xLabelChanged)
    Q_PROPERTY(QString yLabel READ yLabel WRITE setYLabel NOTIFY yLabelChanged)
    Q_PROPERTY(QColor stairsColor READ stairsColor WRITE setStairsColor NOTIFY stairsColorChanged)
    Q_PROPERTY(bool shaded READ isShaded WRITE setShaded NOTIFY shadedChanged)
    
public:
    /**
     * \if ENGLISH
     * @brief Constructor for StairsFunction
     * @param parent Parent object
     * \endif
     * 
     * \if CHINESE
     * @brief StairsFunction 构造函数
     * @param parent 父对象
     * \endif
     */
    explicit StairsFunction(QObject* parent = nullptr);
    
    /**
     * \if ENGLISH
     * @brief Destructor for StairsFunction
     * \endif
     * 
     * \if CHINESE
     * @brief StairsFunction 析构函数
     * \endif
     */
    ~StairsFunction() override;
    
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
    
    // Stairs color property accessors
    QColor stairsColor() const { return m_stairsColor; }
    void setStairsColor(const QColor& color);
    
    // Shaded property accessors
    bool isShaded() const { return m_shaded; }
    void setShaded(bool shaded);
    
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
     * @brief Signal emitted when stairs color changes
     * @param color New stairs color value
     * \endif
     * 
     * \if CHINESE
     * @brief 阶梯颜色改变时发出的信号
     * @param color 新的阶梯颜色值
     * \endif
     */
    void stairsColorChanged(const QColor& color);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when shaded mode changes
     * @param shaded New shaded mode value
     * \endif
     * 
     * \if CHINESE
     * @brief 填充模式改变时发出的信号
     * @param shaded 新的填充模式值
     * \endif
     */
    void shadedChanged(bool shaded);
    
private:
    QString m_title = QStringLiteral("Stairs Plot");
    QString m_xLabel = QStringLiteral("x");
    QString m_yLabel = QStringLiteral("y");
    QColor m_stairsColor = Qt::red;
    bool m_shaded = true;
    
    QIM::QImPlotNode* m_plotNode = nullptr;
    QIM::QImPlotStairsItemNode* m_stairsNode = nullptr;
    QIM::QImPlotValueTrackerNode* m_trackerNode = nullptr;
};

#endif // STAIRSFUNCTION_H
