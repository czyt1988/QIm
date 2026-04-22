#ifndef QIMPLOT3DIMAGEITEMNODE_H
#define QIMPLOT3DIMAGEITEMNODE_H

#include <QtCore/qglobal.h>
#include <QColor>
#include "QImAPI.h"
#include "QImPlot3DItemNode.h"

namespace QIM
{

/**
 * \if ENGLISH
 * @brief Qt-style wrapper for ImPlot3D 3D image visualization
 *
 * @details Provides Qt-style retained mode encapsulation for ImPlot3D 3D image rendering.
 *          Supports rendering an image texture in 3D space defined by a center point and
 *          two direction vectors (axes). Inherits from QImPlot3DItemNode and follows the
 *          same PIMPL design pattern as other 3D item nodes for consistency.
 *
 * @note Images are rendered as rectangles in 3D space with texture coordinates.
 *       Useful for overlaying textures, logos, or pre-rendered graphics in 3D plots.
 * @warning Texture ID must be a valid ImTextureID obtained from the rendering backend.
 *
 * @param[in] parent Parent QObject (optional)
 *
 * @see QImPlot3DItemNode
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot3D 3D图像可视化的Qt风格封装
 *
 * @details 为ImPlot3D 3D图像渲染提供Qt风格的保留模式封装。
 *          支持在由中心点和两个方向向量（轴）定义的3D空间中渲染图像纹理。
 *          继承自QImPlot3DItemNode，并遵循与其他3D元素节点相同的PIMPL设计模式以保持一致性。
 *
 * @note 图像在3D空间中渲染为带纹理坐标的矩形。
 *       适用于在3D图表上叠加纹理、徽标或预渲染图形。
 * @warning 纹理ID必须是来自渲染后端的有效ImTextureID。
 *
 * @param[in] parent 父QObject对象（可选）
 *
 * @see QImPlot3DItemNode
 * \endif
 */
class QIM_CORE_API QImPlot3DImageItemNode : public QImPlot3DItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlot3DImageItemNode)

    Q_PROPERTY(quintptr textureId READ textureId WRITE setTextureId NOTIFY textureIdChanged)

    Q_PROPERTY(double centerX READ centerX WRITE setCenterX NOTIFY centerChanged)

    Q_PROPERTY(double centerY READ centerY WRITE setCenterY NOTIFY centerChanged)

    Q_PROPERTY(double centerZ READ centerZ WRITE setCenterZ NOTIFY centerChanged)

    Q_PROPERTY(double axisUX READ axisUX WRITE setAxisUX NOTIFY axisUChanged)

    Q_PROPERTY(double axisUY READ axisUY WRITE setAxisUY NOTIFY axisUChanged)

    Q_PROPERTY(double axisUZ READ axisUZ WRITE setAxisUZ NOTIFY axisUChanged)

    Q_PROPERTY(double axisVX READ axisVX WRITE setAxisVX NOTIFY axisVChanged)

    Q_PROPERTY(double axisVY READ axisVY WRITE setAxisVY NOTIFY axisVChanged)

    Q_PROPERTY(double axisVZ READ axisVZ WRITE setAxisVZ NOTIFY axisVChanged)

    /**
     * \if ENGLISH
     * @property QImPlot3DImageItemNode::uv0X
     * @brief X component of the lower-left texture coordinate (UV0)
     * @details Defines the X texture coordinate for the lower-left corner of the image.
     *          Default is 0.0.
     * @accessors READ uv0X WRITE setUv0X NOTIFY uv0Changed
     * \endif
     *
     * \if CHINESE
     * @property QImPlot3DImageItemNode::uv0X
     * @brief 左下角纹理坐标(UV0)的X分量
     * @details 定义图像左下角的X纹理坐标。默认值为0.0。
     * @accessors READ uv0X WRITE setUv0X NOTIFY uv0Changed
     * \endif
     */
    Q_PROPERTY(double uv0X READ uv0X WRITE setUv0X NOTIFY uv0Changed)

    /**
     * \if ENGLISH
     * @property QImPlot3DImageItemNode::uv0Y
     * @brief Y component of the lower-left texture coordinate (UV0)
     * @details Defines the Y texture coordinate for the lower-left corner of the image.
     *          Default is 0.0.
     * @accessors READ uv0Y WRITE setUv0Y NOTIFY uv0Changed
     * \endif
     *
     * \if CHINESE
     * @property QImPlot3DImageItemNode::uv0Y
     * @brief 左下角纹理坐标(UV0)的Y分量
     * @details 定义图像左下角的Y纹理坐标。默认值为0.0。
     * @accessors READ uv0Y WRITE setUv0Y NOTIFY uv0Changed
     * \endif
     */
    Q_PROPERTY(double uv0Y READ uv0Y WRITE setUv0Y NOTIFY uv0Changed)

    /**
     * \if ENGLISH
     * @property QImPlot3DImageItemNode::uv1X
     * @brief X component of the upper-right texture coordinate (UV1)
     * @details Defines the X texture coordinate for the upper-right corner of the image.
     *          Default is 1.0.
     * @accessors READ uv1X WRITE setUv1X NOTIFY uv1Changed
     * \endif
     *
     * \if CHINESE
     * @property QImPlot3DImageItemNode::uv1X
     * @brief 右上角纹理坐标(UV1)的X分量
     * @details 定义图像右上角的X纹理坐标。默认值为1.0。
     * @accessors READ uv1X WRITE setUv1X NOTIFY uv1Changed
     * \endif
     */
    Q_PROPERTY(double uv1X READ uv1X WRITE setUv1X NOTIFY uv1Changed)

    /**
     * \if ENGLISH
     * @property QImPlot3DImageItemNode::uv1Y
     * @brief Y component of the upper-right texture coordinate (UV1)
     * @details Defines the Y texture coordinate for the upper-right corner of the image.
     *          Default is 1.0.
     * @accessors READ uv1Y WRITE setUv1Y NOTIFY uv1Changed
     * \endif
     *
     * \if CHINESE
     * @property QImPlot3DImageItemNode::uv1Y
     * @brief 右上角纹理坐标(UV1)的Y分量
     * @details 定义图像右上角的Y纹理坐标。默认值为1.0。
     * @accessors READ uv1Y WRITE setUv1Y NOTIFY uv1Changed
     * \endif
     */
    Q_PROPERTY(double uv1Y READ uv1Y WRITE setUv1Y NOTIFY uv1Changed)

    /**
     * \if ENGLISH
     * @property QImPlot3DImageItemNode::tintColor
     * @brief Tint color applied to the image
     *
     * @details Defines the color multiplier applied to the image texture.
     *          Default is white (1,1,1,1) which renders the image unchanged.
     *          Use alpha component to control transparency.
     * @accessors READ tintColor WRITE setTintColor NOTIFY tintColorChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlot3DImageItemNode::tintColor
     * @brief 应用于图像的色调颜色
     *
     * @details 定义应用于图像纹理的颜色乘数。
     *          默认为白色 (1,1,1,1)，图像保持不变。
     *          使用 alpha 分量控制透明度。
     * @accessors READ tintColor WRITE setTintColor NOTIFY tintColorChanged
     * \endif
     */
    Q_PROPERTY(QColor tintColor READ tintColor WRITE setTintColor NOTIFY tintColorChanged)

    Q_PROPERTY(bool quadModeEnabled READ quadModeEnabled WRITE setQuadModeEnabled NOTIFY quadModeEnabledChanged)

    Q_PROPERTY(double p0x READ p0x WRITE setP0x NOTIFY p0Changed)
    Q_PROPERTY(double p0y READ p0y WRITE setP0y NOTIFY p0Changed)
    Q_PROPERTY(double p0z READ p0z WRITE setP0z NOTIFY p0Changed)
    Q_PROPERTY(double p1x READ p1x WRITE setP1x NOTIFY p1Changed)
    Q_PROPERTY(double p1y READ p1y WRITE setP1y NOTIFY p1Changed)
    Q_PROPERTY(double p1z READ p1z WRITE setP1z NOTIFY p1Changed)
    Q_PROPERTY(double p2x READ p2x WRITE setP2x NOTIFY p2Changed)
    Q_PROPERTY(double p2y READ p2y WRITE setP2y NOTIFY p2Changed)
    Q_PROPERTY(double p2z READ p2z WRITE setP2z NOTIFY p2Changed)
    Q_PROPERTY(double p3x READ p3x WRITE setP3x NOTIFY p3Changed)
    Q_PROPERTY(double p3y READ p3y WRITE setP3y NOTIFY p3Changed)
    Q_PROPERTY(double p3z READ p3z WRITE setP3z NOTIFY p3Changed)

    Q_PROPERTY(double uvP0x READ uvP0x WRITE setUvP0x NOTIFY uvP0Changed)
    Q_PROPERTY(double uvP0y READ uvP0y WRITE setUvP0y NOTIFY uvP0Changed)
    Q_PROPERTY(double uvP1x READ uvP1x WRITE setUvP1x NOTIFY uvP1Changed)
    Q_PROPERTY(double uvP1y READ uvP1y WRITE setUvP1y NOTIFY uvP1Changed)
    Q_PROPERTY(double uvP2x READ uvP2x WRITE setUvP2x NOTIFY uvP2Changed)
    Q_PROPERTY(double uvP2y READ uvP2y WRITE setUvP2y NOTIFY uvP2Changed)
    Q_PROPERTY(double uvP3x READ uvP3x WRITE setUvP3x NOTIFY uvP3Changed)
    Q_PROPERTY(double uvP3y READ uvP3y WRITE setUvP3y NOTIFY uvP3Changed)

public:
    // Image item type = InnerType3D + 7
    enum
    {
        Type = InnerType3D + 7
    };

    // Returns the type identifier of this plot item
    virtual int type() const override
    {
        return Type;
    }

    // Constructs a QImPlot3DImageItemNode with optional parent
    explicit QImPlot3DImageItemNode(QObject* parent = nullptr);

    // Destroys the QImPlot3DImageItemNode
    ~QImPlot3DImageItemNode() override;

    //----------------------------------------------------
    // Property accessors
    //----------------------------------------------------

    // Texture ID
    quintptr textureId() const;
    void setTextureId(quintptr id);

    // Center coordinates
    double centerX() const;
    void setCenterX(double x);
    double centerY() const;
    void setCenterY(double y);
    double centerZ() const;
    void setCenterZ(double z);

    // U axis components
    double axisUX() const;
    void setAxisUX(double x);
    double axisUY() const;
    void setAxisUY(double y);
    double axisUZ() const;
    void setAxisUZ(double z);

    // V axis components
    double axisVX() const;
    void setAxisVX(double x);
    double axisVY() const;
    void setAxisVY(double y);
    double axisVZ() const;
    void setAxisVZ(double z);

    // UV0 coordinates
    double uv0X() const;
    void setUv0X(double x);
    double uv0Y() const;
    void setUv0Y(double y);

    // UV1 coordinates
    double uv1X() const;
    void setUv1X(double x);
    double uv1Y() const;
    void setUv1Y(double y);

    // Tint color
    QColor tintColor() const;
    void setTintColor(const QColor& color);

    // Raw ImPlot3DImageFlags access
    int imageFlags() const;
    void setImageFlags(int flags);

    // Quad mode toggle
    bool quadModeEnabled() const;
    void setQuadModeEnabled(bool enabled);

    // Quad corner point 0
    double p0x() const;
    void setP0x(double x);
    double p0y() const;
    void setP0y(double y);
    double p0z() const;
    void setP0z(double z);

    // Quad corner point 1
    double p1x() const;
    void setP1x(double x);
    double p1y() const;
    void setP1y(double y);
    double p1z() const;
    void setP1z(double z);

    // Quad corner point 2
    double p2x() const;
    void setP2x(double x);
    double p2y() const;
    void setP2y(double y);
    double p2z() const;
    void setP2z(double z);

    // Quad corner point 3
    double p3x() const;
    void setP3x(double x);
    double p3y() const;
    void setP3y(double y);
    double p3z() const;
    void setP3z(double z);

    // Quad per-corner UV 0
    double uvP0x() const;
    void setUvP0x(double x);
    double uvP0y() const;
    void setUvP0y(double y);

    // Quad per-corner UV 1
    double uvP1x() const;
    void setUvP1x(double x);
    double uvP1y() const;
    void setUvP1y(double y);

    // Quad per-corner UV 2
    double uvP2x() const;
    void setUvP2x(double x);
    double uvP2y() const;
    void setUvP2y(double y);

    // Quad per-corner UV 3
    double uvP3x() const;
    void setUvP3x(double x);
    double uvP3y() const;
    void setUvP3y(double y);

    // Convenience: set all quad image parameters at once
    void setQuadImage(quintptr textureId, double p0x, double p0y, double p0z, double p1x, double p1y, double p1z,
                      double p2x, double p2y, double p2z, double p3x, double p3y, double p3z,
                      double uvP0x, double uvP0y, double uvP1x, double uvP1y, double uvP2x, double uvP2y,
                      double uvP3x, double uvP3y, const QColor& tintCol = QColor(255, 255, 255));

Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when texture ID changes
     * @param[in] id New texture ID value
     * \endif
     *
     * \if CHINESE
     * @brief 纹理ID更改时触发
     * @param[in] id 新的纹理ID值
     * \endif
     */
    void textureIdChanged(quintptr id);

    /**
     * \if ENGLISH
     * @brief Emitted when any center coordinate changes
     * @param[in] x New center X coordinate
     * @param[in] y New center Y coordinate
     * @param[in] z New center Z coordinate
     * \endif
     *
     * \if CHINESE
     * @brief 任意中心坐标更改时触发
     * @param[in] x 新的中心X坐标
     * @param[in] y 新的中心Y坐标
     * @param[in] z 新的中心Z坐标
     * \endif
     */
    void centerChanged(double x, double y, double z);

    /**
     * \if ENGLISH
     * @brief Emitted when any U axis component changes
     * @param[in] x New U axis X component
     * @param[in] y New U axis Y component
     * @param[in] z New U axis Z component
     * \endif
     *
     * \if CHINESE
     * @brief 任意U轴分量更改时触发
     * @param[in] x 新的U轴X分量
     * @param[in] y 新的U轴Y分量
     * @param[in] z 新的U轴Z分量
     * \endif
     */
    void axisUChanged(double x, double y, double z);

    /**
     * \if ENGLISH
     * @brief Emitted when any V axis component changes
     * @param[in] x New V axis X component
     * @param[in] y New V axis Y component
     * @param[in] z New V axis Z component
     * \endif
     *
     * \if CHINESE
     * @brief 任意V轴分量更改时触发
     * @param[in] x 新的V轴X分量
     * @param[in] y 新的V轴Y分量
     * @param[in] z 新的V轴Z分量
     * \endif
     */
    void axisVChanged(double x, double y, double z);

    /**
     * \if ENGLISH
     * @brief Emitted when any UV0 coordinate changes
     * @param[in] x New UV0 X coordinate
     * @param[in] y New UV0 Y coordinate
     * \endif
     *
     * \if CHINESE
     * @brief 任意UV0坐标更改时触发
     * @param[in] x 新的UV0 X坐标
     * @param[in] y 新的UV0 Y坐标
     * \endif
     */
    void uv0Changed(double x, double y);

    /**
     * \if ENGLISH
     * @brief Emitted when any UV1 coordinate changes
     * @param[in] x New UV1 X coordinate
     * @param[in] y New UV1 Y coordinate
     * \endif
     *
     * \if CHINESE
     * @brief 任意UV1坐标更改时触发
     * @param[in] x 新的UV1 X坐标
     * @param[in] y 新的UV1 Y坐标
     * \endif
     */
    void uv1Changed(double x, double y);

    /**
     * \if ENGLISH
     * @brief Emitted when tint color changes
     * @param[in] color New tint color
     * \endif
     *
     * \if CHINESE
     * @brief 色调颜色更改时触发
     * @param[in] color 新的色调颜色
     * \endif
     */
    void tintColorChanged(const QColor& color);

    /**
     * \if ENGLISH
     * @brief Emitted when any image flag changes
     * \endif
     *
     * \if CHINESE
     * @brief 任意图像标志更改时触发
     * \endif
     */
    void imageFlagChanged();

    /**
     * \if ENGLISH
     * @brief Emitted when quad mode enabled state changes
     * @param[in] enabled New quad mode enabled state
     * \endif
     *
     * \if CHINESE
     * @brief 四边形模式启用状态更改时触发
     * @param[in] enabled 新的四边形模式启用状态
     * \endif
     */
    void quadModeEnabledChanged(bool enabled);

    /**
     * \if ENGLISH
     * @brief Emitted when any corner point 0 coordinate changes
     * @param[in] x New P0 X coordinate
     * @param[in] y New P0 Y coordinate
     * @param[in] z New P0 Z coordinate
     * \endif
     *
     * \if CHINESE
     * @brief 任意角点0坐标更改时触发
     * @param[in] x 新的P0 X坐标
     * @param[in] y 新的P0 Y坐标
     * @param[in] z 新的P0 Z坐标
     * \endif
     */
    void p0Changed(double x, double y, double z);

    /**
     * \if ENGLISH
     * @brief Emitted when any corner point 1 coordinate changes
     * @param[in] x New P1 X coordinate
     * @param[in] y New P1 Y coordinate
     * @param[in] z New P1 Z coordinate
     * \endif
     *
     * \if CHINESE
     * @brief 任意角点1坐标更改时触发
     * @param[in] x 新的P1 X坐标
     * @param[in] y 新的P1 Y坐标
     * @param[in] z 新的P1 Z坐标
     * \endif
     */
    void p1Changed(double x, double y, double z);

    /**
     * \if ENGLISH
     * @brief Emitted when any corner point 2 coordinate changes
     * @param[in] x New P2 X coordinate
     * @param[in] y New P2 Y coordinate
     * @param[in] z New P2 Z coordinate
     * \endif
     *
     * \if CHINESE
     * @brief 任意角点2坐标更改时触发
     * @param[in] x 新的P2 X坐标
     * @param[in] y 新的P2 Y坐标
     * @param[in] z 新的P2 Z坐标
     * \endif
     */
    void p2Changed(double x, double y, double z);

    /**
     * \if ENGLISH
     * @brief Emitted when any corner point 3 coordinate changes
     * @param[in] x New P3 X coordinate
     * @param[in] y New P3 Y coordinate
     * @param[in] z New P3 Z coordinate
     * \endif
     *
     * \if CHINESE
     * @brief 任意角点3坐标更改时触发
     * @param[in] x 新的P3 X坐标
     * @param[in] y 新的P3 Y坐标
     * @param[in] z 新的P3 Z坐标
     * \endif
     */
    void p3Changed(double x, double y, double z);

    /**
     * \if ENGLISH
     * @brief Emitted when any UV point 0 coordinate changes
     * @param[in] x New UV P0 X coordinate
     * @param[in] y New UV P0 Y coordinate
     * \endif
     *
     * \if CHINESE
     * @brief 任意UV点0坐标更改时触发
     * @param[in] x 新的UV P0 X坐标
     * @param[in] y 新的UV P0 Y坐标
     * \endif
     */
    void uvP0Changed(double x, double y);

    /**
     * \if ENGLISH
     * @brief Emitted when any UV point 1 coordinate changes
     * @param[in] x New UV P1 X coordinate
     * @param[in] y New UV P1 Y coordinate
     * \endif
     *
     * \if CHINESE
     * @brief 任意UV点1坐标更改时触发
     * @param[in] x 新的UV P1 X坐标
     * @param[in] y 新的UV P1 Y坐标
     * \endif
     */
    void uvP1Changed(double x, double y);

    /**
     * \if ENGLISH
     * @brief Emitted when any UV point 2 coordinate changes
     * @param[in] x New UV P2 X coordinate
     * @param[in] y New UV P2 Y coordinate
     * \endif
     *
     * \if CHINESE
     * @brief 任意UV点2坐标更改时触发
     * @param[in] x 新的UV P2 X坐标
     * @param[in] y 新的UV P2 Y坐标
     * \endif
     */
    void uvP2Changed(double x, double y);

    /**
     * \if ENGLISH
     * @brief Emitted when any UV point 3 coordinate changes
     * @param[in] x New UV P3 X coordinate
     * @param[in] y New UV P3 Y coordinate
     * \endif
     *
     * \if CHINESE
     * @brief 任意UV点3坐标更改时触发
     * @param[in] x 新的UV P3 X坐标
     * @param[in] y 新的UV P3 Y坐标
     * \endif
     */
    void uvP3Changed(double x, double y);

protected:
    // Begins drawing the image
    virtual bool beginDraw() override;
};

}  // end namespace QIM

#endif  // QIMPLOT3DIMAGEITEMNODE_H
