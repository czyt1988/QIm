# Core test targets
set(CMAKE_AUTOMOC ON)

# Test 1: TrackedValue
add_executable(tst_tracked_value tst_tracked_value.cpp)
target_link_libraries(tst_tracked_value PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QIm::Core
)
add_test(NAME tst_tracked_value COMMAND tst_tracked_value)

# Test 2: QtImGui Utils
add_executable(tst_qtimgui_utils tst_qtimgui_utils.cpp)
target_link_libraries(tst_qtimgui_utils PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QIm::Core
)
add_test(NAME tst_qtimgui_utils COMMAND tst_qtimgui_utils)

# Test 3: Font File Helper (uses QGuiApplication, needs Qt::Gui)
add_executable(tst_font_file_helper tst_font_file_helper.cpp)
target_link_libraries(tst_font_file_helper PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    Qt${QT_VERSION_MAJOR}::Gui
    QIm::Core
)
add_test(NAME tst_font_file_helper COMMAND tst_font_file_helper)

# Test 4: Abstract Node
add_executable(tst_abstract_node tst_abstract_node.cpp)
target_link_libraries(tst_abstract_node PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QIm::Core
)
add_test(NAME tst_abstract_node COMMAND tst_abstract_node)

# Test 5: Grid Node
add_executable(tst_grid_node tst_grid_node.cpp)
target_link_libraries(tst_grid_node PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QIm::Core
)
add_test(NAME tst_grid_node COMMAND tst_grid_node)
