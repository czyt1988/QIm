# QIm项目概述

在进行开发之前，需先阅读`AGENTS.md`

## 项目概述

这个项目是将 `Dear ImGui`、`ImPlot`、`ImPlot3D` 等 ImGui 生态组件以 **保留模式（Retained Mode）** 封装到 Qt 框架中

目前正在进行`ImPlot`模块的封装，模块路径`src\core\plot`

在开发之前，需要先完整阅读`3rdparty\implot\implot.h`的源码，了解组件的用法，了解有哪些属性设置

## QIm对象树管理理念

QIm的设计哲学就是对象树，每个图表元素都是一个节点，节点之间通过父级-子级关系组织。在开发其他模块时你可以先阅读`docs\zh\render-node.md`

`ImPlot`一般使用标志位来控制属性，QIm提倡使用函数来设置，你可以参考`src\core\plot\QImPlotLineItemNode.h`类的实现

## PIMPL模式

QIm使用PIMPL模式，将实现细节封装在`private`成员中。PIMPL模式所需的宏位于`src\QImAPI.h`：

| 宏                            | 用途                                        |
| ---------------------------- | ----------------------------------------- |
| `QIM_DECLARE_PRIVATE(Class)` | 在类中声明PIMPL私有成员，生成`d_ptr`和内部类`PrivateData` |
| `QIM_DECLARE_PUBLIC(Class)`  | 在`PrivateData`中声明PIMPL公有成员，生成`q_ptr`      |
| `QIM_D(d)`                   | 在非const方法中获取`d_ptr`指针                     |
| `QIM_DC(d)`                  | 在const方法中获取`const d_ptr`指针                |
| `QIM_PIMPL_CONSTRUCT`        | 构造函数中初始化PIMPL快捷宏                          |

## 颜色属性类型别名

为了简化 Plot Item 颜色属性的声明，项目在 `QImPlotItemNode.h` 中提供了类型别名：

| 类型别名 | 定义 | 用途 |
|---------|------|------|
| `QImTrackedColor` | `QImTrackedValue<ImVec4, ImVecComparator<ImVec4>>` | 带脏标记跟踪的 ImVec4 颜色值 |
| `QImOptionalColor` | `std::optional<QImTrackedColor>` | 可选的跟踪颜色值，用于延迟初始化模式 |

### 使用示例

```cpp
// 在 PrivateData 中声明颜色属性
class QImPlotLineItemNode::PrivateData
{
    QImOptionalColor color;  ///< 颜色（延迟初始化：首次渲染时捕获ImPlot默认颜色）
};

// 在 beginDraw() 中使用
bool QImPlotLineItemNode::beginDraw()
{
    // 如果用户设置了颜色，应用到渲染
    if (d->color) {
        ImPlot::SetNextLineStyle(d->color->value(), d->lineWidth.value());
    }
    
    // ... 调用 ImPlot::PlotLine ...
    
    // 首次渲染且未设置颜色时，捕获 ImPlot 分配的默认颜色
    if (!d->color) {
        d->color = ImPlot::GetLastItemColor();
    }
}
```

### 设计说明

`QImOptionalColor` 使用 **延迟初始化模式**（Lazy Initialization）：
1. `std::nullopt`：用户未设置颜色，使用 ImPlot 默认颜色
2. 有值：用户已设置颜色，或已从 ImPlot 捕获默认颜色
3. 配合 `beginDraw()` 实现 Node 与 ImPlot 之间的状态同步

此设计解决了 Node 封装 ImPlot 属性时，因 ImPlot 内部交互导致属性不同步的问题。

## 已实现的Plot Items（全部完成）

| Item Node Class              | ImPlot API      | 描述        | 关键属性                                                 |
| ---------------------------- | --------------- | --------- | ---------------------------------------------------- |
| `QImPlotLineItemNode`        | PlotLine        | 2D线图      | color, segments, loop, shaded, skipNaN               |
| `QImPlotScatterItemNode`     | PlotScatter     | 散点图       | markerSize, markerShape, color, adaptiveSampling     |
| `QImPlotStairsItemNode`      | PlotStairs      | 阶梯图       | color, shaded                                        |
| `QImPlotShadedItemNode`      | PlotShaded      | 填充区域图     | color, referenceValue                                |
| `QImPlotBarsItemNode`        | PlotBars        | 柱状图       | color, barWidth, horizontal                          |
| `QImPlotBarGroupsItemNode`   | PlotBarGroups   | 分组/堆叠柱状图  | groupWidth, horizontal, stacked                      |
| `QImPlotErrorBarsItemNode`   | PlotErrorBars   | 误差棒       | color, horizontal                                    |
| `QImPlotStemsItemNode`       | PlotStems       | 茎叶图       | color, referenceValue, horizontal                    |
| `QImPlotInfLinesItemNode`    | PlotInfLines    | 无限线       | color, horizontal                                    |
| `QImPlotPieChartItemNode`    | PlotPieChart    | 饼图        | center, radius, normalize, exploding                 |
| `QImPlotHeatmapItemNode`     | PlotHeatmap     | 热力图       | rows, cols, scaleMin, scaleMax, bounds               |
| `QImPlotHistogramItemNode`   | PlotHistogram   | 直方图       | bins, barScale, cumulative, density                  |
| `QImPlotHistogram2DItemNode` | PlotHistogram2D | 二维直方图     | xBins, yBins, density                                |
| `QImPlotDigitalItemNode`     | PlotDigital     | 数字信号图     | color                                                |
| `QImPlotImageItemNode`       | PlotImage       | 图像渲染      | textureId, boundsMin, boundsMax, uv0, uv1, tintColor |
| `QImPlotTextItemNode`        | PlotText        | 文本标签      | text, position, pixelOffset, vertical                |
| `QImPlotDummyItemNode`       | PlotDummy       | 占位项（图例条目） | color                                                |
| `QImPlotValueTrackerNode`    | 自定义             | 值追踪器      | （用于鼠标悬停显示数值）                                         |

## 已实现的Plot Tools（全部完成）

| Tool Node Class         | ImPlot API | 描述       | 关键属性                                      |
| ----------------------- | ---------- | -------- | ----------------------------------------- |
| `QImPlotDragPointNode`  | DragPoint  | 可拖动点     | position, color, size, id                 |
| `QImPlotDragLineXNode`  | DragLineX  | 可拖动垂直线   | x, color, thickness, id                   |
| `QImPlotDragLineYNode`  | DragLineY  | 可拖动水平线   | y, color, thickness, id                   |
| `QImPlotDragRectNode`   | DragRect   | 可拖动/调整矩形 | rect(x1,y1,x2,y2), color, id              |
| `QImPlotAnnotationNode` | Annotation | 注释标注     | position, text, color, pixelOffset, clamp |
| `QImPlotTagXNode`       | TagX       | X轴标签     | value, color, text, round                 |
| `QImPlotTagYNode`       | TagY       | Y轴标签     | value, color, text, round                 |

## 创建新的Plot Item步骤

### 1. 头文件结构（参考QImPlotBarsItemNode.h）

```cpp
#ifndef QIMPLOTNEWITEMNODE_H
#define QIMPLOTNEWITEMNODE_H

#include <QColor>
#include "QImPlotItemNode.h"
#include "QImPlotDataSeries.h"  // 如需数据系列

namespace QIM
{
class QImAbstractXYDataSeries;  // 或自定义数据系列

/**
 * \if ENGLISH
 * @brief English description
 * \endif
 *
 * \if CHINESE
 * @brief 中文描述
 * \endif
 */
class QIM_CORE_API QImPlotNewItemNode : public QImPlotItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotNewItemNode)

    // Q_PROPERTY声明（使用Qt命名风格）
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(bool horizontal READ isHorizontal WRITE setHorizontal NOTIFY orientationChanged)
    // 更多属性...

public:
    enum { Type = InnerType + N };  // N为类型编号
    virtual int type() const override { return Type; }

    explicit QImPlotNewItemNode(QObject* parent = nullptr);
    ~QImPlotNewItemNode() override;

    // 数据设置方法
    void setData(QImAbstractXYDataSeries* series);
    template<typename ContainerX, typename ContainerY>
    QImAbstractXYDataSeries* setData(const ContainerX& x, const ContainerY& y);

    // 属性访问器
    QColor color() const;
    void setColor(const QColor& color);
    // 更多访问器...

    // ImPlot标志访问
    int newItemFlags() const;
    void setNewItemFlags(int flags);

Q_SIGNALS:
    void colorChanged(const QColor& color);
    void orientationChanged(bool horizontal);
    void dataChanged();

protected:
    virtual bool beginDraw() override;  // 核心渲染实现
};

} // namespace QIM
#endif
```

### 2. 实现文件结构（使用PIMPL模式）

```cpp
#include "QImPlotNewItemNode.h"
#include "implot.h"
#include "implot_internal.h"

QIM_DECLARE_PRIVATE(QImPlotNewItemNode)

class QImPlotNewItemNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlotNewItemNode)
public:
    PrivateData() = default;
    
    QImAbstractXYDataSeries* dataSeries = nullptr;
    QImOptionalColor color;  ///< 使用类型别名声明颜色属性
    bool horizontal = false;
    // 更多私有成员...
};

// 构造/析构
QImPlotNewItemNode::QImPlotNewItemNode(QObject* parent)
    : QImPlotItemNode(parent)
    , QIM_PIMPL_CONSTRUCT
{
}

// 属性访问器实现
QColor QImPlotNewItemNode::color() const
{
    return (d_ptr->color.has_value()) ? toQColor(d_ptr->color->value()) : QColor();
}

void QImPlotNewItemNode::setColor(const QColor& color)
{
    d_ptr->color = toImVec4(color);
    emit colorChanged(color);
}

// 核心渲染方法
bool QImPlotNewItemNode::beginDraw()
{
    QIM_D(d);
    if (!d->dataSeries) return false;
    
    // 设置样式：如果用户设置了颜色，应用到渲染
    if (d->color) {
        ImPlot::SetNextLineStyle(d->color->value());
    }
    
    // 调用ImPlot API
    ImPlot::PlotNewItem(labelConstData(), 
                         d->dataSeries->xData(), 
                         d->dataSeries->yData(),
                         d->dataSeries->count(),
                         newItemFlags());
    
    // 首次渲染且未设置颜色时，捕获 ImPlot 分配的默认颜色
    if (!d->color) {
        d->color = ImPlot::GetLastItemColor();
    }
    
    return true;
}
```

### 3. 数据系列（如需自定义）

参考`QImPlotDataSeries.h`和`QImPlotErrorDataSeries.h`

### 4. 添加测试函数

详细步骤见`examples/HowToAddNewFunction.md`

## 测试

### 绘图功能测试

本项目绘图的功能实现测试代码位于`examples/qimfigure-test`目录下，具体添加方法请阅读`examples/HowToAddNewFunction.md`

### 测试函数目录结构

```
examples/qimfigure-test/functions/
├── line/           # 线图测试（Line10K, Line1M）
├── datapoints/     # 数据点图测试（Scatter, Stairs, Bars, BarGroups, Histogram, Histogram2D）
├── shaded/         # 填充图测试（Shaded）
├── error/          # 误差棒测试（ErrorBars）
├── other/          # 其他测试（Stems, InfLines, PieChart, Heatmap, Digital, Image, Text, Dummy）
├── TestFunction.h/cpp      # 测试基类
└── TestFunctionManager.cpp # 注册所有测试
```

### 子代理快速参考

### 开发新Plot Item时必读文件

1. **参考实现**: `src/core/plot/QImPlotBarsItemNode.h` (完整属性+Doxygen注释)
2. **简单参考**: `src/core/plot/QImPlotStairsItemNode.h` (简单属性)
3. **颜色属性类型别名**: `src/core/plot/QImPlotItemNode.h` (QImOptionalColor 定义)
4. **ImPlot API**: `3rdparty/implot/implot.h` (原始API签名)
5. **数据系列**: `src/core/plot/QImPlotDataSeries.h` (XY数据接口)
6. **测试模板**: `examples/qimfigure-test/functions/datapoints/BarsFunction.cpp`

### 关键约定

1. **属性命名**: 使用Qt风格（`setColor`/`color`/`colorChanged`）
2. **标志属性**: 使用肯定语义（`isHorizontal`而非`noHorizontal`）
3. **颜色属性**: 使用 `QImOptionalColor` 类型别名，遵循延迟初始化模式
4. **Doxygen注释**: 必须双语（`\if ENGLISH` / `\if CHINESE`）
5. **头文件注释**: public函数仅单行英文简要注释
6. **详细注释**: 在cpp文件中使用完整Doxygen双语块

### 涉及代码开发必须阅读文档

1. **代码风格与注释规范**: `docs\zh\dev\coding-standards.md`
2. **PIMPL开发规范**: `docs\zh\dev\pimpl-dev-guide.md`
3. **Qt集成规范**: `docs\zh\dev\qt-integration.md`

涉及新节点开发，你还需要阅读：

1. **新节点开发指南**: `docs\zh\dev\new-node-guide.md`
2. **渲染性能规范**: `docs\zh\dev\render-guidelines.md`
3. **枚举语义转换规范**: `docs\zh\dev\flag-mapping.md`

这些文档你可以通过**开发规范索引**了解详情: `docs\zh\dev\index.md`

你如果要编译和构建，必须阅读`build.md`