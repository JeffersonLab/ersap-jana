# Create one shared library per service
set(JANA_SERVICES
        jana_service jana_engine
        )

foreach(service ${JANA_SERVICES})
    add_library(${service} SHARED ${service}.cpp)
    target_link_libraries(${service}  Ersap::ersap)
    if (CMAKE_CXX_COMPILER_ID MATCHES Clang)
        target_compile_options(${service} PRIVATE -Wno-return-type-c-linkage)
    endif()
endforeach()

install(TARGETS ${JANA_SERVICES} DESTINATION ${CMAKE_INSTALL_PLUGINDIR}/jana/services)
