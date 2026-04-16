#ifndef QIMPLOTTEXTITEMNODE_H
#define QIMPLOTTEXTITEMNODE_H

#include "QImAPI.h"
#include <QColor>
#include <QPointF>
#include "QImPlotItemNode.h"

namespace QIM
{

/**
 * \if ENGLISH
 * @brief Qt-style wrapper for ImPlot text label visualization
 *
 * @class QImPlotTextItemNode
 * @ingroup plot_items
 *
 * @details Provides Qt-style retained mode encapsulation for ImPlot text labels.
 *          Text labels render centered text at specified plot coordinates with optional
 *          pixel offset. Useful for annotating specific points, marking data features,
 *          or adding descriptive labels to plots.
 *          Inherits from QImPlotItemNode and follows the same PIMPL design pattern
 *          as other plot item nodes for consistency.
 *
 * @note Text labels are rendered at the specified plot position (x, y) and can be
 *       offset in pixel space for fine positioning adjustments.
 *
 * @param[in] parent Parent QObject (optional)
 *
 * @see QImPlotItemNode
 * @see QImPlotNode
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot文本标签的Qt风格封装
 *
 * @class QImPlotTextItemNode
 * @ingroup plot_items
 *
 * @details 为ImPlot文本标签提供Qt风格的保留模式封装。
 *          文本标签在指定的绘图坐标处渲染居中文本，可选像素偏移。
 *          适用于标注特定点、标记数据特征或为绘图添加描述性标签。
 *          继承自QImPlotItemNode，并遵循与其他绘图项节点相同的PIMPL设计模式以保持一致性。
 *
 * @note 文本标签在指定的绘图位置 (x, y) 处渲染，并可以在像素空间中进行偏移以进行精细定位调整。
 *
 * @param[in] parent 父QObject对象（可选）
 *
 * @see QImPlotItemNode
 * @see QImPlotNode
 * \endif
 */
class QIM_CORE_API QImPlotTextItemNode : public QImPlotItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotTextItemNode)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(QPointF position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QPointF pixelOffset READ pixelOffset WRITE setPixelOffset NOTIFY pixelOffsetChanged)
    Q_PROPERTY(bool vertical READ isVertical WRITE setVertical NOTIFY verticalChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    // Unique type identifier for QImPlotTextItemNode
    enum
    {
        Type = InnerType + 9
    };

    // Returns the type identifier of this plot item
    virtual int type() const override
    {
        return Type;
    }

    // Constructs a QImPlotTextItemNode with optional parent
    QImPlotTextItemNode(QObject* parent = nullptr);

    // Destroys the QImPlotTextItemNode
    ~QImPlotTextItemNode();

    //----------------------------------------------------
    // Text and position interface
    //----------------------------------------------------

    // Gets the text content
    QString text() const;

    // Sets the text content
    void setText(const QString& text);

    // Gets the plot position
    QPointF position() const;

    // Sets the plot position
    void setPosition(const QPointF& pos);

    // Sets the plot position (convenience overload)
    void setPosition(double x, double y);

    // Gets the pixel offset
    QPointF pixelOffset() const;

    // Sets the pixel offset
    void setPixelOffset(const QPointF& offset);

    // Sets the pixel offset (convenience overload)
    void setPixelOffset(float dx, float dy);

    //----------------------------------------------------
    // Style property accessors
    //----------------------------------------------------

    // Checks if vertical orientation is enabled
    bool isVertical() const;

    // Sets vertical orientation
    void setVertical(bool vertical);

    // Gets the text color
    QColor color() const;

    // Sets the text color
    void setColor(const QColor& color);

    // Gets the raw ImPlotTextFlags
    int textFlags() const;

    // Sets the raw ImPlotTextFlags
    void setTextFlags(int flags);

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
     * @details 当值实际更改时由setText()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void textChanged(const QString& text);

    /**
     * \if ENGLISH
     * @brief Emitted when position changes
     * @param[in] pos New position
     * @details Triggered by setPosition() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 位置更改时触发
     * @param[in] pos 新位置
     * @details 当值实际更改时由setPosition()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void positionChanged(const QPointF& pos);

    /**
     * \if ENGLISH
     * @brief Emitted when pixel offset changes
     * @param[in] offset New pixel offset
     * @details Triggered by setPixelOffset() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 像素偏移更改时触发
     * @param[in] offset 新像素偏移
     * @details 当值实际更改时由setPixelOffset()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void pixelOffsetChanged(const QPointF& offset);

    /**
     * \if ENGLISH
     * @brief Emitted when vertical orientation changes
     * @param[in] vertical New vertical orientation state
     * @details Triggered by setVertical() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 垂直方向更改时触发
     * @param[in] vertical 新垂直方向状态
     * @details 当值实际更改时由setVertical()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void verticalChanged(bool vertical);

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
     * @details 当值实际更改时由setColor()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void colorChanged(const QColor& color);

    /**
     * \if ENGLISH
     * @brief Emitted when text flags change
     * @details Triggered by any flag property change (vertical, etc.).
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 文本标志更改时触发
     * @details 任何标志属性更改（垂直方向等）时触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void textFlagChanged();

protected:
    // Begins drawing the text label
    virtual bool beginDraw() override;
};

}  // namespace QIM

#endif  // QIMPLOTTEXTITEMNODE_H
