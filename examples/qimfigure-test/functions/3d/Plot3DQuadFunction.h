#ifndef PLOT3DQUADFUNCTION_H
#define PLOT3DQUADFUNCTION_H

#include "../TestFunction.h"
#include <QObject>
#include <QColor>
#include <QPointer>

namespace QIM {
class QImFigureWidget;
class QImPlot3DNode;
class QImPlot3DQuadItemNode;
}

/**
 * \if ENGLISH
 * @brief Test function demonstrating 3D quad plot with property registration
 * 
 * Plot3DQuadFunction creates a 3D plot with a quad item (unit cube with 6 faces)
 * to test 3D quad rendering capabilities. It registers properties for:
 * - Plot title
 * - X, Y and Z axis labels
 * - Fill color
 * - Line color and width
 * - Lines, fill and markers visibility
 * - Marker shape, size, weight, fill/outline color
 * - Fill alpha
 * 
 * @section usage_example Usage Example
 * 
 * \code
 * auto func = new Plot3DQuadFunction(this);
 * func->createPlot(figureWidget);
 * // Properties will be available via getRegisteredProperties()
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlot3DNode
 * @see QImPlot3DQuadItemNode
 * \endif
 * 
 * \if CHINESE
 * @brief 演示 3D 四边形图的测试函数，带有属性注册
 * 
 * Plot3DQuadFunction 创建一个包含四边形元素（单位立方体，6 个面）的 3D 图表，
 * 用于测试 3D 四边形渲染能力。它注册了以下属性：
 * - 图表标题
 * - X、Y 和 Z 轴标签
 * - 填充颜色
 * - 线条颜色和宽度
 * - 线条、填充和标记点的可见性
 * - 标记点形状、大小、权重、填充/轮廓颜色
 * - 填充透明度
 * 
 * @section usage_example 使用示例
 * 
 * \code
 * auto func = new Plot3DQuadFunction(this);
 * func->createPlot(figureWidget);
 * // 属性可通过 getRegisteredProperties() 获取
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlot3DNode
 * @see QImPlot3DQuadItemNode
 * \endif
 */
class Plot3DQuadFunction : public TestFunction {
    Q_OBJECT
    
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString xLabel READ xLabel WRITE setXLabel NOTIFY xLabelChanged)
    Q_PROPERTY(QString yLabel READ yLabel WRITE setYLabel NOTIFY yLabelChanged)
    Q_PROPERTY(QString zLabel READ zLabel WRITE setZLabel NOTIFY zLabelChanged)
    Q_PROPERTY(QColor fillColor READ fillColor WRITE setFillColor NOTIFY fillColorChanged)
    Q_PROPERTY(QColor lineColor READ lineColor WRITE setLineColor NOTIFY lineColorChanged)
    Q_PROPERTY(float lineWidth READ lineWidth WRITE setLineWidth NOTIFY lineWidthChanged)
    Q_PROPERTY(bool linesVisible READ linesVisible WRITE setLinesVisible NOTIFY linesVisibleChanged)
    Q_PROPERTY(bool fillVisible READ fillVisible WRITE setFillVisible NOTIFY fillVisibleChanged)
    Q_PROPERTY(bool markersVisible READ markersVisible WRITE setMarkersVisible NOTIFY markersVisibleChanged)
    Q_PROPERTY(QString markerShape READ markerShape WRITE setMarkerShape NOTIFY markerShapeChanged)
    Q_PROPERTY(float markerSize READ markerSize WRITE setMarkerSize NOTIFY markerSizeChanged)
    Q_PROPERTY(float markerWeight READ markerWeight WRITE setMarkerWeight NOTIFY markerWeightChanged)
    Q_PROPERTY(QColor markerFillColor READ markerFillColor WRITE setMarkerFillColor NOTIFY markerFillColorChanged)
    Q_PROPERTY(QColor markerOutlineColor READ markerOutlineColor WRITE setMarkerOutlineColor NOTIFY markerOutlineColorChanged)
    Q_PROPERTY(float fillAlpha READ fillAlpha WRITE setFillAlpha NOTIFY fillAlphaChanged)
    
public:
    /**
     * \if ENGLISH
     * @brief Constructor for Plot3DQuadFunction
     * @param parent Parent object
     * \endif
     * 
     * \if CHINESE
     * @brief Plot3DQuadFunction 构造函数
     * @param parent 父对象
     * \endif
     */
    explicit Plot3DQuadFunction(QObject* parent = nullptr);
    
    /**
     * \if ENGLISH
     * @brief Destructor for Plot3DQuadFunction
     * \endif
     * 
     * \if CHINESE
     * @brief Plot3DQuadFunction 析构函数
     * \endif
     */
    ~Plot3DQuadFunction() override;
    
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
    
    // Fill color property accessors
    QColor fillColor() const { return m_fillColor; }
    void setFillColor(const QColor& color);
    
    // Line color property accessors
    QColor lineColor() const { return m_lineColor; }
    void setLineColor(const QColor& color);
    
    // Line width property accessors
    float lineWidth() const { return m_lineWidth; }
    void setLineWidth(float width);
    
    // Lines visibility property accessors
    bool linesVisible() const { return m_linesVisible; }
    void setLinesVisible(bool visible);
    
    // Fill visibility property accessors
    bool fillVisible() const { return m_fillVisible; }
    void setFillVisible(bool visible);
    
    // Markers visibility property accessors
    bool markersVisible() const { return m_markersVisible; }
    void setMarkersVisible(bool visible);
    
    // Marker shape property accessors
    QString markerShape() const { return m_markerShape; }
    void setMarkerShape(const QString& shape);
    
    // Marker size property accessors
    float markerSize() const { return m_markerSize; }
    void setMarkerSize(float size);
    
    // Marker weight property accessors
    float markerWeight() const { return m_markerWeight; }
    void setMarkerWeight(float weight);
    
    // Marker fill color property accessors
    QColor markerFillColor() const { return m_markerFillColor; }
    void setMarkerFillColor(const QColor& color);
    
    // Marker outline color property accessors
    QColor markerOutlineColor() const { return m_markerOutlineColor; }
    void setMarkerOutlineColor(const QColor& color);
    
    // Fill alpha property accessors
    float fillAlpha() const { return m_fillAlpha; }
    void setFillAlpha(float alpha);
    
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
     * @brief Signal emitted when line width changes
     * @param width New line width value
     * \endif
     * 
     * \if CHINESE
     * @brief 线宽改变时发出的信号
     * @param width 新的线宽值
     * \endif
     */
    void lineWidthChanged(float width);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when lines visibility changes
     * @param visible New lines visibility value
     * \endif
     * 
     * \if CHINESE
     * @brief 线条可见性改变时发出的信号
     * @param visible 新的线条可见性值
     * \endif
     */
    void linesVisibleChanged(bool visible);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when fill visibility changes
     * @param visible New fill visibility value
     * \endif
     * 
     * \if CHINESE
     * @brief 填充可见性改变时发出的信号
     * @param visible 新的填充可见性值
     * \endif
     */
    void fillVisibleChanged(bool visible);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when markers visibility changes
     * @param visible New markers visibility value
     * \endif
     * 
     * \if CHINESE
     * @brief 标记点可见性改变时发出的信号
     * @param visible 新的标记点可见性值
     * \endif
     */
    void markersVisibleChanged(bool visible);
    
    void markerShapeChanged(const QString& shape);
    
    void markerSizeChanged(float size);
    
    void markerWeightChanged(float weight);
    
    void markerFillColorChanged(const QColor& color);
    
    void markerOutlineColorChanged(const QColor& color);
    
    void fillAlphaChanged(float alpha);
    
private:
    QString m_title = QStringLiteral("3D Quad - Unit Cube");
    QString m_xLabel = QStringLiteral("X");
    QString m_yLabel = QStringLiteral("Y");
    QString m_zLabel = QStringLiteral("Z");
    QColor m_fillColor = QColor(0, 114, 189, 128);
    QColor m_lineColor = Qt::black;
    float m_lineWidth = 1.0f;
    bool m_linesVisible = true;
    bool m_fillVisible = true;
    bool m_markersVisible = false;
    QString m_markerShape = QStringLiteral("Circle");
    float m_markerSize = 4.0f;
    float m_markerWeight = 1.5f;
    QColor m_markerFillColor = QColor(0, 114, 189);
    QColor m_markerOutlineColor = QColor(0, 0, 0);
    float m_fillAlpha = 1.0f;
    
    QPointer<QIM::QImPlot3DNode> m_plot3DNode = nullptr;
    QPointer<QIM::QImPlot3DQuadItemNode> m_quad3DNode = nullptr;
};

#endif // PLOT3DQUADFUNCTION_H