#ifndef PIECHARTFUNCTION_H
#define PIECHARTFUNCTION_H

#include "../TestFunction.h"
#include <QObject>
#include <QColor>
#include <QPointF>

namespace QIM {
class QImFigureWidget;
class QImPlotNode;
class QImPlotPieChartItemNode;
class QImPlotValueTrackerNode;
}

/**
 * \if ENGLISH
 * @brief Test function demonstrating pie chart with property registration
 * 
 * PieChartFunction creates a pie chart with 5 slices to test pie chart rendering
 * capabilities. It registers properties for:
 * - Plot title
 * - Center position (X, Y)
 * - Radius
 * - Label format
 * - Start angle
 * - Normalization flag
 * - Ignore hidden slices flag
 * - Exploding slices flag
 * 
 * @section usage_example Usage Example
 * 
 * \code
 * auto func = new PieChartFunction(this);
 * func->createPlot(figureWidget);
 * // Properties will be available via getRegisteredProperties()
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotNode
 * @see QImPlotPieChartItemNode
 * \endif
 * 
 * \if CHINESE
 * @brief 演示饼图的测试函数，带有属性注册
 * 
 * PieChartFunction 创建一个包含 5 个切片的饼图，用于测试饼图渲染能力。
 * 它注册了以下属性：
 * - 图表标题
 * - 中心位置 (X, Y)
 * - 半径
 * - 标签格式
 * - 起始角度
 * - 归一化标志
 * - 忽略隐藏切片标志
 * - 爆炸切片标志
 * 
 * @section usage_example 使用示例
 * 
 * \code
 * auto func = new PieChartFunction(this);
 * func->createPlot(figureWidget);
 * // 属性可通过 getRegisteredProperties() 获取
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotNode
 * @see QImPlotPieChartItemNode
 * \endif
 */
class PieChartFunction : public TestFunction {
    Q_OBJECT
    
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(double centerX READ centerX WRITE setCenterX NOTIFY centerChanged)
    Q_PROPERTY(double centerY READ centerY WRITE setCenterY NOTIFY centerChanged)
    Q_PROPERTY(double radius READ radius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(QString labelFormat READ labelFormat WRITE setLabelFormat NOTIFY labelFormatChanged)
    Q_PROPERTY(double startAngle READ startAngle WRITE setStartAngle NOTIFY startAngleChanged)
    Q_PROPERTY(bool normalize READ isNormalized WRITE setNormalize NOTIFY normalizeChanged)
    Q_PROPERTY(bool ignoreHidden READ isIgnoreHidden WRITE setIgnoreHidden NOTIFY ignoreHiddenChanged)
    Q_PROPERTY(bool exploding READ isExploding WRITE setExploding NOTIFY explodingChanged)
    
public:
    /**
     * \if ENGLISH
     * @brief Constructor for PieChartFunction
     * @param parent Parent object
     * \endif
     * 
     * \if CHINESE
     * @brief PieChartFunction 构造函数
     * @param parent 父对象
     * \endif
     */
    explicit PieChartFunction(QObject* parent = nullptr);
    
    /**
     * \if ENGLISH
     * @brief Destructor for PieChartFunction
     * \endif
     * 
     * \if CHINESE
     * @brief PieChartFunction 析构函数
     * \endif
     */
    ~PieChartFunction() override;
    
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
    
    // Center X property accessors
    double centerX() const { return m_centerX; }
    void setCenterX(double x);
    
    // Center Y property accessors
    double centerY() const { return m_centerY; }
    void setCenterY(double y);
    
    // Radius property accessors
    double radius() const { return m_radius; }
    void setRadius(double radius);
    
    // Label format property accessors
    QString labelFormat() const { return m_labelFormat; }
    void setLabelFormat(const QString& format);
    
    // Start angle property accessors
    double startAngle() const { return m_startAngle; }
    void setStartAngle(double angle);
    
    // Normalization flag property accessors
    bool isNormalized() const { return m_normalize; }
    void setNormalize(bool normalize);
    
    // Ignore hidden flag property accessors
    bool isIgnoreHidden() const { return m_ignoreHidden; }
    void setIgnoreHidden(bool ignore);
    
    // Exploding flag property accessors
    bool isExploding() const { return m_exploding; }
    void setExploding(bool exploding);
    
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
     * @brief Signal emitted when center position changes
     * @param center New center position (as QPointF)
     * \endif
     * 
     * \if CHINESE
     * @brief 中心位置改变时发出的信号
     * @param center 新的中心位置（QPointF）
     * \endif
     */
    void centerChanged(const QPointF& center);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when radius changes
     * @param radius New radius value
     * \endif
     * 
     * \if CHINESE
     * @brief 半径改变时发出的信号
     * @param radius 新的半径值
     * \endif
     */
    void radiusChanged(double radius);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when label format changes
     * @param format New label format string
     * \endif
     * 
     * \if CHINESE
     * @brief 标签格式改变时发出的信号
     * @param format 新的标签格式字符串
     * \endif
     */
    void labelFormatChanged(const QString& format);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when start angle changes
     * @param angle New start angle value
     * \endif
     * 
     * \if CHINESE
     * @brief 起始角度改变时发出的信号
     * @param angle 新的起始角度值
     * \endif
     */
    void startAngleChanged(double angle);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when normalization flag changes
     * @param normalize New normalization state
     * \endif
     * 
     * \if CHINESE
     * @brief 归一化标志改变时发出的信号
     * @param normalize 新的归一化状态
     * \endif
     */
    void normalizeChanged(bool normalize);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when ignore hidden flag changes
     * @param ignore New ignore hidden state
     * \endif
     * 
     * \if CHINESE
     * @brief 忽略隐藏标志改变时发出的信号
     * @param ignore 新的忽略隐藏状态
     * \endif
     */
    void ignoreHiddenChanged(bool ignore);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when exploding flag changes
     * @param exploding New exploding state
     * \endif
     * 
     * \if CHINESE
     * @brief 爆炸标志改变时发出的信号
     * @param exploding 新的爆炸状态
     * \endif
     */
    void explodingChanged(bool exploding);
    
private:
    QString m_title = QStringLiteral("Pie Chart");
    double m_centerX = 0.0;
    double m_centerY = 0.0;
    double m_radius = 1.0;
    QString m_labelFormat = QStringLiteral("%.1f");
    double m_startAngle = 90.0;
    bool m_normalize = false;
    bool m_ignoreHidden = false;
    bool m_exploding = false;
    
    QIM::QImPlotNode* m_plotNode = nullptr;
    QIM::QImPlotPieChartItemNode* m_pieChartNode = nullptr;
    QIM::QImPlotValueTrackerNode* m_trackerNode = nullptr;
};

#endif // PIECHARTFUNCTION_H