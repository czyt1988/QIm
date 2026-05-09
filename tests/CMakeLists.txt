cmake_minimum_required(VERSION 3.16)

# Enable Qt Test (inherits QIM_BUILD_TESTS from parent scope)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Test REQUIRED)

# Set test properties
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Add core test subdirectory
add_subdirectory(core)

# Add plot3d test subdirectory
add_subdirectory(plot3d)

# Add plot test subdirectory
add_subdirectory(plot)