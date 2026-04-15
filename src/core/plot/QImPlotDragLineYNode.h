#ifndef QIMPLOTDRAGLINEYNODE_H
#define QIMPLOTDRAGLINEYNODE_H

#include "../../QImAPI.h"
#include <QColor>
#include "QImPlotItemNode.h"

namespace QIM
{

/**
 * \if ENGLISH
 * @brief Qt-style wrapper for ImPlot draggable horizontal line tool
 *
 * @class QImPlotDragLineYNode
 * @ingroup plot_tools
 *
 * @details Provides Qt-style retained mode encapsulation for ImPlot's DragLineY tool.
 *          A draggable horizontal line appears at specified y coordinate.
 *          Users can click and drag the line to modify its y position.
 *          The node emits valueChanged() signal when the user drags the line.
 *
 * @note The line position is stored as a double value in plot coordinate space.
 *       ImPlot modifies the position directly through pointers during dragging.
 *
 * @param[in] parent Parent QObject (optional)
 *
 * @see QImPlotItemNode
 * @see QImPlotNode
 * @see ImPlot::DragLineY()
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot可拖拽水平线工具的Qt风格封装
 *
 * @class QImPlotDragLineYNode
 * @ingroup plot_tools
 *
 * @details 为ImPlot的DragLineY工具提供Qt风格的保留模式封装。
 *          可拖拽水平线出现在指定的y坐标处。
 *          用户可以点击并拖拽该线以修改其y位置。
 *          用户拖拽该线时，节点会发出valueChanged()信号。
 *
 * @note 线位置在绘图坐标空间中存储为double值。
 *       拖拽期间，ImPlot通过指针直接修改位置。
 *
 * @param[in] parent 父QObject对象（可选）
 *
 * @see QImPlotItemNode
 * @see QImPlotNode
 * @see ImPlot::DragLineY()
 * \endif
 */
class QIM_CORE_API QImPlotDragLineYNode : public QImPlotItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotDragLineYNode)

    /**
     * \if ENGLISH
     * @property QImPlotDragLineYNode::value
     * @brief Y position of the draggable horizontal line
     *
     * @details Defines the y coordinate where the draggable horizontal line will be rendered.
     *          Coordinate is in plot space.
     *          Emits valueChanged() when the user drags the line.
     * @accessors READ value WRITE setValue NOTIFY valueChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotDragLineYNode::value
     * @brief 可拖拽水平线的Y位置
     *
     * @details 定义可拖拽水平线将渲染的y坐标。
     *          坐标在绘图空间中。
     *          用户拖拽该线时，发出valueChanged()信号。
     * @accessors READ value WRITE setValue NOTIFY valueChanged
     * \endif
     */
    Q_PROPERTY(double value READ value WRITE setValue NOTIFY valueChanged)

    /**
     * \if ENGLISH
     * @property QImPlotDragLineYNode::color
     * @brief Color of the draggable line
     *
     * @details Defines the color for the draggable line.
     *          When not set, uses a default color from ImPlot.
     * @accessors READ color WRITE setColor NOTIFY colorChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotDragLineYNode::color
     * @brief 可拖拽线的颜色
     *
     * @details 定义可拖拽线的颜色。
     *          未设置时，使用ImPlot的默认颜色。
     * @accessors READ color WRITE setColor NOTIFY colorChanged
     * \endif
     */
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

    /**
     * \if ENGLISH
     * @property QImPlotDragLineYNode::thickness
     * @brief Thickness of the draggable line
     *
     * @details Defines the visual thickness of the draggable line in pixels.
     *          Default value is 1.0 pixel.
     * @accessors READ thickness WRITE setThickness NOTIFY thicknessChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotDragLineYNode::thickness
     * @brief 可拖拽线的厚度
     *
     * @details 定义可拖拽线的视觉厚度（以像素为单位）。
     *          默认值为1.0像素。
     * @accessors READ thickness WRITE setThickness NOTIFY thicknessChanged
     * \endif
     */
    Q_PROPERTY(float thickness READ thickness WRITE setThickness NOTIFY thicknessChanged)

    /**
     * \if ENGLISH
     * @property QImPlotDragLineYNode::id
     * @brief Unique identifier for the drag tool
     *
     * @details Defines the integer identifier used by ImPlot to distinguish
     *          between multiple draggable lines in the same plot.
     *          Must be unique within the same plot context.
     * @accessors READ id WRITE setId NOTIFY idChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotDragLineYNode::id
     * @brief 拖拽工具的唯一标识符
     *
     * @details 定义ImPlot用于区分同一绘图中多个可拖拽线的整数标识符。
     *          在同一绘图上下文中必须唯一。
     * @accessors READ id WRITE setId NOTIFY idChanged
     * \endif
     */
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)

    /**
     * \if ENGLISH
     * @property QImPlotDragLineYNode::flags
     * @brief ImPlot drag tool flags
     *
     * @details Defines the ImPlotDragToolFlags bitmask for controlling tool behavior.
     *          Options include NoCursors, NoFit, NoInputs, Delayed.
     * @accessors READ flags WRITE setFlags NOTIFY flagsChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotDragLineYNode::flags
     * @brief ImPlot拖拽工具标志
     *
     * @details 定义用于控制工具行为的ImPlotDragToolFlags位掩码。
     *          选项包括NoCursors、NoFit、NoInputs、Delayed。
     * @accessors READ flags WRITE setFlags NOTIFY flagsChanged
     * \endif
     */
    Q_PROPERTY(int flags READ flags WRITE setFlags NOTIFY flagsChanged)

    Q_PROPERTY(bool cursorsEnabled READ isCursorsEnabled WRITE setCursorsEnabled NOTIFY dragToolFlagChanged)
    Q_PROPERTY(bool fitEnabled READ isFitEnabled WRITE setFitEnabled NOTIFY dragToolFlagChanged)
    Q_PROPERTY(bool inputsEnabled READ isInputsEnabled WRITE setInputsEnabled NOTIFY dragToolFlagChanged)
    Q_PROPERTY(bool delayed READ isDelayed WRITE setDelayed NOTIFY dragToolFlagChanged)

    /**
     * \if ENGLISH
     * @property QImPlotDragLineYNode::clicked
     * @brief Whether the line was clicked in the current frame
     *
     * @details Read-only property indicating if the draggable line was clicked
     *          (mouse button pressed) in the current frame.
     *          Updated after each render cycle.
     * @accessors READ clicked NOTIFY clickedChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotDragLineYNode::clicked
     * @brief 线在当前帧中是否被点击
     *
     * @details 只读属性，指示可拖拽线是否在当前帧中被点击（鼠标按钮按下）。
     *          每次渲染循环后更新。
     * @accessors READ clicked NOTIFY clickedChanged
     * \endif
     */
    Q_PROPERTY(bool clicked READ clicked NOTIFY clickedChanged)

    /**
     * \if ENGLISH
     * @property QImPlotDragLineYNode::hovered
     * @brief Whether the line is hovered in the current frame
     *
     * @details Read-only property indicating if the mouse cursor is hovering
     *          over the draggable line in the current frame.
     *          Updated after each render cycle.
     * @accessors READ hovered NOTIFY hoveredChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotDragLineYNode::hovered
     * @brief 线在当前帧中是否被悬停
     *
     * @details 只读属性，指示鼠标光标是否在当前帧中悬停在可拖拽线上。
     *          每次渲染循环后更新。
     * @accessors READ hovered NOTIFY hoveredChanged
     * \endif
     */
    Q_PROPERTY(bool hovered READ hovered NOTIFY hoveredChanged)

    /**
     * \if ENGLISH
     * @property QImPlotDragLineYNode::held
     * @brief Whether the line is being held (dragged) in the current frame
     *
     * @details Read-only property indicating if the draggable line is currently
     *          being dragged (mouse button held down) in the current frame.
     *          Updated after each render cycle.
     * @accessors READ held NOTIFY heldChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotDragLineYNode::held
     * @brief 线在当前帧中是否被按住（拖拽）
     *
     * @details 只读属性，指示可拖拽线是否在当前帧中被拖拽（鼠标按钮按下）。
     *          每次渲染循环后更新。
     * @accessors READ held NOTIFY heldChanged
     * \endif
     */
    Q_PROPERTY(bool held READ held NOTIFY heldChanged)

public:
    // Unique type identifier for QImPlotDragLineYNode
    enum
    {
        Type = InnerType + 22
    };

    // Returns the type identifier of this plot item
    virtual int type() const override
    {
        return Type;
    }

    // Constructs a QImPlotDragLineYNode with optional parent
    QImPlotDragLineYNode(QObject* parent = nullptr);

    // Destructor
    ~QImPlotDragLineYNode();

    //----------------------------------------------------
    // Value interface
    //----------------------------------------------------

    // Gets the line y position
    double value() const;

    // Sets the line y position
    void setValue(double value);

    //----------------------------------------------------
    // Style and configuration
    //----------------------------------------------------

    // Gets the line color
    QColor color() const;

    // Sets the line color
    void setColor(const QColor& color);

    // Gets the line thickness
    float thickness() const;

    // Sets the line thickness
    void setThickness(float thickness);

    // Gets the drag tool ID
    int id() const;

    // Sets the drag tool ID
    void setId(int id);

    // Gets the ImPlotDragToolFlags
    int flags() const;

    // Sets the ImPlotDragToolFlags
    void setFlags(int flags);

    // Drag tool flag accessors (否定→肯定语义)
    bool isCursorsEnabled() const;
    void setCursorsEnabled(bool enabled);
    bool isFitEnabled() const;
    void setFitEnabled(bool enabled);
    bool isInputsEnabled() const;
    void setInputsEnabled(bool enabled);
    bool isDelayed() const;
    void setDelayed(bool on);

    //----------------------------------------------------
    // Interaction state (read-only)
    //----------------------------------------------------

    // Checks if line was clicked
    bool clicked() const;

    // Checks if line is hovered
    bool hovered() const;

    // Checks if line is being held
    bool held() const;

    //----------------------------------------------------
    // Utility
    //----------------------------------------------------

    // Returns true if the line was modified by user in the last render cycle
    bool wasModified() const;

Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when y position changes
     * @param[in] value New y position
     * @details Triggered by user dragging or programmatic setValue().
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief y位置更改时触发
     * @param[in] value 新y位置
     * @details 用户拖拽或程序调用setValue()时触发。
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
     * @brief Emitted when thickness changes
     * @param[in] thickness New thickness
     * @details Triggered by setThickness() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 厚度更改时触发
     * @param[in] thickness 新厚度
     * @details 当值实际更改时由setThickness()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void thicknessChanged(float thickness);

    /**
     * \if ENGLISH
     * @brief Emitted when ID changes
     * @param[in] id New ID
     * @details Triggered by setId() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief ID更改时触发
     * @param[in] id 新ID
     * @details 当值实际更改时由setId()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void idChanged(int id);

    /**
     * \if ENGLISH
     * @brief Emitted when flags change
     * @param[in] flags New flags
     * @details Triggered by setFlags() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
 *
 * \if CHINESE
 * @brief 标志更改时触发
 * @param[in] flags 新标志
 * @details 当值实际更改时由setFlags()触发。
 *          连接到更新UI元素或执行相关操作。
 * \endif
 */
    void flagsChanged(int flags);

    void dragToolFlagChanged();

    /**
     * \if ENGLISH
     * @brief Emitted when clicked state changes
     * @param[in] clicked New clicked state
     * @details Triggered after each render cycle if clicked state changes.
     *          Connect to detect user interaction with the draggable line.
     * \endif
     *
     * \if CHINESE
     * @brief 点击状态更改时触发
     * @param[in] clicked 新点击状态
     * @details 如果点击状态更改，则在每次渲染循环后触发。
     *          连接到检测用户与可拖拽线的交互。
     * \endif
     */
    void clickedChanged(bool clicked);

    /**
     * \if ENGLISH
     * @brief Emitted when hovered state changes
     * @param[in] hovered New hovered state
     * @details Triggered after each render cycle if hovered state changes.
     *          Connect to detect user interaction with the draggable line.
     * \endif
     *
     * \if CHINESE
     * @brief 悬停状态更改时触发
     * @param[in] hovered 新悬停状态
     * @details 如果悬停状态更改，则在每次渲染循环后触发。
     *          连接到检测用户与可拖拽线的交互。
     * \endif
     */
    void hoveredChanged(bool hovered);

    /**
     * \if ENGLISH
     * @brief Emitted when held state changes
     * @param[in] held New held state
     * @details Triggered after each render cycle if held state changes.
     *          Connect to detect user interaction with the draggable line.
     * \endif
     *
     * \if CHINESE
     * @brief 按住状态更改时触发
     * @param[in] held 新按住状态
     * @details 如果按住状态更改，则在每次渲染循环后触发。
     *          连接到检测用户与可拖拽线的交互。
     * \endif
     */
    void heldChanged(bool held);

protected:
    // Begins drawing the draggable horizontal line
    virtual bool beginDraw() override;
};

}  // namespace QIM

#endif  // QIMPLOTDRAGLINEYNODE_H