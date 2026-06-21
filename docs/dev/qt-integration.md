# Qt Integration Standards

QIm's core design goal is to wrap ImGui immediate-mode components in Qt retained-mode style, enabling Qt developers to use them without knowing ImGui APIs. All QIm code must strictly follow these Qt integration standards.

## Key Features

**Features**

- ✅ **Signal-Slot Design**: Fully leverage Qt's signal-slot mechanism for event communication
- ✅ **Q_PROPERTY Exposure**: All public properties exposed via Qt's property system
- ✅ **Qt Naming Conventions**: Property naming follows Qt habits (setColor/color, setSize/size)
- ✅ **Uppercase Macro Enforcement**: Lowercase Qt signal-slot macros are prohibited

## Signal-Slot Design

### Design Principles

QIm fully leverages Qt's signal-slot mechanism. All node classes use signals and slots for event communication. Each property change should trigger the corresponding signal, enabling external objects to monitor property changes.

### Signal Naming Convention

Property change signals uniformly follow the `xxxChanged` naming pattern:

```cpp
Q_SIGNALS:
    // Property change signals
    void colorChanged(const QColor& color);
    void sizeChanged(float size);
    void opacityChanged(float opacity);
    void visibilityChanged(bool visible);
    void dataChanged(QImAbstractDataSeries* data);
```

!!! warning "Important"
    Multiple flag properties can share the same signal because flags are stored in a single variable. For example, all `ImPlotFlags` related properties share the `plotFlagChanged()` signal.

## Q_PROPERTY Exposure

### Design Principles

To align with the Qt framework, class properties must be exposed using `Q_PROPERTY`, making them accessible through Qt's meta-object system (QMetaObject), supporting signal-slot linkage and QML integration.

### Property Declaration Patterns

**Data properties:**

```cpp
Q_PROPERTY(QImAbstractDataSeries* data READ data WRITE setData NOTIFY dataChanged)
```

**Style properties:**

```cpp
Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
Q_PROPERTY(float size READ size WRITE setSize NOTIFY sizeChanged)
Q_PROPERTY(float opacity READ opacity WRITE setOpacity NOTIFY opacityChanged)
```

**Visibility properties:**

```cpp
Q_PROPERTY(bool visible READ isVisible WRITE setVisible NOTIFY visibilityChanged)
```

**Flag properties (boolean switches):**

```cpp
Q_PROPERTY(bool titleEnabled READ isTitleEnabled WRITE setTitleEnabled NOTIFY plotFlagChanged)
Q_PROPERTY(bool legendEnabled READ isLegendEnabled WRITE setLegendEnabled NOTIFY plotFlagChanged)
```

### Property Naming Conventions

To adapt Qt usage habits, property naming follows these patterns:

| Property Type | getter | setter | signal |
|----------|--------|--------|--------|
| General property | `color()` | `setColor()` | `colorChanged()` |
| Boolean property | `isVisible()` | `setVisible()` | `visibilityChanged()` |
| Flag property (positive) | `isTitleEnabled()` | `setTitleEnabled()` | `plotFlagChanged()` |
| Flag property (direct) | `isEqual()` | `setEqual()` | `plotFlagChanged()` |

!!! tip "Best Practice"
    Follow Qt conventions: `setColor`/`color`, `setSize`/`size`, `setOpacity`/`opacity`, `setVisible`/`visible`, rather than ImGui-style naming.

## Qt Signal-Slot Macro Usage

### Mandatory Rules

**Lowercase Qt signal-slot macros are prohibited**. You must use uppercase versions:

| Prohibited (lowercase) | Must Use (uppercase) | Description |
|-------------|----------------|------|
| `slots` | `Q_SLOTS` | Slot declaration section |
| `signals` | `Q_SIGNALS` | Signal declaration section |
| `emit` | `Q_EMIT` | Emit signal |

### Example

```cpp
class QImPlotNode : public QImAbstractNode
{
    Q_OBJECT

public:
    Q_SLOTS  // ✅ Correct: use uppercase macro
    void setTitleEnabled(bool enabled);

Q_SIGNALS:  // ✅ Correct: use uppercase macro
    void plotFlagChanged();
};

void QImPlotNode::setTitleEnabled(bool enabled)
{
    // ...
    Q_EMIT plotFlagChanged();  // ✅ Correct: use uppercase macro
}
```

!!! warning "Prohibited"
    ```cpp
    slots:      // ❌ Prohibited
    signals:    // ❌ Prohibited
    emit xxx(); // ❌ Prohibited
    ```

## References

- Related Standards: [Flag Mapping Standards](flag-mapping.md), [PIMPL Development Guide](pimpl-dev-guide.md)
- Core Concepts: [Property System](../property-system.md), [Signal/Slot](../signal-slot.md)