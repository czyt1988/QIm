# QIm 项目指南

**Commit:** 6ae8ab2 · **Branch:** feature/sustained-metrics · **Updated:** 2026-05-25

QIm 是一个将 Dear ImGui、ImPlot、ImPlot3D 以保留模式（Retained Mode）封装到 Qt 的 C++17 库。所有图表元素都是 QObject 节点，通过父-子对象树组织。

## 导航

| 任务 | 位置 | 备注 |
|------|----------|-------|
| 新增2D绘图节点 | `src/core/plot/` → 见子目录 AGENTS.md | 本目录有详细2D编码模式 |
| 新增3D绘图节点 | `src/core/plot3d/` → 见子目录 AGENTS.md | 3D与2D有重要差异 |
| 基础节点类 | `src/core/QImAbstractNode.h` | PIMPL例外：基类有私有成员 |
| PIMPL宏定义 | `src/QImAPI.h` | 自定义宏，非Qt标准 |
| 标志宏定义(2D) | `src/core/plot/QImPlot.h:206-244` | 全局宏+本地宏两种风格 |
| 标志宏定义(3D) | `src/core/plot3d/QImPlot3D.h` | QIMPLOT3D_FLAG_* 系列 |
| 开发规范详细版 | `docs/zh/dev/` (9个文件) | 编码前必读 |
| 降采样算法 | `src/core/plot/QImLTTBDownsampler.h` | 仅2D，3D无降采样 |
| 颜色延迟初始化 | `src/core/plot/QImPlotItemNode.h` | QImOptionalColor定义 |
| 构建脚本 | `build.ps1` | 自动探测Qt/VS/CMake |
| Widget层 | `src/widgets/` (7文件) | QImFigureWidget入口 |
| 测试 | `tests/plot/` + `tests/plot3d/` | 纯算法测试，无GUI |

## 构建

**推荐使用一键构建脚本**（自动探测 Qt、VS、CMake 路径，无需手动指定）：

```powershell
# 一键构建（默认 Release + Examples ON）
.\build.ps1

# 常用操作
.\build.ps1 build            # 增量编译
.\build.ps1 rebuild          # 清除 + 重配 + 编译
.\build.ps1 configure -Examples OFF -Benchmark OFF  # 最小化配置（仅主库）
.\build.ps1 help             # 查看所有选项
```

脚本会自动搜索 `D:\Qt`、`C:\Qt` 等目录下的 Qt MSVC 安装，自动检测 VS 版本和 CMake 位置。

**手动构建**（不使用脚本时）：

```powershell
cmake -S . -B build -G "Visual Studio 16 2019" -A x64 -DCMAKE_PREFIX_PATH="<Qt安装路径>"
cmake --build build --config Release
```

**关键注意事项**：
- **必须使用 Visual Studio 生成器**，不要用 Ninja（PowerShell 中 MSVC 环境无法通过 vcvars64.bat 注入）
- **Qt 版本必须与 VS 编译器匹配**：Qt msvc2019 对应 VS2019，Qt msvc2022 对应 VS2022
- Qt 6 需额外链接 `OpenGLWidgets` 组件
- macOS 必须设 `OpenGL 3.3 Core Profile` 默认格式，否则窗口空白
- 构建损坏时先关掉 `build/bin` 下占用的程序，再 `.\build.ps1 rebuild` 重配
- CMake 可能不在 PATH 中（VS2019 内嵌的 CMake 在特殊路径），脚本会自动处理

CMake 选项：`QIM_BUILD_EXAMPLES`(ON)、`QIM_BUILD_TESTS`(OFF)、`QIM_ENABLE_BENCHMARK`(ON)、`QIM_BUILD_QML`(OFF,未完成)。只验主库时用 `.\build.ps1 configure -Examples OFF -Benchmark OFF`。

CI 测试（Ubuntu）需要 `-DQIM_BUILD_TESTS=ON`，运行 `ctest --output-on-failure -R plot`。

## 源码结构

- `src/core/` — 核心库 `QImCore`，直接打包 ImGui+ImPlot+ImPlot3D+qtimgui 源码（无独立第三方库 target）
- `src/core/plot/` — 2D ImPlot 封装，83个文件，扁平目录（有独立 AGENTS.md）
- `src/core/plot3d/` — 3D ImPlot3D 封装，39个文件，扁平目录（有独立 AGENTS.md）
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
- 类的成员变量存储imgui所需要的类型，头文件暴露的接口使用Qt的类型，所有转换在 setter 完成，setter 把qt类型转换为imgui所需要的类型，在beginDraw函数中不需要进行任何的转换直接使用，避免高速刷新的时候带来不必要的转换操作，例如：
  - 所有 QColor→ImVec4 转换在 setter 完成，beginDraw 直接用成员变量 ImVec4
  - QPointF/QPoint同理，一些点的设置在头文件的函数中保留给用户，但如果需要保存则应在setter中转换为ImVec2，作为成员变量保存
- `src\core\QtImGuiUtils.h`定义了ImGui和Qt常见类型的转换，你应该尽量复用，如果发现有ImGui类型和Qt类型需要转换，但`QtImGuiUtils.h`没有定义，你应该判断这个是否是一个通用功能，如果是，你应该加入到`QtImGuiUtils.h`中以便复用
- `src\core\plot\QImPlot.h`定义了ImPlot的枚举和QIm定义的枚举的转换
- ImPlot::SetNextLineStyle 等"每帧设置"API 必须每帧调用（ImGui 即时模式，状态不持久跨帧）

### 枚举语义转换（2D vs 3D 不同）

- **2D ImPlot**：否定语义 `NoXxx` → Qt 肯定语义 `xxxEnabled`，逻辑反转（`enabled = (flags & NoXxx) == 0`）
  - 宏 `QIMPLOT_FLAG_ENABLED_ACCESSOR(ClassName, PropName, ImPlotFlag_NoXxx, signal)` — 自动生成反转 getter/setter
- **2D 肯定语义**：直接映射（`isXxx = (flags & Xxx) != 0`）
  - 宏 `QIMPLOT_FLAG_ACCESSOR(ClassName, PropName, ImPlotFlag_Xxx, signal)` — 直接映射
- **组合标志**（如 `CanvasOnly` = 多个 No 标志组合）：手动实现 getter/setter
- **3D ImPlot3D**：宏逻辑相同（`QIMPLOT3D_FLAG_ENABLED_ACCESSOR`反转 / `QIMPLOT3D_FLAG_ACCESSOR`直接），可见性标志仍用NoXxx反转，样式标志多为肯定映射
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

## 已知偏差（编码注意）

- **QImAbstractNode.h 有私有成员**：基类不遵循PIMPL规则（m_visible/m_enabled/m_zOrder等直接在头文件），所有子类则严格遵循
- **QImPlotNode.h include子类**：`#include "QImPlotLineItemNode.h"`（父include子，非标准Qt模式，为便捷方法addLine服务）
- **标志宏两种风格**：全局宏(`QIMPLOT_FLAG_*`)定义在QImPlot.h；本地宏(`ClassName_FLAG_ACCESSOR`)定义在各.cpp顶部——本地宏更常用
- **PrivateData中flags必须命名`flags`**：标志宏通过`d_ptr->flags`访问，变量名不可改
- **file(GLOB)收集源文件**：CMakeLists用GLOB而非手动列举，新增文件需重配CMake