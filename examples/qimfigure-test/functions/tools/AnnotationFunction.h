#ifndef ANNOTATIONFUNCTION_H
#define ANNOTATIONFUNCTION_H

#include "../TestFunction.h"
#include <QObject>
#include <QColor>
#include <QPointF>
#include <QString>

namespace QIM {
class QImFigureWidget;
class QImPlotNode;
class QImPlotAnnotationNode;
class QImPlotDragPointNode;
class QImPlotLineItemNode;
}

/**
 * \if ENGLISH
 * @brief Test function demonstrating annotation tool with drag point linkage
 * 
 * AnnotationFunction creates a plot with an annotation linked to a draggable point.
 * It demonstrates:
 * - Annotation text labels at plot coordinates
 * - Drag point position tracking via signals
 * - Dynamic annotation position update
 * - Pixel offset and clamp options
 * 
 * @section usage_example Usage Example
 * 
 * \code
 * auto func = new AnnotationFunction(this);
 * func->createPlot(figureWidget);
 * // Drag the point to see annotation follow
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotAnnotationNode
 * @see QImPlotDragPointNode
 * \endif
 * 
 * \if CHINESE
 * @brief 演示注释工具与拖拽点联动的测试函数
 * 
 * AnnotationFunction 创建一个注释与可拖拽点联动的图表。
 * 它演示了：
 * - 在绘图坐标处显示注释文本标签
 * - 通过信号追踪拖拽点位置
 * - 动态更新注释位置
 * - 像素偏移和钳位选项
 * 
 * @section usage_example 使用示例
 * 
 * \code
 * auto func = new AnnotationFunction(this);
 * func->createPlot(figureWidget);
 * // 拖拽点查看注释跟随移动
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotAnnotationNode
 * @see QImPlotDragPointNode
 * \endif
 */
class AnnotationFunction : public TestFunction {
    Q_OBJECT
    
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString annotationText READ annotationText WRITE setAnnotationText NOTIFY annotationTextChanged)
    Q_PROPERTY(QColor annotationColor READ annotationColor WRITE setAnnotationColor NOTIFY annotationColorChanged)
    Q_PROPERTY(bool clamp READ clamp WRITE setClamp NOTIFY clampChanged)
    Q_PROPERTY(double annotationX READ annotationX WRITE setAnnotationX NOTIFY annotationPositionChanged)
    Q_PROPERTY(double annotationY READ annotationY WRITE setAnnotationY NOTIFY annotationPositionChanged)
    Q_PROPERTY(double pixelOffsetX READ pixelOffsetX WRITE setPixelOffsetX NOTIFY annotationPixelOffsetChanged)
    Q_PROPERTY(double pixelOffsetY READ pixelOffsetY WRITE setPixelOffsetY NOTIFY annotationPixelOffsetChanged)
    Q_PROPERTY(bool annotationRound READ isAnnotationRound WRITE setAnnotationRound NOTIFY annotationRoundChanged)
    
public:
    /**
     * \if ENGLISH
     * @brief Constructor for AnnotationFunction
     * @param parent Parent object
     * \endif
     * 
     * \if CHINESE
     * @brief AnnotationFunction 构造函数
     * @param parent 父对象
     * \endif
     */
    explicit AnnotationFunction(QObject* parent = nullptr);
    
    /**
     * \if ENGLISH
     * @brief Destructor for AnnotationFunction
     * \endif
     * 
     * \if CHINESE
     * @brief AnnotationFunction 析构函数
     * \endif
     */
    ~AnnotationFunction() override;
    
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
    
    // Annotation text property accessors
    QString annotationText() const { return m_annotationText; }
    void setAnnotationText(const QString& text);
    
    // Annotation color property accessors
    QColor annotationColor() const { return m_annotationColor; }
    void setAnnotationColor(const QColor& color);
    
    // Clamp property accessors
    bool clamp() const { return m_clamp; }
    void setClamp(bool enabled);
    
    // Annotation position property accessors (split from QPointF position)
    double annotationX() const { return m_annotationX; }
    void setAnnotationX(double x);
    double annotationY() const { return m_annotationY; }
    void setAnnotationY(double y);
    
    // Annotation pixel offset property accessors (split from QPointF pixelOffset)
    double pixelOffsetX() const { return m_pixelOffsetX; }
    void setPixelOffsetX(double x);
    double pixelOffsetY() const { return m_pixelOffsetY; }
    void setPixelOffsetY(double y);
    
    // Annotation round property accessors
    bool isAnnotationRound() const { return m_annotationRound; }
    void setAnnotationRound(bool enabled);
    
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
     * @brief Signal emitted when annotation text changes
     * @param text New text value
     * \endif
     * 
     * \if CHINESE
     * @brief 注释文本改变时发出的信号
     * @param text 新的文本值
     * \endif
     */
    void annotationTextChanged(const QString& text);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when annotation color changes
     * @param color New color value
     * \endif
     * 
     * \if CHINESE
     * @brief 注释颜色改变时发出的信号
     * @param color 新的颜色值
     * \endif
     */
    void annotationColorChanged(const QColor& color);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when clamp setting changes
     * @param enabled New clamp setting
     * \endif
     * 
     * \if CHINESE
     * @brief 钳位设置改变时发出的信号
     * @param enabled 新的钳位设置
     * \endif
     */
    void clampChanged(bool enabled);
    
    void annotationPositionChanged();
    void annotationPixelOffsetChanged();
    void annotationRoundChanged(bool enabled);
    
private Q_SLOTS:
    /**
     * \if ENGLISH
     * @brief Slot to update annotation position when drag point moves
     * @param pos New drag point position
     * \endif
     * 
     * \if CHINESE
     * @brief 当拖拽点移动时更新注释位置的槽函数
     * @param pos 新的拖拽点位置
     * \endif
     */
    void onDragPointMoved(const QPointF& pos);
    
private:
    QString m_title = QStringLiteral("Annotation Tool");
    QString m_annotationText = QStringLiteral("Point Label");
    QColor m_annotationColor = QColor(255, 200, 100);
    bool m_clamp = false;
    QPointF m_pointPosition = QPointF(5.0, 5.0);
    double m_annotationX = 0.5;
    double m_annotationY = 0.5;
    double m_pixelOffsetX = 0.0;
    double m_pixelOffsetY = 0.0;
    bool m_annotationRound = false;
    
    QIM::QImPlotNode* m_plotNode = nullptr;
    QIM::QImPlotLineItemNode* m_lineNode = nullptr;
    QIM::QImPlotDragPointNode* m_dragPointNode = nullptr;
    QIM::QImPlotAnnotationNode* m_annotationNode = nullptr;
};

#endif // ANNOTATIONFUNCTION_H