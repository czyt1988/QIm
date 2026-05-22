# API 参考文档

QIm 的 C++ API 参考文档由 Doxygen 自动生成，包含所有公开类、函数、属性的详细说明。

[:octicons-arrow-right-24: 打开 C++ API 文档](../doxygen/index.html)

---

## 2D 绘图项 (`src/core/plot/`)

2D 绘图项节点均继承自 `QImPlotItemNode`，通过 Qt 属性系统暴露 ImPlot 的图形参数。

| 类名 | 说明 | 详细文档 |
|------|------|----------|
| `QImPlotLineItemNode` | 折线图节点，封装 `ImPlot::PlotLine`，支持线宽、颜色、标记点等属性 | [plot-line.md](plot2d/plot-line.md) |
| `QImPlotScatterItemNode` | 散点图节点，封装 `ImPlot::PlotScatter`，支持标记形状、大小、填充等属性 | [plot-basic-charts.md](plot2d/plot-basic-charts.md) |
| `QImPlotStairsItemNode` | 阶梯图节点，封装 `ImPlot::PlotStairs`，支持阶梯方向（前/后）选择 | [plot-basic-charts.md](plot2d/plot-basic-charts.md) |
| `QImPlotBarsItemNode` | 柱状图节点，封装 `ImPlot::PlotBars`，支持柱宽、方向（水平/垂直）及分组偏移 | [plot-bar-charts.md](plot2d/plot-bar-charts.md) |
| `QImPlotShadedItemNode` | 面积图节点，封装 `ImPlot::PlotShaded`，支持填充透明度和参考基线 | [plot-area-charts.md](plot2d/plot-area-charts.md) |
| `QImPlotErrorBarsItemNode` | 误差棒节点，封装 `ImPlot::PlotErrorBars`，支持正负向误差线独立设置 | [plot-basic-charts.md](plot2d/plot-basic-charts.md) |
| `QImPlotStemsItemNode` | 茎叶图节点，封装 `ImPlot::PlotStems`，支持标记点与基线样式 | [plot-basic-charts.md](plot2d/plot-basic-charts.md) |
| `QImPlotInfLinesItemNode` | 无限线节点，封装 `ImPlot::PlotInfLines`，支持水平/垂直无限参考线 | [plot-special-charts.md](plot2d/plot-special-charts.md) |
| `QImPlotPieChartItemNode` | 饼图节点，封装 `ImPlot::PlotPieChart`，支持标签格式、炸裂效果和角度范围 | [plot-special-charts.md](plot2d/plot-special-charts.md) |
| `QImPlotTextItemNode` | 文本标注节点，封装 `ImPlot::PlotText`，在指定坐标处渲染文本标签 | [plot-annotations.md](plot2d/plot-annotations.md) |
| `QImPlotDummyItemNode` | 占位图例节点，封装 `ImPlot::PlotDummy`，仅在图例中显示条目 | [plot-basic-charts.md](plot2d/plot-basic-charts.md) |
| `QImPlotHistogramItemNode` | 直方图节点，封装 `ImPlot::PlotHistogram`，支持自定义分区数和累积模式 | [plot-basic-charts.md](plot2d/plot-basic-charts.md) |
| `QImPlotHeatmapItemNode` | 热力图节点，封装 `ImPlot::PlotHeatmap`，将二维矩阵数据以颜色网格可视化，支持色彩映射和范围裁剪 | [plot-special-charts.md](plot2d/plot-special-charts.md) |
| `QImPlotHistogram2DItemNode` | 二维直方图节点，封装 `ImPlot::PlotHistogram2D`，以颜色密度图展示双变量联合分布 | [plot-special-charts.md](plot2d/plot-special-charts.md) |
| `QImPlotDigitalItemNode` | 数字信号节点，封装 `ImPlot::PlotDigital`，可视化二值/离散数字信号 | [plot-special-charts.md](plot2d/plot-special-charts.md) |
| `QImPlotImageItemNode` | 图像节点，封装 `ImPlot::PlotImage`，在绘图坐标系中渲染纹理图像 | [plot-special-charts.md](plot2d/plot-special-charts.md) |
| `QImPlotBarGroupsItemNode` | 分组柱状图节点，封装 `ImPlot::PlotBarGroups`，支持多系列并排或堆叠柱状图 | [plot-bar-charts.md](plot2d/plot-bar-charts.md) |

## 2D 交互工具 (`src/core/plot/`)

交互工具节点继承自 `QImPlotItemNode`，提供用户交互能力（拖拽、标签、追踪等）。

| 类名 | 说明 | 详细文档 |
|------|------|----------|
| `QImPlotDragPointNode` | 可拖拽点工具，封装 `ImPlot::DragPoint`，支持鼠标拖拽修改点位置，发出 `positionChanged()` 信号 | [plot-tools.md](plot2d/plot-tools.md) |
| `QImPlotDragLineXNode` | 垂直拖拽线工具，封装 `ImPlot::DragLineX`，支持沿 X 轴拖拽垂直线，发出 `valueChanged()` 信号 | [plot-tools.md](plot2d/plot-tools.md) |
| `QImPlotDragLineYNode` | 水平拖拽线工具，封装 `ImPlot::DragLineY`，支持沿 Y 轴拖拽水平线，发出 `valueChanged()` 信号 | [plot-tools.md](plot2d/plot-tools.md) |
| `QImPlotDragRectNode` | 可拖拽矩形工具，封装 `ImPlot::DragRect`，支持调整矩形位置和尺寸，发出 `rectChanged()` 信号 | [plot-tools.md](plot2d/plot-tools.md) |
| `QImPlotAnnotationNode` | 标注工具，封装 `ImPlot::Annotation`，在指定坐标显示带像素偏移的呼出式文本标签 | [plot-annotations.md](plot2d/plot-annotations.md) |
| `QImPlotTagXNode` | X 轴标签工具，封装 `ImPlot::TagX`，在指定 X 坐标绘制垂直参考线及文本标签 | [plot-tools.md](plot2d/plot-tools.md) |
| `QImPlotTagYNode` | Y 轴标签工具，封装 `ImPlot::TagY`，在指定 Y 坐标绘制水平参考线及文本标签 | [plot-tools.md](plot2d/plot-tools.md) |
| `QImPlotValueTrackerNode` | 智能值追踪器，在鼠标附近数据点显示十字线标注，自动追踪父绘图区中所有可见项目 | [value-tracker.md](plot2d/value-tracker.md) |
| `QImPlotValueTrackerNodeGroup` | 值追踪器组，管理多个追踪器实现跨子图联动光标追踪，支持像素比例同步模式 | [value-tracker.md](plot2d/value-tracker.md) |

## 2D 基础设施 (`src/core/plot/`)

| 类名 | 说明 | 详细文档 |
|------|------|----------|
| `QImPlotNode` | 2D 绘图区节点，封装 `ImPlot::BeginPlot/EndPlot`，管理轴配置、图例、色彩映射和子图项目 | [plot-node.md](plot2d/plot-node.md) |
| `QImPlotColormapManager` | 静态工具类，提供 ImPlot 色彩映射查询和注册 API（查询名称/索引/大小/颜色、采样、注册自定义映射） | — |

## 3D 绘图项 (`src/core/plot3d/`)

3D 绘图项节点均继承自 `QImPlot3DItemNode`。

| 类名 | 说明 | 详细文档 |
|------|------|----------|
| `QImPlot3DLineItemNode` | 三维线图节点，封装 `ImPlot3D::PlotLine`，支持线宽和颜色配置 | [basic-charts.md](plot3d/basic-charts.md) |
| `QImPlot3DScatterItemNode` | 三维散点图节点，封装 `ImPlot3D::PlotScatter`，支持标记填充/轮廓颜色和尺寸 | [basic-charts.md](plot3d/basic-charts.md) |
| `QImPlot3DSurfaceItemNode` | 三维曲面图节点，封装 `ImPlot3D::PlotSurface`，支持填充/线框/标记点三种渲染模式及色彩映射 | [surface-charts.md](plot3d/surface-charts.md) |
| `QImPlot3DTriangleItemNode` | 三维三角形节点，封装 `ImPlot3D::PlotTriangle`，每三个连续点定义三角形 | [mesh.md](plot3d/mesh.md) |
| `QImPlot3DMeshItemNode` | 三维网格节点，封装 `ImPlot3D::PlotMesh`，以顶点/索引方式定义三角形网格 | [mesh.md](plot3d/mesh.md) |
| `QImPlot3DQuadItemNode` | 三维四边形节点，封装 `ImPlot3D::PlotQuad`，每四个连续点定义四边形，支持线/填充/标记独立配置 | [basic-charts.md](plot3d/basic-charts.md) |
| `QImPlot3DImageItemNode` | 三维图像节点，封装 `ImPlot3D::PlotImage`，在 3D 空间中以中心点和两轴方向渲染纹理 | [annotations.md](plot3d/annotations.md) |
| `QImPlot3DTextItemNode` | 三维文本节点，封装 `ImPlot3D::PlotText`，在 3D 坐标处渲染居中文本，支持旋转角度和像素偏移 | [annotations.md](plot3d/annotations.md) |
| `QImPlot3DDummyItemNode` | 三维占位图例节点，封装 `ImPlot3D::PlotDummy`，仅在图例中添加颜色图标条目 | [basic-charts.md](plot3d/basic-charts.md) |

## 3D 基础设施 (`src/core/plot3d/`)

| 类名 | 说明 | 详细文档 |
|------|------|----------|
| `QImPlot3DNode` | 3D 绘图区节点，封装 `ImPlot3D::BeginPlot/EndPlot`，管理轴、视图、图例和 3D 子图项目 | [configuration.md](plot3d/configuration.md) |
| `QImPlot3DStyleNode` | 3D 样式节点，通过 `Q_PROPERTY` 管理所有 `ImPlot3DStyle` 字段和颜色值，支持 Auto/Dark/Light/Classic 主题预设 | [configuration.md](plot3d/configuration.md) |
| `QImPlot3DColormapManager` | 静态工具类，提供 ImPlot3D 色彩映射查询和注册 API（查询、采样、注册自定义映射） | [configuration.md](plot3d/configuration.md) |
| `QImPlot3DAxisTransform` | 抽象接口，用于自定义 3D 坐标轴刻度变换（数据→屏幕 forward 和屏幕→数据 inverse） | [configuration.md](plot3d/configuration.md) |
| `QImPlot3DAxisFormatter` | 抽象基类，用于 3D 坐标轴刻度标签的自定义格式化 | [configuration.md](plot3d/configuration.md) |
| `QImPlot3DDefaultAxisFormatter` | 默认坐标轴格式化器，使用 `%g` 风格进行标准数值格式化输出 | [configuration.md](plot3d/configuration.md) |

## 核心节点 (`src/core/`)

| 类名 | 说明 | 详细文档 |
|------|------|----------|
| `QImAbstractNode` | 抽象节点基类，定义 `beginDraw()`/`endDraw()` 生命周期、z-order、可见性和启用状态管理 | [render-node.md](render-node.md) |
| `QImGridNode` | 通用网格布局节点，支持在同一网格中混合放置 2D 和 3D 子图，基于 Qt 属性系统通知 | [object-tree.md](object-tree.md) |
| `QImSubplots3DNode` | 3D 子图网格节点，继承 `QImGridNode`，提供 `createPlotNode()` 工厂方法创建 `QImPlot3DNode` | [object-tree.md](object-tree.md) |
| `QImWidgetNode` | ImGui 窗口节点基类（Qt 风格 API），封装 `ImGui::Begin/End`，提供 windowTitle/pos/size/contentsMargins 等属性 | [render-node.md](render-node.md) |

## Widgets 组件 (`src/widgets/`)

| 类名 | 说明 | 详细文档 |
|------|------|----------|
| `QImFigureWidget` | 图窗组件，基于 `QOpenGLWidget`，提供 `setSubplotGrid()` 和 `createPlotNode()`/`createPlot3DNode()` 工厂方法 | [figure-widget.md](plot2d/figure-widget.md) |
| `QImWidget` | Widget 基础组件，支持三种渲染模式（Adaptive/Continuous/OnDemand） | [render-mode.md](render-mode.md) |
| `QImPlotTheme` | ImPlot 主题管理器，提供 QColor API 设置非自动颜色，支持 Auto/Classic/Dark/Light 内置主题 | — |

## 工具类 (`src/core/`)

| 类名/文件 | 说明 | 详细文档 |
|-----------|------|----------|
| `QImPlot3DTypes` (头文件) | 3D 数学类型集合：`QImPlot3DPoint`（三维点/向量）、`QImPlot3DRay`（射线）、`QImPlot3DPlane`（平面）、`QImPlot3DBox`（AABB 包围盒）、`QImPlot3DRange`（一维范围），均镜像 ImPlot3D 对应 C 类型 | — |
| `QImWaveformGenerator` | 模板化波形发生器，基于策略（policy-based）设计，内置 SineWave/CosineWave/DampedSineWave/ExponentialDecay/LinearWave，通过 `QIM::make_waveform()` 工厂函数使用 | — |
| `QImSimdArgMinMax` | SIMD 加速 argmin+argmax 单遍历查找，运行时检测 AVX2→SSE4.2→标量回退，返回 `ArgMinMaxResult` 结构体 | — |
| `QImTrackedValue` | 值变化追踪包装器（脏标记模式），自动检测赋值差异并标记脏状态，浮点特化使用模糊比较，提供 `is_dirty()`/`clear()`/`get_and_clear()` 等 API | — |
| `QImFontFileHelper` | 字体文件辅助类，扫描系统字体目录、缓存字族名和文件路径，支持 `getFontFiles()`/`getAvailableFamilies()`/`getRecommendedChineseFontPath()` | [font.md](font.md) |
| `QtImGuiUtils` (头文件) | Qt 与 ImGui/ImPlot/ImPlot3D 类型转换工具集：`toImVec4`/`toQColor`、`toImVec2`/`toQSize`、`toImPlotPoint`/`toQPointF`、`toImPlot3DPoint`/`toQImPlot3DPoint` 等，以及 `fuzzyEqual` 模糊比较和 `QDebug` 支持 | — |

---

### type() 值速查表

`type()` 方法用于快速运行时类型识别，避免 `qobject_cast`/`dynamic_cast` 开销。

| 类别 | 起始基值 | 范围 |
|------|----------|------|
| 2D 绘图项 | `QImPlot3DItemNode::InnerType = 10` | 11–29 |
| 2D 交互工具 | `30` | 30–39 |
| 3D 绘图项 | `QImPlot3DItemNode::InnerType3D = 1000` | 1001– |