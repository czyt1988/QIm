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
 * @class QImPlot3DImageItemNode
 * @ingroup plot3d
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
 * @class QImPlot3DImageItemNode
 * @ingroup plot3d
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

    /**
     * \if ENGLISH
     * @property QImPlot3DImageItemNode::textureId
     * @brief Texture identifier for the image
     *
     * @details Holds the ImTextureID value that references the image texture in the graphics backend.
     *          Must be a valid texture ID obtained from the rendering system.
     *          Default is 0 (null texture). Setting to 0 will result in no image being drawn.
     * @accessors READ textureId WRITE setTextureId NOTIFY textureIdChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlot3DImageItemNode::textureId
     * @brief 图像的纹理标识符
     *
     * @details 保存引用图形后端中图像纹理的 ImTextureID 值。
     *          必须是从渲染系统获得的有效纹理 ID。
     *          默认值为 0（空纹理）。设置为 0 将导致不绘制图像。
     * @accessors READ textureId WRITE setTextureId NOTIFY textureIdChanged
     * \endif
     */
    Q_PROPERTY(quintptr textureId READ textureId WRITE setTextureId NOTIFY textureIdChanged)

    /**
     * \if ENGLISH
     * @property QImPlot3DImageItemNode::centerX
     * @brief X coordinate of the image center in 3D plot coordinates
     * @details Default is 0.0.
     * @accessors READ centerX WRITE setCenterX NOTIFY centerChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlot3DImageItemNode::centerX
     * @brief 图像中心在3D绘图坐标中的X坐标
     * @details 默认值为0.0。
     * @accessors READ centerX WRITE setCenterX NOTIFY centerChanged
     * \endif
     */
    Q_PROPERTY(double centerX READ centerX WRITE setCenterX NOTIFY centerChanged)

    /**
     * \if ENGLISH
     * @property QImPlot3DImageItemNode::centerY
     * @brief Y coordinate of the image center in 3D plot coordinates
     * @details Default is 0.0.
     * @accessors READ centerY WRITE setCenterY NOTIFY centerChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlot3DImageItemNode::centerY
     * @brief 图像中心在3D绘图坐标中的Y坐标
     * @details 默认值为0.0。
     * @accessors READ centerY WRITE setCenterY NOTIFY centerChanged
     * \endif
     */
    Q_PROPERTY(double centerY READ centerY WRITE setCenterY NOTIFY centerChanged)

    /**
     * \if ENGLISH
     * @property QImPlot3DImageItemNode::centerZ
     * @brief Z coordinate of the image center in 3D plot coordinates
     * @details Default is 0.0.
     * @accessors READ centerZ WRITE setCenterZ NOTIFY centerChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlot3DImageItemNode::centerZ
     * @brief 图像中心在3D绘图坐标中的Z坐标
     * @details 默认值为0.0。
     * @accessors READ centerZ WRITE setCenterZ NOTIFY centerChanged
     * \endif
     */
    Q_PROPERTY(double centerZ READ centerZ WRITE setCenterZ NOTIFY centerChanged)

    /**
     * \if ENGLISH
     * @property QImPlot3DImageItemNode::axisUX
     * @brief X component of the U axis vector (first direction/half-extent)
     * @details Defines the direction and half-length of the U axis of the image rectangle.
     *          Default is 1.0.
     * @accessors READ axisUX WRITE setAxisUX NOTIFY axisUChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlot3DImageItemNode::axisUX
     * @brief U轴向量的X分量（第一个方向/半长度）
     * @details 定义图像矩形U轴的方向和半长度。默认值为1.0。
     * @accessors READ axisUX WRITE setAxisUX NOTIFY axisUChanged
     * \endif
     */
    Q_PROPERTY(double axisUX READ axisUX WRITE setAxisUX NOTIFY axisUChanged)

    /**
     * \if ENGLISH
     * @property QImPlot3DImageItemNode::axisUY
     * @brief Y component of the U axis vector (first direction/half-extent)
     * @details Defines the direction and half-length of the U axis of the image rectangle.
     *          Default is 0.0.
     * @accessors READ axisUY WRITE setAxisUY NOTIFY axisUChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlot3DImageItemNode::axisUY
     * @brief U轴向量的Y分量（第一个方向/半长度）
     * @details 定义图像矩形U轴的方向和半长度。默认值为0.0。
     * @accessors READ axisUY WRITE setAxisUY NOTIFY axisUChanged
     * \endif
     */
    Q_PROPERTY(double axisUY READ axisUY WRITE setAxisUY NOTIFY axisUChanged)

    /**
     * \if ENGLISH
     * @property QImPlot3DImageItemNode::axisUZ
     * @brief Z component of the U axis vector (first direction/half-extent)
     * @details Defines the direction and half-length of the U axis of the image rectangle.
     *          Default is 0.0.
     * @accessors READ axisUZ WRITE setAxisUZ NOTIFY axisUChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlot3DImageItemNode::axisUZ
     * @brief U轴向量的Z分量（第一个方向/半长度）
     * @details 定义图像矩形U轴的方向和半长度。默认值为0.0。
     * @accessors READ axisUZ WRITE setAxisUZ NOTIFY axisUChanged
     * \endif
     */
    Q_PROPERTY(double axisUZ READ axisUZ WRITE setAxisUZ NOTIFY axisUChanged)

    /**
     * \if ENGLISH
     * @property QImPlot3DImageItemNode::axisVX
     * @brief X component of the V axis vector (second direction/half-extent)
     * @details Defines the direction and half-length of the V axis of the image rectangle.
     *          Default is 0.0.
     * @accessors READ axisVX WRITE setAxisVX NOTIFY axisVChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlot3DImageItemNode::axisVX
     * @brief V轴向量的X分量（第二个方向/半长度）
     * @details 定义图像矩形V轴的方向和半长度。默认值为0.0。
     * @accessors READ axisVX WRITE setAxisVX NOTIFY axisVChanged
     * \endif
     */
    Q_PROPERTY(double axisVX READ axisVX WRITE setAxisVX NOTIFY axisVChanged)

    /**
     * \if ENGLISH
     * @property QImPlot3DImageItemNode::axisVY
     * @brief Y component of the V axis vector (second direction/half-extent)
     * @details Defines the direction and half-length of the V axis of the image rectangle.
     *          Default is 1.0.
     * @accessors READ axisVY WRITE setAxisVY NOTIFY axisVChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlot3DImageItemNode::axisVY
     * @brief V轴向量的Y分量（第二个方向/半长度）
     * @details 定义图像矩形V轴的方向和半长度。默认值为1.0。
     * @accessors READ axisVY WRITE setAxisVY NOTIFY axisVChanged
     * \endif
     */
    Q_PROPERTY(double axisVY READ axisVY WRITE setAxisVY NOTIFY axisVChanged)

    /**
     * \if ENGLISH
     * @property QImPlot3DImageItemNode::axisVZ
     * @brief Z component of the V axis vector (second direction/half-extent)
     * @details Defines the direction and half-length of the V axis of the image rectangle.
     *          Default is 0.0.
     * @accessors READ axisVZ WRITE setAxisVZ NOTIFY axisVChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlot3DImageItemNode::axisVZ
     * @brief V轴向量的Z分量（第二个方向/半长度）
     * @details 定义图像矩形V轴的方向和半长度。默认值为0.0。
     * @accessors READ axisVZ WRITE setAxisVZ NOTIFY axisVChanged
     * \endif
     */
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

protected:
    // Begins drawing the image
    virtual bool beginDraw() override;
};

}  // end namespace QIM

#endif  // QIMPLOT3DIMAGEITEMNODE_H
