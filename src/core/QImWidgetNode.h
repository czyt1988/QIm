#ifndef QIMWIDGETNODE_H
#define QIMWIDGETNODE_H
#include "QImAbstractNode.h"
#include <QMarginsF>
#include <QRect>
#include <QSize>
namespace QIM
{
/**
 * @brief ImGui 窗口节点的基类（Qt 风格 API）
 *
 * 封装 ImGui::Begin/End 窗口作用域，API 命名遵循 Qt 惯例：
 * - windowTitle() 对应 QWidget::windowTitle()
 * - pos()/move() 对应 QWidget::pos()/move()
 * - minimumSize() 对应 QWidget::minimumSize()
 * - contentsMargins() 对应 QLayout::contentsMargins()
 * - borderRadius() 对应 Qt 样式表 border-radius
 *
 */
class QIM_CORE_API QImWidgetNode : public QImAbstractNode
{
    Q_OBJECT
    Q_PROPERTY(QString windowTitle READ windowTitle WRITE setWindowTitle NOTIFY windowTitleChanged)
    Q_PROPERTY(QPoint pos READ pos WRITE setPos)
    Q_PROPERTY(QSize size READ size WRITE setSize)
    Q_PROPERTY(QSize minimumSize READ minimumSize WRITE setMinimumSize)
    Q_PROPERTY(QMarginsF contentsMargins READ contentsMargins WRITE setContentsMargins)

    // 语义化窗口标志（Qt 风格命名）
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

    // === Qt 风格基本属性 ===

    QString windowTitle() const;
    void setWindowTitle(const QString& title);

    QPoint pos() const;
    void setPos(const QPoint& pos);
    void move(const QPoint& pos)
    {
        setPos(pos);
    }  // Qt 风格便捷方法

    QSize size() const;
    void setSize(const QSize& size);
    void resize(const QSize& size)
    {
        setSize(size);
    }  // Qt 风格便捷方法

    QSize minimumSize() const;
    void setMinimumSize(const QSize& size);

    // Qt 风格边距（对应 QLayout::contentsMargins）
    QMarginsF contentsMargins() const;
    void setContentsMargins(const QMarginsF& margins);
    void setContentsMargins(float paddingX, float paddingY);

    // 判断当前窗口是否是收起状态
    bool isWidgetCollapsed() const;

    // === 语义化窗口标志（Qt 风格）===
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
    bool isCollapseEnabled() const;  // Qt 风格：isCollapsible 而非 canCollapse
    void setCollapseEnabled(bool on);

    // ImGui window flag is ImGuiWindowFlags_NoBackground
    bool isBackgroundEnabled() const;
    void setBackgroundEnabled(bool on);

    // ImGui window flag is ImGuiWindowFlags_AlwaysAutoResize
    bool isResizeToContents() const;  // Qt 风格：过去分词
    void setResizeToContents(bool on);

    // ImGui window flag is  ImGuiWindowFlags_NoBringToFrontOnFocus
    bool noBringToFrontOnFocus() const;
    void setNoBringToFrontOnFocus(bool on);

    bool noFocusOnAppearing() const;
    void setNoFocusOnAppearing(bool on);

    bool noNav() const;
    void setNoNav(bool on);

    // 快速方法，设置为无边框样式
    void setToFrameLess(bool on);

    // 设置窗口适配QOpenGLWidget的窗口大小
    void setFitToGLViewPort(bool fitWidth, bool fitHeight);
    bool isWidthFitToGLViewPort() const;
    bool isHeightFitToGLViewPort() const;
    //
    Q_INVOKABLE QRect geometry() const;  // Qt 风格：geometry() 替代 contentRect()

Q_SIGNALS:
    void windowTitleChanged(const QString& title);

protected:
    virtual bool beginDraw() override;
    virtual void endDraw() override;
    virtual void beginDisabled(bool isDisable) override;
    virtual void endDisabled(bool isDisable) override;
};

}
#endif  // QIMWIDGETNODE_H
