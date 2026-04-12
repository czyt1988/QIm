#ifndef ERRORBARSFUNCTION_H
#define ERRORBARSFUNCTION_H

#include "TestFunction.h"
#include <QObject>
#include <QColor>

namespace QIM {
class QImFigureWidget;
class QImPlotNode;
class QImPlotScatterItemNode;
class QImPlotErrorBarsItemNode;
class QImPlotValueTrackerNode;
}

/**
 * \if ENGLISH
 * @brief Test function demonstrating error bars plot with property registration
 * 
 * ErrorBarsFunction creates a plot with 10 data points showing both symmetric and
 * asymmetric error bars. It demonstrates:
 * - Symmetric error bars (same error in both directions)
 * - Asymmetric error bars (different negative and positive errors)
 * - Horizontal error bars mode
 * - Multiple scatter plots with different marker shapes
 * 
 * Properties registered for interactive editing:
 * - Plot title
 * - X and Y axis labels
 * - Error bars color
 * - Horizontal mode toggle
 * 
 * @section usage_example Usage Example
 * 
 * \code
 * auto func = new ErrorBarsFunction(this);
 * func->createPlot(figureWidget);
 * // Properties will be available via getRegisteredProperties()
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotNode
 * @see QImPlotErrorBarsItemNode
 * @see QImPlotScatterItemNode
 * \endif
 * 
 * \if CHINESE
 * @brief 演示误差棒图的测试函数，带有属性注册
 * 
 * ErrorBarsFunction 创建一个包含 10 个数据点的图表，展示对称和非对称误差棒。
 * 它演示了：
 * - 对称误差棒（两个方向误差相同）
 * - 非对称误差棒（负向和正向误差不同）
 * - 水平误差棒模式
 * - 使用不同标记形状的多个散点图
 * 
 * 注册用于交互式编辑的属性：
 * - 图表标题
 * - X 和 Y 轴标签
 * - 误差棒颜色
 * - 水平模式切换
 * 
 * @section usage_example 使用示例
 * 
 * \code
 * auto func = new ErrorBarsFunction(this);
 * func->createPlot(figureWidget);
 * // 属性可通过 getRegisteredProperties() 获取
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotNode
 * @see QImPlotErrorBarsItemNode
 * @see QImPlotScatterItemNode
 * \endif
 */
class ErrorBarsFunction : public TestFunction {
    Q_OBJECT
    
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString xLabel READ xLabel WRITE setXLabel NOTIFY xLabelChanged)
    Q_PROPERTY(QString yLabel READ yLabel WRITE setYLabel NOTIFY yLabelChanged)
    Q_PROPERTY(QColor errorColor READ errorColor WRITE setErrorColor NOTIFY errorColorChanged)
    Q_PROPERTY(bool horizontalMode READ horizontalMode WRITE setHorizontalMode NOTIFY horizontalModeChanged)
    
public:
    /**
     * \if ENGLISH
     * @brief Constructor for ErrorBarsFunction
     * @param parent Parent object
     * \endif
     * 
     * \if CHINESE
     * @brief ErrorBarsFunction 构造函数
     * @param parent 父对象
     * \endif
     */
    explicit ErrorBarsFunction(QObject* parent = nullptr);
    
    /**
     * \if ENGLISH
     * @brief Destructor for ErrorBarsFunction
     * \endif
     * 
     * \if CHINESE
     * @brief ErrorBarsFunction 析构函数
     * \endif
     */
    ~ErrorBarsFunction() override;
    
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
    
    // Error color property accessors
    QColor errorColor() const { return m_errorColor; }
    void setErrorColor(const QColor& color);
    
    // Horizontal mode property accessors
    bool horizontalMode() const { return m_horizontalMode; }
    void setHorizontalMode(bool horizontal);
    
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
     * @brief Signal emitted when error color changes
     * @param color New error color value
     * \endif
     * 
     * \if CHINESE
     * @brief 误差颜色改变时发出的信号
     * @param color 新的误差颜色值
     * \endif
     */
    void errorColorChanged(const QColor& color);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when horizontal mode changes
     * @param horizontal New horizontal mode value
     * \endif
     * 
     * \if CHINESE
     * @brief 水平模式改变时发出的信号
     * @param horizontal 新的水平模式值
     * \endif
     */
    void horizontalModeChanged(bool horizontal);
    
private:
    QString m_title = QStringLiteral("Error Bars Plot");
    QString m_xLabel = QStringLiteral("x");
    QString m_yLabel = QStringLiteral("y");
    QColor m_errorColor = Qt::red;
    bool m_horizontalMode = false;
    
    QIM::QImPlotNode* m_plotNode = nullptr;
    QIM::QImPlotScatterItemNode* m_scatterNode1 = nullptr;
    QIM::QImPlotErrorBarsItemNode* m_errorBarsNode1 = nullptr;
    QIM::QImPlotScatterItemNode* m_scatterNode2 = nullptr;
    QIM::QImPlotErrorBarsItemNode* m_errorBarsNode2 = nullptr;
    QIM::QImPlotValueTrackerNode* m_trackerNode = nullptr;
};

#endif // ERRORBARSFUNCTION_H
