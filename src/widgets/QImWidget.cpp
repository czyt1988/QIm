#include "QImWidget.h"
// Qt
#include <QTimer>
#include <QColor>
#include <QEvent>
#include <QDebug>
#include <QApplication>
#include <QElapsedTimer>
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
    int lowInterval { 55 };        ///< 高帧率，对应 18FPS 用于持续渲染模式
    int hightInterval { 1000 };    ///< 低帧率，对应 1FPS 用于自适应渲染模式
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
    timer->setInterval(lowInterval);  // 18FPS
    paintElapsed.restart();
    // 字体相关初始化
    std::string fontpath = QImFontFileHelper::getRecommendedChineseFontPath();
    if (fontpath.empty()) {
        this->isNeedAddFont = false;
    }
    this->fontFiles.emplace_back(fontpath);
    this->fontSize      = 16;
    this->isNeedAddFont = true;
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
        this->fontFiles.emplace_back(f.toStdString());
        qDebug() << "font file path=" << f;
    }
    this->fontSize      = QImFontFileHelper::getFontPixelSize(font);
    this->isNeedAddFont = true;
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
        timer->setInterval(lowInterval);
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
        timer->setInterval(hightInterval);
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
    return (elapsed < hightInterval) &&           // 1秒内有交互
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
        const int targetInterval = needHighFPS ? lowInterval : hightInterval;

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

QImWidget::QImWidget(QWidget* parent, Qt::WindowFlags f) : QOpenGLWidget(parent, f), QIM_PIMPL_CONSTRUCT
{
}

QImWidget::~QImWidget()
{
}

void QImWidget::setRenderMode(RenderMode mode)
{
    QIM_D(d);
    d->renderMode = mode;
    d->applyRenderMode();
}

QImWidget::RenderMode QImWidget::renderMode() const
{
    return d_ptr->renderMode;
}

/**
 * @brief 设置刷新间隔，仅对RenderContinuous模式有效
 * @param ms 间隔
 * @see setRenderMode
 */
void QImWidget::setRefreshInterval(int ms)
{
    QIM_D(d);
    d->lowInterval = ms;
    d->timer->setInterval(ms);
}

int QImWidget::refreshInterval() const
{
    return d_ptr->lowInterval;
}

void QImWidget::requestRender()
{
    update();  // 触发 Qt 的重新绘制
}

int QImWidget::minRenderInterval() const
{
    return d_ptr->minRenderInterval;
}

void QImWidget::setMinRenderInterval(int min)
{
    min                      = qMax(0, min);
    d_ptr->minRenderInterval = min;
}

/**
 * @brief 设置
 * @param ranges
 */
void QImWidget::setFontGlyphRanges(GlyphRangesFlags ranges)
{
    QIM_D(d);
    d->fontRangeFlag = ranges;
    d->isNeedAddFont = true;
}

/**
 * @brief 设置ImGui的颜色主题
 * @param style
 */
void QImWidget::setStyleColorsTheme(StyleColorsTheme style)
{
    d_ptr->styleColorTheme = style;
}

/**
 * @brief 添加渲染节点
 *
 * 渲染节点会在虚函数drawIM之前执行
 *
 * @note QImWidget会管理QImAbstractNode的生命周期，在窗口销毁的时候会销毁QImAbstractNode对象，你不需要手动进行delete
 * @param node
 */
void QImWidget::addRenderNode(QImAbstractNode* node)
{
    if (d_ptr->rootRenderNode) {
        d_ptr->rootRenderNode->addChildNode(node);
    }
}

/**
 * @brief 获取所有的渲染节点
 * @return
 */
QList< QImAbstractNode* > QImWidget::renderNodeList() const
{
    if (d_ptr->rootRenderNode) {
        return d_ptr->rootRenderNode->childrenNodes();
    }
    return {};
}

/**
 * @brief 移除节点
 * @note 此函数会对节点进行delete操作
 * @param node
 */
void QImWidget::removeRenderNode(QImAbstractNode* node)
{
    if (d_ptr->rootRenderNode) {
        d_ptr->rootRenderNode->removeChildNode(node);
    }
}

/**
 * @brief 抽取渲染节点
 * @param node
 * @return 成功返回true
 */
bool QImWidget::takeRenderNode(QImAbstractNode* node)
{
    if (!node) {
        return false;
    }
    return d_ptr->rootRenderNode->takeChildNode(node);
}

/**
 * @brief ImGui的颜色主题
 * @return
 */
QImWidget::StyleColorsTheme QImWidget::styleColorsTheme() const
{
    return d_ptr->styleColorTheme.value();
}

QImWidget::GlyphRangesFlags QImWidget::fontGlyphRangesFlag() const
{
    QIM_DC(d);
    return d->fontRangeFlag;
}

/**
 * @brief 初始化GL，重写此方法理论上都应该调用QImWidget::initializeGL()进行imgui的初始化，否则会异常
 *
 * @note 子类都应该显示调用QImWidget::initializeGL()以确保Imgui的初始化
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
    // 这些事件触发渲染
    switch (e->type()) {
    case QEvent::MouseMove:
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
    case QEvent::MouseButtonDblClick:
    case QEvent::Enter:  // 鼠标进入窗口
    case QEvent::Wheel:
    case QEvent::KeyPress:
    case QEvent::KeyRelease:
    case QEvent::FocusIn:
    case QEvent::FocusOut:
    case QEvent::HoverMove:
    case QEvent::Resize:
    case QEvent::Leave:
        if (d_ptr->needDemandUpdate()) {
            requestRender();
        }
        break;
    case QEvent::WindowActivate:
    case QEvent::WindowDeactivate:
        requestRender();
        break;
    case QEvent::FontChange:
    case QEvent::StyleChange:
        if (d_ptr->needDemandUpdate()) {
            requestRender();
        }
        break;
    case QEvent::Hide:
        d_ptr->timer->stop();  // 隐藏时停止渲染
        break;

    case QEvent::Show:
        if (d_ptr->needStartContinuousTimer()) {
            d_ptr->timer->start();  // 非OnDemand模式恢复渲染
        }
        break;
    default:
        break;
    }

    return QOpenGLWidget::event(e);
}

/**
 * @brief 绘制背景
 *
 * 在刷新GL窗口的时候，需要先绘制一遍背景，避免imgui的窗口重影
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
 * @brief 在ImGui调用节点渲染树之前执行的函数，可以在此函数绘制底图，例如背景
 * @note QImWidget::drawBeforeIMRender主要处理背景清除工作，如果重写drawBeforeIMRender，
 * 建议显示调用QImWidget::drawBeforeIMRender(),除非你明确不需要清除背景
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
 * @brief 在ImGui调用Render之前执行的函数，这时所有渲染节点以及执行完
 *
 * 如果你的窗口没有任何渲染节点，可以把这个函数作为你执行imgui的渲染窗口
 *
 * 例如下面这个是进行
 * @code
 * void ImWidget::afterRenderIM()
 * {
 *     static ImVec4 clear_color = ImColor(114, 144, 154);
 *     static bool show_labels   = true;
 *     double x1                 = 0.2;
 *     double x2                 = 0.8;
 *     double y1                 = 0.25;
 *     double y2                 = 0.75;
 *     double f                  = 0.1;
 *
 *     ImPlot::SetCurrentContext(m_context);
 *
 *     static bool use_work_area     = false;
 *     static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
 * ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;
 *
 *     const ImGuiViewport* viewport = ImGui::GetMainViewport();
 *     ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
 *     ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);
 *
 *     if (ImGui::Begin("Example: Fullscreen window", nullptr, flags)) {
 *         ImGui::BulletText("Click and drag the horizontal and vertical lines.");
 *
 *         ImGui::Checkbox("Show Labels##1", &show_labels);
 *         if (ImPlot::BeginPlot("##guides", 0, 0, ImVec2(-1, -1), ImPlotFlags_YAxis2)) {
 *             double xs[ 1000 ], ys[ 1000 ];
 *             for (int i = 0; i < 1000; ++i) {
 *                 xs[ i ] = (x2 + x1) / 2 + abs(x2 - x1) * (i / 1000.0f - 0.5f);
 *                 ys[ i ] = (y1 + y2) / 2 + abs(y2 - y1) / 2 * sin(f * i / 10);
 *             }
 *             ImPlot::PlotLine("Interactive Data", xs, ys, 1000);
 *             ImPlot::EndPlot();
 *         }
 *         ImGui::End();
 *     }
 * }
 * @endcode
 *
 * 在beforeRenderIM也可以执行上面的函数，不过更建议在afterRenderIM上执行，如果在beforeRenderIM执行，记得调用QImWidget::beforeRenderIM()，否则主题会失效
 */
void QImWidget::afterRenderImNodes()
{
#ifdef QIM_ENABLE_DEBUG_PRINT_FPS
    QIM_D(d);
    d->drawFPSToast();
#endif
}

void QImWidget::resetRootRenderNode(QImAbstractNode* node)
{
    d_ptr->rootRenderNode.reset(node);
}

}  // end namespace QIM
