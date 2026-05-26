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
 * @details Provides Qt-style retained mode encapsulation for ImPlot's DragLineY tool.
 *          A draggable horizontal line appears at specified y coordinate.
 *          Users can click and drag the line to modify its y position.
 *          The node emits valueChanged() signal when the user drags the line.
 *
 * @note The line position is stored as a double value in plot coordinate space.
 *       ImPlot modifies the position directly through pointers during dragging.
 *
 * @see QImPlotItemNode
 * @see QImPlotNode
 * @see ImPlot::DragLineY()
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot可拖拽水平线工具的Qt风格封装
 *
 * @details 为ImPlot的DragLineY工具提供Qt风格的保留模式封装。
 *          可拖拽水平线出现在指定的y坐标处。
 *          用户可以点击并拖拽该线以修改其y位置。
 *          用户拖拽该线时，节点会发出valueChanged()信号。
 *
 * @note 线位置在绘图坐标空间中存储为double值。
 *       拖拽期间，ImPlot通过指针直接修改位置。
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

    Q_PROPERTY(double value READ value WRITE setValue NOTIFY valueChanged)

    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

    Q_PROPERTY(float thickness READ thickness WRITE setThickness NOTIFY thicknessChanged)

    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)

    Q_PROPERTY(int flags READ flags WRITE setFlags NOTIFY flagsChanged)

    Q_PROPERTY(bool cursorsEnabled READ isCursorsEnabled WRITE setCursorsEnabled NOTIFY dragToolFlagChanged)
    Q_PROPERTY(bool fitEnabled READ isFitEnabled WRITE setFitEnabled NOTIFY dragToolFlagChanged)
    Q_PROPERTY(bool inputsEnabled READ isInputsEnabled WRITE setInputsEnabled NOTIFY dragToolFlagChanged)
    Q_PROPERTY(bool delayed READ isDelayed WRITE setDelayed NOTIFY dragToolFlagChanged)

    Q_PROPERTY(bool clicked READ clicked NOTIFY clickedChanged)

    Q_PROPERTY(bool hovered READ hovered NOTIFY hoveredChanged)

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

    /**
     * \if ENGLISH
     * @brief Emitted when any drag tool flag changes
     * @details Triggered by any flag property setter (setCursorsEnabled, setFitEnabled,
     *          setInputsEnabled, setDelayed) when value actually changes.
     * \endif
     *
     * \if CHINESE
     * @brief 拖拽工具标志更改时触发
     * @details 任何标志属性setter（setCursorsEnabled、setFitEnabled、
     *          setInputsEnabled、setDelayed）当值实际更改时触发。
     * \endif
     */
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