#ifndef QIMWIDGETNODE_H
#define QIMWIDGETNODE_H
#include "QImAbstractNode.h"
#include <QMarginsF>
#include <QRect>
#include <QSize>
namespace QIM
{
/**
 * \if ENGLISH
 * @brief Base class for ImGui window nodes (Qt-style API)
 *
 * Encapsulates ImGui::Begin/End window scope with Qt-convention API naming:
 * - windowTitle() corresponds to QWidget::windowTitle()
 * - pos()/move() corresponds to QWidget::pos()/move()
 * - minimumSize() corresponds to QWidget::minimumSize()
 * - contentsMargins() corresponds to QLayout::contentsMargins()
 * - borderRadius() corresponds to Qt stylesheet border-radius
 *
 * \endif
 *
 * \if CHINESE
 * @brief ImGui 窗口节点的基类（Qt 风格 API）
 *
 * 封装 ImGui::Begin/End 窗口作用域，API 命名遵循 Qt 惯例：
 * - windowTitle() 对应 QWidget::windowTitle()
 * - pos()/move() 对应 QWidget::pos()/move()
 * - minimumSize() 对应 QWidget::minimumSize()
 * - contentsMargins() 对应 QLayout::contentsMargins()
 * - borderRadius() 对应 Qt 样式表 border-radius
 *
 * \endif
 */
class QIM_CORE_API QImWidgetNode : public QImAbstractNode
{
    Q_OBJECT
    Q_PROPERTY(QString windowTitle READ windowTitle WRITE setWindowTitle NOTIFY windowTitleChanged)
    Q_PROPERTY(QPoint pos READ pos WRITE setPos)
    Q_PROPERTY(QSize size READ size WRITE setSize)
    Q_PROPERTY(QSize minimumSize READ minimumSize WRITE setMinimumSize)
    Q_PROPERTY(QMarginsF contentsMargins READ contentsMargins WRITE setContentsMargins)

    // Semantic window flags (Qt-style naming)
    Q_PROPERTY(bool isTitleBarEnabled READ isTitleBarEnabled WRITE setTitleBarEnabled)
    Q_PROPERTY(bool isResizable READ isResizable WRITE setResizable)
    Q_PROPERTY(bool isMovable READ isMovable WRITE setMovable)
    Q_PROPERTY(bool isScrollbarEnabled READ isScrollbarEnabled WRITE setScrollbarEnabled)
    Q_PROPERTY(bool isCollapseEnabled READ isCollapseEnabled WRITE setCollapseEnabled)
    Q_PROPERTY(bool isBackgroundEnabled READ isBackgroundEnabled WRITE setBackgroundEnabled)
    Q_PROPERTY(bool isResizeToContents READ isResizeToContents WRITE setResizeToContents)
    Q_PROPERTY(bool noBringToFrontOnFocus READ noBringToFrontOnFocus WRITE setNoBringToFrontOnFocus)
    Q_PROPERTY(bool noFocusOnAppearing READ noFocusOnAppearing WRITE setNoFocusOnAppearing)
    Q_PROPERTY(bool noNav READ noNav WRITE setNoNav)

    Q_DISABLE_COPY(QImWidgetNode)
    QIM_DECLARE_PRIVATE(QImWidgetNode)
public:
    explicit QImWidgetNode(QObject* parent = nullptr);
    ~QImWidgetNode() override;

    // === Qt-style basic properties ===

    QString windowTitle() const;
    void setWindowTitle(const QString& title);

    QPoint pos() const;
    void setPos(const QPoint& pos);
    void move(const QPoint& pos)
    {
        setPos(pos);
    }  // Qt-style convenience method

    QSize size() const;
    void setSize(const QSize& size);
    void resize(const QSize& size)
    {
        setSize(size);
    }  // Qt-style convenience method

    QSize minimumSize() const;
    void setMinimumSize(const QSize& size);

    // Qt-style margins (corresponds to QLayout::contentsMargins)
    QMarginsF contentsMargins() const;
    void setContentsMargins(const QMarginsF& margins);
    void setContentsMargins(float paddingX, float paddingY);

    // Check if widget is collapsed
    bool isWidgetCollapsed() const;

    // === Semantic window flags (Qt-style) ===
    // ImGui window flag is ImGuiWindowFlags_NoTitleBar
    bool isTitleBarEnabled() const;
    void setTitleBarEnabled(bool on);

    // ImGui window flag is ImGuiWindowFlags_NoResize
    bool isResizable() const;
    void setResizable(bool on);

    // ImGui window flag is ImGuiWindowFlags_NoMove
    bool isMovable() const;
    void setMovable(bool on);

    // ImGui window flag is ImGuiWindowFlags_NoScrollbar
    bool isScrollbarEnabled() const;
    void setScrollbarEnabled(bool on);

    // ImGui window flag is ImGuiWindowFlags_NoCollapse
    bool isCollapseEnabled() const;  // Qt-style: isCollapsible rather than canCollapse
    void setCollapseEnabled(bool on);

    // ImGui window flag is ImGuiWindowFlags_NoBackground
    bool isBackgroundEnabled() const;
    void setBackgroundEnabled(bool on);

    // ImGui window flag is ImGuiWindowFlags_AlwaysAutoResize
    bool isResizeToContents() const;  // Qt-style: past participle
    void setResizeToContents(bool on);

    // ImGui window flag is  ImGuiWindowFlags_NoBringToFrontOnFocus
    bool noBringToFrontOnFocus() const;
    void setNoBringToFrontOnFocus(bool on);

    bool noFocusOnAppearing() const;
    void setNoFocusOnAppearing(bool on);

    bool noNav() const;
    void setNoNav(bool on);

    // Convenience: set frameless style
    void setToFrameLess(bool on);

    // Fit window to QOpenGLWidget viewport
    void setFitToGLViewPort(bool fitWidth, bool fitHeight);
    bool isWidthFitToGLViewPort() const;
    bool isHeightFitToGLViewPort() const;
    //
    Q_INVOKABLE QRect geometry() const;  // Qt-style: geometry() instead of contentRect()

Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when the window title changes
     * @param[in] title The new window title text
     * @details Triggered by setWindowTitle() when the title value actually changes.
     * \endif
     *
     * \if CHINESE
     * @brief 窗口标题更改时触发
     * @param[in] title 新的窗口标题文本
     * @details 当标题值实际更改时由setWindowTitle()触发。
     * \endif
     */
    void windowTitleChanged(const QString& title);

protected:
    virtual bool beginDraw() override;
    virtual void endDraw() override;
};

}
#endif  // QIMWIDGETNODE_H
