#desktop files
install(FILES
   google_dictionary.desktop
   leo-dict.desktop
   example.desktop
   google_images.desktop
   DESTINATION ${KDE_INSTALL_DATADIR}/parley/plugins)

#script files
install(FILES
   google_dictionary.py
   leo-dict.py
   example.py
   google_images.py
   DESTINATION ${KDE_INSTALL_DATADIR}/parley/plugins)

#ui files
install(FILES
   google_images.ui
   DESTINATION ${KDE_INSTALL_DATADIR}/parley/plugins)

add_subdirectory( wiktionary )
