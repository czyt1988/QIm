#ifndef QIMPLOTDRAGRECTNODE_H
#define QIMPLOTDRAGRECTNODE_H

#include "../../QImAPI.h"
#ifndef QIM_CORE_API
#define QIM_CORE_API
#endif
#include <QColor>
#include <QRectF>
#include "QImPlotItemNode.h"

namespace QIM
{

/**
 * \if ENGLISH
 * @brief Qt-style wrapper for ImPlot draggable rectangle tool
 *
 * @class QImPlotDragRectNode
 * @ingroup plot_tools
 *
 * @details Provides Qt-style retained mode encapsulation for ImPlot's DragRect tool.
 *          A draggable rectangle appears between specified coordinates (x1,y1) and (x2,y2).
 *          Users can click and drag the rectangle to modify its position and size.
 *          The node emits rectChanged() signal when the user drags the rectangle.
 *
 * @note The rectangle coordinates are stored as double values in plot coordinate space.
 *       ImPlot modifies the coordinates directly through pointers during dragging.
 *
 * @param[in] parent Parent QObject (optional)
 *
 * @see QImPlotItemNode
 * @see QImPlotNode
 * @see ImPlot::DragRect()
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot可拖拽矩形工具的Qt风格封装
 *
 * @class QImPlotDragRectNode
 * @ingroup plot_tools
 *
 * @details 为ImPlot的DragRect工具提供Qt风格的保留模式封装。
 *          可拖拽矩形出现在指定的坐标 (x1,y1) 和 (x2,y2) 之间。
 *          用户可以点击并拖拽该矩形以修改其位置和大小。
 *          用户拖拽该矩形时，节点会发出rectChanged()信号。
 *
 * @note 矩形坐标在绘图坐标空间中存储为double值。
 *       拖拽期间，ImPlot通过指针直接修改坐标。
 *
 * @param[in] parent 父QObject对象（可选）
 *
 * @see QImPlotItemNode
 * @see QImPlotNode
 * @see ImPlot::DragRect()
 * \endif
 */
class QIM_CORE_API QImPlotDragRectNode : public QImPlotItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotDragRectNode)

    /**
     * \if ENGLISH
     * @property QImPlotDragRectNode::rect
     * @brief Rectangle coordinates in plot space
     *
     * @details Defines the (x1, y1, x2, y2) coordinates where the draggable rectangle will be rendered.
     *          Coordinates are in plot space.
     *          Emits rectChanged() when the user drags the rectangle.
     * @accessors READ rect WRITE setRect NOTIFY rectChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotDragRectNode::rect
     * @brief 绘图空间中的矩形坐标
     *
     * @details 定义可拖拽矩形将渲染的 (x1, y1, x2, y2) 坐标。
     *          坐标在绘图空间中。
     *          用户拖拽该矩形时，发出rectChanged()信号。
     * @accessors READ rect WRITE setRect NOTIFY rectChanged
     * \endif
     */
    Q_PROPERTY(QRectF rect READ rect WRITE setRect NOTIFY rectChanged)

    /**
     * \if ENGLISH
     * @property QImPlotDragRectNode::color
     * @brief Color of the draggable rectangle border
     *
     * @details Defines the color for the draggable rectangle border.
     *          When not set, uses a default color from ImPlot.
     * @accessors READ color WRITE setColor NOTIFY colorChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotDragRectNode::color
     * @brief 可拖拽矩形边框的颜色
     *
     * @details 定义可拖拽矩形边框的颜色。
     *          未设置时，使用ImPlot的默认颜色。
     * @accessors READ color WRITE setColor NOTIFY colorChanged
     * \endif
     */
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

    /**
     * \if ENGLISH
     * @property QImPlotDragRectNode::id
     * @brief Unique identifier for the drag tool
     *
     * @details Defines the integer identifier used by ImPlot to distinguish
     *          between multiple draggable rectangles in the same plot.
     *          Must be unique within the same plot context.
     * @accessors READ id WRITE setId NOTIFY idChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotDragRectNode::id
     * @brief 拖拽工具的唯一标识符
     *
     * @details 定义ImPlot用于区分同一绘图中多个可拖拽矩形的整数标识符。
     *          在同一绘图上下文中必须唯一。
     * @accessors READ id WRITE setId NOTIFY idChanged
     * \endif
     */
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)

    /**
     * \if ENGLISH
     * @property QImPlotDragRectNode::flags
     * @brief ImPlot drag tool flags
     *
     * @details Defines the ImPlotDragToolFlags bitmask for controlling tool behavior.
     *          Options include NoCursors, NoFit, NoInputs, Delayed.
     * @accessors READ flags WRITE setFlags NOTIFY flagsChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotDragRectNode::flags
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
     * @property QImPlotDragRectNode::clicked
     * @brief Whether the rectangle was clicked in the current frame
     *
     * @details Read-only property indicating if the draggable rectangle was clicked
     *          (mouse button pressed) in the current frame.
     *          Updated after each render cycle.
     * @accessors READ clicked NOTIFY clickedChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotDragRectNode::clicked
     * @brief 矩形在当前帧中是否被点击
     *
     * @details 只读属性，指示可拖拽矩形是否在当前帧中被点击（鼠标按钮按下）。
     *          每次渲染循环后更新。
     * @accessors READ clicked NOTIFY clickedChanged
     * \endif
     */
    Q_PROPERTY(bool clicked READ clicked NOTIFY clickedChanged)

    /**
     * \if ENGLISH
     * @property QImPlotDragRectNode::hovered
     * @brief Whether the rectangle is hovered in the current frame
     *
     * @details Read-only property indicating if the mouse cursor is hovering
     *          over the draggable rectangle in the current frame.
     *          Updated after each render cycle.
     * @accessors READ hovered NOTIFY hoveredChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotDragRectNode::hovered
     * @brief 矩形在当前帧中是否被悬停
     *
     * @details 只读属性，指示鼠标光标是否在当前帧中悬停在可拖拽矩形上。
     *          每次渲染循环后更新。
     * @accessors READ hovered NOTIFY hoveredChanged
     * \endif
     */
    Q_PROPERTY(bool hovered READ hovered NOTIFY hoveredChanged)

    /**
     * \if ENGLISH
     * @property QImPlotDragRectNode::held
     * @brief Whether the rectangle is being held (dragged) in the current frame
     *
     * @details Read-only property indicating if the draggable rectangle is currently
     *          being dragged (mouse button held down) in the current frame.
     *          Updated after each render cycle.
     * @accessors READ held NOTIFY heldChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotDragRectNode::held
     * @brief 矩形在当前帧中是否被按住（拖拽）
     *
     * @details 只读属性，指示可拖拽矩形是否在当前帧中被拖拽（鼠标按钮按下）。
     *          每次渲染循环后更新。
     * @accessors READ held NOTIFY heldChanged
     * \endif
     */
    Q_PROPERTY(bool held READ held NOTIFY heldChanged)

public:
    // Unique type identifier for QImPlotDragRectNode
    enum
    {
        Type = InnerType + 23
    };

    // Returns the type identifier of this plot item
    virtual int type() const override
    {
        return Type;
    }

    // Constructs a QImPlotDragRectNode with optional parent
    QImPlotDragRectNode(QObject* parent = nullptr);

    // Destructor
    ~QImPlotDragRectNode();

    //----------------------------------------------------
    // Rectangle interface
    //----------------------------------------------------

    // Gets the rectangle coordinates
    QRectF rect() const;

    // Sets the rectangle coordinates
    void setRect(const QRectF& rect);

    // Sets the rectangle coordinates (convenience overload)
    void setRect(double x1, double y1, double x2, double y2);

    //----------------------------------------------------
    // Style and configuration
    //----------------------------------------------------

    // Gets the rectangle border color
    QColor color() const;

    // Sets the rectangle border color
    void setColor(const QColor& color);

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

    // Checks if rectangle was clicked
    bool clicked() const;

    // Checks if rectangle is hovered
    bool hovered() const;

    // Checks if rectangle is being held
    bool held() const;

    //----------------------------------------------------
    // Utility
    //----------------------------------------------------

    // Returns true if the rectangle was modified by user in the last render cycle
    bool wasModified() const;

Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when rectangle coordinates change
     * @param[in] rect New rectangle coordinates
     * @details Triggered by user dragging or programmatic setRect().
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 矩形坐标更改时触发
     * @param[in] rect 新矩形坐标
     * @details 用户拖拽或程序调用setRect()时触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void rectChanged(const QRectF& rect);

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
     *          Connect to detect user interaction with the draggable rectangle.
     * \endif
     *
     * \if CHINESE
     * @brief 点击状态更改时触发
     * @param[in] clicked 新点击状态
     * @details 如果点击状态更改，则在每次渲染循环后触发。
     *          连接到检测用户与可拖拽矩形的交互。
     * \endif
     */
    void clickedChanged(bool clicked);

    /**
     * \if ENGLISH
     * @brief Emitted when hovered state changes
     * @param[in] hovered New hovered state
     * @details Triggered after each render cycle if hovered state changes.
     *          Connect to detect user interaction with the draggable rectangle.
     * \endif
     *
     * \if CHINESE
     * @brief 悬停状态更改时触发
     * @param[in] hovered 新悬停状态
     * @details 如果悬停状态更改，则在每次渲染循环后触发。
     *          连接到检测用户与可拖拽矩形的交互。
     * \endif
     */
    void hoveredChanged(bool hovered);

    /**
     * \if ENGLISH
     * @brief Emitted when held state changes
     * @param[in] held New held state
     * @details Triggered after each render cycle if held state changes.
     *          Connect to detect user interaction with the draggable rectangle.
     * \endif
     *
     * \if CHINESE
     * @brief 按住状态更改时触发
     * @param[in] held 新按住状态
     * @details 如果按住状态更改，则在每次渲染循环后触发。
     *          连接到检测用户与可拖拽矩形的交互。
     * \endif
     */
    void heldChanged(bool held);

protected:
    // Begins drawing the draggable rectangle
    virtual bool beginDraw() override;
};

}  // namespace QIM

#endif  // QIMPLOTDRAGRECTNODE_H