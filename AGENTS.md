# QIm 项目指南

QIm 是一个将 Dear ImGui、ImPlot、ImPlot3D 以保留模式（Retained Mode）封装到 Qt 的 C++17 库。所有图表元素都是 QObject 节点，通过父-子对象树组织。

## 构建

必须使用 Visual Studio 生成器，不要使用 Ninja（PowerShell 中 MSVC 环境无法通过 vcvars64.bat 注入）：

```powershell
cmake -S . -B build -G "Visual Studio 16 2019" -A x64 -DCMAKE_PREFIX_PATH="C:/Qt/6.7.3/msvc2019_64"
cmake --build build --config Release
```

Qt 6 需额外链接 `OpenGLWidgets` 组件。macOS 必须设 `OpenGL 3.3 Core Profile` 默认格式，否则窗口空白。构建损坏时先关掉 `build/bin` 下占用的程序，再 `Remove-Item -Recurse -Force build` 重配。

CMake 选项：`QIM_BUILD_EXAMPLES`(ON)、`QIM_BUILD_TESTS`(OFF)、`QIM_ENABLE_BENCHMARK`(ON)、`QIM_BUILD_QML`(OFF,未完成)。只验主库时加 `-DQIM_BUILD_EXAMPLES=OFF -DQIM_ENABLE_BENCHMARK=OFF`。

CI 测试（Ubuntu）需要 `-DQIM_BUILD_TESTS=ON`，运行 `ctest --output-on-failure -R plot`。

## 源码结构

- `src/core/` — 核心库 `QImCore`，直接打包 ImGui+ImPlot+ImPlot3D+qtimgui 源码（无独立第三方库 target）
- `src/core/plot/` — 2D ImPlot 封装，79个文件，扁平目录
- `src/core/plot3d/` — 3D ImPlot3D 封装，39个文件，扁平目录
- `src/widgets/` — Widgets 兼容层 `QImWidgets`（QImFigureWidget + QImWidget + QImPlotTheme）
- CMake 目标名：`QIm::Core`、`QIm::Widgets`（输出文件为 `QImCore.dll`/`QImWidgets.dll`）
- `3rdparty/` — imgui、implot、implot3d、qtimgui（通过 `IMGUI_USER_CONFIG="QImAPI.h"` 注入 QIm 的导出宏）

## 开发规范（必读）

详细规范在 `docs/zh/dev/`，编码前务必阅读。以下是极易违反的要点：

### Qt 宏（强制）

禁止 `slots`/`signals`/`emit`，必须用 `Q_SLOTS`/`Q_SIGNALS`/`Q_EMIT`。

### PIMPL 模式（自定义宏，不是 Qt 标准宏）

宏定义在 `src/QImAPI.h`，使用 `std::unique_ptr` 而非 Qt 原生 `d_ptr`：

| 宏 | 用途 | 位置 |
|---|---|---|
| `QIM_DECLARE_PRIVATE(Class)` | 类中声明 PrivateData + d_ptr + d_func() | 头文件类体内 |
| `QIM_DECLARE_PUBLIC(Class)` | PrivateData 中声明 q_ptr + q_func() | .cpp PrivateData 类体内 |
| `QIM_PIMPL_CONSTRUCT` | 构造函数初始化 d_ptr | 构造函数初始化列表 |
| `QIM_D(d)` | 非const函数获取 `PrivateData*` | .cpp 函数体 |
| `QIM_DC(d)` | const函数获取 `const PrivateData*` | .cpp 函数体 |

使用 PIMPL 的类，头文件中不应有任何私有成员变量定义——全部放 `PrivateData`。

### 注释规范（强制）

- **头文件 public 函数**：仅单行英文 `//`，禁止双语 Doxygen 块
- **Q_PROPERTY**：禁止加任何注释（包括 Doxygen 块）
- **类注释**：仅允许 `@brief`/`@details`/`@note`/`@see`，禁止 `@param`/`@class`/`@ingroup`
- **源文件(.cpp)**：必须双语 Doxygen（`\if ENGLISH`/`\if CHINESE`）
- **信号**：头文件中必须双语 Doxygen（信号没有 .cpp 定义）

### 渲染性能（强制）

- `beginDraw()` 只做 API 调用，直接传递已准备数据——禁止数据转换、条件组装、复杂计算
- 字符串只存 `QByteArray`（UTF8），不存 `QString`；getter 从 UTF8 转 QString，setter 接 QString 后立即转 UTF8
- 所有 QColor→ImVec4 转换在 setter 完成，beginDraw 直接用 `constData()` 传递
- ImPlot::SetNextLineStyle 等"每帧设置"API 必须每帧调用（ImGui 即时模式，状态不持久跨帧）

### 枚举语义转换（2D vs 3D 不同）

- **2D ImPlot**：否定语义 `NoXxx` → Qt 肯定语义 `xxxEnabled`，逻辑反转（`enabled = (flags & NoXxx) == 0`）
  - 宏 `QIMPLOT_FLAG_ENABLED_ACCESSOR(ClassName, PropName, ImPlotFlag_NoXxx, signal)` — 自动生成反转 getter/setter
- **2D 肯定语义**：直接映射（`isXxx = (flags & Xxx) != 0`）
  - 宏 `QIMPLOT_FLAG_ACCESSOR(ClassName, PropName, ImPlotFlag_Xxx, signal)` — 直接映射
- **组合标志**（如 `CanvasOnly` = 多个 No 标志组合）：手动实现 getter/setter
- **3D ImPlot3D**：标志多为肯定语义，使用直接映射而非反转
- 多个标志属性共享同一信号（如 `plotFlagChanged()`），因为底层是同一个 `ImPlotFlags` 变量
- `beginDraw()` 直接传递 `d->flags`，不做组装——所有组装在 setter 通过位操作完成

### 颜色延迟初始化

Plot Item 颜色使用 `QImOptionalColor`（=`std::optional<QImTrackedValue<ImVec4>>`）：
- 用户未设颜色 → 首次渲染时捕获 ImPlot 自动分配的默认颜色
- `color()` 在首次渲染前返回无效 `QColor()`

### DLL 导出

两个导出宏：`QIM_CORE_API`（Core 库）、`QIM_WIDGETS_API`（Widgets 库）。`QIM_CORE_DLL` 定义时，ImGui/ImPlot 的 API 宏被替换为 QIm 导出宏——所有第三方符号通过 QIm DLL 边界导出。

## 节点类层次

```
QImAbstractNode (beginDraw/endDraw 生命周期, z-order, visible/enabled)
  ├── QImPlotNode (2D绘图区, addLine等便捷方法)
  │     └── QImPlotItemNode (InnerType=10, type()枚举模式)
  │           ├── QImPlotLineItemNode (Type=11)
  │           ├── QImPlotScatterItemNode / Bars / Stairs / Shaded / ...
  │           └── 交互工具: DragPoint/DragLineX/DragLineY/DragRect/Annotation/TagX/TagY/ValueTracker
  ├── QImPlot3DNode (3D绘图区)
  │     └── QImPlot3DItemNode (InnerType3D枚举模式)
  │           ├── QImPlot3DLineItemNode / Scatter / Surface / Mesh / Triangle
  ├── QImSubplotsNode / QImSubplots3DNode
  ├── QImGridNode
  └── QImWidgetNode
```

`beginDraw()` 返回 `bool`：`true` 渲染子节点，`false` 跳过。`endDraw()` 默认空实现。

## 数据系列

- 2D：`QImAbstractXYDataSeries`（连续内存优化 `isContiguous()`/`xRawData()`/`stride()`/`offset()`）
- 3D：`QImAbstractXYZDataSeries`（不同抽象）
- 非 `QObject`，节点持有指针但不拥有（用户管理生命周期）

## 测试

- `tests/plot/` — 1个纯算法测试 `tst_downsampler.cpp`
- `tests/plot3d/` — 7个纯单元测试（enums/math_types/conversions/styles/colormaps/utilities/setup_api）
- **无 GUI/渲染测试**——测试系统刻意避免 OpenGL 依赖
- 功能验证通过 `examples/qimfigure-test` 手动运行（TestFunction 基类模式）
- 新测试函数添加见 `examples/HowToAddNewFunction.md`，CMakeLists 用 `GLOB_RECURSE` 自动收录

## 新节点开发步骤

1. 读 ImPlot/ImGui 源码了解枚举和属性
2. 按语义分类枚举（否定/肯定/组合）
3. 为每个标志定义 `Q_PROPERTY bool xxxEnabled` 或 `Q_PROPERTY bool xxx`
4. 选择实现方式：`QIMPLOT_FLAG_ENABLED_ACCESSOR`（否定→肯定）/ `QIMPLOT_FLAG_ACCESSOR`（肯定映射）/ 手动（组合）
5. PrivateData 中用原始 `ImPlotFlags` 存储标志位
6. `beginDraw()` 直接传 flags，不做组装

参考实现：`QImPlotBarsItemNode.h`（完整属性+注释）、`QImPlotStairsItemNode.h`（简单属性）

## 文档构建

MkDocs + i18n 插件（中英双语站点）。Doxygen API 文档由 CI 生成并复制到 site/。本地构建：`mkdocs build --clean`。

## 已知限制

- 不支持自定义线型（虚线/点划线）
- 字体需先加载字体文件
- QIm 内存开销约为 Qwt/QCustomPlot 的 5-15 倍（架构特性）