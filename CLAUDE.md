# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project

QIm wraps Dear ImGui, ImPlot, and ImPlot3D into Qt using a retained-mode (object-tree) architecture. Every chart element is a QObject node. C++17, CMake, Qt 5.14+/6+.

## Build

```powershell
# One-click (auto-detects Qt/VS/CMake paths)
.\build.ps1                 # Release + Examples ON
.\build.ps1 build           # incremental
.\build.ps1 rebuild         # clean + reconfigure + build
.\build.ps1 configure -Examples OFF -Benchmark OFF  # minimal (library only)
.\build.ps1 help            # all options
```

Manual:
```powershell
cmake -S . -B build -G "Visual Studio 16 2019" -A x64 -DCMAKE_PREFIX_PATH="<Qt path>"
cmake --build build --config Release
```

**Critical**: Use Visual Studio generator (not Ninja). Qt version must match VS compiler (msvc2019→VS2019). Qt6 needs `OpenGLWidgets`. macOS needs OpenGL 3.3 Core Profile default format.

CMake options: `QIM_BUILD_EXAMPLES`(ON), `QIM_BUILD_TESTS`(OFF), `QIM_ENABLE_BENCHMARK`(ON), `QIM_BUILD_QML`(OFF, incomplete).

## Tests

```powershell
.\build.ps1 configure -Tests ON
ctest --output-on-failure -R plot   # CI (Ubuntu)
```

Unit tests in `tests/plot/` (downsampler) and `tests/plot3d/` (enums, math, conversions, styles, colormaps, utilities, setup_api). No GUI/rendering tests — system deliberately avoids OpenGL dependency. Functional verification via manual `examples/qimfigure-test` runs (TestFunction base-class pattern). New test functions: see `examples/HowToAddNewFunction.md`, CMakeLists uses `GLOB_RECURSE`.

## Architecture

**Layers (top → bottom)**:
1. **Qt Widget Layer** (`src/widgets/`) — `QImFigureWidget`, `QImWidget`, `QImPlotTheme`. Public API users interact with.
2. **Core Node System** (`src/core/`) — `QImAbstractNode` → `QImWidgetNode` → `QImGridNode`. Node hierarchy backbone. `beginDraw()`/`endDraw()` lifecycle.
3. **2D Plot Engine** (`src/core/plot/`) — `QImPlot` orchestrator, `QImPlotNode` connects nodes to ImPlot. Individual `QImPlot*ItemNode` classes map to ImPlot plot types.
4. **3D Plot Engine** (`src/core/plot3d/`) — `QImPlot3D` orchestrator, mirrors 2D structure for implot3d.
5. **Data Layer** — `QImAbstractXYDataSeries` (2D), `QImAbstractXYZDataSeries` (3D). Not QObjects; nodes hold pointers, users manage lifetime. `QImDownsamplingController` with LTTB/MinMaxLTTB algorithms.

**CMake targets**: `QIm::Core` (QImCore.dll), `QIm::Widgets` (QImWidgets.dll). 3rdparty sources compiled directly into QImCore (no separate targets). `IMGUI_USER_CONFIG="QImAPI.h"` injects QIm export macros into ImGui/ImPlot.

**DLL export**: `QIM_CORE_API` (Core), `QIM_WIDGETS_API` (Widgets). When `QIM_CORE_DLL` is defined, ImGui/ImPlot API macros replaced with QIm export macros.

## Development Rules (mandatory)

### Qt macros
Forbidden: `slots`/`signals`/`emit`. Required: `Q_SLOTS`/`Q_SIGNALS`/`Q_EMIT`.

### PIMPL (custom macros, not Qt standard)
Defined in `src/QImAPI.h`, uses `std::unique_ptr` (not Qt `d_ptr`):

| Macro | Purpose | Location |
|-------|---------|----------|
| `QIM_DECLARE_PRIVATE(Class)` | Declare PrivateData + d_ptr + d_func() | Header class body |
| `QIM_DECLARE_PUBLIC(Class)` | Declare q_ptr + q_func() in PrivateData | .cpp PrivateData body |
| `QIM_PIMPL_CONSTRUCT` | Initialize d_ptr in constructor | Constructor init list |
| `QIM_D(d)` | Get `PrivateData*` in non-const methods | .cpp function body |
| `QIM_DC(d)` | Get `const PrivateData*` in const methods | .cpp function body |

PIMPL classes: no private member variables in header — all go in PrivateData.

### Comments (mandatory)
- Header public functions: single-line English `//` only, no bilingual Doxygen blocks
- Q_PROPERTY: no comments at all (including Doxygen blocks)
- Class comments: only `@brief`/`@details`/`@note`/`@see`, forbid `@param`/`@class`/`@ingroup`
- Source files (.cpp): bilingual Doxygen (`\if ENGLISH`/`\if CHINESE`)
- Signals: bilingual Doxygen in header (signals have no .cpp definition)

### Render performance (mandatory)
- `beginDraw()` only does API calls passing prepared data — forbid data transforms, conditional assembly, complex computation
- Strings stored as `QByteArray` (UTF8), not `QString`; getters convert UTF8→QString, setters accept QString then immediately convert to UTF8
- All QColor→ImVec4 conversions done in setter; beginDraw uses `constData()` directly
- ImPlot "per-frame set" APIs (SetNextLineStyle etc.) must be called every frame (ImGui immediate-mode, state doesn't persist across frames)

### Enum semantic mapping
- **2D ImPlot negative→Qt positive**: `NoXxx` → `xxxEnabled`, logic inverted (`enabled = (flags & NoXxx) == 0`). Macro: `QIMPLOT_FLAG_ENABLED_ACCESSOR(ClassName, PropName, ImPlotFlag_NoXxx, signal)`
- **2D positive→direct**: `isXxx = (flags & Xxx) != 0`. Macro: `QIMPLOT_FLAG_ACCESSOR(ClassName, PropName, ImPlotFlag_Xxx, signal)`
- **Combined flags** (e.g. CanvasOnly = multiple No flags): manual getter/setter
- **3D ImPlot3D**: flags mostly positive semantics, use direct mapping (no inversion)
- Multiple flag properties share same signal (e.g. `plotFlagChanged()`) since underlying variable is same `ImPlotFlags`
- `beginDraw()` passes `d->flags` directly, no assembly — all assembly in setter via bitwise ops

### Color lazy initialization
`QImOptionalColor` (= `std::optional<QImTrackedValue<ImVec4>>`):
- User didn't set color → first render captures ImPlot auto-assigned default color
- `color()` returns invalid `QColor()` before first render

## New Node Development

Steps:
1. Read ImPlot/ImGui source to understand enums and properties
2. Classify enum semantics (negative/positive/combined)
3. Define `Q_PROPERTY bool xxxEnabled` or `Q_PROPERTY bool xxx` per flag
4. Choose implementation: `QIMPLOT_FLAG_ENABLED_ACCESSOR` (neg→pos) / `QIMPLOT_FLAG_ACCESSOR` (direct) / manual (combined)
5. Store flags as raw `ImPlotFlags` in PrivateData
6. `beginDraw()` passes flags directly, no assembly

Reference: `QImPlotBarsItemNode.h` (full properties+comments), `QImPlotStairsItemNode.h` (simple properties)

Must-read docs before new node work: `docs/zh/dev/coding-standards.md`, `docs/zh/dev/pimpl-dev-guide.md`, `docs/zh/dev/qt-integration.md`, `docs/zh/dev/new-node-guide.md`, `docs/zh/dev/render-guidelines.md`, `docs/zh/dev/flag-mapping.md`

## Node Class Hierarchy

```
QImAbstractNode (beginDraw/endDraw lifecycle, z-order, visible/enabled)
  ├── QImPlotNode (2D plot area, addLine convenience methods)
  │     └── QImPlotItemNode (InnerType=10, type() enum pattern)
  │           ├── QImPlotLineItemNode (Type=11), Scatter, Bars, Stairs, Shaded, ...
  │           └── Tools: DragPoint, DragLineX/Y, DragRect, Annotation, TagX/Y, ValueTracker
  ├── QImPlot3DNode (3D plot area)
  │     └── QImPlot3DItemNode (InnerType3D enum pattern)
  │           ├── QImPlot3DLineItemNode, Scatter, Surface, Mesh, Triangle
  ├── QImSubplotsNode / QImSubplots3DNode
  ├── QImGridNode
  └── QImWidgetNode
```

`beginDraw()` returns `bool`: `true` → render children, `false` → skip. `endDraw()` default empty impl.

## Docs build

MkDocs + i18n plugin (bilingual en/zh site). Doxygen API docs generated by CI and copied to site/. Local: `mkdocs build --clean`.