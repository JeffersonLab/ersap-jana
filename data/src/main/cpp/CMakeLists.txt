set(SAMPA_DATA_SRC
        sampa_data_type.cpp
        )

add_library(sampa_data SHARED ${SAMPA_DATA_SRC})
target_link_libraries(sampa_data PUBLIC ersap_jana Ersap::ersap ${OpenCV_LIBS})
target_include_directories(sampa_data PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        )

install(TARGETS sampa_data DESTINATION ${CMAKE_INSTALL_PLUGINDIR}/jana/lib)
