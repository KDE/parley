#desktop files
install(FILES
   test1.desktop
   google_dictionary.desktop
   leo-dict.desktop
   example.desktop
   google_images.desktop
   DESTINATION ${DATA_INSTALL_DIR}/parley/plugins)

#script files
install(FILES
   myscript1.py
   google_dictionary.py
   leo-dict.py
   example.py
   google_images.py
   DESTINATION ${DATA_INSTALL_DIR}/parley/plugins)

#ui files
install(FILES
   google_images.ui
   DESTINATION ${DATA_INSTALL_DIR}/parley/plugins)
