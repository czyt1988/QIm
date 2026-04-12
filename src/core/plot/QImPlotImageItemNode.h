#ifndef QIMPLOTIMAGEITEMNODE_H
#define QIMPLOTIMAGEITEMNODE_H

#include <QtCore/qglobal.h>
#include <QColor>
#include <QPointF>
#include "QImAPI.h"
#include "QImPlotItemNode.h"
#include "QtImGuiUtils.h"

namespace QIM
{

/**
 * \if ENGLISH
 * @brief Qt-style wrapper for ImPlot image visualization
 *
 * @class QImPlotImageItemNode
 * @ingroup plot_items
 *
 * @details Provides Qt-style retained mode encapsulation for ImPlot image rendering.
 *          Supports rendering an image texture within specified plot coordinate bounds.
 *          Inherits from QImPlotItemNode and follows the same PIMPL design pattern
 *          as QImPlotHeatmapItemNode for consistency.
 *
 * @note Images are rendered as axis-aligned rectangles with texture coordinates.
 *       Useful for overlaying icons, logos, or pre-rendered graphics on plots.
 * @warning Texture ID must be a valid ImTextureID obtained from the rendering backend.
 *
 * @param[in] parent Parent QObject (optional)
 *
 * @see QImPlotItemNode
 * @see QImPlotHeatmapItemNode
 * @see QImPlotNode
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot图像可视化的Qt风格封装
 *
 * @class QImPlotImageItemNode
 * @ingroup plot_items
 *
 * @details 为ImPlot图像渲染提供Qt风格的保留模式封装。
 *          支持在指定的绘图坐标边界内渲染图像纹理。
 *          继承自QImPlotItemNode，并遵循与QImPlotHeatmapItemNode相同的PIMPL设计模式以保持一致性。
 *
 * @note 图像作为轴对齐矩形渲染，具有纹理坐标。
 *       适用于在图表上叠加图标、徽标或预渲染图形。
 * @warning 纹理ID必须是来自渲染后端的有效ImTextureID。
 *
 * @param[in] parent 父QObject对象（可选）
 *
 * @see QImPlotItemNode
 * @see QImPlotHeatmapItemNode
 * @see QImPlotNode
 * \endif
 */
class QIM_CORE_API QImPlotImageItemNode : public QImPlotItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotImageItemNode)

    /**
     * \if ENGLISH
     * @property QImPlotImageItemNode::textureId
     * @brief Texture identifier for the image
     *
     * @details Holds the ImTextureID value that references the image texture in the graphics backend.
     *          Must be a valid texture ID obtained from the rendering system (e.g., via ImGui image loading).
     *          Default is 0 (null texture). Setting to 0 will result in no image being drawn.
     * @accessors READ textureId WRITE setTextureId NOTIFY textureIdChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotImageItemNode::textureId
     * @brief 图像的纹理标识符
     *
     * @details 保存引用图形后端中图像纹理的 ImTextureID 值。
     *          必须是从渲染系统获得的有效纹理 ID（例如通过 ImGui 图像加载）。
     *          默认值为 0（空纹理）。设置为 0 将导致不绘制图像。
     * @accessors READ textureId WRITE setTextureId NOTIFY textureIdChanged
     * \endif
     */
    Q_PROPERTY(quintptr textureId READ textureId WRITE setTextureId NOTIFY textureIdChanged)

    /**
     * \if ENGLISH
     * @property QImPlotImageItemNode::boundsMin
     * @brief Lower-left corner position of image in plot coordinates
     *
     * @details Defines the plot coordinates of the lower-left corner
     *          of the image rectangle. Default is (0, 0).
     * @accessors READ boundsMin WRITE setBoundsMin NOTIFY boundsMinChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotImageItemNode::boundsMin
     * @brief 图像左下角在绘图坐标中的位置
     *
     * @details 定义图像矩形左下角的绘图坐标。
     *          默认为 (0, 0)。
     * @accessors READ boundsMin WRITE setBoundsMin NOTIFY boundsMinChanged
     * \endif
     */
    Q_PROPERTY(QPointF boundsMin READ boundsMin WRITE setBoundsMin NOTIFY boundsMinChanged)

    /**
     * \if ENGLISH
     * @property QImPlotImageItemNode::boundsMax
     * @brief Upper-right corner position of image in plot coordinates
     *
     * @details Defines the plot coordinates of the upper-right corner
     *          of the image rectangle. Default is (10, 10).
     * @accessors READ boundsMax WRITE setBoundsMax NOTIFY boundsMaxChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotImageItemNode::boundsMax
     * @brief 图像右上角在绘图坐标中的位置
     *
     * @details 定义图像矩形右上角的绘图坐标。
     *          默认为 (10, 10)。
     * @accessors READ boundsMax WRITE setBoundsMax NOTIFY boundsMaxChanged
     * \endif
     */
    Q_PROPERTY(QPointF boundsMax READ boundsMax WRITE setBoundsMax NOTIFY boundsMaxChanged)

    /**
     * \if ENGLISH
     * @property QImPlotImageItemNode::uv0
     * @brief Lower-left texture coordinate (UV0)
     *
     * @details Defines the texture coordinate for the lower-left corner of the image.
     *          Default is (0, 0) representing the top-left corner of the texture
     *          (ImGui texture coordinates: y-down).
     * @accessors READ uv0 WRITE setUv0 NOTIFY uv0Changed
     * \endif
     *
     * \if CHINESE
     * @property QImPlotImageItemNode::uv0
     * @brief 左下角纹理坐标 (UV0)
     *
     * @details 定义图像左下角的纹理坐标。
     *          默认为 (0, 0)，表示纹理的左上角（ImGui 纹理坐标：y 向下）。
     * @accessors READ uv0 WRITE setUv0 NOTIFY uv0Changed
     * \endif
     */
    Q_PROPERTY(QPointF uv0 READ uv0 WRITE setUv0 NOTIFY uv0Changed)

    /**
     * \if ENGLISH
     * @property QImPlotImageItemNode::uv1
     * @brief Upper-right texture coordinate (UV1)
     *
     * @details Defines the texture coordinate for the upper-right corner of the image.
     *          Default is (1, 1) representing the bottom-right corner of the texture
     *          (ImGui texture coordinates: y-down).
     * @accessors READ uv1 WRITE setUv1 NOTIFY uv1Changed
     * \endif
     *
     * \if CHINESE
     * @property QImPlotImageItemNode::uv1
     * @brief 右上角纹理坐标 (UV1)
     *
     * @details 定义图像右上角的纹理坐标。
     *          默认为 (1, 1)，表示纹理的右下角（ImGui 纹理坐标：y 向下）。
     * @accessors READ uv1 WRITE setUv1 NOTIFY uv1Changed
     * \endif
     */
    Q_PROPERTY(QPointF uv1 READ uv1 WRITE setUv1 NOTIFY uv1Changed)

    /**
     * \if ENGLISH
     * @property QImPlotImageItemNode::tintColor
     * @brief Tint color applied to the image
     *
     * @details Defines the color multiplier applied to the image texture.
     *          Default is white (1,1,1,1) which renders the image unchanged.
     *          Use alpha component to control transparency.
     * @accessors READ tintColor WRITE setTintColor NOTIFY tintColorChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotImageItemNode::tintColor
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
    // Unique type identifier for QImPlotImageItemNode
    enum
    {
        Type = InnerType + 13   // Ensure unique, adjust if needed
    };

    // Returns the type identifier of this plot item
    virtual int type() const override
    {
        return Type;
    }

    // Constructs a QImPlotImageItemNode with optional parent
    QImPlotImageItemNode(QObject* parent = nullptr);

    // Destroys the QImPlotImageItemNode
    ~QImPlotImageItemNode();

    //----------------------------------------------------
    // Property accessors
    //----------------------------------------------------

    // Texture ID
    quintptr textureId() const;
    void setTextureId(quintptr id);

    // Bounds minimum
    QPointF boundsMin() const;
    void setBoundsMin(const QPointF& min);

    // Bounds maximum
    QPointF boundsMax() const;
    void setBoundsMax(const QPointF& max);

    // Texture coordinate UV0
    QPointF uv0() const;
    void setUv0(const QPointF& uv);

    // Texture coordinate UV1
    QPointF uv1() const;
    void setUv1(const QPointF& uv);

    // Tint color
    QColor tintColor() const;
    void setTintColor(const QColor& color);

    // Raw ImPlotImageFlags access
    int imageFlags() const;
    void setImageFlags(int flags);

Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when texture ID changes
     * @param[in] id New texture ID value
     * @details Triggered by setTextureId() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 纹理ID更改时触发
     * @param[in] id 新的纹理ID值
     * @details 当值实际更改时由setTextureId()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void textureIdChanged(quintptr id);

    /**
     * \if ENGLISH
     * @brief Emitted when lower-left bounds change
     * @param[in] min New lower-left bounds position
     * @details Triggered by setBoundsMin() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 左下角边界更改时触发
     * @param[in] min 新的左下角边界位置
     * @details 当值实际更改时由setBoundsMin()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void boundsMinChanged(const QPointF& min);

    /**
     * \if ENGLISH
     * @brief Emitted when upper-right bounds change
     * @param[in] max New upper-right bounds position
     * @details Triggered by setBoundsMax() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 右上角边界更改时触发
     * @param[in] max 新的右上角边界位置
     * @details 当值实际更改时由setBoundsMax()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void boundsMaxChanged(const QPointF& max);

    /**
     * \if ENGLISH
     * @brief Emitted when lower-left texture coordinate changes
     * @param[in] uv New UV0 coordinate
     * @details Triggered by setUv0() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 左下角纹理坐标更改时触发
     * @param[in] uv 新的UV0坐标
     * @details 当值实际更改时由setUv0()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void uv0Changed(const QPointF& uv);

    /**
     * \if ENGLISH
     * @brief Emitted when upper-right texture coordinate changes
     * @param[in] uv New UV1 coordinate
     * @details Triggered by setUv1() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 右上角纹理坐标更改时触发
     * @param[in] uv 新的UV1坐标
     * @details 当值实际更改时由setUv1()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void uv1Changed(const QPointF& uv);

    /**
     * \if ENGLISH
     * @brief Emitted when tint color changes
     * @param[in] color New tint color
     * @details Triggered by setTintColor() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 色调颜色更改时触发
     * @param[in] color 新的色调颜色
     * @details 当值实际更改时由setTintColor()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void tintColorChanged(const QColor& color);

    /**
     * \if ENGLISH
     * @brief Emitted when image flags change
     * @details Triggered by any flag property change.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 图像标志更改时触发
     * @details 任何标志属性更改时触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void imageFlagChanged();

protected:
    // Begins drawing the image
    virtual bool beginDraw() override;
};

}  // end namespace QIM

#endif  // QIMPLOTIMAGEITEMNODE_H