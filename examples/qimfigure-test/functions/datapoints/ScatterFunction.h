#ifndef SCATTERFUNCTION_H
#define SCATTERFUNCTION_H

#include "../TestFunction.h"
#include <QObject>
#include <QColor>

namespace QIM {
class QImFigureWidget;
class QImPlotNode;
class QImPlotScatterItemNode;
class QImPlotValueTrackerNode;
}

/**
 * \if ENGLISH
 * @brief Test function demonstrating 1000-point scatter plot with property registration
 * 
 * ScatterFunction creates a plot with 1000 random scatter points to test
 * scatter plot rendering capabilities. It registers properties for:
 * - Plot title
 * - X and Y axis labels
 * - Marker size, shape, color, and fill
 * 
 * @section usage_example Usage Example
 * 
 * \code
 * auto func = new ScatterFunction(this);
 * func->createPlot(figureWidget);
 * // Properties will be available via getRegisteredProperties()
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotNode
 * @see QImPlotScatterItemNode
 * \endif
 * 
 * \if CHINESE
 * @brief 演示 1000 点散点图的测试函数，带有属性注册
 * 
 * ScatterFunction 创建一个包含 1000 个随机散点的图表，用于测试
 * 散点图渲染能力。它注册了以下属性：
 * - 图表标题
 * - X 和 Y 轴标签
 * - 标记大小、形状、颜色和填充
 * 
 * @section usage_example 使用示例
 * 
 * \code
 * auto func = new ScatterFunction(this);
 * func->createPlot(figureWidget);
 * // 属性可通过 getRegisteredProperties() 获取
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotNode
 * @see QImPlotScatterItemNode
 * \endif
 */
class ScatterFunction : public TestFunction {
    Q_OBJECT
    
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString xLabel READ xLabel WRITE setXLabel NOTIFY xLabelChanged)
    Q_PROPERTY(QString yLabel READ yLabel WRITE setYLabel NOTIFY yLabelChanged)
    Q_PROPERTY(float markerSize READ markerSize WRITE setMarkerSize NOTIFY markerSizeChanged)
    Q_PROPERTY(int markerShape READ markerShape WRITE setMarkerShape NOTIFY markerShapeChanged)
    Q_PROPERTY(bool markerFill READ markerFill WRITE setMarkerFill NOTIFY markerFillChanged)
    Q_PROPERTY(QColor markerColor READ markerColor WRITE setMarkerColor NOTIFY markerColorChanged)
    
public:
    /**
     * \if ENGLISH
     * @brief Constructor for ScatterFunction
     * @param parent Parent object
     * \endif
     * 
     * \if CHINESE
     * @brief ScatterFunction 构造函数
     * @param parent 父对象
     * \endif
     */
    explicit ScatterFunction(QObject* parent = nullptr);
    
    /**
     * \if ENGLISH
     * @brief Destructor for ScatterFunction
     * \endif
     * 
     * \if CHINESE
     * @brief ScatterFunction 析构函数
     * \endif
     */
    ~ScatterFunction() override;
    
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
    
    // Marker size property accessors
    float markerSize() const { return m_markerSize; }
    void setMarkerSize(float size);
    
    // Marker shape property accessors
    int markerShape() const { return m_markerShape; }
    void setMarkerShape(int shape);
    
    // Marker fill property accessors
    bool markerFill() const { return m_markerFill; }
    void setMarkerFill(bool fill);
    
    // Marker color property accessors
    QColor markerColor() const { return m_markerColor; }
    void setMarkerColor(const QColor& color);
    
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
     * @brief Signal emitted when marker size changes
     * @param size New marker size value
     * \endif
     * 
     * \if CHINESE
     * @brief 标记大小改变时发出的信号
     * @param size 新的标记大小值
     * \endif
     */
    void markerSizeChanged(float size);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when marker shape changes
     * @param shape New marker shape value
     * \endif
     * 
     * \if CHINESE
     * @brief 标记形状改变时发出的信号
     * @param shape 新的标记形状值
     * \endif
     */
    void markerShapeChanged(int shape);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when marker fill changes
     * @param fill New marker fill value
     * \endif
     * 
     * \if CHINESE
     * @brief 标记填充改变时发出的信号
     * @param fill 新的标记填充值
     * \endif
     */
    void markerFillChanged(bool fill);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when marker color changes
     * @param color New marker color value
     * \endif
     * 
     * \if CHINESE
     * @brief 标记颜色改变时发出的信号
     * @param color 新的标记颜色值
     * \endif
     */
    void markerColorChanged(const QColor& color);
    
private:
    QString m_title = QStringLiteral("Scatter Plot");
    QString m_xLabel = QStringLiteral("x3");
    QString m_yLabel = QStringLiteral("y3");
    float m_markerSize = 4.0f;
    int m_markerShape = 0;  // ImPlotMarker_Circle
    bool m_markerFill = true;
    QColor m_markerColor = Qt::blue;
    
    QIM::QImPlotNode* m_plotNode = nullptr;
    QIM::QImPlotScatterItemNode* m_scatterNode = nullptr;
    QIM::QImPlotValueTrackerNode* m_trackerNode = nullptr;
};

#endif // SCATTERFUNCTION_H
