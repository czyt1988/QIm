# 3D Plot3D (src/core/plot3d)

39文件：19 .h + 19 .cpp + 1 .h(仅QImPlot3DDataSeries.h)。ImPlot3D 3D绘图封装，扁平目录。

## 节点类层次

```
QImAbstractNode
  ├── QImPlot3DNode             (BeginPlot/EndPlot, 3轴, 3D旋转, 图例)
  │    └── 子节点: QImPlot3DItemNode*
  ├── QImPlot3DItemNode         (InnerType3D=1000, type()枚举, 标签, 颜色)
  │    ├── ScatterItemNode      (Type=1001)  LineItemNode (1002)
  │    ├── SurfaceItemNode      (1003)  MeshItemNode (1004)
  │    ├── TriangleItemNode     (1005)  QuadItemNode (1006)
  │    ├── ImageItemNode        (1007)  TextItemNode (1008)
  │    └── DummyItemNode        (1009)
  └── QImSubplots3DNode         (继承QImGridNode, 创建QImPlot3DNode)

QObject (非渲染配置节点):
  ├── QImPlot3DAxisInfo ×3 (X/Y/Z轴)
  ├── QImPlot3DStyleNode     (每Plot3DNode一个)
  ├── QImPlot3DAxisFormatter  (自定义格式化)
  ├── QImPlot3DAxisTransform  (自定义变换)
  └── QImPlot3DColormapManager(颜色映射)
```

**无交互工具**——2D有8种(DragPoint/Annotation等)，3D没有。

## 与2D的关键差异

| 方面 | 2D (plot/) | 3D (plot3d/) |
|------|-----------|--------------|
| type偏移 | InnerType=10 | InnerType3D=1000 |
| 坐标轴 | 4轴槽位(X1/X2/Y1/Y2) | 3轴(X/Y/Z) |
| 数据系列 | QImAbstractXYDataSeries | QImAbstractXYZDataSeries |
| 零拷贝优化 | isContiguous()+stride()+offset() | 无等价优化 |
| 降采样 | LTTB/MinMaxLTTB | **无3D降采样** |
| 交互工具 | 8种 | **无** |
| 颜色捕获 | ImPlot::GetLastItemColor() | captureItemColor()通过内部上下文 |
| Surface网格 | 无(Heatmap用XY+weights) | 需xCount/yCount网格尺寸 |
| Mesh数据 | 无等价 | vertices+indices直接, 无XYZDataSeries |
| 可见性 | ImPlotItem::Show | RenderIgnoreVisible + userVisible |
| 旋转 | 无 | 3D盒子旋转(欧拉角/四元数) |

## 标志宏（QIMPLOT3D_FLAG_*系列）

定义在`QImPlot3D.h`，逻辑与2D宏完全相同，枚举名不同：

```cpp
// 否定→肯定(反转): getter = (flags & NoXxx) == 0
QIMPLOT3D_FLAG_ENABLED_ACCESSOR(PropName, FlagEnum)  // 如 NoTitle→titleEnabled

// 直接映射: getter = (flags & Xxx) != 0
QIMPLOT3D_FLAG_ACCESSOR(PropName, FlagEnum)  // 如 Segments→segmentsEnabled
```

3D元素标志中，**可见性相关仍用NoXxx反转**（NoLines→linesVisible, NoFill→fillVisible），
但**样式相关多为肯定语义**（Segments, Loop, SkipNaN→直接映射）。

## beginDraw()模式

```
QImPlot3DNode::beginDraw() → return true:
  1. ImPlot3D::BeginPlot(title, size, flags)
  2. SetupAxis ×3 + axisInfo->applySetup()
  3. SetupBoxRotation/Scale (欧拉角或四元数)
  4. SetupLegend + styleNode->applyStyle()
  5. PushColormap

QImPlot3DItemNode子类::beginDraw() → return false (无endDraw):
  1. 验证dataSeries && isValid()
  2. 设置样式(SetNextLineStyle等) — 每帧调用
  3. ImPlot3D::PlotXxx(label, xData, yData, zData, size, flags)
  4. 延迟颜色捕获(captureItemColor)
```

## 颜色捕获（3D专用）

```cpp
// 不同于2D的GetLastItemColor(), 3D使用内部上下文:
ImPlot3DContext* ctx = ImPlot3D::GetCurrentContext();
ImPlot3DItem* item = ctx->CurrentItems->GetItem(labelConstData());
if (item) color = ImGui::ColorConvertU32ToFloat4(item->Color);
```

## 3D独有枚举

| 类别 | 2D | 3D |
|------|---|---|
| 轴ID | QImPlotAxisId | QImPlot3DAxisId (X1/Y1/Z1) |
| 比例 | QImPlotScale | QImPlot3DScaleType |
| 标记 | QImPlotMarkerShape | QImPlot3DMarkerShape |
| 颜色映射 | QImPlotColormap | QImPlot3DColormap |
| 样式变量 | QImPlotStyleVar | QImPlot3DStyleVar |
| 平面 | 无 | QImPlane3D (YZ/XZ/XY) |
| 几何 | 无 | QImPlot3DPoint/Ray/Box/Range |

## 参考实现

- **最简单起点**：`QImPlot3DLineItemNode.h`
- **网格数据**：`QImPlot3DSurfaceItemNode.h` (xCount/yCount)
- **顶点索引**：`QImPlot3DMeshItemNode.h` (cube/sphere/duck工厂方法)