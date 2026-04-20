#include "QImWidget.h"
// Qt
#include <QTimer>
#include <QColor>
#include <QEvent>
#include <QDebug>
#include <QApplication>
#include <QElapsedTimer>
#include <QFileInfo>
// imguis
#include "QtImGui.h"
#include "imgui.h"
#include "imgui_internal.h"
// QIm
#include "QImTrackedValue.hpp"
#include "QImFontFileHelper.h"
#include "QtImGuiUtils.h"
#include "QImWidgetNode.h"
#include "QImAbstractNode.h"
namespace QIM
{
class QImWidget::PrivateData
{
    QIM_DECLARE_PUBLIC(QImWidget)
public:
    PrivateData(QImWidget* p);
    ~PrivateData();
    //
    void reloadFontFile();
    //
    void updateFontGlyphRanges();
    //
    void applyRenderMode();
    //
    bool needDemandUpdate() const;
    bool needStartContinuousTimer() const;
    //
    bool shouldUseHighFPS() const;
    //
    void adaptiveTimer();

public:
    //----------------------------------------------------
    // render about
    //----------------------------------------------------
    QTimer* timer { nullptr };
    QElapsedTimer paintElapsed;
    QtImGui::RenderRef imguiRenderRef { nullptr };  ///< 专门针对此窗口的上下文
    QColor backgroundColor { Qt::white };           ///< 记录背景颜色
    RenderMode renderMode { RenderAdaptive };
    int minRenderInterval { 16 };  ///< 最小渲染间隔(ms)，对应约60FPS
    int highFPSInterval { 55 };  ///< 高帧率间隔(ms)，对应 18FPS 用于持续渲染模式
    int lowFPSInterval { 1000 }; ///< 低帧率间隔(ms)，对应 1FPS 用于自适应渲染模式
    //----------------------------------------------------
    // font about
    //----------------------------------------------------
    std::vector< std::string > fontFiles;
    bool isNeedAddFont { true };
    ImGuiContext* imguiContext { nullptr };
    float fontSize { 16.0 };
    ImVector< ImWchar > fontGlyphRanges;
    GlyphRangesFlags fontRangeFlag { GlyphRangesDefault | GlyphRangesChineseSimplifiedCommon };
    //----------------------------------------------------
    // theme about
    //----------------------------------------------------
    QImTrackedValue< StyleColorsTheme > styleColorTheme { StyleLight };
    //----------------------------------------------------
    // node about
    //----------------------------------------------------
    std::unique_ptr< QImAbstractNode > rootRenderNode;  ///< 渲染根节点
    //----------------------------------------------------
    // debug
    //----------------------------------------------------
#ifdef QIM_ENABLE_DEBUG_PRINT_FPS
    QElapsedTimer fpsIntervalTimer;  ///< 测量1秒间隔的计时器
    QElapsedTimer frameTimer;        ///< 测量单帧时间（用于诊断卡顿）
    int frameCount { 0 };            ///< 当前统计周期内的帧数
    float currentFPS { 0.0f };       ///< 瞬时FPS
    float smoothedFPS { 0.0f };      ///< 平滑FPS（EMA）
    float minFPS { 1000.0f };        ///< 最小观测FPS
    float maxFPS { 0.0f };           ///< 最大观测FPS
    float lastFrameTimeMs { 0.0f };  ///< 上一帧耗时（ms），用于诊断

    static constexpr int SMOOTHING_FRAMES       = 10;
    static constexpr int FPS_UPDATE_INTERVAL_MS = 1000;  ///< FPS更新间隔(1秒)
    void updateFPSStatistics();
    void resetFPSStatistics();
    void drawFPSToast();
#endif
};

QImWidget::PrivateData::PrivateData(QImWidget* p) : q_ptr(p)
{
    timer = new QTimer(p);
    timer->setInterval(highFPSInterval);  // 18FPS
    paintElapsed.restart();
    // 字体相关初始化
    std::string fontpath = QImFontFileHelper::getRecommendedChineseFontPath();
    if (!fontpath.empty() && QFileInfo::exists(QString::fromStdString(fontpath))) {
        this->fontFiles.emplace_back(fontpath);
        this->isNeedAddFont = true;
    } else {
        this->isNeedAddFont = false;
    }
    this->fontSize = 16;
    GlyphRangesFlags f;
    f.setFlag(GlyphRangesDefault);
    f.setFlag(GlyphRangesChineseSimplifiedCommon);

    // 主题
    if (isSystemDarkTheme()) {
        styleColorTheme = StyleDark;
    } else {
        styleColorTheme = StyleLight;
    }
    styleColorTheme.mark_dirty();
    QImWidgetNode* imwidgetNode = new QImWidgetNode();
    imwidgetNode->setAutoIdEnabled(false);  /// 顶层窗口无需自动id
    imwidgetNode->setFitToGLViewPort(true, true);
    imwidgetNode->setToFrameLess(true);
    rootRenderNode.reset(imwidgetNode);
    applyRenderMode();
}

QImWidget::PrivateData::~PrivateData()
{
}

void QImWidget::PrivateData::reloadFontFile()
{
    // get font file
    this->fontFiles.clear();

    //! 不能直接按照Qt返回的字体来设置，默认返回simsunb.ttf
    //! simsunb.ttf 是 SimSun-ExtB（宋体-扩展B），它是 Windows 系统自带的扩展字符集专用字体，主要用于显示 Unicode 扩展B区（CJK Extension B）的生僻字（如古籍、人名中的罕见汉字），不包含常用简体汉字
    //! Qt 的 QApplication::font() 返回的是应用程序当前使用的逻辑字体，但这个字体名称/路径可能被系统字体映射机制重定向。
    QImFontFileHelper::preloadCommonFonts();
    QFont font                 = QApplication::font();
    const QList< QString > ffs = QImFontFileHelper::getFontFiles(font);
    for (const QString& f : ffs) {
        if (f.isEmpty()) {
            continue;
        }
        if (!QFileInfo::exists(f)) {
            continue;
        }
        this->fontFiles.emplace_back(f.toStdString());
        qDebug() << "font file path=" << f;
    }
    this->fontSize      = QImFontFileHelper::getFontPixelSize(font);
    this->isNeedAddFont = !this->fontFiles.empty();
    qDebug() << "font name=" << font << ",fontSize=" << this->fontSize;
}

void QImWidget::PrivateData::updateFontGlyphRanges()
{
    if (!imguiContext) {
        return;
    }
    ImGuiIO& io = imguiContext->IO;
    ImFontGlyphRangesBuilder builder;
    // 总是添加默认范围（ASCII + Latin）
    const ImWchar* defaultRanges = io.Fonts->GetGlyphRangesDefault();
    builder.AddRanges(defaultRanges);
    // 根据标志添加对应语言范围
    if (fontRangeFlag.testFlag(GlyphRangesGreek)) {
        const ImWchar* range = io.Fonts->GetGlyphRangesGreek();
        builder.AddRanges(range);
    }
    if (fontRangeFlag.testFlag(GlyphRangesCyrillic)) {
        const ImWchar* range = io.Fonts->GetGlyphRangesCyrillic();
        builder.AddRanges(range);
    }
    if (fontRangeFlag.testFlag(GlyphRangesThai)) {
        const ImWchar* range = io.Fonts->GetGlyphRangesThai();
        builder.AddRanges(range);
    }

    if (fontRangeFlag.testFlag(GlyphRangesVietnamese)) {
        const ImWchar* range = io.Fonts->GetGlyphRangesVietnamese();
        builder.AddRanges(range);
    }
    if (fontRangeFlag.testFlag(GlyphRangesChineseSimplifiedCommon)) {
        const ImWchar* range = io.Fonts->GetGlyphRangesChineseSimplifiedCommon();
        builder.AddRanges(range);
    }

    if (fontRangeFlag.testFlag(GlyphRangesChineseFull)) {
        // 完整中文（基本区 + 扩展区A）
        const ImWchar* range = io.Fonts->GetGlyphRangesChineseFull();
        builder.AddRanges(range);
    }

    if (fontRangeFlag.testFlag(GlyphRangesJapanese)) {
        const ImWchar* range = io.Fonts->GetGlyphRangesJapanese();
        builder.AddRanges(range);
    }

    if (fontRangeFlag.testFlag(GlyphRangesKorean)) {
        const ImWchar* range = io.Fonts->GetGlyphRangesKorean();
        builder.AddRanges(range);
    }

    // 构建最终的 ranges
    builder.BuildRanges(&(this->fontGlyphRanges));
}

void QImWidget::PrivateData::applyRenderMode()
{
    switch (renderMode) {
    case RenderContinuous:
        // 持续渲染
        timer->setInterval(highFPSInterval);
        if (!timer->isActive()) {
            timer->start();
        }
        break;

    case RenderOnDemand:
        // 停止固定定时器，仅靠事件触发
        timer->stop();
        if (!q_ptr->hasMouseTracking()) {
            q_ptr->setMouseTracking(true);
        }
        break;

    case RenderAdaptive:
        // 启动自适应调度（初始低帧率）
        timer->setInterval(lowFPSInterval);
        if (!timer->isActive()) {
            timer->start();
        }
        break;
    }
}

bool QImWidget::PrivateData::needDemandUpdate() const
{
    return ((renderMode == RenderOnDemand) || (renderMode == RenderAdaptive));
}

bool QImWidget::PrivateData::needStartContinuousTimer() const
{
    return ((renderMode == RenderContinuous) || (renderMode == RenderAdaptive));
}

bool QImWidget::PrivateData::shouldUseHighFPS() const
{
    if (!imguiContext) {
        return false;
    }

    const ImGuiIO& io    = imguiContext->IO;
    const qint64 elapsed = paintElapsed.elapsed();

    // 以下任一条件满足即需高帧率
    return (elapsed < lowFPSInterval) &&           // 1秒内有交互
           (io.WantCaptureMouse ||                // ImGui捕获鼠标
            io.WantCaptureKeyboard ||             // ImGui捕获键盘
            io.WantTextInput ||                   // 文本输入中
            io.NavActive ||                       // 导航激活
            (std::abs(io.MouseDelta.x) > 0.1f ||  // 鼠标移动
             std::abs(io.MouseDelta.y) > 0.1f));
}

void QImWidget::PrivateData::adaptiveTimer()
{
    // ===== Adaptive 模式智能帧率调度 =====
    if (renderMode == RenderAdaptive && timer->isActive()) {
        const bool needHighFPS   = shouldUseHighFPS();
        const int targetInterval = needHighFPS ? highFPSInterval : lowFPSInterval;

        // 仅当间隔变化时调整（避免频繁setInterval开销）
        if (targetInterval != timer->interval()) {
#ifdef QIM_ENABLE_DEBUG_PRINT_FPS
            int oldinterval = timer->interval();
#endif
            timer->setInterval(targetInterval);
#ifdef QIM_ENABLE_DEBUG_PRINT_FPS
            qDebug() << "change refresh timer interval from " << oldinterval << " to " << targetInterval;
#endif
        }
    }
}

#ifdef QIM_ENABLE_DEBUG_PRINT_FPS

void QImWidget::PrivateData::updateFPSStatistics()
{
    // ===== 1. 测量单帧耗时（用于诊断卡顿）=====
    if (frameTimer.isValid()) {
        lastFrameTimeMs = frameTimer.restart();  // restart() 返回上次到现在的ms数
    } else {
        frameTimer.start();
        lastFrameTimeMs = 0.0f;
    }

    // ===== 2. 累计帧数 =====
    ++frameCount;

    // ===== 3. 每秒更新一次FPS =====
    if (!fpsIntervalTimer.isValid()) {
        fpsIntervalTimer.start();  // 首次初始化
    }

    const qint64 elapsedMs = fpsIntervalTimer.elapsed();
    if (elapsedMs >= FPS_UPDATE_INTERVAL_MS) {
        // 精确计算：考虑实际经过的时间（非整1000ms）
        currentFPS = static_cast< float >(frameCount) * 1000.0f / elapsedMs;

        // 更新极值（过滤异常值）
        if (currentFPS >= 0.1f && currentFPS <= 1000.0f) {
            minFPS = qMin(minFPS, currentFPS);
            maxFPS = qMax(maxFPS, currentFPS);
        }

        // 指数移动平均平滑
        if (smoothedFPS == 0.0f) {
            smoothedFPS = currentFPS;
        } else {
            constexpr float alpha = 2.0f / (SMOOTHING_FRAMES + 1);
            smoothedFPS           = alpha * currentFPS + (1.0f - alpha) * smoothedFPS;
        }

        // 重置计数器
        frameCount = 0;
        fpsIntervalTimer.restart();  // 重置1秒计时器
    }
}

void QImWidget::PrivateData::resetFPSStatistics()
{
    frameCount      = 0;
    minFPS          = 1000.0f;
    maxFPS          = 0.0f;
    smoothedFPS     = 0.0f;
    lastFrameTimeMs = 0.0f;
    fpsIntervalTimer.invalidate();
    frameTimer.invalidate();
}

void QImWidget::PrivateData::drawFPSToast()
{
    if (!imguiContext)
        return;

    // 关键：使用 ForegroundDrawList 确保绝对顶层
    ImDrawList* draw_list = ImGui::GetForegroundDrawList();
    if (!draw_list)
        return;

    // ===== 1. 构建显示文本 =====
    char fps_text[ 64 ];
    snprintf(fps_text, sizeof(fps_text), "FPS: %.1f", smoothedFPS);

    char time_text[ 32 ];
    snprintf(time_text, sizeof(time_text), "(%.2f ms)", lastFrameTimeMs);

    const char* mode_str = "Unknown";
    switch (renderMode) {
    case RenderContinuous:
        mode_str = "Continuous";
        break;
    case RenderOnDemand:
        mode_str = "OnDemand";
        break;
    case RenderAdaptive:
        mode_str = "Adaptive";
        break;
    }

    // ===== 2. 颜色计算 =====
    // FPS 颜色：绿(≥30) → 蓝(≥15) → 红(<15)
    ImVec4 fps_color;
    if (smoothedFPS >= 30.0f)
        fps_color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);  // 绿
    else if (smoothedFPS >= 15.0f)
        fps_color = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // 蓝（比纯蓝更易读）
    else
        fps_color = ImVec4(1.0f, 0.3f, 0.3f, 1.0f);  // 红

    // 帧时间颜色：绿(≤16.7ms) → 蓝(≤33.3ms) → 红(>33.3ms)
    ImVec4 time_color;
    if (lastFrameTimeMs <= 16.7f)
        time_color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
    else if (lastFrameTimeMs <= 33.3f)
        time_color = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);
    else
        time_color = ImVec4(1.0f, 0.3f, 0.3f, 1.0f);

    ImVec4 mode_color(0.85f, 0.85f, 0.85f, 1.0f);  // 灰色

    // ===== 3. 布局计算 =====
    const ImVec2 pos(10.0f, 10.0f);  // 固定左上角位置
    const float line_height   = ImGui::GetTextLineHeight();
    const float spacing       = 3.0f;
    const float corner_radius = 4.0f;
    const float padding       = 6.0f;

    // 计算文本尺寸
    ImVec2 fps_size  = ImGui::CalcTextSize(fps_text);
    ImVec2 time_size = ImGui::CalcTextSize(time_text);
    ImVec2 mode_size = ImGui::CalcTextSize(mode_str);

    // 背景区域尺寸（包含所有文本）
    float total_width  = std::max(fps_size.x + spacing + time_size.x, mode_size.x);
    float total_height = line_height * 2 + spacing;

    // ===== 4. 绘制半透明背景 =====
    ImU32 bg_color = ImGui::GetColorU32(ImVec4(0.08f, 0.08f, 0.08f, 0.15f));  // 深灰半透明
    draw_list->AddRectFilled(
        ImVec2(pos.x - padding, pos.y - padding),
        ImVec2(pos.x + total_width + padding, pos.y + total_height + padding),
        bg_color,
        corner_radius
    );

    // ===== 5. 绘制文本 =====
    // FPS 数值（主文本）
    draw_list->AddText(ImVec2(pos.x, pos.y), ImGui::GetColorU32(fps_color), fps_text);

    // 帧时间（右侧对齐）
    draw_list->AddText(ImVec2(pos.x + fps_size.x + spacing, pos.y), ImGui::GetColorU32(time_color), time_text);

    // 渲染模式（第二行）
    draw_list->AddText(ImVec2(pos.x, pos.y + line_height + spacing), ImGui::GetColorU32(mode_color), mode_str);

    // ===== 6. 可选：添加微光边框（提升可读性）=====
    ImU32 border_color = ImGui::GetColorU32(ImVec4(0.3f, 0.3f, 0.3f, 0.5f));
    draw_list->AddRect(
        ImVec2(pos.x - padding, pos.y - padding),
        ImVec2(pos.x + total_width + padding, pos.y + total_height + padding),
        border_color,
        corner_radius,
        ImDrawFlags_RoundCornersAll,
        1.0f
    );
}
#endif

//----------------------------------------------------
// QImWidget
//----------------------------------------------------

/**
 * \if ENGLISH
 * @brief Constructor with optional parent widget and window flags
 * @param[in] parent Parent QWidget
 * @param[in] f Qt window flags
 * \endif
 * \if CHINESE
 * @brief 构造函数（带可选父窗口和窗口标志）
 * @param[in] parent 父 QWidget
 * @param[in] f Qt 窗口标志
 * \endif
 */
QImWidget::QImWidget(QWidget* parent, Qt::WindowFlags f) : QOpenGLWidget(parent, f), QIM_PIMPL_CONSTRUCT
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
QImWidget::~QImWidget()
{
}

/**
 * \if ENGLISH
 * @brief Sets the rendering mode that controls frame rate behavior
 * @param[in] mode RenderContinuous (18FPS), RenderOnDemand (event-driven), or RenderAdaptive (smart switching)
 * \endif
 * \if CHINESE
 * @brief 设置控制帧率行为的渲染模式
 * @param[in] mode RenderContinuous（18FPS）、RenderOnDemand（事件驱动）或 RenderAdaptive（智能切换）
 * \endif
 */
void QImWidget::setRenderMode(RenderMode mode)
{
    QIM_D(d);
    d->renderMode = mode;
    d->applyRenderMode();
}

/**
 * \if ENGLISH
 * @brief Gets the current rendering mode
 * @return Current RenderMode
 * \endif
 * \if CHINESE
 * @brief 获取当前渲染模式
 * @return 当前 RenderMode
 * \endif
 */
QImWidget::RenderMode QImWidget::renderMode() const
{
    QIM_DC(d);
    return d->renderMode;
}

/**
 * \if ENGLISH
 * @brief Sets the refresh interval for continuous rendering mode
 * @param[in] ms Refresh interval in milliseconds
 * @see setRenderMode
 * \endif
 * \if CHINESE
 * @brief 设置刷新间隔，仅对 RenderContinuous 模式有效
 * @param[in] ms 刷新间隔（毫秒）
 * @see setRenderMode
 * \endif
 */
void QImWidget::setRefreshInterval(int ms)
{
    QIM_D(d);
    d->highFPSInterval = ms;
    d->timer->setInterval(ms);
}

/**
 * \if ENGLISH
 * @brief Gets the current refresh interval
 * @return Refresh interval in milliseconds
 * \endif
 * \if CHINESE
 * @brief 获取当前刷新间隔
 * @return 刷新间隔（毫秒）
 * \endif
 */
int QImWidget::refreshInterval() const
{
    return d_ptr->highFPSInterval;
}

/**
 * \if ENGLISH
 * @brief Requests an immediate render update
 * @details Triggers Qt's update() to schedule a repaint on the next frame
 * \endif
 * \if CHINESE
 * @brief 请求立即渲染更新
 * @details 触发 Qt 的 update() 以在下一帧安排重绘
 * \endif
 */
void QImWidget::requestRender()
{
    update();  // 触发 Qt 的重新绘制
}

/**
 * \if ENGLISH
 * @brief Gets the minimum render interval between consecutive frames
 * @return Minimum interval in milliseconds
 * \endif
 * \if CHINESE
 * @brief 获取连续帧之间的最小渲染间隔
 * @return 最小间隔（毫秒）
 * \endif
 */
int QImWidget::minRenderInterval() const
{
    QIM_DC(d);
    return d->minRenderInterval;
}

/**
 * \if ENGLISH
 * @brief Sets the minimum render interval between consecutive frames
 * @param[in] min Minimum interval in milliseconds (0 means no limit)
 * \endif
 * \if CHINESE
 * @brief 设置连续帧之间的最小渲染间隔
 * @param[in] min 最小间隔（毫秒，0 表示无限制）
 * \endif
 */
void QImWidget::setMinRenderInterval(int min)
{
    QIM_D(d);
    min = qMax(0, min);
    d->minRenderInterval = min;
}

/**
 * \if ENGLISH
 * @brief Sets the glyph ranges flags for font loading
 * @param[in] ranges Combination of GlyphRangesFlags (Default, Chinese, Japanese, etc.)
 * @details Triggers font reload on next paint cycle
 * \endif
 * \if CHINESE
 * @brief 设置字体加载的字形范围标志
 * @param[in] ranges GlyphRangesFlags 的组合（默认、中文、日文等）
 * @details 在下次绘制周期触发字体重新加载
 * \endif
 */
void QImWidget::setFontGlyphRanges(GlyphRangesFlags ranges)
{
    QIM_D(d);
    d->fontRangeFlag = ranges;
    d->isNeedAddFont = true;
}

/**
 * \if ENGLISH
 * @brief Sets the ImGui color theme style
 * @param[in] style Theme to apply (StyleDark, StyleLight, or StyleClassic)
 * \endif
 * \if CHINESE
 * @brief 设置 ImGui 颜色主题
 * @param[in] style 要应用的主题（StyleDark、StyleLight 或 StyleClassic）
 * \endif
 */
void QImWidget::setStyleColorsTheme(StyleColorsTheme style)
{
    QIM_D(d);
    d->styleColorTheme = style;
}

/**
 * \if ENGLISH
 * @brief Adds a render node as a child of the root render node
 * @param[in] node Node to add (QImWidget manages its lifetime)
 * @note QImWidget manages QImAbstractNode lifetime; nodes are deleted when the widget is destroyed
 * @details Render nodes are executed before the virtual drawIM() method
 * \endif
 * \if CHINESE
 * @brief 添加渲染节点作为根渲染节点的子节点
 * @param[in] node 要添加的节点（QImWidget 管理其生命周期）
 * @note QImWidget 管理 QImAbstractNode 的生命周期，窗口销毁时会删除节点对象，无需手动 delete
 * @details 渲染节点在虚函数 drawIM() 之前执行
 * \endif
 */
void QImWidget::addRenderNode(QImAbstractNode* node)
{
    QIM_D(d);
    if (d->rootRenderNode) {
        d->rootRenderNode->addChildNode(node);
    }
}

/**
 * \if ENGLISH
 * @brief Gets all render nodes attached to the root render node
 * @return List of child QImAbstractNode pointers
 * \endif
 * \if CHINESE
 * @brief 获取所有附加到根渲染节点的渲染节点
 * @return 子 QImAbstractNode 指针列表
 * \endif
 */
QList< QImAbstractNode* > QImWidget::renderNodeList() const
{
    QIM_DC(d);
    if (d->rootRenderNode) {
        return d->rootRenderNode->childrenNodes();
    }
    return {};
}

/**
 * \if ENGLISH
 * @brief Removes and deletes a render node from the root render node
 * @param[in] node Node to remove (will be deleted)
 * @note This function performs delete on the node
 * \endif
 * \if CHINESE
 * @brief 从根渲染节点移除并删除渲染节点
 * @param[in] node 要移除的节点（将被 delete）
 * @note 此函数会对节点进行 delete 操作
 * \endif
 */
void QImWidget::removeRenderNode(QImAbstractNode* node)
{
    QIM_D(d);
    if (d->rootRenderNode) {
        d->rootRenderNode->removeChildNode(node);
    }
}

/**
 * \if ENGLISH
 * @brief Takes (extracts) a render node without deleting it
 * @param[in] node Node to extract
 * @return true if the node was successfully taken (ownership transferred to caller)
 * \endif
 * \if CHINESE
 * @brief 抽取渲染节点（不删除）
 * @param[in] node 要抽取的节点
 * @return true 表示成功抽取（所有权转移给调用者）
 * \endif
 */
bool QImWidget::takeRenderNode(QImAbstractNode* node)
{
    if (!node) {
        return false;
    }
    QIM_D(d);
    return d->rootRenderNode->takeChildNode(node);
}

/**
 * \if ENGLISH
 * @brief Gets the current ImGui color theme
 * @return Current StyleColorsTheme
 * \endif
 * \if CHINESE
 * @brief 获取当前 ImGui 颜色主题
 * @return 当前 StyleColorsTheme
 * \endif
 */
QImWidget::StyleColorsTheme QImWidget::styleColorsTheme() const
{
    QIM_DC(d);
    return d->styleColorTheme.value();
}

/**
 * \if ENGLISH
 * @brief Gets the current font glyph ranges flags
 * @return Current GlyphRangesFlags configuration
 * \endif
 * \if CHINESE
 * @brief 获取当前字体字形范围标志
 * @return 当前 GlyphRangesFlags 配置
 * \endif
 */
QImWidget::GlyphRangesFlags QImWidget::fontGlyphRangesFlag() const
{
    QIM_DC(d);
    return d->fontRangeFlag;
}

/**
 * \if ENGLISH
 * @brief Initializes OpenGL context and ImGui for this widget
 * @note Subclasses must explicitly call QImWidget::initializeGL() to ensure ImGui initialization
 * @details Creates a per-widget ImGui render context and connects the timer for periodic updates
 * \endif
 * \if CHINESE
 * @brief 初始化 OpenGL 和 ImGui 上下文
 * @note 子类必须显式调用 QImWidget::initializeGL() 以确保 ImGui 初始化
 * @details 为此窗口创建独立的 ImGui 渲染上下文，并连接定时器进行周期性更新
 * \endif
 */
void QImWidget::initializeGL()
{
    QIM_D(d);
    initializeOpenGLFunctions();
    d->imguiRenderRef = QtImGui::initialize(this, false);  // 这里每个窗口一个上下文，必须传入false
    d->imguiContext   = ImGui::GetCurrentContext();
    connect(d->timer, &QTimer::timeout, this, qOverload<>(&QWidget::update));
}

void QImWidget::paintGL()
{
    QIM_D(d);
    // 检查是否需要渲染
    // if (d->lastRenderTime.isValid()) {
    //     if (d->lastRenderTime.elapsed() < d->minRenderInterval) {
    //         return;  // 跳过渲染
    //     }
    // }
    d->adaptiveTimer();
#ifdef QIM_ENABLE_DEBUG_PRINT_FPS
    d->updateFPSStatistics();
#endif
    if (d->imguiRenderRef && d->isNeedAddFont) {
        // 为imgui加载字体
        if (!d->fontFiles.empty() && d->imguiContext) {
            ImGuiIO& io = d->imguiContext->IO;
            io.Fonts->ClearFonts();
            io.Fonts->AddFontDefault();  // 先加载默认英文字体
            if (d->fontGlyphRanges.empty()) {
                d->updateFontGlyphRanges();
            }
            for (const std::string& p : std::as_const(d->fontFiles)) {
                if (p.empty() || !QFileInfo::exists(QString::fromStdString(p))) {
                    qWarning() << "skip invalid font file path:" << QString::fromStdString(p);
                    continue;
                }
                ImFontConfig config;
                config.MergeMode = true;  // 合并模式
                io.Fonts->AddFontFromFileTTF(
                    p.c_str(),  //"C:/WINDOWS/Fonts/msyh.ttc"
                    d->fontSize,
                    &config,
                    d->fontGlyphRanges.Data  // 字符集编码位置
                );
            }
            // 加载完字体后必须重建字体纹理
            io.Fonts->Build();
        }
        d->isNeedAddFont = false;
    }
    drawBackground();
    beforeRenderImNodes();
    if (d->imguiRenderRef) {
        QtImGui::newFrame(d->imguiRenderRef);  // 内部会适配当前屏幕大小和鼠标位置，并最后执行newFrame
        d->rootRenderNode->render();
    }
    afterRenderImNodes();
    if (d->imguiRenderRef) {
        ImGui::Render();
        QtImGui::render(d->imguiRenderRef);
    }
    // 重置计时
    d->paintElapsed.restart();
}

void QImWidget::changeEvent(QEvent* e)
{
    if (e->type() == QEvent::FontChange) {
        QIM_D(d);
        // 字体已更改
        d->reloadFontFile();
    } else if (e->type() == QEvent::WindowStateChange) {
        QIM_D(d);
        if (windowState() & Qt::WindowMinimized) {
            // 最小化：所有模式停止渲染（极致节能）
            if (d->timer->isActive()) {
                d->timer->stop();
            }
        } else {
            // 恢复窗口
            if (d->needStartContinuousTimer()) {
                d->timer->start();
            }
            // OnDemand: 保持停止状态，等待下次交互
        }
    }
    QOpenGLWidget::changeEvent(e);  // 一定要调用基类实现
}

bool QImWidget::event(QEvent* e)
{
    QIM_D(d);
    // 这些事件触发渲染
    switch (e->type()) {
    case QEvent::MouseMove:
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
    case QEvent::MouseButtonDblClick:
    case QEvent::Enter:  // Mouse enters window
    case QEvent::Wheel:
    case QEvent::KeyPress:
    case QEvent::KeyRelease:
    case QEvent::FocusIn:
    case QEvent::FocusOut:
    case QEvent::HoverMove:
    case QEvent::Resize:
    case QEvent::Leave:
        if (d->needDemandUpdate()) {
            requestRender();
        }
        break;
    case QEvent::WindowActivate:
    case QEvent::WindowDeactivate:
        requestRender();
        break;
    case QEvent::FontChange:
    case QEvent::StyleChange:
        if (d->needDemandUpdate()) {
            requestRender();
        }
        break;
    case QEvent::Hide:
        d->timer->stop();
        break;

    case QEvent::Show:
        if (d->needStartContinuousTimer()) {
            d->timer->start();
        }
        break;
    default:
        break;
    }

    return QOpenGLWidget::event(e);
}

/**
 * \if ENGLISH
 * @brief Draws the background color to prevent ImGui window ghosting
 * @details Clears the OpenGL color buffer with the configured backgroundColor
 * \endif
 * \if CHINESE
 * @brief 绘制背景色以避免 ImGui 窗口重影
 * @details 使用配置的 backgroundColor 清除 OpenGL 颜色缓冲区
 * \endif
 */
void QImWidget::drawBackground()
{
    QIM_D(d);
    //
    glViewport(0, 0, devicePixelRatio() * width(), devicePixelRatio() * height());
    glClearColor(d->backgroundColor.redF(), d->backgroundColor.greenF(), d->backgroundColor.blueF(), d->backgroundColor.alphaF());
    glClear(GL_COLOR_BUFFER_BIT);
}

/**
 * \if ENGLISH
 * @brief Hook called before ImGui node rendering tree is executed
 * @note QImWidget::beforeRenderImNodes() handles background clearing and theme switching.
 *       If overridden, explicitly call QImWidget::beforeRenderImNodes() unless you
 *       intentionally skip theme application.
 * @details Applies the ImGui style theme if it has been marked dirty since last render
 * \endif
 * \if CHINESE
 * @brief 在 ImGui 节点渲染树执行之前调用的钩子
 * @note QImWidget::beforeRenderImNodes() 处理背景清除和主题切换。
 *       如果重写此函数，建议显式调用 QImWidget::beforeRenderImNodes()，
 *       除非明确不需要应用主题。
 * @details 若主题在上次渲染后被标记为 dirty，则应用 ImGui 样式主题
 * \endif
 */
void QImWidget::beforeRenderImNodes()
{
    QIM_D(d);
    if (d->styleColorTheme.is_dirty()) {
        switch (d->styleColorTheme.get_and_clear()) {
        case StyleDark:
            ImGui::StyleColorsDark();
            break;
        case StyleLight:
            ImGui::StyleColorsLight();
            break;
        case StyleClassic:
            ImGui::StyleColorsClassic();
            break;
        default:
            break;
        }
    }
}

/**
 * \if ENGLISH
 * @brief Hook called after all render nodes have been executed, before ImGui::Render()
 * @details If your widget has no render nodes, you can use this method to draw
 *          ImGui windows directly. Example:
 * @code
 * void MyWidget::afterRenderImNodes() {
 *     if (ImGui::Begin("My Window", nullptr, flags)) {
 *         // custom ImGui drawing here
 *         ImGui::End();
 *     }
 * }
 * @endcode
 * @note You can also draw in beforeRenderImNodes(), but must call
 *       QImWidget::beforeRenderImNodes() first to ensure theme is applied.
 * \endif
 * \if CHINESE
 * @brief 在所有渲染节点执行完毕后、ImGui::Render() 之前调用的钩子
 * @details 如果你的窗口没有任何渲染节点，可以将此函数作为执行 ImGui 渲染的入口。例如：
 * @code
 * void MyWidget::afterRenderImNodes() {
 *     if (ImGui::Begin("My Window", nullptr, flags)) {
 *         // 在此处自定义 ImGui 绘制
 *         ImGui::End();
 *     }
 * }
 * @endcode
 * @note 也可以在 beforeRenderImNodes() 中执行 ImGui 绘制，但必须先调用
 *       QImWidget::beforeRenderImNodes() 以确保主题生效。
 * \endif
 */
void QImWidget::afterRenderImNodes()
{
#ifdef QIM_ENABLE_DEBUG_PRINT_FPS
    QIM_D(d);
    d->drawFPSToast();
#endif
}

/**
 * \if ENGLISH
 * @brief Replaces the root render node with a new one
 * @param[in] node New root render node (ownership transferred)
 * \endif
 * \if CHINESE
 * @brief 替换根渲染节点
 * @param[in] node 新的根渲染节点（所有权转移）
 * \endif
 */
void QImWidget::resetRootRenderNode(QImAbstractNode* node)
{
    QIM_D(d);
    d->rootRenderNode.reset(node);
}

}  // end namespace QIM
