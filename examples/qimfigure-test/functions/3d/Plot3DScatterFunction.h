#ifndef PLOT3DSCATTERFUNCTION_H
#define PLOT3DSCATTERFUNCTION_H

#include "../TestFunction.h"
#include <QObject>
#include <QColor>
#include <QSizeF>
#include <QPointer>
#include "plot3d/QImPlot3D.h"

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
 * @see QImPlot3DExtNode
 * @see QImPlot3DExtScatterItemNode
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
 * @see QImPlot3DExtNode
 * @see QImPlot3DExtScatterItemNode
 * \endif
 */
class Plot3DScatterFunction : public TestFunction {
    Q_OBJECT
    
    // Existing properties
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString xLabel READ xLabel WRITE setXLabel NOTIFY xLabelChanged)
    Q_PROPERTY(QString yLabel READ yLabel WRITE setYLabel NOTIFY yLabelChanged)
    Q_PROPERTY(QString zLabel READ zLabel WRITE setZLabel NOTIFY zLabelChanged)
    Q_PROPERTY(float markerSize READ markerSize WRITE setMarkerSize NOTIFY markerSizeChanged)
    Q_PROPERTY(int markerShape READ markerShape WRITE setMarkerShape NOTIFY markerShapeChanged)
    Q_PROPERTY(QColor markerFillColor READ markerFillColor WRITE setMarkerFillColor NOTIFY markerFillColorChanged)

    // QImPlot3DNode - Size properties
    Q_PROPERTY(double plotWidth READ plotWidth WRITE setPlotWidth NOTIFY plotWidthChanged)
    Q_PROPERTY(double plotHeight READ plotHeight WRITE setPlotHeight NOTIFY plotHeightChanged)
    Q_PROPERTY(bool autoSize READ isAutoSize WRITE setAutoSize NOTIFY autoSizeChanged)

    // QImPlot3DNode - Flags (NoXxx -> xxxEnabled)
    Q_PROPERTY(bool titleEnabled READ isTitleEnabled WRITE setTitleEnabled NOTIFY titleEnabledChanged)
    Q_PROPERTY(bool legendEnabled READ isLegendEnabled WRITE setLegendEnabled NOTIFY legendEnabledChanged)
    Q_PROPERTY(bool mouseTextEnabled READ isMouseTextEnabled WRITE setMouseTextEnabled NOTIFY mouseTextEnabledChanged)
    Q_PROPERTY(bool clipEnabled READ isClipEnabled WRITE setClipEnabled NOTIFY clipEnabledChanged)
    Q_PROPERTY(bool menusEnabled READ isMenusEnabled WRITE setMenusEnabled NOTIFY menusEnabledChanged)
    Q_PROPERTY(bool rotateEnabled READ isRotateEnabled WRITE setRotateEnabled NOTIFY rotateEnabledChanged)
    Q_PROPERTY(bool panEnabled READ isPanEnabled WRITE setPanEnabled NOTIFY panEnabledChanged)
    Q_PROPERTY(bool zoomEnabled READ isZoomEnabled WRITE setZoomEnabled NOTIFY zoomEnabledChanged)
    Q_PROPERTY(bool inputsEnabled READ isInputsEnabled WRITE setInputsEnabled NOTIFY inputsEnabledChanged)
    Q_PROPERTY(bool equal READ isEqual WRITE setEqual NOTIFY equalChanged)
    Q_PROPERTY(bool canvasEnabled READ isCanvasEnabled WRITE setCanvasEnabled NOTIFY canvasEnabledChanged)

    // QImPlot3DNode - Legend configuration
    Q_PROPERTY(int legendLocation READ legendLocation WRITE setLegendLocation NOTIFY legendLocationChanged)
    Q_PROPERTY(int legendFlags READ legendFlags WRITE setLegendFlags NOTIFY legendFlagsChanged)

    // QImPlot3DAxisInfo - X axis properties
    Q_PROPERTY(double xAxisMinLimit READ xAxisMinLimit WRITE setXAxisMinLimit NOTIFY xAxisMinLimitChanged)
    Q_PROPERTY(double xAxisMaxLimit READ xAxisMaxLimit WRITE setXAxisMaxLimit NOTIFY xAxisMaxLimitChanged)
    Q_PROPERTY(bool xAxisLabelEnabled READ isXAxisLabelEnabled WRITE setXAxisLabelEnabled NOTIFY xAxisLabelEnabledChanged)
    Q_PROPERTY(bool xAxisGridLinesEnabled READ isXAxisGridLinesEnabled WRITE setXAxisGridLinesEnabled NOTIFY xAxisGridLinesEnabledChanged)
    Q_PROPERTY(bool xAxisTickMarksEnabled READ isXAxisTickMarksEnabled WRITE setXAxisTickMarksEnabled NOTIFY xAxisTickMarksEnabledChanged)
    Q_PROPERTY(bool xAxisTickLabelsEnabled READ isXAxisTickLabelsEnabled WRITE setXAxisTickLabelsEnabled NOTIFY xAxisTickLabelsEnabledChanged)
    Q_PROPERTY(bool xAxisLockMin READ isXAxisLockMin WRITE setXAxisLockMin NOTIFY xAxisLockMinChanged)
    Q_PROPERTY(bool xAxisLockMax READ isXAxisLockMax WRITE setXAxisLockMax NOTIFY xAxisLockMaxChanged)
    Q_PROPERTY(bool xAxisAutoFit READ isXAxisAutoFit WRITE setXAxisAutoFit NOTIFY xAxisAutoFitChanged)
    Q_PROPERTY(bool xAxisInvert READ isXAxisInvert WRITE setXAxisInvert NOTIFY xAxisInvertChanged)
    Q_PROPERTY(bool xAxisDecorationsEnabled READ isXAxisDecorationsEnabled WRITE setXAxisDecorationsEnabled NOTIFY xAxisDecorationsEnabledChanged)
    Q_PROPERTY(int xAxisScale READ xAxisScale WRITE setXAxisScale NOTIFY xAxisScaleChanged)

    // QImPlot3DScatterItemNode - Additional marker properties
    Q_PROPERTY(QColor markerOutlineColor READ markerOutlineColor WRITE setMarkerOutlineColor NOTIFY markerOutlineColorChanged)
    Q_PROPERTY(float markerWeight READ markerWeight WRITE setMarkerWeight NOTIFY markerWeightChanged)
    
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
    
    // Marker size property accessors
    float markerSize() const { return m_markerSize; }
    void setMarkerSize(float size);
    
    // Marker shape property accessors
    int markerShape() const { return m_markerShape; }
    void setMarkerShape(int shape);
    
    // Marker fill color property accessors (renamed from markerColor to match underlying API)
    QColor markerFillColor() const { return m_markerFillColor; }
    void setMarkerFillColor(const QColor& color);

    // Marker outline color property accessors
    QColor markerOutlineColor() const { return m_markerOutlineColor; }
    void setMarkerOutlineColor(const QColor& color);

    // Marker weight property accessors
    float markerWeight() const { return m_markerWeight; }
    void setMarkerWeight(float weight);

    // QImPlot3DNode - Size property accessors
    double plotWidth() const { return m_plotWidth; }
    void setPlotWidth(double width);
    double plotHeight() const { return m_plotHeight; }
    void setPlotHeight(double height);
    bool isAutoSize() const { return m_autoSize; }
    void setAutoSize(bool enabled);

    // QImPlot3DNode - Flags property accessors
    bool isTitleEnabled() const { return m_titleEnabled; }
    void setTitleEnabled(bool enabled);
    bool isLegendEnabled() const { return m_legendEnabled; }
    void setLegendEnabled(bool enabled);
    bool isMouseTextEnabled() const { return m_mouseTextEnabled; }
    void setMouseTextEnabled(bool enabled);
    bool isClipEnabled() const { return m_clipEnabled; }
    void setClipEnabled(bool enabled);
    bool isMenusEnabled() const { return m_menusEnabled; }
    void setMenusEnabled(bool enabled);
    bool isRotateEnabled() const { return m_rotateEnabled; }
    void setRotateEnabled(bool enabled);
    bool isPanEnabled() const { return m_panEnabled; }
    void setPanEnabled(bool enabled);
    bool isZoomEnabled() const { return m_zoomEnabled; }
    void setZoomEnabled(bool enabled);
    bool isInputsEnabled() const { return m_inputsEnabled; }
    void setInputsEnabled(bool enabled);
    bool isEqual() const { return m_equal; }
    void setEqual(bool on);
    bool isCanvasEnabled() const { return m_canvasEnabled; }
    void setCanvasEnabled(bool enabled);

    // QImPlot3DNode - Legend configuration accessors
    int legendLocation() const { return m_legendLocation; }
    void setLegendLocation(int location);
    int legendFlags() const { return m_legendFlags; }
    void setLegendFlags(int flags);

    // QImPlot3DAxisInfo - X axis property accessors
    double xAxisMinLimit() const { return m_xAxisMinLimit; }
    void setXAxisMinLimit(double min);
    double xAxisMaxLimit() const { return m_xAxisMaxLimit; }
    void setXAxisMaxLimit(double max);
    bool isXAxisLabelEnabled() const { return m_xAxisLabelEnabled; }
    void setXAxisLabelEnabled(bool enabled);
    bool isXAxisGridLinesEnabled() const { return m_xAxisGridLinesEnabled; }
    void setXAxisGridLinesEnabled(bool enabled);
    bool isXAxisTickMarksEnabled() const { return m_xAxisTickMarksEnabled; }
    void setXAxisTickMarksEnabled(bool enabled);
    bool isXAxisTickLabelsEnabled() const { return m_xAxisTickLabelsEnabled; }
    void setXAxisTickLabelsEnabled(bool enabled);
    bool isXAxisLockMin() const { return m_xAxisLockMin; }
    void setXAxisLockMin(bool on);
    bool isXAxisLockMax() const { return m_xAxisLockMax; }
    void setXAxisLockMax(bool on);
    bool isXAxisAutoFit() const { return m_xAxisAutoFit; }
    void setXAxisAutoFit(bool on);
    bool isXAxisInvert() const { return m_xAxisInvert; }
    void setXAxisInvert(bool on);
    bool isXAxisDecorationsEnabled() const { return m_xAxisDecorationsEnabled; }
    void setXAxisDecorationsEnabled(bool enabled);
    int xAxisScale() const { return m_xAxisScale; }
    void setXAxisScale(int scale);
    
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
     * @brief Signal emitted when marker fill color changes
     * @param color New marker fill color value
     * \endif
     * 
     * \if CHINESE
     * @brief 标记填充颜色改变时发出的信号
     * @param color 新的标记填充颜色值
     * \endif
     */
    void markerFillColorChanged(const QColor& color);

    /**
     * \if ENGLISH
     * @brief Signal emitted when marker outline color changes
     * @param color New marker outline color value
     * \endif
     * 
     * \if CHINESE
     * @brief 标记描边颜色改变时发出的信号
     * @param color 新的标记描边颜色值
     * \endif
     */
    void markerOutlineColorChanged(const QColor& color);

    /**
     * \if ENGLISH
     * @brief Signal emitted when marker weight changes
     * @param weight New marker weight value
     * \endif
     * 
     * \if CHINESE
     * @brief 标记描边粗细改变时发出的信号
     * @param weight 新的标记描边粗细值
     * \endif
     */
    void markerWeightChanged(float weight);

    // QImPlot3DNode - Size signals
    void plotWidthChanged(double width);
    void plotHeightChanged(double height);
    void autoSizeChanged(bool enabled);

    // QImPlot3DNode - Flags signals
    void titleEnabledChanged(bool enabled);
    void legendEnabledChanged(bool enabled);
    void mouseTextEnabledChanged(bool enabled);
    void clipEnabledChanged(bool enabled);
    void menusEnabledChanged(bool enabled);
    void rotateEnabledChanged(bool enabled);
    void panEnabledChanged(bool enabled);
    void zoomEnabledChanged(bool enabled);
    void inputsEnabledChanged(bool enabled);
    void equalChanged(bool on);
    void canvasEnabledChanged(bool enabled);

    // QImPlot3DNode - Legend configuration signals
    void legendLocationChanged(int location);
    void legendFlagsChanged(int flags);

    // QImPlot3DAxisInfo - X axis signals
    void xAxisMinLimitChanged(double min);
    void xAxisMaxLimitChanged(double max);
    void xAxisLabelEnabledChanged(bool enabled);
    void xAxisGridLinesEnabledChanged(bool enabled);
    void xAxisTickMarksEnabledChanged(bool enabled);
    void xAxisTickLabelsEnabledChanged(bool enabled);
    void xAxisLockMinChanged(bool on);
    void xAxisLockMaxChanged(bool on);
    void xAxisAutoFitChanged(bool on);
    void xAxisInvertChanged(bool on);
    void xAxisDecorationsEnabledChanged(bool enabled);
    void xAxisScaleChanged(int scale);
    
private:
    QString m_title = QStringLiteral("3D Scatter Plot");
    QString m_xLabel = QStringLiteral("X");
    QString m_yLabel = QStringLiteral("Y");
    QString m_zLabel = QStringLiteral("Z");
    float m_markerSize = 4.0f;
    int m_markerShape = 0;  // QImPlot3DMarkerShape::Circle
    QColor m_markerFillColor = Qt::blue;
    QColor m_markerOutlineColor = QColor(120, 45, 10);
    float m_markerWeight = 1.0f;

    // QImPlot3DNode - Size members
    double m_plotWidth = 400.0;
    double m_plotHeight = 400.0;
    bool m_autoSize = true;

    // QImPlot3DNode - Flags members
    bool m_titleEnabled = true;
    bool m_legendEnabled = true;
    bool m_mouseTextEnabled = true;
    bool m_clipEnabled = true;
    bool m_menusEnabled = true;
    bool m_rotateEnabled = true;
    bool m_panEnabled = true;
    bool m_zoomEnabled = true;
    bool m_inputsEnabled = true;
    bool m_equal = false;
    bool m_canvasEnabled = true;

    // QImPlot3DNode - Legend configuration members
    int m_legendLocation = static_cast<int>(QIM::QImPlot3DLocation::NorthWest);
    int m_legendFlags = 0;

    // QImPlot3DAxisInfo - X axis members
    double m_xAxisMinLimit = 0.0;
    double m_xAxisMaxLimit = 1.0;
    bool m_xAxisLabelEnabled = true;
    bool m_xAxisGridLinesEnabled = true;
    bool m_xAxisTickMarksEnabled = true;
    bool m_xAxisTickLabelsEnabled = true;
    bool m_xAxisLockMin = false;
    bool m_xAxisLockMax = false;
    bool m_xAxisAutoFit = true;
    bool m_xAxisInvert = false;
    bool m_xAxisDecorationsEnabled = true;
    int m_xAxisScale = static_cast<int>(QIM::QImPlot3DScaleType::Linear);
    
    QPointer<QIM::QImPlot3DNode> m_plot3DNode = nullptr;
    QPointer<QIM::QImPlot3DScatterItemNode> m_scatter3DNode = nullptr;
};

#endif // PLOT3DSCATTERFUNCTION_H
