set(parley_engine_SRCS
    parley_engine.cpp
)

include_directories( ${LIBKDEEDU_INCLUDE_DIR} )

add_library(plasma_engine_parley ${parley_engine_SRCS})
target_link_libraries(plasma_engine_parley
  KF5::Completion
  KF5::Plasma
  LibKdeEduImport__KEduVocDocument
  #todo frameworks remove KDELibs4Support
  KF5::KDELibs4Support
)

install(TARGETS plasma_engine_parley DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-dataengine-parley.desktop DESTINATION ${SERVICES_INSTALL_DIR} )
