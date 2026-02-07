# ================================================================
# 核心库 (QIM Core)
# ================================================================
set(QIMCORE_VERSION_MAJOR 0)
set(QIMCORE_VERSION_MINOR 0)
set(QIMCORE_VERSION_PATCH 1)
set(QIMCORE_VERSION "${QIMCORE_VERSION_MAJOR}.${QIMCORE_VERSION_MINOR}.${QIMCORE_VERSION_PATCH}")
set(QIMCORE_PROJECT_NAME "Core")

add_library(${QIMCORE_PROJECT_NAME} ${LIB_TYPE})
# 创建别名
add_library(${QIM_PROJECT_NAME}::Core ALIAS ${QIMCORE_PROJECT_NAME})
# 设置版本和输出属性
set_target_properties(${QIMCORE_PROJECT_NAME} PROPERTIES
    VERSION ${QIMCORE_VERSION}
    SOVERSION ${QIMCORE_VERSION_MAJOR}
    OUTPUT_NAME ${QIMCORE_PROJECT_NAME}
    DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}"
    RELEASE_POSTFIX "${CMAKE_RELEASE_POSTFIX}"
    RELWITHDEBINFO_POSTFIX "${CMAKE_RELWITHDEBINFO_POSTFIX}"
    MINSIZEREL_POSTFIX "${CMAKE_MINSIZEREL_POSTFIX}"
)



# 收集核心源文件
set(QIMCORE_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE QIMCORE_SOURCES
    "${QIMCORE_SRC_DIR}/*.cpp"
)
file(GLOB_RECURSE QIMCORE_HEADERS
    "${QIMCORE_SRC_DIR}/*.h"
    "${QIMCORE_SRC_DIR}/*.hpp"
)
#plot folder
file(GLOB_RECURSE QIMCORE_PLOT_SOURCES
    "${QIMCORE_SRC_DIR}/plot/*.cpp"
)
file(GLOB_RECURSE QIMCORE_PLOT_HEADERS
    "${QIMCORE_SRC_DIR}/plot/*.h"
    "${QIMCORE_SRC_DIR}/plot/*.hpp"
)

# 添加源文件
target_sources(${QIMCORE_PROJECT_NAME} PRIVATE
    ${QIMCORE_SOURCES}
    ${QIMCORE_HEADERS}
    ${QIMCORE_PLOT_SOURCES}
    ${QIMCORE_PLOT_HEADERS}
    ${QIMCORE_SRC_DIR}/../QImAPI.h
    # imgui
    ${QIM_IMGUI_HEADERS}
    ${QIM_IMGUI_SOURCES}
    # implot
    ${QIM_IMPLOT_HEADERS}
    ${QIM_IMPLOT_SOURCES}
    # implot3d
    ${QIM_IMPLOT3D_HEADERS}
    ${QIM_IMPLOT3D_SOURCES}
    #qimgui
    ${QIM_QTIMGUI_HEADERS}
    ${QIM_QTIMGUI_SOURCES}
)

# 包含目录
target_include_directories(${QIMCORE_PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${QIM_SRC_DIR}>
    $<BUILD_INTERFACE:${QIMCORE_SRC_DIR}>
    $<INSTALL_INTERFACE:include/${QIM_PROJECT_NAME}/core>
    # imgui
    $<BUILD_INTERFACE:${QIM_IMGUI_DIR}>
    $<INSTALL_INTERFACE:include/${QIM_PROJECT_NAME}/imgui>
    # implot
    $<BUILD_INTERFACE:${QIM_IMPLOT_DIR}>
    $<INSTALL_INTERFACE:include/${QIM_PROJECT_NAME}/implot>
    # implot3d
    $<BUILD_INTERFACE:${QIM_IMPLOT3D_DIR}>
    $<INSTALL_INTERFACE:include/${QIM_PROJECT_NAME}/implot3d>
    # qimgui
    $<BUILD_INTERFACE:${QIM_QTIMGUI_DIR}>
    $<INSTALL_INTERFACE:include/${QIM_PROJECT_NAME}/qtimgui>
)






# 预定义宏
if(QIM_BUILD_SHARED)
    target_compile_definitions(${QIMCORE_PROJECT_NAME} PRIVATE
        QIM_CORE_BUILDLIB
        QTIMGUI_BUILDLIB # 给qimgui的构建宏
    )
    target_compile_definitions(${QIMCORE_PROJECT_NAME} PUBLIC
        QIM_CORE_DLL
        IMGUI_USER_CONFIG=\"QImAPI.h\"
        QTIMGUI_DLL # qimgui的引入宏
    )

endif()
if(QIM_ENABLE_DEBUG_PRINT)
    # 允许打印调试
    target_compile_definitions(${QIMCORE_PROJECT_NAME} PRIVATE
        QIM_CORE_DEBUG_PRINT
    )
endif()

# 链接依赖
target_link_libraries(${QIMCORE_PROJECT_NAME} PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::OpenGL
)

# ================================================================
# 安装配置
# ================================================================

include(GNUInstallDirs)
set(QIMCORE_INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR}/${QIM_PROJECT_NAME}/core)

# 添加QImPlotCore到导出集
install(TARGETS ${QIMCORE_PROJECT_NAME} # Core
    EXPORT ${QIM_TARGET_NAME}           # 统一导出集QImTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# 安装头文件
install(FILES ${QIMCORE_SRC_DIR}/../QImAPI.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${QIM_PROJECT_NAME}
        COMPONENT headers)
install(FILES ${QIMCORE_HEADERS}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${QIM_PROJECT_NAME}/core
        COMPONENT headers)
install(FILES ${QIMCORE_PLOT_HEADERS}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${QIM_PROJECT_NAME}/core/plot
        COMPONENT headers)

# 安装第三方库头文件
install(FILES
    ${QIM_IMGUI_HEADERS}
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${QIM_PROJECT_NAME}/imgui"
    COMPONENT headers
)
install(FILES
    ${QIM_IMPLOT_HEADERS}
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${QIM_PROJECT_NAME}/implot"
    COMPONENT headers
)
install(FILES
    ${QIM_IMPLOT3D_HEADERS}
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${QIM_PROJECT_NAME}/implot3d"
    COMPONENT headers
)
