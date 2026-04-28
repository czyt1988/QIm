#ifndef SCATTERFUNCTION_H
#define SCATTERFUNCTION_H

#include "../TestFunction.h"
#include <QObject>
#include <QColor>
#include <QSizeF>
#include "plot/QImPlot.h"

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
 * - Plot title, size, and flags (titleEnabled, legendEnabled, etc.)
 * - X and Y axis labels, limits, flags, and scale type
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
 * - 图表标题、尺寸和标志（titleEnabled、legendEnabled 等）
 * - X 和 Y 轴标签、范围限制、标志和刻度类型
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
    
    // --- Existing scatter properties ---
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString xLabel READ xLabel WRITE setXLabel NOTIFY xLabelChanged)
    Q_PROPERTY(QString yLabel READ yLabel WRITE setYLabel NOTIFY yLabelChanged)
    Q_PROPERTY(float markerSize READ markerSize WRITE setMarkerSize NOTIFY markerSizeChanged)
    Q_PROPERTY(int markerShape READ markerShape WRITE setMarkerShape NOTIFY markerShapeChanged)
    Q_PROPERTY(bool markerFill READ markerFill WRITE setMarkerFill NOTIFY markerFillChanged)
    Q_PROPERTY(QColor markerColor READ markerColor WRITE setMarkerColor NOTIFY markerColorChanged)
    Q_PROPERTY(bool clippingEnabled READ isClippingEnabled WRITE setClippingEnabled NOTIFY clippingEnabledChanged)
    Q_PROPERTY(int downsampleAlgorithm READ downsampleAlgorithm WRITE setDownsampleAlgorithm NOTIFY downsampleAlgorithmChanged)
    Q_PROPERTY(int downsampleThreshold READ downsampleThreshold WRITE setDownsampleThreshold NOTIFY downsampleThresholdChanged)
    
    // --- QImPlotNode: Size properties ---
    Q_PROPERTY(double plotWidth READ plotWidth WRITE setPlotWidth NOTIFY plotSizeChanged)
    Q_PROPERTY(double plotHeight READ plotHeight WRITE setPlotHeight NOTIFY plotSizeChanged)
    Q_PROPERTY(bool autoSize READ isAutoSize WRITE setAutoSize NOTIFY autoSizeChanged)
    
    // --- QImPlotNode: ImPlotFlags ---
    Q_PROPERTY(bool titleEnabled READ isTitleEnabled WRITE setTitleEnabled NOTIFY titleEnabledChanged)
    Q_PROPERTY(bool legendEnabled READ isLegendEnabled WRITE setLegendEnabled NOTIFY legendEnabledChanged)
    Q_PROPERTY(bool mouseTextEnabled READ isMouseTextEnabled WRITE setMouseTextEnabled NOTIFY mouseTextEnabledChanged)
    Q_PROPERTY(bool inputsEnabled READ isInputsEnabled WRITE setInputsEnabled NOTIFY inputsEnabledChanged)
    Q_PROPERTY(bool menusEnabled READ isMenusEnabled WRITE setMenusEnabled NOTIFY menusEnabledChanged)
    Q_PROPERTY(bool boxSelectEnabled READ isBoxSelectEnabled WRITE setBoxSelectEnabled NOTIFY boxSelectEnabledChanged)
    Q_PROPERTY(bool frameEnabled READ isFrameEnabled WRITE setFrameEnabled NOTIFY frameEnabledChanged)
    Q_PROPERTY(bool equal READ isEqual WRITE setEqual NOTIFY equalChanged)
    Q_PROPERTY(bool crosshairs READ isCrosshairs WRITE setCrosshairs NOTIFY crosshairsChanged)
    Q_PROPERTY(bool canvasEnabled READ isCanvasEnabled WRITE setCanvasEnabled NOTIFY canvasEnabledChanged)
    
    // --- QImPlotAxisInfo X1: Limits ---
    Q_PROPERTY(double x1MinLimits READ x1MinLimits WRITE setX1MinLimits NOTIFY x1LimitsChanged)
    Q_PROPERTY(double x1MaxLimits READ x1MaxLimits WRITE setX1MaxLimits NOTIFY x1LimitsChanged)
    // --- QImPlotAxisInfo X1: Flags ---
    Q_PROPERTY(bool x1AutoFit READ isX1AutoFit WRITE setX1AutoFit NOTIFY x1AxisFlagChanged)
    Q_PROPERTY(bool x1Inverted READ isX1Inverted WRITE setX1Inverted NOTIFY x1AxisFlagChanged)
    Q_PROPERTY(bool x1LabelEnabled READ isX1LabelEnabled WRITE setX1LabelEnabled NOTIFY x1AxisFlagChanged)
    Q_PROPERTY(bool x1GridLinesEnabled READ isX1GridLinesEnabled WRITE setX1GridLinesEnabled NOTIFY x1AxisFlagChanged)
    Q_PROPERTY(bool x1TickMarksEnabled READ isX1TickMarksEnabled WRITE setX1TickMarksEnabled NOTIFY x1AxisFlagChanged)
    Q_PROPERTY(bool x1TickLabelsEnabled READ isX1TickLabelsEnabled WRITE setX1TickLabelsEnabled NOTIFY x1AxisFlagChanged)
    Q_PROPERTY(bool x1LockMin READ isX1LockMin WRITE setX1LockMin NOTIFY x1AxisFlagChanged)
    Q_PROPERTY(bool x1LockMax READ isX1LockMax WRITE setX1LockMax NOTIFY x1AxisFlagChanged)
    Q_PROPERTY(bool x1NoDecorations READ isX1NoDecorations WRITE setX1NoDecorations NOTIFY x1AxisFlagChanged)
    // --- QImPlotAxisInfo X1: ScaleType ---
    Q_PROPERTY(int x1ScaleType READ x1ScaleType WRITE setX1ScaleType NOTIFY x1ScaleTypeChanged)
    
    // --- QImPlotAxisInfo Y1: Limits ---
    Q_PROPERTY(double y1MinLimits READ y1MinLimits WRITE setY1MinLimits NOTIFY y1LimitsChanged)
    Q_PROPERTY(double y1MaxLimits READ y1MaxLimits WRITE setY1MaxLimits NOTIFY y1LimitsChanged)
    // --- QImPlotAxisInfo Y1: Flags ---
    Q_PROPERTY(bool y1AutoFit READ isY1AutoFit WRITE setY1AutoFit NOTIFY y1AxisFlagChanged)
    Q_PROPERTY(bool y1Inverted READ isY1Inverted WRITE setY1Inverted NOTIFY y1AxisFlagChanged)
    Q_PROPERTY(bool y1LabelEnabled READ isY1LabelEnabled WRITE setY1LabelEnabled NOTIFY y1AxisFlagChanged)
    Q_PROPERTY(bool y1GridLinesEnabled READ isY1GridLinesEnabled WRITE setY1GridLinesEnabled NOTIFY y1AxisFlagChanged)
    Q_PROPERTY(bool y1TickMarksEnabled READ isY1TickMarksEnabled WRITE setY1TickMarksEnabled NOTIFY y1AxisFlagChanged)
    Q_PROPERTY(bool y1TickLabelsEnabled READ isY1TickLabelsEnabled WRITE setY1TickLabelsEnabled NOTIFY y1AxisFlagChanged)
    Q_PROPERTY(bool y1LockMin READ isY1LockMin WRITE setY1LockMin NOTIFY y1AxisFlagChanged)
    Q_PROPERTY(bool y1LockMax READ isY1LockMax WRITE setY1LockMax NOTIFY y1AxisFlagChanged)
    Q_PROPERTY(bool y1NoDecorations READ isY1NoDecorations WRITE setY1NoDecorations NOTIFY y1AxisFlagChanged)
    // --- QImPlotAxisInfo Y1: ScaleType ---
    Q_PROPERTY(int y1ScaleType READ y1ScaleType WRITE setY1ScaleType NOTIFY y1ScaleTypeChanged)
    
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
    
    // Clipping enabled property accessors
    bool isClippingEnabled() const { return m_clippingEnabled; }
    void setClippingEnabled(bool enabled);
    
    // Downsample algorithm property accessors
    int downsampleAlgorithm() const { return static_cast<int>(m_downsampleAlgorithm); }
    void setDownsampleAlgorithm(int algo);
    
    // Downsample threshold property accessors
    int downsampleThreshold() const { return m_downsampleThreshold; }
    void setDownsampleThreshold(int threshold);
    
    // --- QImPlotNode: Size accessors ---
    double plotWidth() const { return m_plotSize.width(); }
    void setPlotWidth(double width);
    double plotHeight() const { return m_plotSize.height(); }
    void setPlotHeight(double height);
    bool isAutoSize() const { return m_autoSize; }
    void setAutoSize(bool autoSize);
    
    // --- QImPlotNode: ImPlotFlags accessors ---
    bool isTitleEnabled() const { return m_titleEnabled; }
    void setTitleEnabled(bool enabled);
    bool isLegendEnabled() const { return m_legendEnabled; }
    void setLegendEnabled(bool enabled);
    bool isMouseTextEnabled() const { return m_mouseTextEnabled; }
    void setMouseTextEnabled(bool enabled);
    bool isInputsEnabled() const { return m_inputsEnabled; }
    void setInputsEnabled(bool enabled);
    bool isMenusEnabled() const { return m_menusEnabled; }
    void setMenusEnabled(bool enabled);
    bool isBoxSelectEnabled() const { return m_boxSelectEnabled; }
    void setBoxSelectEnabled(bool enabled);
    bool isFrameEnabled() const { return m_frameEnabled; }
    void setFrameEnabled(bool enabled);
    bool isEqual() const { return m_equal; }
    void setEqual(bool on);
    bool isCrosshairs() const { return m_crosshairs; }
    void setCrosshairs(bool on);
    bool isCanvasEnabled() const { return m_canvasEnabled; }
    void setCanvasEnabled(bool enabled);
    
    // --- QImPlotAxisInfo X1: Limits accessors ---
    double x1MinLimits() const { return m_x1MinLimits; }
    void setX1MinLimits(double min);
    double x1MaxLimits() const { return m_x1MaxLimits; }
    void setX1MaxLimits(double max);
    // --- QImPlotAxisInfo X1: Flags accessors ---
    bool isX1AutoFit() const { return m_x1AutoFit; }
    void setX1AutoFit(bool on);
    bool isX1Inverted() const { return m_x1Inverted; }
    void setX1Inverted(bool on);
    bool isX1LabelEnabled() const { return m_x1LabelEnabled; }
    void setX1LabelEnabled(bool enabled);
    bool isX1GridLinesEnabled() const { return m_x1GridLinesEnabled; }
    void setX1GridLinesEnabled(bool enabled);
    bool isX1TickMarksEnabled() const { return m_x1TickMarksEnabled; }
    void setX1TickMarksEnabled(bool enabled);
    bool isX1TickLabelsEnabled() const { return m_x1TickLabelsEnabled; }
    void setX1TickLabelsEnabled(bool enabled);
    bool isX1LockMin() const { return m_x1LockMin; }
    void setX1LockMin(bool on);
    bool isX1LockMax() const { return m_x1LockMax; }
    void setX1LockMax(bool on);
    bool isX1NoDecorations() const { return m_x1NoDecorations; }
    void setX1NoDecorations(bool enabled);
    // --- QImPlotAxisInfo X1: ScaleType accessor ---
    int x1ScaleType() const { return static_cast<int>(m_x1ScaleType); }
    void setX1ScaleType(int type);
    
    // --- QImPlotAxisInfo Y1: Limits accessors ---
    double y1MinLimits() const { return m_y1MinLimits; }
    void setY1MinLimits(double min);
    double y1MaxLimits() const { return m_y1MaxLimits; }
    void setY1MaxLimits(double max);
    // --- QImPlotAxisInfo Y1: Flags accessors ---
    bool isY1AutoFit() const { return m_y1AutoFit; }
    void setY1AutoFit(bool on);
    bool isY1Inverted() const { return m_y1Inverted; }
    void setY1Inverted(bool on);
    bool isY1LabelEnabled() const { return m_y1LabelEnabled; }
    void setY1LabelEnabled(bool enabled);
    bool isY1GridLinesEnabled() const { return m_y1GridLinesEnabled; }
    void setY1GridLinesEnabled(bool enabled);
    bool isY1TickMarksEnabled() const { return m_y1TickMarksEnabled; }
    void setY1TickMarksEnabled(bool enabled);
    bool isY1TickLabelsEnabled() const { return m_y1TickLabelsEnabled; }
    void setY1TickLabelsEnabled(bool enabled);
    bool isY1LockMin() const { return m_y1LockMin; }
    void setY1LockMin(bool on);
    bool isY1LockMax() const { return m_y1LockMax; }
    void setY1LockMax(bool on);
    bool isY1NoDecorations() const { return m_y1NoDecorations; }
    void setY1NoDecorations(bool enabled);
    // --- QImPlotAxisInfo Y1: ScaleType accessor ---
    int y1ScaleType() const { return static_cast<int>(m_y1ScaleType); }
    void setY1ScaleType(int type);
    
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
    
    void clippingEnabledChanged(bool enabled);
    void downsampleAlgorithmChanged(int algo);
    void downsampleThresholdChanged(int threshold);
    
    // --- QImPlotNode: Size signals ---
    void plotSizeChanged();
    void autoSizeChanged(bool autoSize);
    
    // --- QImPlotNode: ImPlotFlags signals ---
    void titleEnabledChanged(bool enabled);
    void legendEnabledChanged(bool enabled);
    void mouseTextEnabledChanged(bool enabled);
    void inputsEnabledChanged(bool enabled);
    void menusEnabledChanged(bool enabled);
    void boxSelectEnabledChanged(bool enabled);
    void frameEnabledChanged(bool enabled);
    void equalChanged(bool on);
    void crosshairsChanged(bool on);
    void canvasEnabledChanged(bool enabled);
    
    // --- QImPlotAxisInfo X1 signals ---
    void x1LimitsChanged(double min, double max);
    void x1AxisFlagChanged();
    void x1ScaleTypeChanged();
    
    // --- QImPlotAxisInfo Y1 signals ---
    void y1LimitsChanged(double min, double max);
    void y1AxisFlagChanged();
    void y1ScaleTypeChanged();
    
private:
    QString m_title = QStringLiteral("Scatter Plot");
    QString m_xLabel = QStringLiteral("x3");
    QString m_yLabel = QStringLiteral("y3");
    float m_markerSize = 4.0f;
    int m_markerShape = 0;  // ImPlotMarker_Circle
    bool m_markerFill = true;
    QColor m_markerColor = Qt::blue;
    bool m_clippingEnabled = true;
    QIM::QImDownsampleAlgorithm m_downsampleAlgorithm = QIM::QImDownsampleAlgorithm::None;
    int m_downsampleThreshold = 0;
    
    // --- QImPlotNode: Size members ---
    QSizeF m_plotSize = QSizeF(-1, -1);  // -1 means auto/default
    bool m_autoSize = true;
    
    // --- QImPlotNode: ImPlotFlags members ---
    bool m_titleEnabled = true;
    bool m_legendEnabled = true;
    bool m_mouseTextEnabled = true;
    bool m_inputsEnabled = true;
    bool m_menusEnabled = true;
    bool m_boxSelectEnabled = true;
    bool m_frameEnabled = true;
    bool m_equal = false;
    bool m_crosshairs = false;
    bool m_canvasEnabled = false;
    
    // --- QImPlotAxisInfo X1 members ---
    double m_x1MinLimits = 0.0;
    double m_x1MaxLimits = 10.0;
    bool m_x1AutoFit = true;
    bool m_x1Inverted = false;
    bool m_x1LabelEnabled = true;
    bool m_x1GridLinesEnabled = true;
    bool m_x1TickMarksEnabled = true;
    bool m_x1TickLabelsEnabled = true;
    bool m_x1LockMin = false;
    bool m_x1LockMax = false;
    bool m_x1NoDecorations = false;
    QIM::QImPlotScaleType m_x1ScaleType = QIM::QImPlotScaleType::Linear;
    
    // --- QImPlotAxisInfo Y1 members ---
    double m_y1MinLimits = 0.0;
    double m_y1MaxLimits = 10.0;
    bool m_y1AutoFit = true;
    bool m_y1Inverted = false;
    bool m_y1LabelEnabled = true;
    bool m_y1GridLinesEnabled = true;
    bool m_y1TickMarksEnabled = true;
    bool m_y1TickLabelsEnabled = true;
    bool m_y1LockMin = false;
    bool m_y1LockMax = false;
    bool m_y1NoDecorations = false;
    QIM::QImPlotScaleType m_y1ScaleType = QIM::QImPlotScaleType::Linear;
    
    QIM::QImPlotNode* m_plotNode = nullptr;
    QIM::QImPlotScatterItemNode* m_scatterNode = nullptr;
    QIM::QImPlotValueTrackerNode* m_trackerNode = nullptr;
};

#endif // SCATTERFUNCTION_H
