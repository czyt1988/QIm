# 2D Plot (src/core/plot)

83文件：44头文件 + 38 .cpp + 1 .hpp。ImPlot 2D绘图封装，扁平目录无子文件夹。

## 节点类层次

```
QImAbstractNode
  ├── QImPlotNode             (BeginPlot/EndPlot, plotFlags, 轴, 图例)
  │    └── QImSubplotsNode    (子图网格)
  ├── QImPlotItemNode         (InnerType=10, type()枚举, 标签, 颜色)
  │    ├── LineItemNode       (Type=11)  ScatterItemNode  BarsItemNode
  │    ├── StairsItemNode     ShadedItemNode  StemsItemNode
  │    ├── ErrorBarsItemNode  InfLinesItemNode  PieChartItemNode
  │    ├── TextItemNode       DummyItemNode  HistogramItemNode
  │    ├── Histogram2DItemNode HeatmapItemNode  ImageItemNode
  │    ├── DigitalItemNode    BarGroupsItemNode
  │    └── 交互工具: DragPoint/DragLineX/DragLineY/DragRect
  │                Annotation/TagX/TagY/ValueTracker/ValueTrackerGroup
  ├── QImPlotLegendNode       (独立图例配置)
  └── QImPlotAxisInfo         (轴配置, 非节点, 有QIM_DECLARE_PRIVATE)
```

## 文件命名

| 模式 | 示例 | 数量 |
|------|------|------|
| `QImPlotNode.*` — 绘图区+轴+图例 | QImPlotNode, QImPlotAxisInfo, QImPlotLegendNode | 6 |
| `QImPlot<Type>ItemNode.*` — 绘图项 | QImPlotLineItemNode, QImPlotBarsItemNode | 40 |
| `QImPlot<Tool>Node.*` — 交互工具 | QImPlotDragPointNode, QImPlotTagXNode | 12 |
| `QImPlot<Type>DataSeries.h` — 数据系列 | QImPlotDataSeries, QImPlotHistogramDataSeries | 7 |
| `QIm<Algo>Downsampler.*` — 降采样 | QImLTTBDownsampler, QImMinMaxLTTBDownsampler | 6 |
| 其他 | QImPlot.h, QImSimdArgMinMax, QImWaveformGenerator | 4 |

## 标志宏（2D本地风格，最常用）

每个.cpp文件顶部定义本地宏，比全局QIMPLOT_FLAG_*更常用：

```cpp
// QImPlotNode.cpp顶部:
#define QImPlotNode_ENABLED_ACCESSOR(PropName, FlagEnum) ...
#define QImPlotNode_FLAG_ACCESSOR(PropName, FlagEnum) ...

// 使用:
QImPlotNode_ENABLED_ACCESSOR(TitleEnabled, ImPlotFlags_NoTitle)  // 否定→肯定
QImPlotNode_FLAG_ACCESSOR(Equal, ImPlotFlags_Equal)               // 直接映射
```

**PrivateData中flags变量必须命名为`flags`**——宏通过`d_ptr->flags`访问。

## beginDraw()模式

```
QImPlotNode::beginDraw() → return true (渲染子节点):
  1. auto-fit检查 → ImPlot::SetNextAxesToFit()
  2. ImPlot::BeginPlot(titleUtf8.constData(), size, plotFlags)
  3. 设置轴/图例/颜色映射
  4. viewport变化 → Q_EMIT plotLimitsChanged()

QImPlotItemNode子类::beginDraw() → return false (无endDraw):
  1. 检查d->data → 无数据则return false
  2. 自适应降采样(如果启用)
  3. 颜色: d->color.has_value() → SetNextLineStyle (每帧调用)
  4. isContiguous() → 零拷贝fast path 或 回调慢路径
  5. ImPlot::PlotXxx(labelConstData(), ...)
  6. 首次渲染捕获默认颜色
```

## 数据系列零拷贝路径

```cpp
// 连续XY模式(零拷贝):
ImPlot::PlotLine(label, xRawData, yRawData, size, flags, offset, stride);

// 连续Y-only模式(零拷贝):
ImPlot::PlotLine(label, yRawData, size, xScale, xStart, flags, offset, stride);

// 非连续回退(回调):
ImPlot::PlotLineG(label, callback, data, size, flags);
```

## 颜色延迟初始化

`QImOptionalColor = std::optional<QImTrackedValue<ImVec4>>`：
- 用户未设 → 首次渲染捕获ImPlot自动分配的颜色
- `color()` 渲染前返回无效 `QColor()`
- setColor()中完成QColor→ImVec4转换，beginDraw直接用`constData()`

## 参考实现

- **完整属性+注释**：`QImPlotBarsItemNode.h`
- **简单属性**：`QImPlotStairsItemNode.h`
- **交互工具**：`QImPlotDragPointNode.h`
- **降采样**：`QImLTTBDownsampler.h`