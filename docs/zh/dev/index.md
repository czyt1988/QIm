# 开发规范

本目录包含 QIm 项目开发过程中需要遵循的规范和设计思想文档。这些规范面向 **QIm 贡献者和开发者**，帮助你正确、高效地开发 QIm 代码。

!!! tip "提示"
    如果你是 QIm 的使用者而非开发者，请参考[核心概念](../render-node.md)和[2D绘图](../plot2d/index.md)文档。

## 为什么需要这些规范

QIm 将 ImGui 即时模式组件封装为 Qt 保留模式对象，这种跨范式映射需要统一的编码规范，确保：

- 所有节点行为一致、风格统一
- 代码可维护、注释可检索
- 渲染性能最优

## 规范文档导航

| 文档 | 说明 | 重要程度 |
|------|------|----------|
| [Qt集成规范](qt-integration.md) | 信号槽设计、Q_PROPERTY属性暴露、宏使用规范 | ⭐⭐⭐ 必读 |
| [代码风格与注释规范](coding-standards.md) | Doxygen双语注释格式、代码风格一致性、Git提交规范 | ⭐⭐⭐ 必读 |
| [PIMPL开发规范](pimpl-dev-guide.md) | QIM_DECLARE_PRIVATE/QIM_DECLARE_PUBLIC宏使用 | ⭐⭐⭐ 必读 |
| [渲染性能规范](render-guidelines.md) | beginDraw最小化原则、UTF8-only存储规范 | ⭐⭐⭐ 必读 |
| [枚举语义转换规范](flag-mapping.md) | ImPlot/ImGui否定→肯定语义转换规则和实现宏 | ⭐⭐⭐ 必读 |
| [新节点开发指南](new-node-guide.md) | 新节点开发的完整操作流程 | ⭐⭐ 推荐 |
| [自定义节点](custom-node.md) | 继承QImAbstractNode创建自定义组件 | ⭐⭐ 推荐 |

## 规范之间的关系

```mermaid
flowchart TD
    A[Qt集成规范] --> B[Q_PROPERTY属性暴露]
    B --> C[枚举语义转换规范]
    C --> D[新节点开发指南]
    E[PIMPL开发规范] --> D
    F[渲染性能规范] --> D
    G[代码风格与注释规范] --> D
    D --> H[自定义节点]
</flowmermaid>