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
