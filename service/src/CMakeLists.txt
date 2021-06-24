# Create one shared library per service
set(JANA_SERVICES
        jana_service
        )

foreach(service ${JANA_SERVICES})
    add_library(${service} SHARED ${service}.cpp)
    target_link_libraries(${service}  Clara::clara)
#    target_link_libraries(${service} jana_data jana_engine Clara::clara)
    if (CMAKE_CXX_COMPILER_ID MATCHES Clang)
        target_compile_options(${service} PRIVATE -Wno-return-type-c-linkage)
    endif()
endforeach()

install(TARGETS ${JANA_SERVICES} DESTINATION ${CMAKE_INSTALL_PLUGINDIR}/jana/service)
