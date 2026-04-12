# QIm项目概述

在进行开发之前，需先阅读`AGENTS.md`

## 项目概述

这个项目是将 `Dear ImGui`、`ImPlot`、`ImPlot3D` 等 ImGui 生态组件以 **保留模式（Retained Mode）** 封装到 Qt 框架中

目前正在进行`ImPlot`模块的封装，模块路径`src\core\plot`

在开发之前，需要先完整阅读`3rdparty\implot\implot.h`的源码，了解组件的用法，了解有哪些属性设置

## QIm对象树管理理念

QIm的设计哲学就是对象树，每个图表元素都是一个节点，节点之间通过父级-子级关系组织。在开发其他模块时你可以先阅读`docs\zh\render-node.md`

`ImPlot`一般使用标志位来控制属性，QIm提倡使用函数来设置，你可以参考`src\core\plot\QImPlotLineItemNode.h`类的实现

## 测试

### 绘图功能测试

本项目绘图的功能实现测试代码位于`examples/qimfigure-test`目录下，具体添加方法请阅读`examples/HowToAddNewFunction.md`
