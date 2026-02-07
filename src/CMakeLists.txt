# ================================================================
# QIM - 源目录配置
# ================================================================

cmake_minimum_required(VERSION 3.15)
# 设置输出目录
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(QIM_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
# 定义库类型
if(QIM_BUILD_SHARED)
    set(LIB_TYPE SHARED)
    message(STATUS "Building QIM as shared libraries")
else()
    set(LIB_TYPE STATIC)
    message(STATUS "Building QIM as static libraries")
endif()

add_subdirectory(core)
if(QIM_BUILD_WIDGETS)
    add_subdirectory(widgets)
endif()
