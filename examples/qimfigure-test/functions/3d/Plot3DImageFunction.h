#ifndef PLOT3DIMAGEFUNCTION_H
#define PLOT3DIMAGEFUNCTION_H

#include "../TestFunction.h"
#include <QObject>
#include <QColor>
#include <QPointer>

namespace QIM {
class QImFigureWidget;
class QImPlot3DNode;
class QImPlot3DImageItemNode;
}

/**
 * \if ENGLISH
 * @brief Test function demonstrating 3D image plot with property registration
 * 
 * Plot3DImageFunction creates a 3D plot with an image item using ImGui's built-in
 * font texture as test texture. It registers properties for:
 * - Plot title
 * - X, Y and Z axis labels
 * - Image center coordinates
 * - Image U and V axis vectors
 * - Image tint color
 * - Image UV coordinates
 * 
 * @note The textureId must be set at runtime via the property panel with a valid
 * GPU texture ID. The default value is 0 (invalid), as the actual texture ID depends
 * on the ImGui backend initialization and must be obtained from the rendering context.
 * 
 * @section usage_example Usage Example
 * 
 * \code
 * auto func = new Plot3DImageFunction(this);
 * func->createPlot(figureWidget);
 * // Set textureId at runtime via property panel with a valid GPU texture ID
 * // Properties will be available via getRegisteredProperties()
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlot3DNode
 * @see QImPlot3DImageItemNode
 * \endif
 * 
 * \if CHINESE
 * @brief 演示 3D 图像图的测试函数，带有属性注册
 * 
 * Plot3DImageFunction 创建一个包含图像元素的 3D 图表，使用 ImGui 内置的
 * 字体纹理作为测试纹理。它注册了以下属性：
 * - 图表标题
 * - X、Y 和 Z 轴标签
 * - 图像中心坐标
 * - 图像 U 和 V 轴向量
 * - 图像色调颜色
 * - 图像 UV 坐标
 * 
 * @note textureId 必须在运行时通过属性面板设置有效的 GPU 纹理 ID。
 * 默认值为 0（无效），因为实际纹理 ID 取决于 ImGui 后端初始化，
 * 必须从渲染上下文中获取。
 * 
 * @section usage_example 使用示例
 * 
 * \code
 * auto func = new Plot3DImageFunction(this);
 * func->createPlot(figureWidget);
 * // 运行时通过属性面板设置有效的 GPU 纹理 ID
 * // 属性可通过 getRegisteredProperties() 获取
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlot3DNode
 * @see QImPlot3DImageItemNode
 * \endif
 */
class Plot3DImageFunction : public TestFunction {
    Q_OBJECT
    
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString xLabel READ xLabel WRITE setXLabel NOTIFY xLabelChanged)
    Q_PROPERTY(QString yLabel READ yLabel WRITE setYLabel NOTIFY yLabelChanged)
    Q_PROPERTY(QString zLabel READ zLabel WRITE setZLabel NOTIFY zLabelChanged)
    
    Q_PROPERTY(double centerX READ centerX WRITE setCenterX NOTIFY centerXChanged)
    Q_PROPERTY(double centerY READ centerY WRITE setCenterY NOTIFY centerYChanged)
    Q_PROPERTY(double centerZ READ centerZ WRITE setCenterZ NOTIFY centerZChanged)
    
    Q_PROPERTY(double axisUX READ axisUX WRITE setAxisUX NOTIFY axisUXChanged)
    Q_PROPERTY(double axisUY READ axisUY WRITE setAxisUY NOTIFY axisUYChanged)
    Q_PROPERTY(double axisUZ READ axisUZ WRITE setAxisUZ NOTIFY axisUZChanged)
    
    Q_PROPERTY(double axisVX READ axisVX WRITE setAxisVX NOTIFY axisVXChanged)
    Q_PROPERTY(double axisVY READ axisVY WRITE setAxisVY NOTIFY axisVYChanged)
    Q_PROPERTY(double axisVZ READ axisVZ WRITE setAxisVZ NOTIFY axisVZChanged)
    
    Q_PROPERTY(QColor tintColor READ tintColor WRITE setTintColor NOTIFY tintColorChanged)
    
    Q_PROPERTY(double uv0X READ uv0X WRITE setUv0X NOTIFY uv0XChanged)
    Q_PROPERTY(double uv0Y READ uv0Y WRITE setUv0Y NOTIFY uv0YChanged)
    Q_PROPERTY(double uv1X READ uv1X WRITE setUv1X NOTIFY uv1XChanged)
    Q_PROPERTY(double uv1Y READ uv1Y WRITE setUv1Y NOTIFY uv1YChanged)
    
public:
    /**
     * \if ENGLISH
     * @brief Constructor for Plot3DImageFunction
     * @param parent Parent object
     * \endif
     * 
     * \if CHINESE
     * @brief Plot3DImageFunction 构造函数
     * @param parent 父对象
     * \endif
     */
    explicit Plot3DImageFunction(QObject* parent = nullptr);
    
    /**
     * \if ENGLISH
     * @brief Destructor for Plot3DImageFunction
     * \endif
     * 
     * \if CHINESE
     * @brief Plot3DImageFunction 析构函数
     * \endif
     */
    ~Plot3DImageFunction() override;
    
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
    
    // Center property accessors
    double centerX() const { return m_centerX; }
    void setCenterX(double x);
    
    double centerY() const { return m_centerY; }
    void setCenterY(double y);
    
    double centerZ() const { return m_centerZ; }
    void setCenterZ(double z);
    
    // Axis U property accessors
    double axisUX() const { return m_axisUX; }
    void setAxisUX(double x);
    
    double axisUY() const { return m_axisUY; }
    void setAxisUY(double y);
    
    double axisUZ() const { return m_axisUZ; }
    void setAxisUZ(double z);
    
    // Axis V property accessors
    double axisVX() const { return m_axisVX; }
    void setAxisVX(double x);
    
    double axisVY() const { return m_axisVY; }
    void setAxisVY(double y);
    
    double axisVZ() const { return m_axisVZ; }
    void setAxisVZ(double z);
    
    // Tint color property accessors
    QColor tintColor() const { return m_tintColor; }
    void setTintColor(const QColor& color);
    
    // UV0 property accessors
    double uv0X() const { return m_uv0X; }
    void setUv0X(double x);
    
    double uv0Y() const { return m_uv0Y; }
    void setUv0Y(double y);
    
    // UV1 property accessors
    double uv1X() const { return m_uv1X; }
    void setUv1X(double x);
    
    double uv1Y() const { return m_uv1Y; }
    void setUv1Y(double y);
    
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
     * @brief Signal emitted when center X changes
     * @param x New center X value
     * \endif
     * 
     * \if CHINESE
     * @brief 中心X坐标改变时发出的信号
     * @param x 新的中心X坐标值
     * \endif
     */
    void centerXChanged(double x);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when center Y changes
     * @param y New center Y value
     * \endif
     * 
     * \if CHINESE
     * @brief 中心Y坐标改变时发出的信号
     * @param y 新的中心Y坐标值
     * \endif
     */
    void centerYChanged(double y);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when center Z changes
     * @param z New center Z value
     * \endif
     * 
     * \if CHINESE
     * @brief 中心Z坐标改变时发出的信号
     * @param z 新的中心Z坐标值
     * \endif
     */
    void centerZChanged(double z);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when axis U X changes
     * @param x New axis U X value
     * \endif
     * 
     * \if CHINESE
     * @brief U轴X分量改变时发出的信号
     * @param x 新的U轴X分量值
     * \endif
     */
    void axisUXChanged(double x);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when axis U Y changes
     * @param y New axis U Y value
     * \endif
     * 
     * \if CHINESE
     * @brief U轴Y分量改变时发出的信号
     * @param y 新的U轴Y分量值
     * \endif
     */
    void axisUYChanged(double y);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when axis U Z changes
     * @param z New axis U Z value
     * \endif
     * 
     * \if CHINESE
     * @brief U轴Z分量改变时发出的信号
     * @param z 新的U轴Z分量值
     * \endif
     */
    void axisUZChanged(double z);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when axis V X changes
     * @param x New axis V X value
     * \endif
     * 
     * \if CHINESE
     * @brief V轴X分量改变时发出的信号
     * @param x 新的V轴X分量值
     * \endif
     */
    void axisVXChanged(double x);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when axis V Y changes
     * @param y New axis V Y value
     * \endif
     * 
     * \if CHINESE
     * @brief V轴Y分量改变时发出的信号
     * @param y 新的V轴Y分量值
     * \endif
     */
    void axisVYChanged(double y);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when axis V Z changes
     * @param z New axis V Z value
     * \endif
     * 
     * \if CHINESE
     * @brief V轴Z分量改变时发出的信号
     * @param z 新的V轴Z分量值
     * \endif
     */
    void axisVZChanged(double z);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when tint color changes
     * @param color New tint color value
     * \endif
     * 
     * \if CHINESE
     * @brief 色调颜色改变时发出的信号
     * @param color 新的色调颜色值
     * \endif
     */
    void tintColorChanged(const QColor& color);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when UV0 X changes
     * @param x New UV0 X value
     * \endif
     * 
     * \if CHINESE
     * @brief UV0 X坐标改变时发出的信号
     * @param x 新的UV0 X坐标值
     * \endif
     */
    void uv0XChanged(double x);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when UV0 Y changes
     * @param y New UV0 Y value
     * \endif
     * 
     * \if CHINESE
     * @brief UV0 Y坐标改变时发出的信号
     * @param y 新的UV0 Y坐标值
     * \endif
     */
    void uv0YChanged(double y);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when UV1 X changes
     * @param x New UV1 X value
     * \endif
     * 
     * \if CHINESE
     * @brief UV1 X坐标改变时发出的信号
     * @param x 新的UV1 X坐标值
     * \endif
     */
    void uv1XChanged(double x);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when UV1 Y changes
     * @param y New UV1 Y value
     * \endif
     * 
     * \if CHINESE
     * @brief UV1 Y坐标改变时发出的信号
     * @param y 新的UV1 Y坐标值
     * \endif
     */
    void uv1YChanged(double y);
    
private:
    QString m_title = QStringLiteral("3D Image - Font Texture");
    QString m_xLabel = QStringLiteral("X");
    QString m_yLabel = QStringLiteral("Y");
    QString m_zLabel = QStringLiteral("Z");
    
    double m_centerX = 0.0;
    double m_centerY = 0.0;
    double m_centerZ = 0.0;
    
    double m_axisUX = 0.5;
    double m_axisUY = 0.0;
    double m_axisUZ = 0.0;
    
    double m_axisVX = 0.0;
    double m_axisVY = 0.5;
    double m_axisVZ = 0.0;
    
    QColor m_tintColor = Qt::white;
    
    double m_uv0X = 0.0;
    double m_uv0Y = 0.0;
    double m_uv1X = 1.0;
    double m_uv1Y = 1.0;
    
    QPointer<QIM::QImPlot3DNode> m_plot3DNode = nullptr;
    QPointer<QIM::QImPlot3DImageItemNode> m_image3DNode = nullptr;
};

#endif // PLOT3DIMAGEFUNCTION_H
