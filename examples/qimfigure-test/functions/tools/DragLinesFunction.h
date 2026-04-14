#ifndef DRAGLINESFUNCTION_H
#define DRAGLINESFUNCTION_H

#include "../TestFunction.h"
#include <QObject>
#include <QColor>

namespace QIM {
class QImFigureWidget;
class QImPlotNode;
class QImPlotDragLineXNode;
class QImPlotDragLineYNode;
class QImPlotLineItemNode;
}

/**
 * \if ENGLISH
 * @brief Test function demonstrating draggable line tools (DragLineX and DragLineY)
 * 
 * DragLinesFunction creates a plot with both horizontal and vertical draggable lines.
 * It demonstrates:
 * - Interactive threshold lines (DragLineX and DragLineY)
 * - Crosshair-style intersection display
 * - Real-time value updates via signals
 * - Customizable line color and thickness
 * 
 * @section usage_example Usage Example
 * 
 * \code
 * auto func = new DragLinesFunction(this);
 * func->createPlot(figureWidget);
 * // Drag the lines to see intersection point changes
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotDragLineXNode
 * @see QImPlotDragLineYNode
 * \endif
 * 
 * \if CHINESE
 * @brief 演示可拖拽线工具 (DragLineX 和 DragLineY) 的测试函数
 * 
 * DragLinesFunction 创建一个包含水平和垂直可拖拽线的图表。
 * 它演示了：
 * - 交互式阈值线 (DragLineX 和 DragLineY)
 * - 十字线式交叉点显示
 * - 通过信号实时更新值
 * - 可自定义的线颜色和粗细
 * 
 * @section usage_example 使用示例
 * 
 * \code
 * auto func = new DragLinesFunction(this);
 * func->createPlot(figureWidget);
 * // 拖拽线条查看交叉点变化
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotDragLineXNode
 * @see QImPlotDragLineYNode
 * \endif
 */
class DragLinesFunction : public TestFunction {
    Q_OBJECT
    
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(double lineXValue READ lineXValue WRITE setLineXValue NOTIFY lineXValueChanged)
    Q_PROPERTY(double lineYValue READ lineYValue WRITE setLineYValue NOTIFY lineYValueChanged)
    Q_PROPERTY(QColor lineXColor READ lineXColor WRITE setLineXColor NOTIFY lineXColorChanged)
    Q_PROPERTY(QColor lineYColor READ lineYColor WRITE setLineYColor NOTIFY lineYColorChanged)
    Q_PROPERTY(float lineThickness READ lineThickness WRITE setLineThickness NOTIFY lineThicknessChanged)
    
public:
    /**
     * \if ENGLISH
     * @brief Constructor for DragLinesFunction
     * @param parent Parent object
     * \endif
     * 
     * \if CHINESE
     * @brief DragLinesFunction 构造函数
     * @param parent 父对象
     * \endif
     */
    explicit DragLinesFunction(QObject* parent = nullptr);
    
    /**
     * \if ENGLISH
     * @brief Destructor for DragLinesFunction
     * \endif
     * 
     * \if CHINESE
     * @brief DragLinesFunction 析构函数
     * \endif
     */
    ~DragLinesFunction() override;
    
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
    
    // DragLineX value property accessors
    double lineXValue() const { return m_lineXValue; }
    void setLineXValue(double value);
    
    // DragLineY value property accessors
    double lineYValue() const { return m_lineYValue; }
    void setLineYValue(double value);
    
    // DragLineX color property accessors
    QColor lineXColor() const { return m_lineXColor; }
    void setLineXColor(const QColor& color);
    
    // DragLineY color property accessors
    QColor lineYColor() const { return m_lineYColor; }
    void setLineYColor(const QColor& color);
    
    // Line thickness property accessors
    float lineThickness() const { return m_lineThickness; }
    void setLineThickness(float thickness);
    
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
     * @brief Signal emitted when DragLineX value changes
     * @param value New X value
     * \endif
     * 
     * \if CHINESE
     * @brief DragLineX 值改变时发出的信号
     * @param value 新的 X 值
     * \endif
     */
    void lineXValueChanged(double value);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when DragLineY value changes
     * @param value New Y value
     * \endif
     * 
     * \if CHINESE
     * @brief DragLineY 值改变时发出的信号
     * @param value 新的 Y 值
     * \endif
     */
    void lineYValueChanged(double value);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when DragLineX color changes
     * @param color New color value
     * \endif
     * 
     * \if CHINESE
     * @brief DragLineX 颜色改变时发出的信号
     * @param color 新的颜色值
     * \endif
     */
    void lineXColorChanged(const QColor& color);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when DragLineY color changes
     * @param color New color value
     * \endif
     * 
     * \if CHINESE
     * @brief DragLineY 颜色改变时发出的信号
     * @param color 新的颜色值
     * \endif
     */
    void lineYColorChanged(const QColor& color);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when line thickness changes
     * @param thickness New thickness value
     * \endif
     * 
     * \if CHINESE
     * @brief 线粗细改变时发出的信号
     * @param thickness 新的粗细值
     * \endif
     */
    void lineThicknessChanged(float thickness);
    
private:
    QString m_title = QStringLiteral("Drag Lines Tool");
    double m_lineXValue = 5.0;
    double m_lineYValue = 5.0;
    QColor m_lineXColor = QColor(255, 100, 100);
    QColor m_lineYColor = QColor(100, 255, 100);
    float m_lineThickness = 2.0f;
    
    QIM::QImPlotNode* m_plotNode = nullptr;
    QIM::QImPlotLineItemNode* m_lineNode = nullptr;
    QIM::QImPlotDragLineXNode* m_dragLineXNode = nullptr;
    QIM::QImPlotDragLineYNode* m_dragLineYNode = nullptr;
};

#endif // DRAGLINESFUNCTION_H