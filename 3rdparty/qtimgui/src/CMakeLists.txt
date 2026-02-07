project(qtimgui)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core Quick Gui Widgets OpenGLWidgets REQUIRED)

set(QT_IMGUI_SOURCES
    ImGuiRenderer.cpp
    QtImGui.cpp
)

set(QT_IMGUI_HEADERS
    ImGuiRenderer.h
    QtImGui.h
)

# qt_imgui_quick: library with a qt renderer for Qml / QtQuick applications
add_library(qt_imgui_quick STATIC ${QT_IMGUI_SOURCES})
target_include_directories(qt_imgui_quick PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(
    qt_imgui_quick
    PUBLIC
    imgui
    Qt6::Gui
    Qt6::OpenGLWidgets
    )
if (ANDROID)
    target_link_libraries(qt_imgui_quick PUBLIC log dl GLESv2 z)
endif()
install(TARGETS qt_imgui_quick ARCHIVE DESTINATION qtimgui/lib)
install(FILES ${QT_IMGUI_HEADERS} DESTINATION qtimgui/include)

# qt_imgui_widget: library with a qt renderer for classic Qt Widget applications
add_library(qt_imgui_widgets STATIC ${QT_IMGUI_SOURCES})
target_include_directories(qt_imgui_widgets PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(
    qt_imgui_widgets
    PUBLIC
    imgui
    Qt6::Gui
    Qt6::OpenGLWidgets
    )
if (ANDROID)
    target_link_libraries(qt_imgui_widgets PUBLIC log dl GLESv2 z)
endif()
target_compile_definitions(qt_imgui_widgets PUBLIC QT_WIDGETS_LIB)
install(TARGETS qt_imgui_widgets ARCHIVE DESTINATION qtimgui/lib)
install(FILES ${QT_IMGUI_HEADERS} DESTINATION qtimgui/include)
