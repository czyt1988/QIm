# Render Performance Guidelines

ImGui nodes are rendered in real-time — `beginDraw()`/`endDraw()` functions execute every frame. Therefore, render functions should not perform complex logic; all data preparation should be completed during property setting (setter). This standard specifies the code writing requirements for node rendering to ensure optimal rendering performance.

## Key Features

**Features**

- ✅ **beginDraw Minimization Principle**: Render functions only do the simplest API calls
- ✅ **UTF8-only Storage Standard**: Nodes only store UTF8 format data internally, avoiding runtime conversions
- ✅ **Setter Pre-processing Principle**: All data conversions and calculations are completed in setters
- ✅ **Avoid Runtime Data Preparation**: No conditional data preparation in beginDraw

## beginDraw Minimization Principle

### Core Rules

The `beginDraw()` function should only do the simplest data passing to ImGui/ImPlot API:

- **Directly pass** already prepared data
- **Do not** do data conversions (e.g. `toUtf8()`)
- **Do not** do conditional data preparation after judgment
- **Do not** do complex calculation logic

```mermaid
flowchart LR
    A[Property setter] -->|Pre-process data| B[PrivateData storage]
    B -->|Directly pass| C[beginDraw]
    C -->|API call| D[ImGui/ImPlot render]
```

### Correct Pattern

```cpp
bool QImPlotNode::beginDraw()
{
    QIM_D(d);
    // d->plotFlags is already maintained by property setters, no need to re-assemble
    d->beginPlotSuccess = ImPlot::BeginPlot(d->titleUtf8.constData(), d->size, d->plotFlags);
    return d->beginPlotSuccess;
}
```

### Wrong Pattern (Prohibited)

```cpp
bool QImPlotNode::beginDraw()
{
    // ❌ Wrong: assemble flags in beginDraw
    ImPlotFlags flags = ImPlotFlags_None;
    if (!d->showTitle) flags |= ImPlotFlags_NoTitle;
    if (!d->showLegend) flags |= ImPlotFlags_NoLegend;
    
    // ❌ Wrong: convert strings in beginDraw
    QByteArray titleUtf8 = d->title.toUtf8();
    
    ImPlot::BeginPlot(titleUtf8.constData(), d->size, flags);
}
```

## String Storage Standard (Important)

### Principle: Nodes Only Store UTF8 Format Data, Not QString

Property interfaces accept `QString` parameters (Qt convention), but ImGui interfaces only accept `char*`. Node internal variables should **only store `QByteArray` (UTF8 format)**, avoiding `toUtf8()` conversions in `beginDraw` and also avoiding storing both QString and QByteArray causing memory waste.

### Correct Pattern

```cpp
// PrivateData - only store QByteArray
class QImPlotAnnotationNode::PrivateData
{
    QByteArray textUtf8;  ///< Text content (UTF8 format, directly usable by ImGui)
    // Do not store QString text;
};

// getter - convert from UTF8 back to QString
QString QImPlotAnnotationNode::text() const
{
    return QString::fromUtf8(d_ptr->textUtf8);
}

// setter - convert and only store UTF8
void QImPlotAnnotationNode::setText(const QString& text)
{
    QIM_D(d);
    QByteArray utf8 = text.toUtf8();
    if (d->textUtf8 != utf8) {
        d->textUtf8 = utf8;
        Q_EMIT textChanged(text);
    }
}

// beginDraw - directly use UTF8 data, no conversion needed
bool QImPlotAnnotationNode::beginDraw()
{
    QIM_D(d);
    if (d->textUtf8.isEmpty()) {
        // No text case
    } else {
        // Directly use UTF8 data
        ImPlot::Annotation(..., "%s", d->textUtf8.constData());
    }
}
```

### Wrong Pattern (Prohibited)

```cpp
// ❌ Wrong: storing QString and QByteArray as two copies of data
QString text;          // Redundant storage
QByteArray textUtf8;   // Redundant

// ❌ Wrong: converting in beginDraw
bool beginDraw() {
    QByteArray utf8 = d->text.toUtf8();  // Performance loss!
    ImPlot::Annotation(..., "%s", utf8.constData());
}
```

!!! warning "Mandatory Standard"
    - Nodes only store `QByteArray` (UTF8 format), **not QString**
    - Getter functions convert from UTF8 back to QString for users
    - Setter functions receive QString and immediately convert to UTF8 for storage
    - beginDraw functions directly use `constData()` to pass to ImGui/ImPlot API

## Other Performance Considerations

### All Data Conversions Completed in Setter

```cpp
void QImPlotLineNode::setColor(const QColor& color)
{
    QIM_D(d);
    // Complete QColor to ImVec4 conversion in setter
    d->imVec4Color = ImVec4(color.redF(), color.greenF(), color.blueF(), color.alphaF());
    if (d->qColor != color) {
        d->qColor = color;
        Q_EMIT colorChanged(color);
    }
}

bool QImPlotLineNode::beginDraw()
{
    QIM_D(d);
    // Directly use already converted ImVec4, no runtime conversion needed
    ImPlot::SetNextLineStyle(d->imVec4Color);
    ImPlot::PlotLine(d->labelUtf8.constData(), d->xData, d->yData, d->count);
}
```

### Avoid Conditional Data Preparation in beginDraw

```cpp
// ❌ Wrong: conditional data preparation
bool beginDraw() {
    QIM_D(d);
    if (d->style == StyleA) {
        prepareStyleAData();  // Should not prepare data during rendering
    } else {
        prepareStyleBData();  // Should not prepare data during rendering
    }
}

// ✅ Correct: complete preparation in setter
void setStyle(Style style) {
    QIM_D(d);
    d->style = style;
    if (style == StyleA) {
        prepareStyleAData();  // Prepare in setter
    } else {
        prepareStyleBData();  // Prepare in setter
    }
}
```

### Use isEmpty() to Check QByteArray

```cpp
bool beginDraw() {
    QIM_D(d);
    // ✅ Correct: use isEmpty() to check QByteArray
    if (d->textUtf8.isEmpty()) {
        // No text
    }
    
    // ❌ Prohibited: should not store QString to check
    // if (d->text.isEmpty()) { ... }
}
```

## Performance Standards Summary

| Standard | Location | Description |
|------|------|------|
| Data conversion | setter | All QColor→ImVec4, QString→QByteArray conversions completed in setter |
| Data storage | PrivateData | Only store formats ImGui can directly use (UTF8, ImVec4, etc.) |
| beginDraw | Render function | Only do API calls, directly pass prepared data |
| String checking | beginDraw | Use `QByteArray::isEmpty()` instead of QString |

## References

- Related Standards: [PIMPL Development Guide](pimpl-dev-guide.md), [Flag Mapping Standards](flag-mapping.md)
- Core Concepts: [Render Node](../render-node.md), [Render Mode](../render-mode.md)