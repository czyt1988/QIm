# Initialization Timing Pitfalls

QIm is built on top of ImGui and ImPlot, both of which have their own global state (context, storage, etc.). When this state is available and when it isn't is the easiest trap to fall into when using QIm. Simply put, all ImGui/ImPlot APIs depend on an already-created context, and QIm's context creation timing depends on Qt's OpenGL initialization flow — if you don't think this through clearly, your program will crash on startup, and the crash location is often far from the real cause.

## Why It Crashes

QIm's `QImFigureWidget` inherits from `QOpenGLWidget`. ImGui and ImPlot contexts are not created in the constructor but when Qt calls `initializeGL()`. `initializeGL()` is a Qt callback that happens just before the widget is first shown — meaning the Qt OpenGL context is established and `initializeGL()` is triggered only after the widget is added to a layout and the window is `show()`-ed.

This means: **calling any ImPlot/ImGui API directly in the MainWindow constructor will crash**. Because at that point `initializeGL()` hasn't executed yet, ImGui's global storage doesn't exist, and ImPlot's colormap registry doesn't exist either. You're reading/writing to a null pointer, which naturally results in ACCESS_VIOLATION.

The crash stack looks something like this:

```
CpuUsageView::buildView()
  → QImPlotColormapManager::addColormap("__bar_groups_custom__")
    → ImPlot::AddColormap()
      → ImPlotColormapData::GetIndex()
        → ImGuiStorage::GetInt()  ← Boom! ImGui context not yet created
```

Honestly, this crash location is far from the root cause. What you see is `ImGuiStorage::GetInt()` erroring, but the real error happened dozens of lines earlier in the constructor — where `addColormap` shouldn't have been called.

## When Is It "Safe"

The signal that the ImGui/ImPlot context is ready is when `QImFigureWidget::initializeGL()` has finished executing. This function does two critical things:

```cpp
void QImFigureWidget::initializeGL()
{
    QImWidget::initializeGL();       // Creates ImGui context
    d->m_context2D = ImPlot::CreateContext();  // Creates ImPlot context
}
```

After this, all ImGui/ImPlot APIs are safe. Before this, calling any of these APIs is gambling.

Which APIs have this problem? Honestly, all APIs that directly or indirectly access ImGui/ImPlot internal state do. Not just `ImPlot::AddColormap()`, but also:

- `ImPlot::AddColormap()` — accesses colormap registry (ImGuiStorage)
- `ImPlot::GetColormapIndex()` — same as above
- `ImGui::CreateContext()` / `ImGui::DestroyContext()` — directly manipulates context
- Any `ImPlot::BeginPlot()` / `ImGui::Begin()` family calls — requires current context

However, QIm's node system calling these in `beginDraw()` is safe because `beginDraw()` is only called within `paintGL()`'s render loop, at which point the context is definitely ready. The problem only occurs with code **outside the render loop** — such as constructors, initialization logic, or one-time registration operations.

## How to Avoid This Pitfall

The core idea is simple: **defer operations that require ImGui/ImPlot context until after the context is ready**. There are several specific approaches — pick the one that best fits your scenario.

### Approach 1: QTimer::singleShot(0, ...) Deferred Call

The simplest and most commonly used approach. Qt's event loop has a characteristic — `QTimer::singleShot(0, ...)` executes the callback when the current event has finished processing and the next event loop iteration begins. At this point, the widget has already completed its first show and `initializeGL()`.

```cpp
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setupUI();
    connectSignals();
    monitor_->start();

    // ❌ Wrong: direct call, ImGui context doesn't exist yet
    // viewManager_->switchTo(ViewMode::CpuUsage, {});

    // ✅ Correct: defer until event loop starts
    QTimer::singleShot(0, this, [this]() {
        viewManager_->switchTo(ViewMode::CpuUsage, {});
    });
}
```

To be honest though, `singleShot(0)` is only "probably safe" — it depends on the Qt event loop's scheduling order, and the trigger timing of `initializeGL()` depends on the platform and Qt version. In the vast majority of cases it's safe, but if you want absolute certainty, use Approach 2.

### Approach 2: Listen for the Widget's Initialization Signal

If QIm provided a signal like `contextReady()` in the future, that would of course be the best approach. Currently QImFigureWidget doesn't have this signal, but you can add one in your own subclass:

```cpp
class MyFigureWidget : public QIM::QImFigureWidget {
    Q_OBJECT
public:
    using QImFigureWidget::QImFigureWidget;
    Q_SIGNALS:
        void contextReady();
protected:
    void initializeGL() override {
        QImFigureWidget::initializeGL();  // Ensure ImGui/ImPlot init completes first
        Q_EMIT contextReady();             // Notify external: context is ready
    }
};
```

Then connect this signal in MainWindow:

```cpp
connect(figureWidget_, &MyFigureWidget::contextReady,
        this, [this]() { viewManager_->switchTo(ViewMode::CpuUsage, {}); });
```

This approach is the most reliable — you don't need to guess when the context is ready; the signal tells you it is.

### Approach 3: Move Registration Operations into beginDraw

Some operations (like registering a colormap) are actually one-time global operations. You can place them in a node's `beginDraw()`, using a `static bool` or member variable to ensure they execute only once:

```cpp
bool MyCustomNode::beginDraw()
{
    if (!colormapRegistered_) {
        QIM::QImPlotColormapManager::addColormap("my_palette", colors, true);
        colormapRegistered_ = true;
    }
    // ... normal render logic
}
```

`beginDraw()` is called within `paintGL()`, so the context is definitely ready. However, this approach has a minor issue — the first render frame may have a one-frame delay (since registration happens during the first frame's rendering process). For colormap registration, a one-frame delay is basically imperceptible.

!!! warning "Note"
    `QImPlotColormapManager::addColormap()` is a global operation; registering once permanently exists in ImPlot's colormap registry. Don't call it every frame, or performance will degrade over time. Use a `static bool` or member variable to ensure it only registers once.

## Judgement Principle

When do you need to worry about this problem? Here's a simple rule of thumb:

**If your code calls ImGui or ImPlot APIs outside of the `paintGL()` / `beginDraw()` render loop, you need to check initialization timing.**

Conversely, if your code only calls these APIs within `beginDraw()` / `endDraw()`, or in signal slots (signal slots execute within the event loop, by which point the widget is usually ready), then it's basically fine.

To put it bluntly, the most error-prone locations are constructors and one-time initialization logic immediately following the constructor. When writing code, always think one step ahead: does this API need a context? Does the context exist at this point?

## Colormap Duplicate Registration Trap

What was described above is all about "calling APIs before context is created." But there's another related but distinct pitfall: **colormap duplicate registration**.

### Symptom

Your program runs fine the first time, but when you switch views and come back, or rebuild a widget, the program crashes directly with an assertion:

```
IM_ASSERT_USER_ERROR(gp.ColormapData.GetIndex(name) == -1,
    "The colormap name has already been used!");
```

The crash stack looks like:

```
ViewManager::switchTo(ViewMode::CpuUsage)
  → CpuUsageView::buildView()
    → QImPlotColormapManager::addColormap("__bar_groups_custom__")
      → ImPlot::AddColormap()
        → IM_ASSERT_USER_ERROR(...)  ← Assert fails, program terminates
```

### Why This Happens

ImPlot's colormap registry is global and persistent. Once you register a colormap named `"my_colormap"`, it lives in ImPlot's registry forever, until the program ends.

The problem lies in `ImPlot::AddColormap()`'s design — **it doesn't allow duplicate registration of same-named colormaps**. If you try to register a name that already exists, ImPlot triggers an assertion failure (in Debug mode) or silently fails (in Release mode, but behavior is undefined).

So if your code looks like this:

```cpp
void MyView::buildView(QImFigureWidget* figure) {
    // ... create plot node ...

    // ❌ Wrong: executes every time buildView is called, but colormap can only be registered once
    QImPlotColormapManager::addColormap("my_palette", colors, true);

    plotNode_->pushColormap("my_palette");
}
```

The first call to `buildView()` succeeds in registering the colormap. User switches to another view and comes back, `buildView()` is called again — **boom**, assertion fails.

### How to Fix

Three approaches, ranked by recommendation:

#### Approach 1: Deferred Registration on First Render (Recommended)

**⚠️ Note: colormap registration must occur after ImPlot context is created, and the context is created in `QImFigureWidget::initializeGL()`. So you cannot register in the constructor!**

Place colormap registration at the first point of use, such as in `ViewManager::switchTo()`, using `static bool` to ensure it only registers once:

```cpp
void ViewManager::switchTo(ViewMode mode, const QList<AggregatedProcessInfo>& data) {
    if (!figure_) return;

    // ✅ Correct: register on first switchTo, ImPlot context is ready by this point
    // Use static bool to ensure only once
    static bool colormapRegistered = false;
    if (!colormapRegistered) {
        QImPlotColormapManager::addColormap(
            "__bar_groups_custom__",
            QList<QColor>(kColorPalette.begin(), kColorPalette.end()),
            true
        );
        colormapRegistered = true;
    }

    // ... rest of switch logic ...
}

void CpuUsageView::buildView(QImFigureWidget* figure) {
    // ... create plot node ...

    // Only need to push, no need to register again
    plotNode_->pushColormap("__bar_groups_custom__");
}
```

This approach is the clearest — registration and usage responsibilities are separated, registration is deferred until the ImPlot context is ready, and the usage side only needs to push.

!!! warning "Warning"
    Never call `addColormap()` in the constructor! When the constructor executes, `QImFigureWidget` may not yet be shown and the ImPlot context is not yet created, which will cause a crash.

#### Approach 2: Deferred Registration in Node's beginDraw

If you must register inside a node, you can use `static bool` in `beginDraw()` to ensure it only executes once:

```cpp
bool MyCustomNode::beginDraw()
{
    // ✅ Correct: static variable initializes only once, and beginDraw is called in the render loop
    static bool colormapRegistered = false;
    if (!colormapRegistered) {
        QImPlotColormapManager::addColormap("my_palette", colors, true);
        colormapRegistered = true;
    }
    // ... normal render logic
}
```

Note: `beginDraw()` is called within `paintGL()`, so the context is definitely ready. But the first render may have a one-frame delay.

Note: `static bool` is function-level singleton, meaning this colormap is only registered once in the entire program lifecycle. If you need to register different colormaps for different views, this approach is fine. But if you need to dynamically change colormap content, this approach won't work — because once registered, colormaps cannot be modified or deleted.

#### Approach 3: Check First, Then Decide Whether to Register

`QImPlotColormapManager` provides the `colormapIndex()` method to check if a colormap with a given name already exists:

```cpp
void CpuUsageView::buildView(QImFigureWidget* figure) {
    // ... create plot node ...

    // ✅ Correct: check first, only register if it doesn't exist
    if (QImPlotColormapManager::colormapIndex("my_palette") == QImPlotColormap(-1)) {
        QImPlotColormapManager::addColormap("my_palette", colors, true);
    }

    plotNode_->pushColormap("my_palette");
}
```

This approach is the most flexible but slightly more verbose. Suitable for scenarios requiring dynamic judgment.

### Key Distinction: addColormap vs pushColormap

Many users easily confuse these two operations:

| Operation | Purpose | Call Frequency | Failure Consequence |
|-----------|---------|---------------|---------------------|
| `addColormap()` | Register a colormap globally in ImPlot | **Should only be called once** | Duplicate calls trigger assertion failure |
| `pushColormap()` | Activate a colormap in the current plot | Can be called every render | If colormap doesn't exist, uses default colormap |

In short: **`addColormap` is registration (global, once), `pushColormap` is usage (local, many times)**.

### When to Worry

If you're using `QImPlotColormapManager::addColormap()`, consider:

1. How many times will this function be called?
2. If called multiple times, will it register a duplicate colormap name?
3. If views can be rebuilt (e.g., switching tabs), will the registration logic be re-executed?

Common dangerous scenarios:

- Calling `addColormap` directly in view construction functions like `buildView()` / `setupPlot()`
- Calling `addColormap` in a widget's `showEvent()` or `initializeGL()` (these functions may be called multiple times)
- Calling `addColormap` in a slot function responding to user actions (user may trigger it repeatedly)

### Complete Example

Refer to the system-monitor example's fix approach:

**ViewManager.cpp** (deferred registration in switchTo, using static bool to ensure it only runs once):

```cpp
#include "ColorPalette.h"
#include "plot/QImPlotColormapManager.h"

ViewManager::ViewManager(QImFigureWidget* figure, QObject* parent)
    : QObject(parent), figure_(figure) {
    // ... create view objects ...
    // ❌ Don't register colormap here! ImPlot context not yet created
}

void ViewManager::switchTo(ViewMode mode, const QList<AggregatedProcessInfo>& data) {
    if (!figure_) return;

    // ✅ Correct: register on first switchTo, ImPlot context is ready by this point
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

    // ... rest of switch logic ...
}
```

**CpuUsageView.cpp** (buildView only pushes, doesn't register):

```cpp
void CpuUsageView::buildView(QImFigureWidget* figure) {
    // ... create plot node and barGroups ...

    // Colormap already registered in ViewManager::switchTo, just use it here
    plotNode_->pushColormap("__bar_groups_custom__");

    // ... rest of setup ...
}
```

## References

- [Rendering Performance Guidelines](render-guidelines.md) — coding standards for beginDraw internals
- [Custom Nodes](custom-node.md) — beginDraw/endDraw implementation details
- [Render Node](../render-node.md) — QImAbstractNode lifecycle overview