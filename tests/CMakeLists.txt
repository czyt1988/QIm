cmake_minimum_required(VERSION 3.16)
project(QImTests LANGUAGES CXX)

# Enable Qt Test
find_package(QT NAMES Qt6 Qt5 COMPONENTS Test REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Test REQUIRED)

# Set test properties
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Add plot3d test subdirectory
add_subdirectory(plot3d)