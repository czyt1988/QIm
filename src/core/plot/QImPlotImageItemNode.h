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
 * @details Provides Qt-style retained mode encapsulation for ImPlot image rendering.
 *          Supports rendering an image texture within specified plot coordinate bounds.
 *          Inherits from QImPlotItemNode and follows the same PIMPL design pattern
 *          as QImPlotHeatmapItemNode for consistency.
 *
 * @note Images are rendered as axis-aligned rectangles with texture coordinates.
 *       Useful for overlaying icons, logos, or pre-rendered graphics on plots.
 * @note Texture ID must be a valid ImTextureID obtained from the rendering backend.
 *
 * @see QImPlotItemNode
 * @see QImPlotHeatmapItemNode
 * @see QImPlotNode
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot图像可视化的Qt风格封装
 *
 * @details 为ImPlot图像渲染提供Qt风格的保留模式封装�?
 *          支持在指定的绘图坐标边界内渲染图像纹理�?
 *          继承自QImPlotItemNode，并遵循与QImPlotHeatmapItemNode相同的PIMPL设计模式以保持一致性�?
 *
 * @note 图像作为轴对齐矩形渲染，具有纹理坐标�?
 *       适用于在图表上叠加图标、徽标或预渲染图形�?
 * @note 纹理ID必须是来自渲染后端的有效ImTextureID�?
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

    Q_PROPERTY(quintptr textureId READ textureId WRITE setTextureId NOTIFY textureIdChanged)

    Q_PROPERTY(QPointF boundsMin READ boundsMin WRITE setBoundsMin NOTIFY boundsMinChanged)

    Q_PROPERTY(QPointF boundsMax READ boundsMax WRITE setBoundsMax NOTIFY boundsMaxChanged)

    Q_PROPERTY(QPointF uv0 READ uv0 WRITE setUv0 NOTIFY uv0Changed)

    Q_PROPERTY(QPointF uv1 READ uv1 WRITE setUv1 NOTIFY uv1Changed)

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
     * @brief 纹理ID更改时触�?
     * @param[in] id 新的纹理ID�?
     * @details 当值实际更改时由setTextureId()触发�?
     *          连接到更新UI元素或执行相关操作�?
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
     * @param[in] min 新的左下角边界位�?
     * @details 当值实际更改时由setBoundsMin()触发�?
     *          连接到更新UI元素或执行相关操作�?
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
     * @param[in] max 新的右上角边界位�?
     * @details 当值实际更改时由setBoundsMax()触发�?
     *          连接到更新UI元素或执行相关操作�?
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
     * @details 当值实际更改时由setUv0()触发�?
     *          连接到更新UI元素或执行相关操作�?
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
     * @details 当值实际更改时由setUv1()触发�?
     *          连接到更新UI元素或执行相关操作�?
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
     * @brief 色调颜色更改时触�?
     * @param[in] color 新的色调颜色
     * @details 当值实际更改时由setTintColor()触发�?
     *          连接到更新UI元素或执行相关操作�?
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
     * @brief 图像标志更改时触�?
     * @details 任何标志属性更改时触发�?
     *          连接到更新UI元素或执行相关操作�?
     * \endif
     */
    void imageFlagChanged();

protected:
    // Begins drawing the image
    virtual bool beginDraw() override;
};

}  // end namespace QIM

#endif  // QIMPLOTIMAGEITEMNODE_H