# Plot test targets
set(CMAKE_AUTOMOC ON)

# Test 1: Downsampler
add_executable(tst_downsampler tst_downsampler.cpp)
target_link_libraries(tst_downsampler PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QIm::Core
)
target_include_directories(tst_downsampler PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core/plot
)
add_test(NAME plot-downsampler COMMAND tst_downsampler)

# Test 2: Plot Axis Info
add_executable(tst_plot_axis_info tst_plot_axis_info.cpp)
target_link_libraries(tst_plot_axis_info PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QIm::Core
)
target_include_directories(tst_plot_axis_info PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core/plot
)
add_test(NAME tst_plot_axis_info COMMAND tst_plot_axis_info)

# Test 3: Downsampling Controller
add_executable(tst_downsampling_controller tst_downsampling_controller.cpp)
target_link_libraries(tst_downsampling_controller PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QIm::Core
)
target_include_directories(tst_downsampling_controller PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core/plot
)
add_test(NAME tst_downsampling_controller COMMAND tst_downsampling_controller)

# Test 4: Data Series
add_executable(tst_data_series tst_data_series.cpp)
target_link_libraries(tst_data_series PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QIm::Core
)
target_include_directories(tst_data_series PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core/plot
)
add_test(NAME tst_data_series COMMAND tst_data_series)

# Test 5: Waveform Generator
add_executable(tst_waveform_generator tst_waveform_generator.cpp)
target_link_libraries(tst_waveform_generator PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QIm::Core
)
target_include_directories(tst_waveform_generator PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core/plot
)
add_test(NAME tst_waveform_generator COMMAND tst_waveform_generator)

# Test 6: Plot Node
add_executable(tst_plot_node tst_plot_node.cpp)
target_link_libraries(tst_plot_node PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QIm::Core
)
target_include_directories(tst_plot_node PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core/plot
)
add_test(NAME tst_plot_node COMMAND tst_plot_node)

# Test 7: Plot Item Node
add_executable(tst_plot_item_node tst_plot_item_node.cpp)
target_link_libraries(tst_plot_item_node PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QIm::Core
)
target_include_directories(tst_plot_item_node PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core/plot
)
add_test(NAME tst_plot_item_node COMMAND tst_plot_item_node)

# Test 8: Subplots Node
add_executable(tst_subplots_node tst_subplots_node.cpp)
target_link_libraries(tst_subplots_node PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QIm::Core
)
target_include_directories(tst_subplots_node PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core/plot
)
add_test(NAME tst_subplots_node COMMAND tst_subplots_node)
