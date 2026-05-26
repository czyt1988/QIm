cmake_minimum_required(VERSION 3.16)

# Main application target name
set(MAIN_PROJECT_NAME QImSystemMonitor)
# Verification target name (Task 0)
set(VERIFY_PROJECT_NAME QImSystemMonitor_verify)

project(${MAIN_PROJECT_NAME} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    OpenGL
)
if(QT_VERSION_MAJOR EQUAL 6)
    find_package(Qt6 COMPONENTS OpenGLWidgets REQUIRED)
endif()

# =============================================================================
# Main Application: QImSystemMonitor
# =============================================================================
set(MAIN_PROJECT_SOURCES
    main.cpp
    MainWindow.cpp
    collector/ProcessCollector.cpp
    aggregator/ProcessAggregator.cpp
    aggregator/HistoryBuffer.cpp
    core/ProcessMonitor.cpp
    widgets/ProcessTreeWidget.cpp
    widgets/ViewModeSelector.cpp
    views/CpuUsageView.cpp
    views/ResourcePieView.cpp
    views/ProcessResource3DView.cpp
    views/SystemMetricsView.cpp
    aggregator/SustainedMetricsTracker.cpp
    widgets/SustainedMetricSelector.cpp
    views/SustainedMetricsView.cpp
    core/ViewManager.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${MAIN_PROJECT_NAME}
        MANUAL_FINALIZATION
        ${MAIN_PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(${MAIN_PROJECT_NAME} SHARED
            ${MAIN_PROJECT_SOURCES}
        )
    else()
        add_executable(${MAIN_PROJECT_NAME}
            ${MAIN_PROJECT_SOURCES}
        )
    endif()
endif()

target_include_directories(${MAIN_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${MAIN_PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::OpenGL
    QIm::Core
    QIm::Widgets
    psapi
    pdh
)
if(QT_VERSION_MAJOR EQUAL 6)
    target_link_libraries(${MAIN_PROJECT_NAME} PRIVATE
        Qt6::OpenGLWidgets
    )
endif()

# GUI application - WIN32 executable (no console window)
set_target_properties(${MAIN_PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(${QT_VERSION} VERSION_LESS 6.1.0)
    set_target_properties(${MAIN_PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER com.example.${MAIN_PROJECT_NAME}
    )
endif()

include(GNUInstallDirs)
install(TARGETS ${MAIN_PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${MAIN_PROJECT_NAME})
endif()
