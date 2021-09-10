# Create one shared library per service
#set(DEMO_SERVICES
#)
#
#foreach(service ${DEMO_SERVICES})
#  add_library(${service} SHARED ${service}.cpp)
#  target_link_libraries(${service} sampa_data Ersap::ersap)
#  if (CMAKE_CXX_COMPILER_ID MATCHES Clang)
#    target_compile_options(${service} PRIVATE -Wno-return-type-c-linkage)
#  endif()
#endforeach()
#
#install(TARGETS ${DEMO_SERVICES} DESTINATION ${CMAKE_INSTALL_PLUGINDIR}/jana/services)


set(SAMPA_SERVICE_SOURCES
        ersap_event_source.cpp
        ersap_event_source.hpp
        group_event_processor.cpp
        group_event_processor.hpp
        ersap_event_group.hpp
        sampa_service.cpp
        sampa_service.hpp
        )

add_library(sampa_service SHARED ${SAMPA_SERVICE_SOURCES})
target_include_directories(sampa_service PUBLIC ${JANA_INCLUDE_DIRS}
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        )
target_link_libraries(sampa_service sampa_data Ersap::ersap ${JANA_LIBRARIES})
if (CMAKE_CXX_COMPILER_ID MATCHES Clang)
  target_compile_options(sampa_service PRIVATE -Wno-return-type-c-linkage)
endif()
install(TARGETS sampa_service DESTINATION ${CMAKE_INSTALL_PLUGINDIR}/jana/services)
