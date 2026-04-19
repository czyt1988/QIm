#ifndef QIMPLOT3DTEXTITEMNODE_H
#define QIMPLOT3DTEXTITEMNODE_H

#include "QImPlot3DItemNode.h"
#include <QColor>
#include <QString>

namespace QIM
{

/**
 * \if ENGLISH
 * @brief Qt-style wrapper for ImPlot3D text label visualization
 *
 * @class QImPlot3DTextItemNode
 * @ingroup plot3d_items
 *
 * @details Provides Qt-style retained mode encapsulation for ImPlot3D text labels.
 *          Text labels render centered text at specified 3D plot coordinates with optional
 *          rotation angle and pixel offset. Useful for annotating specific points in 3D space,
 *          marking data features, or adding descriptive labels to 3D plots.
 *          Inherits from QImPlot3DItemNode and follows the same PIMPL design pattern
 *          as other plot item nodes for consistency.
 *
 * @note Text labels are rendered at the specified 3D plot position (x, y, z) and can be
 *       rotated by an angle (in degrees) and offset in pixel space for fine positioning.
 *       Unlike the 2D version, this uses angle (degrees) instead of vertical boolean.
 *
 * @param[in] parent Parent QObject (optional)
 *
 * @see QImPlot3DItemNode
 * @see QImPlotTextItemNode
 * @see ImPlot3D::PlotText
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot3D 文本标签的 Qt 风格封装
 *
 * @class QImPlot3DTextItemNode
 * @ingroup plot3d_items
 *
 * @details 为 ImPlot3D 文本标签提供 Qt 风格的保留模式封装。
 *          文本标签在指定的 3D 绘图坐标处渲染居中文本，可选旋转角度和像素偏移。
 *          适用于在 3D 空间中标注特定点、标记数据特征或为 3D 绘图添加描述性标签。
 *          继承自 QImPlot3DItemNode，并遵循与其他绘图项节点相同的 PIMPL 设计模式以保持一致性。
 *
 * @note 文本标签在指定的 3D 绘图位置 (x, y, z) 处渲染，并可以按角度（度）旋转并在像素空间中进行偏移
 *       以进行精细定位调整。与 2D 版本不同，此版本使用角度（度）而非垂直布尔值。
 *
 * @param[in] parent 父 QObject 对象（可选）
 *
 * @see QImPlot3DItemNode
 * @see QImPlotTextItemNode
 * @see ImPlot3D::PlotText
 * \endif
 */
class QIM_CORE_API QImPlot3DTextItemNode : public QImPlot3DItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlot3DTextItemNode)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(double x READ x WRITE setX NOTIFY positionChanged)
    Q_PROPERTY(double y READ y WRITE setY NOTIFY positionChanged)
    Q_PROPERTY(double z READ z WRITE setZ NOTIFY positionChanged)
    Q_PROPERTY(double angle READ angle WRITE setAngle NOTIFY angleChanged)
    Q_PROPERTY(double pixelOffsetX READ pixelOffsetX WRITE setPixelOffsetX NOTIFY pixelOffsetChanged)
    Q_PROPERTY(double pixelOffsetY READ pixelOffsetY WRITE setPixelOffsetY NOTIFY pixelOffsetChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    // Unique type identifier for QImPlot3DTextItemNode
    enum
    {
        Type = InnerType3D + 8
    };

    // Returns the type identifier of this plot item
    virtual int type() const override
    {
        return Type;
    }

    // Constructs a QImPlot3DTextItemNode with optional parent
    explicit QImPlot3DTextItemNode(QObject* parent = nullptr);

    // Destroys the QImPlot3DTextItemNode
    ~QImPlot3DTextItemNode() override;

    //----------------------------------------------------
    // Text and position interface
    //----------------------------------------------------

    // Gets the text content
    QString text() const;

    // Sets the text content
    void setText(const QString& text);

    // Gets the X coordinate in plot space
    double x() const;

    // Sets the X coordinate in plot space
    void setX(double x);

    // Gets the Y coordinate in plot space
    double y() const;

    // Sets the Y coordinate in plot space
    void setY(double y);

    // Gets the Z coordinate in plot space
    double z() const;

    // Sets the Z coordinate in plot space
    void setZ(double z);

    // Sets the 3D position (convenience overload)
    void setPosition(double x, double y, double z);

    // Gets the rotation angle in degrees
    double angle() const;

    // Sets the rotation angle in degrees
    void setAngle(double angleDeg);

    // Gets the pixel offset X
    double pixelOffsetX() const;

    // Sets the pixel offset X
    void setPixelOffsetX(double offset);

    // Gets the pixel offset Y
    double pixelOffsetY() const;

    // Sets the pixel offset Y
    void setPixelOffsetY(double offset);

    // Sets the pixel offset (convenience overload)
    void setPixelOffset(double offsetX, double offsetY);

    //----------------------------------------------------
    // Style property accessors
    //----------------------------------------------------

    // Gets the text color
    QColor color() const;

    // Sets the text color
    void setColor(const QColor& color);

Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when text content changes
     * @param[in] text New text content
     * @details Triggered by setText() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 文本内容更改时触发
     * @param[in] text 新文本内容
     * @details 当值实际更改时由 setText() 触发。
     *          连接到更新 UI 元素或执行相关操作。
     * \endif
     */
    void textChanged(const QString& text);

    /**
     * \if ENGLISH
     * @brief Emitted when position changes
     * @param[in] x New X coordinate
     * @param[in] y New Y coordinate
     * @param[in] z New Z coordinate
     * @details Triggered by setX/setY/setZ/setPosition when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 位置更改时触发
     * @param[in] x 新 X 坐标
     * @param[in] y 新 Y 坐标
     * @param[in] z 新 Z 坐标
     * @details 当值实际更改时由 setX/setY/setZ/setPosition 触发。
     *          连接到更新 UI 元素或执行相关操作。
     * \endif
     */
    void positionChanged(double x, double y, double z);

    /**
     * \if ENGLISH
     * @brief Emitted when rotation angle changes
     * @param[in] angleDeg New angle in degrees
     * @details Triggered by setAngle() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 旋转角度更改时触发
     * @param[in] angleDeg 新的角度（度）
     * @details 当值实际更改时由 setAngle() 触发。
     *          连接到更新 UI 元素或执行相关操作。
     * \endif
     */
    void angleChanged(double angleDeg);

    /**
     * \if ENGLISH
     * @brief Emitted when pixel offset changes
     * @param[in] offsetX New pixel offset X
     * @param[in] offsetY New pixel offset Y
     * @details Triggered by setPixelOffsetX/setPixelOffsetY/setPixelOffset when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 像素偏移更改时触发
     * @param[in] offsetX 新像素偏移 X
     * @param[in] offsetY 新像素偏移 Y
     * @details 当值实际更改时由 setPixelOffsetX/setPixelOffsetY/setPixelOffset 触发。
     *          连接到更新 UI 元素或执行相关操作。
     * \endif
     */
    void pixelOffsetChanged(double offsetX, double offsetY);

    /**
     * \if ENGLISH
     * @brief Emitted when text color changes
     * @param[in] color New text color
     * @details Triggered by setColor() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 文本颜色更改时触发
     * @param[in] color 新文本颜色
     * @details 当值实际更改时由 setColor() 触发。
     *          连接到更新 UI 元素或执行相关操作。
     * \endif
     */
    void colorChanged(const QColor& color);

protected:
    // Begins drawing the text label
    virtual bool beginDraw() override;
};

}  // namespace QIM

#endif  // QIMPLOT3DTEXTITEMNODE_H
