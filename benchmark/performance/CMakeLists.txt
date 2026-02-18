# CMakeLists.txt
cmake_minimum_required(VERSION 3.16)
project(PlotPerformance VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# 检查并配置各个库
option(ENABLE_QWT "Enable Qwt testing" ON)
option(ENABLE_QCUSTOMPLOT "Enable QCustomPlot testing" ON)
option(ENABLE_QTCHARTS "Enable QtCharts testing" OFF)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
set(APP_SOURCE_FILES)

if(ENABLE_QWT)
    list(APPEND APP_SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/qwt/QwtPlot.h
        ${CMAKE_CURRENT_SOURCE_DIR}/qwt/QwtPlot.cpp
    )
endif()

if(ENABLE_QCUSTOMPLOT)
    list(APPEND APP_SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/qcustomplot/qcustomplot.h
        ${CMAKE_CURRENT_SOURCE_DIR}/qcustomplot/qcustomplot.cpp
    )
endif()

list(APPEND APP_SOURCE_FILES
    main.cpp
    PerformanceTestWidget.h
    PerformanceTestWidget.cpp
    PerformanceTestWidget.ui
    PerformanceTestController.h
    PerformanceTestController.cpp
    PerformanceTestReportDialog.h
    PerformanceTestReportDialog.cpp
    PerformanceTestReportDialog.ui
)

message(STATUS "APP_SOURCE_FILES=${APP_SOURCE_FILES}")
if(ANDROID)
    add_library(PlotPerformance SHARED
        ${APP_SOURCE_FILES}
    )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
else()
    add_executable(PlotPerformance
        ${APP_SOURCE_FILES}   
    )
endif()



find_package(Qt${QT_VERSION_MAJOR} ${QIM_MIN_QT_VERSION} REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    OpenGL
)
if (ENABLE_QTCHARTS)
    find_package(Qt${QT_VERSION_MAJOR} ${QIM_MIN_QT_VERSION} REQUIRED COMPONENTS
        Charts
    )
endif()

target_link_libraries(PlotPerformance PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::OpenGL
    QIm::Core
    QIm::Widgets
)
if (ENABLE_QTCHARTS)
    target_link_libraries(PlotPerformance PUBLIC
        Qt${QT_VERSION_MAJOR}::Charts
    )
endif()

if(${QT_VERSION_MAJOR} EQUAL 6)
    find_package(Qt6 COMPONENTS OpenGLWidgets REQUIRED)
    target_link_libraries(PlotPerformance PRIVATE
        Qt6::OpenGLWidgets
    )
endif()
if(ENABLE_QWT)
    find_package(Qt${QT_VERSION_MAJOR} ${QIM_MIN_QT_VERSION} REQUIRED COMPONENTS
        Svg
        PrintSupport
        Concurrent
    )
    target_compile_definitions(PlotPerformance PUBLIC
        HAVE_QWT
    )
    target_include_directories(PlotPerformance PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/qwt
    )
    target_link_libraries(PlotPerformance PRIVATE
        Qt${QT_VERSION_MAJOR}::Svg
        Qt${QT_VERSION_MAJOR}::PrintSupport
        Qt${QT_VERSION_MAJOR}::Concurrent
    )
endif()
if(ENABLE_QCUSTOMPLOT)
    target_compile_definitions(PlotPerformance PUBLIC
        HAVE_QCUSTOMPLOT
        QCUSTOMPLOT_USE_OPENGL # support opengl
    )
    target_include_directories(PlotPerformance PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/qcustomplot
    )
    # 查找 OpenGL 包
    find_package(OpenGL REQUIRED)
    # 链接 OpenGL 和 GLU 库
    target_link_libraries(PlotPerformance PRIVATE
        OpenGL::GL     # opengl32.lib
        OpenGL::GLU    # glu32.lib
    )
endif()
if(ENABLE_QTCHARTS)
    target_compile_definitions(PlotPerformance PUBLIC
        HAVE_QTCHARTS
    )
endif()

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.standard-cmake)
endif()
set_target_properties(PlotPerformance PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)

include(GNUInstallDirs)
install(TARGETS PlotPerformance
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(PlotPerformance)
endif()
