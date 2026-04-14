#ifndef TAGSFUNCTION_H
#define TAGSFUNCTION_H

#include "../TestFunction.h"
#include <QObject>
#include <QColor>

namespace QIM {
class QImFigureWidget;
class QImPlotNode;
class QImPlotTagXNode;
class QImPlotTagYNode;
class QImPlotLineItemNode;
}

/**
 * \if ENGLISH
 * @brief Test function demonstrating tag tools (TagX and TagY)
 * 
 * TagsFunction creates a plot with X-axis and Y-axis tag markers.
 * It demonstrates:
 * - X-axis tag (vertical line with label)
 * - Y-axis tag (horizontal line with label)
 * - Customizable tag colors and text
 * - Round option for pixel alignment
 * 
 * @section usage_example Usage Example
 * 
 * \code
 * auto func = new TagsFunction(this);
 * func->createPlot(figureWidget);
 * // Adjust tag positions and text via properties
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotTagXNode
 * @see QImPlotTagYNode
 * \endif
 * 
 * \if CHINESE
 * @brief 演示标签工具 (TagX 和 TagY) 的测试函数
 * 
 * TagsFunction 创建一个包含X轴和Y轴标签标记的图表。
 * 它演示了：
 * - X轴标签（带标签的垂直线）
 * - Y轴标签（带标签的水平线）
 * - 可自定义的标签颜色和文本
 * - 像素对齐的舍入选项
 * 
 * @section usage_example 使用示例
 * 
 * \code
 * auto func = new TagsFunction(this);
 * func->createPlot(figureWidget);
 * // 通过属性调整标签位置和文本
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotTagXNode
 * @see QImPlotTagYNode
 * \endif
 */
class TagsFunction : public TestFunction {
    Q_OBJECT
    
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(double tagXValue READ tagXValue WRITE setTagXValue NOTIFY tagXValueChanged)
    Q_PROPERTY(QString tagXText READ tagXText WRITE setTagXText NOTIFY tagXTextChanged)
    Q_PROPERTY(QColor tagXColor READ tagXColor WRITE setTagXColor NOTIFY tagXColorChanged)
    Q_PROPERTY(double tagYValue READ tagYValue WRITE setTagYValue NOTIFY tagYValueChanged)
    Q_PROPERTY(QString tagYText READ tagYText WRITE setTagYText NOTIFY tagYTextChanged)
    Q_PROPERTY(QColor tagYColor READ tagYColor WRITE setTagYColor NOTIFY tagYColorChanged)
    
public:
    /**
     * \if ENGLISH
     * @brief Constructor for TagsFunction
     * @param parent Parent object
     * \endif
     * 
     * \if CHINESE
     * @brief TagsFunction 构造函数
     * @param parent 父对象
     * \endif
     */
    explicit TagsFunction(QObject* parent = nullptr);
    
    /**
     * \if ENGLISH
     * @brief Destructor for TagsFunction
     * \endif
     * 
     * \if CHINESE
     * @brief TagsFunction 析构函数
     * \endif
     */
    ~TagsFunction() override;
    
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
    
    // TagX value property accessors
    double tagXValue() const { return m_tagXValue; }
    void setTagXValue(double value);
    
    // TagX text property accessors
    QString tagXText() const { return m_tagXText; }
    void setTagXText(const QString& text);
    
    // TagX color property accessors
    QColor tagXColor() const { return m_tagXColor; }
    void setTagXColor(const QColor& color);
    
    // TagY value property accessors
    double tagYValue() const { return m_tagYValue; }
    void setTagYValue(double value);
    
    // TagY text property accessors
    QString tagYText() const { return m_tagYText; }
    void setTagYText(const QString& text);
    
    // TagY color property accessors
    QColor tagYColor() const { return m_tagYColor; }
    void setTagYColor(const QColor& color);
    
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
     * @brief Signal emitted when TagX value changes
     * @param value New X position value
     * \endif
     * 
     * \if CHINESE
     * @brief TagX值改变时发出的信号
     * @param value 新的X位置值
     * \endif
     */
    void tagXValueChanged(double value);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when TagX text changes
     * @param text New text value
     * \endif
     * 
     * \if CHINESE
     * @brief TagX文本改变时发出的信号
     * @param text 新的文本值
     * \endif
     */
    void tagXTextChanged(const QString& text);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when TagX color changes
     * @param color New color value
     * \endif
     * 
     * \if CHINESE
     * @brief TagX颜色改变时发出的信号
     * @param color 新的颜色值
     * \endif
     */
    void tagXColorChanged(const QColor& color);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when TagY value changes
     * @param value New Y position value
     * \endif
     * 
     * \if CHINESE
     * @brief TagY值改变时发出的信号
     * @param value 新的Y位置值
     * \endif
     */
    void tagYValueChanged(double value);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when TagY text changes
     * @param text New text value
     * \endif
     * 
     * \if CHINESE
     * @brief TagY文本改变时发出的信号
     * @param text 新的文本值
     * \endif
     */
    void tagYTextChanged(const QString& text);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when TagY color changes
     * @param color New color value
     * \endif
     * 
     * \if CHINESE
     * @brief TagY颜色改变时发出的信号
     * @param color 新的颜色值
     * \endif
     */
    void tagYColorChanged(const QColor& color);
    
private:
    QString m_title = QStringLiteral("Tags Tool");
    double m_tagXValue = 5.0;
    QString m_tagXText = QStringLiteral("X=5.0");
    QColor m_tagXColor = QColor(255, 100, 100);
    double m_tagYValue = 5.0;
    QString m_tagYText = QStringLiteral("Y=5.0");
    QColor m_tagYColor = QColor(100, 255, 100);
    
    QIM::QImPlotNode* m_plotNode = nullptr;
    QIM::QImPlotLineItemNode* m_lineNode = nullptr;
    QIM::QImPlotTagXNode* m_tagXNode = nullptr;
    QIM::QImPlotTagYNode* m_tagYNode = nullptr;
};

#endif // TAGSFUNCTION_H