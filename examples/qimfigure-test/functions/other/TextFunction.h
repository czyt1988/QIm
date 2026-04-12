#ifndef TEXTFUNCTION_H
#define TEXTFUNCTION_H

#include "../TestFunction.h"
#include <QObject>
#include <QColor>
#include <QPointF>

namespace QIM {
class QImFigureWidget;
class QImPlotNode;
class QImPlotTextItemNode;
}

/**
 * \if ENGLISH
 * @brief Test function demonstrating text label plot with property registration
 * 
 * TextFunction creates a plot with text labels at specified positions.
 * It registers properties for:
 * - Plot title
 * - X and Y axis labels
 * - Text content
 * - Text position (X, Y)
 * - Pixel offset (X, Y)
 * - Text color
 * - Vertical orientation
 * 
 * @section usage_example Usage Example
 * 
 * \code
 * auto func = new TextFunction(this);
 * func->createPlot(figureWidget);
 * // Properties will be available via getRegisteredProperties()
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotNode
 * @see QImPlotTextItemNode
 * \endif
 * 
 * \if CHINESE
 * @brief 演示文本标签图的测试函数，带有属性注册
 * 
 * TextFunction 创建在指定位置带有文本标签的图表。
 * 它注册了以下属性：
 * - 图表标题
 * - X 和 Y 轴标签
 * - 文本内容
 * - 文本位置 (X, Y)
 * - 像素偏移 (X, Y)
 * - 文本颜色
 * - 垂直方向
 * 
 * @section usage_example 使用示例
 * 
 * \code
 * auto func = new TextFunction(this);
 * func->createPlot(figureWidget);
 * // 属性可通过 getRegisteredProperties() 获取
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotNode
 * @see QImPlotTextItemNode
 * \endif
 */
class TextFunction : public TestFunction {
    Q_OBJECT
    
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString xLabel READ xLabel WRITE setXLabel NOTIFY xLabelChanged)
    Q_PROPERTY(QString yLabel READ yLabel WRITE setYLabel NOTIFY yLabelChanged)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(double textX READ textX WRITE setTextX NOTIFY textXChanged)
    Q_PROPERTY(double textY READ textY WRITE setTextY NOTIFY textYChanged)
    Q_PROPERTY(float pixelOffsetX READ pixelOffsetX WRITE setPixelOffsetX NOTIFY pixelOffsetXChanged)
    Q_PROPERTY(float pixelOffsetY READ pixelOffsetY WRITE setPixelOffsetY NOTIFY pixelOffsetYChanged)
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor NOTIFY textColorChanged)
    Q_PROPERTY(bool vertical READ isVertical WRITE setVertical NOTIFY verticalChanged)
    
public:
    /**
     * \if ENGLISH
     * @brief Constructor for TextFunction
     * @param parent Parent object
     * \endif
     * 
     * \if CHINESE
     * @brief TextFunction 构造函数
     * @param parent 父对象
     * \endif
     */
    explicit TextFunction(QObject* parent = nullptr);
    
    /**
     * \if ENGLISH
     * @brief Destructor for TextFunction
     * \endif
     * 
     * \if CHINESE
     * @brief TextFunction 析构函数
     * \endif
     */
    ~TextFunction() override;
    
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
    
    // Text content property accessors
    QString text() const { return m_text; }
    void setText(const QString& text);
    
    // Text X position property accessors
    double textX() const { return m_textX; }
    void setTextX(double x);
    
    // Text Y position property accessors
    double textY() const { return m_textY; }
    void setTextY(double y);
    
    // Pixel offset X property accessors
    float pixelOffsetX() const { return m_pixelOffsetX; }
    void setPixelOffsetX(float x);
    
    // Pixel offset Y property accessors
    float pixelOffsetY() const { return m_pixelOffsetY; }
    void setPixelOffsetY(float y);
    
    // Text color property accessors
    QColor textColor() const { return m_textColor; }
    void setTextColor(const QColor& color);
    
    // Vertical orientation property accessors
    bool isVertical() const { return m_vertical; }
    void setVertical(bool vertical);
    
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
     * @brief Signal emitted when text content changes
     * @param text New text content
     * \endif
     * 
     * \if CHINESE
     * @brief 文本内容改变时发出的信号
     * @param text 新的文本内容
     * \endif
     */
    void textChanged(const QString& text);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when text X position changes
     * @param x New text X position
     * \endif
     * 
     * \if CHINESE
     * @brief 文本 X 位置改变时发出的信号
     * @param x 新的文本 X 位置
     * \endif
     */
    void textXChanged(double x);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when text Y position changes
     * @param y New text Y position
     * \endif
     * 
     * \if CHINESE
     * @brief 文本 Y 位置改变时发出的信号
     * @param y 新的文本 Y 位置
     * \endif
     */
    void textYChanged(double y);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when pixel offset X changes
     * @param x New pixel offset X
     * \endif
     * 
     * \if CHINESE
     * @brief 像素偏移 X 改变时发出的信号
     * @param x 新的像素偏移 X
     * \endif
     */
    void pixelOffsetXChanged(float x);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when pixel offset Y changes
     * @param y New pixel offset Y
     * \endif
     * 
     * \if CHINESE
     * @brief 像素偏移 Y 改变时发出的信号
     * @param y 新的像素偏移 Y
     * \endif
     */
    void pixelOffsetYChanged(float y);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when text color changes
     * @param color New text color
     * \endif
     * 
     * \if CHINESE
     * @brief 文本颜色改变时发出的信号
     * @param color 新的文本颜色
     * \endif
     */
    void textColorChanged(const QColor& color);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when vertical orientation changes
     * @param vertical New vertical orientation
     * \endif
     * 
     * \if CHINESE
     * @brief 垂直方向改变时发出的信号
     * @param vertical 新的垂直方向
     * \endif
     */
    void verticalChanged(bool vertical);
    
private:
    QString m_title = QStringLiteral("Text Label Plot");
    QString m_xLabel = QStringLiteral("x");
    QString m_yLabel = QStringLiteral("y");
    QString m_text = QStringLiteral("Sample Text");
    double m_textX = 5.0;
    double m_textY = 5.0;
    float m_pixelOffsetX = 0.0f;
    float m_pixelOffsetY = 0.0f;
    QColor m_textColor = Qt::red;
    bool m_vertical = false;
    
    QIM::QImPlotNode* m_plotNode = nullptr;
    QIM::QImPlotTextItemNode* m_textNode = nullptr;
};

#endif // TEXTFUNCTION_H
