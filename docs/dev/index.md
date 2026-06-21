# Development Standards

This directory contains the standards and design philosophy documents that must be followed when developing QIm. These standards are for **QIm contributors and developers**, helping you write QIm code correctly and efficiently.

!!! tip "Tip"
    If you are a QIm user rather than a developer, please refer to [Core Concepts](../render-node.md) and [2D Plot](../plot2d/index.md) documentation.

## Why These Standards Are Needed

QIm wraps ImGui immediate-mode components as Qt retained-mode objects. This cross-paradigm mapping requires unified coding standards to ensure:

- All node behaviors are consistent and uniform
- Code is maintainable and comments are searchable
- Rendering performance is optimal

## Standards Navigation

| Document | Description | Importance |
|----------|-------------|------------|
| [Qt Integration Standards](qt-integration.md) | Signal-slot design, Q_PROPERTY exposure, macro conventions | ⭐⭐⭐ Must Read |
| [Coding & Comment Standards](coding-standards.md) | Doxygen bilingual comment format, code style consistency, Git commit conventions | ⭐⭐⭐ Must Read |
| [PIMPL Development Guide](pimpl-dev-guide.md) | QIM_DECLARE_PRIVATE/QIM_DECLARE_PUBLIC macro usage | ⭐⭐⭐ Must Read |
| [Render Performance Guidelines](render-guidelines.md) | beginDraw minimization principle, UTF8-only storage | ⭐⭐⭐ Must Read |
| [Flag Mapping Standards](flag-mapping.md) | ImPlot/ImGui negative→positive semantic conversion rules and implementation macros | ⭐⭐⭐ Must Read |
| [New Node Development Guide](new-node-guide.md) | Complete workflow for developing new nodes | ⭐⭐ Recommended |
| [Custom Node](custom-node.md) | Inheriting QImAbstractNode to create custom components | ⭐⭐ Recommended |

## Relationships Between Standards

```mermaid
flowchart TD
    A[Qt Integration Standards] --> B[Q_PROPERTY Property Exposure]
    B --> C[Flag Mapping Standards]
    C --> D[New Node Development Guide]
    E[PIMPL Development Guide] --> D
    F[Render Performance Guidelines] --> D
    G[Coding & Comment Standards] --> D
    D --> H[Custom Node]
</flowmermaid>