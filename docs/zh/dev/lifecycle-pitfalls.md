# 初始化时序陷阱

QIm 的底层是 ImGui 和 ImPlot，这两个库都有自己的全局状态（context、storage 等）。这些状态在什么时候可用、什么时候不可用，是使用 QIm 时最容易踩的坑。说白了，所有 ImGui/ImPlot API 都依赖一个已经创建好的 context，而 QIm 的 context 创建时机取决于 Qt 的 OpenGL 初始化流程——这件事如果没想清楚，程序一启动就崩溃，而且崩溃位置往往离真正的原因很远。

## 为什么会崩溃

QIm 的 `QImFigureWidget` 继承自 `QOpenGLWidget`。ImGui 和 ImPlot 的 context 不是在构造函数里创建的，而是在 Qt 调用 `initializeGL()` 的时候才创建。`initializeGL()` 是 Qt 的回调，发生在 widget 首次显示之前——也就是说，在 widget 被加入布局、窗口被 `show()` 之后，Qt 才会真正建立 OpenGL 上下文并触发 `initializeGL()`。

这意味着：**在 MainWindow 构造函数里直接调用任何 ImPlot/ImGui API，都会崩溃**。因为此时 `initializeGL()` 还没执行，ImGui 的全局 storage 还不存在，ImPlot 的 colormap registry 也还不存在。你往一个空指针上读写数据，当然会 ACCESS_VIOLATION。

崩溃栈看起来是这样的：

```
CpuUsageView::buildView()
  → QImPlotColormapManager::addColormap("__bar_groups_custom__")
    → ImPlot::AddColormap()
      → ImPlotColormapData::GetIndex()
        → ImGuiStorage::GetInt()  ← 这里炸了，因为 ImGui context 还没创建
```

说实话，这个崩溃位置离根因很远。你看到的是 `ImGuiStorage::GetInt()` 报错，但真正的错误发生在几十行之外的构造函数里——那里不该调用 `addColormap`。

## 什么时候才算"安全"

ImGui/ImPlot context 就绪的标志是 `QImFigureWidget::initializeGL()` 执行完毕。这个函数做了两件关键的事：

```cpp
void QImFigureWidget::initializeGL()
{
    QImWidget::initializeGL();       // 创建 ImGui context
    d->m_context2D = ImPlot::CreateContext();  // 创建 ImPlot context
}
```

在这之后，所有的 ImGui/ImPlot API 才是安全的。在此之前，调用任何这些 API 都是在冒险。

哪些 API 有这个问题？坦白讲，所有直接或间接访问 ImGui/ImPlot 内部状态的 API 都有。不只是 `ImPlot::AddColormap()`，还包括：

- `ImPlot::AddColormap()` — 访问 colormap registry（ImGuiStorage）
- `ImPlot::GetColormapIndex()` — 同上
- `ImGui::CreateContext()` / `ImGui::DestroyContext()` — 直接操作 context
- 任何 `ImPlot::BeginPlot()` / `ImGui::Begin()` 系列的调用 — 需要当前 context

不过 QIm 的节点系统在 `beginDraw()` 里调用这些是安全的，因为 `beginDraw()` 只在 `paintGL()` 的渲染循环里被调用，那时 context 一定已经就绪。问题只出在 **渲染循环之外** 的代码——比如构造函数、初始化逻辑、或者一次性注册操作。

## 怎么避开这个坑

核心思路只有一个：**把需要 ImGui/ImPlot context 的操作，推迟到 context 就绪之后执行**。具体做法有好几种，挑最适合你场景的。

### 方案一：QTimer::singleShot(0, ...) 延迟调用

最简单粗暴，也是最常用的方式。Qt 的事件循环有个特性——`QTimer::singleShot(0, ...)` 会在当前事件处理完毕、下一个事件循环迭代时执行回调。此时 widget 已经完成了首次 show 和 `initializeGL()`。

```cpp
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setupUI();
    connectSignals();
    monitor_->start();

    // ❌ 错误：直接调用，ImGui context 还不存在
    // viewManager_->switchTo(ViewMode::CpuUsage, {});

    // ✅ 正确：延迟到事件循环开始后调用
    QTimer::singleShot(0, this, [this]() {
        viewManager_->switchTo(ViewMode::CpuUsage, {});
    });
}
```

不过说实话，`singleShot(0)` 只是"大概率安全"——它依赖 Qt 事件循环的调度顺序，而 `initializeGL()` 的触发时机取决于平台和 Qt 版本。在绝大多数情况下它是安全的，但如果你想要绝对确定性，用方案二。

### 方案二：监听 widget 的初始化信号

如果 QIm 未来提供了类似 `contextReady()` 的信号，那当然是最好的方式。目前 QImFigureWidget 还没有这个信号，但你可以自己在子类里加一个：

```cpp
class MyFigureWidget : public QIM::QImFigureWidget {
    Q_OBJECT
public:
    using QImFigureWidget::QImFigureWidget;
    Q_SIGNALS:
        void contextReady();
protected:
    void initializeGL() override {
        QImFigureWidget::initializeGL();  // 确保先完成 ImGui/ImPlot 初始化
        Q_EMIT contextReady();             // 通知外部：context 可用了
    }
};
```

然后在 MainWindow 里连接这个信号：

```cpp
connect(figureWidget_, &MyFigureWidget::contextReady,
        this, [this]() { viewManager_->switchTo(ViewMode::CpuUsage, {}); });
```

这种方式最可靠——你不需要猜 context 什么时候就绪，信号告诉你就是就绪了。

### 方案三：把注册操作挪到 beginDraw 里

有些操作（比如注册 colormap）其实是只需要执行一次的全局操作。你可以把它放到节点的 `beginDraw()` 里，用一个 `static bool` 或成员变量来确保只执行一次：

```cpp
bool MyCustomNode::beginDraw()
{
    if (!colormapRegistered_) {
        QIM::QImPlotColormapManager::addColormap("my_palette", colors, true);
        colormapRegistered_ = true;
    }
    // ... 正常渲染逻辑
}
```

`beginDraw()` 在 `paintGL()` 里被调用，context 一定就绪。不过这种方式有个小问题——首次渲染帧可能会有一帧的延迟（因为注册发生在第一帧的渲染过程中）。对于 colormap 注册来说，一帧延迟基本没有影响。

!!! warning "注意"
    `QImPlotColormapManager::addColormap()` 是全局操作，注册一次就永久存在于 ImPlot 的 colormap registry 里。不要每帧都调用它，否则性能会越来越差。用 `static bool` 或成员变量确保只注册一次。

## 判断原则

什么时候需要担心这个问题？有一个简单的判断原则：

**如果你的代码在 `paintGL()` / `beginDraw()` 的渲染循环之外调用了 ImGui 或 ImPlot 的 API，就需要检查初始化时序。**

反过来，如果你的代码只在 `beginDraw()` / `endDraw()` 里调用这些 API，或者在信号槽里调用（信号槽是在事件循环里执行的，此时 widget 通常已经就绪），那基本没问题。

说白了，最容易出问题的地方就是构造函数和紧随构造函数之后的一次性初始化逻辑。写代码的时候多想一步：这个 API 需要 context 吗？context 此时存在吗？

## Colormap 重复注册陷阱

上面说的都是"context 还没创建就调用 API"的问题。但还有一个相关但不同的坑：**colormap 重复注册**。

### 问题现象

你的程序第一次运行正常，但当你切换视图后再切回来，或者重建某个 widget 时，程序直接崩溃，断言信息是：

```
IM_ASSERT_USER_ERROR(gp.ColormapData.GetIndex(name) == -1,
    "The colormap name has already been used!");
```

崩溃栈长这样：

```
ViewManager::switchTo(ViewMode::CpuUsage)
  → CpuUsageView::buildView()
    → QImPlotColormapManager::addColormap("__bar_groups_custom__")
      → ImPlot::AddColormap()
        → IM_ASSERT_USER_ERROR(...)  ← 断言失败，程序终止
```

### 为什么会这样

ImPlot 的 colormap registry 是全局的、持久的。一旦你注册了一个名为 `"my_colormap"` 的 colormap，它就会一直存在于 ImPlot 的 registry 里，直到程序结束。

问题在于 `ImPlot::AddColormap()` 的设计——**它不允许重复注册同名 colormap**。如果你尝试注册一个已经存在的名字，ImPlot 会直接触发断言失败（在 Debug 模式下）或者静默失败（在 Release 模式下，但行为未定义）。

所以如果你的代码长这样：

```cpp
void MyView::buildView(QImFigureWidget* figure) {
    // ... 创建 plot node ...

    // ❌ 错误：每次 buildView 都会执行，但 colormap 只能注册一次
    QImPlotColormapManager::addColormap("my_palette", colors, true);

    plotNode_->pushColormap("my_palette");
}
```

第一次调用 `buildView()` 时，colormap 注册成功。用户切换到别的视图，再切回来，`buildView()` 又被调用——**boom**，断言失败。

### 怎么解决

有三种方案，按推荐程度排序：

#### 方案一：在第一次渲染时延迟注册（推荐）

**⚠️ 注意：colormap 注册必须在 ImPlot context 创建之后进行，而 context 是在 `QImFigureWidget::initializeGL()` 中创建的。所以不能在构造函数中注册！**

把 colormap 注册放在第一次需要使用时进行，比如 `ViewManager::switchTo()` 中，使用 `static bool` 确保只注册一次：

```cpp
void ViewManager::switchTo(ViewMode mode, const QList<AggregatedProcessInfo>& data) {
    if (!figure_) return;

    // ✅ 正确：在第一次 switchTo 时注册，此时 ImPlot context 已就绪
    // 使用 static bool 确保只注册一次
    static bool colormapRegistered = false;
    if (!colormapRegistered) {
        QImPlotColormapManager::addColormap(
            "__bar_groups_custom__",
            QList<QColor>(kColorPalette.begin(), kColorPalette.end()),
            true
        );
        colormapRegistered = true;
    }

    // ... 其余切换逻辑 ...
}

void CpuUsageView::buildView(QImFigureWidget* figure) {
    // ... 创建 plot node ...

    // 只需要 push，不需要再注册
    plotNode_->pushColormap("__bar_groups_custom__");
}
```

这种方式最清晰——注册和使用的职责分离，注册延迟到 ImPlot context 就绪后，使用时只管 push。

!!! warning "警告"
    千万不要在构造函数中调用 `addColormap()`！构造函数执行时 `QImFigureWidget` 可能还未显示，ImPlot context 还未创建，会导致崩溃。

#### 方案二：在节点的 beginDraw 中延迟注册

如果你必须在节点内部注册，可以在 `beginDraw()` 里用 `static bool` 确保只执行一次：

```cpp
bool MyCustomNode::beginDraw()
{
    // ✅ 正确：static 变量只初始化一次，且 beginDraw 在渲染循环中调用
    static bool colormapRegistered = false;
    if (!colormapRegistered) {
        QImPlotColormapManager::addColormap("my_palette", colors, true);
        colormapRegistered = true;
    }
    // ... 正常渲染逻辑
}
```

注意：`beginDraw()` 在 `paintGL()` 里被调用，context 一定就绪。但首次渲染可能会有一帧延迟。

注意：`static bool` 是函数级的单例，意味着这个 colormap 在整个程序生命周期内只注册一次。如果你需要为不同视图注册不同的 colormap，这种方式没问题。但如果你需要动态更换 colormap 的内容，这种方式就不适用了——因为注册后无法修改或删除。

#### 方案三：先检查是否存在，再决定是否注册

`QImPlotColormapManager` 提供了 `colormapIndex()` 方法，可以查询某个名称的 colormap 是否已存在：

```cpp
void CpuUsageView::buildView(QImFigureWidget* figure) {
    // ... 创建 plot node ...

    // ✅ 正确：先检查，不存在才注册
    if (QImPlotColormapManager::colormapIndex("my_palette") == QImPlotColormap(-1)) {
        QImPlotColormapManager::addColormap("my_palette", colors, true);
    }

    plotNode_->pushColormap("my_palette");
}
```

这种方式最灵活，但代码稍微冗长一些。适合需要动态判断的场景。

### 关键区别：addColormap vs pushColormap

很多用户容易混淆这两个操作：

| 操作 | 作用 | 调用次数 | 失败后果 |
|------|------|----------|----------|
| `addColormap()` | 向 ImPlot 全局注册一个 colormap | **只应调用一次** | 重复调用触发断言失败 |
| `pushColormap()` | 在当前 plot 中激活某个 colormap | 每次渲染都可以调用 | 如果 colormap 不存在，使用默认 colormap |

简单说：**`addColormap` 是注册（全局、一次），`pushColormap` 是使用（局部、多次）**。

### 什么时候需要担心

如果你在使用 `QImPlotColormapManager::addColormap()`，就要考虑：

1. 这个函数会被调用几次？
2. 如果调用多次，会不会重复注册同名 colormap？
3. 如果视图可以重建（比如切换标签页），注册逻辑是否会被重新执行？

常见的危险场景：

- 在 `buildView()` / `setupPlot()` 等视图构建函数里直接调用 `addColormap`
- 在 widget 的 `showEvent()` 或 `initializeGL()` 里调用 `addColormap`（这些函数可能被调用多次）
- 在响应用户操作的槽函数里调用 `addColormap`（用户可能重复触发）

### 完整示例

参考 system-monitor 示例的修复方案：

**ViewManager.cpp**（在 switchTo 中延迟注册，使用 static bool 确保只执行一次）：

```cpp
#include "ColorPalette.h"
#include "plot/QImPlotColormapManager.h"

ViewManager::ViewManager(QImFigureWidget* figure, QObject* parent)
    : QObject(parent), figure_(figure) {
    // ... 创建 view 对象 ...
    // ❌ 不要在这里注册 colormap！ImPlot context 还未创建
}

void ViewManager::switchTo(ViewMode mode, const QList<AggregatedProcessInfo>& data) {
    if (!figure_) return;

    // ✅ 正确：在第一次 switchTo 时注册，此时 ImPlot context 已就绪
    static bool colormapRegistered = false;
    if (!colormapRegistered) {
        QImPlotColormapManager::addColormap(
            "__bar_groups_custom__",
            QList<QColor>(QImSystemMonitor::kColorPalette.begin(),
                          QImSystemMonitor::kColorPalette.end()),
            true  // qualitative
        );
        colormapRegistered = true;
    }

    // ... 其余切换逻辑 ...
}
```

**CpuUsageView.cpp**（buildView 中只 push，不注册）：

```cpp
void CpuUsageView::buildView(QImFigureWidget* figure) {
    // ... 创建 plot node 和 barGroups ...

    // Colormap 已在 ViewManager::switchTo 中注册，这里直接使用
    plotNode_->pushColormap("__bar_groups_custom__");

    // ... 其余设置 ...
}
```

## 参考

- [渲染性能规范](render-guidelines.md) — beginDraw 内部的编码规范
- [自定义节点](custom-node.md) — beginDraw/endDraw 的实现细节
- [渲染节点](../render-node.md) — QImAbstractNode 的生命周期概览