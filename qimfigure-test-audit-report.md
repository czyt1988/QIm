# qimfigure-test 例子审计报告

> 审计日期：2026-04-28
> 审计目的：确认 `examples/qimfigure-test` 测试例子是否覆盖了所有节点及节点的所有可设置属性（Q_PROPERTY）

---

## 一、节点覆盖情况总览

### 库中所有可测试节点 vs 测试例子覆盖

> 排除抽象基类 QImAbstractNode / QImPlotItemNode / QImPlot3DItemNode，它们不需要独立测试。

| 库中节点类 | 测试例子是否覆盖 | 测试Function名 |
|---|---|---|
| **QImPlotNode** (2D图表容器) | ✅ 所有2D测试都用了 | 各2D Function |
| **QImSubplotsNode** (2D子图网格) | ✅ | SubplotsFunction |
| **QImPlotLineItemNode** | ✅ | Line10K / Line1M |
| **QImPlotScatterItemNode** | ✅ | ScatterFunction |
| **QImPlotBarsItemNode** | ✅ | BarsFunction |
| **QImPlotBarGroupsItemNode** | ✅ | BarGroupsFunction |
| **QImPlotStairsItemNode** | ✅ | StairsFunction |
| **QImPlotShadedItemNode** | ✅ | ShadedFunction |
| **QImPlotStemsItemNode** | ✅ | StemsFunction |
| **QImPlotInfLinesItemNode** | ✅ | InfLinesFunction |
| **QImPlotErrorBarsItemNode** | ✅ | ErrorBarsFunction |
| **QImPlotPieChartItemNode** | ✅ | PieChartFunction |
| **QImPlotHistogramItemNode** | ✅ | HistogramFunction |
| **QImPlotHistogram2DItemNode** | ✅ | Histogram2DFunction |
| **QImPlotHeatmapItemNode** | ✅ | HeatmapFunction |
| **QImPlotDigitalItemNode** | ✅ | DigitalFunction |
| **QImPlotImageItemNode** | ✅ | ImageFunction |
| **QImPlotTextItemNode** | ✅ | TextFunction |
| **QImPlotDummyItemNode** | ✅ | DummyFunction |
| **QImPlotValueTrackerNode** | ✅ | 多个2D Function中使用 |
| **QImPlotAnnotationNode** | ✅ | AnnotationFunction |
| **QImPlotTagXNode** | ✅ | TagsFunction |
| **QImPlotTagYNode** | ✅ | TagsFunction |
| **QImPlotDragPointNode** | ✅ | DragPointFunction |
| **QImPlotDragLineXNode** | ✅ | DragLinesFunction |
| **QImPlotDragLineYNode** | ✅ | DragLinesFunction |
| **QImPlotDragRectNode** | ✅ | DragRectFunction |
| **QImPlotLegendNode** | ✅ 已补充 | LegendFunction |
| **QImPlot3DNode** (3D图表容器) | ✅ | 各3D Function |
| **QImSubplots3DNode** | ✅ | Plot3DSubplotsFunction |
| **QImPlot3DLineItemNode** | ✅ | Plot3DLineFunction |
| **QImPlot3DScatterItemNode** | ✅ | Plot3DScatterFunction |
| **QImPlot3DSurfaceItemNode** | ✅ | Plot3DSurfaceFunction |
| **QImPlot3DTriangleItemNode** | ✅ | Plot3DTriangleFunction |
| **QImPlot3DMeshItemNode** | ✅ | Plot3DMeshFunction |
| **QImPlot3DQuadItemNode** | ✅ | Plot3DQuadFunction |
| **QImPlot3DImageItemNode** | ✅ | Plot3DImageFunction |
| **QImPlot3DTextItemNode** | ✅ | Plot3DTextFunction |
| **QImPlot3DDummyItemNode** | ✅ | Plot3DDummyFunction |

### 节点覆盖结论

除 **QImPlotLegendNode** 外，所有节点都已在测试例子中覆盖。**✅ QImPlotLegendNode 已通过新增 LegendFunction 补充覆盖。**

---

## 二、属性覆盖缺失详细分析

以下每个节点列出：库中实际 Q_PROPERTY → 测试例子中暴露的属性 → **缺失的属性**。

---

### 🔴 QImPlotNode（所有2D Function共用）— 大量属性缺失

**库中Q_PROPERTY：**

| 属性 | 类型 | 说明 |
|---|---|---|
| title | QString | 图表标题 |
| size | QSizeF | 图表尺寸 |
| autoSize | bool | 自动尺寸 |
| titleEnabled | bool | 标题是否可见 |
| legendEnabled | bool | 图例是否可见 |
| mouseTextEnabled | bool | 鼠标位置文字是否可见 |
| inputsEnabled | bool | 输入交互是否开启 |
| menusEnabled | bool | 右键菜单是否开启 |
| boxSelectEnabled | bool | 框选是否开启 |
| frameEnabled | bool | 边框是否可见 |
| equal | bool | XY轴等比例 |
| crosshairs | bool | 十字光标 |
| canvasEnabled | bool | 画布背景 |

**测试中仅暴露：** `title, xLabel, yLabel`（且 xLabel/yLabel 是 AxisInfo 属性，不是 PlotNode 属性）

**缺失的PlotNode属性：**

| ❌ 缺失属性 | 类型 | 说明 |
|---|---|---|
| size | QSizeF | 图表尺寸 |
| autoSize | bool | 自动尺寸 |
| titleEnabled | bool | 标题是否可见 |
| legendEnabled | bool | 图例是否可见 |
| mouseTextEnabled | bool | 鼠标位置文字是否可见 |
| inputsEnabled | bool | 输入交互是否开启 |
| menusEnabled | bool | 右键菜单是否开启 |
| boxSelectEnabled | bool | 框选是否开启 |
| frameEnabled | bool | 边框是否可见 |
| equal | bool | XY轴等比例 |
| crosshairs | bool | 十字光标 |
| canvasEnabled | bool | 画布背景 |

---

### 🔴 QImPlotLineItemNode — 属性缺失严重

**库中Q_PROPERTY：**

| 属性 | 类型 | 说明 |
|---|---|---|
| segments | bool | 分段绘制 |
| loop | bool | 闭合循环 |
| skipNaN | bool | 跳过NaN值 |
| clippingEnabled | bool | 剪裁 |
| shaded | bool | 填充阴影 |
| downsampleAlgorithm | QImDownsampleAlgorithm | 降采样算法 |
| downsampleThreshold | int | 降采样阈值 |

**测试中仅暴露：** `lineColor, lineLabel`（color/label不是Q_PROPERTY，是直接setter）

**缺失的Line属性：**

| ❌ 缺失属性 | 类型 | 说明 |
|---|---|---|
| segments | bool | 分段绘制 |
| loop | bool | 闭合循环 |
| skipNaN | bool | 跳过NaN值 |
| clippingEnabled | bool | 剪裁 |
| shaded | bool | 填充阴影 |
| downsampleAlgorithm | enum | 降采样算法 |
| downsampleThreshold | int | 降采样阈值 |

---

### 🟡 QImPlotScatterItemNode — 属性有少量缺失

**库中Q_PROPERTY：**

| 属性 | 类型 | 说明 |
|---|---|---|
| markerSize | float | 标记大小 |
| markerShape | int | 标记形状 |
| markerFill | bool | 标记填充 |
| downsampleAlgorithm | QImDownsampleAlgorithm | 降采样算法 |
| downsampleThreshold | int | 降采样阈值 |
| color | QColor | 标记颜色 |
| clippingEnabled | bool | 剪裁 |

**测试中暴露：** `markerSize, markerShape, markerFill, markerColor, clippingEnabled`

**缺失属性：**

| ❌ 缺失属性 | 类型 | 说明 |
|---|---|---|
| downsampleAlgorithm | enum | 降采样算法 |
| downsampleThreshold | int | 降采样阈值 |

---

### 🔴 QImPlotBarsItemNode — 属性缺失

**库中Q_PROPERTY：**

| 属性 | 类型 | 说明 |
|---|---|---|
| barWidth | double | 柱宽 |
| horizontal | bool | 水平方向 |
| color | QColor | 柱颜色 |

**测试中暴露：** `barWidth, barColor`

**缺失属性：**

| ❌ 缺失属性 | 类型 | 说明 |
|---|---|---|
| horizontal | bool | 水平方向柱状图 |

---

### ✅ QImPlotBarGroupsItemNode — 覆盖完整

**库中Q_PROPERTY：** `groupWidth, horizontal, stacked, shift, color`

**测试中暴露：** `groupWidth, horizontal, stacked, shift, barColor`

无 Q_PROPERTY 缺失。

---

### ✅ QImPlotStairsItemNode — Q_PROPERTY覆盖完整

**库中Q_PROPERTY：** `shaded, preStep`

**测试中暴露：** `stairsColor, shaded, preStep`（color 是直接 setter，不在 Q_PROPERTY 中）

---

### ✅ QImPlotShadedItemNode — Q_PROPERTY覆盖完整

**库中Q_PROPERTY：** `referenceValue, color`

**测试中暴露：** `fillColor, referenceValue, twoLineMode`（color→fillColor映射；twoLineMode是Function自定义属性，非ShadedItemNode Q_PROPERTY）

---

### ✅ QImPlotStemsItemNode — 覆盖完整

**库中Q_PROPERTY：** `referenceValue, horizontal, color`

**测试中暴露：** `stemsColor, stemsLabel, referenceValue, horizontal`（label 继承自 QImPlotItemNode Q_PROPERTY）

---

### 🔴 QImPlotInfLinesItemNode — 属性缺失

**库中Q_PROPERTY：** `horizontal, color`

**测试中暴露：** `verticalInfLinesColor, horizontalInfLinesColor, lineColor, verticalValues, horizontalValues`

**缺失属性：**

| ❌ 缺失属性 | 类型 | 说明 |
|---|---|---|
| horizontal | bool | 水平模式（InfLines节点本身属性） |

注：values 不在 Q_PROPERTY 中，是直接 setter 方法。

---

### ✅ QImPlotErrorBarsItemNode — Q_PROPERTY覆盖完整

**库中Q_PROPERTY：** `horizontal, color`

**测试中暴露：** `errorColor, horizontalMode`

---

### ✅ QImPlotPieChartItemNode — Q_PROPERTY覆盖完整

**库中Q_PROPERTY：** `center, radius, labelFormat, startAngle, normalize, ignoreHidden, exploding`

**测试中暴露：** `centerX, centerY, radius, labelFormat, startAngle, normalize, ignoreHidden, exploding`（center拆分为centerX/centerY）

---

### ✅ QImPlotHistogramItemNode — Q_PROPERTY覆盖完整

**库中Q_PROPERTY：** `bins, barScale, rangeMin, rangeMax, cumulative, density, horizontal, outliersIncluded, color, colMajor`

**测试中暴露：** `bins, barScale, rangeMin, rangeMax, cumulative, density, horizontal, outliersIncluded, barColor, colMajor`

---

### ✅ QImPlotHistogram2DItemNode — Q_PROPERTY覆盖完整

**库中Q_PROPERTY：** `xBins, yBins, xRangeMin, xRangeMax, yRangeMin, yRangeMax, density, noOutliers, colMajor`

**测试中暴露：** `xBins, yBins, xRangeMin, xRangeMax, yRangeMin, yRangeMax, density, noOutliers, colMajor`

---

### ✅ QImPlotHeatmapItemNode — Q_PROPERTY覆盖完整

**库中Q_PROPERTY：** `scaleMin, scaleMax, labelFormat, boundsMin, boundsMax, colMajor`

**测试中暴露：** `scaleMin, scaleMax, labelFormat, boundsMin/Max, colMajor`

---

### ✅ QImPlotDigitalItemNode — Q_PROPERTY覆盖完整

**库中Q_PROPERTY：** `color`

**测试中暴露：** `digitalColor, digitalLabel`（label 继承自 QImPlotItemNode）

---

### ✅ QImPlotImageItemNode — Q_PROPERTY覆盖完整

**库中Q_PROPERTY：** `textureId, boundsMin, boundsMax, uv0, uv1, tintColor`

**测试中暴露：** `title, xLabel, yLabel, textureId, boundsMin/Max, uv0/uv1, tintColor`

---

### ✅ QImPlotTextItemNode — Q_PROPERTY覆盖完整

**库中Q_PROPERTY：** `text, position, pixelOffset, vertical, color`

**测试中暴露：** `text, textX, textY, pixelOffsetX, pixelOffsetY, textColor, vertical`（position拆分为textX/textY）

---

### ✅ QImPlotDummyItemNode — Q_PROPERTY覆盖完整

**库中Q_PROPERTY：** `color`

**测试中暴露：** `dummyColor, lineColor`

---

### 🔴 QImPlotAnnotationNode — 属性缺失

**库中Q_PROPERTY：** `position, color, text, pixelOffset, clamp, round`

**测试中暴露：** `annotationText, annotationColor, clamp`

**缺失属性：**

| ❌ 缺失属性 | 类型 | 说明 |
|---|---|---|
| position | QPointF | 注释位置 |
| pixelOffset | QPointF | 像素偏移 |
| round | bool | 圆角对齐 |

---

### 🔴 QImPlotDragPointNode — 属性缺失

**库中Q_PROPERTY：** `position, color, size, id, flags, cursorsEnabled, fitEnabled, inputsEnabled, delayed` + 只读 `clicked, hovered, held`

**测试中暴露：** `pointPosition, pointColor, pointSize, cursorsEnabled, fitEnabled, inputsEnabled, delayed`

**缺失属性：**

| ❌ 缺失属性 | 类型 | 说明 |
|---|---|---|
| id | int | 拖拽点ID |
| flags | int | 工具标志 |

（clicked/hovered/held 是只读，不需要在设置面板暴露）

---

### 🔴 QImPlotDragLineXNode / QImPlotDragLineYNode — 属性缺失

**库中Q_PROPERTY：** `value, color, thickness, id, flags, cursorsEnabled, fitEnabled, inputsEnabled, delayed` + 只读 `clicked, hovered, held`

**测试中暴露：** `lineXValue, lineYValue, lineXColor, lineYColor, lineThickness, cursorsEnabled, fitEnabled, inputsEnabled, delayed`

**缺失属性：**

| ❌ 缺失属性 | 类型 | 说明 |
|---|---|---|
| id | int | 拖拽线ID |
| flags | int | 工具标志 |

---

### 🔴 QImPlotDragRectNode — 属性缺失

**库中Q_PROPERTY：** `rect, color, id, flags, cursorsEnabled, fitEnabled, inputsEnabled, delayed` + 只读 `clicked, hovered, held`

**测试中暴露：** `rect, rectColor, cursorsEnabled, fitEnabled, inputsEnabled, delayed`

**缺失属性：**

| ❌ 缺失属性 | 类型 | 说明 |
|---|---|---|
| id | int | 拖拽矩形ID |
| flags | int | 工具标志 |

---

### 🔴 QImPlotTagXNode / QImPlotTagYNode — 属性缺失

**库中Q_PROPERTY：** `value, color, text, round`

**测试中暴露：** `tagXValue, tagXText, tagXColor, tagYValue, tagYText, tagYColor`

**缺失属性：**

| ❌ 缺失属性 | 类型 | 说明 |
|---|---|---|
| round | bool | 像素对齐圆角 |

---

### 🔴 QImSubplotsNode — 属性缺失严重

**库中Q_PROPERTY：** `title, rows, cols, size, isTitleEnabled, isLegendEnabled, isDefaultMenusEnabled, isResizable, isAlignedEnabled, isShareItemsEnabled, isLinkRows, isLinkColumns, isLinkAllX, isLinkAllY, isColumnMajor`

**测试中暴露：** `title, rows, cols, linkAllX, linkAllY, legendEnabled, resizable`

**缺失属性：**

| ❌ 缺失属性 | 类型 | 说明 |
|---|---|---|
| size | QSizeF | 子图区域尺寸 |
| isTitleEnabled | bool | 标题可见 |
| isDefaultMenusEnabled | bool | 默认菜单 |
| isAlignedEnabled | bool | 对齐模式 |
| isShareItemsEnabled | bool | 共享项目 |
| isLinkRows | bool | 链接行 |
| isLinkColumns | bool | 链接列 |
| isColumnMajor | bool | 列主序 |

---

### 🔴 QImPlotLegendNode — 完全缺失，无任何测试

**库中Q_PROPERTY：** `location, orientation, outside, buttonEnabled, highlightItemEnabled, highlightAxisEnabled, sort, reverse`

**✅ 已补充 LegendFunction，覆盖所有8个 Q_PROPERTY**

---

## 三、3D节点属性缺失分析

### 🔴 QImPlot3DNode（所有3D Function共用）— 大量属性缺失

**库中Q_PROPERTY：** `title, size, autoSize, titleEnabled, legendEnabled, mouseTextEnabled, clipEnabled, menusEnabled, rotateEnabled, panEnabled, zoomEnabled, inputsEnabled, equal, canvasEnabled, legendLocation, legendFlags`

**测试中仅暴露：** `title, xLabel, yLabel, zLabel`（label是3DAxisInfo属性）

**缺失的Plot3DNode属性：**

| ❌ 缺失属性 | 类型 | 说明 |
|---|---|---|
| size | QSizeF | 3D图表尺寸 |
| autoSize | bool | 自动尺寸 |
| titleEnabled | bool | 标题可见 |
| legendEnabled | bool | 图例可见 |
| mouseTextEnabled | bool | 鼠标文字 |
| clipEnabled | bool | 剪裁 |
| menusEnabled | bool | 菜单 |
| rotateEnabled | bool | 旋转交互 |
| panEnabled | bool | 平移交互 |
| zoomEnabled | bool | 缩放交互 |
| inputsEnabled | bool | 输入交互 |
| equal | bool | 等比例 |
| canvasEnabled | bool | 画布背景 |
| legendLocation | enum | 图例位置 |
| legendFlags | int | 图例标志 |

---

### 🔴 QImPlot3DLineItemNode — 属性缺失

**库中Q_PROPERTY：** `color, lineWeight, segmentsEnabled, loopEnabled, skipNaNEnabled`

**测试中暴露：** `lineColor, lineWeight`

**缺失属性：**

| ❌ 缺失属性 | 类型 | 说明 |
|---|---|---|
| segmentsEnabled | bool | 分段绘制 |
| loopEnabled | bool | 闭合循环 |
| skipNaNEnabled | bool | 跳过NaN |

---

### 🔴 QImPlot3DScatterItemNode — 属性缺失严重

**库中Q_PROPERTY：** `markerFillColor, markerOutlineColor, markerSize, markerWeight, markerShape`

**测试中暴露：** `markerSize, markerShape, markerColor`（markerColor → 应对应 markerFillColor，但缺失 markerOutlineColor 和 markerWeight）

**缺失属性：**

| ❌ 缺失属性 | 类型 | 说明 |
|---|---|---|
| markerFillColor | QColor | 标记填充色（需独立于markerColor） |
| markerOutlineColor | QColor | 标记轮廓色 |
| markerWeight | float | 标记轮廓粗细 |

---

### 🔴 QImPlot3DSurfaceItemNode — 属性缺失严重

**库中Q_PROPERTY：** `xCount, yCount, linesVisible, fillVisible, markersVisible, markerShape, markerSize, markerWeight, fillColor, lineColor, markerFillColor, markerOutlineColor, lineWidth, fillAlpha, colormapEnabled, colormap, colormapScaleMin, colormapScaleMax`

**测试中暴露：** `fillColor, lineColor, lineWidth, linesVisible, fillVisible, markersVisible, colormapEnabled`

**缺失属性：**

| ❌ 缺失属性 | 类型 | 说明 |
|---|---|---|
| xCount | int | X方向网格数 |
| yCount | int | Y方向网格数 |
| markerShape | int | 标记形状 |
| markerSize | float | 标记大小 |
| markerWeight | float | 标记轮廓粗细 |
| markerFillColor | QColor | 标记填充色 |
| markerOutlineColor | QColor | 标记轮廓色 |
| fillAlpha | float | 填充透明度 |
| colormap | int | 颜色映射类型 |
| colormapScaleMin | double | 颜色映射最小值 |
| colormapScaleMax | double | 颜色映射最大值 |

---

### 🔴 QImPlot3DMeshItemNode / Triangle / Quad — 属性缺失严重

**库中Q_PROPERTY（Mesh/Triangle/Quad共用）：** `linesVisible, fillVisible, markersVisible, markerShape, markerSize, markerWeight, fillColor, lineColor, markerFillColor, markerOutlineColor, lineWidth, fillAlpha`（Mesh额外：`perTriangleFillColorEnabled, perTriangleLineColorEnabled`）

**测试中暴露：** `fillColor, lineColor`

**缺失属性（所有3个节点）：**

| ❌ 缺失属性 | 类型 | 说明 |
|---|---|---|
| linesVisible | bool | 线条可见 |
| fillVisible | bool | 填充可见 |
| markersVisible | bool | 标记可见 |
| markerShape | int | 标记形状 |
| markerSize | float | 标记大小 |
| markerWeight | float | 标记轮廓粗细 |
| markerFillColor | QColor | 标记填充色 |
| markerOutlineColor | QColor | 标记轮廓色 |
| lineWidth | float | 线宽 |
| fillAlpha | float | 填充透明度 |
| perTriangleFillColorEnabled* | bool | Mesh节点专有 |
| perTriangleLineColorEnabled* | bool | Mesh节点专有 |

---

### 🔴 QImPlot3DImageItemNode — 属性缺失严重

**库中Q_PROPERTY：** `textureId, centerX/centerY/centerZ, axisUX/UY/UZ, axisVX/VY/VZ, uv0X/uv0Y, uv1X/uv1Y, tintColor, quadModeEnabled, p0x/p0y/p0z, p1x/p1y/p1z, p2x/p2y/p2z, p3x/p3y/p3z, uvP0x/uvP0y, uvP1x/uvP1y, uvP2x/uvP2y, uvP3x/uvP3y`

**测试中仅暴露：** `title, xLabel, yLabel, zLabel`

**几乎所有Q_PROPERTY缺失！**

---

### 🔴 QImPlot3DTextItemNode — 属性缺失

**库中Q_PROPERTY：** `text, x, y, z, angle, pixelOffsetX, pixelOffsetY, color`

**测试中暴露：** `title, xLabel, yLabel, zLabel, text, color`

**缺失属性：**

| ❌ 缺失属性 | 类型 | 说明 |
|---|---|---|
| x | double | 文本X位置 |
| y | double | 文本Y位置 |
| z | double | 文本Z位置 |
| angle | double | 旋转角度 |
| pixelOffsetX | double | 像素偏移X |
| pixelOffsetY | double | 像素偏移Y |

---

## 四、AxisInfo属性缺失分析

**QImPlotAxisInfo** 和 **QImPlot3DAxisInfo** 虽然不是 Node（继承 QObject），但它们是关键的配置对象，每个 2D/3D Function 都在使用。目前测试中仅通过 xLabel/yLabel 覆盖了 `label` 属性。

### 缺失的2D AxisInfo属性（每个测试Function都应覆盖）

| ❌ 缺失属性 | 类型 | 说明 |
|---|---|---|
| minLimits | double | 轴最小值 |
| maxLimits | double | 轴最大值 |
| autoFit | bool | 自动适配 |
| inverted | bool | 轴反转 |
| labelEnabled | bool | 标签可见 |
| gridLinesEnabled | bool | 网格线 |
| tickMarksEnabled | bool | 刻度线 |
| tickLabelsEnabled | bool | 刻度标签 |
| lockMin | bool | 锁定最小值 |
| lockMax | bool | 锁定最大值 |
| noDecorations | bool | 无装饰 |
| scaleType | enum | 缩放类型（线性/对数等） |

### 缺失的3D AxisInfo属性（每个3D Function都应覆盖）

| ❌ 缺失属性 | 类型 | 说明 |
|---|---|---|
| minLimit / maxLimit | double | 轴范围 |
| labelEnabled | bool | 标签 |
| gridLinesEnabled | bool | 网格 |
| tickMarksEnabled / tickLabelsEnabled | bool | 刻度 |
| lockMin / lockMax | bool | 锁定 |
| autoFit | bool | 自动 |
| invert | bool | 反转 |
| decorationsEnabled | bool | 装饰 |
| scale | enum | 缩放类型 |

---

## 五、总结与优先级建议

### 🔴 严重缺失（需新增测试Function）

| 优先级 | 缺失内容 | 建议操作 |
|---|---|---|
| P0 | **QImPlotLegendNode** — ✅ 已补充 | 新增 LegendFunction |

### 🔴 属性缺失最严重的节点（优先补充）

| 优先级 | 节点 | 缺失属性数量 | 建议操作 |
|---|---|---|---|
| P0 | QImPlotNode | 13个 | 在一个典型2D Function中补充所有PlotNode属性 |
| P0 | QImPlot3DNode | 15个 | 在一个典型3D Function中补充所有3DPlotNode属性 |
| P0 | QImPlotLineItemNode | 7个 | 在Line10K或Line1M中补充 |
| P1 | QImPlot3DSurfaceItemNode | 11个 | 在Plot3DSurfaceFunction中补充 |
| P1 | QImPlot3DMesh / Triangle / Quad | 12-13个 | 在对应3D Function中补充 |
| P1 | QImPlot3DImageItemNode | ~30个 | 在Plot3DImageFunction中补充 |
| P1 | QImPlot3DScatterItemNode | 3个 | 在Plot3DScatterFunction中补充 |
| P1 | QImPlot3DTextItemNode | 6个 | 在Plot3DTextFunction中补充 |
| P1 | QImPlotAxisInfo | ~13个 | 在一个典型2D Function中补充Axis属性 |
| P1 | QImPlot3DAxisInfo | ~11个 | 在一个典型3D Function中补充3D Axis属性 |
| P2 | QImPlotAnnotationNode | 3个 | 补充 position, pixelOffset, round |
| P2 | QImSubplotsNode | 8个 | 补充子图属性 |
| P2 | QImPlotTagX/YNode | 1个 | 补充 round |
| P2 | DragPoint / Line / Rect | 2个 | 补充 id, flags |
| P2 | QImPlotBarsItemNode | 1个 | 补充 horizontal |
| P2 | QImPlotInfLinesItemNode | 1个 | 补充 horizontal |
| P2 | QImPlot3DLineItemNode | 3个 | 补充 segments/loop/skipNaN |

### ✅ Q_PROPERTY覆盖完整的节点（无需修改）

- QImPlotBarGroupsItemNode
- QImPlotStairsItemNode（Q_PROPERTY层面）
- QImPlotShadedItemNode（Q_PROPERTY层面）
- QImPlotStemsItemNode
- QImPlotErrorBarsItemNode
- QImPlotPieChartItemNode
- QImPlotHistogramItemNode
- QImPlotHistogram2DItemNode
- QImPlotHeatmapItemNode
- QImPlotDigitalItemNode
- QImPlotImageItemNode（Q_PROPERTY层面）
- QImPlotTextItemNode（Q_PROPERTY层面）
- QImPlotDummyItemNode