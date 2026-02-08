# QIm介绍

`QIm`将`Dear ImGui`与`ImPlot`/`ImPlot3D` 生态集成至Qt框架，为 Qt 开发者提供原生级（Qt-native）的高性能数据可视化能力。

本库通过传统界面的保留模式（Retained Mode）封装，开发者无需学习 `ImGui` 的即时模式（Immediate Mode）编程范式，即可直接使用熟悉的 Qt 信号槽、属性系统与对象树管理机制，构建实时数据监控、科学绘图2D/3D界面、工程仿真可视化等高性能应用场景。

`QIm`计划把ImGui生态的成熟库进行封装，目前正在做`ImPlot`和`ImPlot3D`的封装，旨在为Qt开发环境提供一款开箱即用的数据可视化库。

## 效果

通过`QIm`的`Plot`模块，可以实现如下数据可视化效果：



## 使用方法

```cpp
QIM::QImFigureWidget* figure = new QIM::QImFigureWidget();
figure->setSubplotGrid(2, 1);
QIM::QImPlotNode* plot1 = figure->createPlotNode()) {
plot1->x1Axis()->setLabel(u8"x1");
plot1->y1Axis()->setLabel(u8"y1");
QVector<double> x1 = ...;
QVector<double> y1 = ...;
plot1->addLine(x1, y1, "curve a");

QIM::QImPlotNode* plot2 = ui->figureWidget1->createPlotNode()) 
plot2->x1Axis()->setLabel(u8"x2");
plot2->y1Axis()->setLabel(u8"y2");
plot2->setLegendEnabled(true);
std::vector<double> x2 = ...;
std::vector<double> y2 = ...;
plot2->addLine(x2, y2, "curve 1");
std::deque<double> x3 = ...;
std::deque<double> y3 = ...;
plot2->addLine(x3, y3, "curve 2");
```

## 特性

当前`QIm`的`Plot`模块存在如下**缺陷**，选用请自行评估

- 不能任意添加字体，你需要先加载字体文件才能加载对应字体
- 不支持线形，你无法指定虚线、点划线等操作
- 超过50万点后渲染开始有压力（目前在使用LTTB算法降采样，目标是千万无压力）

## 快速构建

项目使用`cmake`构建,建议执行安装(install)后使用

```cmake
# 创建构建目录
mkdir build && cd build
# 配置（指定 Qt 安装路径，或确保 Qt 已加入环境变量）
cmake .. -G "Visual Studio 17 2022" -A x64 ^
         -DCMAKE_PREFIX_PATH="C:/Qt/6.5.0/msvc2019_64" ^
         -DCMAKE_BUILD_TYPE=Release
# 构建
cmake --build . --config Release
# 安装（可选，默认安装到 build/install 目录）
cmake --install .
```

引入QIm模块：

```cmake
set(MIN_QT_VERSION 5.14)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} ${MIN_QT_VERSION} COMPONENTS
    Core
    Gui
    Widgets
    OpenGL
    REQUIRED
)
# Qt6要单独加载OpenGLWidgets模块
if(${QT_VERSION_MAJOR} EQUAL 6)
    find_package(Qt${QT_VERSION_MAJOR} ${MIN_QT_VERSION} COMPONENTS
        OpenGLWidgets
        REQUIRED
    )
    target_link_libraries(<your_target> PRIVATE
        Qt${QT_VERSION_MAJOR}::OpenGLWidgets
    )
endif()
find_package(QIm REQUIRED)
target_link_libraries(<your_target> PRIVATE 
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::OpenGL
    QIm::Core 
    QIm::Widgets
)

```

## 核心设计

### 渲染树

QIm针对ImGui的特性，设计了渲染树（Render Tree） 和渲染节点`QImAbstractNode`，所有ImGui相关的都可以封装到渲染节点中