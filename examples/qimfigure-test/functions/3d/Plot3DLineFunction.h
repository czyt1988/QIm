#ifndef PLOT3DLINEFUNCTION_H
#define PLOT3DLINEFUNCTION_H

#include "../TestFunction.h"
#include <QObject>
#include <QColor>
#include <QPointer>

namespace QIM {
class QImFigureWidget;
class QImPlot3DNode;
class QImPlot3DLineItemNode;
}

/**
 * \if ENGLISH
 * @brief Test function demonstrating 3D line plot with property registration
 * 
 * Plot3DLineFunction creates a 3D plot with a spiral line to test
 * 3D rendering capabilities. It registers properties for:
 * - Plot title
 * - X, Y and Z axis labels
 * - Line color
 * - Line weight
 * 
 * @section usage_example Usage Example
 * 
 * \code
 * auto func = new Plot3DLineFunction(this);
 * func->createPlot(figureWidget);
 * // Properties will be available via getRegisteredProperties()
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlot3DExtNode
 * @see QImPlot3DExtLineItemNode
 * \endif
 * 
 * \if CHINESE
 * @brief 演示 3D 线图的测试函数，带有属性注册
 * 
 * Plot3DLineFunction 创建一个包含螺旋线的 3D 图表，用于测试
 * 3D 渲染能力。它注册了以下属性：
 * - 图表标题
 * - X、Y 和 Z 轴标签
 * - 线条颜色
 * - 线宽
 * 
 * @section usage_example 使用示例
 * 
 * \code
 * auto func = new Plot3DLineFunction(this);
 * func->createPlot(figureWidget);
 * // 属性可通过 getRegisteredProperties() 获取
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlot3DExtNode
 * @see QImPlot3DExtLineItemNode
 * \endif
 */
class Plot3DLineFunction : public TestFunction {
    Q_OBJECT
    
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString xLabel READ xLabel WRITE setXLabel NOTIFY xLabelChanged)
    Q_PROPERTY(QString yLabel READ yLabel WRITE setYLabel NOTIFY yLabelChanged)
    Q_PROPERTY(QString zLabel READ zLabel WRITE setZLabel NOTIFY zLabelChanged)
    Q_PROPERTY(QColor lineColor READ lineColor WRITE setLineColor NOTIFY lineColorChanged)
    Q_PROPERTY(float lineWeight READ lineWeight WRITE setLineWeight NOTIFY lineWeightChanged)
    Q_PROPERTY(bool segmentsEnabled READ segmentsEnabled WRITE setSegmentsEnabled NOTIFY segmentsEnabledChanged)
    Q_PROPERTY(bool loopEnabled READ loopEnabled WRITE setLoopEnabled NOTIFY loopEnabledChanged)
    Q_PROPERTY(bool skipNaNEnabled READ skipNaNEnabled WRITE setSkipNaNEnabled NOTIFY skipNaNEnabledChanged)
    
public:
    /**
     * \if ENGLISH
     * @brief Constructor for Plot3DLineFunction
     * @param parent Parent object
     * \endif
     * 
     * \if CHINESE
     * @brief Plot3DLineFunction 构造函数
     * @param parent 父对象
     * \endif
     */
    explicit Plot3DLineFunction(QObject* parent = nullptr);
    
    /**
     * \if ENGLISH
     * @brief Destructor for Plot3DLineFunction
     * \endif
     * 
     * \if CHINESE
     * @brief Plot3DLineFunction 析构函数
     * \endif
     */
    ~Plot3DLineFunction() override;
    
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
    
    /**
     * \if ENGLISH
     * @brief Cleanup plot nodes before deletion
     * \endif
     * 
     * \if CHINESE
     * @brief 删除前清理绘图节点
     * \endif
     */
    void cleanupPlot() override;
    
    // Title property accessors
    QString title() const { return m_title; }
    void setTitle(const QString& title);
    
    // X axis label property accessors
    QString xLabel() const { return m_xLabel; }
    void setXLabel(const QString& label);
    
    // Y axis label property accessors
    QString yLabel() const { return m_yLabel; }
    void setYLabel(const QString& label);
    
    // Z axis label property accessors
    QString zLabel() const { return m_zLabel; }
    void setZLabel(const QString& label);
    
    // Line color property accessors
    QColor lineColor() const { return m_lineColor; }
    void setLineColor(const QColor& color);
    
    // Line weight property accessors
    float lineWeight() const { return m_lineWeight; }
    void setLineWeight(float weight);
    
    // Segments enabled property accessors
    bool segmentsEnabled() const { return m_segmentsEnabled; }
    void setSegmentsEnabled(bool enabled);
    
    // Loop enabled property accessors
    bool loopEnabled() const { return m_loopEnabled; }
    void setLoopEnabled(bool enabled);
    
    // Skip NaN enabled property accessors
    bool skipNaNEnabled() const { return m_skipNaNEnabled; }
    void setSkipNaNEnabled(bool enabled);
    
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
     * @brief Signal emitted when Z axis label changes
     * @param label New Z axis label value
     * \endif
     * 
     * \if CHINESE
     * @brief Z 轴标签改变时发出的信号
     * @param label 新的 Z 轴标签值
     * \endif
     */
    void zLabelChanged(const QString& label);
    
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
     * @brief Signal emitted when line weight changes
     * @param weight New line weight value
     * \endif
     * 
     * \if CHINESE
     * @brief 线宽改变时发出的信号
     * @param weight 新的线宽值
     * \endif
     */
    void lineWeightChanged(float weight);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when segments enabled changes
     * @param enabled New segments enabled value
     * \endif
     * 
     * \if CHINESE
     * @brief 线段模式启用改变时发出的信号
     * @param enabled 新的线段模式启用值
     * \endif
     */
    void segmentsEnabledChanged(bool enabled);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when loop enabled changes
     * @param enabled New loop enabled value
     * \endif
     * 
     * \if CHINESE
     * @brief 循环模式启用改变时发出的信号
     * @param enabled 新的循环模式启用值
     * \endif
     */
    void loopEnabledChanged(bool enabled);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when skip NaN enabled changes
     * @param enabled New skip NaN enabled value
     * \endif
     * 
     * \if CHINESE
     * @brief 跳过NaN模式启用改变时发出的信号
     * @param enabled 新的跳过NaN模式启用值
     * \endif
     */
    void skipNaNEnabledChanged(bool enabled);
    
private:
    QString m_title = QStringLiteral("3D Line - Spiral");
    QString m_xLabel = QStringLiteral("X");
    QString m_yLabel = QStringLiteral("Y");
    QString m_zLabel = QStringLiteral("Z");
    QColor m_lineColor = Qt::blue;
    float m_lineWeight = 1.0f;
    bool m_segmentsEnabled = false;
    bool m_loopEnabled = false;
    bool m_skipNaNEnabled = false;
    
    QPointer<QIM::QImPlot3DNode> m_plot3DNode = nullptr;
    QPointer<QIM::QImPlot3DLineItemNode> m_line3DNode = nullptr;
};

#endif // PLOT3DLINEFUNCTION_H
