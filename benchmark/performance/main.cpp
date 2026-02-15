// main.cpp
#include "PerformanceTestWidget.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    PerformanceTestWidget window;
    window.setWindowTitle("绘图库性能对比测试");
    window.resize(1200, 800);
    window.show();

    return app.exec();
}
