set(parley_engine_SRCS
    parley_engine.cpp
)

include_directories( ${LIBKDEEDU_INCLUDE_DIR} )

kde4_add_plugin(plasma_engine_parley ${parley_engine_SRCS})
target_link_libraries(plasma_engine_parley ${KDE4_KDECORE_LIBS}
  ${KDE4_PLASMA_LIBS}
  ${LIBKDEEDU_KEDUVOCDOCUMENT_LIBRARIES}
)

install(TARGETS plasma_engine_parley DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-dataengine-parley.desktop DESTINATION ${SERVICES_INSTALL_DIR} )

