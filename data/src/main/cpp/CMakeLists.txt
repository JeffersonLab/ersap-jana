set(DEMO_DATA_SRC
  image_data_type.cpp
)

add_library(demo_data SHARED ${DEMO_DATA_SRC})
target_link_libraries(demo_data PUBLIC ersap_jana Ersap::ersap ${OpenCV_LIBS})
target_include_directories(demo_data PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

install(TARGETS demo_data DESTINATION ${CMAKE_INSTALL_PLUGINDIR}/jana/lib)
