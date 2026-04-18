#include "QImPlotNode.h"
#include <cmath>
#include <vector>
#include <array>
// implot
#include "implot.h"
#include "implot_internal.h"  // 用于 ImAxis 枚举
// Qt
#include <QDebug>
// QIM
#include "QImPlotAxisInfo.h"
#include "QImTrackedValue.hpp"
#include "QtImGuiUtils.h"
#include "QImPlotItemNode.h"
#include "QImPlotLineItemNode.h"
#include "QImPlotLegendNode.h"
namespace QIM
{

// PIMPL 实现
class QImPlotNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlotNode)
public:
    PrivateData(QImPlotNode* q);
    void renderAllAxis();
    ImPlotAxis* imPlotAxis(QImPlotAxisId axisId) const;

public:
    // UTF-8 缓存（避免渲染时转换）
    QImTrackedValue< QByteArray > titleUtf8;
    ImPlotPlot* plot { nullptr };
    // 尺寸
    ImVec2 size { -1, -1 };
    bool autoSize { true };

    // 交互标志
    ImPlotFlags plotFlags { ImPlotFlags_None };
    bool beginPlotSuccess { false };
    //===============================================================
    // 固定的节点
    //===============================================================
    // 坐标轴
    std::array< QImPlotAxisInfo*, static_cast< std::size_t >(QImPlotAxisId::AxisCount) > axisInfo { nullptr };
    // legend
    std::unique_ptr< QImPlotLegendNode > legendNode;
    //===============================================================
    // 功能
    //===============================================================
    QImTrackedValue< bool > axesToFit { false };  ///< 是否需要自适应坐标轴，只需一次即可，因此使用QImTrackedValue
};

QImPlotNode::PrivateData::PrivateData(QImPlotNode* q) : q_ptr(q)
{
    // 默认属性初始化
    titleUtf8 = "##Plot";
    // 默认构造两个坐标轴
    for (int i = 0; i < static_cast< int >(QImPlotAxisId::AxisCount); ++i) {
        QImPlotAxisId id = static_cast< QImPlotAxisId >(i);
        axisInfo[ i ]    = new QImPlotAxisInfo(static_cast< QImPlotAxisId >(i), q);
        if ((id != QImPlotAxisId::X1) && (id != QImPlotAxisId::Y1)) {
            axisInfo[ i ]->setEnabled(false);
        }
    }
    legendNode = std::make_unique< QImPlotLegendNode >();
}

void QImPlotNode::PrivateData::renderAllAxis()
{
    for (int i = 0; i < static_cast< int >(QImPlotAxisId::AxisCount); ++i) {
        axisInfo[ i ]->render();
    }
}

ImPlotAxis* QImPlotNode::PrivateData::imPlotAxis(QImPlotAxisId axisId) const
{
    if (!plot) {
        return nullptr;
    }
    ImAxis idx = plot->CurrentX;
    if (axisId != QImPlotAxisId::Auto) {
        idx = toImAxis(axisId);
    }
    return plot->Axes + idx;
}

// ==================== 公共接口实现 ====================

/**
 * \if ENGLISH
 * @brief Constructs a QImPlotNode with optional parent
 * @param[in] parent Parent QObject for ownership management
 * @details Creates a plot node with default title "##Plot" and auto-size enabled.
 *          Initializes 6 axes (X1-X3, Y1-Y3) with X1 and Y1 enabled by default.
 *          Also creates an internal legend node.
 * \endif
 *
 * \if CHINESE
 * @brief 构造 QImPlotNode，可选指定父对象
 * @param[in] parent 用于所有权管理的父 QObject
 * @details 创建标题默认为 "##Plot" 且自动大小启用的绘图节点。
 *          初始化 6 个坐标轴（X1-X3, Y1-Y3），默认启用 X1 和 Y1。
 *          同时创建内部图例节点。
 * \endif
 */
QImPlotNode::QImPlotNode(QObject* parent) : QImAbstractNode(parent), QIM_PIMPL_CONSTRUCT
{
    setObjectName(QStringLiteral("PlotNode"));
}

/**
 * \if ENGLISH
 * @brief Constructs a QImPlotNode with specified title and optional parent
 * @param[in] title Plot title string (supports ImGui double-hash prefix "##" for hidden titles)
 * @param[in] parent Parent QObject for ownership management
 * @details Convenience constructor that sets the plot title immediately after creation.
 * \endif
 *
 * \if CHINESE
 * @brief 构造 QImPlotNode，指定标题和可选父对象
 * @param[in] title 绘图标题字符串（支持 ImGui 双井号前缀 "##" 隐藏标题）
 * @param[in] parent 用于所有权管理的父 QObject
 * @details 便捷构造函数，创建后立即设置绘图标题。
 * \endif
 */
QImPlotNode::QImPlotNode(const QString& title, QObject* parent) : QImPlotNode(parent)
{
    setTitle(title);
}

/**
 * \if ENGLISH
 * @brief Destructor for QImPlotNode
 * @details Destroys the plot node and all associated resources.
 *          Child nodes (axes, items) are automatically destroyed via QObject ownership.
 * \endif
 *
 * \if CHINESE
 * @brief QImPlotNode 的析构函数
 * @details 销毁绘图节点及所有关联资源。
 *          子节点（坐标轴、图形项）通过 QObject 所有权机制自动销毁。
 * \endif
 */
QImPlotNode::~QImPlotNode()
{
}

// === 标题 ===
/**
 * \if ENGLISH
 * @brief Gets the plot title text
 * @return Current title string
 * @details Returns the UTF-8 decoded title string.
 *          Titles prefixed with "##" (ImGui convention) are hidden but used as unique identifiers.
 * \endif
 *
 * \if CHINESE
 * @brief 获取绘图标题文本
 * @return 当前标题字符串
 * @details 返回 UTF-8 解码后的标题字符串。
 *          以 "##" 前缀的标题（ImGui 规范）会被隐藏但用作唯一标识符。
 * \endif
 */
QString QImPlotNode::title() const
{
    QIM_DC(d);
    return QString::fromUtf8(d->titleUtf8.value());
}

/**
 * \if ENGLISH
 * @brief Sets the plot title text
 * @param[in] title New title string (supports "##" prefix for hidden unique IDs)
 * @details Stores the title as UTF-8 QByteArray internally (render-minimization principle).
 *          Emits titleChanged() signal only when the value actually changes.
 * \endif
 *
 * \if CHINESE
 * @brief 设置绘图标题文本
 * @param[in] title 新标题字符串（支持 "##" 前缀作为隐藏的唯一 ID）
 * @details 内部将标题存储为 UTF-8 QByteArray（遵循渲染最小化原则）。
 *          仅在值实际改变时触发 titleChanged() 信号。
 * \endif
 */
void QImPlotNode::setTitle(const QString& title)
{
    QIM_D(d);
    d->titleUtf8 = title.toUtf8();
    if (d->titleUtf8.is_dirty()) {
        Q_EMIT titleChanged(title);
    }
}

// === 尺寸 ===
/**
 * \if ENGLISH
 * @brief Gets the plot size in floating-point pixels
 * @return Current plot size as QSizeF
 * @details Returns the actual size of the plot area.
 *          QSizeF(-1,-1) indicates auto-size mode where plot fills available space.
 * \endif
 *
 * \if CHINESE
 * @brief 获取绘图尺寸（浮点像素）
 * @return 当前绘图尺寸，以 QSizeF 返回
 * @details 返回绘图区域的实际大小。
 *          QSizeF(-1,-1) 表示自动大小模式，绘图将填充可用空间。
 * \endif
 */
QSizeF QImPlotNode::size() const
{
    QIM_DC(d);
    return toQSizeF(d->size);
}

/**
 * \if ENGLISH
 * @brief Sets the plot size in floating-point pixels
 * @param[in] size New plot size; QSizeF(-1,-1) enables auto-size mode
 * @details Converts QSizeF to internal ImVec2 storage. Negative values indicate auto-size.
 *          Emits sizeChanged() signal only when the size actually changes.
 *          Also updates autoSize flag based on whether both dimensions are -1.
 * \endif
 *
 * \if CHINESE
 * @brief 设置绘图尺寸（浮点像素）
 * @param[in] size 新绘图尺寸；QSizeF(-1,-1) 启用自动大小模式
 * @details 将 QSizeF 转换为内部 ImVec2 存储。负值表示自动大小。
 *          仅在尺寸实际改变时触发 sizeChanged() 信号。
 *          同时根据两个维度是否均为 -1 更新 autoSize 标志。
 * \endif
 */
void QImPlotNode::setSize(const QSizeF& size)
{
    QIM_D(d);
    ImVec2 newSize = toImVec2(size);

    // 特殊处理：QSizeF(-1,-1) 应转换为 ImVec2(-1,-1)
    if (size.width() < 0)
        newSize.x = -1.0f;
    if (size.height() < 0)
        newSize.y = -1.0f;

    if (newSize.x != d->size.x || newSize.y != d->size.y) {
        d->size     = newSize;
        d->autoSize = (newSize.x == -1.0f && newSize.y == -1.0f);
        Q_EMIT sizeChanged(size);
    }
}

/**
 * \if ENGLISH
 * @brief Checks if auto-size mode is enabled
 * @return true if plot automatically fills available space
 * @details Auto-size mode uses ImVec2(-1,-1), making the plot expand to fill parent area.
 * \endif
 *
 * \if CHINESE
 * @brief 检查自动大小模式是否启用
 * @return true 表示绘图自动填充可用空间
 * @details 自动大小模式使用 ImVec2(-1,-1)，使绘图扩展以填充父区域。
 * \endif
 */
bool QImPlotNode::isAutoSize() const
{
    QIM_DC(d);
    return d->autoSize;
}

/**
 * \if ENGLISH
 * @brief Enables or disables auto-size mode
 * @param[in] autoSize true to enable auto-size (fills parent), false for fixed size (0,0 default)
 * @details When enabling auto-size, internal size is set to ImVec2(-1,-1).
 *          When disabling, size is set to ImVec2(0,0) as default fixed size.
 *          Emits autoSizeChanged() and sizeChanged() signals on change.
 * \endif
 *
 * \if CHINESE
 * @brief 启用或禁用自动大小模式
 * @param[in] autoSize true 启用自动大小（填充父区域），false 固定大小（0,0 默认）
 * @details 启用自动大小时，内部尺寸设为 ImVec2(-1,-1)。
 *          禁用时，尺寸设为 ImVec2(0,0) 作为默认固定大小。
 *          值改变时触发 autoSizeChanged() 和 sizeChanged() 信号。
 * \endif
 */
void QImPlotNode::setAutoSize(bool autoSize)
{
    QIM_D(d);
    if (d->autoSize != autoSize) {
        d->autoSize = autoSize;
        d->size     = autoSize ? ImVec2(-1, -1) : ImVec2(0, 0);  // 0,0 表示默认大小
        Q_EMIT autoSizeChanged(autoSize);
        Q_EMIT sizeChanged(size());
    }
}

/**
 * \if ENGLISH
 * @brief Gets axis info object for specified axis ID
 * @param[in] aid QImPlotAxisId identifying which axis (X1-X3, Y1-Y3)
 * @return Pointer to QImPlotAxisInfo for the specified axis
 * @details Returns the axis info object that controls axis properties (limits, flags, label, etc.).
 * \endif
 *
 * \if CHINESE
 * @brief 获取指定轴 ID 的坐标轴信息对象
 * @param[in] aid QImPlotAxisId 标识哪个轴（X1-X3, Y1-Y3）
 * @return 指定轴的 QImPlotAxisInfo 指针
 * @details 返回控制轴属性（范围、标志、标签等）的坐标轴信息对象。
 * \endif
 */
QImPlotAxisInfo* QImPlotNode::axisInfo(QImPlotAxisId aid) const
{
    QIM_DC(d);
    return d->axisInfo[ static_cast< int >(aid) ];
}

/**
 * \if ENGLISH
 * @brief Gets the primary X-axis (X1) info object
 * @return Pointer to QImPlotAxisInfo for X1 axis
 * @details Convenience accessor for the bottom X-axis, which is enabled by default.
 * \endif
 *
 * \if CHINESE
 * @brief 获取主 X 轴（X1）信息对象
 * @return X1 轴的 QImPlotAxisInfo 指针
 * @details 底部 X 轴的便捷访问器，默认启用。
 * \endif
 */
QImPlotAxisInfo* QImPlotNode::x1Axis() const
{
    QIM_DC(d);
    return d->axisInfo[ static_cast< int >(QImPlotAxisId::X1) ];
}

/**
 * \if ENGLISH
 * @brief Gets the primary Y-axis (Y1) info object
 * @return Pointer to QImPlotAxisInfo for Y1 axis
 * @details Convenience accessor for the left Y-axis, which is enabled by default.
 * \endif
 *
 * \if CHINESE
 * @brief 获取主 Y 轴（Y1）信息对象
 * @return Y1 轴的 QImPlotAxisInfo 指针
 * @details 左侧 Y 轴的便捷访问器，默认启用。
 * \endif
 */
QImPlotAxisInfo* QImPlotNode::y1Axis() const
{
    QIM_DC(d);
    return d->axisInfo[ static_cast< int >(QImPlotAxisId::Y1) ];
}

/**
 * \if ENGLISH
 * @brief Gets the secondary X-axis (X2) info object
 * @return Pointer to QImPlotAxisInfo for X2 axis
 * @details Convenience accessor for the top X-axis, disabled by default.
 * \endif
 *
 * \if CHINESE
 * @brief 获取次级 X 轴（X2）信息对象
 * @return X2 轴的 QImPlotAxisInfo 指针
 * @details 顶部 X 轴的便捷访问器，默认禁用。
 * \endif
 */
QImPlotAxisInfo* QImPlotNode::x2Axis() const
{
    QIM_DC(d);
    return d->axisInfo[ static_cast< int >(QImPlotAxisId::X2) ];
}

/**
 * \if ENGLISH
 * @brief Gets the secondary Y-axis (Y2) info object
 * @return Pointer to QImPlotAxisInfo for Y2 axis
 * @details Convenience accessor for the right Y-axis, disabled by default.
 * \endif
 *
 * \if CHINESE
 * @brief 获取次级 Y 轴（Y2）信息对象
 * @return Y2 轴的 QImPlotAxisInfo 指针
 * @details 右侧 Y 轴的便捷访问器，默认禁用。
 * \endif
 */
QImPlotAxisInfo* QImPlotNode::y2Axis() const
{
    QIM_DC(d);
    return d->axisInfo[ static_cast< int >(QImPlotAxisId::Y2) ];
}

/**
 * \if ENGLISH
 * @brief Gets the third X-axis (X3) info object
 * @return Pointer to QImPlotAxisInfo for X3 axis
 * @details Convenience accessor for an additional X-axis, disabled by default.
 * \endif
 *
 * \if CHINESE
 * @brief 获取第三 X 轴（X3）信息对象
 * @return X3 轴的 QImPlotAxisInfo 指针
 * @details 附加 X 轴的便捷访问器，默认禁用。
 * \endif
 */
QImPlotAxisInfo* QImPlotNode::x3Axis() const
{
    QIM_DC(d);
    return d->axisInfo[ static_cast< int >(QImPlotAxisId::X3) ];
}

/**
 * \if ENGLISH
 * @brief Gets the third Y-axis (Y3) info object
 * @return Pointer to QImPlotAxisInfo for Y3 axis
 * @details Convenience accessor for an additional Y-axis, disabled by default.
 * \endif
 *
 * \if CHINESE
 * @brief 获取第三 Y 轴（Y3）信息对象
 * @return Y3 轴的 QImPlotAxisInfo 指针
 * @details 附加 Y 轴的便捷访问器，默认禁用。
 * \endif
 */
QImPlotAxisInfo* QImPlotNode::y3Axis() const
{
    QIM_DC(d);
    return d->axisInfo[ static_cast< int >(QImPlotAxisId::Y3) ];
}

/*duplicate-removed*/

/**
 * \if ENGLISH
 * @brief Checks if a specific axis is enabled
 * @param[in] aid QImPlotAxisId identifying which axis
 * @return true if the axis info object exists (always true for all 6 axes)
 * @details Checks axis existence; use axisInfo(aid)->setEnabled() to actually enable/disable.
 * \endif
 *
 * \if CHINESE
 * @brief 检查指定坐标轴是否启用
 * @param[in] aid QImPlotAxisId 标识哪个轴
 * @return true 表示轴信息对象存在（6 个轴始终存在）
 * @details 检查轴是否存在；使用 axisInfo(aid)->setEnabled() 来实际启用/禁用。
 * \endif
 */
bool QImPlotNode::isAxisEnabled(QImPlotAxisId aid) const
{
    return (axisInfo(aid) != nullptr);
}

/**
 * \if ENGLISH
 * @brief Enables or disables a specific axis
 * @param[in] aid QImPlotAxisId identifying which axis
 * @param[in] on true to enable the axis, false to disable
 * @details Delegates to QImPlotAxisInfo::setEnabled() for the specified axis.
 * \endif
 *
 * \if CHINESE
 * @brief 启用或禁用指定坐标轴
 * @param[in] aid QImPlotAxisId 标识哪个轴
 * @param[in] on true 启用轴，false 禁用
 * @details 委托给 QImPlotAxisInfo::setEnabled() 处理指定轴。
 * \endif
 */
void QImPlotNode::setAxisEnabled(QImPlotAxisId aid, bool on)
{
    if (auto axis = axisInfo(aid)) {
        axis->setEnabled(on);
    }
}

// ===== 辅助宏定义=====
#ifndef QImPlotNode_FLAG_ACCESSOR
#define QImPlotNode_FLAG_ACCESSOR(FlagName, FlagEnum)                                                                  \
    bool QImPlotNode::is##FlagName() const                                                                             \
    {                                                                                                                  \
        QIM_DC(d);                                                                                                     \
        return (d->plotFlags & FlagEnum) != 0;                                                                         \
    }                                                                                                                  \
    void QImPlotNode::set##FlagName(bool on)                                                                           \
    {                                                                                                                  \
        QIM_D(d);                                                                                                      \
        const ImPlotFlags oldFlags = d->plotFlags;                                                                     \
        if (on)                                                                                                        \
            d->plotFlags |= FlagEnum;                                                                                  \
        else                                                                                                           \
            d->plotFlags &= ~FlagEnum;                                                                                 \
        if (d->plotFlags != oldFlags)                                                                                  \
            Q_EMIT plotFlagChanged();                                                                                  \
    }
#endif
#ifndef QImPlotNode_ENABLED_ACCESSOR
#define QImPlotNode_ENABLED_ACCESSOR(PropName, FlagEnum)                                                               \
    bool QImPlotNode::is##PropName() const                                                                             \
    {                                                                                                                  \
        QIM_DC(d);                                                                                                     \
        return (d->plotFlags & FlagEnum) == 0;                                                                         \
    }                                                                                                                  \
    void QImPlotNode::set##PropName(bool enabled)                                                                      \
    {                                                                                                                  \
        QIM_D(d);                                                                                                      \
        const ImPlotFlags oldFlags = d->plotFlags;                                                                     \
        if (enabled)                                                                                                   \
            d->plotFlags &= ~FlagEnum;                                                                                 \
        else                                                                                                           \
            d->plotFlags |= FlagEnum;                                                                                  \
        if (d->plotFlags != oldFlags)                                                                                  \
            Q_EMIT plotFlagChanged();                                                                                  \
    }
#endif
// ===== 标志访问器实现 =====

/**
 * \if ENGLISH
 * @fn QImPlotNode::isTitleEnabled
 * @brief Checks if plot title is visible
 * @return true if title is enabled (ImPlotFlags_NoTitle NOT set)
 * @details When disabled, plot title text is hidden. Titles are also hidden if the plot name
 *          starts with double hashes (e.g., "##MyPlot") regardless of this flag.
 * @note Corresponds to ImPlotFlags_NoTitle flag with inverted logic:
 *       enabled = flag NOT set, disabled = flag set.
 * @see setTitleEnabled()
 * \endif
 *
* \if CHINESE
 * @fn QImPlotNode::isTitleEnabled
 * @brief 检查绘图标题是否可见
 * @return true 表示标题启用（未设置 ImPlotFlags_NoTitle）
 * @details 禁用后，绘图标题文本将隐藏。如果绘图名称以双井号开头（例如 "##MyPlot"），
 *          无论此标志如何都会隐藏标题。
 * @note 对应 ImPlotFlags_NoTitle 标志，逻辑反转：
 *       启用 = 未设置标志位，禁用 = 设置标志位。
 * @see setTitleEnabled()
 * \endif
 */
QImPlotNode_ENABLED_ACCESSOR(TitleEnabled, ImPlotFlags_NoTitle)

    /**
     * \if ENGLISH
     * @fn QImPlotNode::isLegendEnabled
     * @brief Checks if legend is visible
     * @return true if legend is enabled (ImPlotFlags_NoLegend NOT set)
     * @details When disabled, the interactive legend panel (showing series names and colors) is hidden.
     *          Legend visibility can also be toggled at runtime via right-click menu unless menus are disabled.
     * @note Corresponds to ImPlotFlags_NoLegend flag with inverted logic:
     *       enabled = flag NOT set, disabled = flag set.
     * @see setLegendEnabled()
     * \endif
     *
     * \if CHINESE
     * @fn QImPlotNode::isLegendEnabled
     * @brief 检查图例是否可见
     * @return true 表示图例启用（未设置 ImPlotFlags_NoLegend）
     * @details 禁用后，交互式图例面板（显示序列名称和颜色）将隐藏。
     *          除非禁用菜单，否则图例可见性也可通过右键菜单在运行时切换。
     * @note 对应 ImPlotFlags_NoLegend 标志，逻辑反转：
     *       启用 = 未设置标志位，禁用 = 设置标志位。
     * @see setLegendEnabled()
     * \endif
     */
    QImPlotNode_ENABLED_ACCESSOR(LegendEnabled, ImPlotFlags_NoLegend)

    /**
     * \if ENGLISH
     * @fn QImPlotNode::isMouseTextEnabled
     * @brief Checks if mouse coordinates are displayed inside plot
     * @return true if mouse text is enabled (ImPlotFlags_NoMouseText NOT set)
     * @details When enabled, the current mouse position in plot coordinates (e.g., "X: 1.23, Y: 4.56")
     *          is displayed in the bottom-left corner of the plot area while hovering.
     * @note Corresponds to ImPlotFlags_NoMouseText flag with inverted logic:
     *       enabled = flag NOT set, disabled = flag set.
     * @see setMouseTextEnabled()
     * \endif
     *
     * \if CHINESE
     * @fn QImPlotNode::isMouseTextEnabled
     * @brief 检查鼠标坐标是否在绘图内部显示
     * @return true 表示鼠标文本启用（未设置 ImPlotFlags_NoMouseText）
     * @details 启用后，悬停时当前鼠标位置的绘图坐标（例如 "X: 1.23, Y: 4.56"）
     *          会显示在绘图区域的左下角。
     * @note 对应 ImPlotFlags_NoMouseText 标志，逻辑反转：
     *       启用 = 未设置标志位，禁用 = 设置标志位。
     * @see setMouseTextEnabled()
     * \endif
     */
    QImPlotNode_ENABLED_ACCESSOR(MouseTextEnabled, ImPlotFlags_NoMouseText)

    /**
     * \if ENGLISH
     * @fn QImPlotNode::isInputsEnabled
     * @brief Checks if user interaction with plot is enabled
     * @return true if inputs are enabled (ImPlotFlags_NoInputs NOT set)
     * @details When disabled, all user interactions are blocked:
     *          - No panning/zooming via mouse drag or wheel
     *          - No box selection
     *          - No context menu via right-click
     *          - No legend item toggling
     * @note Corresponds to ImPlotFlags_NoInputs flag with inverted logic:
     *       enabled = flag NOT set, disabled = flag set.
     * @warning Disabling inputs also implicitly disables menus and box selection regardless of their individual flags.
     * @see setInputsEnabled()
     * \endif
     *
     * \if CHINESE
     * @fn QImPlotNode::isInputsEnabled
     * @brief 检查用户与绘图的交互是否启用
     * @return true 表示输入启用（未设置 ImPlotFlags_NoInputs）
     * @details 禁用后，所有用户交互被阻断：
     *          - 无法通过鼠标拖拽或滚轮平移/缩放
     *          - 无法框选
     *          - 无法通过右键打开上下文菜单
     *          - 无法切换图例项
     * @note 对应 ImPlotFlags_NoInputs 标志，逻辑反转：
     *       启用 = 未设置标志位，禁用 = 设置标志位。
     * @warning 禁用输入也会隐式禁用菜单和框选，无论其单独标志如何设置。
     * @see setInputsEnabled()
     * \endif
     */
    QImPlotNode_ENABLED_ACCESSOR(InputsEnabled, ImPlotFlags_NoInputs)

    /**
     * \if ENGLISH
     * @fn QImPlotNode::isMenusEnabled
     * @brief Checks if context menus are accessible via right-click
     * @return true if menus are enabled (ImPlotFlags_NoMenus NOT set)
     * @details When enabled, right-clicking the plot opens a context menu with options:
     *          - Toggle legend items
     *          - Lock/unlock axes
     *          - Change scale types (linear/log/time)
     *          - Query data values
     * @note Corresponds to ImPlotFlags_NoMenus flag with inverted logic:
     *       enabled = flag NOT set, disabled = flag set.
     * @warning Menus are also disabled when inputs are disabled (isInputsEnabled() == false).
     * @see setMenusEnabled()
     * \endif
     *
     * \if CHINESE
     * @fn QImPlotNode::isMenusEnabled
     * @brief 检查是否可通过右键单击访问上下文菜单
     * @return true 表示菜单启用（未设置 ImPlotFlags_NoMenus）
     * @details 启用后，右键单击绘图会打开包含以下选项的上下文菜单：
     *          - 切换图例项
     *          - 锁定/解锁坐标轴
     *          - 更改刻度类型（线性/对数/时间）
     *          - 查询数据值
     * @note 对应 ImPlotFlags_NoMenus 标志，逻辑反转：
     *       启用 = 未设置标志位，禁用 = 设置标志位。
     * @warning 当输入被禁用时（isInputsEnabled() == false），菜单也会被禁用。
     * @see setMenusEnabled()
     * \endif
     */
    QImPlotNode_ENABLED_ACCESSOR(MenusEnabled, ImPlotFlags_NoMenus)

    /**
     * \if ENGLISH
     * @fn QImPlotNode::isBoxSelectEnabled
     * @brief Checks if box selection (rubber-band selection) is enabled
     * @return true if box selection is enabled (ImPlotFlags_NoBoxSelect NOT set)
     * @details When enabled, dragging with right mouse button creates a
     *          selection rectangle.
     * @note Corresponds to ImPlotFlags_NoBoxSelect flag with inverted logic:
     *       enabled = flag NOT set, disabled = flag set.
     * @warning Box selection requires inputs to be enabled (isInputsEnabled() must be true).
     * @see setBoxSelectEnabled()
     * \endif
     *
     * \if CHINESE
     * @brief 检查框选（套索选择）是否启用
     * @return true 表示框选启用（未设置 ImPlotFlags_NoBoxSelect）
     * @details 启用后，右键拖动鼠标左键会创建选择矩形。

     * @note 对应 ImPlotFlags_NoBoxSelect 标志，逻辑反转：
     *       启用 = 未设置标志位，禁用 = 设置标志位。
     * @warning 框选要求输入启用（isInputsEnabled() 必须为 true）。
     * @see setBoxSelectEnabled()
     * \endif
     */
    QImPlotNode_ENABLED_ACCESSOR(BoxSelectEnabled, ImPlotFlags_NoBoxSelect)

    /**
     * \if ENGLISH
     * @fn QImPlotNode::isFrameEnabled
     * @brief Checks if ImGui frame border/background is rendered around plot
     * @return true if frame is enabled (ImPlotFlags_NoFrame NOT set)
     * @details When disabled, the plot renders without the standard ImGui frame (border and background fill).
     *          Plot area extends to the full allocated space, useful for embedding plots in custom UI layouts.
     * @note Corresponds to ImPlotFlags_NoFrame flag with inverted logic:
     *       enabled = flag NOT set, disabled = flag set.
     * @see setFrameEnabled()
     * \endif
     *
     * \if CHINESE
     * @fn QImPlotNode::isFrameEnabled
     * @brief 检查绘图周围是否渲染 ImGui 框架边框/背景
     * @return true 表示框架启用（未设置 ImPlotFlags_NoFrame）
     * @details 禁用后，绘图将不渲染标准 ImGui 框架（边框和背景填充）。
     *          绘图区域扩展到完整分配空间，适用于在自定义 UI 布局中嵌入绘图。
     * @note 对应 ImPlotFlags_NoFrame 标志，逻辑反转：
     *       启用 = 未设置标志位，禁用 = 设置标志位。
     * @see setFrameEnabled()
     * \endif
     */
    QImPlotNode_ENABLED_ACCESSOR(FrameEnabled, ImPlotFlags_NoFrame)

    /**
     * \if ENGLISH
     * @fn QImPlotNode::isEqual
     * @brief Checks if X and Y axes are constrained to equal scale (1:1 aspect ratio)
     * @return true if equal scaling is enabled (ImPlotFlags_Equal set)
     * @details When enabled, one unit on X-axis equals one unit on Y-axis in screen pixels.
     *          Prevents distortion of geometric shapes (e.g., circles appear as circles, not ellipses).
     *          Particularly useful for 2D graphics, CAD applications, and map visualizations.
     * @note Corresponds to ImPlotFlags_Equal flag (direct mapping, no inversion).
     * @warning May cause plot content to be clipped if aspect ratio of data differs significantly from plot area.
     * @see setEqual()
     * \endif
     *
     * \if CHINESE
     * @fn QImPlotNode::isEqual
     * @brief 检查 X 轴和 Y 轴是否约束为等比例缩放（1:1 宽高比）
     * @return true 表示等比例缩放启用（设置了 ImPlotFlags_Equal）
     * @details 启用后，X 轴上的一个单位在屏幕像素上等于 Y 轴上的一个单位。
     *          防止几何形状失真（例如圆形显示为圆形而非椭圆）。
     *          特别适用于 2D 图形、CAD 应用和地图可视化。
     * @note 对应 ImPlotFlags_Equal 标志（直接映射，无逻辑反转）。
     * @warning 如果数据的宽高比与绘图区域差异较大，可能导致绘图内容被裁剪。
     * @see setEqual()
     * \endif
     */
    QImPlotNode_FLAG_ACCESSOR(Equal, ImPlotFlags_Equal)

    /**
     * \if ENGLISH
     * @fn QImPlotNode::isCrosshairs
     * @brief Checks if crosshair cursor replaces default cursor when hovering plot
     * @return true if crosshairs are enabled (ImPlotFlags_Crosshairs set)
     * @details When enabled, the mouse cursor changes to a crosshair (+) when hovering over the plot area.
     *          Provides precise visual feedback for coordinate reading and point selection.
     * @note Corresponds to ImPlotFlags_Crosshairs flag (direct mapping, no inversion).
     * @see setCrosshairs()
     * \endif
     *
     * \if CHINESE
     * @brief 检查悬停绘图时是否用十字线光标替换默认光标
     * @return true 表示十字线启用（设置了 ImPlotFlags_Crosshairs）
     * @details 启用后，鼠标悬停在绘图区域时会变为十字线（+）光标。
     *          为坐标读取和点选择提供精确的视觉反馈。
     * @note 对应 ImPlotFlags_Crosshairs 标志（直接映射，无逻辑反转）。
     * @see setCrosshairs()
     * \endif
     */
    QImPlotNode_FLAG_ACCESSOR(Crosshairs, ImPlotFlags_Crosshairs)

    /**
     * \if ENGLISH
     * @brief Checks if plot renders in canvas-only mode (minimal UI decorations)
     * @return true if canvas mode is enabled (ImPlotFlags_CanvasOnly NOT set)
     * @details Canvas mode disables multiple UI elements simultaneously:
     *          - Title (ImPlotFlags_NoTitle)
     *          - Legend (ImPlotFlags_NoLegend)
     *          - Context menus (ImPlotFlags_NoMenus)
     *          - Box selection (ImPlotFlags_NoBoxSelect)
     *          - Mouse coordinate display (ImPlotFlags_NoMouseText)
     *          Frame (ImPlotFlags_NoFrame) and inputs (ImPlotFlags_NoInputs) are NOT affected.
     * @note Corresponds to ImPlotFlags_CanvasOnly flag (combination of 5 NoXX flags) with inverted logic:
     *       enabled = flag NOT set (all decorations visible),
     *       disabled = flag set (all 5 decorations hidden).
     * @warning Setting canvasEnabled=false does NOT affect frameEnabled or inputsEnabled properties.
     *          To completely hide all UI elements, also set frameEnabled=false and inputsEnabled=false.
     * @see setCanvasEnabled(), setFrameEnabled(), setInputsEnabled()
     * \endif
     *
     * \if CHINESE
     * @brief 检查绘图是否以纯画布模式渲染（最小化 UI 装饰）
     * @return true 表示画布模式启用（未设置 ImPlotFlags_CanvasOnly）
     * @details 画布模式同时禁用多个 UI 元素：
     *          - 标题（ImPlotFlags_NoTitle）
     *          - 图例（ImPlotFlags_NoLegend）
     *          - 上下文菜单（ImPlotFlags_NoMenus）
     *          - 框选（ImPlotFlags_NoBoxSelect）
     *          - 鼠标坐标显示（ImPlotFlags_NoMouseText）
     *          框架（ImPlotFlags_NoFrame）和输入（ImPlotFlags_NoInputs）**不受影响**。
     * @note 对应 ImPlotFlags_CanvasOnly 标志（5 个 NoXX 标志的组合），逻辑反转：
     *       启用 = 未设置标志位（所有装饰可见），
     *       禁用 = 设置标志位（隐藏全部 5 个装饰）。
     * @warning 设置 canvasEnabled=false **不会**影响 frameEnabled 或 inputsEnabled 属性。
     *          要完全隐藏所有 UI 元素，还需设置 frameEnabled=false 和 inputsEnabled=false。
     * @see setCanvasEnabled(), setFrameEnabled(), setInputsEnabled()
     * \endif
     */
    bool QImPlotNode::isCanvasEnabled() const
{
    QIM_DC(d);
    return (d->plotFlags & ImPlotFlags_CanvasOnly) == 0;
}

/**
 * \if ENGLISH
 * @brief Enables/disables canvas-only mode (hides multiple UI decorations simultaneously)
 * @param enabled true to show all decorations, false to hide title/legend/menus/box-select/mouse-text
 * @details Convenience method that sets/clears all 5 component flags of ImPlotFlags_CanvasOnly:
 *          - enabled=true: clears NoTitle, NoLegend, NoMenus, NoBoxSelect, NoMouseText
 *          - enabled=false: sets NoTitle, NoLegend, NoMenus, NoBoxSelect, NoMouseText
 *          Frame and inputs flags remain unchanged.
 * @note More efficient than setting each decoration flag individually.
 *       Does NOT affect ImPlotFlags_NoFrame or ImPlotFlags_NoInputs.
 * @see isCanvasEnabled(), setFrameEnabled(), setInputsEnabled()
 * \endif
 *
 * \if CHINESE
 * @brief 启用/禁用纯画布模式（同时隐藏多个 UI 装饰）
 * @param enabled true 显示所有装饰，false 隐藏标题/图例/菜单/框选/鼠标文本
 * @details 便捷方法，设置/清除 ImPlotFlags_CanvasOnly 的全部 5 个组件标志：
 *          - enabled=true：清除 NoTitle、NoLegend、NoMenus、NoBoxSelect、NoMouseText
 *          - enabled=false：设置 NoTitle、NoLegend、NoMenus、NoBoxSelect、NoMouseText
 *          框架和输入标志保持不变。
 * @note 比单独设置每个装饰标志更高效。
 *       **不会**影响 ImPlotFlags_NoFrame 或 ImPlotFlags_NoInputs。
 * @see isCanvasEnabled(), setFrameEnabled(), setInputsEnabled()
 * \endif
 */
void QImPlotNode::setCanvasEnabled(bool enabled)
{
    QIM_D(d);
    const ImPlotFlags oldFlags = d->plotFlags;
    if (enabled) {
        d->plotFlags &= ~ImPlotFlags_CanvasOnly;  // 清除所有5个组件标志
    } else {
        d->plotFlags |= ImPlotFlags_CanvasOnly;  // 设置所有5个组件标志
    }
    if (d->plotFlags != oldFlags) {
        Q_EMIT plotFlagChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Returns the raw ImPlotFlags bitmask value for direct ImPlot API usage
 * @return Current combined flags as ImPlotFlags bitmask
 * @details Provides direct access to the underlying ImPlotFlags bitmask stored internally.
 * @see setPlotFlags(), ImPlot::BeginPlot()
 * \endif
 *
 * \if CHINESE
 * @brief 返回原始 ImPlotFlags 位掩码值，用于直接调用 ImPlot API
 * @return 当前组合标志的 ImPlotFlags 位掩码
 * @details 提供对内部存储的 ImPlotFlags 位掩码的直接访问。
 * @see setPlotFlags()
 * \endif
 */
int QImPlotNode::imPlotFlags() const
{
    QIM_DC(d);
    return d->plotFlags;
}

/**
 * \if ENGLISH
 * @brief Sets the raw ImPlotFlags bitmask value with signal emission on change
 * @param flags New combined flags as ImPlotFlags bitmask
 * @details Directly replaces the internal flag storage with the provided bitmask value.
 *          All individual property states (titleEnabled, legendEnabled, etc.) are conceptually
 *          updated to reflect the new flags value. Emits plotFlagChanged() signal ONLY if the
 *          new value differs from current value (checked via != comparison).
 * @note This method bypasses individual property validation logic but maintains signal emission
 *       for reactive UI updates. Prefer individual property setters for type-safe configuration.
 * @warning May set invalid flag combinations that would be rejected by individual setters
 *          (e.g., CanvasOnly with partial component flags set).
 * @see plotFlags(), setTitleEnabled(), setCanvasEnabled()
 * \endif
 *
 * \if CHINESE
 * @brief 设置原始 ImPlotFlags 位掩码值，值变更时触发信号
 * @param flags 新的组合标志 ImPlotFlags 位掩码
 * @details 直接用提供的位掩码值替换内部标志存储。
 *          所有单独的属性状态（titleEnabled、legendEnabled 等）在概念上都会更新以反映新标志值。
 *          仅当新值与当前值不同时（通过 != 比较），才会触发 plotFlagChanged() 信号。
 * @note 此方法绕过单独的属性验证逻辑，但保留信号发射以支持响应式 UI 更新。
 *       为类型安全配置，建议优先使用单独的属性设置器。
 * @warning 可能设置会被单独设置器拒绝的无效标志组合
 *          （例如 CanvasOnly 与部分组件标志同时设置）。
 * @see plotFlags()
 * \endif
 */
void QImPlotNode::setImPlotFlags(int flags)
{
    QIM_D(d);
    if (d->plotFlags != flags) {
        d->plotFlags = flags;
        Q_EMIT plotFlagChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Adds a plot item node as child of this plot
 * @param[in] item Pointer to QImPlotItemNode to add
 * @details Convenience method that delegates to addChildNode(). The item becomes a child
 *          of this plot node and will be rendered within the plot area.
 * \endif
 *
 * \if CHINESE
 * @brief 添加绘图项节点为此绘图的子节点
 * @param[in] item 要添加的 QImPlotItemNode 指针
 * @details 便捷方法，委托给 addChildNode()。该项成为此绘图节点的子节点，
 *          并将在绘图区域内渲染。
 * \endif
 */
void QImPlotNode::addPlotItem(QImPlotItemNode* item)
{
    addChildNode(item);
}

/**
 * \if ENGLISH
 * @brief Gets all plot item child nodes
 * @return List of QImPlotItemNode pointers that are children of this plot
 * @details Finds all direct and indirect child nodes that are QImPlotItemNode type.
 * \endif
 *
 * \if CHINESE
 * @brief 获取所有绘图项子节点
 * @return 此绘图子节点的 QImPlotItemNode 指针列表
 * @details 查找所有直接和间接的 QImPlotItemNode 类型子节点。
 * \endif
 */
QList< QImPlotItemNode* > QImPlotNode::plotItemNodes() const
{
    return findChildrenNodes< QImPlotItemNode* >();
}

/**
 * \if ENGLISH
 * @brief Adds a line item node to this plot
 * @param[in] lineItem Pointer to QImPlotLineItemNode to add
 * @details Convenience method that delegates to addPlotItem().
 * \endif
 *
 * \if CHINESE
 * @brief 添加线图项节点到此绘图
 * @param[in] lineItem 要添加的 QImPlotLineItemNode 指针
 * @details 便捷方法，委托给 addPlotItem()。
 * \endif
 */
void QImPlotNode::addLine(QImPlotLineItemNode* lineItem)
{
    addPlotItem(lineItem);
}

/**
 * \if ENGLISH
 * @brief Gets the internal legend node
 * @return Pointer to QImPlotLegendNode that controls legend appearance and behavior
 * \endif
 *
 * \if CHINESE
 * @brief 获取内部图例节点
 * @return 控制图例外观和行为的 QImPlotLegendNode 指针
 * \endif
 */
QImPlotLegendNode* QImPlotNode::legendNode() const
{
    QIM_DC(d);
    return d->legendNode.get();
}

/**
 * \if ENGLISH
 * @brief Checks if the mouse cursor is hovering over the plot area
 * @return true if the plot area is currently hovered
 * @details Requires valid ImPlot context (after beginDraw). Returns false if plot pointer is null.
 * \endif
 *
 * \if CHINESE
 * @brief 检查鼠标光标是否悬停在绘图区域上方
 * @return true 表示绘图区域当前被悬停
 * @details 需要有效的 ImPlot 上下文（beginDraw 之后）。如果 plot 指针为空则返回 false。
 * \endif
 */
bool QImPlotNode::isPlotHovered() const
{
    QIM_DC(d);
    return d->plot && d->plot->Hovered;
}

/**
 * \if ENGLISH
 * @brief Converts screen pixel coordinates to plot data coordinates
 * @param[in] screenX X coordinate in screen pixels
 * @param[in] screenY Y coordinate in screen pixels
 * @return QPointF containing plot data coordinates, or empty QPointF if no valid plot context
 * @details Uses the current X/Y axis pair for conversion. Requires valid ImPlot context.
 * \endif
 *
 * \if CHINESE
 * @brief 将屏幕像素坐标转换为绘图数据坐标
 * @param[in] screenX 屏幕像素中的 X 坐标
 * @param[in] screenY 屏幕像素中的 Y 坐标
 * @return 包含绘图数据坐标的 QPointF，无有效绘图上下文时返回空 QPointF
 * @details 使用当前 X/Y 轴对进行转换。需要有效的 ImPlot 上下文。
 * \endif
 */
QPointF QImPlotNode::pixelsToPlot(const float& screenX, const float& screenY)
{
    QIM_D(d);
    if (!d->plot) {
        return QPointF();
    }
    ImPlotAxis& x_axis = d->plot->Axes[ d->plot->CurrentX ];
    ImPlotAxis& y_axis = d->plot->Axes[ d->plot->CurrentY ];
    return QPointF(x_axis.PixelsToPlot(screenX), y_axis.PixelsToPlot(screenY));
}

/**
 * \if ENGLISH
 * @brief Converts plot data coordinates to screen pixel coordinates
 * @param[in] doubleX X coordinate in plot data space
 * @param[in] doubleY Y coordinate in plot data space
 * @return QPointF containing screen pixel coordinates, or empty QPointF if no valid plot context
 * @details Uses the current X/Y axis pair for conversion. Requires valid ImPlot context.
 * \endif
 *
 * \if CHINESE
 * @brief 将绘图数据坐标转换为屏幕像素坐标
 * @param[in] doubleX 绘图数据空间中的 X 坐标
 * @param[in] doubleY 绘图数据空间中的 Y 坐标
 * @return 包含屏幕像素坐标的 QPointF，无有效绘图上下文时返回空 QPointF
 * @details 使用当前 X/Y 轴对进行转换。需要有效的 ImPlot 上下文。
 * \endif
 */
QPointF QImPlotNode::plotToPixels(const double& doubleX, const double& doubleY)
{
    QIM_D(d);
    if (!d->plot) {
        return QPointF();
    }
    ImPlotAxis& x_axis = d->plot->Axes[ d->plot->CurrentX ];
    ImPlotAxis& y_axis = d->plot->Axes[ d->plot->CurrentY ];
    return QPointF(x_axis.PlotToPixels(doubleX), y_axis.PlotToPixels(doubleY));
}

/**
 * \if ENGLISH
 * @brief Formats a value using the axis tick format
 * @param[in] val Value to format
 * @param[in] xAxisId Axis ID whose format to use (Auto uses current axis)
 * @return Formatted string for the value
 * @details Uses ImPlot's internal axis formatting. Falls back to std::to_string if no valid context.
 * \endif
 *
 * \if CHINESE
 * @brief 使用坐标轴刻度格式格式化数值
 * @param[in] val 要格式化的值
 * @param[in] xAxisId 使用哪个轴的格式（Auto 使用当前轴）
 * @return 数值的格式化字符串
 * @details 使用 ImPlot 内部坐标轴格式化。无有效上下文时回退为 std::to_string。
 * \endif
 */
std::string QImPlotNode::axisValueText(double val, QImPlotAxisId xAxisId) const
{
    QIM_DC(d);
    ImPlotContext* context = ImPlot::GetCurrentContext();
    if (!context || !context->CurrentPlot) {
        return std::to_string(val);
    }
    ImPlotPlot* currentPlot = context->CurrentPlot;
    char buffer[ 128 ]        = { 0 };
    ImAxis idx = currentPlot->CurrentX;
    if (xAxisId != QImPlotAxisId::Auto) {
        idx = static_cast< ImAxis >(toImAxis(xAxisId));
    }
    if (idx < 0 || idx >= ImAxis_COUNT) {
        return std::to_string(val);
    }
    const ImPlotAxis* axisptr = currentPlot->Axes + idx;
    if (!axisptr) {
        return std::to_string(val);
    }
    ImPlot::LabelAxisValue(*axisptr, val, buffer, sizeof(buffer), false);
    return std::string(buffer);
}

/**
 * \if ENGLISH
 * @brief Requests axes to auto-fit to data on next render
 * @details Sets the axesToFit flag which triggers ImPlot::SetNextAxesToFit() during the next beginDraw().
 *          The flag is consumed after one render frame.
 * \endif
 *
 * \if CHINESE
 * @brief 请求坐标轴在下次渲染时自动适配数据
 * @details 设置 axesToFit 标志，在下一次 beginDraw() 时触发 ImPlot::SetNextAxesToFit()。
 *          标志在渲染一帧后即被消耗。
 * \endif
 */
void QImPlotNode::rescaleAxes()
{
    QIM_D(d);
    d->axesToFit = true;
}

/**
 * \if ENGLISH
 * @brief Alias for rescaleAxes() - requests axes to auto-fit on next render
 * \endif
 *
 * \if CHINESE
 * @brief rescaleAxes() 的别名 - 请求坐标轴在下次渲染时自动适配
 * \endif
 */
void QImPlotNode::setAxesToFit()
{
    QIM_D(d);
    d->axesToFit = true;
}

/**
 * \if ENGLISH
 * @brief Begins plot rendering by calling ImPlot::BeginPlot()
 * @return true (always returns true to allow style cleanup even on failure)
 * @details Handles auto-fit request, sets title/size/flags, renders all axes, and configures legend.
 *          The UTF-8 title is passed directly to ImPlot to avoid per-frame string conversion.
 * \endif
 *
 * \if CHINESE
 * @brief 通过调用 ImPlot::BeginPlot() 开始绘图渲染
 * @return true（始终返回 true 以允许样式清理，即使失败时）
 * @details 处理自动适配请求，设置标题/尺寸/标志，渲染所有坐标轴，并配置图例。
 *          UTF-8 标题直接传递给 ImPlot 以避免每帧字符串转换。
 * \endif
 */
bool QImPlotNode::beginDraw()
{
    QIM_D(d);
    // 功能
    if (d->axesToFit.is_dirty() && d->axesToFit.value()) {
        d->axesToFit = false;
        d->axesToFit.mark_clean();
        ImPlot::SetNextAxesToFit();
    }
    const char* title   = (d->titleUtf8->isEmpty() ? nullptr : d->titleUtf8->constData());
    d->beginPlotSuccess = ImPlot::BeginPlot(title, d->size, d->plotFlags);
    if (!d->beginPlotSuccess) {
        // 不成功也返回true，因为有些样式的推入或colormap需要pop出来
        return true;
    }
    d->plot = ImPlot::GetCurrentPlot();
    // 构建坐标轴
    d->renderAllAxis();
    ImPlot::SetupLegend(ImPlotLocation_East);
    return true;
}

/**
 * \if ENGLISH
 * @brief Ends plot rendering by calling ImPlot::EndPlot()
 * @details Only calls EndPlot() if beginPlot() was successful (beginPlotSuccess flag is true).
 * \endif
 *
 * \if CHINESE
 * @brief 通过调用 ImPlot::EndPlot() 结束绘图渲染
 * @details 仅在 beginPlot() 成功时（beginPlotSuccess 标志为 true）调用 EndPlot()。
 * \endif
 */
void QImPlotNode::endDraw()
{
    QIM_D(d);
    if (d->beginPlotSuccess) {
        ImPlot::EndPlot();
    }
}

}  // namespace QIM
