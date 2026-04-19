#ifndef PLOT3DDUMMYFUNCTION_H
#define PLOT3DDUMMYFUNCTION_H

#include "../TestFunction.h"
#include <QObject>
#include <QColor>
#include <QPointer>

namespace QIM {
class QImFigureWidget;
class QImPlot3DNode;
class QImPlot3DDummyItemNode;
class QImPlot3DLineItemNode;
}

/**
 * \if ENGLISH
 * @brief Test function demonstrating 3D dummy items with property registration
 * 
 * Plot3DDummyFunction creates a 3D plot with 3 dummy items of different colors
 * in the legend to test 3D legend customization capabilities. It registers 
 * properties for:
 * - Plot title
 * - X, Y and Z axis labels
 * - Dummy item colors (3 items with labels "Sensor A", "Sensor B", "Sensor C")
 * 
 * @note Dummy items are legend-only: they appear only in the legend to demonstrate
 * legend customization, but do not render any geometry in the 3D plot area.
 * 
 * @section usage_example Usage Example
 * 
 * \code
 * auto func = new Plot3DDummyFunction(this);
 * func->createPlot(figureWidget);
 * // Properties will be available via getRegisteredProperties()
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlot3DNode
 * @see QImPlot3DDummyItemNode
 * \endif
 * 
 * \if CHINESE
 * @brief 演示 3D 虚拟项的测试函数，带有属性注册
 * 
 * Plot3DDummyFunction 创建一个包含 3 个不同颜色虚拟项的 3D 图表，
 * 用于测试 3D 图例自定义能力。它注册了以下属性：
 * - 图表标题
 * - X、Y 和 Z 轴标签
 * - 虚拟项颜色（3 个项，标签分别为"Sensor A"、"Sensor B"、"Sensor C"）
 * 
 * @note 虚拟项仅显示在图例中：它们只出现在图例中用于演示图例自定义，
 * 但不会在 3D 绘图区域渲染任何几何图形。
 * 
 * @section usage_example 使用示例
 * 
 * \code
 * auto func = new Plot3DDummyFunction(this);
 * func->createPlot(figureWidget);
 * // 属性可通过 getRegisteredProperties() 获取
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlot3DNode
 * @see QImPlot3DDummyItemNode
 * \endif
 */
class Plot3DDummyFunction : public TestFunction {
    Q_OBJECT
    
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString xLabel READ xLabel WRITE setXLabel NOTIFY xLabelChanged)
    Q_PROPERTY(QString yLabel READ yLabel WRITE setYLabel NOTIFY yLabelChanged)
    Q_PROPERTY(QString zLabel READ zLabel WRITE setZLabel NOTIFY zLabelChanged)
    Q_PROPERTY(QColor dummy1Color READ dummy1Color WRITE setDummy1Color NOTIFY dummy1ColorChanged)
    Q_PROPERTY(QColor dummy2Color READ dummy2Color WRITE setDummy2Color NOTIFY dummy2ColorChanged)
    Q_PROPERTY(QColor dummy3Color READ dummy3Color WRITE setDummy3Color NOTIFY dummy3ColorChanged)
    
public:
    /**
     * \if ENGLISH
     * @brief Constructor for Plot3DDummyFunction
     * @param parent Parent object
     * \endif
     * 
     * \if CHINESE
     * @brief Plot3DDummyFunction 构造函数
     * @param parent 父对象
     * \endif
     */
    explicit Plot3DDummyFunction(QObject* parent = nullptr);
    
    /**
     * \if ENGLISH
     * @brief Destructor for Plot3DDummyFunction
     * \endif
     * 
     * \if CHINESE
     * @brief Plot3DDummyFunction 析构函数
     * \endif
     */
    ~Plot3DDummyFunction() override;
    
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
    
    // Dummy 1 color property accessors
    QColor dummy1Color() const { return m_dummy1Color; }
    void setDummy1Color(const QColor& color);
    
    // Dummy 2 color property accessors
    QColor dummy2Color() const { return m_dummy2Color; }
    void setDummy2Color(const QColor& color);
    
    // Dummy 3 color property accessors
    QColor dummy3Color() const { return m_dummy3Color; }
    void setDummy3Color(const QColor& color);
    
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
     * @brief Signal emitted when dummy 1 color changes
     * @param color New dummy 1 color value
     * \endif
     * 
     * \if CHINESE
     * @brief 虚拟项 1 颜色改变时发出的信号
     * @param color 新的虚拟项 1 颜色值
     * \endif
     */
    void dummy1ColorChanged(const QColor& color);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when dummy 2 color changes
     * @param color New dummy 2 color value
     * \endif
     * 
     * \if CHINESE
     * @brief 虚拟项 2 颜色改变时发出的信号
     * @param color 新的虚拟项 2 颜色值
     * \endif
     */
    void dummy2ColorChanged(const QColor& color);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when dummy 3 color changes
     * @param color New dummy 3 color value
     * \endif
     * 
     * \if CHINESE
     * @brief 虚拟项 3 颜色改变时发出的信号
     * @param color 新的虚拟项 3 颜色值
     * \endif
     */
    void dummy3ColorChanged(const QColor& color);
    
private:
    QString m_title = QStringLiteral("3D Dummy Items");
    QString m_xLabel = QStringLiteral("X");
    QString m_yLabel = QStringLiteral("Y");
    QString m_zLabel = QStringLiteral("Z");
    QColor m_dummy1Color = Qt::red;
    QColor m_dummy2Color = Qt::green;
    QColor m_dummy3Color = Qt::blue;
    
    QPointer<QIM::QImPlot3DNode> m_plot3DNode = nullptr;
    QPointer<QIM::QImPlot3DDummyItemNode> m_dummy1Node = nullptr;
    QPointer<QIM::QImPlot3DDummyItemNode> m_dummy2Node = nullptr;
    QPointer<QIM::QImPlot3DDummyItemNode> m_dummy3Node = nullptr;
    QPointer<QIM::QImPlot3DLineItemNode> m_lineNode = nullptr;
};

#endif // PLOT3DDUMMYFUNCTION_H
