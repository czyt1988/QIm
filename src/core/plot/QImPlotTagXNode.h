#ifndef QIMPLOTTAGXNODE_H
#define QIMPLOTTAGXNODE_H

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
 * @brief Qt-style wrapper for ImPlot X-axis tag tool
 *
 * @class QImPlotTagXNode
 * @ingroup plot_tools
 *
 * @details Provides Qt-style retained mode encapsulation for ImPlot's TagX tool.
 *          A tag appears as a vertical line with optional text label at specified x coordinate.
 *          Tags are useful for marking specific x values on the plot.
 *
 * @note The tag position is in plot coordinate space.
 *       The tag is static (non-interactive) and does not support dragging.
 *
 * @param[in] parent Parent QObject (optional)
 *
 * @see QImPlotItemNode
 * @see QImPlotNode
 * @see ImPlot::TagX()
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot X轴标签工具的Qt风格封装
 *
 * @class QImPlotTagXNode
 * @ingroup plot_tools
 *
 * @details 为ImPlot的TagX工具提供Qt风格的保留模式封装。
 *          标签在指定的x坐标处显示为带可选文本标签的垂直线。
 *          标签用于标记绘图上的特定x值。
 *
 * @note 标签位置在绘图坐标空间中。
 *       标签是静态的（非交互式），不支持拖拽。
 *
 * @param[in] parent 父QObject对象（可选）
 *
 * @see QImPlotItemNode
 * @see QImPlotNode
 * @see ImPlot::TagX()
 * \endif
 */
class QIM_CORE_API QImPlotTagXNode : public QImPlotItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotTagXNode)

    Q_PROPERTY(double value READ value WRITE setValue NOTIFY valueChanged)

    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)

    Q_PROPERTY(bool round READ round WRITE setRound NOTIFY roundChanged)

public:
    // Unique type identifier for QImPlotTagXNode
    enum
    {
        Type = InnerType + 25
    };

    // Returns the type identifier of this plot item
    virtual int type() const override
    {
        return Type;
    }

    // Constructs a QImPlotTagXNode with optional parent
    QImPlotTagXNode(QObject* parent = nullptr);

    // Destructor
    ~QImPlotTagXNode();

    //----------------------------------------------------
    // Position interface
    //----------------------------------------------------

    // Gets the tag x position
    double value() const;

    // Sets the tag x position
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
     * @brief Emitted when x position changes
     * @param[in] value New x position
     * @details Triggered by setValue() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief X位置更改时触发
     * @param[in] value 新X位置
     * @details 当值实际更改时由setValue()触发。
     *          连接到更新UI元素或执行相关操作。
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
    // Begins drawing the X-axis tag
    virtual bool beginDraw() override;
};

}  // namespace QIM

#endif  // QIMPLOTTAGXNODE_H
