project(plasma-parley)
add_subdirectory( engine )

# include_directories( ${LIBKDEEDU_INCLUDE_DIR} )
set(parley_plasma_SRCS
    parley_plasma.cpp
    )

ki18n_wrap_ui(parley_plasma_SRCS config.ui)

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
    DESTINATION ${KDE_INSTALL_PLUGINDIR}
    )

install(
    FILES plasma_parley.desktop
    DESTINATION ${KDE_INSTALL_KSERVICES5DIR}
    )

install(
    FILES parley_plasma_card.svg
    DESTINATION ${KDE_INSTALL_DATADIR}/desktoptheme/default/widgets/
    )
