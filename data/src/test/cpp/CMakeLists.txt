
add_executable(sampa_serializer_tests sampa_serializer_tests.cc)
target_link_libraries(sampa_serializer_tests PUBLIC gtest_main sampa_data Ersap::ersap)
target_include_directories(sampa_serializer_tests PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        )

gtest_discover_tests(sampa_serializer_tests)

