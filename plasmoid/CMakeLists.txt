project(plasma-parley)

include_directories( ${CMAKE_SOURCE_DIR}/libkdeedu/keduvocdocument/   )
set(parley_plasma_SRCS parley_plasma.cpp)
kde4_add_ui_files(parley_plasma_SRCS config.ui)

kde4_add_plugin(plasma_applet_parley ${parley_plasma_SRCS})
target_link_libraries(plasma_applet_parley
            plasma ${KDE4_KIO_LIBS}
            keduvocdocument
            )
install(
    TARGETS plasma_applet_parley
    DESTINATION ${PLUGIN_INSTALL_DIR}
    )

install(
    FILES plasma_parley.desktop
    DESTINATION ${SERVICES_INSTALL_DIR}
    )

install(
    FILES parley_plasma_card.svg
    DESTINATION ${DATA_INSTALL_DIR}/desktoptheme/default/widgets/)
