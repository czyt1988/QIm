#ifndef SHADEDFUNCTION_H
#define SHADEDFUNCTION_H

#include "../TestFunction.h"
#include <QObject>
#include <QColor>

namespace QIM {
class QImFigureWidget;
class QImPlotNode;
class QImPlotShadedItemNode;
class QImPlotValueTrackerNode;
}

/**
 * \if ENGLISH
 * @brief Test function demonstrating shaded (filled area) plot with property registration
 * 
 * ShadedFunction creates a plot with 100 sine wave points to demonstrate
 * shaded/filled area visualization capabilities. It supports both single-line mode
 * (fill to reference value) and double-line mode (fill between two lines).
 * It registers properties for:
 * - Plot title
 * - X and Y axis labels
 * - Fill color
 * - Reference value (for single-line mode)
 * - Fill mode (single-line or double-line)
 * 
 * @section usage_example Usage Example
 * 
 * \code
 * auto func = new ShadedFunction(this);
 * func->createPlot(figureWidget);
 * // Properties will be available via getRegisteredProperties()
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotNode
 * @see QImPlotShadedItemNode
 * \endif
 * 
 * \if CHINESE
 * @brief 演示填充区域图的测试函数，带有属性注册
 * 
 * ShadedFunction 创建一个包含 100 个正弦波点的图表，用于演示
 * 填充/阴影区域可视化功能。它支持单线模式（填充到参考值）
 * 和双线模式（两条线之间填充）。它注册了以下属性：
 * - 图表标题
 * - X 和 Y 轴标签
 * - 填充颜色
 * - 参考值（用于单线模式）
 * - 填充模式（单线或双线）
 * 
 * @section usage_example 使用示例
 * 
 * \code
 * auto func = new ShadedFunction(this);
 * func->createPlot(figureWidget);
 * // 属性可通过 getRegisteredProperties() 获取
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotNode
 * @see QImPlotShadedItemNode
 * \endif
 */
class ShadedFunction : public TestFunction {
    Q_OBJECT
    
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString xLabel READ xLabel WRITE setXLabel NOTIFY xLabelChanged)
    Q_PROPERTY(QString yLabel READ yLabel WRITE setYLabel NOTIFY yLabelChanged)
    Q_PROPERTY(QColor fillColor READ fillColor WRITE setFillColor NOTIFY fillColorChanged)
    Q_PROPERTY(double referenceValue READ referenceValue WRITE setReferenceValue NOTIFY referenceValueChanged)
    Q_PROPERTY(bool twoLineMode READ twoLineMode WRITE setTwoLineMode NOTIFY twoLineModeChanged)
    
public:
    /**
     * \if ENGLISH
     * @brief Constructor for ShadedFunction
     * @param parent Parent object
     * \endif
     * 
     * \if CHINESE
     * @brief ShadedFunction 构造函数
     * @param parent 父对象
     * \endif
     */
    explicit ShadedFunction(QObject* parent = nullptr);
    
    /**
     * \if ENGLISH
     * @brief Destructor for ShadedFunction
     * \endif
     * 
     * \if CHINESE
     * @brief ShadedFunction 析构函数
     * \endif
     */
    ~ShadedFunction() override;
    
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
    
    // Fill color property accessors
    QColor fillColor() const { return m_fillColor; }
    void setFillColor(const QColor& color);
    
    // Reference value property accessors
    double referenceValue() const { return m_referenceValue; }
    void setReferenceValue(double value);
    
    // Two-line mode property accessors
    bool twoLineMode() const { return m_twoLineMode; }
    void setTwoLineMode(bool enabled);
    
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
     * @brief Signal emitted when fill color changes
     * @param color New fill color value
     * \endif
     * 
     * \if CHINESE
     * @brief 填充颜色改变时发出的信号
     * @param color 新的填充颜色值
     * \endif
     */
    void fillColorChanged(const QColor& color);
    
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
     * @brief Signal emitted when two-line mode changes
     * @param enabled New two-line mode state
     * \endif
     * 
     * \if CHINESE
     * @brief 双线模式改变时发出的信号
     * @param enabled 新的双线模式状态
     * \endif
     */
    void twoLineModeChanged(bool enabled);
    
private:
    QString m_title = QStringLiteral("Shaded Plot");
    QString m_xLabel = QStringLiteral("x");
    QString m_yLabel = QStringLiteral("y");
    QColor m_fillColor = QColor(100, 150, 255, 180);
    double m_referenceValue = 0.0;
    bool m_twoLineMode = false;
    
    QIM::QImPlotNode* m_plotNode = nullptr;
    QIM::QImPlotShadedItemNode* m_shadedNode = nullptr;
    QIM::QImPlotValueTrackerNode* m_trackerNode = nullptr;
};

#endif // SHADEDFUNCTION_H
