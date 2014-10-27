project(plasma-parley)
add_subdirectory( engine )

# include_directories( ${LIBKDEEDU_INCLUDE_DIR} )
set(parley_plasma_SRCS
    parley_plasma.cpp
    )

qt5_wrap_ui(parley_plasma_SRCS config.ui)

add_library(plasma_applet_parley ${parley_plasma_SRCS})
target_link_libraries(plasma_applet_parley
  Qt5::Widgets
  KF5::Completion
  KF5::CoreAddons    #for KRandomSequence
  KF5::Plasma
  # KF5::KIOCore
  #    KF5::KIOWidgets
  LibKdeEduImport__KEduVocDocument
  #todo frameworks remove KDELibs4Support
  KF5::KDELibs4Support
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
    DESTINATION ${DATA_INSTALL_DIR}/desktoptheme/default/widgets/
    )
