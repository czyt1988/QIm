#ifndef PLOT3DTEXTFUNCTION_H
#define PLOT3DTEXTFUNCTION_H

#include "../TestFunction.h"
#include <QObject>
#include <QColor>

namespace QIM {
class QImFigureWidget;
class QImPlot3DNode;
class QImPlot3DTextItemNode;
}

/**
 * \if ENGLISH
 * @brief Test function demonstrating 3D text label with property registration
 * 
 * Plot3DTextFunction creates a 3D plot with a text label to test
 * 3D text rendering capabilities. It registers properties for:
 * - Plot title
 * - X, Y and Z axis labels
 * - Text content
 * - Text position (X, Y, Z)
 * - Text rotation angle
 * - Pixel offset (X, Y)
 * - Text color
 * 
 * @section usage_example Usage Example
 * 
 * \code
 * auto func = new Plot3DTextFunction(this);
 * func->createPlot(figureWidget);
 * // Properties will be available via getRegisteredProperties()
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlot3DNode
 * @see QImPlot3DTextItemNode
 * \endif
 * 
 * \if CHINESE
 * @brief 演示 3D 文本标签的测试函数，带有属性注册
 * 
 * Plot3DTextFunction 创建一个包含文本标签的 3D 图表，用于测试
 * 3D 文本渲染能力。它注册了以下属性：
 * - 图表标题
 * - X、Y 和 Z 轴标签
 * - 文本内容
 * - 文本位置 (X, Y, Z)
 * - 文本旋转角度
 * - 像素偏移 (X, Y)
 * - 文本颜色
 * 
 * @section usage_example 使用示例
 * 
 * \code
 * auto func = new Plot3DTextFunction(this);
 * func->createPlot(figureWidget);
 * // 属性可通过 getRegisteredProperties() 获取
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlot3DNode
 * @see QImPlot3DTextItemNode
 * \endif
 */
class Plot3DTextFunction : public TestFunction {
    Q_OBJECT
    
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString xLabel READ xLabel WRITE setXLabel NOTIFY xLabelChanged)
    Q_PROPERTY(QString yLabel READ yLabel WRITE setYLabel NOTIFY yLabelChanged)
    Q_PROPERTY(QString zLabel READ zLabel WRITE setZLabel NOTIFY zLabelChanged)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(double x READ x WRITE setX NOTIFY positionChanged)
    Q_PROPERTY(double y READ y WRITE setY NOTIFY positionChanged)
    Q_PROPERTY(double z READ z WRITE setZ NOTIFY positionChanged)
    Q_PROPERTY(double angle READ angle WRITE setAngle NOTIFY angleChanged)
    Q_PROPERTY(double pixelOffsetX READ pixelOffsetX WRITE setPixelOffsetX NOTIFY pixelOffsetChanged)
    Q_PROPERTY(double pixelOffsetY READ pixelOffsetY WRITE setPixelOffsetY NOTIFY pixelOffsetChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    
public:
    /**
     * \if ENGLISH
     * @brief Constructor for Plot3DTextFunction
     * @param parent Parent object
     * \endif
     * 
     * \if CHINESE
     * @brief Plot3DTextFunction 构造函数
     * @param parent 父对象
     * \endif
     */
    explicit Plot3DTextFunction(QObject* parent = nullptr);
    
    /**
     * \if ENGLISH
     * @brief Destructor for Plot3DTextFunction
     * \endif
     * 
     * \if CHINESE
     * @brief Plot3DTextFunction 析构函数
     * \endif
     */
    ~Plot3DTextFunction() override;
    
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
    
    // Z axis label property accessors
    QString zLabel() const { return m_zLabel; }
    void setZLabel(const QString& label);
    
    // Text property accessors
    QString text() const { return m_text; }
    void setText(const QString& text);
    
    // Position property accessors
    double x() const { return m_x; }
    void setX(double x);
    
    double y() const { return m_y; }
    void setY(double y);
    
    double z() const { return m_z; }
    void setZ(double z);
    
    // Angle property accessor
    double angle() const { return m_angle; }
    void setAngle(double angle);
    
    // Pixel offset property accessors
    double pixelOffsetX() const { return m_pixelOffsetX; }
    void setPixelOffsetX(double offset);
    
    double pixelOffsetY() const { return m_pixelOffsetY; }
    void setPixelOffsetY(double offset);
    
    // Color property accessor
    QColor color() const { return m_color; }
    void setColor(const QColor& color);
    
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
     * @brief Signal emitted when text changes
     * @param text New text value
     * \endif
     * 
     * \if CHINESE
     * @brief 文本改变时发出的信号
     * @param text 新的文本值
     * \endif
     */
    void textChanged(const QString& text);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when position changes
     * @param x New X coordinate
     * @param y New Y coordinate
     * @param z New Z coordinate
     * \endif
     * 
     * \if CHINESE
     * @brief 位置改变时发出的信号
     * @param x 新的 X 坐标
     * @param y 新的 Y 坐标
     * @param z 新的 Z 坐标
     * \endif
     */
    void positionChanged(double x, double y, double z);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when angle changes
     * @param angle New angle value
     * \endif
     * 
     * \if CHINESE
     * @brief 角度改变时发出的信号
     * @param angle 新的角度值
     * \endif
     */
    void angleChanged(double angle);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when pixel offset changes
     * @param offsetX New pixel offset X
     * @param offsetY New pixel offset Y
     * \endif
     * 
     * \if CHINESE
     * @brief 像素偏移改变时发出的信号
     * @param offsetX 新的像素偏移 X
     * @param offsetY 新的像素偏移 Y
     * \endif
     */
    void pixelOffsetChanged(double offsetX, double offsetY);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when color changes
     * @param color New color value
     * \endif
     * 
     * \if CHINESE
     * @brief 颜色改变时发出的信号
     * @param color 新的颜色值
     * \endif
     */
    void colorChanged(const QColor& color);
    
private:
    QString m_title = QStringLiteral("3D Text - Hello QIm!");
    QString m_xLabel = QStringLiteral("X");
    QString m_yLabel = QStringLiteral("Y");
    QString m_zLabel = QStringLiteral("Z");
    QString m_text = QStringLiteral("Hello QIm 3D!");
    double m_x = 0.0;
    double m_y = 0.0;
    double m_z = 0.5;
    double m_angle = 0.0;
    double m_pixelOffsetX = 0.0;
    double m_pixelOffsetY = 0.0;
    QColor m_color = Qt::red;
    
    QIM::QImPlot3DNode* m_plot3DNode = nullptr;
    QIM::QImPlot3DTextItemNode* m_text3DNode = nullptr;
};

#endif // PLOT3DTEXTFUNCTION_H
