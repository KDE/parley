
install(FILES
   test1.desktop
   test2.desktop
   test3.desktop
   google_dictionary.desktop
   leo-dict.desktop
   DESTINATION ${DATA_INSTALL_DIR}/parley/plugins)

# for now on the same directory but later maybe somewhere else
install(FILES
   myscript1.py
   myscript2.py
   myscript3.py
   google_dictionary.py
   leo-dict.py
   DESTINATION ${DATA_INSTALL_DIR}/parley/plugins)