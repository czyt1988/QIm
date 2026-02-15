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

QImPlotNode::QImPlotNode(QObject* parent) : QImAbstractNode(parent), QIM_PIMPL_CONSTRUCT
{
    setObjectName(QStringLiteral("PlotNode"));
}

QImPlotNode::QImPlotNode(const QString& title, QObject* parent) : QImPlotNode(parent)
{
    setTitle(title);
}

QImPlotNode::~QImPlotNode()
{
}

// === 标题 ===
QString QImPlotNode::title() const
{
    QIM_DC(d);
    return QString::fromUtf8(d->titleUtf8.value());
}

void QImPlotNode::setTitle(const QString& title)
{
    QIM_D(d);
    d->titleUtf8 = title.toUtf8();
    if (d->titleUtf8.is_dirty()) {
        Q_EMIT titleChanged(title);
    }
}

// === 尺寸 ===
QSizeF QImPlotNode::size() const
{
    QIM_DC(d);
    return toQSizeF(d->size);
}

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

bool QImPlotNode::isAutoSize() const
{
    QIM_DC(d);
    return d->autoSize;
}

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

QImPlotAxisInfo* QImPlotNode::axisInfo(QImPlotAxisId aid) const
{
    QIM_DC(d);
    return d->axisInfo[ static_cast< int >(aid) ];
}

QImPlotAxisInfo* QImPlotNode::x1Axis() const
{
    return d_ptr->axisInfo[ static_cast< int >(QImPlotAxisId::X1) ];
}

QImPlotAxisInfo* QImPlotNode::y1Axis() const
{
    return d_ptr->axisInfo[ static_cast< int >(QImPlotAxisId::Y1) ];
}

QImPlotAxisInfo* QImPlotNode::x2Axis() const
{
    return d_ptr->axisInfo[ static_cast< int >(QImPlotAxisId::X2) ];
}

QImPlotAxisInfo* QImPlotNode::y2Axis() const
{
    return d_ptr->axisInfo[ static_cast< int >(QImPlotAxisId::Y2) ];
}

QImPlotAxisInfo* QImPlotNode::x3Axis() const
{
    return d_ptr->axisInfo[ static_cast< int >(QImPlotAxisId::X3) ];
}

QImPlotAxisInfo* QImPlotNode::y3Axis() const
{
    return d_ptr->axisInfo[ static_cast< int >(QImPlotAxisId::Y3) ];
}

bool QImPlotNode::isAxisEnabled(QImPlotAxisId aid) const
{
    return (axisInfo(aid) != nullptr);
}

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
 * \if
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
        emit plotFlagChanged();
    }
}

void QImPlotNode::addPlotItem(QImPlotItemNode* item)
{
    addChildNode(item);
}

QList< QImPlotItemNode* > QImPlotNode::plotItemNodes() const
{
    return findChildrenNodes< QImPlotItemNode* >();
}

void QImPlotNode::addLine(QImPlotLineItemNode* lineItem)
{
    addPlotItem(lineItem);
}

QImPlotLegendNode* QImPlotNode::legendNode() const
{
    return d_ptr->legendNode.get();
}

bool QImPlotNode::isPlotHovered() const
{
    return d_ptr->plot->Hovered;
}

QPointF QImPlotNode::pixelsToPlot(const float& screenX, const float& screenY)
{
    ImPlotAxis& x_axis = d_ptr->plot->Axes[ d_ptr->plot->CurrentX ];
    ImPlotAxis& y_axis = d_ptr->plot->Axes[ d_ptr->plot->CurrentY ];
    return QPointF(x_axis.PixelsToPlot(screenX), y_axis.PixelsToPlot(screenY));
}

QPointF QImPlotNode::plotToPixels(const double& doubleX, const double& doubleY)
{
    ImPlotAxis& x_axis = d_ptr->plot->Axes[ d_ptr->plot->CurrentX ];
    ImPlotAxis& y_axis = d_ptr->plot->Axes[ d_ptr->plot->CurrentY ];
    return QPointF(x_axis.PlotToPixels(doubleX), y_axis.PlotToPixels(doubleY));
}

std::string QImPlotNode::axisValueText(double val, QImPlotAxisId xAxisId) const
{
    QIM_DC(d);
    if (!d->plot) {
        return std::to_string(val);
    }
    char buffer[ 128 ]        = { 0 };
    const ImPlotAxis* axisptr = d->imPlotAxis(xAxisId);
    ImPlot::LabelAxisValue(*axisptr, val, buffer, sizeof(buffer), false);
    return std::string(buffer);
}

void QImPlotNode::rescaleAxes()
{
    d_ptr->axesToFit = true;
}

void QImPlotNode::setAxesToFit()
{
    d_ptr->axesToFit = true;
}

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
    if (!(d->plot)) {
        d->plot = ImPlot::GetCurrentPlot();
    }
    // 构建坐标轴
    d->renderAllAxis();
    ImPlot::SetupLegend(ImPlotLocation_East);
    return true;
}

void QImPlotNode::endDraw()
{
    if (d_ptr->beginPlotSuccess) {
        ImPlot::EndPlot();
    }
}

}  // namespace QIM
