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

QImWidgetNode::QImWidgetNode(QObject* parent) : QImAbstractNode(parent), QIM_PIMPL_CONSTRUCT
{
}

QImWidgetNode::~QImWidgetNode()
{
}

QString QImWidgetNode::windowTitle() const
{
    QIM_DC(d);
    return QString::fromUtf8(d->windowTitleUtf8);
}

void QImWidgetNode::setWindowTitle(const QString& title)
{
    QIM_D(d);
    const QByteArray utf8 = title.toUtf8();
    if (d->windowTitleUtf8 != utf8) {
        d->windowTitleUtf8 = utf8;
        Q_EMIT windowTitleChanged(title);
    }
}

QPoint QImWidgetNode::pos() const
{
    QIM_DC(d);
    return QPoint(
        d->pos.x == FLT_MAX ? -1 : static_cast< int >(d->pos.x), d->pos.y == FLT_MAX ? -1 : static_cast< int >(d->pos.y)
    );
}

void QImWidgetNode::setPos(const QPoint& pos)
{
    QIM_D(d);
    ImVec2 newPos(pos.x() >= 0 ? static_cast< float >(pos.x()) : FLT_MAX, pos.y() >= 0 ? static_cast< float >(pos.y()) : FLT_MAX);

    if (d->pos.x != newPos.x || d->pos.y != newPos.y) {
        d->pos = newPos;
    }
}

QSize QImWidgetNode::size() const
{
    QIM_DC(d);
    return QSize(d->size.x > 0 ? static_cast< int >(d->size.x) : -1, d->size.y > 0 ? static_cast< int >(d->size.y) : -1);
}

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

QSize QImWidgetNode::minimumSize() const
{
    QIM_DC(d);
    return QSize(
        d->minimumSize.x > 0 ? static_cast< int >(d->minimumSize.x) : 0,
        d->minimumSize.y > 0 ? static_cast< int >(d->minimumSize.y) : 0
    );
}

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

void QImWidgetNode::setContentsMargins(const QMarginsF& margins)
{
    // ImGui 的 WindowPadding 是对称的（左右相同，上下相同）
    // 我们取 left/top 作为 padding 值
    const float paddingX = static_cast< float >(margins.left());
    const float paddingY = static_cast< float >(margins.top());
    setContentsMargins(paddingX, paddingY);
}

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
 * @brief 判断当前窗口是否收起状态
 *
 * @note 这个函数必须在setCollapseEnabled 设置为true时起作用
 * @return
 */
bool QImWidgetNode::isWidgetCollapsed() const
{
    return d_ptr->isWidgetCollapsed;
}

// === 语义化窗口标志（单一标志位操作）===

bool QImWidgetNode::isTitleBarEnabled() const
{
    QIM_DC(d);
    return !(d->windowFlags & ImGuiWindowFlags_NoTitleBar);
}

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

bool QImWidgetNode::isResizable() const
{
    QIM_DC(d);
    return !(d->windowFlags & ImGuiWindowFlags_NoResize);
}

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

bool QImWidgetNode::isMovable() const
{
    QIM_DC(d);
    return !(d->windowFlags & ImGuiWindowFlags_NoMove);
}

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

bool QImWidgetNode::isScrollbarEnabled() const
{
    QIM_DC(d);
    return !(d->windowFlags & ImGuiWindowFlags_NoScrollbar);
}

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

bool QImWidgetNode::isCollapseEnabled() const  // Qt 风格：isCollapsible
{
    QIM_DC(d);
    return !(d->windowFlags & ImGuiWindowFlags_NoCollapse);
}

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

bool QImWidgetNode::isBackgroundEnabled() const
{
    QIM_DC(d);
    return !(d->windowFlags & ImGuiWindowFlags_NoBackground);
}

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

bool QImWidgetNode::isResizeToContents() const
{
    QIM_DC(d);
    return d->windowFlags & ImGuiWindowFlags_AlwaysAutoResize;
}

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

bool QImWidgetNode::noBringToFrontOnFocus() const
{
    QIM_DC(d);
    return d->windowFlags & ImGuiWindowFlags_NoBringToFrontOnFocus;
}

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

bool QImWidgetNode::noFocusOnAppearing() const
{
    QIM_DC(d);
    return d->windowFlags & ImGuiWindowFlags_NoFocusOnAppearing;
}

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

bool QImWidgetNode::noNav() const
{
    QIM_DC(d);
    return d->windowFlags & ImGuiWindowFlags_NoNav;
}

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

void QImWidgetNode::setFitToGLViewPort(bool fitWidth, bool fitHeight)
{
    d_ptr->fitWidthToGlViewPort  = fitWidth;
    d_ptr->fitHeightToGlViewPort = fitHeight;
}

bool QImWidgetNode::isWidthFitToGLViewPort() const
{
    return d_ptr->fitWidthToGlViewPort;
}

bool QImWidgetNode::isHeightFitToGLViewPort() const
{
    return d_ptr->fitHeightToGlViewPort;
}

// === Qt 风格状态查询 ===
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

    d->isWidgetCollapsed =
        ImGui::Begin(d->windowTitleUtf8.isEmpty() ? "##Widget" : d->windowTitleUtf8.constData(), nullptr, d->windowFlags);
    // 这里永远返回true，imgui的begin返回的是是否收起状态，ImGui::Begin无论如何也要匹配ImGui::End
    return true;
}

void QImWidgetNode::endDraw()
{
    QIM_D(d);
    // imgui的begin返回的是是否收起状态，ImGui::Begin无论如何也要匹配ImGui::End
    ImGui::End();
    // 匹配 PushStyleVar
    for (std::size_t i = 0; i < d->styleVars.size(); ++i) {
        ImGui::PopStyleVar();
    }
}

/**
 * @brief 对Enable/Disabled状态的开始绘制
 *  * isDisable==true时，默认调用ImGui::BeginDisabled();
 *  * @param isDisable
 */
void QImWidgetNode::beginDisabled(bool isDisable)
{
    if (isDisable) {
        ImGui::BeginDisabled();
    }
}

/**
 * @brief 对Enable/Disabled状态的结束绘制
 *  * @param isDisable
 */
void QImWidgetNode::endDisabled(bool isDisable)
{
    if (isDisable) {
        ImGui::EndDisabled();
    }
}

}  // namespace QIM
