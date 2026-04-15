# 代码风格与注释规范

本规范规定了 QIm 项目中所有代码的风格一致性要求和注释格式标准，同时包含 Git 提交规范。遵循这些规范确保代码可维护、注释可检索、团队协作顺畅。

## 主要功能特性

**特性**

- ✅ **代码风格一致性**：严格保持与现有代码一致的命名、缩进、头文件组织
- ✅ **Doxygen双语注释**：源文件使用中英文双块注释格式
- ✅ **头文件简洁注释**：头文件仅保留单行英文简要注释
- ✅ **类注释双语规范**：类和信号在头文件中使用双语Doxygen注释
- ✅ **Git提交规范**：结构化提交信息格式

## 代码风格

### 基本原则

- **严格保持与现有代码一致**：命名规范、缩进、头文件组织等
- **遵循 Qt 开发最佳实践**：使用 `Q_PROPERTY`、`Q_SIGNALS`、`Q_SLOT` 等宏，禁止使用 `slot`、`signal` 等小写命名的宏（详见 [Qt集成规范](qt-integration.md)）

### 命名规范

| 类别 | 规范 | 示例 |
|------|------|------|
| 类名 | QIm前缀 + 大驼峰 | `QImPlotNode`、`QImPlotLineNode` |
| 方法名 | 小驼峰，Qt风格 | `setTitle()`、`addLine()` |
| 属性名 | 小驼峰，Qt风格 | `color`、`size`、`opacity` |
| 信号 | xxxChanged模式 | `colorChanged()`、`plotFlagChanged()` |
| 私有数据类 | PrivateData | 内部类，存放PIMPL实现细节 |

## 注释规范（强制）

所有新增代码必须使用 **Doxygen 格式**，并区分中英文。

### 源文件（.cpp）注释规范

源文件中的函数实现必须使用完整的中英文双块 Doxygen 注释：

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

!!! warning "重要"
    原则上详细函数注释应该写在对应的 `.cpp` 文件中，而不是头文件中。

### 头文件（.h）注释规范

头文件中的注释遵循以下规则：

1. **`public` 函数声明**：仅添加**单行英文简要注释**（使用 `//` 或简洁的 `/** */`）
2. **不要**在头文件中写入详细的双语 Doxygen 块
3. **特例**：Qt 信号（`Q_SIGNALS` 区域下的函数）和类的注释需在头文件中使用双语 Doxygen

```cpp
// 类的注释规范见下一节
class MyClass {
public:
    // Constructor for MyClass (English only)
    MyClass(); 

    // Get the color property (English only)
    QColor color() const;
};
```

### 类的 doxygen 注释规范

类的 doxygen 注释需要在**头文件**中按双语要求添加：

```cpp
/**
 * \if ENGLISH
 * @brief [English description of the class]
 * \endif
 *
 * \if CHINESE
 * @brief [中文类说明]
 * \endif
 */
class MyClass {
public:
    // Constructor for MyClass (English only)
    MyClass(); 
};
```

!!! tip "技巧"
    对于功能性较强的类，类的注释中应该加入使用示例，以便使用者了解如何使用。

### 信号的 doxygen 注释规范

Qt 信号没有 `.cpp` 中的定义，因此信号的 doxygen 注释需要在**头文件**中使用双语格式添加：

```cpp
Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when the color property changes
     * @param[in] color The new color value
     * \endif
     *
     * \if CHINESE
     * @brief 颜色属性变更时发射
     * @param[in] color 新的颜色值
     * \endif
     */
    void colorChanged(const QColor& color);
```

### 注释位置总结

| 注释类型 | 位置 | 格式 |
|----------|------|------|
| 函数详细注释 | `.cpp` | 双语 Doxygen 块 |
| public 函数简要注释 | `.h` | 单行英文 `//` |
| 类注释 | `.h` | 双语 Doxygen 块 |
| 信号注释 | `.h` | 双语 Doxygen 块 |

## Git 提交规范

在完成当前任务后，需提交所有更改到 Git 仓库。创建有意义的提交信息保证下次任务能清楚了解这次任务的实现情况。

### 提交信息格式

提交信息最好包含以下信息：

1. **任务类型**（例如：实现、修复、文档更新）
2. **实现内容的简要描述**
3. **相关文件列表**
4. **关联到计划书**（如果适用）

### 提交信息示例

```
文档更新：重组文档结构，拆分plot2d/plot3d/dev目录

- 移动 figure-widget.md、plot-line.md 等到 plot2d/ 目录
- 创建 dev/ 开发规范目录，编写6个规范文档
- 更新 mkdocs.yml 导航配置
- 精简 AGENTS.md 内容

关联计划：文档结构重组计划
```

## 参考

- 相关规范：[Qt集成规范](qt-integration.md)、[PIMPL开发规范](pimpl-dev-guide.md)
- 文档撰写规范：`docs/doc-writing-guide.md`