# ================================================================
# 窗口库 (QIM Widgets)
# ================================================================
message(STATUS "Building QIM Widgets compatibility layer")
set(QIMWIDGETS_VERSION_MAJOR 0)
set(QIMWIDGETS_VERSION_MINOR 0)
set(QIMWIDGETS_VERSION_PATCH 1)
set(QIMWIDGETS_VERSION "${QIMWIDGETS_VERSION_MAJOR}.${QIMWIDGETS_VERSION_MINOR}.${QIMWIDGETS_VERSION_PATCH}")
set(QIMWIDGETS_PROJECT_NAME "Widgets")

add_library(${QIMWIDGETS_PROJECT_NAME} ${LIB_TYPE})
# 创建别名
add_library(${QIM_PROJECT_NAME}::Widgets ALIAS ${QIMWIDGETS_PROJECT_NAME})
# 设置版本和输出属性
set_target_properties(${QIMWIDGETS_PROJECT_NAME} PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
    VERSION ${QIMWIDGETS_VERSION}
    SOVERSION ${QIMWIDGETS_VERSION_MAJOR}
    OUTPUT_NAME ${QIMWIDGETS_PROJECT_NAME}
    DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}"
    RELEASE_POSTFIX "${CMAKE_RELEASE_POSTFIX}"
    RELWITHDEBINFO_POSTFIX "${CMAKE_RELWITHDEBINFO_POSTFIX}"
    MINSIZEREL_POSTFIX "${CMAKE_MINSIZEREL_POSTFIX}"
)

################################################################

# 收集Widgets源文件
set(QIMWIDGETS_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB_RECURSE QIMWIDGETS_SOURCES
    "${QIMWIDGETS_SRC_DIR}/*.cpp"
)

file(GLOB_RECURSE QIMWIDGETS_HEADERS
    "${QIMWIDGETS_SRC_DIR}/*.h"
)


# 添加源文件
target_sources(${QIMWIDGETS_PROJECT_NAME} PRIVATE
    ${QIMWIDGETS_SOURCES}
    ${QIMWIDGETS_HEADERS}
    ${QIMWIDGETS_SRC_DIR}/../QImAPI.h
)

# 包含目录
target_include_directories(${QIMWIDGETS_PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${QIM_SRC_DIR}>
    $<BUILD_INTERFACE:${QIMWIDGETS_SRC_DIR}>
    $<INSTALL_INTERFACE:include/${QIM_PROJECT_NAME}/widgets>
)

# 链接依赖
target_link_libraries(${QIMWIDGETS_PROJECT_NAME} PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::OpenGL
    QIm::Core
)
if(${QT_VERSION_MAJOR} EQUAL 6)
    find_package(Qt6 COMPONENTS OpenGLWidgets REQUIRED)
    target_link_libraries(${QIMWIDGETS_PROJECT_NAME} PRIVATE
        Qt6::OpenGLWidgets
    )
endif()

# 预定义宏
if(QIM_BUILD_SHARED)
    target_compile_definitions(${QIMWIDGETS_PROJECT_NAME} PRIVATE
        QIM_WIDGETS_BUILDLIB
    )
    target_compile_definitions(${QIMWIDGETS_PROJECT_NAME} PUBLIC
        QIM_WIDGETS_DLL
    )
endif()
if(QIM_ENABLE_DEBUG_PRINT_FPS)
    target_compile_definitions(${QIMWIDGETS_PROJECT_NAME} PRIVATE
        QIM_ENABLE_DEBUG_PRINT_FPS
    )
endif()
# ================================================================
# 安装配置
# ================================================================

include(GNUInstallDirs)
# 添加QImWidgets到导出集
install(TARGETS ${QIMWIDGETS_PROJECT_NAME}   # Widgets
    EXPORT ${QIM_TARGET_NAME}           # 统一导出集QImTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# 安装头文件
install(FILES ${QIMWIDGETS_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${QIM_PROJECT_NAME}/widgets
    COMPONENT headers
)

# 安装第三方库头文件
install(FILES
    ${QIM_QTIMGUI_HEADERS}
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${QIM_PROJECT_NAME}/qtimgui"
    COMPONENT headers
)
