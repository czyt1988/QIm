#ifndef QIMWIDGET_H
#define QIMWIDGET_H
#include "QImAPI.h"
#include <QOpenGLFunctions>
#if QT_VERSION_MAJOR >= 6
#include <QtOpenGLWidgets/QOpenGLWidget>
#else
#include <QOpenGLWidget>
#endif
namespace QIM
{
class QImAbstractNode;
/**
 * @brief qt窗口快速使用ImGui的封装
 *
 * 你只需要重写drawIM函数，就可以在窗口上绘制imgui
 *
 * @code
 * #include "QImWidget.h"
 * #include "imgui.h"
 * class ImTestWidget : public QIM::QImWidget
 * {
 *     Q_OBJECT
 * public:
 *     ImTestWidget(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags()) : QIM::QImWidget(parent, f)
 *     {
 *     }
 *
 * public:
 *     virtual void drawIM() override
 *     {
 *         ImGui::Begin(u8"控制面板");
 *         ImGui::Text("这是一段中文文本！");
 *         ImGui::Text("Hello, 世界！");
 *         ImGui::Text("中文输入测试: 你好，世界！");
 *         ImGui::End();
 *     }
 * };
 * @endcode
 */
class QIM_WIDGETS_API QImWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImWidget)
public:
    /**
     * @brief Imgui的字形范围
     */
    enum GlyphRanges
    {
        GlyphRangesDefault                 = 0x01,
        GlyphRangesGreek                   = 0x02,
        GlyphRangesKorean                  = 0x04,
        GlyphRangesChineseFull             = 0x08,
        GlyphRangesChineseSimplifiedCommon = 0x10,
        GlyphRangesJapanese                = 0x20,
        GlyphRangesCyrillic                = 0x40,
        GlyphRangesThai                    = 0x80,
        GlyphRangesVietnamese              = 0x100
    };
    Q_DECLARE_FLAGS(GlyphRangesFlags, GlyphRanges)

    /**
     * @brief Imgui的颜色样式
     */
    enum StyleColorsTheme
    {
        StyleDark,
        StyleLight,
        StyleClassic
    };

    /**
     * @brief 渲染模式枚举
     *
     * 三种渲染策略，适用于不同场景：
     * - Continuous: 持续渲染（18 FPS），适用于动画、实时数据可视化等
     * - OnDemand: 仅在事件触发时渲染单帧，适用于静态内容显示（节能最优）
     * - Adaptive: 智能自适应（默认），交互时持续渲染 FPS，静止时1 FPS
     */
    enum RenderMode
    {
        RenderContinuous,  // 持续渲染
        RenderOnDemand,  // 按需渲染，此模式完全依赖qt的事件触发渲染,自动会setMouseTracking,如果有动画，建议使用RenderContinuous
        RenderAdaptive   // 自适应
    };

public:
    QImWidget(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~QImWidget();
    //----------------------------------------------------
    // 渲染控制
    //----------------------------------------------------
    void setRenderMode(RenderMode mode);
    RenderMode renderMode() const;
    // 设置刷新间隔
    void setRefreshInterval(int ms);
    int refreshInterval() const;
    // 强制请求渲染
    void requestRender();
    // 最小渲染时间间隔，避免过高渲染间隔导致频繁触发绘制，默认为16，对应60FPS
    int minRenderInterval() const;
    void setMinRenderInterval(int min);
    //----------------------------------------------------
    // 节点控制
    //----------------------------------------------------
    // 添加渲染节点
    void addRenderNode(QImAbstractNode* node);
    // 获取所有渲染节点
    QList< QImAbstractNode* > renderNodeList() const;
    // 移除节点
    void removeRenderNode(QImAbstractNode* node);
    // 抽取渲染节点
    bool takeRenderNode(QImAbstractNode* node);
    //----------------------------------------------------
    // 主题字体控制
    //----------------------------------------------------
    // 设置使用中文字体
    void setFontGlyphRanges(GlyphRangesFlags ranges);
    GlyphRangesFlags fontGlyphRangesFlag() const;
    // 设置颜色主题
    void setStyleColorsTheme(StyleColorsTheme style);
    StyleColorsTheme styleColorsTheme() const;

public:
    // 绘制背景
    virtual void drawBackground();
    // 在调用渲染节点树进行ImGui Render之前执行的函数，可以在此函数绘制底图，例如背景
    virtual void beforeRenderImNodes();
    // 在调用渲染节点树之后执行的函数，可以在此函数绘制顶图
    virtual void afterRenderImNodes();

protected:
    // 重置渲染根节点，默认是一个QImWidgetNode
    void resetRootRenderNode(QImAbstractNode* node);
    // 初始化GL，重写此方法理论上都应该调用QImWidget::initializeGL()进行imgui的初始化，否则会异常
    void initializeGL() override;
    void paintGL() override;
    void changeEvent(QEvent* e) override;
    bool event(QEvent* e) override;
};
}
#endif  // QIMWIDGET_H
