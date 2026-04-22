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