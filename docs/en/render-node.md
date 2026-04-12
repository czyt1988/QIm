# Render Node

## Introduction

ImGui natively uses procedural programming model (`BeginXxx()`/`EndXxx()` pairs), which in complex UIs leads to:

- Scope nesting chaos (indentation hell)
- Difficult state management (window position, collapse state, etc.)
- Poor code reusability (repetitive Begin/End template code)

`QImAbstractNode` is a **Qt-style tree node base class** designed for **Dear ImGui**, encapsulating ImGui's imperative immediate mode API into object-oriented declarative architecture. It manages ImGui Sections (Window, Child, Group, Plot, etc.) through tree structure, achieving modularized, reusable, and maintainable UI components.

**QImAbstractNode's Solution**:

For native ImGui code like this:
```cpp
// Traditional ImGui (procedural)
if (ImGui::Begin("Window")) {
    if (ImPlot::BeginPlot("Plot")) {
        ImPlot::PlotLine(...);
        ImPlot::EndPlot();
    }
    ImGui::End();
}
```

Using QImAbstractNode approach becomes:

```cpp
auto window = new QImWindowNode(root);
window->setTitle("Window");

auto plot = new QImPlotNode(window);  // Auto nesting
plot->setTitle("Plot");

auto line = new QImPlotLineNode(plot); // Auto becomes Plot's child node
line->setData(...);
```

Each UI element = one object, UI structure = object tree, rendering = recursive tree traversal

## Features

`QImAbstractNode` provides `beginDraw()` and `endDraw()` virtual functions, corresponding to ImGui's `beginXX` and `endXX` functions

When instantiating `QImAbstractNode`, you must override these two virtual functions

`QImAbstractNode` provides tree node management

### 1. Encapsulates Basic "Window" Functionality

Basic window functionality is `visible` and `enabled`, already encapsulated in `QImAbstractNode`

| State | Purpose | ImGui Equivalent |
|--------|---------|-------------------|
| `visible` | Controls render participation | Skip `render()` call directly |
| `enabled` | Controls interaction availability | `ImGui::BeginDisabled()` / `EndDisabled()` |

Also added signals `visibleChanged` and `enabledChanged`
```cpp
Q_SIGNALS:
    void visibleChanged(bool visible);
    void enabledChanged(bool enabled);
```

### 2. ImGui ID Auto Isolation

ImGui's immediate response operation causes two controls with same labels to be treated as same control, for example:

```cpp
if (ImGui::Button("OK")) { /* click */ }  // ID = "OK"
if (ImGui::Button("OK")) { /* never triggers, same ID as above */ }
```

To make this work properly:

```cpp
ImGui::PushID("Dialog1");  // Push prefix
if (ImGui::Button("OK")) { /* ID = hash("Dialog1") + hash("OK") */ }
ImGui::PopID();  // Pop prefix

ImGui::PushID("Dialog2");  // Different prefix
if (ImGui::Button("OK")) { /* ID = hash("Dialog2") + hash("OK") */ }
ImGui::PopID();
```

`QImAbstractNode` maintains an ImGui render segment to avoid control conflicts. When render function is called, it automatically pushes an ID, and pops it when render ends, so users don't need to worry about IDs:

```cpp
void QImAbstractNode::render() {
    ImGui::PushID(this);  // Use this pointer as unique identifier
    // ... child node rendering ...
    ImGui::PopID();
}
```

## References

- Related docs: [Object Tree](object-tree.md), [Custom Node](custom-node.md)
- API Reference: `src/core/QImAbstractNode.h`