#ifndef QIMPLOTANNOTATIONNODE_H
#define QIMPLOTANNOTATIONNODE_H

#include "../../QImAPI.h"
#ifndef QIM_CORE_API
#define QIM_CORE_API
#endif
#include <QColor>
#include <QPointF>
#include <QString>
#include "QImPlotItemNode.h"

namespace QIM
{

/**
 * \if ENGLISH
 * @brief Qt-style wrapper for ImPlot annotation tool
 *
 * @class QImPlotAnnotationNode
 * @ingroup plot_tools
 *
 * @details Provides Qt-style retained mode encapsulation for ImPlot's Annotation tool.
 *          An annotation appears as a callout text label at specified plot coordinates.
 *          The annotation can be positioned with pixel offset and clamping options.
 *
 * @note The annotation position is in plot coordinate space.
 *       The annotation is static (non-interactive) and does not support dragging.
 *
 * @param[in] parent Parent QObject (optional)
 *
 * @see QImPlotItemNode
 * @see QImPlotNode
 * @see ImPlot::Annotation()
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot注释工具的Qt风格封装
 *
 * @class QImPlotAnnotationNode
 * @ingroup plot_tools
 *
 * @details 为ImPlot的Annotation工具提供Qt风格的保留模式封装。
 *          注释在指定的绘图坐标处显示为标注文本标签。
 *          注释可以通过像素偏移和钳位选项进行定位。
 *
 * @note 注释位置在绘图坐标空间中。
 *       注释是静态的（非交互式），不支持拖拽。
 *
 * @param[in] parent 父QObject对象（可选）
 *
 * @see QImPlotItemNode
 * @see QImPlotNode
 * @see ImPlot::Annotation()
 * \endif
 */
class QIM_CORE_API QImPlotAnnotationNode : public QImPlotItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotAnnotationNode)

    // Position of the annotation in plot coordinates (READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QPointF position READ position WRITE setPosition NOTIFY positionChanged)

    // Text color of the annotation (READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

    // Text content of the annotation, supports printf-style formatting (READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)

    // Pixel offset from the plot position (READ pixelOffset WRITE setPixelOffset NOTIFY pixelOffsetChanged)
    Q_PROPERTY(QPointF pixelOffset READ pixelOffset WRITE setPixelOffset NOTIFY pixelOffsetChanged)

    // Whether to clamp the annotation within plot area (READ clamp WRITE setClamp NOTIFY clampChanged)
    Q_PROPERTY(bool clamp READ clamp WRITE setClamp NOTIFY clampChanged)

    // Whether to round the annotation position to integer pixels (READ round WRITE setRound NOTIFY roundChanged)
    Q_PROPERTY(bool round READ round WRITE setRound NOTIFY roundChanged)

public:
    // Unique type identifier for QImPlotAnnotationNode
    enum
    {
        Type = InnerType + 24
    };

    // Returns the type identifier of this plot item
    virtual int type() const override
    {
        return Type;
    }

    // Constructs a QImPlotAnnotationNode with optional parent
    QImPlotAnnotationNode(QObject* parent = nullptr);

    // Destructor
    ~QImPlotAnnotationNode();

    //----------------------------------------------------
    // Position interface
    //----------------------------------------------------

    // Gets the annotation position
    QPointF position() const;

    // Sets the annotation position
    void setPosition(const QPointF& pos);

    // Sets the annotation position (convenience overload)
    void setPosition(double x, double y);

    //----------------------------------------------------
    // Text and formatting
    //----------------------------------------------------

    // Gets the annotation text
    QString text() const;

    // Sets the annotation text
    void setText(const QString& text);

    // Sets the annotation text with printf-style formatting
    void setText(const char* fmt, ...);

    //----------------------------------------------------
    // Style and configuration
    //----------------------------------------------------

    // Gets the annotation text color
    QColor color() const;

    // Sets the annotation text color
    void setColor(const QColor& color);

    // Gets the pixel offset
    QPointF pixelOffset() const;

    // Sets the pixel offset
    void setPixelOffset(const QPointF& offset);

    // Sets the pixel offset (convenience overload)
    void setPixelOffset(double x, double y);

    // Gets the clamp setting
    bool clamp() const;

    // Sets the clamp setting
    void setClamp(bool clamp);

    // Gets the round setting
    bool round() const;

    // Sets the round setting
    void setRound(bool round);

Q_SIGNALS:
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
     * @brief Emitted when color changes
     * @param[in] color New color
     * @details Triggered by setColor() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 颜色更改时触发
     * @param[in] color 新颜色
     * @details 当值实际更改时由setColor()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void colorChanged(const QColor& color);

    /**
     * \if ENGLISH
     * @brief Emitted when text changes
     * @param[in] text New text
     * @details Triggered by setText() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 文本更改时触发
     * @param[in] text 新文本
     * @details 当值实际更改时由setText()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void textChanged(const QString& text);

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
     * @brief Emitted when clamp setting changes
     * @param[in] clamp New clamp setting
     * @details Triggered by setClamp() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 钳位设置更改时触发
     * @param[in] clamp 新钳位设置
     * @details 当值实际更改时由setClamp()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void clampChanged(bool clamp);

    /**
     * \if ENGLISH
     * @brief Emitted when round setting changes
     * @param[in] round New round setting
     * @details Triggered by setRound() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 舍入设置更改时触发
     * @param[in] round 新舍入设置
     * @details 当值实际更改时由setRound()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void roundChanged(bool round);

protected:
    // Begins drawing the annotation
    virtual bool beginDraw() override;
};

}  // namespace QIM

#endif  // QIMPLOTANNOTATIONNODE_H
