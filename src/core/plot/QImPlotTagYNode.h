#ifndef QIMPLOTTAGYNODE_H
#define QIMPLOTTAGYNODE_H

#include "../../QImAPI.h"
#ifndef QIM_CORE_API
#define QIM_CORE_API
#endif
#include <QColor>
#include <QString>
#include "QImPlotItemNode.h"

namespace QIM
{

/**
 * \if ENGLISH
 * @brief Qt-style wrapper for ImPlot Y-axis tag tool
 *
 * @class QImPlotTagYNode
 * @ingroup plot_tools
 *
 * @details Provides Qt-style retained mode encapsulation for ImPlot's TagY tool.
 *          A tag appears as a horizontal line with optional text label at specified y coordinate.
 *          Tags are useful for marking specific y values on the plot.
 *
 * @note The tag position is in plot coordinate space.
 *       The tag is static (non-interactive) and does not support dragging.
 *
 * @param[in] parent Parent QObject (optional)
 *
 * @see QImPlotItemNode
 * @see QImPlotNode
 * @see ImPlot::TagY()
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot Y 轴标签工具的 Qt 风格封装
 *
 * @class QImPlotTagYNode
 * @ingroup plot_tools
 *
 * @details 为 ImPlot 的 TagY 工具提供 Qt 风格的保留模式封装。
 *          标签在指定的 y 坐标处显示为带可选文本标签的水平线。
 *          标签用于标记绘图上的特定 y 值。
 *
 * @note 标签位置在绘图坐标空间中。
 *       标签是静态的（非交互式），不支持拖拽。
 *
 * @param[in] parent 父 QObject 对象（可选）
 *
 * @see QImPlotItemNode
 * @see QImPlotNode
 * @see ImPlot::TagY()
 * \endif
 */
class QIM_CORE_API QImPlotTagYNode : public QImPlotItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotTagYNode)

    /**
     * \if ENGLISH
     * @property QImPlotTagYNode::value
     * @brief Y position of the tag in plot coordinates
     *
     * @details Defines the y coordinate where the tag will be rendered.
     *          Coordinate is in plot space.
     * @accessors READ value WRITE setValue NOTIFY valueChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotTagYNode::value
     * @brief 标签在绘图坐标中的 Y 位置
     *
     * @details 定义标签将渲染的 y 坐标。
     *          坐标在绘图空间中。
     * @accessors READ value WRITE setValue NOTIFY valueChanged
     * \endif
     */
    Q_PROPERTY(double value READ value WRITE setValue NOTIFY valueChanged)

    /**
     * \if ENGLISH
     * @property QImPlotTagYNode::color
     * @brief Color of the tag line
     *
     * @details Defines the color for the tag horizontal line.
     *          When not set, uses a default color from ImPlot.
     * @accessors READ color WRITE setColor NOTIFY colorChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotTagYNode::color
     * @brief 标签线的颜色
     *
     * @details 定义标签水平线的颜色。
     *          未设置时，使用 ImPlot 的默认颜色。
     * @accessors READ color WRITE setColor NOTIFY colorChanged
     * \endif
     */
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

    /**
     * \if ENGLISH
     * @property QImPlotTagYNode::text
     * @brief Text content of the tag
     *
     * @details Defines the text label displayed by the tag.
     *          Supports printf-style formatting when using setText() with arguments.
     *          Default is empty string (no text displayed).
     * @accessors READ text WRITE setText NOTIFY textChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotTagYNode::text
     * @brief 标签的文本内容
     *
     * @details 定义标签显示的文本标签。
     *          使用带参数的 setText() 时支持 printf 风格格式化。
     *          默认为空字符串（不显示文本）。
     * @accessors READ text WRITE setText NOTIFY textChanged
     * \endif
     */
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)

    /**
     * \if ENGLISH
     * @property QImPlotTagYNode::round
     * @brief Whether to round the tag position to integer pixels
     *
     * @details When true, the tag position will be rounded to integer pixel coordinates.
     *          This can improve line rendering sharpness.
     *          Default is false.
     * @accessors READ round WRITE setRound NOTIFY roundChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotTagYNode::round
     * @brief 是否将标签位置舍入为整数像素
     *
     * @details 为 true 时，标签位置将舍入为整数像素坐标。
     *          这可以提高线渲染的清晰度。
     *          默认值为 false。
     * @accessors READ round WRITE setRound NOTIFY roundChanged
     * \endif
     */
    Q_PROPERTY(bool round READ round WRITE setRound NOTIFY roundChanged)

public:
    // Unique type identifier for QImPlotTagYNode
    enum
    {
        Type = InnerType + 26
    };

    // Returns the type identifier of this plot item
    virtual int type() const override
    {
        return Type;
    }

    // Constructs a QImPlotTagYNode with optional parent
    QImPlotTagYNode(QObject* parent = nullptr);

    // Destructor
    ~QImPlotTagYNode();

    //----------------------------------------------------
    // Position interface
    //----------------------------------------------------

    // Gets the tag y position
    double value() const;

    // Sets the tag y position
    void setValue(double value);

    //----------------------------------------------------
    // Text and formatting
    //----------------------------------------------------

    // Gets the tag text
    QString text() const;

    // Sets the tag text
    void setText(const QString& text);

    // Sets the tag text with printf-style formatting
    void setText(const char* fmt, ...);

    //----------------------------------------------------
    // Style and configuration
    //----------------------------------------------------

    // Gets the tag line color
    QColor color() const;

    // Sets the tag line color
    void setColor(const QColor& color);

    // Gets the round setting
    bool round() const;

    // Sets the round setting
    void setRound(bool round);

Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when y position changes
     * @param[in] value New y position
     * @details Triggered by setValue() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief Y 位置更改时触发
     * @param[in] value 新 Y 位置
     * @details 当值实际更改时由 setValue() 触发。
     *          连接到更新 UI 元素或执行相关操作。
     * \endif
     */
    void valueChanged(double value);

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
     * @details 当值实际更改时由 setColor() 触发。
     *          连接到更新 UI 元素或执行相关操作。
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
     * @details 当值实际更改时由 setText() 触发。
     *          连接到更新 UI 元素或执行相关操作。
     * \endif
     */
    void textChanged(const QString& text);

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
     * @details 当值实际更改时由 setRound() 触发。
     *          连接到更新 UI 元素或执行相关操作。
     * \endif
     */
    void roundChanged(bool round);

protected:
    // Begins drawing the Y-axis tag
    virtual bool beginDraw() override;
};

}  // namespace QIM

#endif  // QIMPLOTTAGYNODE_H
