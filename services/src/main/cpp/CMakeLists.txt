# Create one shared library per service
set(DEMO_SERVICES
  jana_service sampa_service
)

foreach(service ${DEMO_SERVICES})
  add_library(${service} SHARED ${service}.cpp)
  target_link_libraries(${service} sampa_data ersap_jana Ersap::ersap)
  if (CMAKE_CXX_COMPILER_ID MATCHES Clang)
    target_compile_options(${service} PRIVATE -Wno-return-type-c-linkage)
  endif()
endforeach()

install(TARGETS ${DEMO_SERVICES} DESTINATION ${CMAKE_INSTALL_PLUGINDIR}/jana/services)
