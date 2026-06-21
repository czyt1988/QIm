# Coding & Comment Standards

This standard specifies the code style consistency requirements and comment format standards for all QIm project code, along with Git commit conventions. Following these standards ensures code is maintainable, comments are searchable, and team collaboration is smooth.

## Key Features

**Features**

- ✅ **Code Style Consistency**: Strictly maintain consistency with existing code naming, indentation, header organization
- ✅ **Doxygen Bilingual Comments**: Source files use dual Chinese-English block comment format
- ✅ **Header Concise Comments**: Header files only retain single-line English brief comments
- ✅ **Class Comment Bilingual Standards**: Classes and signals use bilingual Doxygen comments in header files
- ✅ **Git Commit Standards**: Structured commit message format

## Code Style

### Basic Principles

- **Strictly maintain consistency with existing code**: naming conventions, indentation, header file organization, etc.
- **Follow Qt development best practices**: Use `Q_PROPERTY`, `Q_SIGNALS`, `Q_SLOT` etc. macros; prohibit `slot`, `signal` etc. lowercase macros (see [Qt Integration Standards](qt-integration.md))

### Naming Conventions

| Category | Convention | Example |
|------|------|------|
| Class name | QIm prefix + PascalCase | `QImPlotNode`, `QImPlotLineNode` |
| Method name | camelCase, Qt-style | `setTitle()`, `addLine()` |
| Property name | camelCase, Qt-style | `color`, `size`, `opacity` |
| Signal | xxxChanged pattern | `colorChanged()`, `plotFlagChanged()` |
| Private data class | PrivateData | Inner class storing PIMPL implementation details |

## Comment Standards (Mandatory)

All new code must use **Doxygen format**, distinguishing Chinese and English.

### Forbidden Patterns (Mandatory)

The following patterns are **strictly prohibited** in any new code. AI-generated code is particularly prone to violating these rules, so check each one carefully.

#### 1. ❌ Header file public function bilingual Doxygen → ✅ Single-line English comment

```cpp
// ❌ Forbidden: Header file public function with bilingual Doxygen block
/**
 * \if ENGLISH
 * @brief Gets the heatmap scale minimum value
 * @return The minimum scale value
 * \endif
 *
 * \if CHINESE
 * @brief 获取热力图缩放最小值
 * @return 最小缩放值
 * \endif
 */
double scaleMin() const;

// ✅ Correct: Header file public function uses only single-line English comment
// Gets the minimum scale value
double scaleMin() const;
```

#### 2. ❌ Q_PROPERTY with bilingual Doxygen comments → ✅ No comments at all

```cpp
// ❌ Forbidden: Q_PROPERTY with bilingual Doxygen comments above it
/**
 * \if ENGLISH
 * @property QImPlotHeatmapItemNode::scaleMin
 * @brief Minimum value for color scaling
 * @details Defines the lower bound of the color scale.
 * @accessors READ scaleMin WRITE setScaleMin NOTIFY scaleMinChanged
 * \endif
 *
 * \if CHINESE
 * @property QImPlotHeatmapItemNode::scaleMin
 * @brief 颜色缩放的最小值
 * @details 定义颜色尺度的下限。
 * @accessors READ scaleMin WRITE setScaleMin NOTIFY scaleMinChanged
 * \endif
 */
Q_PROPERTY(double scaleMin READ scaleMin WRITE setScaleMin NOTIFY scaleMinChanged)

// ✅ Correct: Q_PROPERTY with no comments at all, only grouping comments allowed (e.g., // == Title properties ==)
// == Title properties ==
Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
Q_PROPERTY(bool titleEnabled READ isTitleEnabled WRITE setTitleEnabled NOTIFY plotFlagChanged)
```

#### 3. ❌ Class comment with @param, @class, @ingroup → ✅ Only @brief/@details/@note/@see

```cpp
// ❌ Forbidden: Class comment containing @param, @class, @ingroup
/**
 * \if ENGLISH
 * @brief Qt-style wrapper for ImPlot heatmap visualization
 * @class QImPlotHeatmapItemNode
 * @ingroup plot_items
 * @details Provides Qt-style retained mode encapsulation...
 * @param[in] parent Parent QObject (optional)
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot热力图可视化的Qt风格封装
 * @class QImPlotHeatmapItemNode
 * @ingroup plot_items
 * @details 为ImPlot热力图提供Qt风格的保留模式封装...
 * @param[in] parent 父QObject对象（可选）
 * \endif
 */

// ✅ Correct: Class comment uses only @brief/@details/@note/@see, forbidding @param/@class/@ingroup
/**
 * \if ENGLISH
 * @brief ImPlot plot area node
 * @details Manages the lifecycle, axis configuration, and rendering context
 *          for a single ImPlot plot area within the QIm object tree.
 * @note Uses the PIMPL pattern via QIM_DECLARE_PRIVATE for encapsulation.
 * @see QImPlotItemNode, QImPlotAxisInfo, QImPlotLegendNode
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot 绘图区域节点
 * @details 在QIm对象树中管理单个ImPlot绘图区域的生命周期、坐标轴配置和渲染上下文。
 * @note 通过QIM_DECLARE_PRIVATE采用PIMPL模式实现封装。
 * @see QImPlotItemNode, QImPlotAxisInfo, QImPlotLegendNode
 * \endif
 */
```

!!! danger "Mandatory Rules"
    - ❌ **Header file public functions**: Strictly prohibited to use bilingual Doxygen, only single-line English `//` allowed
    - ❌ **Q_PROPERTY**: Strictly prohibited to add any comments (including Doxygen blocks)
    - ❌ **Class comments**: Strictly prohibited to use `@param`, `@class`, `@ingroup`, only `@brief`/`@details`/`@note`/`@see` allowed

Function implementations in source files must use complete dual Chinese-English Doxygen comment blocks:

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

!!! warning "Important"
    In principle, detailed function comments should be written in the corresponding `.cpp` file, not in header files.

### Header File (.h) Comment Standards

Comments in header files follow these rules:

1. **`public` function declarations**: Only add **single-line English brief comments** (using `//` or concise `/** */`)
2. **Strictly prohibited** to write detailed bilingual Doxygen blocks in header files
3. **Exception**: Qt signals (functions under `Q_SIGNALS` section) and class comments need bilingual Doxygen in header files

```cpp
// The following is a real node class comment template, based on QImPlotNode (see src/core/plot/QImPlotNode.h)
/**
 * \if ENGLISH
 * @brief ImPlot plot area node
 * @details Manages the lifecycle, axis configuration, and rendering context
 *          for a single ImPlot plot area within the QIm object tree.
 * @note Uses the PIMPL pattern via QIM_DECLARE_PRIVATE for encapsulation.
 * @see QImPlotItemNode, QImPlotAxisInfo, QImPlotLegendNode
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot 绘图区域节点
 * @details 在QIm对象树中管理单个ImPlot绘图区域的生命周期、坐标轴配置和渲染上下文。
 * @note 通过QIM_DECLARE_PRIVATE采用PIMPL模式实现封装。
 * @see QImPlotItemNode, QImPlotAxisInfo, QImPlotLegendNode
 * \endif
 */
class QImPlotNode : public QImAbstractNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotNode)

    // Q_PROPERTY with no comments!
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(bool titleEnabled READ isTitleEnabled WRITE setTitleEnabled NOTIFY plotFlagChanged)

public:
    // Constructs a QImPlotNode with optional parent (English only!)
    QImPlotNode(QObject* parent = nullptr);

    // Gets the plot title (English only!)
    QString title() const;

Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when the title property changes
     * @param[in] title The new title value
     * \endif
     *
     * \if CHINESE
     * @brief 标题属性变更时发射
     * @param[in] title 新的标题值
     * \endif
     */
    void titleChanged(const QString& title);
};
```

**Again, it is strictly forbidden to write bilingual Doxygen blocks for class member functions in header files.**

### Class Doxygen Comment Standards

Class Doxygen comments must be added in **header files** using bilingual format, **only allowing** `@brief`/`@details`/`@note`/`@see`, **strictly prohibiting** `@param`/`@class`/`@ingroup`:

```cpp
/**
 * \if ENGLISH
 * @brief ImPlot plot area node
 * @details Manages the lifecycle, axis configuration, and rendering context
 *          for a single ImPlot plot area within the QIm object tree.
 * @note Uses the PIMPL pattern via QIM_DECLARE_PRIVATE for encapsulation.
 * @see QImPlotItemNode, QImPlotAxisInfo, QImPlotLegendNode
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot 绘图区域节点
 * @details 在QIm对象树中管理单个ImPlot绘图区域的生命周期、坐标轴配置和渲染上下文。
 * @note 通过QIM_DECLARE_PRIVATE采用PIMPL模式实现封装。
 * @see QImPlotItemNode, QImPlotAxisInfo, QImPlotLegendNode
 * \endif
 */
class QImPlotNode : public QImAbstractNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotNode)

    // Q_PROPERTY with no comments!
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)

public:
    // Constructs a QImPlotNode with optional parent (English only!)
    QImPlotNode(QObject* parent = nullptr);
};
```

!!! tip "Tip"
    For highly functional classes, usage examples should be included in the class comment to help users understand how to use it.

### Signal Doxygen Comment Standards

Qt signals have no definition in `.cpp` files, so their Doxygen comments must be added in **header files** using bilingual format:

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

### Comment Location Summary

| Comment Type | Location | Format |
|----------|------|------|
| Function detailed comments | `.cpp` | Bilingual Doxygen block |
| public function brief comments | `.h` | Single-line English `//` |
| Class comments | `.h` | Bilingual Doxygen block (only @brief/@details/@note/@see) |
| Signal comments | `.h` | Bilingual Doxygen block |
| **Q_PROPERTY comments** | **Forbidden** | **No comments at all** |

## Git Commit Standards

After completing a task, all changes must be committed to the Git repository. Create meaningful commit messages to ensure future tasks can clearly understand what was implemented.

### Commit Message Format

Commit messages should include the following information:

1. **Task type** (e.g.: implementation, fix, documentation update)
2. **Brief description of what was implemented**
3. **Related file list**
4. **Link to plan document** (if applicable)

### Commit Message Example

```
Documentation update: Restructure docs, split into plot2d/plot3d/dev directories

- Move figure-widget.md, plot-line.md etc. to plot2d/ directory
- Create dev/ standards directory, write 6 standards documents
- Update mkdocs.yml navigation configuration
- Simplify AGENTS.md content

Related plan: Documentation restructuring plan
```

## References

- Related Standards: [Qt Integration Standards](qt-integration.md), [PIMPL Development Guide](pimpl-dev-guide.md)
- Documentation Writing Standards: `docs/doc-writing-guide.md`