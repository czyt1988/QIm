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

    /**
     * \if ENGLISH
     * @property QImPlotAnnotationNode::position
     * @brief Position of the annotation in plot coordinates
     *
     * @details Defines the (x, y) position where the annotation will be rendered.
     *          Coordinates are in plot space.
     * @accessors READ position WRITE setPosition NOTIFY positionChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotAnnotationNode::position
     * @brief 注释在绘图坐标中的位置
     *
     * @details 定义注释将渲染的 (x, y) 位置。
     *          坐标在绘图空间中。
     * @accessors READ position WRITE setPosition NOTIFY positionChanged
     * \endif
     */
    Q_PROPERTY(QPointF position READ position WRITE setPosition NOTIFY positionChanged)

    /**
     * \if ENGLISH
     * @property QImPlotAnnotationNode::color
     * @brief Text color of the annotation
     *
     * @details Defines the color for the annotation text.
     *          When not set, uses a default color from ImPlot.
     * @accessors READ color WRITE setColor NOTIFY colorChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotAnnotationNode::color
     * @brief 注释文本的颜色
     *
     * @details 定义注释文本的颜色。
     *          未设置时，使用ImPlot的默认颜色。
     * @accessors READ color WRITE setColor NOTIFY colorChanged
     * \endif
     */
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

    /**
     * \if ENGLISH
     * @property QImPlotAnnotationNode::text
     * @brief Text content of the annotation
     *
     * @details Defines the text label displayed by the annotation.
     *          Supports printf-style formatting when using setText() with arguments.
     *          Default is empty string (no text displayed).
     * @accessors READ text WRITE setText NOTIFY textChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotAnnotationNode::text
     * @brief 注释的文本内容
     *
     * @details 定义注释显示的文本标签。
     *          使用带参数的setText()时支持printf风格格式化。
     *          默认为空字符串（不显示文本）。
     * @accessors READ text WRITE setText NOTIFY textChanged
     * \endif
     */
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)

    /**
     * \if ENGLISH
     * @property QImPlotAnnotationNode::pixelOffset
     * @brief Pixel offset from the plot position
     *
     * @details Defines the offset in pixels from the plot coordinates.
     *          Positive values move the annotation right/down, negative values left/up.
     *          Default is (0, 0).
     * @accessors READ pixelOffset WRITE setPixelOffset NOTIFY pixelOffsetChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotAnnotationNode::pixelOffset
     * @brief 相对于绘图位置的像素偏移
     *
     * @details 定义相对于绘图坐标的像素偏移。
     *          正值将注释向右/下移动，负值向左/上移动。
     *          默认值为 (0, 0)。
     * @accessors READ pixelOffset WRITE setPixelOffset NOTIFY pixelOffsetChanged
     * \endif
     */
    Q_PROPERTY(QPointF pixelOffset READ pixelOffset WRITE setPixelOffset NOTIFY pixelOffsetChanged)

    /**
     * \if ENGLISH
     * @property QImPlotAnnotationNode::clamp
     * @brief Whether to clamp the annotation within plot area
     *
     * @details When true, the annotation will be clamped to stay within the plot area.
     *          When false, the annotation can be positioned outside the plot area.
     *          Default is false.
     * @accessors READ clamp WRITE setClamp NOTIFY clampChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotAnnotationNode::clamp
     * @brief 是否将注释钳位在绘图区域内
     *
     * @details 为true时，注释将被钳位以保持在绘图区域内。
     *          为false时，注释可以定位在绘图区域外。
     *          默认值为false。
     * @accessors READ clamp WRITE setClamp NOTIFY clampChanged
     * \endif
     */
    Q_PROPERTY(bool clamp READ clamp WRITE setClamp NOTIFY clampChanged)

    /**
     * \if ENGLISH
     * @property QImPlotAnnotationNode::round
     * @brief Whether to round the annotation position to integer pixels
     *
     * @details When true, the annotation position will be rounded to integer pixel coordinates.
     *          This can improve text rendering sharpness.
     *          Default is false.
     * @accessors READ round WRITE setRound NOTIFY roundChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotAnnotationNode::round
     * @brief 是否将注释位置舍入为整数像素
     *
     * @details 为true时，注释位置将舍入为整数像素坐标。
     *          这可以提高文本渲染的清晰度。
     *          默认值为false。
     * @accessors READ round WRITE setRound NOTIFY roundChanged
     * \endif
     */
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
