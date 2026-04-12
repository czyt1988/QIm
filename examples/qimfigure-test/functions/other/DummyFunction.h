#ifndef DUMMYFUNCTION_H
#define DUMMYFUNCTION_H

#include "../TestFunction.h"
#include <QObject>
#include <QColor>

namespace QIM {
class QImFigureWidget;
class QImPlotNode;
class QImPlotLineItemNode;
class QImPlotDummyItemNode;
class QImPlotValueTrackerNode;
}

/**
 * \if ENGLISH
 * @brief Test function demonstrating dummy plot items with property registration
 * 
 * DummyFunction creates a plot with dummy items that appear only in the legend
 * as colored placeholders, overlaid on a sine wave background curve. It registers 
 * properties for:
 * - Plot title
 * - X and Y axis labels
 * - Background line color
 * - Dummy item color
 * 
 * @section usage_example Usage Example
 * 
 * \code
 * auto func = new DummyFunction(this);
 * func->createPlot(figureWidget);
 * // Properties will be available via getRegisteredProperties()
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotNode
 * @see QImPlotDummyItemNode
 * \endif
 * 
 * \if CHINESE
 * @brief 演示虚拟图例项的测试函数，带有属性注册
 * 
 * DummyFunction 创建一个在正弦波背景曲线上叠加虚拟图例项的图表。
 * 它注册了以下属性：
 * - 图表标题
 * - X 和 Y 轴标签
 * - 背景线颜色
 * - 虚拟项颜色
 * 
 * @section usage_example 使用示例
 * 
 * \code
 * auto func = new DummyFunction(this);
 * func->createPlot(figureWidget);
 * // 属性可通过 getRegisteredProperties() 获取
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotNode
 * @see QImPlotDummyItemNode
 * \endif
 */
class DummyFunction : public TestFunction {
    Q_OBJECT
    
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString xLabel READ xLabel WRITE setXLabel NOTIFY xLabelChanged)
    Q_PROPERTY(QString yLabel READ yLabel WRITE setYLabel NOTIFY yLabelChanged)
    Q_PROPERTY(QColor lineColor READ lineColor WRITE setLineColor NOTIFY lineColorChanged)
    Q_PROPERTY(QColor dummyColor READ dummyColor WRITE setDummyColor NOTIFY dummyColorChanged)
    
public:
    /**
     * \if ENGLISH
     * @brief Constructor for DummyFunction
     * @param parent Parent object
     * \endif
     * 
     * \if CHINESE
     * @brief DummyFunction 构造函数
     * @param parent 父对象
     * \endif
     */
    explicit DummyFunction(QObject* parent = nullptr);
    
    /**
     * \if ENGLISH
     * @brief Destructor for DummyFunction
     * \endif
     * 
     * \if CHINESE
     * @brief DummyFunction 析构函数
     * \endif
     */
    ~DummyFunction() override;
    
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
    
    // Background line color property accessors
    QColor lineColor() const { return m_lineColor; }
    void setLineColor(const QColor& color);
    
    // Dummy item color property accessors
    QColor dummyColor() const { return m_dummyColor; }
    void setDummyColor(const QColor& color);
    
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
     * @brief Signal emitted when background line color changes
     * @param color New line color value
     * \endif
     * 
     * \if CHINESE
     * @brief 背景线颜色改变时发出的信号
     * @param color 新的线颜色值
     * \endif
     */
    void lineColorChanged(const QColor& color);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when dummy item color changes
     * @param color New dummy item color value
     * \endif
     * 
     * \if CHINESE
     * @brief 虚拟项颜色改变时发出的信号
     * @param color 新的虚拟项颜色值
     * \endif
     */
    void dummyColorChanged(const QColor& color);
    
private:
    QString m_title = QStringLiteral("Dummy Item Plot");
    QString m_xLabel = QStringLiteral("x");
    QString m_yLabel = QStringLiteral("y");
    QColor m_lineColor = Qt::blue;
    QColor m_dummyColor = Qt::red;
    
    QIM::QImPlotNode* m_plotNode = nullptr;
    QIM::QImPlotLineItemNode* m_lineNode = nullptr;
    QIM::QImPlotDummyItemNode* m_dummyNode = nullptr;
    QIM::QImPlotValueTrackerNode* m_trackerNode = nullptr;
};

#endif // DUMMYFUNCTION_H
