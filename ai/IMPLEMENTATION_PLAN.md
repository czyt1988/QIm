# QIm ImPlot 图表类型封装实施方案

我正在开发一个名为 **QIm** 的 Qt-ImGui 集成绘图库，该项目将 `Dear ImGui`、`ImPlot`、`ImPlot3D` 等 ImGui 生态组件以 **保留模式（Retained Mode）** 封装到 Qt 框架中，使 Qt 开发者能够使用熟悉的编程范式（信号槽、属性系统、对象树）构建高性能实时数据可视化应用。

- 项目仓库：https://github.com/czyt1988/QIm
- 源码文件位置：`src/`
- 2D绘图核心代码位置：`src/core/plot/`（ImPlot 封装模块）
- 当前进度：已完成基础框架搭建，`ImGui` 绘制封装完成，`ImPlot` 中已完成 **曲线（Line）、散点图（Scatter）、阶梯图（Stairs）、柱状图（Bars）、填充图（Shaded）、误差棒图（ErrorBars）、茎叶图（Stems）** 的封装

## 📋 项目概述

本方案旨在完成 **QIm** 项目中 **ImPlot 剩余图表类型** 的 Qt 风格封装。QIm 是一个将 ImGui 生态（Dear ImGui、ImPlot、ImPlot3D）以保留模式（Retained Mode）封装到 Qt 框架中的开源库，使 Qt 开发者能够使用熟悉的编程范式（信号槽、属性系统、对象树）构建高性能实时数据可视化应用。

## 当前状态分析

### 已完成功能
- ✅ **基础框架搭建**：QImAbstractNode、QImPlotItemNode 基类体系
- ✅ **ImGui 绘制封装**：Qt 与 ImGui 的桥接层
- ✅ **曲线绘制封装**：QImPlotLineItemNode（ImPlot::PlotLine）
- ✅ **散点图绘制封装**：QImPlotScatterItemNode（ImPlot::PlotScatter）
- ✅ **阶梯图绘制封装**：QImPlotStairsItemNode（ImPlot::PlotStairs）
- ✅ **柱状图绘制封装**：QImPlotBarsItemNode（ImPlot::PlotBars）
- ✅ **填充图绘制封装**：QImPlotShadedItemNode（ImPlot::PlotShaded）
- ✅ **误差棒图绘制封装**：QImPlotErrorBarsItemNode（ImPlot::PlotErrorBars）
- ✅ **茎叶图绘制封装**：QImPlotStemsItemNode（ImPlot::PlotStems）
- ✅ **数据系列抽象**：QImAbstractXYDataSeries 数据接口
- ✅ **降采样支持**：LTTB 和 MinMaxLTTB 降采样算法

### 待封装图表类型
基于 ImPlot v0.18 WIP 官方文档和 Demo 分析，需要封装的图表类型如下：

## 待封装图表类型清单及优先级排序

### 第一优先级（核心图表类型）
| 图表类型 | ImPlot API | 用途说明 | 复杂度 | 状态 |
|---------|------------|----------|--------|------|
| **散点图** | `PlotScatter()` | 离散数据点可视化 | 低 | ✅ 已完成 |
| **阶梯图** | `PlotStairs()` | 阶梯状数据可视化 | 低 | ✅ 已完成 |
| **柱状图** | `PlotBars()` | 分类数据比较 | 中 | ✅ 已完成 |
| **填充图** | `PlotShaded()` | 区域填充可视化 | 中 | ✅ 已完成 |

### 第二优先级（高级图表类型）
| 图表类型 | ImPlot API | 用途说明 | 复杂度 |
|---------|------------|----------|--------|
| **误差棒图** | `PlotErrorBars()` | 数据误差范围可视化 | 中 | ✅ 已完成 |
| **茎叶图** | `PlotStems()` | 离散数据与基线连接 | 低 | ✅ 已完成 |
| **无限线** | `PlotInfLines()` | 垂直/水平参考线 | 低 | ✅ 已完成 |
| **饼图** | `PlotPieChart()` | 比例数据可视化 | 高 |

### 第三优先级（专业图表类型）
| 图表类型 | ImPlot API | 用途说明 | 复杂度 |
|---------|------------|----------|--------|
| **热力图** | `PlotHeatmap()` | 矩阵数据可视化 | 高 |
| **直方图** | `PlotHistogram()` | 数据分布可视化 | 高 |
| **2D直方图** | `PlotHistogram2D()` | 二维数据分布 | 高 |
| **数字图** | `PlotDigital()` | 数字信号可视化 | 中 |
| **图像图** | `PlotImage()` | 图像显示 | 中 |
| **文本标注** | `PlotText()` | 图表文本标注 | 低 |
| **虚拟项** | `PlotDummy()` | 占位/分组项 | 低 |

### 特殊图表类型（需要自定义实现）
| 图表类型 | 说明 | 复杂度 |
|---------|------|--------|
| **分组柱状图** | `PlotBarGroups()` | 多组数据比较 | 高 |
| **堆叠柱状图** | 基于 `PlotBarGroups()` | 累计数据比较 | 高 |
| **蜡烛图** | 自定义实现（见 Demo） | 金融数据可视化 | 高 |

## 设计规范

严格保持与现有代码一致（命名规范、缩进、头文件组织等）

遵循 Qt 开发最佳实践（使用 `Q_PROPERTY`、`Q_SIGNALS`、`Q_SLOT` 等宏）

**封装设计原则：**
- **严格遵循现有代码风格**（参考 `QImPlotLineItemNode`）
- **Qt 范式优先**：
   - 使用 `Q_PROPERTY` 暴露属性
   - 使用 `Q_SIGNALS` 定义信号
   - 使用 `Q_SLOTS` 定义槽函数
   - 使用 PIMPL 模式（参考现有实现）
- **组件化设计**：每个图表类型封装为独立 QObject 派生类
- **低耦合**：ImGui/ImPlot 原生 API 调用集中在底层适配层
- **可扩展**：预留接口支持未来添加自定义图表或主题
- **必须使用 Doxygen 格式的中英文双语注释**，具体要求见[注释与文档规范](#注释与文档规范)
- **文件命名规范**：
   - 头文件：`QImPlot[类型]ItemNode.h`
   - 源文件：`QImPlot[类型]ItemNode.cpp`

### 1. 继承体系设计

```
QImAbstractNode (基类)
    └── QImPlotItemNode (绘图项基类)
        ├── QImPlotLineItemNode (已实现)
        ├── QImPlotScatterItemNode (已实现)
        ├── QImPlotStairsItemNode (已实现)
        ├── QImPlotBarsItemNode (已实现)
        ├── QImPlotShadedItemNode (已实现)
        ├── QImPlotErrorBarsItemNode (已实现)
        ├── QImPlotStemsItemNode (已实现)
        ├── QImPlotInfLinesItemNode (已实现)
        ├── QImPlotPieChartItemNode
        ├── QImPlotHeatmapItemNode
        ├── QImPlotHistogramItemNode
        ├── QImPlotDigitalItemNode
        ├── QImPlotImageItemNode
        ├── QImPlotTextItemNode
        └── QImPlotDummyItemNode
```

### 2. 数据系列扩展

现有数据系列体系：
```
QImAbstractPlotDataSeries (抽象基类)
    └── QImAbstractXYDataSeries (XY数据接口)
        └── QImVectorXYDataSeries (模板实现)
```

需要扩展的数据系列类型：
- **矩阵数据系列**：`QImAbstractMatrixDataSeries`（用于热力图）
- **分类数据系列**：`QImAbstractCategoryDataSeries`（用于柱状图、饼图）
- **误差数据系列**：`QImAbstractErrorDataSeries`（用于误差棒图）

### 3. 类模板设计规范

#### 3.1 头文件模板
```cpp
#ifndef QIMPLOT[类型]ITEMNODE_H
#define QIMPLOT[类型]ITEMNODE_H

#include "QImPlotItemNode.h"
#include "QImPlotDataSeries.h"

namespace QIM
{

class QIM_CORE_API QImPlot[类型]ItemNode : public QImPlotItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlot[类型]ItemNode)
    
    // 属性声明
    Q_PROPERTY(Type propertyName READ getter WRITE setter NOTIFY signal)
    
public:
    enum
    {
        Type = InnerType + [序号]
    };
    
    virtual int type() const override
    {
        return Type;
    }
    
    QImPlot[类型]ItemNode(QObject* parent = nullptr);
    ~QImPlot[类型]ItemNode();
    
    // 数据设置接口
    void setData(QImAbstract[数据]DataSeries* series);
    template<typename Container>
    QImAbstract[数据]DataSeries* setData(const Container& data);
    
    // 样式属性访问器
    Type propertyName() const;
    void setPropertyName(Type value);
    
    // 标志属性访问器（采用肯定语义）
    bool isFlagName() const;
    void setFlagName(bool on);
    
Q_SIGNALS:
    void propertyChanged();
    void flagChanged();
    
protected:
    virtual bool beginDraw() override;
};

} // namespace QIM

#endif // QIMPLOT[类型]ITEMNODE_H
```

#### 3.2 实现文件模板
```cpp
#include "QImPlot[类型]ItemNode.h"
#include "implot.h"
#include "implot_internal.h"
#include "QtImGuiUtils.h"

namespace QIM
{

class QImPlot[类型]ItemNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlot[类型]ItemNode)
public:
    PrivateData(QImPlot[类型]ItemNode* p);
    
    std::unique_ptr<QImAbstract[数据]DataSeries> data;
    ImPlot[类型]Flags flags { ImPlot[类型]Flags_None };
    // 样式跟踪值
    QImTrackedValue<ImVec4> color;
    QImTrackedValue<float> size;
};

QImPlot[类型]ItemNode::PrivateData::PrivateData(QImPlot[类型]ItemNode* p) : q_ptr(p)
{
}

// 构造函数
QImPlot[类型]ItemNode::QImPlot[类型]ItemNode(QObject* parent) 
    : QImPlotItemNode(parent), QIM_PIMPL_CONSTRUCT
{
}

// 析构函数
QImPlot[类型]ItemNode::~QImPlot[类型]ItemNode()
{
}

// 数据设置
void QImPlot[类型]ItemNode::setData(QImAbstract[数据]DataSeries* series)
{
    QIM_D(d);
    d->data.reset(series);
}

// 绘制实现
bool QImPlot[类型]ItemNode::beginDraw()
{
    QIM_D(d);
    if (!d->data || d->data->size() == 0) {
        return false;
    }
    
    // 应用样式
    if (d->color.is_dirty() || d->size.is_dirty()) {
        ImPlot::SetNext[类型]Style(d->color.value(), d->size.value());
    }
    
    // 调用 ImPlot API
    // ... 具体绘制逻辑
    
    return true;
}

// 属性访问器实现
// ...

} // namespace QIM
```

## Qt 集成方案

### 1. 信号槽设计

充分发挥Qt的信号槽机制，将图表的关键交互事件暴露为信号，但要考虑到性能问题，一些频繁变更的属性避免作为信号触发。

### 2. 属性暴露方式

为了贴合Qt框架，类的属性使用`Q_PROPERTY`暴露

如：
```cpp
// 数据属性
Q_PROPERTY(QImAbstractDataSeries* data READ data WRITE setData NOTIFY dataChanged)

// 样式属性
Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
Q_PROPERTY(float size READ size WRITE setSize NOTIFY sizeChanged)
Q_PROPERTY(float opacity READ opacity WRITE setOpacity NOTIFY opacityChanged)

// 可见性属性
Q_PROPERTY(bool visible READ isVisible WRITE setVisible NOTIFY visibilityChanged)
```

## 对象树管理

QIm的设计哲学就是对象树，每个图表元素都是一个节点，节点之间通过父级-子级关系组织。最终使用效果如下：
```cpp
// 创建绘图节点树
auto plot = new QImPlotNode(parent);
plot->setTitle("示例图表");

// 添加坐标轴
plot->setupAxisX("X轴");
plot->setupAxisY("Y轴");

// 添加数据系列
auto line = new QImPlotLineItemNode(plot);
line->setLabel("温度曲线");
line->setData(xData, yData);
line->setColor(Qt::red);

auto bars = new QImPlotBarsItemNode(plot);
bars->setLabel("月度销量");
bars->setData(categories, values);
bars->setBarWidth(0.6);

// 自动管理生命周期
// 父节点删除时自动删除所有子节点
```

组合图表示例：
```cpp
// 误差棒+散点图组合
auto scatter = new QImPlotScatterItemNode(plot);//默认父节点为plot
auto errors = new QImPlotErrorBarsItemNode(plot);

// 共享数据系列
auto dataSeries = new QImVectorXYDataSeries(xData, yData);
auto errorSeries = new QImVectorErrorDataSeries(yErrors);

scatter->setData(dataSeries);
errors->setData(dataSeries, errorSeries);

// 同步样式
scatter->setColor(Qt::blue);
errors->setColor(Qt::blue);
```

## 注释与文档规范

### 1️. 代码风格
- 严格保持与现有代码一致（命名规范、缩进、头文件组织等）
- 遵循 Qt 开发最佳实践（使用 `Q_PROPERTY`、`Q_SIGNALS`、`Q_SLOT` 等宏）

### 2️. 注释规范（强制）

#### 2.1 源文件（.cpp）注释规范

所有新增代码必须使用 **Doxygen 格式**，并区分中英文：
```cpp
/**
 * \if ENGLISH
 * @brief [English brief description]
 * @param[in] param_name [English parameter description]
 * @return [English return value description]
 * @details [English detailed explanation]
 * \endif
 *
 * \if CHINESE
 * @brief [中文简要说明]
 * @param[in] param_name [中文参数描述]
 * @return [中文返回值描述]
 * @details [中文详细说明]
 * \endif
 */
```
#### 2.2 头文件（.h）注释规范

- 头文件中的 `public` 函数声明旁，仅添加**单行英文简要注释**（使用 `//`  或简洁的 `/** */`）。
- **不要**在头文件中写入详细的双语 Doxygen 块（类的doxygen注释除外、信号的doxygen注释除外），详细内容应保留在对应的 `.cpp` 文件中，以保持头文件整洁。
- 示例：
```cpp
// 类的注释规范建见下一节
class MyClass {
public:
    // Constructor for MyClass (English only)
    MyClass(); 
};
```
- 但有些特例，例如qt的信号（头文件中Q_SIGNALS关键字下面的函数），它没有在cpp中的定义，这些函数的doxygen注释需要在头文件中按上面中英文要求添加，你需要把信号的doxygen注释转换为中英双语。
- 另外类的doxygen注释也需要在头文件中按上面中英文要求添加。

#### 2.3 类的doxygen注释规范

- 类的doxygen注释需要在头文件中按上面中英文要求添加。
- 示例：
```cpp
/**
 * \if ENGLISH
 * @brief [English description]
 * \endif
 *
 * \if CHINESE
 * @brief [中文说明]
 * \endif
 */
class MyClass {
public:
    // Constructor for MyClass (English only)
    MyClass(); 
};
```
- 对于功能性较强的类，类的注释中应该加入使用示例，以便使用者了解如何使用

## 示例验证

- 新功能开发完后，需在 `examples/qimfigure-test` 中添加对应的使用示例，验证功能正确性
    - 功能应在`examples/qimfigure-test`的`void drawPlot2D()`函数中增加对应示例
    - 散点图示例：已在`drawPlot2D()`中添加随机数据散点图，包含标记大小、形状、颜色等自定义设置

## Git 提交

在完成当前任务后，需提交所有更改到 Git 仓库。
创建有意义的提交信息保证下次任务能清楚了解这次任务的实现情况
提交信息最好包含以下信息：
- 任务类型（例如：实现、修复、文档更新）
- 图表类型（如果有）
- 实现内容的简要描述
- 相关文件列表
- 关联到计划书（如果适用）

## 补充说明

**重要：**每次任务应该修改 `IMPLEMENTATION_PLAN.md` 文件，添加新的图表类型或修改已有的图表类型的实现计划。

- 如遇设计决策不确定处，请列出选项并说明利弊，由我最终确认
- 可参考 ImPlot 官方 Demo 示例理解各图表类型的参数与行为
- 方案需兼顾 **易用性**（对 Qt 开发者友好）与 **性能**（保留 ImGui 即时模式优势）
