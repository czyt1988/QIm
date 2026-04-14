# QIm项目指南

我是一名出色的C++/Qt架构师，有非常丰富的C++编程经验，我正在开发一个名为 **QIm** 的 Qt-ImGui 集成绘图库，该项目将 `Dear ImGui`、`ImPlot`、`ImPlot3D` 等 ImGui 生态组件以 **保留模式（Retained Mode）** 封装到 Qt 框架中，使 Qt 开发者能够使用熟悉的编程范式（信号槽、属性系统、对象树）构建高性能实时数据可视化应用。

## 项目概述

- 项目仓库：https://github.com/czyt1988/QIm
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

## PIMPL模式

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

## 编码注意事项

### 节点渲染函数尽量不做过多操作

ImGui的节点是实时渲染，在渲染函数`beginDraw`中，你不应该进行过多的复杂逻辑，所有逻辑应该在属性设置过程中处理完

例如QString到std::string的转换，属性接口接受QString参数，ImGui接口只接受char*，节点变量应该保存`std::string`或者`QByteArray`对象而不是QString对象，避免在`beginDraw`函数中进行toUtf8()转换

## Git 提交规范

在完成当前任务后，需提交所有更改到 Git 仓库。
创建有意义的提交信息保证下次任务能清楚了解这次任务的实现情况
提交信息最好包含以下信息：

- 任务类型（例如：实现、修复、文档更新）
- 实现内容的简要描述
- 相关文件列表
- 关联到计划书（如果适用）

