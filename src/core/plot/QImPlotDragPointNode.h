#ifndef QIMPLOTDRAGPOINTNODE_H
#define QIMPLOTDRAGPOINTNODE_H

#include "../../QImAPI.h"
#ifndef QIM_CORE_API
#define QIM_CORE_API
#endif
#include <QColor>
#include <QPointF>
#include "QImPlotItemNode.h"
#include "QImPlot.h"

namespace QIM
{

/**
 * \if ENGLISH
 * @brief Qt-style wrapper for ImPlot draggable point tool
 *
 * @class QImPlotDragPointNode
 * @ingroup plot_tools
 *
 * @details Provides Qt-style retained mode encapsulation for ImPlot's DragPoint tool.
 *          A draggable point appears as a colored marker at specified plot coordinates.
 *          Users can click and drag the point to modify its position.
 *          The node emits positionChanged() signal when the user drags the point.
 *
 * @note The point position is stored as double values in plot coordinate space.
 *       ImPlot modifies the position directly through pointers during dragging.
 *
 * @param[in] parent Parent QObject (optional)
 *
 * @see QImPlotItemNode
 * @see QImPlotNode
 * @see ImPlot::DragPoint()
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot可拖拽点工具的Qt风格封装
 *
 * @class QImPlotDragPointNode
 * @ingroup plot_tools
 *
 * @details 为ImPlot的DragPoint工具提供Qt风格的保留模式封装。
 *          可拖拽点在指定的绘图坐标处显示为彩色标记。
 *          用户可以点击并拖拽该点以修改其位置。
 *          用户拖拽该点时，节点会发出positionChanged()信号。
 *
 * @note 点位置在绘图坐标空间中存储为double值。
 *       拖拽期间，ImPlot通过指针直接修改位置。
 *
 * @param[in] parent 父QObject对象（可选）
 *
 * @see QImPlotItemNode
 * @see QImPlotNode
 * @see ImPlot::DragPoint()
 * \endif
 */
class QIM_CORE_API QImPlotDragPointNode : public QImPlotItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotDragPointNode)

    /**
     * \if ENGLISH
     * @property QImPlotDragPointNode::position
     * @brief Position of the draggable point in plot coordinates
     *
     * @details Defines the (x, y) position where the draggable point will be rendered.
     *          Coordinates are in plot space (not pixels).
     *          Emits positionChanged() when the user drags the point.
     * @accessors READ position WRITE setPosition NOTIFY positionChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotDragPointNode::position
     * @brief 可拖拽点在绘图坐标中的位置
     *
     * @details 定义可拖拽点将渲染的 (x, y) 位置。
     *          坐标在绘图空间中（不是像素）。
     *          用户拖拽该点时，发出positionChanged()信号。
     * @accessors READ position WRITE setPosition NOTIFY positionChanged
     * \endif
     */
    Q_PROPERTY(QPointF position READ position WRITE setPosition NOTIFY positionChanged)

    /**
     * \if ENGLISH
     * @property QImPlotDragPointNode::color
     * @brief Color of the draggable point
     *
     * @details Defines the color for the draggable point marker.
     *          When not set, uses a default color from ImPlot.
     * @accessors READ color WRITE setColor NOTIFY colorChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotDragPointNode::color
     * @brief 可拖拽点的颜色
     *
     * @details 定义可拖拽点标记的颜色。
     *          未设置时，使用ImPlot的默认颜色。
     * @accessors READ color WRITE setColor NOTIFY colorChanged
     * \endif
     */
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

    /**
     * \if ENGLISH
     * @property QImPlotDragPointNode::size
     * @brief Size of the draggable point marker
     *
     * @details Defines the visual size of the draggable point in pixels.
     *          Default value is 4.0 pixels.
     * @accessors READ size WRITE setSize NOTIFY sizeChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotDragPointNode::size
     * @brief 可拖拽点标记的大小
     *
     * @details 定义可拖拽点的视觉大小（以像素为单位）。
     *          默认值为4.0像素。
     * @accessors READ size WRITE setSize NOTIFY sizeChanged
     * \endif
     */
    Q_PROPERTY(float size READ size WRITE setSize NOTIFY sizeChanged)

    /**
     * \if ENGLISH
     * @property QImPlotDragPointNode::id
     * @brief Unique identifier for the drag tool
     *
     * @details Defines the integer identifier used by ImPlot to distinguish
     *          between multiple draggable points in the same plot.
     *          Must be unique within the same plot context.
     * @accessors READ id WRITE setId NOTIFY idChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotDragPointNode::id
     * @brief 拖拽工具的唯一标识符
     *
     * @details 定义ImPlot用于区分同一绘图中多个可拖拽点的整数标识符。
     *          在同一绘图上下文中必须唯一。
     * @accessors READ id WRITE setId NOTIFY idChanged
     * \endif
     */
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)

    /**
     * \if ENGLISH
     * @property QImPlotDragPointNode::flags
     * @brief ImPlot drag tool flags
     *
     * @details Defines the ImPlotDragToolFlags bitmask for controlling tool behavior.
     *          Options include NoCursors, NoFit, NoInputs, Delayed.
     * @accessors READ flags WRITE setFlags NOTIFY flagsChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotDragPointNode::flags
     * @brief ImPlot拖拽工具标志
     *
     * @details 定义用于控制工具行为的ImPlotDragToolFlags位掩码。
     *          选项包括NoCursors、NoFit、NoInputs、Delayed。
     * @accessors READ flags WRITE setFlags NOTIFY flagsChanged
     * \endif
     */
    Q_PROPERTY(int flags READ flags WRITE setFlags NOTIFY flagsChanged)

    /**
     * \if ENGLISH
     * @property QImPlotDragPointNode::cursorsEnabled
     * @brief Enable cursor display during dragging
     * @details When true (default), cursors are shown while dragging the point.
     *          When false, ImPlotDragToolFlags_NoCursors is set.
     * @accessors READ isCursorsEnabled WRITE setCursorsEnabled NOTIFY dragToolFlagChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotDragPointNode::cursorsEnabled
     * @brief 启用拖拽时光标显示
     * @details 为true时（默认），拖拽时显示光标。
     *          为false时，设置ImPlotDragToolFlags_NoCursors。
     * @accessors READ isCursorsEnabled WRITE setCursorsEnabled NOTIFY dragToolFlagChanged
     * \endif
     */
    Q_PROPERTY(bool cursorsEnabled READ isCursorsEnabled WRITE setCursorsEnabled NOTIFY dragToolFlagChanged)

    /**
     * \if ENGLISH
     * @property QImPlotDragPointNode::fitEnabled
     * @brief Enable auto-fit when dragging
     * @details When true (default), plot auto-fits to include the dragged point.
     *          When false, ImPlotDragToolFlags_NoFit is set.
     * @accessors READ isFitEnabled WRITE setFitEnabled NOTIFY dragToolFlagChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotDragPointNode::fitEnabled
     * @brief 启用拖拽时自动适配
     * @details 为true时（默认），绘图自动适配以包含拖拽点。
     *          为false时，设置ImPlotDragToolFlags_NoFit。
     * @accessors READ isFitEnabled WRITE setFitEnabled NOTIFY dragToolFlagChanged
     * \endif
     */
    Q_PROPERTY(bool fitEnabled READ isFitEnabled WRITE setFitEnabled NOTIFY dragToolFlagChanged)

    /**
     * \if ENGLISH
     * @property QImPlotDragPointNode::inputsEnabled
     * @brief Enable user input for dragging
     * @details When true (default), the point responds to mouse input.
     *          When false, ImPlotDragToolFlags_NoInputs is set (point becomes non-interactive).
     * @accessors READ isInputsEnabled WRITE setInputsEnabled NOTIFY dragToolFlagChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotDragPointNode::inputsEnabled
     * @brief 启用拖拽输入
     * @details 为true时（默认），点响应鼠标输入。
     *          为false时，设置ImPlotDragToolFlags_NoInputs（点变为不可交互）。
     * @accessors READ isInputsEnabled WRITE setInputsEnabled NOTIFY dragToolFlagChanged
     * \endif
     */
    Q_PROPERTY(bool inputsEnabled READ isInputsEnabled WRITE setInputsEnabled NOTIFY dragToolFlagChanged)

    /**
     * \if ENGLISH
     * @property QImPlotDragPointNode::delayed
     * @brief Delayed commit mode
     * @details When true, position changes are only committed after mouse release.
     *          When false (default), position changes are committed immediately during drag.
     * @accessors READ isDelayed WRITE setDelayed NOTIFY dragToolFlagChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotDragPointNode::delayed
     * @brief 延迟提交模式
     * @details 为true时，位置更改仅在鼠标释放后提交。
     *          为false时（默认），位置更改在拖拽期间立即提交。
     * @accessors READ isDelayed WRITE setDelayed NOTIFY dragToolFlagChanged
     * \endif
     */
    Q_PROPERTY(bool delayed READ isDelayed WRITE setDelayed NOTIFY dragToolFlagChanged)

    /**
     * \if ENGLISH
     * @property QImPlotDragPointNode::clicked
     * @brief Whether the point was clicked in the current frame
     *
     * @details Read-only property indicating if the draggable point was clicked
     *          (mouse button pressed) in the current frame.
     *          Updated after each render cycle.
     * @accessors READ clicked NOTIFY clickedChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotDragPointNode::clicked
     * @brief 点在当前帧中是否被点击
     *
     * @details 只读属性，指示可拖拽点是否在当前帧中被点击（鼠标按钮按下）。
     *          每次渲染循环后更新。
     * @accessors READ clicked NOTIFY clickedChanged
     * \endif
     */
    Q_PROPERTY(bool clicked READ clicked NOTIFY clickedChanged)

    /**
     * \if ENGLISH
     * @property QImPlotDragPointNode::hovered
     * @brief Whether the point is hovered in the current frame
     *
     * @details Read-only property indicating if the mouse cursor is hovering
     *          over the draggable point in the current frame.
     *          Updated after each render cycle.
     * @accessors READ hovered NOTIFY hoveredChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotDragPointNode::hovered
     * @brief 点在当前帧中是否被悬停
     *
     * @details 只读属性，指示鼠标光标是否在当前帧中悬停在可拖拽点上。
     *          每次渲染循环后更新。
     * @accessors READ hovered NOTIFY hoveredChanged
     * \endif
     */
    Q_PROPERTY(bool hovered READ hovered NOTIFY hoveredChanged)

    /**
     * \if ENGLISH
     * @property QImPlotDragPointNode::held
     * @brief Whether the point is being held (dragged) in the current frame
     *
     * @details Read-only property indicating if the draggable point is currently
     *          being dragged (mouse button held down) in the current frame.
     *          Updated after each render cycle.
     * @accessors READ held NOTIFY heldChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotDragPointNode::held
     * @brief 点在当前帧中是否被按住（拖拽）
     *
     * @details 只读属性，指示可拖拽点是否在当前帧中被拖拽（鼠标按钮按下）。
     *          每次渲染循环后更新。
     * @accessors READ held NOTIFY heldChanged
     * \endif
     */
    Q_PROPERTY(bool held READ held NOTIFY heldChanged)

public:
    // Unique type identifier for QImPlotDragPointNode
    enum
    {
        Type = InnerType + 20
    };

    // Returns the type identifier of this plot item
    virtual int type() const override
    {
        return Type;
    }

    // Constructs a QImPlotDragPointNode with optional parent
    QImPlotDragPointNode(QObject* parent = nullptr);

    // Destructor
    ~QImPlotDragPointNode();

    //----------------------------------------------------
    // Position interface
    //----------------------------------------------------

    // Gets the point position
    QPointF position() const;

    // Sets the point position
    void setPosition(const QPointF& pos);

    // Sets the point position (convenience overload)
    void setPosition(double x, double y);

    //----------------------------------------------------
    // Style and configuration
    //----------------------------------------------------

    // Gets the point color
    QColor color() const;

    // Sets the point color
    void setColor(const QColor& color);

    // Gets the point size
    float size() const;

    // Sets the point size
    void setSize(float size);

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

    // Checks if point was clicked
    bool clicked() const;

    // Checks if point is hovered
    bool hovered() const;

    // Checks if point is being held
    bool held() const;

    //----------------------------------------------------
    // Utility
    //----------------------------------------------------

    // Returns true if the point was modified by user in the last render cycle
    bool wasModified() const;

Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when position changes
     * @param[in] pos New position
     * @details Triggered by user dragging or programmatic setPosition().
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 位置更改时触发
     * @param[in] pos 新位置
     * @details 用户拖拽或程序调用setPosition()时触发。
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
     * @brief Emitted when size changes
     * @param[in] size New size
     * @details Triggered by setSize() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 大小更改时触发
     * @param[in] size 新大小
     * @details 当值实际更改时由setSize()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void sizeChanged(float size);

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

    /**
     * \if ENGLISH
     * @brief Emitted when any drag tool flag property changes
     * @details Shared signal for cursorsEnabled, fitEnabled, inputsEnabled, delayed.
     * \endif
     *
     * \if CHINESE
     * @brief 任何拖拽工具标志属性更改时触发
     * @details cursorsEnabled、fitEnabled、inputsEnabled、delayed共享的信号。
     * \endif
     */
    void dragToolFlagChanged();

    /**
     * \if ENGLISH
     * @brief Emitted when clicked state changes
     * @param[in] clicked New clicked state
     * @details Triggered after each render cycle if clicked state changes.
     *          Connect to detect user interaction with the draggable point.
     * \endif
     *
     * \if CHINESE
     * @brief 点击状态更改时触发
     * @param[in] clicked 新点击状态
     * @details 如果点击状态更改，则在每次渲染循环后触发。
     *          连接到检测用户与可拖拽点的交互。
     * \endif
     */
    void clickedChanged(bool clicked);

    /**
     * \if ENGLISH
     * @brief Emitted when hovered state changes
     * @param[in] hovered New hovered state
     * @details Triggered after each render cycle if hovered state changes.
     *          Connect to detect user interaction with the draggable point.
     * \endif
     *
     * \if CHINESE
     * @brief 悬停状态更改时触发
     * @param[in] hovered 新悬停状态
     * @details 如果悬停状态更改，则在每次渲染循环后触发。
     *          连接到检测用户与可拖拽点的交互。
     * \endif
     */
    void hoveredChanged(bool hovered);

    /**
     * \if ENGLISH
     * @brief Emitted when held state changes
     * @param[in] held New held state
     * @details Triggered after each render cycle if held state changes.
     *          Connect to detect user interaction with the draggable point.
     * \endif
     *
     * \if CHINESE
     * @brief 按住状态更改时触发
     * @param[in] held 新按住状态
     * @details 如果按住状态更改，则在每次渲染循环后触发。
     *          连接到检测用户与可拖拽点的交互。
     * \endif
     */
    void heldChanged(bool held);

protected:
    // Begins drawing the draggable point
    virtual bool beginDraw() override;
};

}  // namespace QIM

#endif  // QIMPLOTDRAGPOINTNODE_H