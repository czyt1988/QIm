#ifndef IMAGEFUNCTION_H
#define IMAGEFUNCTION_H

#include "../TestFunction.h"
#include <QObject>
#include <QColor>
#include <QPointF>

namespace QIM {
class QImFigureWidget;
class QImPlotNode;
class QImPlotImageItemNode;
class QImPlotValueTrackerNode;
}

/**
 * \if ENGLISH
 * @brief Test function demonstrating image rendering with property registration
 * 
 * ImageFunction creates an image item with placeholder texture ID to test image rendering
 * capabilities. It registers properties for:
 * - Plot title
 * - X and Y axis labels
 * - Texture ID (placeholder)
 * - Bounds (lower-left and upper-right corners)
 * - Texture coordinates UV0 and UV1
 * - Tint color
 * 
 * @section usage_example Usage Example
 * 
 * \code
 * auto func = new ImageFunction(this);
 * func->createPlot(figureWidget);
 * // Properties will be available via getRegisteredProperties()
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotNode
 * @see QImPlotImageItemNode
 * \endif
 * 
 * \if CHINESE
 * @brief 演示图像渲染的测试函数，带有属性注册
 * 
 * ImageFunction 创建一个带有占位纹理ID的图像项，用于测试图像渲染能力。
 * 它注册了以下属性：
 * - 图表标题
 * - X 和 Y 轴标签
 * - 纹理ID（占位符）
 * - 边界（左下角和右上角）
 * - 纹理坐标 UV0 和 UV1
 * - 色调颜色
 * 
 * @section usage_example 使用示例
 * 
 * \code
 * auto func = new ImageFunction(this);
 * func->createPlot(figureWidget);
 * // 属性可通过 getRegisteredProperties() 获取
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotNode
 * @see QImPlotImageItemNode
 * \endif
 */
class ImageFunction : public TestFunction {
    Q_OBJECT
    
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString xLabel READ xLabel WRITE setXLabel NOTIFY xLabelChanged)
    Q_PROPERTY(QString yLabel READ yLabel WRITE setYLabel NOTIFY yLabelChanged)
    Q_PROPERTY(quintptr textureId READ textureId WRITE setTextureId NOTIFY textureIdChanged)
    Q_PROPERTY(QPointF boundsMin READ boundsMin WRITE setBoundsMin NOTIFY boundsMinChanged)
    Q_PROPERTY(QPointF boundsMax READ boundsMax WRITE setBoundsMax NOTIFY boundsMaxChanged)
    Q_PROPERTY(double boundsMinX READ boundsMinX WRITE setBoundsMinX NOTIFY boundsMinXChanged)
    Q_PROPERTY(double boundsMinY READ boundsMinY WRITE setBoundsMinY NOTIFY boundsMinYChanged)
    Q_PROPERTY(double boundsMaxX READ boundsMaxX WRITE setBoundsMaxX NOTIFY boundsMaxXChanged)
    Q_PROPERTY(double boundsMaxY READ boundsMaxY WRITE setBoundsMaxY NOTIFY boundsMaxYChanged)
    Q_PROPERTY(QPointF uv0 READ uv0 WRITE setUv0 NOTIFY uv0Changed)
    Q_PROPERTY(QPointF uv1 READ uv1 WRITE setUv1 NOTIFY uv1Changed)
    Q_PROPERTY(QColor tintColor READ tintColor WRITE setTintColor NOTIFY tintColorChanged)
    
public:
    /**
     * \if ENGLISH
     * @brief Constructor for ImageFunction
     * @param parent Parent object
     * \endif
     * 
     * \if CHINESE
     * @brief ImageFunction 构造函数
     * @param parent 父对象
     * \endif
     */
    explicit ImageFunction(QObject* parent = nullptr);
    
    /**
     * \if ENGLISH
     * @brief Destructor for ImageFunction
     * \endif
     * 
     * \if CHINESE
     * @brief ImageFunction 析构函数
     * \endif
     */
    ~ImageFunction() override;
    
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
    
    // Texture ID property accessors
    quintptr textureId() const { return m_textureId; }
    void setTextureId(quintptr id);
    
    // Bounds minimum property accessors
    QPointF boundsMin() const { return m_boundsMin; }
    void setBoundsMin(const QPointF& min);
    
    // Bounds minimum X coordinate property accessors
    double boundsMinX() const { return m_boundsMin.x(); }
    void setBoundsMinX(double x);
    
    // Bounds minimum Y coordinate property accessors
    double boundsMinY() const { return m_boundsMin.y(); }
    void setBoundsMinY(double y);
    
    // Bounds maximum property accessors
    QPointF boundsMax() const { return m_boundsMax; }
    void setBoundsMax(const QPointF& max);
    
    // Bounds maximum X coordinate property accessors
    double boundsMaxX() const { return m_boundsMax.x(); }
    void setBoundsMaxX(double x);
    
    // Bounds maximum Y coordinate property accessors
    double boundsMaxY() const { return m_boundsMax.y(); }
    void setBoundsMaxY(double y);
    
    // Texture coordinate UV0 property accessors
    QPointF uv0() const { return m_uv0; }
    void setUv0(const QPointF& uv);
    
    // Texture coordinate UV1 property accessors
    QPointF uv1() const { return m_uv1; }
    void setUv1(const QPointF& uv);
    
    // Tint color property accessors
    QColor tintColor() const { return m_tintColor; }
    void setTintColor(const QColor& color);
    
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
     * @brief Signal emitted when texture ID changes
     * @param id New texture ID value
     * \endif
     * 
     * \if CHINESE
     * @brief 纹理ID改变时发出的信号
     * @param id 新的纹理ID值
     * \endif
     */
    void textureIdChanged(quintptr id);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when lower-left bounds change
     * @param min New lower-left bounds position
     * \endif
     * 
     * \if CHINESE
     * @brief 左下角边界改变时发出的信号
     * @param min 新的左下角边界位置
     * \endif
     */
    void boundsMinChanged(const QPointF& min);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when lower-left bounds X coordinate changes
     * @param x New X coordinate
     * \endif
     * 
     * \if CHINESE
     * @brief 左下角边界X坐标改变时发出的信号
     * @param x 新的X坐标
     * \endif
     */
    void boundsMinXChanged(double x);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when lower-left bounds Y coordinate changes
     * @param y New Y coordinate
     * \endif
     * 
     * \if CHINESE
     * @brief 左下角边界Y坐标改变时发出的信号
     * @param y 新的Y坐标
     * \endif
     */
    void boundsMinYChanged(double y);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when upper-right bounds change
     * @param max New upper-right bounds position
     * \endif
     * 
     * \if CHINESE
     * @brief 右上角边界改变时发出的信号
     * @param max 新的右上角边界位置
     * \endif
     */
    void boundsMaxChanged(const QPointF& max);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when upper-right bounds X coordinate changes
     * @param x New X coordinate
     * \endif
     * 
     * \if CHINESE
     * @brief 右上角边界X坐标改变时发出的信号
     * @param x 新的X坐标
     * \endif
     */
    void boundsMaxXChanged(double x);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when upper-right bounds Y coordinate changes
     * @param y New Y coordinate
     * \endif
     * 
     * \if CHINESE
     * @brief 右上角边界Y坐标改变时发出的信号
     * @param y 新的Y坐标
     * \endif
     */
    void boundsMaxYChanged(double y);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when lower-left texture coordinate changes
     * @param uv New UV0 coordinate
     * \endif
     * 
     * \if CHINESE
     * @brief 左下角纹理坐标改变时发出的信号
     * @param uv 新的UV0坐标
     * \endif
     */
    void uv0Changed(const QPointF& uv);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when upper-right texture coordinate changes
     * @param uv New UV1 coordinate
     * \endif
     * 
     * \if CHINESE
     * @brief 右上角纹理坐标改变时发出的信号
     * @param uv 新的UV1坐标
     * \endif
     */
    void uv1Changed(const QPointF& uv);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when tint color changes
     * @param color New tint color
     * \endif
     * 
     * \if CHINESE
     * @brief 色调颜色改变时发出的信号
     * @param color 新的色调颜色
     * \endif
     */
    void tintColorChanged(const QColor& color);
    
private:
    QString m_title = QStringLiteral("Image Chart");
    QString m_xLabel = QStringLiteral("x");
    QString m_yLabel = QStringLiteral("y");
    quintptr m_textureId = 0;
    QPointF m_boundsMin = QPointF(0.0, 0.0);
    QPointF m_boundsMax = QPointF(10.0, 10.0);
    QPointF m_uv0 = QPointF(0.0, 0.0);
    QPointF m_uv1 = QPointF(1.0, 1.0);
    QColor m_tintColor = Qt::white;
    
    QIM::QImPlotNode* m_plotNode = nullptr;
    QIM::QImPlotImageItemNode* m_imageNode = nullptr;
    QIM::QImPlotValueTrackerNode* m_trackerNode = nullptr;
};

#endif // IMAGEFUNCTION_H