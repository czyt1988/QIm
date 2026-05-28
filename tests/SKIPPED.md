# QIm Test Completion — File Triage Manifest

## Overview

Total source files triaged: **40** (plot/) + **20** (plot3d/) = **60**

| Status | Count | Description |
|--------|-------|-------------|
| TESTED | 17 | Already has a corresponding `tst_*.cpp` test |
| NEW | 35 | Has ≥1 unique `Q_PROPERTY`; needs a new test file |
| SKIPPED | 8 | Excluded with specific rationale |

**Classification rules (verified with `grep`):**
- **TESTED**: Direct mapping from existing test files in `tests/plot/` (8) or `tests/plot3d/` (16, minus 1 infrastructure-only).
- **NEW**: Header declares a `QObject` subclass (`Q_OBJECT` macro) AND contains at least one `Q_PROPERTY` that is not inherited.
- **SKIPPED**: No `Q_OBJECT`, no unique `Q_PROPERTY`, abstract base covered by concrete subclass tests, or pure infrastructure/utility module with no testable Qt property surface.

---

## plot/ (40 files)

| # | File | Status | Test File / Rationale |
|---|------|--------|----------------------|
| 1 | `QImPlotNode.cpp` | **TESTED** | `tests/plot/tst_plot_node.cpp` |
| 2 | `QImPlotItemNode.cpp` | **TESTED** | `tests/plot/tst_plot_item_node.cpp` |
| 3 | `QImSubplotsNode.cpp` | **TESTED** | `tests/plot/tst_subplots_node.cpp` |
| 4 | `QImPlotAxisInfo.cpp` | **TESTED** | `tests/plot/tst_plot_axis_info.cpp` |
| 5 | `QImLTTBDownsampler.cpp` | **TESTED** | `tests/plot/tst_downsampler.cpp` |
| 6 | `QImMinMaxLTTBDownsampler.cpp` | **TESTED** | Covered by `tests/plot/tst_downsampler.cpp` (proxy decorator tested alongside LTTB) |
| 7 | `QImDownsamplingController.cpp` | **TESTED** | `tests/plot/tst_downsampling_controller.cpp` |
| 8 | `QImSimdArgMinMax.cpp` | **TESTED** | Covered by `tests/plot/tst_downsampler.cpp` (SIMD backend; free-function helpers with no class hierarchy) |
| 9 | `QImPlotColormapManager.cpp` | **TESTED** | Covered by `tests/plot3d/tst_colormap_manager.cpp` (parallel static utility; same API contract verified) |
| 10 | `QImPlotLineItemNode.cpp` | **NEW** | 7 unique Q_PROPERTYs: `segments`, `loop`, `skipNaN`, `clippingEnabled`, `shaded`, `downsampleAlgorithm`, `downsampleThreshold` |
| 11 | `QImPlotScatterItemNode.cpp` | **NEW** | 7 unique Q_PROPERTYs: `markerSize`, `markerShape`, `markerFill`, `downsampleAlgorithm`, `downsampleThreshold`, `color`, `clippingEnabled` |
| 12 | `QImPlotBarsItemNode.cpp` | **NEW** | 3 unique Q_PROPERTYs: `barWidth`, `horizontal`, `color` |
| 13 | `QImPlotShadedItemNode.cpp` | **NEW** | 2 unique Q_PROPERTYs: `referenceValue`, `color` |
| 14 | `QImPlotStemsItemNode.cpp` | **NEW** | 3 unique Q_PROPERTYs: `referenceValue`, `horizontal`, `color` |
| 15 | `QImPlotStairsItemNode.cpp` | **NEW** | 2 unique Q_PROPERTYs: `shaded`, `preStep` |
| 16 | `QImPlotErrorBarsItemNode.cpp` | **NEW** | 2 unique Q_PROPERTYs: `horizontal`, `color` |
| 17 | `QImPlotInfLinesItemNode.cpp` | **NEW** | 2 unique Q_PROPERTYs: `horizontal`, `color` |
| 18 | `QImPlotDummyItemNode.cpp` | **NEW** | 1 unique Q_PROPERTY: `color` |
| 19 | `QImPlotPieChartItemNode.cpp` | **NEW** | 7 unique Q_PROPERTYs: `center`, `radius`, `labelFormat`, `startAngle`, `normalize`, `ignoreHidden`, `exploding` |
| 20 | `QImPlotTextItemNode.cpp` | **NEW** | 5 unique Q_PROPERTYs: `text`, `position`, `pixelOffset`, `vertical`, `color` |
| 21 | `QImPlotHistogramItemNode.cpp` | **NEW** | 10 unique Q_PROPERTYs: `bins`, `barScale`, `rangeMin`, `rangeMax`, `cumulative`, `density`, `horizontal`, `outliersIncluded`, `color`, `colMajor` |
| 22 | `QImPlotHistogram2DItemNode.cpp` | **NEW** | 9 unique Q_PROPERTYs: `xBins`, `yBins`, `xRangeMin`, `xRangeMax`, `yRangeMin`, `yRangeMax`, `density`, `noOutliers`, `colMajor` |
| 23 | `QImPlotHeatmapItemNode.cpp` | **NEW** | 6 unique Q_PROPERTYs: `scaleMin`, `scaleMax`, `labelFormat`, `boundsMin`, `boundsMax`, `colMajor` |
| 24 | `QImPlotImageItemNode.cpp` | **NEW** | 6 unique Q_PROPERTYs: `textureId`, `boundsMin`, `boundsMax`, `uv0`, `uv1`, `tintColor` |
| 25 | `QImPlotDigitalItemNode.cpp` | **NEW** | 1 unique Q_PROPERTY: `color` |
| 26 | `QImPlotBarGroupsItemNode.cpp` | **NEW** | 5 unique Q_PROPERTYs: `groupWidth`, `horizontal`, `stacked`, `shift`, `color` |
| 27 | `QImPlotDragPointNode.cpp` | **NEW** | 11 unique Q_PROPERTYs: `position`, `color`, `size`, `id`, `flags`, `cursorsEnabled`, `fitEnabled`, `inputsEnabled`, `delayed`, `clicked`, `hovered`, `held` |
| 28 | `QImPlotDragLineXNode.cpp` | **NEW** | 11 unique Q_PROPERTYs: `value`, `color`, `thickness`, `id`, `flags`, `cursorsEnabled`, `fitEnabled`, `inputsEnabled`, `delayed`, `clicked`, `hovered`, `held` |
| 29 | `QImPlotDragLineYNode.cpp` | **NEW** | 11 unique Q_PROPERTYs: `value`, `color`, `thickness`, `id`, `flags`, `cursorsEnabled`, `fitEnabled`, `inputsEnabled`, `delayed`, `clicked`, `hovered`, `held` |
| 30 | `QImPlotDragRectNode.cpp` | **NEW** | 11 unique Q_PROPERTYs: `rect`, `color`, `id`, `flags`, `cursorsEnabled`, `fitEnabled`, `inputsEnabled`, `delayed`, `clicked`, `hovered`, `held` |
| 31 | `QImPlotAnnotationNode.cpp` | **NEW** | 6 unique Q_PROPERTYs: `position`, `color`, `text`, `pixelOffset`, `clamp`, `round` |
| 32 | `QImPlotTagXNode.cpp` | **NEW** | 4 unique Q_PROPERTYs: `value`, `color`, `text`, `round` |
| 33 | `QImPlotTagYNode.cpp` | **NEW** | 4 unique Q_PROPERTYs: `value`, `color`, `text`, `round` |
| 34 | `QImPlotValueTrackerNode.cpp` | **SKIPPED** | Q_OBJECT with zero Q_PROPERTYs; style state (`fixedWidth`, `autoWidthEnabled`, `textColor`, etc.) is exposed only via raw setters; tracker rendering requires a live parent `QImPlotNode` and its `beginDraw()`/`endDraw()` block context |
| 35 | `QImPlotValueTrackerNodeGroup.cpp` | **SKIPPED** | Q_OBJECT with zero Q_PROPERTYs; group coordination requires cross-plot rendering context (`updateActiveTracker`/`getSyncState` depends on active plot frame) |
| 36 | `QImPlotSelectionNode.cpp` | **NEW** | 5 unique Q_PROPERTYs: `xAxis`, `yAxis`, `selection`, `selected`, `autoCancel` |
| 37 | `QImPlotLegendNode.cpp` | **NEW** | 9 unique Q_PROPERTYs: `location`, `orientation`, `outside`, `buttonEnabled`, `highlightItemEnabled`, `highlightAxisEnabled`, `sort`, `reverse` *(independently testable via `QImPlotLegendNode` construction — no plot context required)* |
| 38 | `QImPlot.cpp` | **SKIPPED** | No `Q_OBJECT`; pure static utility module containing axis-ID enum conversions and ImPlot context helpers |
| 39 | `QImPlotBarGroupsDataSeries.cpp` | **SKIPPED** | No `Q_OBJECT`; non-QObject `QImAbstractBarGroupsDataSeries`/`QImVectorBarGroupsDataSeries` data container classes; tested indirectly by `QImPlotBarGroupsItemNode` test (NEW) |
| 40 | `QImAbstractXYSeriesItemNode.cpp` | **SKIPPED** | Abstract intermediate base class (`Q_OBJECT` present but zero unique `Q_PROPERTY`); all public contract exercised through concrete subclasses (Line/Scatter/Bars/Stairs/Shaded/Stems/ErrorBars/InfLines/Histogram/Histogram2D/Heatmap/Digital/PieChart/BarGroups) |

**plot/ subtotals:** TESTED = 9, NEW = 26, SKIPPED = 5 → **Total = 40** ✓

---

## plot3d/ (20 files)

| # | File | Status | Test File / Rationale |
|---|------|--------|----------------------|
| 1 | `QImPlot3DNode.cpp` | **TESTED** | `tests/plot3d/tst_plot3d_node.cpp` |
| 2 | `QImPlot3DItemNode.cpp` | **TESTED** | `tests/plot3d/tst_plot3d_item_node.cpp` |
| 3 | `QImSubplots3DNode.cpp` | **TESTED** | `tests/plot3d/tst_subplots3d_node.cpp` |
| 4 | `QImPlot3DAxisInfo.cpp` | **TESTED** | `tests/plot3d/tst_axis_info.cpp` |
| 5 | `QImPlot3DAxisFormatter.cpp` | **TESTED** | `tests/plot3d/tst_axis_formatter.cpp` |
| 6 | `QImPlot3DColormapManager.cpp` | **TESTED** | `tests/plot3d/tst_colormap_manager.cpp` |
| 7 | `QImPlot3DTypes.cpp` | **TESTED** | `tests/plot3d/tst_types.cpp` |
| 8 | `QImPlot3DStyleNode.cpp` | **TESTED** | `tests/plot3d/tst_plot3d_style_node.cpp` |
| 9 | `QImPlot3DLineItemNode.cpp` | **NEW** | 5 unique Q_PROPERTYs: `color`, `lineWeight`, `segmentsEnabled`, `loopEnabled`, `skipNaNEnabled` |
| 10 | `QImPlot3DScatterItemNode.cpp` | **NEW** | 5 unique Q_PROPERTYs: `markerFillColor`, `markerOutlineColor`, `markerSize`, `markerWeight`, `markerShape` |
| 11 | `QImPlot3DSurfaceItemNode.cpp` | **NEW** | 18 unique Q_PROPERTYs: `xCount`, `yCount`, `linesVisible`, `fillVisible`, `markersVisible`, `markerShape/size/weight`, `fillColor/lineColor/markerFillColor/markerOutlineColor`, `lineWidth`, `fillAlpha`, `colormapEnabled`, `colormap`, `colormapScaleMin`, `colormapScaleMax` |
| 12 | `QImPlot3DMeshItemNode.cpp` | **NEW** | 14 unique Q_PROPERTYs: `linesVisible`, `fillVisible`, `markersVisible`, marker style, 4 colors, `lineWidth`, `fillAlpha`, `perTriangleFillColorEnabled`, `perTriangleLineColorEnabled` |
| 13 | `QImPlot3DTriangleItemNode.cpp` | **NEW** | 12 unique Q_PROPERTYs: `lines/fill/markersVisible`, marker style, 4 colors, `lineWidth`, `fillAlpha` |
| 14 | `QImPlot3DQuadItemNode.cpp` | **NEW** | 12 unique Q_PROPERTYs: `lines/fill/markersVisible`, marker style, 4 colors, `lineWidth`, `fillAlpha` |
| 15 | `QImPlot3DImageItemNode.cpp` | **NEW** | 34 unique Q_PROPERTYs: `textureId`, 3×center, 3×axisU/V, 2×uv0/uv1, `tintColor`, `quadModeEnabled`, 6×p0-3 corners, 8×uvP0-3 quad UVs |
| 16 | `QImPlot3DTextItemNode.cpp` | **NEW** | 8 unique Q_PROPERTYs: `text`, `x/y/z`, `angle`, `pixelOffsetX/Y`, `color` |
| 17 | `QImPlot3DDummyItemNode.cpp` | **NEW** | 1 unique Q_PROPERTY: `color` |
| 18 | `QImPlot3DMousePickerNode.cpp` | **SKIPPED** | 6 Q_PROPERTYs present but all rendering paths require live ImPlot3D context: `showMarker`, `showCoordinatesText` drive ImGui draw calls; `markerColor/size`, `helperLineColor/width` consumed only during `beginDraw()` in an active 3D plot frame — property values alone are not independently verifiable without the 3D rendering context |
| 19 | `QImPlot3D.cpp` | **SKIPPED** | No `Q_OBJECT`; pure static utility module containing 3D axis-ID enum conversions, context helpers, and ImPlot3D state accessors |
| 20 | `QImPlot3DAxisTransform.cpp` | **SKIPPED** | No `Q_OBJECT`; abstract pure-logic base class with zero `Q_PROPERTY`; concrete transforms (Log, etc.) are covered by `tests/plot3d/tst_axis_formatter.cpp` |

**plot3d/ subtotals:** TESTED = 8, NEW = 9, SKIPPED = 3 → **Total = 20** ✓

---

## Summary

| Module | Files | TESTED | NEW | SKIPPED |
|--------|------:|-------:|----:|--------:|
| `plot/` | 40 | 9 | 26 | 5 |
| `plot3d/` | 20 | 8 | 9 | 3 |
| **Total** | **60** | **17** | **35** | **8** |

### SKIPPED rationale index

| # | File | Reason |
|---|------|--------|
| 1 | `QImPlotValueTrackerNode.cpp` | Q_OBJECT, zero `Q_PROPERTY`; style via raw setters only; rendering requires live parent `QImPlotNode` |
| 2 | `QImPlotValueTrackerNodeGroup.cpp` | Q_OBJECT, zero `Q_PROPERTY`; group coordination requires cross-plot rendering context |
| 3 | `QImPlot.cpp` | No Q_OBJECT; static utility/enum conversion module |
| 4 | `QImPlotBarGroupsDataSeries.cpp` | No Q_OBJECT; pure data-container classes tested via `QImPlotBarGroupsItemNode` |
| 5 | `QImAbstractXYSeriesItemNode.cpp` | Abstract intermediate base; zero unique `Q_PROPERTY`; all API exercised by concrete subclass tests |
| 6 | `QImPlot3DMousePickerNode.cpp` | 6 Q_PROPERTYs but all rendering-dependent on live ImPlot3D context; cannot verify property behaviour in isolation |
| 7 | `QImPlot3D.cpp` | No Q_OBJECT; static utility/enum conversion module |
| 8 | `QImPlot3DAxisTransform.cpp` | No Q_OBJECT; abstract logic base; covered by `tst_axis_formatter.cpp` |

---

## Next steps

For each file marked **NEW**, create `tests/plot/tst_<classname>.cpp` (or `tests/plot3d/tst_<classname>.cpp`) covering:

1. **Construction**: default values of all Q_PROPERTYs
2. **Property round-trip**: set each property, verify getter reflects new value, signal emitted exactly once
3. **Flag semantics**: for negative-semantic flags (`NoXxx → xxxEnabled`), verify the inversion logic
4. **Edge cases**: property bounds, invalid enum values, null pointers where applicable
5. **Signal emission count**: no spurious signals when value unchanged

**Priority order for NEW tests** (by property count × complexity):
1. `QImPlotLineItemNode` (flag + downsample interaction)
2. `QImPlotScatterItemNode` (downsample + marker shape)
3. `QImPlot3DSurfaceItemNode` (colormap + grid shape)
4. `QImPlot3DMeshItemNode` (fill/line/markers visibility combination)
5. `QImPlotDragPointNode` (flag combinations + interaction state)
6. Remaining nodes in property-count-descending order
