# QIm项目概述

在进行开发之前，需先阅读`AGENTS.md`

## 项目概述

这个项目是将 `Dear ImGui`、`ImPlot`、`ImPlot3D` 等 ImGui 生态组件以 **保留模式（Retained Mode）** 封装到 Qt 框架中

目前正在进行`ImPlot`模块的封装，模块路径`src\core\plot`

在开发之前，需要先完整阅读`3rdparty\implot\implot.h`的源码，了解组件的用法，了解有哪些属性设置

## 构建指引

### 构建环境

- CMake 3.15+
- Visual Studio 2019（MSVC 14.29+）
- Qt 6.7+ (msvc2019\_64) 或 Qt 5.14+

### 构建步骤

本项目使用 CMake 构建，推荐使用 **Visual Studio 生成器**，因为它会自动初始化 MSVC 编译器环境。如果使用 Ninja 生成器，需要手动初始化 MSVC 环境变量。

#### 方法一：Visual Studio 生成器（推荐）

```powershell
# 在项目根目录下
cd build

# 配置（使用 Visual Studio 2019 生成器）
cmake .. -G "Visual Studio 16 2019" -A x64 -DCMAKE_PREFIX_PATH="D:/Qt/6.7.3/msvc2019_64"

# 构建（Debug 或 Release）
cmake --build . --config Debug
# 或
cmake --build . --config Release
```

#### 方法二：Ninja 生成器（需初始化 MSVC 环境）

使用 Ninja 时必须先初始化 MSVC 环境变量（`INCLUDE`、`LIB`、`PATH` 等），否则编译器会找不到标准库头文件。

在 PowerShell 中手动初始化 MSVC 环境**

```powershell
# 初始化 MSVC 环境（vcvarsall.bat 会设置 INCLUDE、LIB、PATH 等关键变量）
cmd /c '"D:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64 & set' | ForEach-Object {
    if ($_ -match '^([^=]+)=(.*)$') {
        [Environment]::SetEnvironmentVariable($matches[1], $matches[2], 'Process')
    }
}

# 然后使用 Ninja 构建
cd F:\src\QIm\build
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH="D:/Qt/6.7.3/msvc2019_64"
cmake --build .
```

### 注意事项

1. **生成器选择**：推荐使用 Visual Studio 生成器（自动处理 MSVC 环境）；使用 Ninja 需先初始化 MSVC 环境变量
2. **Qt路径**：根据实际 Qt 安装位置调整 `CMAKE_PREFIX_PATH`
3. **清理构建**：如需重新配置，删除 `build` 目录下的 `CMakeCache.txt` 和 `CMakeFiles` 目录：

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

QIM_DECLARE_PRIVATE(QImPlotNewItemNode)

class QImPlotNewItemNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlotNewItemNode)
public:
    PrivateData() = default;
    
    QImAbstractXYDataSeries* dataSeries = nullptr;
    QColor color;
    bool horizontal = false;
    // 更多私有成员...
};

// 构造/析构
QImPlotNewItemNode::QImPlotNewItemNode(QObject* parent)
    : QImPlotItemNode(parent)
    , d_ptr(new PrivateData)
{
}

// 属性访问器实现
QColor QImPlotNewItemNode::color() const
{
    QIM_DC(d);
    return d->color;
}

void QImPlotNewItemNode::setColor(const QColor& color)
{
    QIM_D(d);
    if (d->color != color) {
        d->color = color;
        emit colorChanged(color);
    }
}

// 核心渲染方法
bool QImPlotNewItemNode::beginDraw()
{
    QIM_D(d);
    if (!d->dataSeries) return false;
    
    // 设置样式
    if (d->color.isValid()) {
        ImPlot::SetNextLineStyle(ImVec4(d->color.redF(), d->color.greenF(), 
                                        d->color.blueF(), d->color.alphaF()));
    }
    
    // 调用ImPlot API
    ImPlot::PlotNewItem(labelConstData(), 
                         d->dataSeries->xData(), 
                         d->dataSeries->yData(),
                         d->dataSeries->count(),
                         newItemFlags());
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

## 子代理快速参考

### 开发新Plot Item时必读文件

1. **参考实现**: `src/core/plot/QImPlotBarsItemNode.h` (完整属性+Doxygen注释)
2. **简单参考**: `src/core/plot/QImPlotStairsItemNode.h` (简单属性)
3. **ImPlot API**: `3rdparty/implot/implot.h` (原始API签名)
4. **数据系列**: `src/core/plot/QImPlotDataSeries.h` (XY数据接口)
5. **测试模板**: `examples/qimfigure-test/functions/datapoints/BarsFunction.cpp`

### 关键约定

1. **属性命名**: 使用Qt风格（`setColor`/`color`/`colorChanged`）
2. **标志属性**: 使用肯定语义（`isHorizontal`而非`noHorizontal`）
3. **Doxygen注释**: 必须双语（`\if ENGLISH` / `\if CHINESE`）
4. **头文件注释**: public函数仅单行英文简要注释
5. **详细注释**: 在cpp文件中使用完整Doxygen双语块

