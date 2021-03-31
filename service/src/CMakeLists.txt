
set(jana_service_SOURCES
        clara_event_source.cpp
        clara_event_source.hpp
        group_event_processor.cpp
        group_event_processor.hpp
        jana_service.cpp
        jana_service.hpp
        sampa_event.hpp
        )

add_library(jana_service SHARED ${jana_service_SOURCES})
target_include_directories(jana_service PRIVATE ${JANA_INCLUDE_DIRS})
target_link_libraries(jana_service Clara::clara ${JANA_LIBRARIES})
if (CMAKE_CXX_COMPILER_ID MATCHES Clang)
    target_compile_options(jana_service PRIVATE -Wno-return-type-c-linkage)
endif()

