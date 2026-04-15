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

## 开发规范

详细的开发规范文档已迁移至 `docs/zh/dev/` 目录，请按需阅读：

| 规范文档 | 说明 | 重要程度 |
|----------|------|----------|
| [Qt集成规范](docs/zh/dev/qt-integration.md) | 信号槽设计、Q_PROPERTY属性暴露、宏使用规范 | ⭐⭐⭐ 必读 |
| [代码风格与注释规范](docs/zh/dev/coding-standards.md) | Doxygen双语注释格式、代码风格一致性、Git提交规范 | ⭐⭐⭐ 必读 |
| [PIMPL开发规范](docs/zh/dev/pimpl-dev-guide.md) | QIM_DECLARE_PRIVATE/QIM_DECLARE_PUBLIC宏使用 | ⭐⭐⭐ 必读 |
| [渲染性能规范](docs/zh/dev/render-guidelines.md) | beginDraw最小化原则、UTF8-only存储规范 | ⭐⭐⭐ 必读 |
| [枚举语义转换规范](docs/zh/dev/flag-mapping.md) | ImPlot/ImGui否定→肯定语义转换规则和实现宏 | ⭐⭐⭐ 必读 |
| [新节点开发指南](docs/zh/dev/new-node-guide.md) | 新节点开发的完整操作流程 | ⭐⭐ 推荐 |
| [自定义节点](docs/zh/dev/custom-node.md) | 继承QImAbstractNode创建自定义组件 | ⭐⭐ 推荐 |

### 快速规范速查

以下是最关键的规范要点速查，详细内容请阅读上方对应文档：

- **Qt宏**：禁止使用 `slots`/`signals`/`emit`，必须使用 `Q_SLOTS`/`Q_SIGNALS`/`Q_EMIT`
- **字符串存储**：节点只存储 `QByteArray`（UTF8格式），不存储 `QString`
- **渲染最小化**：`beginDraw()` 只做API调用，所有数据转换在setter中完成
- **枚举语义**：ImPlot否定语义(`NoXxx`) → Qt肯定语义(`xxxEnabled`)
- **注释规范**：源文件双语Doxygen，头文件单行英文，类和信号在头文件中双语

## QIm对象树管理理念

QIm的设计哲学就是对象树，每个图表元素都是一个节点，节点之间通过父级-子级关系组织。具体可以通过[渲染节点文档](docs/zh/render-node.md)来了解

## 文档结构

项目文档使用 mkdocs 组织，主要目录结构如下：

```
docs/zh/
├── index.md           ← 首页
├── render-node.md     ← 渲染节点理念
├── plot2d/            ← 2D绘图（ImPlot封装）
├── plot3d/            ← 3D绘图（ImPlot3D封装，预留）
├── dev/               ← 开发规范
│   ├── qt-integration.md
│   ├── coding-standards.md
│   ├── pimpl-dev-guide.md
│   ├── render-guidelines.md
│   ├── flag-mapping.md
│   ├── new-node-guide.md
│   └── custom-node.md
```

英文文档位于 `docs/en/`，结构与中文版对应。