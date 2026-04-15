#ifndef DRAGRECTFUNCTION_H
#define DRAGRECTFUNCTION_H

#include "../TestFunction.h"
#include <QObject>
#include <QColor>
#include <QRectF>

namespace QIM {
class QImFigureWidget;
class QImPlotNode;
class QImPlotDragRectNode;
class QImPlotLineItemNode;
}

/**
 * \if ENGLISH
 * @brief Test function demonstrating draggable rectangle tool
 * 
 * DragRectFunction creates a plot with a draggable rectangle selection area.
 * It demonstrates:
 * - Interactive rectangle drag and resize
 * - Real-time rect coordinates updates via signals
 * - Customizable rectangle border color
 * 
 * @section usage_example Usage Example
 * 
 * \code
 * auto func = new DragRectFunction(this);
 * func->createPlot(figureWidget);
 * // Drag the rectangle corners to resize, drag center to move
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotDragRectNode
 * \endif
 * 
 * \if CHINESE
 * @brief 演示可拖拽矩形工具的测试函数
 * 
 * DragRectFunction 创建一个包含可拖拽矩形选择区域的图表。
 * 它演示了：
 * - 交互式矩形拖拽和调整大小
 * - 通过信号实时更新矩形坐标
 * - 可自定义的矩形边框颜色
 * 
 * @section usage_example 使用示例
 * 
 * \code
 * auto func = new DragRectFunction(this);
 * func->createPlot(figureWidget);
 * // 拖拽矩形角点调整大小，拖拽中心移动
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotDragRectNode
 * \endif
 */
class DragRectFunction : public TestFunction {
    Q_OBJECT
    
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QRectF rect READ rect WRITE setRect NOTIFY rectChanged)
    Q_PROPERTY(QColor rectColor READ rectColor WRITE setRectColor NOTIFY rectColorChanged)
    Q_PROPERTY(bool cursorsEnabled READ isCursorsEnabled WRITE setCursorsEnabled NOTIFY cursorsEnabledChanged)
    Q_PROPERTY(bool fitEnabled READ isFitEnabled WRITE setFitEnabled NOTIFY fitEnabledChanged)
    Q_PROPERTY(bool inputsEnabled READ isInputsEnabled WRITE setInputsEnabled NOTIFY inputsEnabledChanged)
    Q_PROPERTY(bool delayed READ isDelayed WRITE setDelayed NOTIFY delayedChanged)
    
public:
    /**
     * \if ENGLISH
     * @brief Constructor for DragRectFunction
     * @param parent Parent object
     * \endif
     * 
     * \if CHINESE
     * @brief DragRectFunction 构造函数
     * @param parent 父对象
     * \endif
     */
    explicit DragRectFunction(QObject* parent = nullptr);
    
    /**
     * \if ENGLISH
     * @brief Destructor for DragRectFunction
     * \endif
     * 
     * \if CHINESE
     * @brief DragRectFunction 析构函数
     * \endif
     */
    ~DragRectFunction() override;
    
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
    
    // Rect property accessors
    QRectF rect() const { return m_rect; }
    void setRect(const QRectF& rect);
    
    // Rect color property accessors
    QColor rectColor() const { return m_rectColor; }
    void setRectColor(const QColor& color);
    
    // Cursors enabled property accessors
    bool isCursorsEnabled() const { return m_cursorsEnabled; }
    void setCursorsEnabled(bool enabled);
    
    // Fit enabled property accessors
    bool isFitEnabled() const { return m_fitEnabled; }
    void setFitEnabled(bool enabled);
    
    // Inputs enabled property accessors
    bool isInputsEnabled() const { return m_inputsEnabled; }
    void setInputsEnabled(bool enabled);
    
    // Delayed property accessors
    bool isDelayed() const { return m_delayed; }
    void setDelayed(bool on);
    
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
     * @brief Signal emitted when rect coordinates change
     * @param rect New rect coordinates
     * \endif
     * 
     * \if CHINESE
     * @brief 矩形坐标改变时发出的信号
     * @param rect 新的矩形坐标
     * \endif
     */
    void rectChanged(const QRectF& rect);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when rect color changes
     * @param color New color value
     * \endif
     * 
     * \if CHINESE
     * @brief 矩形颜色改变时发出的信号
     * @param color 新的颜色值
     * \endif
     */
    void rectColorChanged(const QColor& color);
    void cursorsEnabledChanged(bool enabled);
    void fitEnabledChanged(bool enabled);
    void inputsEnabledChanged(bool enabled);
    void delayedChanged(bool on);
    
private:
    QString m_title = QStringLiteral("Drag Rect Tool");
    QRectF m_rect = QRectF(2.0, 2.0, 6.0, 6.0);
    QColor m_rectColor = QColor(255, 200, 100);
    bool m_cursorsEnabled = true;
    bool m_fitEnabled = true;
    bool m_inputsEnabled = true;
    bool m_delayed = false;
    
    QIM::QImPlotNode* m_plotNode = nullptr;
    QIM::QImPlotLineItemNode* m_lineNode = nullptr;
    QIM::QImPlotDragRectNode* m_dragRectNode = nullptr;
};

#endif // DRAGRECTFUNCTION_H