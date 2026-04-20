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
 * \if ENGLISH
 * @brief Qt widget wrapper for quick ImGui integration
 *
 * Simply override the drawIM() function to render ImGui content on the widget.
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
 *         ImGui::Begin("Control Panel");
 *         ImGui::Text("Hello, World!");
 *         ImGui::End();
 *     }
 * };
 * @endcode
 * \endif
 *
 * \if CHINESE
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
 * \endif
 */
class QIM_WIDGETS_API QImWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImWidget)
public:
    /**
     * @brief ImGui glyph range flags
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
     * @brief ImGui color style themes
     */
    enum StyleColorsTheme
    {
        StyleDark,
        StyleLight,
        StyleClassic
    };

    /**
     * \if ENGLISH
     * @brief Render mode enumeration
     * @details Three rendering strategies for different scenarios:
     * - Continuous: Continuous rendering at 18 FPS, suitable for animations and real-time data visualization
     * - OnDemand: Single frame rendering on event trigger, optimal for static content (energy saving)
     * - Adaptive: Smart adaptive (default), continuous during interaction, 1 FPS when idle
     * \endif
     *
     * \if CHINESE
     * @brief 渲染模式枚举
     * @details 三种渲染策略，适用于不同场景：
     * - Continuous: 持续渲染（18 FPS），适用于动画、实时数据可视化等
     * - OnDemand: 仅在事件触发时渲染单帧，适用于静态内容显示（节能最优）
     * - Adaptive: 智能自适应（默认），交互时持续渲染 FPS，静止时1 FPS
     * \endif
     */
    enum RenderMode
    {
        RenderContinuous,  // Continuous rendering
        RenderOnDemand,  // On-demand; relies on Qt events for rendering, auto-enables mouse tracking. Use RenderContinuous for animations
        RenderAdaptive   // Adaptive
    };

public:
    QImWidget(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~QImWidget();
    //----------------------------------------------------
    // Render control
    //----------------------------------------------------
    void setRenderMode(RenderMode mode);
    RenderMode renderMode() const;
    // Set refresh interval
    void setRefreshInterval(int ms);
    int refreshInterval() const;
    // Force render request
    void requestRender();
    // Minimum render interval to avoid excessive redraw triggers, default 16ms (60 FPS)
    int minRenderInterval() const;
    void setMinRenderInterval(int min);
    //----------------------------------------------------
    // Node control
    //----------------------------------------------------
    // Add render node
    void addRenderNode(QImAbstractNode* node);
    // Get all render nodes
    QList< QImAbstractNode* > renderNodeList() const;
    // Remove node
    void removeRenderNode(QImAbstractNode* node);
    // Extract render node
    bool takeRenderNode(QImAbstractNode* node);
    //----------------------------------------------------
    // Theme and font control
    //----------------------------------------------------
    // Set font glyph ranges
    void setFontGlyphRanges(GlyphRangesFlags ranges);
    GlyphRangesFlags fontGlyphRangesFlag() const;
    // Set color theme
    void setStyleColorsTheme(StyleColorsTheme style);
    StyleColorsTheme styleColorsTheme() const;

public:
    // Draw background
    virtual void drawBackground();
    // Called before rendering node tree for ImGui render; paint background here
    virtual void beforeRenderImNodes();
    // Called after rendering node tree; paint overlay here
    virtual void afterRenderImNodes();

protected:
    // Reset root render node (default is QImWidgetNode)
    void resetRootRenderNode(QImAbstractNode* node);
    // Initialize GL; overriding this must call QImWidget::initializeGL() or ImGui will fail
    void initializeGL() override;
    void paintGL() override;
    void changeEvent(QEvent* e) override;
    bool event(QEvent* e) override;
};
}
#endif  // QIMWIDGET_H
