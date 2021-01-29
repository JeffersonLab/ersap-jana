INCLUDE(ExternalProject)
SET_DIRECTORY_PROPERTIES(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/ThirdParty)

ExternalProject_Add(
  googletest
  URL https://github.com/google/googletest/archive/release-1.8.1.zip
  URL_HASH SHA256=927827c183d01734cc5cfef85e0ff3f5a92ffe6188e0d18e909c5efebf28a0c7
  TIMEOUT 60
  INSTALL_COMMAND ""
  LOG_DOWNLOAD ON
  LOG_CONFIGURE ON
  LOG_BUILD ON
)

find_package(Threads QUIET)

ExternalProject_Get_Property(googletest SOURCE_DIR)
ExternalProject_Get_Property(googletest BINARY_DIR)

# avoid error with set_target_properties
file(MAKE_DIRECTORY "${SOURCE_DIR}/googlemock/include")
file(MAKE_DIRECTORY "${SOURCE_DIR}/googletest/include")

add_library(GTest::GTest IMPORTED STATIC IMPORTED)
add_library(GTest::GMock IMPORTED STATIC IMPORTED)

set_target_properties(GTest::GTest PROPERTIES
  IMPORTED_LOCATION
    "${BINARY_DIR}/googlemock/gtest/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_STATIC_LIBRARY_SUFFIX}"
  INTERFACE_INCLUDE_DIRECTORIES
    "${SOURCE_DIR}/googletest/include"
  IMPORTED_LINK_INTERFACE_LIBRARIES
    Threads::Threads
)

set_target_properties(GTest::GMock PROPERTIES
  IMPORTED_LOCATION
    "${BINARY_DIR}/googlemock/${CMAKE_FIND_LIBRARY_PREFIXES}gmock${CMAKE_STATIC_LIBRARY_SUFFIX}"
  INTERFACE_INCLUDE_DIRECTORIES
    "${SOURCE_DIR}/googlemock/include"
  IMPORTED_LINK_INTERFACE_LIBRARIES
    GTest::GTest
)

add_dependencies(GTest::GMock googletest)
