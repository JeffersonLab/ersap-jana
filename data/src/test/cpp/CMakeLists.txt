
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(sampa_serializer_tests sampa_serializer_tests.cc)
target_link_libraries(sampa_serializer_tests PUBLIC gtest_main sampa_data ersap_jana Ersap::ersap)
target_include_directories(sampa_serializer_tests PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        )

include(GoogleTest)
gtest_discover_tests(sampa_serializer_tests)

