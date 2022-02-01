

add_executable(ftcal_trigger_service_tests ftcal_trigger_service_tests.cc)
target_include_directories(ftcal_trigger_service_tests PUBLIC ${JANA_INCLUDE_DIR})
target_link_libraries(ftcal_trigger_service_tests PUBLIC gtest_main ftcal_trigger_service Ersap::ersap)
target_include_directories(ftcal_trigger_service_tests PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        )

gtest_discover_tests(ftcal_trigger_service_tests)

