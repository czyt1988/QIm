#include <QApplication>
#include <QSurfaceFormat>
#include <QOpenGLContext>

#ifdef _WIN32
#    include "MainWindow.h"
#else
#    error "QIm System Monitor is only supported on Windows"
#endif

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

#ifdef _WIN32
    QSurfaceFormat glFormat;
    if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL) {
        glFormat.setVersion(3, 3);
        glFormat.setProfile(QSurfaceFormat::CoreProfile);
    }
    QSurfaceFormat::setDefaultFormat(glFormat);

    MainWindow window;
    window.show();
#endif

    return app.exec();
}