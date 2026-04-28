#ifndef LINE10KFUNCTION_H
#define LINE10KFUNCTION_H

#include "../TestFunction.h"
#include <QObject>
#include <QColor>
#include "plot/QImPlot.h"

namespace QIM {
class QImFigureWidget;
class QImPlotNode;
class QImPlotLineItemNode;
class QImPlotValueTrackerNode;
}

/**
 * \if ENGLISH
 * @brief Test function demonstrating 10K point line plot with property registration
 * 
 * Line10KFunction creates a plot with 10,000 cosine wave points to test
 * high-performance rendering capabilities. It registers properties for:
 * - Plot title
 * - X and Y axis labels
 * - Line color
 * - Line label
 * 
 * @section usage_example Usage Example
 * 
 * \code
 * auto func = new Line10KFunction(this);
 * func->createPlot(figureWidget);
 * // Properties will be available via getRegisteredProperties()
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotNode
 * @see QImPlotLineItemNode
 * \endif
 * 
 * \if CHINESE
 * @brief 演示 10K 点线图的测试函数，带有属性注册
 * 
 * Line10KFunction 创建一个包含 10,000 个余弦波点的图表，用于测试
 * 高性能渲染能力。它注册了以下属性：
 * - 图表标题
 * - X 和 Y 轴标签
 * - 线条颜色
 * - 线条标签
 * 
 * @section usage_example 使用示例
 * 
 * \code
 * auto func = new Line10KFunction(this);
 * func->createPlot(figureWidget);
 * // 属性可通过 getRegisteredProperties() 获取
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotNode
 * @see QImPlotLineItemNode
 * \endif
 */
class Line10KFunction : public TestFunction {
    Q_OBJECT
    
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString xLabel READ xLabel WRITE setXLabel NOTIFY xLabelChanged)
    Q_PROPERTY(QString yLabel READ yLabel WRITE setYLabel NOTIFY yLabelChanged)
    Q_PROPERTY(QColor lineColor READ lineColor WRITE setLineColor NOTIFY lineColorChanged)
    Q_PROPERTY(QString lineLabel READ lineLabel WRITE setLineLabel NOTIFY lineLabelChanged)
    
    // Line flags (affirmative semantics)
    Q_PROPERTY(bool segments READ isSegments WRITE setSegments NOTIFY segmentsChanged)
    Q_PROPERTY(bool loop READ isLoop WRITE setLoop NOTIFY loopChanged)
    Q_PROPERTY(bool skipNaN READ isSkipNaN WRITE setSkipNaN NOTIFY skipNaNChanged)
    Q_PROPERTY(bool clippingEnabled READ isClippingEnabled WRITE setClippingEnabled NOTIFY clippingEnabledChanged)
    Q_PROPERTY(bool shaded READ isShaded WRITE setShaded NOTIFY shadedChanged)
    // Downsampling
    Q_PROPERTY(int downsampleAlgorithm READ downsampleAlgorithm WRITE setDownsampleAlgorithm NOTIFY downsampleAlgorithmChanged)
    Q_PROPERTY(int downsampleThreshold READ downsampleThreshold WRITE setDownsampleThreshold NOTIFY downsampleThresholdChanged)
    
public:
    /**
     * \if ENGLISH
     * @brief Constructor for Line10KFunction
     * @param parent Parent object
     * \endif
     * 
     * \if CHINESE
     * @brief Line10KFunction 构造函数
     * @param parent 父对象
     * \endif
     */
    explicit Line10KFunction(QObject* parent = nullptr);
    
    /**
     * \if ENGLISH
     * @brief Destructor for Line10KFunction
     * \endif
     * 
     * \if CHINESE
     * @brief Line10KFunction 析构函数
     * \endif
     */
    ~Line10KFunction() override;
    
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
    
    // Line color property accessors
    QColor lineColor() const { return m_lineColor; }
    void setLineColor(const QColor& color);
    
    // Line label property accessors
    QString lineLabel() const { return m_lineLabel; }
    void setLineLabel(const QString& label);
    
    // Line flag accessors (affirmative semantics)
    bool isSegments() const { return m_segments; }
    void setSegments(bool on);
    
    bool isLoop() const { return m_loop; }
    void setLoop(bool on);
    
    bool isSkipNaN() const { return m_skipNaN; }
    void setSkipNaN(bool on);
    
    bool isClippingEnabled() const { return m_clippingEnabled; }
    void setClippingEnabled(bool enabled);
    
    bool isShaded() const { return m_shaded; }
    void setShaded(bool on);
    
    // Downsampling accessors
    int downsampleAlgorithm() const { return static_cast<int>(m_downsampleAlgorithm); }
    void setDownsampleAlgorithm(int algo);
    
    int downsampleThreshold() const { return m_downsampleThreshold; }
    void setDownsampleThreshold(int threshold);
    
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
     * @brief Signal emitted when line label changes
     * @param label New line label value
     * \endif
     * 
     * \if CHINESE
     * @brief 线条标签改变时发出的信号
     * @param label 新的线条标签值
     * \endif
     */
    void lineLabelChanged(const QString& label);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when segments flag changes
     * @param on New segments value
     * \endif
     * 
     * \if CHINESE
     * @brief 分段标志改变时发出的信号
     * @param on 新的分段值
     * \endif
     */
    void segmentsChanged(bool on);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when loop flag changes
     * @param on New loop value
     * \endif
     * 
     * \if CHINESE
     * @brief 循环标志改变时发出的信号
     * @param on 新的循环值
     * \endif
     */
    void loopChanged(bool on);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when skipNaN flag changes
     * @param on New skipNaN value
     * \endif
     * 
     * \if CHINESE
     * @brief 跳过NaN标志改变时发出的信号
     * @param on 新的跳过NaN值
     * \endif
     */
    void skipNaNChanged(bool on);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when clippingEnabled flag changes
     * @param enabled New clippingEnabled value
     * \endif
     * 
     * \if CHINESE
     * @brief 裁剪启用标志改变时发出的信号
     * @param enabled 新的裁剪启用值
     * \endif
     */
    void clippingEnabledChanged(bool enabled);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when shaded flag changes
     * @param on New shaded value
     * \endif
     * 
     * \if CHINESE
     * @brief 阴影标志改变时发出的信号
     * @param on 新的阴影值
     * \endif
     */
    void shadedChanged(bool on);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when downsample algorithm changes
     * @param algo New downsample algorithm value
     * \endif
     * 
     * \if CHINESE
     * @brief 降采样算法改变时发出的信号
     * @param algo 新的降采样算法值
     * \endif
     */
    void downsampleAlgorithmChanged(int algo);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when downsample threshold changes
     * @param threshold New downsample threshold value
     * \endif
     * 
     * \if CHINESE
     * @brief 降采样阈值改变时发出的信号
     * @param threshold 新的降采样阈值值
     * \endif
     */
    void downsampleThresholdChanged(int threshold);
    
private:
    QString m_title = QStringLiteral("10K Points");
    QString m_xLabel = QStringLiteral("x1");
    QString m_yLabel = QStringLiteral("y1");
    QColor m_lineColor = Qt::blue;
    QString m_lineLabel = QStringLiteral("curve a");
    
    // Line flags
    bool m_segments = false;
    bool m_loop = false;
    bool m_skipNaN = false;
    bool m_clippingEnabled = true;
    bool m_shaded = false;
    
    // Downsampling
    QIM::QImDownsampleAlgorithm m_downsampleAlgorithm = QIM::QImDownsampleAlgorithm::Auto;
    int m_downsampleThreshold = 1000;
    
    QIM::QImPlotNode* m_plotNode = nullptr;
    QIM::QImPlotLineItemNode* m_lineNode = nullptr;
    QIM::QImPlotValueTrackerNode* m_trackerNode = nullptr;
};

#endif // LINE10KFUNCTION_H
