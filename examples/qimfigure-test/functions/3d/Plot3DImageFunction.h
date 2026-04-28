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
 * - Image texture ID
 * - Image center coordinates
 * - Image U and V axis vectors
 * - Image UV coordinates
 * - Image tint color
 * - Quad mode toggle
 * - Quad corner points (P0-P3)
 * - Quad per-corner UV coordinates
 * 
 * @note The textureId must be set at runtime via the property panel with a valid
 * GPU texture ID.
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
 * - 图像纹理 ID
 * - 图像中心坐标
 * - 图像 U 和 V 轴向量
 * - 图像 UV 坐标
 * - 图像色调颜色
 * - 四边形模式开关
 * - 四边形角点坐标 (P0-P3)
 * - 四边形逐角 UV 坐标
 * 
 * @note textureId 必须在运行时通过属性面板设置有效的 GPU 纹理 ID。
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
    
    Q_PROPERTY(int textureId READ textureId WRITE setTextureId NOTIFY textureIdChanged)
    
    Q_PROPERTY(double centerX READ centerX WRITE setCenterX NOTIFY centerXChanged)
    Q_PROPERTY(double centerY READ centerY WRITE setCenterY NOTIFY centerYChanged)
    Q_PROPERTY(double centerZ READ centerZ WRITE setCenterZ NOTIFY centerZChanged)
    
    Q_PROPERTY(double axisUX READ axisUX WRITE setAxisUX NOTIFY axisUXChanged)
    Q_PROPERTY(double axisUY READ axisUY WRITE setAxisUY NOTIFY axisUYChanged)
    Q_PROPERTY(double axisUZ READ axisUZ WRITE setAxisUZ NOTIFY axisUZChanged)
    
    Q_PROPERTY(double axisVX READ axisVX WRITE setAxisVX NOTIFY axisVXChanged)
    Q_PROPERTY(double axisVY READ axisVY WRITE setAxisVY NOTIFY axisVYChanged)
    Q_PROPERTY(double axisVZ READ axisVZ WRITE setAxisVZ NOTIFY axisVZChanged)
    
    Q_PROPERTY(double uv0X READ uv0X WRITE setUv0X NOTIFY uv0XChanged)
    Q_PROPERTY(double uv0Y READ uv0Y WRITE setUv0Y NOTIFY uv0YChanged)
    Q_PROPERTY(double uv1X READ uv1X WRITE setUv1X NOTIFY uv1XChanged)
    Q_PROPERTY(double uv1Y READ uv1Y WRITE setUv1Y NOTIFY uv1YChanged)
    
    Q_PROPERTY(QColor tintColor READ tintColor WRITE setTintColor NOTIFY tintColorChanged)
    
    Q_PROPERTY(bool quadModeEnabled READ quadModeEnabled WRITE setQuadModeEnabled NOTIFY quadModeEnabledChanged)
    
    Q_PROPERTY(double p0x READ p0x WRITE setP0x NOTIFY p0xChanged)
    Q_PROPERTY(double p0y READ p0y WRITE setP0y NOTIFY p0yChanged)
    Q_PROPERTY(double p0z READ p0z WRITE setP0z NOTIFY p0zChanged)
    Q_PROPERTY(double p1x READ p1x WRITE setP1x NOTIFY p1xChanged)
    Q_PROPERTY(double p1y READ p1y WRITE setP1y NOTIFY p1yChanged)
    Q_PROPERTY(double p1z READ p1z WRITE setP1z NOTIFY p1zChanged)
    Q_PROPERTY(double p2x READ p2x WRITE setP2x NOTIFY p2xChanged)
    Q_PROPERTY(double p2y READ p2y WRITE setP2y NOTIFY p2yChanged)
    Q_PROPERTY(double p2z READ p2z WRITE setP2z NOTIFY p2zChanged)
    Q_PROPERTY(double p3x READ p3x WRITE setP3x NOTIFY p3xChanged)
    Q_PROPERTY(double p3y READ p3y WRITE setP3y NOTIFY p3yChanged)
    Q_PROPERTY(double p3z READ p3z WRITE setP3z NOTIFY p3zChanged)
    
    Q_PROPERTY(double uvP0x READ uvP0x WRITE setUvP0x NOTIFY uvP0xChanged)
    Q_PROPERTY(double uvP0y READ uvP0y WRITE setUvP0y NOTIFY uvP0yChanged)
    Q_PROPERTY(double uvP1x READ uvP1x WRITE setUvP1x NOTIFY uvP1xChanged)
    Q_PROPERTY(double uvP1y READ uvP1y WRITE setUvP1y NOTIFY uvP1yChanged)
    Q_PROPERTY(double uvP2x READ uvP2x WRITE setUvP2x NOTIFY uvP2xChanged)
    Q_PROPERTY(double uvP2y READ uvP2y WRITE setUvP2y NOTIFY uvP2yChanged)
    Q_PROPERTY(double uvP3x READ uvP3x WRITE setUvP3x NOTIFY uvP3xChanged)
    Q_PROPERTY(double uvP3y READ uvP3y WRITE setUvP3y NOTIFY uvP3yChanged)
    
public:
    explicit Plot3DImageFunction(QObject* parent = nullptr);
    ~Plot3DImageFunction() override;
    
    void createPlot(QIM::QImFigureWidget* figure) override;
    void cleanupPlot() override;
    
    // Title and axis label accessors
    QString title() const { return m_title; }
    void setTitle(const QString& title);
    QString xLabel() const { return m_xLabel; }
    void setXLabel(const QString& label);
    QString yLabel() const { return m_yLabel; }
    void setYLabel(const QString& label);
    QString zLabel() const { return m_zLabel; }
    void setZLabel(const QString& label);
    
    // Texture ID accessor
    int textureId() const { return m_textureId; }
    void setTextureId(int id);
    
    // Center coordinate accessors
    double centerX() const { return m_centerX; }
    void setCenterX(double x);
    double centerY() const { return m_centerY; }
    void setCenterY(double y);
    double centerZ() const { return m_centerZ; }
    void setCenterZ(double z);
    
    // Axis U accessors
    double axisUX() const { return m_axisUX; }
    void setAxisUX(double x);
    double axisUY() const { return m_axisUY; }
    void setAxisUY(double y);
    double axisUZ() const { return m_axisUZ; }
    void setAxisUZ(double z);
    
    // Axis V accessors
    double axisVX() const { return m_axisVX; }
    void setAxisVX(double x);
    double axisVY() const { return m_axisVY; }
    void setAxisVY(double y);
    double axisVZ() const { return m_axisVZ; }
    void setAxisVZ(double z);
    
    // UV0 accessors
    double uv0X() const { return m_uv0X; }
    void setUv0X(double x);
    double uv0Y() const { return m_uv0Y; }
    void setUv0Y(double y);
    
    // UV1 accessors
    double uv1X() const { return m_uv1X; }
    void setUv1X(double x);
    double uv1Y() const { return m_uv1Y; }
    void setUv1Y(double y);
    
    // Tint color accessor
    QColor tintColor() const { return m_tintColor; }
    void setTintColor(const QColor& color);
    
    // Quad mode accessor
    bool quadModeEnabled() const { return m_quadModeEnabled; }
    void setQuadModeEnabled(bool enabled);
    
    // Point 0 accessors
    double p0x() const { return m_p0x; }
    void setP0x(double x);
    double p0y() const { return m_p0y; }
    void setP0y(double y);
    double p0z() const { return m_p0z; }
    void setP0z(double z);
    
    // Point 1 accessors
    double p1x() const { return m_p1x; }
    void setP1x(double x);
    double p1y() const { return m_p1y; }
    void setP1y(double y);
    double p1z() const { return m_p1z; }
    void setP1z(double z);
    
    // Point 2 accessors
    double p2x() const { return m_p2x; }
    void setP2x(double x);
    double p2y() const { return m_p2y; }
    void setP2y(double y);
    double p2z() const { return m_p2z; }
    void setP2z(double z);
    
    // Point 3 accessors
    double p3x() const { return m_p3x; }
    void setP3x(double x);
    double p3y() const { return m_p3y; }
    void setP3y(double y);
    double p3z() const { return m_p3z; }
    void setP3z(double z);
    
    // UV Point 0 accessors
    double uvP0x() const { return m_uvP0x; }
    void setUvP0x(double x);
    double uvP0y() const { return m_uvP0y; }
    void setUvP0y(double y);
    
    // UV Point 1 accessors
    double uvP1x() const { return m_uvP1x; }
    void setUvP1x(double x);
    double uvP1y() const { return m_uvP1y; }
    void setUvP1y(double y);
    
    // UV Point 2 accessors
    double uvP2x() const { return m_uvP2x; }
    void setUvP2x(double x);
    double uvP2y() const { return m_uvP2y; }
    void setUvP2y(double y);
    
    // UV Point 3 accessors
    double uvP3x() const { return m_uvP3x; }
    void setUvP3x(double x);
    double uvP3y() const { return m_uvP3y; }
    void setUvP3y(double y);
    
Q_SIGNALS:
    void titleChanged(const QString& title);
    void xLabelChanged(const QString& label);
    void yLabelChanged(const QString& label);
    void zLabelChanged(const QString& label);
    void textureIdChanged(int id);
    void centerXChanged(double x);
    void centerYChanged(double y);
    void centerZChanged(double z);
    void axisUXChanged(double x);
    void axisUYChanged(double y);
    void axisUZChanged(double z);
    void axisVXChanged(double x);
    void axisVYChanged(double y);
    void axisVZChanged(double z);
    void uv0XChanged(double x);
    void uv0YChanged(double y);
    void uv1XChanged(double x);
    void uv1YChanged(double y);
    void tintColorChanged(const QColor& color);
    void quadModeEnabledChanged(bool enabled);
    void p0xChanged(double x);
    void p0yChanged(double y);
    void p0zChanged(double z);
    void p1xChanged(double x);
    void p1yChanged(double y);
    void p1zChanged(double z);
    void p2xChanged(double x);
    void p2yChanged(double y);
    void p2zChanged(double z);
    void p3xChanged(double x);
    void p3yChanged(double y);
    void p3zChanged(double z);
    void uvP0xChanged(double x);
    void uvP0yChanged(double y);
    void uvP1xChanged(double x);
    void uvP1yChanged(double y);
    void uvP2xChanged(double x);
    void uvP2yChanged(double y);
    void uvP3xChanged(double x);
    void uvP3yChanged(double y);
    
private:
    QString m_title = QStringLiteral("3D Image - Font Texture");
    QString m_xLabel = QStringLiteral("X");
    QString m_yLabel = QStringLiteral("Y");
    QString m_zLabel = QStringLiteral("Z");
    int m_textureId = 0;
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
    bool m_quadModeEnabled = false;
    double m_p0x = -0.5;
    double m_p0y = -0.5;
    double m_p0z = 0.0;
    double m_p1x = 0.5;
    double m_p1y = -0.5;
    double m_p1z = 0.0;
    double m_p2x = 0.5;
    double m_p2y = 0.5;
    double m_p2z = 0.0;
    double m_p3x = -0.5;
    double m_p3y = 0.5;
    double m_p3z = 0.0;
    double m_uvP0x = 0.0;
    double m_uvP0y = 0.0;
    double m_uvP1x = 1.0;
    double m_uvP1y = 0.0;
    double m_uvP2x = 1.0;
    double m_uvP2y = 1.0;
    double m_uvP3x = 0.0;
    double m_uvP3y = 1.0;
    
    QPointer<QIM::QImPlot3DNode> m_plot3DNode = nullptr;
    QPointer<QIM::QImPlot3DImageItemNode> m_image3DNode = nullptr;
};

#endif // PLOT3DIMAGEFUNCTION_H