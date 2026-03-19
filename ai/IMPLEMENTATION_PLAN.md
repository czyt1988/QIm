# QIm ImPlot 图表类型封装实施方案

我正在开发一个名为 **QIm** 的 Qt-ImGui 集成绘图库，该项目将 `Dear ImGui`、`ImPlot`、`ImPlot3D` 等 ImGui 生态组件以 **保留模式（Retained Mode）** 封装到 Qt 框架中，使 Qt 开发者能够使用熟悉的编程范式（信号槽、属性系统、对象树）构建高性能实时数据可视化应用。

- 项目仓库：https://github.com/czyt1988/QIm
- 源码文件位置：`src/`
- 2D绘图核心代码位置：`src/core/plot/`（ImPlot 封装模块）
- 当前进度：已完成基础框架搭建，`ImGui` 绘制封装完成，`ImPlot` 中已完成 **曲线（Line）、散点图（Scatter）、阶梯图（Stairs）、柱状图（Bars）、填充图（Shaded）、误差棒图（ErrorBars）、茎叶图（Stems）、无限线（InfLines）** 的封装

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

## 补充说明

**重要：**每次任务应该修改 `IMPLEMENTATION_PLAN.md` 文件，添加新的图表类型或修改已有的图表类型的实现计划。

- 如遇设计决策不确定处，请列出选项并说明利弊，由我最终确认
- 可参考 ImPlot 官方 Demo 示例理解各图表类型的参数与行为
- 方案需兼顾 **易用性**（对 Qt 开发者友好）与 **性能**（保留 ImGui 即时模式优势）