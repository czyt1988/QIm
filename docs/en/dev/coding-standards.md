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

### Source File (.cpp) Comment Standards

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
2. **Do not** write detailed bilingual Doxygen blocks in header files
3. **Exception**: Qt signals (functions under `Q_SIGNALS` section) and class comments need bilingual Doxygen in header files

```cpp
// Class comment standards see next section
class MyClass {
public:
    // Constructor for MyClass (English only)
    MyClass(); 

    // Get the color property (English only)
    QColor color() const;
};
```

### Class Doxygen Comment Standards

Class Doxygen comments must be added in **header files** using bilingual format:

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
| Class comments | `.h` | Bilingual Doxygen block |
| Signal comments | `.h` | Bilingual Doxygen block |

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