# QIm介绍

`QIm` 是一个将 `Dear ImGui`、`ImPlot`、`ImPlot3D` 等ImGui生态集成到Qt框架中的开源库。它通过保留模式（Retained Mode）的封装，让Qt开发者能够使用熟悉的Qt编程范式（信号槽、属性系统、对象树）来构建高性能的实时数据可视化应用。

基于`QIm`，你无需学习 `ImGui`，即可直接使用熟悉的 Qt 信号槽、属性系统与对象树管理机制，构建实时数据监控、科学绘图2D/3D界面、工程仿真可视化等高性能应用场景。

`QIm`计划把ImGui生态的成熟库进行封装，目前正在做`ImPlot`和`ImPlot3D`的封装，旨在为Qt开发环境提供一款开箱即用的数据可视化库。

## 效果

通过`QIm`的`Plot`模块，可以实现如下数据可视化效果：

### 2D绘图

|  |  |  |
|:---:|:---:|:---:|
| ![bars](./docs/assets/plot2D/bars.gif) | ![candle](./docs/assets/plot2D/candle.gif) | ![controls](./docs/assets/plot2D/controls.gif) |
| ![dnd](./docs/assets/plot2D/dnd.gif) | ![heat](./docs/assets/plot2D/heat.gif) | ![markers](./docs/assets/plot2D/markers.gif) |
| ![pie](./docs/assets/plot2D/pie.gif) | ![query](./docs/assets/plot2D/query.gif) | ![realtime](./docs/assets/plot2D/rt.gif) |
| ![shaded](./docs/assets/plot2D/shaded.gif) | ![stem](./docs/assets/plot2D/stem.gif) | ![tables](./docs/assets/plot2D/tables.gif) |

### 3D绘图

## 核心设计

传统的ImGui采用即时模式（Immediate Mode）：

```cpp
// 传统ImGui代码
if (ImGui::Begin("Window")) {
    if (ImPlot::BeginPlot("Plot")) {
        ImPlot::PlotLine(...);
        ImPlot::EndPlot();
    }
    ImGui::End();
}
```

QIm将其转换为保留模式（Retained Mode）：

```cpp
// QIm方式 - 面向对象，更符合Qt习惯
auto window = new QImWindowNode(root);
window->setTitle("Window");

auto plot = new QImPlotNode(window);  // 自动成为子节点
plot->setTitle("Plot");

auto line = new QImPlotLineNode(plot);
line->setData(...);
```

## 快速开始

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

### 简单示例

`QIm`目前把`ImPlot`进行了封装，同步封装了对应的`Widget`窗口，在一个Qt桌面应用程序中，像建立窗口一样即可实现ImGui的界面，同时还支持信号槽，实现Qt环境的数据交互

```cpp
#include <QImFigureWidget.h>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr) : QMainWindow(parent) {
        // 创建绘图窗口
        QIM::QImFigureWidget* figure = new QIM::QImFigureWidget(this);
        setCentralWidget(figure);
        
        // 配置2行1列的子图
        figure->setSubplotGrid(2, 1);
        
        // 创建第一个子图
        QIM::QImPlotNode* plot1 = figure->createPlotNode();
        plot1->x1Axis()->setLabel("时间 (s)");
        plot1->y1Axis()->setLabel("幅度");
        
        // 添加数据
        QVector<double> x = {0, 1, 2, 3, 4};
        QVector<double> y = {0, 1, 4, 9, 16};
        plot1->addLine(x, y, "二次曲线");
        
        // 第二个子图
        QIM::QImPlotNode* plot2 = figure->createPlotNode();
        plot2->setLegendEnabled(true);
        
        // 添加多条曲线
        std::vector<double> x2 = {0, 1, 2, 3, 4};
        std::vector<double> sin_y, cos_y;
        for (double val : x2) {
            sin_y.push_back(std::sin(val));
            cos_y.push_back(std::cos(val));
        }
        plot2->addLine(x2, sin_y, "sin(x)");
        plot2->addLine(x2, cos_y, "cos(x)");
    }
};
```

当前`QIm`的`Plot`模块存在如下**缺陷**，选用请自行评估

- 不能任意添加字体，你需要先加载字体文件才能加载对应字体
- 不支持线形，你无法指定虚线、点划线等操作
- 超过50万点后渲染开始有压力（目前在使用LTTB算法降采样，目标是千万无压力）

### 渲染模式控制

QIm提供三种渲染策略适应不同场景：

```cpp
QImWidget* widget = new QImWidget();
widget->setRenderMode(QImWidget::RenderAdaptive);  // 默认：智能自适应

// 三种模式：
// 1. RenderAdaptive: 交互时高帧率(18FPS)，静止时低帧率(1FPS)
// 2. RenderContinuous: 持续渲染(18FPS)，适合动画
// 3. RenderOnDemand: 仅在事件触发时渲染，最节能
```

### 自定义节点开发

继承`QImAbstractNode`创建自定义组件：

```cpp
class CustomPlotNode : public QImAbstractNode {
    Q_OBJECT
public:
    CustomPlotNode(QObject* parent = nullptr) : QImAbstractNode(parent) {}
    
protected:
    bool beginDraw() override {
        // 对应 ImGui::Begin("MyCustomWindow")
        return ImGui::Begin("MyCustomWindow", nullptr, m_flags);
    }
    
    void endDraw() override {
        ImGui::End();
    }
    
private:
    ImGuiWindowFlags m_flags = 0;
};
```

**QIm** - 让ImGui快速应用在Qt程序中