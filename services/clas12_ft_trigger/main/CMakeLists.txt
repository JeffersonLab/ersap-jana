
include_directories(${JANA_INCLUDE_DIRS})  # Need this for Cling as well

find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})

# Generate ROOT dictionaries for each of our ROOT object types
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
ROOT_GENERATE_DICTIONARY(G__JObjectDrawable system/JObjectDrawable.h)

set(FT_TRIGGER_SERVICE_SOURCES
        ersap_event_source.hpp
        ersap_event_processor.hpp
        ersap_event_group.hpp
        ft_trigger_service.cpp
        ft_trigger_service.hpp
        InputDataFormat.cpp
        OutputDataFormat.cpp
        FT/FTCalHit.cc
        FT/FTCalHit_factory.cc
        FT/FTCalCluster.cc
        FT/FTCalCluster_factory.cc
        FT/FTCalCluster_factory_hdbscan.cc
        FT/FTCalCluster_factory_km.cc
        FT/FTHodoHit.cc
        FT/FTHodoHit_factory.cc
        FT/hdbscandir/AllMetrics.cc
        FT/hdbscandir/cluster.cc
        FT/hdbscandir/Hdb.cc
        FT/hdbscandir/hdbscan_main.cc
        FT/hdbscandir/hdbscanAlgorithm.cc
        FT/hdbscandir/hdbscanRunner.cc
        FT/hdbscandir/undirectedGraph.cc
        FT/hdbscandir/utils.cc
        FT/kmeans/kmeans.cc
        DAQ/fa250VTPMode7Hit_factory.cc
        DAQ/faWaveboardHit.cc
        DAQ/faWaveboardHit_factory.cc
        system/JObjectDrawable.cc
        TT/TranslationTable.cc
        TT/TranslationTable_factory.cc
        TriggerDecision_FTCalCluster_factory.cc
        TriggerDecision_MinBias_factory.cc
        G__JObjectDrawable
        )

add_library(clas12_ft_trigger_service SHARED ${FT_TRIGGER_SERVICE_SOURCES})
target_include_directories(clas12_ft_trigger_service PUBLIC ${ROOT_INCLUDE_DIRS}
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        )
target_link_libraries(clas12_ft_trigger_service Ersap::ersap ${JANA_LIBRARIES} ${ROOT_LIBRARIES})
if (CMAKE_CXX_COMPILER_ID MATCHES Clang)
    target_compile_options(clas12_ft_trigger_service PRIVATE -Wno-return-type-c-linkage)
endif()
install(TARGETS clas12_ft_trigger_service DESTINATION ${CMAKE_INSTALL_PLUGINDIR}/jana/services)
