#include "QImWidgetNode.h"
#include "imgui.h"
#include "QtImGuiUtils.h"
namespace QIM
{

class QImWidgetNode::PrivateData
{
public:
    QIM_DECLARE_PUBLIC(QImWidgetNode)
    explicit PrivateData(QImWidgetNode* q);

    // UTF-8 缓存
    QByteArray windowTitleUtf8;

    // 位置/大小（FLT_MAX 表示未设置）
    ImVec2 pos         = ImVec2(0, 0);
    ImVec2 size        = ImVec2(-1, -1);  // size小于0的为窗口尺寸
    ImVec2 minimumSize = ImVec2(0, 0);

    //
    bool fitWidthToGlViewPort { true };   ///< 是否宽度填充opengl区域
    bool fitHeightToGlViewPort { true };  ///< 是否高度填充opengl区域
    // 单一窗口标志位
    ImGuiWindowFlags windowFlags = 0;
    bool isWidgetCollapsed {
        false
    };  ///< 由于这里无论是否开启窗口成功，都要设置样式，因此这里的beginDraw永远成功，但要通过此变量记录是否ImGui::Begin成功
    // 样式变量栈
    struct StyleVar
    {
        StyleVar(const ImGuiStyleVar& v1, const ImVec2& v2) : idx(v1), value(v2)
        {
        }
        ImGuiStyleVar idx;
        ImVec2 value;
    };
    std::vector< StyleVar > styleVars;

    // 窗口状态缓存
    mutable ImVec2 contentMin = ImVec2(0, 0);
    mutable ImVec2 contentMax = ImVec2(0, 0);
};

QImWidgetNode::PrivateData::PrivateData(QImWidgetNode* q) : q_ptr(q)
{
    // 默认启用标准窗口特性
    windowFlags = ImGuiWindowFlags_None;

    // 设置默认样式（匹配 ImGui 默认值）
    styleVars.emplace_back(ImGuiStyleVar_WindowPadding, ImVec2(1.0f, 1.0f));
    styleVars.emplace_back(ImGuiStyleVar_WindowMinSize, ImVec2(32.0f, 32.0f));
}

// === Qt 风格基本属性实现 ===

/**
 * \if ENGLISH
 * @brief Constructor with optional parent
 * @param[in] parent Parent QObject
 * \endif
 * \if CHINESE
 * @brief 构造函数（带可选父对象）
 * @param[in] parent 父对象
 * \endif
 */
QImWidgetNode::QImWidgetNode(QObject* parent) : QImAbstractNode(parent), QIM_PIMPL_CONSTRUCT
{
}

/**
 * \if ENGLISH
 * @brief Destructor
 * \endif
 * \if CHINESE
 * @brief 析构函数
 * \endif
 */
QImWidgetNode::~QImWidgetNode()
{
}

/**
 * \if ENGLISH
 * @brief Gets the ImGui window title
 * @return Current window title as QString
 * \endif
 * \if CHINESE
 * @brief 获取 ImGui 窗口标题
 * @return 当前窗口标题（QString）
 * \endif
 */
QString QImWidgetNode::windowTitle() const
{
    QIM_DC(d);
    return QString::fromUtf8(d->windowTitleUtf8);
}

/**
 * \if ENGLISH
 * @brief Sets the ImGui window title
 * @param[in] title New window title
 * \endif
 * \if CHINESE
 * @brief 设置 ImGui 窗口标题
 * @param[in] title 新窗口标题
 * \endif
 */
void QImWidgetNode::setWindowTitle(const QString& title)
{
    QIM_D(d);
    const QByteArray utf8 = title.toUtf8();
    if (d->windowTitleUtf8 != utf8) {
        d->windowTitleUtf8 = utf8;
        Q_EMIT windowTitleChanged(title);
    }
}

/**
 * \if ENGLISH
 * @brief Gets the window position
 * @return Window position as QPoint (-1 means unset)
 * \endif
 * \if CHINESE
 * @brief 获取窗口位置
 * @return 窗口位置（QPoint），-1 表示未设置
 * \endif
 */
QPoint QImWidgetNode::pos() const
{
    QIM_DC(d);
    return QPoint(
        d->pos.x == FLT_MAX ? -1 : static_cast< int >(d->pos.x), d->pos.y == FLT_MAX ? -1 : static_cast< int >(d->pos.y)
    );
}

/**
 * \if ENGLISH
 * @brief Sets the window position
 * @param[in] pos New position (-1 coordinates mean unset, mapped to FLT_MAX)
 * \endif
 * \if CHINESE
 * @brief 设置窗口位置
 * @param[in] pos 新位置（-1 坐标表示未设置，映射为 FLT_MAX）
 * \endif
 */
void QImWidgetNode::setPos(const QPoint& pos)
{
    QIM_D(d);
    ImVec2 newPos(pos.x() >= 0 ? static_cast< float >(pos.x()) : FLT_MAX, pos.y() >= 0 ? static_cast< float >(pos.y()) : FLT_MAX);

    if (d->pos.x != newPos.x || d->pos.y != newPos.y) {
        d->pos = newPos;
    }
}

/**
 * \if ENGLISH
 * @brief Gets the window size
 * @return Window size as QSize (-1 dimensions mean unset/auto)
 * \endif
 * \if CHINESE
 * @brief 获取窗口尺寸
 * @return 窗口尺寸（QSize），-1 表示未设置/自动
 * \endif
 */
QSize QImWidgetNode::size() const
{
    QIM_DC(d);
    return QSize(d->size.x > 0 ? static_cast< int >(d->size.x) : -1, d->size.y > 0 ? static_cast< int >(d->size.y) : -1);
}

/**
 * \if ENGLISH
 * @brief Sets the window size
 * @param[in] size New size (0 or negative dimensions are treated as unset)
 * \endif
 * \if CHINESE
 * @brief 设置窗口尺寸
 * @param[in] size 新尺寸（0 或负值视为未设置）
 * \endif
 */
void QImWidgetNode::setSize(const QSize& size)
{
    QIM_D(d);
    ImVec2 newSize(
        size.width() > 0 ? static_cast< float >(size.width()) : 0.0f,
        size.height() > 0 ? static_cast< float >(size.height()) : 0.0f
    );

    if (!fuzzyEqual(d->size, newSize)) {
        d->size = newSize;
    }
}

/**
 * \if ENGLISH
 * @brief Gets the minimum window size
 * @return Minimum size as QSize
 * \endif
 * \if CHINESE
 * @brief 获取窗口最小尺寸
 * @return 最小尺寸（QSize）
 * \endif
 */
QSize QImWidgetNode::minimumSize() const
{
    QIM_DC(d);
    return QSize(
        d->minimumSize.x > 0 ? static_cast< int >(d->minimumSize.x) : 0,
        d->minimumSize.y > 0 ? static_cast< int >(d->minimumSize.y) : 0
    );
}

/**
 * \if ENGLISH
 * @brief Sets the minimum window size via ImGui WindowMinSize style variable
 * @param[in] size New minimum size
 * \endif
 * \if CHINESE
 * @brief 通过 ImGui WindowMinSize 样式变量设置窗口最小尺寸
 * @param[in] size 新最小尺寸
 * \endif
 */
void QImWidgetNode::setMinimumSize(const QSize& size)
{
    QIM_D(d);

    // 查找或添加 WindowMinSize 样式
    auto it = std::find_if(d->styleVars.begin(), d->styleVars.end(), [](const PrivateData::StyleVar& var) {
        return var.idx == ImGuiStyleVar_WindowMinSize;
    });

    ImVec2 newSize(
        size.width() > 0 ? static_cast< float >(size.width()) : 0.0f,
        size.height() > 0 ? static_cast< float >(size.height()) : 0.0f
    );

    if (it != d->styleVars.end()) {
        if (!fuzzyEqual(it->value, newSize)) {
            it->value      = newSize;
            d->minimumSize = newSize;
        }
    } else {
        d->styleVars.emplace_back(ImGuiStyleVar_WindowMinSize, newSize);
        d->minimumSize = newSize;
    }
}

// === Qt 风格 contentsMargins ===

/**
 * \if ENGLISH
 * @brief Gets the window content margins (mapped from ImGui WindowPadding)
 * @return Margins as QMarginsF (left/top/right/bottom), defaults to (8,8,8,8)
 * @details ImGui WindowPadding is symmetric, so left=right and top=bottom.
 * \endif
 * \if CHINESE
 * @brief 获取窗口内容边距（从 ImGui WindowPadding 映射）
 * @return 边距（QMarginsF：左/上/右/下），默认值为 (8,8,8,8)
 * @details ImGui WindowPadding 是对称的，因此左=右、上=下。
 * \endif
 */
QMarginsF QImWidgetNode::contentsMargins() const
{
    QIM_DC(d);
    // 查找 WindowPadding 样式
    for (const auto& var : std::as_const(d->styleVars)) {
        if (var.idx == ImGuiStyleVar_WindowPadding) {
            return QMarginsF(var.value.x, var.value.y, var.value.x, var.value.y);
        }
    }
    // 默认值
    return QMarginsF(8.0, 8.0, 8.0, 8.0);
}

/**
 * \if ENGLISH
 * @brief Sets the window content margins via QMarginsF
 * @param[in] margins New margins (only left and top are used due to ImGui symmetry)
 * \endif
 * \if CHINESE
 * @brief 通过 QMarginsF 设置窗口内容边距
 * @param[in] margins 新边距（因 ImGui 对称性，仅使用 left 和 top）
 * \endif
 */
void QImWidgetNode::setContentsMargins(const QMarginsF& margins)
{
    // ImGui 的 WindowPadding 是对称的（左右相同，上下相同）
    // 我们取 left/top 作为 padding 值
    const float paddingX = static_cast< float >(margins.left());
    const float paddingY = static_cast< float >(margins.top());
    setContentsMargins(paddingX, paddingY);
}

/**
 * \if ENGLISH
 * @brief Sets the window content margins via explicit padding values
 * @param[in] paddingX Horizontal padding (left and right)
 * @param[in] paddingY Vertical padding (top and bottom)
 * \endif
 * \if CHINESE
 * @brief 通过显式填充值设置窗口内容边距
 * @param[in] paddingX 水平填充（左和右）
 * @param[in] paddingY 垂直填充（上和下）
 * \endif
 */
void QImWidgetNode::setContentsMargins(float paddingX, float paddingY)
{
    QIM_D(d);
    auto it = std::find_if(d->styleVars.begin(), d->styleVars.end(), [](const PrivateData::StyleVar& var) {
        return var.idx == ImGuiStyleVar_WindowPadding;
    });
    ImVec2 newPadding(paddingX, paddingY);
    if (it != d->styleVars.end()) {
        if (!fuzzyEqual(it->value, newPadding)) {
            it->value = newPadding;
        }
    } else {
        d->styleVars.emplace_back(ImGuiStyleVar_WindowPadding, newPadding);
    }
}

/**
 * \if ENGLISH
 * @brief Checks whether the ImGui window is currently collapsed
 * @return true if the window is collapsed (minimized to title bar)
 * @note Only effective when collapseEnabled is set to true
 * \endif
 * \if CHINESE
 * @brief 判断当前窗口是否处于收起状态
 * @return true 如果窗口已收起（折叠为标题栏）
 * @note 此函数仅在 setCollapseEnabled 设置为 true 时有效
 * \endif
 */
bool QImWidgetNode::isWidgetCollapsed() const
{
    QIM_DC(d);
    return d->isWidgetCollapsed;
}

// === 语义化窗口标志（单一标志位操作）===

/**
 * \if ENGLISH
 * @brief Checks whether the window title bar is visible
 * @return true if title bar is enabled
 * \endif
 * \if CHINESE
 * @brief 检查窗口标题栏是否可见
 * @return true 如果标题栏已启用
 * \endif
 */
bool QImWidgetNode::isTitleBarEnabled() const
{
    QIM_DC(d);
    return !(d->windowFlags & ImGuiWindowFlags_NoTitleBar);
}

/**
 * \if ENGLISH
 * @brief Enables or disables the window title bar
 * @param[in] on true to show title bar, false to hide it
 * \endif
 * \if CHINESE
 * @brief 启用或禁用窗口标题栏
 * @param[in] on true 显示标题栏，false 隐藏标题栏
 * \endif
 */
void QImWidgetNode::setTitleBarEnabled(bool on)
{
    QIM_D(d);
    const bool current = isTitleBarEnabled();
    if (current != on) {
        if (on) {
            d->windowFlags &= ~ImGuiWindowFlags_NoTitleBar;
        } else {
            d->windowFlags |= ImGuiWindowFlags_NoTitleBar;
        }
    }
}

/**
 * \if ENGLISH
 * @brief Checks whether the window can be resized by the user
 * @return true if resizing is enabled
 * \endif
 * \if CHINESE
 * @brief 检查窗口是否允许用户调整大小
 * @return true 如果已启用窗口大小调整
 * \endif
 */
bool QImWidgetNode::isResizable() const
{
    QIM_DC(d);
    return !(d->windowFlags & ImGuiWindowFlags_NoResize);
}

/**
 * \if ENGLISH
 * @brief Enables or disables window resizing
 * @param[in] on true to allow resizing, false to disable
 * \endif
 * \if CHINESE
 * @brief 启用或禁用窗口大小调整
 * @param[in] on true 允许调整大小，false 禁止调整大小
 * \endif
 */
void QImWidgetNode::setResizable(bool on)
{
    QIM_D(d);
    const bool current = isResizable();
    if (current != on) {
        if (on) {
            d->windowFlags &= ~ImGuiWindowFlags_NoResize;
        } else {
            d->windowFlags |= ImGuiWindowFlags_NoResize;
        }
    }
}

/**
 * \if ENGLISH
 * @brief Checks whether the window can be moved by the user
 * @return true if moving is enabled
 * \endif
 * \if CHINESE
 * @brief 检查窗口是否允许用户移动
 * @return true 如果已启用窗口移动
 * \endif
 */
bool QImWidgetNode::isMovable() const
{
    QIM_DC(d);
    return !(d->windowFlags & ImGuiWindowFlags_NoMove);
}

/**
 * \if ENGLISH
 * @brief Enables or disables window movement
 * @param[in] on true to allow moving, false to disable
 * \endif
 * \if CHINESE
 * @brief 启用或禁用窗口移动
 * @param[in] on true 允许移动，false 禁止移动
 * \endif
 */
void QImWidgetNode::setMovable(bool on)
{
    QIM_D(d);
    const bool current = isMovable();
    if (current != on) {
        if (on) {
            d->windowFlags &= ~ImGuiWindowFlags_NoMove;
        } else {
            d->windowFlags |= ImGuiWindowFlags_NoMove;
        }
    }
}

/**
 * \if ENGLISH
 * @brief Checks whether the window scrollbar is visible
 * @return true if scrollbar is enabled
 * \endif
 * \if CHINESE
 * @brief 检查窗口滚动条是否可见
 * @return true 如果滚动条已启用
 * \endif
 */
bool QImWidgetNode::isScrollbarEnabled() const
{
    QIM_DC(d);
    return !(d->windowFlags & ImGuiWindowFlags_NoScrollbar);
}

/**
 * \if ENGLISH
 * @brief Enables or disables the window scrollbar
 * @param[in] on true to show scrollbar, false to hide it
 * \endif
 * \if CHINESE
 * @brief 启用或禁用窗口滚动条
 * @param[in] on true 显示滚动条，false 隐藏滚动条
 * \endif
 */
void QImWidgetNode::setScrollbarEnabled(bool on)
{
    QIM_D(d);
    const bool current = isScrollbarEnabled();
    if (current != on) {
        if (on) {
            d->windowFlags &= ~ImGuiWindowFlags_NoScrollbar;
        } else {
            d->windowFlags |= ImGuiWindowFlags_NoScrollbar;
        }
    }
}

/**
 * \if ENGLISH
 * @brief Checks whether the window collapse button is enabled
 * @return true if collapsing is enabled
 * \endif
 * \if CHINESE
 * @brief 检查窗口折叠按钮是否启用
 * @return true 如果已启用折叠功能
 * \endif
 */
bool QImWidgetNode::isCollapseEnabled() const  // Qt 风格：isCollapsible
{
    QIM_DC(d);
    return !(d->windowFlags & ImGuiWindowFlags_NoCollapse);
}

/**
 * \if ENGLISH
 * @brief Enables or disables the window collapse (minimize to title bar) button
 * @param[in] on true to enable collapsing, false to disable
 * \endif
 * \if CHINESE
 * @brief 启用或禁用窗口折叠（收起至标题栏）按钮
 * @param[in] on true 启用折叠，false 禁用折叠
 * \endif
 */
void QImWidgetNode::setCollapseEnabled(bool on)
{
    QIM_D(d);
    const bool current = isCollapseEnabled();
    if (current != on) {
        if (on) {
            d->windowFlags &= ~ImGuiWindowFlags_NoCollapse;
        } else {
            d->windowFlags |= ImGuiWindowFlags_NoCollapse;
        }
    }
}

/**
 * \if ENGLISH
 * @brief Checks whether the window background is drawn
 * @return true if background rendering is enabled
 * \endif
 * \if CHINESE
 * @brief 检查窗口是否绘制背景
 * @return true 如果背景渲染已启用
 * \endif
 */
bool QImWidgetNode::isBackgroundEnabled() const
{
    QIM_DC(d);
    return !(d->windowFlags & ImGuiWindowFlags_NoBackground);
}

/**
 * \if ENGLISH
 * @brief Enables or disables window background rendering
 * @param[in] on true to draw background, false for transparent background
 * \endif
 * \if CHINESE
 * @brief 启用或禁用窗口背景渲染
 * @param[in] on true 绘制背景，false 透明背景
 * \endif
 */
void QImWidgetNode::setBackgroundEnabled(bool on)
{
    QIM_D(d);
    const bool current = isBackgroundEnabled();
    if (current != on) {
        if (on) {
            d->windowFlags &= ~ImGuiWindowFlags_NoBackground;
        } else {
            d->windowFlags |= ImGuiWindowFlags_NoBackground;
        }
    }
}

/**
 * \if ENGLISH
 * @brief Checks whether the window auto-resizes to fit its contents
 * @return true if auto-resize (AlwaysAutoResize) is enabled
 * \endif
 * \if CHINESE
 * @brief 检查窗口是否自动调整大小以适应内容
 * @return true 如果已启用自动调整大小（AlwaysAutoResize）
 * \endif
 */
bool QImWidgetNode::isResizeToContents() const
{
    QIM_DC(d);
    return d->windowFlags & ImGuiWindowFlags_AlwaysAutoResize;
}

/**
 * \if ENGLISH
 * @brief Enables or disables auto-resize to fit content
 * @param[in] on true to enable AlwaysAutoResize, false for fixed size
 * \endif
 * \if CHINESE
 * @brief 启用或禁用自动调整大小以适应内容
 * @param[in] on true 启用 AlwaysAutoResize，false 固定大小
 * \endif
 */
void QImWidgetNode::setResizeToContents(bool on)
{
    QIM_D(d);
    const bool current = isResizeToContents();
    if (current != on) {
        if (on) {
            d->windowFlags |= ImGuiWindowFlags_AlwaysAutoResize;
        } else {
            d->windowFlags &= ~ImGuiWindowFlags_AlwaysAutoResize;
        }
    }
}

/**
 * \if ENGLISH
 * @brief Checks whether the window stays behind other windows on focus
 * @return true if NoBringToFrontOnFocus flag is set
 * \endif
 * \if CHINESE
 * @brief 检查窗口在获得焦点时是否保持在其他窗口后面
 * @return true 如果已设置 NoBringToFrontOnFocus 标志
 * \endif
 */
bool QImWidgetNode::noBringToFrontOnFocus() const
{
    QIM_DC(d);
    return d->windowFlags & ImGuiWindowFlags_NoBringToFrontOnFocus;
}

/**
 * \if ENGLISH
 * @brief Sets whether the window stays behind on focus
 * @param[in] on true to prevent bringing to front on focus, false for normal behavior
 * \endif
 * \if CHINESE
 * @brief 设置窗口在获得焦点时是否保持在后面
 * @param[in] on true 阻止获得焦点时前置，false 正常行为
 * \endif
 */
void QImWidgetNode::setNoBringToFrontOnFocus(bool on)
{
    QIM_D(d);
    const bool current = noBringToFrontOnFocus();
    if (current != on) {
        if (on) {
            d->windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
        } else {
            d->windowFlags &= ~ImGuiWindowFlags_NoBringToFrontOnFocus;
        }
    }
}

/**
 * \if ENGLISH
 * @brief Checks whether the window skips focus when first appearing
 * @return true if NoFocusOnAppearing flag is set
 * \endif
 * \if CHINESE
 * @brief 检查窗口首次出现时是否跳过获取焦点
 * @return true 如果已设置 NoFocusOnAppearing 标志
 * \endif
 */
bool QImWidgetNode::noFocusOnAppearing() const
{
    QIM_DC(d);
    return d->windowFlags & ImGuiWindowFlags_NoFocusOnAppearing;
}

/**
 * \if ENGLISH
 * @brief Sets whether the window skips focus when first appearing
 * @param[in] on true to skip focus on appearing, false for normal focus behavior
 * \endif
 * \if CHINESE
 * @brief 设置窗口首次出现时是否跳过获取焦点
 * @param[in] on true 跳过焦点，false 正常获取焦点
 * \endif
 */
void QImWidgetNode::setNoFocusOnAppearing(bool on)
{
    QIM_D(d);
    const bool current = noFocusOnAppearing();
    if (current != on) {
        if (on) {
            d->windowFlags |= ImGuiWindowFlags_NoFocusOnAppearing;
        } else {
            d->windowFlags &= ~ImGuiWindowFlags_NoFocusOnAppearing;
        }
    }
}

/**
 * \if ENGLISH
 * @brief Checks whether keyboard/gamepad navigation is disabled for this window
 * @return true if NoNav flag is set
 * \endif
 * \if CHINESE
 * @brief 检查此窗口是否禁用键盘/游戏手柄导航
 * @return true 如果已设置 NoNav 标志
 * \endif
 */
bool QImWidgetNode::noNav() const
{
    QIM_DC(d);
    return d->windowFlags & ImGuiWindowFlags_NoNav;
}

/**
 * \if ENGLISH
 * @brief Enables or disables keyboard/gamepad navigation for this window
 * @param[in] on true to disable navigation, false to enable it
 * \endif
 * \if CHINESE
 * @brief 启用或禁用此窗口的键盘/游戏手柄导航
 * @param[in] on true 禁用导航，false 启用导航
 * \endif
 */
void QImWidgetNode::setNoNav(bool on)
{
    QIM_D(d);
    const bool current = noNav();
    if (current != on) {
        if (on) {
            d->windowFlags |= ImGuiWindowFlags_NoNav;
        } else {
            d->windowFlags &= ~ImGuiWindowFlags_NoNav;
        }
    }
}

/**
 * \if ENGLISH
 * @brief Sets the window to frameless mode (no decorations: title bar, resize, scrollbar, collapse)
 * @param[in] on true for NoDecoration mode, false for normal window with all decorations
 * \endif
 * \if CHINESE
 * @brief 设置窗口为无边框模式（无装饰：标题栏、调整大小、滚动条、折叠）
 * @param[in] on true 启用 NoDecoration 模式，false 正常窗口包含所有装饰
 * \endif
 */
void QImWidgetNode::setToFrameLess(bool on)
{
    QIM_D(d);

    if (on) {
        // 移除所有装饰禁用标志        // 添加所有装饰禁用标志
        d->windowFlags = ImGuiWindowFlags_NoDecoration;
        // d->windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar
        //                   | ImGuiWindowFlags_NoCollapse;
    } else {
        d->windowFlags = ImGuiWindowFlags_None;
    }
}

/**
 * \if ENGLISH
 * @brief Sets whether the window fills the OpenGL viewport in width and/or height
 * @param[in] fitWidth true to fill viewport width
 * @param[in] fitHeight true to fill viewport height
 * @details When enabled, the window position is set to (0,0) and size to viewport size.
 * \endif
 * \if CHINESE
 * @brief 设置窗口是否在宽度和/或高度上填充 OpenGL 视口
 * @param[in] fitWidth true 填充视口宽度
 * @param[in] fitHeight true 填充视口高度
 * @details 启用后，窗口位置设为 (0,0)，尺寸设为视口尺寸。
 * \endif
 */
void QImWidgetNode::setFitToGLViewPort(bool fitWidth, bool fitHeight)
{
    QIM_D(d);
    d->fitWidthToGlViewPort  = fitWidth;
    d->fitHeightToGlViewPort = fitHeight;
}

bool QImWidgetNode::isWidthFitToGLViewPort() const
{
    QIM_DC(d);
    return d->fitWidthToGlViewPort;
}

bool QImWidgetNode::isHeightFitToGLViewPort() const
{
    QIM_DC(d);
    return d->fitHeightToGlViewPort;
}



// === Qt 风格状态查询 ===

/**
 * \if ENGLISH
 * @brief Gets the window geometry (frame rectangle)
 * @return QRect representing the window's content area bounds
 * \endif
 * \if CHINESE
 * @brief 获取窗口几何信息（框架矩形）
 * @return QRect 表示窗口内容区域的边界
 * \endif
 */
QRect QImWidgetNode::geometry() const  // Qt: geometry() = frame rect
{
    QIM_DC(d);
    return QRect(
        static_cast< int >(d->contentMin.x),
        static_cast< int >(d->contentMin.y),
        static_cast< int >(d->contentMax.x - d->contentMin.x),
        static_cast< int >(d->contentMax.y - d->contentMin.y)
    );
}

// === 核心渲染逻辑 ===

/**
 * \if ENGLISH
 * @brief Begins ImGui window rendering, applies style variables and window flags
 * @return Always returns true (ImGui::Begin must always be paired with End)
 * @details Sets window position and size, pushes style variables, and calls
 *          ImGui::Begin(). If the window is disabled, ImGui::BeginDisabled() is called.
 *          The collapsed state is tracked via isWidgetCollapsed.
 * \endif
 * \if CHINESE
 * @brief 开始 ImGui 窗口渲染，应用样式变量和窗口标志
 * @return 始终返回 true（ImGui::Begin 必须与 End 配对）
 * @details 设置窗口位置和尺寸，压入样式变量，并调用
 *          ImGui::Begin()。若窗口已禁用，则调用 ImGui::BeginDisabled()。
 *          折叠状态通过 isWidgetCollapsed 追踪。
 * \endif
 */
bool QImWidgetNode::beginDraw()
{
    QIM_D(d);

    if (d->fitWidthToGlViewPort || d->fitHeightToGlViewPort) {
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImVec2 viewport_size = ImGui::GetMainViewport()->Size;
        if (!d->fitWidthToGlViewPort) {
            viewport_size.x = d->size.x;
        }
        if (!d->fitHeightToGlViewPort) {
            viewport_size.y = d->size.y;
        }
        ImGui::SetNextWindowSize(viewport_size);
    } else {
        ImGui::SetNextWindowPos(d->pos);
        ImGui::SetNextWindowSize(d->size);
    }

    // 应用样式变量
    for (const auto& var : d->styleVars) {
        ImGui::PushStyleVar(var.idx, var.value);
    }
    if (!isEnabled()) {
        ImGui::BeginDisabled();
    }
    d->isWidgetCollapsed =
        ImGui::Begin(d->windowTitleUtf8.isEmpty() ? "##Widget" : d->windowTitleUtf8.constData(), nullptr, d->windowFlags);
    // 这里永远返回true，imgui的begin返回的是是否收起状态，ImGui::Begin无论如何也要匹配ImGui::End
    return true;
}

/**
 * \if ENGLISH
 * @brief Ends ImGui window rendering, pops style variables and restores disabled state
 * @details Calls ImGui::End(), pops all pushed style variables, and
 *          calls ImGui::EndDisabled() if the window was disabled.
 * \endif
 * \if CHINESE
 * @brief 结束 ImGui 窗口渲染，弹出样式变量并恢复禁用状态
 * @details 调用 ImGui::End()，弹出所有压入的样式变量，并在
 *          窗口已禁用时调用 ImGui::EndDisabled()。
 * \endif
 */
void QImWidgetNode::endDraw()
{
    QIM_D(d);
    // imgui的begin返回的是是否收起状态，ImGui::Begin无论如何也要匹配ImGui::End
    ImGui::End();
    if (!isEnabled()) {
        ImGui::EndDisabled();
    }
    // 匹配 PushStyleVar
    for (std::size_t i = 0; i < d->styleVars.size(); ++i) {
        ImGui::PopStyleVar();
    }
}

}  // namespace QIM
