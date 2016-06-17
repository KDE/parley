#desktop files
install(FILES
   google_dictionary.desktop
   leo-dict.desktop
   example.desktop
   DESTINATION ${KDE_INSTALL_DATADIR}/parley/plugins)

#script files
install(FILES
   google_dictionary.py
   leo-dict.py
   example.py
   DESTINATION ${KDE_INSTALL_DATADIR}/parley/plugins)

add_subdirectory( wiktionary )
