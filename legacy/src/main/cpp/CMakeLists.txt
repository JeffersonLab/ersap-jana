set(LEGACY_SRC
        jana_engine sampa_engine
)

add_library(ersap_jana SHARED ${LEGACY_SRC})
target_link_libraries(ersap_jana PUBLIC ${OpenCV_LIBS})
target_include_directories(ersap_jana PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

install(TARGETS ersap_jana DESTINATION ${CMAKE_INSTALL_PLUGINDIR}/jana/lib)
