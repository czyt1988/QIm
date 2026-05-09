# Plot3D test targets
set(CMAKE_AUTOMOC ON)

# Test 1: Enums
add_executable(tst_plot3d_enums tst_plot3d_enums.cpp)
target_link_libraries(tst_plot3d_enums PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QIm::Core
)
add_test(NAME tst_plot3d_enums COMMAND tst_plot3d_enums)

# Test 2: Math types
add_executable(tst_plot3d_math_types tst_plot3d_math_types.cpp)
target_link_libraries(tst_plot3d_math_types PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QIm::Core
)
add_test(NAME tst_plot3d_math_types COMMAND tst_plot3d_math_types)

# Test 3: Conversions
add_executable(tst_plot3d_conversions tst_plot3d_conversions.cpp)
target_link_libraries(tst_plot3d_conversions PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QIm::Core
)
add_test(NAME tst_plot3d_conversions COMMAND tst_plot3d_conversions)

# Test 4: Setup API
add_executable(tst_plot3d_setup_api tst_plot3d_setup_api.cpp)
target_link_libraries(tst_plot3d_setup_api PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QIm::Core
)
add_test(NAME tst_plot3d_setup_api COMMAND tst_plot3d_setup_api)

# Test 5: Styles
add_executable(tst_plot3d_styles tst_plot3d_styles.cpp)
target_link_libraries(tst_plot3d_styles PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QIm::Core
)
add_test(NAME tst_plot3d_styles COMMAND tst_plot3d_styles)

# Test 6: Colormaps
add_executable(tst_plot3d_colormaps tst_plot3d_colormaps.cpp)
target_link_libraries(tst_plot3d_colormaps PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QIm::Core
)
add_test(NAME tst_plot3d_colormaps COMMAND tst_plot3d_colormaps)

# Test 7: Utilities
add_executable(tst_plot3d_utilities tst_plot3d_utilities.cpp)
target_link_libraries(tst_plot3d_utilities PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QIm::Core
)
add_test(NAME tst_plot3d_utilities COMMAND tst_plot3d_utilities)

# Test 8: Data Series 3D
add_executable(tst_data_series3d tst_data_series3d.cpp)
target_link_libraries(tst_data_series3d PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QIm::Core
)
target_include_directories(tst_data_series3d PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core/plot3d
)
add_test(NAME tst_data_series3d COMMAND tst_data_series3d)

# Test 9: Axis Info
add_executable(tst_axis_info tst_axis_info.cpp)
target_link_libraries(tst_axis_info PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QIm::Core
)
target_include_directories(tst_axis_info PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core/plot3d
)
add_test(NAME tst_axis_info COMMAND tst_axis_info)

# Test 10: Axis Formatter
add_executable(tst_axis_formatter tst_axis_formatter.cpp)
target_link_libraries(tst_axis_formatter PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QIm::Core
)
target_include_directories(tst_axis_formatter PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core/plot3d
)
add_test(NAME tst_axis_formatter COMMAND tst_axis_formatter)

# Test 11: Colormap Manager
add_executable(tst_colormap_manager tst_colormap_manager.cpp)
target_link_libraries(tst_colormap_manager PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QIm::Core
)
target_include_directories(tst_colormap_manager PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core/plot3d
)
add_test(NAME tst_colormap_manager COMMAND tst_colormap_manager)

# Test 12: Types
add_executable(tst_types tst_types.cpp)
target_link_libraries(tst_types PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QIm::Core
)
target_include_directories(tst_types PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core/plot3d
)
add_test(NAME tst_types COMMAND tst_types)

# Test 13: Plot3D Node
add_executable(tst_plot3d_node tst_plot3d_node.cpp)
target_link_libraries(tst_plot3d_node PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QIm::Core
)
target_include_directories(tst_plot3d_node PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core/plot3d
)
add_test(NAME tst_plot3d_node COMMAND tst_plot3d_node)

# Test 14: Plot3D Item Node
add_executable(tst_plot3d_item_node tst_plot3d_item_node.cpp)
target_link_libraries(tst_plot3d_item_node PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QIm::Core
)
target_include_directories(tst_plot3d_item_node PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core/plot3d
)
add_test(NAME tst_plot3d_item_node COMMAND tst_plot3d_item_node)

# Test 15: Plot3D Style Node
add_executable(tst_plot3d_style_node tst_plot3d_style_node.cpp)
target_link_libraries(tst_plot3d_style_node PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QIm::Core
)
target_include_directories(tst_plot3d_style_node PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core/plot3d
)
add_test(NAME tst_plot3d_style_node COMMAND tst_plot3d_style_node)

# Test 16: Subplots 3D Node
add_executable(tst_subplots3d_node tst_subplots3d_node.cpp)
target_link_libraries(tst_subplots3d_node PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QIm::Core
)
target_include_directories(tst_subplots3d_node PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core/plot3d
)
add_test(NAME tst_subplots3d_node COMMAND tst_subplots3d_node)
