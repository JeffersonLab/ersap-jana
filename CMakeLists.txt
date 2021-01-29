cmake_minimum_required(VERSION 3.5.1)
project(ERSAP_JANA C CXX)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
            Debug Release MinSizeRel RelWithDebInfo)
endif()

if(DEFINED ENV{CLARA_HOME} AND CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    # Provide useful default paths when $CLARA_HOME is defined
    set(CMAKE_INSTALL_PREFIX "$ENV{CLARA_HOME}" CACHE PATH "" FORCE)
    list(APPEND CMAKE_PREFIX_PATH "$ENV{CLARA_HOME}")
    list(APPEND CMAKE_PREFIX_PATH "$ENV{JANA_HOME}/lib/cmake/JANA")
endif()

if(NOT DEFINED CMAKE_INSTALL_PLUGINDIR)
    set(CMAKE_INSTALL_PLUGINDIR plugins CACHE PATH "Clara plugins directory (plugins)")
endif()
mark_as_advanced(CMAKE_INSTALL_PLUGINDIR)

if(POLICY CMP0074)
    cmake_policy(SET CMP0074 NEW)
endif()

option(ERSAP_JANA_BUILD_TESTS "Build tests" ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra)

if(APPLE)
    set(CMAKE_MACOSX_RPATH ON)
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
    set(CMAKE_INSTALL_RPATH @loader_path/../lib)
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)
#list(APPEND CMAKE_MODULE_PATH $ENV{JANA_HOME}/cmake)

find_package(Clara REQUIRED)
find_package(JANA REQUIRED)

#add_subdirectory(engine/src)
#add_subdirectory(data/src)
add_subdirectory(service/src)


#if(ERSAP_JANA_BUILD_TESTS)
#    enable_testing()
#    include(cmake/External/GTest.cmake)
#    add_subdirectory(data/src/test)
#endif(ERSAP_JANA_BUILD_TESTS)
