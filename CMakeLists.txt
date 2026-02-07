cmake_minimum_required(VERSION 3.15)
########################################################
# version
########################################################
set(QIM_VERSION_MAJOR 0)
set(QIM_VERSION_MINOR 0)
set(QIM_VERSION_PATCH 1)
set(QIM_VERSION "${QIM_VERSION_MAJOR}.${QIM_VERSION_MINOR}.${QIM_VERSION_PATCH}")
string(TIMESTAMP QIM_COMPILE_DATETIME %y%m%d)
string(TIMESTAMP QIM_COMPILE_DATETIME_YEAR %y)
string(TIMESTAMP QIM_COMPILE_DATETIME_MONTH %m)
string(TIMESTAMP QIM_COMPILE_DATETIME_DAY %d)
message(STATUS "QIM Version is ${QIM_VERSION}")

########################################################
# project
########################################################
set(QIM_PROJECT_NAME "QIm")
set(QIM_TARGET_NAME "QImTargets") # 这是所有模块统一的导出集
project(${QIM_PROJECT_NAME}
        VERSION ${QIM_VERSION}
        LANGUAGES CXX
        DESCRIPTION "QIm : ImGui ,ImPlot and ImPlot3D libraries wrapped in Qt"
        )

########################################################
# option
########################################################
option(QIM_BUILD_EXAMPLES "Build examples" ON)
option(QIM_BUILD_SHARED "Build as shared library" ON)
option(QIM_BUILD_WIDGETS "Build Qt Widgets compatibility layer" ON)
option(QIM_ENABLE_DEBUG_PRINT "Enable Debug Print" ON)
option(QIM_ENABLE_DEBUG_PRINT_FPS "Enable Debug Print fps" ON)
option(QIM_BUILD_QML "Build QML compatibility layer" OFF) # TODO:QML


# postfix
set(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "add a postfix, usually d on windows")
set(CMAKE_RELEASE_POSTFIX "" CACHE STRING "add a postfix, usually empty on windows")
set(CMAKE_RELWITHDEBINFO_POSTFIX "" CACHE STRING "add a postfix, usually empty on windows")
set(CMAKE_MINSIZEREL_POSTFIX "" CACHE STRING "add a postfix, usually empty on windows")

# c++ stadnard 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif(MSVC)

set(CMAKE_AUTOMOC ON)
# msvc
if(MSVC)
# msvc utf-8
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
    # make sure __cplusplus is defined when using msvc and enable parallel build
    string(APPEND CMAKE_CXX_FLAGS " /Zc:__cplusplus /MP")
    # use response file for object files
    set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_OBJECTS 1)
    set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_INCLUDES 1)
    set(CMAKE_CXX_RESPONSE_FILE_PREFIX "@")
    set(CMAKE_CXX_RESPONSE_FILE_EXTENSION ".rsp")
endif()

if(CMAKE_GENERATOR STREQUAL "Ninja")
    # ninja
    set(CMAKE_NINJA_FORCE_RESPONSE_FILE "1")
endif()

########################################################
# export macro
########################################################
########################################################
# 导出宏配置（支持MSVC和MinGW）
########################################################
if(WIN32 AND QIM_BUILD_SHARED)
    # Windows下构建动态库需要的导出宏
    if(MSVC)
        # MSVC编译器
        set(QIM_LIB_EXPORT_MACRO "__declspec(dllexport)")
    else()
        # MinGW/GCC编译器
        set(QIM_LIB_EXPORT_MACRO "__attribute__((visibility(\"default\")))")
    endif()
    message(STATUS "Setting export macros for Windows shared library build")
elseif(QIM_BUILD_SHARED)
    # Linux/macOS构建动态库
    set(QIM_LIB_EXPORT_MACRO "__attribute__((visibility(\"default\")))")
else()
    # 静态库或非Windows平台
    set(QIM_LIB_EXPORT_MACRO "")
endif()

########################################################
# Qt Setting
########################################################
set(QIM_MIN_QT_VERSION 5.14)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)

message(STATUS "Found Qt${QT_VERSION_MAJOR} version ${Qt${QT_VERSION_MAJOR}_VERSION}")

find_package(Qt${QT_VERSION_MAJOR} ${QIM_MIN_QT_VERSION} REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    OpenGL
)

# 根据选项添加可选组件
if(QIM_BUILD_QML)
    find_package(Qt${QT_VERSION_MAJOR} ${QIM_MIN_QT_VERSION} REQUIRED COMPONENTS
        Quick
        QuickWidgets
    )
endif()
########################################################
# path setting
########################################################
set(QIM_3RDPARTY_DIR ${CMAKE_CURRENT_LIST_DIR}/3rdparty)
set(QIM_IMGUI_DIR ${QIM_3RDPARTY_DIR}/imgui)
set(QIM_IMPLOT_DIR ${QIM_3RDPARTY_DIR}/implot)
set(QIM_IMPLOT3D_DIR ${QIM_3RDPARTY_DIR}/implot3d)
set(QIM_QTIMGUI_DIR ${QIM_3RDPARTY_DIR}/qtimgui/src)

########################################################
# 第三方库
########################################################

########################################################
# imgui files
########################################################
# ImGui 依赖
set(QIM_IMGUI_HEADERS
    ${QIM_IMGUI_DIR}/imconfig.h
    ${QIM_IMGUI_DIR}/imgui.h
    ${QIM_IMGUI_DIR}/imgui_internal.h
    ${QIM_IMGUI_DIR}/imstb_truetype.h
    ${QIM_IMGUI_DIR}/imstb_textedit.h
    ${QIM_IMGUI_DIR}/imstb_rectpack.h
)
set(QIM_IMGUI_SOURCES
    ${QIM_IMGUI_DIR}/imgui.cpp
    ${QIM_IMGUI_DIR}/imgui_demo.cpp
    ${QIM_IMGUI_DIR}/imgui_draw.cpp
    ${QIM_IMGUI_DIR}/imgui_tables.cpp
    ${QIM_IMGUI_DIR}/imgui_widgets.cpp
)


########################################################
# implot files
########################################################
set(QIM_IMPLOT_HEADERS
    ${QIM_IMPLOT_DIR}/implot.h
    ${QIM_IMPLOT_DIR}/implot_internal.h
)
set(QIM_IMPLOT_SOURCES
    ${QIM_IMPLOT_DIR}/implot.cpp
    ${QIM_IMPLOT_DIR}/implot_items.cpp
    ${QIM_IMPLOT_DIR}/implot_demo.cpp
)


########################################################
# implot3d files
########################################################

set(QIM_IMPLOT3D_HEADERS
    ${QIM_IMPLOT3D_DIR}/implot3d.h
    ${QIM_IMPLOT3D_DIR}/implot3d_internal.h
)
set(QIM_IMPLOT3D_SOURCES
    ${QIM_IMPLOT3D_DIR}/implot3d.cpp
    ${QIM_IMPLOT3D_DIR}/implot3d_demo.cpp
    ${QIM_IMPLOT3D_DIR}/implot3d_items.cpp
    ${QIM_IMPLOT3D_DIR}/implot3d_meshes.cpp
)


########################################################
# qtimgui files
########################################################

set(QIM_QTIMGUI_HEADERS
    ${QIM_QTIMGUI_DIR}/QtImGui.h
    ${QIM_QTIMGUI_DIR}/ImGuiRenderer.h
)
set(QIM_QTIMGUI_SOURCES
    ${QIM_QTIMGUI_DIR}/QtImGui.cpp
    ${QIM_QTIMGUI_DIR}/ImGuiRenderer.cpp
)
########################################################
# source
########################################################
add_subdirectory(src)
if(QIM_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

########################################################
# 总安装
########################################################
include(GNUInstallDirs)
# 生成统一的 QImConfig.cmake
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
# 统一导出所有模块的 target
install(EXPORT ${QIM_TARGET_NAME}
    FILE ${QIM_TARGET_NAME}.cmake
    NAMESPACE ${QIM_PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${QIM_PROJECT_NAME}
)
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)


# ===== 调试：打印所有 targets =====
message(STATUS "")
message(STATUS "==========================================")
message(STATUS "  Project: ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "  CMake: ${CMAKE_VERSION}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "==========================================")

# 获取所有 targets
get_property(all_targets DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY BUILDSYSTEM_TARGETS)
list(SORT all_targets)  # 排序便于阅读

message(STATUS "Targets (${CMAKE_PROJECT_NAME}):")
foreach(target IN LISTS all_targets)
    if(TARGET ${target})
        get_target_property(type ${target} TYPE)
        get_target_property(aliased ${target} ALIASED_TARGET)
        if(aliased AND NOT aliased STREQUAL "ALIASED_TARGET-NOTFOUND")
            message(STATUS "  → ${target} (ALIAS of ${aliased})")
        else()
            message(STATUS "  • ${target} [${type}]")
        endif()
    endif()
endforeach()
message(STATUS "==========================================")
message(STATUS "")
