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

# Test 9: Plot Bars Item Node
add_executable(tst_qimplotbarsitemnode tst_qimplotbarsitemnode.cpp)
target_link_libraries(tst_qimplotbarsitemnode PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QIm::Core
)
target_include_directories(tst_qimplotbarsitemnode PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core/plot
)
add_test(NAME tst_qimplotbarsitemnode COMMAND tst_qimplotbarsitemnode)

# Test 10: Plot Scatter Item Node
add_executable(tst_qimplotscatteritemnode tst_qimplotscatteritemnode.cpp)
target_link_libraries(tst_qimplotscatteritemnode PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QIm::Core
)
target_include_directories(tst_qimplotscatteritemnode PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core/plot
)
add_test(NAME tst_qimplotscatteritemnode COMMAND tst_qimplotscatteritemnode)

# Test 11: Plot Stairs, Shaded, Stems
add_executable(tst_qimplotstairs_shaded_stems tst_qimplotstairs_shaded_stems.cpp)
target_link_libraries(tst_qimplotstairs_shaded_stems PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QIm::Core
)
target_include_directories(tst_qimplotstairs_shaded_stems PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core/plot
)
add_test(NAME tst_qimplotstairs_shaded_stems COMMAND tst_qimplotstairs_shaded_stems)

# Test 12: Plot Error Bars Item Node
add_executable(tst_qimploterrorbarsitemnode tst_qimploterrorbarsitemnode.cpp)
target_link_libraries(tst_qimploterrorbarsitemnode PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QIm::Core
)
target_include_directories(tst_qimploterrorbarsitemnode PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core/plot
)
add_test(NAME tst_qimploterrorbarsitemnode COMMAND tst_qimploterrorbarsitemnode)

# Test 13: Plot Inf Lines + Dummy
add_executable(tst_qimplotinflines_dummy tst_qimplotinflines_dummy.cpp)
target_link_libraries(tst_qimplotinflines_dummy PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QIm::Core
)
target_include_directories(tst_qimplotinflines_dummy PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core/plot
)
add_test(NAME tst_qimplotinflines_dummy COMMAND tst_qimplotinflines_dummy)

# Test 14: Plot Pie Chart Item Node
add_executable(tst_qimplotpiechartitemnode tst_qimplotpiechartitemnode.cpp)
target_link_libraries(tst_qimplotpiechartitemnode PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QIm::Core
)
target_include_directories(tst_qimplotpiechartitemnode PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core/plot
)
add_test(NAME tst_qimplotpiechartitemnode COMMAND tst_qimplotpiechartitemnode)

# Test 15: Plot Text Item Node
add_executable(tst_qimplottextitemnode tst_qimplottextitemnode.cpp)
target_link_libraries(tst_qimplottextitemnode PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QIm::Core
)
target_include_directories(tst_qimplottextitemnode PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core/plot
)
add_test(NAME tst_qimplottextitemnode COMMAND tst_qimplottextitemnode)

# Test 16: Plot Histogram Item Node
add_executable(tst_qimplothistogramitemnode tst_qimplothistogramitemnode.cpp)
target_link_libraries(tst_qimplothistogramitemnode PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QIm::Core
)
target_include_directories(tst_qimplothistogramitemnode PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core/plot
)
add_test(NAME tst_qimplothistogramitemnode COMMAND tst_qimplothistogramitemnode)

# Test 17: Plot Heatmap Item Node
add_executable(tst_qimplotheatmapitemnode tst_qimplotheatmapitemnode.cpp)
target_link_libraries(tst_qimplotheatmapitemnode PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QIm::Core
)
target_include_directories(tst_qimplotheatmapitemnode PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core/plot
)
add_test(NAME tst_qimplotheatmapitemnode COMMAND tst_qimplotheatmapitemnode)

# Test 18: Plot Histogram2D + Digital
add_executable(tst_qimplot_histogram2d_digital tst_qimplot_histogram2d_digital.cpp)
target_link_libraries(tst_qimplot_histogram2d_digital PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QIm::Core
)
target_include_directories(tst_qimplot_histogram2d_digital PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core/plot
)
add_test(NAME tst_qimplot_histogram2d_digital COMMAND tst_qimplot_histogram2d_digital)

# Test 19: Plot Image + Bar Groups
add_executable(tst_qimplot_image_bargroups tst_qimplot_image_bargroups.cpp)
target_link_libraries(tst_qimplot_image_bargroups PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QIm::Core
)
target_include_directories(tst_qimplot_image_bargroups PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core/plot
)
add_test(NAME tst_qimplot_image_bargroups COMMAND tst_qimplot_image_bargroups)

# Test 20: Plot Drag Tools
add_executable(tst_qimplot_drag_tools tst_qimplot_drag_tools.cpp)
target_link_libraries(tst_qimplot_drag_tools PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QIm::Core
)
target_include_directories(tst_qimplot_drag_tools PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core/plot
)
add_test(NAME tst_qimplot_drag_tools COMMAND tst_qimplot_drag_tools)

# Test 21: Plot Rect Annotation
add_executable(tst_qimplot_rect_annotation tst_qimplot_rect_annotation.cpp)
target_link_libraries(tst_qimplot_rect_annotation PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QIm::Core
)
target_include_directories(tst_qimplot_rect_annotation PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core/plot
)
add_test(NAME tst_qimplot_rect_annotation COMMAND tst_qimplot_rect_annotation)

# Test 22: Plot Tag X/Y
add_executable(tst_qimplot_tagxy tst_qimplot_tagxy.cpp)
target_link_libraries(tst_qimplot_tagxy PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QIm::Core
)
target_include_directories(tst_qimplot_tagxy PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core/plot
)
add_test(NAME tst_qimplot_tagxy COMMAND tst_qimplot_tagxy)

# Test 23: Plot Tracker Selection
add_executable(tst_qimplot_tracker_selection tst_qimplot_tracker_selection.cpp)
target_link_libraries(tst_qimplot_tracker_selection PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QIm::Core
)
target_include_directories(tst_qimplot_tracker_selection PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core/plot
)
add_test(NAME tst_qimplot_tracker_selection COMMAND tst_qimplot_tracker_selection)

# Test 24: Plot Legend Node
add_executable(tst_qimplotlegendnode tst_qimplotlegendnode.cpp)
target_link_libraries(tst_qimplotlegendnode PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QIm::Core
)
target_include_directories(tst_qimplotlegendnode PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core/plot
)
add_test(NAME tst_qimplotlegendnode COMMAND tst_qimplotlegendnode)

# Test 24: Plot Line Item Node
add_executable(tst_qimplotlineitemnode tst_qimplotlineitemnode.cpp)
target_link_libraries(tst_qimplotlineitemnode PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QIm::Core
)
target_include_directories(tst_qimplotlineitemnode PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core/plot
)
add_test(NAME tst_qimplotlineitemnode COMMAND tst_qimplotlineitemnode)
