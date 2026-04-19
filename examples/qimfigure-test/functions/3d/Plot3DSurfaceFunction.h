#ifndef PLOT3DSURFACEFUNCTION_H
#define PLOT3DSURFACEFUNCTION_H

#include "../TestFunction.h"
#include <QObject>
#include <QColor>

namespace QIM {
class QImFigureWidget;
class QImPlot3DNode;
class QImPlot3DSurfaceItemNode;
}

/**
 * \if ENGLISH
 * @brief Test function demonstrating 3D surface plot with property registration
 * 
 * Plot3DSurfaceFunction creates a 3D plot with a sin(x)*cos(y) surface to test
 * 3D surface rendering capabilities. It registers properties for:
 * - Plot title
 * - X, Y and Z axis labels
 * - Surface fill color and visibility
 * - Line color, width and visibility
 * - Markers visibility
 * - Colormap enable/disable
 * 
 * @section usage_example Usage Example
 * 
 * \code
 * auto func = new Plot3DSurfaceFunction(this);
 * func->createPlot(figureWidget);
 * // Properties will be available via getRegisteredProperties()
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlot3DNode
 * @see QImPlot3DSurfaceItemNode
 * \endif
 * 
 * \if CHINESE
 * @brief 演示 3D 曲面图的测试函数，带有属性注册
 * 
 * Plot3DSurfaceFunction 创建一个包含 sin(x)*cos(y) 曲面的 3D 图表，用于测试
 * 3D 曲面渲染能力。它注册了以下属性：
 * - 图表标题
 * - X、Y 和 Z 轴标签
 * - 曲面填充颜色和可见性
 * - 线条颜色、宽度和可见性
 * - 标记点可见性
 * - 颜色映射启用/禁用
 * 
 * @section usage_example 使用示例
 * 
 * \code
 * auto func = new Plot3DSurfaceFunction(this);
 * func->createPlot(figureWidget);
 * // 属性可通过 getRegisteredProperties() 获取
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlot3DNode
 * @see QImPlot3DSurfaceItemNode
 * \endif
 */
class Plot3DSurfaceFunction : public TestFunction {
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
    Q_PROPERTY(bool colormapEnabled READ colormapEnabled WRITE setColormapEnabled NOTIFY colormapEnabledChanged)
    
public:
    /**
     * \if ENGLISH
     * @brief Constructor for Plot3DSurfaceFunction
     * @param parent Parent object
     * \endif
     * 
     * \if CHINESE
     * @brief Plot3DSurfaceFunction 构造函数
     * @param parent 父对象
     * \endif
     */
    explicit Plot3DSurfaceFunction(QObject* parent = nullptr);
    
    /**
     * \if ENGLISH
     * @brief Destructor for Plot3DSurfaceFunction
     * \endif
     * 
     * \if CHINESE
     * @brief Plot3DSurfaceFunction 析构函数
     * \endif
     */
    ~Plot3DSurfaceFunction() override;
    
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
    
    // Fill color property accessors
    QColor fillColor() const { return m_fillColor; }
    void setFillColor(const QColor& color);
    
    // Line color property accessors
    QColor lineColor() const { return m_lineColor; }
    void setLineColor(const QColor& color);
    
    // Line width property accessors
    float lineWidth() const { return m_lineWidth; }
    void setLineWidth(float width);
    
    // Lines visible property accessors
    bool linesVisible() const { return m_linesVisible; }
    void setLinesVisible(bool visible);
    
    // Fill visible property accessors
    bool fillVisible() const { return m_fillVisible; }
    void setFillVisible(bool visible);
    
    // Markers visible property accessors
    bool markersVisible() const { return m_markersVisible; }
    void setMarkersVisible(bool visible);
    
    // Colormap enabled property accessors
    bool colormapEnabled() const { return m_colormapEnabled; }
    void setColormapEnabled(bool enabled);
    
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
     * @brief Signal emitted when lines visible changes
     * @param visible New lines visible value
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
     * @brief Signal emitted when fill visible changes
     * @param visible New fill visible value
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
     * @brief Signal emitted when markers visible changes
     * @param visible New markers visible value
     * \endif
     * 
     * \if CHINESE
     * @brief 标记点可见性改变时发出的信号
     * @param visible 新的标记点可见性值
     * \endif
     */
    void markersVisibleChanged(bool visible);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when colormap enabled changes
     * @param enabled New colormap enabled value
     * \endif
     * 
     * \if CHINESE
     * @brief 颜色映射启用改变时发出的信号
     * @param enabled 新的颜色映射启用值
     * \endif
     */
    void colormapEnabledChanged(bool enabled);
    
private:
    QString m_title = QStringLiteral("3D Surface - sin(x)*cos(y)");
    QString m_xLabel = QStringLiteral("X");
    QString m_yLabel = QStringLiteral("Y");
    QString m_zLabel = QStringLiteral("Z");
    QColor m_fillColor = Qt::blue;
    QColor m_lineColor = Qt::black;
    float m_lineWidth = 1.0f;
    bool m_linesVisible = true;
    bool m_fillVisible = true;
    bool m_markersVisible = false;
    bool m_colormapEnabled = true;
    
    QIM::QImPlot3DNode* m_plot3DNode = nullptr;
    QIM::QImPlot3DSurfaceItemNode* m_surface3DNode = nullptr;
};

#endif // PLOT3DSURFACEFUNCTION_H
