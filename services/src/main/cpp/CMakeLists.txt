# Create one shared library per service
set(DEMO_SERVICES
  # jana_service    # Turned off because this wasn't being useful anyway
)

foreach(service ${DEMO_SERVICES})
  add_library(${service} SHARED ${service}.cpp)
  target_link_libraries(${service} sampa_data Ersap::ersap)
  if (CMAKE_CXX_COMPILER_ID MATCHES Clang)
    target_compile_options(${service} PRIVATE -Wno-return-type-c-linkage)
  endif()
endforeach()

install(TARGETS ${DEMO_SERVICES} DESTINATION ${CMAKE_INSTALL_PLUGINDIR}/jana/services)


# Do something different for sampa_service, at least for now
set(SAMPA_SERVICE_SOURCES
        ersap_event_source.cpp
        ersap_event_source.hpp
        group_event_processor.cpp
        group_event_processor.hpp
        sampa_service.cpp
        sampa_service.hpp
        )

add_library(sampa_service SHARED ${SAMPA_SERVICE_SOURCES})
target_include_directories(sampa_service PRIVATE ${JANA_INCLUDE_DIRS})
target_link_libraries(sampa_service sampa_data Ersap::ersap ${JANA_LIBRARIES})
if (CMAKE_CXX_COMPILER_ID MATCHES Clang)
  target_compile_options(sampa_service PRIVATE -Wno-return-type-c-linkage)
endif()
install(TARGETS sampa_service DESTINATION ${CMAKE_INSTALL_PLUGINDIR}/jana/services)

