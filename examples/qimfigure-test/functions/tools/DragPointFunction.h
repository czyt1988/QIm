#ifndef DRAGPOINTFUNCTION_H
#define DRAGPOINTFUNCTION_H

#include "../TestFunction.h"
#include <QObject>
#include <QColor>
#include <QPointF>

namespace QIM {
class QImFigureWidget;
class QImPlotNode;
class QImPlotDragPointNode;
class QImPlotLineItemNode;
}

/**
 * \if ENGLISH
 * @brief Test function demonstrating draggable point tool with property registration
 * 
 * DragPointFunction creates a plot with a draggable point and a reference curve.
 * It demonstrates:
 * - Interactive drag point tool
 * - Real-time position updates via signals
 * - Customizable point color, size, and flags
 * 
 * @section usage_example Usage Example
 * 
 * \code
 * auto func = new DragPointFunction(this);
 * func->createPlot(figureWidget);
 * // Drag the point to see position changes
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotDragPointNode
 * \endif
 * 
 * \if CHINESE
 * @brief 演示可拖拽点工具的测试函数，带有属性注册
 * 
 * DragPointFunction 创建一个包含可拖拽点和参考曲线的图表。
 * 它演示了：
 * - 交互式可拖拽点工具
 * - 通过信号实时更新位置
 * - 可自定义的点颜色、大小和标志
 * 
 * @section usage_example 使用示例
 * 
 * \code
 * auto func = new DragPointFunction(this);
 * func->createPlot(figureWidget);
 * // 拖拽点查看位置变化
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotDragPointNode
 * \endif
 */
class DragPointFunction : public TestFunction {
    Q_OBJECT
    
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QPointF pointPosition READ pointPosition WRITE setPointPosition NOTIFY pointPositionChanged)
    Q_PROPERTY(QColor pointColor READ pointColor WRITE setPointColor NOTIFY pointColorChanged)
    Q_PROPERTY(float pointSize READ pointSize WRITE setPointSize NOTIFY pointSizeChanged)
    
public:
    /**
     * \if ENGLISH
     * @brief Constructor for DragPointFunction
     * @param parent Parent object
     * \endif
     * 
     * \if CHINESE
     * @brief DragPointFunction 构造函数
     * @param parent 父对象
     * \endif
     */
    explicit DragPointFunction(QObject* parent = nullptr);
    
    /**
     * \if ENGLISH
     * @brief Destructor for DragPointFunction
     * \endif
     * 
     * \if CHINESE
     * @brief DragPointFunction 析构函数
     * \endif
     */
    ~DragPointFunction() override;
    
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
    
    // Point position property accessors
    QPointF pointPosition() const { return m_pointPosition; }
    void setPointPosition(const QPointF& pos);
    
    // Point color property accessors
    QColor pointColor() const { return m_pointColor; }
    void setPointColor(const QColor& color);
    
    // Point size property accessors
    float pointSize() const { return m_pointSize; }
    void setPointSize(float size);
    
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
     * @brief Signal emitted when point position changes
     * @param pos New position value
     * \endif
     * 
     * \if CHINESE
     * @brief 点位置改变时发出的信号
     * @param pos 新的位置值
     * \endif
     */
    void pointPositionChanged(const QPointF& pos);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when point color changes
     * @param color New color value
     * \endif
     * 
     * \if CHINESE
     * @brief 点颜色改变时发出的信号
     * @param color 新的颜色值
     * \endif
     */
    void pointColorChanged(const QColor& color);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when point size changes
     * @param size New size value
     * \endif
     * 
     * \if CHINESE
     * @brief 点大小改变时发出的信号
     * @param size 新的大小值
     * \endif
     */
    void pointSizeChanged(float size);
    
private:
    QString m_title = QStringLiteral("Drag Point Tool");
    QPointF m_pointPosition = QPointF(5.0, 5.0);
    QColor m_pointColor = QColor(255, 100, 100);
    float m_pointSize = 8.0f;
    
    QIM::QImPlotNode* m_plotNode = nullptr;
    QIM::QImPlotLineItemNode* m_lineNode = nullptr;
    QIM::QImPlotDragPointNode* m_dragPointNode = nullptr;
};

#endif // DRAGPOINTFUNCTION_H