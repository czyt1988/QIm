#ifndef PLOT3DSCATTERFUNCTION_H
#define PLOT3DSCATTERFUNCTION_H

#include "../TestFunction.h"
#include <QObject>
#include <QColor>

namespace QIM {
class QImFigureWidget;
class QImPlot3DNode;
class QImPlot3DScatterItemNode;
}

/**
 * \if ENGLISH
 * @brief Test function demonstrating 1000-point 3D scatter plot with property registration
 * 
 * Plot3DScatterFunction creates a 3D plot with 1000 random XYZ scatter points to test
 * 3D scatter plot rendering capabilities. It registers properties for:
 * - Plot title
 * - X, Y, and Z axis labels
 * - Marker size, shape, and color
 * 
 * @section usage_example Usage Example
 * 
 * \code
 * auto func = new Plot3DScatterFunction(this);
 * func->createPlot(figureWidget);
 * // Properties will be available via getRegisteredProperties()
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlot3DNode
 * @see QImPlot3DScatterItemNode
 * \endif
 * 
 * \if CHINESE
 * @brief 演示 1000 点 3D 散点图的测试函数，带有属性注册
 * 
 * Plot3DScatterFunction 创建一个包含 1000 个随机 XYZ 散点的 3D 图表，用于测试
 * 3D 散点图渲染能力。它注册了以下属性：
 * - 图表标题
 * - X、Y 和 Z 轴标签
 * - 标记大小、形状和颜色
 * 
 * @section usage_example 使用示例
 * 
 * \code
 * auto func = new Plot3DScatterFunction(this);
 * func->createPlot(figureWidget);
 * // 属性可通过 getRegisteredProperties() 获取
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlot3DNode
 * @see QImPlot3DScatterItemNode
 * \endif
 */
class Plot3DScatterFunction : public TestFunction {
    Q_OBJECT
    
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString xLabel READ xLabel WRITE setXLabel NOTIFY xLabelChanged)
    Q_PROPERTY(QString yLabel READ yLabel WRITE setYLabel NOTIFY yLabelChanged)
    Q_PROPERTY(QString zLabel READ zLabel WRITE setZLabel NOTIFY zLabelChanged)
    Q_PROPERTY(float markerSize READ markerSize WRITE setMarkerSize NOTIFY markerSizeChanged)
    Q_PROPERTY(int markerShape READ markerShape WRITE setMarkerShape NOTIFY markerShapeChanged)
    Q_PROPERTY(QColor markerColor READ markerColor WRITE setMarkerColor NOTIFY markerColorChanged)
    
public:
    /**
     * \if ENGLISH
     * @brief Constructor for Plot3DScatterFunction
     * @param parent Parent object
     * \endif
     * 
     * \if CHINESE
     * @brief Plot3DScatterFunction 构造函数
     * @param parent 父对象
     * \endif
     */
    explicit Plot3DScatterFunction(QObject* parent = nullptr);
    
    /**
     * \if ENGLISH
     * @brief Destructor for Plot3DScatterFunction
     * \endif
     * 
     * \if CHINESE
     * @brief Plot3DScatterFunction 析构函数
     * \endif
     */
    ~Plot3DScatterFunction() override;
    
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
    
    // Z axis label property accessors
    QString zLabel() const { return m_zLabel; }
    void setZLabel(const QString& label);
    
    // Marker size property accessors
    float markerSize() const { return m_markerSize; }
    void setMarkerSize(float size);
    
    // Marker shape property accessors
    int markerShape() const { return m_markerShape; }
    void setMarkerShape(int shape);
    
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
    QString m_title = QStringLiteral("3D Scatter Plot");
    QString m_xLabel = QStringLiteral("X");
    QString m_yLabel = QStringLiteral("Y");
    QString m_zLabel = QStringLiteral("Z");
    float m_markerSize = 4.0f;
    int m_markerShape = 0;  // QImPlot3DMarkerShape::Circle
    QColor m_markerColor = Qt::blue;
    
    QIM::QImPlot3DNode* m_plot3DNode = nullptr;
    QIM::QImPlot3DScatterItemNode* m_scatter3DNode = nullptr;
};

#endif // PLOT3DSCATTERFUNCTION_H