

add_executable(sampa_service_tests sampa_service_tests.cc)
target_link_libraries(sampa_service_tests PUBLIC gtest_main sampa_service Ersap::ersap)
target_include_directories(sampa_service_tests PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        )

gtest_discover_tests(sampa_service_tests)

