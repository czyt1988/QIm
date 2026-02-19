# 渲染节点

## 简介

ImGui 原生采用过程式编程模型（`BeginXxx()`/`EndXxx()` 对），这在复杂 UI 中容易导致：

- 作用域嵌套混乱（缩进地狱）
- 状态管理困难（窗口位置、折叠状态等）
- 代码复用性差（重复的 Begin/End 模板代码）

`QImAbstractNode` 是一个为 **Dear ImGui** 设计的 **Qt 风格树形节点基类**，它将 ImGui 的命令式即时模式（Immediate Mode）API 封装为面向对象的声明式架构。通过树形结构管理 ImGui 的 Section（如 Window、Child、Group、Plot 等），实现 UI 组件的模块化、可复用和易维护。

**QImAbstractNode 的解决方案**：

对于原生ImGui的如下代码：
```cpp
// 传统 ImGui（过程式）
if (ImGui::Begin("Window")) {
    if (ImPlot::BeginPlot("Plot")) {
        ImPlot::PlotLine(...);
        ImPlot::EndPlot();
    }
    ImGui::End();
}
```

使用QImAbstractNode的方式，将变为下面的方式：

```cpp
auto window = new QImWindowNode(root);
window->setTitle("Window");

auto plot = new QImPlotNode(window);  // 自动嵌套
plot->setTitle("Plot");

auto line = new QImPlotLineNode(plot); // 自动成为 Plot 的子节点
line->setData(...);
```

每个 UI 元素 = 一个对象，UI 结构 = 对象树，渲染 = 递归遍历树

## 特性

`QImAbstractNode`提供了`beginDraw()`和`endDraw()`两个虚函数，对应`ImGui`的`beginXX`和`endXX`函数

在实例化`QImAbstractNode`时必须重写这两个虚函数

`QImAbstractNode`提供了树形节点管理

### 1. 封装了"窗口"的基本功能

一个窗口的基本功能是`visible`和`enabled`，这个在`QImAbstractNode`里已经封装好

| 状态 | 作用 | ImGui 对应 |
|------|------|-----------|
| `visible` | 控制是否参与渲染 | 直接跳过 `render()` 调用 |
| `enabled` | 封装了控制交互可用性 | `ImGui::BeginDisabled()` / `EndDisabled()` |

同时也增加了信号`visibleChanged`和`enabledChanged`
```cpp
Q_SIGNALS:
    void visibleChanged(bool visible);
    void enabledChanged(bool enabled);
```


### 2. ImGui ID 自动隔离

ImGui的即时响应式操作导致当两个控件使用相同标签时，ImGui 会认为它们是同一个控件，例如：

```cpp
if (ImGui::Button("OK")) { /* 点击 */ }  // ID = "OK"
if (ImGui::Button("OK")) { /* 永远不会触发，因为和上面是同一个ID */ }
```

如果要让上面的代码正常工作，应该这样操作：

```cpp
ImGui::PushID("Dialog1");  // 压入前缀
if (ImGui::Button("OK")) { /* ID = hash("Dialog1") + hash("OK") */ }
ImGui::PopID();  // 弹出前缀

ImGui::PushID("Dialog2");  // 不同前缀
if (ImGui::Button("OK")) { /* ID = hash("Dialog2") + hash("OK") */ }
ImGui::PopID();
```

`QImAbstractNode`实际是维护一个`ImGui`的渲染段为了避免控件的冲突，渲染函数调用的时候，会默认给推入一个id，渲染结束时再推出一个id，这样用户就无需关注id的事情

```cpp
void QImAbstractNode::render() {
    ImGui::PushID(this);  // 使用 this 指针作为唯一标识
    // ... 子节点渲染 ...
    ImGui::PopID();
}
```


