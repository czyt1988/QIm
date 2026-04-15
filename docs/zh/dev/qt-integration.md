# Qt集成规范

QIm的核心设计目标是将 ImGui 即时模式组件以 Qt 保留模式的方式封装，让 Qt 开发者无需了解 ImGui API 即可使用。为实现这一目标，所有 QIm 代码必须严格遵循以下 Qt 集成规范。

## 主要功能特性

**特性**

- ✅ **信号槽设计**：充分发挥Qt信号槽机制进行事件通讯
- ✅ **Q_PROPERTY属性暴露**：所有公开属性使用Qt属性系统暴露
- ✅ **Qt命名规范**：属性命名遵循Qt习惯（setColor/color、setSize/size）
- ✅ **大写宏强制使用**：禁止小写Qt信号槽宏

## 信号槽设计

### 设计原则

QIm 充分发挥 Qt 的信号槽机制，所有节点类使用信号和槽进行事件通讯。每个属性变化都应触发对应的信号，使外部对象能够监听属性变更。

### 信号命名规范

属性变更信号统一使用 `xxxChanged` 命名模式：

```cpp
Q_SIGNALS:
    // 属性变更信号
    void colorChanged(const QColor& color);
    void sizeChanged(float size);
    void opacityChanged(float opacity);
    void visibilityChanged(bool visible);
    void dataChanged(QImAbstractDataSeries* data);
```

!!! warning "重要"
    多个标志属性可共享同一个信号，因为标志位是统一存储的。例如所有 `ImPlotFlags` 相关属性共享 `plotFlagChanged()` 信号。

## Q_PROPERTY属性暴露

### 设计原则

为了贴合 Qt 框架，类的属性必须使用 `Q_PROPERTY` 暴露，使属性可以通过 Qt 元对象系统（QMetaObject）访问，支持信号槽联动和 QML 集成。

### 属性声明模式

**数据属性：**

```cpp
Q_PROPERTY(QImAbstractDataSeries* data READ data WRITE setData NOTIFY dataChanged)
```

**样式属性：**

```cpp
Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
Q_PROPERTY(float size READ size WRITE setSize NOTIFY sizeChanged)
Q_PROPERTY(float opacity READ opacity WRITE setOpacity NOTIFY opacityChanged)
```

**可见性属性：**

```cpp
Q_PROPERTY(bool visible READ isVisible WRITE setVisible NOTIFY visibilityChanged)
```

**标志属性（布尔开关）：**

```cpp
Q_PROPERTY(bool titleEnabled READ isTitleEnabled WRITE setTitleEnabled NOTIFY plotFlagChanged)
Q_PROPERTY(bool legendEnabled READ isLegendEnabled WRITE setLegendEnabled NOTIFY plotFlagChanged)
```

### 属性命名规范

为适配 Qt 使用习惯，属性命名遵循以下模式：

| 属性类型 | getter | setter | signal |
|----------|--------|--------|--------|
| 通用属性 | `color()` | `setColor()` | `colorChanged()` |
| 布尔属性 | `isVisible()` | `setVisible()` | `visibilityChanged()` |
| 标志属性（肯定） | `isTitleEnabled()` | `setTitleEnabled()` | `plotFlagChanged()` |
| 标志属性（直接映射） | `isEqual()` | `setEqual()` | `plotFlagChanged()` |

!!! tip "最佳实践"
    遵循 Qt 惯例：`setColor`/`color`、`setSize`/`size`、`setOpacity`/`opacity`、`setVisible`/`visible`，而非 ImGui 风格命名。

## Qt信号槽宏使用规范

### 强制规则

**禁止使用小写 Qt 信号槽宏**，必须使用大写版本：

| 禁止（小写） | 必须使用（大写） | 说明 |
|-------------|----------------|------|
| `slots` | `Q_SLOTS` | 槽声明区域 |
| `signals` | `Q_SIGNALS` | 信号声明区域 |
| `emit` | `Q_EMIT` | 发射信号 |

### 示例

```cpp
class QImPlotNode : public QImAbstractNode
{
    Q_OBJECT

public:
    Q_SLOTS  // ✅ 正确：使用大写宏
    void setTitleEnabled(bool enabled);

Q_SIGNALS:  // ✅ 正确：使用大写宏
    void plotFlagChanged();
};

void QImPlotNode::setTitleEnabled(bool enabled)
{
    // ...
    Q_EMIT plotFlagChanged();  // ✅ 正确：使用大写宏
}
```

!!! warning "禁止"
    ```cpp
    slots:      // ❌ 禁止
    signals:    // ❌ 禁止
    emit xxx(); // ❌ 禁止
    ```

## 参考

- 相关规范：[枚举语义转换规范](flag-mapping.md)、[PIMPL开发规范](pimpl-dev-guide.md)
- 核心概念：[属性系统](../property-system.md)、[信号槽](../signal-slot.md)