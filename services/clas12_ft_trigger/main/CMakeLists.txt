
set(FT_TRIGGER_SERVICE_SOURCES
        ersap_event_source.hpp
        ersap_event_processor.hpp
        ersap_event_group.hpp
        ft_trigger_service.cpp
        ft_trigger_service.hpp
        )

add_library(clas12_ft_trigger_service SHARED ${FT_TRIGGER_SERVICE_SOURCES})
target_include_directories(clas12_ft_trigger_service PUBLIC ${JANA_INCLUDE_DIRS}
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        )
target_link_libraries(clas12_ft_trigger_service Ersap::ersap ${JANA_LIBRARIES})
if (CMAKE_CXX_COMPILER_ID MATCHES Clang)
    target_compile_options(clas12_ft_trigger_service PRIVATE -Wno-return-type-c-linkage)
endif()
install(TARGETS clas12_ft_trigger_service DESTINATION ${CMAKE_INSTALL_PLUGINDIR}/jana/services)
