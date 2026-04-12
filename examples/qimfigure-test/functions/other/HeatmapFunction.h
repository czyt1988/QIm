#ifndef HEATMAPFUNCTION_H
#define HEATMAPFUNCTION_H

#include "../TestFunction.h"
#include <QObject>
#include <QColor>
#include <QPointF>

namespace QIM {
class QImFigureWidget;
class QImPlotNode;
class QImPlotHeatmapItemNode;
class QImPlotValueTrackerNode;
}

/**
 * \if ENGLISH
 * @brief Test function demonstrating heatmap with property registration
 * 
 * HeatmapFunction creates a 10x10 heatmap with sinusoidal pattern to test heatmap rendering
 * capabilities. It registers properties for:
 * - Plot title
 * - X and Y axis labels
 * - Scale minimum and maximum
 * - Label format
 * - Bounds (lower-left and upper-right corners)
 * - Column-major data layout flag
 * 
 * @section usage_example Usage Example
 * 
 * \code
 * auto func = new HeatmapFunction(this);
 * func->createPlot(figureWidget);
 * // Properties will be available via getRegisteredProperties()
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotNode
 * @see QImPlotHeatmapItemNode
 * \endif
 * 
 * \if CHINESE
 * @brief 演示热力图的测试函数，带有属性注册
 * 
 * HeatmapFunction 创建一个包含 10x10 正弦图案的热力图，用于测试热力图渲染能力。
 * 它注册了以下属性：
 * - 图表标题
 * - X 和 Y 轴标签
 * - 缩放最小值和最大值
 * - 标签格式
 * - 边界（左下角和右上角）
 * - 列主序数据布局标志
 * 
 * @section usage_example 使用示例
 * 
 * \code
 * auto func = new HeatmapFunction(this);
 * func->createPlot(figureWidget);
 * // 属性可通过 getRegisteredProperties() 获取
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotNode
 * @see QImPlotHeatmapItemNode
 * \endif
 */
class HeatmapFunction : public TestFunction {
    Q_OBJECT
    
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString xLabel READ xLabel WRITE setXLabel NOTIFY xLabelChanged)
    Q_PROPERTY(QString yLabel READ yLabel WRITE setYLabel NOTIFY yLabelChanged)
    Q_PROPERTY(double scaleMin READ scaleMin WRITE setScaleMin NOTIFY scaleMinChanged)
    Q_PROPERTY(double scaleMax READ scaleMax WRITE setScaleMax NOTIFY scaleMaxChanged)
    Q_PROPERTY(QString labelFormat READ labelFormat WRITE setLabelFormat NOTIFY labelFormatChanged)
    Q_PROPERTY(QPointF boundsMin READ boundsMin WRITE setBoundsMin NOTIFY boundsMinChanged)
    Q_PROPERTY(QPointF boundsMax READ boundsMax WRITE setBoundsMax NOTIFY boundsMaxChanged)
    Q_PROPERTY(double boundsMinX READ boundsMinX WRITE setBoundsMinX NOTIFY boundsMinXChanged)
    Q_PROPERTY(double boundsMinY READ boundsMinY WRITE setBoundsMinY NOTIFY boundsMinYChanged)
    Q_PROPERTY(double boundsMaxX READ boundsMaxX WRITE setBoundsMaxX NOTIFY boundsMaxXChanged)
    Q_PROPERTY(double boundsMaxY READ boundsMaxY WRITE setBoundsMaxY NOTIFY boundsMaxYChanged)
    Q_PROPERTY(bool colMajor READ isColMajor WRITE setColMajor NOTIFY colMajorChanged)
    
public:
    /**
     * \if ENGLISH
     * @brief Constructor for HeatmapFunction
     * @param parent Parent object
     * \endif
     * 
     * \if CHINESE
     * @brief HeatmapFunction 构造函数
     * @param parent 父对象
     * \endif
     */
    explicit HeatmapFunction(QObject* parent = nullptr);
    
    /**
     * \if ENGLISH
     * @brief Destructor for HeatmapFunction
     * \endif
     * 
     * \if CHINESE
     * @brief HeatmapFunction 析构函数
     * \endif
     */
    ~HeatmapFunction() override;
    
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
    
    // Scale minimum property accessors
    double scaleMin() const { return m_scaleMin; }
    void setScaleMin(double min);
    
    // Scale maximum property accessors
    double scaleMax() const { return m_scaleMax; }
    void setScaleMax(double max);
    
    // Label format property accessors
    QString labelFormat() const { return m_labelFormat; }
    void setLabelFormat(const QString& format);
    
    // Bounds minimum property accessors
    QPointF boundsMin() const { return m_boundsMin; }
    void setBoundsMin(const QPointF& min);
    
    // Bounds minimum X coordinate property accessors
    double boundsMinX() const { return m_boundsMin.x(); }
    void setBoundsMinX(double x);
    
    // Bounds minimum Y coordinate property accessors
    double boundsMinY() const { return m_boundsMin.y(); }
    void setBoundsMinY(double y);
    
    // Bounds maximum property accessors
    QPointF boundsMax() const { return m_boundsMax; }
    void setBoundsMax(const QPointF& max);
    
    // Bounds maximum X coordinate property accessors
    double boundsMaxX() const { return m_boundsMax.x(); }
    void setBoundsMaxX(double x);
    
    // Bounds maximum Y coordinate property accessors
    double boundsMaxY() const { return m_boundsMax.y(); }
    void setBoundsMaxY(double y);
    
    // Column-major flag property accessors
    bool isColMajor() const { return m_colMajor; }
    void setColMajor(bool colMajor);
    
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
     * @brief Signal emitted when scale minimum changes
     * @param min New scale minimum value
     * \endif
     * 
     * \if CHINESE
     * @brief 缩放最小值改变时发出的信号
     * @param min 新的缩放最小值
     * \endif
     */
    void scaleMinChanged(double min);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when scale maximum changes
     * @param max New scale maximum value
     * \endif
     * 
     * \if CHINESE
     * @brief 缩放最大值改变时发出的信号
     * @param max 新的缩放最大值
     * \endif
     */
    void scaleMaxChanged(double max);
    
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
     * @brief Signal emitted when lower-left bounds change
     * @param min New lower-left bounds position
     * \endif
     * 
     * \if CHINESE
     * @brief 左下角边界改变时发出的信号
     * @param min 新的左下角边界位置
     * \endif
     */
    void boundsMinChanged(const QPointF& min);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when lower-left bounds X coordinate changes
     * @param x New X coordinate
     * \endif
     * 
     * \if CHINESE
     * @brief 左下角边界X坐标改变时发出的信号
     * @param x 新的X坐标
     * \endif
     */
    void boundsMinXChanged(double x);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when lower-left bounds Y coordinate changes
     * @param y New Y coordinate
     * \endif
     * 
     * \if CHINESE
     * @brief 左下角边界Y坐标改变时发出的信号
     * @param y 新的Y坐标
     * \endif
     */
    void boundsMinYChanged(double y);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when upper-right bounds change
     * @param max New upper-right bounds position
     * \endif
     * 
     * \if CHINESE
     * @brief 右上角边界改变时发出的信号
     * @param max 新的右上角边界位置
     * \endif
     */
    void boundsMaxChanged(const QPointF& max);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when upper-right bounds X coordinate changes
     * @param x New X coordinate
     * \endif
     * 
     * \if CHINESE
     * @brief 右上角边界X坐标改变时发出的信号
     * @param x 新的X坐标
     * \endif
     */
    void boundsMaxXChanged(double x);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when upper-right bounds Y coordinate changes
     * @param y New Y coordinate
     * \endif
     * 
     * \if CHINESE
     * @brief 右上角边界Y坐标改变时发出的信号
     * @param y 新的Y坐标
     * \endif
     */
    void boundsMaxYChanged(double y);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when column-major flag changes
     * @param colMajor New column-major state
     * \endif
     * 
     * \if CHINESE
     * @brief 列主序标志改变时发出的信号
     * @param colMajor 新的列主序状态
     * \endif
     */
    void colMajorChanged(bool colMajor);
    
private:
    QString m_title = QStringLiteral("Heatmap Chart");
    QString m_xLabel = QStringLiteral("x");
    QString m_yLabel = QStringLiteral("y");
    double m_scaleMin = 0.0;
    double m_scaleMax = 0.0;
    QString m_labelFormat = QStringLiteral("%.1f");
    QPointF m_boundsMin = QPointF(0.0, 0.0);
    QPointF m_boundsMax = QPointF(1.0, 1.0);
    bool m_colMajor = false;
    
    QIM::QImPlotNode* m_plotNode = nullptr;
    QIM::QImPlotHeatmapItemNode* m_heatmapNode = nullptr;
    QIM::QImPlotValueTrackerNode* m_trackerNode = nullptr;
};

#endif // HEATMAPFUNCTION_H