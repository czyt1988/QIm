# QIm项目指南

我是一名出色的C++/Qt架构师，有非常丰富的C++编程经验，我正在开发一个名为 **QIm** 的 Qt-ImGui 集成绘图库，该项目将 `Dear ImGui`、`ImPlot`、`ImPlot3D` 等 ImGui 生态组件以 **保留模式（Retained Mode）** 封装到 Qt 框架中，使 Qt 开发者能够使用熟悉的编程范式（信号槽、属性系统、对象树）构建高性能实时数据可视化应用。

## 项目概述

- 项目仓库：<https://github.com/czyt1988/QIm>
- 源码文件位置：`src/`
- 2D绘图核心代码位置：`src/core/plot/`（ImPlot 封装模块）

## 开发环境

### 编译依赖

- CMake 3.16+
- C++17 兼容编译器
- Qt 5.14+ 或 Qt 6

## 构建指引

具体构建过程可参阅：[build.md文件](./build.md)

## Qt 集成方案

为了适配Qt的使用习惯，在属性命名上，尽量使用Qt的命名方式，例如：`setColor`、`color`、`setSize`、`size`、`setOpacity`、`opacity`、`setVisible`、`visible`

### 1. 信号槽设计

充分发挥 Qt 的信号槽机制，工具类使用信号和槽进行事件通讯。

### 2. 属性暴露方式

为了贴合 Qt 框架，类的属性使用 `Q_PROPERTY` 暴露：

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

### 3. Qt信号槽相关宏都使用大写

禁止使用`slot`,`signals`,`emit`等小写宏
应该使用`Q_SLOTS`,`Q_SIGNALS`,`Q_EMIT`等大写宏

## 注释与文档规范

### 1️. 代码风格

- 严格保持与现有代码一致（命名规范、缩进、头文件组织等）
- 遵循 Qt 开发最佳实践（使用 `Q_PROPERTY`、`Q_SIGNALS`、`Q_SLOT` 等宏，禁止使用 `slot`、`signal` 等小写命名的宏）

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

**注意:**原则上详细函数注释应该写在对应的 `.cpp` 文件中，而不是头文件中。

#### 2.2 头文件（.h）注释规范

- 头文件中的 `public` 函数声明旁，仅添加**单行英文简要注释**（使用 `//` 或简洁的 `/** */`）。
- **不要**在头文件中写入详细的双语 Doxygen 块（类的 doxygen 注释除外、信号的 doxygen 注释除外），详细内容应保留在对应的 `.cpp` 文件中，以保持头文件整洁。
- 示例：

```cpp
// 类的注释规范建见下一节
class MyClass {
public:
    // Constructor for MyClass (English only)
    MyClass(); 
};
```

- 但有些特例，例如 Qt 的信号（头文件中 Q_SIGNALS 关键字下面的函数），它没有在 cpp 中的定义，这些函数的 doxygen 注释需要在头文件中按上面中英文要求添加，你需要把信号的 doxygen 注释转换为中英双语。
- 另外类的 doxygen 注释也需要在头文件中按上面中英文要求添加。

#### 2.3 类的 doxygen 注释规范

- 类的 doxygen 注释需要在头文件中按上面中英文要求添加。
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

## QIm对象树管理理念

QIm的设计哲学就是对象树，每个图表元素都是一个节点，节点之间通过父级-子级关系组织。具体可以通过`docs\zh\render-node.md`来了解

## 编码注意事项

### PIMPL模式

QIm使用PIMPL模式，将实现细节封装在`private`成员中，对外提供接口在`public`成员中。QIm的PIMPL模式所需的宏位于`src\QImAPI.h`，主要涉及如下宏：

- `QIM_DECLARE_PRIVATE`: 在类中声明PIMPL的私有成员，它将会生成`private`成员变量`d_ptr`，并定义一个内部类`PrivateData`
- `QIM_DECLARE_PUBLIC`: 在内部类`PrivateData`中声明PIMPL的公有成员，它将会生成`public`成员变量`q_ptr`，指向属主
- 通过`QIM_DECLARE_PRIVATE`和`QIM_DECLARE_PUBLIC`宏，QIm类就可以使用PIMPL模式，并互相声明为友元，可以互相访问私有变量
- `QIM_PIMPL_CONSTRUCT`: 在构造函数中初始化PIMPL的私有成员变量的快捷操作宏
- `QIM_D`和`QIM_DC`: 分别用于获取`d_ptr`指针：

    ```cpp
    void MyClass::foo1() {
        QIM_D(d);// 扩展为PrivateData* d = d_func()
        d->xx();
    }

    void MyClass::foo2() const{
        QIM_DC(d);// 扩展为const PrivateData* d = d_func()
        d->xxc();
    }
    ```

### 节点渲染函数尽量不做过多操作

ImGui的节点是实时渲染，在渲染函数`beginDraw`中，你不应该进行过多的复杂逻辑，所有逻辑应该在属性设置过程中处理完。

#### 字符串存储规范（重要）

**原则：节点只存储UTF8格式数据，不存储QString**

属性接口接受QString参数（符合Qt习惯），但ImGui接口只接受`char*`，节点内部变量应该**只存储`QByteArray`（UTF8格式）**，避免在`beginDraw`函数中进行`toUtf8()`转换，也避免同时存储QString和QByteArray造成内存浪费。

**正确模式：**

```cpp
// PrivateData - 只存储QByteArray
class QImPlotAnnotationNode::PrivateData
{
    QByteArray textUtf8;  ///< 文本内容(UTF8格式，直接供ImGui使用)
    // 不要存储 QString text;
};

// getter - 从UTF8转换回QString
QString QImPlotAnnotationNode::text() const
{
    return QString::fromUtf8(d_ptr->textUtf8);
}

// setter - 转换后只存储UTF8
void QImPlotAnnotationNode::setText(const QString& text)
{
    QIM_D(d);
    QByteArray utf8 = text.toUtf8();
    if (d->textUtf8 != utf8) {
        d->textUtf8 = utf8;
        emit textChanged(text);
    }
}

// beginDraw - 直接使用UTF8数据，无需转换
bool QImPlotAnnotationNode::beginDraw()
{
    QIM_D(d);
    if (d->textUtf8.isEmpty()) {
        // 无文本的情况
    } else {
        // 直接使用UTF8数据
        ImPlot::Annotation(..., "%s", d->textUtf8.constData());
    }
}
```

**错误模式（禁止）：**

```cpp
// 错误：存储QString和QByteArray两份数据
QString text;          // 多余存储
QByteArray textUtf8;   // 冗余

// 错误：在beginDraw中进行转换
bool beginDraw() {
    QByteArray utf8 = d->text.toUtf8();  // 性能损失！
    ImPlot::Annotation(..., "%s", utf8.constData());
}
```

#### 其他性能注意事项

- 所有数据转换、计算都应该在setter中完成
- `beginDraw`中只做最简单的数据传递给ImGui/ImPlot API
- 避免在`beginDraw`中进行条件判断后的数据准备
- 使用`isEmpty()`检查QByteArray，而不是QString

### ImPlot的枚举语义转换

#### 设计原则与动机

QIm的核心目标是让Qt开发者无需了解ImPlot/ImGui的API和枚举即可使用。因此：

1. **头文件不暴露ImPlot/ImGui类型**：用户看到的只有Qt风格的属性和方法，不应出现`ImPlotFlags`、`ImAxis`等ImPlot原生类型
2. **枚举转换为Qt属性**：ImPlot/ImGui通过位标志（bit flags）控制功能，QIm将这些标志拆解为独立的`Q_PROPERTY`布尔属性，每个属性对应一个功能开关
3. **保留原始标志访问接口**：同时提供一个`imPlotFlags()`/`setImPlotFlags()`方法，供高级用户直接操作底层标志位，但常规用户无需关注

#### 否定→肯定语义转换

ImPlot/ImGui的枚举大量使用**否定语义**（`NoXxx`），例如`ImPlotFlags_NoTitle`表示"禁用标题"。这在Qt属性系统中不自然——Qt开发者期望用`setTitleEnabled(true)`来启用标题，而非`setNoTitle(false)`来"不禁用标题"。

**转换规则：**

| ImPlot 否定语义枚举 | QIm 肯定语义属性 | 逻辑关系 |
| --- | --- | --- |
| `ImPlotFlags_NoTitle` | `titleEnabled` | `enabled = (flags & NoTitle) == 0` |
| `ImPlotFlags_NoLegend` | `legendEnabled` | `enabled = (flags & NoLegend) == 0` |
| `ImPlotFlags_NoMouseText` | `mouseTextEnabled` | `enabled = (flags & NoMouseText) == 0` |
| `ImPlotFlags_NoInputs` | `inputsEnabled` | `enabled = (flags & NoInputs) == 0` |
| `ImPlotFlags_NoMenus` | `menusEnabled` | `enabled = (flags & NoMenus) == 0` |
| `ImPlotFlags_NoBoxSelect` | `boxSelectEnabled` | `enabled = (flags & NoBoxSelect) == 0` |
| `ImPlotFlags_NoFrame` | `frameEnabled` | `enabled = (flags & NoFrame) == 0` |

对于**肯定语义**的ImPlot枚举，直接映射即可：

| ImPlot 肯定语义枚举 | QIm 肯定语义属性 | 逻辑关系 |
| --- | --- | --- |
| `ImPlotFlags_Equal` | `equal` | `on = (flags & Equal) != 0` |
| `ImPlotFlags_Crosshairs` | `crosshairs` | `on = (flags & Crosshairs) != 0` |

对于**组合标志**（由多个否定标志组合而成），也转换为肯定语义：

| ImPlot 组合枚举 | QIm 肯定语义属性 | 逻辑关系 |
| --- | --- | --- |
| `ImPlotFlags_CanvasOnly` (= NoTitle\|NoLegend\|NoMenus\|NoBoxSelect\|NoMouseText) | `canvasEnabled` | `enabled = (flags & CanvasOnly) == 0` |

#### Q_PROPERTY封装模式

每个标志对应一个Qt属性，使用`Q_PROPERTY`暴露：

```cpp
// 否定语义转换后的属性（大多数情况）
Q_PROPERTY(bool titleEnabled READ isTitleEnabled WRITE setTitleEnabled NOTIFY plotFlagChanged)
Q_PROPERTY(bool legendEnabled READ isLegendEnabled WRITE setLegendEnabled NOTIFY plotFlagChanged)
Q_PROPERTY(bool menusEnabled READ isMenusEnabled WRITE setMenusEnabled NOTIFY plotFlagChanged)

// 肯定语义直接映射的属性
Q_PROPERTY(bool equal READ isEqual WRITE setEqual NOTIFY plotFlagChanged)
Q_PROPERTY(bool crosshairs READ isCrosshairs WRITE setCrosshairs NOTIFY plotFlagChanged)
```

命名规范：

- **getter**: `isXxxEnabled()` 或 `isXxx()`（肯定语义）
- **setter**: `setXxxEnabled(bool enabled)` 或 `setXxx(bool on)`（肯定语义）
- **signal**: 同一节点多个标志属性可共享一个信号（如`plotFlagChanged()`），因为标志位是统一存储的，任何标志变化都影响同一个`ImPlotFlags`值

#### 实现宏

QIm提供了两个辅助宏来简化标志属性的getter/setter实现，定义在`src/core/plot/QImPlot.h`中：

**1. `QIMPLOT_FLAG_ACCESSOR` — 肯定语义标志（直接映射）**

用于ImPlot原生就是肯定语义的标志（如`ImPlotFlags_Equal`、`ImPlotFlags_Crosshairs`）：

```cpp
// 宏用法：QIMPLOT_FLAG_ACCESSOR(ClassName, FlagName, FlagEnum, emitFunName)
// 生成 isFlagName() 和 setFlagName(bool on)
//
// getter逻辑: return (flags & FlagEnum) != 0
// setter逻辑: on ? flags |= FlagEnum : flags &= ~FlagEnum

QIMPLOT_FLAG_ACCESSOR(QImPlotNode, Equal, ImPlotFlags_Equal, plotFlagChanged)
QIMPLOT_FLAG_ACCESSOR(QImPlotNode, Crosshairs, ImPlotFlags_Crosshairs, plotFlagChanged)
```

**2. `QIMPLOT_FLAG_ENABLED_ACCESSOR` — 否定→肯定语义转换（反转映射）**

用于ImPlot否定语义标志（如`ImPlotFlags_NoTitle`、`ImPlotFlags_NoMenus`），转换为Qt肯定语义属性：

```cpp
// 宏用法：QIMPLOT_FLAG_ENABLED_ACCESSOR(ClassName, PropName, FlagEnum, emitFunName)
// 生成 isPropName() 和 setPropName(bool enabled)
//
// getter逻辑: return (flags & FlagEnum) == 0   ← 关键：反转判断
// setter逻辑: enabled ? flags &= ~FlagEnum : flags |= FlagEnum  ← 关键：反转设置

QIMPLOT_FLAG_ENABLED_ACCESSOR(QImPlotNode, TitleEnabled, ImPlotFlags_NoTitle, plotFlagChanged)
QIMPLOT_FLAG_ENABLED_ACCESSOR(QImPlotNode, LegendEnabled, ImPlotFlags_NoLegend, plotFlagChanged)
QIMPLOT_FLAG_ENABLED_ACCESSOR(QImPlotNode, MenusEnabled, ImPlotFlags_NoMenus, plotFlagChanged)
```

**3. 组合标志的特殊处理**

组合标志（如`CanvasOnly`）无法使用上述宏，需要手动实现getter/setter，因为它们的setter需要同时设置/清除多个子标志：

```cpp
// getter — 仍然是否定→肯定反转
bool QImPlotNode::isCanvasEnabled() const
{
    QIM_DC(d);
    return (d->plotFlags & ImPlotFlags_CanvasOnly) == 0;
}

// setter — enabled时清除所有子标志，disabled时设置所有子标志
void QImPlotNode::setCanvasEnabled(bool enabled)
{
    QIM_D(d);
    const ImPlotFlags oldFlags = d->plotFlags;
    if (enabled) {
        d->plotFlags &= ~ImPlotFlags_CanvasOnly;  // 清除所有5个No子标志
    } else {
        d->plotFlags |= ImPlotFlags_CanvasOnly;   // 设置所有5个No子标志
    }
    if (d->plotFlags != oldFlags) {
        Q_EMIT plotFlagChanged();
    }
}
```

#### beginDraw()中的标志使用

在`beginDraw()`中，标志位已经通过属性setter组装完毕，直接传递给ImPlot API即可：

```cpp
bool QImPlotNode::beginDraw()
{
    QIM_D(d);
    // d->plotFlags 已由各属性setter维护好，无需重新组装
    d->beginPlotSuccess = ImPlot::BeginPlot(title, d->size, d->plotFlags);
    // ...
}
```

**关键要点**：`beginDraw()`中不做标志位的逻辑组装，所有组装工作在setter中完成。`d->plotFlags`是一个统一的`ImPlotFlags`整型变量，各属性的setter通过位操作（`|=`, `&= ~`）修改这个变量，`beginDraw()`只需直接传递。

#### 属性默认值

属性默认值应与ImPlot默认行为一致。ImPlot的`ImPlotFlags_None = 0`表示所有否定标志都未设置（即所有功能默认启用），因此：

```cpp
// PrivateData中初始化
ImPlotFlags plotFlags { ImPlotFlags_None };  // 默认0，所有No*标志未设置

// 对应的QIm属性默认值：
// titleEnabled   → true  (因为 NoTitle 未设置)
// legendEnabled  → true  (因为 NoLegend 未设置)
// menusEnabled   → true  (因为 NoMenus 未设置)
// equal          → false (因为 Equal 未设置)
// crosshairs     → false (因为 Crosshairs 未设置)
```

#### 开发新节点的操作指南

当开发一个新节点，如果其渲染函数涉及ImPlot/ImGui的枚举标志，按以下步骤进行：

1. **阅读ImPlot/ImGui源码**：理解所有相关枚举的含义、默认值和组合关系
2. **语义分类**：将枚举分为三类：
   - 否定语义（`NoXxx`）→ 需要转换为肯定语义属性（`xxxEnabled`）
   - 肯定语义（`Xxx`）→ 直接映射为肯定语义属性（`xxx`）
   - 组合标志（多个标志的组合）→ 需要手动实现getter/setter
3. **定义Q_PROPERTY**：为每个标志创建`Q_PROPERTY`布尔属性，使用肯定语义命名
4. **选择实现方式**：
   - 否定→肯定：使用`QIMPLOT_FLAG_ENABLED_ACCESSOR`宏
   - 肯定→肯定：使用`QIMPLOT_FLAG_ACCESSOR`宏
   - 组合标志：手动实现
5. **在PrivateData中存储原始标志位**：使用`ImPlotFlags`/`ImGuiFlags`等原始类型存储，各属性setter通过位操作维护
6. **在beginDraw()中直接传递**：将`d->flags`直接传给ImPlot/ImGui API，不做额外组装逻辑

## Git 提交规范

在完成当前任务后，需提交所有更改到 Git 仓库。
创建有意义的提交信息保证下次任务能清楚了解这次任务的实现情况
提交信息最好包含以下信息：

- 任务类型（例如：实现、修复、文档更新）
- 实现内容的简要描述
- 相关文件列表
- 关联到计划书（如果适用）
